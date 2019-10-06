#include "ModuleScene.h"
#include "Application.h"
#include "Globals.h"
#include "Primitive.h"


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

bool ModuleScene::Init()
{
	return true;
}

bool ModuleScene::Start()
{
	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
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

void ModuleScene::CheckAABBCollisions()
{
	for (int i = 0; i < boxes.size(); i++) {
		for (int j = 0; j < boxes.size(); j++) {
			
			if (i != j) {
				if (boxes[i]->Intersects(*boxes[j])) {

					CONSOLELOG("Box %d intersects with box %d", i, j);

				}
			}		
		}	
	}
}

void ModuleScene::DrawScene()
{
	PPlane grid(0, 1, 0, 5);
	grid.axis = true;
	grid.color = White;
	grid.Render();
}
