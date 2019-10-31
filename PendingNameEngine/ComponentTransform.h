#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENT_TRANSFORM_H_

#include "Globals.h"
#include "Component.h"
#include "Transform.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* p);
	~ComponentTransform();

	bool CleanUp();

	float3 GetGlobalPosition() const;
	float3 GetLocalPosition() const;
	void SetPosition(float3 _pos);

	float3 GetGlobalRotation() const;
	float3 GetLocalRotation() const;
	void SetRotation(float3 _euler);

	float3 GetGlobalScale() const;
	float3 GetLocalScale() const;
	void SetScale(float3 _scale);

	const float4x4 GetGlobalMatrix() const;
	void CalculateGlobalMatrix();

	const float* GetGLMatrix();

	bool ToUpdate() const;
	void SetUpdate(bool set);


public:
	Transform transform;
private:
	float4x4 global_mat;
	bool update = false;
	
};


#endif // !_COMPONENT_TRANSFORM_H_
