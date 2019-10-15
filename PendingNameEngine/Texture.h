#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "Globals.h"

class Texture
{
public:
	Texture();
	Texture(uint id, uint w, uint h);
	~Texture();

	uint texture_id;
	uint width;
	uint height;
};






#endif // !_TEXTURE_H_


