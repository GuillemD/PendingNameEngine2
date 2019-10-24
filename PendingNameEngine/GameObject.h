#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
#include <list>

enum ComponentTYPE;
class Component;
class ComponentTransform;
class ComponentMesh;

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

	//childs
	void AddChild(GameObject* child);

	//components
	Component* GetComponent(ComponentTYPE _type) const;
	Component* AddComponent(ComponentTYPE _type);

	//utility
	bool IsActive() const;
	void SetActive(bool a);

	bool IsStatic() const;
	void SetStatic(bool st);

	bool IsSelected() const;
	void SetSelected(bool sel);
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
