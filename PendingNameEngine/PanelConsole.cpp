#include "PanelConsole.h"


PanelConsole::PanelConsole()
{
	
	ConsoleLog("Console started.\n");
	PanelName = "Console";
}

PanelConsole::PanelConsole(const char * _name)
{
	
	ConsoleLog("Console started.\n");
	PanelName = _name;
}

void PanelConsole::Draw()
{
		ImGui::SetNextWindowSize(ImVec2(800, 300), ImGuiCond_FirstUseEver);
		ImGui::Begin("Console", &active);
		if (ImGui::Button("Clear Console")) {
			EmptyConsole();
		}
		ImGui::Separator();
		ImGui::TextUnformatted(ConsoleText.begin());

		if (ScrollToBottom) {
			ImGui::SetScrollHere(1.0F);
			ScrollToBottom = false;
		}
		ImGui::End();
	
}

void PanelConsole::EmptyConsole()
{
	ConsoleText.clear();
}

void PanelConsole::ConsoleLog(char * txt)
{
	ConsoleText.appendf(txt);
}


PanelConsole::~PanelConsole()
{
}
