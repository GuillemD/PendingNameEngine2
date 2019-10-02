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

	LoadConfig();

	SDL_SetWindowTitle(App->window->window, app_name.c_str()); //Set app name as window title

	//Resetting FPS BUFFER
	fps_buffer.resize(FPSBUFFER_SIZE);

	for (int i = 0; i < fps_buffer.size(); i++)
	{
		fps_buffer[i] = 0;
	}

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
	//APPLICATION CONFIGURATION WINDOW
	++this_sec_frame_count;// We add 1 each time we update, 1 time per frame
	++total_frame_count;//This one is used for calculating our average fps, so we add 1 each time also.
	//its important to do this before, otherwise we would show 1 less fps.



	if (last_sec_frame_timer.Read() >= 1000) { // Basically if a whole 1000ms(1s) has passed restart the frame timer.
		last_sec_frame_timer.Start();
		this_sec_frame_count = last_sec_frame_count; //We store this second frames in the previous one
		last_sec_frame_count = 0;					// and then we restart the frame count

	}

	average_fps = float(total_frame_count) / (ms_since_start.Read() / 1000.f);


	if (fps_buffer.size() >= FPSBUFFER_SIZE)
	{
		for (int a = 0; a <= fps_buffer.size() - 2; a++)
		{
			fps_buffer[a] = fps_buffer[a + 1];
		}
		fps_buffer.pop_back();
	}
	if (ms_buffer.size() >= MSBUFFER_SIZE)
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

	if (want_to_save_config)
	{
		SaveConfig();
		want_to_save_config = false;
	}
	if (want_to_load_config)
	{
		LoadConfig();
		want_to_load_config = false;
	}
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

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	fps_buffer.clear();
	ms_buffer.clear();
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
		static char name_tmp[100];
		strcpy_s(name_tmp, 100, GetAppName());
		if (ImGui::InputText("App Name", name_tmp, 100, ImGuiInputTextFlags_AutoSelectAll))
		{
			App->SetAppName(name_tmp);
			SDL_SetWindowTitle(App->window->window, app_name.c_str());
		}
			

		static char org_tmp[100];
		strcpy_s(org_tmp, 100, GetOrgName());
		if (ImGui::InputText("Organization", org_tmp, 100, ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrgName(org_tmp);

		static char vs_tmp[20];
		strcpy_s(vs_tmp, 20, GetVersion());
		if (ImGui::InputText("Version", vs_tmp, 20, ImGuiInputTextFlags_AutoSelectAll))
			App->SetVersion(vs_tmp);


		if (ImGui::Checkbox("Vsync", &vsync)) {
			if (vsync) {
				SDL_GL_SetSwapInterval(1);
			}
			else {
				SDL_GL_SetSwapInterval(0);
			}
		}
		char title[30];
		ImVec2 size = ImGui::GetContentRegionAvail();
		sprintf_s(title, 30, "Frames per second: %.1f", fps_buffer[fps_buffer.size()-1]);

		float highest_fps = 0;
		for (int i = 0; i < fps_buffer.size(); i++)
		{
			if (fps_buffer[i] > highest_fps)
				highest_fps = fps_buffer[i];
		}
		float mean_fps = 0;
		int count_fps = 0;
		for (int j = 0; j < fps_buffer.size(); j++)
		{
			if (fps_buffer[j] != 0)
			{
				mean_fps += fps_buffer[j];
				count_fps++;
			}
		}
		mean_fps /= count_fps;
		
		ImGui::PlotHistogram("", &fps_buffer[0], FPSBUFFER_SIZE, 0, title, 0.0f, (highest_fps- mean_fps)+mean_fps+(highest_fps*0.3f), ImVec2(size.x, 100));

		sprintf_s(title, 30, "ms: %.1f", ms_buffer[ms_buffer.size() - 1]);

		float highest_ms = 0;
		for (int i = 0; i < ms_buffer.size(); i++)
		{
			if (ms_buffer[i] > highest_ms)
				highest_ms = ms_buffer[i];
		}
		float mean_ms = 0;
		int count_ms = 0;
		for (int j = 0; j < ms_buffer.size(); j++)
		{
			if (ms_buffer[j] != 0)
			{
				mean_fps += ms_buffer[j];
				count_ms++;
			}
		}
		mean_fps /= count_ms;
		ImGui::PlotHistogram("", &ms_buffer[0], MSBUFFER_SIZE, 0, title, 0.0f, (highest_ms - mean_ms) + mean_ms + (highest_ms*0.3f), ImVec2(size.x, 100));
	}
	//TODO
	//ADD memory consumption graphics and info

}

void Application::ShowHardwareConfig()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version v_compiled;
		SDL_VERSION(&v_compiled);
		ImGui::Text("SDL version: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d.%d.%d", v_compiled.major, v_compiled.minor, v_compiled.patch);
		ImGui::Separator();

		ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", SDL_GetCPUCount());
		ImGui::Text("Cache: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d kb", SDL_GetCPUCacheLineSize());
		ImGui::Text("RAM: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d Gb", SDL_GetSystemRAM());
		ImGui::Separator();
		ImGui::Columns(3, "caps");
		ImGui::Text("Caps: ");
		if (SDL_Has3DNow()) ImGui::TextColored(YELLOW, "3DNow");
		if (SDL_HasAVX()) ImGui::TextColored(YELLOW, "AVX");
		if (SDL_HasAltiVec()) ImGui::TextColored(YELLOW, "AltiVec");
		if (SDL_HasMMX()) ImGui::TextColored(YELLOW, "MMX");
		ImGui::NextColumn();

		if (SDL_HasRDTSC()) ImGui::TextColored(YELLOW, "RDTSC");
		if (SDL_HasSSE()) ImGui::TextColored(YELLOW, "SSE");
		if (SDL_HasSSE2()) ImGui::TextColored(YELLOW, "SSE2");
		ImGui::NextColumn();

		if (SDL_HasSSE3()) ImGui::TextColored(YELLOW, "SSE3"); 
		if (SDL_HasSSE41()) ImGui::TextColored(YELLOW, "SSE41");
		if (SDL_HasSSE42()) ImGui::TextColored(YELLOW, "SSE42");

		ImGui::Columns(1);
		ImGui::Separator();

		VRAMUsage();
		
	}
}

void Application::VRAMUsage() 
{
	
	uint64_t b_vram, c_vram, a_vram, r_vram;

	uint vendor, device;
	std::wstring brand;

	if (getGraphicsDeviceInfo(&vendor, &device, &brand, &b_vram, &c_vram, &a_vram, &r_vram))
	{
		gpu_vendor = vendor;
		gpu_device = device;
		gpu_brand = ConvertWStrToChar(brand);
		budget_vram = (float)b_vram / (1024.0f * 1024.0f);
		current_vram = (float)c_vram / (1024.0f * 1024.0f);
		available_vram = (float)a_vram / (1024.0f * 1024.0f);
		reserved_vram = (float)r_vram / (1024.0f * 1024.0f);
	}
	ImGui::Text("GPU: Vendor"); ImGui::SameLine(); 
	ImGui::TextColored(YELLOW, "%u", gpu_vendor); ImGui::SameLine(); 
	ImGui::Text("Device"); ImGui::SameLine(); 
	ImGui::TextColored(YELLOW, "%u", gpu_device);

	ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", gpu_brand );

	ImGui::Separator();
	ImGui::Text("Budget VRAM:");
	ImGui::SameLine();
	ImGui::TextColored(YELLOW, "%.2f Mb", budget_vram);
	
	ImGui::Text("Current VRAM:");
	ImGui::SameLine();
	ImGui::TextColored(YELLOW, "%.2f Mb", current_vram);
	
	ImGui::Text("Available VRAM:");
	ImGui::SameLine();
	ImGui::TextColored(YELLOW, "%.2f Mb", available_vram);
	
	ImGui::Text("Reserved VRAM:");
	ImGui::SameLine();
	ImGui::TextColored(YELLOW, "%.2f Mb", reserved_vram);

}

char* Application::ConvertWStrToChar(std::wstring & wStr) const
{
	const wchar_t *str = wStr.c_str();
	size_t size = wcslen(str) * 2 + 2;
	char * dest = new char[size];
	size_t c_size;
	wcstombs_s(&c_size, dest, size, str, size);

	return dest;
}

bool Application::SaveConfig()
{
	bool ret = true;

	FILE* fp = fopen("config.json", "wb"); //writebinary
	Document doc;
	char writeBuffer[65536];
	doc.Parse(writeBuffer);
	doc.SetObject();
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	Document::AllocatorType& alloc = doc.GetAllocator();

	Value app(kObjectType);
	Value name_tmp;
	name_tmp.SetString(app_name.c_str(), alloc);
	Value org_tmp;
	org_tmp.SetString(org_name.c_str(), alloc);
	Value version_tmp;
	version_tmp.SetString(app_version.c_str(), alloc);

	app.AddMember("app_name", name_tmp , alloc);
	app.AddMember("organization", org_tmp, alloc);
	app.AddMember("version", version_tmp, alloc);
	doc.AddMember("App", app, alloc);

	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret; it++)
	{
		ret = (*it)->Save(doc, os); 
	}

	Writer<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(fp);

	CONSOLELOG("Configuration saved correctly!");

	return ret;
}

bool Application::LoadConfig()
{
	bool ret = true;
	FILE* fp = fopen("config.json", "rb"); // readbinary
	Document doc;
	char readBuffer[65536];
	FileReadStream os(fp, readBuffer, sizeof(readBuffer));

	
	doc.ParseStream(os);
	doc.IsObject();
	
	
	SetAppName(doc["App"]["app_name"].GetString());
	SetOrgName(doc["App"]["organization"].GetString());
	SetVersion(doc["App"]["version"].GetString());


	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret; it++)
	{
		//ret = (*it)->Load(&doc); TODO
	}
	

	fclose(fp);

	return ret;
}

