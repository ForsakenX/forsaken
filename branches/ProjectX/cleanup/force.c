#include <stdio.h>
#include "typedefs.h"
#include "dinput.h"
#include "controls.h"
#include "force.h"
#include "xmem.h"


#undef DEBUG_FF_DYNAMIC


typedef struct _FeedbackEffect
{
	FF_EffectType type;
	DWORD update;
	int playing;
	LPDIRECTINPUTEFFECT lpEffect;
	DIEFFECT effect;
	union
	{
		DICONSTANTFORCE constant;
		DIRAMPFORCE ramp;
		DIPERIODIC periodic;
		DICONDITION condition[ MAX_JOYSTICK_AXIS ];
	};
	DIENVELOPE envelope;
	DWORD axes[	MAX_JOYSTICK_AXIS ];
	LONG direction[	MAX_JOYSTICK_AXIS ];
} FeedbackEffect;


extern void DebugPrintf( const char * format, ... );
extern BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);

extern JOYSTICKINFO			JoystickInfo[MAX_JOYSTICKS]; 
extern LPDIRECTINPUTDEVICE2	lpdiJoystick[MAX_JOYSTICKS];
extern DIDEVCAPS			diJoystickCaps[MAX_JOYSTICKS];
extern int					Num_Joysticks;


static FeedbackEffect FB_Effect[ MAX_JOYSTICKS ][ FF_EFFECT_MAX ];

static FF_EffectClass effect_class[ FF_TYPE_MAX ];
static GUID effect_guid[ FF_TYPE_MAX ];
static char *effect_name[ FF_EFFECT_MAX ];
static char *effect_type[ FF_TYPE_MAX ];
static DWORD effect_class_mask[ FF_CLASS_MAX ] =
{
	DIEFT_CONSTANTFORCE,
	DIEFT_RAMPFORCE,
	DIEFT_PERIODIC,
	DIEFT_CONDITION,
	DIEFT_CUSTOMFORCE
};


#define INIT_EFFECT_TYPE( eclass, etype, guid ) effect_class[ etype ] = eclass, effect_guid[ etype ] = guid, effect_type[ etype ] = #etype
#define INIT_EFFECT_NAME( id, name ) effect_name[ id ] = name

#define JOYSTICK_COMMAND( JOYSTICK, COMMAND )\
(\
	err = (COMMAND),\
	err = ( ( ( err == DIERR_INPUTLOST ) ||\
			  ( err == DIERR_NOTEXCLUSIVEACQUIRED ) ) &&\
		    SUCCEEDED( IDirectInputDevice2_Acquire( lpdiJoystick[ (JOYSTICK) ] ) ) )\
		? (COMMAND) : err\
)


static char *FF_ErrorMsg( HRESULT err )
{
	static char buf[ 80 ];

	switch( err )
	{
	case DI_BUFFEROVERFLOW:
//	case DI_NOEFFECT:
//	case DI_NOTATTACHED:
//	case DI_PROPNOEFFECT:
		return "DI_BUFFEROVERFLOW/DI_NOEFFECT/DI_NOTATTACHED/DI_PROPNOEFFECT";
	case DI_DOWNLOADSKIPPED:
		return "DI_DOWNLOADSKIPPED";
	case DI_EFFECTRESTARTED:
		return "DI_EFFECTRESTARTED";
	case DI_OK:
		return "DI_OK";
	case DI_POLLEDDEVICE:
		return "DI_POLLEDDEVICE";
	case DI_TRUNCATED:
		return "DI_TRUNCATED";
	case DI_TRUNCATEDANDRESTARTED:
		return "DI_TRUNCATEDANDRESTARTED";
	case DIERR_ACQUIRED:
		return "DIERR_ACQUIRED";
	case DIERR_ALREADYINITIALIZED:
		return "DIERR_ALREADYINITIALIZED";
	case DIERR_BADDRIVERVER:
		return "DIERR_BADDRIVERVER";
	case DIERR_BETADIRECTINPUTVERSION:
		return "DIERR_BETADIRECTINPUTVERSION";
	case DIERR_DEVICEFULL:
		return "DIERR_DEVICEFULL";
	case DIERR_DEVICENOTREG:
		return "DIERR_DEVICENOTREG";
	case DIERR_EFFECTPLAYING:
		return "DIERR_EFFECTPLAYING";
	case DIERR_HASEFFECTS:
		return "DIERR_HASEFFECTS";
	case DIERR_GENERIC:
		return "DIERR_GENERIC";
	case DIERR_HANDLEEXISTS:
//	case DIERR_OTHERAPPHASPRIO:
//	case DIERR_READONLY:
		return "DIERR_HANDLEEXISTS/DIERR_OTHERAPPHASPRIO/DIERR_READONLY";
	case DIERR_INCOMPLETEEFFECT:
		return "DIERR_INCOMPLETEEFFECT";
	case DIERR_INPUTLOST:
		return "DIERR_INPUTLOST";
	case DIERR_INVALIDPARAM:
		return "DIERR_INVALIDPARAM";
	case DIERR_NOAGGREGATION:
		return "DIERR_NOAGGREGATION";
	case DIERR_NOINTERFACE:
		return "DIERR_NOINTERFACE";
	case DIERR_NOTACQUIRED:
		return "DIERR_NOTACQUIRED";
	case DIERR_NOTBUFFERED:
		return "DIERR_NOTBUFFERED";
	case DIERR_NOTDOWNLOADED:
		return "DIERR_NOTDOWNLOADED";
	case DIERR_NOTEXCLUSIVEACQUIRED:
		return "DIERR_NOTEXCLUSIVEACQUIRED";
	case DIERR_NOTFOUND:
		return "DIERR_NOTFOUND";
	case DIERR_NOTINITIALIZED:
		return "DIERR_NOTINITIALIZED";
//	case DIERR_OBJECTNOTFOUND:
		return "DIERR_OBJECTNOTFOUND";
	case DIERR_OLDDIRECTINPUTVERSION:
		return "DIERR_OLDDIRECTINPUTVERSION";
	case DIERR_OUTOFMEMORY:
		return "DIERR_OUTOFMEMORY";
	case DIERR_UNSUPPORTED:
		return "DIERR_UNSUPPORTED";
	case DIERR_MOREDATA:
		return "DIERR_MOREDATA";
//	case DIERR_DEVICENOTREG:
		return "DIERR_DEVICENOTREG";
	case E_PENDING:
		return "E_PENDING";
	default:
		sprintf( buf, "<unknown = 0x%08X>", (int) err );
		return buf;
	}
}


BOOL CALLBACK FF_EnumEffect( LPCDIEFFECTINFO pdei, LPVOID pvRef )
{
	GUID CompareGUID;
	int j, e;

	j = (int) pvRef;

	CompareGUID = pdei->guid;

	for ( e = 0; e < FF_TYPE_MAX; e++ )
	{
		if ( IsEqualGuid( &CompareGUID, &effect_guid[ e ] ) )
		{
			JoystickInfo[ j ].feedback->effect_info[ e ] = *pdei;
			JoystickInfo[ j ].feedback->effect_setting[ e ].enable = 1;
			JoystickInfo[ j ].feedback->effect_setting[ e ].gain = MAX_GAIN;
			break;
		}
	}
	{
		DebugPrintf( "Joystick %s supports%s force feedback effect %s\n",
			JoystickInfo[ j ].Name,
			( e < FF_TYPE_MAX ) ? "*" : "",
			pdei->tszName );
		if ( pdei->dwEffType & DIEFT_CONDITION )
			DebugPrintf( "  DIEFT_CONDITION\n" );
		if ( pdei->dwEffType & DIEFT_CONSTANTFORCE )
			DebugPrintf( "  DIEFT_CONSTANTFORCE\n" );
		if ( pdei->dwEffType & DIEFT_CUSTOMFORCE )
			DebugPrintf( "  DIEFT_CUSTOMFORCE\n" );
		if ( pdei->dwEffType & DIEFT_DEADBAND )
			DebugPrintf( "  DIEFT_DEADBAND\n" );
		if ( pdei->dwEffType & DIEFT_FFATTACK )
			DebugPrintf( "  DIEFT_FFATTACK\n" );
		if ( pdei->dwEffType & DIEFT_FFFADE )
			DebugPrintf( "  DIEFT_FFFADE\n" );
		if ( pdei->dwEffType & DIEFT_HARDWARE )
			DebugPrintf( "  DIEFT_HARDWARE\n" );
		if ( pdei->dwEffType & DIEFT_PERIODIC )
			DebugPrintf( "  DIEFT_PERIODIC\n" );
		if ( pdei->dwEffType & DIEFT_POSNEGCOEFFICIENTS )
			DebugPrintf( "  DIEFT_POSNEGCOEFFICIENTS\n" );
		if ( pdei->dwEffType & DIEFT_POSNEGSATURATION )
			DebugPrintf( "  DIEFT_POSNEGSATURATION\n" );
		if ( pdei->dwEffType & DIEFT_RAMPFORCE )
			DebugPrintf( "  DIEFT_RAMPFORCE\n" );
		if ( pdei->dwEffType & DIEFT_SATURATION )
			DebugPrintf( "  DIEFT_SATURATION\n" );
		if ( pdei->dwStaticParams & DIEP_AXES )
			DebugPrintf( "  Static DIEP_AXES\n" );
		if ( pdei->dwStaticParams & DIEP_DIRECTION )
			DebugPrintf( "  Static DIEP_DIRECTION\n" );
		if ( pdei->dwStaticParams & DIEP_DURATION )
			DebugPrintf( "  Static DIEP_DURATION\n" );
		if ( pdei->dwStaticParams & DIEP_ENVELOPE )
			DebugPrintf( "  Static DIEP_ENVELOPE\n" );
		if ( pdei->dwStaticParams & DIEP_GAIN )
			DebugPrintf( "  Static DIEP_GAIN\n" );
		if ( pdei->dwStaticParams & DIEP_SAMPLEPERIOD )
			DebugPrintf( "  Static DIEP_SAMPLEPERIOD\n" );
		if ( pdei->dwStaticParams & DIEP_TRIGGERBUTTON )
			DebugPrintf( "  Static DIEP_TRIGGERBUTTON\n" );
		if ( pdei->dwStaticParams & DIEP_TRIGGERREPEATINTERVAL )
			DebugPrintf( "  Static DIEP_TRIGGERREPEATINTERVAL\n" );
		if ( pdei->dwStaticParams & DIEP_TYPESPECIFICPARAMS )
			DebugPrintf( "  Static DIEP_TYPESPECIFICPARAMS\n" );
		if ( pdei->dwDynamicParams & DIEP_AXES )
			DebugPrintf( "  Dynamic DIEP_AXES\n" );
		if ( pdei->dwDynamicParams & DIEP_DIRECTION )
			DebugPrintf( "  Dynamic DIEP_DIRECTION\n" );
		if ( pdei->dwDynamicParams & DIEP_DURATION )
			DebugPrintf( "  Dynamic DIEP_DURATION\n" );
		if ( pdei->dwDynamicParams & DIEP_ENVELOPE )
			DebugPrintf( "  Dynamic DIEP_ENVELOPE\n" );
		if ( pdei->dwDynamicParams & DIEP_GAIN )
			DebugPrintf( "  Dynamic DIEP_GAIN\n" );
		if ( pdei->dwDynamicParams & DIEP_SAMPLEPERIOD )
			DebugPrintf( "  Dynamic DIEP_SAMPLEPERIOD\n" );
		if ( pdei->dwDynamicParams & DIEP_TRIGGERBUTTON )
			DebugPrintf( "  Dynamic DIEP_TRIGGERBUTTON\n" );
		if ( pdei->dwDynamicParams & DIEP_TRIGGERREPEATINTERVAL )
			DebugPrintf( "  Dynamic DIEP_TRIGGERREPEATINTERVAL\n" );
		if ( pdei->dwDynamicParams & DIEP_TYPESPECIFICPARAMS )
			DebugPrintf( "  Dynamic DIEP_TYPESPECIFICPARAMS\n" );
	}
	return DIENUM_CONTINUE;
}



char *FF_EffectName( FF_EffectID id )
{
	static char buf[ 256 ];

	if ( id >= ( FF_EffectID) 0 && id < FF_EFFECT_MAX )
	{
		return effect_name[ id ];
	}
	else
	{
		sprintf( buf, "Effect#%d", id );
		return buf;
	}
}


int FF_Init( void )
{
	INIT_EFFECT_NAME( FF_EFFECT_PrimaryRecoil,		"PrimaryRecoil" );
	INIT_EFFECT_NAME( FF_EFFECT_SecondaryRecoil,	"SecondaryRecoil" );
	INIT_EFFECT_NAME( FF_EFFECT_Jolt,				"Impact" );
	INIT_EFFECT_NAME( FF_EFFECT_Engine,				"Engine" );
	INIT_EFFECT_NAME( FF_EFFECT_Damper,				"Damper" );
	INIT_EFFECT_NAME( FF_EFFECT_Inertia,			"Inertia" );
	INIT_EFFECT_TYPE( FF_CLASS_Constant,	FF_TYPE_Constant,	GUID_ConstantForce );
	INIT_EFFECT_TYPE( FF_CLASS_Periodic,	FF_TYPE_Triangle,	GUID_Triangle );
	INIT_EFFECT_TYPE( FF_CLASS_Periodic,	FF_TYPE_SawtoothDown,GUID_SawtoothDown );
	INIT_EFFECT_TYPE( FF_CLASS_Periodic,	FF_TYPE_Sine,		GUID_Sine );
	INIT_EFFECT_TYPE( FF_CLASS_Condition,	FF_TYPE_Spring,		GUID_Spring );
	INIT_EFFECT_TYPE( FF_CLASS_Condition,	FF_TYPE_Damper,		GUID_Damper );
	INIT_EFFECT_TYPE( FF_CLASS_Condition,	FF_TYPE_Inertia,	GUID_Inertia );
	return 1;
}


int FF_InitJoystick( int joystick )
{
	int e;
	FeedbackInfo *feedback;

	for ( e = 0; e < FF_EFFECT_MAX; e++ )
	{
		FB_Effect[ joystick ][ e ].lpEffect = NULL;
	}
	JoystickInfo[ joystick ].feedback = NULL;
	if ( !( diJoystickCaps[ joystick ].dwFlags & DIDC_FORCEFEEDBACK	) )
	{
		DebugPrintf( "FF_InitJoystick: joystick %s does not support force feedback\n",
			JoystickInfo[ joystick ].Name );
		return 0;
	}
	JoystickInfo[ joystick ].feedback = (FeedbackInfo *) calloc( sizeof( FeedbackInfo ), 1 );
	if ( !JoystickInfo[ joystick ].feedback )
	{
		DebugPrintf( "FF_InitJoystick: joystick %s unable to allocate FeedbackInfo\n",
			JoystickInfo[ joystick ].Name );
		return 0;
	}
	feedback = JoystickInfo[ joystick ].feedback;
	feedback->enable = 1;
	feedback->gain = MAX_GAIN;
	feedback->xscale = 1.0F;
	feedback->yscale = 1.0F;
	for ( e = 0; e < FF_EFFECT_MAX; e++ )
	{
		feedback->effect_setting[ e ].enable = 0;
		feedback->effect_setting[ e ].gain = 0.0F;
	}
	IDirectInputDevice2_EnumEffects( lpdiJoystick[ joystick ], FF_EnumEffect,
		(LPVOID) joystick, DIEFT_ALL );
	return 1;
}


BOOL CALLBACK FF_ReleaseEffect( LPDIRECTINPUTEFFECT peff, LPVOID pvRef)
{
	HRESULT err;
	int joystick;

	joystick = (int) pvRef;
	DebugPrintf( "FF_ReleaseEffect: releasing effect for joystick %d\n", joystick );
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Unload( peff ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_ReleaseEffect: joystick %s force feedback effect Unload() failed (%s)\n",
			JoystickInfo[ joystick ].Name, FF_ErrorMsg( err ) );
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Release( peff ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_ReleaseEffect: joystick %s force feedback effect Release() failed (%s)\n",
			JoystickInfo[ joystick ].Name, FF_ErrorMsg( err ) );
	}
	return DIENUM_CONTINUE;
}


int FF_ReleaseJoystick( int joystick )
{
	HRESULT err;

	if ( lpdiJoystick[ joystick ] )
	{
		if ( diJoystickCaps[ joystick ].dwFlags & DIDC_FORCEFEEDBACK )
		{
			err = JOYSTICK_COMMAND( joystick,
				IDirectInputDevice2_EnumCreatedEffectObjects( lpdiJoystick[ joystick ], FF_ReleaseEffect, (LPVOID) joystick, 0 ) );
			if ( FAILED( err ) )
			{
				DebugPrintf( "FF_ReleaseJoystick: joystick %s EnumCreatedEffectObjects() failed (%s)\n",
					JoystickInfo[ joystick ].Name, FF_ErrorMsg( err ) );
			}
		}
	}
	if ( JoystickInfo[ joystick ].feedback )
	{
		free( JoystickInfo[ joystick ].feedback );
		JoystickInfo[ joystick ].feedback = NULL;
	}

	return 1;
}


int FF_CheckJoystick( int joystick )
{
	HRESULT err;
	DWORD status;
	static DWORD last_status = 0;

	err = JOYSTICK_COMMAND( joystick,
		IDirectInputDevice2_GetForceFeedbackState( lpdiJoystick[ joystick ], &status ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_CheckJoystick: joystick %s GetForceFeedbackState() failed (%s)\n",
			JoystickInfo[ joystick ].Name, FF_ErrorMsg( err ) );
		return 0;
	}
	status ^= last_status;
	last_status ^= status;
	
	if ( status & DIGFFS_ACTUATORSOFF )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_ACTUATORSOFF=%d The device's force feedback actuators are disabled.\n",
		( last_status & DIGFFS_ACTUATORSOFF ) ? 1 : 0 );
	if ( status & DIGFFS_ACTUATORSON )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_ACTUATORSON=%d The device's force feedback actuators are enabled.\n",
		( last_status & DIGFFS_ACTUATORSON ) ? 1 : 0 );
	if ( status & DIGFFS_DEVICELOST )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_DEVICELOST=%d The device suffered an unexpected failure and is in an indeterminate state. It must be reset either by unacquiring and reacquiring the device, or by sending a DISFFC_RESET command.\n",
		( last_status & DIGFFS_DEVICELOST ) ? 1 : 0 );
	if ( status & DIGFFS_EMPTY )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_EMPTY=%d The device has no downloaded effects.\n",
		( last_status & DIGFFS_EMPTY ) ? 1 : 0 );
	if ( status & DIGFFS_PAUSED )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_PAUSED=%d Playback of all active effects has been paused.\n",
		( last_status & DIGFFS_PAUSED ) ? 1 : 0 );
	if ( status & DIGFFS_POWEROFF )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_POWEROFF=%d The force feedback system is not currently available. If the device cannot report the power state, then neither DIGFFS_POWERON nor DIGFFS_POWEROFF will be returned.\n",
		( last_status & DIGFFS_POWEROFF ) ? 1 : 0 );
	if ( status & DIGFFS_POWERON )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_POWERON=%d Power to the force feedback system is currently available. If the device cannot report the power state, then neither DIGFFS_POWERON nor DIGFFS_POWEROFF will be returned.\n",
		( last_status & DIGFFS_POWERON ) ? 1 : 0 );
	if ( status & DIGFFS_SAFETYSWITCHOFF )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_SAFETYSWITCHOFF=%d The safety switch is currently off, meaning that the device cannot operate. If the device cannot report the state of the safety switch, then neither DIGFFS_SAFETYSWITCHON nor DIGFFS_SAFETYSWITCHOFF will be returned.\n",
		( last_status & DIGFFS_SAFETYSWITCHOFF ) ? 1 : 0 );
	if ( status & DIGFFS_SAFETYSWITCHON )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_SAFETYSWITCHON=%d The safety switch is currently on, meaning that the device can operate. If the device cannot report the state of the safety switch, then neither DIGFFS_SAFETYSWITCHON nor DIGFFS_SAFETYSWITCHOFF will be returned.\n",
		( last_status & DIGFFS_SAFETYSWITCHON ) ? 1 : 0 );
	if ( status & DIGFFS_STOPPED )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_STOPPED=%d No effects are playing and the device is not paused.\n",
		( last_status & DIGFFS_STOPPED ) ? 1 : 0 );
	if ( status & DIGFFS_USERFFSWITCHOFF )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_USERFFSWITCHOFF=%d The user force feedback switch is currently off, meaning that the device cannot operate. If the device cannot report the state of the user force feedback switch, then neither DIGFFS_USERFFSWITCHON nor DIGFFS_USERFFSWITCHOFF will be returned.\n",
		( last_status & DIGFFS_USERFFSWITCHOFF ) ? 1 : 0 );
	if ( status & DIGFFS_USERFFSWITCHON )
		DebugPrintf( "FF_CheckJoystick: DIGFFS_USERFFSWITCHON=%d The user force feedback switch is currently on, meaning that the device can operate. If the device cannot report the state of the user force feedback switch, then neither DIGFFS_USERFFSWITCHON nor DIGFFS_USERFFSWITCHOFF will be returned.\n",
		( last_status & DIGFFS_USERFFSWITCHON ) ? 1 : 0 );
	return 1;
}


static int FF_SetupCommonDefaults( FeedbackEffect *fb, FF_EffectID id, FeedbackInfo *feedback, DIDEVCAPS *caps )
{
	fb->update = 0;
	fb->playing = 0;
	fb->effect.dwSize = sizeof( fb->effect );
	fb->effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	fb->effect.dwDuration = 0;
	fb->effect.dwSamplePeriod = caps->dwFFSamplePeriod * 2;
	fb->effect.dwGain = (DWORD) ( feedback->gain * feedback->effect_setting[ id ].gain * DI_FFNOMINALMAX );
	fb->effect.dwTriggerButton = DIEB_NOTRIGGER;
	fb->effect.dwTriggerRepeatInterval = 0;
	fb->effect.lpEnvelope = NULL;
	fb->envelope.dwSize = sizeof( fb->envelope );
	return 1;
}


static int FF_SetupPeriodicDefaults( FeedbackEffect *fb, FF_EffectType type  )
{
	fb->type = type;
	fb->effect.cbTypeSpecificParams = sizeof( fb->periodic );
	fb->effect.lpvTypeSpecificParams = &fb->periodic;
	fb->periodic.dwMagnitude = DI_FFNOMINALMAX;
	fb->periodic.lOffset = 0;
	fb->periodic.dwPhase = 0;
	fb->periodic.dwPeriod = DI_SECONDS;
	return 1;
}


static int FF_SetupConstantDefaults( FeedbackEffect *fb, FF_EffectType type  )
{
	fb->type = type;
	fb->effect.cbTypeSpecificParams = sizeof( fb->constant );
	fb->effect.lpvTypeSpecificParams = &fb->constant;
	fb->constant.lMagnitude = DI_FFNOMINALMAX;
	return 1;
}


static int FF_SetupConditionDefaults( FeedbackEffect *fb, FF_EffectType type  )
{
	DWORD axis;

	fb->type = type;
	fb->effect.cbTypeSpecificParams = fb->effect.cAxes * sizeof( fb->condition[ 0 ] );
	fb->effect.lpvTypeSpecificParams = &fb->condition[ 0 ];
	for ( axis = 0; axis < fb->effect.cAxes; axis++ )
	{
		fb->condition[ axis ].lOffset = 0;
		fb->condition[ axis ].lPositiveCoefficient = 1;
		fb->condition[ axis ].lNegativeCoefficient = 1;
		fb->condition[ axis ].dwPositiveSaturation = DI_FFNOMINALMAX;
		fb->condition[ axis ].dwNegativeSaturation = DI_FFNOMINALMAX;
		fb->condition[ axis ].lDeadBand = (LONG) ( 0.2F * DI_FFNOMINALMAX );
	}
	return 1;
}


static int FF_SetupAxes( JOYSTICKINFO *joy, FeedbackEffect *fb )
{
	fb->effect.rgdwAxes = &fb->axes[ 0 ];
	fb->effect.rglDirection = &fb->direction[ 0 ];
	if ( joy->Axis[ AXIS_XAxis ].feedback )
	{
		if ( joy->Axis[ AXIS_YAxis ].feedback )
		{
			fb->effect.cAxes = 2;
			fb->axes[ 0 ] = DIJOFS_X;
			fb->axes[ 1 ] = DIJOFS_Y;
			fb->direction[ 0 ] = 1;
			fb->direction[ 1 ] = 0;
		}
		else
		{
			fb->effect.cAxes = 1;
			fb->axes[ 0 ] = DIJOFS_X;
			fb->direction[ 0 ] = 1;
		}
	}
	else if ( joy->Axis[ AXIS_YAxis ].feedback )
	{
		fb->effect.cAxes = 1;
		fb->axes[ 0 ] = DIJOFS_Y;
		fb->direction[ 0 ] = 1;
	}
	else
	{
		fb->effect.cAxes = 0;
		return 0;
	}
	return 1;
}


static DWORD FF_UpdateGain( FeedbackEffect *fb, DIEFFECTINFO *effect, DWORD gain )
{
	if ( effect->dwDynamicParams & DIEP_GAIN )
	{
		if ( fb->effect.dwGain != gain )
		{
			fb->effect.dwGain = gain;
			return DIEP_GAIN;
		}
		else
			return 0; // no change
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateGain: force feedback effect type %d (%s) does not support dynamic updating of gain\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdateDuration( FeedbackEffect *fb, DIEFFECTINFO *effect, DWORD duration )
{
	if ( effect->dwDynamicParams & DIEP_DURATION )
	{
		if ( fb->effect.dwDuration != duration )
		{
			fb->effect.dwDuration = duration;
			return DIEP_DURATION;
		}
		else
			return 0; // no change
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateDuration: force feedback effect type %d (%s) does not support dynamic updating of duration\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdateEnvelope( FeedbackEffect *fb, DIEFFECTINFO *effect, 
							   DWORD attack_level, DWORD attack_time,
							   DWORD fade_level, DWORD fade_time )
{
	if ( effect->dwDynamicParams & DIEP_ENVELOPE )
	{
		if ( !fb->effect.lpEnvelope ||
			fb->envelope.dwAttackLevel != attack_level ||
			fb->envelope.dwAttackTime != attack_time ||
			fb->envelope.dwFadeLevel != fade_level ||
			fb->envelope.dwFadeTime != fade_time )
		{
			fb->effect.lpEnvelope = &fb->envelope;
			fb->envelope.dwAttackLevel = attack_level;
			fb->envelope.dwAttackTime = attack_time;
			fb->envelope.dwFadeLevel = fade_level;
			fb->envelope.dwFadeTime = fade_time;
			return DIEP_ENVELOPE;
		}
		else
			return 0; // no change
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateEnvelope: force feedback effect type %d (%s) does not support dynamic updating of envelope\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdatePeriodic( FeedbackEffect *fb, DIEFFECTINFO *effect,
							   DWORD magnitude, LONG offset, DWORD period )
{
	if ( effect->dwDynamicParams & DIEP_TYPESPECIFICPARAMS )
	{
		if ( fb->periodic.dwMagnitude != magnitude ||
			fb->periodic.lOffset != offset ||
			fb->periodic.dwPeriod != period )
		{
			fb->periodic.dwMagnitude = magnitude;
			fb->periodic.lOffset = offset;
			fb->periodic.dwPeriod =	period;
			return DIEP_TYPESPECIFICPARAMS;
		}
		else
			return 0; // no change
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdatePeriodic: force feedback effect type %d (%s) does not support dynamic updating of periodic params\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdateConstant( FeedbackEffect *fb, DIEFFECTINFO *effect,
							   LONG magnitude )
{
	if ( effect->dwDynamicParams & DIEP_TYPESPECIFICPARAMS )
	{
		if ( fb->constant.lMagnitude != magnitude )
		{
			fb->constant.lMagnitude = magnitude;
			return DIEP_TYPESPECIFICPARAMS;
		}
		else
			return 0; // no change
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateConstant: force feedback effect type %d (%s) does not support dynamic updating of constant params\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdateCondition( FeedbackEffect *fb, DIEFFECTINFO *effect,
							   LONG coefficient, DWORD saturation )
{
	if ( effect->dwDynamicParams & DIEP_TYPESPECIFICPARAMS )
	{
		DWORD update;
		DWORD axis;

		update = 0;

		for ( axis = 0; axis < fb->effect.cAxes; axis++ )
		{
			if ( fb->condition[ axis ].lPositiveCoefficient != coefficient
				|| fb->condition[ axis ].lNegativeCoefficient != coefficient
				|| fb->condition[ axis ].dwPositiveSaturation != saturation
				|| fb->condition[ axis ].dwNegativeSaturation != saturation )
			{
				fb->condition[ axis ].lPositiveCoefficient = coefficient;
				fb->condition[ axis ].lNegativeCoefficient = coefficient;
				fb->condition[ axis ].dwPositiveSaturation = saturation;
				fb->condition[ axis ].dwNegativeSaturation = saturation;
				update = DIEP_TYPESPECIFICPARAMS;
			}
		}
		return update;
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateConstant: force feedback effect type %d (%s) does not support dynamic updating of constant params\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static DWORD FF_UpdateDirection( FeedbackEffect *fb, DIEFFECTINFO *effect,
								FeedbackInfo *feedback,
								VECTOR *direction )
{
	if ( effect->dwDynamicParams & DIEP_DIRECTION )
	{
		DWORD update;
		DWORD axis_num;
		float axis, min_axis, *axis_direction;
		LONG lAxis;

		update = 0;
		axis_direction = (float *) &direction->x;
		min_axis = 0.0F;
		for ( axis_num = 0; axis_num < fb->effect.cAxes; axis_num++ )
		{
			switch ( fb->axes[ axis_num ] )
			{
			case DIJOFS_X:
				axis = feedback->xscale * -axis_direction[ AXIS_XAxis ];
				break;
			case DIJOFS_Y:
				axis = feedback->yscale * axis_direction[ AXIS_ZAxis ];
				break;
			default:
				axis = 0;
			}
			if ( ( axis != 0.0F ) && ( !min_axis || fabs( axis ) < min_axis ) )
				min_axis = (float) fabs( axis );
		}
		for ( axis_num = 0; axis_num < fb->effect.cAxes; axis_num++ )
		{
			switch ( fb->axes[ axis_num ] )
			{
			case DIJOFS_X:
				axis = feedback->xscale * -axis_direction[ AXIS_XAxis ];
				break;
			case DIJOFS_Y:
				axis = feedback->yscale * axis_direction[ AXIS_ZAxis ];
				break;
			default:
				axis = axis_direction[ 0 ];
				DebugPrintf( "FF_UpdateDirection: axis[ %d ] = %d unknown\n",
					axis_num, fb->axes[ axis_num ] );
			}
			if ( min_axis )
			{
				lAxis = (LONG) ( axis / min_axis );
				if ( fb->direction[ axis_num ] != lAxis )
				{
					fb->direction[ axis_num ] = lAxis;
					update |= DIEP_DIRECTION;
				}
			}
		}
		if ( !min_axis )
		{
			if ( effect->dwDynamicParams & DIEP_GAIN )
			{
				fb->effect.dwGain = 0;
				update = DIEP_GAIN;
			}
		}
		return update;
	}
	else
	{
#ifdef DEBUG_FF_DYNAMIC
		DebugPrintf( "FF_UpdateDirection: force feedback effect type %d (%s) does not support dynamic updating of direction\n",
			fb->type, effect_type[ fb->type ] );
#endif
		return 0;
	}
}


static int FF_CheckEffect( int joystick, FF_EffectID id, FF_EffectClass eclass,
						  JOYSTICKINFO **pjoy, FeedbackEffect **pfb, DIEFFECTINFO **peffect )
{
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	FeedbackInfo *feedback;
	DIEFFECTINFO *effect;

	if ( joystick < 0 || joystick >= Num_Joysticks
		|| !JoystickInfo[ joystick ].connected )
	{
		return 0;
	}
	if ( !( diJoystickCaps[ joystick ].dwFlags & DIDC_FORCEFEEDBACK	) )
	{
		return 0;
	}
	joy = &JoystickInfo[ joystick ];
	feedback = joy->feedback;
	if ( !feedback )
	{
		return 0;
	}
	if ( !feedback->enable || feedback->gain < MIN_GAIN )
	{
		return 0;
	}
	if ( !feedback->effect_setting[ id ].enable || feedback->effect_setting[ id ].gain < MIN_GAIN )
	{
		return 0;
	}
	fb = &FB_Effect[ joystick ][ id ];
	effect = &feedback->effect_info[ fb->type ];
	if ( !fb->lpEffect )
	{
		DebugPrintf( "FF_CheckEffect: joystick %s force feedback effect #%d not created\n",
			joy->Name, id );
		return 0;
	}
	if ( eclass != FF_CLASS_MAX && effect_class[ fb->type ] != eclass )
	{
		DebugPrintf( "FF_CheckEffect: class mismatch found for force feedback effect #%d type %d (%s)\n",
			id, fb->type, effect_type[ fb->type ] );
		return 0;
	}

	if ( pjoy )
		*pjoy = joy;
	if ( pfb )
		*pfb = fb;
	if ( peffect )
		*peffect = effect;

	return 1;
}


static int FF_CheckEffectSetting( int joystick, FF_EffectID id, FF_EffectClass eclass,
						  JOYSTICKINFO **pjoy, FeedbackEffect **pfb, DIEFFECTINFO **peffect )
{
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	FeedbackInfo *feedback;
	DIEFFECTINFO *effect;

	if ( joystick < 0 || joystick >= Num_Joysticks
		|| !JoystickInfo[ joystick ].connected )
	{
		return 0;
	}
	if ( !( diJoystickCaps[ joystick ].dwFlags & DIDC_FORCEFEEDBACK	) )
	{
		return 0;
	}
	joy = &JoystickInfo[ joystick ];
	feedback = joy->feedback;
	if ( !feedback )
	{
		return 0;
	}
	fb = &FB_Effect[ joystick ][ id ];
	effect = &feedback->effect_info[ fb->type ];
	if ( !fb->lpEffect )
	{
		DebugPrintf( "FF_CheckEffect: joystick %s force feedback effect #%d not created\n",
			joy->Name, id );
		return 0;
	}
	if ( eclass != FF_CLASS_MAX && effect_class[ fb->type ] != eclass )
	{
		DebugPrintf( "FF_CheckEffect: class mismatch found for force feedback effect #%d type %d (%s)\n",
			id, fb->type, effect_type[ fb->type ] );
		return 0;
	}

	if ( pjoy )
		*pjoy = joy;
	if ( pfb )
		*pfb = fb;
	if ( peffect )
		*peffect = effect;

	return 1;
}


int FF_Supported( int joystick )
{
	DIDEVCAPS *caps;
	
	if ( joystick < 0 || joystick >= Num_Joysticks
		|| !JoystickInfo[ joystick ].connected )
	{
		return 0;
	}
	caps = &diJoystickCaps[ joystick ];
	return ( ( caps->dwFlags & DIDC_FORCEFEEDBACK ) ? 1 : 0 );
}


int FF_CreateEffect( int joystick, FF_EffectID id, FF_EffectType type )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIDEVCAPS *caps;
	
	if ( joystick < 0 || joystick >= Num_Joysticks
		|| !JoystickInfo[ joystick ].connected )
	{
		return 0;
	}
	caps = &diJoystickCaps[ joystick ];
	if ( !( caps->dwFlags & DIDC_FORCEFEEDBACK ) )
	{
		DebugPrintf( "FF_CreateEffect: joystick %s does not support force feedback\n",
			JoystickInfo[ joystick ].Name );
		return 0;
	}
	joy = &JoystickInfo[ joystick ];
	if ( !joy->feedback->effect_info[ type ].dwSize )
	{
		DebugPrintf( "FF_CreateEffect: joystick %s does not support force feedback effect type %d (%s)\n",
			joy->Name, type, effect_type[ type ] );
		return 0;
	}
	fb = &FB_Effect[ joystick ][ id ];
	if ( fb->lpEffect )
	{
		DebugPrintf( "FF_CreateEffect: joystick %s force feedback effect #%d already created\n",
			joy->Name, id );
		return 0;
	}
	FF_SetupCommonDefaults( fb, id, joy->feedback, caps );
	if ( !FF_SetupAxes( joy, fb ) )
	{
		DebugPrintf( "FF_CreateEffect: joystick %s does not support force feedback in X or Y axis\n",
			joy->Name );
		return 0;
	}
	switch ( effect_class[ type ] )
	{
	case FF_CLASS_Constant:
		FF_SetupConstantDefaults( fb, type );
		break;
	case FF_CLASS_Periodic:
		FF_SetupPeriodicDefaults( fb, type );
		break;
	case FF_CLASS_Condition:
		FF_SetupConditionDefaults( fb, type );
		break;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputDevice2_CreateEffect( lpdiJoystick[ joystick ],
		&effect_guid[ type ], &fb->effect, &fb->lpEffect, NULL ) );
	
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_CreateEffect: joystick %s force feedback effect #%d (%s) CreateEffect() failed\n",
			joy->Name, id,
			joy->feedback->effect_info[ type ].tszName );
		fb->lpEffect = NULL;
		return 0;
	}
	return 1;
}


int FF_StartEffect( int joystick, FF_EffectID id )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Start( fb->lpEffect, 1, 0 ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_StartEffect: joystick %s force feedback effect #%d (%s) Start() failed (%s)\n",
			joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
		return 0;
	}
	fb->playing = 1;
	return 1;
}


int FF_StopEffect( int joystick, FF_EffectID id )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Stop( fb->lpEffect ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_StopEffect: joystick %s force feedback effect #%d (%s) Stop() failed\n",
			joy->Name, id, effect->tszName );
		return 0;
	}
	fb->playing = 0;
	return 1;
}


int FF_DownloadEffect( int joystick, FF_EffectID id )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Download( fb->lpEffect ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_DownloadEffect: joystick %s force feedback effect #%d (%s) Download() failed\n",
			joy->Name, id, effect->tszName );
		return 0;
	}
	return 1;
}


int FF_GetEffectState( int joystick, FF_EffectID id, DWORD *state )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_GetEffectStatus( fb->lpEffect, state ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_GetEffectState: joystick %s force feedback effect #%d (%s) GetEffectStatus() failed (%s)\n",
			joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
		*state = err;
		return 0;
	}
	return 1;
}


int FF_CheckEffectDownloaded( int joystick, FF_EffectID id )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	DWORD state;

	if ( !FF_CheckEffect( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_GetEffectStatus( fb->lpEffect, &state ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_CheckEffectDownloaded: joystick %s force feedback effect #%d (%s) GetEffectStatus() failed (%s)\n",
			joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
		if ( err == DIERR_NOTDOWNLOADED )
		{
			err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Download( fb->lpEffect ) );
			if ( FAILED( err ) )
			{
				DebugPrintf( "FF_CheckEffectDownloaded: joystick %s force feedback effect #%d (%s) Download() failed\n",
					joy->Name, id, effect->tszName );
				return 0;
			}
			else
			{
				DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) downloaded (%s)\n",
					joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
				err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_GetEffectStatus( fb->lpEffect, &state ) );
				if ( FAILED( err ) )
				{
					DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) GetEffectStatus() failed (%s) even after downloading\n",
						joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
					return 0;
				}
			}
		}
		else
			return 0;
	}
	fb->playing = ( state & DIEGES_PLAYING ) ? 1 : 0;
	return 1;
}


int FF_CheckEffectPlaying( int joystick, FF_EffectID id )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	DWORD state;

	if ( !FF_CheckEffect( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_GetEffectStatus( fb->lpEffect, &state ) );
	if ( FAILED( err ) )
	{
		DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) GetEffectStatus() failed (%s)\n",
			joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
		if ( err == DIERR_NOTDOWNLOADED )
		{
			err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Download( fb->lpEffect ) );
			if ( FAILED( err ) )
			{
				DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) Download() failed\n",
					joy->Name, id, effect->tszName );
				return 0;
			}
			else
			{
				DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) downloaded (%s)\n",
					joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
				err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_GetEffectStatus( fb->lpEffect, &state ) );
				if ( FAILED( err ) )
				{
					DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) GetEffectStatus() failed (%s) even after downloading\n",
						joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
					return 0;
				}
			}
		}
		else
			return 0;
	}
	if ( !( state & DIEGES_PLAYING ) )
	{
		fb->playing = 0;
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_Start( fb->lpEffect, 1, 0 ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) Start() failed (%s)\n",
				joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
			return 0;
		}
		DebugPrintf( "FF_CheckEffectPlaying: joystick %s force feedback effect #%d (%s) restarted (%s)\n",
			joy->Name, id, effect->tszName, FF_ErrorMsg( err ) );
	}
	fb->playing = 1;
	return 1;
}


int FF_EffectPlaying( int joystick, FF_EffectID id )
{
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	return fb->playing;
}


int FF_EffectActive( int joystick, FF_EffectID id )
{
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;

	if ( !FF_CheckEffect( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	return 1;
}


int FF_SetEffectEnable( int joystick, FF_EffectID id, int enable )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	FeedbackInfo *feedback;
	DWORD update;

	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}
	feedback = joy->feedback;
	update = FF_UpdateGain( fb, effect,
		(DWORD) ( ( feedback->enable && enable ) ?
				  ( feedback->gain * feedback->effect_setting[ id ].gain * DI_FFNOMINALMAX ) : 0 ) );

	if ( update )
	{
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_SetParameters( fb->lpEffect, &fb->effect, update ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_SetEffectEnable: joystick %s force feedback effect #%d type %d (%s) SetParameters() failed\n",
				JoystickInfo[ joystick ].Name, id, fb->type, effect_type[ fb->type ] );
			return 0;
		}
		if ( update & DIEP_START )
			fb->playing = 1;
		joy->feedback->effect_setting[ id ].enable = enable;
	}

	return 1;
}


int FF_SetEffectGain( int joystick, FF_EffectID id, float gain )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	FeedbackInfo *feedback;
	DWORD update;
	
	if ( !FF_CheckEffectSetting( joystick, id, FF_CLASS_MAX, &joy, &fb, &effect ) )
	{
		return 0;
	}

	feedback = joy->feedback;
	update = FF_UpdateGain( fb, effect,
		(DWORD) ( ( feedback->enable && feedback->effect_setting[ id ].enable ) ?
				  ( feedback->gain * gain * DI_FFNOMINALMAX ) : 0 ) );

	if ( update )
	{
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_SetParameters( fb->lpEffect, &fb->effect, update ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_SetEffectGain: joystick %s force feedback effect #%d type %d (%s) SetParameters() failed\n",
				JoystickInfo[ joystick ].Name, id, fb->type, effect_type[ fb->type ] );
			return 0;
		}
		if ( update & DIEP_START )
			fb->playing = 1;
		joy->feedback->effect_setting[ id ].gain = gain;
	}

	return 1;
}


int FF_UpdatePeriodicEffect( int joystick, FF_EffectID id, 
							float magnitude, float offset, float duration, float frequency,
							float attack_level, float attack_time,
							float fade_level, float fade_time,
							VECTOR *direction, DWORD flags )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	DWORD update;
	FeedbackInfo *feedback;
	
	if ( !FF_CheckEffect( joystick, id, FF_CLASS_Periodic, &joy, &fb, &effect ) )
	{
		return 0;
	}

	update = fb->update | flags;
	feedback = joy->feedback;

	update |= FF_UpdateGain( fb, effect,
		(DWORD) ( ( feedback->enable && feedback->effect_setting[ id ].enable ) ?
				  ( feedback->gain * feedback->effect_setting[ id ].gain * DI_FFNOMINALMAX ) : 0 ) );

	update |= FF_UpdateDuration( fb, effect,
		( duration > 0.0F ) ? (DWORD ) ( duration * DI_SECONDS ) : INFINITE );

	update |= FF_UpdateEnvelope( fb, effect,
		(DWORD) ( attack_level * DI_FFNOMINALMAX ),
		(DWORD) ( attack_time * DI_SECONDS ),
		(DWORD) ( fade_level * DI_FFNOMINALMAX ),
		(DWORD) ( fade_time * DI_SECONDS ) );

	update |= FF_UpdatePeriodic( fb, effect,
		(DWORD) ( magnitude * DI_FFNOMINALMAX ),
		(DWORD) ( offset * DI_FFNOMINALMAX ),
		(DWORD) ( DI_SECONDS / frequency ) );

	update |= FF_UpdateDirection( fb, effect, joy->feedback, direction );

	if ( update )
	{
		fb->update = update;
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_SetParameters( fb->lpEffect, &fb->effect, update ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_UpdatePeriodicEffect: joystick %s force feedback effect #%d type %d (%s) SetParameters() failed\n",
				JoystickInfo[ joystick ].Name, id, fb->type, effect_type[ fb->type ] );
			return 0;
		}
		if ( update & DIEP_START )
			fb->playing = 1;
		fb->update = 0;
	}

	return 1;
}



int FF_UpdateConstantEffect( int joystick, FF_EffectID id, 
							float magnitude, float duration,
							float attack_level, float attack_time,
							float fade_level, float fade_time,
							VECTOR *direction, DWORD flags )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	DWORD update;
	FeedbackInfo *feedback;
	
	if ( !FF_CheckEffect( joystick, id, FF_CLASS_Constant, &joy, &fb, &effect ) )
	{
		return 0;
	}

	update = fb->update | flags;
	feedback = joy->feedback;

	update |= FF_UpdateGain( fb, effect,
		(DWORD) ( ( feedback->enable && feedback->effect_setting[ id ].enable ) ?
				  ( feedback->gain * feedback->effect_setting[ id ].gain * DI_FFNOMINALMAX ) : 0 ) );

	update |= FF_UpdateDuration( fb, effect,
		( duration > 0.0F ) ? (DWORD ) ( duration * DI_SECONDS ) : INFINITE );

	update |= FF_UpdateEnvelope( fb, effect,
		(DWORD) ( attack_level * DI_FFNOMINALMAX ),
		(DWORD) ( attack_time * DI_SECONDS ),
		(DWORD) ( fade_level * DI_FFNOMINALMAX ),
		(DWORD) ( fade_time * DI_SECONDS ) );

	update |= FF_UpdateConstant( fb, effect,
		(DWORD) ( magnitude * DI_FFNOMINALMAX ) );

	update |= FF_UpdateDirection( fb, effect, joy->feedback, direction );

	if ( update )
	{
		fb->update = update;
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_SetParameters( fb->lpEffect, &fb->effect, update ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_UpdateConstantEffect: joystick %s force feedback effect #%d type %d (%s) SetParameters() failed\n",
				JoystickInfo[ joystick ].Name, id, fb->type, effect_type[ fb->type ] );
			DebugPrintf( "FF_UpdateConstantEffect: err %08X (%s)\n",
				err, FF_ErrorMsg( err ) );
			return 0;
		}
		if ( update & DIEP_START )
			fb->playing = 1;
		fb->update = 0;
	}

	return 1;
}



int FF_UpdateConditionEffect( int joystick, FF_EffectID id, 
							 float duration, float coefficient, float saturation, DWORD flags )
{
	HRESULT err;
	JOYSTICKINFO *joy;
	FeedbackEffect *fb;
	DIEFFECTINFO *effect;
	DWORD update;
	FeedbackInfo *feedback;
	float gain;
	
	if ( !FF_CheckEffect( joystick, id, FF_CLASS_Condition, &joy, &fb, &effect ) )
	{
		return 0;
	}

	update = fb->update | flags;
	feedback = joy->feedback;

	gain = ( feedback->enable && feedback->effect_setting[ id ].enable ) ?
		( feedback->gain * feedback->effect_setting[ id ].gain ) : 0.0F;
	update |= FF_UpdateGain( fb, effect,
		(DWORD) ( gain * DI_FFNOMINALMAX ) );

	update |= FF_UpdateDuration( fb, effect,
		( duration > 0.0F ) ? (DWORD ) ( duration * DI_SECONDS ) : INFINITE );

	update |= FF_UpdateCondition( fb, effect,
		(LONG) ( gain * coefficient * DI_FFNOMINALMAX ),
		(DWORD) ( gain * saturation * DI_FFNOMINALMAX ) );

	if ( update )
	{
		fb->update = update;
		err = JOYSTICK_COMMAND( joystick, IDirectInputEffect_SetParameters( fb->lpEffect, &fb->effect, update ) );
		if ( FAILED( err ) )
		{
			DebugPrintf( "FF_UpdateConditionEffect: joystick %s force feedback effect #%d type %d (%s) SetParameters() failed\n",
				JoystickInfo[ joystick ].Name, id, fb->type, effect_type[ fb->type ] );
			return 0;
		}
		if ( update & DIEP_START )
			fb->playing = 1;
		fb->update = 0;
	}

	return 1;
}
