
#ifndef EXTFORCE_INCLUDED
#define EXTFORCE_INCLUDED

#include "main.h"
/*
 * Defines
 */
#define	EXTERNALFORCEACTIVE 0
#define	EXTERNALFORCEINACTIVE 1

#define	EXTERNALFORCETYPE_Move 0
#define	EXTERNALFORCETYPE_Shield 1
#define	EXTERNALFORCETYPE_Shake 2
 
/*
 * Structures
 */
typedef struct EXTERNALFORCE{
		u_int16_t	Status;
		u_int16_t	ForceType;
		u_int16_t	Group;
		VECTOR	Origin;
		VECTOR	Pos;
		VECTOR	Dir;
		VECTOR	Up;
		float	MaxForce;
		float	MinForce;
		float	Width;
		float	Height;
		float	Range;
		u_int16_t	Type;				// sphere / box / convex hull
		VECTOR	half_size;
		TRIGGER_ZONE	*Zone;
		u_int16_t	num_sides;
struct	EXTERNALFORCE * NextInGroup;
}EXTERNALFORCE;

/*
 * fn prototypes
 */
void ReleaseExternalForces( void );
bool ExternalForcesLoad( char * Filename );
void ExternalForcesZoneCheck( VECTOR * OldPos , VECTOR * NewPos , EXTERNALFORCE * EFpnt );
bool ExternalForcesAreaCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group , VECTOR * ExtForce , float * Shield);
void StartExternalForce( u_int16_t * Data );
void StopExternalForce( u_int16_t * Data );
void DisplayExternalForcesInGroup( u_int16_t Group );
FILE * LoadExternalForces( FILE * fp );
FILE * SaveExternalForces( FILE * fp );

#endif	// EXTFORCE_INCLUDED

