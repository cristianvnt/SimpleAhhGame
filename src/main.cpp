#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Logger.h"

#include <iostream>
#include <string>

// Settings
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

float DeltaTime = 0.f;
float LastFrame = 0.f;

// Functions
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void ClearScreen();
void FPS(GLFWwindow* window);

void DrawCircle(float x, float y, float radius);

const char* vertexShaderSrc = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.f);\n"
"}\n\0";


const char* fragmentShaderSrc = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.4f, 0.3f, 1.0f);\n"
"}\n\0";


int main()
{
	// Init GLFW and set hints
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window and set the context to it
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SimpleAhhGame", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// VSync disabled
	glfwSwapInterval(0);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD";
		return -1;
	}

	// Shaders
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GL_CHECK(glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr));
	GL_CHECK(glCompileShader(vertexShader));
	LOG::LogShader(vertexShader, ShaderType::VERTEX);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GL_CHECK(glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr));
	GL_CHECK(glCompileShader(fragmentShader));
	LOG::LogShader(fragmentShader, ShaderType::FRAGMENT);

	unsigned int programShader = glCreateProgram();
	GL_CHECK(glAttachShader(programShader, vertexShader));
	GL_CHECK(glAttachShader(programShader, fragmentShader));
	GL_CHECK(glLinkProgram(programShader));
	LOG::LogProgram(programShader);

	GL_CHECK(glDeleteShader(vertexShader));
	GL_CHECK(glDeleteShader(fragmentShader));

	// Data set up
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	unsigned int vao, vbo;
	GL_CHECK(glGenVertexArrays(1, &vao));
	GL_CHECK(glGenBuffers(1, &vbo));

	GL_CHECK(glBindVertexArray(vao));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_CHECK(glEnableVertexAttribArray(0));
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0));

	// Unbind
	GL_CHECK(glBindVertexArray(0));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// Wireframe mode
	GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		DeltaTime = currentFrame - LastFrame;
		LastFrame = currentFrame;

		FPS(window);

		// Process input and clear screen's color buffer
		ProcessInput(window);
		ClearScreen();

		// Render
		GL_CHECK(glUseProgram(programShader));
		glBindVertexArray(vao);
		GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

		// Swap the buffers and poll I/O events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GL_CHECK(glViewport(0, 0, width, height));
}

void ClearScreen()
{
	GL_CHECK(glClearColor(0.5f, 0.5f, 0.7f, 1.f));
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void FPS(GLFWwindow* window)
{
	static float timerSec = 0.f;
	static int fpsCount = 0;

	timerSec += DeltaTime;
	fpsCount++;

	if (timerSec >= 0.1f)
	{
		int avgFPS = static_cast<int>(fpsCount / timerSec);
		std::string title = "SimpleAhhGame - FPS: " + std::to_string(avgFPS);
		glfwSetWindowTitle(window, title.c_str());

		timerSec = 0.f;
		fpsCount = 0;
	}
}

void DrawCircle(float x, float y, float radius)
{
	return;
}
