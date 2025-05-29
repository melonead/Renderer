#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 position)
{
    localMatrix = glm::translate(localMatrix, position);
}

void Entity::update(Welol::Camera& camera, glm::mat4& projectionMatrix, Welol::Renderer& renderer)
{
    renderInfo->update(renderer, camera, projectionMatrix, localMatrix);
}

Entity::~Entity()
{
    
}