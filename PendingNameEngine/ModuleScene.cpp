#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "ComponentMesh.h"

#include "ModuleImporter.h"
#include "MeshImporter.h"
#include "ModuleInput.h"

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
	App->importer->first_load = false;
	ret = App->importer->Import(".//Assets//Baker_house.png");
	
	return ret;
}


update_status ModuleScene::Update(float dt)
{

	if (!to_delete.empty())
		DeleteGameObjects();

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		GameObject* testGo= new GameObject();
		testGo->AddComponent(CMP_MESH);
		testGo->AddComponent(CMP_TRANSFORM);
		ComponentMesh* second =(ComponentMesh*)testGo->GetComponent(CMP_MESH);
		second->SetMesh(App->importer->mesh_import->LoadOwnFileFormat("Library/Meshes/Baker_house.caca"));
		
		
		
		
	}

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
