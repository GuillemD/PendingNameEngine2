#include "Texture.h"



Texture::Texture()
{
}

Texture::Texture(uint id, uint w, uint h)
{
	texture_id = id;
	width = w;
	height = h;
}
Texture::Texture(uint id, uint w, uint h,string name_)
{
	texture_id = id;
	width = w;
	height = h;
	tex_name = name_;
}


Texture::~Texture()
{
}
