 
/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       dsutil.cpp
 *  Content:    Routines for dealing with sounds from resources
 *
 *
 ***************************************************************************/
#ifndef	DSUTIL_INCLUDED
#define	DSUTIL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "new3d.h"
#include "sbufferhand.h"
///////////////////////////////////////////////////////////////////////////////
//
// DSLoadSoundBuffer    Loads an IDirectSoundBuffer from a Win32 resource in
//                      the current application.
//
// Params:
//  pDS         -- Pointer to an IDirectSound that will be used to create
//                 the buffer.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
// Returns an IDirectSoundBuffer containing the wave data or NULL on error.
//
// example:
//  in the application's resource script (.RC file)
//      Turtle WAV turtle.wav
//
//  some code in the application:
//      IDirectSoundBuffer *pDSB = DSLoadSoundBuffer(pDS, "Turtle");
//
//      if (pDSB)
//      {
//          IDirectSoundBuffer_Play(pDSB, 0, 0, DSBPLAY_TOEND);
//          /* ... */
//
///////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_ON
IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, char *lpName , DWORD dwFlags, char *from_file, int from_line );
IDirectSoundBuffer *DSLoadCompoundSoundBuffer(IDirectSound *pDS, DWORD dwFlags, int *num_allocated_ptr, char *from_file, int from_line );
#else
IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, char *lpName , DWORD dwFlags );
IDirectSoundBuffer *DSLoadCompoundSoundBuffer(IDirectSound *pDS, DWORD dwFlags, int *num_allocated_ptr );
#endif

///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer  Reloads an IDirectSoundBuffer from a Win32 resource in
//                      the current application. normally used to handle
//                      a DSERR_BUFFERLOST error.
// Params:
//  pDSB        -- Pointer to an IDirectSoundBuffer to be reloaded.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
// Returns a BOOL indicating whether the buffer was successfully reloaded.
//
// example:
//  in the application's resource script (.RC file)
//      Turtle WAV turtle.wav
//
//  some code in the application:
//  TryAgain:
//      HRESULT hres = IDirectSoundBuffer_Play(pDSB, 0, 0, DSBPLAY_TOEND);
//
//      if (FAILED(hres))
//      {
//          if ((hres == DSERR_BUFFERLOST) &&
//              DSReloadSoundBuffer(pDSB, "Turtle"))
//          {
//              goto TryAgain;
//          }
//          /* deal with other errors... */
//      }
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, char *lpName);

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource    Finds a WAV resource in a Win32 module.
//
// Params:
//  hModule     -- Win32 module handle of module containing WAV resource.
//                 Pass NULL to indicate current application.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
//  ppWaveHeader-- Optional pointer to WAVEFORMATEX * to receive a pointer to
//                 the WAVEFORMATEX header in the specified WAV resource.
//                 Pass NULL if not required.
//
//  ppbWaveData -- Optional pointer to BYTE * to receive a pointer to the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
//  pdwWaveSize -- Optional pointer to DWORD to receive the size of the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
// Returns a BOOL indicating whether a valid WAV resource was found.
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);

void * DSGetWave( char *lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize);

void * DSGetMultiWave( WAVEFORMATEX *pWaveHeaderStore, BYTE **ppbWaveData, DWORD *pcbWaveSize, DWORD dwFlags, int *num_allocated_ptr );


///////////////////////////////////////////////////////////////////////////////
//
// HSNDOBJ             Handle to a SNDOBJ object.
//
//  SNDOBJs are implemented in dsutil as an example layer built on top
//      of DirectSound.
//
//      A SNDOBJ is generally used to manage individual
//      sounds which need to be played multiple times concurrently.  A
//      SNDOBJ represents a queue of IDirectSoundBuffer objects which
//      all refer to the same buffer memory.
//
//      A SNDOBJ also automatically reloads the sound resource when
//      DirectSound returns a DSERR_BUFFERLOST
//
///////////////////////////////////////////////////////////////////////////////

typedef struct SFXNAME{
	char		*Name;			// Name of the Sfx...
	int			Flags;
	int			Priority;			// for compound sfx
	int			SfxLookup;			// for biker / computer speech
}SFXNAME;

#define SFX_Dynamic			1	// sound is loaded up as required
#define SFX_BikeComp		2	// bike computer speech 
#define SFX_Biker			4	// biker speech
#define SFX_LevelSpec		8	// level specific sfx
#define SFX_Looping			16	// level specific sfx
#define SFX_BikerSpeechOveride	32	// use when biker speech must play ( will cut off any existing speech )
#define SFX_Title	64	// use when biker speech must play ( will cut off any existing speech )
#define SFX_End	128	// end of sfx batch
#define SFX_InGame	256	// sfx must be loaded in game, regardless of whether or not SFX_Title is set
#define SFX_BikeCompNoOveride	512

#define SFX_HOLDERTYPE_Static 0
#define SFX_HOLDERTYPE_Compound 1
#define SFX_HOLDERTYPE_Dynamic 2
#define SFX_HOLDERTYPE_Looping 3
#define SFX_HOLDERTYPE_Taunt 4

#define IS_COMPOUND( flags ) ( (!(flags & SFX_Looping)) && (!(flags & SFX_Dynamic)))

#define SFX_BUFFER_HW 512	// use 9th bit of DWORD to indicate HW buffer
#define SFX_BUFFER_SW 1024	// use 10th bit of DWORD to indicate SW buffer

#define	MAX_DUP_BUFFERS	4 // max num occurances of any one sfx
#define MAX_COMPOUND_SFX 256 // max number of individual sfx that can be stored in a compound buffer
#define MAX_COMPOUND_BUFFERS 16	// max number of mixing channels
#define MIN_COMPOUND_BUFFERS 8	// min number of mixing channels ( otherwise sw mixing will be used )

#define NUM_SFX_DETAIL_LEVELS 2

typedef struct
{
	int SfxNum;
	int Variant;
	DWORD StartPos;
	UINT Length;
	DWORD Bytes;
} TEMPSFXINFO;

typedef struct _SPOT_SFX_LIST
{
	//struct					_SPOT_SFX_LIST *next;	// next list item
	//struct					_SPOT_SFX_LIST *prev;	// prev list item
	BOOL					used;					// is sfx in use?
	int16					sfxindex;					// sfx num, from enum list
	int						variant;					// sfx num, from enum list
	int						flags;
	VECTOR					*pos;					// current sfx position vector
	VECTOR					fixedpos;
	int						type;					// fixed or variable group?
	uint16					*group;					// current sfx group num
	uint16					fixedgroup;				// current sfx group num
	float					freq;					// frequency ( 0 for original frequency )
	float					vol;					// vol ( 0 = zero volume, 1 = full volume )
	BOOL					bufferloaded;			// flag to indicate if buffer is loaded ( or about to be loaded )
	IDirectSoundBuffer		*buffer;				// buffer address
	IDirectSound3DBuffer	*buffer3D;				// 3D buffer interface address
	DWORD					buffersize;
	float					distance;
	int						SfxHolderIndex;
	int						SfxThreadInfoIndex;
	uint16					Effects;
	uint32					uid;
} SPOT_SFX_LIST;

#define MAX_LOOPING_SFX 64

typedef struct _SNDOBJ
{
	IDirectSoundBuffer		*Dup_Buffer[MAX_DUP_BUFFERS];
	IDirectSound3DBuffer	*Dup_3DBuffer[MAX_DUP_BUFFERS];
	BOOL					CompoundBuffer;	// is sound part of compound buffer?
	DWORD					StartPos;			// start offset in buffer...
	UINT					Length;			// length of sample (ms)...
	int						CompoundBufferLookup[MAX_DUP_BUFFERS];
	float					Buffer_Dist[MAX_DUP_BUFFERS];
	clock_t					Buffer_TimeStamp[MAX_DUP_BUFFERS];
	DWORD					TransferRate;	// bytes per second
	DWORD					Bytes;
	int						looping_sfx_index[MAX_DUP_BUFFERS];
	int						SfxHolderIndex[MAX_DUP_BUFFERS];
} SNDOBJ;

typedef struct _GENERIC_SFX_INF0
{
	int					SfxNum;					// which sfx?
	BOOL				Retain;					// retain info for this sound until explicitly deallocated?
	float				Dist;					// distance of sound ( used for 2D sounds only )
	uint16				*Group;					// address of group no. of sound
	VECTOR				*pos;					// address of position vector of sound
	DWORD				Freq;					// freq of sound ( 0 for original freq )
	float				Volume;					// sound volume ( 0 = nothing, 1 = Max )
} GENERIC_SFX_INFO;

typedef struct _GENERIC_SFX
{
	GENERIC_SFX_INFO	*Info;					
	BOOL				Allocated;				// is this slot occupied?
	int					Flags;					// flags ( as copied from original list )
	int					BufferIndex;			// index no. of sound buffer
	IDirectSoundBuffer	*Buffer;				// buffer address
} GENERIC_SFX;

#define _HSNDOBJ_DEFINED

///////////////////////////////////////////////////////////////////////////////
//
// SndObjCreate     Loads a SNDOBJ from a Win32 resource in
//                  the current application.
//
// Params:
//  pDS         -- Pointer to an IDirectSound that will be used to create
//                 the SNDOBJ.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
//  iConcurrent -- Integer representing the number of concurrent playbacks of
//                 to plan for.  Attempts to play more than this number will
//                 succeed but will restart the least recently played buffer
//                 even if it is not finished playing yet.
//
// Returns an HSNDOBJ or NULL on error.
//
// NOTES:
//      SNDOBJs automatically restore and reload themselves as required.
//
///////////////////////////////////////////////////////////////////////////////
SNDOBJ *SndObjCreate(IDirectSound *pDS, char *lpName, int iConcurrent , DWORD dwFlags, int sfx);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjDestroy  Frees a SNDOBJ and releases all of its buffers.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to free.
//
///////////////////////////////////////////////////////////////////////////////
void SndObjDestroy(SNDOBJ *hSO);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjPlay   Plays a buffer in a SNDOBJ.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to play a buffer from.
//
//  dwPlayFlags -- Flags to pass to IDirectSoundBuffer::Play.  It is not
//                 legal to play an SndObj which has more than one buffer
//                 with the DSBPLAY_LOOPING flag.  Pass 0 to stop playback.
//
///////////////////////////////////////////////////////////////////////////////
BOOL SndObjPlay(SNDOBJ *pSO, DWORD dwPlayFlags , long Volume);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjStop   Stops one or more buffers in a SNDOBJ.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to play a buffer from.
//
///////////////////////////////////////////////////////////////////////////////
BOOL SndObjStop(SNDOBJ *hSO);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjGetFreeBuffer      returns one of the cloned buffers that is
//                          not currently playing
//
// Params:
//  hSO         -- Handle to a SNDOBJ
//
// NOTES:
//  This function is provided so that callers can set things like pan etc
//  before playing the buffer.
//
// EXAMPLE:
//  ...
//
///////////////////////////////////////////////////////////////////////////////
IDirectSoundBuffer *SndObjGetFreeBuffer(SNDOBJ *hSO , IDirectSound3DBuffer **pDSB3D );

///////////////////////////////////////////////////////////////////////////////
//
// helper routines
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD dwWaveSize);
BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);

BOOL DSParseWave(void *Buffer, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize, void **End);

IDirectSoundBuffer *DSLoad3DSoundBuffer(IDirectSound *pDS, char *lpName, IDirectSound3DBuffer ** lpDirectSound3DBuffer, DWORD dwflags );
void GetSfxPath( int sfxnum, char *path );

#ifdef __cplusplus
}
#endif

#endif // ifndef DSUTIL_INCLUDED
