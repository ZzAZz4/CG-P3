

#ifndef OGL_GLWINDOW_H
#define OGL_GLWINDOW_H

#include <glm/glm.hpp>
#include <memory>
#include <string_view>

using KeyCallback = void (*)(void*, int key, int scancode, int action, int mods);
using MouseButtonCallback = void (*)(void*, int button, int action, int mods);
using MouseMovedCallback = void (*)(void*, double x, double y);
using MouseScrollCallback = void (*)(void*, double x, double y);
using WindowResizeCallback = void (*)(void*, int width, int height);
using WindowRefreshCallback = void (*)(void*);

struct WindowCallbackInfo {
    void* userData = nullptr;
    KeyCallback keyCallback = nullptr;
    MouseButtonCallback mouseButtonCallback = nullptr;
    MouseMovedCallback mouseMoveCallback = nullptr;
    MouseScrollCallback mouseScrollCallback = nullptr;
    WindowResizeCallback resizeCallback = nullptr;
};

struct GLWindow {
    inline static GLWindow* active_window = nullptr;

    WindowCallbackInfo callbackInfo;
    struct GLFWwindow* Handle;
    int Width;
    int Height;
    float AspectRatio;
    glm::vec4 clearColor{0.0f, 0.0f, 0.0f, 1.0f};

    /* creates a window with the given width and height */
    GLWindow(int width, int height, const char* title, const WindowCallbackInfo& callbackInfo);
    ~GLWindow();

    /* asks if the window is still open */
    bool ShouldClose() const;
    /* closes the window */
    void Close();

    /* clears the display */
    void ClearDisplay() const;

    /* swaps the buffers */
    void SwapBuffers() const;

    /* polls for events */
    void PollEvents() const;

    /* Specifies if the mouse cursor is visible */
    void SetMouseLock(bool lock);

    /* Enables Depth Testing */
    void EnableDepthTest() const;

    /* Enables Culling */
    void EnableCulling(int cullFace, int spinDirection) const;

    /* Enables VSync */
    void SetSwapInterval(int interval) const;

    /* Sets the active window */
    static void SetActive(GLWindow* window);

private:
    static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void OnMouseMoved(GLFWwindow* window, double x, double y);
    static void OnMouseScroll(GLFWwindow* window, double x, double y);
    static void OnResized(GLFWwindow* window, int width, int height);
    static void OnError(int errorCode, const char* description);
    static void OnDebug(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                        const char* message, const void*);
};

#endif // OGL_GLWINDOW_H
