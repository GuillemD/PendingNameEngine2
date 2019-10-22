#ifndef _MESHIMPORTER_H_
#define _MESHIMPORTER_H_

#include "Globals.h"
#include "Mesh.h"

class GameObject;
class aiScene;
class aiNode;
class Mesh;


class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool CleanUp();

	bool ImportMesh(const char* full_path);
	void LoadMesh(const aiScene* _scene, const aiNode* _node, GameObject* parent, const char* _full_path);


};

#endif // !_MESHIMPORTER_H_
	
