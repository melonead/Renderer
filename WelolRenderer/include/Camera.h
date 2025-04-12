#pragma once
#include <glm/glm.hpp>

namespace Welol {

    class Camera
    {
    public:
        Camera(glm::vec3 pos, glm::vec3 target);
        glm::vec3 position;
        glm::mat4 basisMatrix{glm::mat4(1.0f)};
        glm::mat4& getViewMatrix(){ return basisMatrix; };
        void update();
        void forwardBy(float forwardRate);
    private:
        void yawBy(float angleDelta);
        void pitchBy(float angleDelta);
        void rollBy(float angleDelta);
        glm::mat4 computeBasisInitCam(const glm::vec3& target);
        
    };
}