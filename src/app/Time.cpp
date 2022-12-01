#include "Time.h"
#include <GLFW/glfw3.h>

void Time::Init() { time = (float)glfwGetTime(); }

void Time::Update() {
    auto currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - time;
    time = currentFrame;
}

float Time::GetFPS() { return 1.0f / deltaTime; }
