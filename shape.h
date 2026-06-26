#ifndef SHAPE_H
#define SHAPE_H

#define YES_EBO true
#define NO_EBO false

#include <vector>
#include <fstream>

#include "matrix.h"
#include "utils.h"
#include "indices_info.h"
#include "vector.h"
#include "shader_list.h"
#include "vertex.h"
#include "texture_list.h"
#include "material.h"

inline Material base_material;
inline std::string base_texture = "NO_TEXTURE";

class Shape
{
public:
    bool has_faces, has_edges, has_points, uses_texture;
    unsigned int VAO, VBO, EBO;
    Point3 center;
    std::string shader_name;

    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices;

    std::vector <IndicesInfo> info_faces,
                              info_edges,
                              info_points;

    

    Shape();
    
    ~Shape();

    void init_buffers();

    void draw(ShaderList& shaders, TextureList& in_textures, const Matrix_4& in_world = Matrix_4());

    void set_shader_name(const std::string& in_shader_name);

    void set_face_color(int in_id, Material* in_material);
    void set_edge_color(int in_id, Material* in_material);
    void set_point_color(int in_id, Material* in_material);
    void set_textures(int in_id, std::string in_texture);

    void add_edges(Material* in_material = &base_material);
    void add_points(Material* in_material = &base_material);
    void add_faces(Material* in_material = &base_material);
    void add_textures(const std::string in_texture = base_texture);

    Vector3 get_normal(int in_face_id);
protected:
    std::vector<Vector3> normals;

    virtual void setup_edges(Material *in_material = &base_material);
    virtual void setup_points(Material *in_material = &base_material);
};

class Mesh3D: public Shape
{
public:
    Mesh3D(std::filesystem::path in_current_path, std::string in_file_path);
private:
    void load_object(std::string in_path);
};


class Circle : public Shape
{
public:
    Circle(const unsigned int& in_points, const float& in_radius = 1.0f);

private:
    unsigned int n_points;
    float radius;

    void create_circle(Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
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

    void create_sector(const float& in_ox, const float& in_oy, Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
};

class Rectangle : public Shape
{
public:
    Rectangle(const float& in_height, const float& in_width);

private:
    void create_rectangle(float in_height, float in_width, Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
};

class Elipse : public Shape
{
public:
    Elipse(const unsigned int& in_points,
           const float& in_height,
           const float& in_width);

private:
    void create_elipse(float in_height, float in_width, int in_points, Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
};

class Pyramid : public Shape
{
public:
    Pyramid(const float& in_height, const float& in_base);

private:
    float height, base;

    void create_pyramid(Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
};

class Pyramid3 : public Shape
{
public:
    Pyramid3(const float& in_height, const float& in_base);

private:
    float height, base;

    void create_pyramid(Material *in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
};

class Cube : public Shape
{
public:
    Cube(const float& in_size);

private:
    float size;

    void create_cube(Material* in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
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

    void create_cone(Material* in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
};

class Sphere : public Shape
{
public:
    Sphere(const unsigned int& in_points,
           const float& in_radius = 1.0f);

private:
    unsigned int points;
    float radius;

    void create_sphere(Material* in_material);
    void setup_edges(Material* in_material = &base_material) override;
    void setup_points(Material* in_material = &base_material) override;
};

#endif