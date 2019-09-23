#include "PanelConsole.h"


PanelConsole::PanelConsole()
{
	height = 100;
	width = 500;
	ConsoleText.append("Console started.\n");
}

void PanelConsole::Draw()
{
	
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
	ConsoleText.empty();
}

void PanelConsole::ConsoleLog(char * txt)
{
	ConsoleText.append(txt);
}


PanelConsole::~PanelConsole()
{
}
