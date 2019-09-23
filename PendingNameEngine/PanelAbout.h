#ifndef _PANELABOUT_H_
#define _PANELABOUT_H_

#include "Panel.h"
class PanelAbout :
	public Panel
{
public:
	PanelAbout();
	PanelAbout(const char* _name);

	~PanelAbout();

	void Draw() override;
	
public:
	
};

#endif //_PANELABOUT_H_
