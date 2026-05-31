#include "scene.h"

SceneNode::SceneNode(int in_id, Shape* in_shape) :
    id(in_id), shape(in_shape),
    public_transform(), private_transform(),
    parent_transform()
{ }

void SceneNode::draw(ShaderList& shaders, TextureList& in_textures, const Matrix_4& in_parent)
{
    parent_transform = in_parent;
    if (shape)
        shape->draw(shaders, in_textures, parent_transform * private_transform);
    
    for (auto &child : children)
        child->draw(shaders, in_textures, parent_transform * public_transform);
}

Point3 SceneNode::get_center()
{
    Point3 to_return;

    if (shape)
        to_return = parent_transform * private_transform * shape->center;

    return to_return;
}

Point3 SceneNode::get_center_local()
{
	Point3 to_return;

	if (shape)
		to_return = private_transform * shape->center;

	return to_return;
}

Vector3 SceneNode::get_normal(int in_face_id)
{
    Vector3 normal;

    if (shape)
    {
        normal = shape->get_normal(in_face_id);
        normal = private_transform.transform_normal(normal);
    }

    return normal;
}

void SceneNode::add_children(SceneNode* in_children)
{
    children.push_back(in_children);
}

void SceneNode::traslate(const Vector3& in_m, bool is_public)
{
    private_transform.traslate(in_m);
    if (is_public)
        public_transform.traslate(in_m);
}

void SceneNode::rotate_x_global(float in_angle, bool is_public)
{
    private_transform.rotate_x(in_angle);

    if (is_public)
        public_transform.rotate_x(in_angle);
}
void SceneNode::rotate_y_global(float in_angle, bool is_public)
{
    private_transform.rotate_y(in_angle);

    if (is_public)
        public_transform.rotate_y(in_angle);
}
void SceneNode::rotate_z_global(float in_angle, bool is_public)
{
    private_transform.rotate_z(in_angle);

    if (is_public)
        public_transform.rotate_z(in_angle);
}
void SceneNode::rotate_x_local(float in_angle, bool is_public)
{
    Matrix_4 R;
    R.rotate_x(in_angle);

    private_transform = private_transform * R;
    if (is_public)
        public_transform = public_transform * R;
}
void SceneNode::rotate_y_local(float in_angle, bool is_public)
{
    Matrix_4 R;
    R.rotate_y(in_angle);

    private_transform = private_transform * R;
    if (is_public)
        public_transform = public_transform * R;
}
void SceneNode::rotate_z_local(float in_angle, bool is_public)
{
    Matrix_4 R;
    R.rotate_z(in_angle);

    private_transform = private_transform * R;
    if (is_public)
        public_transform = public_transform * R;
}

void SceneNode::scale(const Vector3& in_s, bool is_public)
{
    Matrix_4 E;
    E.scale(in_s);

    private_transform = private_transform * E;
    if (is_public)
        public_transform  = public_transform * E;
}
