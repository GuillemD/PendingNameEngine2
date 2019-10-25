#include "MeshImporter.h"
#include "Application.h"

#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "TextureImporter.h"

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

		LoadMesh(scene, root, App->scene->root, full_path);

		aiReleaseImport(scene);
	}
	else
	{
		CONSOLELOG("Error loading scene %s", full_path);
		ret = false;
	}


	return ret;
}

void MeshImporter::LoadMesh(const aiScene * _scene, const aiNode * _node, GameObject* parent, const char * _full_path)
{
	GameObject* go = new GameObject();
	bool correct_num_faces = true;

	if (_node->mNumMeshes < 1)
	{
		go = new GameObject();
		string aux_name(_full_path);

		if (string(_node->mName.C_Str()) == string("RootNode"))
		{
			
			go->is_root = true;
			
			int pos = aux_name.find_last_of('//');
			int len = aux_name.length() - pos;
			aux_name = aux_name.substr(pos + 1, len);

			int pos2 = aux_name.find_last_of('.');
			aux_name = aux_name.substr(0, pos2);
			go->go_name = aux_name;
			
		}
		else
		{
			go->SetParent(App->scene->root);
			go->go_name = aux_name;
		}
		
		if(parent != nullptr)
			parent->SetChild(go);

		if (_node->mTransformation[0] != nullptr)
		{

			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;

			_node->mTransformation.Decompose(scaling, rotation, translation);

			float3 pos(translation.x, translation.y, translation.z);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			float3 scale(scaling.x, scaling.y, scaling.z);

			aiVector3D euler_rotation = rotation.GetEuler();
			euler_rotation *= RADTODEG;

			float3 euler_angles = { euler_rotation.x,euler_rotation.y, euler_rotation.z };


			go->trans->SetPosition(pos);
			go->trans->SetRotation(rot);
			go->trans->SetScale(scale);
			go->trans->SetEulerRotation(euler_angles);

		}
		App->scene->AddGameObject(go);
	}
	else if (_node->mNumMeshes > 0)
	{
		for (int i = 0; i < _node->mNumMeshes; i++)
		{
			GameObject* child = new GameObject();
			child->go_name = _node->mName.C_Str();

			aiMesh* imp_mesh = _scene->mMeshes[_node->mMeshes[i]];
			Mesh* mesh = nullptr;

			if (imp_mesh->HasPositions())
			{
				mesh = new Mesh();

				//vertices
				mesh->num_vertices = imp_mesh->mNumVertices;
				mesh->vertices = new float3[mesh->num_vertices];
				memcpy(mesh->vertices, imp_mesh->mVertices, sizeof(float3)*mesh->num_vertices);

				glGenBuffers(1, (GLuint*)&mesh->vertices_id);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				CONSOLELOG(" %d vertex loaded", mesh->num_vertices);

			}

			if (imp_mesh->HasFaces())
			{
				//indices
				mesh->num_indices = imp_mesh->mNumFaces * 3; //num triangles * 3
				mesh->indices = new uint[mesh->num_indices];

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
						memcpy(&mesh->indices[j * 3], triangle.mIndices, sizeof(uint) * 3);
					}
				}
				glGenBuffers(1, (GLuint*)&mesh->indices_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				CONSOLELOG("%d indices loaded", mesh->num_indices);

			}

			if (imp_mesh->HasNormals())
			{
				mesh->num_normals = mesh->num_vertices;
				mesh->normals = new float3[mesh->num_normals];
				memcpy(mesh->normals, &imp_mesh->mNormals[0], sizeof(float3)*mesh->num_normals);

				glGenBuffers(1, (GLuint*)&mesh->normals_id);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->normals_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				CONSOLELOG(" %d normals loaded", mesh->num_normals);
			}

			if (imp_mesh->HasTextureCoords(0))
			{
				mesh->num_texcoords = imp_mesh->mNumVertices;
				mesh->texcoords = new float[mesh->num_texcoords * 3];

				memcpy(mesh->texcoords, imp_mesh->mTextureCoords[0], sizeof(float)*mesh->num_texcoords * 3);

				glGenBuffers(1, (GLuint*)&mesh->texcoords_id);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->texcoords_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*mesh->texcoords_id * 3, mesh->texcoords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				CONSOLELOG(" %d texcoords loaded", mesh->num_texcoords);
			}

			/*if (_scene->HasMaterials())
			{
				aiMaterial* mat = nullptr;
				mat = _scene->mMaterials[imp_mesh->mMaterialIndex];

				aiString name;
				mat->GetTexture(aiTextureType_DIFFUSE, 0, &name);

				aiColor3D col;
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, col);

				if (string(name.C_Str()) != string(""))
				{
					Material* new_material = nullptr;
					
				}

			}*/
			ComponentMesh* m_cmp = (ComponentMesh*)child->AddComponent(CMP_MESH);
			m_cmp->SetMesh(mesh);
			m_cmp->CreateBB();
			m_cmp->draw_bb = false;

			parent->AddChild(child);

			if (_node->mTransformation[0] != nullptr)
			{

				aiVector3D translation;
				aiVector3D scaling;
				aiQuaternion rotation;

				_node->mTransformation.Decompose(scaling, rotation, translation);

				float3 pos(translation.x, translation.y, translation.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
				float3 scale(scaling.x, scaling.y, scaling.z);

				aiVector3D euler_rotation = rotation.GetEuler();
				euler_rotation *= RADTODEG;

				float3 euler_angles = { euler_rotation.x,euler_rotation.y, euler_rotation.z };


				child->trans->SetPosition(pos);
				child->trans->SetRotation(rot);
				child->trans->SetScale(scale);
				child->trans->SetEulerRotation(euler_angles);

			}
			
			App->scene->AddGameObject(child);

			App->camera->can_focus = true;
			App->camera->Focus(m_cmp->GetMesh()->bb);
			App->camera->can_focus = false;


		}
		go = parent;
	}

	if (_node->mNumChildren > 0)
	{
		for (int k = 0; k < _node->mNumChildren; k++)
		{
			LoadMesh(_scene, _node->mChildren[k], go, _full_path);
		}
	}

}


void AssimpToConsoleLog(const char * str, char * userData)
{
	CONSOLELOG("%s", str);
}


