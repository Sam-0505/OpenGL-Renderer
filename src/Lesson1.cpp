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

// Global Variables
const char* title = "OpenGL Starters";
int pheight = 768;
int pwidth = 1024;
bool gFullScreen = false;
GLFWwindow* pWindow ;
bool gWireframe;

//const float MOUSE_SENSITIVITY = 0.25f;
float gRadius = 10.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;

//Shaders
//const std::string texture2 = "D:/OpenGL/Project1/textures/airplane.png";
//const std::string floorTexture = "D:/OpenGL/Project1/textures/grid.jpg";

FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;


// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void update(double elapsedTime);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_scrollInput(GLFWwindow* window, double Xoff, double Yoff);
void showFPS(GLFWwindow* window);
bool initOpenGL();

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
	
	const int modelNum = 4;
	Mesh mesh[modelNum];
	Texture2D texture2D[modelNum];

	mesh[0].loadOBJ("D:/OpenGL/Project1/models/tea.obj");
	mesh[1].loadOBJ("D:/OpenGL/Project1/models/woodcrate.obj");
	mesh[2].loadOBJ("D:/OpenGL/Project1/models/robot.obj");
	mesh[3].loadOBJ("D:/OpenGL/Project1/models/floor.obj");

	texture2D[0].loadTexture("D:/OpenGL/Project1/textures/crate.jpg", true);
	texture2D[1].loadTexture("D:/OpenGL/Project1/textures/woodcrate_diffuse.jpg", true);
	texture2D[2].loadTexture("D:/OpenGL/Project1/textures/robot_diffuse.jpg", true);
	texture2D[3].loadTexture("D:/OpenGL/Project1/textures/tile_floor.jpg", true);

	//Model Position
	glm::vec3 modPos[] = 
	{
		glm::vec3(-2.5f, 1.0f, 0.0f),	// crate1
		glm::vec3(2.5f, 1.0f, 0.0f),	// crate2
		glm::vec3(0.0f, 1.0f, 0.0f),	// robot
		glm::vec3(0.0f, 0.0f, 0.0f)		// floor
	};

	// Model scale
	glm::vec3 modScale[] = 
	{
		glm::vec3(1.0f, 1.0f, 1.0f),	// crate1
		glm::vec3(1.0f, 1.0f, 1.0f),	// crate2
		glm::vec3(1.0f, 1.0f, 1.0f),	// robot
		glm::vec3(10.0f, 1.0f, 10.0f)	// floor
	};

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert","basic.frag");

	//OrbitCamera orbitCamera;

	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	
	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		
		glfwPollEvents();
		update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texture2D1.bind(0);

		glm::mat4 model, view, projection;

		// Rotates around the cube center
		

		//orbitCamera.setLookAt(cubePos);
		//orbitCamera.setRadius(gRadius);
		//orbitCamera.rotate(gYaw, gPitch);

		// Create the View matrix
		//view = orbitCamera.getViewMatrix();
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)pwidth / (float)pheight, 0.1f, 100.0f);

		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		shaderProgram.use();

		// Pass the matrices to the shader
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		for (int i = 0; i < modelNum; i++)
		{
			model = glm::translate(glm::mat4(), modPos[i]) * glm::scale(glm::mat4(), modScale[i]);
			shaderProgram.setUniform("model", model);

			texture2D[i].bind(0);
			mesh[i].draw();
			texture2D[i].unbind(0);
		}

		glfwSwapBuffers(pWindow); 
		glfwPollEvents();

		lastTime = currentTime;
	}
	
	glfwTerminate();
	return 0;
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
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(pWindow, pwidth / 2.0, pheight / 2.0);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	glViewport(0, 0, pwidth, pheight);

	glEnable(GL_DEPTH_TEST);

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

	// Update angles based on Left Mouse Button input to orbit around the cube
	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE)==1)
	{
		// each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
		gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	}

	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE)==1)
	{
		// each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
		gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
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
	//gRadius -= Yoff;

	double fov = fpsCamera.getFOV() + Yoff * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
	double mouseX, mouseY;
	glfwGetCursorPos(pWindow, &mouseX, &mouseY);
	fpsCamera.rotate((float)(pwidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(pheight / 2.0 - mouseY) * MOUSE_SENSITIVITY);
	glfwSetCursorPos(pWindow,pwidth/ 2.0, pheight / 2.0);

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
}