#include "Time.h"

void Time::init(GLdouble time)
{
	previousTime = time;
	currentTime = time;
	lastSecond = time;
	lastFrame = time;
	lastClick = time;
}

void Time::update(GLdouble time)
{
	previousTime = currentTime;
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


bool Time::click()
{
	if ((currentTime - lastClick) >= 0.2f) {
		lastClick = currentTime;
		return true;
	}
	return false;
}

bool Time::footstep()
{
	if ((currentTime - lastFootstepSound) >= 0.4f) {
		lastFootstepSound = currentTime;
		return true;
	}
	return false;
}
