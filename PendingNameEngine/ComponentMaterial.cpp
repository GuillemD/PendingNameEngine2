#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial()
{
}


ComponentMaterial::~ComponentMaterial()
{
}

Texture * ComponentMaterial::GetTexture()
{
	return texture;
}

bool ComponentMaterial::SetTexture(Texture* newtex)
{
	texture = newtex;
	if (texture != nullptr)
		return true;
	return false;
}
