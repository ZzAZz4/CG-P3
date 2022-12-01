#include <GLFW/glfw3.h>
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer ()
    : BasicApplication(1280, 720, "Mesh Renderer"), menu(*this) {
    this->basicShader.Construct(
        Shader::FromFile(GL_VERTEX_SHADER, "../res/shaders/basic.vert.glsl"),
        Shader::FromFile(GL_FRAGMENT_SHADER, "../res/shaders/basic.frag.glsl")
    );
    this->phongShader.Construct(
        Shader::FromFile(GL_VERTEX_SHADER, "../res/shaders/phong.vert.glsl"),
        Shader::FromFile(GL_FRAGMENT_SHADER, "../res/shaders/phong.frag.glsl")
    );
    this->window.EnableDepthTest();
}

void MeshRenderer::Start () {
    this->player.Start();
    this->player.OnResize(this->window.Width, this->window.Height);
    this->LoadMesh("../res/animation/model-0000.obj", RenderMode::POINTS);
    this->window.SetMouseLock(!this->showMenu);
}

void MeshRenderer::Update () {
    this->player.Update();
    this->activeMesh.Update();

    if (!paused) {
        this->currentAnimationTime += Time::time;
        this->currentAnimationTime = glm::min(this->currentAnimationTime, this->animationTime);

        const auto mesh = int(100.f * this->currentAnimationTime / this->animationTime);
        if (mesh != this->currentMesh) {
            this->currentMesh = mesh;
            char name[] = "../res/animation/model-0000.obj";
            sprintf(name, "../res/animation/model-%03d0.obj", this->currentMesh);
            this->LoadMesh(name, this->activeMesh.renderMode);
        }
    }
}

void MeshRenderer::Render () {
    Camera::SetActiveCamera(&this->player.camera);
    if (this->activeMesh.renderMode == RenderMode::TRIANGLES) {
        phongShader.Bind();
        phongShader.setMat4("view", this->player.camera.View);
        phongShader.setMat4("projection", this->player.camera.Projection);
        phongShader.setVec3("directionalLightColor", glm::vec3(1.f, 1.f, 1.f));
        phongShader.setVec3("directionalLightDirection", glm::vec3(0.0f, 1.0f, 0.0f));
        phongShader.setVec3("directionalLightIntensity", glm::vec3(0.6f));
        phongShader.setVec3("ambientLight", glm::vec3(0.3f));
        phongShader.setVec3("viewPosition", this->player.camera.Position);
        this->activeMesh.Render(phongShader);

        basicShader.Bind();
        basicShader.setMat4("view", this->player.camera.View);
        basicShader.setMat4("projection", this->player.camera.Projection);
        this->activeMesh.renderMode = RenderMode::TRIANGLE_LINES;
        this->activeMesh.Render(basicShader);
        this->activeMesh.renderMode = RenderMode::TRIANGLES;
    }
    else {
        basicShader.Bind();
        basicShader.setMat4("view", this->player.camera.View);
        basicShader.setMat4("projection", this->player.camera.Projection);
        this->activeMesh.Render(basicShader);
    }
    if (this->showMenu) {
        this->menu.Render();
    }
}

void MeshRenderer::LoadMesh (std::string_view inputfile, RenderMode renderMode) {
    this->activeMesh = load_simple_obj(inputfile);
    this->activeMesh.Start();
    this->activeMesh.renderMode = renderMode;
}


void MeshRenderer::OnKeyPressed (int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        this->showMenu = !this->showMenu;
        this->window.SetMouseLock(!this->showMenu);
    }
    if (!this->showMenu) {
        this->player.OnKeyPressed(key, scancode, action, mods);
    } else {
        this->player.reclip = true;
    }
}

void MeshRenderer::OnMouseClicked (int , int , int ) {

}

void MeshRenderer::OnMouseMove (double xposd, double yposd) {
    if (!this->showMenu) {
        player.OnMouseMove(xposd, yposd);
    }
}
void MeshRenderer::OnMouseScroll (double xoffset, double yoffset) {
    player.OnMouseScroll(xoffset, yoffset);
}
void MeshRenderer::OnResize (int width, int height) {
    player.OnResize(width, height);
}

void MeshRenderer::GenerateMeshes (std::string_view datadir, std::string_view outdir) {
    char buff[1024];
    sprintf(buff, "cd ../venv/Scripts && "
                  "activate && "
                  "cd ../../cmake-build-debug && "
                  "python ../src/py/main.py %s %s", datadir.data(), outdir.data());
    system(buff);
}
