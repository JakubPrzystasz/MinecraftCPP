#include "Engine.h"

int main() {

	Engine engine = Engine();
	engine.InitializeWindow(800, 600, "Minecraft");
	engine.WindowLoop();
	
	return 0;
}

