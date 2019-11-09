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
	if (ImGui::Begin(PanelName.c_str(), &active, ImVec2(500, 500),-0.5f, NULL))
	{
		App->ShowApplicationConfig();
		App->ShowHardwareConfig();
		App->input->ShowInputConfig();
		App->window->ShowWindowConfig();
		App->renderer3D->ShowRendererConfig();
	}
	ImGui::End();
}
