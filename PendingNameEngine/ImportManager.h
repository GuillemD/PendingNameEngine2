#ifndef _IMPORT_MANAGER_H_
#define _IMPORT_MANAGER_H_

#include "Module.h"

class ImportManager : public Module
{
public:
	ImportManager(bool start_enabled = true);
	~ImportManager();

	void Import();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

};
#endif // !_IMPORT_MANAGER_H_
