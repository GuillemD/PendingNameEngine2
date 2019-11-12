#ifndef _MESHIMPORTER_H_
#define _MESHIMPORTER_H_

#include "Globals.h"
#include "Mesh.h"
#include <string>

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

	void SaveInOwnFileFormat(Mesh* mesh, std::string name);
	void LoadOwnFileFormat(const char* path);
	LineSegment GetTriNormal(float3 p1, float3 p2, float3 p3);


};

#endif // !_MESHIMPORTER_H_
	
