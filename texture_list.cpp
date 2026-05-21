#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <filesystem>

#include "texture_list.h"

TextureList::TextureList():
    textures()
{}

void TextureList::add_texture(const std::string& texture_name, const std::string& texture_path)
{
    std::filesystem::path current_path = std::filesystem::current_path();
	current_path = current_path.parent_path();
    current_path = current_path / "ownProjects" / "rubik" / "textures" / texture_path;
    //std::cout << "Ruta actual: "<< current_path << "\n";

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(current_path.string().c_str(), &width, &height, &nrChannels, 0);

    std::cout << "Loading textures from: " << current_path << "\n";
    if (!data)
    {
        std::cout << "STB error: " << stbi_failure_reason() << "\n";
        return;
    }

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    textures[texture_name] = texture;
}

void TextureList::use_texture(const std::string& texture_name, int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    if (textures.find(texture_name) != textures.end())
    {
        unsigned int texture = textures[texture_name];
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}