#ifndef _MODULECAMERA_H_
#define _MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"



class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void ShowCameraConfig();

	void Focus(const AABB& box);


private:
	
	void CalculateViewMatrix();
	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool isPlaying = false;
public:
	
	vec3 X, Y, Z, Position, Reference;

	bool can_focus = false;
	
};

#endif //_MODULECAMERA_H_