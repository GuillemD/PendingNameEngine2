#include "ComponentAudioListener.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "mmgr/mmgr.h"



ComponentAudioListener::ComponentAudioListener(GameObject * p)
{
	SetOwner(p);
	type = CMP_A_LISTENER;
	
	ComponentTransform* trans = (ComponentTransform*)p->GetComponent(CMP_TRANSFORM);
	float3 pos = trans->GetLocalPosition();
	sound_object = Wwise::CreateObject(owner->UniqueId, owner->go_name.c_str(), pos, true);
}


ComponentAudioListener::~ComponentAudioListener()
{
}

bool ComponentAudioListener::Update()
{
	float3 pos = App->renderer3D->active_cam->GetFrustum().pos;
	float3 front = App->renderer3D->active_cam->GetFrustum().front;
	float3 top = App->renderer3D->active_cam->GetFrustum().up;

	sound_object->SetPos(pos, front, top);
	return true;
}

Wwise::WwiseGO * ComponentAudioListener::GetSoundObject() const
{
	return sound_object;
}
