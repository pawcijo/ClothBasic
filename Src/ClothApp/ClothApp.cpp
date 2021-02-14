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
#include <chrono>

#include "Src/Util/Utils/Utils.hpp"

//imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
	clothCollisionShader = new Shader("", "", "Shaders/ClothResolveCollision.comp");
	lastX = windowRef.iHeight / 2;
	lastY = windowRef.iWidth / 2;
	clothParticleWidth = std::get<unsigned>(config.GetValueFromMap("ParticleWidthNumber"));
	clothParticleHight = std::get<unsigned>(config.GetValueFromMap("ParticleHeightNumber"));

	glGenQueries(1, &query);
	glGenQueries(1, &query2);

	// Setup delta time
	// TODO change to elapsed time
	float dt = 1.0f / 60.0f;
	clothResolveShader->use();
	clothResolveShader->setFloat("springConstant", *springConstant);
	clothResolveShader->setFloat("dt", dt);

	clothUpdateShader->use();
	clothUpdateShader->setFloat("dt", dt);
	clothUpdateShader->setFloat("dampingLength", dt);
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

void ClothApp::PhysixUpdate(float elapsedTime)
{

	if (*CPU_SIMULATION_ON)
	{

		if (*cpuConstantTimeStep)
		{
			cloth1.AddForce(glm::vec3(0, *gravityForce, 0) *
							TIME_STEPSIZE2);
			cloth1.Update(TIME_STEPSIZE2, *CPUConstrainteResolveNumberPerUpdate);
		}
		else
		{
			cloth1.AddForce(glm::vec3(0, *gravityForce, 0) *
							elapsedTime);
			cloth1.Update(elapsedTime, *CPUConstrainteResolveNumberPerUpdate);
		}
	}

	if (*GPU_SIMULATION_ON)
	{

		if (*gpuClothResolveConstraintOn)
		{
			clothResolveShader->use();
			clothResolveShader->setFloat("time", glfwGetTime());
			clothResolveShader->setFloat("springConstant", *springConstant);
			int constraintSize = cloth1.getConstraintsData().size() / 8;

			for (int j = 0; j < *clothConstraintsResolvePerUpdate; j++)
			{

				clothResolveShader->setInt("computeNumber", j);
				for (int i = 0; i < 8; i++)
				{
					if (i < 4)
					{
						for (int k = 0; k < *clothStructuralConstraintsRepetition; k++)
						{
							clothResolveShader->setInt("constraintNumber", i);
							clothResolveShader->setInt("offset", (cloth1.getConstraintsData().size() / 8) * i);
							glDispatchCompute(std::ceil(constraintSize / 1024.0), 1, 1);
						}
					}
				}
			}

			for (int j = 0; j < *clothConstraintsResolvePerUpdate; j++)
			{

				clothResolveShader->setInt("computeNumber", j);
				for (int i = 0; i < 8; i++)
				{
					if (i > 4)
					{
						for (int k = 0; k < *clothShearAndBendingConstraintsRepetition; k++)
						{
							clothResolveShader->setInt("constraintNumber", i);
							clothResolveShader->setInt("offset", (cloth1.getConstraintsData().size() / 8) * i);
							glDispatchCompute(std::ceil(constraintSize / 1024.0), 1, 1);
						}
					}
				}
			}

			cloth1.retriveData();
		}
		glm::vec3 aaBBPostion = calculateAABBCenter(exampleToUpdate);
		*aaBBXPosition = aaBBPostion.x;
		*aaBBYPosition = aaBBPostion.y;
		*aaBBZPosition = aaBBPostion.z;

		if (*gpuClothUpdateOn)
		{
			clothUpdateShader->use();
			clothUpdateShader->setVec3("aaBBPosition", aaBBPostion);
			clothUpdateShader->setFloat("dt", elapsedTime);
			clothUpdateShader->setFloat("dampingLength", *dampingLength);
			clothUpdateShader->setBool("checkCollision", *gpuClothCollisionOn);

			cloth1.AddForceGPU(glm::vec3(0, *gravityForce, 0));
			glDispatchCompute(std::ceil(cloth1.getPositionData().size() / 512.0), 1, 1);
			cloth1.retriveData();
		}

		if (*gpuClothCollisionOn)
		{
			clothCollisionShader->use();
			clothCollisionShader->setVec3("aaBBPosition", aaBBPostion);
			clothCollisionShader->setBool("checkCollision", *gpuClothCollisionOn);
			cloth1.AddForceGPU(glm::vec3(0, *gravityForce, 0));
			glDispatchCompute(std::ceil(cloth1.getPositionData().size() / 512.0), 1, 1);
			cloth1.retriveData();
		}
	}
}

void ClothApp::run()
{

	camera.Position = glm::vec3(-100, 10, -5);

	exampleToUpdate = Shapes::BatchedCube::vertices;
	for (int i = 0; i < exampleToUpdate.size(); i++)
	{
		exampleToUpdate[i] *= 10;
	}

	std::vector<unsigned> clothIndicies;
	std::vector<float> clothColorBuffer;

	clothColorBuffer = generateClothColor(cloth1.GetParticles().size());
	clothIndicies = genereteIndicies(cloth1.GetClothSize());

	Transform clothTransform = Transform::origin();
	Transform gpuCloth = Transform::origin();
	gpuCloth.translate(glm::vec3(300, 0, 0));
	Transform subDataCubeTransform = Transform::origin();

	SubDataObject subDataCube(exampleToUpdate, Shapes::BatchedCube::colors, Shapes::Cube::indices, subDataCubeTransform);

	DrawMode configDrawMode = DrawMode(std::get<unsigned>(config.GetValueFromMap("Drawmode")));

	if (configDrawMode == DrawMode::EWireFrame)
	{
		printf("Config draw mode : WireFrame \n");
	}
	else
	{
		printf("Config draw mode : Default \n");
	}

	SubDataObject subDataAABB(clothController.GetVertexInfo(), clothColorBuffer, clothIndicies, clothTransform);
	SubDataObject subDataCloth(clothController.GetVertexInfo(), clothColorBuffer, clothIndicies, clothTransform);

	unsigned next_tick = (glfwGetTime() * 1000);
	int loops;
	float interpolation = 1.0;
	const int TICKS_PER_SECOND = 64;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(windowRef.window, true);
	const char *glsl_version = "#version 430";
	ImGui_ImplOpenGL3_Init(glsl_version);

	std::chrono::steady_clock::time_point end;
	bool firstRun = true;
	float timeElapsedInS;

	while (!glfwWindowShouldClose(windowRef.window))
	{

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

		loops = 0;

		while ((glfwGetTime() * 1000) > next_tick && loops < MAX_FRAMESKIP)
		{
			processKeys();
			processMouse();
			if (firstRun)
			{
				timeElapsedInS = 1.0 / 60;
				firstRun = false;
			}

			if (*realTimeCorrection)
			{
				PhysixUpdate(timeElapsedInS);
			}
			else
			{
				PhysixUpdate(1.0 / 60);
			}

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

		subDataCloth.Draw(subDataShader3D, clothController.GetVertexInfo(), clothColorBuffer, clothIndicies, *CpuWireFrameMode);
		clothController.Draw(subDataShader3D_2, gpuCloth, *GpuWireFrameMode);
		if (*drawAABB)
			subDataCube.Draw(subDataShader3D, exampleToUpdate, Shapes::BatchedCube::colors, Shapes::BatchedCube::indices, *CpuWireFrameMode);

		glDisable(GL_DEPTH_TEST);
		//draw 2D
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(windowRef.window);
		glfwPollEvents();

		end = std::chrono::steady_clock::now();

		timeElapsedInS = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
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
	ImGui::Begin("Cloth simulation");
	ImGui::Text("This is simple cloth simulation on cpu and gpu,press F1 to use UI, press M to add Forece and make it move !");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("AABB position  %.3f  %.3f  %.3f ", *aaBBXPosition, *aaBBYPosition, *aaBBZPosition);
	ImGui::SliderInt("Contraints Resolve Per Update", clothConstraintsResolvePerUpdate, 0, 1000);
	ImGui::SliderInt("Contraints Structural Resolve Per Update", clothStructuralConstraintsRepetition, 0, 100);
	ImGui::SliderInt("Contraints Shear And Bending Resolve Per Update", clothShearAndBendingConstraintsRepetition, 0, 100);
	ImGui::SliderInt("Contraints resolve number for CPU", CPUConstrainteResolveNumberPerUpdate, 0, 100);
	ImGui::SliderFloat("Spring constant", springConstant, 0.001, 1);
	ImGui::SliderFloat("Damping length", dampingLength, 0.001, 1);
	ImGui::SliderFloat("Gravity Force", gravityForce, -5, 1);
	ImGui::Checkbox("ClothUpdateOn", gpuClothUpdateOn);
	ImGui::Checkbox("ClothResolveConstraintOn", gpuClothResolveConstraintOn);
	ImGui::Checkbox("GPU WireFrameMode", GpuWireFrameMode);
	ImGui::Checkbox("CPU WireFrameMode", CpuWireFrameMode);
	ImGui::Checkbox("Draw AABB", drawAABB);
	ImGui::Checkbox("CPU simulation", CPU_SIMULATION_ON);
	ImGui::Checkbox("GPU simulation", GPU_SIMULATION_ON);

	ImGui::Checkbox("Time(Chrono) interpolation", realTimeCorrection);
	ImGui::Checkbox("Constant time step for cpu", cpuConstantTimeStep);

	ImGui::SameLine();
	ImGui::Checkbox("ClothCollisionOn", gpuClothCollisionOn);

	if (ImGui::Button("AABB UP"))
	{
		moveAABB(exampleToUpdate, glm::vec3(0, 1, 0));
	}
	ImGui::SameLine();
	if (ImGui::Button("AABB DOWN"))
	{
		moveAABB(exampleToUpdate, glm::vec3(0, -1, 0));
	}
	ImGui::SameLine();
	if (ImGui::Button("AABB LEFT"))
	{
		moveAABB(exampleToUpdate, glm::vec3(-1, 0, 0));
	}
	ImGui::SameLine();
	if (ImGui::Button("AABB RIGHT"))
	{
		moveAABB(exampleToUpdate, glm::vec3(1, 0, 0));
	}
	ImGui::SameLine();
	if (ImGui::Button("AABB FORWARD"))
	{
		moveAABB(exampleToUpdate, glm::vec3(0, 0, 1));
	}
	ImGui::SameLine();
	if (ImGui::Button("AABB BACKWARD"))
	{
		moveAABB(exampleToUpdate, glm::vec3(0, 0, -1));
	}
	ImGui::End();
	ImGui::Render();
}
