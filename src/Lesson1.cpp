#include <iostream>
#include <sstream>
#define	GLEW_STATIC
#include "GL/glew.h"// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "ShaderProgram.h"
#include"Texture2D.h"

// Global Variables
const char* title = "OpenGL Starters";
const int height = 600;
const int width = 800;
bool gFullScreen = false;
GLFWwindow* pWindow ;
bool gWireframe;

//Shaders
const std::string texture1 = "D:/OpenGL/Project1/textures/airplane.png";
const std::string texture2 = "D:/OpenGL/Project1/textures/crate.jpg";

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
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
	GLfloat vertices[] =
	{
		//triangle 1	//tex_coords
		-0.5f,0.5f,0.0f,0.0f,1.0f,//topleft
		0.5f,-0.5f,0.0f,1.0f,0.0f,//bottomright
		-0.5f,-0.5f,0.0f,0.0f,0.0f,//bottomleft
		0.5f,0.5f,0.0f,1.0f,1.0f//topright
	};

	// 2. Set up buffers on the GPU
	GLuint indices[]=
	{
  		0,2,1,//triangle1
		1,3,0 //triangle2
	};

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
	 
	// Set up index buffer
	glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert","basic.frag");

	Texture2D texture2D1;
	texture2D1.loadTexture(texture1, true);

	Texture2D texture2D2;
	texture2D2.loadTexture(texture2, true);
	
	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);
		
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT);

		texture2D1.bind(0);
		texture2D2.bind(1);

		shaderProgram.use();

		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

		GLfloat time = glfwGetTime();
		GLfloat blueColor = (sin(time) / 2) + 0.5f;
		glm::vec2 pos;
		pos.x = sin(time) / 2;
		pos.y = cos(time) / 2;
		shaderProgram.setUniform("posOffset", pos);
		shaderProgram.setUniform("vertColor", glm::vec4(0.0f, 0.0f, blueColor, 1.0f));

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glfwSwapBuffers(pWindow); 
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
		pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
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
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
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