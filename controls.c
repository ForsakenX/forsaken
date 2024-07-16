/* handle keyboard and mouse control */

#include <math.h>

#include "input.h"
#include "config.h"
#include "title.h"
#include "controls.h"
#include <stdio.h>
#include "text.h"
#include "main.h"
#include "compobjects.h"
#include "quat.h"
#include "object.h"
#include "networking.h"
#include "primary.h"
#include "secondary.h"
#include "sfx.h"
#include "render.h"
#include "util.h"

// TEMP!! - for testing looping SFX
#include "sfx.h"
#include "compobjects.h"
#include "quat.h"
#include "object.h"
#include "networking.h"
#include "ships.h"


extern MENU MENU_NEW_Battle;
extern MENU *CurrentMenu;

extern render_info_t render_info;
extern  bool  Cheated;
extern bool WaitingToQuit;
extern bool CheatsDisabled;
extern  bool  JoystickInput;

extern  BYTE  MyGameStatus;
extern  float framelag;
extern  float NitroFuel;
extern USERCONFIG *player_config;

extern  float MaxMoveSpeed;
extern  float MoveAccell;
extern  float MoveDecell;
extern  float MaxTurboSpeed;
extern  float TurboAccell;
extern  float TurboDecell;
extern  float MaxTurnSpeed;
extern  float TurnAccell;
extern  float TurnDecell;
extern  float MaxRollSpeed;
extern  float RollAccell;
extern  float RollDecell;
extern  float MaxBankAngle;
extern  float BankAccell;
extern  float BankDecell;
extern bool DebugInfo;

extern  int8_t  PrimaryToFireLookup[ MAXPRIMARYWEAPONS ];
extern  bool  PrimaryWeaponCheat;
extern  int8_t  SecondaryToFireLookup[ MAXSECONDARYWEAPONS ];
extern  bool  SecondaryWeaponCheat;
extern  bool  GodMode;

extern  int FontHeight;

/*===================================================================
    Some Keyboard and Mouse Globals
===================================================================*/

#define MOUSE_ZSTEP     (120)

#define MAX_MOUSE_DELTA_X (40.0F)
#define MAX_MOUSE_DELTA_Y (40.0F)

#define MouseXFactor    (  1.0F / ( MAX_MOUSE_DELTA_X * MAX_MOUSE_DELTA_X ) )
#define MouseYFactor    ( -1.0F / ( MAX_MOUSE_DELTA_Y * MAX_MOUSE_DELTA_Y ) )

#define KEY_HELD( K )     ( key_state[ new_input ][ K ] )
#define KEY_PRESSED( K )    ( !( key_state[ old_input ][ K ] ) && ( key_state[ new_input ][ K ] ) )
#define KEY_RELEASED( K )   ( ( key_state[ old_input ][ K ] ) && !( key_state[ new_input ][ K ] ) )

#ifdef DINPUTJOY

#define JOYSTICK_BUTTON_HELD( J, B )    ( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 )
#define JOYSTICK_BUTTON_PRESSED( J, B )   ( !( js[ old_input ][ J ].rgbButtons[ B ] & 0x80) && ( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 ) )
#define JOYSTICK_BUTTON_RELEASED( J, B )  ( ( js[ old_input ][ J ].rgbButtons[ B ] & 0x80) && !( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 ) )

#define JOYSTICK_POV_HELD( J, P, D )    ( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 )
#define JOYSTICK_POV_PRESSED( J, P, D )   ( !( js_pov[ old_input ][ J ][ P ][ D ] & 0x80 ) && ( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 ) )
#define JOYSTICK_POV_RELEASED( J, P, D )  ( ( js_pov[ old_input ][ J ][ P ][ D ] & 0x80 ) && !( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 ) )

#else // ! DINPUTJOY

#define JOYSTICK_BUTTON_HELD( J, B )    ( js_buttons[ new_input ][ J ][ B ] )
#define JOYSTICK_BUTTON_PRESSED( J, B )   ( !( js_buttons[ old_input ][ J ][ B ]) && ( js_buttons[ new_input ][ J ][ B ] ) )
#define JOYSTICK_BUTTON_RELEASED( J, B )  ( ( js_buttons[ old_input ][ J ][ B ]) && !( js_buttons[ new_input ][ J ][ B ] ) )

#define JOYSTICK_POV_HELD( J, P, D )    ( js_pov[ new_input ][ J ][ P ][ D ] )
#define JOYSTICK_POV_PRESSED( J, P, D )   ( !( js_pov[ old_input ][ J ][ P ][ D ] ) && ( js_pov[ new_input ][ J ][ P ][ D ] ) )
#define JOYSTICK_POV_RELEASED( J, P, D )  ( ( js_pov[ old_input ][ J ][ P ][ D ] ) && !( js_pov[ new_input ][ J ][ P ][ D ] ) )

#endif

int GetPOVMask( DWORD pov );

bool CheatsEnabled = false;

#define TOTAL_JOYSTICK_ACTIONS  140 // 5 axis positions, 3 axis rotations, 4 POV hats and 128 buttons!
#define TOTAL_JOYSTICK_AXIS   8 // 5 axis positions, 3 axis rotations

#ifdef DINPUTJOY
DIJOYSTATE2 js[ INPUT_BUFFERS ][ MAX_JOYSTICKS ];
#else
int js_buttons[ INPUT_BUFFERS ][ MAX_JOYSTICKS ][ MAX_JOYSTICK_BUTTONS ];
#endif

#ifdef DINPUTJOY
extern LPDIRECTINPUTDEVICE2 lpdiJoystick[MAX_JOYSTICKS];
#endif

extern MENU MENU_QuickTextSend;
extern MENU MENU_QuickTextSendWhisper;
extern MENU MENU_NEW_StartSinglePlayer;

int old_input = 0;
int new_input = 1;

bool joystick_poll( int joysticknum );

#ifdef DINPUTJOY
BYTE 
#else
u_int8_t
#endif
js_pov[ INPUT_BUFFERS ][ MAX_JOYSTICKS ][ MAX_JOYSTICK_POVS ][ MAX_POV_DIRECTIONS ];

// (Sfx.c)
extern void SendBikerTaunt();

bool flush_input = true;

u_int8_t key_state[ INPUT_BUFFERS ][ SDLK_LAST ];

char *ShipActionText[NUM_SHIP_ACTIONS] = {
  "No Action",
  "Rotate Up",
  "Rotate Down",
  "Rotate Left",
  "Rotate Right",
  "Roll Left",
  "Roll Right",
  "Slide Up",
  "Slide Down",
  "Slide Left",
  "Slide Right",
  "Move Forward",
  "Move Back",
  "Cruise Increase",
  "Cruise Decrease",
  "Nitro",
  "Fire Primary",
  "Fire Secondary",
  "Drop Mine",
  "Next Primary",
  "Prev Primary",
  "Next Secondary",
  "Prev Secondary",
  "Drop Primary",
  "Drop Secondary",
  "Drop Shield",
  "Drop Ammo",
  "Slide Mode",
  "Roll Mode",
};

char *ShipAxisText[NUM_SHIP_AXIS_ACTIONS] = {
  "No Action",
  "Rotate Up/Down",
  "Rotate Left/Right",
  "Roll Left/Right",
  "Slide Up/Down",
  "Slide Left/Right",
  "Move Forward/Back",
};

char *ShipAxisSeperateText[NUM_SHIP_AXIS_ACTIONS * 2] = {
  "No Action",
  "No Action",
  "Rotate Up",
  "Rotate Down",
  "Rotate Left",
  "Rotate Right",
  "Roll Left",
  "Roll Right",
  "Slide Up",
  "Slide Down",
  "Slide Left",
  "Slide Right",
  "Move Forward",
  "Move Back",
};

char FlashText[ 128 ];
float FlashTextActive = 0.0F;

bool FullRearView = false;
bool ShowStats = false;
bool ShowMessages = false;	// display long list of chat messages
bool ShowStatistics = false;	// display statistics in game
bool ShowNetworkInfo = false;	// display enet info in game 

void FlashMenuText( char *text, float activetime, u_int16_t sfx )
{
  if( text )
  {
    strcpy( FlashText, text );
    FlashTextActive = activetime;
    if( sfx != 0xFF )
    {
      PlaySfx( sfx, 1.0F );
    }
  }
}

void ProcessMenuFlashText( void )
{
  if ( FlashTextActive > 0.0F )
  {
    FlashTextActive -= framelag;
    if ( FlashTextActive > 0.0F )
    {
      CenterPrint4x5Text( FlashText , render_info.window_size.cy - FontHeight * 2, 2 );
    }
  }
}

int EnableCheats( char *cheat )
{
  CheatsEnabled = true;
  FlashMenuText( "cheat mode enabled", 120.0F, SFX_Secret );
  Cheated = true;

  return 1;
}

int JimBeam( char *cheat )
{
  if( PrimaryWeaponCheat )
  {
    PrimaryToFireLookup[ PULSAR ] = PULSAR;
    PrimaryToFireLookup[ TROJAX ] = TROJAX;
    PrimaryToFireLookup[ PYROLITE_RIFLE ] = PYROLITE_RIFLE;
    PrimaryToFireLookup[ TRANSPULSE_CANNON ] = TRANSPULSE_CANNON;
    PrimaryToFireLookup[ SUSS_GUN ] = SUSS_GUN;
    PrimaryToFireLookup[ LASER ] = LASER;
    PrimaryWeaponCheat = false;
    FlashMenuText( "Beam me down", 120.0F, SFX_Secret );
  }
  else
  {
    PrimaryWeaponCheat = true;
    FlashMenuText( "Beam me up", 120.0F, SFX_Secret );
  }
  return 1;
}

int Lumberjack( char *cheat )
{
#if 0
  static int line = 0;
  static char *msg[] = {
    "he's a lumberjack",
    "and he's okay,",
    "he works all night",
    "and he sleeps all day."
  };
  FlashMenuText( msg[ line++ % ( sizeof( msg ) / sizeof( msg[ 0 ] ) ) ], 120.0F, SFX_Secret );
#else
  if( SecondaryWeaponCheat )
  {
    FlashMenuText( "...and he sleeps all day", 120.0F, SFX_Secret );
    SecondaryToFireLookup[ MUGMISSILE ] = MUGMISSILE;
    SecondaryToFireLookup[ SOLARISMISSILE ] = SOLARISMISSILE;
    SecondaryToFireLookup[ THIEFMISSILE ] = THIEFMISSILE;
    SecondaryToFireLookup[ SCATTERMISSILE ] = SCATTERMISSILE;
    SecondaryToFireLookup[ GRAVGONMISSILE ] = GRAVGONMISSILE;
    SecondaryToFireLookup[ MULTIPLEMISSILE ] = MULTIPLEMISSILE;
    SecondaryToFireLookup[ TITANSTARMISSILE ] = TITANSTARMISSILE;
    SecondaryToFireLookup[ PURGEMINE ] = PURGEMINE;
    SecondaryToFireLookup[ PINEMINE ] = PINEMINE;
    SecondaryToFireLookup[ QUANTUMMINE ] = QUANTUMMINE;
    SecondaryToFireLookup[ SPIDERMINE ] = SPIDERMINE;
//    SecondaryToFireLookup[ PINEMISSILE ] = PINEMISSILE;
//    SecondaryToFireLookup[ TITANSTARSHRAPNEL ] = TITANSTARSHRAPNEL;
//    SecondaryToFireLookup[ ENEMYSPIRALMISSILE ] = ENEMYSPIRALMISSILE;
//    SecondaryToFireLookup[ ENEMYHOMINGMISSILE ] = ENEMYHOMINGMISSILE;
//    SecondaryToFireLookup[ ENEMYBLUEHOMINGMISSILE ] = ENEMYBLUEHOMINGMISSILE;
//    SecondaryToFireLookup[ ENEMYFIREBALL ] = ENEMYFIREBALL;
//    SecondaryToFireLookup[ ENEMYTENTACLE ] = ENEMYTENTACLE;
//    SecondaryToFireLookup[ ENEMYDEPTHCHARGE ] = ENEMYDEPTHCHARGE;
    SecondaryWeaponCheat = false;
  }
  else
  {
    SecondaryWeaponCheat = true;
    FlashMenuText( "He works all night...", 120.0F, SFX_Secret );
  }
#endif
  return 1;
}

int ToggleGodMode( char *cheat )
{
  if( !GodMode )
  {
    GodMode = true;
    FlashMenuText( "God mode enabled", 120.0F, SFX_Secret );
    GivemeAllWeapons();
  }
  else
  {
    FlashMenuText( "God mode disabled", 120.0F, SFX_Secret );
    LoseAllWeapons();
    GodMode = false;
  }
  return 1;
}

static struct
{
  unsigned char cheatcode[ 16 ];
  int (*cheatfunc)( char * );
  bool allowmultiplayer;
  int next;
} CheatTable[] = 
{
  { { SDLK_b, SDLK_u, SDLK_b, SDLK_b, SDLK_l, SDLK_e, SDLK_s, 0 }, EnableCheats, true, 0 },
  { { SDLK_j, SDLK_i, SDLK_m, SDLK_b, SDLK_e, SDLK_a, SDLK_m, 0 }, JimBeam, false, 0 },
  { { SDLK_l, SDLK_u, SDLK_m, SDLK_b, SDLK_e, SDLK_r, SDLK_j, SDLK_a, SDLK_c, SDLK_k, 0 }, Lumberjack, false, 0 },
  { { SDLK_i, SDLK_a, SDLK_m, SDLK_z, SDLK_e, SDLK_u, SDLK_s, 0 }, ToggleGodMode, false, 0 },
};
// NOTE: add any new cheats to DisableCheats function to ensure they are not active in multiplayer game

int ShipAxisLookup[NUM_SHIP_AXIS_ACTIONS] = { SHIPACTION_Nothing, SHIPACTION_RotateUp, SHIPACTION_RotateLeft, SHIPACTION_RollLeft,
                        SHIPACTION_SlideUp, SHIPACTION_SlideLeft, SHIPACTION_MoveForward };

char *JoystickPOVDirections[MAX_POV_DIRECTIONS] = { "Up", "Down", "Left", "Right" };

#ifdef DINPUTJOY
HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty,
                   DWORD dwObject, DWORD dwHow, DWORD dwValue);
#endif

// this function receives higher level encoded bits

#ifdef USEINLINE
__inline
#endif
short key_pressed( USERKEY *k )
{
  int j;
  VirtualKeycode c;

  for ( j = 0; j < k->keys; j++ )
  {
    c = k->key[ j ];
    if ( KEY_ON_KEYBOARD( c ) )
    {
      if ( KEY_PRESSED( c ) )
        return 1;
    }
    else if ( KEY_ON_MOUSE( c ) )
    {
      switch( c )
      {
      case UP_MOUSE:
        if ( MOUSE_WHEEL_UP_PRESSED() )
          return 1;
        break;
      case DOWN_MOUSE:
        if ( MOUSE_WHEEL_DOWN_PRESSED() )
          return 1;
        break;
	  default:
        if ( MOUSE_BUTTON_PRESSED( c - LEFT_MOUSE ) )
          return 1;
        break;
      }
    }
    else if ( KEY_ON_JOYSTICK( c ) )
    {
      int joystick;

      joystick = KEY_JOYSTICK( c );
      if ( !JoystickInfo[ joystick ].connected )
        continue;
      if ( KEY_ON_JOYSTICK_BUTTON( c ) )
      {
        int button;

        button = KEY_JOYSTICK_BUTTON( c );
        if ( JOYSTICK_BUTTON_PRESSED( joystick, button ) )
          return 1;
      }
      else if ( KEY_ON_JOYSTICK_POV( c ) )
      {
        int pov, dir;

        pov = KEY_JOYSTICK_POV( c );
        dir = KEY_JOYSTICK_POVDIR( c );
        if ( JOYSTICK_POV_PRESSED( joystick, pov, dir ) )
          return 1;
      }
    }
  }
  return 0;
}

// this function receives higher level encoded bits

#ifdef USEINLINE
__inline
#endif
short key_held( USERKEY *k )
{
  int j;
  VirtualKeycode c;

  for ( j = 0; j < k->keys; j++ )
  {
    c = k->key[ j ];
    if ( KEY_ON_KEYBOARD( c ) )
    {
      if ( KEY_HELD( c ) )
        return 1;
    }
    else if ( KEY_ON_MOUSE( c ) )
    {
      switch( c )
      {
      case UP_MOUSE:
        if ( MOUSE_WHEEL_UP() )
          return 1;
        break;
      case DOWN_MOUSE:
        if ( MOUSE_WHEEL_DOWN() )
          return 1;
        break;
	  default:
        if ( MOUSE_BUTTON_HELD( c - LEFT_MOUSE ) )
          return 1;
        break;
      }
    }
    else if ( KEY_ON_JOYSTICK( c ) )
    {
      int joystick;

      joystick = KEY_JOYSTICK( c );

      if ( !JoystickInfo[ joystick ].connected )
        continue;

      if ( KEY_ON_JOYSTICK_BUTTON( c ) )
      {
        int button;

        button = KEY_JOYSTICK_BUTTON( c );
        if ( JOYSTICK_BUTTON_HELD( joystick, button ) )
          return 1;
      }
      else if ( KEY_ON_JOYSTICK_POV( c ) )
      {
        int pov, dir;
        pov = KEY_JOYSTICK_POV( c );
        dir = KEY_JOYSTICK_POVDIR( c );
        if ( JOYSTICK_POV_HELD( joystick, pov, dir ) )
          return 1;
      }
    }
  }
  return 0;
}

void DisableCheats( void )
{
  CheatsEnabled = false;
  DebugInfo = false;

  // used to indicate cheats disabled for multiplayer game...
  CheatsDisabled = true;

  if( PrimaryWeaponCheat )
    JimBeam( NULL );
  if( SecondaryWeaponCheat )
    Lumberjack( NULL );
  if( GodMode )
    ToggleGodMode( NULL );
}

void CheckCheats( int key )
{
  int j, i;
  int cheats;
  unsigned char ch;

  cheats = sizeof( CheatTable ) / sizeof( CheatTable[ 0 ] );
  if ( key )
  {
    for ( j = 0; j < cheats; j++ )
    {
      ch = CheatTable[ j ].cheatcode[ CheatTable[ j ].next ];
      if ( key == ch )
      {
        CheatTable[ j ].next++;
        if ( !CheatTable[ j ].cheatcode[ CheatTable[ j ].next ] )
        {
          if ( ( MyGameStatus != STATUS_Normal ) || ( CheatTable[ j ].allowmultiplayer ) )
          {
            if ( CheatTable[ j ].cheatfunc && ( CheatsEnabled || !j ) )
            {
              CheatTable[ j ].cheatfunc( CheatTable[ j ].cheatcode );
        
              // reset all cheats...
              for( i = 0; i < cheats; i++ )
              {
                CheatTable[ i ].next = 0;
              }
              break;    
            }
          }
          CheatTable[ j ].next = 0;
        }
      }
      else
      {
        CheatTable[ j ].next = 0;
      }
    }
  }
}

static void ReadKeyboard( void )
{
	int i, nkeys;
	u_int8_t * keys;

	if( flush_input )
	{
		memset( &key_state[ new_input ], 0, SDLK_LAST );
		return;
	}

#if SDL_VERSION_ATLEAST(2,0,0)
	keys = SDL_GetKeyboardState( &nkeys );
#else
	keys = SDL_GetKeyState( &nkeys );
#endif

	//printf("number of keys = %d\n",nkeys);

	memset( &key_state[ new_input ], 0, SDLK_LAST );

	for( i = 0; i < SDLK_LAST; i++ )
		key_state[ new_input ][ i ] = keys[ i ];
}

void clear_last_mouse_state(void)
{
	int i;
	for ( i = 0; i < MAX_MOUSE_BUTTONS; i++ )
	{
		mouse_states[ new_input ].buttons[ i ] = 0;
		mouse_states[ old_input ].buttons[ i ] = 0;
		mouse_state.buttons[ i ] = 0;
	}
}

static void ReadMouse( void )
{
	int i;
	mouse_state_t * state = read_mouse();
	mouse_states[ new_input ].xrel = state->xrel;
	mouse_states[ new_input ].yrel = state->yrel;
	mouse_states[ new_input ].wheel = mouse_state.wheel;
	for ( i = 0; i < MAX_MOUSE_BUTTONS; i++ )
		mouse_states[ new_input ].buttons[ i ] = mouse_state.buttons[ i ];
}

void ReadInput( void )
{
	int i;

	if ( WaitingToQuit )
		return;

	old_input = new_input;
	new_input++;

	if ( new_input >= INPUT_BUFFERS )
		new_input = 0;

	ReadMouse();
	ReadKeyboard();

	if ( JoystickInput )
	{
		for (i = 0; i < Num_Joysticks; i++)
		{
			if (JoystickInfo[i].connected)
				joystick_poll(i);
		}
	}

	flush_input = false;
}

void control_ship( USERCONFIG *conf, SHIPCONTROL *ctrl )
{
  int   j, joystick;
  float mouse_dx;
  float mouse_dy;
  float pitch_sign;
  float turn_sign;
  float MaxMove, MaxTurbo, MaxTurn, MaxRoll, MaxBank;

  ctrl->pitch   = 0.0F;
  ctrl->yaw   = 0.0F;
  ctrl->roll    = 0.0F;
  ctrl->bank    = 0.0F;
  ctrl->right   = 0.0F;
  ctrl->up    = 0.0F;
  ctrl->forward = 0.0F;
  ctrl->cruise_control = 0;
  ctrl->turbo   = 0;
  ctrl->fire_primary = 0;
  ctrl->fire_secondary = 0;
  ctrl->fire_mine = 0;
  ctrl->select_primary = 0;
  ctrl->select_secondary = 0;
  ctrl->select_next_primary = 0;
  ctrl->select_prev_primary = 0;
  ctrl->select_next_secondary = 0;
  ctrl->select_prev_secondary = 0;
  ctrl->drop_primary = 0;
  ctrl->drop_secondary = 0;
  ctrl->drop_shield = 0;
  ctrl->drop_ammo = 0;

  if ( !conf )
    return; // bail if no config supplied

  if ( CurrentMenu )
    return; // disable bike controls if using menus

  mouse_dx = mouse_states[ new_input ].xrel * conf->mouse_x_sensitivity * 4.0F;
  mouse_dy = mouse_states[ new_input ].yrel * conf->mouse_y_sensitivity * 4.0F;

#if 0
  if ( mouse_dx < -MAX_MOUSE_DELTA_X )
    mouse_dx = -MAX_MOUSE_DELTA_X;
  else if ( mouse_dx > MAX_MOUSE_DELTA_X )
    mouse_dx = MAX_MOUSE_DELTA_X;
  if ( mouse_dy < -MAX_MOUSE_DELTA_Y )
    mouse_dy = -MAX_MOUSE_DELTA_Y;
  else if ( mouse_dy > MAX_MOUSE_DELTA_Y )
    mouse_dy = MAX_MOUSE_DELTA_Y;
#endif

// TODO
// win32 appear to return greator values even in some resolution
// we should also configure a fine control setting for the mouse to turn on/off the fabs() trick
#ifdef WIN32
	mouse_dx = mouse_dx * (float) fabs( mouse_dx ) * MouseXFactor; // factor = 0.000625
	mouse_dy = mouse_dy * (float) fabs( mouse_dy ) * MouseYFactor; // factor = -0.000625
#else
	mouse_dx = mouse_dx * (float) fabs( mouse_dx ) * 0.01f; // tweaked for linux
	mouse_dy = mouse_dy * (float) fabs( mouse_dy ) * -0.01f;
#endif

#if 0
  if ( mouse_dx < -framelag )
    mouse_dx = -framelag;
  if ( mouse_dx > framelag )
    mouse_dx = framelag;
  if ( mouse_dy < -framelag )
    mouse_dy = -framelag;
  if ( mouse_dy > framelag )
    mouse_dy = framelag;
#endif

  pitch_sign = ( conf->invert_pitch ) ? -1.0F : 1.0F;
  turn_sign = ( conf->invert_turn ) ? -1.0F : 1.0F;

  if ( key_held( &conf->full_rear_view ) )
    FullRearView = true;
  else
    FullRearView = false;

  if( key_held( &conf->show_messages))
	  ShowMessages = true;
  else
	  ShowMessages = false;

  if( key_held( &conf->show_stats))
	  ShowStatistics = true;
  else
	  ShowStatistics = false;

  if( key_held( &conf->show_networkinfo))
	  ShowNetworkInfo = true;
  else
	  ShowNetworkInfo = false;

  if ( key_pressed( &conf->headlights ) )
		Ships[WhoIAm].headlights = !Ships[WhoIAm].headlights;

#ifndef INPUT_DISABLED

  ctrl->slide_mode = key_held( &conf->move );
  ctrl->roll_mode = key_held( &conf->roll );
  if ( ctrl->slide_mode )
  {
    if ( key_held( &conf->left ) )
    {
      ctrl->right -= MoveAccell * MaxMoveSpeed * framelag;
    }
    if ( key_held( &conf->right ) )
    {
      ctrl->right += MoveAccell * MaxMoveSpeed * framelag;
    }
    if ( mouse_dx != 0.0F )
    {
      ctrl->right += MoveAccell * MaxMoveSpeed * mouse_dx;
    }

    if ( key_held( &conf->up ) )
    {
      ctrl->up += MoveAccell * MaxMoveSpeed * framelag;
    }
    if ( key_held( &conf->down ) )
    {
      ctrl->up -= MoveAccell * MaxMoveSpeed * framelag;
    }
    if ( mouse_dy != 0.0F )
    {
      ctrl->up += MoveAccell * MaxMoveSpeed * mouse_dy;
    }
  }
  else
  {
    if ( ctrl->roll_mode )
    {
      if ( key_held( &conf->left ) )
      {
        ctrl->roll += RollAccell * MaxRollSpeed * framelag;
      }
      if ( key_held( &conf->right ) )
      {
        ctrl->roll -= RollAccell * MaxRollSpeed * framelag;
      }
      if ( mouse_dx != 0.0F )
      {
        ctrl->roll -= RollAccell * MaxRollSpeed* mouse_dx;
      }
    }
    else
    {
      if ( key_held( &conf->left ) )
      {
        ctrl->yaw  -= TurnAccell * MaxTurnSpeed * framelag;
        ctrl->bank += BankAccell * MaxBankAngle * framelag;
      }
      if ( key_held( &conf->right ) )
      {
        ctrl->yaw  += TurnAccell * MaxTurnSpeed * framelag;
        ctrl->bank -= BankAccell * MaxBankAngle * framelag;
      }
      if ( mouse_dx != 0.0F )
      {
        ctrl->yaw  += TurnAccell * MaxTurnSpeed * mouse_dx * turn_sign;
        ctrl->bank -= BankAccell * MaxBankAngle * mouse_dx * turn_sign;
      }
    }
    
    if ( key_held( &conf->down ) )
    {
      ctrl->pitch -= TurnAccell * MaxTurnSpeed * framelag;
    }
    if ( key_held( &conf->up ) )
    {
      ctrl->pitch += TurnAccell * MaxTurnSpeed * framelag;
    }
    if ( mouse_dy != 0.0F )
    {
      ctrl->pitch += TurnAccell * MaxTurnSpeed * pitch_sign * mouse_dy ;
    }
  }

  if ( key_held( &conf->move_left ) )
  {
    ctrl->right -= MoveAccell * MaxMoveSpeed * framelag;
  }
  if ( key_held( &conf->move_right ) )
  {
    ctrl->right += MoveAccell * MaxMoveSpeed * framelag;
  }
  
  if ( key_held( &conf->move_down ) )
  {
    ctrl->up -= MoveAccell * MaxMoveSpeed * framelag;
  }
  if ( key_held( &conf->move_up ) )
  {
    ctrl->up += MoveAccell * MaxMoveSpeed * framelag;
  }
  
  if ( key_held( &conf->roll_left ) )
  {
    ctrl->roll += RollAccell * MaxRollSpeed * framelag;
  }
  if ( key_held( &conf->roll_right ) )
  {
    ctrl->roll -= RollAccell * MaxRollSpeed * framelag;
  }

  if( NitroFuel && ( key_pressed ( &conf->turbo ) ) )
  {
    PlaySfx( SFX_NitroStart, 0.66F );
  }

  ctrl->turbo = key_held( &conf->turbo );
  if ( ctrl->turbo )
  {
    if ( NitroFuel > 0.0F )
    {
      ctrl->forward += TurboAccell * MaxTurboSpeed * framelag;
    }
    else
    {
      ctrl->forward += MoveAccell * MaxMoveSpeed * framelag;
    }
  }
  else if ( key_held( &conf->move_forward ) )
  {
    ctrl->forward += MoveAccell * MaxMoveSpeed * framelag;
  }
  if ( key_held( &conf->move_backward ) )
  {
    ctrl->forward -= MoveAccell * MaxMoveSpeed * framelag;
  }

  if ( key_pressed( &conf->cruise_faster ) )
    ctrl->cruise_control++;
  if ( key_pressed( &conf->cruise_slower ) )
    ctrl->cruise_control--;

  ctrl->fire_primary      = key_held( &conf->fire_primary );
  ctrl->fire_secondary    = key_held( &conf->fire_secondary );
  ctrl->fire_mine       = key_pressed( &conf->fire_mine );

  ctrl->select_next_primary = key_pressed( &conf->select_next_primary );
  ctrl->select_prev_primary = key_pressed( &conf->select_prev_primary );
  
  ctrl->select_next_secondary = key_pressed( &conf->select_next_secondary );
  ctrl->select_prev_secondary = key_pressed( &conf->select_prev_secondary );
  
  ctrl->drop_primary      = key_pressed( &conf->drop_primary );
  ctrl->drop_secondary    = key_pressed( &conf->drop_secondary );
  ctrl->drop_shield     = key_pressed( &conf->drop_shield );
  ctrl->drop_ammo       = key_pressed( &conf->drop_ammo );

  ctrl->select_primary = 0;
  for ( j = 0; j < MAX_PRIMARY_WEAPONS; j++ )
  {
    if ( key_pressed( &conf->select_primary[ j ] ) )
    {
      ctrl->select_primary = j + 1;
    }
  }
  ctrl->select_secondary = 0;
  for ( j = 0; j < MAX_SECONDARY_WEAPONS; j++ )
  {
    if ( key_pressed( &conf->select_secondary[ j ] ) )
    {
      ctrl->select_secondary = j + 1;
    }
  }

  if ( ( SDL_GetModState() & KMOD_CTRL ) && key_pressed( &conf->send_msg ) && MyGameStatus == STATUS_Normal )
  {
    MenuRestart( &MENU_QuickTextSendWhisper );
  }
  else if ( ( key_pressed( &conf->send_msg ) ) && MyGameStatus == STATUS_Normal )
  {
    MenuRestart( &MENU_QuickTextSend );
  }

#ifdef PLAYER_SPEECH_TAUNTS
  if( key_pressed( &conf->send_speech )  )
  {
    PlaySpecificBikerSpeech( SFX_BIKER_TN, Ships[ WhoIAm ].Object.Group, &Ships[WhoIAm].Object.Pos, 0.0F, player_config->bike, -1, false ); // don't update
    if( MyGameStatus == STATUS_Normal )
    {
		SendBikerTaunt(); // (Sfx.c)
      //SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_SpeechTaunt, 0);
    }
  }
#endif
  
	if ( JoystickInput )
	{
		for (joystick = 0; joystick < Num_Joysticks; joystick++)
		{
			if (JoystickInfo[joystick].connected && JoystickInfo[joystick].assigned)
				ReadJoystickInput(ctrl, joystick);
		}
	}

#endif // INPUT_DISABLED

#ifdef LUA_BOT
	ProcessBot1();
#endif

  MaxMove = MoveAccell * MaxMoveSpeed * framelag;
  MaxTurbo = TurboAccell * MaxTurboSpeed * framelag;
  MaxTurn = TurnAccell * MaxTurnSpeed * framelag;
  MaxRoll = RollAccell * MaxRollSpeed * framelag;
  MaxBank = BankAccell * MaxBankAngle * framelag;

  CLAMP( ctrl->pitch, MaxTurn );
  CLAMP( ctrl->yaw, MaxTurn );
  CLAMP( ctrl->roll, MaxRoll );
  CLAMP( ctrl->bank, MaxBank );
  CLAMP( ctrl->right, MaxMove );
  CLAMP( ctrl->up, MaxMove );

  if ( ctrl->turbo && NitroFuel > 0.0F )
  {
    if ( ctrl->forward > MaxTurbo )
      ctrl->forward = MaxTurbo;
    else if ( ctrl->forward < -MaxMove )
      ctrl->forward = -MaxMove;
  }
  else
  {
    CLAMP( ctrl->forward, MaxMove );
  }
}

int AnyKeyReleased( void )
{
  int k;

  if ( CurrentMenu )
    return 0;

  for ( k = 0; k < SDLK_LAST; k++ )
  {
    if ( KEY_RELEASED( k ) )
      return 1;
  }

  for ( k = 0; k < MAX_MOUSE_BUTTONS; k++ )
  {
    if ( MOUSE_BUTTON_RELEASED( k ) )
      return 1;
  }

  if( IsAnyJoystickButtonReleased() )
  {
    return 1;
  }

  return 0;
}

int WhichMousePressed( void )
{
  int k;
  int key;

  key = 0;
  for ( k = 0; k < MAX_MOUSE_BUTTONS; k++ )
  {
    if ( MOUSE_BUTTON_PRESSED( k ) )
    {
      if ( !key )
        key = LEFT_MOUSE + k;
      else
        return 0;
    }
  }
  if ( MOUSE_WHEEL_UP_PRESSED() )
  {
    if ( !key )
      key = UP_MOUSE;
    else
      return 0;
  }
  if ( MOUSE_WHEEL_DOWN_PRESSED() )
  {
    if ( !key )
      key = DOWN_MOUSE;
    else
      return 0;
  }

  return key;
}

int WhichJoystickPressed( void )
{
  int key = 0;

  int k;

  for ( k = 0; k < Num_Joysticks; k++ )
  {
    if ( JoystickInfo[ k ].connected )
    {
      int b, p, d;

      for ( b = 0; b < JoystickInfo[ k ].NumButtons; b++ )
      {
        if ( JOYSTICK_BUTTON_PRESSED( k, b ) )
        {
          if ( !key )
            key = JOYSTICK_BUTTON_KEYCODE( k, b );
          else
            return 0;
        }
      }

      for ( p = 0; p < JoystickInfo[ k ].NumPOVs; p++ )
      {
        for ( d = 0; d < MAX_POV_DIRECTIONS; d++ )
        {
          if ( JOYSTICK_POV_PRESSED( k, p, d ) )
          {
            if ( !key )
              key = JOYSTICK_POVDIR_KEYCODE( k, p, d );
            else
              return 0;
          }
        }
      }
    }
  }

  return key;
}

void DoShipAction( SHIPCONTROL *ctrl, int Action, float amount )
{
  switch (Action)
  {
  case SHIPACTION_RotateUp:
    if ( ctrl->slide_mode )
    {
      ctrl->up += MoveAccell * MaxMoveSpeed * amount;
    }
    else
    {
      ctrl->pitch += TurnAccell * MaxTurnSpeed * amount;
    }
    return;
  case SHIPACTION_RotateLeft:
    if ( ctrl->slide_mode )
    {
      ctrl->right += MoveAccell * MaxMoveSpeed * amount;
    }
    else if ( ctrl->roll_mode )
    {
      ctrl->roll -= RollAccell * MaxRollSpeed * amount;
    }
    else
    {
      ctrl->yaw  += TurnAccell * MaxTurnSpeed * amount;
      ctrl->bank -= BankAccell * MaxBankAngle * amount;
    }
    return;
  case SHIPACTION_RollLeft:
    ctrl->roll -= RollAccell * MaxRollSpeed * amount;
    return;
  case SHIPACTION_SlideUp:
    ctrl->up -= MoveAccell * MaxMoveSpeed * amount;
    return;
  case SHIPACTION_SlideLeft:
    ctrl->right += MoveAccell * MaxMoveSpeed * amount;
    return;
  case SHIPACTION_MoveForward:
    ctrl->forward -= MoveAccell * MaxMoveSpeed  * amount;
    return;
  }
}

// this function doesn't really do anything important needed to port
#ifdef DINPUTJOY
HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty,
                   DWORD dwObject, DWORD dwHow, DWORD dwValue)
{
   DIPROPDWORD dipdw;

   dipdw.diph.dwSize       = sizeof(dipdw);
   dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
   dipdw.diph.dwObj        = dwObject;
   dipdw.diph.dwHow        = dwHow;
   dipdw.dwData            = dwValue;

   return pdev->lpVtbl->SetProperty(pdev, guidProperty, &dipdw.diph);

}

// this function sets the input range to -100 <-> +100 (we already do that in sdl)
// it also sets the dead zone which we do not support in sdl yet.. probably easy to do though...
void SetUpJoystickAxis(int joystick)
{
  DIPROPRANGE diprg;
  bool DeadzoneNotSet = false;
  int i;
  
  // set axis range to (-100 ... +100)
  // This lets us test against 0 to see which way the stick is pointed.
  diprg.diph.dwSize       = sizeof(diprg);
  diprg.diph.dwHeaderSize = sizeof(diprg.diph);
  diprg.diph.dwHow        = DIPH_BYOFFSET;
  diprg.lMin              = -100;
  diprg.lMax              = +100;
      
  DebugPrintf( "SetUpJoystickAxis: setting up %d axes for joystick #%d='%s'\n",
    JoystickInfo[ joystick ].NumAxis,
    joystick,
    JoystickInfo[ joystick ].Name );
  if (JoystickInfo[joystick].Axis[AXIS_XAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up X axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_X;
    if( !lpdiJoystick[joystick] )
	{
      JoystickInfo[joystick].Axis[AXIS_XAxis].exists = false; // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for X axis because lpdiJoystick[joystick] is NULL\n" );
	}
	else if ( lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_XAxis].exists = false; // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for X axis\n" );
    }
	else
    {
      // set  dead zone
      // Units are ten thousandths, so multiply %age by 100.
      DebugPrintf( "SetUpJoystickAxis: setting up X axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_XAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_X, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_XAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for X axis\n" );
      }
    }
  }
  if (JoystickInfo[joystick].Axis[AXIS_YAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up Y axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_Y;
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_YAxis].exists = false; // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for Y axis\n" );
    }else
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Y axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_YAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_Y, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_YAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Y axis\n" );
      }
    }
  }
  if (JoystickInfo[joystick].Axis[AXIS_ZAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up Z axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_Z;
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_ZAxis].exists = false; // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for Z axis\n" );
    }else
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Z axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_ZAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_Z, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_ZAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Z axis\n" );
      }
    }
  }
  if (JoystickInfo[joystick].Axis[AXIS_RxAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up Rx axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_RX;
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_RxAxis].exists = false;  // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for Rx axis\n" );
    }else
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Rx axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_RxAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_RX, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_RxAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Rx axis\n" );
      }
    }
  }
  if (JoystickInfo[joystick].Axis[AXIS_RyAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up Ry axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_RY;
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_RyAxis].exists = false;  // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for Ry axis\n" );
    }else
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Ry axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_RyAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_RY, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_RyAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Ry axis\n" );
      }
    }
  }
  if (JoystickInfo[joystick].Axis[AXIS_RzAxis].exists)
  {
    DebugPrintf( "SetUpJoystickAxis: setting up Rz axis range=(%d,%d) for joystick #%d='%s'\n",
      diprg.lMin, diprg.lMax,
      joystick,
      JoystickInfo[ joystick ].Name );
    diprg.diph.dwObj        = DIJOFS_RZ;
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_RzAxis].exists = false;  // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for Rz axis\n" );
    }else
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Rz axis deadzone=%d for joystick #%d='%s'\n",
        JoystickInfo[ joystick ].Axis[AXIS_RzAxis].deadzone,
        joystick,
        JoystickInfo[ joystick ].Name );
      if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_RZ, DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_RzAxis].deadzone * 100)) != DI_OK)
      {
        // cannot set deadzone - but just ignore for now...
        DeadzoneNotSet = true;
        DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Rz axis\n" );
      }
    }
  }
  for (i = 0; i < 2; i++)
  {
    if (JoystickInfo[joystick].Axis[AXIS_SliderAxis0 + i].exists)
    {
      DebugPrintf( "SetUpJoystickAxis: setting up Slider%d range=(%d,%d) for joystick #%d='%s'\n",
        i,
        diprg.lMin, diprg.lMax,
        joystick,
        JoystickInfo[ joystick ].Name );
      diprg.diph.dwObj        = DIJOFS_SLIDER(i);
      if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
      {
        JoystickInfo[joystick].Axis[AXIS_SliderAxis0 + i].exists = false; // cannot set range, therefore do not allow axis
        DebugPrintf( "SetUpJoystickAxis: failed to set range for Slider%d axis\n", i );
      }else
      {
        DebugPrintf( "SetUpJoystickAxis: setting up Slider%d deadzone=%d for joystick #%d='%s'\n",
          i,
          JoystickInfo[ joystick ].Axis[AXIS_SliderAxis0 + i].deadzone,
          joystick,
          JoystickInfo[ joystick ].Name );
        if (SetDIDwordProperty(lpdiJoystick[joystick], DIPROP_DEADZONE, DIJOFS_SLIDER(i), DIPH_BYOFFSET, (DWORD)(JoystickInfo[joystick].Axis[AXIS_SliderAxis0 + i].deadzone * 100)) != DI_OK)
        {
          // cannot set deadzone - but just ignore for now...
          DeadzoneNotSet = true;
          DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Slider%d axis\n", i );
        }
      }
    }
  }

  // could do something about unset deadzones here...
  if (DeadzoneNotSet)
    DeadzoneNotSet = false;

}

// this just gets the pov direction which is internal to the dinput version
// nothing here to port...
int GetPOVDirection( DIJOYSTATE2 *data, int POVNum )
{
  int dir;
  
  if (!((data->rgdwPOV[POVNum] == 65535) || (data->rgdwPOV[POVNum] == -1)))
  {
    dir = POV_Centre;
    if( data->rgdwPOV[POVNum] > 27000 || data->rgdwPOV[POVNum] < 9000 ) // up
    {
      dir |= POV_Up;
    }
    if( data->rgdwPOV[POVNum] > 9000 && data->rgdwPOV[POVNum] < 27000 ) // down
    {
      dir |= POV_Down;
    }
    if( data->rgdwPOV[POVNum] < 18000 && data->rgdwPOV[POVNum] > 0 ) // right
    {
      dir |= POV_Right;
    }
    if( data->rgdwPOV[POVNum] > 18000 ) // left
    {
      dir |= POV_Left;
    }
    return dir;
  }else
    return POV_Centre;
}
// nothing here to port again we have this all done already in sdl version

#define POV_UP      0
#define POV_RIGHT   9000
#define POV_DOWN    18000
#define POV_LEFT    27000

#define POV_RANGE   36000
#define POV_HALFRANGE 18000
#define POV_TOLERANCE 6750  // this gives 45 deg for each of 8 compass directions

int GetPOVMask( DWORD pov )
{
  static DWORD POVDir[ MAX_POV_DIRECTIONS ] =
  {
    POV_UP,
    POV_DOWN,
    POV_LEFT,
    POV_RIGHT
  };
  int mask;
  int dir;
  int dpov;

  mask = 0;
  if ( pov != 0xFFFF && pov != 0xFFFFFFFF )
  {
    for ( dir = 0; dir < MAX_POV_DIRECTIONS; dir++ )
    {
      dpov = abs( pov - POVDir[ dir ] );
      if ( dpov > POV_HALFRANGE )
        dpov = POV_RANGE - dpov;
      if ( dpov <= POV_TOLERANCE )
        mask |= ( 1 << dir );
    }
  }
  return mask;
}

bool joystick_poll( int joysticknum )
{
   HRESULT hRes;
   int i, j, povdir;

   /* joystick doesn't exist */
   if( !lpdiJoystick[joysticknum] )
     return false;

poll:

   /* poll the device */
   hRes = IDirectInputDevice2_Poll(lpdiJoystick[joysticknum]);

   /* not needed or succeeded */
   if ( hRes == DI_NOEFFECT && hRes == DI_OK )
     goto state;

   /* lets look for some errors */
   switch ( hRes )
   {

   // Access to the input device has been lost. It must be reacquired. 
   case DIERR_INPUTLOST:

   // The operation cannot be performed unless the device is acquired. 
   case DIERR_NOTACQUIRED:

     // acquire the device
     hRes = IDirectInputDevice2_Acquire(lpdiJoystick[joysticknum]);

     // must be a deeper issue
     if ( hRes != DI_OK )
       return false;

     // try again
     goto poll;

     break;

   // The object has not been initialized. 
   case DIERR_NOTINITIALIZED:

     // must be a deeper issue
     return false;
     break;

   }

state:

   // get data from the joystick
   hRes = IDirectInputDevice_GetDeviceState( lpdiJoystick[joysticknum],
                                             sizeof(DIJOYSTATE2),
                                             &js[ new_input ][joysticknum]);
   // if we got an error
   if ( hRes == DI_OK )
     goto povs;
   
   // lets check out some errors
   switch (hRes)
   {

   // Access to the input device has been lost. It must be reacquired. 
   case DIERR_INPUTLOST:

   // The operation cannot be performed unless the device is acquired. 
   case DIERR_NOTACQUIRED:

     // acquire the device
     hRes = IDirectInputDevice2_Acquire(lpdiJoystick[joysticknum]);

     // must be a deeper issue
     if ( hRes != DI_OK )
       return false;

     // try again
     goto state;

     break;

   case DIERR_INVALIDPARAM:
   case DIERR_NOTINITIALIZED:
      
     // must be a deeper issue
     return false;
     break;

   // Data is not yet available. 
   case E_PENDING:
     // no data just say ok and get out of here
     return true;
     break;

   }

povs:

   /* who knows what the rest of this does */

   // for each hat switch
   for (i = 0; i < JoystickInfo[joysticknum].NumPOVs; i++)
   {
     povdir = GetPOVMask( js[ new_input ][ joysticknum ].rgdwPOV[ i ] );
     for (j = 0; j < MAX_POV_DIRECTIONS; j++)
		js_pov[ new_input ][ joysticknum ][ i ][ j ] =
        ( povdir & ( 1 << j ) ) ? 0x80 : 0;
   }

   /* everything fine */
   return true;
}

#else // ! DINPUTJOY

void SetUpJoystickAxis(int joystick)
{
}

bool joystick_poll( int joysticknum )
{
	int i;

	if(joysticknum >= Num_Joysticks)
		return false;

	// copy joystick button state
	for( i = 0; i < MAX_JOYSTICK_BUTTONS; i++ )
	{
		js_buttons[new_input][joysticknum][i] = 
			joy_button_state[joysticknum][i];
	}

	// copy hat state
	for( i = 0; i < MAX_JOYSTICK_POVS; i++ )
	{
		int b;
		for( b = 0; b < MAX_POV_DIRECTIONS; b++ )
		{
			js_pov[new_input][joysticknum][i][b] =
				joy_hat_state[joysticknum][i][b];
		}
	}

	return true;
}

#endif // !  DINPUTJOY

void ReadJoystickInput(SHIPCONTROL *ctrl, int joysticknum)
{
   int  ShipAction, axis;
   float amount;

   JOYSTICKAXIS *joyaxis;

#ifdef DINPUTJOY

	// this is a good thing cause sdl also considers a slider an axis
	long *axisptr[MAX_JOYSTICK_AXIS] =
	{
		&js[ new_input ][joysticknum].lX,  // 0
		&js[ new_input ][joysticknum].lY,  // 1
		&js[ new_input ][joysticknum].lZ,  // 2
		&js[ new_input ][joysticknum].lRx, // 3
		&js[ new_input ][joysticknum].lRy, // 4
		&js[ new_input ][joysticknum].lRz, // 5
		&js[ new_input ][joysticknum].rglSlider[0], // 6
		&js[ new_input ][joysticknum].rglSlider[1]  // 7
	};

	if( !lpdiJoystick[joysticknum] )
		return;

#else // ! DINPUTJOY

	if( joysticknum >= Num_Joysticks )
		return;

#endif


	for (axis = 0; axis < MAX_JOYSTICK_AXIS; axis++)
	{

#ifdef DINPUTJOY
			amount = (float) *axisptr[ axis ];
#else
			amount = (float) joy_axis_state[ joysticknum ][ axis ];
#endif

		if ((JoystickInfo[joysticknum].Axis[axis].exists) && amount)
		{
			/* the axis were looking at */
			joyaxis = &JoystickInfo[ joysticknum ].Axis[ axis ];

			/* the action it performs */
			ShipAction = joyaxis->action;

			/* continue if the action is nothing */
			if ( ShipAction == SHIPACTION_Nothing )
				continue;

			/* amount of movement detected */
			amount = amount * joyaxis->sensitivity;

			/* if were using fine control */
			if ( joyaxis->fine )
				amount *= (float) fabs( amount );

			/* if the axis is inverted */
			if ( joyaxis->inverted )
				amount  = -amount;

			/* perform the action */
			DoShipAction( ctrl, ShipAction, framelag * amount );
		}
	}
}

/*===================================================================
  Procedure : Check if any buttons of a specific joystick are
        : pressed
  Input   : int     Joystick Number
  Output    : bool    true/false
===================================================================*/
bool IsJoystickButtonPressed( int joysticknum )
{
  int i;

#ifdef DINPUTJOY
  if( !lpdiJoystick[joysticknum] )
    return( false );
#else
  if( joysticknum >= Num_Joysticks )
	  return false;
#endif

  for( i = 0; i < JoystickInfo[joysticknum].NumButtons; i++)
  {
    if ( JOYSTICK_BUTTON_PRESSED( joysticknum, i ) )
    {
      return( true );
    }
  }

  return( false );
}

/*===================================================================
  Procedure : Check if any buttons of a specific joystick are
        : released
  Input   : int     Joystick Number
  Output    : bool    true/false
===================================================================*/
bool IsJoystickButtonReleased( int joysticknum )
{
  int i;

#ifdef DINPUTJOY
  if( !lpdiJoystick[joysticknum] )
    return( false );
#else
  if( joysticknum >= Num_Joysticks )
	  return false;
#endif

  for( i = 0; i < JoystickInfo[joysticknum].NumButtons; i++)
  {
    if ( JOYSTICK_BUTTON_RELEASED( joysticknum, i ) )
    {
      return( true );
    }
  }

  return( false );
}

/*===================================================================
  Procedure : Check if any buttons of a any connected joystick
        : are pressed
  Input   : Nothing
  Output    : bool    true/false
===================================================================*/
bool IsAnyJoystickButtonPressed( void )
{
  int joystick;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    if( IsJoystickButtonPressed( joystick ) )
    {
      return( true );
    }
  }
  return( false );
}

/*===================================================================
  Procedure : Check if any buttons of a any connected joystick
        : are released
  Input   : Nothing
  Output    : bool    true/false
===================================================================*/
bool IsAnyJoystickButtonReleased( void )
{
  int joystick;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    if( IsJoystickButtonReleased( joystick ) )
    {
      return( true );
    }
  }
  return( false );
}
