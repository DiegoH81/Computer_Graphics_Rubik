#ifndef SHADER_LIST_H
#define SHADER_LIST_H

/*
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
*/


#include <vector>
#include <map>
#include <string>
#include "matrix.h"

class ShaderList
{
public:
    ShaderList();

    void create_vertex_shader(const char *vertexShaderSource);

    void add_fragment_shader(const std::string& shader_name, const char *fragment_Shader_Source);

    void delete_shaders();

    void use_shader(const std::string& in_shader);

    void delete_programs();

    void set_vec2(const std::string& shader_name, const std::string& uniform_name, float x, float y);

    void set_vec3(const std::string& shader_name, const std::string& uniform_name, float x, float y, float z);

    void set_mat4(const std::string& shader_name, const std::string& uniform_name, const Matrix_4& in_matrix);

    unsigned int get_cur_prog(const std::string& shader_name);

    void set_float(const std::string& shader_name, const std::string& uniform_name, float val);

    void set_texture(const std::string& shader_name, const std::string& uniform_name, int slot);

    void set_bool(const std::string& shader_name, const std::string& uniform_name, bool in_bool);

private:
    unsigned int VERTEX;
    std::map<std::string, unsigned int> shader_programs;
};


#endif