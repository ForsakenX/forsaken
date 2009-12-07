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

void sound_play( void * buffer );
void sound_play_looping( void * buffer );
BOOL sound_is_playing( void * buffer );
void sound_stop( void * buffer );
long sound_size( void * buffer );
void sound_release( void * buffer );
void sound_3d_release( void * buffer );
void sound_set_freq( void* buffer, float freq );
void sound_volume( void * buffer, long volume );
void sound_pan( void * buffer, long pan );
long sound_get_rate( void * buffer );
void sound_get_seek( void * buffer, long * time );
void sound_set_seek( void * buffer, long time );
void sound_position( void * buffer, float x, float y, float z, float min_distance, float max_distance );
void* sound_load(char* file);
BOOL sound_duplicate( void * source, void ** destination );

#endif
