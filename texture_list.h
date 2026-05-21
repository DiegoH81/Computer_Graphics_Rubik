#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include <string>
#include <map>

class TextureList
{
public:
    TextureList();

    void add_texture(const std::string& texture_name, const std::string& texture_path);
    void use_texture(const std::string& texture_name, int slot);
private:
    std::map<std::string, unsigned int> textures;
};

#endif