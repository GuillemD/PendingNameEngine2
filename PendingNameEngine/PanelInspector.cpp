#include "PanelInspector.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Transform.h"
#include "ComponentMesh.h"
#include "Mesh.h"
#include "Globals.h"
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
	if (App->scene->selected_go != nullptr)
	{
		if (App->scene->selected_go->go_name != "")
		{
			ImGui::Text("Name:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", App->scene->selected_go->go_name.c_str());
			bool is_go_active = App->scene->selected_go->IsActive();
			if (ImGui::Checkbox("Active", &is_go_active))
			{
				App->scene->selected_go->SetActive(is_go_active);
			}
			ImGui::SameLine();
			bool is_go_static = App->scene->selected_go->IsStatic();
			if (ImGui::Checkbox("Static", &is_go_static))
			{
				App->scene->selected_go->SetStatic(is_go_static);
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Not yet functional");
			}
			ImGui::Separator();

			for (auto it : App->scene->selected_go->components)
			{
				if ((*it).GetType() == CMP_TRANSFORM)
				{
					if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ComponentTransform* aux_trans = (ComponentTransform*)App->scene->selected_go->GetComponent(CMP_TRANSFORM);
						float3 position;
						float3 rotation;
						float3 scale;

						if ((*it).GetOwner()->IsRoot())
						{
							position = aux_trans->GetPosition();
							rotation = aux_trans->GetEulerRotation();
							scale = aux_trans->GetScale();
						}
						else
						{
							
							if ((*it).GetOwner()->GetParent() != nullptr)
							{
								ComponentTransform* parent_trans = (ComponentTransform*)(*it).GetOwner()->GetParent()->GetComponent(CMP_TRANSFORM);
								position = aux_trans->GetPosition() - parent_trans->GetPosition();
								rotation = aux_trans->GetEulerRotation() - parent_trans->GetEulerRotation();
								scale = (aux_trans->GetScale() - parent_trans->GetScale()) + float3(1.f, 1.f, 1.f);
							}
							else
							{
								position = aux_trans->GetPosition();
								rotation = aux_trans->GetEulerRotation();
								scale = aux_trans->GetScale();
							}
						}

						ImGui::Text("Position: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%f | %f | %f", position.x, position.y, position.z);
						ImGui::Text("Rotation: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%f | %f | %f", rotation.x, rotation.y, rotation.z);
						ImGui::Text("Scale: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%f | %f | %f", scale.x, scale.y, scale.z);
					}
				}
			}

		}
	}
	

	ImGui::End();
}
