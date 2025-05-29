#include "objectFactory.h"

class Entity
{
public:
    Entity(glm::vec3 position);
    ~Entity();
    void update(Welol::Camera& camera, glm::mat4& projectionMatrix, Welol::Renderer& renderer);
    ObjectRenderInfo* renderInfo;
private:
    glm::mat4 localMatrix{1.0f};
};