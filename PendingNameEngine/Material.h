#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Texture.h"
#include "Color.h"

class Material
{
public:
	Material();
	~Material();

	Texture* GetDiffuse() const;
	void SetDiffuse(Texture* texture);

	void UnloadMaterial();

public:
	Color color = { 1.0f,1.0f,1.0f };
	Texture* diff_texture = nullptr;

};

#endif // !_MATERIAL_H_



