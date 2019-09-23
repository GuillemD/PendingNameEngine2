#pragma once
#include "Panel.h"
class Console :
	public Panel
{
public:
	Console();
	
	~Console();

	void Draw();
	void EmptyConsole();
	void ConsoleLog(char* txt);
public:
	ImGuiTextBuffer ConsoleText;
	bool ScrollToBottom = true;
};

