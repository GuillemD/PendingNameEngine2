#ifndef __WISE__
#define __WISE__
#include "WwiseLibs.h"

namespace Wwise
{
	/*class Wwise
	{		
	};*/

	bool Init();
	void ProcessAudio();
	void TermSoundEngine();
	AkBankID LoadBank(const char* path);
}

#endif // !__WISE__



