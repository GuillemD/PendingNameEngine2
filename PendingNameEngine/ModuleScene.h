#ifndef _MODULE_SCENE_
#define _MODULE_SCENE_
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"
#include "Texture.h"
#include "GameObject.h"

#include "Octree.h"
#include "ImGui/ImGuizmo/ImGuizmo.h"

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
	void DrawGameObjects(ComponentCamera* cam_to_draw, bool is_editor);
	void ClearScene();
	void DeleteGameObjects();
	void DeleteGameObject(GameObject* go_to_delete);

	void AddGameObject(GameObject* go);
	void AddGameObjectToDelete(GameObject* go_to_delete);

	void SetSelectedGO(GameObject* go);

	void AddStaticGO(GameObject* go);
	void RemoveGoFromStaticList(GameObject* go);
	
	void ShowOctreeConfig();

	void DrawGizmo();
	void TestMouseRayHit(LineSegment ray);
	GameObject* GetClosestGO(LineSegment ray, list<GameObject*> list);

public:
	GameObject* selected_go = nullptr;
	std::vector<GameObject*> scene_gameobjects;
	std::list<GameObject*> static_gameobjects;
	std::list<GameObject*> root_gameobjects;
	std::list<ComponentCamera*> cameras;

	Octree* octree;

	Texture* checkers=nullptr;

	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;

	//AUDIO
	GameObject* default_listener = nullptr;
	GameObject* bg_music = nullptr;
	
private:
	std::vector<GameObject*> to_delete;
	float3 last_gizmo_scale;
	

};

#endif // !_MODULE_SCENE_



