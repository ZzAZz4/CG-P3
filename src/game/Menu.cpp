

#include <imgui.h>
#include "Menu.h"
#include "MeshRenderer.h"


Menu::Menu (MeshRenderer& app) : Gui(app.window), app(app) {}

static float animationTime = 2000.f;

void Menu::SetupFrame () {
    ImGui::Begin("Menu");
    ImGui::SliderFloat("Animation", &app.currentAnimationTime, 0.f, app.animationTime);
    ImGui::Spacing();

    ImGui::InputFloat("Animation Time", &animationTime);
    if (ImGui::Button("Set animation time")) {
        app.animationTime = animationTime;
    }
    ImGui::Spacing();
    ImGui::Text("Fill mode (currently %s):", app.activeMesh.renderMode == RenderMode::TRIANGLES ? "TRIANGLES" :
        app.activeMesh.renderMode == RenderMode::TRIANGLE_LINES ? "TRIANGLE_LINES" : "POINTS");

    if (ImGui::Button("Triangles")) {
        app.activeMesh.renderMode = RenderMode::TRIANGLES;
    };
    ImGui::SameLine();
    if (ImGui::Button("Triangle lines")) {
        app.activeMesh.renderMode = RenderMode::TRIANGLE_LINES;
    };
    ImGui::SameLine();
    if (ImGui::Button("Points")) {
        app.activeMesh.renderMode = RenderMode::POINTS;
    };

    ImGui::Spacing();
    if (ImGui::Button("Pause")) {
        app.paused = !app.paused;
    }
    ImGui::End();
}

