#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_

#include "Component.h"
#include "Texture.h"
#include "Material.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* p);
	~ComponentMaterial();

	bool CleanUp();

	Material* GetMaterial() const;
	void SetMaterial(Material* mat);

	Color GetColor() const;
	void SetColor(Color col);

private:
	Material* material = nullptr;
};





#endif // !_COMPONENT_MATERIAL_



