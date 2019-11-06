#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/Plane.h"


#define NEAR 0.25f
#define FAR 300.0f

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* p);
	~ComponentCamera();


	bool Update();
	bool CleanUp();
	void Draw();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);

	void CalculateViewMatrix();
	float* GetViewMatrix();
	const float* GetGLViewMatrix();
	void UpdateProjectionMatrix();

	bool ContainsAABB(AABB& bb);
	math::Frustum* GetFrustum();

	void SetNearPlaneDist(float np);
	float GetNearPlaneDist() const;

	void SetFarPlaneDist(float fp);
	float GetFarPlaneDist() const;

	void SetFOV(float new_fov);
	float GetFOV() const;

	float GetAspectRatio()const;

	void SetEditor(bool set);

	//Frustum
	void UpdateFrustum();
	void DrawFrustum();

public:

	float3 X, Y, Z, Position, Reference;

	Frustum camera_frustum;
	Color bg_color;

	bool is_editor;
	bool draw_frustum;
	float sensitivity;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;

};



#endif // !_COMPONENTCAMERA_H_

