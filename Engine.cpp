#include "Engine.h"


Engine* Engine::instance = nullptr;

void Engine::updateWindow()
{
	//Update internal timer
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

	if (input->IsKeyDown(Key::KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

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
		chunk.blocks[5] = glm::vec3(0.f,0.f,0.f);
	//std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;
}

void Engine::renderFrame()
{

	for (const auto& block : chunk.blocks) {   // Range-for!
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, block);
		quad.shadingProgram->Use();
		quad.shadingProgram->SetData("projection", camera.Projection);
		quad.shadingProgram->SetData("view", camera.GetViewMatrix());
		quad.shadingProgram->SetData("model", model);
		quad.Draw();
	}

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

	/////
	rs->AddShadingProgram("test", "Shaders/shader.vert", "Shaders/shader.frag");
	quad.Init();
	quad.AddFace((Face*)&quad.FrontFace);
	quad.AddFace((Face*)&quad.BackFace);
	quad.AddFace((Face*)&quad.TopFace);
	quad.AddFace((Face*)&quad.BottomFace);
	quad.AddFace((Face*)&quad.RightFace);
	quad.AddFace((Face*)&quad.LeftFace);
	quad.BindData();
		
	quad.SetShadingProgram(rs->GetShadingProgram("test"));
	
	quad.AddTexture("face",rs->GetTexture("Textures/cobble.png"));

	quad.shadingProgram->Use();
	quad.Textures["face"]->Init();

	quad.shadingProgram->SetData("texture1", quad.Textures["face"]->GetId());
	quad.Textures["face"]->Bind(GL_TEXTURE1);

	quad.shadingProgram->Use();
	quad.shadingProgram->SetData("projection", camera.Projection);
	quad.shadingProgram->SetData("view", camera.GetViewMatrix());

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