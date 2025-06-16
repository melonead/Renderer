static void moveEntity(Entity& entity, glm::vec3<float>& displacement)
{
    glm::mat4 m = glm::translate(entity.getLocalMatrix(), displacement);
    entity.setLocalMatrix(m);
}

static float distanceToSegment(glm::vec3<float> lineStart, glm::vec3<float> lineEnd, glm::vec3<float> point)
{
    glm::vec3<float> startToEnd = lineEnd - lineStart;
    glm::vec3<float> startToPoint = point - lineStart;
    glm::vec3<float> endToPoint = point - lineEnd;

    // case for when the point projects outside the line
    float e = glm::dot(startToPoint, startToEnd);
    if (e <= 0.0f)
        return glm::dot(startToPoint, startToPoint);
    float f = glm::dot(startToEnd, startToEnd);
    if(e >= f)
        return glm::dot(endToPoint, endToPoint);

    // case for when the point projects on line

    return glm::dot(startToPoint, startToPoint) - e * e / f;
}


static void decideAxisOfMovement(glm::vec3<float>& mousePos, Entity& entity)
{
    // The gizmo axis correspond to the world axis
    AABB aabb;
    aabb.update(entity);
    // The center of AABB is also the position of the gizmo
    float thresholdDistance = 0.00005f;
    // compute distance from mouse to each of the gizmo axis
    
    float gizmoDistance = 5.0f;
    glm::vec3<float> start = aabb.center;
    glm::vec3<float> end = aabb.center + glm::vec3(gizmoDistance, 0.0f, 0.0f);

    glm::vec3<float> axes = {
        {gizmoDistance, 0.0f, 0.0f},
        {0.0f, gizmoDistance, 0.0f},
        {0.0f, 0.0f, gizmoDistance}
    }

    float distance = 99999.0f;
    int lastDistance = 99999.0f;
    int shortDistanceAxis = 0;
    for (int i = 0; i < 3; i++)
    {
        glm::vec3<float> axisEnd = start + axes[i];
        distance = distanceToSegment(start, axisEnd, mousePos);
        if (distance < lastDistance)
        {
            lastDistance = distance;
            shortDistanceAxis = i;
        }
    }
}
