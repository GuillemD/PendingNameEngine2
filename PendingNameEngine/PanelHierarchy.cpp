#include "PanelHierarchy.h"
#include "Application.h"
#include "GameObject.h"



PanelHierarchy::PanelHierarchy()
{
	PanelName = "Hierarchy";
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
	ImGui::SetNextWindowSize(ImVec2(190, 600));
	ImGui::Begin("Hierarchy", &active);
	
	for (std::list<GameObject*>::iterator it = App->scene->root_gameobjects.begin(); it != App->scene->root_gameobjects.end(); it++)
	{
		(*it)->PrintMyHierarchy();
	}
	
	ImGui::End();
}

