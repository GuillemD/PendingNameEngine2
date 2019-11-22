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
					if (TextureExistsInLibrary(path)) {
						
						CONSOLELOG("Texture exists in library, loading it from there. ");
						texture_import->LoadTextureFromPath(GetLibraryTexturePath(path).c_str());
					}
					else {
						texture_import->LoadTextureFromPath(path.c_str());
					}
					
					
					texture_path = path;
				}

			}
			else {
				texture_import->AddTextureToLibrary(path);
			}
		}
		else
		{
			CONSOLELOG("ERROR: File format not recognised");
		}
	}

	return ret;
}

bool ModuleImporter::TextureExistsInLibrary(string path)
{
	
	string thename;
	string tmp;

	thename = path;

	uint last_bar = (thename.find(".//Assets//") != std::string::npos) ? thename.find_last_of('/') : thename.find_last_of('/');
	uint ext_point = thename.find_last_of('.');

	for (int i = last_bar + 1; i < ext_point; i++) {
		tmp.push_back(thename[i]);
	}

	if (App->fs->Exists((LIBRARY_TEXTURES_FOLDER + tmp + ".dds").c_str())) {
		return true;
	}

	thename = path;
	tmp.clear();

	uint lastbar = (thename.find("\\Assets\\") != std::string::npos) ? thename.find_last_of('\\') : thename.find_last_of('\\');
	uint extpoint = thename.find_last_of('.');

	for (int i = lastbar + 1; i < extpoint; i++) {
		tmp.push_back(thename[i]);
	}

	if (App->fs->Exists((LIBRARY_TEXTURES_FOLDER + tmp + ".dds").c_str())) {
		return true;
	}


	return false;
}

string  ModuleImporter::GetLibraryTexturePath(string path)
{
	string thename;
	string tmp;

	thename = path;

	uint last_bar = (thename.find(".//Assets//") != std::string::npos) ? thename.find_last_of('/') : thename.find_last_of('/');
	uint ext_point = thename.find_last_of('.');

	for (int i = last_bar + 1; i < ext_point; i++) {
		tmp.push_back(thename[i]);
	}

	if (App->fs->Exists((LIBRARY_TEXTURES_FOLDER + tmp + ".dds").c_str())) {
		return ("Library/Textures/" + tmp + ".dds");
	}

	thename = path;
	tmp.clear();

	uint lastbar = (thename.find("\\Assets\\") != std::string::npos) ? thename.find_last_of('\\') : thename.find_last_of('\\');
	uint extpoint = thename.find_last_of('.');

	for (int i = lastbar + 1; i < extpoint; i++) {
		tmp.push_back(thename[i]);
	}

	if (App->fs->Exists((LIBRARY_TEXTURES_FOLDER + tmp + ".dds").c_str())) {
		return ("Library/Textures/" + tmp + ".dds");
	}
}


	

int ModuleImporter::GetILVersion() const
{
	int v = texture_import->GetVersion();
	return v;
}


