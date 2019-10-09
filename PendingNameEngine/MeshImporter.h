#ifndef _MESHIMPORTER_H_
#define _MESHIMPORTER_H_
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Globals.h"

class aiScene;
class aiMesh;
class Mesh;

using namespace std;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool CleanUp();

	bool Import(const string& full_path);
	void LoadMesh(const aiScene* _scene, const aiNode* _node, const char* _full_path);

};

#endif // !_MESHIMPORTER_H_
	
