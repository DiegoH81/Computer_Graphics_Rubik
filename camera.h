#ifndef CAMERA_H
#define CAMERA_H

#include <queue>

#include "vector.h"
#include "matrix.h"
#include "utils.h"

class Camera
{
public:
    Point3 pos, objective;
    Vector3 v, u, w;

    void set_pos(const Point3& in_pos);

    void set_objective(const Point3& in_objective);

    void traslate(const Vector3& in_m);

    void rotate_x(float in_angle);

    void rotate_y(float in_angle);

    void rotate_z(float in_angle);

    void orbit_x(float in_angle);

    void orbit_y(float in_angle);

    void orbit_z(float in_angle);

    Matrix_4 get_look_at();

private:
    void initialize();
};



#endif