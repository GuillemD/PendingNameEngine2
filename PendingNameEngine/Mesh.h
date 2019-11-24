#ifndef _MESH_H_
#define _MESH_H_

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>
#include "Assimp.h"



enum MESH_TYPE {
	CUBE_MESH = 0,
	SPHERE_MESH,
	PLANE_MESH,
	FBX_MESH
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVertices();
	void LoadIndices();
	void LoadNormals();
	void LoadTexcoords();

	void Reset();

	//void DrawFacesNormals();

public:

	MESH_TYPE type;
	AABB bb;

	float3* vertices = nullptr;
	uint num_vertices = 0;
	uint vertices_id = 0;

	int* indices = nullptr;
	uint num_indices = 0;
	uint indices_id = 0;

	float* texcoords = nullptr;
	uint num_texcoords = 0;
	uint texcoords_id = 0;

	float3* normals = nullptr;
	uint num_normals = 0;
	uint normals_id = 0;

	//bool drawnormals = false;

	//std::vector<LineSegment> facesnormals;

};
#endif // !_MESH_H_
