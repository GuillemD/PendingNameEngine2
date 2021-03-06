#ifndef  _WWISELIBS_
#define _WWISELIBS_


#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "WWISE/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h> 
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#ifdef _DEBUG
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/CommunicationCentral.lib")
#pragma comment( lib, "Wwise/ws2_32.lib")
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkRoomVerbFX.lib")
#include <AK/Comm/AkCommunication.h>
#else
#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkRoomVerbFX.lib")
#endif 

#pragma comment( lib, "Wwise/dinput8.lib")
#pragma comment( lib, "Wwise/dsound.lib")
#pragma comment( lib, "Wwise/dxguid.lib")

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);
	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}

#endif // ! _WWISELIBS_
