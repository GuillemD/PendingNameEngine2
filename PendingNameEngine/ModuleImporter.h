#ifndef _MODULEIMPORTER_H_
#define _MODULEIMPORTER_H_

#include "Module.h"


using namespace std;
class MeshImporter;
class TextureImporter;

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	 ~ModuleImporter();

	 bool Start();
	 update_status Update(float dt);
	 bool CleanUp();

	 bool Import(string path);

	 int GetILVersion() const;
public:
	MeshImporter* mesh_import;
	TextureImporter*  texture_import;
	string mesh_path = "";

};
#endif // !_MODULEIMPORTER_H_
