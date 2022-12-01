#include "BasicApplication.h"

BasicApplication::BasicApplication (int width, int height, std::string_view name)
    : window(
    width, height, name.data(), WindowCallbackInfo{
        .userData = this,
        .keyCallback = OnKeyPressedCallback,
        .mouseButtonCallback = OnMouseClickedCallback,
        .mouseMoveCallback = OnMouseMoveCallback,
        .mouseScrollCallback = OnMouseScrollCallback,
        .resizeCallback = OnResizeCallback,
    }) {
    GLWindow::SetActive(&window);
    Time::Init();
}

void BasicApplication::Run () {
    Start();

    while (!window.ShouldClose()) {
        window.PollEvents();
        Time::Update();
        Update();

        window.ClearDisplay();
        Render();

        window.SwapBuffers();
    }

}


void BasicApplication::OnKeyPressedCallback(void* self, int key, int scancode, int action, int mods) {
    static_cast<BasicApplication*>(self)->OnKeyPressed(key, scancode, action, mods);
}

void BasicApplication::OnMouseClickedCallback(void* self, int button, int action, int mods) {
    static_cast<BasicApplication*>(self)->OnMouseClicked(button, action, mods);
}

void BasicApplication::OnMouseMoveCallback(void* self, double xposd, double yposd) {
    static_cast<BasicApplication*>(self)->OnMouseMove(xposd, yposd);
}

void BasicApplication::OnMouseScrollCallback(void* self, double xoffset, double yoffset) {
    static_cast<BasicApplication*>(self)->OnMouseScroll(xoffset, yoffset);
}

void BasicApplication::OnResizeCallback(void* self, int width, int height) {
    static_cast<BasicApplication*>(self)->OnResize(width, height);
}
void BasicApplication::OnKeyPressed (int, int, int, int) {}
void BasicApplication::OnMouseClicked (int, int, int ) {}
void BasicApplication::OnMouseMove (double, double) {}
void BasicApplication::OnMouseScroll (double, double) {}
void BasicApplication::OnResize (int, int) {}
