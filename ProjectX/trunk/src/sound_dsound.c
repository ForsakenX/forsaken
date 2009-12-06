#ifdef SOUND_DSOUND

#define DIRECTSOUND_VERSION 0x0700
#include <dsound.h>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>

#include "main.h"
#include "util.h"
#include "file.h"
#include "sound.h"

LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUND3DLISTENER	lpDS3DListener;
LPDIRECTSOUNDBUFFER		glpPrimaryBuffer;
DSCAPS					DSCaps;
#define _HSNDOBJ_DEFINED

// globals
BOOL Sound3D = FALSE;
BOOL FreeHWBuffers = TRUE;

//
// Generic Functions
//

// Initializes main globals
// a. A DirectSound Object
// b. A DirectSound3DListener Object
// c. A Primary Buffer.
BOOL sound_init( void )
{
	DSBUFFERDESC dsbdesc;
	int iErr;
	lpDS = NULL;
	if FAILED( CoInitialize(NULL) )
		return FALSE;

	// Attempt to initialize with DirectSound.
	// First look for DSOUND.DLL using CoCreateInstance.
	iErr = CoCreateInstance(&CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER,
								 &IID_IDirectSound, (void **) &lpDS);
	
		
	if ((iErr >= DS_OK)	&& (lpDS)) // Found DSOUND.DLL
		iErr = IDirectSound_Initialize(lpDS, NULL);	// Try to init Direct Sound.

	if (iErr < DS_OK)
		return FALSE; // Failed to get DirectSound, so no sound-system available.

	// build sound_caps structure
	{
		DSCAPS DSCaps;
		memset (&DSCaps, 0, sizeof (DSCAPS));
		DSCaps.dwSize = sizeof(DSCAPS);
		IDirectSound_GetCaps( lpDS, &DSCaps );
		sound_caps.memory = DSCaps.dwMaxContigFreeHwMemBytes;
		sound_caps.buffers = DSCaps.dwFreeHwMixingStaticBuffers;
		sound_caps.min_volume = ( DSBVOLUME_MIN / 3);
	}

	// Succeeded in getting DirectSound.
	// Check to see if there is 3D acceleration.
	/*
	DSCAPS	dsCaps;
	dsCaps.dwSize = sizeof(DSCAPS);
	IDirectSound_GetCaps(lpDS, &dsCaps);
	// Allow 3D sound only if acceleration exists.
	Sound3D = ((dsCaps.dwMaxHw3DAllBuffers > 0) ? TRUE : FALSE);
	*/

	// If here, got a valid sound-interface...
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	dsbdesc.dwBufferBytes = 0; //dwBufferBytes and lpwfxFormat must be set this way.
	dsbdesc.lpwfxFormat = NULL;
	
	// Set control-level of DirectSound. (To normal, default.)
	if (IDirectSound_SetCooperativeLevel(lpDS, GetActiveWindow(), DSSCL_EXCLUSIVE /*DSSCL_NORMAL*/) >= DS_OK)    
	{
		// Create primary buffer.
		if ( IDirectSound_CreateSoundBuffer( lpDS, &dsbdesc, &glpPrimaryBuffer, NULL ) == DS_OK )
		{
			
			DWORD dwSizeWritten;
			LPWAVEFORMATEX lpwaveinfo;
			
			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, NULL, 0, &dwSizeWritten );
			lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, lpwaveinfo, dwSizeWritten, 0 );
			
			lpwaveinfo->nChannels = 2;
			lpwaveinfo->nSamplesPerSec = 22050;
			lpwaveinfo->wBitsPerSample = 16;
			lpwaveinfo->nBlockAlign = 4;
			lpwaveinfo->nAvgBytesPerSec = lpwaveinfo->nSamplesPerSec * lpwaveinfo->nBlockAlign;

			if ( IDirectSoundBuffer_SetFormat( glpPrimaryBuffer, lpwaveinfo ) != DS_OK )
			{
				free(lpwaveinfo);
				return FALSE;
			}

			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, lpwaveinfo, dwSizeWritten, 0 );
			DebugPrintf("using primary buffer format: wFormatTag %d, nChannels %d, nSamplesPerSec %d, nAvgBytesPerSec %d, nBlockAlign %d, wBitsPerSample %d\n",
				lpwaveinfo->wFormatTag, lpwaveinfo->nChannels, lpwaveinfo->nSamplesPerSec, lpwaveinfo->nAvgBytesPerSec, lpwaveinfo->nBlockAlign, lpwaveinfo->wBitsPerSample );
			
			free(lpwaveinfo);

			// If no 3D, we are done.
			if (!Sound3D)
				return(TRUE);

			// If 3D, need to get listener interface.
			if (IDirectSoundBuffer_QueryInterface(glpPrimaryBuffer, &IID_IDirectSound3DListener,
															  (void **) &lpDS3DListener) >= DS_OK)
			{
				// Start primary-buffer looped-playing to reduce overhead on secondary-play calls.
				glpPrimaryBuffer->lpVtbl->Play(glpPrimaryBuffer, 0, 0, DSBPLAY_LOOPING);
				return(TRUE);
			}
			else
			{
				// Failed to get 3D, so we only have 2D-controls. Release listener if valid, and go with 2D.
				if (lpDS3DListener)
				{
					IDirectSound3DListener_Release(lpDS3DListener);
					lpDS3DListener = NULL;
				}
				
				// Flag all 3D as off.
				Sound3D = FALSE;

				return(TRUE);	// Still succeed initialization, but with 2D.
			}
		}
	}

	// If here, failed to initialize sound system in some way. (Either in SetCoopLevel, or creating primary-buffer.)
	IDirectSound_Release(lpDS);
	lpDS = NULL;
	
	DebugPrintf("returning FALSE from Init_SoundGlobals at point 2\n");

	return(FALSE);
}

// not really sure what this function is doing besides the obvious fact of using hw memory
// but it's one of the last things using the dsound api so i had to move it into this file
// even though it's touching structures from sfx.c that i would rather it not touch

// note: managing of hw/sw will not be needed in openAL

#define MIN_SOUNDCARD_HW_MEM 262144	// 256K 
static void* sound_buffer_load_compound(BOOL use_sound_hw, int *num_allocated_ptr);
int sound_load_to_hw( void )
{
	DWORD FreeMem = 1;  // so that it equates to TRUE
	DWORD buffers_before;
	int AllocatedCompoundSfx = 0;
	int j, i;
	BOOL use_sound_hw = 0;

	NumDupCompoundBuffers = 0;

	if ( sound_caps.memory > MIN_SOUNDCARD_HW_MEM )
	{
		FreeMem = sound_caps.memory;
		use_sound_hw = TRUE;
		DebugPrintf("Loading compound sfx buffer in HW\n");
	}

	// if we have hardware mixing channels & hardware mem
	if ( FreeMem && ( sound_caps.buffers >= MIN_COMPOUND_BUFFERS ) )
	{
		buffers_before = sound_caps.buffers;

		// load first compound buffer
		CompoundSfxBuffer[0].buffer = sound_buffer_load_compound(
			use_sound_hw,
			&AllocatedCompoundSfx 
		);

		// if buffer succesfully loaded...
		if ( CompoundSfxBuffer[0].buffer )
		{
			if (!sound_buffer_in_hw(CompoundSfxBuffer[ 0 ].buffer))
			{
   				// no point in using compound buffer, since HW mixing channels are not being used
				DebugPrintf("Crap sound driver detected compound buffer will not be created\n");
				sound_buffer_release( CompoundSfxBuffer[0].buffer );
				DebugPrintf("Releasing sound buffer %s %d\n", __FILE__, __LINE__ );
			}
			else
			{
				CompoundSfxBuffer[0].current_sfx = -1;

				// duplicate for rest of buffers ( limit number of buffers to MAX_COMPOUND_BUFFERS )
				NumDupCompoundBuffers = sound_caps.buffers;

				if ( NumDupCompoundBuffers > MAX_COMPOUND_BUFFERS )
					NumDupCompoundBuffers = MAX_COMPOUND_BUFFERS;

				for (j = 1; j < NumDupCompoundBuffers; j++)
				{
					if ( !sound_buffer_duplicate( 
						CompoundSfxBuffer[0].buffer,
						&CompoundSfxBuffer[j].buffer 
					))
					{	
						DebugPrintf("unable to duplicate more than %d compound buffers\n",j);
				
						// if insufficient buffers created...
						if ( j < MIN_COMPOUND_BUFFERS )
						{
							// free all created buffers
							for ( i = 0; i < j; i++ )
								 sound_buffer_release( CompoundSfxBuffer[ i ].buffer );

							// return 0 to indicate no hw buffers
							return 0;
						}

						break;
					}

					CompoundSfxBuffer[j].current_sfx = -1;
				}

				NumDupCompoundBuffers = j;
			}
		}
		else
		{
			DebugPrintf("unable to create compound buffer\n");
		}
	}
	else
	{
		DebugPrintf("not loading compound sfx buffer becuase not enough mixing channels or not enough free memory in sound driver.\n");
	}

	return AllocatedCompoundSfx;
}

void sound_commit_any_pending( void )
{
	if ( !lpDS3DListener )
		return;
	IDirectSound3DListener_CommitDeferredSettings(
		lpDS3DListener
	);
}

//
// Listener
//

BOOL sound_listener_position( float x, float y, float z )
{
	return IDirectSound3DListener_SetPosition(
		lpDS3DListener,	
		x, y, z, 
		DS3D_DEFERRED
	) == DS_OK;
}

BOOL sound_listener_velocity( float x, float y, float z )
{
	return IDirectSound3DListener_SetVelocity(
		lpDS3DListener,	
		x, y, z, 
		DS3D_DEFERRED
	) == DS_OK;
}

BOOL sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
)
{
	return IDirectSound3DListener_SetOrientation(
		lpDS3DListener,	
		fx, fy, fz, 
		ux, uy, uz, 
		DS3D_DEFERRED
	) == DS_OK;
}

//
// Buffers
//

void sound_destroy( void )
{
	if ( !lpDS )
		return;
	sound_buffer_release( glpPrimaryBuffer );
	IDirectSound_Release(lpDS);
}

void sound_buffer_play( void * buffer )
{
	IDirectSoundBuffer_Play(
		(IDirectSoundBuffer*)buffer, 0, 0, 0 
	);
}

void sound_buffer_play_looping( void * buffer )
{
	IDirectSoundBuffer_Play(
		(IDirectSoundBuffer*)buffer, 0, 0,
		DSBPLAY_LOOPING 
	);
}

void sound_buffer_stop( void * buffer )
{
	IDirectSoundBuffer_Stop( 
		(IDirectSoundBuffer*) buffer 
	);
}

DWORD sound_buffer_size( void * buffer )
{
	DSBCAPS dsbcaps; 
	dsbcaps.dwSize = sizeof( DSBCAPS );
	IDirectSoundBuffer_GetCaps(
		(IDirectSoundBuffer*)buffer,
		&dsbcaps 
	);
	return dsbcaps.dwBufferBytes;
}

BOOL sound_buffer_in_hw( void * buffer )
{
	DSBCAPS DSBCaps;
	memset (&DSBCaps, 0, sizeof( DSBCAPS ) );
	DSBCaps.dwSize = sizeof( DSBCAPS );
	IDirectSoundBuffer_GetCaps(
		(IDirectSoundBuffer*) buffer,
		&DSBCaps 
	);
	return (DSBCaps.dwFlags & DSBCAPS_LOCHARDWARE);
}

void sound_buffer_release( void * ptr )
{
	IDirectSoundBuffer* buffer = ptr;
	if (buffer != NULL)
		return;
	buffer->lpVtbl->Release(buffer);
	buffer = NULL;
}

void sound_buffer_3d_release( void * buffer )
{
	IDirectSound3DBuffer_Release(
		(IDirectSound3DBuffer*) buffer 
	);
}

BOOL sound_buffer_is_playing( void * buffer )
{
	DWORD dwStatus;
	IDirectSoundBuffer_GetStatus(
		(IDirectSoundBuffer*)buffer, 
		&dwStatus 
	);
	return (dwStatus & DSBSTATUS_PLAYING);
}

BOOL sound_buffer_duplicate( void * buffer, void** duplicate )
{
	return IDirectSound_DuplicateSoundBuffer(
		lpDS, 
		(LPDIRECTSOUNDBUFFER)  buffer, 
		(LPDIRECTSOUNDBUFFER*) duplicate
	) == DS_OK;
}

void sound_buffer_set_freq( void* ptr, float freq )
{
	IDirectSoundBuffer * buffer = ptr;
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, OrigFreq;

	// BUG:  Appears buffer pointer goes bad or is passed in as NULL
	if(!buffer)
	{
		DebugPrintf("BUG: sound_buffer_set_freq() buffer passed in was null\n");
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
		DebugPrintf("sound_buffer_set_freq: failed\n");
}

void sound_buffer_volume( void * buffer, long volume )
{
	IDirectSoundBuffer_SetVolume( (IDirectSoundBuffer*) buffer, volume );
}

void sound_buffer_pan( void * buffer, long pan )
{
	IDirectSoundBuffer_SetPan( (IDirectSoundBuffer*) buffer, pan );
}

DWORD sound_buffer_get_freq( void * buffer ) // samples per sec
{
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, freq;
	IDirectSoundBuffer_GetFormat(
		(IDirectSoundBuffer*) buffer,
		NULL, 0, &dwSizeWritten 
	);
	lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
	IDirectSoundBuffer_GetFormat( 
		(IDirectSoundBuffer*) buffer,
		lpwaveinfo, dwSizeWritten, 0 
	);
	freq = lpwaveinfo->nSamplesPerSec; 
	free(lpwaveinfo);
	return freq;
}

DWORD sound_buffer_get_rate( void * buffer ) // avg bytes per second
{
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, datarate;
	IDirectSoundBuffer_GetFormat( 
		(IDirectSoundBuffer*) buffer, 
		NULL, 0, &dwSizeWritten 
	);
	lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
	IDirectSoundBuffer_GetFormat( 
		(IDirectSoundBuffer*) buffer, 
		lpwaveinfo, dwSizeWritten, 0 
	);
	datarate = lpwaveinfo->nAvgBytesPerSec; 
	free(lpwaveinfo);
	return datarate;
}

// this gets the current play location
void sound_buffer_get_position( void * buffer, DWORD* time )
{
	IDirectSoundBuffer_GetCurrentPosition(
		(IDirectSoundBuffer*) buffer,
		time,
		NULL
	);
}

// this moves to a specific offset in the buffer
void sound_buffer_set_position( void * buffer, DWORD time )
{
	IDirectSoundBuffer_SetCurrentPosition(
		(IDirectSoundBuffer*) buffer,
		time
	);
}

// this sets the location in 3d space of the sound
void sound_buffer_set_3d_position( void * buffer, float x, float y, float z, float min, float max )
{			
	IDirectSound3DBuffer_SetPosition(
		(IDirectSound3DBuffer*) buffer,
		x, y, z, DS3D_IMMEDIATE
	);
	IDirectSound3DBuffer_SetMinDistance(
		(IDirectSound3DBuffer*) buffer,
		min, DS3D_IMMEDIATE
	); 
	IDirectSound3DBuffer_SetMaxDistance(
		(IDirectSound3DBuffer*) buffer,
		max, DS3D_IMMEDIATE
	); 
}

// loads a sound buffer object
// only used in one location so options are hard coded
static void* sound_buffer_load(char *name, DWORD flags);
void* sound_buffer_load_for_sfx(char* file)
{
	return (void*) sound_buffer_load(
		file, 
		DSBCAPS_CTRLFREQUENCY | 
		DSBCAPS_STATIC | 
		DSBCAPS_CTRLPAN | 
		DSBCAPS_CTRLVOLUME | 
		DSBCAPS_LOCSOFTWARE
	);
}

//
// Sources
//

void sound_source_destroy( sound_source_t * source )
{
	int i;
	if (!source)
		return;
	for (i = 0; i < MAX_DUP_BUFFERS; i++)
	{
		if (source->Dup_3DBuffer[i])
		{
            IDirectSound3DBuffer_Release(
				(IDirectSound3DBuffer*)source->Dup_3DBuffer[i]
			);
	        source->Dup_3DBuffer[i] = NULL;
		}
		if (source->Dup_Buffer[i])
		{
            sound_buffer_release(source->Dup_Buffer[i]);
	        source->Dup_Buffer[i] = NULL;
		}
	}
    free(source);
}

// Loads a sound_source_t from a Win32 resource in the current application.
static void* sound_buffer_load(char* file, DWORD flags);
sound_source_t *sound_source_create(char *path, int sfx_flags, int sfx)
{
    sound_source_t *pSO = NULL;
	void * Buffer = NULL;
	int i;
	HRESULT hres;
	DSBCAPS dsbcaps;	
	DWORD buf_flags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;

    pSO = (sound_source_t *)malloc(sizeof(sound_source_t));

	if ( !FreeHWBuffers )
		buf_flags |= DSBCAPS_LOCSOFTWARE;

    if (pSO != NULL)
    {
		memset( pSO, 0, sizeof(sound_source_t) );

		pSO->Dup_Buffer[0] = NULL;
		pSO->CompoundBuffer = FALSE;
		pSO->looping_sfx_index[0] = -1;

		// if sound is already loaded into HW...
		if ( ( IS_COMPOUND( sfx_flags ) ) && CompoundSfxAllocated[sfx] ) 
		{				  
			pSO->CompoundBuffer = TRUE;
			for (i = 0; i < MAX_DUP_BUFFERS; i++)
			   pSO->CompoundBufferLookup[i] = -1;
		}
		else
		{
				// create buffer. Will create in hw if available, since only 16 channels have been used for compound sfx
				if( buf_flags & DSBCAPS_CTRL3D )
				{
					pSO->Dup_Buffer[0] = NULL; //DSLoad3DSoundBuffer(pDS, path, &pSO->Dup_3DBuffer[0], buf_flags );
				}
				else
				{
					pSO->Dup_Buffer[0] = sound_buffer_load(path, buf_flags );
				}

			if( !pSO->Dup_Buffer[ 0 ] )
			{
				Msg("Unable to create sound buffer for %s\n", path );
				sound_source_destroy( pSO );
				return pSO;
			}


			// get caps of buffer
			memset( &dsbcaps, 0, sizeof( DSBCAPS ) );
			dsbcaps.dwSize = sizeof( DSBCAPS );
			IDirectSoundBuffer_GetCaps( (IDirectSoundBuffer*)pSO->Dup_Buffer[ 0 ], &dsbcaps );

			// if buffer in hw, should check free channels here, but Ensoniq driver always reports back 256
			// so we will just have to try duplicating until failure
			for (i = 1; i < MAX_DUP_BUFFERS; i++)
			{
				pSO->looping_sfx_index[i] = -1;
				// duplicate 2D buffer...
				if ( !sound_buffer_duplicate(
					pSO->Dup_Buffer[0],
					&pSO->Dup_Buffer[i]
				))
				{
					DebugPrintf("unable to duplicate sound buffer\n");

					// invalidate buffer & all duplicates
					sound_source_destroy( pSO );

					// was original buffer hw? if so, try to recreate in sw
					if ( dsbcaps.dwFlags & DSBCAPS_LOCHARDWARE )
					{
						DebugPrintf("trying to recreate in sw\n");
						FreeHWBuffers = FALSE;
						// recreate all buffers up to & including this one in software
						if( buf_flags & DSBCAPS_CTRL3D )
						{
							pSO->Dup_Buffer[ 0 ] = NULL; //DSLoad3DSoundBuffer(pDS, path, &pSO->Dup_3DBuffer[ 0 ], buf_flags | DSBCAPS_LOCSOFTWARE );
						}else{
							pSO->Dup_Buffer[ 0 ] = sound_buffer_load(path, buf_flags | DSBCAPS_LOCSOFTWARE );
						}

						if( !pSO->Dup_Buffer[ 0 ] )
						{
							Msg("Unable to create sound buffer for %s\n", path );
							sound_source_destroy( pSO );
							return pSO;
						}

						i = 0;
						continue;

					}else
					{
						// couldn't duplicate buffer in sw - just break out with buffer info still marked as invalid
						Msg("unable to duplicate buffers in sw\n");
						break;
					}
				}

				// query for 3D interface if we are using 3D...
				if ( pSO->Dup_3DBuffer[0] )
				{
					hres = IDirectSoundBuffer_QueryInterface((IDirectSoundBuffer*)pSO->Dup_Buffer[i], &IID_IDirectSound3DBuffer, &pSO->Dup_3DBuffer[i]);        
				}

			}
		}
    }

    return pSO;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//  These functions should probably be exposed since openAL allows creating buffers
//  But apparently only SndObjCreate requires it
//
//
//
////////////////////////////////////////////////////////////////////////////////////////

// these functions are helpers for the following sound buffer loading functions
static BOOL DSParseWave(void *Buffer, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize, void **End);
static void * DSGetWave( char *lpName , WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize);
static BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize);
static void * DSGetMultiWave( WAVEFORMATEX *pWaveHeaderStore, BYTE **ppbWaveData, DWORD *pcbWaveSize, DWORD dwFlags, int *num_allocated_ptr );

static void* sound_buffer_load(char *name, DWORD flags)
{
    IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;
	void * Buffer = NULL;

    if (Buffer = DSGetWave(name, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
    {
        dsBD.dwSize = sizeof(dsBD);
        dsBD.dwFlags = flags ;

        if ( IDirectSound_CreateSoundBuffer( lpDS, &dsBD, &pDSB, NULL ) == DS_OK )
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
                sound_buffer_release(pDSB);
                pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
        }
    }

	if( Buffer != NULL )
		free( Buffer );
    return pDSB;
}

static void* sound_buffer_load_compound(BOOL use_sound_hw, int *num_allocated_ptr)
{
	IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;
	void * Buffer = NULL;
	WAVEFORMATEX format;
	DWORD flags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;

	if(use_sound_hw)
		flags |= DSBCAPS_LOCHARDWARE;

    if (Buffer = DSGetMultiWave(&format, &pbWaveData, &dsBD.dwBufferBytes, flags, num_allocated_ptr))
    {
        dsBD.dwSize = sizeof(dsBD);
        dsBD.dwFlags = flags;

		dsBD.lpwfxFormat = &format;

		if ( IDirectSound_CreateSoundBuffer( lpDS, &dsBD, &pDSB, NULL ) == DS_OK )
        {
            if (!DSFillSoundBuffer(pDSB, Buffer, dsBD.dwBufferBytes))
            {
                sound_buffer_release(pDSB);
                pDSB = NULL;
            }
        }
    }

	if( Buffer != NULL )
		free( Buffer );
    return pDSB;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//  I really feel these don't belong in this file cause they use things from sfx.h
//  But they are basically part of the sound_buffer_load process
//
//
//
////////////////////////////////////////////////////////////////////////////////////////

BOOL CustomCompoundBufferSize = FALSE;
DWORD UserTotalCompoundSfxBufferSize = 0;
extern SFXNAME Sfx_Filenames[MAX_SFX];
static void * DSGetMultiWave( WAVEFORMATEX *pWaveHeaderStore, BYTE **ppbWaveData, DWORD *pcbWaveSize, DWORD dwFlags, int *num_allocated_ptr )
{
	long File_Size;
	long Read_Size;
	void *Buffer;
	void *End;
	int i, min, index, variant;
	char *TempCompositeBuffer, *CompositeBuffer, *IntermediateBuffer;
	char TempFileName[256];
	WAVEFORMATEX TempWaveHeader;
	int temp_num_allocated;

	LPWAVEFORMATEX pWaveHeader;
	
	DWORD BytesPerSec, dwGapSize, total_size, dwBufferOffset;
	float gap_size; 
	
	DWORD BufferSizeSoFar, MaxBufferSize;
	BOOL SfxChecked[ MAX_SFX ];
	DSCAPS DSCaps;
	BOOL error;
	
	memset (&DSCaps, 0, sizeof (DSCAPS));
	DSCaps.dwSize = sizeof(DSCAPS);
	IDirectSound_GetCaps( lpDS, &DSCaps );
	
	memset (SfxChecked, 0, sizeof( BOOL ) * MAX_SFX);

	BytesPerSec = (CompoundSfxBitDepth * CompoundSfxChannels / 8) * CompoundSfxFrequency;
	gap_size = (float)BytesPerSec * CompoundSfxGap;
	dwGapSize = (DWORD)gap_size;

	total_size = 0;

	// for all compound sfx...
	for ( i = 0; Sfx_Filenames[ i ].Name; i++ )
	{
		if ( ( IS_COMPOUND( Sfx_Filenames[ i ].Flags ) ) && !CompoundSfxAllocated[ i ] && SndLookup[ i ].Requested )
		{
			// if sfx does not exist, ignore
			if ( !SndLookup[ i ].Num_Variants )
				continue;

			for ( variant = 0; variant < SndLookup[ i ].Num_Variants; variant++ )
			{
				GetFullSfxPath( TempFileName, i, variant, SndLookup[ i ].Num_Variants );

				if ( File_Exists( TempFileName ) )
				{
					File_Size = Get_File_Size( TempFileName );	// how big is the file...
					if( !File_Size ) return NULL;

					Buffer = malloc( File_Size );							// alloc enough space to load it...
					if( Buffer == NULL ) return( NULL );					// if couldnt then return

					Read_Size = Read_File( TempFileName, Buffer, File_Size ); // Read it in making a note of the Size returned
					if( Read_Size != File_Size ) return( NULL );			// if size read doesnt qual file size return

					DSParseWave( Buffer, &pWaveHeader, ppbWaveData, pcbWaveSize, &End);

					// get size of file...
					total_size += *pcbWaveSize;
						
					// add space for gap afterwards...
					total_size += dwGapSize;

					free ( Buffer );
				}
			}
		}
	}

	if ( dwFlags & DSBCAPS_LOCHARDWARE )
		MaxBufferSize = DSCaps.dwMaxContigFreeHwMemBytes;
	else
	{
		return NULL;
	}

	if ( total_size > MaxBufferSize )
		total_size = MaxBufferSize;

	if ( CustomCompoundBufferSize && ( total_size > UserTotalCompoundSfxBufferSize ) )
		total_size = UserTotalCompoundSfxBufferSize;

	CompositeBuffer = (char *)malloc( total_size );
	TempCompositeBuffer = CompositeBuffer;
	dwBufferOffset = 0;
	BufferSizeSoFar = 0;
	do
	{
		min = -1;
		index = -1;

		// find sfx with next highest priority...
		for ( i = 0; Sfx_Filenames[ i ].Name; i++ )
			if ( IS_COMPOUND( Sfx_Filenames[ i ].Flags ) )
				if ( ( min == -1 ) && !SfxChecked[ i ] && !CompoundSfxAllocated[ i ] && SndLookup[ i ].Num_Variants && SndLookup[ i ].Requested )
				{
					min = Sfx_Filenames[ i ].Priority;
					index = i;
				}else
				{
					if ( ( Sfx_Filenames[ i ].Priority < min ) && !SfxChecked[ i ] && !CompoundSfxAllocated[ i ] && SndLookup[ i ].Num_Variants && SndLookup[ i ].Requested )
					{
						min = Sfx_Filenames[ i ].Priority;
						index = i;
					}
				}

		if ( index != -1 )	// sfx has been found...
		{
			SfxChecked[ index ] = TRUE;

			error = FALSE;
			IntermediateBuffer = TempCompositeBuffer;
			temp_num_allocated = *num_allocated_ptr;

			for ( variant = 0; variant < SndLookup[ index ].Num_Variants; variant++ )
			{

				GetFullSfxPath( TempFileName, index, variant, SndLookup[ index ].Num_Variants );
			
				if ( File_Exists( TempFileName ) )
				{
					File_Size = Get_File_Size( TempFileName );	// how big is the file...
					if( !File_Size )
					{
						error = TRUE;
						DebugPrintf("DSGetMultiWave() File size returned was zero\n");
						break;
					}

					Buffer = malloc( File_Size );		 // alloc enough space to load it...

					if( Buffer == NULL )
					{
						error = TRUE;
						DebugPrintf("DSGetMultiWave() unable to malloc memory for temp buffer\n");
						break;
					}

					Read_Size = Read_File( TempFileName, Buffer, File_Size ); // Read it in making a note of the Size returned

					if( Read_Size != File_Size )
					{
						error = TRUE;
						DebugPrintf("DSGetMultiWave() read file size not equal to actual file size\n");
						break;
					}

					DSParseWave( Buffer, &pWaveHeader, ppbWaveData, pcbWaveSize, &End);

					if ( 
						( (pWaveHeader)->nChannels != CompoundSfxChannels ) ||
						( (pWaveHeader)->nSamplesPerSec != CompoundSfxFrequency ) ||
						( (pWaveHeader)->wBitsPerSample != CompoundSfxBitDepth ) 
					)
					{
						DebugPrintf("Dsutil.c: DSGetMultiWave() - sfx %d variant %d not of correct type for composite buffer, ignoring\n", index, variant);
						*pWaveHeader = TempWaveHeader;
						error = TRUE;
						break;
					}
					else
					{
						TempWaveHeader = *pWaveHeader;

						// if pcbWaveSize < available mem left, and format is OK...
						if ( ( (*pcbWaveSize + dwGapSize) < ( total_size - BufferSizeSoFar ) ) || !total_size )
						{
							//DebugPrintf("sfx %d marked allocated\n", index);
						   
							BufferSizeSoFar += *pcbWaveSize + dwGapSize;

							TempSfxInfo[ *num_allocated_ptr ].SfxNum = index;
							TempSfxInfo[ *num_allocated_ptr ].StartPos = dwBufferOffset;
							TempSfxInfo[ *num_allocated_ptr ].Length = (*pcbWaveSize * 1000) / BytesPerSec;
							TempSfxInfo[ *num_allocated_ptr ].Bytes = *pcbWaveSize;
							TempSfxInfo[ *num_allocated_ptr ].Variant = variant;

							(*num_allocated_ptr)++;

							// check to see if max sfx allocated...
							if (*num_allocated_ptr == MAX_COMPOUND_SFX)
							{
								DebugPrintf("tried to allocate more than MAX_COMPOUND_SFX sfx\n");
								error = TRUE;
								break;
							}

							dwBufferOffset = dwBufferOffset + *pcbWaveSize + dwGapSize;

							// copy wave data to composite buffer...
							memcpy( TempCompositeBuffer, *ppbWaveData, *pcbWaveSize );
							TempCompositeBuffer += *pcbWaveSize;
							
							// add gap
							memset( TempCompositeBuffer, 0, dwGapSize );
							TempCompositeBuffer += dwGapSize;
						}else
						{
							DebugPrintf("Not enough HW mem for SFX %d\n", index);
							error = TRUE;
							break;
						}
					}

					*pWaveHeaderStore = *pWaveHeader;

					free ( Buffer );
				}
			}

			if ( error )
			{
				if ( Buffer )
					free ( Buffer );
				
				// move current buffer ptr back to position b4 this sfx
				TempCompositeBuffer = IntermediateBuffer;

				// invalidate compound sfx info for this sfx
				*num_allocated_ptr = temp_num_allocated;
			}else
			{
				CompoundSfxAllocated[ index ] = TRUE;
			}
		}
	}while ( index != -1 );

	*pcbWaveSize = BufferSizeSoFar;

	return (void *)CompositeBuffer;
}

static BOOL DSParseWave(void *Buffer, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize, void **End)
{
    DWORD *pdw;
    DWORD *pdwEnd;
    DWORD dwRiff;
    DWORD dwType;
    DWORD dwLength;

    if (ppWaveHeader)
        *ppWaveHeader = NULL;

    if (ppbWaveData)
        *ppbWaveData = NULL;

    if (pcbWaveSize)
        *pcbWaveSize = 0;

    pdw = (DWORD *)Buffer;
    dwRiff = *pdw++;
    dwLength = *pdw++;
    dwType = *pdw++;

    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
        goto exit;      // not even RIFF

    if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
        goto exit;      // not a WAV

    pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);
	*End = (void *)pdwEnd;

    while (pdw < pdwEnd)
    {
        dwType = *pdw++;
        dwLength = *pdw++;

        switch (dwType)
        {
        case mmioFOURCC('f', 'm', 't', ' '):
            if (ppWaveHeader && !*ppWaveHeader)
            {
                if (dwLength < sizeof(WAVEFORMAT))
                    goto exit;      // not a WAV

                *ppWaveHeader = (WAVEFORMATEX *)pdw;

                if ((!ppbWaveData || *ppbWaveData) &&
                    (!pcbWaveSize || *pcbWaveSize))
                {
                    return TRUE;
                }
            }
            break;

        case mmioFOURCC('d', 'a', 't', 'a'):
            if ((ppbWaveData && !*ppbWaveData) ||
                (pcbWaveSize && !*pcbWaveSize))
            {
                if (ppbWaveData)
                    *ppbWaveData = (LPBYTE)pdw;

                if (pcbWaveSize)
                    *pcbWaveSize = dwLength;

                if (!ppWaveHeader || *ppWaveHeader)
                    return TRUE;
            }
            break;
        }

        pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
    }

exit:
    return FALSE;
}

static void * DSGetWave( char *lpName , WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
	long			File_Size;
	long			Read_Size;
	void		*	Buffer;
	void *End;
	
	File_Size = Get_File_Size( lpName );	// how big is the file...
	if( !File_Size ) return NULL;
	Buffer = malloc( File_Size );							// alloc enough space to load it...
	if( Buffer == NULL ) return( NULL );					// if couldnt then return
	Read_Size = Read_File( lpName, Buffer, File_Size ); // Read it in making a note of the Size returned
	if( Read_Size != File_Size ) return( NULL );			// if size read doesnt qual file size return

	DSParseWave( Buffer , ppWaveHeader, ppbWaveData, pcbWaveSize, &End);

	return Buffer;
}

static BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
    if (pDSB && pbWaveData && cbWaveSize)
    {
        LPVOID pMem1, pMem2;
        DWORD dwSize1, dwSize2;

        if (SUCCEEDED(IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
            &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
        {
            CopyMemory(pMem1, pbWaveData, dwSize1);

            if ( 0 != dwSize2 )
                CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);

            IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
            return TRUE;
        }
    }

    return FALSE;
}

#endif // SOUND_DSOUND
