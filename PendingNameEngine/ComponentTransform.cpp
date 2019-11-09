#include "ComponentTransform.h"
#include "GameObject.h"
#include "ComponentMesh.h"


ComponentTransform::ComponentTransform(GameObject* p)
{
	SetOwner(p);
	transform.pos = float3::zero;
	transform.rot = Quat::identity;
	transform.scale = float3::one;
	transform.euler_rot = float3::zero;

	global_mat.SetIdentity();

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

float3 ComponentTransform::GetGlobalPosition() const
{
	if (owner->IsRoot())
		return transform.pos;
	else
	{
		ComponentTransform* parent = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
		return (transform.pos + parent->transform.pos);
	}
}

float3 ComponentTransform::GetLocalPosition() const
{
	return transform.pos;
}



void ComponentTransform::SetPosition(float3 _pos)
{
	transform.pos = _pos;
	CalculateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalRotation() const
{
	if (owner->GetParent() != nullptr)
	{
		ComponentTransform* parent = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
		return GetLocalRotation() + parent->GetLocalRotation();
	}
	return GetLocalRotation();
}

float3 ComponentTransform::GetLocalRotation() const
{
	return transform.euler_rot;
}

void ComponentTransform::SetRotation(float3 _euler)
{
	transform.euler_rot = _euler;
	transform.rot = Quat::FromEulerXYZ(DEGTORAD*_euler.x, DEGTORAD*_euler.y, DEGTORAD*_euler.z);
	CalculateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalScale() const
{
	if (owner->IsRoot())
		return transform.scale;
	else
	{
		ComponentTransform* parent = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
		return (transform.scale + parent->transform.scale);
	}
}

float3 ComponentTransform::GetLocalScale() const
{
	return transform.scale;
}


void ComponentTransform::SetScale(float3 _scale)
{
	transform.scale = _scale;
	CalculateGlobalMatrix();
}

const float4x4 ComponentTransform::GetGlobalMatrix() const
{
	return global_mat;
}

void ComponentTransform::CalculateGlobalMatrix()
{
	if (owner->IsRoot())
	{
		global_mat = float4x4::FromTRS(transform.pos, transform.rot, transform.scale);
		auto child = owner->GetAllChilds()->begin();
		while (child != owner->GetAllChilds()->end())
		{
			ComponentTransform* child_trans = (ComponentTransform*)(*child)->GetComponent(CMP_TRANSFORM);
			child_trans->CalculateGlobalMatrix();
			child++;
		}
	}
	else
	{
		ComponentTransform* parent_trans = (ComponentTransform*)GetOwner()->GetParent()->GetComponent(CMP_TRANSFORM);

		global_mat = global_mat.FromTRS(transform.pos, transform.rot, transform.scale);
		global_mat = parent_trans->global_mat * global_mat;
	}
	update = true;
}

const float * ComponentTransform::GetGLMatrix()
{
	return (float*)global_mat.Transposed().v;
}

void ComponentTransform::SetViewMatrixFromCam(float4x4 mat)
{
	float3 pos = { 0,0,0 };
	Quat rot = { 0,0,0,0 };
	float3 scale = { 0,0,0 };

	float4x4 rotation = rot.ToFloat4x4();

	mat.Transpose();
	mat.Inverse();
	mat.Decompose(pos, rotation, scale);

	float3 euler = rotation.ToEulerXYZ();

	rot = rot.FromEulerXYZ(euler.x, euler.y, euler.z);

	SetUpdate(true);
	transform.pos = pos;
	SetRotation(euler);
}

bool ComponentTransform::ToUpdate() const
{
	return update;
}

void ComponentTransform::SetUpdate(bool set)
{
	update = set;
}



