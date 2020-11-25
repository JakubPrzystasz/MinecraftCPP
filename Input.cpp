#include "Input.h"

Input* Input::instance = nullptr;
std::map<Key, bool> Input::keyState;
std::map<Key, bool> Input::previousState;
MousePosition Input::mousePos;
MousePosition Input::previousMousePos;
bool Input::MouseButton_1;
bool Input::MouseButton_2;

Input* Input::GetInstance()
{
    if (instance == nullptr) {
        instance = new Input();
    }
    return instance;
}

void Input::Update(GLFWwindow *window)
{
	Input::previousState = Input::keyState;
	/// <summary>
	/// Check if key has changed state
	/// </summary>
	for (auto& key : Input::keyState)
	{
		key.second = glfwGetKey(window, (int)key.first) == GLFW_PRESS ? true : false;
	}
	/// <summary>
	/// Get position of mouse
	/// </summary>
	MouseButton_1 = glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? true : false;
	MouseButton_2 = glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ? true : false;
	
	previousMousePos = mousePos;
	if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		glfwGetCursorPos(window, &Input::mousePos.x, &Input::mousePos.y);
}

bool Input::IsKeyDown(Key key)
{
	return !previousState.at(key) && keyState.at(key) ? true : false;
}

bool Input::IsKeyUp(Key key)
{
	return previousState.at(key) && !keyState.at(key) ? true : false;
}

bool Input::GetKeyState(Key key)
{
	return keyState.at(key);
}

MousePosition Input::GetMousePos()
{
	return mousePos;
}

MousePosition Input::GetMouseOffset()
{
	MousePosition tmp;
	tmp.x = mousePos.x - previousMousePos.x;
	tmp.y = previousMousePos.y - mousePos.y;
	return tmp;
}

bool Input::MouseButton1()
{
	return MouseButton_1;
}

bool Input::MouseButton2()
{
	return MouseButton_2;
}

Input::Input() {

	keyState.insert(std::make_pair(Key::KEY_UKNNOWN, false));
	keyState.insert(std::make_pair(Key::KEY_SPACE, false));
	keyState.insert(std::make_pair(Key::KEY_APOSTROPHE, false));
	keyState.insert(std::make_pair(Key::KEY_COMMA, false));
	keyState.insert(std::make_pair(Key::KEY_MINUS, false));
	keyState.insert(std::make_pair(Key::KEY_PERIOD, false));
	keyState.insert(std::make_pair(Key::KEY_SLASH, false));
	keyState.insert(std::make_pair(Key::KEY_0, false));
	keyState.insert(std::make_pair(Key::KEY_1, false));
	keyState.insert(std::make_pair(Key::KEY_2, false));
	keyState.insert(std::make_pair(Key::KEY_3, false));
	keyState.insert(std::make_pair(Key::KEY_4, false));
	keyState.insert(std::make_pair(Key::KEY_5, false));
	keyState.insert(std::make_pair(Key::KEY_6, false));
	keyState.insert(std::make_pair(Key::KEY_7, false));
	keyState.insert(std::make_pair(Key::KEY_8, false));
	keyState.insert(std::make_pair(Key::KEY_9, false));
	keyState.insert(std::make_pair(Key::KEY_SEMICOLON, false));
	keyState.insert(std::make_pair(Key::KEY_EQUAL, false));
	keyState.insert(std::make_pair(Key::KEY_A, false));
	keyState.insert(std::make_pair(Key::KEY_B, false));
	keyState.insert(std::make_pair(Key::KEY_C, false));
	keyState.insert(std::make_pair(Key::KEY_D, false));
	keyState.insert(std::make_pair(Key::KEY_E, false));
	keyState.insert(std::make_pair(Key::KEY_F, false));
	keyState.insert(std::make_pair(Key::KEY_G, false));
	keyState.insert(std::make_pair(Key::KEY_H, false));
	keyState.insert(std::make_pair(Key::KEY_I, false));
	keyState.insert(std::make_pair(Key::KEY_J, false));
	keyState.insert(std::make_pair(Key::KEY_K, false));
	keyState.insert(std::make_pair(Key::KEY_L, false));
	keyState.insert(std::make_pair(Key::KEY_M, false));
	keyState.insert(std::make_pair(Key::KEY_N, false));
	keyState.insert(std::make_pair(Key::KEY_O, false));
	keyState.insert(std::make_pair(Key::KEY_P, false));
	keyState.insert(std::make_pair(Key::KEY_Q, false));
	keyState.insert(std::make_pair(Key::KEY_R, false));
	keyState.insert(std::make_pair(Key::KEY_S, false));
	keyState.insert(std::make_pair(Key::KEY_T, false));
	keyState.insert(std::make_pair(Key::KEY_U, false));
	keyState.insert(std::make_pair(Key::KEY_V, false));
	keyState.insert(std::make_pair(Key::KEY_W, false));
	keyState.insert(std::make_pair(Key::KEY_X, false));
	keyState.insert(std::make_pair(Key::KEY_Y, false));
	keyState.insert(std::make_pair(Key::KEY_Z, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT_BRACKET, false));
	keyState.insert(std::make_pair(Key::KEY_BACKSLASH, false));
	keyState.insert(std::make_pair(Key::KEY_RIGHT_BRACKET, false));
	keyState.insert(std::make_pair(Key::KEY_GRAVE_ACCENT, false));
	keyState.insert(std::make_pair(Key::KEY_WORLD_1, false));
	keyState.insert(std::make_pair(Key::KEY_WORLD_2, false));
	keyState.insert(std::make_pair(Key::KEY_ESCAPE, false));
	keyState.insert(std::make_pair(Key::KEY_TAB, false));
	keyState.insert(std::make_pair(Key::KEY_BACKSPACE, false));
	keyState.insert(std::make_pair(Key::KEY_INSERT, false));
	keyState.insert(std::make_pair(Key::KEY_DELETE, false));
	keyState.insert(std::make_pair(Key::KEY_RIGHT, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT, false));
	keyState.insert(std::make_pair(Key::KEY_DOWN, false));
	keyState.insert(std::make_pair(Key::KEY_UP, false));
	keyState.insert(std::make_pair(Key::KEY_PAGE_UP, false));
	keyState.insert(std::make_pair(Key::KEY_PAGE_DOWN, false));
	keyState.insert(std::make_pair(Key::KEY_END, false));
	keyState.insert(std::make_pair(Key::KEY_CAPSLOCK, false));
	keyState.insert(std::make_pair(Key::KEY_SCROLLOCK, false));
	keyState.insert(std::make_pair(Key::KEY_NUMLOCK, false));
	keyState.insert(std::make_pair(Key::KEY_PRINTSCREEN, false));
	keyState.insert(std::make_pair(Key::KEY_PAUSE, false));
	keyState.insert(std::make_pair(Key::KEY_F1, false));
	keyState.insert(std::make_pair(Key::KEY_F2, false));
	keyState.insert(std::make_pair(Key::KEY_F3, false));
	keyState.insert(std::make_pair(Key::KEY_F4, false));
	keyState.insert(std::make_pair(Key::KEY_F5, false));
	keyState.insert(std::make_pair(Key::KEY_F6, false));
	keyState.insert(std::make_pair(Key::KEY_F7, false));
	keyState.insert(std::make_pair(Key::KEY_F8, false));
	keyState.insert(std::make_pair(Key::KEY_F9, false));
	keyState.insert(std::make_pair(Key::KEY_F10, false));
	keyState.insert(std::make_pair(Key::KEY_F11, false));
	keyState.insert(std::make_pair(Key::KEY_F12, false));
	keyState.insert(std::make_pair(Key::KEY_F13, false));
	keyState.insert(std::make_pair(Key::KEY_F14, false));
	keyState.insert(std::make_pair(Key::KEY_F15, false));
	keyState.insert(std::make_pair(Key::KEY_F16, false));
	keyState.insert(std::make_pair(Key::KEY_F17, false));
	keyState.insert(std::make_pair(Key::KEY_F18, false));
	keyState.insert(std::make_pair(Key::KEY_F19, false));
	keyState.insert(std::make_pair(Key::KEY_F20, false));
	keyState.insert(std::make_pair(Key::KEY_F21, false));
	keyState.insert(std::make_pair(Key::KEY_F22, false));
	keyState.insert(std::make_pair(Key::KEY_F23, false));
	keyState.insert(std::make_pair(Key::KEY_F24, false));
	keyState.insert(std::make_pair(Key::KEY_F25, false));
	keyState.insert(std::make_pair(Key::KEY_KP_0, false));
	keyState.insert(std::make_pair(Key::KEY_KP_1, false));
	keyState.insert(std::make_pair(Key::KEY_KP_2, false));
	keyState.insert(std::make_pair(Key::KEY_KP_3, false));
	keyState.insert(std::make_pair(Key::KEY_KP_4, false));
	keyState.insert(std::make_pair(Key::KEY_KP_5, false));
	keyState.insert(std::make_pair(Key::KEY_KP_6, false));
	keyState.insert(std::make_pair(Key::KEY_KP_7, false));
	keyState.insert(std::make_pair(Key::KEY_KP_8, false));
	keyState.insert(std::make_pair(Key::KEY_KP_9, false));
	keyState.insert(std::make_pair(Key::KEY_KP_DECIMAL, false));
	keyState.insert(std::make_pair(Key::KEY_KP_DIVIDE, false));
	keyState.insert(std::make_pair(Key::KEY_KP_MULTIPLY, false));
	keyState.insert(std::make_pair(Key::KEY_KP_SUBTRACT, false));
	keyState.insert(std::make_pair(Key::KEY_KP_ADD, false));
	keyState.insert(std::make_pair(Key::KEY_KP_ENTER, false));
	keyState.insert(std::make_pair(Key::KEY_KP_EQUAL, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT_SHIFT, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT_CONTROL, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT_ALT, false));
	keyState.insert(std::make_pair(Key::KEY_LEFT_SUPER, false));
	keyState.insert(std::make_pair(Key::KEY_RIGHT_SHIFT, false));
	keyState.insert(std::make_pair(Key::KEY_RIGHT_CONTROL, false));
	keyState.insert(std::make_pair(Key::KEY_RIGHT_ALT, false));
	keyState.insert(std::make_pair(Key::KEY_MENU, false));

	previousState = keyState;

	mousePos.x = 0.0;
	mousePos.y = 0.0;
	previousMousePos = mousePos;

}