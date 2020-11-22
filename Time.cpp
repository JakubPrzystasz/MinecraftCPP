#include "Time.h"

void Time::init(GLdouble time)
{
	previousTime = time;
	currentTime = time;
	lastSecond = time;
	lastFrame = time;
}

void Time::update(GLdouble time)
{
	currentTime = time;
	deltaTime = currentTime - previousTime;
	if((currentTime - lastSecond) >= 0.25){
		lastSecond = currentTime;
		FPS = counter * 4;
		counter = 0;
	}
}

bool Time::renderFrame()
{
	if ((currentTime - lastFrame) >= frameTime){
		counter++;
		lastFrame = currentTime;
		return true;
	}
	return false;
}
