#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "GLFW/glfw3.h"
#include "ImGui/imconfig.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
//#include "ImGui/imgui_impl_opengl3.cpp"
//#include "ImGui/imgui_impl_glfw.cpp"

class Interface
{
public:
	Interface();
	~Interface();

	bool initImGui(GLFWwindow* pWindow);
	void UILoader(Mesh mesh[], glm::vec3 modPos[], glm::vec3 modScale[], int c);
	void setShaderValues(ShaderProgram* shaderProgram);
	GLFWwindow* Window;
	void draw();
	int importFile(std::string file[]);

private:
	struct Dir_light
	{
		ImVec4 dambient;
		ImVec4 dspecular;
		ImVec4 ddiffuse;
		glm::vec3 ddir;
		bool show;
	};
	std::vector<Dir_light> dlight;

	struct Point_light
	{
		ImVec4 pambient;
		ImVec4 pspecular;
		ImVec4 pdiffuse;
		glm::vec3 ppos;
		bool show;
	};
	std::vector<Point_light> plight;

	struct Spot_light
	{
		glm::vec3 spos;
		glm::vec3 sambient;
		glm::vec3 sspecular;
		glm::vec3 sdiffuse;
		glm::vec3 sdir;
		float innerAngle;
		float outerAngle;
		bool show;
	};
	std::vector<Spot_light> slight;

	void dir_show_par(int k);
	void point_show_par(int k);
	void spot_show_par(int k);
	void create_dirlight();
	void create_spotlight();
	void create_pointlight();

	glm::vec3 vec3Convert(ImVec4 var);
	ImFont* font1;
	ImFont* font2;

	const char* glsl_version = "#version 330 core";
};

#endif // INTERFACE_H
