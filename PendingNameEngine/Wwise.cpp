#include "Wwise.h"
#include <assert.h>
#include "Globals.h"


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

	LoadBank("Library/Audio/SoundBanks/Init.bnk");

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

Wwise::WwiseGO * Wwise::CreateObject(ulong _id, const char * _name, float3 _pos, bool listener)
{
	WwiseGO* object = new WwiseGO(_id, _name);

	if (listener)
	{
		AkGameObjectID listener_id = object->getID();
		AK::SoundEngine::SetDefaultListeners(&listener_id, 1);
	}

	object->SetPos(_pos, { 1,0,0 }, { 0,1,0 });
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

void Wwise::WwiseGO::SetPos(float3 _pos, float3 _front, float3 _top)
{
	position.X = -_pos.x; position.Y = _pos.y; position.Z = -_pos.z;

	front.X = _front.x; front.Y = _front.y; front.Z = _front.z;

	top.X = _top.x; top.Y = _top.y; top.Z = _top.z;

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
