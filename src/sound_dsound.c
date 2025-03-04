#ifdef SOUND_DSOUND

#define DIRECTSOUND_VERSION 0x0700
#include <dsound.h>
#include <stdio.h>

#include "main.h"
#include "util.h"
#include "file.h"
#include "sound.h"

//
// globals
//

LPDIRECTSOUND lpDS = NULL;
bool Sound3D = false;

//
// Generic Functions
//

bool sound_init( void )
{
	int iErr;
	lpDS = NULL;
	if FAILED( CoInitialize(NULL) )
		return false;

	// Attempt to initialize with DirectSound.
	// First look for DSOUND.DLL using CoCreateInstance.
	iErr = CoCreateInstance(&CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER,
								 &IID_IDirectSound, (void **) &lpDS);
	
		
	if ((iErr >= DS_OK)	&& (lpDS)) // Found DSOUND.DLL
		iErr = IDirectSound_Initialize(lpDS, NULL);	// Try to init Direct Sound.

	if (iErr < DS_OK)
		return false; // Failed to get DirectSound, so no sound-system available.

	// build sound_caps structure
	sound_minimum_volume = (DSBVOLUME_MIN / 3);
	
	// Set control-level of DirectSound. (To normal, default.)
	if(IDirectSound_SetCooperativeLevel(lpDS, GetActiveWindow(), DSSCL_EXCLUSIVE) >= DS_OK)
		return true;

	// If here, failed to initialize sound system in some way
	IDirectSound_Release(lpDS);
	lpDS = NULL;
	return(false);
}

//
// Listener
//

//	 In dsound only valid when using 3d sound
//   which this game does not currently use
//   this is left here for openAL later

bool sound_listener_position( float x, float y, float z )
{
	return true;
}

bool sound_listener_velocity( float x, float y, float z )
{
	return true;
}

bool sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
)
{
	return true;
}

//
// Buffers
//

void sound_destroy( void )
{
	if ( !lpDS )
		return;
	IDirectSound_Release(lpDS);
}

void sound_play( sound_t * buffer )
{
	IDirectSoundBuffer_Play( buffer, 0, 0, 0 );
}

void sound_play_looping( sound_t * buffer )
{
	IDirectSoundBuffer_Play( buffer, 0, 0, DSBPLAY_LOOPING );
}

void sound_stop( sound_t * buffer )
{
	IDirectSoundBuffer_Stop( buffer );
}

void sound_release( sound_t * buffer )
{
	if (buffer != NULL)
		return;
	buffer->lpVtbl->Release(buffer);
	buffer = NULL;
}

bool sound_is_playing( sound_t * buffer )
{
	DWORD dwStatus;
	IDirectSoundBuffer_GetStatus( buffer, &dwStatus );
	return (dwStatus & DSBSTATUS_PLAYING);
}

void sound_set_pitch( sound_t * buffer, float freq )
{
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, OrigFreq;

	// BUG:  Appears buffer pointer goes bad or is passed in as NULL
	if(!buffer)
	{
		DebugPrintf("BUG: sound_set_pitch() buffer passed in was null\n");
		return;
	}

	if ( !freq || ( freq == 1.0F ) )
	{
		OrigFreq = DSBFREQUENCY_ORIGINAL; 
	}
	else
	{
		// get original frequency of buffer
		IDirectSoundBuffer_GetFormat( buffer, NULL, 0, &dwSizeWritten );
		lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
		IDirectSoundBuffer_GetFormat( buffer, lpwaveinfo, dwSizeWritten, 0 );
		OrigFreq = lpwaveinfo->nSamplesPerSec; 
		free(lpwaveinfo);
	
		// work out new frequency
		OrigFreq = (DWORD)( (float)OrigFreq * freq );

		if ( OrigFreq < DSBFREQUENCY_MIN )
			OrigFreq = DSBFREQUENCY_MIN;

		if ( OrigFreq > DSBFREQUENCY_MAX )
			OrigFreq = DSBFREQUENCY_MAX;
	}

	// set frequency
	if ( IDirectSoundBuffer_SetFrequency( buffer, OrigFreq ) != DS_OK )
		DebugPrintf("sound_set_pitch: failed\n");
}

void sound_volume( sound_t * buffer, long volume )
{
	IDirectSoundBuffer_SetVolume( buffer, volume );
}

void sound_pan( sound_t * buffer, long pan )
{
	IDirectSoundBuffer_SetPan( buffer, pan );
}

// this sets the location in 3d space of the sound
void sound_position( sound_t * buffer, float x, float y, float z, float min, float max )
{
}

sound_t * sound_load(char *name)
{
    IDirectSoundBuffer *sound_buffer = NULL;
    DSBUFFERDESC buffer_description = {0};
	WAVEFORMATEX buffer_format;
	SDL_AudioSpec wav_spec;
	u_int32_t wav_length;
	u_int8_t *wav_buffer;
	DWORD flags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE;

	if (!lpDS)
		return NULL;

	if( SDL_LoadWAV(name, &wav_spec, &wav_buffer, &wav_length) == NULL )
	{
		DebugPrintf("Could not open test.wav: %s\n", SDL_GetError());
		return NULL;
	}

	// http://msdn.microsoft.com/en-us/library/ms897764.aspx
    buffer_description.dwSize			= sizeof(buffer_description);
    buffer_description.dwFlags			= flags;
	buffer_description.dwBufferBytes	= (DWORD) wav_length;
	buffer_description.lpwfxFormat		= &buffer_format;

	// http://msdn.microsoft.com/en-us/library/dd757720%28VS.85%29.aspx
	buffer_format.wFormatTag		= (WORD)	WAVE_FORMAT_PCM;
	buffer_format.nChannels			= (WORD)	wav_spec.channels;
	buffer_format.wBitsPerSample	= (WORD)	((wav_spec.format == AUDIO_U8 || wav_spec.format == AUDIO_S8) ? 8 : 16);
	buffer_format.nSamplesPerSec	= (DWORD)	wav_spec.freq;
	buffer_format.nBlockAlign		= (WORD)	(buffer_format.nChannels * buffer_format.wBitsPerSample) / 8;
	buffer_format.nAvgBytesPerSec	= (DWORD)	(buffer_format.nSamplesPerSec * buffer_format.nBlockAlign);
	buffer_format.cbSize			= (WORD)	0;

	// http://msdn.microsoft.com/en-us/library/ms898123.aspx
	if( IDirectSound_CreateSoundBuffer( lpDS, &buffer_description, &sound_buffer, NULL ) == DS_OK )
    {
        LPVOID pMem1, pMem2;
        DWORD dwSize1, dwSize2;
        if (SUCCEEDED(IDirectSoundBuffer_Lock(sound_buffer, 0, wav_length, &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
        {
            CopyMemory(pMem1, wav_buffer, dwSize1);
            if ( 0 != dwSize2 )
                CopyMemory(pMem2, wav_buffer+dwSize1, dwSize2);
            IDirectSoundBuffer_Unlock(sound_buffer, pMem1, dwSize1, pMem2, dwSize2);
        }
		else
		{
            sound_release(sound_buffer);
            sound_buffer = NULL;
		}
    }
    else
    {
        sound_buffer = NULL;
    }

	SDL_FreeWAV(wav_buffer);

    return sound_buffer;
}

bool sound_duplicate( sound_t * source, sound_t ** destination )
{
	return IDirectSound_DuplicateSoundBuffer( lpDS, source, destination	) == DS_OK;
}

#endif // SOUND_DSOUND
