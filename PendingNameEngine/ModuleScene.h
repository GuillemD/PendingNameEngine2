#ifndef _MODULE_SCENE_
#define _MODULE_SCENE_
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"
#include "Texture.h"
#include "GameObject.h"

class ComponentCamera;
class ModuleScene :
	public Module {
public:
	ModuleScene();
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	
	update_status PreUpdate(float dt);
	update_status Update(float dt);

	bool CleanUp();

	void DrawScene();
	void DrawGameObjects(ComponentCamera* cam_to_draw);
	void ClearScene();
	void DeleteGameObjects();
	void DeleteGameObject(GameObject* go_to_delete);

	void AddGameObject(GameObject* go);
	void AddGameObjectToDelete(GameObject* go_to_delete);

	void SetSelectedGO(GameObject* go);

public:
	GameObject* selected_go = nullptr;
	std::vector<GameObject*> scene_gameobjects;
	std::list<GameObject*> root_gameobjects;
	std::list<ComponentCamera*> cameras;

	Texture* checkers=nullptr;
	
private:
	std::vector<GameObject*> to_delete;

	

};

#endif // !_MODULE_SCENE_



