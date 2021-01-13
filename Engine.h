#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm> 
#include <glm/glm.hpp>
#include "ResourceManager.h"
#include "Input.h"
#include "Time.h"
#include "Camera.h"
#include "World.h"

class Engine
{
private:

	/// <summary>
	/// Prevent from creating multiple instances of 
	/// engine in one process
	/// </summary>
	static Engine* instance;

protected:

	/// <summary>
	/// Main window
	/// </summary>
	GLFWwindow* window;

	/// <summary>
	/// Width of screen and viewport
	/// </summary>
	GLuint screenWidth;

	/// <summary>
	/// Height of screen and viewport
	/// </summary>
	GLuint screenHeight;

	/// <summary>
	/// Title of main program window
	/// </summary>
	std::string windowTitle;

	/// <summary>
	/// Processes user input 
	/// </summary>
	void updateWindow();

	/// <summary>
	/// Renders single frame for window
	/// </summary>
	void renderFrame();

	/// <summary>
	/// Window resize callback function
	/// </summary>
	static void windowSizeCallback(GLFWwindow* window, int width, int height);

	World* world;

	Model crossHair;

	int crossHairSize;

	Text text;

	Camera camera;

	glm::mat4 ProjectionMatrix;

	vec2 posDelta;

	bool polygonRenderMode;

	bool showDebugData;

	std::array<std::string, 5> DebugData;

	//collisions
	vec3 onBlockPosition;
	vec2 lastPosition;

public:
	Engine();

	/// <summary>
	/// Process input
	/// </summary>
	Input* input = Input::GetInstance();

	/// <summary>
	/// Compute Delta time, frames, time based actions
	/// </summary>
	Time timer;

	/// <summary>
	/// Resources
	/// </summary>
	ResourceManager* rs = ResourceManager::GetInstance();

	/// <summary>
	/// This initializes required libraries,
	/// OpenGL version 3.3 and creates all buffers
	/// </summary>
	void InitializeWindow(GLuint width, GLuint height, std::string title);

	/// <summary>
	/// Main loop for render and input processing
	/// </summary>
	void WindowLoop();

	/// <summary>
	/// Set frame rate of window
	/// </summary>
	void SetFrameRate(GLuint fps);

	/// Free all resources, and GL buffers
	~Engine();
};

