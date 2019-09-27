#include "PanelConfiguration.h"
#include "Application.h"

PanelConfig::PanelConfig()
{
	PanelName = "Configuration";
}

PanelConfig::PanelConfig(const char * _name)
{
	PanelName = _name;
}

PanelConfig::~PanelConfig()
{
}

void PanelConfig::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	if (ImGui::Begin(PanelName.c_str(), &active, NULL))
	{
		App->ShowApplicationConfig();
		App->ShowHardwareConfig();
	}
	ImGui::End();
}
