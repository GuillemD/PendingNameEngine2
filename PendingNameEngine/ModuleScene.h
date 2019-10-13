#ifndef _MODULE_SCENE_
#define _MODULE_SCENE_
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"

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
	void CheckAABBCollisions();

	void DrawScene();

public:

	std::vector<AABB*> boxes;
	std::vector<Mesh*> scene_meshes;

};






#endif // !_MODULE_SCENE_



