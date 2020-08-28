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

Interface::Interface()
{
	dambient=ImVec4(0.2f,0.2,0.2f,1.0f);
	dspecular=ImVec4(1.0f,1.0f,1.0f,1.0f);
	ddiffuse= ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ddir[0] = 1.0f;
	ddir[1] = 1.0f;
	ddir[2] = 0.0f;

	ppos[0] = 0.0f;
	ppos[1] = 1.0f;
	ppos[2] = 10.0f;
	pambient=ImVec4(0.2f,0.2,0.2f,1.0f);
	pspecular=ImVec4(1.0f,1.0f,1.0f,1.0f);
	pdiffuse= ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

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

void Interface::UILoader()
{
	//ImFont* font1 = io.Fonts->AddFontDefault();
	//ImFont* font2 = io.Fonts->AddFontFromFileTTF("D:/OpenGL/Project1/common/fonts/segoe-ui-4-cufonfonts/Segoe UI Bold.ttf", 16.0f);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(1.0f,1.0f,1.0f,1.0f);
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	static float f = 0.0f;
	static int counter = 0;
	bool show_dirlight = false;
	bool show_plight = false;
	bool show_slight = false;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	if (ImGui::CollapsingHeader("Asset"))
	{
		static int selected = -1;
		for (int n = 0; n < 5; n++)
		{
			char buf[32];
			sprintf_s(buf, "Object %d", n);
			if (ImGui::Selectable(buf, selected == n))
				selected = n;
		}
	}
	if (ImGui::CollapsingHeader("Lights"))
	{
		static int selected = -1;
		char buf[32];
		sprintf_s(buf, "Spot Light", 0);
		if (ImGui::Selectable(buf, selected == 0))
		{
			selected = 0;
		}
		sprintf_s(buf, "Directional Light", 1);
		if (ImGui::Selectable(buf, selected == 1))
		{
			selected = 1;
		}
		sprintf_s(buf, "Point Light", 2);
		if (ImGui::Selectable(buf, selected == 2))
		{
			selected = 2;
		}
		if (selected == 1)
			show_dirlight = true;
	}
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::DragFloat3("Direction", ddir, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", (float*)&dambient);
		ImGui::ColorEdit3("Specular", (float*)&dspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&ddiffuse);
	}
	if (ImGui::CollapsingHeader("Point Light"))
	{
		ImGui::DragFloat3("Position", ppos, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Specular", (float*)&pspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&pdiffuse);
		ImGui::ColorEdit3("Ambient", (float*)&pambient);
	}

	/*
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::DragFloat3("Position", ppos, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", pambient);
		ImGui::ColorEdit3("Specular", pspecular);
		ImGui::ColorEdit3("Diffuse", pdiffuse);
	}
	*/

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Hello None");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Hello!");
	ImGui::PushFont(font2);
	ImGui::Text("Hello!");
	ImGui::PopFont();
	if (show_dirlight == true)
	{
		ImGui::DragFloat3("Direction", ddir, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", (float*)&dambient);
		ImGui::ColorEdit3("Specular", (float*)&dspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&ddiffuse);
	}
	if (show_plight == true)
	{
		ImGui::DragFloat3("Position", ppos, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Specular", (float*)&pspecular);
		ImGui::ColorEdit3("Diffuse", (float*)&pdiffuse);
		ImGui::ColorEdit3("Ambient", (float*)&pambient);
	}

	/*
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::DragFloat3("Position", ppos, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", pambient);
		ImGui::ColorEdit3("Specular", pspecular);
		ImGui::ColorEdit3("Diffuse", pdiffuse);
	}
	*/

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Interface::setShaderValues(ShaderProgram* shaderProgram)
{
	shaderProgram->setUniform("dlight.ambient", vec3Convert(dambient));
	shaderProgram->setUniform("dlight.specular", vec3Convert(dspecular));
	shaderProgram->setUniform("dlight.diffuse", vec3Convert(ddiffuse));
	shaderProgram->setUniform("dlight.direction", glm::make_vec3(ddir));

	shaderProgram->setUniform("plight.position", glm::make_vec3(ppos));
	shaderProgram->setUniform("plight.ambient", vec3Convert(pambient));
	shaderProgram->setUniform("plight.specular", vec3Convert(pspecular));
	shaderProgram->setUniform("plight.diffuse", vec3Convert(pdiffuse));
	shaderProgram->setUniform("plight.constant", 1.0f);
	shaderProgram->setUniform("plight.linear", 0.07f);
	shaderProgram->setUniform("plight.quad", 0.017f);

	shaderProgram->setUniform("slight.position", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderProgram->setUniform("slight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shaderProgram->setUniform("slight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderProgram->setUniform("slight.diffuse", glm::vec3(1.0f, 0.8f, 0.3f));
	shaderProgram->setUniform("slight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
	shaderProgram->setUniform("slight.cosInnerCone", cos(glm::radians(30.0f)));
	shaderProgram->setUniform("slight.cosOuterCone", cos(glm::radians(60.0f)));
	shaderProgram->setUniform("slight.constant", 1.0f);
	shaderProgram->setUniform("slight.linear", 0.07f);
	shaderProgram->setUniform("slight.quad", 0.017f);
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