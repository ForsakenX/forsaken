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
#include <math.h>

//
// globals
//

// tells forsaken to use 3d functions
BOOL Sound3D = FALSE;

static ALCdevice* Device = NULL;
static ALCcontext* Context = NULL;

struct {
	int buffers;
	int sources;
	int playing;
} stats;

//
// buffer description
//

#define MAX_PATH 500

typedef struct sound_buffer_t {
	ALuint id;
	char path[MAX_PATH];
};

typedef struct sound_source_t {
	ALuint id;
	ALuint buffer;
	BOOL playing;
	char path[MAX_PATH];
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
		DebugPrintf("error:  :%s\n", alGetString(error));

	alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &version_major);
	alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &version_minor);
	
	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("error:  :%s\n", alGetString(error));

	DebugPrintf("ALC version: %d.%d\n", (int)version_major, (int)version_minor);
	DebugPrintf("ALC extensions: %s\n", alcGetString(device, ALC_EXTENSIONS));

	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("error:  :%s\n", alGetString(error));

	DebugPrintf("OpenAL vendor string: %s\n", alGetString(AL_VENDOR));
	DebugPrintf("OpenAL renderer string: %s\n", alGetString(AL_RENDERER));
	DebugPrintf("OpenAL version string: %s\n", alGetString(AL_VERSION));
	DebugPrintf("OpenAL extensions: %s\n", alGetString(AL_EXTENSIONS));

	if ((error = alGetError()) != AL_NO_ERROR)
		DebugPrintf("error:  :%s\n", alGetString(error));

	DebugPrintf("openal: info end\n");
}

BOOL sound_init( void )
{
	// TODO - disabling the ability to re-init sound system
	// on ubuntu using "drivers = pulse" will handle in alcOpenDevice
	// is there really even a reason we need to re-init sound?
	static int initialized = 0;
	if(initialized)
		return TRUE;
	initialized = 1;

	Device = alcOpenDevice(NULL); // preferred device
	if(!Device)
		return FALSE;

	Context = alcCreateContext(Device,NULL);
	if(!Context)
		return FALSE;

	alcMakeContextCurrent(Context);

	// since the game was based around dsound
	// it performs some volume calculations using this value
	// dsound defines DSBVOLUME_MIN as -10000
	// and forsaken defined min volume as 1/3 of that
	sound_minimum_volume = -10000 / 3;

	// global listener sound set to 50% to reduce crackling
	// TODO - we should probably have a global sound level setting
	alListenerf(AL_GAIN, 0.5f);

	{
		ALfloat f;
		ALfloat pos[3];
		alGetListenerf(AL_GAIN,&f);
		DebugPrintf("listener gain: %f\n",f);
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

BOOL sound_listener_position( float x, float y, float z )
{
	alListener3f(AL_POSITION, x, y, -z);
	return TRUE;
}

BOOL sound_listener_velocity( float x, float y, float z )
{
	alListener3f(AL_VELOCITY, x, y, -z);
	return TRUE;
}

// TODO - does the at-vector equate to forward vector?
BOOL sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
)
{
	float vector[6] = {fx,fy,-fz,ux,uy,-uz};
	alListenerfv(AL_ORIENTATION, &vector[0]);
	return TRUE;
}

//
// Buffers
//

// (only 3d buffer routine)
// TODO - we'll want to set velocity some day for moving players ?
void sound_position( sound_source_t * source, float x, float y, float z, float min, float max )
{
	alSource3f(source->id, AL_POSITION, x, y, -z);
	alSourcef(source->id, AL_MAX_DISTANCE, max);
	alSourcef(source->id, AL_REFERENCE_DISTANCE, min); // is this right?
}

void sound_play( sound_source_t * source )
{
	if(!source->playing)
		stats.playing++;
	source->playing = TRUE;
	//
	alSourcePlay( source->id );
	//
	DebugPrintf("sound_play: playing sound='%s' count=%d\n",
		source->path,
		stats.playing);
}

void sound_play_looping( sound_source_t * source )
{
	if(!source->playing)
		stats.playing++;
	source->playing = TRUE;
	//
	alSourcei( source->id, AL_LOOPING, AL_TRUE );
	sound_play( source );
	//
	DebugPrintf("sound_play_looping: playing %d\n",stats.playing);
}

void sound_stop( sound_source_t * source )
{
	if(source->playing)
		stats.playing--;
	source->playing = FALSE;
	//
	alSourceStop( source->id );
	//
	DebugPrintf("sound_stop: playing %d\n",stats.playing);
}

long sound_size( sound_source_t * source )
{
	ALint size;
	alGetBufferi( source->buffer, AL_SIZE, &size );
	return (long) size;
}

void sound_release_source( sound_source_t * source )
{
	if(!source)
		return;
	if(source->playing)
		stats.playing--;
	source->playing = FALSE;
	// deleting source implicitly detaches buffer
	alDeleteSources( 1, &source->id );
	// clean up resources
	free(source);
	source = NULL;
	// show stats
	stats.sources--;
	DebugPrintf("sound_release_source: buffers %d sources %d playing %d\n",
				stats.buffers,stats.sources,stats.playing);
}

void sound_release_buffer( sound_buffer_t * buffer )
{
	// buffers will not delete if they are attached to other sources
	alGetError(); // clear error so we can see if buffer is attached elsewhere
	alDeleteBuffers( 1, &buffer->id );
	// if buffer attached elsewhere than error is generated
	if(alGetError() == AL_NO_ERROR)
		stats.buffers--;
	else
		DebugPrintf("sound_release_buffer: error buffer %d still attached to a source.\n",
			buffer->id);
	// clean up resources
	free(buffer);
	buffer = NULL;
	// show stats
	DebugPrintf("sound_release_buffer: buffers %d sources %d playing %d\n",
				stats.buffers,stats.sources,stats.playing);
}

long sound_bps( sound_source_t * source )
{
	long rate;
	ALint freq, bits, channels;
	alGetBufferi( source->buffer, AL_BITS, &bits );
	alGetBufferi( source->buffer, AL_FREQUENCY, &freq );
	alGetBufferi( source->buffer, AL_CHANNELS, &channels );
	// bits should be a multiple of 8
	// the remainder is lost when you convert to long, anyway
	rate = (freq * channels * bits) / 8;
	return rate;
}

float sound_get_pitch( sound_source_t * source )
{
	ALfloat pitch;
	alGetSourcef( source->id, AL_PITCH, &pitch );
	return (float) pitch;
}

void sound_set_pitch( sound_source_t * source, float pitch )
{
	ALfloat f = pitch ? pitch : 1.0f ; // 1.0f is default
	alSourcef( source->id, AL_PITCH, f );
	//DebugPrintf("sound_pitch: %f\n",f);
}

void sound_volume( sound_source_t * source, long millibels )
{
	// gain is scaled to (silence) 0.0f through (no change) 1.0f
	// millibels = hundredths of decibels (dB)
	// defined in Dsound.h as (no change) 0 and (silence) -10,000
	ALfloat f = (ALfloat) pow(10.0, millibels/2000.0);
	alSourcef(source->id, AL_GAIN, f);
	//DebugPrintf("sound_volume: %d\n",millibels);
}

void sound_pan( sound_source_t * source, long _pan )
{
	// where pan is -1 (left) to +1 (right)
	// must be scaled from -1 <-> +1
	// probably need to scale down by 10,000, since dsound goes from -10000 to +10000
	// so:
	float pan = (float) _pan / 10000.0f;
	alSource3f(source->id, AL_POSITION, pan, (float) sqrt(1 - pan*pan), 0.0f);
	//DebugPrintf("sound_pan: %f\n",pan);
}

BOOL sound_is_playing( sound_source_t * source )
{
	ALint state;
	alGetSourcei( source->id, AL_SOURCE_STATE, &state );
	return (state == AL_PLAYING);
}

long sound_get_seek( sound_source_t * source )
{
	ALint i;
	alGetSourcei(source->id, AL_BYTE_OFFSET, &i);
	return (long) i;
}

void sound_set_seek( sound_source_t * source, long bytes )
{
	alSourcei(source->id, AL_BYTE_OFFSET, bytes);
}

sound_buffer_t * sound_load(char *path)
{
	ALenum error;
	ALenum format;
	SDL_AudioSpec wav_spec;
	Uint8 *wav_buffer;

	// create the buffer
	sound_buffer_t * buffer = malloc(sizeof(sound_buffer_t));
	char * file_path = convert_path(path);
	strncpy(buffer->path,file_path,MAX_PATH-1);

	// clear error code
	alGetError();

	// Generate Buffers
	alGenBuffers(1, &buffer->id);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenBuffers: %s\n", alGetString(error));
		return NULL;
	}

	if( SDL_LoadWAV(file_path, &wav_spec, &wav_buffer, &wav_spec.size) == NULL )
	{
		DebugPrintf("Could not open: %s\n", SDL_GetError());
		return NULL;
	}

	if(wav_spec.format == AUDIO_U8 || wav_spec.format == AUDIO_S8) // 8 bit
	{
		// openal only supports usigned 8bit
		if(wav_spec.format == AUDIO_S8)
		{
			int i;
			for(i = 0; i < (int) wav_spec.size; i++)
				wav_buffer[i] ^= 0x80; // converts S8 to U8
		}
		if(wav_spec.channels == 1)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_STEREO8;
	}
	else // 16 bit
	{
		// openal only supports signed 16bit
		if(wav_spec.format == AUDIO_U16)
		{
			int i;
			for(i = 0;i < (int)wav_spec.size/2;i++)
				((Uint16*)wav_buffer)[i] ^= 0x8000; // converts U16 to S16
		}
		if(wav_spec.channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
	}

	// Copy data into AL Buffer 0
	alBufferData(buffer->id,format,wav_buffer,wav_spec.size,wav_spec.freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alBufferData: %s\n", alGetString(error));
		alDeleteBuffers(1, &buffer->id);
		return NULL;
	}

	// data has been coppied
	SDL_FreeWAV(wav_buffer);

	stats.buffers++;
	DebugPrintf("sound_load: buffers %d sources %d playing %d\n",
				stats.buffers,stats.sources,stats.playing);

	return buffer;
}

sound_source_t * sound_source( sound_buffer_t * buffer )
{
	ALenum error;

	sound_source_t * source = malloc(sizeof(sound_source_t));
	source->playing = FALSE;
	source->buffer = buffer->id;
	strncpy(source->path,buffer->path,MAX_PATH);

	// clear errors
	alGetError();

	// generate a new source id
	alGenSources(1,&source->id);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alGenSources: %s\n", alGetString(error));
		free(source);
		source = NULL;
		return NULL;
	}

	// attach the buffer to the source
	alSourcei(source->id, AL_BUFFER, source->buffer);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DebugPrintf("alSourcei AL_BUFFER: %s\n", alGetString(error));
		free(source);
		source = NULL;
		return NULL;
	}

	alSourcei(source->id,AL_SOURCE_RELATIVE,
		Sound3D ? AL_FALSE : AL_TRUE);

	stats.sources++;
	DebugPrintf("sound_source: sources %d buffers %d playing %d\n",
				stats.sources,stats.buffers,stats.playing);

	return source;
}

#endif // SOUND_OPENAL
