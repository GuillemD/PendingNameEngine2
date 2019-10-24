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

	root = new GameObject();
	root->go_name = "RootGameObject";
	scene_gameobjects.push_back(root);

	ret = App->importer->Import(".//Assets//BakerHouse.fbx");
	//ret = App->importer->Import(".//Assets//Baker_house.png");

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

void ModuleScene::CreateAABB(int min_x, int min_y,int min_z, int max_x, int max_y, int max_z)
{
	if (min_x < max_x &&min_y < max_y&&min_z < max_z) {
		AABB* tmp = new AABB(float3(min_x, min_y, min_z), float3(max_x, max_y, max_z));
		boxes.push_back(tmp);
		CONSOLELOG("Created AABB with coordinates Minimum Coordinates: %d , %d , %d . Maximum Coordinates: %d , %d , %d .", min_x, min_y, min_z, max_x, max_y, max_z);


	} else CONSOLELOG("Error creating an AABB with those coordinates, is any minimum coordinate higher than the maximum one?")


	int test = 0;
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
	scene_gameobjects.clear();
	App->importer->mesh_path = "";
}

void ModuleScene::AddGameObject(GameObject * go)
{
	scene_gameobjects.push_back(go);
}
