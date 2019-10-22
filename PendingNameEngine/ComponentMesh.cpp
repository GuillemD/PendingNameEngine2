#include "ComponentMesh.h"
#include "Application.h"
#include "GameObject.h"

#include "OpenGL.h"



ComponentMesh::ComponentMesh(GameObject* p)
{
	SetOwner(p);
	type = CMP_MESH;

	active = true;
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{

	if (mesh == nullptr)
	{
		return false;
	}

	//update bb when transform changes
	return true;
}

bool ComponentMesh::CleanUp()
{
	if (mesh != nullptr)
	{
		mesh->Reset();
		mesh = nullptr;
	}
	return true;
}

void ComponentMesh::Draw()
{
	if (mesh == nullptr)
		return;

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

void ComponentMesh::SetMesh(Mesh * m)
{
	mesh = m;
}
