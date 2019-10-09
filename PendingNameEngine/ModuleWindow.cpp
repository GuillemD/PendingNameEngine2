#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled)
{
	name = "Window";

}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_GetDesktopDisplayMode(0, &dm);

		//Create window
		if (!loaded_window_config)
		{
			screen_w = uint(dm.w) - 100;
			screen_h = uint(dm.h) - 100;
		}

		max_w = uint(dm.w);
		max_h = uint(dm.h);
		
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fs_desktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetAppName(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

bool ModuleWindow::Start()
{
	SetWindowSize(screen_w, screen_h);
	return true;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::Save(Document & doc, FileWriteStream & os) 
{
	Document::AllocatorType& alloc = doc.GetAllocator();
	Value window(kObjectType);

	window.AddMember("width", screen_w, alloc);
	window.AddMember("height", screen_h, alloc);
	window.AddMember("fullscreen", fullscreen, alloc);
	window.AddMember("resizable", resizable, alloc);
	window.AddMember("borderless", borderless, alloc);
	window.AddMember("fs_desktop", fs_desktop, alloc);

	doc.AddMember("window", window, alloc);

;	return true;
}

bool ModuleWindow::Load(Document* doc) 
{
	bool ret = true;

	Document aux;
	aux.Parse(App->readBuffer);
	ret = aux.IsObject();

	SetWidth(aux["window"]["width"].GetUint());
	SetHeight( aux["window"]["height"].GetUint());

	SetFullscreen(aux["window"]["fullscreen"].GetBool());
	SetResizable(aux["window"]["resizable"].GetBool());
	SetBorderless(aux["window"]["borderless"].GetBool());
	SetFullscreenDesktop(aux["window"]["fs_desktop"].GetBool());
	
	loaded_window_config = true;
	
	return ret;
}

void ModuleWindow::ShowWindowConfig()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::SliderInt("Width", (int*)&screen_w, min_w, max_w))
			SetWidth(screen_w);

		if (ImGui::SliderInt("Height", (int*)&screen_h, min_h, max_h))
			SetHeight(screen_h);

		ImGui::Separator();
		ImGui::Columns(2);
		bool aux1 = fullscreen;
		bool aux2 = fs_desktop;
		if (ImGui::Checkbox("Fullscreen", &aux1))
		{
			SetFullscreen(aux1);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
		{
			SetResizable(resizable);
		}			
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Save and restart to apply");
		}
		if (ImGui::Checkbox("Borderless", &borderless))
		{
			SetBorderless(borderless);			
		}
		if (ImGui::Checkbox("Fullscreen Desktop", &aux2))
		{
			SetFullscreenDesktop(aux2);
		}
		ImGui::NextColumn();

		ImGui::Text("Default resolutions");
		if (ImGui::Button("1280x1024",ImVec2(100,20)))
		{
			SetWindowSize(1280, 1024);
		}
		ImGui::SameLine();
		if (ImGui::Button("1366x768", ImVec2(100,20)))
		{
			SetWindowSize(1366, 768);
		}
		if (ImGui::Button("1440x900", ImVec2(100,20)))
		{
			SetWindowSize(1440, 900);
		}
		ImGui::SameLine();
		if (ImGui::Button("1600x900", ImVec2(100, 20)))
		{
			SetWindowSize(1600, 900);
		}

		ImGui::Columns(1);
		ImGui::Separator();
	}
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, GetHeight());
	screen_w = width;
	if (!fullscreen)
		App->renderer3D->OnResize(screen_w, screen_h);
}

void ModuleWindow::SetHeight(uint height)
{
	SDL_SetWindowSize(window, GetWidth(), height);
	screen_h = height;
	if (!fullscreen)
		App->renderer3D->OnResize(screen_w, screen_h);
}

void ModuleWindow::SetWindowSize(uint w, uint h)
{
	SDL_SetWindowSize(window, w, h);
	screen_w = w;
	screen_h = h;

	if (!fullscreen)
		App->renderer3D->OnResize(screen_w, screen_h);
}

void ModuleWindow::SetFullscreen(bool fs)
{
	if (fs != fullscreen) //If config != previous saved value
	{
		fullscreen = fs;
		if (fullscreen)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) == 0)
				fs_desktop = false;
			else
				CONSOLELOG("Switch to fullscreen failed: %s\n", SDL_GetError());
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				CONSOLELOG("Switch to window mode failed: %s\n", SDL_GetError());
		}
	}
	
}

void ModuleWindow::SetResizable(bool res)
{
	resizable = res;
}

void ModuleWindow::SetBorderless(bool brdr)
{
	borderless = brdr;

	if (borderless)
	{
		if (!fullscreen && !fs_desktop)
			SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		if (!fullscreen && !fs_desktop)
			SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

void ModuleWindow::SetFullscreenDesktop(bool fs_desk)
{
	if (fs_desk != fs_desktop) //avoid mismatch when loading new config
	{
		fs_desktop = fs_desk;
		if (fs_desktop)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) == 0)
				fullscreen = false;
			else
				CONSOLELOG("Switch to fullscreen failed: %s\n", SDL_GetError());
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				CONSOLELOG("Switch to window mode failed: %s\n", SDL_GetError());
		}

	}
	
}
