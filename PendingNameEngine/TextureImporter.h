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
	int GetILVersion();
	uint LoadTextureFromPath(const char* path);

	char* current_path="";

};

#endif _TEXTUREIMPORTER_H_