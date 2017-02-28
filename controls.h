#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include "main.h"
#include "config.h"

/*===================================================================
		Some Keyboard and Mouse Globals
===================================================================*/

#define CRUISE_NORMAL		(0)
#define CRUISE_FORWARD		(1)
#define CRUISE_NITRO		(2)
#define CRUISE_REVERSE		(-1)

#define MAXMOVESPEED	(45.0F*GLOBAL_SCALE)
#define MOVEACCELL		0.065F
#define MOVEDECELL		0.0611F
#define MAXTURBOSPEED   (MAXMOVESPEED * 2.0F)
#define TURBOACCELL		0.13F
#define TURBODECELL		0.1151F
#define MAXTURNSPEED	2.625F
#define TURNACCELL		0.065F
#define TURNDECELL		0.0611F
#define MAXROLLSPEED	1.875F
#define ROLLACCELL		0.25F
#define ROLLDECELL		0.2F 
#define MAXBANKANGLE    30.0F
#define BANKACCELL		0.03F
#define BANKDECELL		0.03F

#define CLAMP( V, MAX )\
	if ( (V) > (MAX) ) { (V) = (MAX);  }\
	else if ( (V) < -(MAX) ) { (V) = -(MAX); }

/* ship control structures */

typedef struct {
	float pitch;
	float yaw;
	float roll;
	float bank;
	float right;
	float up;
	float forward;
	int cruise_control;
	int turbo;
	int slide_mode;
	int roll_mode;
	int fire_primary;
	int fire_secondary;
	int fire_mine;
	int select_primary;			// #(1 to MAX_PRIMARY_WEAPONS) of primary weapon to select, or 0 otherwise
	int select_secondary;		// #(1 to MAX_SECONDARY_WEAPONS) of secondary weapon to select, or 0 otherwise
	int select_next_primary;
	int select_prev_primary;
	int select_next_secondary;
	int select_prev_secondary;
	int drop_primary;
	int drop_secondary;
	int drop_shield;
	int drop_ammo;
} SHIPCONTROL;

typedef struct {
	bool exists;
	char *name;
	int action;
	float sensitivity;
	int deadzone;	// percentage
	bool inverted;
	bool fine;
} JOYSTICKAXIS;

typedef struct {
	char *name;
	int action;
} JOYSTICKBTN;

#define MAX_POV_DIRECTIONS 4
#define POV_Centre	0
#define POV_Up		1
#define POV_Down	2
#define POV_Left	4
#define POV_Right	8

typedef struct {
	char *name;
	char *dirname[MAX_POV_DIRECTIONS];
	int action[MAX_POV_DIRECTIONS];
} JOYSTICKPOV;

enum
{
	AXIS_XAxis,
	AXIS_YAxis,
	AXIS_ZAxis,
	AXIS_RxAxis,
	AXIS_RyAxis,
	AXIS_RzAxis,
	AXIS_SliderAxis0,
	AXIS_SliderAxis1,
};					 

#define AXIS_Start AXIS_XAxis
#define AXIS_End AXIS_SliderAxis1

#define NUM_SHIP_ACTIONS		SHIPACTION_MAX
#define NUM_SHIP_AXIS_ACTIONS	7

enum {
	SHIPACTION_Nothing,				// 0
	SHIPACTION_RotateUp,			// 1
	SHIPACTION_RotateDown,			// 2
	SHIPACTION_RotateLeft,			// 3
	SHIPACTION_RotateRight,			// 4
	SHIPACTION_RollLeft,			// 5
	SHIPACTION_RollRight,			// 6
	SHIPACTION_SlideUp,				// 7
	SHIPACTION_SlideDown,			// 8
	SHIPACTION_SlideLeft,			// 9
	SHIPACTION_SlideRight,			// 10
	SHIPACTION_MoveBack,			// 11
	SHIPACTION_MoveForward,			// 12
	SHIPACTION_CruiseIncrease,		// 13
	SHIPACTION_CruiseDecrease,		// 14
	SHIPACTION_Turbo,				// 15
	SHIPACTION_FirePrimary,			// 16
	SHIPACTION_FireSecondary,		// 17
	SHIPACTION_DropMine,			// 18
	SHIPACTION_SelectNextPrimary,	// 19
	SHIPACTION_SelectPrevPrimary,	// 20
	SHIPACTION_SelectNextSecondary, // 21
	SHIPACTION_SelectPrevSecondary, // 22
	SHIPACTION_DropPrimary,			// 23
	SHIPACTION_DropSecondary,		// 24
	SHIPACTION_DropShield,			// 25
	SHIPACTION_DropAmmo,			// 26
	SHIPACTION_SlideMode,			// 27
	SHIPACTION_RollMode,			// 28
	SHIPACTION_MAX					// always last in list
};

enum {
	JOYSTICK_XAxisPos,
	JOYSTICK_YAxisPos,
	JOYSTICK_ZAxisPos,
	JOYSTICK_XAxisRot,
	JOYSTICK_YAxisRot,
	JOYSTICK_ZAxisRot,
	JOYSTICK_ExtraAxisPos,
	JOYSTICK_POV0Up = JOYSTICK_ExtraAxisPos + 2,	// 2 possible extra axis
	JOYSTICK_POV0Down,
	JOYSTICK_POV0Left,
	JOYSTICK_POV0Right,
	JOYSTICK_Button0 = JOYSTICK_POV0Up + (4 * 4),		// 4 POV directions, 4 possible POV controls
};

extern void
control_ship( USERCONFIG *conf, SHIPCONTROL *ctrl );

extern int
AnyKeyReleased( void );

extern void
ReadInput( void );

extern int
WhichMousePressed( void );

extern int
WhichJoystickPressed( void );

void ReadJoystickInput(SHIPCONTROL *ctrl, int joysticknum);
void SetUpJoystickAxis(int joystick);
bool IsJoystickButtonPressed( int joysticknum );
bool IsAnyJoystickButtonPressed( void );
bool IsJoystickButtonReleased( int joysticknum );
bool IsAnyJoystickButtonReleased( void );
int ToggleGodMode( char *cheat );
void FlashMenuText( char *text, float activetime, u_int16_t sfx );
void ProcessMenuFlashText( void );
void DisableCheats( void );

#endif // CONTROLS_H
