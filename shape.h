#ifndef SHAPE_H
#define SHAPE_H

#define YES_EBO true
#define NO_EBO false

#include <vector>

#include "matrix.h"
#include "utils.h"
#include "indices_info.h"
#include "vector.h"
#include "shader_list.h"
#include "vertex.h"
#include "texture_list.h"

inline Color base_color(70, 130, 180, true);
inline std::string base_texture = "NO_TEXTURE";

class Shape
{
public:
    bool has_faces, has_edges, has_points, uses_texture;
    unsigned int VAO, VBO, EBO;
    Point3 center;

    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices;

    std::vector <IndicesInfo> info_faces,
                              info_edges,
                              info_points;

    Shape();
    
    ~Shape();

    void init_buffers();

    void draw(ShaderList& shaders, TextureList& in_textures, const Matrix_4& in_world = Matrix_4());

    void set_face_color(int in_id, Color* in_color);
    void set_edge_color(int in_id, Color* in_color);
    void set_point_color(int in_id, Color* in_color);
    void set_textures(int in_id, std::string in_texture);

    void add_edges(Color *in_color = &base_color);
    void add_points(Color *in_color = &base_color);
    void add_faces(Color *in_color = &base_color);
    void add_textures(const std::string in_texture = base_texture);

    Vector3 get_normal(int in_face_id);
protected:
    std::vector<Vector3> normals;

    virtual void setup_edges(Color *in_color = &base_color);
    virtual void setup_points(Color *in_color = &base_color);
};


class Circle : public Shape
{
public:
    Circle(const unsigned int& in_points, const float& in_radius = 1.0f);

private:
    unsigned int n_points;
    float radius;

    void create_circle(Color *in_color);
    void setup_edges(Color* in_color = &base_color) override;
};

class CircularSector : public Shape
{
public:
    CircularSector(const unsigned int& in_points,
                    const float& in_start,
                    const float& in_end,
                    const float& in_radius = 1.0f,
					
                    const float& in_ox = 0.0f,
                    const float& in_oy = 0.0f);

private:
    unsigned int n_points;
    float radius, start_angle, end_angle;

    void create_sector(const float& in_ox, const float& in_oy, Color *in_color);
    void setup_edges(Color* in_color = &base_color) override;
};

class Rectangle : public Shape
{
public:
    Rectangle(const float& in_height, const float& in_width);

private:
    void create_rectangle(float in_height, float in_width, Color *in_color);
    void setup_edges(Color* in_color = &base_color) override;
    void setup_points(Color* in_color = &base_color) override;
};

class Elipse : public Shape
{
public:
    Elipse(const unsigned int& in_points,
           const float& in_height,
           const float& in_width);

private:
    void create_elipse(float in_height, float in_width, int in_points, Color *in_color);
    void setup_edges(Color* in_color = &base_color) override;
};

class Pyramid : public Shape
{
public:
    Pyramid(const float& in_height, const float& in_base);

private:
    float height, base;

    void create_pyramid(Color *in_color);
    void setup_edges(Color* in_color = &base_color) override;
    void setup_points(Color* in_color = &base_color) override;
};

class Cube : public Shape
{
public:
    Cube(const float& in_size);

private:
    float size;

    void create_cube(Color* in_color);
    void setup_edges(Color* in_color = &base_color) override;
    void setup_points(Color* in_color = &base_color) override;
};

class Cone : public Shape
{
public:
    Cone(const unsigned int& in_points,
         const float& in_height,
         const float& in_radius = 1.0f);

private:
    float height, radius;
    unsigned int points;

    void create_cone(Color* in_color);
    void setup_edges(Color* in_color = &base_color) override;
    void setup_points(Color* in_color = &base_color) override;
};

class Sphere : public Shape
{
public:
    Sphere(const unsigned int& in_points,
           const float& in_radius = 1.0f);

private:
    unsigned int points;
    float radius;

    void create_sphere(Color* in_color);
    void setup_edges(Color* in_color = &base_color) override;
    void setup_points(Color* in_color = &base_color) override;
};

#endif