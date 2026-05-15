#ifndef SCENE_H
#define SCENE_H

#include "utils.h"
#include "vector.h"
#include "shape.h"
#include "shaderList.h"
#include "matrix.h"

#define PUBLIC_TR true
#define PRIVATE_TR false

class SceneNode
{
public:
    int id;

    Matrix_4 public_transform,
             private_transform;

    Matrix_4 parent_transform;

    Shape* shape;
    
    SceneNode(int in_id, Shape* in_shape = nullptr) :
        id(in_id), shape(in_shape),
        public_transform(), private_transform(),
        parent_transform()
    { }

    void draw(ShaderList& shaders, const Matrix_4& in_parent)
    {
        parent_transform = in_parent;
        if (shape)
            shape->draw(shaders, parent_transform * private_transform);
        
        for (auto &child : children)
            child->draw(shaders, parent_transform * public_transform);
    }

    Point3 get_center()
    {
        Point3 to_return;

        if (shape)
            to_return = parent_transform * private_transform * shape->center;

        return to_return;
    }

    void add_children(SceneNode* in_children)
    {
        children.push_back(in_children);
    }

    void traslate(const Vector3& in_m, bool is_public)
    {
        private_transform.traslate(in_m);
        if (is_public)
            public_transform.traslate(in_m);
    }
    
    void rotate_x_global(float in_angle, bool is_public)
    {
        private_transform.rotate_x(in_angle);

        if (is_public)
            public_transform.rotate_x(in_angle);
    }
    void rotate_y_global(float in_angle, bool is_public)
    {
        private_transform.rotate_y(in_angle);

        if (is_public)
            public_transform.rotate_y(in_angle);
    }
    void rotate_z_global(float in_angle, bool is_public)
    {
        private_transform.rotate_z(in_angle);

        if (is_public)
            public_transform.rotate_z(in_angle);
    }
    void rotate_x_local(float in_angle, bool is_public)
    {
        Matrix_4 R;
        R.rotate_x(in_angle);

        private_transform = private_transform * R;
        if (is_public)
            public_transform = public_transform * R;
    }
    void rotate_y_local(float in_angle, bool is_public)
    {
        Matrix_4 R;
        R.rotate_y(in_angle);

        private_transform = private_transform * R;
        if (is_public)
            public_transform = public_transform * R;
    }
    void rotate_z_local(float in_angle, bool is_public)
    {
        Matrix_4 R;
        R.rotate_z(in_angle);

        private_transform = private_transform * R;
        if (is_public)
            public_transform = public_transform * R;
    }
    
    void scale(const Vector3& in_s, bool is_public)
    {
        Matrix_4 E;
        E.scale(in_s);

        private_transform = private_transform * E;
        if (is_public)
            public_transform  = public_transform * E;
    }

private:
    std::vector<SceneNode*> children;
};


#endif