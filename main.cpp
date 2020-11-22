#include "Engine.h"

int main() {

	Engine engine = Engine();
	engine.InitializeWindow(800, 450, "Minecraft");
	engine.WindowLoop();
}

