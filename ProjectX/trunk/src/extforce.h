
#ifndef EXTFORCE_INCLUDED
#define EXTFORCE_INCLUDED

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
		uint16	Status;
		uint16	ForceType;
		uint16	Group;
		VECTOR	Origin;
		VECTOR	Pos;
		VECTOR	Dir;
		VECTOR	Up;
		float	MaxForce;
		float	MinForce;
		float	Width;
		float	Height;
		float	Range;
		uint16	Type;				// sphere / box / convex hull
		VECTOR	half_size;
		TRIGGER_ZONE	*Zone;
		uint16	num_sides;
struct	EXTERNALFORCE * NextInGroup;
}EXTERNALFORCE;

/*
 * fn prototypes
 */
void ReleaseExternalForces( void );
BOOL ExternalForcesLoad( char * Filename );
void ExternalForcesZoneCheck( VECTOR * OldPos , VECTOR * NewPos , EXTERNALFORCE * EFpnt );
BOOL ExternalForcesAreaCheck( VECTOR * OldPos , VECTOR * NewPos , uint16 Group , VECTOR * ExtForce , float * Shield);
void StartExternalForce( uint16 * Data );
void StopExternalForce( uint16 * Data );
void DisplayExternalForcesInGroup( uint16 Group );
FILE * LoadExternalForces( FILE * fp );
FILE * SaveExternalForces( FILE * fp );

#endif	// EXTFORCE_INCLUDED

