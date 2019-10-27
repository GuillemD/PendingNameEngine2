#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"



ModuleScene::ModuleScene()
{
	name = "Scene";
}

ModuleScene::ModuleScene(bool start_enabled)
{
}


ModuleScene::~ModuleScene()
{
}


bool ModuleScene::Start()
{
	bool ret = true;
	selected_go = new GameObject();

	ret = App->importer->Import(".//Assets//BakerHouse.fbx");
	ret = App->importer->Import(".//Assets//Baker_house.png");

	return ret;
}


update_status ModuleScene::Update(float dt)
{
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
			//to_delete.push_back((*it));
			if ((*it)->GetParent() == nullptr)
				(*it)->DeleteGameObject();
		}
		DeleteAllGOs();
	}
	//scene_gameobjects.clear();
	App->importer->mesh_path = "";
}

void ModuleScene::DeleteAllGOs()
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
			to_delete.push_back(go_to_delete);
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

void ModuleScene::SetSelectedGO(GameObject * go)
{
	selected_go = go;
}
