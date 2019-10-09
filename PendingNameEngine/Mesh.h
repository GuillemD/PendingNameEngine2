#ifndef _MESH_H_
#define _MESH_H_

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Primitive.h"
#include <string>
//#include  "PAR/par_shapes.h"

#define DEFAULT_COLOR {1.0f,1.0f,1.0f}

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

	uint CreateBuffer();

	void DefineCube(float3 _pos, float size);
	//void DefinePARCube();
	void DefinePlane(float3 _pos);

	void LoadDataToVRAM();
	void Reset();

public:
	std::string mesh_name = "";

	//transform
	float3 pos = { 0,0,0 };
	Quat rot = Quat::identity;
	float3 scale = { 1,1,1 };

	MESH_TYPE type;
	float3 color = { 0,0,0 };

	float3* vertices = nullptr;
	uint num_vertices = 0;
	uint vertices_id = 0;

	uint* indices = nullptr;
	uint num_indices = 0;
	uint indices_id = 0;

	float* texcoords = nullptr;
	uint num_texcoords = 0;
	uint texcoords_id = 0;

	float3* normals = nullptr;
	uint num_normals = 0;
	uint normals_id = 0;

};
#endif // !_MESH_H_
