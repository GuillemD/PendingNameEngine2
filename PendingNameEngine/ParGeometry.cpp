#include "ParGeometry.h"
#include "Application.h"
#include "OpenGL.h"
#include "Mesh.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "PAR/par_shapes.h"


ParGeometry::ParGeometry()
{
}


ParGeometry::~ParGeometry()
{
}

void ParGeometry::GeometryGenerator(par_shapes_mesh_s * par, float3 pos, char * name)
{
	GameObject* primitive = new GameObject();
	primitive->is_root = true;
	primitive->go_name = name;

	ComponentTransform* c_trans = (ComponentTransform*)primitive->GetComponent(CMP_TRANSFORM);
	c_trans->SetPosition(pos);

	ComponentMesh* c_mesh = (ComponentMesh*)primitive->AddComponent(CMP_MESH);

	Mesh* mesh = new Mesh();
	mesh->num_vertices = par->npoints;
	mesh->vertices = new float3[mesh->num_vertices];
	memcpy(mesh->vertices, par->points, sizeof(float3)*mesh->num_vertices);

	mesh->num_indices =par->ntriangles * 3;
	mesh->indices = new int[mesh->num_indices];
	for (uint i = 0; i < mesh->num_indices; i++)
	{
		mesh->indices[i] = (uint)par->triangles[i];
	}

	mesh->LoadVertices();
	mesh->LoadIndices();

	c_mesh->SetMesh(mesh);

	App->scene->AddGameObject(primitive);
}
