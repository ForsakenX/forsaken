#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#include "main.h"
#include "sfx.h"
#include "SDL.h"

//
// Globals
//

// this will eventually be removed but is required right now
BOOL Sound3D;

struct {
	int memory; // hw contiguous bytes of ram
	int buffers; // hw static mixing buffers
	int min_volume;
} sound_caps; // capabilities

//
// Generic Functions
//

BOOL sound_init( void );
void sound_destroy( void );
void sound_commit_any_pending( void );

//
// Listener
//

BOOL sound_listener_position( float x, float y, float z );
BOOL sound_listener_velocity( float x, float y, float z );
BOOL sound_listener_orientation( 
	float fx, float fy, float fz, // forward vector
	float ux, float uy, float uz  // up vector
);

//
// Buffers
//

typedef struct
{
        BOOL used;
        void* buffer;
        int SfxHolderIndex;
} SBUFFERLIST;

#define MAX_SYNCHRONOUS_DYNAMIC_SFX 16

SBUFFERLIST SBufferList[ MAX_SYNCHRONOUS_DYNAMIC_SFX ];

void sound_buffer_play( void * buffer );
void sound_buffer_play_looping( void * buffer );
BOOL sound_buffer_is_playing( void * buffer );
void sound_buffer_stop( void * buffer );
DWORD sound_buffer_size( void * buffer );
void sound_buffer_release( void * buffer );
void sound_buffer_3d_release( void * buffer );
void sound_buffer_set_freq( void* buffer, float freq );
BOOL sound_buffer_duplicate( void * buffer, void** duplicate );
void sound_buffer_volume( void * buffer, long volume );
void sound_buffer_pan( void * buffer, long pan );
DWORD sound_buffer_get_freq( void * buffer ); // samples per second
DWORD sound_buffer_get_rate( void * buffer ); // avg bytes per second
void sound_buffer_get_position( void * buffer, DWORD* time );
void sound_buffer_set_position( void * buffer, DWORD time );
void sound_buffer_set_3d_position( void * buffer, float x, float y, float z, float min_distance, float max_distance );

// special buffer loader that uses proper setting for a single location
// this appears to be the only location that directly loads it's own buffers
// all other buffers are being loaded by sound_source_create
void* sound_buffer_load_for_sfx(char* file);

//
// Sources
//

#define	MAX_DUP_BUFFERS	4 // max num occurances of any one sfx

typedef struct
{
	void*			Dup_Buffer[MAX_DUP_BUFFERS];
	void*			Dup_3DBuffer[MAX_DUP_BUFFERS];
	BOOL			CompoundBuffer;	// is sound part of compound buffer?
	DWORD			StartPos;		// start offset in buffer...
	unsigned int	Length;			// length of sample (ms)...
	int				CompoundBufferLookup[MAX_DUP_BUFFERS];
	float			Buffer_Dist[MAX_DUP_BUFFERS];
	Uint32			Buffer_TimeStamp[MAX_DUP_BUFFERS];
	DWORD			TransferRate;	// bytes per second
	DWORD			Bytes;
	int				looping_sfx_index[MAX_DUP_BUFFERS];
	int				SfxHolderIndex[MAX_DUP_BUFFERS];
} sound_source_t;

void sound_source_destroy( void * source );
sound_source_t *sound_source_create(char *path, int sfx_flags, int sfx);

#endif
