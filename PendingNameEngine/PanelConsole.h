#ifndef _PANELCONSOLE_H_
#define _PANELCONSOLE_H_

#include "Panel.h"
class PanelConsole :
	public Panel
{
public:
	PanelConsole();
	PanelConsole(const char* _name);
	
	~PanelConsole();

	void Draw() override;
	void EmptyConsole();
	void ConsoleLog(char* txt);



public:
	ImGuiTextBuffer ConsoleText;
	bool ScrollToBottom = true;
};

#endif //_PANELCONSOLE_H_
