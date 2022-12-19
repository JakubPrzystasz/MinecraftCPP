#include "Engine.h"

int main() {

	Engine engine = Engine();
	engine.InitializeWindow(1240, 700, "Minecraft++");
	engine.WindowLoop();
	
	return 0;
}

