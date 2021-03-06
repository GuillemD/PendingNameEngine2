#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/Plane.h"


#define NEAR 0.25f
#define FAR 100.0f

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* p);
	~ComponentCamera();


	bool Update();
	bool CleanUp();
	void Draw();


	float* GetViewMatrix()const ;
	float4x4 GetViewMatrix4x4() const;
	float* GetProjectionMatrix() const;
	float4x4 GetProjectionMatrix4x4() const;
	const float* GetGLViewMatrix() const;
	void UpdateProjectionMatrix();


	void SetNearPlaneDist(float np);
	float GetNearPlaneDist() const;

	void SetFarPlaneDist(float fp);
	float GetFarPlaneDist() const;

	void SetFOV(float new_fov);
	float GetFOV() const;

	float GetAspectRatio()const;

	void SetEditor(bool set);

	float GetSpeed() const;
	void SetSpeed(const float new_speed);

	//Frustum
	math::Frustum GetFrustum() const;
	void UpdateFrustum();
	void DrawFrustum();
	bool ContainsAABB(AABB& bb);

public:

	Frustum camera_frustum;
	Color bg_color;

	bool is_editor;
	bool draw_frustum;
	bool frustum_cull;
	float speed;

};



#endif // !_COMPONENTCAMERA_H_

