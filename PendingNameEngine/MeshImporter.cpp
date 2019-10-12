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
	if (scene != nullptr)
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
	bool correct_num_faces = false;

	if (_node->mTransformation[0] != nullptr)
	{
		
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotation;

		_node->mTransformation.Decompose(scaling, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		float3 scale(scaling.x, scaling.y, scaling.z);

		m->pos = pos;
		m->rot = rot;
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
					if (imp_mesh->mFaces[i].mNumIndices != 3)
					{
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[j*3], imp_mesh->mFaces[i].mIndices, sizeof(uint) * 3);
						correct_num_faces = true;
						
						
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
			m->LoadDataToVRAM();

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


