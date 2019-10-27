#ifndef _PANEL_INSPECTOR_H_
#define _PANEL_INSPECTOR_H_

#include "Panel.h"

class PanelInspector :
	public Panel
{
public:
	PanelInspector();
	PanelInspector(const char* _name);
	~PanelInspector();

	bool usecheckers = false;
	Texture* prevtex = nullptr;

	void Draw() override;

};
#endif // !_PANEL_INSPECTOR_H_
