#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	// Callbacks
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD";
		return -1;
	}

	//GL_CHECK(glEnable(GL_DEPTH_TEST));
	
	// VSync disabled
	glfwSwapInterval(0);

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