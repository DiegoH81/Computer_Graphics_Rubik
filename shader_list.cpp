#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader_list.h"

ShaderList::ShaderList():
    VERTEX(0), shader_programs() {}


void ShaderList::create_vertex_shader(const char *vertexShaderSource)
{
    VERTEX = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VERTEX, 1, &vertexShaderSource, nullptr);
    glCompileShader(VERTEX);
}

void ShaderList::add_fragment_shader(const std::string& shader_name, const char *fragment_Shader_Source)
{
    unsigned int fragment_source_shader = 0;
    fragment_source_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_source_shader, 1, &fragment_Shader_Source, nullptr);
    glCompileShader(fragment_source_shader);


    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    
    glAttachShader(shader_program, VERTEX);
    glAttachShader(shader_program, fragment_source_shader);
    glLinkProgram(shader_program);
    

    glDeleteShader(fragment_source_shader);

    shader_programs[shader_name] = shader_program;
}

void ShaderList::delete_shaders()
{
    glDeleteShader(VERTEX);
}

void ShaderList::use_shader(const std::string& in_shader)
{
    glUseProgram(shader_programs[in_shader]);
}

void ShaderList::delete_programs()
{
    for (auto &i: shader_programs)
        glDeleteProgram(i.second);
}

void ShaderList::set_vec2(const std::string& shader_name, const std::string& uniform_name, float x, float y)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform2f(uniform, x, y);
}

void ShaderList::set_vec3(const std::string& shader_name, const std::string& uniform_name, float x, float y, float z)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform3f(uniform, x, y, z);
}

void ShaderList::set_mat4(const std::string& shader_name, const std::string& uniform_name, const Matrix_4& in_matrix)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniformMatrix4fv(uniform, 1, GL_TRUE, in_matrix.matrix.data());
}

unsigned int ShaderList::get_cur_prog(const std::string& shader_name)
{
    return shader_programs[shader_name];
}

void ShaderList::set_float(const std::string& shader_name, const std::string& uniform_name, float val)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());

    glUniform1f(uniform, val);
}

void ShaderList::set_texture(const std::string& shader_name, const std::string& uniform_name, int slot)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform1i(uniform, slot);
}

void ShaderList::set_bool(const std::string& shader_name, const std::string& uniform_name, bool in_bool)
{
    unsigned int current_program = shader_programs[shader_name];
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform1i(uniform, in_bool? 1 : 0);
}