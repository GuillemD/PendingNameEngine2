#include "ModuleAudio.h"
#include "Application.h"

#include "Wwise.h"

#include "mmgr/mmgr.h"

#define DEFAULT_VOLUME 50.0f

ModuleAudio::ModuleAudio(bool start_enabled)
{
	name = "Audio";
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	LOG("Initializing Audio");
	bool ret = true;

	volume = DEFAULT_VOLUME;

	ret = Wwise::Init();


	return ret;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	Wwise::ProcessAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	LOG("Clean Up audio");

	Wwise::TermSoundEngine();

	return true;
}

void ModuleAudio::SetVolume(const char * rtpc, float volume)
{
	
}

float ModuleAudio::GetVolume() const
{
	return volume;
}
