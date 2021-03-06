#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;		// the program ID

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath); // Constructor reads and builds the shader
	void use(); // use/active the shader
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;
	void setVec3(const std::string &name, glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
};
#endif // SHADER_H