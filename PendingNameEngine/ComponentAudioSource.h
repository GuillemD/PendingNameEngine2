#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include "Component.h"
#include "Wwise.h"

#include "WWISE/AK/Wwise_IDs.h"

class WwiseGO;
class ComponentAudioSource :
	public Component
{
public:
	ComponentAudioSource(GameObject* p);
	~ComponentAudioSource();

	bool Update();

	Wwise::WwiseGO* GetSoundObject() const;

	AkUniqueID GetSoundId() const;
	void SetSoundId(AkUniqueID id);

private:
	Wwise::WwiseGO* sound_object = nullptr;
	AkUniqueID object_id = 0;
};


#endif // !_AUDIO_SOURCE_H_


