/*
 * The X Men, October 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 7 $
 *
 * $Header: /PcProjectX/Include/ExtForce.h 7     17/02/98 16:35 Collinsd $
 *
 * $Log: /PcProjectX/Include/ExtForce.h $
 * 
 * 7     17/02/98 16:35 Collinsd
 * Added ExternalForce and Teleports load/save function.
 * 
 * 6     18/11/97 12:40 Oliverc
 * Added origin, up, width, and height to external force, and changed
 * force calculation accordingly
 * 
 * 5     24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 4     10/17/97 11:48a Phillipd
 * 
 * 3     10/14/97 4:49p Phillipd
 * 
 * 2     10/09/97 3:30p Phillipd
 * External Forces coming...
 * Shield and hull are now floats
 * 
 * 1     10/09/97 12:16p Phillipd
 * 
 */
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

