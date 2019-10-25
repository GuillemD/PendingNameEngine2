#include "Material.h"
#include "OpenGL.h"


Material::Material()
{
}


Material::~Material()
{
}

Texture * Material::GetDiffuse() const
{
	return diff_texture;
}

void Material::SetDiffuse(Texture * texture)
{
	if (texture != nullptr)
		diff_texture = texture;
}

void Material::UnloadMaterial()
{
	uint tex_id = diff_texture->GetTextureId();
	glDeleteTextures(1, (GLuint*)&tex_id);
	diff_texture->SetTextureId(0);
}
