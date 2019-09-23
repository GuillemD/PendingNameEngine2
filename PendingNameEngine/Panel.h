#ifndef __Panels__
#define __Panels__

#include "ImGui/imgui.h"
#include <string>
class Panel
{
public:
	Panel();
	Panel(const char* _name);
	Panel(int h, int w, int x, int y);
	virtual ~Panel();


	virtual void Draw();
	int GetHeight() { return height; }
	int GetWidth() { return width; }
	int GetPosX() { return pos_x; }
	int GetPosY() { return pos_y; }
	bool isActive() { return active; }
	void SetActive() { active = !active; }

public:
	int height = 10;
	int width = 10;
	int pos_x = 0;
	int pos_y = 0;
	bool active = false;
	std::string PanelName;
};





#endif // !__Panels__

