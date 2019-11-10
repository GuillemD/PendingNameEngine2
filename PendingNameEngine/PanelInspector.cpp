#include "PanelInspector.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "Transform.h"
#include "Material.h"
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
	ImGui::SetNextWindowSize(ImVec2(290, 600));
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
							position = aux_trans->GetGlobalPosition();
							rotation = aux_trans->GetGlobalRotation();
							scale = aux_trans->GetGlobalScale();
						}
						else
						{
							position = aux_trans->GetLocalPosition();
							rotation = aux_trans->GetLocalRotation();
							scale = aux_trans->GetLocalScale();							
						}
						if (ImGui::DragFloat3("Position", (float*)&position, 0.25f) && (*it).GetOwner()->IsStatic() == false) 
						{
							aux_trans->SetPosition(position);
						}
						if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.25f, -360, 360) && (*it).GetOwner()->IsStatic() == false)
						{
							aux_trans->SetRotation(rotation);
						}
						if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25f) && (*it).GetOwner()->IsStatic() == false)
						{
							aux_trans->SetScale(scale);
						}
					}
				}
				else if ((*it).GetType() == CMP_MESH)
				{
					if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ComponentMesh* aux_mesh = (ComponentMesh*)App->scene->selected_go->GetComponent(CMP_MESH);
						if (aux_mesh->GetMesh() != nullptr)
						{
							ImGui::Columns(2, "Mesh Info", true);
							ImGui::Text("Num Vertices: ");
							ImGui::Text("Num Indices: ");
							ImGui::Text("Num Faces: ");
							ImGui::Text("Has Normals: ");
							ImGui::Text("Has TexCoords: ");
							ImGui::NextColumn();
							ImGui::TextColored(YELLOW, "%d", aux_mesh->GetMesh()->num_vertices);
							ImGui::TextColored(YELLOW, "%d", aux_mesh->GetMesh()->num_indices);
							ImGui::TextColored(YELLOW, "%d", aux_mesh->GetMesh()->num_indices / 3);
							if(aux_mesh->GetMesh()->normals_id > 0)
								ImGui::TextColored(YELLOW, "yes");
							else
								ImGui::TextColored(YELLOW, "-");

							if (aux_mesh->GetMesh()->texcoords_id > 0)
								ImGui::TextColored(YELLOW, "yes");
							else
								ImGui::TextColored(YELLOW, "-");

							ImGui::Columns(1);
						}
						ImGui::Separator();
						if (aux_mesh->GetMesh()->num_normals > 0)
						{
							if (ImGui::Checkbox("Draw Faces Normals", &aux_mesh->GetMesh()->drawnormals)) {
								if (aux_mesh->GetMesh()->drawnormals)
								{

								}
							}
						}
						ImGui::Separator();
					}
				}
				else if ((*it).GetType() == CMP_MATERIAL)
				{
					if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ComponentMaterial* aux_mat = (ComponentMaterial*)App->scene->selected_go->GetComponent(CMP_MATERIAL);
						
						if (aux_mat->GetMaterial() != nullptr)
						{
							ImGui::Text("Texture Id: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", aux_mat->GetMaterial()->GetDiffuse()->GetTextureId());
							ImGui::Text("Texture Path: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", aux_mat->GetMaterial()->GetDiffuse()->GetName());

							ImGui::Separator();
							ImGui::Text("Width: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", aux_mat->GetMaterial()->GetDiffuse()->GetWidth());
							ImGui::SameLine();
							ImGui::Text("Height: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", aux_mat->GetMaterial()->GetDiffuse()->GetHeight());
							ImGui::Separator();

							ImTextureID tex = (uint*)aux_mat->GetMaterial()->GetDiffuse()->GetTextureId();
							ImVec2 size = ImGui::GetContentRegionAvail();
							size.y = size.x;
							ImGui::Image(tex, size);
						}
						
						if (ImGui::Checkbox("Enable checkers texture", &usecheckers)) {
							if (usecheckers) {
								prevtex = aux_mat->GetMaterial()->GetDiffuse();
								aux_mat->GetMaterial()->SetDiffuse(App->scene->checkers);
							}
							else {
								aux_mat->GetMaterial()->SetDiffuse(prevtex);

							}

						}
						
					}
				}
			}

		}
	}
	

	ImGui::End();
}
