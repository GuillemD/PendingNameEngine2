#ifndef _TEXTUREIMPORTER_H_
#define _TEXTUREIMPORTER_H_


#include "Globals.h"
#include "Texture.h"

class Material;

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	bool Start();
	bool CleanUp();

	Texture* LoadTextureFromPath(const char* path);
	int GetVersion() const;

	char* current_path="";

	bool CreateCheckersTexture();

};

#endif _TEXTUREIMPORTER_H_