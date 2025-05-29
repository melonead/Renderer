#pragma once

#include <glm/glm.hpp>
#include "AABB.h"
#include "Entity.h"

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

static Ray computeMouseRay(float mousex, float mousey, float scrWidth, float scrHeight, glm::mat4& view, glm::mat4& projection)
{
    Ray res;

    // compute start and end in NDC
    glm::vec4 start(
        (mousex / scrWidth - 0.5f) * 2.0f,
        (mousey / scrHeight - 0.5f) * 2.0f,
        -1.0f,
        1.0f
    );

    glm::vec4 end(
        (mousex / scrWidth - 0.5f) * 2.0f,
        (mousey / scrHeight - 0.5f) * 2.0f,
        0.0f,
        1.0f
    );

    // compute start and end in world space
    glm::mat4 inverseVP = glm::inverse(projection * view);

    start = inverseVP * start;
    start /= start.w;

    end = inverseVP * end;
    end /= end.w;

    res.direction = glm::normalize(end - start);

    res.origin = glm::vec3(start.x, start.y, start.z);


    return res;
}

class MousePicker
{
public:
    MousePicker() = default;
    bool testRayIntersection(Ray& ray, Entity& entity);
private:
};