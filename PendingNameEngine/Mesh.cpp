#include "Mesh.h"
#include "Application.h"

#include "OpenGL.h"


Mesh::Mesh()
{
	color = DEFAULT_COLOR;
	Reset();
}

Mesh::~Mesh()
{
	
}

uint Mesh::CreateBuffer()
{
	uint buffer;
	glGenBuffers(1, (GLuint*)&buffer);
	return buffer;
}


void Mesh::DefineCube(float3 _pos, float size)
{
	pos = _pos;

	type = MESH_TYPE::CUBE_MESH;

	num_vertices = 8;
	vertices = new float3[num_vertices];
	{
		
		vertices[0].x = pos.x + 0.0f;
		vertices[0].y = pos.y + 0.0f;
		vertices[0].z = pos.z + 0.0f;
		
		vertices[1].x = pos.x + size;
		vertices[1].y = pos.y + 0.0f;
		vertices[1].z = pos.z + 0.0f;
		
		vertices[2].x = pos.x + 0.0f;
		vertices[2].y = pos.y + size;
		vertices[2].z = pos.z + 0.0f;
		
		vertices[3].x = pos.x + size;
		vertices[3].y = pos.y + size;
		vertices[3].z = pos.z + 0.0f;
		
		vertices[4].x = pos.x + 0.0f;
		vertices[4].y = pos.y + 0.0f;
		vertices[4].z = pos.z + size;
		
		vertices[5].x = pos.x + size;
		vertices[5].y = pos.y + 0.0f;
		vertices[5].z = pos.z + size;
		
		vertices[6].x = pos.x + 0.0f;
		vertices[6].y = pos.y + size;
		vertices[6].z = pos.z + size;
		
		vertices[7].x = pos.x + size;
		vertices[7].y = pos.y + size;
		vertices[7].z = pos.z + size;

	}

	num_indices = 36;
	indices = new uint[num_indices];
	//
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3,

	indices[6] = 1;
	indices[7] = 3;
	indices[8] = 5;

	indices[9] = 3;
	indices[10] = 5;
	indices[11] = 7;

	indices[12] = 1;
	indices[13] = 0;
	indices[14] = 4;

	indices[15] = 1;
	indices[16] = 4;
	indices[17] = 5;

	indices[18] = 7;
	indices[19] = 5;
	indices[20] = 4;

	indices[21] = 7;
	indices[22] = 4;
	indices[23] = 6;

	indices[24] = 3;
	indices[25] = 7;
	indices[26] = 2;

	indices[27] = 2;
	indices[28] = 7;
	indices[29] = 6;

	indices[30] = 6;
	indices[31] = 4;
	indices[32] = 0;

	indices[33] = 6;
	indices[34] = 0;
	indices[35] = 2;

}

//void Mesh::DefinePARCube()
//{
//	par_shapes_mesh* cube = par_shapes_create_cube();
//
//	type = MESH_TYPE::CUBE_MESH;
//
//	num_vertices = cube->npoints;
//	vertices = new float3[num_vertices];
//	memcpy(vertices, cube->points, sizeof(float3)*num_vertices);
//
//	num_indices = cube->ntriangles;
//	indices = new uint[num_indices];
//	memcpy(indices, cube->triangles, sizeof(cube->triangles));
//	
//
//	if (cube->normals != nullptr)
//	{
//		num_normals = cube->npoints;
//		normals = new float3[num_normals];
//		memcpy(normals, cube->normals, sizeof(float3)*num_normals);
//	}
//	if (cube->tcoords != nullptr)
//	{
//		num_texcoords = cube->npoints;
//		texcoords = new float[num_texcoords * 3];
//		memcpy(texcoords, cube->tcoords, sizeof(float)*num_texcoords * 3);
//	}
//
//	par_shapes_free_mesh(cube);
//}

void Mesh::DefinePlane(float3 _pos)
{

	pos = _pos;

	type = MESH_TYPE::PLANE_MESH;

	num_vertices = 4;
	vertices = new float3[num_vertices];


	vertices[0].x = 0.0f;
	vertices[0].y = 0.0f;
	vertices[0].z = 0.0f;

	vertices[1].x = 1.0f;
	vertices[1].y = 0.0f;
	vertices[1].z = 0.0f;

	vertices[2].x = 0.0f;
	vertices[2].y = 0.0f;
	vertices[2].z = 1.0f;

	vertices[3].x = 1.0f;
	vertices[3].y = 0.0f;
	vertices[3].z = 1.0f;


	num_indices = 6;
	indices = new uint[num_indices];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	if (num_normals != 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, this->normals_id);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (num_texcoords != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, this->texcoords_id);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices_id);
	glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void Mesh::LoadVertices()
{
	if (num_vertices != 0)
	{
		glGenBuffers(1, (GLuint*)&vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Mesh::LoadIndices()
{
	if (num_indices != 0)
	{
		glGenBuffers(1, (GLuint*)&indices_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Mesh::LoadNormals()
{
	/*if (num_normals != 0)
	{
		glGenBuffers(1, (GLuint*)&normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_normals*3, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}*/
}

void Mesh::LoadTexcoords()
{
	/*if (texcoords != 0)
	{
		glGenBuffers(1, (GLuint*)&texcoords_id);
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texcoords_id * 3, texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}*/
}

void Mesh::Reset()
{
	vertices_id = indices_id = texcoords_id = normals_id = 0;
	num_vertices = num_indices = num_texcoords = num_normals = 0;
}
