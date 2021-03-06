#pragma once
#include <GLFW/glfw3.h>

class Time {
public:
	GLdouble previousTime;
	GLdouble currentTime;
	GLdouble deltaTime;
	GLdouble lastSecond;
	GLuint FPS;
	GLuint counter;
	GLdouble frameTime;
	GLdouble lastFrame;
	GLdouble lastClick;
	GLdouble lastFootstepSound;
	void init(GLdouble time);
	void update(GLdouble time);
	bool renderFrame();
	bool click();
	bool footstep();
};