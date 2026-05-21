#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector3
{
public:
    float x, y, z;

    Vector3(const float& in_x = 0.0f, const float& in_y = 0.0f, const float& in_z = 0.0f);

    Vector3 operator + (const Vector3& other) const;

    Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator - (const Vector3& other) const;

    Vector3 operator / (const float& div) const;

    Vector3& operator = (const Vector3& other);
};

class Vector2
{
public:
    float x, y;

    Vector2(const float& in_x = 0.0f, const float& in_y = 0.0f);

    Vector2& operator = (const Vector3& other);
};

Vector3 cross (const Vector3& v1, const Vector3& v2);

float vec_length(const Vector3& in_vec);

Vector3 normalize (const Vector3& in_vec);

using Point3 = Vector3;
using Point2 = Vector2;

#endif