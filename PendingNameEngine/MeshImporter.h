#ifndef _MESHIMPORTER_H_
#define _MESHIMPORTER_H_

#include "Mesh.h"
#include "Globals.h"

class Mesh;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool Update();
	bool CleanUp();

	//Mesh* CreateCube();
	//Mesh* CreatePARCube();
};

#endif // !_MESHIMPORTER_H_
	
