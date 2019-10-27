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
	//ComponentMaterial* c_material = (ComponentMaterial*)primitive->AddComponent(CMP_MATERIAL);

	Mesh* mesh = new Mesh();
	mesh->num_vertices = par->npoints;
	mesh->vertices = new float3[mesh->num_vertices];
	/*for (uint i = 0; i < mesh->num_vertices; i+=3)
	{
		mesh->vertices[i].x = par->points[i];
		mesh->vertices[i].y = par->points[i+1];
		mesh->vertices[i].z = par->points[i+2];
	}*/
	memcpy(mesh->vertices, par->points, sizeof(float3)*mesh->num_vertices);
	mesh->num_indices = mesh->num_vertices * 3;
	mesh->indices = new int[mesh->num_indices];
	for (uint i = 0; i < mesh->num_indices; i++)
	{
		mesh->indices[i] = (uint)par->triangles[i];
	}
	//memcpy(mesh->indices, par->triangles, sizeof(int)*mesh->num_indices);


	mesh->LoadVertices();
	mesh->LoadIndices();

	c_mesh->SetMesh(mesh);

	App->scene->AddGameObject(primitive);
}
