#include "GameObject.h"
#include "Application.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "OpenGL.h"

GameObject::GameObject()
{
	go_name = "";
	selected = false;
	is_root = false;
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

void GameObject::DeleteGameObject()
{
	if (this->IsSelected())
	{
		App->scene->SetSelectedGO(nullptr);
	}
	if (!childs.empty())
	{
		for (auto it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->DeleteGameObject();
		}
	}
	App->scene->AddGameObjectToDelete(this);

}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * new_parent)
{
	if (this->parent == new_parent)
		return;

	if (parent)
		this->parent->DeleteChild(this);

	this->parent = new_parent;

	if (new_parent != nullptr)
		new_parent->childs.push_back(this);

	is_root = false;
	App->scene->root_gameobjects.remove(this);
}

void GameObject::AddChild(GameObject * child)
{
	if (child != nullptr)
	{
		child->SetParent(this);
	}
}

void GameObject::SetChild(GameObject * child)
{
	if (child != nullptr)
	{
		child->parent = this;
	}
}

GameObject * GameObject::GetChild(uint id) const
{
	uint i = 0;

	for (auto it = childs.begin(); it != childs.end(); it++, i++)
	{
		if (i == id)
			return (*it);
	}
	return nullptr;
}

void GameObject::DeleteChild(GameObject * child_to_delete)
{
	if (!childs.empty())
	{
		for (auto it = childs.begin(); it != childs.end(); it++)
		{
			if ((*it) == child_to_delete)
			{
				childs.erase(it);
				return;
			}
		}
	}
}

std::vector<GameObject*>* GameObject::GetAllChilds() 
{
	return &childs;
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
				aux = new ComponentMaterial(this);
				break;
		}

	}
	components.push_back(aux);
	return aux;
}

void GameObject::RemoveComponent(ComponentTYPE _type)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == _type)
		{
			(*it)->CleanUp();
			components.erase(it);
		}
	}
}

void GameObject::PushComponent(Component * cmp)
{
	components.push_back(cmp);
}

void GameObject::DeleteComponents()
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->CleanUp();
	}
		
	components.clear();
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

bool GameObject::IsRoot() const
{
	return is_root;
}

void GameObject::PrintMyHierarchy()
{
	uint flags = 0;

	if (this->childs.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	ComponentMesh* c_m = (ComponentMesh*)GetComponent(CMP_MESH);

	if (this->IsSelected())
	{
		flags |= ImGuiTreeNodeFlags_Selected;

		
		if (c_m != nullptr)
		{
			c_m->draw_bb = true;
		}
	}
	else
	{
		if(c_m != nullptr)
			c_m->draw_bb = false;
	}
		

	if (ImGui::TreeNodeEx(this->go_name.c_str(), flags))
	{
		if (ImGui::IsMouseClicked(0))
		{
			if (ImGui::IsItemHovered())
			{
				if (string(App->scene->selected_go->go_name) != string(this->go_name))
				{
					App->scene->selected_go->SetSelected(false);
					this->SetSelected(true);
					App->scene->SetSelectedGO(this);
				}
			}
		}
		if (!this->childs.empty())
		{
			for (std::vector<GameObject*>::iterator it = this->childs.begin(); it != this->childs.end(); it++)
			{
				(*it)->PrintMyHierarchy();
			}
		}
		
		ImGui::TreePop();
	}

}

const float * GameObject::GetGLMatrix()
{
	return trans->GetGLMatrix();
}


