/*****************************************************

	audio.h
		Audio Support

*****************************************************/

#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <vector>
#include <string>

#include <types/vector3d.h>
#include <mgr/file/file.h>

#ifdef MERCURY_OPENAL
#include <al/al.h>
#include <al/alc.h>
#endif

#include <mgr/thread/thread.h>

namespace mercury {

typedef enum {
	SOUND_CODEC_OGG,
	SOUND_CODEC_WAV
} SOUND_CODEC;

class sound;

class soundThread : public thread {
	sound* pkCurrent;
	bool running;
	void update ();
public:
	soundThread (sound*);
	virtual ~soundThread ();
	inline bool isRunning () {return running;}
	int run ();
};

// A sound
class sound {
public:

	sound ();
	virtual ~sound ();

	vector3d position;
	vector3d velocity;

	unsigned int buffers[2]; // front and back buffers
	unsigned int source;     // audio source
	unsigned int format;     // internal format

	file *pkFile;
	void *codecData;
	SOUND_CODEC sndCodec;
	soundThread* pkThread;
	bool loopFlag;
	float volume;
};

// Class for sound codecs
class soundCodec {
private:

public:
	virtual sound *load(file *pkFile) = 0;
	virtual bool free(sound *snd) = 0;
	virtual bool playback(sound *snd) = 0;
	virtual bool playing(sound *snd) = 0;
	virtual bool stream(sound *snd, unsigned int buffer) = 0;
	virtual bool update(sound *snd) = 0;
	virtual bool rewind (sound* snd) = 0;
};

// Base sound manager supports no audio
// This is a fall back in case no support
class soundManager {
public:
	virtual bool  setPosition (vector3d pos) {
		return false;
	}
	virtual bool  setVelocity (vector3d vel) {
		return false;
	}
	virtual bool  setOrientation (vector3d orAt, vector3d orUp) {
		return false;
	}
	virtual bool  setSoundVolume (float vol) {
		return false;
	}
	virtual bool  setSoundVolume (sound* snd, float vol) {
		return false;
	}
	virtual float getSoundVolume (void) {
		return 0.0f;
	}
	virtual float getSoundVolume (sound* snd) {
		return 0.0f;
	}
	virtual bool playbackSound (sound *snd) {
		return false;
	}
	virtual bool soundPlaying (sound *snd){
		return false;
	}
	virtual bool updateSound (sound *snd) {
		return false;
	}
	virtual sound *loadSound (std::string fileName, bool loop=false) {
		return 0;
	}
};

#ifdef MERCURY_OPENAL

class soundManagerOAL : public soundManager {
private:
	char *deviceDefault;
	char *deviceList;
	char *deviceCurrent;

	ALCdevice *device;
	ALCcontext *context;

	vector3d listenerPosition;
	vector3d listenerVelocity;
	vector3d listenerOrientationAt;
	vector3d listenerOrientationUp;

public:
	soundManager();
	~soundManager();

	// Set/Get functions
	bool setPosition(vector3d pos)
	{
		listenerPosition = pos;
		alListener3f(AL_POSITION,listenerPosition.x,listenerPosition.y,listenerPosition.z);
		if (alGetError()==AL_NO_ERROR)
			return true;
		return false;
	}

	bool setVelocity(vector3d vel)
	{
		listenerVelocity = vel;
		alListener3f(AL_VELOCITY,listenerVelocity.x,listenerVelocity.y,listenerVelocity.z);
		if (alGetError()==AL_NO_ERROR)
			return true;
		return false;
	}

	bool setOrientation(vector3d orAt, vector3d orUp)
	{
		listenerOrientationAt = orAt;
		listenerOrientationUp = orUp;

		float orientation[6] = {listenerOrientationAt.x,listenerOrientationAt.y,listenerOrientationAt.z,listenerOrientationUp.x,listenerOrientationUp.y,listenerOrientationUp.z};
		alListenerfv(AL_ORIENTATION,orientation);
		if (alGetError()==AL_NO_ERROR)
			return true;
		return false;
	}

	bool setSoundVolume(float vol) {
		alListenerf(AL_GAIN, vol);
		if (alGetError()==AL_NO_ERROR)
			return true;
		return false;
	}

	float getSoundVolume(void) {
		float val = 0.0f;
		alGetListenerf(AL_GAIN,&val);
		return val;
	}

	bool setSoundVolume (sound* snd, float vol);

	inline float getSoundVolume (sound* snd) {
		return snd->volume;
	}

	// Audio Functionality
	sound *loadSound(std::string fileName, bool loop=false); //Mike-added looping flag
	bool playbackSound(sound *snd);
	bool soundPlaying(sound *snd);
	bool updateSound(sound *snd);
};

#endif

}

#endif


