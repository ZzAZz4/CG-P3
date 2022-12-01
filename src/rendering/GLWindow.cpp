#include "GLWindow.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

GLWindow::GLWindow(int width, int height, const char* title, const WindowCallbackInfo& callbackInfo)
    : callbackInfo(callbackInfo), Width(width), Height(height),
      AspectRatio((float)width / (float)height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    this->Handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!Handle) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(Handle);
    glfwSetWindowUserPointer(Handle, this);
    if (this->callbackInfo.userData == nullptr) {
        this->callbackInfo.userData = this;
    }

    glfwSetErrorCallback(GLWindow::OnError);
    glfwSetKeyCallback(Handle, GLWindow::OnKeyEvent);
    glfwSetMouseButtonCallback(Handle, GLWindow::OnMouseButton);
    glfwSetScrollCallback(Handle, GLWindow::OnMouseScroll);
    glfwSetCursorPosCallback(Handle, GLWindow::OnMouseMoved);
    glfwSetWindowSizeCallback(Handle, GLWindow::OnResized);
    //    glfwSetWindowRefreshCallback(handle, GLWindow::OnRefresh);

    //    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLWindow::OnDebug, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
}

GLWindow::~GLWindow() {
    // TODO: multiple windows?
    glfwTerminate();
}

bool GLWindow::ShouldClose() const { return glfwWindowShouldClose(Handle); }

void GLWindow::Close() { glfwSetWindowShouldClose(Handle, true); }

void GLWindow::ClearDisplay() const {
    // idk if clearColor is necessary but ok
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::SwapBuffers() const { glfwSwapBuffers(Handle); }

void GLWindow::PollEvents() const { glfwPollEvents(); }

void GLWindow::OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* glWindow = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (glWindow->callbackInfo.keyCallback) {
        glWindow->callbackInfo.keyCallback(glWindow->callbackInfo.userData, key, scancode, action,
                                           mods);
    }
}

void GLWindow::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
    auto* glWindow = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (glWindow->callbackInfo.mouseButtonCallback) {
        glWindow->callbackInfo.mouseButtonCallback(glWindow->callbackInfo.userData, button, action,
                                                   mods);
    }
}

void GLWindow::OnMouseMoved(GLFWwindow* window, double x, double y) {
    auto* glWindow = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (glWindow->callbackInfo.mouseMoveCallback) {
        glWindow->callbackInfo.mouseMoveCallback(glWindow->callbackInfo.userData, x, y);
    }
}

void GLWindow::OnMouseScroll(GLFWwindow* window, double x, double y) {
    auto* glWindow = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (glWindow->callbackInfo.mouseScrollCallback) {
        glWindow->callbackInfo.mouseScrollCallback(glWindow->callbackInfo.userData, x, y);
    }
}

void GLWindow::OnResized(GLFWwindow* window, int width, int height) {
    auto* glWindow = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    glWindow->Width = width;
    glWindow->Height = height;
    glWindow->AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glViewport(0, 0, width, height);
    if (glWindow->callbackInfo.resizeCallback) {
        glWindow->callbackInfo.resizeCallback(glWindow->callbackInfo.userData, width, height);
    }
}

void GLWindow::OnError(int errorCode, const char* description) {
    fprintf(stderr, "GLFW Error (%d): %s", errorCode, description);
    throw std::runtime_error("GLFW Error");
}

void GLWindow::OnDebug(unsigned int source, unsigned int type, unsigned int id,
                       unsigned int severity, int length, const char* message, const void*) {
    if (id == 131185 || id == 131218 || id == 131169 || id == 131076 || id == 131204) {
        return;
    }
    fprintf(stderr,
            "GL CALLBACK: %s\n"
            "source = 0x%x, type = 0x%x, severity = 0x%x\n%"
            "s\n",
            type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "", source, type, severity, message);
}

void GLWindow::SetMouseLock(bool lock) {
    int mode = lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(Handle, GLFW_CURSOR, mode);
}

void GLWindow::EnableDepthTest() const { glEnable(GL_DEPTH_TEST); }

void GLWindow::EnableCulling(int cullFace, int spinDirection) const {
    glEnable(GL_CULL_FACE);
    glCullFace(cullFace);
    glFrontFace(spinDirection);
}

void GLWindow::SetActive(GLWindow* window) {
    active_window = window;
    glfwMakeContextCurrent(window->Handle);
}

void GLWindow::SetSwapInterval (int interval) const {
    glfwSwapInterval(interval);
}
