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
	for (std::vector<Mesh*>::iterator it = App->scene->scene_meshes.begin(); it != App->scene->scene_meshes.end(); it++)
	{
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("%s \n", (*it)->mesh_name.c_str());
		ImGui::Separator();
		ImGui::Text("Position x:%.3f, y:%.3f, z:%.3f .", (*it)->pos.x, (*it)->pos.y, (*it)->pos.z);
		ImGui::Text("Rotation x:%.3f, y:%.3f, z:%.3f .", (*it)->euler_rot.x, (*it)->euler_rot.y, (*it)->euler_rot.z);
		ImGui::Text("Scale    x:%.3f, y:%.3f, z:%.3f .", (*it)->scale.x, (*it)->scale.y, (*it)->scale.z);
		ImGui::Separator();
		ImGui::Text("Total normals: %d\nTotal vertices: %d\nTotal indices: %d\n", (*it)->num_normals, (*it)->num_vertices, (*it)->num_vertices);
		ImGui::Separator();
		ImGui::Text("Texture:");
		ImTextureID tex = (uint*)App->scene->textures[1]->texture_id;
		ImVec2 size = ImGui::GetWindowSize();
		size.y = size.x;
		ImGui::Image(tex, size);
	}
	ImGui::Separator();
	/*for (std::vector<Texture*>::iterator it = App->scene->textures.begin(); it != App->scene->textures.end(); it++)
	{
		ImTextureID tex = (uint*)(*it)->texture_id;
		ImVec2 size = ImGui::GetWindowSize();
		size.y = size.x;
		ImGui::Image(tex, size);
		i++;
	}*/

	ImGui::End();
}
