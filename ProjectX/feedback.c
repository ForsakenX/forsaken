#include <stdio.h>
#include "typedefs.h"
#include "dplay.h"
#include "new3d.h"
#include "compobjects.h"
#include "quat.h"
#include "object.h"
#include "mydplay.h"
#include "2dtextures.h"
#include "mload.h"
#include "primary.h"
#include "secondary.h"
#include "water.h"
#include "controls.h"
#include "force.h"
#include "feedback.h"


#define LOGITECH_HACK


#define GAME_SECOND			(71.0F)

#define OFFTIME_FACTOR		(0.75F)

#define WATER_COEFFICIENT	(0.5F)
#define WATER_SATURATION	(1.0F)
#define NORMAL_COEFFICIENT	(0.1F)
#define NORMAL_SATURATION	(0.2F)

#define TURBO_MAGNITUDE		(0.1F)
#define TURBO_OFFSET		(0.2F)
#define TURBO_FREQUENCY		(40.0F)

#define PRIMARY_CHECK_INTERVAL		(10.0F * GAME_SECOND)
#define SECONDARY_CHECK_INTERVAL	(10.0F * GAME_SECOND)
#define JOLT_CHECK_INTERVAL			(10.0F * GAME_SECOND)
#define ENGINE_CHECK_INTERVAL		(10.0F * GAME_SECOND)
#define DAMPER_CHECK_INTERVAL		(10.0F * GAME_SECOND)
#define INERTIA_CHECK_INTERVAL		(10.0F * GAME_SECOND)


extern GLOBALSHIP Ships[MAX_PLAYERS];
extern BYTE WhoIAm;
extern float framelag;
extern int Num_Joysticks;
extern DIDEVCAPS diJoystickCaps[MAX_JOYSTICKS];
extern PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern SECONDARYWEAPONATTRIB SecondaryWeaponAttribs[ TOTALSECONDARYWEAPONS ];


int FB_Joystick = 0;


static VECTOR JoltForce = { 0.0F, 0.0F, 0.0F };


int FB_ChooseJoystick( void )
{
	int j;

	for ( j = 0; j < Num_Joysticks; j++ )
	{
		if ( diJoystickCaps[ j ].dwFlags & DIDC_FORCEFEEDBACK )
			return j;
	}
	return 0;
}


int FB_Initialise( void )
{
	FB_Joystick = FB_ChooseJoystick();
	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	FF_CreateEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil, FF_TYPE_SawtoothDown );
	FF_CreateEffect( FB_Joystick, FF_EFFECT_SecondaryRecoil, FF_TYPE_SawtoothDown );
	FF_CreateEffect( FB_Joystick, FF_EFFECT_Jolt, FF_TYPE_Constant );
	FF_CreateEffect( FB_Joystick, FF_EFFECT_Engine, FF_TYPE_Sine );
	FF_CreateEffect( FB_Joystick, FF_EFFECT_Damper, FF_TYPE_Damper );
	FF_CreateEffect( FB_Joystick, FF_EFFECT_Inertia, FF_TYPE_Spring );

	return 1;
}



int FB_Start( void )
{
	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	JoltForce.x = 0.0F;
	JoltForce.y = 0.0F;
	JoltForce.z = 0.0F;
	FB_Engine();
	FB_Damper();
	FB_Inertia();
	FF_StartEffect( FB_Joystick, FF_EFFECT_Damper );
	FF_StartEffect( FB_Joystick, FF_EFFECT_Inertia );
	FF_CheckEffectDownloaded( FB_Joystick, FF_EFFECT_PrimaryRecoil );
	FF_CheckEffectDownloaded( FB_Joystick, FF_EFFECT_SecondaryRecoil );
	FF_CheckEffectDownloaded( FB_Joystick, FF_EFFECT_Jolt );
	FF_CheckEffectDownloaded( FB_Joystick, FF_EFFECT_Engine );
	FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Damper );
	FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Inertia );

	return 1;
}



int FB_Stop( void )
{
	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	JoltForce.x = 0.0F;
	JoltForce.y = 0.0F;
	JoltForce.z = 0.0F;
	FF_StopEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil );
	FF_StopEffect( FB_Joystick, FF_EFFECT_SecondaryRecoil );
	FF_StopEffect( FB_Joystick, FF_EFFECT_Jolt );
	FF_StopEffect( FB_Joystick, FF_EFFECT_Engine );
	FF_StopEffect( FB_Joystick, FF_EFFECT_Damper );
	FF_StopEffect( FB_Joystick, FF_EFFECT_Inertia );

	return 1;
}



int FB_FirePrimary( int firing, float power )
{
	GLOBALSHIP *s;
	OBJECT *obj;
	float magnitude, offset, duration, frequency;
	float attack_level, attack_time;
	PRIMARYWEAPONATTRIB *weapon;
	static struct
	{
		float magnitude;
		float attack_level, attack_time;
	} *param, PrimaryParams[ MAXPRIMARYWEAPONS ] =
	{
		// Pulsar
		{
			0.2F,
			0.2F, 0.0F
		},
		// Trojax
		{
			0.4F,
			0.4F, 0.0F
		},
		// Pyrolite
		{
			0.20F,
			0.25F, 0.1F
		},
		// Transpulse
		{
			0.25F,
			0.25F, 0.0F
		},
		// Suss gun
		{
			0.25F,
			0.25F, 0.0F
		},
		// Laser
		{
			0.15F,
			0.15F, 0.0F
		},
	};
	static VECTOR dir = { 0.0F, 0.0F, -1.0F };
	static BYTE last_primary = 0;
	static float checktime = 0.0F;
#ifdef LOGITECH_HACK
	static float offtime = 0.0F;
#endif

	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;
#ifdef LOGITECH_HACK
	if ( offtime > 0.0F && !firing )
	{
		offtime -= framelag;
		if ( offtime < 0.0F && FF_EffectPlaying( FB_Joystick, FF_EFFECT_PrimaryRecoil ) )
		{
			FF_StopEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil );
		}
	}
#endif
	if ( s->Primary == TROJAX )
	{
		if ( firing )
		{
			weapon = &PrimaryWeaponAttribs[ s->Primary ];
			param = &PrimaryParams[ s->Primary ];
			magnitude = param->magnitude * ( 0.3F + 0.7F * power );
			if ( magnitude > 0.01F )
			{
				offset = magnitude * 0.5F;
				duration = magnitude / MAX_MAGNITUDE;
				frequency = 1.0F / magnitude;
				attack_level = param->attack_level * ( 0.3F + 0.7F * power );
				attack_time = param->attack_time;
				FF_UpdatePeriodicEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil,
					magnitude, offset, duration, frequency,
					attack_level, attack_time, 0.0F, 0.0F, &dir, DIEP_START );
				last_primary = s->Primary;
				checktime -= framelag;
				if ( checktime < 0.0F )
				{
					FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_PrimaryRecoil );
					checktime = PRIMARY_CHECK_INTERVAL;
				}
#ifdef LOGITECH_HACK
				offtime = OFFTIME_FACTOR * GAME_SECOND / frequency;
#endif
			}
		}
	}
	else
	{
		if ( firing )
		{
			if ( !FF_EffectPlaying( FB_Joystick, FF_EFFECT_PrimaryRecoil ) || s->Primary != last_primary )
			{
				weapon = &PrimaryWeaponAttribs[ s->Primary ];
				param = &PrimaryParams[ s->Primary ];
				magnitude = param->magnitude * ( 0.7F + 0.3F * obj->PowerLevel / ( MAXPOWERLEVELS - 1 ) );
				offset = magnitude * 0.5F;
				duration = INFINITE_DURATION;
				frequency = GAME_SECOND / weapon->FireDelay[ obj->PowerLevel ];
				attack_level = param->attack_level;
				attack_time = param->attack_time;
				FF_UpdatePeriodicEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil,
					magnitude, offset, duration, frequency,
					attack_level, attack_time, 0.0F, 0.0F, &dir, DIEP_START );
				last_primary = s->Primary;
			}
			checktime -= framelag;
			if ( checktime < 0.0F )
			{
				FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_PrimaryRecoil );
				checktime = PRIMARY_CHECK_INTERVAL;
			}
		}
		else
		{
			if ( last_primary != TROJAX && FF_EffectPlaying( FB_Joystick, FF_EFFECT_PrimaryRecoil ) )
			{
				FF_StopEffect( FB_Joystick, FF_EFFECT_PrimaryRecoil );
			}
		}
	}

	return 1;
}



int FB_FireSecondary( int firing )
{
	GLOBALSHIP *s;
	OBJECT *obj;
	float magnitude, offset, duration, frequency;
	SECONDARYWEAPONATTRIB *weapon;
	static struct
	{
		float magnitude;
	} *param, SecondaryParams[ MAXSECONDARYWEAPONS ] =
	{
		// Mug
		{
			0.125F,
		},
		// Solaris
		{
			0.125F,
		},
		// Thief
		{
			0.0F,
		},
		// Scatter
		{
			0.15F,
		},
		// Gravgon
		{
			0.225F,
		},
		// MFRL
		{
			0.175F,
		},
		// Titan
		{
			0.3F,
		},
		// Purge mine
		{
			0.0F,
		},
		// Pine mine
		{
			0.0F,
		},
		// Quantum mine
		{
			0.0F,
		},
		// Spider mine
		{
			0.0F,
		},
	};
	static VECTOR dir = { 0.0F, 0.0F, -1.0F };
	static BYTE last_secondary = 0;
	static float checktime = 0.0F;
#ifdef LOGITECH_HACK
	static float offtime = 0.0F;
#endif

	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;
	if ( firing )
	{
		if ( s->Secondary == MULTIPLEMISSILE )
		{
			if ( !FF_EffectPlaying( FB_Joystick, FF_EFFECT_SecondaryRecoil ) || last_secondary != s->Secondary )
			{
				weapon = &SecondaryWeaponAttribs[ s->Secondary ];
				param = &SecondaryParams[ s->Secondary ];
				magnitude = param->magnitude;
				offset = magnitude * 0.5F;
				duration = INFINITE_DURATION;
				frequency = GAME_SECOND / weapon->FireDelay;
				FF_UpdatePeriodicEffect( FB_Joystick, FF_EFFECT_SecondaryRecoil,
					magnitude, offset, duration, frequency,
					magnitude, 0.0F, 0.0F, 0.0F, &dir, DIEP_START );
				checktime -= framelag;
				if ( checktime < 0.0F )
				{
					FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_SecondaryRecoil );
					checktime = SECONDARY_CHECK_INTERVAL;
				}
			}
			last_secondary = s->Secondary;
		}
		else if ( s->Secondary < PURGEMINE )
		{
			weapon = &SecondaryWeaponAttribs[ s->Secondary ];
			param = &SecondaryParams[ s->Secondary ];
			magnitude = param->magnitude;
			offset = magnitude * 0.5F;
			frequency = GAME_SECOND / weapon->FireDelay;
			duration = 1.0F / frequency;
			FF_UpdatePeriodicEffect( FB_Joystick, FF_EFFECT_SecondaryRecoil,
				magnitude, offset, duration, frequency,
				magnitude, 0.0F, 0.0F, 0.0F, &dir, DIEP_START );
			checktime -= framelag;
			if ( checktime < 0.0F )
			{
				FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_SecondaryRecoil );
				checktime = SECONDARY_CHECK_INTERVAL;
			}
			last_secondary = s->Secondary;
#ifdef LOGITECH_HACK
			offtime = OFFTIME_FACTOR * weapon->FireDelay;
#endif
		}
	}
	else
	{
#ifdef LOGITECH_HACK
		if ( offtime > 0.0F )
			offtime -= framelag;
		if ( ( offtime < 0.0F || last_secondary == MULTIPLEMISSILE ) && FF_EffectPlaying( FB_Joystick, FF_EFFECT_SecondaryRecoil ) )
#else
		if ( last_secondary == MULTIPLEMISSILE && FF_EffectPlaying( FB_Joystick, FF_EFFECT_SecondaryRecoil ) )
#endif
		{
			FF_StopEffect( FB_Joystick, FF_EFFECT_SecondaryRecoil );
		}
	}

	return 1;
}


int FB_JoltForce( VECTOR *jolt )
{
	JoltForce.x += jolt->x;
	JoltForce.y += jolt->y;
	JoltForce.z += jolt->z;

	return 1;
}


int FB_Jolt( void )
{
	static float checktime = 0.0F;
	//static float jolt_time = 0.0F;
	GLOBALSHIP *s;
	OBJECT *obj;
	VECTOR jolt;
	float magnitude, duration;
	float attack_level, attack_time;
	float fade_level, fade_time;
	MATRIX InvMat;
	
	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;
	if ( framelag )
		magnitude = VectorLength( &JoltForce ) / framelag;
	else
		magnitude = 0.0F;
	magnitude /= MAXTURBOSPEED;
	if ( magnitude > 0.01F )
	{
		MatrixTranspose( &obj->Mat, &InvMat );
		ApplyMatrix( &InvMat, &JoltForce, &jolt );
		duration = 0.25F;
		attack_level = (float) sqrt( magnitude );
		attack_time = 0.1F;
		fade_level = 0.0F;
		fade_time = 0.15F;
		if ( magnitude > MAX_MAGNITUDE )
			magnitude = MAX_MAGNITUDE;
		if ( attack_level > MAX_MAGNITUDE )
			attack_level = MAX_MAGNITUDE;
		FF_UpdateConstantEffect( FB_Joystick, FF_EFFECT_Jolt,
			magnitude, duration,
			attack_level, attack_time,
			fade_level, fade_time,
			&jolt, DIEP_START );
		checktime -= framelag;
		if ( checktime < 0.0F )
		{
			checktime = JOLT_CHECK_INTERVAL;
			FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Jolt );
		}
	}
	JoltForce.x = 0.0F;
	JoltForce.y = 0.0F;
	JoltForce.z = 0.0F;
	return 1;
}



int FB_Engine( void )
{
	static float checktime = 0.0F;
	GLOBALSHIP *s;
	OBJECT *obj;
	float magnitude, offset;
	float duration, frequency;
	static VECTOR dir = { 0.0F, 0.0F, -1.0F };
	static int last_turbo = 0;

	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;
	if ( obj->Flags & SHIP_Turbo )
	{
		if ( !FF_EffectPlaying( FB_Joystick, FF_EFFECT_Engine ) || !last_turbo )
		{
			magnitude = TURBO_MAGNITUDE;
			offset = TURBO_OFFSET;
			duration = INFINITE_DURATION;
			frequency = TURBO_FREQUENCY;
			FF_UpdatePeriodicEffect( FB_Joystick, FF_EFFECT_Engine,
				magnitude, offset, duration, frequency,
				0.0F, 0.0F, 0.0F, 0.0F, &dir, DIEP_START );
		}
		checktime -= framelag;
		if ( checktime < 0.0F )
		{
			FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Engine );
		}
		last_turbo = 1;
	}
	else
	{
		if ( FF_EffectPlaying( FB_Joystick, FF_EFFECT_Engine ) )
		{
			FF_StopEffect( FB_Joystick, FF_EFFECT_Engine );
		}
		last_turbo = 0;
	}

	return 1;
}



int FB_Damper( void )
{
	GLOBALSHIP *s;
	OBJECT *obj;
	float coefficient, saturation;
	static float checktime = 0.0F;

	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;
	if ( obj->Flags & SHIP_InWater )
	{
		coefficient = WATER_COEFFICIENT;
		saturation = WATER_SATURATION;
	}
	else
	{
		coefficient = NORMAL_COEFFICIENT;
		saturation = NORMAL_SATURATION;
	}
	FF_UpdateConditionEffect( FB_Joystick, FF_EFFECT_Damper,
		INFINITE_DURATION, coefficient, saturation, 0 );
	checktime -= framelag;
	if ( checktime < 0.0F )
	{
		checktime = DAMPER_CHECK_INTERVAL;
		FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Damper );
	}
	return 1;
}



int FB_Inertia( void )
{
	static float checktime = 0.0F;
	GLOBALSHIP *s;
	OBJECT *obj;
	float coefficient, saturation;
	/*
	static struct
	{
		float coefficient;
		float saturation;
	} *param, BikeParams[] =
	{
		{ 0.0F, 0.0F },
	};
	*/

	if ( !FF_Supported( FB_Joystick ) )
		return 0;
	s = &Ships[ WhoIAm ];
	obj = &s->Object;

	coefficient = 1.0F * MAX_COEFFICIENT;
	saturation = MAX_SATURATION;
	FF_UpdateConditionEffect( FB_Joystick, FF_EFFECT_Inertia,
		INFINITE_DURATION, coefficient, saturation, 0 );
	checktime -= framelag;
	if ( checktime < 0.0F )
	{
		checktime = INERTIA_CHECK_INTERVAL;
		FF_CheckEffectPlaying( FB_Joystick, FF_EFFECT_Inertia );
	}

	return 1;
}
