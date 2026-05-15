#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

class Vertex
{
public:
    Point3 pos;
    Vector3 normal;
    Point2 tex_coord;

    Vertex(const Point3& in_pos = Point3(),
           const Vector3& in_normal = Vector3(),
           const Point2& in_tex_coord = Point2()):
        pos(in_pos), normal(in_normal), tex_coord(in_tex_coord)
    {}

};

#endif