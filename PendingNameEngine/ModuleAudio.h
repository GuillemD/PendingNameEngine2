#ifndef _MODULE_AUDIO_H_
#define _MODULE_AUDIO_H_

#include "Module.h"

class ModuleAudio :
	public Module
{

public:

	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();

	bool Init();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
};


#endif // !_MODULE_AUDIO_H_

