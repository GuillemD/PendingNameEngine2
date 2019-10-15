#include "MeshImporter.h"
#include "Application.h"
#include "OpenGL.h"
#include <string>
#include "Assimp.h"

void AssimpToConsoleLog(const char* str, char* userData);

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Start()
{
	bool ret = true;

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpToConsoleLog;
	aiAttachLogStream(&stream);

	return ret;
}

bool MeshImporter::CleanUp()
{
	bool ret = true;

	aiDetachAllLogStreams();

	return ret;
}

bool MeshImporter::ImportMesh(const char* full_path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* root = scene->mRootNode;
		LoadMesh(scene, root, full_path);

		aiReleaseImport(scene);
	}
	else
	{
		CONSOLELOG("Error loading scene %s", full_path);
		ret = false;
	}


	return ret;
}

void MeshImporter::LoadMesh(const aiScene * _scene, const aiNode * _node, const char * _full_path)
{
	Mesh* m = new Mesh();
	bool correct_num_faces = true;

	if (_node->mTransformation[0] != nullptr)
	{
		
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotation;

		_node->mTransformation.Decompose(scaling, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);

		aiVector3D euler_rotation = rotation.GetEuler();
		euler_rotation *= RADTODEG;

		m->pos = pos;
		m->euler_rot.x = euler_rotation.x;
		m->euler_rot.y = euler_rotation.y;
		m->euler_rot.z = euler_rotation.z;
		m->scale = scale;

	}
	if (_node->mNumMeshes > 0)
	{
		for (int i = 0; i < _node->mNumMeshes; i++)
		{
			aiMesh* imp_mesh = _scene->mMeshes[_node->mMeshes[i]];
			string name = _node->mName.C_Str();
			m->mesh_name = name;

			if (imp_mesh->HasPositions())
			{
				//vertices
				m->num_vertices = imp_mesh->mNumVertices;
				m->vertices = new float3[m->num_vertices];
				memcpy(m->vertices, imp_mesh->mVertices, sizeof(float3)*m->num_vertices);

				CONSOLELOG("Mesh %s with %d vertex loaded", m->mesh_name.c_str(), m->num_vertices);
				
			}

			if (imp_mesh->HasFaces())
			{
				//indices
				m->num_indices = imp_mesh->mNumFaces * 3; //num triangles * 3
				m->indices = new uint[m->num_indices];

				for (int j = 0; j < imp_mesh->mNumFaces; j++)
				{
					aiFace triangle = imp_mesh->mFaces[j];
					if (triangle.mNumIndices != 3)
					{
						CONSOLELOG("WARNING, geometry face %d with != 3 indices!", j);
						correct_num_faces = false;
					}
					else
					{
						memcpy(&m->indices[j * 3], triangle.mIndices, sizeof(uint) * 3);
					}
				}
				CONSOLELOG("Mesh %s with %d indices loaded", m->mesh_name.c_str(), m->num_indices);
				
			}
			
			if (imp_mesh->HasNormals())
			{
				m->num_normals = m->num_vertices;
				m->normals = new float3[m->num_normals];
				memcpy(m->normals, &imp_mesh->mNormals[0], sizeof(float3)*m->num_normals);
				
				CONSOLELOG("Mesh %s with %d normals loaded", m->mesh_name.c_str(), m->num_normals);
				
			}
			
			if (imp_mesh->HasTextureCoords(0))
			{
				m->num_texcoords = imp_mesh->mNumVertices;
				m->texcoords = new float[m->num_texcoords * 3];
				memcpy(m->texcoords, imp_mesh->mTextureCoords[0], sizeof(float)*m->num_texcoords * 3);

				CONSOLELOG("Mesh %s with %d texcoords loaded", m->mesh_name.c_str(), m->num_texcoords);
				
			}

			glGenBuffers(1, (GLuint*)&m->vertices_id);
			glBindBuffer(GL_ARRAY_BUFFER, m->vertices_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m->num_vertices * 3, m->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&m->indices_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->indices_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m->num_indices, m->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (m->num_normals != 0)
			{
				glGenBuffers(1, (GLuint*)&m->normals_id);
				glBindBuffer(GL_ARRAY_BUFFER, m->normals_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m->num_normals * 3, m->normals, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			if (m->texcoords != 0)
			{
				glGenBuffers(1, (GLuint*)&m->texcoords_id);
				glBindBuffer(GL_ARRAY_BUFFER, m->texcoords_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m->texcoords_id * 3, m->texcoords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			m->bb.SetNegativeInfinity();
			m->bb.Enclose((float3*)imp_mesh->mVertices, imp_mesh->mNumVertices);

			App->camera->can_focus = true;
			App->camera->Focus(m->bb);
			App->camera->can_focus = false;

			if(imp_mesh->HasPositions() && correct_num_faces)
				App->scene->scene_meshes.push_back(m);
			
		}
	}
	if (_node->mNumChildren > 0)
	{
		for (int k = 0; k < _node->mNumChildren; k++)
		{
			LoadMesh(_scene, _node->mChildren[k], _full_path);
		}
	}
}


void AssimpToConsoleLog(const char * str, char * userData)
{
	CONSOLELOG("%s", str);
}


