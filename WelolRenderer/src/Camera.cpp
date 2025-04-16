#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Welol {

    bool equal(const glm::vec3& lhs, const glm::vec3& rhs, float epsilon)  {
        return fabs(lhs.x - rhs.x) < epsilon && fabs(lhs.y - rhs.y) < epsilon && fabs(lhs.z - rhs.z) < epsilon;
    }
    
    Camera::Camera(glm::vec3 pos, glm::vec3 target)
        : position(pos)
    {
        viewMatrix = lookAt(target);
    }



    glm::mat4 Camera::lookAt(const glm::vec3& target) 
    {

        glm::mat4 basis = glm::mat4(1.0f);
        // Forward direction is from the target to the camera 
        // because the camera is actually static and the scene
        // is the one that has moved.
        glm::vec3 forwardBasis = glm::normalize(position - target);
        glm::vec3 leftBasis = glm::cross(forwardBasis, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 upBasis = glm::cross(forwardBasis, leftBasis);
        
        
        // These represent the rotation matrix, they are later to be inverted
        // to compensate for the fact that it is the scene that rotates and not
        // the camera.
        basis[0] = glm::vec4(leftBasis, 0.0f);
        basis[1] = glm::vec4(upBasis, 0.0f);
        basis[2] = glm::vec4(forwardBasis, 0.0f);
        basis[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
        // Invert the rotation matrix since it is the scene that is 
        // to rotate an not the camera actually. Inverting an orthonormal
        // square matrix can be done by transposing it
        basis = glm::transpose(basis); 
        
        // Translation bit of the construction
        glm::mat4 translation = glm::mat4(1.0f);
        translation[0].w = -position.x;
        translation[1].w = -position.y;
        translation[2].w = -position.z;
        

        // This line is equivalent to the translation done above.
        // GLM will put the translation vector on the bottom most row, but we want it
        // at the right most column, thus the transposition.
        /*  glm::version                 desired version
            |1  0  0  0|                 |1  0  0  tx |                                
            |0  1  0  0|                 |0  1  0  ty |                   
            |0  0  1  0|                 |0  0  0  tz |                   
            |tx ty tz 1|                 |0  0  0  1  |
                                           
        */
        /*
        glm::mat4 translation = glm::transpose(glm::translate(glm::mat4(1.0f), -position));
        */


        translation = glm::transpose(translation);

        basis = basis * translation;

       
        return basis;
    }
    
    void Camera::moveForward(float forwardRate)
    {
        // We negate because the local forward basis for the camera will be
        // facing the opposite direction the view space forward basis is facing.
        glm::vec3 translation = glm::vec3(-glm::transpose(viewMatrix)[2]) * forwardRate;

        position += translation;
        viewMatrix = glm::translate(viewMatrix, -translation);

    }

    void Camera::moveLeft(float rate)
    {
        // We negate because the local left basis for the camera will be
        // facing the opposite direction the view space left basis is facing.
        glm::vec3 translation = glm::vec3(-glm::transpose(viewMatrix)[0]) * rate;

        position += translation;
        viewMatrix = glm::translate(viewMatrix, -translation);

    }

    void Camera::moveUp(float rate)
    {
        // No need of negating the up basis since they face the same direction
        // as the view matrix.
        glm::vec3 translation = glm::vec3(glm::transpose(viewMatrix)[1]) * rate;

        position += translation;
        viewMatrix = glm::translate(viewMatrix, -translation);
    }

    // REVISIT: This is expensive, use the premultiplied version
    // of the rotation matrices as a single matrix. (Will save three matrix computations)
    glm::mat4 Camera::angleToMatrix(float angle)
    {   
        float cosTheta = glm::cos(glm::radians(angle));
        float sinTheta = glm::sin(glm::radians(angle));
        // rotation around the x axis
        glm::mat4 xRotationMatrix = glm::mat4(1.0f);
        xRotationMatrix[0] = glm::vec4(1.0f, 0.0f,     0.0f,      0.0f);
        xRotationMatrix[1] = glm::vec4(0.0f, cosTheta, -sinTheta, 0.0f);
        xRotationMatrix[2] = glm::vec4(1.0f, sinTheta, cosTheta,  0.0f);
        xRotationMatrix[3] = glm::vec4(1.0f, 0.0f,     0.0f,      1.0f);
        // rotation around the y axis
        glm::mat4 yRotationMatrix = glm::mat4(1.0f);
        yRotationMatrix[0] = glm::vec4(cosTheta,  0.0f, sinTheta,  0.0f);
        yRotationMatrix[1] = glm::vec4(0.0f,      1.0f, 0.0f,      0.0f);
        yRotationMatrix[2] = glm::vec4(-sinTheta, 0.0f, cosTheta,  0.0f);
        yRotationMatrix[3] = glm::vec4(0.0f,      0.0f, 0.0f,      1.0f);
        // rotation around the z axis
        glm::mat4 zRotationMatrix = glm::mat4(1.0f);
        zRotationMatrix[0] = glm::vec4(cosTheta,  -sinTheta, 0.0f, 0.0f);
        zRotationMatrix[1] = glm::vec4(sinTheta,   cosTheta, 0.0f, 0.0f);
        zRotationMatrix[2] = glm::vec4(0.0f,       0.0f,     1.0f, 0.0f);
        zRotationMatrix[3] = glm::vec4(0.0f,       0.0f,     0.0f, 1.0f);

        return xRotationMatrix * yRotationMatrix * zRotationMatrix;

    }

    RectangularSystemCamera::RectangularSystemCamera(glm::vec3 pos, glm::vec3 target)
        : Camera(pos, target)
    {   
        
    }


    void RectangularSystemCamera::update(
        float forwardRate, 
        float cameraRotYaw, 
        float cameraRotPitch,
        float mouseX,
        float mouseY,
        bool rotFlag
    )
    {

        /*
        moveForward(forwardRate);
        rollBy(cameraRotYaw * 10.0f);
        yawBy(cameraRotYaw * 10.0f);
        pitchBy(cameraRotPitch * 10.0f);
        */
        static float PI = 3.141592653589793;

        glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
        glm::vec3 pivot(0.0f, 0.0f, 0.0f);

        float delatAngle = PI / 800.0f;
        
        float deltaAngleX = (mouseX - lastMousePos.x) * delatAngle;
        float deltaAngleY = (mouseY - lastMousePos.y) * delatAngle;
        
        // For vertical axis rotation rotate around world vertical axis
        arcRotationMatrix = glm::mat4(1.0f);
        rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        arcRotationMatrix = glm::rotate(arcRotationMatrix, deltaAngleX, rotationAxis);

        // For the horizontal axis rotation rotate around the camera's left axis
        rotationAxis = glm::vec3(-glm::transpose(viewMatrix)[0]);
        arcRotationMatrix = glm::rotate(arcRotationMatrix, -deltaAngleY, rotationAxis);
        
        viewMatrix *= arcRotationMatrix;
        
        lastMousePos.x = mouseX;
        lastMousePos.y = mouseY;
 

    }

    void RectangularSystemCamera::pitchBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void RectangularSystemCamera::yawBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void RectangularSystemCamera::rollBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 0.0f, 1.0f));
    }


    SphericalSystemCamera::SphericalSystemCamera(glm::vec3 pos, glm::vec3 target)
        : Camera(pos, target)    
    {
        
    }

    void SphericalSystemCamera::update(float forwardRate, float cameraRotYaw, float cameraRotPitch, float lastMouseX,
        float lastMouseY, bool rotFlag)
    {
        //basisMatrix = glm::rotate(basisMatrix, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
        // basisMatrix = glm::rotate(basisMatrix, glm::radians(0.05f), glm::vec3(1.0f, 0.0f, 0.0f));
        // rotateAroundWorldAxes(cameraRotYaw, cameraRotPitch);
        rotateAroundWorldAxes(-cameraRotYaw, -cameraRotPitch);
        //std::cout << "aldkjfasdl;kfjsdl;fj" << std::endl;
    }

    void SphericalSystemCamera::rotateAroundWorldAxes(float yaw, float pitch)
    {
        // glm::vec3 oldPosition = position;
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(rot, glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        rot = glm::rotate(rot, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(-pitch), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec4 p = rot * glm::vec4(position, 1.0f);
        position = glm::vec3(p);
        //camTarget = glm::vec3(p);
        viewMatrix = lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        // basisMatrix = glm::translate(basisMatrix, -(position));
        // glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    }

}

/*
    Wiered Bugs:
    1. Initializing a member variable with it's unitialized value. In the constructor.
       The member variable was in a base class.
       Class::Class()
            : memberVar(memberVar)
       {
       }

*/