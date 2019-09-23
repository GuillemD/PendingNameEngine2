#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules; 

	std::string app_name;
	std::string org_name;
	std::string app_version;

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
	
	

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;
#endif //_APPLICATION_H_