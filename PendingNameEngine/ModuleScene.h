#ifndef _MODULE_SCENE_
#define _MODULE_SCENE_
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"
#include "Texture.h"
#include "GameObject.h"
class ModuleScene :
	public Module {
public:
	ModuleScene();
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	
	update_status Update(float dt);

	bool CleanUp();
	
	void CreateAABB(int min_x, int min_y,int min_z, int max_x, int max_y, int max_z);
	//void CheckAABBCollisions();

	void DrawScene();
	void ClearScene();

	void AddGameObject(GameObject* go);

public:
	GameObject* root = nullptr;
	std::vector<GameObject*> scene_gameobjects;
	std::vector<AABB*> boxes;
	std::vector<Texture*> textures;
};

#endif // !_MODULE_SCENE_



