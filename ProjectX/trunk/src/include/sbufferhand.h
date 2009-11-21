

/*==========================================================================
 *
 *
 *  File:       sbufferhand.h
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

