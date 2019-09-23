#include "Console.h"



Console::Console()
{
	height = 100;
	width = 500;
	ConsoleText.append("Console started.\n");
}

void Console::Draw()
{
	
		ImGui::Begin("Console");
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

void Console::EmptyConsole()
{
	ConsoleText.empty();
}

void Console::ConsoleLog(char * txt)
{
	ConsoleText.append(txt);
}


Console::~Console()
{
}
