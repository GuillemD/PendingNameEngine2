#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "Globals.h"

#include <string>

using namespace std;
class Texture
{
public:
	Texture();
	Texture(uint id, uint w, uint h);
	Texture(uint id, uint w, uint h, string name_);
	~Texture();

	const char* GetName() const { return tex_name.c_str(); }
	void SetName(const char* n) { tex_name = n; }

	uint GetTextureId() const { return texture_id; }
	void SetTextureId(uint id) { texture_id = id; }

	uint GetWidth() const { return width; }
	void SetWidth(uint w) { width = w; }

	uint GetHeight() const { return height; }
	void SetHeight(uint h) { height = h; }

	uint texture_id;
	uint width;
	uint height;
	string tex_name = "";
	string tex_path = "";
};

#endif // !_TEXTURE_H_


