#ifndef __WISE__
#define __WISE__
#include "WwiseLibs.h"
#include "MathGeoLib/include/MathGeoLib.h"

typedef unsigned long ulong;
namespace Wwise
{
	class WwiseGO
	{		
	public:
		WwiseGO(ulong _ID, const char* _name);
		~WwiseGO();
		ulong getID() const;
		const char* getName() const;

		void SetPos(float x = 0, float y = 0, float z = 0, float frontx = 1, float fronty = 0, float frontz = 0, float topx = 0, float topy = 1, float topz = 0);
		float3 GetPos() const;

		void ev_Play(ulong id);
		void ev_Pause(ulong id);
		void ev_Resume(ulong id);
		void ev_Stop(ulong id);
		void ReverbAuxSend(AkReal32 value, AkGameObjectID target);
		void NullAuxSend(AkGameObjectID target);

	private:
		AkGameObjectID ID;
		const char* name;

		AkVector position = { 0,0,0 };
		AkVector front = { 0,0,0 };
		AkVector top = { 0,0,0 };
	public:
		bool playing = false;
		bool paused = false;
	};

	bool Init();
	void ProcessAudio();
	void TermSoundEngine();

	AkBankID LoadBank(const char* path);
	WwiseGO* CreateObject(ulong _id, const char* _name, float x, float y, float z, bool listener);
	
}

#endif // !__WISE__



