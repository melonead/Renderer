#pragma once
#include "objectFactory.h"

class Entity
{
public:
    Entity(glm::vec3 position);
    ~Entity();
    void update(Welol::Camera& camera, glm::mat4& projectionMatrix, Welol::Renderer& renderer);
    ObjectRenderInfo* renderInfo;
    glm::mat4& getLocalMatrix() {return localMatrix;}
    void setLocalMatrix(glm::mat4& matrix) {localMatrix = matrix;}

    glm::vec3& getPosition() {return position;}
    void setPosition(glm::vec3& pos) {position = pos;}
private:
    glm::mat4 localMatrix{1.0f};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
};