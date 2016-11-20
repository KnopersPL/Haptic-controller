#include "haptic.h"
bool Haptic::Init()
{
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
	{
		#ifdef DEBUG
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		#endif // DEBUG

		return false;
	}
	return true;
}

int Haptic::GetDevNum()
{
	return SDL_NumJoysticks();
}

Haptic::HapticDev* Haptic::HapticDev::initDev(int id)
{
	if (id + 1 > GetDevNum())
		return NULL;
	HapticDev* dev = new HapticDev();
	dev->gGameController = SDL_JoystickOpen(id);
	if (dev->gGameController)
	{
		dev->gControllerHaptic = SDL_HapticOpenFromJoystick(dev->gGameController);
		if (dev->gControllerHaptic == NULL)
		{
			#ifdef DEBUG
			printf("Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
			#endif // DEBUG
			delete dev;
			return NULL;
		}
		if (SDL_HapticRumbleInit(dev->gControllerHaptic) < 0)
		{
			#ifdef DEBUG
			printf("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
			#endif // DEBUG
			delete dev;
			return NULL;
		}
		return dev;
	}
	delete dev;
	return NULL;
}

Haptic::HapticDev::HapticDev()
{
	this->gEffect = new SDL_HapticEffect();
	memset(this->gEffect, 0, sizeof(SDL_HapticEffect));
	
	this->gEffect->type = SDL_HAPTIC_CONSTANT;
	this->gEffect->constant.direction.type = SDL_HAPTIC_CARTESIAN;
}
void Haptic::HapticDev::haptic(short motor, short level, int duration)
{
	if (level > 100) level = 100;
	else if (level < 0) level = 0;

	if (duration < 0) duration = 0;

	this->gEffect->constant.direction.dir[0] = (motor & Haptic::LMOTOR); //Right
	this->gEffect->constant.direction.dir[1] = (motor & Haptic::HMOTOR); //Left
	this->gEffect->constant.length = duration;
	this->gEffect->constant.level = level * 0x7FFF / 100;

	int id = SDL_HapticNewEffect(this->gControllerHaptic, this->gEffect);
	if (id == -1)
	{
		#ifdef DEBUG
		printf("%s\n", SDL_GetError());
		#endif // DEBUG
		return;
	}
	SDL_HapticRunEffect(this->gControllerHaptic, id, 1);
	SDL_Delay(duration);
	SDL_HapticStopAll(this->gControllerHaptic);
	SDL_HapticDestroyEffect(this->gControllerHaptic, id);
}
const char* Haptic::HapticDev::getDevName()
{
	return SDL_JoystickName(gGameController);
}

Haptic::HapticDev::~HapticDev()
{
	SDL_HapticClose(this->gControllerHaptic);
	SDL_JoystickClose(this->gGameController);
	delete gEffect;
}

const char* Haptic::HapticDev::getSupported()
{
	unsigned int supported;
	char* buffer = (char*)malloc(256 * sizeof(char));
	char* bufptr = buffer;
	supported = SDL_HapticQuery(this->gControllerHaptic);
	
	 bufptr += sprintf(bufptr, "Supported effects [%d effects, %d playing]:\n",
		SDL_HapticNumEffects(this->gControllerHaptic), SDL_HapticNumEffectsPlaying(this->gControllerHaptic));
	
	if (supported & SDL_HAPTIC_CONSTANT)
		bufptr += sprintf(bufptr, "- constant\n");
	if (supported & SDL_HAPTIC_SINE)
		bufptr += sprintf(bufptr, "- sine\n");
	if (supported & SDL_HAPTIC_TRIANGLE)
		bufptr += sprintf(bufptr, "- triangle\n");
	if (supported & SDL_HAPTIC_SAWTOOTHUP)
		bufptr += sprintf(bufptr, "- sawtoothup\n");
	if (supported & SDL_HAPTIC_SAWTOOTHDOWN)
		bufptr += sprintf(bufptr, "- sawtoothdown\n");
	if (supported & SDL_HAPTIC_RAMP)
		bufptr += sprintf(bufptr, "- ramp\n");
	if (supported & SDL_HAPTIC_FRICTION)
		bufptr += sprintf(bufptr, "- friction\n");
	if (supported & SDL_HAPTIC_SPRING)
		bufptr += sprintf(bufptr, "- spring\n");
	if (supported & SDL_HAPTIC_DAMPER)
		bufptr += sprintf(bufptr, "- damper\n");
	if (supported & SDL_HAPTIC_INERTIA)
		bufptr += sprintf(bufptr, "- intertia\n");
	if (supported & SDL_HAPTIC_CUSTOM)
		bufptr += sprintf(bufptr, "- custom\n");
	bufptr += sprintf(bufptr, "Supported capabilities:\n");
	if (supported & SDL_HAPTIC_GAIN)
		bufptr += sprintf(bufptr, "- gain\n");
	if (supported & SDL_HAPTIC_AUTOCENTER)
		bufptr += sprintf(bufptr, "- autocenter\n");
	if (supported & SDL_HAPTIC_STATUS)
		bufptr += sprintf(bufptr, "- status\n");
	
	return buffer;
}