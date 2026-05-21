#ifndef INDICES_INFO_H
#define INDICES_INFO_H


/*
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
*/


#include <string>
#include "color.h"


class IndicesInfo
{
public:
    unsigned int start_indice, count, draw_mode;
    Color* color;
    bool uses_EBO;
    std::string texture_name;

    IndicesInfo(unsigned int in_start_indice,
                unsigned int in_indices_count,
                unsigned int in_draw_mode,
                bool in_uses_EBO, 
                Color* in_color,
                const std::string& in_texture_name = ""):
        start_indice(in_start_indice),
        count(in_indices_count),
        draw_mode(in_draw_mode),
        uses_EBO(in_uses_EBO),
        color(in_color),
        texture_name(in_texture_name)
    {}
};

#endif