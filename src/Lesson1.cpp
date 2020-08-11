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

// Global Variables
const char* title = "OpenGL Starters";
int pheight = 768;
int pwidth = 1024;
bool gFullScreen = false;
GLFWwindow* pWindow ;
bool gWireframe;

const float MOUSE_SENSITIVITY = 0.25f;
float gRadius = 10.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;

//Shaders
//const std::string texture2 = "D:/OpenGL/Project1/textures/airplane.png";
const std::string texture1 = "D:/OpenGL/Project1/textures/crate.jpg";
const std::string floorTexture = "D:/OpenGL/Project1/textures/grid.jpg";

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
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

	// Set up our quad

	// 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
	GLfloat vertices[] = {
		// position		 // tex coords

	   // front face
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		 // left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 // right face
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  // top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		  // bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	// Cube position
	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 floorPos = glm::vec3(0.0f, 0.0f, 0.0f);
	// 2. Set up buffers on the GPU
	/*GLuint indices[]=
	{
  		0,2,1,//triangle1
		1,3,0 //triangle2
	};*/

	// 2. Set up buffer on the GPU
	GLuint vbo, ibo, vao;

	glGenBuffers(1, &vbo);// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// copy the data from CPU to GPU

	// The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
	// variables to vertex shaders.
	glGenVertexArrays(1, &vao);// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(vao);// Make it the current one
	
	// Position attribute, identified as "0"
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat) * 5,NULL);//(AttribIndex,No. of components,Data type,Normalize?,Stride Length,Offset
	glEnableVertexAttribArray(0);//Enable first Attribute or attribute"0"

	// Texture Coord attribute, identified as "1"
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5,(GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	/*
	// Set up index buffer
	glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert","basic.frag");

	Texture2D texture2D1;
	texture2D1.loadTexture(texture1, true);

	//Texture2D texture2D2;
	//texture2D2.loadTexture(texture2, true);

	Texture2D floorTexture2D;
	floorTexture2D.loadTexture(floorTexture, true);

	OrbitCamera orbitCamera;

	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	
	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture2D1.bind(0);
		//texture2D2.bind(1);

		glm::mat4 model, view, projection;

		// Update the cube position and orientation.  Rotate first then translate
		//cubeAngle += (float)(deltaTime * 50.0f);
		//if (cubeAngle >= 360.0f) cubeAngle = 0.0f;

		// Rotates around the cube center
		model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		// Uncomment this line and comment the one above to flip the transformation of the cube.  Watch what happens!
		// Rotates around??
		//model = glm::rotate(model, glm::radians(gCubeAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, cubePos);

		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);

		orbitCamera.setLookAt(cubePos);
		orbitCamera.setRadius(gRadius);
		orbitCamera.rotate(gYaw, gPitch);

		// Create the View matrix
		view = orbitCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(45.0f), (float)pwidth / (float)pheight, 0.1f, 100.0f);

		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		shaderProgram.use();

		// Pass the matrices to the shader
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
		//glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		floorTexture2D.bind(0);
		model = glm::translate(model, floorPos) * glm::scale(model,glm::vec3(10.0f,0.0f,10.0f));
		shaderProgram.setUniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(pWindow); 

		lastTime = currentTime;
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo);
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
	glfwSetKeyCallback(pWindow, glfw_onKey);

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

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glViewport(0, 0, pwidth, pheight);

	glEnable(GL_DEPTH_TEST);

	return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window,GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
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
	gRadius -= Yoff;
}

