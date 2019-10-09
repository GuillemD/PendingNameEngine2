#include "ModuleImporter.h"
#include "Application.h"

ModuleImporter::ModuleImporter(bool start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}


bool ModuleImporter::CleanUp()
{
	return true;
}
