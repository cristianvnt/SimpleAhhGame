#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Logger.h"
#include "Math.h"

#include <iostream>
#include <string>

// Settings
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

float DeltaTime = 0.f;
float LastFrame = 0.f;

// Functions
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void ClearScreen();
void FPS(GLFWwindow* window);

void CircleCoords(std::vector<glm::vec3>& coords);

const char* vertexShaderSrc = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * vec4(aPos, 1.f);\n"
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
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SimpleAhhGame", nullptr, nullptr);
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

	// Data set up
	std::vector<glm::vec3> circleCoords;
	CircleCoords(circleCoords);

	unsigned int vao, vbo;
	GL_CHECK(glGenVertexArrays(1, &vao));
	GL_CHECK(glGenBuffers(1, &vbo));

	GL_CHECK(glBindVertexArray(vao));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, circleCoords.size() * sizeof(glm::vec3), circleCoords.data(), GL_STATIC_DRAW));

	GL_CHECK(glEnableVertexAttribArray(0));
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));

	// Unbind
	GL_CHECK(glBindVertexArray(0));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// Shaders
	Shader shader("resources/shaders/shader.vert", "resources/shaders/shader.frag");

	// Wireframe mode
	GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

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
		shader.Bind();
		glBindVertexArray(vao);
		// Orthogonal view
		float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
		glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f);
		shader.SetUniformMat4fv("projection", projection);
		GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, circleCoords.size()));

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

void CircleCoords(std::vector<glm::vec3>& coords)
{
	float radius = 0.3f;
	const int steps = 120;
	const float angle = PI * 2.f / steps;

	coords.emplace_back(glm::vec3(0.f, 0.f, 0.f));

	for (int i = 0; i <= steps; i++)
	{
		float theta = i * angle;
		float newX = radius * glm::cos(theta);
		float newY = radius * glm::sin(theta);

		coords.emplace_back(glm::vec3(newX, newY, 0.f));
	}
}