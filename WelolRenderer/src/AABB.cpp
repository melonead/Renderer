#include "AABB.h"

void AABB::update(Entity& entity)
{
    center = entity.getPosition() + (entity.renderInfo->min + entity.renderInfo->max) * 0.5f;
    xDir = glm::vec3(entity.getLocalMatrix()[0].x, entity.getLocalMatrix()[0].y, entity.getLocalMatrix()[0].z);
    yDir = glm::vec3(entity.getLocalMatrix()[1].x, entity.getLocalMatrix()[1].y, entity.getLocalMatrix()[1].z);
    zDir = glm::vec3(entity.getLocalMatrix()[2].x, entity.getLocalMatrix()[2].y, entity.getLocalMatrix()[2].z);

    halfLengths.x = (entity.renderInfo->max.x - entity.renderInfo->min.x) * 0.5f;
    halfLengths.y = (entity.renderInfo->max.y - entity.renderInfo->min.y) * 0.5f;
    halfLengths.z = (entity.renderInfo->max.z - entity.renderInfo->min.z) * 0.5f;

    min = entity.getPosition() + entity.renderInfo->min;
    max = entity.getPosition() + entity.renderInfo->max;
}
