#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "ComponentMesh.h"


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


		if ((*it)->GetComponent(CMP_MESH)!=nullptr) {
			ComponentMesh* tmp = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

			if (tmp->GetMesh()->drawnormals) {
				tmp->GetMesh()->DrawFacesNormals();
			}
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
