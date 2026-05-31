#include "matrix.h"

Matrix_4::Matrix_4():
    matrix(16)
{ 
    for (int i = 0; i < 16; i++)
    {
        if (i % 5 == 0)
            matrix[i] = 1;
        else
            matrix[i] = 0;
    }
}

Matrix_4& Matrix_4:: operator = (const Matrix_4& other)
{
    matrix = other.matrix;
    return *this;
}

Matrix_4 Matrix_4::operator * (const Matrix_4& other)
{
    Matrix_4 to_return;
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int pos = 4 * i + j;
            float total = 0;
            for (int k = 0; k < 4; k++)
            {
                int row = (4 * i) + k;
                int col = (4 * k) + j;
                
                total += matrix[row] * other.matrix[col];
            }
            to_return.matrix[pos] = total;
        }
    }

    return to_return;
}

Point3 Matrix_4::operator * (const Point3& in_point)
{
    Point3 to_return;

    to_return.x = in_point.x * matrix[0] + in_point.y * matrix[1] + in_point.z * matrix[2] + matrix[3];
    to_return.y = in_point.x * matrix[4] + in_point.y * matrix[5] + in_point.z * matrix[6] + matrix[7];
    to_return.z = in_point.x * matrix[8] + in_point.y * matrix[9] + in_point.z * matrix[10] + matrix[11];

    return to_return;
}

void Matrix_4::print()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int pos = 4 * i + j;

            std::cout << matrix[pos] << "\t";
        }
        std::cout << "\n";
    }
}

void Matrix_4::set_matrix(std::vector<float> in_mat)
{
    matrix = in_mat;
}

void Matrix_4::scale(const Vector3& in_s)
{
    Matrix_4 mat;
    mat.set_matrix({ in_s.x,      0,      0,   0,
                            0, in_s.y,      0,   0,
                            0,      0, in_s.z,   0,
                            0,      0,      0,   1   });

    *this = mat * (*this);
}

void Matrix_4::traslate(const Vector3& in_m)
{
    Matrix_4 mat;
    mat.set_matrix({ 1, 0, 0, in_m.x,
                    0, 1, 0, in_m.y,
                    0, 0, 1, in_m.z,
                    0, 0, 0,      1 });

    *this = mat * (*this);
}

void Matrix_4::rotate_x(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float sin = std::sin(ang);
    float cos = std::cos(ang);

    Matrix_4 mat;
    mat.set_matrix({ 1,   0,    0,  0,
                        0, cos, -sin,  0,
                        0, sin,  cos,  0,
                        0,   0,    0,  1});

    *this = mat * (*this);
}

void Matrix_4::rotate_y(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float sin = std::sin(ang);
    float cos = std::cos(ang);

    Matrix_4 mat;
    mat.set_matrix({  cos,  0,  sin,  0,
                        0,  1,    0,  0,
                        -sin,  0,  cos,  0,
                        0,  0,    0,  1});

    *this = mat * (*this);
}

void Matrix_4::rotate_z(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float sin = std::sin(ang);
    float cos = std::cos(ang);

    Matrix_4 mat;
    mat.set_matrix({ cos, -sin,  0,  0,
                        sin,  cos,  0,  0,
                        0,    0,  1,  0,
                        0,    0,  0,  1});

    *this = mat * (*this);
}

void Matrix_4::transpose()
{
    std::vector<float> temp(16, 0);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int pos = (4*i) + j;
            int t_pos = i + (4 * j);
            temp[pos] = matrix[t_pos];
        }
    }

    matrix = temp;
}


Matrix_4 get_perspective(const float& in_y_fov,
                         const float& in_aspect_ratio,
                         const float& in_near, const float& in_far)
{
    Matrix_4 to_return;

    float ang = utils::ang_to_rad(in_y_fov);

    float top = std::tan(ang / 2.0f) * in_near;
    float right = top * in_aspect_ratio;

    float A = -(in_far + in_near)/(in_far - in_near);
    float B = (-2 * in_far * in_near)/(in_far - in_near);

    to_return.set_matrix({ in_near/right,        0.0f, 0.0f, 0.0f,
                                    0.0f, in_near/top, 0.0f, 0.0f,
                                    0.0f,        0.0f,    A,    B,
                                    0.0f,        0.0f, -1.0f, 0.0f });

    

    return to_return;
}

Vector3 Matrix_4::transform_normal(const Vector3& in_vector)
{
    float x = matrix[0]*in_vector.x + matrix[1]*in_vector.y + matrix[2]*in_vector.z;
    float y = matrix[4]*in_vector.x + matrix[5]*in_vector.y + matrix[6]*in_vector.z;
    float z = matrix[8]*in_vector.x + matrix[9]*in_vector.y + matrix[10]*in_vector.z;
    return Vector3(x, y, z);
}