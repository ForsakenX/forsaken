#ifdef SOUND_OPENAL

#include "main.h"
#include "util.h"
#include "sound.h"
#include "SDL.h"
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
	Uint32 size;
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

void sound_destroy( void )
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
	alListener3f(AL_POSITION, x, y, z);
	return TRUE;
}

BOOL sound_listener_velocity( float x, float y, float z )
{
	alListener3f(AL_VELOCITY, x, y, z);
	return TRUE;
}

// TODO - does the at-vector equate to forward vector?
BOOL sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
)
{
	float vector[6] = {fx,fy,fz,ux,uy,uz};
	alListenerfv(AL_ORIENTATION, &vector[0]);
	return TRUE;
}

//
// Buffers
//

void sound_play( sound_t * source )
{
	alSourcePlay( source->source );
}

void sound_play_looping( sound_t * source )
{
	alSourcei( source->source, AL_LOOPING, AL_TRUE );
	sound_play( source );
}

void sound_stop( sound_t * source )
{
	alSourceStop( source->source );
}

long sound_size( sound_t * source )
{
	ALint size;
	alGetBufferi( source->buffer, AL_SIZE, &size );
	return (long) size;
}

long sound_rate( sound_t * source )
{
	ALint freq;
	alGetBufferi( source->buffer, AL_FREQUENCY, &freq );
	return (long) freq;
}

void sound_release( sound_t * source )
{
	alDeleteBuffers( 1, &source->buffer );
	alDeleteSources( 1, &source->source );
}

BOOL sound_is_playing( sound_t * source )
{
	ALint state;
	alGetSourcei( source->source, AL_SOURCE_STATE, &state	);
	return (state == AL_PLAYING);
}

void sound_get_seek( sound_t * source, long * bytes )
{
	alGetSourcei(source->source, AL_BYTE_OFFSET, bytes);
}

void sound_set_seek( sound_t * source, long bytes )
{
	alSourcei(source->source, AL_BYTE_OFFSET, bytes);
}

// TODO - min, position, velocity, relative ?
void sound_position( sound_t * source, float x, float y, float z, float min, float max )
{
	alSource3f(source->source, AL_POSITION, x, y, z);
	alSourcef(source->source, AL_MAX_DISTANCE, max);
}

sound_t * sound_load(char *name)
{
	ALenum error;
	ALenum format;
	SDL_AudioSpec wav_spec;
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

	if( SDL_LoadWAV(name, &wav_spec, &wav_buffer, &source->size) == NULL )
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

// i think we can ignore these
BOOL sound_duplicate( sound_t * source, sound_t ** destination ){return TRUE;}
void sound_set_freq( void* ptr, float freq ){}

// i believe this is only for 2d sounds
void sound_volume( sound_t * source, long volume ){}
void sound_pan( sound_t * source, long pan ){}

#endif // SOUND_OPENAL
