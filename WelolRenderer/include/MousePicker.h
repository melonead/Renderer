#pragma once

#include <glm/glm.hpp>
#include "AABB.h"
#include "Entity.h"

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

static Ray computeMouseRay(float mousex, float mousey, float scrWidth, float scrHeight, Welol::Camera& camera, glm::mat4& projection)
{
    Ray res;

    // compute start and end in NDC
    //glm::vec4 start(
    //    ((float)mousex / scrWidth - 0.5f) * 2.0f,
    //    ((float)mousey / scrHeight - 0.5f) * 2.0f * -1.0f,
    //    -1.0f,
    //    1.0f
    //);

    //glm::vec4 end(
    //    ((float)mousex / scrWidth - 0.5f) * 2.0f,
    //    ((float)mousey / scrHeight - 0.5f) * 2.0f * -1.0f,
    //    0.0f,
    //    1.0f
    //);

    //// compute start and end in world space
    //glm::mat4 inverseVP = glm::inverse(projection * camera.getViewMatrix());

    //start = inverseVP * start;
    //start /= start.w;

    //end = inverseVP * end;
    //end /= end.w;

    //res.direction = glm::normalize(end - start);
    //res.direction.y;

    //res.origin = glm::vec3(start.x, start.y, start.z);

    float x = (2.0f * mousex) / scrWidth - 1.0f;
    float y = 1.0f - (2.0f * mousey) / scrHeight;
    float z = 1.0f;

    glm::vec3 ray_nds = glm::vec3(x, y, z);

    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;

    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec4 intermediate_ray_world = glm::inverse(camera.getViewMatrix()) * ray_eye;

    glm::vec3 ray_world = glm::vec3(intermediate_ray_world.x, intermediate_ray_world.y, intermediate_ray_world.z);

    ray_world = glm::normalize(ray_world);

    res.direction = ray_world;

    res.origin = camera.getPosition();


    return res;
}

class MousePicker
{
public:
    MousePicker() = default;
    bool testRayIntersection(Ray& ray, Entity& entity);
private:
};