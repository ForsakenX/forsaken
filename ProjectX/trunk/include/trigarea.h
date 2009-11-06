
#ifndef TRIGGERAREA_INCLUDED
#define TRIGGERAREA_INCLUDED

/*
 * Defines
 */
#define TRIGGER_AREA_STATE_Off 0
#define TRIGGER_AREA_STATE_On 1

#define ZONE_Sphere			0
#define ZONE_Box			1
#define ZONE_Polygonal		2

enum {

	TRIGGER_AREA_GENTYPE_Initialised,		// Initialised immediatly
	TRIGGER_AREA_GENTYPE_Time,			// Initialised after time
	TRIGGER_AREA_GENTYPE_Trigger,			// Initialised by trigger
};

/*
 * Structures
 */
typedef struct TRIGGER_ZONE
{
	VECTOR	normal;
	float	offset;
}TRIGGER_ZONE;

typedef struct TRIGGER_AREA
{
	uint16	state;
	uint16	group;
	uint16	generation_type;
	float	generation_delay;
	uint16	type;
	VECTOR	pos;
	VECTOR	half_size;
	TRIGGER_ZONE	*Zone;
	uint16	num_sides;
	uint16	when_player_isin;
	uint16	when_player_enters;
	uint16	when_player_exits;
	uint16	when_player_shoots;
	uint16	when_enemy_isin;
	uint16	when_enemy_enters;
	uint16	when_enemy_exits;
	uint16	when_enemy_shoots;
	int16	player_primary;
	int16	player_secondary;
	int16	enemy_primary;
	int16	enemy_secondary;
struct	TRIGGER_AREA * NextSameGroup_player;
struct	TRIGGER_AREA * NextSameGroup_player_shoots;
struct	TRIGGER_AREA * NextSameGroup_enemy;
struct	TRIGGER_AREA * NextSameGroup_enemy_shoots;
}TRIGGER_AREA;

/*
 * fn prototypes
 */

BOOL TriggerAreaload( char * Filename );
void ReleaseTriggerArea( void );
BOOL TriggerAreaPlayerCheck( VECTOR * OldPos , VECTOR * NewPos , uint16 Group );
BOOL TriggerAreaPlayerShootsCheck( VECTOR * OldPos , VECTOR * NewPos , uint16 Group, uint8 WeaponType, uint8 Weapon );
BOOL TriggerAreaEnemyCheck( VECTOR * OldPos , VECTOR * NewPos , uint16 Group );
BOOL TriggerAreaEnemyShootsCheck( VECTOR * OldPos , VECTOR * NewPos , uint16 Group, uint8 WeaponType, uint8 Weapon );
void DispGroupTriggerAreas( uint16 Group );
void EnableTriggerArea( uint16 * Data );
void DisableTriggerArea( uint16 * Data );
void ProcessTriggerAreas( void );
FILE * SaveTriggerAreas( FILE * fp );
FILE * LoadTriggerAreas( FILE * fp );
#endif	// TRIGGERAREA_INCLUDED


