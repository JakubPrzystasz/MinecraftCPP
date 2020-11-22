#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include "Key.h"

/// <summary>
/// Structure representing mouse position on screen
/// </summary>
struct MousePosition
{
	double x;
	double y;
};


/// <summary>
/// Singleton class that processes input from window
/// </summary>
class Input
{
private:
	static std::map<Key, bool> keyState;
	static std::map<Key, bool> previousState;
	static MousePosition mousePos;
	Input();
	static Input* instance;
public:

    Input(Input& other) = delete;

    void operator=(const Input&) = delete;
	
	static Input* GetInstance();

	static void Update(GLFWwindow *window);

	//Return true when was pressed
	static bool IsKeyDown(Key key);

	//Return true when key was released
	static bool IsKeyUp(Key key);

	/// <summary>
	/// Returns true when key is being held
	/// </summary>
	static bool GetKeyState(Key key);

	static MousePosition GetMousePos();

	friend class Engine;
};
