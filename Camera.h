#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Input.h"
#include "Vectors.h"

enum class WorldDirection {
    North = 'N',
    South = 'S',
    West =  'W',
    East = 'E',
    Unknown = 'U'
};

enum class CameraMovement {
    FORWARD = 0,
    BACKWARD = 1,
    RIGHT = 2,
    LEFT = 3,
    UP = 4,
    DOWN = 5
};

class Camera
{
private:
    void updateCameraVectors();
public:
    class Ray {
    private:
        glm::vec3 RayStart;
        glm::vec3 RayEnd;
        GLfloat Yaw;
        GLfloat Pitch;
    public:
        Ray(Camera camera) {
            RayStart = camera.Position;
            RayEnd = camera.Position;
            Yaw = camera.Yaw;
            Pitch = camera.Pitch;
        };
        void Step(GLfloat Scale);
        const glm::vec3& GetEnd() const;
        float GetLength() const;
    };

    Camera();
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
    glm::vec3 LastPosition;
    bool isJumping;
    GLfloat JumpStart;

    glm::mat4 GetViewMatrix();

    glm::vec3 GetPositionDelta();

    void SetScreenRatio(vec2 ScreenResolution);

    void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime);

    void ProcessMouseMovement(MousePosition mousePos, GLboolean constrainPitch = true);

    WorldDirection GetLookDirection();
};

