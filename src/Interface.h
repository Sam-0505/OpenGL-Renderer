#ifndef INTERFACE_H
#define INTERFACE_H
#include "ShaderProgram.h"
#include "GLFW/glfw3.h"
#include "ImGui/imconfig.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
//#include "ImGui/imgui_impl_opengl3.cpp"
//#include "ImGui/imgui_impl_glfw.cpp"

class Interface
{
public:
	Interface();
	~Interface();

	bool initImGui(GLFWwindow* pWindow);
	void UILoader();
	void removeUI();
	void setShaderValues(ShaderProgram* shaderProgram);
	GLFWwindow* Window;
	glm::vec3 vec3Convert(ImVec4 var);
	void draw();

private:
	ImVec4 dambient;
	ImVec4 dspecular;
	ImVec4 ddiffuse;
	float ddir[3];

	ImVec4 pambient;
	ImVec4 pspecular;
	ImVec4 pdiffuse;
	float ppos[3];
	ImFont* font1;
	ImFont* font2;

	const char* glsl_version = "#version 330 core";
};

#endif // INTERFACE_H
