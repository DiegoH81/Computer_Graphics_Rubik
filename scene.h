#ifndef SCENE_H
#define SCENE_H

#include "utils.h"
#include "vector.h"
#include "shape.h"
#include "shader_list.h"
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
	
	std::vector<SceneNode*> children;
    Shape* shape;
    
    SceneNode(int in_id, Shape* in_shape = nullptr);

    void draw(ShaderList& shaders, TextureList& in_textures, const Matrix_4& in_parent);

    Point3 get_center();
	Point3 get_center_local();
    Vector3 get_normal(int in_face_id);


    void add_children(SceneNode* in_children);

    void traslate(const Vector3& in_m, bool is_public);
    
    void rotate_x_global(float in_angle, bool is_public);
    void rotate_y_global(float in_angle, bool is_public);
    void rotate_z_global(float in_angle, bool is_public);
    void rotate_x_local(float in_angle, bool is_public);
    void rotate_y_local(float in_angle, bool is_public);
    void rotate_z_local(float in_angle, bool is_public);
    void scale(const Vector3& in_s, bool is_public);
};


#endif