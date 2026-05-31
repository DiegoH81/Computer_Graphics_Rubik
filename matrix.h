#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

#include "utils.h"
#include "vector.h"

class Matrix_4
{
public:
    std::vector<float> matrix;

    Matrix_4();

    Matrix_4& operator = (const Matrix_4& other);

    Matrix_4 operator * (const Matrix_4& other);

    Point3 operator * (const Point3& in_point);

    void print();
    
    void set_matrix(std::vector<float> in_mat);

    void scale(const Vector3& in_s);

    void traslate(const Vector3& in_m);

    void rotate_x(float in_angle);
    
    void rotate_y(float in_angle);

    void rotate_z(float in_angle);

    void transpose();

    Vector3 transform_normal(const Vector3& in_vector);
};


Matrix_4 get_perspective(const float& in_y_fov,
                         const float& in_aspect_ratio,
                         const float& in_near, const float& in_far);

#endif