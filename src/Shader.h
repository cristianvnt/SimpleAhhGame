#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Logger.h"

#include <iostream>
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int _rendererID;
	std::unordered_map<std::string, int> _uniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	// Bind/Unbind the shader
	void Bind() const;
	void Unbind() const;

	// Utility uniform func
	int GetUniformLocation(const std::string& name);

	void SetUniformB(const std::string& name, bool value);
	void SetUniformI(const std::string& name, int value);
	void SetUniformF(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniform4f(const std::string& name, const glm::vec4& value);
	void SetUniformVec2(const std::string& name, float x, float y);
	void SetUniformVec2(const std::string& name, const glm::vec2& value);
	void SetUniformVec3(const std::string& name, float x, float y, float z);
	void SetUniformVec3(const std::string& name, const glm::vec3& value);
	void SetUniformVec4(const std::string& name, float x, float y, float z, float w);
	void SetUniformVec4(const std::string& name, const glm::vec4& value);
	void SetUniformMat2fv(const std::string& name, const glm::mat2& mat);
	void SetUniformMat3fv(const std::string& name, const glm::mat3& mat);
	void SetUniformMat4fv(const std::string& name, const glm::mat4& mat);
};

#endif // SHADER_H