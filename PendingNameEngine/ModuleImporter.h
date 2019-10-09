#ifndef _MODULEIMPORTER_H_
#define _MODULEIMPORTER_H_

#include "Module.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	 ~ModuleImporter();

	 bool Start();
	 update_status Update(float dt);
	 bool CleanUp();

};
#endif // !_MODULEIMPORTER_H_
