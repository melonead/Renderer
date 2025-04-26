#include "WelolMath.h"
#include "math.h"

namespace WelolMath {
    
    Vec3::Vec3(float a, float b, float c)
        : x(a),
          y(b),
          z(c) {}

    Vec3 Vec3::operator-(Vec3& operand)
    {
        return Vec3(
            x - operand.x,
            y - operand.y,
            z - operand.z
        );
    }

    Vec3 Vec3::operator+(Vec3& operand)
    {
        return Vec3(
            x + operand.x,
            y + operand.y,
            z + operand.z
        );
    }

    Vec3 Vec3::operator*(float mult)
    {
        return Vec3(
            x * mult,
            y * mult,
            z * mult
        );
    }

    

    Vec4::Vec4(float a, float b, float c, float d)
        : x(a),
          y(b),
          z(c),
          w(d) {}

    
    Vec4 Vec4::operator-(const Vec4& operand)
    {
        return Vec4(
            x - operand.x,
            y - operand.y,
            z - operand.z,
            w - operand.w
        );
    }

    Vec4 Vec4::operator+(const Vec4& operand)
    {
        return Vec4(
            x + operand.x,
            y + operand.y,
            z + operand.z,
            w + operand.w
        );
    }

    Vec4 Vec4::operator*(float mult)
    {
        return Vec4(
            x * mult,
            y * mult,
            z * mult,
            w * mult
        );
    }

    

    Mat4x4::Mat4x4(float diag)
    {
        // First column
        mat[0][0] = diag;
        mat[0][1] = 0.0f;
        mat[0][2] = 0.0f;
        mat[0][3] = 0.0f;
        // Second column
        mat[1][0] = 0.0f;
        mat[1][1] = diag;
        mat[1][2] = 0.0f;
        mat[1][3] = 0.0f;
        // Third column
        mat[2][0] = 0.0f;
        mat[2][1] = 0.0f;
        mat[2][2] = diag;
        mat[2][3] = 0.0f;
        // Fourth column
        mat[3][0] = 0.0f;
        mat[3][1] = 0.0f;
        mat[3][2] = 0.0f;
        mat[3][3] = diag;

    }

    Vec4 Mat4x4::operator*(const Vec4& vecR)
    {   
        Vec4 result;
        result.x = mat[0][0] * vecR.x + mat[0][1] * vecR.x + mat[0][2] * vecR.x + mat[0][3] * vecR.x;
        result.y = mat[1][0] * vecR.y + mat[1][1] * vecR.y + mat[1][2] * vecR.y + mat[1][3] * vecR.y;
        result.z = mat[2][0] * vecR.z + mat[2][1] * vecR.z + mat[2][2] * vecR.z + mat[2][3] * vecR.z;
        result.w = mat[3][0] * vecR.w + mat[3][1] * vecR.w + mat[3][2] * vecR.w + mat[3][3] * vecR.w;
        return result;
    }

    float* Mat4x4::operator[](int a)
    {
        return mat[a];
    }

    Vec4 Mat4x4::getColumn4(int col)
    {
        return Vec4(
            mat[col][0], mat[col][1], mat[col][2], mat[col][3]
        );
    }

    Vec3 Mat4x4::getColumn3(int col)
    {
        return Vec3(
            mat[col][0], mat[col][1], mat[col][2]
        );
    }
}