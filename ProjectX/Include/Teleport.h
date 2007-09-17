
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

