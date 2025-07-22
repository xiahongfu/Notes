#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_vec.hpp>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void deleteProgram();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const GLchar* name, glm::mat4& value) const;
	void setVec3(const GLchar* name, glm::vec3& value) const;
	void setVec3(const GLchar* name, glm::vec3&& value) const;
	int getUniformLocation(const std::string& name) const;
};
