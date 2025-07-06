#pragma once

#include <glm/glm.hpp>
#include "AABB.h"
#include "Entity.h"
#include "WelolMath.h"


class MousePicker
{
public:
    MousePicker() = default;
    bool testRayIntersection(Ray& ray, Entity& entity);
private:
};
