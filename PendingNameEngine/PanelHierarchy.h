#ifndef _PANEL_HIERARCHY_
#define _PANEL_HIERARCHY_

#include "Application.h"
#include "Panel.h"


class PanelHierarchy :
	public Panel
{
public:
	PanelHierarchy();
	PanelHierarchy(char* _name);
	~PanelHierarchy();

	void Draw() override;
};







#endif // !_PANEL_HIERARCHY_


