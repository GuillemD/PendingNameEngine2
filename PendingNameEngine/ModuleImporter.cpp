#include "ModuleImporter.h"
#include "Application.h"

#include "MeshImporter.h"


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

	return ret;
}

update_status ModuleImporter::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}


bool ModuleImporter::CleanUp()
{
	mesh_import->CleanUp();
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
			}
			else
			{
				CONSOLELOG("Mesh already loaded");
			}
		}
		else if (extension == "PNG" || extension == "png" || extension == "DDS" || extension == "dds")
		{
			if (!App->scene->scene_meshes.empty())
			{
				//check if texture already loaded
				//load new texture
			}
		}
		else
		{
			CONSOLELOG("ERROR: File format not recognised");
		}
	}

	return ret;
}


