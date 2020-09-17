#include "Interface.h"
#include "GLFW/glfw3.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "ImGui/imconfig.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.cpp"
#include "ImGui/imgui_impl_glfw.cpp"
#include <sstream>
#include <fstream>

Interface::Interface()
{
	struct Dir_light dlight_new;
	struct Spot_light slight_new;
	struct Point_light plight_new;
	dlight_new.dambient=ImVec4(0.2f,0.2,0.2f,1.0f);
	dlight_new.dspecular=ImVec4(1.0f,1.0f,1.0f,1.0f);
	dlight_new.ddiffuse= ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	dlight_new.ddir=glm::vec3(1.0f, 1.0f, 0.0f);
	dlight_new.show = false;
	dlight.push_back(dlight_new);

	plight_new.ppos=glm::vec3(1.0f, 1.0f, 10.0f);
	plight_new.pambient=ImVec4(0.2f,0.2,0.2f,1.0f);
	plight_new.pspecular=ImVec4(1.0f,1.0f,1.0f,1.0f);
	plight_new.pdiffuse=ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	plight_new.show = false;
	plight.push_back(plight_new);

	slight_new.spos=glm::vec3(0.0f,1.0f,5.0f);
	slight_new.sambient=glm::vec3(0.2f);
	slight_new.sspecular=glm::vec3(1.0f);
	slight_new.sdiffuse=glm::vec3(1.0f);
	slight_new.sdir=glm::vec3(1.0f);
	slight_new.innerAngle=30.0f;
	slight_new.outerAngle=60.0f;
	slight_new.show = false;
	slight.push_back(slight_new);
	
	Window = NULL;
	font1 = NULL;
	font2 = NULL;
}

Interface::~Interface()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool Interface::initImGui(GLFWwindow* pWindow)
{
	Window = pWindow;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	font1 = io.Fonts->AddFontDefault();
	font2 = io.Fonts->AddFontFromFileTTF("D:/OpenGL/Project1/common/fonts/segoe-ui-4-cufonfonts/Segoe UI Bold.ttf", 16.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

int Interface::UILoader(Mesh mesh[], glm::vec3 modPos[],glm::vec3 modScale[],int c)
{
	int del = -1;
	//ImFont* font1 = io.Fonts->AddFontDefault();
	//ImFont* font2 = io.Fonts->AddFontFromFileTTF("D:/OpenGL/Project1/common/fonts/segoe-ui-4-cufonfonts/Segoe UI Bold.ttf", 16.0f);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("My First Tool");
	bool show_demo_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(1.0f,1.0f,1.0f,1.0f);
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow(&show_demo_window);

	if (ImGui::Button("Add a Directional Light"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		create_dirlight();
	if (ImGui::Button("Add a Spot Light"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		create_spotlight();
	if (ImGui::Button("Add a Point Light"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		create_pointlight();

	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	if (ImGui::CollapsingHeader("Asset"))
	{
		static int selected = -1;
		for (int n = 0; n <c; n++)
		{
			char buf[32];
			if (mesh[n].name == "")
			{
				mesh[n].name = "Object" + n;
			}
			sprintf_s(buf, mesh[n].name.c_str(), n);
			if (ImGui::Selectable(buf, selected == n))
				selected = n;
		}
		for (int n = 0; n < c; n++)
		{
			if (selected == n)
			{
				ImGui::Begin("Light Parameters");                          // Create a window called "Hello, world!" and append into it.
					ImGui::DragFloat3("Position",glm::value_ptr(modPos[n]), 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat3("Scale", glm::value_ptr(modScale[n]), 0.01f, 0.0f, 1.0f);
					if (ImGui::Button("Delete"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					{
						del = n;
					}
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
		}
	}
	if (ImGui::CollapsingHeader("Lights"))
	{
		static int selected = -1;
		char buf[32];
		sprintf_s(buf, "Spot Light", 0);
		for (int n = 0; n < dlight.size(); n++)
		{
			sprintf_s(buf, "Directional Light %d", n);
			if (ImGui::Selectable(buf, selected == n))
			{
				selected = n;
			}
		}
		for (int n = dlight.size(); n < dlight.size() + plight.size(); n++)
		{
			sprintf_s(buf, "Point Light %d", n- dlight.size());
			if (ImGui::Selectable(buf, selected == n))
			{
				selected = n;
			}
		}

		for (int n = dlight.size() + plight.size(); n < dlight.size() + plight.size() + slight.size(); n++)
		{
			sprintf_s(buf, "Spot Light %d", n- dlight.size() + plight.size());
			if (ImGui::Selectable(buf, selected == n))
			{
				selected = n;
			}
		}
		if (selected > -1)
		{
			if (selected < dlight.size())
			{
				dir_show_par(selected);
			}
			else if (selected < dlight.size() + plight.size())
			{
				point_show_par(selected-dlight.size());
			}
			else if (selected < dlight.size() + plight.size() + slight.size())
			{
				spot_show_par(selected - dlight.size() - plight.size());
			}
		}
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	return del;
}

void Interface::setShaderValues(ShaderProgram* shaderProgram)
{
	shaderProgram->setUniform("Dir_num", (int)dlight.size());
	shaderProgram->setUniform("Point_num", (int)plight.size());
	shaderProgram->setUniform("Spot_num", (int)slight.size());
	for (int i = 0; i < dlight.size(); i++)
	{
		//Directional Light Uniforms
		std::string damb = "dlight[" + std::to_string(i) + "].ambient"; //create a string for every index no. to pass onto uniform
		shaderProgram->setUniform(damb.c_str(), vec3Convert(dlight[i].dambient));

		std::string dspec = "dlight[" + std::to_string(i) + "].specular";
		shaderProgram->setUniform(dspec.c_str(), vec3Convert(dlight[i].dspecular));

		std::string ddif = "dlight[" + std::to_string(i) + "].diffuse";
		shaderProgram->setUniform(ddif.c_str(), vec3Convert(dlight[i].ddiffuse));

		std::string dd = "dlight[" + std::to_string(i) + "].direction";
		shaderProgram->setUniform(dd.c_str(), dlight[i].ddir);
	}

	for (int i = 0; i < plight.size(); i++)
	{
		//Point Light uniforms setting
		std::string pp = "plight[" + std::to_string(i) + "].position";
		shaderProgram->setUniform(pp.c_str(), plight[i].ppos);

		std::string pamb = "plight[" + std::to_string(i) + "].ambient";
		shaderProgram->setUniform(pamb.c_str(), vec3Convert(plight[i].pambient));

		std::string pspec = "plight[" + std::to_string(i) + "].specular";
		shaderProgram->setUniform("plight[0].specular", vec3Convert(plight[i].pspecular));

		std::string pdif = "plight[" + std::to_string(i) + "].diffuse";
		shaderProgram->setUniform(pdif.c_str(), vec3Convert(plight[i].pdiffuse));

		std::string pconst = "plight[" + std::to_string(i) + "].constant";
		shaderProgram->setUniform(pconst.c_str(), 1.0f);

		std::string plin = "plight[" + std::to_string(i) + "].linear";
		shaderProgram->setUniform(plin.c_str(), 0.07f);

		std::string pquad = "plight[" + std::to_string(i) + "].quad";
		shaderProgram->setUniform(pquad.c_str(), 0.017f);
	}

	for (int i = 0; i < slight.size(); i++)
	{
		//Spot Light uniforms setting
		std::string samb = "slight[" + std::to_string(i) + "].ambient"; //create a string for every index no. to pass onto uniform
		shaderProgram->setUniform(samb.c_str(), slight[i].sambient);

		std::string sspec = "slight[" + std::to_string(i) + "].specular";
		shaderProgram->setUniform(sspec.c_str(), slight[i].sspecular);

		std::string sdif = "slight[" + std::to_string(i) + "].diffuse";
		shaderProgram->setUniform(sdif.c_str(), slight[i].sdiffuse);

		std::string sd = "slight[" + std::to_string(i) + "].direction";
		shaderProgram->setUniform(sd.c_str(), slight[i].sdir);

		//Point Light uniforms setting
		std::string sp = "slight[" + std::to_string(i) + "].position";
		shaderProgram->setUniform(sp.c_str(), slight[i].spos);

		std::string sic = "slight[" + std::to_string(i) + "].cosInnerCone";
		shaderProgram->setUniform(sic.c_str(), glm::radians(glm::cos(slight[i].innerAngle)));

		std::string soc = "slight[" + std::to_string(i) + "].cosOuterCone";
		shaderProgram->setUniform(soc.c_str(), glm::radians(glm::cos(slight[i].outerAngle)));

		std::string sconst = "slight[" + std::to_string(i) + "].constant";
		shaderProgram->setUniform(sconst.c_str(), 1.0f);

		std::string slin = "slight[" + std::to_string(i) + "].linear";
		shaderProgram->setUniform(slin.c_str(), 0.07f);

		std::string squad = "slight[" + std::to_string(i) + "].quad";
		shaderProgram->setUniform(squad.c_str(), 0.017f);
	}
	}

glm::vec3 Interface::vec3Convert(ImVec4 var)
{
	glm::vec3 convert(var.x, var.y, var.z);
	return convert;
}

void Interface::draw()
{

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Interface::dir_show_par(int k)
{
	ImGui::Begin("Light Parameters");                          // Create a window called "Hello, world!" and append into it.
	ImGui::PopFont();
	if (k != -1)
	{
		ImGui::DragFloat3("Direction", glm::value_ptr(dlight[k].ddir), 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", (float*)&dlight[k].dambient);
		ImGui::ColorEdit3("Specular", (float*)&dlight[k].dspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&dlight[k].ddiffuse);
	}
	if (ImGui::Button("Delete"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		dlight.erase(dlight.begin()+k);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
void Interface::point_show_par(int k)
{
	ImGui::Begin("Light Parameters");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	if (k != -1)
	{
		ImGui::DragFloat3("Position", glm::value_ptr(plight[k].ppos), 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Specular", (float*)&plight[k].pspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&plight[k].pdiffuse);
		ImGui::ColorEdit3("Ambient", (float*)&plight[k].pambient);
	}
	if (ImGui::Button("Delete"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		plight.erase(plight.begin() + k);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
void Interface::spot_show_par(int k)
{
	ImGui::Begin("Light Parameters");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	if (k != -1)
	{
		ImGui::DragFloat3("Position", glm::value_ptr(slight[k].spos), 0.1f, 0.0f, 10.0f);
		ImGui::ColorEdit3("Specular", (float*)&slight[k].sspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&slight[k].sdiffuse);
		ImGui::ColorEdit3("Ambient", (float*)&slight[k].sambient);
		ImGui::DragFloat3("Direction", glm::value_ptr(slight[k].sdir), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("InnerAngle", &(slight[k].innerAngle), 5.0f, 0.0f, 180.0f, "%.1f");
		ImGui::DragFloat("InnerAngle", &(slight[k].outerAngle), 5.0f, slight[k].innerAngle, 180.0f, "%.1f");
	}
	if (ImGui::Button("Delete"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		slight.erase(slight.begin() + k);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Interface::create_dirlight()
{
	struct Dir_light dlight_new;
	dlight_new.dambient = ImVec4(0.2f, 0.2, 0.2f, 1.0f);
	dlight_new.dspecular = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	dlight_new.ddiffuse = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	dlight_new.ddir = glm::vec3(1.0f, 1.0f, 0.0f);
	dlight_new.show = false;
	dlight.push_back(dlight_new);
}
void Interface::create_pointlight()
{
	struct Point_light plight_new;
	plight_new.ppos = glm::vec3(1.0f, 1.0f, 10.0f);
	plight_new.pambient = ImVec4(0.2f, 0.2, 0.2f, 1.0f);
	plight_new.pspecular = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	plight_new.pdiffuse = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	plight_new.show = false;
	plight.push_back(plight_new);
}
void Interface::create_spotlight()
{
	struct Spot_light slight_new;
	slight_new.spos = glm::vec3(0.0f, 1.0f, 5.0f);
	slight_new.sambient = glm::vec3(0.2f);
	slight_new.sspecular = glm::vec3(1.0f);
	slight_new.sdiffuse = glm::vec3(1.0f);
	slight_new.sdir = glm::vec3(1.0f);
	slight_new.innerAngle = 30.0f;
	slight_new.outerAngle = 60.0f;
	slight_new.show = false;
	slight.push_back(slight_new);
}
int Interface::importFile(std::string file[])
{
	bool send_file=false;
	ImGui::Begin("Hello None");                         
	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	
	static char buf1[64] = "";
	ImGui::InputText("Object", buf1, 64);
	
	static char buf2[64] = "";
	ImGui::InputText("Texture", buf2, 64);

	file[0] = buf1;
	file[1] = buf2;
	if (ImGui::Button("Import"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		send_file = true;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	if (send_file)
		return 1;
	else
		return 0;
}
