#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_

#include "Component.h"
#include "Texture.h"
class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();
	Texture* GetTexture();
	bool SetTexture(Texture* newtex);

private:
	Texture* texture = nullptr;
};





#endif // !_COMPONENT_MATERIAL_



