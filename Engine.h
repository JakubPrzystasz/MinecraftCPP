#pragma once
#pragma warning( disable : 4005 )
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include "ResourceManager.h"
#include "Input.h"
#include "Time.h"

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
	GLuint screenWidth = 800;

	/// <summary>
	/// Height of screen and viewport
	/// </summary>
	GLuint screenHeight = 600;

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

	static void windowSizeCallback(GLFWwindow* window, int width, int height);

	Model* model;
public:
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
	void InitializeWindow(GLuint width, GLuint height,std::string title);

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

