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

		void SetPos(float3 _pos, float3 _front, float3 _top);
		float3 GetPos() const;

		void ev_Play(ulong id);
		void ev_Pause(ulong id);
		void ev_Resume(ulong id);

	private:
		AkGameObjectID ID;
		const char* name;

		AkVector position = { 0,0,0 };
		AkVector front = { 1,0,0 };
		AkVector top = { 0,1,0 };
	public:
		bool playing = false;
		bool paused = false;
	};

	bool Init();
	void ProcessAudio();
	void TermSoundEngine();

	AkBankID LoadBank(const char* path);
	WwiseGO* CreateObject(ulong _id, const char* _name, float3 _pos, bool listener);
}

#endif // !__WISE__



