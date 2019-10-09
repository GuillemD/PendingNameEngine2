#ifndef _MESHIMPORTER_H_
#define _MESHIMPORTER_H_

#include "Globals.h"

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool Update();
	bool CleanUp();

};

#endif // !_MESHIMPORTER_H_
	
