#ifndef SHADER_LIST_H
#define SHADER_LIST_H

/*
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
*/


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <cmath>

#include "matrix.h"
#include "material.h"
#include "utils.h"

class Light; // forward declaration

class ShaderList
{
public:
    ShaderList(std::filesystem::path in_current_path);

    void create_shader_path(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path);

    void create_shader(const std::string& shader_name, const char * vertex_source, const char * fragment_source);

    void use_shader(const std::string& in_shader);

    void delete_programs();

    void set_vec2(const std::string& shader_name, const std::string& uniform_name, float x, float y);

    void set_vec3(const std::string& shader_name, const std::string& uniform_name, float x, float y, float z);

    void set_mat4(const std::string& shader_name, const std::string& uniform_name, const Matrix_4& in_matrix);

    unsigned int get_cur_prog(const std::string& shader_name);

    void set_float(const std::string& shader_name, const std::string& uniform_name, float val);

    void set_int(const std::string& shader_name, const std::string& uniform_name, int val);

    void set_texture(const std::string& shader_name, const std::string& uniform_name, int slot);

    void set_bool(const std::string& shader_name, const std::string& uniform_name, bool in_bool);

    void set_material(const std::string& shader_name, const std::string& uniform_name, Material* in_material);

private:
    std::map<std::string, unsigned int> shader_programs;
    std::filesystem::path current_path;

    std::string read_shader_source(const std::string& source_path);
};


#endif