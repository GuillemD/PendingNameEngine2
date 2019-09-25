#ifndef _MODULEGUI_H_
#define _MODULEGUI_H_

#include "Module.h"
#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"

#include "PCG_c_basic/pcg_basic.h"

#include "MathGeoLib/include/MathGeoLib.h"

class PanelConsole;
class PanelConfig;
class Panel;


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

	void DrawGUI();

private:
	//Functions
	void CreateMainMenu();

	void ShowDemoWindow();
	void ShowRNG();
	void ShowConsole();
	void ShowAbout();
	void ShowConfig();

public:
	pcg32_random_t seed = PCG32_INITIALIZER;
	int input_min = 0; int input_max = 100;
	int rand_bounded_int = 0;
	float rand_float = 0.0f;


	PanelConsole* console = nullptr;
	PanelConfig* config = nullptr;

	//tmp needs to be moved somewhere else
	std::vector<AABB> aabb_array;


private:
	//Variables
	bool want_to_quit = false;
	bool show_demo_window = false;
	bool show_rng = false;
	bool show_about = false;
	

};

#endif // _MODULEGUI_H_
