#include "Engine.h"

Engine* Engine::instance = nullptr;

vec3 Engine::CalculateOnBlockPosition(vec3 pos)
{
	vec3 ret = vec3(0, 0, 0);

	if (pos.x < 0)
		ret.x = 1 + (static_cast<GLfloat>(pos.x) - static_cast<int>(pos.x));
	else
		ret.x = static_cast<GLfloat>(pos.x) - static_cast<int>(pos.x);


	if (pos.y < 0)
		ret.y = 1 + (static_cast<GLfloat>(pos.y) - static_cast<int>(pos.y));
	else
		ret.y = static_cast<GLfloat>(pos.y) - static_cast<int>(pos.y);


	if (pos.z < 0)
		ret.z = 1 + (static_cast<GLfloat>(pos.z) - static_cast<int>(pos.z));
	else
		ret.z = static_cast<GLfloat>(pos.z) - static_cast<int>(pos.z);

	return ret;
}

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

	if (input->IsKeyDown(Key::KEY_SPACE)) {
		if((static_cast<GLfloat>(timer.currentTime) - camera.JumpStart) > 0.2f && world->GetBlock(camera.Position + glm::vec3(0,-2,0)) != BlockName::Air) 
			camera.JumpStart = static_cast<GLfloat>(timer.currentTime);
	}

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

	if (!world->worldGenerated)
		return;


	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		world->SetBlock(camera.Position, BlockName::Cobble);
	}

	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		world->SetBlock(camera.Position, BlockName::Air);
	}

	ProcessMovement();

	//Chunk render
	auto chunkPos = world->GetChunkPosition(camera.Position);
	if ((lastPosition - chunkPos) != vec2(0, 0))
		world->SetRenderedChunks(chunkPos);
	lastPosition = chunkPos;

	//Debug data
	auto onBlockPos = CalculateOnBlockPosition(camera.Position);
	auto playerPosDelta = camera.GetPositionDelta();
	DebugData.clear();
	std::stringstream STRING;
	STRING << "FPS: " << timer.FPS << "   Active jobs:" << world->GetJobsCount();
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Player position (X,Y,Z): " <<
		std::fixed << std::setprecision(1) << camera.Position.x << ", " <<
		std::fixed << std::setprecision(1) << camera.Position.y << ", " <<
		std::fixed << std::setprecision(1) << camera.Position.z;
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Chunk (X,Y): " << chunkPos.x << ", " << chunkPos.y << "   " << " Look Direction: " << static_cast<char>(camera.GetLookDirection());
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "On block position (X,Y,Z): " <<
		std::fixed << std::setprecision(2) << onBlockPos.x << ", " <<
		std::fixed << std::setprecision(2) << onBlockPos.y << ", " <<
		std::fixed << std::setprecision(2) << onBlockPos.z << "  Standing on: " << Cube::GetBlockName(world->GetBlock(vec3(camera.Position) + vec3(0, -2, 0)));
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Player pos delta: "
		 << playerPosDelta.x << ", "
		 << playerPosDelta.y << ", "
		 << playerPosDelta.z;
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Generated chunks: " << world->GetChunksCount() << "    Built meshes: " << world->GetMeshCount();
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Ray: ";
	DebugData.push_back(STRING.str());
}

void Engine::renderFrame()
{
	if (!world->worldGenerated) {
		text.RenderText("Generating world...", static_cast<float>(320), static_cast<float>(300), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		GLuint x = world->GetChunksCount();
		if (world->GetChunksCount() >= world->GetPlatformSize(world->GetRenderDistance() + world->GetChunkOffset())) {
			world->BuildMesh();
			if (world->GetMeshCount() >= world->GetPlatformSize(world->GetRenderDistance() + world->GetChunkOffset())) {
				world->SetRenderedChunks(vec2(0, 0));
				world->worldGenerated = true;
			} else
				return;
		}
		return;
	}

	world->DrawChunks(camera);
	crossHair.Draw();
	if (showDebugData) {
		int line = 0;
		for (std::string& str : DebugData) {
			text.RenderText(str, static_cast<float>(5), static_cast<float>(585 - (line++ * 25)), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		}
	}
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
	RenderDistance = 10;
	ChunkSize = 4;
	ChunkOffset = 10;
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

	glEnable(GL_DEPTH_TEST);

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
	rs->AddBlock(BlockName::Cobble, { 0,14 }, { 0,14 }, { 0,14 }, { 0,14 }, { 0,14 }, { 0,14 });
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
	world->SetChunkSize(ChunkSize);
	world->SetChunkOffset(ChunkOffset);
	world->SetRenderDistance(RenderDistance);
	world->StartThreads();
	world->GenerateWorld();

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
		glClearColor(0.678f, 0.847f, 0.902f, 1.f);
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

void Engine::ProcessMovement()
{
	if (world->GetBlock(camera.Position + glm::vec3(0, -2, 0)) == BlockName::Air)
		camera.Position += glm::vec3(0, -7 * static_cast<GLfloat>(timer.deltaTime), 0);

	if (timer.currentTime - camera.JumpStart < 0.2f) 
		camera.Position += glm::vec3(0, 12 * static_cast<GLfloat>(timer.deltaTime), 0);

	//Calculate collisions
	auto playerPosDelta = camera.Position - camera.LastPosition;
	//Standing on block:
	if (playerPosDelta.y < 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(0, -2, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.y = camera.LastPosition.y;
	}
	//Flying up:
	if (playerPosDelta.y > 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(0, 1, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.y = camera.LastPosition.y;
	}

	//adjacent blocks:
	if (playerPosDelta.z < 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(0, 0, -1);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.z = camera.LastPosition.z;
		blockPos = vec3(camera.LastPosition) + vec3(0, -1, -1);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.z = camera.LastPosition.z;
	}
	//Flying up:
	if (playerPosDelta.z > 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(0, 0, 1);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.z = camera.LastPosition.z;
		blockPos = vec3(camera.LastPosition) + vec3(0, -1, 1);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.z = camera.LastPosition.z;
	}

	if (playerPosDelta.x < 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(-1, 0, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.x = camera.LastPosition.x;
		blockPos = vec3(camera.LastPosition) + vec3(-1, -1, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.x = camera.LastPosition.x;
	}
	//Flying up:
	if (playerPosDelta.x > 0) {
		auto blockPos = vec3(camera.LastPosition) + vec3(1, 0, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.x = camera.LastPosition.x;
		blockPos = vec3(camera.LastPosition) + vec3(1, -1, 0);
		if (world->GetBlock(blockPos) != BlockName::Air)
			camera.Position.x = camera.LastPosition.x;
	}

	camera.LastPosition = camera.Position;
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