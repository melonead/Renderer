#include "MousePicker.h"



bool MousePicker::testRayIntersection(Ray& ray, Entity& entity)
{
    // Create the AABB
    AABB aabb;

    aabb.update(entity);

    float tmin = -99999.0f;
    float tmax = 99999.0f;

    float eps = 0.000001f;

    glm::vec3 aabbDirections[3] = {
        aabb.xDir, aabb.yDir, aabb.zDir
    };

    glm::vec3 rayOriginToAABBCenter = aabb.center - ray.origin;

    float t1 = 0.0f;
    float t2 = 0.0f;

    for (int i = 0; i < 3; i++)
    {
        float dotRB = glm::dot(aabbDirections[i], rayOriginToAABBCenter);
        float dotDir = glm::dot(aabbDirections[i], ray.direction);

        // Test if the ray is not parallel to the plane (side of aabb) i.e not perpendicular to the normals which are the
        // aabb side directions.
        if (abs(dotDir) > eps)
        {
            t1 = (dotRB + aabb.halfLengths[i]) / dotDir;
            t2 = (dotRB - aabb.halfLengths[i]) / dotDir;

            // Swap to make sure t1 always holds min and t2 max.
            if (t1 > t2)
            {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            if (t1 > tmin)
                tmin = t1;
            if (t2 < tmax)
                tmax = t2;
            
            // No intersection.
            if (tmin > tmax)
                return false;
            // No intersection, aabb is behind the ray origin
            if (tmax < 0.0f)
                return false;
        } else 
        {
            // We are parallel to the plane (one of the sides of the aabb)/slab so cannot intersect
            return false;
        }
    }

    // If we get here there is an intersection
    // We can test for whether we are intersection

    return true;
}
