#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture_list.h"

TextureList::TextureList():
    textures()
{}

void TextureList::add_texture(const std::string& texture_name, const std::string& texture_path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);

    printf("Loading texture from: %s\n", texture_path.c_str());
    if (!data)
    {
        printf("STB error: %s\n", stbi_failure_reason());
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