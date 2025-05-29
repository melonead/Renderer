#include <glm/glm.hpp>


struct AABB
{
    glm::vec3 center;
    glm::vec3 xDir;
    glm::vec3 yDir;
    glm::vec3 zDir;
    glm::vec3 halfLengths;
};