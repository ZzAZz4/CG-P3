#include "Player.h"
#include <app/Time.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


static bool firstMouse = true;
static float lastX = nanf("");
static float lastY = nanf("");


Player::Player ()
    : camera(glm::vec3(0.f), 90.f, 0.f) {}


void Player::Update () {
    float velocity = (this->actionFlags.run ? this->runSpeed : this->speed) * Time::deltaTime;

    glm::vec3 flatFront = glm::normalize(glm::vec3(this->camera.Front.x, 0, this->camera.Front.z));

    if (this->actionFlags.forward) this->camera.Position += flatFront * velocity;
    if (this->actionFlags.backward) this->camera.Position -= flatFront * velocity;
    if (this->actionFlags.right) this->camera.Position += this->camera.Right * velocity;
    if (this->actionFlags.left) this->camera.Position -= this->camera.Right * velocity;
    if (this->actionFlags.up) this->camera.Position += glm::vec3(0, velocity, 0);
    if (this->actionFlags.down) this->camera.Position -= glm::vec3(0, velocity, 0);
    this->camera.Update();
}

void Player::OnMouseMove (double xposd, double yposd) {
    auto xpos = static_cast<float>(xposd);
    auto ypos = static_cast<float>(yposd);
    if (firstMouse || reclip) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        reclip = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    this->camera.Pitch = glm::clamp(this->camera.Pitch + yoffset * this->mouseSensitivity, -89.0f, 89.0f);
    this->camera.Yaw += xoffset * this->mouseSensitivity;
}

void Player::OnResize (int width, int height) {
    this->camera.AspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Player::OnKeyPressed (int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) {
        return;
    }
    if (key == GLFW_KEY_W) {
        this->actionFlags.forward = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_S) {
        this->actionFlags.backward = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_A) {
        this->actionFlags.left = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_D) {
        this->actionFlags.right = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_SPACE) {
        this->actionFlags.up = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
        this->actionFlags.down = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
        this->actionFlags.run = action == GLFW_PRESS;
    }
}

void Player::OnMouseScroll (double xoffset, double yoffset) {
    this->camera.Zoom = glm::clamp(this->camera.Zoom - (float) yoffset, 1.0f, 45.0f);
}

void Player::OnMouseClick (int button, int action, int mods) {

}

void Player::Start () {
}

void Player::Render () {
}
