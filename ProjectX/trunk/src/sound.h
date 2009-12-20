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
typedef IDirectSoundBuffer sound_t;
#else
typedef struct sound_t sound_t;
#endif

sound_t * sound_load(char* file);
BOOL sound_duplicate( sound_t * s, sound_t ** destination );

void sound_play( sound_t * s );
void sound_play_looping( sound_t * s );
BOOL sound_is_playing( sound_t * s );
void sound_stop( sound_t * s );
long sound_size( sound_t * s );
void sound_release( sound_t * s );
void sound_set_pitch( sound_t * s, float freq );
void sound_volume( sound_t * s, long decibels );
void sound_pan( sound_t * s, long pan );
long sound_bps( sound_t * s );
long sound_get_seek( sound_t * s );
void sound_set_seek( sound_t * s, long bytes );
void sound_position( sound_t * s, float x, float y, float z, float min_distance, float max_distance );

#endif
