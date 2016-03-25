/*****************************************************

	audio.cpp
		Audio Support

*****************************************************/

#include <iostream>
#include <mgr/sound/sound.h>
#include <mgr/file/file.h>
#include <core/kernel.h>
#include <codec/ogg.h>

namespace mercury {

soundThread::soundThread (sound* pkSound)
	: pkCurrent (pkSound), running(false)
{
	if (pkSound)
		running=true;
}

soundThread::~soundThread () {

}

int soundThread::run () {

	//! update sound
	while (running)
		if (pkCurrent)
			update ();

	return 0;
}

void soundThread::update () {

	soundManager* pkSoundManager = kernel::get()->getSoundManager();

	//! play sound
	while(pkSoundManager->updateSound(pkCurrent))
		if(!pkSoundManager->playbackSound(pkCurrent))
			return;

	//! done running, thread can close
	running = false;
}

sound::sound ()
: pkFile(0), codecData(0), loopFlag(false), pkThread(0), sndCodec(SOUND_CODEC_OGG),
volume (1.0f) {

}

sound::~sound () {

	if (pkFile)
		delete pkFile;

	if (codecData)
		delete [] codecData;

	if (pkThread)
		delete pkThread;
}

#ifdef MERCURY_OPENAL

#include <al/al.h>
#include <al/alc.h>

#ifdef _WIN32
#pragma comment (lib, "openal32.lib")
#endif

#define OAL_NUM_BUFFERS 20

soundManagerOAL::soundManagerOAL()
{
	int error; 

	//! Default values
	deviceList = deviceDefault = deviceCurrent = NULL;
	listenerPosition = vector3d(0,0,0);
	listenerVelocity = vector3d(0,0,0);
	listenerOrientationAt = vector3d(0,0,0);
	listenerOrientationUp = vector3d(0,1,0);

	//! Enumerate devices
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) // Enumeration is present
	{
		deviceList = (char *)alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
		deviceDefault = (char *)alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
	}

	//! Open device
	device = alcOpenDevice(deviceCurrent);
	
	if(device == NULL)
	{
		std::cout << "Failed to open OAL Device. Exiting SoundManager.\n";
		return;
	}


	//! Open context on device
	context = alcCreateContext(device,NULL); 
	if((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "Failed to create OAL Context. Exiting SoundManager.\n";
		return;
	}

	alcMakeContextCurrent(context);
	if((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "Failed to make OAL Context Current. Exiting SoundManager.\n";
		return;
	}

	//! Clear error codes
	alGetError();

	//! Create buffers
	//alGenBuffers(OAL_NUM_BUFFERS, g_Buffers);
	if((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "Failed to generate OAL Buffers. Exiting SoundManager.\n";
		return;
	}

	//! Create listener
	alListener3f(AL_POSITION,listenerPosition.x,listenerPosition.y,listenerPosition.z);
	alListener3f(AL_VELOCITY,listenerVelocity.x,listenerVelocity.y,listenerVelocity.z);

	float orientation[6] = {listenerOrientationAt.x,listenerOrientationAt.y,listenerOrientationAt.z,listenerOrientationUp.x,listenerOrientationUp.y,listenerOrientationUp.z};
	alListenerfv(AL_ORIENTATION,orientation);
}

soundManagerOAL::~soundManagerOAL()
{
	context = alcGetCurrentContext();
	device = alcGetContextsDevice(context);

	//! Remove context and device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

}


sound *soundManagerOAL::loadSound(std::string fileName, bool loop)
{

	kernel* pkKernel = kernel::get();
	if (!pkKernel)
		return NULL;

	fileManager* pkFileMngr = pkKernel->getFileManager ();
	if (!pkFileMngr)
		return NULL;

	file* pkFile = pkFileMngr->open (fileName);
	if (!pkFile)
		return NULL;
	if(!pkFile->isOpened())
		return NULL;

	std::string ext = pkFile->getFileExtension ();

	if(ext=="ogg" || ext=="OGG")
	{
		soundCodecOGG ogg;
		sound* pkSound = ogg.load (pkFile);

		if (!pkSound) {
			pkFileMngr->close (pkFile);
			return NULL;
		}

		pkSound->sndCodec = SOUND_CODEC_OGG;
		pkSound->loopFlag = loop;

		//! added by Mike, create thread for sound
		pkSound->pkThread = new soundThread (pkSound);
		kernel::get()->getThreadManager()->createThread (pkSound->pkThread);

		return pkSound;
	}
	else
		std::cout << "\n\r*** warning: Unsupported sound " << pkFile->getFilename() << " type: '" << ext << "'";

	return NULL;
}

bool soundManagerOAL::playbackSound(sound *snd)
{
	if(!snd)
		return false;

	if(snd->sndCodec == SOUND_CODEC_OGG)
	{
		soundCodecOGG ogg;
		return ogg.playback(snd);
	}
	else
		return false;
}

bool soundManagerOAL::soundPlaying(sound *snd)
{
	if(!snd)
		return false;

	if(snd->sndCodec == SOUND_CODEC_OGG)
	{
		soundCodecOGG ogg;
		return ogg.playing(snd);
	}
	else
		return false;
}

bool soundManagerOAL::updateSound(sound *snd)
{
	if(!snd)
		return false;

	if(snd->sndCodec == SOUND_CODEC_OGG)
	{
		soundCodecOGG ogg;

		//! modified by Mike, soundPlaying() doesnt seem to
		//! work to well (not sure why) so using a workaround:

		bool res=ogg.update (snd);
		if (!res)
			if (snd->loopFlag) {
				ogg.rewind (snd);
				return true;
			}

		return res;
	}
	else
		return false;
}

bool soundManagerOAL::setSoundVolume (sound* snd, float vol) {

	if (!snd)
		return false;
	alSourcef (snd->source, AL_GAIN, vol);
	if (alGetError()==AL_NO_ERROR) {
		snd->volume=vol;
		return true;
	}
	return false;
}

#endif

}
