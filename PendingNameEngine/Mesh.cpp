#include "Mesh.h"
#include "Application.h"

#include "OpenGL.h"


Mesh::Mesh()
{
	Reset();
}

Mesh::~Mesh()
{
	
}


void Mesh::LoadVertices()
{
	if (num_vertices != 0)
	{
		glGenBuffers(1, &vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Mesh::LoadIndices()
{
	if (num_indices != 0)
	{
		glGenBuffers(1, &indices_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Mesh::LoadNormals()
{
	if (num_normals != 0)
	{
		glGenBuffers(1, &normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_normals*3, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Mesh::LoadTexcoords()
{
	if (texcoords != 0)
	{
		glGenBuffers(1, &texcoords_id);
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_texcoords * 3, texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Mesh::Reset()
{
	vertices_id = indices_id = texcoords_id = normals_id = 0;
	num_vertices = num_indices = num_texcoords = num_normals = 0;
	vertices = nullptr;
	indices = nullptr;
	normals = nullptr;
	texcoords = nullptr;
}

/*void Mesh::DrawFacesNormals()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	for (uint i = 0; i < facesnormals.size() - 1; i++)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv((GLfloat*)&float3(facesnormals[i].b.x, facesnormals[i].b.y, facesnormals[i].b.z));
		glVertex3fv((GLfloat*)&float3(facesnormals[i].a.x, facesnormals[i].a.y, facesnormals[i].a.z));
	}
	glEnd();
}*/
