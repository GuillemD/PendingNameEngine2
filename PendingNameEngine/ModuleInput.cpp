#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput( bool start_enabled)
{
	name = "Input";
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();
	mouse_x_motion = mouse_y_motion = 0;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN; //Print KEY Log
				PrintInputLog(i, KEY_DOWN);
			}
			else if(keyboard[i] != KEY_REPEAT)
			{
				PrintInputLog(i, KEY_REPEAT);
				keyboard[i] = KEY_REPEAT;
			}
				
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				PrintInputLog(i, KEY_UP);
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		
		if (mouse_buttons[i] == KEY_DOWN)
		{
			PrintInputLog(1000 + i, KEY_DOWN);
			PrintInputLog(1000 + i, KEY_REPEAT);
			mouse_buttons[i] = KEY_REPEAT;
		}
		if (mouse_buttons[i] == KEY_UP)
		{
			mouse_buttons[i] = KEY_IDLE;
			PrintInputLog(1000 + i, KEY_UP);
		}
		
	}

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0)
	{

		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x_motion = e.motion.xrel;
			mouse_y_motion = e.motion.yrel;
			mouse_x = e.motion.x;
			mouse_y = e.motion.y;
			break;
			
			case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[e.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
			mouse_buttons[e.button.button - 1] = KEY_UP;
			break; 

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::ShowInputConfig()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		
		ImGui::Columns(3, "mouse info");
		ImGui::Text("Mouse Position: ");
		ImGui::TextColored(YELLOW, "X: %d", App->input->mouse_x);
		ImGui::TextColored(YELLOW, "Y: %d", App->input->mouse_y);
		ImGui::NextColumn();
		ImGui::Text("Mouse Motion: ");
		ImGui::TextColored(YELLOW, "X: %d", App->input->mouse_x_motion);
		ImGui::TextColored(YELLOW, "Y: %d", App->input->mouse_y_motion);
		ImGui::NextColumn();
		ImGui::Text("Mouse Wheel: ");
		ImGui::TextColored(YELLOW, "%d", App->input->mouse_z);
		
		ImGui::Columns(1);
		ImGui::Separator();
		
		ImGui::BeginChild("Input Log");
		ImGui::TextUnformatted(input_log_buf.begin());
		if (scroll_log)
			ImGui::SetScrollHere(1.0f);
		scroll_log = false;
		ImGui::EndChild();
	}
	
}

void ModuleInput::PrintInputLog(uint key, uint state)
{
	static char entry[512];
	static const char* states[] = { "IDLE", "DOWN", "REPEAT", "UP" };

	if (key < 1000)
		sprintf_s(entry, 512, "Kybrd: %03u - %s\n", key, states[state]);
	else
		sprintf_s(entry, 512, "Mouse: %03u - %s\n", key - 1000, states[state]);

	AddInputToBuffer(entry);
}

void ModuleInput::AddInputToBuffer(const char * entry)
{

	
	input_log_buf.appendf(entry);
	scroll_log = true;
}
