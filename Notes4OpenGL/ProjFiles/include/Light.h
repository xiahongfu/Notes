#pragma once
#include <string>
#include "Shader.h"
#include <iostream>
#include <glm/glm.hpp>

class PointLight
{
public:
    PointLight(glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float distance=-1.0f)
        : lightColor(lightColor), lightPos(lightPos), ambient(ambient), diffuse(diffuse), specular(specular)
    {
        constant = 1.0f;
        if (distance <= 0.0f)
        {
            linear = 0.0f;
            quadratic = 0.0f;
        } else if (distance <= 7)
        {
            linear = 0.7f;
            quadratic = 1.8f;
        } else if (distance <= 13)
        {
            linear = 0.35f;
            quadratic = 0.44f;
        } else if (distance <= 20)
        {
            linear = 0.22f;
            quadratic = 0.20f;
        } else if (distance <= 32)
        {
            linear = 0.14f;
            quadratic = 0.07f;
        } else if (distance <= 50)
        {
            linear = 	0.09f;
            quadratic = 0.032f;
        } else
        {
            linear = 	0.07f;
            quadratic = 0.017f;
        }
    }
    void SetValue(Shader& shader, std::string lightName = "light")
    {
        shader.setVec3((lightName + ".color").c_str(), lightColor);
        shader.setVec3((lightName + ".position").c_str(), lightPos);
        shader.setVec3((lightName + ".ambient").c_str(), ambient);
        shader.setVec3((lightName + ".diffuse").c_str(), diffuse);
        shader.setVec3((lightName + ".specular").c_str(), specular);
        
        shader.setFloat(lightName + ".constant", constant);
        shader.setFloat(lightName + ".linear", linear);
        shader.setFloat(lightName + ".quadratic", quadratic);
    }
private:
    glm::vec3 lightColor;
    glm::vec3 lightPos;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

class DirectionalLight
{
public:
    DirectionalLight(glm::vec3 lightColor, glm::vec3 lightDir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : lightColor(lightColor), lightDir(lightDir), ambient(ambient), diffuse(diffuse), specular(specular)
    {}
    void SetValue(Shader& shader, std::string lightName = "light")
    {
        shader.setVec3((lightName + ".color").c_str(), lightColor);
        shader.setVec3((lightName + ".direction").c_str(), lightDir);
        shader.setVec3((lightName + ".ambient").c_str(), ambient);
        shader.setVec3((lightName + ".diffuse").c_str(), diffuse);
        shader.setVec3((lightName + ".specular").c_str(), specular);
    }
private:
    glm::vec3 lightColor;
    glm::vec3 lightDir;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Spotlight
{
public:
    Spotlight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection,
        glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff)
        : lightColor(lightColor), lightPosition(lightPosition), lightDirection(lightDirection),
        ambient(ambient), diffuse(diffuse), specular(specular),
        cutOff(glm::cos(glm::radians(cutOff))), outerCutOff(glm::cos(glm::radians(outerCutOff)))
    {}
    void SetValue(Shader& shader, std::string lightName = "light")
    {
        shader.setVec3((lightName + ".color").c_str(), lightColor);
        shader.setVec3((lightName + ".position").c_str(), lightPosition);
        shader.setVec3((lightName + ".direction").c_str(), lightDirection);
        shader.setVec3((lightName + ".ambient").c_str(), ambient);
        shader.setVec3((lightName + ".diffuse").c_str(), diffuse);
        shader.setVec3((lightName + ".specular").c_str(), specular);
        shader.setFloat(lightName + ".cutOff", cutOff);
        shader.setFloat(lightName + ".outerCutOff", outerCutOff);
    }
private:
    glm::vec3 lightColor;
    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float cutOff;
    float outerCutOff;
};