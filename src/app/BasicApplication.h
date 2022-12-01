

#ifndef HOMEWORK_BASICAPPLICATION_H
#define HOMEWORK_BASICAPPLICATION_H

#include "../rendering/GLWindow.h"
#include "Time.h"

class BasicApplication {

public:
    BasicApplication(int width, int height, std::string_view name);
    virtual ~BasicApplication() = default;

    void Run();

    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() {};

    virtual void OnKeyPressed(int key, int scancode, int action, int mods);
    virtual void OnMouseClicked(int button, int action, int mods);
    virtual void OnMouseMove(double xposd, double yposd);
    virtual void OnMouseScroll(double xoffset, double yoffset);
    virtual void OnResize(int width, int height);

    GLWindow window;
private:
    static void OnKeyPressedCallback(void* self, int key, int scancode, int action, int mods);
    static void OnMouseClickedCallback(void* self, int button, int action, int mods);
    static void OnMouseMoveCallback(void* self, double xposd, double yposd);
    static void OnMouseScrollCallback(void* self, double xoffset, double yoffset);
    static void OnResizeCallback(void* self, int width, int height);
};


#endif //HOMEWORK_BASICAPPLICATION_H
