#include "ComponentAudioSource.h"


#include "ComponentTransform.h"


ComponentAudioSource::ComponentAudioSource(GameObject* p)
{
	SetOwner(p);
	type = CMP_A_SOURCE;

	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
	float3 pos = trans->GetLocalPosition();
	sound_object = Wwise::CreateObject(owner->UniqueId, owner->go_name.c_str(), pos, false);
}


ComponentAudioSource::~ComponentAudioSource()
{
}

bool ComponentAudioSource::Update()
{
	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
	float3 pos = trans->GetLocalPosition();
	sound_object->SetPos(pos, { 1,0,0 }, { 0,1,0 });
	return true;
}

Wwise::WwiseGO * ComponentAudioSource::GetSoundObject() const
{
	return sound_object;
}

AkUniqueID ComponentAudioSource::GetSoundId() const
{
	return object_id;
}

void ComponentAudioSource::SetSoundId(AkUniqueID id)
{
	object_id = id;
}
