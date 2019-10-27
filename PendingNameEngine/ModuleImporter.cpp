#include "ModuleImporter.h"
#include "Application.h"

#include "MeshImporter.h"
#include "TextureImporter.h"

#include "Component.h"
#include "ComponentMaterial.h"


ModuleImporter::ModuleImporter(bool start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	bool ret = true;

	mesh_import->Start();
	texture_import->Start();
	return ret;
}

update_status ModuleImporter::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}


bool ModuleImporter::CleanUp()
{
	mesh_import->CleanUp();
	texture_import->CleanUp();
	return true;
}

bool ModuleImporter::Import(string path)
{
	bool ret = true;
	

	string::size_type idx;
	
	idx = path.rfind('.');
	if (idx != string::npos)
	{
		string extension = path.substr(idx + 1);
		if (extension == "FBX" || extension == "fbx")
		{
			if (mesh_path != path)
			{
				App->scene->ClearScene();
				ret = mesh_import->ImportMesh(path.c_str());
				mesh_path = path;
				texture_path = "";
			}
			else
			{
				CONSOLELOG("Mesh already loaded");
			}
		}
		else if (extension == "PNG" || extension == "png" || extension == "DDS" || extension == "dds")
		{
			if (!App->scene->scene_gameobjects.empty())
			{
				if (texture_path != path)
				{
					texture_import->LoadTextureFromPath(path.c_str());
					texture_path = path;
				}

			}
		}
		else
		{
			CONSOLELOG("ERROR: File format not recognised");
		}
	}

	return ret;
}


	

int ModuleImporter::GetILVersion() const
{
	int v = texture_import->GetVersion();
	return v;
}


