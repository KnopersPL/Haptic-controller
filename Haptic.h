#ifndef MY_LIB_HAPTIC
#define MY_LIB_HAPTIC
#include <SDL.h>
#include <stdio.h>
#include <cstring>

#define DEBUG 0

namespace Haptic
{
	enum
	{
		HMOTOR = 1,
		LMOTOR = (1 << 1)
	};
	class HapticDev
	{
		private:
			SDL_Joystick* gGameController = NULL;
			SDL_Haptic* gControllerHaptic = NULL;
			SDL_HapticEffect* gEffect = NULL;
			int gEffectId = -1;
		
			HapticDev();
		public:
			static HapticDev* initDev(int id);
			const char* getDevName();
			const char* getSupported();
			void haptic(short motor, short level, int duration);
			~HapticDev();
	};

	bool Init();
	int GetDevNum();
}
#endif