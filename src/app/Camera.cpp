#include "Camera.h"

Camera::Camera(const glm::vec3& position, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM) {
    Position = position;
    Yaw = yaw;
    Pitch = pitch;
}

void Camera::Update() {
    RecalculateProjection();
    RecalculateView();
}

void Camera::RecalculateProjection() {
    Projection = glm::perspective(glm::radians(Zoom), this->AspectRatio, 0.1f, 100.0f);
}

void Camera::RecalculateView() {
    glm::vec3 front;
    front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    front.y = glm::sin(glm::radians(Pitch));
    front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    View = glm::lookAt(Position, Position + Front, glm::vec3(0.0f, 1.0f, 0.0f));
    Right = glm::normalize(glm::vec3(View[0][0], View[1][0], View[2][0]));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::SetActiveCamera (Camera* camera) {
    active = camera;
}
