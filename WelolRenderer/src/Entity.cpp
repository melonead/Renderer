#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 pos)
{
    localMatrix = glm::translate(localMatrix, pos);
    position = pos;
}

void Entity::update(Welol::Camera& camera, glm::mat4& projectionMatrix, Welol::Renderer& renderer)
{
    renderInfo->update(renderer, camera, projectionMatrix, localMatrix);
}

Entity::~Entity()
{
    
}
