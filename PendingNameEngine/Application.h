#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "ModuleGUI.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"

#define FPSBUFFER_SIZE 100
#define MSBUFFER_SIZE 100

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleGUI* gui;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleScene* scene;
	ModuleImporter* importer;
	ModuleFileSystem* fs;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules; 

	std::string app_name;
	std::string org_name;
	std::string app_version;

	std::vector<float>	fps_buffer;
	std::vector<float>	ms_buffer;


	uint total_frame_count = 0;
	uint this_sec_frame_count = 0;
	uint last_sec_frame_count = 0;
	uint last_sec_ms = 0;

	Timer last_sec_frame_timer;
	Timer ms_since_start; 

	float average_fps = -1.0f;

	bool vsync = VSYNC;

	float budget_vram = .0f;
	float current_vram = .0f;
	float available_vram = .0f;
	float reserved_vram = .0f;

	uint gpu_vendor = 0;
	uint gpu_device = 0;
	char* gpu_brand = "";


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
	void ShowHardwareConfig();

	bool want_to_save_config = false;
	bool want_to_load_config = false;

	char readBuffer[65536];

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool SaveConfig();
	bool LoadConfig();
	void VRAMUsage();
	char* ConvertWStrToChar(std::wstring& wStr) const;

};

#endif //_APPLICATION_H_