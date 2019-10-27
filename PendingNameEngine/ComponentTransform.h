#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENT_TRANSFORM_H_

#include "Globals.h"
#include "Component.h"
#include "Transform.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* p);
	~ComponentTransform();

	bool CleanUp();

	float3 GetPosition() const;
	void SetPosition(float3 _pos);

	Quat GetRotation() const;
	void SetRotation(Quat _rot);

	float3 GetScale() const;
	void SetScale(float3 _scale);

	float3 GetEulerRotation() const;
	void SetEulerRotation(float3 _euler);

public:
	Transform transform;
	
};


#endif // !_COMPONENT_TRANSFORM_H_
