/*
 * The X Men, October 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 8 $
 *
 * $Header: /PcProjectX/Include/Teleport.h 8     9/03/98 20:13 Oliverc $
 *
 * $Log: /PcProjectX/Include/Teleport.h $
 * 
 * 8     9/03/98 20:13 Oliverc
 * Changed teleports to orient player's bike consistently using direction
 * and up vector
 * 
 * 7     17/02/98 16:35 Collinsd
 * Added ExternalForce and Teleports load/save function.
 * 
 * 6     11/19/97 12:37p Phillipd
 * 
 * 5     24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 4     10/21/97 5:07p Phillipd
 * 
 * 3     10/20/97 3:50p Phillipd
 * 
 * 2     10/14/97 4:49p Phillipd
 * 
 * 1     10/13/97 10:37a Phillipd
 * 
 */
#ifndef TELEPORT_INCLUDED
#define TELEPORT_INCLUDED

/*
 * Defines
 */
#define	TELEPORTINACTIVE 1
#define	TELEPORTACTIVE 0

#define	TELEPORTSHIPS 1
#define	TELEPORTENEMIES 2
#define	MAXTELEPORTLINKS 8
/*
 * Structures
 */
typedef struct TELEPORT{
		uint16	Status;
		uint16	Group;
		uint16	Type;					// what stuff can teleport
		uint16	zone_type;				// sphere / box / convex hull
		VECTOR	Pos;
		VECTOR	Dir;
		VECTOR	Up;
		VECTOR	half_size;
		TRIGGER_ZONE	*Zone;
		uint16	num_sides;

		uint16	num_links;
		uint16	Links[MAXTELEPORTLINKS];
struct	TELEPORT * NextInGroup;
}TELEPORT;

/*
 * fn prototypes
 */
void ReleaseTeleports( void );
BOOL TeleportsLoad( char * Filename );
void TeleportsZoneCheck( VECTOR * OldPos , VECTOR * NewPos , TELEPORT * Tport );
BOOL TeleportsAreaCheck( VECTOR * OldPos , VECTOR * NewPos ,uint16 Group, OBJECT *obj );
void StartTeleport( uint16 * Data );
void StopTeleport( uint16 * Data );
void DisplayTeleportsInGroup( uint16 Group );
FILE * SaveTeleports( FILE * fp );
FILE * LoadTeleports( FILE * fp );

#endif	// TELEPORT_INCLUDED

