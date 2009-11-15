#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#define DIRECTINPUT_VERSION 0x0700

#include "SDL.h"
#include "dinput.h"
#include "controls.h"

#define MAX_MOUSE_BUTTONS (3)

typedef struct {

	// wheel state -1 (down) 0 (nothing) 1 (up)
	int wheel;

	// left (0) , middle (1) , right (2)
	int buttons[MAX_MOUSE_BUTTONS];

	// relative mouse location
	int xrel; 
	int yrel;

} mouse_state_t;

// this holds the current state each loop
mouse_state_t mouse_state;

int input_grabbed;
void input_grab( BOOL grab );

BOOL handle_events( void );

#endif