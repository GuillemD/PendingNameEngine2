#include "GameObject.h"
#include "Application.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
	go_name = "";
	selected = false;
	is_active = true;
	is_static = false;

	//create component transform
	Component* cmp = new ComponentTransform(this);
	components.push_back(cmp);
	trans = (ComponentTransform*)cmp;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (!is_active)
		return;

	//update components
	for (auto it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->Update();
	}
}

void GameObject::Draw()
{
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * new_parent)
{
	parent = new_parent;
}

Component * GameObject::GetComponent(ComponentTYPE _type) const
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == _type && (*it))
			return (*it);
	}
	return nullptr;
}

Component * GameObject::AddComponent(ComponentTYPE _type)
{
	Component* aux = GetComponent(_type);
	if (aux != nullptr)
		return GetComponent(_type);
	else
	{
		switch (_type)
		{
			case CMP_TRANSFORM:
				aux = new ComponentTransform(this);
				break;
			case CMP_MESH:
				aux = new ComponentMesh(this);
				break;
			case CMP_MATERIAL:
				break;
		}

	}
	components.push_back(aux);
	return aux;
}
