#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <fstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: _rendererID(0)
{
	ShaderProgramSource source = ParseShader(vertexPath, fragmentPath);
	std::cout << source.VertexSource << source.FragmentSource;
	_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(_rendererID);
}

ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::fstream vShaderFile;
	std::fstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& err)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << err.what() << "\n";
	}

	return { vertexCode.c_str(), fragmentCode.c_str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program;
	GL_CHECK(program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GL_CHECK(glAttachShader(program, vs));
	GL_CHECK(glAttachShader(program, fs));
	GL_CHECK(glLinkProgram(program));
	GL_CHECK(glValidateProgram(program));

	GL_CHECK(glDeleteShader(vs));
	GL_CHECK(glDeleteShader(fs));

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id;
	GL_CHECK(id = glCreateShader(type));
	const char* src = source.c_str();
	GL_CHECK(glShaderSource(id, 1, &src, nullptr));
	GL_CHECK(glCompileShader(id));
	LOG::LogShader(id, type);

	return id;
}

void Shader::Bind() const
{
	GL_CHECK(glUseProgram(_rendererID));
}

void Shader::Unbind() const
{
	GL_CHECK(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name)
{
	// Cache location for faster location retrieval
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		return _uniformLocationCache[name];

	unsigned int location;
	GL_CHECK(location = glGetUniformLocation(_rendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!\n";

	_uniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniformB(const std::string& name, bool value)
{
	GL_CHECK(glUniform1i(GetUniformLocation(name), (int)value));
}

void Shader::SetUniformI(const std::string& name, int value)
{
	GL_CHECK(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformF(const std::string& name, float value)
{
	GL_CHECK(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	GL_CHECK(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
	GL_CHECK(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::SetUniformVec2(const std::string& name, float x, float y)
{
	GL_CHECK(glUniform2f(GetUniformLocation(name), x, y));
}

void Shader::SetUniformVec2(const std::string& name, const glm::vec2& value)
{
	GL_CHECK(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::SetUniformVec3(const std::string& name, float x, float y, float z)
{
	GL_CHECK(glUniform3f(GetUniformLocation(name), x, y, z));
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value)
{
	GL_CHECK(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::SetUniformVec4(const std::string& name, float x, float y, float z, float w)
{
	GL_CHECK(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformVec4(const std::string& name, const glm::vec4& value)
{
	GL_CHECK(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::SetUniformMat2fv(const std::string& name, const glm::mat2& mat)
{
	GL_CHECK(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::SetUniformMat3fv(const std::string& name, const glm::mat3& mat)
{
	GL_CHECK(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::SetUniformMat4fv(const std::string& name, const glm::mat4& mat)
{
	GL_CHECK(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}
