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

	camera.LastPosition = camera.Position;

	if (input->GetKeyState(Key::KEY_W))
		camera.ProcessKeyboard(CameraMovement::FORWARD, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_A))
		camera.ProcessKeyboard(CameraMovement::LEFT, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_S))
		camera.ProcessKeyboard(CameraMovement::BACKWARD, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_D))
		camera.ProcessKeyboard(CameraMovement::RIGHT, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_LEFT_SHIFT) && flyMode)
		camera.ProcessKeyboard(CameraMovement::DOWN, (GLfloat)timer.deltaTime);

	if (input->GetKeyState(Key::KEY_SPACE) && flyMode)
		camera.ProcessKeyboard(CameraMovement::UP, (GLfloat)timer.deltaTime);

	if (input->IsKeyDown(Key::KEY_SPACE) && !flyMode) {
		if ((static_cast<GLfloat>(timer.currentTime) - camera.JumpStart) > 0.2f && world->GetBlock(camera.Position + glm::vec3(0, -2, 0)) != BlockName::Air)
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

	if (input->IsKeyDown(Key::KEY_UP) && flyMode)
		camera.MovementSpeed += 1.f;

	if (input->IsKeyDown(Key::KEY_DOWN) && flyMode)
		camera.MovementSpeed -= 1.f;

	if (input->IsKeyDown(Key::KEY_F)) {
		flyMode = !flyMode;
		if (!flyMode)
			camera.MovementSpeed = 7.f;
	}

	for (GLuint key : range(48, 57)) {
		if (input->IsKeyDown((Key)key)) {
			SelectedBlock = (BlockName)(key - 47);
			break;
		}
	}


	if (input->IsScrollUp())
	{
		GLuint i = static_cast<int>(SelectedBlock) + 1;
		if (i > rs->GetBlocksCount())
			i = 1;
		SelectedBlock = (BlockName)i;
	}
	if (input->IsScrollDown())
	{
		GLuint i = static_cast<int>(SelectedBlock) - 1;
		if (i < 1)
			i = rs->GetBlocksCount();
		SelectedBlock = (BlockName)i;
	}


	if (!world->worldGenerated)
		return;

	ProcessMovement();

	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && timer.click())
	{
		//Block breaking
		auto rayEnd = GetRayEnd();
		for (auto& it : rayEnd) {
			auto block = world->GetBlock(it);
			if (block != BlockName::Air) {
				if (block != BlockName::TNT)
					rs->SoundEngine->play2D("Audio/break.mp3", false);
				else
					rs->SoundEngine->play2D("Audio/tnt.mp3", false);
				world->SetBlock(it, BlockName::Air);
				break;
			}
		}
	}

	vec3 pointVec;

	if (input->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT) && timer.click())
	{
		//Block placing
		auto rayEnd = GetRayEnd();
		for (auto& it : rayEnd) {
			auto block = world->GetBlock(it);
			if (block == BlockName::Air)
				pointVec = it;
			else
				break;
		}
		if (pointVec != rayEnd.back()) {
			world->SetBlock(pointVec, SelectedBlock);
			rs->SoundEngine->play2D("Audio/place.mp3", false);
		}
	}

	//Calculate position delta
	vec3 positionDelta;
	auto blockBelow = world->GetBlock(vec3(0, -2, 0) + camera.Position);
	{
		vec3 acutalPos = {
				static_cast<int>(camera.Position.x),
				static_cast<int>(camera.Position.y),
				static_cast<int>(camera.Position.z)
		};
		if (acutalPos != lastPlayerPosition)
		{
			positionDelta = acutalPos - lastPlayerPosition;
			lastPlayerPosition = acutalPos;
		}
	}

	//Player moved to another block - play footstep
	if (positionDelta != vec3(0, 0, 0) && blockBelow != BlockName::Air && timer.footstep())
		rs->SoundEngine->play2D("Audio/footStep.mp3", false);


	//Chunk render
	auto chunkPos = world->GetChunkPosition(camera.Position);
	if ((lastChunk - chunkPos) != vec2(0, 0))
		world->SetRenderedChunks(chunkPos);
	lastChunk = chunkPos;

	//Debug data
	auto rayEnd = GetRayEnd();
	auto onBlockPos = CalculateOnBlockPosition(camera.Position);
	auto onChunkPos = world->ToChunkPosition(camera.Position);
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
	STRING << "Chunk (X,Y): " << chunkPos.x << ", " << chunkPos.y << "   " <<
		" Look Direction: " << static_cast<char>(camera.GetLookDirection());
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "On block position (X,Y,Z): " <<
		std::fixed << std::setprecision(2) << onBlockPos.x << ", " <<
		std::fixed << std::setprecision(2) << onBlockPos.y << ", " <<
		std::fixed << std::setprecision(2) << onBlockPos.z << "  " <<
		"Standing on: " << Cube::GetBlockName(blockBelow);
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Player pos delta: "
		<< std::fixed << std::setprecision(5) << playerPosDelta.x << ", "
		<< std::fixed << std::setprecision(5) << playerPosDelta.y << ", "
		<< std::fixed << std::setprecision(5) << playerPosDelta.z;
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Generated chunks: " << world->GetChunksCount() <<
		"    Built meshes: " << world->GetMeshCount();
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "Ray: " <<
		std::fixed << std::setprecision(2) << pointVec.x << "  " <<
		std::fixed << std::setprecision(2) << pointVec.y << "  " <<
		std::fixed << std::setprecision(2) << pointVec.z << "  ";
	DebugData.push_back(STRING.str());
	std::stringstream().swap(STRING);
	STRING << "On chunk pos: " <<
		std::fixed << std::setprecision(0) << onChunkPos.x << ", " <<
		std::fixed << std::setprecision(0) << onChunkPos.y << ", " <<
		std::fixed << std::setprecision(0) << onChunkPos.z;
	DebugData.push_back(STRING.str());
}

void Engine::renderFrame()
{
	if (!world->worldGenerated) {
		text.RenderText("Generating world...", static_cast<float>((screenWidth/2) - 80), static_cast<float>(screenHeight/2), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		GLuint x = world->GetChunksCount();
		if (world->GetChunksCount() >= SectionSize) {
			world->BuildMesh();
			if (world->GetMeshCount() >= world->GetPlatformSize(world->GetRenderDistance())) {
				world->SetRenderedChunks(vec2(0, 0));
				world->worldGenerated = true;
			}
			else
				return;
		}
		return;
	}

	world->DrawChunks(camera);
	crossHair.Draw();
	if (showDebugData) {
		int line = 0;
		for (std::string& str : DebugData) {
			text.RenderText(str, static_cast<float>(5), static_cast<float>(screenHeight - 15 - (line++ * 25)), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		}
	}

	if (flyMode) {
		std::stringstream STRING;
		STRING << "Speed: " << camera.MovementSpeed;
		text.RenderText("Fly mode", static_cast<float>(screenWidth-100), static_cast<float>(screenHeight-20), 0.4f, glm::vec3(0.f, 0.f, 0.f));
		text.RenderText(STRING.str(), static_cast<float>(screenWidth - 100), static_cast<float>(screenHeight - 40), 0.4f, glm::vec3(0.f, 0.f, 0.f));
	}

	std::stringstream STRING;
	STRING << "Selected block: " << Cube::GetBlockName(SelectedBlock);
	text.RenderText(STRING.str(), static_cast<float>(10), static_cast<float>(10), 0.4f, glm::vec3(0.f, 0.f, 0.f));
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
	RenderDistance = 8;
	ChunkSize = 8;
	ChunkOffset = 2;
	RayRange = 7;
	flyMode = false;
	SelectedBlock = BlockName::Cobble;
	StartPosition = glm::vec3(0, 40, 0);
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

	//Mouse wheel 
	glfwSetScrollCallback(window, input->ScrollCallback);
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
	//Wood
	rs->AddBlock(BlockName::Wood, { 4,14 }, { 4,14 }, { 5,14 }, { 5,14 }, { 4,14 }, { 4,14 });
	rs->GetBlock(BlockName::Wood)->BindFaces();
	//Leave
	rs->AddBlock(BlockName::Leave, { 4,12 }, { 4,12 }, { 5,12 }, { 5,12 }, { 4,12 }, { 4,12 });
	rs->GetBlock(BlockName::Leave)->BindFaces();
	//Sand
	rs->AddBlock(BlockName::Sand, { 2,14 }, { 2,14 }, { 2,14 }, { 2,14 }, { 2,14 }, { 2,14 });
	rs->GetBlock(BlockName::Sand)->BindFaces();
	//Workbench
	rs->AddBlock(BlockName::Workbench, { 11,12 }, { 11,12 }, { 11,13 }, { 4,15 }, { 12,12 }, { 12,12 });
	rs->GetBlock(BlockName::Workbench)->BindFaces();
	//Plank
	rs->AddBlock(BlockName::Plank, { 4,15 }, { 4,15 }, { 4,15 }, { 4,15 }, { 4,15 }, { 4,15 });
	rs->GetBlock(BlockName::Plank)->BindFaces();
	//TNT
	rs->AddBlock(BlockName::TNT, { 8,15 }, { 8,15 }, { 9,15 }, { 10,15 }, { 8,15 }, { 8,15 });
	rs->GetBlock(BlockName::TNT)->BindFaces();
	//Glass
	rs->AddBlock(BlockName::Glass, { 1,12 }, { 1,12 }, { 1,12 }, { 1,12 }, { 1,12 }, { 1,12 });
	rs->GetBlock(BlockName::Glass)->BindFaces();

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
	SectionSize = world->GenerateWorld();

	camera.Position = StartPosition;
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

	if (!flyMode) {
		auto block = world->GetBlock(camera.Position + glm::vec3(0, -2, 0));
		if (block == BlockName::Air)
			camera.Position += glm::vec3(0, -7 * static_cast<GLfloat>(timer.deltaTime), 0);

		if (timer.currentTime - camera.JumpStart < 0.2f)
			camera.Position += glm::vec3(0, 12 * static_cast<GLfloat>(timer.deltaTime), 0);
		else
			camera.isJumping = false;

	}

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

}

glm::vec3 Engine::ForwardsVector()
{
	glm::vec3 forward = glm::normalize(camera.Front);
	return forward;
}

std::vector<vec3> Engine::GetRayEnd()
{
	// Ensures passed direction is normalized
	GLfloat range = RayRange;
	auto startPoint = camera.Position;
	auto nDirection = glm::normalize(ForwardsVector());
	auto endPoint = startPoint + nDirection * range;
	auto startVoxel = vec3(static_cast<int>(std::floor(camera.Position.x)), static_cast<int>(std::floor(camera.Position.y)), static_cast<int>(std::floor(camera.Position.z)));

	// +1, -1, or 0
	int stepX = (nDirection.x > 0) ? 1 : ((nDirection.x < 0) ? -1 : 0);
	int stepY = (nDirection.y > 0) ? 1 : ((nDirection.y < 0) ? -1 : 0);
	int stepZ = (nDirection.z > 0) ? 1 : ((nDirection.z < 0) ? -1 : 0);

	float tDeltaX =
		(stepX != 0) ? fmin(stepX / (endPoint.x - startPoint.x), FLT_MAX) : FLT_MAX;
	float tDeltaY =
		(stepY != 0) ? fmin(stepY / (endPoint.y - startPoint.y), FLT_MAX) : FLT_MAX;
	float tDeltaZ =
		(stepZ != 0) ? fmin(stepZ / (endPoint.z - startPoint.z), FLT_MAX) : FLT_MAX;

	float tMaxX = (stepX > 0.0f) ? tDeltaX * (1.0f - startPoint.x + startVoxel.x)
		: tDeltaX * (startPoint.x - startVoxel.x);
	float tMaxY = (stepY > 0.0f) ? tDeltaY * (1.0f - startPoint.y + startVoxel.y)
		: tDeltaY * (startPoint.y - startVoxel.y);
	float tMaxZ = (stepZ > 0.0f) ? tDeltaZ * (1.0f - startPoint.z + startVoxel.z)
		: tDeltaZ * (startPoint.z - startVoxel.z);

	auto currentVoxel = startVoxel;
	std::vector<vec3> intersected;
	intersected.push_back(startVoxel);

	// sanity check to prevent leak
	while (intersected.size() < range * 3) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				currentVoxel.x += stepX;
				tMaxX += tDeltaX;
			}
			else {
				currentVoxel.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		else {
			if (tMaxY < tMaxZ) {
				currentVoxel.y += stepY;
				tMaxY += tDeltaY;
			}
			else {
				currentVoxel.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
			break;
		intersected.push_back(currentVoxel);
	}

	return intersected;
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