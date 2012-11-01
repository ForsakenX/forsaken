#include <stdio.h>
#include <math.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "compobjects.h"
#include "object.h"
#include "networking.h"
#include "triggers.h"
#include "pickups.h"
#include "util.h"
#include "rtlight.h"
#include "lights.h"


#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif




/******************************************************************************************
	Defines
*******************************************************************************************/
#define RTL_VERSION_NUMBER	2


#define HALF_PI		(0.5F * PI)
#define TWO_PI		(2.0F * PI)


#define fread_var( F, V ) \
	if ( fread( &(V), sizeof( V ), 1, (F) ) != 1 ) \
	{\
		rt_lights = 0;\
		fclose( F );\
		Msg( "LoadRTLights() error reading '" #V "' from Real-Time Lights (.RTL) file " );\
		return false;\
	}


#define RANDOM() ( (float) rand() / RAND_MAX )


/******************************************************************************************
	Externals
*******************************************************************************************/
extern	float			framelag;
extern	char			LevelNames[MAXLEVELS][128];
extern	XLIGHT			XLights[ MAXXLIGHTS ];




/******************************************************************************************
	Globals
*******************************************************************************************/
u_int16_t rt_lights = 0;
RT_LIGHT *rt_light = NULL;




/******************************************************************************************
	Load real-time game lights for level
*******************************************************************************************/
extern int16_t		LevelNum;
bool LoadRTLights( void )
{
	FILE	*	fp;
	int8_t		Filename[ 256 ];
	char	*	NewExt = ".RTL";
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	int			j;
	RT_LIGHT *	light;
	u_int16_t		type;
	XLIGHT	*	xlight;
	RT_FIXED_LIGHT		*fixed;
	RT_PULSING_LIGHT	*pulse;
	RT_FLICKERING_LIGHT	*flicker;
	RT_SPOT_LIGHT		*spot;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &Filename[ 0 ], NewExt );


	fp = file_open( &Filename[ 0 ], "rb" );

	if( fp )
	{
		fread_var( fp, MagicNumber );
		fread_var( fp, VersionNumber );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != RTL_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadRTLights() Incompatible Real-Time Lights (.RTL) file %s", &Filename[ 0 ] );
			return( false );
		}

		fread_var( fp, rt_lights );

		if ( rt_lights )
		{
			rt_light = (RT_LIGHT *) calloc( rt_lights, sizeof( RT_LIGHT ) );
			if ( !rt_light )
			{
				fclose( fp );
				Msg( "LoadRTLights() malloc failed for %d Real-Time Lights ", rt_lights );
				rt_lights = 0;
				return false ;
			}

			for ( j = 0; j < rt_lights; j++ )
			{
				rt_light[ j ].xlight = (u_int16_t) -1;
				rt_light[ j ].enabled = false;
			}

			for ( j = 0; j < rt_lights; j++ )
			{
				light = &rt_light[ j ];
				fread_var( fp, type );
				light->type = (LIGHTTYPE) type;
				fread_var( fp, light->group );
				fread_var( fp, light->pos.x );
				fread_var( fp, light->pos.y );
				fread_var( fp, light->pos.z );
				fread_var( fp, light->range );
				fread_var( fp, light->r );
				fread_var( fp, light->g );
				fread_var( fp, light->b );
				light->r *= 255.0F;
				light->g *= 255.0F;
				light->b *= 255.0F;
				fread_var( fp, type );
				light->generation_type = (GENTYPE) type;
				fread_var( fp, light->generation_delay );
				switch ( light->type )
				{
				case LIGHT_FIXED:
					fixed = &light->fixed;
					fread_var( fp, type );
					fixed->on_type = (PULSETYPE) type;
					fread_var( fp, type );
					fixed->off_type = (PULSETYPE) type;
					fread_var( fp, fixed->on_time );
					fread_var( fp, fixed->off_time );
					fixed->on_time *= ANIM_SECOND;
					fixed->off_time *= ANIM_SECOND;
					break;
				case LIGHT_PULSING:
					pulse = &light->pulse;
					fread_var( fp, type );
					pulse->type = (PULSETYPE) type;
					fread_var( fp, pulse->on_time );
					fread_var( fp, pulse->stay_on_time );
					fread_var( fp, pulse->off_time );
					fread_var( fp, pulse->stay_off_time );
					pulse->on_time *= ANIM_SECOND;
					pulse->stay_on_time *= ANIM_SECOND;
					pulse->off_time *= ANIM_SECOND;
					pulse->stay_off_time *= ANIM_SECOND;
					pulse->stay_on_point = pulse->on_time + pulse->stay_on_time;
					pulse->off_point = pulse->stay_on_point + pulse->off_time;
					pulse->total_time = pulse->off_point + pulse->stay_off_time;
					break;
				case LIGHT_FLICKERING:
					flicker = &light->flicker;
					fread_var( fp, flicker->stay_on_chance );
					fread_var( fp, flicker->stay_off_chance );
					fread_var( fp, flicker->stay_on_time );
					fread_var( fp, flicker->stay_off_time );
					flicker->stay_on_time *= ANIM_SECOND;
					flicker->stay_off_time *= ANIM_SECOND;
					break;
				case LIGHT_SPOT:
					spot = &light->spot;
					fread_var( fp, spot->dir.x );
					fread_var( fp, spot->dir.y );
					fread_var( fp, spot->dir.z );
					fread_var( fp, spot->up.x );
					fread_var( fp, spot->up.y );
					fread_var( fp, spot->up.z );
					fread_var( fp, spot->cone );
					fread_var( fp, spot->rotation_period );
					if ( spot->rotation_period )
						spot->rotation_speed = 2 * PI / ( spot->rotation_period * ANIM_SECOND );
					else
						spot->rotation_speed = 0.0F;
					spot->angle = 0.0F;
					break;
				}
				light->now_time = 0.0F;
				switch ( light->generation_type )
				{
				case GENTYPE_Initialised:
					light->enabled = true;
					light->state = STATE_TURNING_ON;
					light->delay = 0.0F;
					break;
				case GENTYPE_Time:
					light->enabled = false;
					light->state = STATE_OFF;
					light->delay = light->generation_delay * ANIM_SECOND;
					break;
				case GENTYPE_Trigger:
					light->enabled = false;
					light->state = STATE_OFF;
					light->delay = 0.0F;
					break;
				}
				light->xlight = FindFreeXLight();
				if ( light->xlight != (u_int16_t) -1 )
				{
					xlight = &XLights[ light->xlight ];
					xlight->Pos = light->pos;
					xlight->Size = light->range;
					xlight->SizeCount = 0.0F;
					xlight->r = light->r;
					xlight->g = light->g;
					xlight->b = light->b;
					xlight->Group = light->group;
					xlight->Visible = false;
					if ( light->type == LIGHT_SPOT )
					{
						xlight->Type = SPOT_LIGHT;
						xlight->CosArc = (float) cos( D2R( light->spot.cone * 0.5F ) );
					}
				}
			}
		}
		fclose( fp );
	}

	return true;
}


/******************************************************************************************
	Release real-time game lights for level
*******************************************************************************************/
bool ReleaseRTLights( void )
{
	int j;
	RT_LIGHT *light;

	if ( rt_light )
	{
		for ( j = 0; j < rt_lights; j++ )
		{
			light = &rt_light[ j ];
			if ( light->xlight != (u_int16_t) -1 )
			{
				KillUsedXLight( light->xlight );
				light->xlight = (u_int16_t) -1;
			}
		}

		free( rt_light );
	}
	rt_light = NULL;
	rt_lights = 0;

	return true;
}



static void InterpLightOn( RT_LIGHT *light, float t, PULSETYPE type )
{
	XLIGHT *xlight;

	xlight = &XLights[ light->xlight ];
	switch ( type )
	{
	case PULSE_BLINK:
		t = 1.0F;
		break;
	case PULSE_RAMP:
		break;
	case PULSE_HALFWAVE:
		t = (float) sin( HALF_PI * t );
		break;
	case PULSE_WAVE:
		t = 0.5F + 0.5F * (float) sin( PI * t - HALF_PI );
		break;
	}
	xlight->r = t * light->r;
	xlight->g = t * light->g;
	xlight->b = t * light->b;
	xlight->Visible = ( t > 0.0F ) ? true : false;
	light->intensity = t;
}


static void InterpLightOff( RT_LIGHT *light, float t, PULSETYPE type )
{
	XLIGHT *xlight;

	xlight = &XLights[ light->xlight ];
	switch ( type )
	{
	case PULSE_BLINK:
		t = 0.0F;
		break;
	case PULSE_RAMP:
		t = 1.0F - t;
		break;
	case PULSE_HALFWAVE:
		t = (float) sin( HALF_PI + HALF_PI * t );
		break;
	case PULSE_WAVE:
		t = 0.5F + 0.5F * (float) sin( PI * t + HALF_PI );
		break;
	}
	xlight->r = t * light->r;
	xlight->g = t * light->g;
	xlight->b = t * light->b;
	xlight->Visible = ( t > 0.0F ) ? true : false;
	light->intensity = t;
}


static float TurnOnPoint( RT_LIGHT *light, PULSETYPE type )
{
	float t = 0.0f;

	switch ( type )
	{
	case PULSE_BLINK:
		t = light->intensity;
		break;
	case PULSE_RAMP:
		t = light->intensity;
		break;
	case PULSE_HALFWAVE:
		t = (float) asin( light->intensity ) / HALF_PI;
		break;
	case PULSE_WAVE:
		t = ( (float) asin( light->intensity ) + HALF_PI ) / PI;
		break;
	}

	return t;
}


static float TurnOffPoint( RT_LIGHT *light, PULSETYPE type )
{
	float t = 0.0f;

	switch ( type )
	{
	case PULSE_BLINK:
		t = 1.0F - light->intensity;
		break;
	case PULSE_RAMP:
		t = 1.0F - light->intensity;
		break;
	case PULSE_HALFWAVE:
		t = ( HALF_PI - (float) asin( light->intensity ) ) / HALF_PI;
		break;
	case PULSE_WAVE:
		t = ( HALF_PI - (float) asin( light->intensity ) ) / PI;
		break;
	}

	return t;
}


/******************************************************************************************
	Process real-time game lights
*******************************************************************************************/
void ProcessRTLights( void )
{
	int j;
	RT_LIGHT *light;
	XLIGHT *xlight;
	RT_FIXED_LIGHT *fixed;
	RT_PULSING_LIGHT *pulse;
	RT_FLICKERING_LIGHT *flicker;
	RT_SPOT_LIGHT *spot;
	float chance;
	MATRIX rotmat;

	for ( j = 0; j < rt_lights; j++ )
	{
		light = &rt_light[ j ];
		if ( light->xlight == (u_int16_t) -1 )
			continue; // only happens if run out of XLIGHTs
		xlight = &XLights[ light->xlight ];
		if ( light->delay > 0.0F )
		{
			light->delay -= framelag;
			if ( light->delay <= 0.0F )
			{
				light->enabled = true;
			}
		}
		if ( light->enabled )
		{
			switch ( light->type )
			{
			case LIGHT_FIXED:
				fixed = &light->fixed;
				switch ( light->state )
				{
				case STATE_OFF:
					light->now_time = 0.0F;
					xlight->Visible = false;
					light->intensity = 0.0F;
					break;
				case STATE_TURNING_ON:
					if ( light->delay < 0.0F )
					{
						light->now_time += -light->delay;
						light->delay = 0.0F;
					}
					else
						light->now_time += framelag;
					xlight->Visible = true;
					if ( light->now_time < fixed->on_time )
					{
						InterpLightOn( light, light->now_time / fixed->on_time, fixed->on_type );
					}
					else
					{
						xlight->r = light->r;
						xlight->g = light->g;
						xlight->b = light->b;
						xlight->Visible = true;
						light->state = STATE_ON;
					}
					break;
				case STATE_ON:
					xlight->r = light->r;
					xlight->g = light->g;
					xlight->b = light->b;
					xlight->Visible = true;
					break;
				case STATE_TURNING_OFF:
					light->now_time += framelag;
					xlight->Visible = true;
					if ( light->now_time < fixed->off_time )
					{
						InterpLightOff( light, light->now_time / fixed->off_time, fixed->off_type );
					}
					else
					{
						light->state = STATE_OFF;
						light->now_time = 0.0F;
						xlight->Visible = false;
					}
					break;
				}
				break;
			case LIGHT_PULSING:
				pulse = &light->pulse;
				if ( light->delay < 0.0F )
				{
					light->now_time += -light->delay;
					light->delay = 0.0F;
				}
				else
					light->now_time += framelag;
				if ( light->now_time > pulse->total_time )
				{
					light->now_time = FMOD( light->now_time, pulse->total_time );
				}
				if ( light->now_time < pulse->on_time )
				{
					// light is turning on
					light->state = STATE_TURNING_ON;
					InterpLightOn( light, light->now_time / pulse->on_time, pulse->type );
					xlight->Visible = true;
				}
				else if ( light->now_time < pulse->stay_on_point )
				{
					// light is staying on
					light->state = STATE_ON;
					xlight->r = light->r;
					xlight->g = light->g;
					xlight->b = light->b;
					xlight->Visible = true;
				}
				else if ( light->now_time < pulse->off_point )
				{
					// light is turning off
					light->state = STATE_TURNING_OFF;
					InterpLightOff( light, ( light->now_time - pulse->stay_on_point ) / pulse->off_time, pulse->type );
					xlight->Visible = true;
				}
				else // light->now_time < pulse->total_time
				{
					// light is staying off
					light->state = STATE_OFF;
					xlight->Visible = false;
				}
				break;
			case LIGHT_FLICKERING:
				flicker = &light->flicker;
				chance = RANDOM();
				if ( light->delay < 0.0F )
				{
					light->now_time += -light->delay;
					light->delay = 0.0F;
				}
				else
					light->now_time += framelag;
				if ( light->state == STATE_ON )
				{
					// check chance of switching off
					if ( light->now_time > flicker->stay_on_time &&
						chance > flicker->stay_on_chance )
					{
						light->state = STATE_OFF;
						light->now_time = 0.0F;
						xlight->Visible = false;
					}
				}
				else // light is off
				{
					// check chance of switching on
					if ( light->now_time > flicker->stay_off_time &&
						chance > flicker->stay_off_chance )
					{
						light->state = STATE_ON;
						light->now_time = 0.0F;
						xlight->Visible = true;
					}
				}
				break;
			case LIGHT_SPOT:
				spot = &light->spot;
				if ( spot->rotation_speed )
				{
					// rotate spotlight beam
					if ( light->delay < 0.0F )
					{
						light->now_time += -light->delay;
						light->delay = 0.0F;
					}
					else
						light->now_time += framelag;
					spot->angle = light->now_time * spot->rotation_speed;
					if ( spot->angle > TWO_PI )
						spot->angle = FMOD( spot->angle, TWO_PI );
					MatrixFromAxisAndAngle( spot->angle, &spot->up, &rotmat );
					ApplyMatrix( &rotmat, &spot->dir, &xlight->Dir );
					NormaliseVector( &xlight->Dir );
				}
				light->state = STATE_ON;
				xlight->Visible = true;
				break;
			}
		}
		else
		{
			// light is disabled, check if turning off
			if ( light->state == STATE_TURNING_OFF )
			{
				switch ( light->type )
				{
				case LIGHT_FIXED:
					fixed = &light->fixed;
					light->now_time += framelag;
					if ( light->now_time < fixed->off_time )
					{
						InterpLightOff( light, light->now_time / fixed->off_time, fixed->off_type );
						xlight->Visible = true;
					}
					else
					{
						light->state = STATE_OFF;
						light->now_time = 0.0F;
						xlight->Visible = false;
					}
					break;
				case LIGHT_PULSING:
					pulse = &light->pulse;
					light->now_time += framelag;
					if ( light->now_time < pulse->off_time )
					{
						InterpLightOff( light, light->now_time / pulse->off_time, pulse->type );
						xlight->Visible = true;
					}
					else
					{
						light->state = STATE_OFF;
						light->now_time = 0.0F;
						xlight->Visible = false;
					}
					break;
				case LIGHT_FLICKERING:
					light->state = STATE_OFF;
					xlight->Visible = false;
					break;
				case LIGHT_SPOT:
					light->state = STATE_OFF;
					xlight->Visible = false;
					break;
				}
			}
			else if ( light->delay <= 0.0F )
			{
				light->state = STATE_OFF;
				xlight->Visible = false;
			}
		}
	}
}



/******************************************************************************************
	Enable real-time game light
*******************************************************************************************/
void EnableRTLight( u_int16_t *data )
{
	int id;
	RT_LIGHT *light;

	id = *data;
	if ( id >= rt_lights )
		return;
	light = &rt_light[ id ];
	if ( !light->enabled )
	{
		light->delay = light->generation_delay * ANIM_SECOND;
		if ( !light->delay )
			light->enabled = true;
	}
	switch ( light->state )
	{
	case STATE_OFF:
		light->state = STATE_TURNING_ON;
		light->now_time = 0.0F;
		break;
	case STATE_TURNING_ON:
		break;
	case STATE_ON:
		break;
	case STATE_TURNING_OFF:
		light->state = STATE_TURNING_ON;
		switch ( light->type )
		{
		case LIGHT_FIXED:
			light->now_time = light->fixed.on_time * TurnOnPoint( light, light->fixed.on_type );
			break;
		case LIGHT_PULSING:
			light->now_time = light->pulse.on_time * TurnOnPoint( light, light->pulse.type );
			break;
		case LIGHT_FLICKERING:
			break;
		case LIGHT_SPOT:
			break;
		}
		break;
	}
}



/******************************************************************************************
	Disable real-time game light
*******************************************************************************************/
void DisableRTLight( u_int16_t *data )
{
	int id;
	RT_LIGHT *light;

	id = *data;
	if ( id >= rt_lights )
		return;
	light = &rt_light[ id ];
	light->enabled = false;
	switch ( light->state )
	{
	case STATE_OFF:
		break;
	case STATE_TURNING_ON:
		light->state = STATE_TURNING_OFF;
		switch ( light->type )
		{
		case LIGHT_FIXED:
			light->now_time = light->fixed.off_time * TurnOffPoint( light, light->fixed.off_type );
			break;
		case LIGHT_PULSING:
			light->now_time = light->pulse.off_time * TurnOffPoint( light, light->pulse.type );
			break;
		}
		break;
	case STATE_ON:
		light->state = STATE_TURNING_OFF;
		light->now_time = 0.0F;
		break;
	case STATE_TURNING_OFF:
		light->now_time -= light->pulse.stay_on_point;
		break;
	}
}

/*===================================================================
	Procedure	:	Save RTLights Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveRealTimeLights( FILE * fp )
{
	int			i;
	RT_LIGHT *	RTLPtr;

	if( fp )
	{
		fwrite( &rt_lights, sizeof( u_int16_t ), 1, fp );

		RTLPtr = rt_light;
		
		for( i = 0; i < rt_lights; i++ )
		{
			fwrite( &RTLPtr->xlight, sizeof( u_int16_t ), 1, fp );
			fwrite( &RTLPtr->enabled, sizeof( bool ), 1, fp );
			fwrite( &RTLPtr->state, sizeof( LIGHTSTATE ), 1, fp );
			fwrite( &RTLPtr->now_time, sizeof( float ), 1, fp );
			fwrite( &RTLPtr->delay, sizeof( float ), 1, fp );
			fwrite( &RTLPtr->intensity, sizeof( float ), 1, fp );
			RTLPtr++;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load RTLights Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadRealTimeLights( FILE * fp )
{
	int			i;
	RT_LIGHT *	RTLPtr;
	u_int16_t		NumLights;

	if( fp )
	{
		fread( &NumLights, sizeof( u_int16_t ), 1, fp );

		if( NumLights != rt_lights )
		{
			fclose( fp );
			return( NULL );
		}

		RTLPtr = rt_light;
		
		for( i = 0; i < rt_lights; i++ )
		{
			fread( &RTLPtr->xlight, sizeof( u_int16_t ), 1, fp );
			fread( &RTLPtr->enabled, sizeof( bool ), 1, fp );
			fread( &RTLPtr->state, sizeof( LIGHTSTATE ), 1, fp );
			fread( &RTLPtr->now_time, sizeof( float ), 1, fp );
			fread( &RTLPtr->delay, sizeof( float ), 1, fp );
			fread( &RTLPtr->intensity, sizeof( float ), 1, fp );
			RTLPtr++;
		}
	}

	return( fp );
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
