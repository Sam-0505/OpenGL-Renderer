#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#define	GLEW_STATIC
#include "GL/glew.h"// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"
#include "Interface.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// Global Variables
OrbitCamera orbitCamera;
const char* title = "OpenGL Starters";
int pheight = 768;
int pwidth = 1024;
bool gFullScreen = false;
GLFWwindow* pWindow ;
bool gWireframe;
bool pan = false;
bool show_UI = true;
bool showOpen = false;

//const float MOUSE_SENSITIVITY = 0.25f;
float gRadius = 10.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;
double lastmouseX = pwidth / 2.0; 
double lastmouseY = pheight / 2.0;

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
void saveImage();

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

	Interface UI;
	UI.initImGui(pWindow);
	/*
	Mesh mesh0, mesh1, mesh2, mesh3, mesh4, mesh5,mesh6;
	std::vector<Mesh> mesh;
	mesh.push_back(mesh0);
	mesh.push_back(mesh1);
	mesh.push_back(mesh2);
	mesh.push_back(mesh3);
	mesh.push_back(mesh4);
	mesh.push_back(mesh5);
	//mesh.push_back(mesh6);
	*/
	int c = 6;
	Mesh mesh[20];
	mesh[0].loadOBJ("../models/crate.obj"); 
	mesh[1].loadOBJ("../models/woodcrate.obj");
	mesh[2].loadOBJ("../models/robot.obj");
	mesh[3].loadOBJ("../models/floor.obj");
	mesh[4].loadOBJ("../models/bowling_pin.obj");
	mesh[5].loadOBJ("../models/bunny.obj");
	mesh[0].name="crate";
	mesh[1].name="woodcrate";
	mesh[2].name="robot";
	mesh[3].name="floor";
	mesh[4].name="bowling_pin";
	mesh[5].name="bunny";

	/*
	Texture2D texture0, texture1, texture2, texture3, texture4, texture5, texture6;
	std::vector<Texture2D> texture2D;
	texture2D.push_back(texture0);
	texture2D.push_back(texture1);
	texture2D.push_back(texture2);
	texture2D.push_back(texture3);
	texture2D.push_back(texture4);
	texture2D.push_back(texture5);
	//texture2D.push_back(texture6);
	*/
	Texture2D texture2D[20];
	texture2D[0].loadTexture("../textures/crate.jpg", true);
	texture2D[1].loadTexture("../textures/woodcrate_diffuse.jpg", true);
	texture2D[2].loadTexture("../textures/robot_diffuse.jpg", true);
	texture2D[3].loadTexture("../textures/tile_floor.jpg", true);
	texture2D[4].loadTexture("../textures/AMF.tga", true);
	texture2D[5].loadTexture("../textures/bunny_diffuse.jpg", true);

	//Model Position
	glm::vec3 modPos[20];
	modPos[0]=glm::vec3(-2.5f, 1.0f, 0.0f);	// crate1
	modPos[1]=glm::vec3(2.5f, 1.0f, 0.0f);	// crate2
	modPos[2]=glm::vec3(0.0f, 1.0f, 0.0f);	// robot
	modPos[3]=glm::vec3(0.0f, 0.0f, 0.0f);		// floor
	modPos[4]=glm::vec3(0.0f, 0.0f, 2.0f);	// pin
	modPos[5]=glm::vec3(-2.0f, 0.0f, 2.0f);// bunny

		//Model Position
	glm::vec3 modRot[20];
	modRot[0] = glm::vec3(30.0f);	// crate1
	modRot[1] = glm::vec3(0.0f);	// crate2
	modRot[2] = glm::vec3(0.0f);	// robot
	modRot[3] = glm::vec3(0.0f);		// floor
	modRot[4] = glm::vec3(0.0f);	// pin
	modRot[5] = glm::vec3(0.0f);// bunny

	// Model scale
	glm::vec3 modScale[20];
	modScale[0]=glm::vec3(1.0f, 1.0f, 1.0f);	// crate1
	modScale[1]=glm::vec3(1.0f, 1.0f, 1.0f);	// crate2
	modScale[2]=glm::vec3(1.0f, 1.0f, 1.0f);	// robot
	modScale[3]=glm::vec3(10.0f, 1.0f, 10.0f);	// floor
	modScale[4]=glm::vec3(0.1f, 0.1f, 0.1f);	// pin
	modScale[5]=glm::vec3(0.7f, 0.7f, 0.7f);	// bunny

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("../shaders/basic.vert", "../shaders/basic.frag");

	double lastTime = glfwGetTime();
	glm::vec3 viewPos = orbitCamera.getViewPos();

	while (!glfwWindowShouldClose(pWindow))
	{
		int del=-1;
		if(!show_UI)
			saveImage();

		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		//update(deltaTime,&lastmouseX,&lastmouseY);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texture2D1.bind(0);

		if (show_UI) //to remove the UI when the image is rendered
			del=UI.UILoader(mesh,modPos,modRot,modScale,c);//del is the index number of the object to remove

		if (del != -1)
		{
			for (int i = del; i < c; i++)
			{
				mesh[del].mVAO = mesh[del + 1].mVAO;
				mesh[del].mVBO = mesh[del + 1].mVBO;
				mesh[del].name = mesh[del + 1].name;
				texture2D[del].mTexture = texture2D[del + 1].mTexture;
				modPos[del] = modPos[del + 1];
				modScale[del] = modScale[del + 1];
			}
			mesh[c].mLoaded = false;
			texture2D[c].mTexture = 0;
			c--;
		}

		if (showOpen)//showOpen is true means a asset has to be imported
		{
			std::string file[3];
			int isImport= UI.importFile(file);
			if (isImport)
			{
				std::cout << "Hey" << std::endl;
				std::cout << c << std::endl;
				std::cout << file[0] << std::endl;
				mesh[c].loadOBJ(file[0]);
				mesh[c].name=file[2];
				std::cout << c << std::endl;
				std::cout << file[1] << std::endl;
				texture2D[c].loadTexture(file[1], true);
				modPos[c] = glm::vec3(0.0f, 5.0f, 0.0f);
				modScale[c] = glm::vec3(1.0f);
				c++;
				std::cout << c << std::endl;
				showOpen = false;
			}
		}
		

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

		UI.setShaderValues(&shaderProgram);

		for (int i = 0; i < c; i++)
		{
			model = glm::translate(glm::mat4(), modPos[i]) *glm::rotate(glm::mat4(),glm::radians(modRot[i].x),glm::vec3(1.0f,0.0f,0.0f))* glm::rotate(glm::mat4(), glm::radians(modRot[i].y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(), glm::radians(modRot[i].z), glm::vec3(0.0f, 0.0f, 1.0f)) *glm::scale(glm::mat4(), modScale[i]);
			shaderProgram.setUniform("model", model);

			shaderProgram.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			shaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.setUniformSampler("material.diffuseMap", 0);
			shaderProgram.setUniform("material.shininess", 32.0f);

			texture2D[i].bind(0);
			mesh[i].draw();
			texture2D[i].unbind(0);
		}

		if (show_UI) 
			UI.draw();

		glfwSwapBuffers(pWindow);

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
	glfwSetCursorPos(pWindow, pwidth / 2.0, pheight / 2.0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		show_UI = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		show_UI = true;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		showOpen = true;
	}
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

void saveImage() //save the current vieww as png image
{
	int k = 0;
	std::string filename;
	while (true)
	{
		filename = "../Downloads/image" + std::to_string(k) + ".png";
		std::ifstream isfile;
		isfile.open(filename);
		if (!isfile)
		{
			std::cout << "File can't open" << std::endl;
			break;
		}
		k++;
	}
	int width, height;
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * pwidth;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * pheight;
	std::vector<char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, pwidth, pheight, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
	stbi_flip_vertically_on_write(true);
	const char* filepath = filename.c_str();
	stbi_write_png(filepath, pwidth, pheight, nrChannels, buffer.data(), stride);
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