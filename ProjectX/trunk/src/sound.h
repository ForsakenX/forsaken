#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#include "main.h"
#include "SDL.h"

//
// Globals
//

// this will eventually be removed but is required right now
BOOL Sound3D;

// the game appears to need this probably wont in openAL
int sound_minimum_volume;

//
// Generic Functions
//

BOOL sound_init( void );
void sound_destroy( void );

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

#ifdef SOUND_DSOUND
#include <dsound.h>
typedef IDirectSoundBuffer sound_source_t;
#else
typedef struct sound_source_t sound_source_t;
typedef struct sound_buffer_t sound_buffer_t;
#endif

sound_buffer_t * sound_load(char* file);
sound_source_t * sound_source( sound_buffer_t * buffer );

void sound_play( sound_source_t * s );
void sound_play_looping( sound_source_t * s );
BOOL sound_is_playing( sound_source_t * s );
void sound_stop( sound_source_t * s );
void sound_release_source( sound_source_t * s );
void sound_release_buffer( sound_buffer_t * s );
void sound_set_pitch( sound_source_t * s, float freq );
void sound_volume( sound_source_t * s, long decibels );
void sound_pan( sound_source_t * s, long pan );
long sound_bps( sound_source_t * s );
long sound_get_seek( sound_source_t * s );
void sound_set_seek( sound_source_t * s, long bytes );
void sound_position( sound_source_t * s, float x, float y, float z, float min_distance, float max_distance );

#endif
