#include "vector.h"

Vector3::Vector3(const float& in_x, const float& in_y, const float& in_z):
    x(in_x), y(in_y), z(in_z)
{}

Vector3 Vector3::operator + (const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator - (const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator / (const float& div) const
{
    if (div < 1e-8f)
        return Vector3(0.0f, 0.0f, 0.0f);

    return Vector3(x / div, y / div, z / div);
}

Vector3& Vector3::operator = (const Vector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}

Vector3 cross (const Vector3& v1, const Vector3& v2)
{
    float new_x = v1.y * v2.z - v2.y * v1.z;
    float new_y = - (v1.x * v2.z - v2.x * v1.z);
    float new_z = v1.x * v2.y - v2.x * v1.y;

    return Vector3(new_x, new_y, new_z);
}

float vec_length(const Vector3& in_vec)
{
    return std::sqrt(in_vec.x * in_vec.x + in_vec.y * in_vec.y + in_vec.z * in_vec.z);
}

Vector3 normalize (const Vector3& in_vec)
{
    float l = vec_length(in_vec);
    return (in_vec / l);
}

Vector2::Vector2(const float& in_x, const float& in_y):
    x(in_x), y(in_y)
{}

Vector2& Vector2::operator = (const Vector3& other)
{
    x = other.x;
    y = other.y;

    return *this;
}