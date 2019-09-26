#ifndef _MODULE_SCENE_
#define _MODULE_SCENE_
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"
class ModuleScene :
	public Module {
public:
	ModuleScene();
	ModuleScene(bool start_enabled = true);
	~ModuleScene();




	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	
	void CreateAABB(int min_x, int min_y,int min_z, int max_x, int max_y, int max_z);
	void CheckAABBCollisions();

public:

	std::vector<AABB*> boxes;

};






#endif // !_MODULE_SCENE_



