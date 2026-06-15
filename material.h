#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"

class Material
{
public:
    Vector3 ambient, diffuse, specular;
    float shininess, alpha_value;

    Material() : ambient(Vector3(1.0f, 1.0f, 1.0f)), diffuse(Vector3(1.0f, 1.0f, 1.0f)), specular(Vector3(0.0f, 0.0f, 0.0f)), shininess(1.0f), alpha_value(1.0f)
    {}
};

#endif