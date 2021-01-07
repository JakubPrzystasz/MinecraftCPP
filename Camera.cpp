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
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(GLfloat screenRatio)
{
    Position = glm::vec3({0,25,0});
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3({0.f,0.f,-1.f});
    FOV = 45.0f;
    Yaw = -90.f;
    Pitch = 0.f;
    MovementSpeed = 10;
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
    glm::vec3 PositionDelta = glm::vec3(0);
    switch (direction) {
    case CameraMovement::FORWARD:
        PositionDelta += Front * velocity;
        PositionDelta.y = 0;
        break;
    case CameraMovement::BACKWARD:
        PositionDelta -= Front * velocity;
        PositionDelta.y = 0;
        break;
    case CameraMovement::LEFT:
        PositionDelta -= Right * velocity;
        PositionDelta.y = 0;
        break;
    case CameraMovement::RIGHT:
        PositionDelta += Right * velocity;
        PositionDelta.y = 0;
        break;
    case CameraMovement::UP:
        PositionDelta += glm::vec3(0, 1 *velocity,0);
        break;
    case CameraMovement::DOWN:
        PositionDelta -= glm::vec3(0, 1 * velocity, 0);
        break;
    };
    Position += PositionDelta;
}

void Camera::ProcessMouseMovement(MousePosition mousePos, GLboolean constrainPitch)
{
    mousePos.x *= MouseSensitivity;
    mousePos.y *= MouseSensitivity;

    Yaw += (GLfloat)mousePos.x;
    if (Yaw > 360)
        Yaw = 0;
    if (Yaw < -360)
        Yaw = 0;
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

WorldDirection Camera::GetLookDirection()
{
    if ((Yaw > 0 && Yaw <= 45) || (Yaw > 315 && Yaw <= 0))
        return WorldDirection::North;

    if (Yaw > 45 && Yaw <= 135)
        return WorldDirection::East;

    if (Yaw > 135 && Yaw <= 225)
        return WorldDirection::South;

    if (Yaw > 225 && Yaw <= 315)
        return WorldDirection::West;

    if (Yaw > -45 || Yaw <= -315)
        return WorldDirection::North;

    if (Yaw > -135 && Yaw <= -45)
        return WorldDirection::West;

    if (Yaw > -225 && Yaw <= -135)
        return WorldDirection::South;

    if (Yaw > -315 && Yaw <= -225)
        return WorldDirection::East;

    return WorldDirection::Unknown;

}
