#ifndef FORCE_H
#define FORCE_H


#include "dinput.h"
#include "new3d.h"



#define INFINITE_DURATION	(0.0F)
#define MAX_MAGNITUDE		(1.0F)
#define MAX_COEFFICIENT		(1.0F)
#define MAX_SATURATION		(1.0F)

#define MIN_GAIN			(0.001F)
#define MAX_GAIN			(1.0F)

typedef enum
{
	FF_EFFECT_PrimaryRecoil,
	FF_EFFECT_SecondaryRecoil,
	FF_EFFECT_Jolt,
	FF_EFFECT_Engine,
	FF_EFFECT_Damper,
	FF_EFFECT_Inertia,
	FF_EFFECT_MAX
} FF_EffectID;


typedef enum
{
	FF_TYPE_Constant,
	FF_TYPE_Triangle,
	FF_TYPE_SawtoothDown,
	FF_TYPE_Sine,
	FF_TYPE_Spring,
	FF_TYPE_Damper,
	FF_TYPE_Inertia,
	FF_TYPE_MAX
} FF_EffectType;


typedef enum
{
	FF_CLASS_Constant,
	FF_CLASS_Ramp,
	FF_CLASS_Periodic,
	FF_CLASS_Condition,
	FF_CLASS_Custom,
	FF_CLASS_MAX
} FF_EffectClass;


typedef struct _FeedbackInfo
{
	int enable;
	float gain;
	float xscale, yscale;
	struct
	{
		int enable;
		float gain;
	} effect_setting[ FF_EFFECT_MAX ];
	DIEFFECTINFO effect_info[ FF_TYPE_MAX ];
} FeedbackInfo;

extern int FF_Supported( int joystick );

extern int FF_Init( void );

extern int FF_InitJoystick( int joystick );

extern int FF_ReleaseJoystick( int joystick );

extern int FF_CheckJoystick( int joystick );

extern int FF_CreateEffect( int joystick, FF_EffectID id, FF_EffectType type );

extern int FF_StartEffect( int joystick, FF_EffectID id );

extern int FF_StopEffect( int joystick, FF_EffectID id );

extern int FF_DownloadEffect( int joystick, FF_EffectID id );

extern int FF_GetEffectState( int joystick, FF_EffectID id, DWORD *state );

extern int FF_CheckEffectDownloaded( int joystick, FF_EffectID id );

extern int FF_CheckEffectPlaying( int joystick, FF_EffectID id );

extern int FF_EffectPlaying( int joystick, FF_EffectID id );

extern int FF_EffectActive( int joystick, FF_EffectID id );

extern char *FF_EffectName( FF_EffectID id );

extern int FF_SetEffectEnable( int joystick, FF_EffectID id, int enable );

extern int FF_SetEffectGain( int joystick, FF_EffectID id, float gain );

extern int FF_UpdatePeriodicEffect( int joystick, FF_EffectID id,
							float magnitude, float offset, float duration, float frequency,
							float attack_level, float attack_time,
							float fade_level, float fade_time,
							VECTOR *direction, DWORD flags );

extern int FF_UpdateConstantEffect( int joystick, FF_EffectID id, 
							float magnitude, float duration,
							float attack_level, float attack_time,
							float fade_level, float fade_time,
							VECTOR *direction, DWORD flags );

extern int FF_UpdateConditionEffect( int joystick, FF_EffectID id, 
							 float duration, float coefficient,
							 float saturation, DWORD flags );

#endif // FORCE_H
