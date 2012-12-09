#ifndef RTLIGHT_H
#define RTLIGHT_H

typedef enum
{
	LIGHT_FIXED,
	LIGHT_PULSING,
	LIGHT_FLICKERING,
	LIGHT_SPOT,
} LIGHTTYPE;

typedef enum
{
	GENTYPE_Initialised,		// Initialised immediatly
	GENTYPE_Time,				// Initialised after time
	GENTYPE_Trigger,			// Initialised by trigger
} GENTYPE;

typedef enum
{
	STATE_OFF,
	STATE_TURNING_ON,
	STATE_ON,
	STATE_TURNING_OFF,
} LIGHTSTATE;

typedef enum
{
	PULSE_BLINK,
	PULSE_RAMP,
	PULSE_HALFWAVE,
	PULSE_WAVE,
} PULSETYPE;

typedef struct
{
	PULSETYPE on_type;
	PULSETYPE off_type;
	float on_time;
	float off_time;
} RT_FIXED_LIGHT;

typedef struct
{
	PULSETYPE type;
	float on_time;
	float stay_on_time;
	float off_time;
	float stay_off_time;
	float stay_on_point;
	float off_point;
	float total_time;
} RT_PULSING_LIGHT;

typedef struct
{
	float stay_on_chance;
	float stay_off_chance;
	float stay_on_time;
	float stay_off_time;
} RT_FLICKERING_LIGHT;

typedef struct
{
	VECTOR dir;
	VECTOR up;
	float cone;
	float rotation_period;
	float rotation_speed;
	float angle;
} RT_SPOT_LIGHT;

typedef struct _RTLIGHT
{
	LIGHTTYPE type;
	u_int16_t group;
	VECTOR pos;
	float range;
	float r;
	float g;
	float b;
	GENTYPE generation_type;
	float generation_delay;
	union
	{
		RT_FIXED_LIGHT fixed;
		RT_PULSING_LIGHT pulse;
		RT_FLICKERING_LIGHT flicker;
		RT_SPOT_LIGHT spot;
	};
	u_int16_t xlight;
	bool enabled;
	LIGHTSTATE state;
	float now_time;
	float delay;
	float intensity;
} RT_LIGHT;


extern bool LoadRTLights( void );

extern bool ReleaseRTLights( void );

extern void ProcessRTLights( void );

extern void EnableRTLight( u_int16_t *data );

extern void DisableRTLight( u_int16_t *data );

FILE * SaveRealTimeLights( FILE * fp );
FILE * LoadRealTimeLights( FILE * fp );

#endif // RTLIGHT_H
