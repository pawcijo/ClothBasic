#include "Src/ClothApp/ClothApp.hpp"

#include "Src/Util/Config/ConfigUtils.hpp"

#include "Src/HelloWorld/Square/Square.hpp"
#include "Src/HelloWorld/Object3D/Object3D.hpp"
#include "Src/HelloWorld/SubDataObject/SubDataObject.hpp"
#include "Src/HelloWorld/Circle/Circle.hpp"
#include "Src/Shapes/Shapes.hpp"
#include "Src/Util/ClothGeneration/ClothGeneration.hpp"
#include <glm/glm.hpp>
#include <cmath>

//imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

float dt = 1.0f / 60.0f;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

static ClothApp *ptr;
ClothApp::ClothApp(Window &window) : windowRef(window), config(ConfigUtils::ConfigLoader()),
									 cloth1(std::get<float>(config.GetValueFromMap("ClothWidth")),
											std::get<float>(config.GetValueFromMap("ClothHeight")),
											std::get<unsigned>(config.GetValueFromMap("ParticleWidthNumber")),
											std::get<unsigned>(config.GetValueFromMap("ParticleHeightNumber"))),
									 clothController(cloth1),
									 clothDebugInfo(cloth1, clothController)
{

	ptr = this;
	printf("ClothApp created .\n");
	glfwSetKeyCallback(window.window, key_callback);
	glfwSetCursorPosCallback(window.window, cursor_position_callback);

	pushingForce = std::get<float>(config.GetValueFromMap("ForceForPushingCloth"));
	shader2D = new Shader("Shaders/Cloth.vs", "Shaders/Cloth.fs");
	shader3D = new Shader("Shaders/Cloth3D.vs", "Shaders/Cloth3D.fs");
	subDataShader3D = new Shader("Shaders/SubDataCloth3D.vs", "Shaders/SubDataCloth3D.fs");
	subDataShader3D_2 = new Shader("Shaders/SubDataCloth3D_2.vs", "Shaders/SubDataCloth3D_2.fs");
	clothResolveShader = new Shader("", "", "Shaders/ClothResolve.comp");
	clothUpdateShader = new Shader("", "", "Shaders/ClothUpdate.comp");
	lastX = windowRef.iHeight / 2;
	lastY = windowRef.iWidth / 2;
	clothParticleWidth = std::get<unsigned>(config.GetValueFromMap("ParticleWidthNumber"));
	clothParticleHight = std::get<unsigned>(config.GetValueFromMap("ParticleHeightNumber"));

	glGenQueries(1, &query);
	glGenQueries(1, &query2);

	// Setup delta time
	// TODO change to elapsed time
	clothResolveShader->use();
	clothResolveShader->setFloat("springConstant", *springConstant);
	glUniform1f(0, dt);
	clothUpdateShader->use();
	glUniform1f(0, dt);
}

void ClothApp::processMouse()
{
	if (processMouseMovement)
	{
		if (mouseCallBack)
		{
			if (mouseToUpdate)
			{
				mouseToUpdate = false;
				if (firstMouse)
				{
					lastX = posx;
					lastY = posy;
					firstMouse = false;
				}

				float xoffset = posx - lastX;
				float yoffset =
					lastY - posy; // reversed since y-coordinates go from bottom to top

				lastX = posx;
				lastY = posy;

				camera.ProcessMouseMovement(xoffset, yoffset);
			}
		}
	}
}

void ClothApp::setViewPerspective(Camera &aCamera)
{

	projection = glm::perspective(
		aCamera.Zoom, (float)windowRef.iWidth / (float)windowRef.iHeight,
		0.1f, 10000.0f);
	view = aCamera.GetViewMatrix();

	shader3D->use();
	shader3D->setMat4("projection", projection);
	shader3D->setMat4("view", view);

	subDataShader3D->use();
	subDataShader3D->setMat4("projection", projection);
	subDataShader3D->setMat4("view", view);

	subDataShader3D_2->use();
	subDataShader3D_2->setMat4("projection", projection);
	subDataShader3D_2->setMat4("view", view);
}

void ClothApp::Update()
{
}

void ClothApp::PhysixUpdate()
{

	if (CPU_SIMULATION_ON)
	{
		cloth1.AddForce(glm::vec3(0, -0.2, 0) *
						TIME_STEPSIZE2); // TODO change  time_step to reliable time

		cloth1.Update(TIME_STEPSIZE2, 5);
	}

	clothResolveShader->use();
	clothResolveShader->setFloat("time", glfwGetTime());
	clothResolveShader->setFloat("springConstant", *springConstant);
	int constraintSize = cloth1.getConstraintsData().size() / 8;

	for (int j = 0; j < *clothConstraintsResolvePerUpdate; j++)
	{

		clothResolveShader->setInt("computeNumber", j);
		for (int i = 0; i < 8; i++)
		{
			clothResolveShader->setInt("constraintNumber", i);
			clothResolveShader->setInt("offset", (cloth1.getConstraintsData().size() / 8) * i);
			glDispatchCompute(std::ceil(constraintSize / 512.0), 1, 1);
		}
	}
	cloth1.retriveData();

	clothUpdateShader->use();
	clothUpdateShader->setFloat("time", glfwGetTime());
	cloth1.AddForceGPU(glm::vec3(0, -0.5, 0));
	glDispatchCompute(std::ceil(cloth1.getPositionData().size() / 512.0), 1, 1);
	cloth1.retriveData();
}

void ClothApp::run()
{

	camera.Position = glm::vec3(-100, 10, -5);
	exampleToUpdate = Shapes::BatchedCube::vertices;
	plane = Shapes::Rectangle::vertices;

	std::vector<unsigned> clothIndicies;
	std::vector<float> clothColorBuffer;

	clothColorBuffer = generateClothColor(cloth1.GetParticles().size());
	clothIndicies = genereteIndicies(cloth1.GetClothSize());

	Transform cubeTransform = Transform::origin();
	Transform subDataCubeTransform = Transform::origin();
	Transform subDataPlaneTransform = Transform::origin();
	Transform circleTransform = Transform::origin();
	Transform clothTransform = Transform::origin();
	Transform gpuCloth = Transform::origin();

	gpuCloth.translate(glm::vec3(-100, 0, 0));

	DrawMode configDrawMode = DrawMode(std::get<unsigned>(config.GetValueFromMap("Drawmode")));

	if (configDrawMode == DrawMode::EWireFrame)
	{
		printf("Config draw mode : WireFrame \n");
	}
	else
	{
		printf("Config draw mode : Default \n");
	}

	Object3D cube(Shapes::Cube::vertices, Shapes::Cube::indices, cubeTransform);
	cube.transform.scaleTransform(10, 10, 10);
	cube.transform.translate(glm::vec3(-15.0f, 0.0f, 0.0f));

	SubDataObject subDataCube(exampleToUpdate, Shapes::BatchedCube::colors, Shapes::Cube::indices, subDataCubeTransform);
	subDataCube.transform.scaleTransform(10, 10, 10);
	subDataCube.transform.translate(glm::vec3(10, 10, 10));

	SubDataObject subDataPlace(plane, Shapes::Rectangle::colors, Shapes::Rectangle::indices, subDataPlaneTransform);
	subDataPlace.transform.scaleTransform(10, 10, 10);
	subDataPlace.transform.translate(glm::vec3(10, 10, 10));

	SubDataObject subDataCloth(clothController.GetVertexInfo(), clothColorBuffer, clothIndicies, clothTransform, configDrawMode);
	subDataCloth.transform.scaleTransform(1, 1, 1);
	subDataCloth.transform.translate(glm::vec3(1, 1, 1));

	Circle circle(170, 0.5, circleTransform);
	circleTransform.translate(glm::vec3(-0.93, 0.89, 0));
	circleTransform.scaleTransform(1, (float)windowRef.iWidth / (float)windowRef.iHeight, 1);
	circleTransform.scaleTransform(0.1, 0.1, 0.1);

	unsigned next_tick = (glfwGetTime() * 1000);
	int loops;
	float interpolation = 1.0;
	const int TICKS_PER_SECOND = 64;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(windowRef.window, true);
	const char *glsl_version = "#version 430";
	ImGui_ImplOpenGL3_Init(glsl_version);

	while (!glfwWindowShouldClose(windowRef.window))
	{

		loops = 0;

		while ((glfwGetTime() * 1000) > next_tick && loops < MAX_FRAMESKIP)
		{
			processKeys();
			processMouse();

			PhysixUpdate();

			globalCameraPosition = camera.Position;
			globalCameraYaw = camera.Yaw;
			globalCameraPitch = camera.Pitch;

			next_tick += SKIP_TICKS;
			loops++;
		}

		interpolation =
			float((glfwGetTime() * 1000) + SKIP_TICKS - next_tick) /
			float(SKIP_TICKS);



		ImGuiStuff();

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw 3D
		glEnable(GL_DEPTH_TEST);
		setViewPerspective(camera);

		subDataCube.Draw(subDataShader3D, exampleToUpdate, Shapes::BatchedCube::colors, Shapes::BatchedCube::indices);
		subDataPlace.Draw(subDataShader3D, plane, Shapes::Rectangle::colors, Shapes::Rectangle::indices);
		subDataCloth.Draw(subDataShader3D, clothController.GetVertexInfo(), clothColorBuffer, clothIndicies);
		//subDataCloth.Draw2(subDataShader3D_2, clothController.GetVertexInfo_2(), clothColorBuffer, clothIndicies);

		clothController.Draw(subDataShader3D_2, gpuCloth);
		cube.Draw(shader3D);

		glDisable(GL_DEPTH_TEST);
		//draw 2D
		circle.Draw(shader2D);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(windowRef.window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void ClothApp::processKeys()
{

	if (glfwGetKey(windowRef.window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.Position += cameraSpeed * camera.Front;
	}

	if (glfwGetKey(windowRef.window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		for (int i = 0; i < exampleToUpdate.size(); i++)
		{
			exampleToUpdate[i] = exampleToUpdate[i] + 1;
		}
	}

	if (glfwGetKey(windowRef.window, GLFW_KEY_E) == GLFW_PRESS)
	{

		for (int i = 0; i < exampleToUpdate.size(); i++)
		{
			exampleToUpdate[i] = exampleToUpdate[i] - 1;
		}
	}

	if (glfwGetKey(windowRef.window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.Position -= cameraSpeed * camera.Front;
	}
	if (glfwGetKey(windowRef.window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.Position -=
			glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	}
	if (glfwGetKey(windowRef.window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.Position +=
			glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	}
	if (glfwGetKey(windowRef.window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.Position += cameraSpeed * camera.Up;
	}

	if (glfwGetKey(windowRef.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.Position -= cameraSpeed * camera.Up;
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		keyPressedStatus[key] = keyPressedStatus[key] ? false : true;
		if (keyPressedStatus[key])
		{
			processMouseMovement = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{

			processMouseMovement = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		printf("keyPressedStatus :  %s \n", keyPressedStatus[key] ? "true" : "false");
	}

	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{

		keyPressedStatus[key] = keyPressedStatus[key] ? false : true;
		CPU_SIMULATION_ON != CPU_SIMULATION_ON;

		if (keyPressedStatus[key])
		{
			CPU_SIMULATION_ON = true;
			std::cout << "CPU SIMULATION ON \n";
		}
		else
		{
			CPU_SIMULATION_ON = false;
			std::cout << "CPU SIMULATION OFF \n";
		}
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		ptr->clothDebugInfo.ShowLastRowInfo();
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		ptr->clothDebugInfo.ShowLastRowInfo_2();
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		ptr->clothDebugInfo.ShowMatrixY();
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		ptr->clothDebugInfo.MoveLastRow(ptr->pushingForce);
		std::vector<int> whatToMove;
		for (int i = 10; i > 0; i--)
		{
			whatToMove.push_back(ptr->cloth1.getPositionData().size() - i);
		}
		ptr->cloth1.AddForceGPU(glm::vec3(0, 0, 150), whatToMove);
	}

	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		printf("Camera Position:  x:%f y:%f z:%f \n", globalCameraPosition.x, globalCameraPosition.y, globalCameraPosition.z);
		printf("Camera yaw:%f pitch:%f \n", globalCameraYaw, globalCameraPitch);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouseToUpdate = true;
	posx = xpos;
	posy = ypos;
}

void ClothApp::ImGuiStuff()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Hello, world!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderInt("Contraints Resolve Per Update", clothConstraintsResolvePerUpdate, 0, 1000);
	ImGui::SliderFloat("Spring constant", springConstant, 0.001, 1);
	ImGui::End();
	ImGui::Render();
}

// add to update to get info about last row position
/*
	std::cout<<"\n";
	std::cout << "\n" << "GPU :" << "\n";
	for(int i = 0;i<10;i++)
{
		int firstVerticleId = cloth1.getConstraintsData()[i].x;
		int secondParticleId = cloth1.getConstraintsData()[i].y;

		float P1_x = cloth1.getPositionData()[firstVerticleId].x;
		float P2_x = cloth1.getPositionData()[secondParticleId].x;

		float P1_y = cloth1.getPositionData()[firstVerticleId].y;
		float P2_y = cloth1.getPositionData()[secondParticleId].y;

		float P1_z = cloth1.getPositionData()[firstVerticleId].z;
		float P2_z = cloth1.getPositionData()[secondParticleId].z;

		bool P1_moveable = cloth1.getPositionData()[firstVerticleId].w;
		bool P2_moveable = cloth1.getPositionData()[secondParticleId].w;

		glm::vec4 P1_Acceler = cloth1.getAcelerationsData()[firstVerticleId];
		glm::vec4 P2_Acceler = cloth1.getAcelerationsData()[secondParticleId];


		std::cout << "Contraint [" << i << "]: \n"
			<< "Indexes : " << firstVerticleId << " | " << secondParticleId << "\n"
			<< "Cords :" << P1_x << " " << P1_y << " " << P1_z << " | " << P2_x << " " << P2_y << " " << P2_z << "\n"
			<< "Acceleration: " << P1_Acceler.x << " " << P1_Acceler.y << " " << P1_Acceler.z << " | "
			<< P2_Acceler.x << " " << P2_Acceler.y << " " << P2_Acceler.z << "\n"
			<< "P1 moveable : " << P1_moveable << " P2 moveable : " << P2_moveable << "\n";

}


	std::cout << "\n" << "CPU :" << "\n";
	for (int i = 0; i < 10; i++)
	{
		int firstVerticleId = cloth1.CPUconstraints[i].p1->getIndex();
		int secondParticleId = cloth1.CPUconstraints[i].p2->getIndex();

		float P1_x = cloth1.CPUparticles[firstVerticleId].GetPosition().x;
		float P2_x = cloth1.CPUparticles[secondParticleId].GetPosition().x;

		float P1_y = cloth1.CPUparticles[firstVerticleId].GetPosition().y;
		float P2_y = cloth1.CPUparticles[secondParticleId].GetPosition().y;

		float P1_z = cloth1.CPUparticles[firstVerticleId].GetPosition().z;
		float P2_z = cloth1.CPUparticles[secondParticleId].GetPosition().z;

		glm::vec3 P1_Acceler = cloth1.CPUparticles[firstVerticleId].GetAcceleration();
		glm::vec3 P2_Acceler = cloth1.CPUparticles[secondParticleId].GetAcceleration();


		std::cout << "Contraint [" << i << "]: \n"
			<< "Indexes : " << firstVerticleId << " | " << secondParticleId << "\n"
			<< "Cords : " << P1_x << " " << P1_y << " " << P1_z << " | " << P2_x << " " << P2_y << " " << P2_z << "\n"
			<< "Acceleration: " << P1_Acceler.x <<" "<<P1_Acceler.y <<" " << P1_Acceler.z <<" | "
			<< P2_Acceler.x<< " "<<P2_Acceler.y <<" " << P2_Acceler.z << "\n";

	}
	*/

//TO DODO's
/*

  *	DIVIDE CONTRAINTS 
	to remove race conditions  
	Notes:
	- 8 new buffers, same size and same shader, the forumala is the same (move based on rest distance and particle positions)
	- partly done for now with offset that divides buffer to 8 alligned buffers
	- [DONE] add imgui live variable change + fps counter from imgui <3  [must have] [26]

  * create selfcolision compute shader [must have] [27-28]
  * create otherobject collision shader [must have] [28-30]


*/