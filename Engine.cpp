#include "Engine.h"

Engine* Engine::instance = nullptr;

void Engine::updateWindow()
{

	timer.update(glfwGetTime());
	input->Update(window);

	camera.ProcessMouseMovement(input->GetMouseOffset());

	if (camera.Position.x < 0)
		onBlockPosition.x = 1 + (static_cast<GLfloat>(camera.Position.x) - static_cast<int>(camera.Position.x));
	else
		onBlockPosition.x = static_cast<GLfloat>(camera.Position.x) - static_cast<int>(camera.Position.x);


	if (camera.Position.y < 0)
		onBlockPosition.y = 1 + (static_cast<GLfloat>(camera.Position.y) - static_cast<int>(camera.Position.y));
	else
		onBlockPosition.y = static_cast<GLfloat>(camera.Position.y) - static_cast<int>(camera.Position.y);


	if (camera.Position.z < 0)
		onBlockPosition.z = 1 + (static_cast<GLfloat>(camera.Position.z) - static_cast<int>(camera.Position.z));
	else
		onBlockPosition.z = static_cast<GLfloat>(camera.Position.z) - static_cast<int>(camera.Position.z);


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

	if (input->IsKeyDown(Key::KEY_O))
		showDebugData = !showDebugData;

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
	
	if((lastPosition - chunkPos) != vec2(0,0))
		world->SetRenderedChunks(chunkPos);

	lastPosition = chunkPos;

	std::stringstream STRING;
	STRING << "FPS: " << timer.FPS << "   Active jobs:" << world->GetJobsCount();
	DebugData[0] = STRING.str();
	std::stringstream().swap(STRING);
	STRING << "Player position (X,Y,Z): " <<
		std::fixed << std::setprecision(1) << camera.Position.x << ", " <<
		std::fixed << std::setprecision(1) << camera.Position.y << ", " <<
		std::fixed << std::setprecision(1) << camera.Position.z;
	DebugData[1] = STRING.str();
	std::stringstream().swap(STRING);
	STRING << "Chunk (X,Y): " << chunkPos.x << ", " << chunkPos.y << "      " << "Generated chunks: " << world->GetChunksCount() << "   " << " Direction: " << static_cast<char>(camera.GetLookDirection());
	DebugData[2] = STRING.str();
	std::stringstream().swap(STRING);
	STRING << "On block position (X,Y,Z): " <<
		std::fixed << std::setprecision(1) << onBlockPosition.x << ", " <<
		std::fixed << std::setprecision(1) << onBlockPosition.y << ", " <<
		std::fixed << std::setprecision(1) << onBlockPosition.z << "  Standing on: " << (int)world->GetBlock(vec3(camera.Position) + vec3(0, -2, 0));
	DebugData[3] = STRING.str();
}

void Engine::renderFrame()
{
	world->DrawChunks(world->GetChunkPosition(camera.Position));

	if (showDebugData) {
		int line = 0;
		for (std::string& str : DebugData) {
			text.RenderText(str, static_cast<float>(5), static_cast<float>(585-(line++*25)), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		}
	}

	crossHair.Draw();
}

void Engine::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Engine::Engine()
{
	camera = Camera();
	polygonRenderMode = true;
	showDebugData = false;
	screenHeight = 600;
	screenWidth = 800;
	crossHairSize = 8;
	onBlockPosition = vec3(0,0,0);
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

	camera.SetScreenRatio(vec2(screenHeight, screenWidth));

	ProjectionMatrix = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));

	DebugData[0] = std::string("");
	DebugData[1] = std::string("");
	DebugData[2] = std::string("");
	DebugData[3] = std::string("");

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
	rs->AddShadingProgram("text", "Shaders/text.vert", "Shaders/text.frag");
	rs->AddShadingProgram("crossHair", "Shaders/crosshair.vert", "Shaders/crosshair.frag");

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
	text.Init(ProjectionMatrix);

	//Crosshair
	{
		crossHair = Model();
		crossHair.Init();
		crossHair.SetShadingProgram(rs->GetShadingProgram("crossHair"));
		crossHair.shadingProgram->Use();
		crossHair.shadingProgram->SetData("projection", ProjectionMatrix);
		crossHair.AddTexture("corssHairTexture", rs->GetTexture("Textures/crosshair.png"));
		GLuint ind[6] = { 1, 2, 3, // right bottom -> left bottom -> left top 
					  0, 1, 3 }; // right top -> right bottom -> left top};
		crossHair.AddIndices(ind, 6);

		//calculate pixels in center of screen
		vec3 center = vec3(static_cast<GLfloat>(screenWidth) / 2, static_cast<GLfloat>(screenHeight) / 2, 0.f);
		crossHair.AddVertex(Vertex((center + vec3(crossHairSize, crossHairSize, 0)), glm::vec2(1.0f, 1.0f))); //right top
		crossHair.AddVertex(Vertex((center + vec3(crossHairSize, -crossHairSize, 0)), glm::vec2(1.0f, 0.0f)));//right bottom
		crossHair.AddVertex(Vertex((center + vec3(-crossHairSize, -crossHairSize, 0)), glm::vec2(0.0f, 0.0f)));//left bottom
		crossHair.AddVertex(Vertex((center + vec3(-crossHairSize, crossHairSize, 0)), glm::vec2(0.0f, 1.0f)));//left top
		crossHair.BindData();
	}
	//Crosshair

	world = World::GetInstance();
	world->SetCamera(&camera);
	world->SetChunkSize(16);
	world->StartThreads();
	world->SetRenderDistance(1);
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