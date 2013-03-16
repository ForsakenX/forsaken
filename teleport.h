
#ifndef TELEPORT_INCLUDED
#define TELEPORT_INCLUDED

#include "main.h"
#include "new3d.h"
#include "triggers.h"
#include "trigarea.h"

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
		u_int16_t	Status;
		u_int16_t	Group;
		u_int16_t	Type;					// what stuff can teleport
		u_int16_t	zone_type;				// sphere / box / convex hull
		VECTOR	Pos;
		VECTOR	Dir;
		VECTOR	Up;
		VECTOR	half_size;
		TRIGGER_ZONE	*Zone;
		u_int16_t	num_sides;

		u_int16_t	num_links;
		u_int16_t	Links[MAXTELEPORTLINKS];
struct	TELEPORT * NextInGroup;
}TELEPORT;

/*
 * fn prototypes
 */
void ReleaseTeleports( void );
bool TeleportsLoad( char * Filename );
void TeleportsZoneCheck( VECTOR * OldPos , VECTOR * NewPos , TELEPORT * Tport );
bool TeleportsAreaCheck( VECTOR * OldPos , VECTOR * NewPos ,u_int16_t Group, OBJECT *obj );
void StartTeleport( u_int16_t * Data );
void StopTeleport( u_int16_t * Data );
void DisplayTeleportsInGroup( u_int16_t Group );
FILE * SaveTeleports( FILE * fp );
FILE * LoadTeleports( FILE * fp );

#endif	// TELEPORT_INCLUDED

