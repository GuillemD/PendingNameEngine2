#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
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

	void DeleteGameObject();

	//parent
	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);

	//childs
	void AddChild(GameObject* child);
	void SetChild(GameObject* child);
	GameObject* GetChild(uint id)const;
	void DeleteChild(GameObject* child_to_delete);

	//components
	Component* GetComponent(ComponentTYPE _type) const;
	Component* AddComponent(ComponentTYPE _type);
	void RemoveComponent(ComponentTYPE _type);
	void PushComponent(Component* cmp);
	void DeleteComponents();

	//utility
	bool IsActive() const;
	void SetActive(bool a);

	bool IsStatic() const;
	void SetStatic(bool st);

	bool IsSelected() const;
	void SetSelected(bool sel);

	bool IsRoot() const;

	void PrintMyHierarchy();

public:
	GameObject* parent = nullptr;
	bool selected;
	bool is_root;
	std::string go_name;
	std::list<Component*> components;
	ComponentTransform* trans;
	AABB* go_bb;

private:
	bool is_active;
	bool is_static;
	std::vector<GameObject*> childs;
	
};
#endif // !_GAMEOBJECT_H_
