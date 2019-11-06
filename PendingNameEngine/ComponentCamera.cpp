#include "ComponentCamera.h"
#include "GameObject.h"



ComponentCamera::ComponentCamera(GameObject* p)
{
	SetOwner(p);

	type = CMP_CAMERA;
	active = true;

	camera_frustum.type = FrustumType::PerspectiveFrustum;
	camera_frustum.nearPlaneDistance = NEAR;
	camera_frustum.farPlaneDistance = FAR;
	bg_color = Black;

	horizontal_fov = 90;
	aspect_ratio = ASPECT_RATIO;

	SetFOV(DegToRad(60));

}


ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::ContainsAABB(AABB & bb)
{
	int num_vertices = bb.NumVertices();
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < num_vertices; j++)
		{
			Plane plane = camera_frustum.GetPlane(i);
			if (plane.Contains(bb.CornerPoint(j)))
			{
				return true;
			}
		}
	}
	return false;
}

float ComponentCamera::GetNearPlaneDistance() const
{
	return camera_frustum.nearPlaneDistance;
}

float ComponentCamera::GetFarPlaneDistance() const
{
	return camera_frustum.farPlaneDistance;
}

void ComponentCamera::SetFOV(float new_fov)
{
	camera_frustum.verticalFov = new_fov;
	camera_frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(camera_frustum.verticalFov / 2)) * 2;
}

float ComponentCamera::GetFOV() const
{
	return camera_frustum.verticalFov;
}

void ComponentCamera::SetAspectRatio(float new_ar)
{
	aspect_ratio = new_ar;
	camera_frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(camera_frustum.verticalFov / 2)) * 2;
}
