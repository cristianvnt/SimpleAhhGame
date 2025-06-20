#ifndef LOGGER_H
#define LOGGER_H

#include <glad/glad.h>

enum class ShaderType
{
	VERTEX = GL_VERTEX_ARRAY,
	FRAGMENT = GL_FRAGMENT_SHADER
};

namespace LOG
{
	void LogShader(unsigned int shader, ShaderType type);
	void LogProgram(unsigned int programID);
}

namespace ERR
{
	void glClearError();
	bool glCheckError_(const char* func, const char* file, int line);
	constexpr const char* GetGLErrorString(GLenum err);
}

#define GL_ASSERT(x) \
	do { \
		if (!(x)) \
		__debugbreak(); \
	} while (false)

#define GL_CHECK(x) \
	do { \
		ERR::glClearError(); \
		x; \
		GL_ASSERT(ERR::glCheckError_(#x, __FILE__, __LINE__)); \
	} while (false)

#endif