/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 7 $
 *
 * $Header: /PcProjectX/Include/SBufferHand.h 7     28/02/98 16:18 Philipy $
 *
 * $Log: /PcProjectX/Include/SBufferHand.h $
 * 
 * 7     28/02/98 16:18 Philipy
 * fixed memory leak
 * sfx loader does not now try to load sfx into hw if it knows that there
 * are insufficient resources
 * added buffer valid check b4 modifying looping sfx
 * 
 * 6     12/02/98 14:16 Philipy
 * 
 * 5     12/02/98 11:27 Philipy
 * releasing buffer now sets pointer to NULL to try and trap error
 * 
 * 4     11/02/98 12:57 Philipy
 * sbufferhand now gives call stack to 2 levels
 * 
 * 3     10/02/98 9:39 Philipy
 * fixed release build bug
 * 
 * 2     9/02/98 12:21 Philipy
 * added sound buffer memory managment
 * only one piece of bike computer speech can now play at a time
 * 
 * 1     5/02/98 17:32 Philipy
 * 
 * 
 */

/*==========================================================================
 *
 *
 *  File:       SBufferHand.h
 *  Content:    sound buffer handle include file
 *
 *
 ***************************************************************************/

#ifndef SBUFFERHAND_INCLUDED
#define SBUFFERHAND_INCLUDED

#include <dsound.h>

#ifdef DEBUG_ON

#define MakeSoundBuffer( A, B, C, D )	XMakeSoundBuffer( (A), (B), (C), (D), __FILE__, __LINE__, from_file, from_line )
#define SoundBufferRelease( A )	XSoundBufferRelease( (A), __FILE__, __LINE__ )
#define SoundBufferDuplicate( A, B, C )	XSoundBufferDuplicate( (A), (B), (C), __FILE__, __LINE__ )

/*
 * fn prototypes
 */
void XSBuffer_Init( void );
BOOL XMakeSoundBuffer( LPDIRECTSOUND pds, LPCDSBUFFERDESC lpcDSBufferDesc , LPLPDIRECTSOUNDBUFFER lplpDirectSoundBuffer , IUnknown FAR * pUnkOuter, char *in_file, int in_line, char *from_file, int from_line );
BOOL XSoundBufferDuplicate( LPDIRECTSOUND pds, LPDIRECTSOUNDBUFFER lpDsbOriginal,  LPLPDIRECTSOUNDBUFFER lplpDsbDuplicate, char *in_file, int in_line );

void XSoundBufferRelease( LPDIRECTSOUNDBUFFER *Pnt, char *in_file, int in_line );
int UnMallocedSBufferBlocks( void );
BOOL CheckSBufferValid( LPDIRECTSOUNDBUFFER lpDirectSoundBuffer );

#else // DEBUG_ON

#define MakeSoundBuffer( A, B, C, D ) ( IDirectSound_CreateSoundBuffer( (A), (B), (C), (D)) == DS_OK )
#define SoundBufferDuplicate( A, B, C ) ( IDirectSound_DuplicateSoundBuffer( (A), (B), (C) ) == DS_OK )
#define SoundBufferRelease(x) {if (*x != NULL) {(*x)->lpVtbl->Release( *x ); *x = NULL;}}

BOOL CheckSBufferValid( LPDIRECTSOUNDBUFFER lpDirectSoundBuffer );

#endif // DEBUG_ON

#endif

