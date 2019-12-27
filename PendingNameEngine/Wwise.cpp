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

Wwise::WwiseGO::WwiseGO(unsigned long _ID, const char * _name)
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
