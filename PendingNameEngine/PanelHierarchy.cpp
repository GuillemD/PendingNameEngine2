#include "PanelHierarchy.h"



PanelHierarchy::PanelHierarchy()
{
}

PanelHierarchy::PanelHierarchy(char * _name)
{
	PanelName = _name;
}


PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 800));
	ImGui::Begin("Hierarchy", &active);
	for (auto go : App->scene->scene_gameobjects) {
		go->PrintMyHierarchy();
	}
	
	ImGui::End();
}
