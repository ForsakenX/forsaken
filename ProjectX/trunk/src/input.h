#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#ifdef DINPUTJOY
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>
#endif

#include "main.h"
#include <SDL.h>
#include "controls.h"

/////////////
// Buffers //
/////////////

#define INPUT_BUFFERS (2)

int old_input;
int new_input;

////////////
// Events //
////////////

BOOL input_grabbed;
void input_grab( _Bool grab );

_Bool handle_events( void );

//////////////////
// Input Buffer //
//////////////////

// stores buffered input events
// mouse_input_enum if (code > SDL_LAST && < DIK_JOYSTICK)
// joystick event if code > DIK_JOYSTICK

// TODO - probably ok to rewrite this to simply have codes

#define MAX_INPUT_BUFFER 100
int input_buffer[MAX_INPUT_BUFFER];
int input_buffer_count;
int input_buffer_find( int code );
void input_buffer_reset( void );

///////////
// Mouse //
///////////

#define MAX_MOUSE_BUTTONS (255)

typedef struct {

	// wheel state -1 (down) 0 (nothing) 1 (up)
	int wheel;

	// left (0) , middle (1) , right (2)
	int buttons[MAX_MOUSE_BUTTONS];

	// relative mouse movement
	int xrel;
	int yrel;

} mouse_state_t;

// this holds the current state each loop
mouse_state_t mouse_state;

// special keydefs for mouse actions
// this is only used by MenuProcess
enum {
	MOUSE_RANGE = SDLK_LAST,
	LEFT_MOUSE,
	MIDDLE_MOUSE,
	RIGHT_MOUSE,
	UP_MOUSE,
	DOWN_MOUSE
} mouse_input_enum;

#define MOUSE_RANGE_END LEFT_MOUSE + MAX_MOUSE_BUTTONS

mouse_state_t mouse_states[ INPUT_BUFFERS ];

#define MOUSE_BUTTON_HELD( B )    ( mouse_states[ new_input ].buttons[ B ] )
#define MOUSE_BUTTON_PRESSED( B ) ( !( mouse_states[ old_input ].buttons[ B ] ) && ( mouse_states[ new_input ].buttons[ B ] ) )
#define MOUSE_BUTTON_RELEASED( B )  ( ( mouse_states[ old_input ].buttons[ B ] ) && !( mouse_states[ new_input ].buttons[ B ] ) )

#define MOUSE_WHEEL_UP()				( mouse_states[ new_input ].wheel > 0 )
#define MOUSE_WHEEL_DOWN()				( mouse_states[ new_input ].wheel < 0 )
#define MOUSE_WHEEL_UP_PRESSED()		( !( mouse_states[ old_input ].wheel > 0 ) && ( mouse_states[ new_input ].wheel > 0 ) )
#define MOUSE_WHEEL_DOWN_PRESSED()      ( !( mouse_states[ old_input ].wheel < 0 ) && ( mouse_states[ new_input ].wheel < 0 ) )

mouse_state_t* read_mouse(void);

///////////////
// Joysticks //
///////////////

#define MAX_JOYSTICKS			16
#define MAX_JOYSTICK_BUTTONS	128
#define MAX_JOYSTICK_POVS		4
#define MAX_JOYSTICK_AXIS		8
#define MAX_JOYSTICK_TEXT		128
#define MAX_JOYNAME				16

extern int Num_Joysticks;

typedef struct {
	_Bool assigned;
	_Bool connected;
#ifndef DINPUTJOY
	SDL_Joystick * sdl_joy;
#endif
	char *Name;
	int NumButtons;
	JOYSTICKBTN Button[MAX_JOYSTICK_BUTTONS];
	int NumPOVs;
	JOYSTICKPOV POV[MAX_JOYSTICK_POVS];
	int NumAxis;
	JOYSTICKAXIS Axis[MAX_JOYSTICK_AXIS];
} JOYSTICKINFO;

JOYSTICKINFO JoystickInfo[MAX_JOYSTICKS];

_Bool joysticks_init(void);
_Bool joysticks_cleanup( void );

// this holds the current state joysticks
long joy_axis_state[ MAX_JOYSTICKS ][ MAX_JOYSTICK_AXIS ];
_Bool joy_button_state[ MAX_JOYSTICKS ][ MAX_JOYSTICK_BUTTONS ];
// hat has 4 directions, diagnols turn on both adjecent angles
u_int8_t joy_hat_state[ MAX_JOYSTICKS ][ MAX_JOYSTICK_POVS ][ MAX_POV_DIRECTIONS ];

enum {
	JOY_HAT_UP,
	JOY_HAT_RIGHT,
	JOY_HAT_DOWN,
	JOY_HAT_LEFT
} joy_hat_enum;

// TODO - we need settings/implementation for joystick dead zones

#endif
