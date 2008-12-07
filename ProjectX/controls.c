/* handle keyboard and mouse control */

#include <math.h>
#include <dplay.h>
#include <dplobby.h>
#include "windows.h"
#include "typedefs.h"
#include "dinput.h"
#include "config.h"
#include "title.h"
#include "controls.h"
#include <stdio.h>
#include "text.h"
#include "d3dmain.h"
#include "main.h"
#include "compobjects.h"
#include "quat.h"
#include "object.h"
#include "mydplay.h"
#include "primary.h"
#include "secondary.h"
#include "sfx.h"
#include "d3dappi.h"

#if 1

// TEMP!! - for testing looping SFX
#include "sfx.h"
#include "dplay.h"
#include "compobjects.h"
#include "quat.h"
#include "object.h"
#include "mydplay.h"
#include "ships.h"

extern GLOBALSHIP Ships[];

#endif
extern MENU MENU_NEW_Battle;
extern MENU *CurrentMenu;

extern  BOOL  Cheated;
extern BOOL WaitingToQuit;
extern BOOL CheatsDisabled;
extern  BOOL  MouseInput;
extern  BOOL  JoystickInput;

extern  BYTE  MyGameStatus;
extern  float framelag;  
extern  float framelag2;  
extern  float NitroFuel;
extern USERCONFIG *player_config;
extern int  Num_Joysticks;

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
extern BOOL DebugInfo;

extern  int8  PrimaryToFireLookup[ MAXPRIMARYWEAPONS ];
extern  BOOL  PrimaryWeaponCheat;
extern  int8  SecondaryToFireLookup[ MAXSECONDARYWEAPONS ];
extern  BOOL  SecondaryWeaponCheat;
extern  BOOL  GodMode;
extern  BOOL LevelSelectMode;

extern  int FontHeight;

int NakedGirls( char *cheat );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Some Keyboard and Mouse Globals
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

#define MAX_KEYS      (256)

#define MOUSE_ZSTEP     (120)

#define MAX_MOUSE_DELTA_X (40.0F)
#define MAX_MOUSE_DELTA_Y (40.0F)

#define MouseXFactor    (  1.0F / ( MAX_MOUSE_DELTA_X * MAX_MOUSE_DELTA_X ) )
#define MouseYFactor    ( -1.0F / ( MAX_MOUSE_DELTA_Y * MAX_MOUSE_DELTA_Y ) )




#define KEY_HELD( K )     ( KeyState[ new_input ][ K ] & 0x80 )
#define KEY_PRESSED( K )    ( !( KeyState[ old_input ][ K ] & 0x80) && ( KeyState[ new_input ][ K ] & 0x80 ) )
#define KEY_RELEASED( K )   ( ( KeyState[ old_input ][ K ] & 0x80) && !( KeyState[ new_input ][ K ] & 0x80 ) )


#define MOUSE_BUTTON_HELD( B )    ( MouseState[ new_input ].rgbButtons[ B ] & 0x80 )
#define MOUSE_BUTTON_PRESSED( B ) ( !( MouseState[ old_input ].rgbButtons[ B ] & 0x80 ) && ( MouseState[ new_input ].rgbButtons[ B ] & 0x80 ) )
#define MOUSE_BUTTON_RELEASED( B )  ( ( MouseState[ old_input ].rgbButtons[ B ] & 0x80 ) && !( MouseState[ new_input ].rgbButtons[ B ] & 0x80 ) )


#define MOUSE_WHEEL_UP()          ( MouseState[ new_input ].lZ > 0 )
#define MOUSE_WHEEL_DOWN()          ( MouseState[ new_input ].lZ < 0 )
#define MOUSE_WHEEL_UP_PRESSED()      ( !( MouseState[ old_input ].lZ > 0 ) && ( MouseState[ new_input ].lZ > 0 ) )
#define MOUSE_WHEEL_UP_RELEASED()     ( ( MouseState[ old_input ].lZ > 0 ) && !( MouseState[ new_input ].lZ > 0 ) )
#define MOUSE_WHEEL_DOWN_PRESSED()      ( !( MouseState[ old_input ].lZ < 0 ) && ( MouseState[ new_input ].lZ < 0 ) )
#define MOUSE_WHEEL_DOWN_RELEASED()     ( ( MouseState[ old_input ].lZ < 0 ) && !( MouseState[ new_input ].lZ < 0 ) )


#define JOYSTICK_BUTTON_HELD( J, B )    ( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 )
#define JOYSTICK_BUTTON_PRESSED( J, B )   ( !( js[ old_input ][ J ].rgbButtons[ B ] & 0x80) && ( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 ) )
#define JOYSTICK_BUTTON_RELEASED( J, B )  ( ( js[ old_input ][ J ].rgbButtons[ B ] & 0x80) && !( js[ new_input ][ J ].rgbButtons[ B ] & 0x80 ) )

#define JOYSTICK_POV_HELD( J, P, D )    ( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 )
#define JOYSTICK_POV_PRESSED( J, P, D )   ( !( js_pov[ old_input ][ J ][ P ][ D ] & 0x80 ) && ( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 ) )
#define JOYSTICK_POV_RELEASED( J, P, D )  ( ( js_pov[ old_input ][ J ][ P ][ D ] & 0x80 ) && !( js_pov[ new_input ][ J ][ P ][ D ] & 0x80 ) )

extern VIRTUALKEYMAP vkey_map[];

int GetPOVMask( DWORD pov );

BOOL CheatsEnabled = FALSE;

DIDEVICEOBJECTDATA rgod[DINPUT_BUFFERSIZE]; /* Receives buffered data */
DWORD BufferedKey[ DINPUT_BUFFERSIZE ];
int16 NumKeysToProcess;
#define TOTAL_JOYSTICK_ACTIONS  140 // 5 axis positions, 3 axis rotations, 4 POV hats and 128 buttons!
#define TOTAL_JOYSTICK_AXIS   8 // 5 axis positions, 3 axis rotations
DIJOYSTATE2   js[ INPUT_BUFFERS ][ MAX_JOYSTICKS ];
BYTE      js_pov[ INPUT_BUFFERS ][ MAX_JOYSTICKS ][ MAX_JOYSTICK_POVS ][ MAX_POV_DIRECTIONS ];
JOYSTICKINFO  JoystickInfo[MAX_JOYSTICKS];

extern LPDIRECTINPUTDEVICE lpdiMouse;
extern LPDIRECTINPUTDEVICE lpdiKeyboard;
extern LPDIRECTINPUTDEVICE lpdiBufferedKeyboard;
extern LPDIRECTINPUTDEVICE2 lpdiJoystick[MAX_JOYSTICKS];
extern MENU MENU_QuickTextSend;
extern MENU MENU_QuickTextSendWhisper;
extern MENU MENU_NEW_StartSinglePlayer;

// (Sfx.c)
extern SendBikerTaunt();

BOOL flush_input = TRUE;

static uint16 old_input = 0;
uint16 new_input = 1;
static DIMOUSESTATE MouseState[ INPUT_BUFFERS ];
static uint8 KeyState[ INPUT_BUFFERS ][ MAX_KEYS ];

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

BOOL FullRearView = FALSE;
BOOL Headlights = FALSE;
BOOL ShowStats = FALSE;
BOOL ShowMessages = FALSE;	// display long list of chat messages
BOOL ShowStatistics = FALSE;	// display statistics in game

void FlashMenuText( char *text, float activetime, uint16 sfx )
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
      CenterPrint4x5Text( FlashText , d3dappi.szClient.cy - FontHeight * 2, 2 );
    }
  }
}

int EnableCheats( char *cheat )
{
  CheatsEnabled = TRUE;
  FlashMenuText( "cheat mode enabled", 120.0F, SFX_Secret );
  Cheated = TRUE;

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
    PrimaryWeaponCheat = FALSE;
    FlashMenuText( "Beam me down", 120.0F, SFX_Secret );
  }
  else
  {
    PrimaryWeaponCheat = TRUE;
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
    SecondaryWeaponCheat = FALSE;
  }
  else
  {
    SecondaryWeaponCheat = TRUE;
    FlashMenuText( "He works all night...", 120.0F, SFX_Secret );
  }
#endif
  return 1;
}

int ToggleGodMode( char *cheat )
{
  if( !GodMode )
  {
    GodMode = TRUE;
    FlashMenuText( "God mode enabled", 120.0F, SFX_Secret );
    GivemeAllWeapons();
  }
  else
  {
    FlashMenuText( "God mode disabled", 120.0F, SFX_Secret );
    LoseAllWeapons();
    GodMode = FALSE;
  }
  return 1;
}

int ToggleLevelSelectMode( char *cheat )
{
  if( !LevelSelectMode )
  {
    LevelSelectMode = TRUE;
    FlashMenuText( "Level select mode enabled", 120.0F, SFX_Secret );
  }
  else
  {
    LevelSelectMode = FALSE;
    FlashMenuText( "Level select mode disabled", 120.0F, SFX_Secret );
  }
  
  InitInGameMenu( &MENU_InGame );
  AllowLevelSelect( &MENU_NEW_StartSinglePlayer );

  return 1;
}

static struct
{
#if CHEATS_AS_PLAINTEXT
  unsigned char *cheatcode;
#else
  unsigned char cheatcode[ 16 ];
#endif
  int (*cheatfunc)( char * );
  BOOL allowmultiplayer;
  int next;
} CheatTable[] = 
{
#if CHEATS_AS_PLAINTEXT
  { "BUBBLES", EnableCheats, TRUE },
  { "TITSOOT", NakedGirls, TRUE },
  { "JIMBEAM", JimBeam, FALSE },
  { "LUMBERJACK", Lumberjack, FALSE },
  { "IAMZEUS", ToggleGodMode, FALSE },
  { "FULLMONTY", ToggleLevelSelectMode, FALSE },
#else
  { { DIK_B, DIK_U, DIK_B, DIK_B, DIK_L, DIK_E, DIK_S, 0 }, EnableCheats, TRUE },
  { { DIK_T, DIK_I, DIK_T, DIK_S, DIK_O, DIK_O, DIK_T, 0 }, NakedGirls, TRUE },
  { { DIK_J, DIK_I, DIK_M, DIK_B, DIK_E, DIK_A, DIK_M, 0 }, JimBeam, FALSE },
  { { DIK_L, DIK_U, DIK_M, DIK_B, DIK_E, DIK_R, DIK_J, DIK_A, DIK_C, DIK_K, 0 }, Lumberjack, FALSE },
  { { DIK_I, DIK_A, DIK_M, DIK_Z, DIK_E, DIK_U, DIK_S, 0 }, ToggleGodMode, FALSE },
  { { DIK_F, DIK_U, DIK_L, DIK_L, DIK_M, DIK_O, DIK_N, DIK_T, DIK_Y, 0 }, ToggleLevelSelectMode, FALSE },
#endif
};
// NOTE: add any new cheats to DisableCheats function to ensure they are not active in multiplayer game

VirtualKeycode asckey[ 256 ];


int ShipAxisLookup[NUM_SHIP_AXIS_ACTIONS] = { SHIPACTION_Nothing, SHIPACTION_RotateUp, SHIPACTION_RotateLeft, SHIPACTION_RollLeft,
                        SHIPACTION_SlideUp, SHIPACTION_SlideLeft, SHIPACTION_MoveForward };
#if 0

// Test SFX stuff...
VECTOR TestSfxPos;
uint16 TestSfxGroup;
extern GLOBALSHIP Ships[MAX_PLAYERS+1];

#endif

char *JoystickPOVDirections[MAX_POV_DIRECTIONS] = { "Up", "Down", "Left", "Right" };

HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty,
                   DWORD dwObject, DWORD dwHow, DWORD dwValue);

BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
void DebugPrintf( const char * format, ... );
#ifdef USEINLINE
_inline
#endif
 short
key_pressed( USERKEY *k )
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
      case DIK_LBUTTON:
      case DIK_RBUTTON:
      case DIK_MBUTTON:
      case DIK_TBUTTON:
        if ( MOUSE_BUTTON_PRESSED( c - DIK_LBUTTON ) )
          return 1;
        break;
      case DIK_WHEELUP:
        if ( MOUSE_WHEEL_UP_PRESSED() )
          return 1;
        break;
      case DIK_WHEELDOWN:
        if ( MOUSE_WHEEL_DOWN_PRESSED() )
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

#ifdef USEINLINE
_inline
#endif
 short
key_released( USERKEY *k )
{
  int j;
  VirtualKeycode c;

  for ( j = 0; j < k->keys; j++ )
  {
    c = k->key[ j ];
    if ( KEY_ON_KEYBOARD( c ) )
    {
      if ( KEY_RELEASED( c ) )
        return 1;
    }
    else if ( KEY_ON_MOUSE( c ) )
    {
      switch( c )
      {
      case DIK_LBUTTON:
      case DIK_RBUTTON:
      case DIK_MBUTTON:
      case DIK_TBUTTON:
        if ( MOUSE_BUTTON_RELEASED( c - DIK_LBUTTON ) )
          return 1;
        break;
      case DIK_WHEELUP:
        if ( MOUSE_WHEEL_UP_RELEASED() )
          return 1;
        break;
      case DIK_WHEELDOWN:
        if ( MOUSE_WHEEL_DOWN_RELEASED() )
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
        if ( JOYSTICK_BUTTON_RELEASED( joystick, button ) )
          return 1;
      }
      else if ( KEY_ON_JOYSTICK_POV( c ) )
      {
        int pov, dir;

        pov = KEY_JOYSTICK_POV( c );
        dir = KEY_JOYSTICK_POVDIR( c );
        if ( JOYSTICK_POV_RELEASED( joystick, pov, dir ) )
          return 1;
      }
    }
  }
  return 0;
}


#ifdef USEINLINE
_inline
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
      case DIK_LBUTTON:
      case DIK_RBUTTON:
      case DIK_MBUTTON:
      case DIK_TBUTTON:
        if ( MOUSE_BUTTON_HELD( c - DIK_LBUTTON ) )
          return 1;
        break;
      case DIK_WHEELUP:
        if ( MOUSE_WHEEL_UP() )
          return 1;
        break;
      case DIK_WHEELDOWN:
        if ( MOUSE_WHEEL_DOWN() )
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



static void InitAscKey( void )
{
  int j, k;
  char *keyword;

  for ( j = 0; j < 256; j++ )
  {
    asckey[ j ] = 0;
  }
  for ( k = 0; vkey_map[ k ].keyword; k++ )
  {
    keyword = vkey_map[ k ].keyword;
    if ( strstr( keyword, "DIK_" ) == keyword &&
      strlen( keyword ) == 5 &&
      !asckey[ keyword[ 4 ] ] )
    {
      asckey[ keyword[ 4 ] ] = vkey_map[ k ].keycode;
    }
  }
}

void DisableCheats( void )
{
  CheatsEnabled = FALSE;
  DebugInfo = FALSE;

  // used to indicate cheats disabled for multiplayer game...
  CheatsDisabled = TRUE;

  if( PrimaryWeaponCheat )
    JimBeam( NULL );
  if( SecondaryWeaponCheat )
    Lumberjack( NULL );
  if( GodMode )
    ToggleGodMode( NULL );
  /*
  no need to disable level select mode
  if( LevelSelectMode )
    ToggleLevelSelectMode( NULL );
  */
}

void CheckCheats( VirtualKeycode key )
{
  int j, i;
  int cheats;
  unsigned char ch;
#if CHEATS_AS_PLAINTEXT
  static int init = 0;
#endif

#if CHEATS_AS_PLAINTEXT
  if ( !init )
  {
    InitAscKey();
    init = 1;
  }
#endif

  cheats = sizeof( CheatTable ) / sizeof( CheatTable[ 0 ] );
  if ( key )
  {
    for ( j = 0; j < cheats; j++ )
    {
      ch = CheatTable[ j ].cheatcode[ CheatTable[ j ].next ];
#if CHEATS_AS_PLAINTEXT
      if ( key == asckey[ ch ] )
#else
      if ( key == ch )
#endif
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


static void ReadKeyboard( int dup_last )
{
  int j;

  if ( dup_last )
  {
    for ( j = 0; j < MAX_KEYS; j++ )
      KeyState[ new_input ][ j ] = KeyState[ old_input ][ j ];
  }
  else if( !lpdiKeyboard || IDirectInputDevice_GetDeviceState( lpdiKeyboard, sizeof( KeyState[ 0 ] ), KeyState[ new_input ] ) != DI_OK || flush_input )
    {
        // failed to read the keyboard
    for ( j = 0; j < MAX_KEYS; j++ )
      KeyState[ new_input ][ j ] = 0;
    }
}

void ReadBufferedKeyboard( void )
{
    uint16 i;
  
  if (lpdiBufferedKeyboard) 
  {

        DWORD cod;
        HRESULT hr;
    DIDEVICEOBJECTDATA keyinfo;
again:;
        cod = DINPUT_BUFFERSIZE;
        hr = lpdiBufferedKeyboard->lpVtbl->GetDeviceData(lpdiBufferedKeyboard, sizeof(DIDEVICEOBJECTDATA),
                                        rgod, &cod, 0);
        if (hr != DI_OK) 
    {

            /* << insert recovery code here if you need any >> */

            if (hr == DIERR_INPUTLOST) 
      {
          hr = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);
                if (SUCCEEDED(hr)) 
        {
                    goto again;
                }
            }
        }
        /*
         *  In order for it to be worth our while to parse the
         *  buffer elements, the GetDeviceData must have succeeded,
         *  and we must have received some data at all.
         */

    /*
    if (SUCCEEDED(hr) && cod > 0) 
    {
      keyinfo = rgod[0];
      
      if (keyinfo.dwData & 0x80)    // only set for key down
        BufferedKey = keyinfo.dwOfs;
      else
        BufferedKey = 0;
        }
    else
      BufferedKey = 0;
    */
    NumKeysToProcess = 0;

    if ( SUCCEEDED ( hr ) ) 
    {
      for ( i = 0; i < cod; i++ )
      {
        keyinfo = rgod[ i ];

        if ( keyinfo.dwData & 0x80 )
        {
          BufferedKey[ NumKeysToProcess++ ] = keyinfo.dwOfs;
        }
      }
    }
    }
}

static void ReadMouse( int dup_last )
{
	int j;
	HRESULT hr;

	if ( !MouseInput || !lpdiMouse )
		goto fail;

	if ( dup_last )
	{
		MouseState[ new_input ] = MouseState[ old_input ];
		if ( MOUSE_WHEEL_UP()   )	MouseState[ new_input ].lZ--;
		if ( MOUSE_WHEEL_DOWN() )	MouseState[ new_input ].lZ++;
		return;
	}

	hr = IDirectInputDevice_GetDeviceState( lpdiMouse, sizeof(DIMOUSESTATE), &MouseState[ new_input ] );

	switch (hr)
	{
	case DI_OK:
#if 1
		if ( MouseState[ new_input ].lZ > 0 )		MouseState[ new_input ].lZ =  1;
		else if ( MouseState[ new_input ].lZ < 0 )	MouseState[ new_input ].lZ = -1;
#else
		MouseState[ new_input ].lZ /= MOUSE_ZSTEP;
#endif
		break;
	case DIERR_INPUTLOST:
		// re-aquire mouse
		hr = IDirectInputDevice_Acquire(lpdiMouse);
		goto fail;
	default:
		goto fail;
	}

	return;

// failed to read the mouse
fail:

	MouseState[ new_input ].lX = 0;
	MouseState[ new_input ].lY = 0;
	MouseState[ new_input ].lZ = 0;

	for ( j = 0; j < 4; j++ )
		MouseState[ new_input ].rgbButtons[ j ] = 0;

}

float framelagfix = 0.0F;

void ReadInput( void )
{
  int i;

  if ( WaitingToQuit ) return;

  old_input = new_input;
  new_input++;
  if ( new_input >= INPUT_BUFFERS )
    new_input = 0;

  framelagfix -= framelag2;
  
  /* read mouse input */
  if( framelagfix <= 0.0F )
  {
    ReadMouse( 0 );
    framelagfix = 2.0F;
  }
  else
  {
    ReadMouse( 1 );
  }

  /* read keyboard input */
  ReadKeyboard( 0 );

  /* read joysticks inputs */
  for (i = 0; i < Num_Joysticks; i++)
    if (JoystickInfo[i].connected)
      PollJoystick(i);

  flush_input = FALSE;
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

#ifdef MULTIPLE_READINPUTS
  ReadInput();
#endif

  if ( CurrentMenu )
    return; // disable bike controls if using menus

  mouse_dx = MouseState[ new_input ].lX * conf->mouse_x_sensitivity * 4.0F;
  mouse_dy = MouseState[ new_input ].lY * conf->mouse_y_sensitivity * 4.0F;

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

//  mouse_dx = mouse_dx * MouseXFactor;
//  mouse_dy = mouse_dy * MouseYFactor;
  mouse_dx = mouse_dx * (float) fabs( mouse_dx ) * MouseXFactor;
  mouse_dy = mouse_dy * (float) fabs( mouse_dy ) * MouseYFactor;

  if ( mouse_dx < -framelag )
    mouse_dx = -framelag;
  if ( mouse_dx > framelag )
    mouse_dx = framelag;
  if ( mouse_dy < -framelag )
    mouse_dy = -framelag;
  if ( mouse_dy > framelag )
    mouse_dy = framelag;

  pitch_sign = ( conf->invert_pitch ) ? -1.0F : 1.0F;
  turn_sign = ( conf->invert_turn ) ? -1.0F : 1.0F;

  if ( key_held( &conf->full_rear_view ) )
    FullRearView = TRUE;
  else
    FullRearView = FALSE;

  if( key_held( &conf->show_messages))
	  ShowMessages = TRUE;
  else
	  ShowMessages = FALSE;

  if( key_held( &conf->show_stats))
	  ShowStatistics = TRUE;
  else
	  ShowStatistics = FALSE;

  if ( key_pressed( &conf->headlights ) )
    Headlights = !Headlights;

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

  if ( ( IsKeyHeld( DIK_LCONTROL ) || IsKeyHeld( DIK_RCONTROL ) ) && ( key_pressed( &conf->send_msg ) ) && MyGameStatus == STATUS_Normal )
  {
    MenuRestart( &MENU_QuickTextSendWhisper );
  }else if ( ( key_pressed( &conf->send_msg ) ) && MyGameStatus == STATUS_Normal )
  {
    MenuRestart( &MENU_QuickTextSend );
  }
#ifdef PLAYER_SPEECH_TAUNTS
  if( key_pressed( &conf->send_speech )  )
  {
    PlaySpecificBikerSpeech( SFX_BIKER_TN, Ships[ WhoIAm ].Object.Group, &Ships[WhoIAm].Object.Pos, 0.0F, player_config->bike, -1, FALSE ); // don't update
    if( MyGameStatus == STATUS_Normal )
    {
		SendBikerTaunt(); // (Sfx.c)
      //SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_SpeechTaunt, 0);
    }
  }
#endif
  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    if (JoystickInfo[joystick].connected && JoystickInfo[joystick].assigned)
      ReadJoystickInput(ctrl, joystick);
  }

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


int AnyKeyPressed( void )
{
  int k;
  static VirtualKeycode null_key[] =
  {
#if 0
    DIK_ESCAPE,
    DIK_F1,
    DIK_F2,
    DIK_F3,
    DIK_F4,
    DIK_F5,
    DIK_F6,
    DIK_F7,
    DIK_F8,
    DIK_F9,
    DIK_F10,
    DIK_F11,
    DIK_F12,
    DIK_F13,
    DIK_F14,
    DIK_F15,
#endif
    (VirtualKeycode) -1,
  };

#ifdef MULTIPLE_READINPUTS
  ReadInput();
#endif

  if ( CurrentMenu )
    return 0;

  for ( k = 0; null_key[ k ] != (VirtualKeycode) -1; k++ )
  {
    if ( KEY_PRESSED( null_key[ k ] ) )
      return 0;
  }

  for ( k = 0; k < MAX_KEYS; k++ )
  {
    if ( KEY_PRESSED( k ) )
      return 1;
  }
  for ( k = 0; k < MAX_MOUSE_BUTTONS; k++ )
  {
    if ( MOUSE_BUTTON_PRESSED( k ) )
      return 1;
  }

  if( IsAnyJoystickButtonPressed() )
  {
    return 1;
  }

  return 0;
}


int AnyKeyReleased( void )
{
  int k;
  static VirtualKeycode null_key[] =
  {
    DIK_ESCAPE,
    DIK_F1,
    DIK_F2,
    DIK_F3,
    DIK_F4,
    DIK_F5,
    DIK_F6,
    DIK_F7,
    DIK_F8,
    DIK_F9,
    DIK_F10,
    DIK_F11,
    DIK_F12,
    DIK_F13,
    DIK_F14,
    DIK_F15,
    (VirtualKeycode) -1,
  };

#ifdef MULTIPLE_READINPUTS
  ReadInput();
#endif

  if ( CurrentMenu )
    return 0;

  for ( k = 0; null_key[ k ] != (VirtualKeycode) -1; k++ )
  {
    if ( KEY_RELEASED( null_key[ k ] ) )
      return 0;
  }

  for ( k = 0; k < MAX_KEYS; k++ )
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


int IsKeyPressed( int di_keycode )
{
  switch( di_keycode )
  {
  case DIK_LBUTTON:
    if ( MOUSE_BUTTON_PRESSED( 0 ) )
      return 1;
    break;
  case DIK_RBUTTON:
    if ( MOUSE_BUTTON_PRESSED( 1 ) )
      return 1;
    break;
  case DIK_MBUTTON:
    if ( MOUSE_BUTTON_PRESSED( 2 ) )
      return 1;
    break;
  default:
    if ( KEY_PRESSED( di_keycode ) )
      return 1;
  }
  return 0;
}


int IsKeyHeld( int di_keycode )
{
  switch( di_keycode )
  {
  case DIK_LBUTTON:
    if ( MOUSE_BUTTON_HELD( 0 ) )
      return 1;
    break;
  case DIK_RBUTTON:
    if ( MOUSE_BUTTON_HELD( 1 ) )
      return 1;
    break;
  case DIK_MBUTTON:
    if ( MOUSE_BUTTON_HELD( 2 ) )
      return 1;
    break;
  case DIK_WHEELUP:
    if ( MOUSE_WHEEL_UP() )
      return 1;
    break;
  case DIK_WHEELDOWN:
    if ( MOUSE_WHEEL_DOWN() )
      return 1;
    break;
  default:
    if ( KEY_HELD( di_keycode ) )
      return 1;
  }
  return 0;
}

int CheckMouse( void )
{
  int k;
  int key;

  key = 0;
  for ( k = 0; k < MAX_MOUSE_BUTTONS; k++ )
  {
    if ( MOUSE_BUTTON_PRESSED( k ) )
    {
      if ( !key )
        key = DIK_LBUTTON + k;
      else
        return 0;
    }
  }
  if ( MOUSE_WHEEL_UP() )
  {
    if ( !key )
      key = DIK_WHEELUP;
    else
      return 0;
  }
  if ( MOUSE_WHEEL_DOWN() )
  {
    if ( !key )
      key = DIK_WHEELDOWN;
    else
      return 0;
  }

  return key;
}

int WhichKeyPressed( void )
{
  int k;
  for ( k = 0; k < MAX_KEYS; k++ )
    if ( KEY_PRESSED( k ) )
		return k;
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
        key = DIK_LBUTTON + k;
      else
        return 0;
    }
  }
  if ( MOUSE_WHEEL_UP_PRESSED() )
  {
    if ( !key )
      key = DIK_WHEELUP;
    else
      return 0;
  }
  if ( MOUSE_WHEEL_DOWN_PRESSED() )
  {
    if ( !key )
      key = DIK_WHEELDOWN;
    else
      return 0;
  }

  return key;
}


int WhichJoystickPressed( void )
{
  int k;
  int key;

  key = 0;
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


int WhichKeyHeld( void )
{
  int k;
  int key;

  key = 0;
  for ( k = 0; k < MAX_KEYS; k++ )
  {
    if ( KEY_HELD( k ) )
    {
      if ( !key )
        key = k;
      else
        return 0;
    }
  }
  for ( k = 0; k < MAX_MOUSE_BUTTONS; k++ )
  {
    if ( MOUSE_BUTTON_HELD( k ) )
    {
      if ( !key )
        key = DIK_LBUTTON + k;
      else
        return 0;
    }
  }
  if ( MOUSE_WHEEL_UP() )
  {
    if ( !key )
      key = DIK_WHEELUP;
    else
      return 0;
  }
  if ( MOUSE_WHEEL_DOWN() )
  {
    if ( !key )
      key = DIK_WHEELDOWN;
    else
      return 0;
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

/*--------------------------------------------------------------------------
| Requests joystick data and performs any needed processing.
*-------------------------------------------------------------------------*/
BOOL PollJoystick( int joysticknum )
{

   HRESULT hRes;
   int i, j, povdir;

   /* this global is changed by a menu toggle */
   if ( !JoystickInput )
     return TRUE;

   /* joystick doesn't exist */
   if( !lpdiJoystick[joysticknum] )
     return FALSE;

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
       return FALSE;

     // try again
     goto poll;

     break;

   // The object has not been initialized. 
   case DIERR_NOTINITIALIZED:

     // must be a deeper issue
     return FALSE;
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
       return FALSE;

     // try again
     goto state;

     break;

   case DIERR_INVALIDPARAM:
   case DIERR_NOTINITIALIZED:
      
     // must be a deeper issue
     return FALSE;
     break;

   // Data is not yet available. 
   case E_PENDING:
     // no data just say ok and get out of here
     return TRUE;
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
   return TRUE;

}

/*--------------------------------------------------------------------------
|
| SetDIDwordProperty
|
| Set a DWORD property on a DirectInputDevice.
|
*-------------------------------------------------------------------------*/
HRESULT
SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty,
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

void SetUpJoystickAxis(int joystick)
{
  DIPROPRANGE diprg;
  BOOL DeadzoneNotSet = FALSE;
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
    if(lpdiJoystick[joystick]->lpVtbl->SetProperty(lpdiJoystick[joystick], DIPROP_RANGE, &diprg.diph) != DI_OK)
    {
      JoystickInfo[joystick].Axis[AXIS_XAxis].exists = FALSE; // cannot set range, therefore do not allow axis
      DebugPrintf( "SetUpJoystickAxis: failed to set range for X axis\n" );
    }else
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
        DeadzoneNotSet = TRUE;
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
      JoystickInfo[joystick].Axis[AXIS_YAxis].exists = FALSE; // cannot set range, therefore do not allow axis
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
        DeadzoneNotSet = TRUE;
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
      JoystickInfo[joystick].Axis[AXIS_ZAxis].exists = FALSE; // cannot set range, therefore do not allow axis
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
        DeadzoneNotSet = TRUE;
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
      JoystickInfo[joystick].Axis[AXIS_RxAxis].exists = FALSE;  // cannot set range, therefore do not allow axis
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
        DeadzoneNotSet = TRUE;
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
      JoystickInfo[joystick].Axis[AXIS_RyAxis].exists = FALSE;  // cannot set range, therefore do not allow axis
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
        DeadzoneNotSet = TRUE;
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
      JoystickInfo[joystick].Axis[AXIS_RzAxis].exists = FALSE;  // cannot set range, therefore do not allow axis
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
        DeadzoneNotSet = TRUE;
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
        JoystickInfo[joystick].Axis[AXIS_SliderAxis0 + i].exists = FALSE; // cannot set range, therefore do not allow axis
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
          DeadzoneNotSet = TRUE;
          DebugPrintf( "SetUpJoystickAxis: failed to set deadzone for Slider%d axis\n", i );
        }
      }
    }
  }

  // could do something about unset deadzones here...
  if (DeadzoneNotSet)
    DeadzoneNotSet = FALSE;
}

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

// returns the position of the least sig. bit...
int BitPos (int flag)
{
  int i = 0;
  int mask = 1;

  if (flag == 0)
    return 0;

  while (!(mask & flag))  
  {
    flag >>= 1;
    i++;
  }

  return i;
}

// returns TRUE if it is OK to repeat the given ship action
static BOOL RepeatShipActionOK ( int action )
{
  switch (action)
  {
  case SHIPACTION_RotateUp:
  case SHIPACTION_RotateDown:
  case SHIPACTION_RotateLeft:
  case SHIPACTION_RotateRight:
  case SHIPACTION_RollLeft:
  case SHIPACTION_RollRight:
  case SHIPACTION_SlideUp:
  case SHIPACTION_SlideDown:
  case SHIPACTION_SlideLeft:
  case SHIPACTION_SlideRight:
  case SHIPACTION_MoveForward:
  case SHIPACTION_MoveBack:
  case SHIPACTION_Turbo:
  case SHIPACTION_FirePrimary:
  case SHIPACTION_FireSecondary:
    return TRUE;
  default: 
    return FALSE;
  }
}

void ReadJoystickInput(SHIPCONTROL *ctrl, int joysticknum)
{

   int  ShipAction, axis;
   float amount;

   JOYSTICKAXIS *joyaxis;

   LONG *axisptr[MAX_JOYSTICK_AXIS] =
   {
    &js[ new_input ][joysticknum].lX,
    &js[ new_input ][joysticknum].lY,
    &js[ new_input ][joysticknum].lZ, 
    &js[ new_input ][joysticknum].lRx,
    &js[ new_input ][joysticknum].lRy,
    &js[ new_input ][joysticknum].lRz,
    &js[ new_input ][joysticknum].rglSlider[0],
    &js[ new_input ][joysticknum].rglSlider[1]
   };

   /* joystick is disabled by menu toggle */
   if ( !JoystickInput )
     return;

   if( !lpdiJoystick[joysticknum] )
    return;

  for (axis = 0; axis < MAX_JOYSTICK_AXIS; axis++){
    if ((JoystickInfo[joysticknum].Axis[axis].exists) && (*axisptr[axis])){

      /* the axis were looking at */
      joyaxis = &JoystickInfo[ joysticknum ].Axis[ axis ];

      /* the action it performs */
      ShipAction = joyaxis->action;

      /* continue if the action is nothing */
      if ( ShipAction == SHIPACTION_Nothing ) continue;

      /* amount of movement detected */
      amount = (float) *axisptr[ axis ] * joyaxis->sensitivity;

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

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Check if any buttons of a specific joystick are
        : pressed
  Input   : int     Joystick Number
  Output    : BOOL    TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL IsJoystickButtonPressed( int joysticknum )
{
  int i;

  if( !lpdiJoystick[joysticknum] )
    return( FALSE );

  for( i = 0; i < JoystickInfo[joysticknum].NumButtons; i++)
  {
    if ( JOYSTICK_BUTTON_PRESSED( joysticknum, i ) )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Check if any buttons of a specific joystick are
        : released
  Input   : int     Joystick Number
  Output    : BOOL    TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL IsJoystickButtonReleased( int joysticknum )
{
  int i;

  if( !lpdiJoystick[joysticknum] )
    return( FALSE );

  for( i = 0; i < JoystickInfo[joysticknum].NumButtons; i++)
  {
    if ( JOYSTICK_BUTTON_RELEASED( joysticknum, i ) )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Check if any buttons of a any connected joystick
        : are pressed
  Input   : Nothing
  Output    : BOOL    TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL IsAnyJoystickButtonPressed( void )
{
  int joystick;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    if( IsJoystickButtonPressed( joystick ) )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Check if any buttons of a any connected joystick
        : are released
  Input   : Nothing
  Output    : BOOL    TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL IsAnyJoystickButtonReleased( void )
{
  int joystick;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    if( IsJoystickButtonReleased( joystick ) )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

