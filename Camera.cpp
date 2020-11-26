#include "Camera.h"

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(GLfloat screenRatio)
{
    Position = glm::vec3({0.f,0.f,0.f});
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3({0.f,0.f,-1.f});
    FOV = 45.0f;
    Yaw = -90.f;
    Pitch = 0.f;
    MovementSpeed = 0.0001f;
    MouseSensitivity = 0.1f;
    Right = glm::vec3();
    Up = glm::vec3();
    //SETUP projection matrix
    Projection = glm::perspective(FOV, screenRatio, 0.001f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = MovementSpeed * deltaTime;
    switch (direction) {
    case CameraMovement::FORWARD:
        Position += Front * velocity;
        break;
    case CameraMovement::BACKWARD:
        Position -= Front * velocity;
        break;
    case CameraMovement::LEFT:
        Position -= Right * velocity;
        break;
    case CameraMovement::RIGHT:
        Position += Right * velocity;
        break;
    };
}

void Camera::ProcessMouseMovement(MousePosition mousePos, GLboolean constrainPitch)
{
    mousePos.x *= MouseSensitivity;
    mousePos.y *= MouseSensitivity;

    Yaw += (GLfloat)mousePos.x;
    Pitch += (GLfloat)mousePos.y;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
   
    updateCameraVectors();
}
