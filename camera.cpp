#include "camera.h"

void Camera::set_pos(const Point3& in_pos)
{
    pos = in_pos;
    initialize();
}

void Camera::set_objective(const Point3& in_objective)
{
    objective = in_objective;
    initialize();
}

void Camera::traslate(const Vector3& in_m)
{
    pos = pos + in_m;
    objective = objective + in_m;
    initialize();
}

void Camera::rotate_x(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);


    objective = objective - pos;
    Vector3 temp = objective;

    objective.y = temp.y * cos - temp.z * sin;
    objective.z = temp.y * sin + temp.z * cos;

    objective = objective + pos;

    initialize();
}

void Camera::rotate_y(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);


    objective = objective - pos;
    Vector3 temp = objective;

    objective.x = temp.x * cos + temp.z * sin;
    objective.z = -temp.x * sin + temp.z * cos;

    objective = objective + pos;

    initialize();
}

void Camera::rotate_z(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);


    objective = objective - pos;
    Vector3 temp = objective;

    objective.x = temp.x * cos - temp.y * sin;
    objective.y = temp.x * sin + temp.y * cos;

    objective = objective + pos;

    initialize();
}

void Camera::orbit_x(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);

    pos = pos - objective;
    Vector3 temp = pos;

    pos.y = temp.y * cos - temp.z * sin;
    pos.z = temp.y * sin + temp.z * cos;

    pos = pos + objective;

    initialize();
}

void Camera::orbit_y(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);

    pos = pos - objective;
    Vector3 temp = pos;

    pos.x = temp.x * cos + temp.z * sin;
    pos.z = -temp.x * sin + temp.z * cos;

    pos = pos + objective;

    initialize();
}

void Camera::orbit_z(float in_angle)
{
    float ang = utils::ang_to_rad(in_angle);
    float cos = std::cos(ang);
    float sin = std::sin(ang);

    pos = pos - objective;
    Vector3 temp = pos;

    pos.x = temp.x * cos - temp.y * sin;
    pos.y = temp.x * sin + temp.y * cos;

    pos = pos + objective;

    initialize();
}

Matrix_4 Camera::get_look_at()
{
    Matrix_4 all_vecs_camera;
    Matrix_4 traslate_camera;

    all_vecs_camera.set_matrix({ v.x,  v.y,  v.z, 0.0f,
                                 u.x,  u.y,  u.z, 0.0f,
                                 w.x,  w.y,  w.z, 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f});

    traslate_camera.set_matrix({1.0f, 0.0f, 0.0f, -pos.x,
                                0.0f, 1.0f, 0.0f, -pos.y,
                                0.0f, 0.0f, 1.0f, -pos.z,
                                0.0f, 0.0f, 0.0f,   1.0f});

    return all_vecs_camera * traslate_camera;
}

void Camera::initialize()
{
    Vector3 up(0.0f, 1.0f, 0.0f);
    w = normalize(pos - objective);
    v = normalize(cross(up, w));
    u = normalize(cross(w, v));
}