#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"


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
	selected_go = new GameObject();

	//Editor Camera
	App->camera->SetCamPos({0.0,10.f,5.f});
	App->camera->LookAt({ 0,0,0 });

	//Game Cam Test
	GameObject* game_cam = new GameObject();
	game_cam->go_name = "Main Camera";
	AddGameObject(game_cam);
	SetSelectedGO(game_cam);
	game_cam->SetSelected(true);

	ComponentCamera* cmp_cam = (ComponentCamera*)game_cam->AddComponent(CMP_CAMERA);

	//Initial Mesh
	ret = App->importer->Import(".//Assets//BakerHouse.fbx");
	App->importer->first_load = false;
	ret = App->importer->Import(".//Assets//Baker_house.png");

	return ret;
}


update_status ModuleScene::Update(float dt)
{

	if (!to_delete.empty())
		DeleteGameObjects();

	return UPDATE_CONTINUE;
}


bool ModuleScene::CleanUp()
{
	return true;
}


void ModuleScene::DrawScene()
{
	PPlane grid(0, 1, 0, 5);
	grid.axis = true;
	grid.color = { 1.0f,1.0f,1.0f };

	App->renderer3D->DebugRenderSettings();
	grid.Render();
	App->renderer3D->SetDefaultSettings();

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		(*it)->Draw();
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
	App->scene->selected_go = nullptr;
}

void ModuleScene::DeleteGameObjects()
{

	for (auto it = to_delete.begin(); it != to_delete.end();)
	{
		(*it)->DeleteComponents();

		if ((*it)->parent != nullptr)
			(*it)->parent->DeleteChild((*it));

		(*it)->parent = nullptr;

		DeleteGameObject((*it));

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
		}
	}
	
}

void ModuleScene::AddGameObjectToDelete(GameObject * go_to_delete)
{
	to_delete.push_back(go_to_delete);
}

void ModuleScene::SetSelectedGO(GameObject * go)
{
	selected_go = go;
}
