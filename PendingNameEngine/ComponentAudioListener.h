#ifndef _AUDIO_LISTENER_H_
#define _AUDIO_LISTENER_H_
#include "Component.h"
#include "Wwise.h"

class WwiseGO;

class ComponentAudioListener :
	public Component
{
public:

	ComponentAudioListener(GameObject* p);
	~ComponentAudioListener();

	bool Update();

	Wwise::WwiseGO* GetSoundObject() const;

private:

	Wwise::WwiseGO* sound_object = nullptr;
};


#endif // !_AUDIO_LISTENER_H_


