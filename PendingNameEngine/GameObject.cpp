#include "GameObject.h"
#include "Application.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "OpenGL.h"

GameObject::GameObject()
{
	go_name = "";
	selected = false;
	is_active = true;
	is_static = false;
	go_bb = nullptr;

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
	if (!is_active)
		return;

	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Draw();
	}
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * new_parent)
{
	parent = new_parent;
}

void GameObject::AddChild(GameObject * child)
{
	if (child != nullptr)
	{
		child->SetParent(this);
		childs.push_back(child);
	}
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

bool GameObject::IsActive() const
{
	return is_active;
}

void GameObject::SetActive(bool a)
{
	is_active = a;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetStatic(bool st)
{
	is_static = st;
}

bool GameObject::IsSelected() const
{
	return selected;
}

void GameObject::SetSelected(bool sel)
{
	selected = sel;
}

void GameObject::PrintMyHierarchy()
{
	if (childs.empty() != true) {
		if (ImGui::TreeNodeEx(go_name.c_str())) {

			for (auto c : childs) {
				c->PrintMyHierarchy();

			}
			ImGui::TreePop();
		}
	}
	else {
		bool selected = false;
		ImGui::Selectable(go_name.c_str(), &selected);
	}
}


