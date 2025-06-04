#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include "WelolMath.h"

namespace Welol {

    class Camera
    {
    public:
        Camera(glm::vec3 pos, glm::vec3 target);
        glm::mat4& getViewMatrix() {return viewMatrix; };
        WelolMath::Mat4x4& wLGetViewMatrix() {return wLViewMatrix;};
        void update(
            float forwardRate,
            bool rotateCamera,
            float mouseX,
            float mouseY,
            bool forwardCamera
        );

        void setLastMousePos(float lastX, float lastY)
        {
            lastMousePos.x = lastX;
            lastMousePos.y = lastY;
        }

        void setTarget(glm::vec3& target) {focusTarget = target;}

        glm::vec3& getPosition() {return position; };
        
        void moveForward(float forwardRate);
    private:
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 focusTarget{0.0f, 0.0f, 0.0f};
        glm::mat4 viewMatrix{glm::mat4(1.0f)};
        glm::vec2 lastMousePos{2.0f,5.0f};


        bool set = false;
        WelolMath::Mat4x4 wLViewMatrix{1.0f};
        WelolMath::Vec3 wLposition{0.0f, 0.0f, 0.0f};
        WelolMath::Vec3 wLFocusTarget{0.0f, 0.0f, 0.0f};
        WelolMath::Vec3 leftAxis{0.0f, 0.0f, 0.0f};
        float distanceFromTarget{0.0f};

        float scrollRate{1.5f};

        void lookAt(WelolMath::Vec3& pos, WelolMath::Vec3& target);
        void moveLeft(float rate);
        void moveUp(float rate);

        void pitchBy(float angle);
        void yawBy(float angle);
        void rollBy(float angle);

        glm::mat4 angleToMatrix(float angle);
    };

}