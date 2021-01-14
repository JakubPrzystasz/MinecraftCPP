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


Camera::Camera()
{
    Position = glm::vec3({ 0,30,0 });
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3({ 0.f,0.f,-1.f });
    FOV = 45.0f;
    Yaw = -90.f;
    Pitch = 0.f;
    MovementSpeed = 7;
    MouseSensitivity = 0.1f;
    Right = glm::vec3(0);
    Up = glm::vec3(0);
    Projection = glm::mat4(0);
    isJumping = false;
    LastPosition = glm::vec3(0, 0, 0);
    JumpStart = 0.f;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetPositionDelta()
{
    return (Position - LastPosition);
}

void Camera::SetScreenRatio(vec2 ScreenResolution)
{
    GLfloat screenRatio = static_cast<GLfloat>(
        static_cast<GLfloat>(ScreenResolution.y)/ 
        static_cast<GLfloat>(ScreenResolution.x));
    Projection = glm::perspective(FOV, screenRatio, 0.001f, 100.0f);
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

void Camera::Ray::Step(GLfloat Scale)
{
    auto& p = RayEnd;

    p.x -= glm::cos(Yaw) * Scale;
    p.z -= glm::sin(Yaw) * Scale;
    p.y -= glm::tan(Pitch) * Scale;
}

const glm::vec3& Camera::Ray::GetEnd() const
{
    return RayEnd;
}

float Camera::Ray::GetLength() const
{
    return glm::distance(RayStart,RayEnd);
}
