#ifdef SOUND_OPENAL

#include "main.h"
#include "util.h"
#include "sound.h"
#include "file.h"
#include "SDL.h"
#include <al.h>
#include <alc.h>
//#include <efx.h>
//#include <efx-creative.h>

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

static void print_info ( void ) 
{
	ALint version_major, version_minor;
	ALenum error;
	ALCdevice *device;

	DebugPrintf("openal: info start\n");

	// Check for EAX 2.0 support
	DebugPrintf("EAX2.0 support = %s\n",
		alIsExtensionPresent("EAX2.0")?"true":"false");

	if(alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) 
	{
		DebugPrintf("default playback: %s\n",alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
		DebugPrintf("default capture: %s\n",alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
		{	
			// print all playback devices
			const char * s = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
			do { 
				DebugPrintf("playback: %s\n", s); 
				while(*s++ != '\0'); 
			} while(*s != '\0');
			// print all capture devices
			s = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
			do { 
				DebugPrintf("capture: %s\n", s); 
				while(*s++ != '\0'); 
			} while(*s != '\0');
		}
	} 
	else 
		DebugPrintf("No device enumeration available\n");

	DebugPrintf("Default device: %s\n", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
	DebugPrintf("Default capture device: %s\n", (alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER)));

	device = alcGetContextsDevice(alcGetCurrentContext());

	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("blah :%s\n", alGetString(error));

	alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &version_major);
	alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &version_minor);
	
	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("blah :%s\n", alGetString(error));

	DebugPrintf("ALC version: %d.%d\n", (int)version_major, (int)version_minor);
	DebugPrintf("ALC extensions: %s\n", alcGetString(device, ALC_EXTENSIONS));

	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("blah :%s\n", alGetString(error));

	DebugPrintf("OpenAL vendor string: %s\n", alGetString(AL_VENDOR));
	DebugPrintf("OpenAL renderer string: %s\n", alGetString(AL_RENDERER));
	DebugPrintf("OpenAL version string: %s\n", alGetString(AL_VERSION));
	DebugPrintf("OpenAL extensions: %s\n", alGetString(AL_EXTENSIONS));

	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("blah :%s\n", alGetString(error));

	DebugPrintf("openal: info end\n");
}

BOOL sound_init( void )
{
	Device = alcOpenDevice(NULL); // preferred device
	if(!Device)
		return FALSE;

	Context = alcCreateContext(Device,NULL);
	if(!Context)
		return FALSE;

	alcMakeContextCurrent(Context);

	{
		ALint i;
		ALfloat pos[3];
		alGetListeneri(AL_GAIN,&i);
		DebugPrintf("listener gain: %d\n",i);
		alGetListenerfv(AL_POSITION,pos);
		DebugPrintf("listener position: %f %f %f\n",pos[0],pos[1],pos[2]);
	}

	print_info();

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
	if(!source)
		return;
	alDeleteBuffers( 1, &source->buffer );
	alDeleteSources( 1, &source->source );
	free(source);
	source = NULL;
}

BOOL sound_is_playing( sound_t * source )
{
	ALint state;
	alGetSourcei( source->source, AL_SOURCE_STATE, &state	);
	return (state == AL_PLAYING);
}

void sound_get_seek( sound_t * source, long * bytes )
{
	ALint i;
	alGetSourcei(source->source, AL_BYTE_OFFSET, &i);
	*bytes = (long) i;
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

sound_t * sound_load(char *path)
{
	ALenum error;
	ALenum format;
	SDL_AudioSpec wav_spec;
	Uint8 *wav_buffer;
	sound_t * source = malloc(sizeof(sound_t));
	char * file_path = convert_path(path);

	// Generate Buffers
	alGetError(); // clear error code
	alGenBuffers(1, &source->buffer);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenBuffers :%d\n", error);
		return NULL;
	}

	if( SDL_LoadWAV(file_path, &wav_spec, &wav_buffer, &wav_spec.size) == NULL )
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
	alBufferData(source->buffer,format,wav_buffer,wav_spec.size,wav_spec.freq);
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

	{
		ALint i;
		ALfloat pos[3];
		sound_listener_position(0,0,0);
		alGetSourcei(source->source,AL_GAIN,&i);
		DebugPrintf("buffer gain: %d\n",i);
		alGetSource3f(source->source,AL_POSITION,&pos[0],&pos[1],&pos[2]);
		DebugPrintf("buffer position: %f %f %f\n",pos[0],pos[1],pos[2]);
		alGetSourcei(source->source,AL_SOURCE_RELATIVE,&i);
		DebugPrintf("buffer relative? %s\n",(i==AL_TRUE)?"true":"false");
	}

	return source;
}

BOOL sound_duplicate( sound_t * source, sound_t ** destination )
{
	*destination = malloc(sizeof(sound_t));
	memcpy(*destination,source,sizeof(sound_t));
	return TRUE;
}

// i think we can ignore these
void sound_set_freq( void* ptr, float freq ){}

// i believe this is only for 2d sounds
void sound_volume( sound_t * source, long volume ){}
void sound_pan( sound_t * source, long pan ){}

#endif // SOUND_OPENAL
