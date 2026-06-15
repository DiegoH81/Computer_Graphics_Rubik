#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader_list.h"
#include "light.h"

ShaderList::ShaderList(std::filesystem::path in_current_path):
    shader_programs(), current_path(in_current_path)
{
    current_path = current_path / "shaders";
}

void ShaderList::create_shader_path(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path)
{
    // Vertex Shader
    auto new_path =  current_path  / vertex_path;
    auto shader_source_vrtx = read_shader_source(new_path.string());
    const char* const_src_vrtx = shader_source_vrtx.c_str();

    unsigned int VERTEX = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VERTEX, 1, &const_src_vrtx, nullptr);
    glCompileShader(VERTEX);

    // Fragment Shader
    new_path =  current_path  / fragment_path;
    auto shader_source_frgmt = read_shader_source(new_path.string());
    const char* const_src_frgmt = shader_source_frgmt.c_str();

    unsigned int fragment_source_shader = 0;
    fragment_source_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_source_shader, 1, &const_src_frgmt, nullptr);
    glCompileShader(fragment_source_shader);


    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    
    glAttachShader(shader_program, VERTEX);
    glAttachShader(shader_program, fragment_source_shader);
    glLinkProgram(shader_program);
    
    glDeleteShader(VERTEX);
    glDeleteShader(fragment_source_shader);

    shader_programs[shader_name] = shader_program;

}

void ShaderList::create_shader(const std::string& shader_name, const char * vertex_source, const char * fragment_source)
{
    // Vertex Shader

    unsigned int VERTEX = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VERTEX, 1, &vertex_source, nullptr);
    glCompileShader(VERTEX);

    // Fragment Shader
    
    unsigned int fragment_source_shader = 0;
    fragment_source_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_source_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_source_shader);


    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    
    glAttachShader(shader_program, VERTEX);
    glAttachShader(shader_program, fragment_source_shader);
    glLinkProgram(shader_program);
    
    glDeleteShader(VERTEX);
    glDeleteShader(fragment_source_shader);

    shader_programs[shader_name] = shader_program;

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
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform2f(uniform, x, y);
}

void ShaderList::set_vec3(const std::string& shader_name, const std::string& uniform_name, float x, float y, float z)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform3f(uniform, x, y, z);
}

void ShaderList::set_mat4(const std::string& shader_name, const std::string& uniform_name, const Matrix_4& in_matrix)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);
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
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform1f(uniform, val);
}

void ShaderList::set_int(const std::string& shader_name, const std::string& uniform_name, int val)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());

    glUniform1i(uniform, val);
}

void ShaderList::set_texture(const std::string& shader_name, const std::string& uniform_name, int slot)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform1i(uniform, slot);
}

void ShaderList::set_bool(const std::string& shader_name, const std::string& uniform_name, bool in_bool)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);
    int uniform = glGetUniformLocation(current_program, uniform_name.c_str());
    glUniform1i(uniform, in_bool? 1 : 0);
}

void ShaderList::set_material(const std::string& shader_name, const std::string& uniform_name, Material* in_material)
{
    unsigned int current_program = shader_programs[shader_name];
    glUseProgram(current_program);

    int ambient = glGetUniformLocation(current_program, (uniform_name + ".ambient").c_str());
    int diffuse = glGetUniformLocation(current_program, (uniform_name + ".diffuse").c_str());
    int specular = glGetUniformLocation(current_program, (uniform_name + ".specular").c_str());
    int shiny = glGetUniformLocation(current_program, (uniform_name + ".shininess").c_str());
    int alpha_value = glGetUniformLocation(current_program, (uniform_name + ".alpha_value").c_str());

    glUniform3f(ambient, in_material->ambient.x, in_material->ambient.y, in_material->ambient.z);
    glUniform3f(diffuse, in_material->diffuse.x, in_material->diffuse.y, in_material->diffuse.z);
    glUniform3f(specular, in_material->specular.x, in_material->specular.y, in_material->specular.z);
    glUniform1f(shiny, in_material->shininess);
    glUniform1f(alpha_value, in_material->alpha_value);
}

std::string ShaderList::read_shader_source(const std::string& source_path)
{
    std::ifstream file(source_path, std::ios::binary | std::ios::ate);
    
    if (!file.is_open())
    {
        std::cout << "Error opening file (SHADER): " << source_path << std::endl;
        return "";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string to_return_content(size, '\0');
    
    if (file.read(&to_return_content[0], size))
        return to_return_content;

    return "";
}