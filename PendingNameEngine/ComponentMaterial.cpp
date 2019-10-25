#include "ComponentMaterial.h"
#include "GameObject.h"




ComponentMaterial::ComponentMaterial()
{

	type = CMP_MATERIAL;
	active = true;
	material = new Material();
}

ComponentMaterial::ComponentMaterial(GameObject* p)
{
	SetOwner(p);
	type = CMP_MATERIAL;
	active = true;
	material = new Material();
}


ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::CleanUp()
{
	if (material != nullptr)
	{
		material->UnloadMaterial();
	}
	return true;
}

Material * ComponentMaterial::GetMaterial() const
{
	return material;
}

void ComponentMaterial::SetMaterial(Material * mat)
{
	if (mat != nullptr)
		material = mat;
}

Color ComponentMaterial::GetColor() const
{
	return material->color;
}

void ComponentMaterial::SetColor(Color col)
{
	material->color = col;
}

