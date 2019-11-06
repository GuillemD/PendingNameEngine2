#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/Plane.h"

#define NEAR 1.0f
#define FAR 1000.0f
#define H_FOV 90
#define ASPECT_RATIO 0.992f

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* p);
	~ComponentCamera();

	bool ContainsAABB(AABB& bb);
	float GetNearPlaneDistance() const;
	float GetFarPlaneDistance() const;

	void SetFOV(float new_fov);
	float GetFOV() const;

	void SetAspectRatio(float new_ar);

public:

	Frustum camera_frustum;
	Color bg_color;

	float horizontal_fov;

	float aspect_ratio;
};



#endif // !_COMPONENTCAMERA_H_

