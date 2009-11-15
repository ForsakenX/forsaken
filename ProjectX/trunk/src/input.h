#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#define DIRECTINPUT_VERSION 0x0700

#include "dinput.h"
#include "controls.h"

typedef struct {

	// wheel state -1 (down) 0 (nothing) 1 (up)
	int wheel;

	// left (0) , middle (1) , right (2)
	int buttons[3];

	// relative mouse location
	int xrel; 
	int yrel;

	// absolute mouse location
	//int x;
	//int y;

} mouse_state_t;

// this holds the current state each loop
mouse_state_t mouse_state;

int input_grabbed;
void input_grab( BOOL grab );

BOOL handle_events( void );

#endif