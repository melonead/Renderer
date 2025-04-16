#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace Welol {

    class Camera
    {
    public:
        Camera(glm::vec3 pos, glm::vec3 target);
        virtual glm::mat4& getViewMatrix() = 0;
        virtual void update(
            float forwardRate, 
            float cameraRotYaw, 
            float cameraRotPitch, 
            float mouseX,
            float mouseY,
            bool rotFlag
        ) = 0;

        void setLastMousePos(float lastX, float lastY)
        {
            lastMousePos.x = lastX;
            lastMousePos.y = lastY;
        }

    protected:
        glm::vec3 position;
        glm::vec3 camTarget = glm::vec3(5.0f,5.0f, 5.f);
        glm::mat4 viewMatrix{glm::mat4(1.0f)};
        glm::vec2 lastMousePos{2.0f,5.0f};

        glm::mat4 lookAt(const glm::vec3& target);
        void moveForward(float forwardRate);
        void moveLeft(float rate);
        void moveUp(float rate);

        glm::mat4 angleToMatrix(float angle);

        // arc ball stuff
        float radius {60.0f};
        glm::mat4x4 arcRotationMatrix{1.0f};
        glm::vec3 getUnitVector(float x, float y);
        glm::vec3 getVector(float x, float y);
        glm::vec3 getVectorWithProject(float x, float y);
        float angleBetween(glm::vec3& vecA, glm::vec3& vecB);
    };
    
    class SphericalSystemCamera : public Camera
    {
    public:
        SphericalSystemCamera(glm::vec3 pos, glm::vec3 target); 
        glm::mat4& getViewMatrix() {return viewMatrix; };   
        void update(
            float forwardRate, 
            float cameraRotYaw, 
            float cameraRotPitch, 
            float mouseX,
            float mouseY,
            bool rotFlag
        );
    private:
        void rotateAroundWorldAxes(float yaw, float pitch);
        
    };

    class RectangularSystemCamera : public Camera
    {
    public:
        RectangularSystemCamera(glm::vec3 pos, glm::vec3 target); 
        glm::mat4& getViewMatrix(){ return viewMatrix; };   
        void update(
            float forwardRate, 
            float cameraRotYaw, 
            float cameraRotPitch, 
            float mouseX,
            float mouseY,
            bool rotFlag
        );
    private:
        void yawBy(float angleDelta);
        void pitchBy(float angleDelta);
        void rollBy(float angleDelta);
        
    };

}