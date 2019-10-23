#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
#include <list>

class Component;
enum ComponentTYPE;
class ComponentTransform;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void Draw();

	//parent
	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);

	void AddChild(GameObject* child);

	//components
	Component* GetComponent(ComponentTYPE _type) const;
	Component* AddComponent(ComponentTYPE _type);
public:
	GameObject* parent = nullptr;
	bool selected;
	std::string go_name;
	std::list<Component*> components;
	ComponentTransform* trans;

private:
	bool is_active;
	bool is_static;
	std::vector<GameObject*> childs;
	
};
#endif // !_GAMEOBJECT_H_
