#include "ImportManager.h"
#include "Application.h"

ImportManager::ImportManager(bool start_enabled)
{
	name = "Import Manager";
}

ImportManager::~ImportManager()
{
}

void ImportManager::Import()
{
}

bool ImportManager::Start()
{
	return true;
}

update_status ImportManager::Update(float dt)
{
	return update_status();
}

bool ImportManager::CleanUp()
{
	return true;
}
