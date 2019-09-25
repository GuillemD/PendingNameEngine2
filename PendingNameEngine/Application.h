#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleGUI* gui;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules; 

	std::string app_name;
	std::string org_name;
	std::string app_version;

	std::vector<float>	fps_buffer;
	std::vector<float>	ms_buffer;
	std::vector<float>	memory_buffer;

	uint total_frame_count = 0;
	uint this_sec_frame_count = 0;
	uint last_sec_frame_count = 0;
	Uint32 last_sec_ms = 0;

	Timer last_second_frame_timer;
	Timer ms_since_start;

	float average_fps = -1.0f;

	bool vsync = VSYNC;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void OpenBrowser(const char* url);
	void OpenFile(const char* path);
	const char* GetVersion() const;
	void SetVersion(const char* version) { app_version = version; }
	const char* GetAppName() const;
	void SetAppName(const char* name) { app_name = name; }
	const char* GetOrgName() const;
	void SetOrgName(const char* org) { org_name = org; }

	void ShowApplicationConfig();
	
	

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif //_APPLICATION_H_