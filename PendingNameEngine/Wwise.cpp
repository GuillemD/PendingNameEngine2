#include "Wwise.h"
#include <assert.h>
#include "Globals.h"

#include "WWISE/AK/Wwise_IDs.h"
#include <AK/Plugin/AkRoomVerbFXFactory.h>

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

bool Wwise::Init()
{
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;
	if (AK::MemoryMgr::Init(&memSettings) != AK_Success) {
		assert(!"Could not create the memory manager");
		return false;
	}

	////

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!AK::StreamMgr::Create(stmSettings)) {
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success) {
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	////

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	platformInitSettings.hWnd = GetActiveWindow();
	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	////

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	////

#ifndef AK_OPTIMIZED

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}

#endif // AK_OPTIMIZED

	LoadBank("SoundBanks/Init.bnk");

	return true;
}

void Wwise::ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}

void Wwise::TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	//
	// Terminate Communication Services
	//
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	
	//
	// Terminate the music engine
	//
	AK::MusicEngine::Term();

	//
	// Terminate the sound engine
	//
	AK::SoundEngine::Term();

	//
	// Terminate the streaming device and streaming manager
	//
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	//Terminate the Memory Manager
	AK::MemoryMgr::Term();
}

AkBankID Wwise::LoadBank(const char * path)
{
	
	AkBankID ret;
	AKRESULT eResult = AK::SoundEngine::LoadBank(path, AK_DEFAULT_POOL_ID, ret);
	if (eResult == AK_WrongBankVersion) {
		assert(!"Wrongbankversion");
		return false;
	}
	else if (eResult != AK_Success) {
		assert(!"Could not load sound bank :(");
		return false;
	}
	
	return ret;
}

Wwise::WwiseGO * Wwise::CreateObject(ulong _id, const char * _name, float x, float y, float z, bool listener)
{
	WwiseGO* object = new WwiseGO(_id, _name);

	if (listener)
	{
		AkGameObjectID listener_id = object->getID();
		AK::SoundEngine::SetDefaultListeners(&listener_id, 1);
	}

	object->SetPos(x, y, z);
	return object;
}

Wwise::WwiseGO::WwiseGO(ulong _ID, const char * _name)
{
	ID = _ID;
	name = _name;
	AKRESULT eResult = AK::SoundEngine::RegisterGameObj(ID, name);
	if (eResult != AK_Success)
	{
		LOG("Failed to register GameObject to Wwise!");
	}


}

Wwise::WwiseGO::~WwiseGO()
{
	AKRESULT eResult = AK::SoundEngine::UnregisterGameObj(ID);
	if (eResult != AK_Success)
	{
		LOG("Failed to unregister GameObject from Wwise!");
	}
}

ulong Wwise::WwiseGO::getID() const
{
	return ID;
}

const char * Wwise::WwiseGO::getName() const
{
	return name;
}

void Wwise::WwiseGO::SetPos(float x , float y, float z, float frontx, float fronty, float frontz, float topx , float topy, float topz)
{
	position.X = -x; position.Y = y; position.Z = -z;

	front.X = frontx; front.Y = fronty; front.Z = frontz;

	top.X = topx; top.Y = topy; top.Z = topz;

	float length_f = sqrt(pow(front.X, 2) + pow(front.Y, 2) + pow(front.Z, 2));
	float length_t = sqrt(pow(top.X, 2) + pow(top.Y, 2) + pow(top.Z, 2));

	front.X = front.X / length_f;
	front.Y = front.Y / length_f;
	front.Z = front.Z / length_f;

	top.X = top.X / length_t;
	top.Y = top.Y / length_t;
	top.Z = top.Z / length_t;

	float dot = top.X*front.X + top.Y*front.Y + top.Z*front.Z;
	if (dot >= 0.0001)
		LOG("Warning -- Vectors are not orthogonal.");

	AkSoundPosition sound_pos;
	sound_pos.Set(position, front, top);
	AKRESULT result = AK::SoundEngine::SetPosition((AkGameObjectID)ID, sound_pos);
	if (result != AK_Success)
		LOG("ERROR -- Could not update position.");
}

float3 Wwise::WwiseGO::GetPos() const
{

	float3 ret = { position.X,position.Y, position.Z };
	return ret;
}

void Wwise::WwiseGO::ev_Play(ulong id)
{
	if (AK::SoundEngine::PostEvent(id, ID) == AK_INVALID_PLAYING_ID)
	{
		assert(!"ERROR with event: Play");
	}
}

void Wwise::WwiseGO::ev_Pause(ulong id)
{
	if (AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, ID) == AK_INVALID_PLAYING_ID)
	{
		assert(!"ERROR with event: Pause");
	}
}

void Wwise::WwiseGO::ev_Resume(ulong id)
{
	if (AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, ID) == AK_INVALID_PLAYING_ID)
	{
		assert(!"ERROR with event: Resume");
	}
}

void Wwise::WwiseGO::ev_Stop(ulong id)
{
	if (AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, ID) == AK_INVALID_PLAYING_ID)
	{
		assert(!"ERROR with event: Stop");
	}
}

void Wwise::WwiseGO::ReverbAuxSend(AkReal32 value, AkGameObjectID target)
{
	AkAuxSendValue reverb_zone[1];

	reverb_zone[0].listenerID = AK_INVALID_GAME_OBJECT;
	reverb_zone[0].auxBusID = AK::AUX_BUSSES::REVERB;
	reverb_zone[0].fControlValue = value;

	AKRESULT res = AK::SoundEngine::SetGameObjectAuxSendValues(target, reverb_zone, 1);
	if (res != AK_Success)
		LOG("ERROR with auxiliary sends.");
}

void Wwise::WwiseGO::NullAuxSend(AkGameObjectID target)
{	
	AKRESULT res = AK::SoundEngine::SetGameObjectAuxSendValues(target, NULL, 0);
	if (res != AK_Success)
		LOG("ERROR with auxiliary sends.");
}
