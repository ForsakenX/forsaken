#include "input.h"
#include "util.h"
#include "render.h"
#include "SDL.h"

extern BOOL RenderModeReset( void );
extern void SetGamePrefs( void );
extern void FadeHoloLight(float Brightness);
extern float HoloLightBrightness;
extern float RoomDarkness;
extern void DarkenRoom2(float darkness);
extern void ProcessVduItems( MENU * Menu );
extern BOOL InitialTexturesSet;
extern BOOL cursor_clipped;
extern BYTE MyGameStatus;
extern BOOL QuitRequested;
extern render_info_t render_info;
extern void CleanUpAndPostQuit(void);

void input_grab( BOOL grab )
{
	// always acquire and hide mouse if in fullscreen
	if( render_info.fullscreen )
	{
		input_grabbed = TRUE;
		SDL_WM_GrabInput( TRUE );
		SDL_ShowCursor( FALSE );
		return;
	}
	// window mode
	input_grabbed = grab;
	SDL_WM_GrabInput( grab==1 ? SDL_GRAB_ON : SDL_GRAB_OFF );
	SDL_ShowCursor( grab==1 ? SDL_DISABLE : SDL_ENABLE );
	DebugPrintf("input state: %s\n",(grab==1?"grabbed":"free"));
}

//
// Window/Input Events
//

void app_active( SDL_ActiveEvent active )
{
	DebugPrintf("window active state set to: %s\n",(active.gain?"true":"false"));

	// lost focus so release inputs
	if( ! active.gain )
		input_grab( FALSE );

	// gained focus
	else
	{
		// fullscreen always has exclusive inputs
		if( render_info.fullscreen )
			input_grab(TRUE);

		// window mode
		else
		{
			// only grab inputs if we are playing
			// TODO - this should not grab the mouse if we are in the menu !
			if( MyGameStatus == STATUS_Normal )
				input_grab(TRUE);
			else
				input_grab(FALSE);
		}
	}

	if(active.gain)
		RenderModeReset();

	switch( active.state )
	{
	case SDL_APPMOUSEFOCUS: // mouse
		DebugPrintf("Mouse event\n");
		break;
	case SDL_APPINPUTFOCUS: // keyboard
		DebugPrintf("keyboard event\n");
		break;
	case SDL_APPACTIVE: // minimize/iconified
		DebugPrintf("Iconify event\n");
		break;
	}
}

// TODO
//	when this is ready then pass SDL_RESIZABLE to SDL_SetVideoMode
//  need to resize video with SDL_SetVideoMode
BOOL bIgnoreWM_SIZE = FALSE; // ignores resize events
void app_resize( SDL_ResizeEvent resize )
{
	DebugPrintf("Window size changed.\n");

	// TODO - need to save resize.w/h somewhere

	if (!bIgnoreWM_SIZE)
		return;

	// we should save the size
	// then startup at this size next time

	if( MyGameStatus == STATUS_Title )
	{
		LastMenu = CurrentMenu;	
		VduClear();
	}

	// resize d3d to match the window size..
	// TODO does d3d do the resizing on it's own now?

	if( MyGameStatus == STATUS_Title )
	{
		FadeHoloLight(HoloLightBrightness);
		DarkenRoom2(RoomDarkness);
		ProcessVduItems( CurrentMenu );
		InitialTexturesSet = FALSE;
	}

	SetGamePrefs();
}

void app_quit( void )
{
	// in case the window isn't in the foreground
	// we'd end up with a huge black spot from being previously covered
	render_flip(&render_info);

	// release mouse so they can interact with message box
	input_grab( FALSE );

	// ask them to confirm clossing
	if(Msg("Are you sure you want to exit?") )
	{
		// user wants to quit
		// let our code know we're quitting and not failing
		// let the message reach DefWindowProc so it calls CloseWindow
		CleanUpAndPostQuit();
		return;
	}

	// let them click to get focus again
	if( ! render_info.fullscreen )
		input_grab( TRUE );
}

// TODO - how do i know if the key is pressed/released or repeating?
// TODO - may need to enable keyboard repeat
//	int SDL_EnableKeyRepeat(int delay, int interval);
//	SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL
//
// NOTE - 
//	SDLK_CAPSLOCK and SDLK_NUMLOCK will never repeat cause of sun workstation compatbility
//	edit lines 403 and 449 of src/events if you want to change sdl behavior


void app_keyboard( SDL_KeyboardEvent key )
{	
	// TODO - use unicode characters
	// int SDL_EnableUNICODE(int enable);

	// A lot of the keysyms are unavailable on most keyboards.
	// So, you should not hardcode any keysym unless it's one 
	// of the universal keys that are available on all keyboards.

	/*
	// If the high 9 bits of the character are 0, then this maps
	// to the equivalent ASCII character.
	char ch;
	if ( (keysym.unicode & 0xFF80) == 0 ) {
	  ch = keysym.unicode & 0x7F;
	}
	else {
	  printf("An International Character.\n");
	}
	*/

	/*
	key.keysym
		typedef struct{
		  Uint8 scancode;	// Hardware specific scancode
		  SDLKey sym;		// SDL virtual keysym
		  SDLMod mod;		// Current key modifiers
		  Uint16 unicode;	// Translated character
		} SDL_keysym;
	*/

	/* mod
		KMOD_NONE 		No modifiers applicable
		KMOD_LSHIFT 	Left Shift is down
		KMOD_RSHIFT		Right Shift is down
		KMOD_LCTRL		Left Control is down
		KMOD_RCTRL		Right Control is down
		KMOD_LALT		Left Alt is down
		KMOD_RALT		Right Alt is down
		KMOD_LMETA		Left Meta is down
		KMOD_RMETA		Right Meta is down
		KMOD_NUM		Numlock is down
		KMOD_CAPS		Capslock is down
		KMOD_MODE		Mode is down
		KMOD_CTRL		A Control key is down
		KMOD_SHIFT		A Shift key is down
		KMOD_ALT		An Alt key is down
		KMOD_META		A Meta key is down
	*/

	switch( key.keysym.sym )
	{
	case SDLK_F12:
		if( key.keysym.mod & KMOD_SHIFT )
			MenuGoFullScreen( NULL );
		break;
	}
	
	// TODO - we'll probably want to rewrite code that depends on this to use the new
	//        methods that use SDL_GetKeyState and provide functions such as "is key held" etc..
	// TODO - we could probably have mouse events added here as keyboard events
	//			to emulate mouse menu navigation... ex: right click maps to escape
	if( key.type == SDL_KEYUP )
		if( keyboard_buffer_count < MAX_KEY_BOARD_BUFFER )
			keyboard_buffer[ keyboard_buffer_count++ ] = key.keysym;
}

void reset_keyboard_buffer( void )
{
	keyboard_buffer_count = 0;
}

int buffered_key_released( SDLKey key )
{
	int i;
	for ( i = 0 ; i < keyboard_buffer_count; i++ )
		if( keyboard_buffer[ i ].sym == key )
			return 1;
	return 0;
}

// mouse wheel button down/up are sent at same time
// so if we react to the up event then we undo the down event !
// so we must ignore up events and reset the value each game loop
// since a wheel event can never be held down this is ok...

void mouse_wheel_up( void )
{
	//DebugPrintf("mouse wheel up pressed\n");
	mouse_state.wheel = 1;
}

void mouse_wheel_down( void )
{
	//DebugPrintf("mouse wheel down pressed\n");
	mouse_state.wheel = -1;
}

void reset_mouse_wheel( void )
{
	//DebugPrintf("mouse wheel state reset\n");
	mouse_state.wheel = 0;
}

//
// mouse button events
//

void app_mouse_button( SDL_MouseButtonEvent _event )
{
	/*
	typedef struct{
	  Uint8 type;
	  Uint8 which;	// The input device index
	  Uint8 button;	// The mouse button index (SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN)
	  Uint8 state;
	  Uint16 x, y;
	} SDL_MouseButtonEvent;
	*/

	// pass down mouse events for menu processing
	if(  _event.type == SDL_MOUSEBUTTONDOWN )
	{
		if( keyboard_buffer_count < MAX_KEY_BOARD_BUFFER )
		{
			SDL_keysym key;
			memset(&key,0,sizeof(SDL_keysym));
			key.sym = _event.button - 1 + LEFT_MOUSE;
			keyboard_buffer[ keyboard_buffer_count++ ] = key;
		}
	}

	switch( _event.button )
	{

	//
	// TODO - handle standard mouse buttons
	//

	case SDL_BUTTON_LEFT:	// 1
	case SDL_BUTTON_MIDDLE:	// 2
	case SDL_BUTTON_RIGHT:	// 3
		// save the button state
		mouse_state.buttons[ _event.button - 1 ] = ( _event.type == SDL_MOUSEBUTTONDOWN );
		//DebugPrintf("sdl mouse button %d %s\n",_event.button,
		//	(mouse_state.buttons[ _event.button - 1 ]?"pressed":"released"));
		break;

	// mouse wheel button down/up are sent at same time
	// so if we react to the up event then we undo the down event !
	// so we must ignore up events and reset it further bellow manually
	// since a wheel event can never be held down this is ok...

	case SDL_BUTTON_WHEELUP:
		if( _event.type == SDL_MOUSEBUTTONDOWN )
			mouse_wheel_up();
		break;
	case SDL_BUTTON_WHEELDOWN:
		if( _event.type == SDL_MOUSEBUTTONDOWN )
			mouse_wheel_down();
		break;

	//
	// TODO - handle non standard mouse buttons
	//

	default: 
		break;
	}
}

//
// mouse movement events
//
	/*
	typedef struct{
	  Uint8 type;
	  Uint8 state;
	  Uint16 x, y;
	  Sint16 xrel, yrel;
	} SDL_MouseMotionEvent;
	*/

void app_mouse_motion( SDL_MouseMotionEvent motion )
{
	mouse_state.xrel = motion.xrel;
	mouse_state.yrel = motion.yrel;
	// for now we only support relative motion
	//mouse_state.x = motion.x;
	//mouse_state.y = motion.y;
}

// the motion event only goes off if we move the mouse
// hence we need to clear out the old values each loop
// other wise absence of movement will cause old value to stick around
// and the player will continue to move by the last motion
void reset_mouse_motion( void )
{
	mouse_state.xrel = 0;
	mouse_state.yrel = 0;
	//mouse_state.x = 0;
	//mouse_state.y = 0;
}

void app_joy_axis( SDL_JoyAxisEvent axis )
{
	// sdl axis value (range: -32768 to 32767)
	// dinput axis value (range: -100 to 100)
	float value = (float) axis.value;
	value = (value / 32767.0f) * 100.0f;
	joy_state[ axis.which ] = (long) value;
}

void app_joy_ball( SDL_JoyBallEvent ball )
{
}

void app_joy_button( SDL_JoyButtonEvent button )
{
}

void app_joy_hat( SDL_JoyHatEvent hat )
{
}

void reset_events( void )
{
	reset_mouse_motion();
	reset_mouse_wheel();
	reset_keyboard_buffer();
}

BOOL handle_events( void )
{
	SDL_Event _event;

	/*
	  SDL_MouseMotionEvent motion;
	  SDL_MouseButtonEvent button;
	  SDL_JoyAxisEvent jaxis;
	  SDL_JoyBallEvent jball;
	  SDL_JoyHatEvent jhat;
	  SDL_JoyButtonEvent jbutton;
	*/

	reset_events();

	//DebugPrintf("processing events\n");

	while( SDL_PollEvent( &_event ) )
	{
		switch( _event.type )
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			app_keyboard( _event.key );
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			app_mouse_button( _event.button );
			break;

		case SDL_MOUSEMOTION:
			app_mouse_motion( _event.motion );
			break;

		case SDL_JOYAXISMOTION:
			app_joy_axis( _event.jaxis );
			break;

		case SDL_JOYBALLMOTION:
			app_joy_ball( _event.jball );
			break;

		case SDL_JOYBUTTONDOWN:
			app_joy_button( _event.jbutton );
			break;

		case SDL_JOYHATMOTION:
			app_joy_hat( _event.jhat );
			break;

		case SDL_ACTIVEEVENT:
			app_active( _event.active );
			break;

		case SDL_VIDEORESIZE:
			app_resize( _event.resize );
			break;

		case SDL_VIDEOEXPOSE: // need redraw
            render_flip(&render_info);
			break;

		case SDL_QUIT:
			app_quit();
			break;

		// platform specific _event type
		// must be enabled using SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE)
		case SDL_SYSWMEVENT:
			DebugPrintf("recived a platform specific _event type\n");
			break;

		// to avoid threading issues timers will add an event to the queue
		// so that we can call the callbacks from the same thread
		case SDL_USEREVENT:
			{
				void (*p) (void*) = _event.user.data1; // callback
				p(_event.user.data2); // callback( data )
			}
			break;

		}
	}

	//DebugPrintf("DONE processing events\n");

	if ( quitting )
	{
		DebugPrintf("about to CleanUpAndPostQuit ( from WindowProc )\n");
		quitting = FALSE;
		CleanUpAndPostQuit();
	}

	return TRUE;
}

#ifndef DINPUTJOY
BOOL joysticks_init(void)
{
	int i, j, k;

	joysticks_cleanup();

	Num_Joysticks = SDL_NumJoysticks();

	DebugPrintf( "joysticks_init: %d joysticks connected\n", Num_Joysticks );

	if (Num_Joysticks > MAX_JOYSTICKS)
		Num_Joysticks = MAX_JOYSTICKS;

	for (i = 0; i < Num_Joysticks; i++)
	{
		SDL_Joystick * joy = SDL_JoystickOpen(i);

		// failed to open joystick
		if(!joy)
		{
			DebugPrintf(
				"joysticks_init: joystick (%d), '%s' failed to open\n",
				i, SDL_JoystickName(i)
			);
			continue;
		}

		// setup defaults
		JoystickInfo[i].sdl_joy 	= joy;
		JoystickInfo[i].assigned	= FALSE;
		JoystickInfo[i].connected	= TRUE;
		JoystickInfo[i].NumAxis		= SDL_JoystickNumAxes(joy);
		JoystickInfo[i].NumButtons	= SDL_JoystickNumButtons(joy);
		JoystickInfo[i].NumPOVs		= SDL_JoystickNumHats(joy);

		// TODO
		// JoystickInfo[i].NumBalls = SDL_JoystickNumBalls(joy);

		JoystickInfo[i].Name = malloc(strlen( SDL_JoystickName(i) ) + 1);
		strcpy( JoystickInfo[i].Name, SDL_JoystickName(i) );

		DebugPrintf( 
			"joysticks_init: joystick (%d), name='%s', axises=%d, buttons=%d, hats=%d\n", 
			i, JoystickInfo[i].Name, JoystickInfo[i].NumAxis, JoystickInfo[i].NumButtons,
			JoystickInfo[i].NumPOVs
		);

		for (j = 0; j < JoystickInfo[i].NumAxis; j++)
		{	
			JoystickInfo[i].Axis[j].action		= SHIPACTION_Nothing;
			JoystickInfo[i].Axis[j].inverted	= FALSE;
			JoystickInfo[i].Axis[j].deadzone	= 20;
			JoystickInfo[i].Axis[j].fine		= TRUE;
			JoystickInfo[i].Axis[j].exists		= TRUE;

			JoystickInfo[i].Axis[j].name =
				(char*) malloc (MAX_JOYNAME+1);

			sprintf(
				JoystickInfo[i].Axis[j].name,
				"Axis %d",
				j
			);
		}

		for( j = 0; j < JoystickInfo[i].NumButtons; j++ )
		{
			JoystickInfo[i].Button[j].action = SHIPACTION_Nothing;

			JoystickInfo[i].Button[j].name = 
				(char *) malloc (MAX_JOYNAME+1);

			sprintf(
				JoystickInfo[i].Button[j].name,
				"Button %d", 
				j
			);
		}

		for (j = 0; j < JoystickInfo[i].NumPOVs; j++)
		{
			JoystickInfo[i].POV[j].name = 
				(char *) malloc (MAX_JOYNAME+1);
  
			sprintf(
				JoystickInfo[i].POV[j].name,
				"Hat %d", 
				j
			);

			for (k = 0; k < MAX_POV_DIRECTIONS; k++)
			{
				JoystickInfo[i].POV[j].action[k] =
					SHIPACTION_Nothing;      
				
				 JoystickInfo[i].POV[j].dirname[k] = 
					(char *) malloc (MAX_JOYNAME+1);

				sprintf(
					JoystickInfo[i].POV[j].dirname[k],
					"%s %d",
					JoystickInfo[i].POV[j].name,
					k
				);
			}
		}
	}

	return TRUE;
}

BOOL joysticks_cleanup( void )
{
	int i, j, k;
	for (i = 0; i < Num_Joysticks; i++)
	{
		JoystickInfo[i].assigned = FALSE;
		JoystickInfo[i].connected = FALSE;
		JoystickInfo[i].NumButtons = 0;
		JoystickInfo[i].NumPOVs = 0;
		JoystickInfo[i].NumAxis = 0;

		if(JoystickInfo[i].Name)
		{
			free(JoystickInfo[i].Name);
		}
		JoystickInfo[i].Name = NULL;

		if(JoystickInfo[i].sdl_joy)
		{
			if(SDL_JoystickOpened(i))
			{
				SDL_JoystickClose(
					JoystickInfo[i].sdl_joy
				);
			}
		}

		for (j = 0; j < JoystickInfo[i].NumButtons; i++)
		{
			if(JoystickInfo[i].Button[j].name)
			{
				free(JoystickInfo[i].Button[j].name);
			}
			JoystickInfo[i].Button[j].name = NULL;
			JoystickInfo[i].Button[j].action = SHIPACTION_Nothing;
		}

		for (j = 0; j < JoystickInfo[i].NumPOVs; j++)
		{
			if(JoystickInfo[i].POV[j].name)
			{
				free(JoystickInfo[i].POV[j].name);
			}
			JoystickInfo[i].POV[j].name = NULL;
			for ( k = 0; k < MAX_POV_DIRECTIONS; k++ )
			{
				if ( JoystickInfo[ i ].POV[ j ].dirname[ k ] )
				{
					free( JoystickInfo[ i ].POV[ j ].dirname[ k ] );
				}
				JoystickInfo[ i ].POV[ j ].dirname[ k ] = NULL;
				JoystickInfo[ i ].POV[ j ].action[ k ] = SHIPACTION_Nothing;
			}
		}

		for (j = 0; j < MAX_JOYSTICK_AXIS; j++)
		{
			if(JoystickInfo[i].Axis[j].name)
			{
				free(JoystickInfo[i].Axis[j].name);
			}
			JoystickInfo[i].Axis[j].name = NULL;
			JoystickInfo[i].Axis[j].exists = FALSE;
			JoystickInfo[i].Axis[j].action = SHIPACTION_Nothing;
			JoystickInfo[i].Axis[j].sensitivity = 0.0f;
			JoystickInfo[i].Axis[j].deadzone = 0;
			JoystickInfo[i].Axis[j].inverted = FALSE;
			JoystickInfo[i].Axis[j].fine = FALSE;
		}
	}
	return TRUE;
}

#endif // ! DINPUTJOY
