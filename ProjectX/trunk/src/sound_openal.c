#ifdef SOUND_OPENAL

#include "main.h"
#include "util.h"
#include "sound.h"
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <efx-creative.h>

//
// globals
//

// tells forsaken to use 3d functions
BOOL Sound3D = TRUE;

static ALCdevice* Device = NULL;
static ALCcontext* Context = NULL;

//
// buffer description
//

typedef struct sound_t {
	ALuint source;
	ALuint buffer;
};

//
// Generic Functions
//

BOOL sound_init( void )
{
	Device = alcOpenDevice(NULL); // preferred device
	if(!Device)
		return FALSE;

	Context = alcCreateContext(Device,NULL);
	if(!Context)
		return FALSE;

	alcMakeContextCurrent(Context);

	// Check for EAX 2.0 support
	DebugPrintf("openal: EAX2.0 support = %s\n",
		alIsExtensionPresent("EAX2.0")?"true":"false");

	return TRUE;
}

void sound_cleanup( void )
{
	ALCcontext * Context = alcGetCurrentContext();
	ALCdevice * Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

//
// Listener
//

//	 In dsound only valid when using 3d sound
//   which this game does not currently use
//   this is left here for openAL later

BOOL sound_listener_position( float x, float y, float z )
{
	return TRUE;
}

BOOL sound_listener_velocity( float x, float y, float z )
{
	return TRUE;
}

BOOL sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
)
{
	return TRUE;
}

//
// Buffers
//

void sound_destroy( void )
{
}

void sound_play( sound_t * source )
{
}

void sound_play_looping( sound_t * source )
{
}

void sound_stop( sound_t * source )
{
}

long sound_size( sound_t * source )
{
	return 0;
}

void sound_release( sound_t * source )
{
}

BOOL sound_is_playing( sound_t * source )
{
	return FALSE;
}

void sound_set_freq( void* ptr, float freq )
{
}

void sound_volume( sound_t * source, long volume )
{
}

void sound_pan( sound_t * source, long pan )
{
}

long sound_get_rate( sound_t * source ) // avg bytes per second
{
	return 0;
}

// this gets the current play location
void sound_get_seek( sound_t * source, long * time )
{
}

// this moves to a specific offset in the buffer
void sound_set_seek( sound_t * source, long time )
{
}

// this sets the location in 3d space of the sound
void sound_position( sound_t * source, float x, float y, float z, float min, float max )
{			
}

sound_t * sound_load(char *name)
{
	ALenum error;
	ALenum format;
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 *wav_buffer;
	sound_t * source = malloc(sizeof(sound_t));

	// Generate Buffers
	alGetError(); // clear error code
	alGenBuffers(1, &source->buffer);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenBuffers :%d\n", error);
		return NULL;
	}

	if( SDL_LoadWAV(name, &wav_spec, &wav_buffer, &wav_length) == NULL )
	{
		DebugPrintf("Could not open test.wav: %s\n", SDL_GetError());
		return NULL;
	}

	if(wav_spec.format == AUDIO_U8 || wav_spec.format == AUDIO_S8) // 8 bit
	{
		if(wav_spec.channels == 1)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_STEREO8;
	}
	else // 16 bit
	{
		if(wav_spec.channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
	}

	// Copy data into AL Buffer 0
	alBufferData(source->buffer,format,&wav_buffer,wav_spec.size,wav_spec.freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alBufferData buffer 0 : %d\n", error);
		alDeleteBuffers(1, &source->buffer);
		return NULL;
	}

	// data has been coppied
	SDL_FreeWAV(wav_buffer);

	// Generate Sources
	alGenSources(1,&source->source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenSources 1 : %d\n", error);
		return NULL;
	}

	// Attach buffer 0 to source
	alSourcei(source->source, AL_BUFFER, source->buffer);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alSourcei AL_BUFFER 0 : %d\n", error);
		return NULL;
	}

	return source;
}

BOOL sound_duplicate( sound_t * source, sound_t ** destination )
{
	return FALSE;
}

#endif // SOUND_OPENAL
