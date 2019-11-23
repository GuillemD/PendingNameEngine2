#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include "mmgr/mmgr.h"


ModuleScene::ModuleScene()
{
	name = "Scene";

}

ModuleScene::ModuleScene(bool start_enabled)
{
	name = "Scene";

}


ModuleScene::~ModuleScene()
{
}


bool ModuleScene::Start()
{
	bool ret = true;

	//Octree
	octree = new Octree();
	octree->Create(float3::zero, float3::zero);
	octree->update_octree = true;
	octree->draw_octree = false;

	//Selected go
	selected_go = new GameObject();

	//Editor Camera
	App->camera->SetCamPos({0.0,10.f,5.f});
	App->camera->LookAt({ 0,0,0 });

	//Guizmo
	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::LOCAL;

	//Game Cam Test
	GameObject* game_cam = new GameObject();
	game_cam->go_name = "Main Camera";
	AddGameObject(game_cam);
	SetSelectedGO(game_cam);
	game_cam->is_root = true;
	

	ComponentCamera* cmp_cam = (ComponentCamera*)game_cam->AddComponent(CMP_CAMERA);
	ComponentTransform* cmp_trans = (ComponentTransform*)game_cam->GetComponent(CMP_TRANSFORM);

	cmp_trans->SetPosition({ 0.f,0.f,-15.f });
	//Initial Mesh
	ret = App->importer->Import(".//Assets//BakerHouse.fbx");
	App->importer->first_load = false;
	ret = App->importer->Import(".//Assets//Baker_house.png");

	for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		ComponentTransform* aux = (ComponentTransform*)(*it)->GetComponent(CMP_TRANSFORM);

		aux->SetRotation({ 0.f,0.f,0.f });
		aux->SetScale({ 1.f,1.f,1.f });
	}

	return ret;
}


update_status ModuleScene::PreUpdate(float dt)
{
	if (!to_delete.empty())
		DeleteGameObjects();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		ComponentCamera* aux_cam = (ComponentCamera*)(*it)->GetComponent(CMP_CAMERA);

		if (aux_cam != nullptr)
		{
			if (aux_cam->GetOwner()->IsActive())
			{
				App->renderer3D->rendering_cameras.push_back(aux_cam);
			}
		}
	}


	if (octree->update_octree)
	{
		octree->min_point = float3::zero;
		octree->max_point = float3::zero;

		for (std::list<GameObject*>::iterator it = static_gameobjects.begin(); it != static_gameobjects.end(); it++)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);
			if (mesh != nullptr && mesh->GetMesh() != nullptr)
				octree->Recalculate(mesh->GetMesh()->bb.minPoint, mesh->GetMesh()->bb.maxPoint);
		}

		octree->Update();

		for (std::list<GameObject*>::iterator it = static_gameobjects.begin(); it != static_gameobjects.end(); it++)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);
			if (mesh != nullptr && mesh->GetMesh() != nullptr)
				octree->Insert(*it);
		}
		octree->update_octree = false;
	}

	
		
	return UPDATE_CONTINUE;
}


bool ModuleScene::CleanUp()
{
	for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		delete (*it);
	}

	delete octree;

	return true;
}


void ModuleScene::DrawScene()
{
	PPlane grid(0, 1, 0, 20);
	grid.axis = true;
	grid.color = { 1.0f,1.0f,1.0f };

	App->renderer3D->DebugRenderSettings();
	grid.Render();
	App->renderer3D->SetDefaultSettings();

	DrawGizmo();

	for (std::list<ComponentCamera*>::iterator it = App->renderer3D->rendering_cameras.begin(); it != App->renderer3D->rendering_cameras.end(); it++)
	{
		DrawGameObjects(*it);
	}
	//DrawGameObjects(App->renderer3D->active_cam);
	App->renderer3D->rendering_cameras.clear();

}

void ModuleScene::DrawGameObjects(ComponentCamera * cam_to_draw)
{

	
	if (cam_to_draw->frustum_cull)
	{
		std::list<GameObject*> intersections;

		if (octree->GetRoot() != nullptr && octree->optimise)
		{
			octree->GetObjectIntersections(intersections, &cam_to_draw->camera_frustum.MinimalEnclosingAABB());

			intersections.sort();
			intersections.unique();

			for (auto it = intersections.begin(); it != intersections.end(); it++)
			{
				ComponentMesh* aux_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

				if (!aux_mesh || cam_to_draw->GetOwner() == nullptr)continue;
				{
					if (!cam_to_draw->ContainsAABB(aux_mesh->GetMesh()->bb))
						continue;
				}
				(*it)->Draw();
			}
		}
		else
		{
			for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
			{
				ComponentMesh* aux_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);
				if (cam_to_draw->GetOwner() && aux_mesh && cam_to_draw->frustum_cull)
				{
					if (!cam_to_draw->ContainsAABB(aux_mesh->GetMesh()->bb))
						continue;
				}
				(*it)->Draw();
			}
		}
		
	}
	else
	{
		for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
		{
			(*it)->Draw();
		}
	}
	
}

void ModuleScene::ClearScene()
{
	if (!scene_gameobjects.empty())
	{
		for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
		{
			if ((*it)->GetParent() == nullptr)
				(*it)->DeleteGameObject();
		}
		DeleteGameObjects();

		if (!root_gameobjects.empty())
			root_gameobjects.clear();
	}
	App->importer->mesh_path = "";
	App->importer->texture_path = "";
	
	
}

void ModuleScene::DeleteGameObjects()
{

	for (auto it = to_delete.begin(); it != to_delete.end();)
	{
		if ((*it)->IsRoot())
		{
			root_gameobjects.remove(*it);
		}
		if ((*it)->IsStatic())
		{
			(*it)->SetStatic(false);
		}
		(*it)->DeleteComponents();

		if ((*it)->parent != nullptr)
			(*it)->parent->DeleteChild((*it));

		(*it)->parent = nullptr;

		DeleteGameObject((*it));

		if ((*it) != App->renderer3D->GetCamObject())
		{
			delete (*it);
			(*it) = nullptr;
		}

		it = to_delete.erase(it);
	}
}

void ModuleScene::DeleteGameObject(GameObject * go_to_delete)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if (go_to_delete == (*it))
		{
			scene_gameobjects.erase(it);
			return;
		}
	}
}

void ModuleScene::AddGameObject(GameObject * go)
{
	if (go != nullptr)
	{
		scene_gameobjects.push_back(go);
		if (go->GetParent() == nullptr)
		{
			root_gameobjects.push_back(go);
			go->is_root = true;
		}
	}
	
}

void ModuleScene::AddGameObjectToDelete(GameObject * go_to_delete)
{
	to_delete.push_back(go_to_delete);
}

void ModuleScene::SetSelectedGO(GameObject * go)
{
	selected_go->SetSelected(false);
	if(go != nullptr)
		go->SetSelected(true);
	selected_go = go;
}

void ModuleScene::AddStaticGO(GameObject * go)
{
	static_gameobjects.push_back(go);
}

void ModuleScene::RemoveGoFromStaticList(GameObject * go)
{
	for (auto it = static_gameobjects.begin(); it != static_gameobjects.end(); it++)
	{
		if ((*it) == go)
		{
			static_gameobjects.erase(it);
			return;
		}
	}
}

void ModuleScene::ShowOctreeConfig()
{
	if (ImGui::CollapsingHeader("Octree"))
	{
		if (ImGui::Checkbox("Draw Octree", &octree->draw_octree))
		{

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Optimize", &octree->optimise))
		{

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Check intersections with frustum to check if and object is drawn.");
		}
		ImGui::Separator();

		ImGui::Text("Num. Static GameObjects: ");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%d", static_gameobjects.size());
	}
}

void ModuleScene::DrawGizmo()
{
	if (selected_go)
	{
		ImGuizmo::Enable(true);
		if (selected_go->IsStatic()) ImGuizmo::Enable(false);

		ImVec2 window_pos = ImGui::GetWindowPos();
		ImVec2 window_size = ImVec2(App->window->GetWidth(), App->window->GetHeight());
		ImGuizmo::SetRect(window_pos.x, window_pos.y, window_size.x, window_size.y);

		float4x4 selected_view = selected_go->GetGlobalMatrix().Transposed();
		float transformation[16]; //float4x4
		//ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate(App->camera->GetEditorCam()->GetViewMatrix(), App->camera->GetEditorCam()->GetProjectionMatrix(), mCurrentGizmoOperation, mCurrentGizmoMode, selected_view.ptr(), transformation);


		if (ImGuizmo::IsOver() && ImGuizmo::IsUsing())
		{
			float position[3];
			float rotation[3];
			float scale[3];

			ImGuizmo::DecomposeMatrixToComponents(transformation, position, rotation, scale);

			ComponentTransform* trans = (ComponentTransform*)selected_go->GetComponent(CMP_TRANSFORM);

			float3 pos;
			float3 rot;
			float3 sc;

			if (trans->GetOwner()->IsRoot())
			{
				pos = trans->GetGlobalPosition();
				rot = trans->GetGlobalRotation();
				sc = trans->GetGlobalScale();
			}
			else
			{
				pos = trans->GetLocalPosition();
				rot = trans->GetLocalRotation();
				sc = trans->GetLocalScale();
			}

			float3 g_pos(position[0], position[1], position[2]);
			float3 g_rot(rotation[0], rotation[1], rotation[2]);
			float3 g_sc(scale[0], scale[1], scale[2]);

			switch (mCurrentGizmoOperation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				if (selected_go->GetParent() != nullptr)
				{
					g_pos = selected_go->GetParent()->GetGlobalMatrix().Inverted().TransformPos(g_pos);
				}
				trans->SetPosition(pos + g_pos);
				break;
			case ImGuizmo::OPERATION::ROTATE:
				if (selected_go->GetParent() != nullptr)
				{
					g_rot = selected_go->GetParent()->GetGlobalMatrix().Inverted().TransformPos(g_rot);
				}
				trans->SetRotation(rot + g_rot);
				break;
			case ImGuizmo::OPERATION::SCALE:
				break;
			}
		}
	}
}




