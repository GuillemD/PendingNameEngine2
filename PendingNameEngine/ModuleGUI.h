#ifndef _MODULEGUI_H_
#define _MODULEGUI_H_

#include "Module.h"
#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"

class ModuleGUI : public Module
{
public:

	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Init();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
};

#endif // _MODULEGUI_H_
