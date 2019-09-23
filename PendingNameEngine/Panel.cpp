#include "Panel.h"

Panel::Panel()
{
}

Panel::Panel(const char * _name)
{
	PanelName = _name;
}

Panel::Panel(int h, int w, int x, int y)
{
	height = h;
	width = w;
	pos_x = x;
	pos_y = y;

}


Panel::~Panel()
{
}

void Panel::Draw()
{
}
