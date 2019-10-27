#include "Component.h"


Component::Component()
{
}

Component::Component(GameObject* p)
{
}

Component::~Component()
{
}

bool Component::Update()
{
	return false;
}

bool Component::CleanUp()
{
	return false;
}

void Component::Draw()
{
}
