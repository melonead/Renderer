#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "WelolMath.h"

namespace Welol {
    
    Camera::Camera(glm::vec3 pos, glm::vec3 target)
        : position(pos),
          focusTarget(target)
    {
        // wLposition = WelolMath::Vec3(pos.x, pos.y, pos.z);
        // WelolMath::Vec3 p{pos.x, pos.y, pos.z};
        // WelolMath::Vec3 t{target.x, target.y, target.z};
        // lookAt(p, t);

        viewMatrix = glm::lookAt(pos, focusTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    }



    void Camera::lookAt(WelolMath::Vec3& eye, WelolMath::Vec3& target) 
    {

        WelolMath::Mat4x4 rotation(1.0f);
        WelolMath::Mat4x4 translation(1.0f);
        WelolMath::Vec3 temporaryUp{0.0f, 1.0f, 0.0f};

        WelolMath::Vec3 forwardBasis = WelolMath::normalize(eye - target);
        
        float eps = 0.00001f;

        if(fabs(forwardBasis.x) < eps && fabs(forwardBasis.z) < eps)
        {
            // forward vector is pointing +Y axis
            if(forwardBasis.y > 0)
            {   
                std::cout << "facing up" << std::endl;
                temporaryUp = WelolMath::Vec3(0.0f, 0.0f, -1.0f);
            }
            // forward vector is pointing -Y axis
            else
            {
                std::cout << "facing down" << std::endl;
                temporaryUp = WelolMath::Vec3(0.0f, 0.0f, 1.0f);
            }
        }
        // in general, up vector is straight up
        else
        {
            temporaryUp = WelolMath::Vec3(0.0f, 1.0f, 0.0f);
        }



        // Order of the dot product parameters matter, reverse order will give a flipped scene
        WelolMath::Vec3 leftBasis = WelolMath::cross(temporaryUp, forwardBasis);
        WelolMath::Vec3 upBasis = WelolMath::cross(forwardBasis, leftBasis);

        
        // rotation[0] = glm::vec4(leftBasis.x, leftBasis.y, leftBasis.z, 0.0f);
        // rotation[1] = glm::vec4(upBasis.x, upBasis.y, upBasis.z, 0.0f);
        // rotation[2] = glm::vec4(forwardBasis.x, forwardBasis.y, forwardBasis.z, 0.0f);

        rotation[0][0] = leftBasis.x; 
        rotation[1][0] = leftBasis.y; 
        rotation[2][0] = leftBasis.z; 
        rotation[3][0] = -WelolMath::dot(leftBasis, eye); 

        rotation[0][1] = upBasis.x; 
        rotation[1][1] = upBasis.y; 
        rotation[2][1] = upBasis.z; 
        rotation[3][1] = -WelolMath::dot(upBasis, eye); 

        rotation[0][2] =  forwardBasis.x;
        rotation[1][2] =  forwardBasis.y;
        rotation[2][2] =  forwardBasis.z;
        rotation[3][2] = -WelolMath::dot(forwardBasis, eye);

        if (set == false)
        {
            set = true;
            leftAxis = leftBasis;
        }

    
        wLViewMatrix = rotation;    
    }
    
    void Camera::moveForward(float direction)
    {
        // We negate because the local forward basis for the camera will be
        // facing the opposite direction the view space forward basis is facing.
        glm::vec3 translation = glm::vec3(-glm::transpose(viewMatrix)[2]) * scrollRate * direction;

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


    void Camera::update(
        float forwardRate, 
        float cameraRotYaw, 
        float cameraRotPitch,
        float mouseX,
        float mouseY,
        bool forwardCamera
    )
    {

        /*
        moveForward(forwardRate);
        rollBy(cameraRotYaw * 10.0f);
        yawBy(cameraRotYaw * 10.0f);
        pitchBy(cameraRotPitch * 10.0f);
        */

        if (forwardCamera)
            moveForward(0.05f);
        static float PI = 3.141592653589793;
        
        glm::vec3 pivot(0.0f, 0.0f, 0.0f);
        glm::mat4 arcRotationMatrix = glm::mat4(1.0f);
        glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);

        float deltaAngle = PI / 800.0f;
        
        float deltaAngleX = (mouseX - lastMousePos.x) * deltaAngle;
        float deltaAngleY = (mouseY - lastMousePos.y) * deltaAngle;



        glm::mat4x4 rotMat(1.0f);
        glm::vec3 rotAxis = -glm::transpose(viewMatrix)[0];
        rotMat = glm::rotate(rotMat, deltaAngleY, rotAxis);

        rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        rotMat = glm::rotate(rotMat, -deltaAngleX, rotAxis);
        position = glm::vec3(rotMat * glm::vec4((position - focusTarget), 1.0f)) + focusTarget;


        glm::vec3 cameraForward = glm::vec3(glm::transpose(viewMatrix)[2]);
        glm::vec3 cameraUp = glm::vec3(glm::transpose(viewMatrix)[1]);
        glm::vec3 tempUp = glm::vec3(0.0f, 1.0f, 0.0f);

        /*float eps = 0.1f;

        if(fabs(cameraForward.x) < eps && fabs(cameraForward.z) < eps)
        {
            if (cameraUp.y > 0.0f)
            {
                std::cout << "facing down" << std::endl;
                tempUp = glm::vec3(0.0f, 0.0f, 1.0f);
            }
            else
            { 
                std::cout << "facing up" << std::endl;
                tempUp = glm::vec3(0.0f, 0.0f, -1.0f);
            }
        } 
        else
        {
            tempUp = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        */
        viewMatrix = glm::lookAt(position, focusTarget, tempUp);


        /*
        std::cout << "radius: " << WelolMath::vectorLength(wLposition - wLFocusTarget) << std::endl;


        WelolMath::Vec3 rotAxis = WelolMath::Vec3(0.0f, 1.0f, 0.0f);//WelolMath::transpose(wLViewMatrix).getColumn3(0);
        WelolMath::Vec3 toRotate = wLposition - wLFocusTarget;
        WelolMath::Vec3 rotated = WelolMath::rotateAroundAxis(toRotate, rotAxis, -deltaAngleX);
        wLposition = rotated + wLFocusTarget;

        std::cout << " lx: " << leftAxis.x << " ly: " << leftAxis.y << " lz: " << leftAxis.z << std::endl;

        rotAxis = WelolMath::transpose(wLViewMatrix).getColumn3(0);
        toRotate = wLposition - wLFocusTarget;
        rotated = WelolMath::rotateAroundAxis(toRotate, rotAxis, deltaAngleY);

        wLposition = rotated + wLFocusTarget;

        rotAxis = WelolMath::Vec3(0.0f, 1.0f, 0.0f);


        lookAt(wLposition, wLFocusTarget);
        */
        
        //std::cout << " x: " << position.x << " y: " << position.y << " z: " << position.z << std::endl;
        //std::cout << deltaAngleX << std::endl;
        
        lastMousePos.x = mouseX;
        lastMousePos.y = mouseY;

        
 

    }

    void Camera::pitchBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void Camera::yawBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Camera::rollBy(float angleDelta)
    {
        viewMatrix = glm::rotate(viewMatrix, glm::radians(angleDelta), glm::vec3(0.0f, 0.0f, 1.0f));
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