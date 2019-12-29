#include "GameObject.h"
#include "Application.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "ComponentCamera.h"
#include "ComponentAudioListener.h"
#include "ComponentAudioSource.h"

#include "OpenGL.h"

#include "mmgr/mmgr.h"

GameObject::GameObject()
{
	go_name = "";
	selected = false;
	is_root = false;
	is_active = true;
	is_static = false;
	UniqueId = (long long)pcg32_random_r(&App->gui->seed);

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

	for (auto it = components.begin(); it != components.end(); it++)
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
	this->Update();

}

void GameObject::Delete()
{
	App->scene->AddGameObjectToDelete(this);
}

void GameObject::DeleteGameObject()
{
	if (this->IsSelected())
	{
		App->scene->SetSelectedGO(nullptr);
		selected = false;
	}
	if (!childs.empty())
	{
		for (auto it = childs.begin(); it != childs.end(); it++)
		{
			if ((*it)->IsStatic())
			{
				(*it)->SetStatic(false);
			}

			(*it)->DeleteGameObject();
		}
	}
	this->Delete();
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
			case CMP_CAMERA:
				aux = new ComponentCamera(this);
				break;
			case CMP_A_LISTENER:
				aux = new ComponentAudioListener(this);
				break;
			case CMP_A_SOURCE:
				aux = new ComponentAudioSource(this);
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
		delete (*it);
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
	if (st)
	{
		if (!is_static)
		{
			App->scene->AddStaticGO(this);
			App->scene->octree->Insert(this);
		}
			
	}
	else
	{
		if (is_static)
		{
			App->scene->RemoveGoFromStaticList(this);
			App->scene->octree->Erase(this);
		}
			
	}

	is_static = st;

	auto child_list = this->childs.begin();
	while (child_list != this->childs.end())
	{
		(*child_list)->SetStatic(st);
		child_list++;
	}
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
		
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			for (auto it = App->scene->scene_gameobjects.begin(); it != App->scene->scene_gameobjects.end(); it++)
			{
				if ((*it)->IsSelected())
				{
					(*it)->SetSelected(false);
				}
			}
			this->SetSelected(true);
			App->scene->SetSelectedGO(this);
			
		}
		if (ImGui::IsItemClicked(1))
		{
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
			ImGui::OpenPopup("Options");
		}
		if (ImGui::BeginPopup("Options"))
		{
			if (App->scene->selected_go != nullptr)
			{
				if (ImGui::MenuItem("Delete"))
				{
					App->scene->selected_go->DeleteGameObject();
					App->scene->octree->Update();
				}
			}
			ImGui::EndPopup();
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

math::float4x4 GameObject::GetGlobalMatrix()
{
	return trans->GetGlobalMatrix();
}

void GameObject::UpdateCam()
{
	ComponentCamera* cam = (ComponentCamera*)this->GetComponent(CMP_CAMERA);
	if (cam != nullptr)
	{
		cam->Update();
	}
}

void GameObject::UpdateBoundingBox()
{
	ComponentMesh* mesh = (ComponentMesh*)this->GetComponent(CMP_MESH);

	if (mesh != nullptr)
	{
		mesh->UpdateBB();
	}
}


