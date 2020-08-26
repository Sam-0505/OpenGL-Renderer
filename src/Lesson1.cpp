#include <iostream>
#include <sstream>
#define	GLEW_STATIC
#include "GL/glew.h"// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.cpp"
#include "ImGui/imgui_impl_glfw.cpp"

// Global Variables
OrbitCamera orbitCamera;
const char* title = "OpenGL Starters";
int pheight = 768;
int pwidth = 1024;
bool gFullScreen = false;
GLFWwindow* pWindow ;
bool gWireframe;
const char* glsl_version = "#version 330 core";
bool pan = false;

//const float MOUSE_SENSITIVITY = 0.25f;
float gRadius = 10.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;
double lastmouseX = pwidth / 2.0; 
double lastmouseY = pheight / 2.0;
//Shaders
//const std::string texture2 = "D:/OpenGL/Project1/textures/airplane.png";
//const std::string floorTexture = "D:/OpenGL/Project1/textures/grid.jpg";

//FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;


// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
//void update(double elapsedTime, double* lastmouseX, double* lastmouseY);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_scrollInput(GLFWwindow* window, double Xoff, double Yoff);
void showFPS(GLFWwindow* window);
bool initOpenGL();

void UILoader(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color);

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}
	
	const int modelNum = 6;
	Mesh mesh[modelNum];
	Texture2D texture2D[modelNum];

	mesh[0].loadOBJ("D:/OpenGL/Project1/models/crate.obj");
	mesh[1].loadOBJ("D:/OpenGL/Project1/models/woodcrate.obj");
	mesh[2].loadOBJ("D:/OpenGL/Project1/models/robot.obj");
	mesh[3].loadOBJ("D:/OpenGL/Project1/models/floor.obj");
	mesh[4].loadOBJ("D:/OpenGL/Project1/models/bowling_pin.obj");
	mesh[5].loadOBJ("D:/OpenGL/Project1/models/bunny.obj");

	texture2D[0].loadTexture("D:/OpenGL/Project1/textures/crate.jpg", true);
	texture2D[1].loadTexture("D:/OpenGL/Project1/textures/woodcrate_diffuse.jpg", true);
	texture2D[2].loadTexture("D:/OpenGL/Project1/textures/robot_diffuse.jpg", true);
	texture2D[3].loadTexture("D:/OpenGL/Project1/textures/tile_floor.jpg", true);
	texture2D[4].loadTexture("D:/OpenGL/Project1/textures/AMF.tga", true);
	texture2D[5].loadTexture("D:/OpenGL/Project1/textures/bunny_diffuse.jpg", true);

	Mesh lightMesh;
	lightMesh.loadOBJ("D:/OpenGL/Project1/models/crate.obj");

	//Model Position
	glm::vec3 modPos[] = 
	{
		glm::vec3(-2.5f, 1.0f, 0.0f),	// crate1
		glm::vec3(2.5f, 1.0f, 0.0f),	// crate2
		glm::vec3(0.0f, 1.0f, 0.0f),	// robot
		glm::vec3(0.0f, 0.0f, 0.0f),		// floor
		glm::vec3(0.0f, 0.0f, 2.0f),	// pin
		glm::vec3(-2.0f, 0.0f, 2.0f)	// bunny
	};

	// Model scale
	glm::vec3 modScale[] = 
	{
		glm::vec3(1.0f, 1.0f, 1.0f),	// crate1
		glm::vec3(1.0f, 1.0f, 1.0f),	// crate2
		glm::vec3(1.0f, 1.0f, 1.0f),	// robot
		glm::vec3(10.0f, 1.0f, 10.0f),	// floor
		glm::vec3(0.1f, 0.1f, 0.1f),	// pin
		glm::vec3(0.7f, 0.7f, 0.7f)		// bunny
	};

	ShaderProgram lightProgram;
	lightProgram.loadShaders("D:/OpenGL/Project1/shaders/light.vert", "D:/OpenGL/Project1/shaders/light.frag");

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("D:/OpenGL/Project1/shaders/basic.vert","D:/OpenGL/Project1/shaders/basic.frag");

	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	float lightAngle = 0.0f;
	
	glm::vec3 lightPos = glm::vec3(0.0f,1.0f,10.0f);
	glm::vec3 lightColor= glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 viewPos = orbitCamera.getViewPos();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		
		glfwPollEvents();
		//update(deltaTime,&lastmouseX,&lastmouseY);

		UILoader(show_demo_window, show_another_window, clear_color);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texture2D1.bind(0);

		glm::mat4 model, view, projection;
		
		orbitCamera.setRadius(gRadius);
		orbitCamera.rotate(gYaw, gPitch);

		// Create the View matrix
		view = orbitCamera.getViewMatrix();
		//view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)pwidth / (float)pheight, 0.1f, 100.0f);
		glm::vec3 viewPos = orbitCamera.getViewPos();

		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		shaderProgram.use();

		// Pass the matrices to the shader
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		shaderProgram.setUniform("viewPos", viewPos);

		shaderProgram.setUniform("dlight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setUniform("dlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setUniform("dlight.diffuse", lightColor);
		shaderProgram.setUniform("dlight.direction", glm::vec3(1.0f, 1.0f, 1.0f));

		shaderProgram.setUniform("plight.position", lightPos);
		shaderProgram.setUniform("plight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setUniform("plight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setUniform("plight.diffuse", lightColor);
		shaderProgram.setUniform("plight.constant", 1.0f);
		shaderProgram.setUniform("plight.linear", 0.07f);
		shaderProgram.setUniform("plight.quad", 0.017f);

		shaderProgram.setUniform("slight.position", lightPos);
		shaderProgram.setUniform("slight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setUniform("slight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setUniform("slight.diffuse", lightColor);
		shaderProgram.setUniform("slight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
		shaderProgram.setUniform("slight.cosInnerCone", cos(glm::radians(30.0f)));
		shaderProgram.setUniform("slight.cosOuterCone", cos(glm::radians(60.0f)));
		shaderProgram.setUniform("slight.constant", 1.0f);
		shaderProgram.setUniform("slight.linear", 0.07f);
		shaderProgram.setUniform("slight.quad", 0.017f);

		for (int i = 0; i < modelNum; i++)
		{
			model = glm::translate(glm::mat4(), modPos[i]) * glm::scale(glm::mat4(), modScale[i]);
			shaderProgram.setUniform("model", model);

			shaderProgram.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			shaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.setUniform("material.diffuseMap", 0);
			shaderProgram.setUniform("material.shininess", 32.0f);

			texture2D[i].bind(0);
			mesh[i].draw();
			texture2D[i].unbind(0);
		}

		lightAngle+= (float)deltaTime * 50.0f;
		//lightPos.x = 8.0f * sin(glm::radians(lightAngle));
		model = glm::translate(glm::mat4(), lightPos);
		lightProgram.use();
		lightProgram.setUniform("model", model);
		lightProgram.setUniform("lightColor", lightColor);
		lightProgram.setUniform("view", view);
		lightProgram.setUniform("projection", projection);
		lightProgram.setUniform("lightPos",lightPos);
		lightMesh.draw();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(pWindow); 
		glfwPollEvents();

		lastTime = currentTime;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void UILoader(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		if (ImGui::CollapsingHeader("Directional Light"))
		{

		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	
	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

bool initOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL)
		{
			pWindow = glfwCreateWindow(pVmode->width, pVmode->height, title, pMonitor, NULL);
		}
	}
	else
	{
		pWindow = glfwCreateWindow(pwidth, pheight, title, NULL, NULL);
	}
	if (pWindow == NULL)
	{
		std::cerr << "Failed to create a Window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(pWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(pWindow, glfw_onFramebufferSize);
	glfwSetCursorPosCallback(pWindow, glfw_onMouseMove);
	glfwSetScrollCallback(pWindow,glfw_scrollInput);

	// Hides and grabs cursor, unlimited movement
	glfwSetCursorPos(pWindow, pwidth / 2.0, pheight / 2.0);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	glViewport(0, 0, pwidth, pheight);

	glEnable(GL_DEPTH_TEST);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window,GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		gWireframe = !gWireframe;
	if (gWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	pwidth = width;
	pheight = height;
	glViewport(0, 0, width, height);
}

void showFPS(GLFWwindow* window)
{
	static double prevSec = 0.0;
	static int frameCount = 0;
	double elapsedSec;
	double currentSec = glfwGetTime();

	elapsedSec = currentSec - prevSec;

	//update the FPS 4times in a second
	if (elapsedSec >= 0.25)
	{
		prevSec = currentSec;
		double fps = (double)frameCount / elapsedSec;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< title << " "
			<< "FPS: " << fps << " "
			<< "FrameTime:" << msPerFrame << "(ms)";
		glfwSetWindowTitle(window, outs.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}

void glfw_onMouseMove(GLFWwindow* window, double posX, double posY)
{
	static glm::vec2 lastMousePos = glm::vec2(0, 0);
	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE)==1)
	{
		// each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
			gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
			gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	}
	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1)
	{
		// each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
		orbitCamera.move(MOUSE_SENSITIVITY*0.5f*glm::vec3((float)posX - lastMousePos.x, (float)posY - lastMousePos.y, 0.0f));
	}

	// Change orbit camera radius with the Right Mouse Button
	/*if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT)==1)
	{
		float dx = 0.01f * ((float)posX - lastMousePos.x);
		float dy = 0.01f * ((float)posY - lastMousePos.y);
		gRadius += dx - dy;
	}*/

	lastMousePos.x = (float)posX;
	lastMousePos.y = (float)posY;
}


void glfw_scrollInput(GLFWwindow* window, double Xoff, double Yoff)
{
	gRadius -= Yoff;

	//double fov = fpsCamera.getFOV() + Yoff * ZOOM_SENSITIVITY;

	//fov = glm::clamp(fov, 1.0, 120.0);

	//fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
/*
void update(double elapsedTime,double *lastmouseX, double * lastmouseY)
{
	double mouseX, mouseY;
	double lastX = *lastmouseX;
	double lastY = *lastmouseY;
	glfwGetCursorPos(pWindow, &mouseX, &mouseY);
	fpsCamera.rotate((float)(lastX - mouseX) * MOUSE_SENSITIVITY, (float)(lastY - mouseY) * MOUSE_SENSITIVITY);
	glfwSetCursorPos(pWindow,mouseX, mouseY);

	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		fpsCamera.move((float)elapsedTime * MOVE_SPEED * fpsCamera.getLook());
	}
	else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		fpsCamera.move((float)elapsedTime * MOVE_SPEED * -fpsCamera.getLook());
	}

	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		fpsCamera.move((float)elapsedTime * MOVE_SPEED * fpsCamera.getRight());
	}
	else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		fpsCamera.move((float)elapsedTime * MOVE_SPEED *-fpsCamera.getRight());
	}

	// Up/down
	if (glfwGetKey(pWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	else if (glfwGetKey(pWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
	*lastmouseX=mouseX;
	*lastmouseY=mouseY;
}
*/