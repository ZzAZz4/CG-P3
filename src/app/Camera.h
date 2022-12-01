#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles,
// Vectors and Matrices for use in OpenGL
class Camera {
public:
    glm::mat4 Projection{};
    glm::mat4 View{};

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    float Yaw;
    float Pitch;
    float Zoom;
    float AspectRatio = nanf("");

    // constructor with vectors
    Camera(const glm::vec3& position, float yaw, float pitch);
    void Update();

    inline static Camera* active = nullptr;
    static void SetActiveCamera(Camera* camera);

private:
    void RecalculateProjection();
    void RecalculateView();
};

#endif