#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture(const char* path, GLenum texture, GLenum format);
    Texture(const char* path, GLenum texture);

private:
    unsigned int texId;
    void loadTexture(const char* filepath, GLenum format);
    unsigned int loadTexture(char const * path);
};
