
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shape.h"

// SHAPE

Shape::Shape():
    vertices(), indices(), center(), shader_name("UNIQUE"), material(&base_material),

    has_faces(false),
    has_edges(false),
    has_points(false),
    uses_texture(false),
    normals()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Shape::~Shape()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Shape::init_buffers()
{
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Specify how to read
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shape::set_shader_name(const std::string& in_shader_name)
{
    shader_name = in_shader_name;
}

void Shape::set_material(Material* in_material)
{
    material = in_material;
}

void Shape::draw(ShaderList& shaders, TextureList& in_textures, const Matrix_4& in_world)
{
    glBindVertexArray(VAO);

    shaders.use_shader(shader_name);
    shaders.set_mat4(shader_name, "model", in_world);
    shaders.set_bool(shader_name, "useTexture", uses_texture);
    shaders.set_material(shader_name, "material", material);

    if (has_faces)
    {
        for (auto &face: info_faces)
        {
            if (uses_texture)
            {
                in_textures.use_texture(face.texture_name, 0);
                shaders.set_texture(shader_name, "ourTexture", 0);
            }
            else
            {
                auto &color = face.color;
                shaders.set_vec3(shader_name, "color", color->r, color->g, color->b);
            }
            
            if (!face.uses_EBO)
                glDrawArrays(face.draw_mode, face.start_indice, face.count);
            else
                glDrawElements(face.draw_mode, face.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * face.start_indice));
        }
        
    }

    if (has_edges)
    {
        for (auto &edge: info_edges)
        {
            auto &color = edge.color;
            shaders.set_vec3(shader_name, "color", color->r, color->g, color->b);
            
            if (!edge.uses_EBO)
                glDrawArrays(edge.draw_mode, edge.start_indice, edge.count);
            else
                glDrawElements(edge.draw_mode, edge.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * edge.start_indice));
        }
    }

    if (has_points)
    {
        for (auto &point: info_points)
        {
            auto &color = point.color;
            shaders.set_vec3(shader_name, "color", color->r, color->g, color->b);
            
            if (!point.uses_EBO)
                glDrawArrays(point.draw_mode, point.start_indice, point.count);
            else
                glDrawElements(point.draw_mode, point.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * point.start_indice));
        }
    }
}

void Shape::set_face_color(int in_id, Color* in_color)
{
    if (in_id == ALL_IDs)
    {
        for (auto &f : info_faces)
            f.color = in_color;
        return;
    }

    if (in_id < 0 || in_id >= info_faces.size() || in_color == nullptr)
        return;
    
    info_faces[in_id].color = in_color;
}

void Shape::set_edge_color(int in_id, Color* in_color)
{
    if (in_id == ALL_IDs)
    {
        for (auto &e : info_edges)
            e.color = in_color;
        return;
    }
    if (in_id < 0 || in_id >= info_edges.size() || in_color == nullptr)
        return;
    info_edges[in_id].color = in_color;
}

void Shape::set_point_color(int in_id, Color* in_color)
{
    if (in_id == ALL_IDs)
    {
        for (auto &p : info_points)
            p.color = in_color;
        return;
    }
    if (in_id < 0 || in_id >= info_points.size() || in_color == nullptr)
        return;
    info_points[in_id].color = in_color;
}

void Shape::set_textures(int in_id, std::string in_texture)
{
    if (in_id == ALL_IDs)
    {
        for (auto &p : info_faces)
            p.texture_name = in_texture;
        return;
    }
    if (in_id < 0 || in_id >= info_faces.size() || in_texture.empty())
        return;
    info_faces[in_id].texture_name = in_texture;
}

void Shape::add_edges(Color *in_color)
{
    has_edges = true;
    for (auto &f : info_edges)
        f.color = in_color;
}

void Shape::add_points(Color *in_color)
{
    has_points = true;
    for (auto &f : info_points)
        f.color = in_color;
}

void Shape::add_faces(Color *in_color)
{
    has_faces = true;
    for (auto &f : info_faces)
        f.color = in_color;
}

void Shape::add_textures(const std::string in_texture)
{
    uses_texture = true;
    for (auto &f : info_faces)
        f.texture_name = in_texture;
}

void Shape::setup_edges(Color *in_color)
{ }

void Shape::setup_points(Color *in_color)
{ }

Vector3 Shape::get_normal(int in_face_id)
{
    if (in_face_id < 0 || in_face_id >= normals.size())
        return Vector3();

    return normals[in_face_id];
}

// MESH 3D

Mesh3D::Mesh3D(std::filesystem::path in_current_path, std::string in_file_path)
{
    in_current_path = in_current_path / in_file_path;
    load_object(in_current_path.string());
    
    init_buffers();
}

void Mesh3D::load_object(std::string in_path)
{
    std::vector<Point3>  positions;
    std::vector<Vector3> normals_raw;

    std::vector<Point2>  uvs_raw;

    std::ifstream file(in_path);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            positions.push_back(Point3(x, y, z));
        }
        else if (token == "vt")
        {
            float u, v;
            ss >> u >> v;
            uvs_raw.push_back(Point2(u, v));
        }
        else if (token == "vn")
        {
            float x, y, z;
            ss >> x >> y >> z;
            normals_raw.push_back(Vector3(x, y, z));
        }
        else if (token == "f")
        {
            std::vector<std::tuple<int,int,int>> face_verts;

            std::string chunk;
            while (ss >> chunk)
            {
                int vi = 0, vti = 0, vni = 0;
                if (chunk.find("//") != std::string::npos)
                    sscanf(chunk.c_str(), "%d//%d", &vi, &vni);
                else
                    sscanf(chunk.c_str(), "%d/%d/%d", &vi, &vti, &vni);
                face_verts.push_back({vi, vti, vni});
            }

            
            for (int i = 1; i + 1 < (int)face_verts.size(); i++)
            {
                for (int j : {0, i, i+1})
                {
                    auto [vi, vti, vni] = face_verts[j];

                    Point2 uv = (vti > 0) ? uvs_raw[vti-1] : Point2(0,0);

                    vertices.push_back(Vertex(
                        positions[vi-1],
                        normals_raw[vni-1],
                        uv
                    ));
                    
                    indices.push_back(indices.size());
                }
            }
        }
    }
    info_faces.push_back(IndicesInfo(0, indices.size(), GL_TRIANGLES, YES_EBO, &base_color));
}


// CIRCLE

Circle::Circle(const unsigned int& in_points, const float& in_radius)
: n_points(in_points), radius(in_radius), Shape()
{
    create_circle(&base_color);
    setup_edges(&base_color);

    init_buffers();
}

void Circle::setup_edges(Color* in_color) 
{
    int v_count = vertices.size() - 1;

    info_edges.push_back(IndicesInfo(1, v_count, GL_LINE_LOOP, NO_EBO, in_color));
}


void Circle::create_circle(Color *in_color)
{
    float step = 360.0 / float(n_points);
    

    vertices.push_back(Point3(center.x, center.y, 0.0f));

    for (int i = 0; i <= n_points; i++)
    {
        float ang = utils::ang_to_rad(i * step);
        float x = center.x + radius * std::cos(ang);
        float y = center.y + radius * std::sin(ang);
        
        vertices.push_back(Point3(x, y, 0.0f));
    }

    int v_count = vertices.size();
    info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, NO_EBO, in_color));
}


// CIRCULAR SECTOR

CircularSector::CircularSector(const unsigned int& in_points,
                const float& in_start,
                const float& in_end,
                const float& in_radius,
                
                const float& in_ox,
                const float& in_oy):
    n_points(in_points), radius(in_radius), start_angle(in_start), end_angle(in_end),
    Shape()
{
    create_sector(in_ox, in_oy, &base_color);
    setup_edges(&base_color);

    init_buffers();
}

void CircularSector::setup_edges(Color* in_color)
{
    int v_count = vertices.size();

    info_edges.push_back(IndicesInfo(0, v_count, GL_LINE_LOOP, NO_EBO, in_color));
}


void CircularSector::create_sector(const float& in_ox, const float& in_oy, Color *in_color)
{
    float range = end_angle - start_angle;
    float step = range / float(n_points);
    
    vertices.push_back(Point3(in_ox, in_oy, 0.0f));

    for (int i = 0; i <= n_points; i++)
    {
        float ang = utils::ang_to_rad(start_angle+ (i * step));
        float x = in_ox + radius * std::cos(ang);
        float y = in_oy + radius * std::sin(ang);
        
        vertices.push_back(Point3(x, y, 0.0f));
    }

    float mid_angle = start_angle + (end_angle - start_angle) / 2.0f;
    float mid_radius = radius / 2.0f;

    float ang = utils::ang_to_rad(mid_angle);
    center.x = in_ox + mid_radius * std::cos(ang);
    center.y = in_oy + mid_radius * std::sin(ang);



    int v_count = vertices.size();
    info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, NO_EBO, in_color));
}

// RECTANGLE

Rectangle::Rectangle(const float& in_height, const float& in_width)
    : Shape()
{
    create_rectangle(in_height, in_width, &base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Rectangle::setup_edges(Color* in_color)
{
    int v_count = vertices.size();

    info_edges.push_back(IndicesInfo(0, v_count, GL_LINE_LOOP, NO_EBO, in_color));
}

void Rectangle::setup_points(Color* in_color)
{
    int v_count = vertices.size();

    for (int i = 0; i < v_count; i++)
        info_points.push_back(IndicesInfo(i, 1, GL_POINTS, NO_EBO, in_color));
}

void Rectangle::create_rectangle(float in_height, float in_width, Color *in_color)
{
    float x_mid = in_width / 2.0f;
    float y_mid = in_height / 2.0f;


    std::vector<float> l_x = {-x_mid, x_mid, x_mid, -x_mid};
    std::vector<float> l_y = {y_mid, y_mid, -y_mid, -y_mid};
    
    

    for (int i = 0; i < 4; i++)
        vertices.push_back(Point3(l_x[i] + center.x, l_y[i] + center.y, 0.0f));

    int v_count = vertices.size();
    info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, NO_EBO, in_color));
}

// ELIPSE
Elipse::Elipse(const unsigned int& in_points,
               const float& in_height,
               const float& in_width)
{
    create_elipse(in_height, in_width, in_points, &base_color);
    setup_edges(&base_color);

    init_buffers();
}


void Elipse::setup_edges(Color* in_color) 
{
    int v_count = vertices.size()- 1;

    info_edges.push_back(IndicesInfo(1, v_count, GL_LINE_LOOP, NO_EBO, in_color));
}

void Elipse::create_elipse(float in_height, float in_width, int in_points, Color *in_color)
{
    vertices.push_back(Point3(center.x, center.y, 0.0f));

    float step = 360.0 / float(in_points);

    for (int i = 0; i <= in_points; i++)
    {
        float ang_step = utils::ang_to_rad(i * step);

        float x = std::cos(ang_step) * in_width;
        float y = std::sin(ang_step) * in_height;

        vertices.push_back(Point3(center.x + x, center.y + y, 0.0f));
    }

    int v_count = vertices.size();
    info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, NO_EBO, in_color));
}

// PYRAMID
Pyramid::Pyramid(const float& in_height, const float& in_base)
    : Shape(), height(in_height), base(in_base)
{
    create_pyramid(&base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Pyramid::setup_points(Color* in_color) 
{
    int s_indice = indices.size();

    unsigned int top = 0;
    unsigned int fl  = 1;
    unsigned int fr  = 2;
    unsigned int br  = 5;
    unsigned int bl  = 8;

    indices.push_back(top);
    indices.push_back(fl);
    indices.push_back(fr);
    indices.push_back(br);
    indices.push_back(bl);

    for (int i = 0; i < 5; i++)
        info_points.push_back(IndicesInfo(s_indice + i, 1, GL_POINTS, NO_EBO, in_color));
}

void Pyramid::setup_edges(Color* in_color) 
{
    int s_indice = indices.size();

    unsigned int top = 0;
    unsigned int fl  = 1;
    unsigned int fr  = 2;
    unsigned int br  = 5;
    unsigned int bl  = 8;


    indices.push_back(top); indices.push_back(fl);
    indices.push_back(top); indices.push_back(fr);
    indices.push_back(top); indices.push_back(bl);
    indices.push_back(top); indices.push_back(br);

    indices.push_back(fl);  indices.push_back(fr);
    indices.push_back(fr);  indices.push_back(br);
    indices.push_back(br);  indices.push_back(bl);
    indices.push_back(bl);  indices.push_back(fl);

    for (int i = 0; i < 8; i++)
        info_edges.push_back(IndicesInfo(s_indice + (2 * i), 2, GL_LINES, YES_EBO, in_color));
}

void Pyramid::create_pyramid(Color *in_color)
{
    float h = height / 2.0f;
    float b = base  / 2.0f;


    Point3 top(center.x, center.y + h, center.z);
    Point3 fl(center.x - b, center.y - h, center.z + b); // Front-Left
    Point3 fr(center.x + b, center.y - h, center.z + b); // Front-Right
    Point3 br(center.x + b, center.y - h, center.z - b); // Back-Right
    Point3 bl(center.x - b, center.y - h, center.z - b); // Back-Left

    Vector3 v1, v2, normal;
    unsigned int start_idx;

    // TOP, FL, FR - FRONT
    v1 = Vector3(fl.x - top.x, fl.y - top.y, fl.z - top.z);
    v2 = Vector3(fr.x - top.x, fr.y - top.y, fr.z - top.z);
    normal = cross(v1, v2); 
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(fl, normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(0, 3, GL_TRIANGLES, YES_EBO, in_color));

    // TOP - FR - BR - RIGHT
    v1 = fr - top;
    v2 = br - top;
    normal = cross(v1, v2);
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, normal, Point2(0,0)));
    vertices.push_back(Vertex(br, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(3, 3, GL_TRIANGLES, YES_EBO, in_color));

    // BACK - TOP - BR - BL
    v1 = br - top;
    v2 = bl - top;
    normal = cross(v1, v2);
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(br, normal, Point2(0,0)));
    vertices.push_back(Vertex(bl, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(6, 3, GL_TRIANGLES, YES_EBO, in_color));

    // LEFT - TOP - BL - FL
    v1 = bl - top;
    v2 = fl - top;
    normal = cross(v1, v2);
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(bl, normal, Point2(0,0)));
    vertices.push_back(Vertex(fl, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(9, 3, GL_TRIANGLES, YES_EBO, in_color));

    // BASE
    Vector3 down_normal(0.0f, -1.0f, 0.0f);
    start_idx = vertices.size();
    
    vertices.push_back(Vertex(fl, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(br, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, down_normal, Point2(0,0)));
    
    vertices.push_back(Vertex(fl, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(bl, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(br, down_normal, Point2(0,0)));

    indices.push_back(start_idx);
    indices.push_back(start_idx + 1);
    indices.push_back(start_idx + 2);
    indices.push_back(start_idx + 3);
    indices.push_back(start_idx + 4);
    indices.push_back(start_idx + 5);
    
    info_faces.push_back(IndicesInfo(12, 6, GL_TRIANGLES, YES_EBO, in_color));
}

// PYRAMID 3
Pyramid3::Pyramid3(const float& in_height, const float& in_base)
    : Shape(), height(in_height), base(in_base)
{
    create_pyramid(&base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Pyramid3::setup_points(Color* in_color) 
{
    int s_indice = indices.size();

    unsigned int top = 0;
    unsigned int fl  = 1;
    unsigned int fr  = 2;
    unsigned int br  = 5;

    indices.push_back(top);
    indices.push_back(fl);
    indices.push_back(fr);
    indices.push_back(br);

    for (int i = 0; i < 4; i++)
        info_points.push_back(IndicesInfo(s_indice + i, 1, GL_POINTS, NO_EBO, in_color));
}

void Pyramid3::setup_edges(Color* in_color) 
{
    int s_indice = indices.size();

    unsigned int top = 0;
    unsigned int fl = 1;
    unsigned int fr = 2;
    unsigned int br = 5;


    indices.push_back(top); indices.push_back(fl);
    indices.push_back(top); indices.push_back(fr);
    indices.push_back(top); indices.push_back(br);

    indices.push_back(fl);  indices.push_back(fr);
    indices.push_back(fr);  indices.push_back(br);
    indices.push_back(fl);  indices.push_back(fr);

    for (int i = 0; i < 6; i++)
        info_edges.push_back(IndicesInfo(s_indice + (2 * i), 2, GL_LINES, YES_EBO, in_color));
}

void Pyramid3::create_pyramid(Color *in_color)
{
    float h = height / 2.0f;
    float b = base  / 2.0f;


    Point3 top(center.x, center.y + h, center.z);
    Point3 fl(center.x - b, center.y - h, center.z + b); // Front-Left
    Point3 fr(center.x + b, center.y - h, center.z + b); // Front-Right
    Point3 br(center.x + b, center.y - h, center.z - b); // Back-Right

    Vector3 v1, v2, normal;
    unsigned int start_idx;

    // TOP, FL, FR - FRONT
    v1 = Vector3(fl.x - top.x, fl.y - top.y, fl.z - top.z);
    v2 = Vector3(fr.x - top.x, fr.y - top.y, fr.z - top.z);
    normal = cross(v1, v2); 
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(fl, normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(0, 3, GL_TRIANGLES, YES_EBO, in_color));

    // TOP - FR - BR - RIGHT
    v1 = Vector3(fr.x - top.x, fr.y - top.y, fr.z - top.z);
    v2 = Vector3(br.x - top.x, br.y - top.y, br.z - top.z);
    normal = cross(v1, v2);
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, normal, Point2(0,0)));
    vertices.push_back(Vertex(br, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(3, 3, GL_TRIANGLES, YES_EBO, in_color));

    // BACK - TOP - BR - FL
    v1 = top - br;
    v2 = fl - br;
    normal = cross(v1, v2);
    normal = normalize(normal);

    start_idx = vertices.size();
    vertices.push_back(Vertex(top, normal, Point2(0,0)));
    vertices.push_back(Vertex(br, normal, Point2(0,0)));
    vertices.push_back(Vertex(fl, normal, Point2(0,0)));
    indices.push_back(start_idx); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
    info_faces.push_back(IndicesInfo(6, 3, GL_TRIANGLES, YES_EBO, in_color));

    

    // BASE
    Vector3 down_normal(0.0f, -1.0f, 0.0f);
    start_idx = vertices.size();
    
    vertices.push_back(Vertex(fl, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(br, down_normal, Point2(0,0)));
    vertices.push_back(Vertex(fr, down_normal, Point2(0,0)));
    
    indices.push_back(start_idx);
    indices.push_back(start_idx + 1);
    indices.push_back(start_idx + 2);
    
    info_faces.push_back(IndicesInfo(9, 3, GL_TRIANGLES, YES_EBO, in_color));
}

// CUBE

Cube::Cube(const float& in_size):
    Shape(), size(in_size)
{
    create_cube(&base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Cube::setup_points(Color* in_color) 
{
    int v_count = 8;

    for (int i = 0; i < v_count; i++)
        info_points.push_back(IndicesInfo (i, 1, GL_POINTS, NO_EBO, in_color));
}

void Cube::setup_edges(Color* in_color) 
{
    int s_indice = indices.size();
    // Front
    indices.push_back(0); indices.push_back(1);
    indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3);
    indices.push_back(3); indices.push_back(0);

    // Back
    indices.push_back(4); indices.push_back(5);
    indices.push_back(5); indices.push_back(6);
    indices.push_back(6); indices.push_back(7);
    indices.push_back(7); indices.push_back(4);

    // Right
    indices.push_back(1); indices.push_back(5);
    indices.push_back(2); indices.push_back(6);

    // Left
    indices.push_back(0); indices.push_back(4);
    indices.push_back(3); indices.push_back(7);

    for (int i = 0; i < 12; i++)
        info_edges.push_back(IndicesInfo (s_indice + (2 * i), 2, GL_LINES, YES_EBO, in_color));
}

void Cube::create_cube(Color* in_color)
{
    float s = size / 2.0f;

    //    7   6
    //  3   2
    //
    //    4   5
    //  0   1

    // Front
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z + s), Vector3(0, 0, 1), Point2(0, 0))); // 0 FL        [0]
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z + s), Vector3(0, 0, 1), Point2(1, 0))); // 1 FR        [1]
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z + s), Vector3(0, 0, 1), Point2(1, 1))); // 2 TR        [2]
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z + s), Vector3(0, 0, 1), Point2(0, 1))); // 3 TL        [3]

    normals.push_back(Vector3(0, 0, 1));

    // Back
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z - s), Vector3(0, 0, -1), Point2(0, 0))); // 4 BL        [4]
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z - s), Vector3(0, 0, -1), Point2(1, 0))); // 5 BR        [5]
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z - s), Vector3(0, 0, -1), Point2(1, 1))); // 6 TR back   [7]
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z - s), Vector3(0, 0, -1), Point2(0, 1))); // 7 TL back   [6]

    normals.push_back(Vector3(0, 0, -1));

    // Left
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z - s), Vector3(-1, 0, 0), Point2(0, 0))); // 4 BL        [8]
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z + s), Vector3(-1, 0, 0), Point2(1, 0))); // 0 FL        [9]
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z + s), Vector3(-1, 0, 0), Point2(1, 1))); // 3 TL        [10]
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z - s), Vector3(-1, 0, 0), Point2(0, 1))); // 7 TL back   [11]

    normals.push_back(Vector3(-1, 0, 0));

    // Right
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z + s), Vector3(1, 0, 0), Point2(0, 0))); // 1 FR
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z - s), Vector3(1, 0, 0), Point2(1, 0))); // 5 BR
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z - s), Vector3(1, 0, 0), Point2(1, 1))); // 6 TR back
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z + s), Vector3(1, 0, 0), Point2(0, 1))); // 2 TR

    normals.push_back(Vector3(1, 0, 0));

    // Top
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z + s), Vector3(0, 1, 0), Point2(0, 0))); // 3 TL
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z + s), Vector3(0, 1, 0), Point2(1, 0))); // 2 TR
    vertices.push_back(Vertex(Point3(center.x + s, center.y + s, center.z - s), Vector3(0, 1, 0), Point2(1, 1))); // 6 TR back
    vertices.push_back(Vertex(Point3(center.x - s, center.y + s, center.z - s), Vector3(0, 1, 0), Point2(0, 1))); // 7 TL back

    normals.push_back(Vector3(0, 1, 0));

    // Down
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z + s), Vector3(0, -1, 0), Point2(0, 0))); // 0 FL
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z + s), Vector3(0, -1, 0), Point2(1, 0))); // 1 FR
    vertices.push_back(Vertex(Point3(center.x + s, center.y - s, center.z - s), Vector3(0, -1, 0), Point2(1, 1))); // 5 BR
    vertices.push_back(Vertex(Point3(center.x - s, center.y - s, center.z - s), Vector3(0, -1, 0), Point2(0, 1))); // 4 BL

    normals.push_back(Vector3(0, -1, 0));


    // INDICES

    // Front
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);
    info_faces.push_back(IndicesInfo(0, 6, GL_TRIANGLES, YES_EBO, in_color));

    // Back
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(4); indices.push_back(6); indices.push_back(7);
    info_faces.push_back(IndicesInfo(6, 6, GL_TRIANGLES, YES_EBO, in_color));

    // Left
    indices.push_back(8); indices.push_back(9); indices.push_back(10);
    indices.push_back(8); indices.push_back(10); indices.push_back(11);
    info_faces.push_back(IndicesInfo(12, 6, GL_TRIANGLES, YES_EBO, in_color));

    // Right
    indices.push_back(12); indices.push_back(13); indices.push_back(14);
    indices.push_back(12); indices.push_back(14); indices.push_back(15);
    info_faces.push_back(IndicesInfo(18, 6, GL_TRIANGLES, YES_EBO, in_color));

    // Top
    indices.push_back(16); indices.push_back(17); indices.push_back(18);
    indices.push_back(16); indices.push_back(18); indices.push_back(19);
    info_faces.push_back(IndicesInfo(24, 6, GL_TRIANGLES, YES_EBO, in_color));

    // Bottom
    indices.push_back(20); indices.push_back(21); indices.push_back(22);
    indices.push_back(20); indices.push_back(22); indices.push_back(23);
    info_faces.push_back(IndicesInfo(30, 6, GL_TRIANGLES, YES_EBO, in_color));
}

// CONE

Cone::Cone(const unsigned int& in_points,
        const float& in_height,
        const float& in_radius):
    Shape(), height(in_height), radius(in_radius), points(in_points)
{
    create_cone(&base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Cone::setup_edges(Color* in_color) 
{
    int s_indice = indices.size();

    unsigned int top_idx = 0; 

    for (unsigned int i = 1; i <= points; i++) 
    {
        // Top to base
        indices.push_back(top_idx);
        indices.push_back(i);

        // line in circle
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    info_edges.push_back(IndicesInfo(s_indice, points * 2 * 2, GL_LINES, YES_EBO, in_color));
}

void Cone::setup_points(Color* in_color) 
{
    info_points.push_back(IndicesInfo(0, 1, GL_POINTS, NO_EBO, in_color));
}

void Cone::create_cone(Color* in_color)
{
    float h = height / 2.0f;
    float step = 360.0f / float(points);

    // Cuerpo lateral
    unsigned int lateral_start_idx = vertices.size();
    vertices.push_back(Vertex(Point3(center.x, center.y + h, center.z), Vector3(0, 1, 0), Point2(0,0))); // Punta

    for (int i = 0; i <= points; i++)
    {
        float ang = utils::ang_to_rad(i * step);
        float cos_a = std::cos(ang);
        float sin_a = std::sin(ang);

        float x = center.x + radius * cos_a;
        float z = center.z + radius * sin_a;

        float r_over_h = radius / height; 
        Vector3 norm_lateral(cos_a, r_over_h, sin_a);
        norm_lateral = normalize(norm_lateral);

        vertices.push_back(Vertex(Point3(x, center.y - h, z), norm_lateral, Point2(0,0)));
    }

    // Laterales indices
    for (unsigned int i = 1; i <= points; i++)
    {
        indices.push_back(lateral_start_idx);
        indices.push_back(lateral_start_idx + i);
        indices.push_back(lateral_start_idx + i + 1);
    }
    info_faces.push_back(IndicesInfo(0, points * 3, GL_TRIANGLES, YES_EBO, in_color));


    // BASE
    unsigned int base_start_idx = vertices.size();
    Vector3 down_normal(0, -1, 0);

    // Centro de la base
    vertices.push_back(Vertex(Point3(center.x, center.y - h, center.z), down_normal, Point2(0,0)));

    // Vertices de la base, normal hacia abajo
    for (int i = 0; i <= points; i++)
    {
        float ang = utils::ang_to_rad(i * step);
        float x = center.x + radius * std::cos(ang);
        float z = center.z + radius * std::sin(ang);
        vertices.push_back(Vertex(Point3(x, center.y - h, z), down_normal, Point2(0,0)));
    }

    // Indices base
    unsigned int base_start_index_buffer = indices.size();
    unsigned int center_vertex_pos = base_start_idx;

    for (unsigned int i = 1; i <= points; i++)
    {
        indices.push_back(center_vertex_pos);
        indices.push_back(center_vertex_pos + i + 1);
        indices.push_back(center_vertex_pos + i);
    }
    info_faces.push_back(IndicesInfo(base_start_index_buffer, points * 3, GL_TRIANGLES, YES_EBO, in_color));
}
// SPHERE

Sphere::Sphere(const unsigned int& in_points,
        const float& in_radius):
    Shape(), points(in_points), radius(in_radius)
{
    create_sphere(&base_color);
    setup_edges(&base_color);
    setup_points(&base_color);

    init_buffers();
}

void Sphere::setup_edges(Color* in_color) 
{
    int v_count = vertices.size();
    int step = v_count / 6;

    int start = 0;
    for (int i = 0; i < 5; i++)
    {
        info_edges.push_back(IndicesInfo(start, step, GL_LINE_LOOP, NO_EBO, in_color));
        start += step;
    }

    info_edges.push_back(IndicesInfo(start, (v_count - start), GL_LINE_LOOP, NO_EBO, in_color));
}

void Sphere::setup_points(Color* in_color) 
{
    int v_count = vertices.size();
    int step = v_count / 6;

    int start = 0;
    for (int i = 0; i < 5; i++)
    {
        info_points.push_back(IndicesInfo(start, step, GL_POINTS, NO_EBO, in_color));
        start += step;
    }

    info_points.push_back(IndicesInfo(start, (v_count - start), GL_POINTS, NO_EBO, in_color));
}

void Sphere::create_sphere(Color* in_color)
{
    float stack_step  = 180.0f / float(points);
    float sector_step = 360.0f / float(points);

    for (int i = 0; i <= points; i++)
    {
        float cur_stack_ang  = utils::ang_to_rad(-90.0f + (i * stack_step));
        float cos_stack = std::cos(cur_stack_ang);
        float sin_stack = std::sin(cur_stack_ang);

        for (int j = 0; j <= points; j++)
        {
            float cur_sector_ang = utils::ang_to_rad(j * sector_step);
            float cos_sector = std::cos(cur_sector_ang);
            float sin_sector = std::sin(cur_sector_ang);

            float x = (radius * cos_stack) * cos_sector;
            float y = (radius * cos_stack) * sin_sector;
            float z = radius * sin_stack;

            Point3 pos(x + center.x, y + center.y, z + center.z);
            Vector3 normal(cos_stack * cos_sector, cos_stack * sin_sector, sin_stack);

            vertices.push_back(Vertex(pos, normal, Point2(0, 0)));
        }
    }

    // Face info
    for (unsigned int i = 0; i < points; i++)
    {
        unsigned int k1 = i * (points + 1);
        unsigned int k2 = (i + 1) * (points + 1);

        for (unsigned int j = 0; j < points; j++)
        {
            indices.push_back(k1 + j); indices.push_back(k2 + j); indices.push_back(k1 + 1 + j);
            indices.push_back(k1 + 1 + j); indices.push_back(k2 + j); indices.push_back(k2 + 1 + j);   
        }
    }

    int section_num = 8;
    int total_size = indices.size() / 3; 
    int batch_size = total_size / section_num;

    for (int i = 0; i < section_num - 1; i++)
        info_faces.push_back(IndicesInfo(i * batch_size * 3, batch_size * 3, GL_TRIANGLES, YES_EBO, in_color));

    info_faces.push_back(IndicesInfo((section_num - 1) * batch_size * 3, (total_size - ((section_num - 1) * batch_size)) * 3, GL_TRIANGLES, YES_EBO, in_color));
}