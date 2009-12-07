#ifdef SOUND_OPENAL

#include "main.h"
#include "util.h"
#include "sound.h"
#include <al.h>
#include <alc.h>
#include <efx.h>
#include <efx-creative.h>

// globals
BOOL Sound3D = TRUE;
ALCdevice* Device = NULL;
ALCcontext* Context = NULL;

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

void sound_buffer_play( void * buffer )
{
}

void sound_buffer_play_looping( void * buffer )
{
}

void sound_buffer_stop( void * buffer )
{
}

DWORD sound_buffer_size( void * buffer )
{
	return 0;
}

void sound_buffer_release( void * ptr )
{
}

void sound_buffer_3d_release( void * buffer )
{
}

BOOL sound_buffer_is_playing( void * buffer )
{
	return FALSE;
}

void sound_buffer_set_freq( void* ptr, float freq )
{
}

void sound_buffer_volume( void * buffer, long volume )
{
}

void sound_buffer_pan( void * buffer, long pan )
{
}

DWORD sound_buffer_get_freq( void * buffer ) // samples per sec
{
	return 0;
}

DWORD sound_buffer_get_rate( void * buffer ) // avg bytes per second
{
	return 0;
}

// this gets the current play location
void sound_buffer_get_position( void * buffer, DWORD* time )
{
}

// this moves to a specific offset in the buffer
void sound_buffer_set_position( void * buffer, DWORD time )
{
}

// this sets the location in 3d space of the sound
void sound_buffer_set_3d_position( void * buffer, float x, float y, float z, float min, float max )
{			
}

void* sound_buffer_load(char *name)
{
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 *wav_buffer;

	ALuint buffer_id;
	ALuint * source_id = malloc(sizeof(ALuint));
	ALenum error;
	ALenum format;

	// Generate Buffers
	alGetError(); // clear error code
	alGenBuffers(1, &buffer_id);

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
	alBufferData(buffer_id,format,&wav_buffer,wav_spec.size,wav_spec.freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alBufferData buffer 0 : %d\n", error);
		alDeleteBuffers(1, &buffer_id);
		return NULL;
	}

	// data has been coppied
	SDL_FreeWAV(wav_buffer);

	// Generate Sources
	alGenSources(1,source_id);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenSources 1 : %d\n", error);
		return NULL;
	}

	// Attach buffer 0 to source
	alSourcei(*source_id, AL_BUFFER, buffer_id);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alSourcei AL_BUFFER 0 : %d\n", error);
		return NULL;
	}

	return source_id;
}

//
// Sources
//

void sound_source_destroy( sound_source_t * source )
{
}

sound_source_t *sound_source_create(char *path)
{
	return NULL;
}

#endif // SOUND_OPENAL
