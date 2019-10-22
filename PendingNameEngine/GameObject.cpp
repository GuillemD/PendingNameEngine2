#include "GameObject.h"
#include "Application.h"

GameObject::GameObject()
{
	go_name = "";
	parent = nullptr;
	selected = false;
	is_active = true;
	is_static = false;

	//create component transform
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
