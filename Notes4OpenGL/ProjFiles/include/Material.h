#pragma once
#include "Shader.h"
#include "Texture.h"
#include <iostream>
#include <glm/glm.hpp>

class Material
{
public:
    Material(Texture& diffuse, Texture& specular, Texture& emissionMap, float shininess=32.0f):
        diffuse(diffuse), specular(specular), emissionMap(emissionMap), shininess(shininess) {}
    void SetValue(Shader& shader, std::string matName="material")
    {
        shader.setVec3((matName + ".ambient").c_str(), glm::vec3(0.0f, 0.1f, 0.06f));
        shader.setInt(matName + ".diffuse", diffuse.texIdx);
        shader.setInt(matName + ".specular", specular.texIdx);
        shader.setInt(matName + ".emissionMap", emissionMap.texIdx);
        shader.setFloat(matName + ".shininess", shininess);
    }
private:
    Texture diffuse, specular, emissionMap;
    float shininess;
};
