#include "Application.h"
#include "ModuleGUI.h"


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	bool ret = true;
	ImGui_ImplSdl_Init(App->window->window);
	return ret;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	//create main menu
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	bool ret = true;

	ImGui_ImplSdl_Shutdown();

	return ret;
}
