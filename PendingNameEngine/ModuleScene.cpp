#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentAudioSource.h"

#include "mmgr/mmgr.h"

#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "ModuleInput.h"

ModuleScene::ModuleScene()
{
	name = "Scene";
	last_gizmo_scale = { 1,1,1 };

}

ModuleScene::ModuleScene(bool start_enabled)
{
	name = "Scene";
	last_gizmo_scale = { 1,1,1 };

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
	App->camera->SetCamPos({0.0,50.f,50.f});
	App->camera->LookAt({ 0,0,0 });

	//Audio Listener
	default_listener = new GameObject();
	default_listener->go_name = "Audio Listener";
	default_listener->AddComponent(CMP_A_LISTENER);
	AddGameObject(default_listener);

	//BG Music Source
	bg_music = new GameObject();
	bg_music->go_name = "BackgroundMusicSource";
	ComponentAudioSource* music_source = (ComponentAudioSource*)bg_music->AddComponent(CMP_A_SOURCE);
	music_source->SetSoundId(AK::EVENTS::MUSIC);
	music_source->GetSoundObject()->ev_Play(AK::EVENTS::MUSIC);
	music_source->GetSoundObject()->playing = true;

	//Guizmo
	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::LOCAL;

	//Game Cam Test
	GameObject* game_cam = new GameObject();
	game_cam->go_name = "Game Camera";
	AddGameObject(game_cam);
	SetSelectedGO(game_cam);
	game_cam->is_root = true;
	

	ComponentCamera* cmp_cam = (ComponentCamera*)game_cam->AddComponent(CMP_CAMERA);
	ComponentTransform* cmp_trans = (ComponentTransform*)game_cam->GetComponent(CMP_TRANSFORM);

	cmp_trans->SetPosition({ 0.f,0.f,29.f });
	cmp_trans->SetRotation({ 0.0f,180.f,0.0f });


	//SCENE
	App->importer->Import("Assets/Meshes_Textures/Moose.FBX");
	App->importer->Import("Assets/Meshes_Textures/Sci_fi_Train.fbx");
	App->importer->Import("Assets/Meshes_Textures/ReverbBox.FBX");

	App->importer->first_load = false;

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->go_name == "Moose")
		{
			moose = (*it);
		}
		else if ((*it)->go_name == "Sci_fi_Train")
		{
			train = (*it);
		}
		else if ((*it)->go_name == "ReverbBox")
		{
			reverb_zone = (*it);
		}
	}
	ComponentTransform* moose_trans = (ComponentTransform*)moose->GetComponent(CMP_TRANSFORM);
	moose_trans->SetPosition({ 0,0,10 });
	moose_trans->SetRotation({ -90,0,-30 });
	moose_trans->SetScale({ 4,4,4 });
	ComponentAudioSource* audio_source_moose = (ComponentAudioSource*)moose->AddComponent(CMP_A_SOURCE);
	audio_source_moose->SetSoundId(AK::EVENTS::GOOSE);

	ComponentTransform* train_trans = (ComponentTransform*)train->GetComponent(CMP_TRANSFORM);
	train_trans->SetPosition({ 50,0,-20 });
	train_trans->SetRotation({ 0,90,0 });
	train_trans->SetScale({ 0.015f,0.015f,0.015f });
	ComponentAudioSource* audio_source_train = (ComponentAudioSource*)train->AddComponent(CMP_A_SOURCE);
	audio_source_train->SetSoundId(AK::EVENTS::STEAM_TRAIN);
	audio_source_train->GetSoundObject()->ev_Play(AK::EVENTS::STEAM_TRAIN);
	audio_source_train->GetSoundObject()->playing = true;
	//Set Reverb Zone
	
	ComponentTransform* rev_trans = (ComponentTransform*)reverb_zone->GetComponent(CMP_TRANSFORM);
	rev_trans->SetPosition({ 0,3.5f,-25 });
	rev_trans->SetRotation({ 0,0,90 });
	rev_trans->SetScale({ 0.7f,1.f,0.5f });

	return ret;
}


update_status ModuleScene::PreUpdate(float dt)
{
	if (!to_delete.empty())
		DeleteGameObjects();

	ComponentTransform* t_trans = (ComponentTransform*)train->GetComponent(CMP_TRANSFORM);
	ComponentAudioSource* as_train = (ComponentAudioSource*)train->GetComponent(CMP_A_SOURCE);
	if (train_left)
	{
		t_trans->SetPosition({ t_trans->GetGlobalPosition().x - 20 * dt, 0, -20 });
		if (t_trans->transform.pos.x <= -220)
			train_left = false;
	}
	else
	{
		t_trans->SetPosition({ t_trans->GetGlobalPosition().x + 20 * dt, 0, -20 });
		if (t_trans->transform.pos.x >= 220)
			train_left = true;
	}
	if (IsTrainInsideReverbBox())
	{
		as_train->GetSoundObject()->ReverbAuxSend(0.75f, as_train->GetSoundObject()->getID());
	}
	else
	{
		as_train->GetSoundObject()->NullAuxSend(as_train->GetSoundObject()->getID());
	}

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

	//editor
	if(App->renderer3D->rendering_cameras.empty())
		DrawGameObjects(App->camera->GetEditorCam(),true);

	for (std::list<ComponentCamera*>::iterator it = App->renderer3D->rendering_cameras.begin(); it != App->renderer3D->rendering_cameras.end(); it++)
	{
		DrawGameObjects(*it,false);
	}
	
	App->renderer3D->rendering_cameras.clear();
	DrawGizmo();

}

void ModuleScene::DrawGameObjects(ComponentCamera * cam_to_draw, bool is_editor)
{
	std::list<GameObject*> intersections;
	octree->GetObjectIntersections(intersections, &cam_to_draw->camera_frustum.MinimalEnclosingAABB());

	intersections.sort();
	intersections.unique();
	for (auto it = intersections.begin(); it != intersections.end(); it++)
	{
		(*it)->Draw();
	}
	for (std::vector<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		ComponentMesh* aux_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

		if (!is_editor)
		{
			if (cam_to_draw->GetOwner() && aux_mesh && cam_to_draw->frustum_cull)
			{
				if (!cam_to_draw->ContainsAABB(aux_mesh->GetMesh()->bb))
					continue;
			}
			(*it)->Draw();
		}
		else
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
	if(selected_go != nullptr)
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
		ImGuizmo::BeginFrame();

		ImGuizmo::Enable(true);
		if (selected_go->IsStatic()) ImGuizmo::Enable(false);

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		
		float4x4 selected_view = selected_go->GetGlobalMatrix().Transposed();
		float transformation[16]; //float4x4
		
		ImGuizmo::Manipulate(App->camera->GetEditorCam()->GetViewMatrix(), App->camera->GetEditorCam()->GetProjectionMatrix(), mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&selected_view, transformation);

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
				if (last_gizmo_scale.x != g_sc.x || last_gizmo_scale.y != g_sc.y || last_gizmo_scale.z != g_sc.z)
				{
					float3 aux = g_sc;
					g_sc -= last_gizmo_scale;
					last_gizmo_scale = aux;
					trans->SetScale(sc + g_sc);
				}
				break;
			}
		}
		else
		{
			last_gizmo_scale = { 1,1,1 };
		}
	}
}

void ModuleScene::TestMouseRayHit(LineSegment ray)
{
	list<GameObject*> intersections;

	auto it = scene_gameobjects.begin();
	while (it != scene_gameobjects.end())
	{
		GameObject* go = (*it);

		ComponentMesh* c_mesh = (ComponentMesh*)go->GetComponent(CMP_MESH);
		if (c_mesh == nullptr || c_mesh->GetMesh() == nullptr)
		{
			it++;
			continue;
		}
		

		bool hit = ray.Intersects(c_mesh->GetMesh()->bb);
		if (hit)
		{
			intersections.push_back(go);
		}
		it++;
	}

	GameObject* closest = GetClosestGO(ray, intersections);

	SetSelectedGO(closest);
}

GameObject * ModuleScene::GetClosestGO(LineSegment ray, list<GameObject*> list)
{
	float3 nearest_point;

	float near_dist = 100000;
	float dist;

	GameObject* ret = nullptr;

	auto it = list.begin();
	while (it != list.end())
	{
		GameObject* go = (*it);

		ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(CMP_MESH);
		if (mesh)
		{
			float3 point = { 0,0,0 };
			if (mesh->GetClosestPoint(ray, point, dist))
			{
				if (dist < near_dist)
				{
					near_dist = dist;
					nearest_point = point;
					ret = go;
				}
			}
		}

		it++;
	}

	return ret;
}

bool ModuleScene::IsTrainInsideReverbBox()
{
	ComponentMesh* train_mesh = (ComponentMesh*)train->GetChild(0)->GetComponent(CMP_MESH);

	ComponentMesh* box_mesh = (ComponentMesh*)reverb_zone->GetChild(0)->GetComponent(CMP_MESH);

	if (train_mesh && box_mesh)
	{
		if (train_mesh->GetMesh()->bb.Intersects(box_mesh->GetMesh()->bb))
		{
			return true;
		}
	}
	return false;
}




