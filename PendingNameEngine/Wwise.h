#ifndef __WISE__
#define __WISE__
#include "WwiseLibs.h"

namespace Wwise
{
	class WwiseGO
	{		
	public:
		WwiseGO(unsigned long _ID, const char* _name);
		~WwiseGO();


	private:
		AkGameObjectID ID;
		const char* name;
	};

	bool Init();
	void ProcessAudio();
	void TermSoundEngine();
	AkBankID LoadBank(const char* path);
}

#endif // !__WISE__



