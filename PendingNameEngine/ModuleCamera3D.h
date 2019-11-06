#ifndef _MODULECAMERA_H_
#define _MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ShowCameraConfig();

	void Focus(const AABB& box);
	float3 RotateCam(const float3& u, const float3& v, float angle);
	float* GetViewMat();

private:
	
	GameObject* editor_cam;

	bool isPlaying = false;
public:

	bool can_focus = false;	
	
};

#endif //_MODULECAMERA_H_