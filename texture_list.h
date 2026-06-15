#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include <string>
#include <map>
#include <filesystem>

class TextureList
{
public:
    TextureList(std::filesystem::path in_current_path);

    void add_texture(const std::string& texture_name, const std::string& texture_path);
    void use_texture(const std::string& texture_name, int slot);
private:
    std::map<std::string, unsigned int> textures;
    std::filesystem::path current_path;
};

#endif