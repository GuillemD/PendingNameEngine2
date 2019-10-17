#include "PanelInspector.h"
#include "Application.h"

PanelInspector::PanelInspector()
{
	PanelName = "Inspector";
}

PanelInspector::PanelInspector(const char * _name)
{
	PanelName = _name;
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("Inspector", &active);
	int i = 1;

	for (std::vector<Texture*>::iterator it = App->scene->textures.begin(); it != App->scene->textures.end(); it++)
	{
		ImTextureID tex = (uint*)(*it)->texture_id;
		ImVec2 size = ImGui::GetWindowSize();
		size.y = size.x;
		ImGui::Image(tex, size);
		i++;
	}

	ImGui::End();
}
