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
#include "ModuleScene.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"

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

	bool want_to_save_config = false;

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


private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	//bool SaveConfig();

};

#endif //_APPLICATION_H_