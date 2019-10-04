#ifndef _MODULEWINDOW_H_
#define _MODULEWINDOW_H_

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool Start();
	bool CleanUp();

	bool Save(Document& doc, FileWriteStream& os);
	bool Load(Document* doc);

	void ShowWindowConfig();

	void SetTitle(const char* title);

	uint GetWidth() const { return screen_w; }
	uint GetHeight() const { return screen_h; }
	void SetWidth(uint width);
	void SetHeight(uint height);
	void SetWindowSize(uint w, uint h);

	void SetFullscreen(bool fs);
	void SetResizable(bool res);
	void SetBorderless(bool brdr);
	void SetFullscreenDesktop(bool fs_desk);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	bool fullscreen = false;
	bool borderless = false;
	bool resizable = true;
	bool fs_desktop = false;

	SDL_DisplayMode dm;

	uint screen_w = SCREEN_WIDTH;
	uint screen_h = SCREEN_HEIGHT;

	uint min_h = 480;
	uint min_w = 640;
	uint max_h = 2000;
	uint max_w = 3000;

	bool loaded_window_config = false;
};

#endif // _MODULEWINDOW_H_

