#include "PanelInspector.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentAudioSource.h"
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
	ImGui::SetNextWindowSize(ImVec2(290, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("Inspector", &active);
	GameObject* selected = nullptr;
	if (App->scene->selected_go != nullptr)
	{
		selected = App->scene->selected_go;
		if (selected->go_name != "")
		{
			ImGui::Text("Name:");
			ImGui::SameLine();

			ImGui::TextColored(YELLOW, "%s", selected->go_name.c_str());
			
			ImGui::Text("Unique id: %llu", App->scene->selected_go->UniqueId);

			bool is_go_active = selected->IsActive();
			if (ImGui::Checkbox("Active", &is_go_active))
			{
				selected->SetActive(is_go_active);
			}
			ImGui::SameLine();
			bool is_go_static = selected->IsStatic();
			if (ImGui::Checkbox("Static", &is_go_static))
			{
				selected->SetStatic(is_go_static);
				App->scene->octree->Update();
			}
		
			ImGui::Separator();

			for (std::list<Component*>::iterator it = selected->components.begin(); it != selected->components.end(); it++ )
			{
				DrawComponent((*it));
			}

		}
	}
	

	ImGui::End();
}

void PanelInspector::DrawComponent(Component * cmp)
{
	switch (cmp->GetType())
	{
	case CMP_TRANSFORM:
		DrawComponentTransform((ComponentTransform*)cmp);
		break;
	case CMP_MESH:
		DrawComponentMesh((ComponentMesh*)cmp);
		break;
	case CMP_MATERIAL:
		DrawComponentMaterial((ComponentMaterial*)cmp);
		break;
	case CMP_CAMERA:
		DrawComponentCamera((ComponentCamera*)cmp);
		break;
	case CMP_A_SOURCE:
		DrawComponentASource((ComponentAudioSource*)cmp);
		break;
	case CMP_A_LISTENER:
		DrawComponentAListener((ComponentAudioListener*)cmp);
		break;
	default:
		break;
	}
}

void PanelInspector::DrawComponentTransform(ComponentTransform * trans)
{

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 position;
		float3 rotation;
		float3 scale;

		if (trans->GetOwner()->IsRoot())
		{
			position = trans->GetGlobalPosition();
			rotation = trans->GetGlobalRotation();
			scale = trans->GetGlobalScale();
		}
		else
		{
			position = trans->GetLocalPosition();
			rotation = trans->GetLocalRotation();
			scale = trans->GetLocalScale();
		}

		if (ImGui::RadioButton("Translate", App->scene->mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			App->scene->mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", App->scene->mCurrentGizmoOperation == ImGuizmo::ROTATE))
			App->scene->mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", App->scene->mCurrentGizmoOperation == ImGuizmo::SCALE))
			App->scene->mCurrentGizmoOperation = ImGuizmo::SCALE;

		if (ImGui::DragFloat3("Position", (float*)&position, 0.25f) && trans->GetOwner()->IsStatic() == false)
		{
			trans->SetPosition(position);
		}
		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.25f, -360, 360) && trans->GetOwner()->IsStatic() == false)
		{
			trans->SetRotation(rotation);
		}
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25f) && trans->GetOwner()->IsStatic() == false)
		{
			trans->SetScale(scale);
		}
	}
}

void PanelInspector::DrawComponentMesh(ComponentMesh * mesh)
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		
		if (mesh->GetMesh() != nullptr)
		{
			ImGui::Columns(2, "Mesh Info", true);
			ImGui::Text("Num Vertices: ");
			ImGui::Text("Num Indices: ");
			ImGui::Text("Num Faces: ");
			ImGui::Text("Has Normals: ");
			ImGui::Text("Has TexCoords: ");
			ImGui::NextColumn();
			ImGui::TextColored(YELLOW, "%d", mesh->GetMesh()->num_vertices);
			ImGui::TextColored(YELLOW, "%d", mesh->GetMesh()->num_indices);
			ImGui::TextColored(YELLOW, "%d", mesh->GetMesh()->num_indices / 3);
			if (mesh->GetMesh()->normals_id > 0)
				ImGui::TextColored(YELLOW, "yes");
			else
				ImGui::TextColored(YELLOW, "-");

			if (mesh->GetMesh()->texcoords_id > 0)
				ImGui::TextColored(YELLOW, "yes");
			else
				ImGui::TextColored(YELLOW, "-");

			ImGui::Columns(1);
		}
		ImGui::Separator();
		
	}
}

void PanelInspector::DrawComponentMaterial(ComponentMaterial * mat)
{
	if (ImGui::CollapsingHeader("Material"))
	{

		if (mat->GetMaterial()->GetDiffuse() != nullptr)
		{

			ImGui::Text("Texture Id: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", mat->GetMaterial()->GetDiffuse()->GetTextureId());
			ImGui::Text("Texture Path: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%s", mat->GetMaterial()->GetDiffuse()->GetName());

			ImGui::Separator();
			ImGui::Text("Width: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", mat->GetMaterial()->GetDiffuse()->GetWidth());
			ImGui::SameLine();
			ImGui::Text("Height: "); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", mat->GetMaterial()->GetDiffuse()->GetHeight());
			ImGui::Separator();

			ImTextureID tex = (uint*)mat->GetMaterial()->GetDiffuse()->GetTextureId();
			ImVec2 size = ImGui::GetContentRegionAvail();
			size.y = size.x;
			ImGui::Image(tex, size);

			if (ImGui::Checkbox("Enable checkers texture", &usecheckers)) {
				if (usecheckers) {
					prevtex = mat->GetMaterial()->GetDiffuse();
					mat->GetMaterial()->SetDiffuse(App->scene->checkers);
				}
				else {
					mat->GetMaterial()->SetDiffuse(prevtex);

				}
			}
		}
	}
}

void PanelInspector::DrawComponentCamera(ComponentCamera * cam)
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		Color bg = cam->bg_color;
		if (ImGui::ColorEdit4("Background Color", &bg.r))
		{
			cam->bg_color = bg;
		}
		ImGui::Separator();

		float fov = cam->GetFOV();
		if (ImGui::SliderFloat("Field of View", &fov, 1, 150))
		{
			cam->SetFOV(fov);
		}
		float near_plane = cam->GetNearPlaneDist();
		float far_plane = cam->GetFarPlaneDist();
		if (ImGui::DragFloat("Near Plane", &near_plane, 0.02f, 0.01, far_plane - 0.1f))
		{
			cam->SetNearPlaneDist(near_plane);
		}
		if (ImGui::DragFloat("Far Plane", &far_plane, 0.02f, 0.01, near_plane + 0.1f))
		{
			cam->SetFarPlaneDist(far_plane);
		}
		ImGui::Spacing();

		ImGui::Checkbox("Frustum Cull", &cam->frustum_cull);

		cam->Update();
	}
}

void PanelInspector::DrawComponentASource(ComponentAudioSource * source)
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (source->GetSoundId() != 0)
		{
			if (ImGui::Button("Play"))
			{
				source->GetSoundObject()->ev_Play(source->GetSoundId());
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				source->GetSoundObject()->ev_Stop(source->GetSoundId());
			}
		}
		else
		{
			ImGui::TextColored(YELLOW, "There's no Audio Event!");
		}
	}
}

void PanelInspector::DrawComponentAListener(ComponentAudioListener * listener)
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(YELLOW,"Visual representation of Listener ");
		ImGui::TextColored(YELLOW, "Position. Audio Listener Wwise GO  ");
		ImGui::TextColored(YELLOW, "is tied to editor camera position.");
		ImGui::TextColored(YELLOW, "Use Module Audio in the Configuration");
		ImGui::TextColored(YELLOW, "Window to control listener properties.");
	}
}
