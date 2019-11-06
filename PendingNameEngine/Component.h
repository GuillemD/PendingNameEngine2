#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Globals.h"
#include "GameObject.h"

enum ComponentTYPE { CMP_TRANSFORM = 0, CMP_MESH, CMP_MATERIAL, CMP_CAMERA };

class GameObject;

class Component
{
public:
	Component();
	Component(GameObject* p);
	~Component();

	virtual bool Update();
	virtual bool CleanUp();

	virtual void Draw();

	ComponentTYPE GetType() const { return type; }
	void SetType(ComponentTYPE _type) { type = _type; }

	GameObject* GetOwner() const { return owner; }
	void SetOwner(GameObject* new_go) { owner = new_go; }

public:
	GameObject* owner = nullptr;
	ComponentTYPE type;
	bool active;
};
#endif // !_COMPONENT_H_
