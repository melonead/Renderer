#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Welol {

    Camera::Camera(glm::vec3 pos, glm::vec3 target)
        : position{pos}
    {   
        basisMatrix = computeBasisInitCam(target);
    }

    void Camera::update(float forwardRate, float cameraRotYaw, float cameraRotPitch)
    {

        //forwardBy(forwardRate);
        yawBy(cameraRotYaw);
        //pitchBy(cameraRotPitch);
        //rollBy(cameraRotYaw);
    }

    void Camera::pitchBy(float angleDelta)
    {
        basisMatrix = glm::rotate(basisMatrix, glm::radians(angleDelta), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void Camera::yawBy(float angleDelta)
    {
        basisMatrix = glm::rotate(basisMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Camera::rollBy(float angleDelta)
    {
        basisMatrix = glm::rotate(basisMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Camera::forwardBy(float forwardRate)
    {
        glm::mat4 basisMatrixTranspose = glm::transpose(basisMatrix);

        // REVISIT: I'm hoping that the third column is the forward
        // and that index 3 is the the third column.
        glm::vec3 translation = glm::vec3(basisMatrixTranspose[2]) * forwardRate;

        position += translation;
        basisMatrix = glm::translate(basisMatrix, -translation);
    }

    glm::mat4 Camera::computeBasisInitCam(const glm::vec3& target) 
    {
        glm::mat4 basis = glm::mat4(1.0f);
        // Forward direction is from the target to the camera 
        // because the camera is actually static and the scene
        // is the one that has moved.
        glm::vec3 forwardBasis = glm::normalize(position - target);
        glm::vec3 leftBasis = glm::cross(forwardBasis, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 upBasis = glm::cross(forwardBasis, leftBasis);
        
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);
        // REVISIT: Does each vec4 represent a row or a coloumn, I'm hoping it 
        // represents a column.
        basis[0] = glm::vec4(leftBasis, 0.0f);
        basis[1] = glm::vec4(upBasis, 0.0f);
        basis[2] = glm::vec4(forwardBasis, 0.0f);
        basis[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        basis *= translationMatrix;
        return basis;
    }
}