#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	gui = new ModuleGUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(scene);
	AddModule(gui);
	

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		delete (*item);
	}

}

bool Application::Init()
{
	bool ret = true;

	SetAppName(TITLE);
	SetOrgName(ORGANISATION);
	SetVersion(VERSION);

	// Call Init() in all modules
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		ret = (*item)->Start();
	}
	
	ms_timer.Start();
	ms_since_start.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->Update(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
			ret = (*item)->PostUpdate(dt);
	}

	//APPLICATION CONFIGURATION WINDOW
	this_sec_frame_count++;// We add 1 each time we update, 1 time per frame
	total_frame_count++;//This one is used for calculating our average fps, so we add 1 each time also.
	//its important to do this before, otherwise we would show 1 less fps.

	average_fps = total_frame_count / (ms_since_start.Read() / 1000.f);

	if (last_second_frame_timer.Read() >= 1000) { // Basically if a whole 1000ms(1s) has passed restart the frame timer.
		last_second_frame_timer.Start();
		last_sec_frame_count = this_sec_frame_count; //We store this second frames in the previous one
		this_sec_frame_count = 0;					// and then we restart the frame count
	}

	// We will only show 30 bars in our histogram

	if (fps_buffer.size() >= 100)
	{
		for (int a = 0; a <= fps_buffer.size() - 2; a++)
		{
			fps_buffer[a] = fps_buffer[a + 1];
		}
		fps_buffer.pop_back();
	}
	if (ms_buffer.size() >= 100)
	{
		for (int a = 0; a <= ms_buffer.size() - 2; a++)
		{
			ms_buffer[a] = ms_buffer[a + 1];
		}
		ms_buffer.pop_back();
	}

	last_sec_ms = ms_timer.Read();
	ms_buffer.push_back((float)last_sec_ms);
	if (last_sec_ms) fps_buffer.push_back((float)(1000 / last_sec_ms));

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenBrowser(const char * url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::OpenFile(const char * path)
{
	ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOW);
}

const char * Application::GetVersion() const
{
	return app_version.c_str();
}

const char * Application::GetAppName() const
{
	return app_name.c_str();
}

const char * Application::GetOrgName() const
{
	return org_name.c_str();
}

void Application::ShowApplicationConfig()
{
	if(ImGui::CollapsingHeader("Application"))
	{
		char title[30];

		ImGui::Text("App name: ");
		ImGui::SameLine();
		ImGui::TextColored(GREEN, "%s", App->GetAppName());

		ImGui::Text("Org name: ");
		ImGui::SameLine();
		ImGui::TextColored(GREEN, "%s", App->GetOrgName());

		ImGui::Text("Version: ");
		ImGui::SameLine();
		ImGui::TextColored(GREEN, "%s", App->GetVersion());
		ImGui::Separator();

		if (ImGui::Checkbox("Vsync", &vsync)) {
			if (vsync) {
				SDL_GL_SetSwapInterval(1);
			}
			else {
				SDL_GL_SetSwapInterval(0);
			}
		}

		sprintf_s(title, 30, "Frames per second: %.1f", App->fps_buffer[App->fps_buffer.size() - 1]);
		ImGui::PlotHistogram("", &App->fps_buffer[0], App->fps_buffer.size(), 0, title, 0.0f, 120.0f, ImVec2(310, 100));

		sprintf_s(title, 30, "ms: %.1f", App->ms_buffer[App->ms_buffer.size() - 1]);
		ImGui::PlotHistogram("", &App->ms_buffer[0], App->ms_buffer.size(), 0, title, 0.0f, 120.0f, ImVec2(310, 100));
	}
	
}

