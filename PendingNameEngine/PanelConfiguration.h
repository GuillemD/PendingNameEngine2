#ifndef _PANELCONFIGURATION_H_
#define _PANELCONFIGURATION_H_

#include "Panel.h"

class PanelConfig :
	public Panel
{
public:
	PanelConfig();
	PanelConfig(const char* _name);
	~PanelConfig();

	void Draw() override;

};
	

#endif // !_PANELCONFIGURATION_H_

