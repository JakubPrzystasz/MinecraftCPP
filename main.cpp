#include "Engine.h"

int main() {

	Engine engine = Engine();
	engine.InitializeWindow(933, 700, "Minecraft");
	engine.WindowLoop();
}

