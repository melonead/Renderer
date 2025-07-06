#pragma once
#include "math.h"
#include <glm/glm.hpp>

struct Plane
{
    glm::vec3 normal;
    glm::vec3 center;
    float displacement;
};


struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

namespace WelolMath {

    struct Vec3
    {   
        Vec3() = default;
        Vec3(float a, float b, float c);
        float x;
        float y;
        float z;

        Vec3 operator-(Vec3& operand);
        Vec3 operator+(Vec3& operand);
        Vec3 operator*(float mult);
    };
    
    struct Vec4
    {
        Vec4() = default;
        Vec4(float a, float b, float c, float d);
        float x;
        float y;
        float z;
        float w;

        Vec4 operator-(const Vec4& operand);
        Vec4 operator+(const Vec4& operand);
        Vec4 operator*(float mult);
    };
    
    struct Mat4x4
    {
        Mat4x4(float diag);
        float mat[4][4];
        Vec4 operator*(const Vec4& vecR);
        float* operator[](int col);
        Vec4 getColumn4(int col);
        Vec3 getColumn3(int col);
        float* getDataPtr() {return mat[0];};
    };




    static float dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    static float dot(const Vec4& a, const Vec4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return Vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }

    static Vec4 cross(const Vec4& a, const Vec4& b)
    {

        return Vec4(
            a.y * b.z - b.z * a.w,
            a.z * b.w - b.w * a.x,
            a.w * b.x - b.x * a.y,
            a.x * b.y - b.y * a.z
        );
    }

    static Vec3 normalize(const Vec3& a)
    {
        float temp = dot(a, a);
        float mag = sqrtf(temp);
        return Vec3(
            a.x / mag, a.y / mag, a.z / mag
        );
    } 

    static Vec4 normalize(const Vec4& a)
    {
        float temp = dot(a, a);
        float mag = sqrtf(temp);
        return Vec4(
            a.x / mag, a.y / mag, a.z / mag, a.w / mag
        );
    }

    static Mat4x4 transpose(Mat4x4& mat)
    {
        Mat4x4 result(1.0f);
        for(int col = 0; col < 4; col++)
        {
            for(int row = 0; row < 4; row++)
            {
                // REVISIT: I want mat to be passed as const reference but it is giveing
                // errors, why?
                result[col][row] = mat[row][col];
            }
        }
        return result;
    }

    static Vec4 operator*(float mult, const Vec4& vec)
    {
        return Vec4(
            vec.x * mult,
            vec.y * mult,
            vec.z * mult,
            vec.w * mult
        );
    }

    static Vec4 operator+(const Vec4& vec1, const Vec4& vec2)
    {
        return Vec4(
            vec1.x + vec2.x,
            vec1.y + vec2.y,
            vec1.z + vec2.z,
            vec1.w + vec2.w
        );
    }

    static Vec3 operator+(const Vec3& vec1, const Vec3& vec2)
    {
        return Vec3(
            vec1.x + vec2.x,
            vec1.y + vec2.y,
            vec1.z + vec2.z
        );
    }

    static Vec3 operator*(float mult, const Vec3& vec)
    {
        return Vec3(
            vec.x * mult,
            vec.y * mult,
            vec.z * mult
        );
    }

    static Vec3 rotateAroundAxis(Vec3& vectorToRotate, Vec3& rotationAxis, float angle)
    {
        float c = cosf(angle);
        float s = sinf(angle);
        return (1.0f - c) * dot(vectorToRotate, rotationAxis) * rotationAxis +
                c * vectorToRotate + s * cross(rotationAxis, vectorToRotate); 
    }

    static float vectorLength(const Vec3& vec)
    {
        return sqrtf(
            dot(vec, vec)
        );
    }
}

