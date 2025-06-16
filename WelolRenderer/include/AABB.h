#include <glm/glm.hpp>
#include "Entity.h"

class AABB
{
public:
    AABB() = default;
    ~AABB() = default;
    void update(Entity& entity);

    glm::vec3 center;
    glm::vec3 xDir;
    glm::vec3 yDir;
    glm::vec3 zDir;
    glm::vec3 halfLengths;
    glm::vec3 min;
    glm::vec3 max;
};

