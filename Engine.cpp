#include "Engine.h"

Engine* Engine::instance = nullptr;

void Engine::updateWindow()
{

	timer.update(glfwGetTime());
	input->Update(window);

	camera.ProcessMouseMovement(input->GetMouseOffset());

	if (input->GetKeyState(Key::KEY_W))
		camera.ProcessKeyboard(CameraMovement::FORWARD, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_A))
		camera.ProcessKeyboard(CameraMovement::LEFT, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_S))
		camera.ProcessKeyboard(CameraMovement::BACKWARD, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_D))
		camera.ProcessKeyboard(CameraMovement::RIGHT, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_LEFT_SHIFT))
		camera.ProcessKeyboard(CameraMovement::DOWN, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_SPACE))
		camera.ProcessKeyboard(CameraMovement::UP, (GLfloat)timer.deltaTime);

	if (input->IsKeyDown(Key::KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (input->IsKeyDown(Key::KEY_I)) {
		polygonRenderMode = !polygonRenderMode;
		if (!polygonRenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (input->IsKeyDown(Key::KEY_G))
	{
		int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
		switch (inputMode) {
		case GLFW_CURSOR_DISABLED:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case GLFW_CURSOR_NORMAL:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		}
	}

	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		world->SetBlock(camera.Position, BlockName::Cobble);
	}

	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		world->SetBlock(camera.Position, BlockName::Air);
	}

	auto chunkPos = world->GetChunkPosition(camera.Position);
	world->SetRenderedChunks(chunkPos);


	auto __tmp__ = world->GetChunk(chunkPos);
	if (__tmp__ == nullptr)
		return;
	auto __x__ = World::ToChunkPosition(camera.Position);
	auto __y__ = __tmp__->ToWorldPosition(__x__);

	if (timer.printDebug()) {
		system("cls");
		std::cout << "FPS: " << timer.FPS << std::endl <<
			"x: " << camera.Position.x <<
			" y: " << camera.Position.y <<
			" z: " << camera.Position.z << std::endl <<
			"x: " << __x__.x <<
			" y: " << __x__.y <<
			" z: " << __x__.z << std::endl <<
			"x: " << __y__.x <<
			" y: " << __y__.y <<
			" z: " << __y__.z << std::endl <<
			"Chunk: " << chunkPos.x << " " << chunkPos.y << std::endl;
	}

}

void Engine::renderFrame()
{
	world->DrawChunks(camera);
	//crossHair.Draw();
	//text.RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
}

void Engine::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Engine::InitializeWindow(GLuint width, GLuint height, const std::string title)
{
	if (instance != nullptr) {
		std::cout << "ERROR::ENGINE::INSTANCE EXIST" << std::endl;
	}

	instance = this;
	this->screenWidth = width;
	this->screenHeight = height;
	this->windowTitle = title;

	//Make random seed 
	srand((unsigned int)time(NULL));

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERROR::GLFW::UNABLE TO INITIALIZE" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(screenWidth, screenHeight, windowTitle.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::FAILED TO INITAILZE" << std::endl;
		glfwDestroyWindow(window);
		return;
	}

	///Window resize callback
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	///Mouse mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Console output precision
	std::cout << std::fixed << std::setprecision(2);

	//Load shaders
	rs->AddShadingProgram("block", "Shaders/block.vert", "Shaders/block.frag");
	rs->AddShadingProgram("crossHair", "Shaders/crosshair.vert", "Shaders/crosshair.frag");
	rs->AddShadingProgram("text", "Shaders/text.vert", "Shaders/text.frag");

	//Add blocks
	//Grass
	rs->AddBlock(BlockName::Grass, { 3,15 }, { 3,15 }, { 0,0 }, { 2,15 }, { 3,15 }, { 3,15 });
	rs->GetBlock(BlockName::Grass)->BindFaces();
	//Dirt
	rs->AddBlock(BlockName::Dirt, { 2,15 }, { 2,15 }, { 2,15 }, { 2,15 }, { 2,15 }, { 2,15 });
	rs->GetBlock(BlockName::Dirt)->BindFaces();
	//Stone
	rs->AddBlock(BlockName::Stone, { 1,15 }, { 1,15 }, { 1,15 }, { 1,15 }, { 1,15 }, { 1,15 });
	rs->GetBlock(BlockName::Stone)->BindFaces();
	//Cobble
	rs->AddBlock(BlockName::Cobble, { 0,2 }, { 0,14 }, { 0,14 }, { 0,14 }, { 0,14 }, { 1,10 });
	rs->GetBlock(BlockName::Cobble)->BindFaces();

	//Text
	text.Init();

	//Crosshair
	glEnable(GL_BLEND);
	crossHair = Model();
	crossHair.Init();
	crossHair.SetShadingProgram(rs->GetShadingProgram("crossHair"));
	crossHair.shadingProgram->Use();
	crossHair.AddTexture("tex", rs->GetTexture("Textures/crossHair.png"));
	crossHair.shadingProgram->SetData("corssHairTexture", crossHair.Textures["tex"]->GetId());
	GLuint ind[6] = { 1, 2, 3, // right bottom -> left bottom -> left top 
				  0, 1, 3 }; // right top -> right bottom -> left top};
	crossHair.AddIndices(ind, 6);
	crossHair.AddVertex(Vertex(glm::vec3(0.02, 0.03, 0), glm::vec2(1.0f, 1.0f))); //right top
	crossHair.AddVertex(Vertex(glm::vec3(0.02, -0.03, 0), glm::vec2(1.0f, 0.0f)));//right bottom
	crossHair.AddVertex(Vertex(glm::vec3(-0.02, -0.03, 0), glm::vec2(0.0f, 0.0f)));//left bottom
	crossHair.AddVertex(Vertex(glm::vec3(-0.02, 0.03, 0), glm::vec2(0.0f, 1.0f)));//left top
	crossHair.BindData();
	//Crosshair

	world = World::GetInstance();
	world->SetChunkSize(16);
	world->StartThreads();
	world->SetRenderedChunks(vec2(0, 0));
}


void Engine::WindowLoop()
{
	if (window) {

		/// <summary>
		/// Render loop, set timer 
		/// </summary>
		glfwSetTime(0.f);
		timer.init(glfwGetTime());
		SetFrameRate(60);
		glClearColor(0.678f, 0.847f, 0.902f, 1.07f);
		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window))
		{
			/// <summary>
			/// Update internal clock, process input
			/// </summary>
			updateWindow();

			/// <summary>
			/// Render single frame
			/// </summary>
			if (timer.renderFrame())
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderFrame();
				/// Draw recently rendered frame on window,
				glfwSwapBuffers(window);
			}
			/// <summary>
			/// Poll all recently occured events
			/// </summary>	
			glfwPollEvents();
		}
		world->StopThreads();
	}
}

void Engine::SetFrameRate(GLuint fps)
{
	timer.frameTime = (GLfloat)((GLfloat)1.0f / (GLfloat)fps);
}

Engine::~Engine() {
	ResourceManager* rs = ResourceManager::GetInstance();
	rs->FreeResources();
	glfwTerminate();
}