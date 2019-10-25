#include "PanelInspector.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Transform.h"
#include "ComponentMesh.h"
#include "Mesh.h"
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
	ImGui::SetNextWindowSize(ImVec2(400, 800));
	ImGui::Begin("Inspector", &active);
	int i = 1;
	for (std::vector<GameObject*>::iterator it = App->scene->scene_gameobjects.begin(); it != App->scene->scene_gameobjects.end(); it++)
	{
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("%s \n", (*it)->go_name.c_str());
		ImGui::Separator();
	

		ComponentTransform* tmp_trans = (ComponentTransform*)(*it)->GetComponent(CMP_TRANSFORM);
		ComponentMesh* tmp_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

		float3 f = tmp_trans->GetPosition();
		ImGui::Text("Position x:%.3f, y:%.3f, z:%.3f .", f.x, f.y, f.z);
		f = tmp_trans->GetEulerRotation();
		ImGui::Text("Rotation x:%.3f, y:%.3f, z:%.3f .", f.x, f.y, f.z);
		f = tmp_trans->GetScale();
		ImGui::Text("Scale    x:%.3f, y:%.3f, z:%.3f .", f.x, f.y, f.z);
		ImGui::Separator();
		
		//ImGui::Text("Total normals: %d\nTotal vertices: %d\nTotal indices: %d\n", tmp->num_normals, tmp->num_indices, tmp->num_vertices);
		ImGui::Separator();
		ImGui::Text("Texture:");
		ImTextureID tex = (uint*)App->scene->textures[0]->texture_id;
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
