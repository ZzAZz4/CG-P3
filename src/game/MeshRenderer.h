

#ifndef HOMEWORK_MESHRENDERER_H
#define HOMEWORK_MESHRENDERER_H

#include <app/BasicApplication.h>
#include <rendering/ShaderProgram.h>
#include "Mesh.h"
#include "Player.h"
#include "Menu.h"

class MeshRenderer : public BasicApplication {
public:
    MeshRenderer();
    ~MeshRenderer() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    static void GenerateMeshes(std::string_view datadir, std::string_view outdir);
    void LoadMesh(std::string_view inputfile, RenderMode mode);

    void OnKeyPressed(int key, int scancode, int action, int mods) override;
    void OnMouseClicked(int button, int action, int mods) override;
    void OnMouseMove(double xposd, double yposd) override;
    void OnMouseScroll(double xoffset, double yoffset) override;
    void OnResize(int width, int height) override;

private:
    SimpleMesh activeMesh;
    Player player;
    Menu menu;
    ShaderProgram basicShader;
    ShaderProgram phongShader;
    float animationTime = 2000.f;
    float currentAnimationTime = 0.f;
    int currentMesh = 0;
    bool showMenu = false;
    bool paused = false;

    friend class Menu;
};


#endif //HOMEWORK_MESHRENDERER_H
