#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"

enum class CameraMovement {
    FORWARD = 0,
    BACKWARD = 1,
    RIGHT = 2,
    LEFT = 3
};

class Camera
{
private:
    void updateCameraVectors();
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    GLfloat FOV;
    GLfloat Yaw;
    GLfloat Pitch;
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    glm::mat4 Projection;

    Camera(GLfloat screenRatio);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime);

    void ProcessMouseMovement(MousePosition mousePos, GLboolean constrainPitch = true);

};

