#ifndef _TEXTUREIMPORTER_H_
#define _TEXTUREIMPORTER_H_


#include "Globals.h"

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	bool Start();
	bool CleanUp();

	uint LoadTextureFromPath(const char* path);
	int GetVersion() const;

	char* current_path="";

};

#endif _TEXTUREIMPORTER_H_