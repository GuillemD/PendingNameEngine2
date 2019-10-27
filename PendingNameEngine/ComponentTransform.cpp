#include "ComponentTransform.h"
#include "GameObject.h"




ComponentTransform::ComponentTransform(GameObject* p)
{
	SetOwner(p);
	transform.pos = float3::zero;
	transform.rot = Quat::identity;
	transform.scale = float3::one;
	transform.euler_rot = float3::zero;

	type = CMP_TRANSFORM;
	active = true;

}


ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::CleanUp()
{
	return true;
}

float3 ComponentTransform::GetPosition() const
{
	return transform.pos;
}

void ComponentTransform::SetPosition(float3 _pos)
{
	transform.pos = _pos;
}

Quat ComponentTransform::GetRotation() const
{
	return transform.rot;
}

void ComponentTransform::SetRotation(Quat _rot)
{
	transform.rot = _rot;
}

float3 ComponentTransform::GetScale() const
{
	return transform.scale;
}

void ComponentTransform::SetScale(float3 _scale)
{
	transform.scale = _scale;
}

float3 ComponentTransform::GetEulerRotation() const
{
	return transform.euler_rot;
}

void ComponentTransform::SetEulerRotation(float3 _euler)
{
	transform.euler_rot = _euler;
}
