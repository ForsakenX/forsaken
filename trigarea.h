
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
	u_int16_t	state;
	u_int16_t	group;
	u_int16_t	generation_type;
	float	generation_delay;
	u_int16_t	type;
	VECTOR	pos;
	VECTOR	half_size;
	TRIGGER_ZONE	*Zone;
	u_int16_t	num_sides;
	u_int16_t	when_player_isin;
	u_int16_t	when_player_enters;
	u_int16_t	when_player_exits;
	u_int16_t	when_player_shoots;
	u_int16_t	when_enemy_isin;
	u_int16_t	when_enemy_enters;
	u_int16_t	when_enemy_exits;
	u_int16_t	when_enemy_shoots;
	int16_t	player_primary;
	int16_t	player_secondary;
	int16_t	enemy_primary;
	int16_t	enemy_secondary;
struct	TRIGGER_AREA * NextSameGroup_player;
struct	TRIGGER_AREA * NextSameGroup_player_shoots;
struct	TRIGGER_AREA * NextSameGroup_enemy;
struct	TRIGGER_AREA * NextSameGroup_enemy_shoots;
}TRIGGER_AREA;

/*
 * fn prototypes
 */

bool TriggerAreaload( char * Filename );
void ReleaseTriggerArea( void );
bool TriggerAreaPlayerCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group );
bool TriggerAreaPlayerShootsCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group, u_int8_t WeaponType, u_int8_t Weapon );
bool TriggerAreaEnemyCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group );
bool TriggerAreaEnemyShootsCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group, u_int8_t WeaponType, u_int8_t Weapon );
void DispGroupTriggerAreas( u_int16_t Group );
void EnableTriggerArea( u_int16_t * Data );
void DisableTriggerArea( u_int16_t * Data );
void ProcessTriggerAreas( void );
FILE * SaveTriggerAreas( FILE * fp );
FILE * LoadTriggerAreas( FILE * fp );
#endif	// TRIGGERAREA_INCLUDED


