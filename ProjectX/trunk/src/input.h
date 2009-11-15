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

#define MAX_KEY_BOARD_BUFFER 100
SDL_keysym keyboard_buffer[MAX_KEY_BOARD_BUFFER];
keyboard_buffer_count;

int input_grabbed;
void input_grab( BOOL grab );

BOOL handle_events( void );

#define INPUT_BUFFERS		(2)

mouse_state_t mouse_states[ INPUT_BUFFERS ];

#define MOUSE_BUTTON_HELD( B )    ( mouse_states[ new_input ].buttons[ B ] )
#define MOUSE_BUTTON_PRESSED( B ) ( !( mouse_states[ old_input ].buttons[ B ] ) && ( mouse_states[ new_input ].buttons[ B ] ) )
#define MOUSE_BUTTON_RELEASED( B )  ( ( mouse_states[ old_input ].buttons[ B ] ) && !( mouse_states[ new_input ].buttons[ B ] ) )

#define MOUSE_WHEEL_UP()				( mouse_states[ new_input ].wheel > 0 )
#define MOUSE_WHEEL_DOWN()				( mouse_states[ new_input ].wheel < 0 )
#define MOUSE_WHEEL_UP_PRESSED()		( !( mouse_states[ old_input ].wheel > 0 ) && ( mouse_states[ new_input ].wheel > 0 ) )
#define MOUSE_WHEEL_DOWN_PRESSED()      ( !( mouse_states[ old_input ].wheel < 0 ) && ( mouse_states[ new_input ].wheel < 0 ) )

#endif