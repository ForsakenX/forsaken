
/*
Trigger zone format (*.zon):

num_zones : u_int16_t
{
	group : u_int16_t
	type : u_int16_t // 0 = ZONE_Sphere, 1 = ZONE_Box, 2 = ZONE_Polygonal
	pos( x, y, z ) : all float
	half_size( x, y, z ) : all float
	if type != ZONE_Sphere
	{
		num_sides : u_int16_t
		{
			normal( x, y, z ) : all float
			offset : float
		}[ num_sides ]
	}
	when_player_isin : u_int16_t // index into action table
	when_player_enters : u_int16_t // index into action table
	when_player_exits : u_int16_t // index into action table
	when_player_shoots : u_int16_t // index into action table
	when_enemy_isin : u_int16_t // index into action table
	when_enemy_enters : u_int16_t // index into action table
	when_enemy_exits : u_int16_t // index into action table
	when_enemy_shoots : u_int16_t // index into action table
}[ num_zones ]
*/
 
 
/*===================================================================
		Include Files...	
===================================================================*/

#include <stdio.h>
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "main.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "file.h"
#include "triggers.h"
#include "trigarea.h"
#include "pickups.h"
#include "mload.h"
#include "bsp.h"
#include "enemies.h"
#include "skin.h"

#include "primary.h"
#include "secondary.h"
#include "util.h"


/*===================================================================
		Externals ...
===================================================================*/
extern	bool			ShowTrigZones;
extern	BYTE			WhoIAm;
bool ClassifyPointInHull( VECTOR * Pos, TRIGGER_ZONE * Sides, int16_t NumSides, int16_t Side );
bool RayToHull( TRIGGER_ZONE * StartSide , VECTOR * StartPos , VECTOR * EndPos , u_int16_t StartNumSides );
extern TRIGGERMOD	*	TrigMods;
void TriggerAreaProcess( VECTOR * OldPos , VECTOR * NewPos , TRIGGER_AREA * Area );
extern	float framelag;
/*===================================================================
		Globals ...
===================================================================*/
u_int16_t	NumZones;
TRIGGER_AREA * Zones = NULL;

TRIGGER_AREA * GroupTriggerArea_player[MAXGROUPS];
TRIGGER_AREA * GroupTriggerArea_player_shoots[MAXGROUPS];
TRIGGER_AREA * GroupTriggerArea_enemy[MAXGROUPS];
TRIGGER_AREA * GroupTriggerArea_enemy_shoots[MAXGROUPS];

/*===================================================================
		Defines
===================================================================*/
#define	ZON_VERSION_NUMBER	2

/*===================================================================
	Procedure	:		Load .zon File
	Input		:		char	*	Filename
	Output		:		bool	true/false
===================================================================*/
bool TriggerAreaload( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int16_t			*	int16_tpnt;
	u_int16_t			*	u_int16_tpnt;
	u_int32_t			*	u_int32_tpnt;
	int			i, j;
	TRIGGER_AREA * OldAreaPnt;
	TRIGGER_AREA * AreaPnt;
	TRIGGER_ZONE * ZonePnt;
	float * floatpnt;
	u_int32_t	MagicNumber;
	u_int32_t	VersionNumber;

	if( Zones != NULL )
	{
		free(Zones);
		Zones = NULL;
	}
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		GroupTriggerArea_player[i] = NULL;      
		GroupTriggerArea_player_shoots[i] = NULL;
		GroupTriggerArea_enemy[i] = NULL;       
		GroupTriggerArea_enemy_shoots[i] = NULL;		
	}
	

	File_Size = Get_File_Size( Filename );	

	if( !File_Size ) return true;

	Buffer = malloc( File_Size );
	OrgBuffer = Buffer;

	if( Buffer == NULL ) return false;

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size ) return false;

	u_int32_tpnt = (u_int32_t *) Buffer;
	MagicNumber = *u_int32_tpnt++;
	VersionNumber = *u_int32_tpnt++;
	Buffer = (char *) u_int32_tpnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != ZON_VERSION_NUMBER  ) )
	{
		Msg( "TriggerAreaLoad() Incompatible triggerzone( .ZON ) file %s", Filename );
		return( false );
	}

	u_int16_tpnt = (u_int16_t *) Buffer;
	NumZones = *u_int16_tpnt++;
	Buffer = (char*) u_int16_tpnt;
	Zones = (TRIGGER_AREA*) calloc( NumZones , sizeof(TRIGGER_AREA) );	
	AreaPnt = Zones;

	for( i = 0 ; i < NumZones ; i++ )
	{
		u_int16_tpnt = (u_int16_t *) Buffer;
		AreaPnt->group = *u_int16_tpnt++;
		AreaPnt->generation_type = *u_int16_tpnt++;
		floatpnt = (float * ) u_int16_tpnt;
		AreaPnt->generation_delay = *floatpnt++;
		u_int16_tpnt = (u_int16_t *) floatpnt;

		if( AreaPnt->generation_type != TRIGGER_AREA_GENTYPE_Initialised )
		{
			AreaPnt->state = TRIGGER_AREA_STATE_Off;
		}else{
			AreaPnt->state = TRIGGER_AREA_STATE_On;
		}

		AreaPnt->type = *u_int16_tpnt++;

		floatpnt = (float * ) u_int16_tpnt;

		AreaPnt->pos.x = *floatpnt++;
		AreaPnt->pos.y = *floatpnt++;
		AreaPnt->pos.z = *floatpnt++;
		AreaPnt->half_size.x = *floatpnt++;
		AreaPnt->half_size.y = *floatpnt++;
		AreaPnt->half_size.z = *floatpnt++;

		Buffer = (char*) floatpnt;
		if( AreaPnt->type != ZONE_Sphere )
		{
			// convex hull...
			u_int16_tpnt = (u_int16_t*) Buffer;
			AreaPnt->num_sides = *u_int16_tpnt++;
			Buffer = (char*) u_int16_tpnt;
			AreaPnt->Zone = (TRIGGER_ZONE*) malloc( AreaPnt->num_sides * sizeof( TRIGGER_ZONE ) );
			ZonePnt = AreaPnt->Zone;

			floatpnt = (float * ) Buffer;
			
			for( j = 0 ; j < AreaPnt->num_sides ; j++ )
			{
				ZonePnt->normal.x = *floatpnt++;
				ZonePnt->normal.y = *floatpnt++;
				ZonePnt->normal.z = *floatpnt++;
				ZonePnt->offset   = *floatpnt++;
				ZonePnt++;
			}
			Buffer = (char*) floatpnt;
		}
		u_int16_tpnt = (u_int16_t *) Buffer;

		AreaPnt->when_player_isin = *u_int16_tpnt++;
		AreaPnt->when_player_enters = *u_int16_tpnt++;
		AreaPnt->when_player_exits = *u_int16_tpnt++;
		AreaPnt->when_player_shoots = *u_int16_tpnt++;
		int16_tpnt = (int16_t *) u_int16_tpnt;
		AreaPnt->player_primary = *int16_tpnt++;
		AreaPnt->player_secondary = *int16_tpnt++;
		u_int16_tpnt = (u_int16_t *) int16_tpnt;
		AreaPnt->when_enemy_isin = *u_int16_tpnt++;
		AreaPnt->when_enemy_enters = *u_int16_tpnt++;
		AreaPnt->when_enemy_exits = *u_int16_tpnt++;
		AreaPnt->when_enemy_shoots = *u_int16_tpnt++;
		int16_tpnt = (int16_t *) u_int16_tpnt;
		AreaPnt->enemy_primary = *int16_tpnt++;
		AreaPnt->enemy_secondary = *int16_tpnt++;
		Buffer = (char*) int16_tpnt;

		AreaPnt++;
	}
	
	free( OrgBuffer );

	// Make up Group Link List....

	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		AreaPnt = Zones;
		OldAreaPnt = NULL;
		for( j = 0 ; j < NumZones ; j++ )
		{
			if( ( AreaPnt->group == i ) &&( ( AreaPnt->when_player_isin != 0xffff ) ||
											( AreaPnt->when_player_enters != 0xffff ) ||
											( AreaPnt->when_player_exits != 0xffff ) ) )
			{
				if( !OldAreaPnt )
				{
					// found the first....
					GroupTriggerArea_player[i] = AreaPnt;
					OldAreaPnt = AreaPnt;
				}else{
					// found another
					OldAreaPnt->NextSameGroup_player = AreaPnt;
					OldAreaPnt = AreaPnt;
				}
			}
			AreaPnt++;
		}
	}
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		AreaPnt = Zones;
		OldAreaPnt = NULL;
		for( j = 0 ; j < NumZones ; j++ )
		{
			if( ( AreaPnt->group == i ) &&( AreaPnt->when_player_shoots != 0xffff ) )
			{
				if( !OldAreaPnt )
				{
					// found the first....
					GroupTriggerArea_player_shoots[i] = AreaPnt;
					OldAreaPnt = AreaPnt;
				}else{
					// found another
					OldAreaPnt->NextSameGroup_player_shoots = AreaPnt;
					OldAreaPnt = AreaPnt;
				}
			}
			AreaPnt++;
		}
	}
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		AreaPnt = Zones;
		OldAreaPnt = NULL;
		for( j = 0 ; j < NumZones ; j++ )
		{
			if( ( AreaPnt->group == i ) &&( ( AreaPnt->when_enemy_isin != 0xffff ) ||
											( AreaPnt->when_enemy_enters != 0xffff ) ||
											( AreaPnt->when_enemy_exits != 0xffff ) ) )
			{
				if( !OldAreaPnt )
				{
					// found the first....
					GroupTriggerArea_enemy[i] = AreaPnt;
					OldAreaPnt = AreaPnt;
				}else{
					// found another
					OldAreaPnt->NextSameGroup_enemy = AreaPnt;
					OldAreaPnt = AreaPnt;
				}
			}
			AreaPnt++;
		}
	}
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		AreaPnt = Zones;
		OldAreaPnt = NULL;
		for( j = 0 ; j < NumZones ; j++ )
		{
			if( ( AreaPnt->group == i ) &&( AreaPnt->when_enemy_shoots != 0xffff ) )
			{
				if( !OldAreaPnt )
				{
					// found the first....
					GroupTriggerArea_enemy_shoots[i] = AreaPnt;
					OldAreaPnt = AreaPnt;
				}else{
					// found another
					OldAreaPnt->NextSameGroup_enemy_shoots = AreaPnt;
					OldAreaPnt = AreaPnt;
				}
			}
			AreaPnt++;
		}
	}
	return true;
}

/*===================================================================
 	Procedure	:		Free any memory taken up with zone loading..
	Input		:		void
	Output		:		void
===================================================================*/
void ReleaseTriggerArea( void )
{
	int			i;
	TRIGGER_AREA * AreaPnt;

	if( Zones )
	{
		AreaPnt = Zones;

		for( i = 0 ; i < NumZones ; i++ )
		{
			if( AreaPnt->Zone )
			{
				free( AreaPnt->Zone );
			}
			AreaPnt++;
		}
		free(Zones);
		Zones = NULL;
	}
}


bool	Entry;
bool	Exit;
bool	In;
/*===================================================================
 	Procedure	:		Check Player...
	Input		:		void
	Output		:		void
===================================================================*/
bool	TriggerAreaPlayerCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group )
{
	TRIGGER_AREA * Area;

	Area = GroupTriggerArea_player[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			TriggerAreaProcess( OldPos , NewPos , Area );
			
			if( Entry )
			{
				if( Area->when_player_enters != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_player_enters] );
			}
			if( In )
			{
				if( Area->when_player_isin != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_player_isin] );
			}
			if( Exit )
			{
				if( Area->when_player_exits != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_player_exits] );
			}
		}

		Area = Area->NextSameGroup_player;
	}
	return true;
}

/*===================================================================
 	Procedure	:		Check Player's shot hits trigger area...
	Input		:		VECTOR	*	Old Position 
				:		VECTOR	*	New Position 
				:		u_int16_t		Group
				:		u_int8_t		Weapon Type
				:		u_int8_t		Weapon
	Output		:		bool		true
===================================================================*/
bool	TriggerAreaPlayerShootsCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group, u_int8_t WeaponType, u_int8_t Weapon )
{
	TRIGGER_AREA * Area;
	bool	CanActivate;

	Area = GroupTriggerArea_player_shoots[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			CanActivate = false;
			
			if( ( ( Area->player_primary != -1 )|| ( Area->player_secondary != -1 ) ) &&
				( ( Area->player_primary != 0 ) || ( Area->player_secondary != 0 ) ) )
			{
				switch( WeaponType )
				{
				case WEPTYPE_Primary:
					if( Weapon == Area->player_primary )
					{
						CanActivate = true;
					}
					break;
					
				case WEPTYPE_Secondary:
					if( Weapon == Area->player_secondary )
					{
						CanActivate = true;
					}
					break;
				}
			}
			else
			{
				CanActivate = true;
			}
			if( CanActivate )
			{
				TriggerAreaProcess( OldPos , NewPos , Area );
				
//				if( Entry || In || Exit )
				if( Entry )			/* Entry only, trigger will not activate if bullet starts in zone */
				{
					if( Area->when_player_shoots != 0xffff )
						ApplyTriggerMod( &TrigMods[Area->when_player_shoots] );
				}
			}
		}

		Area = Area->NextSameGroup_player_shoots;
	}
	return true;
}
/*===================================================================
 	Procedure	:		Check Enemy...
	Input		:		void
	Output		:		void
===================================================================*/
bool	TriggerAreaEnemyCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group )
{
	TRIGGER_AREA * Area;

	Area = GroupTriggerArea_enemy[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			TriggerAreaProcess( OldPos , NewPos , Area );
			
			if( Entry )
			{
				if( Area->when_enemy_enters != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_enemy_enters] );
			}
			if( In )
			{
				if( Area->when_enemy_isin != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_enemy_isin] );
			}
			if( Exit )
			{
				if( Area->when_enemy_exits != 0xffff )
					ApplyTriggerMod( &TrigMods[Area->when_enemy_exits] );
			}
		}

		Area = Area->NextSameGroup_enemy;
	}
	return true;
}

/*===================================================================
 	Procedure	:		Check Enemy's shot hits trigger area...
	Input		:		VECTOR	*	Old Position 
				:		VECTOR	*	New Position 
				:		u_int16_t		Group
				:		u_int8_t		Weapon Type
				:		u_int8_t		Weapon
	Output		:		bool		true
===================================================================*/
bool	TriggerAreaEnemyShootsCheck( VECTOR * OldPos , VECTOR * NewPos , u_int16_t Group, u_int8_t WeaponType, u_int8_t Weapon )
{
	TRIGGER_AREA * Area;
	bool	CanActivate;

	Area = GroupTriggerArea_enemy_shoots[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			CanActivate = false;
			
			if( ( ( Area->enemy_primary != -1 )|| ( Area->enemy_secondary != -1 ) ) &&
				( ( Area->enemy_primary != 0 ) || ( Area->enemy_secondary != 0 ) ) )
			{
				switch( WeaponType )
				{
				case WEPTYPE_Primary:
					if( Weapon == Area->enemy_primary )
					{
						CanActivate = true;
					}
					break;
					
				case WEPTYPE_Secondary:
					if( Weapon == Area->enemy_secondary )
					{
						CanActivate = true;
					}
					break;
				}
			}
			else
			{
				CanActivate = true;
			}
			if( CanActivate )
			{
				TriggerAreaProcess( OldPos , NewPos , Area );
				
//				if( Entry || In || Exit )
				if( Entry )			/* Entry only, trigger will not activate if bullet starts in zone */
				{
					if( Area->when_enemy_shoots != 0xffff )
						ApplyTriggerMod( &TrigMods[Area->when_enemy_shoots] );
				}
			}
		}

		Area = Area->NextSameGroup_enemy_shoots;
	}
	return true;
}


/*===================================================================
 	Procedure	:		Check Ray to Col Zone
	Input		:		VECTOR	*	StartPos
				:		VECTOR	*	EndPos
				:		ZONESIDE *	Sides
				:		int16_t		NumSides
				:		VECTOR	*	Intersection Point ( TBFI )
				:		ZONESIDE **	Intersection Side ( TBFI )
				:		float		Collision Radius
	Output		:		void
===================================================================*/
bool RayToHull( TRIGGER_ZONE * StartSide , VECTOR * StartPos , VECTOR * EndPos , u_int16_t StartNumSides )
{
	float		d1, d2;
	float		DistToPlane;
	TRIGGER_ZONE *	Sides;
	int16_t		NumSides;
	VECTOR		TempInt;
	int16_t		InsideCount = 0;

	Sides = StartSide;
	NumSides = StartNumSides;

	while( NumSides-- )
	{
		d1 = DotProduct( StartPos, &Sides->normal ) + Sides->offset;
		d2 = DotProduct( EndPos, &Sides->normal ) + Sides->offset;

		if( ( d1 >= 0.0F ) && ( d2 >= 0.0F ) ) return( false );

		if( ( ( d1 > 0.0F ) && ( d2 < 0.0F ) ) || ( ( d1 < 0.0F ) && ( d2 > 0.0F ) ) )
		{
			DistToPlane = ( d1 / ( d1 - d2 ) );
			TempInt.x = StartPos->x + ( DistToPlane * ( EndPos->x - StartPos->x ) );
			TempInt.y = StartPos->y + ( DistToPlane * ( EndPos->y - StartPos->y ) );
			TempInt.z = StartPos->z + ( DistToPlane * ( EndPos->z - StartPos->z ) );

			if( ClassifyPointInHull( &TempInt, StartSide, StartNumSides, NumSides ) )
			{
				if( ( d1 < 0.0F ) && ( d2 > 0.0F ) )
				{
					Exit = true;
//					return( true );
				}

				if( ( d1 > 0.0F ) && ( d2 < 0.0F ) )
				{
					Entry = true;
//					return( true );
				}
			}
		}
		else
		{
			InsideCount++;
		}

		Sides++;
	}

	if( InsideCount == StartNumSides )
	{
		In = true;
//		return( true );
	}

	if( Entry || Exit || In ) return( true );

	return( false );
}

/*===================================================================
 	Procedure	:		Is point inside convex shape?
	Input		:		VECTOR	*	Pos
				:		ZONESIDE *	Sides
				:		int16_t		NumSides
	Output		:		bool		true/false
===================================================================*/
bool ClassifyPointInHull( VECTOR * Pos, TRIGGER_ZONE * Sides, int16_t NumSides, int16_t Side )
{
	float		d1;

	while( NumSides-- )
	{
		if( NumSides != Side )
		{
			d1 = DotProduct( Pos, &Sides->normal ) + Sides->offset;
			if( d1 > 0.0F ) return false;
		}
		Sides++;
	}

	return true;
}

/*===================================================================
 	Procedure	:		Process and clasify 2 positions with an area..
	Input		:		void
	Output		:		void
===================================================================*/

void TriggerAreaProcess( VECTOR * OldPos , VECTOR * NewPos , TRIGGER_AREA * Area )
{
	bool	OldIn;
	bool	NewIn;

	Entry = false;
	Exit = false;
	In = false;
	if( Area->type == ZONE_Sphere )
	{
   		OldIn = DistanceVector2Vector( &Area->pos , OldPos ) < Area->half_size.x;
   		NewIn = DistanceVector2Vector( &Area->pos , NewPos ) < Area->half_size.x;

   		if( !OldIn && NewIn )
   			Entry = true;
   		if( OldIn && !NewIn )
   			Exit = true;
   		if( OldIn && NewIn )
   		{
   			In = true;
   		}
   	}else{
   		RayToHull( Area->Zone , OldPos , NewPos , Area->num_sides );
   	}
}

/*
		If Both on positive side .....Miss...

		If Both on negative go on to next....

		different sides...calc intersection point...if on negative of all remaining then
													if started on positive then an entry..
													otherwise an exit...
													if started on positive then carryon with intersection and endpoint

*/

/*===================================================================
 	Procedure	:		Display all trigger areas in group
	Input		:		u_int16_t	Group
	Output		:		Nothing
===================================================================*/
void DispGroupTriggerAreas( u_int16_t Group )
{
	TRIGGER_AREA * Area;

	if( !ShowTrigZones ) return;

	Area = GroupTriggerArea_player[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 0, 64, 0, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 0, 64, 0 ,128 );
			}
		}
		else
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 64, 0, 64, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 64, 0, 64 ,128 );
			}
		}
		Area = Area->NextSameGroup_player;
	}

	Area = GroupTriggerArea_player_shoots[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 0, 128, 0, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 0, 128, 0 ,128 );
			}
		}
		else
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 64, 0, 64, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 64, 0, 64 ,128 );
			}
		}
		Area = Area->NextSameGroup_player;
	}

	Area = GroupTriggerArea_enemy[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 0, 64, 0, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 0, 64, 0, 128 );
			}
		}
		else
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 64, 0, 64, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 64, 0, 64 ,128 );
			}
		}
		Area = Area->NextSameGroup_player;
	}

	Area = GroupTriggerArea_enemy_shoots[Group];
	while( Area )
	{
		if( Area->state == TRIGGER_AREA_STATE_On )
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 128, 0, 0, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 128, 0, 0 ,128 );
			}
		}
		else
		{
			if( Area->type == ZONE_Sphere )
			{
				DisplaySphereZone( &Area->pos, Area->half_size.x, Group, 64, 0, 64, 128 );
			}
			else
			{
				DisplayTriggerZone( Area->Zone, Area->num_sides, Group, 64, 0, 64 ,128 );
			}
		}
		Area = Area->NextSameGroup_player;
	}
}



/*===================================================================
 	Procedure	:		Enable trigger area
	Input		:		u_int16_t	* Data
	Output		:		Nothing
===================================================================*/
void EnableTriggerArea( u_int16_t * Data )
{
	TRIGGER_AREA * AreaPnt;
	AreaPnt = Zones;
	if( !AreaPnt )
		return;
	AreaPnt += *Data;
	AreaPnt->state = TRIGGER_AREA_STATE_On;
}


/*===================================================================
 	Procedure	:		Disable trigger area
	Input		:		u_int16_t	* Data
	Output		:		Nothing
===================================================================*/
void DisableTriggerArea( u_int16_t * Data )
{
	TRIGGER_AREA * AreaPnt;
	AreaPnt = Zones;
	if( !AreaPnt )
		return;
	AreaPnt += *Data;
	AreaPnt->state = TRIGGER_AREA_STATE_Off;
}
/*===================================================================
 	Procedure	:		Check Enemy...
	Input		:		void
	Output		:		void
===================================================================*/
void ProcessTriggerAreas( void )
{
	TRIGGER_AREA * Area;
	int i;
	Area = Zones;
	if( !Area )
		return;

	for( i = 0 ; i < NumZones ; i++ )
	{
		if( ( Area->state == TRIGGER_AREA_STATE_Off ) && ( Area->generation_type == TRIGGER_AREA_GENTYPE_Time ) && ( Area->generation_delay > 0.0F ) )
		{
			Area->generation_delay -= framelag;
			if( Area->generation_delay <= 0.0F )
			{
				Area->state = TRIGGER_AREA_STATE_On;
			}
		}
		Area++;
	}
}

/*===================================================================
	Procedure	:	Save TriggerAreas arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveTriggerAreas( FILE * fp )
{
	int				i;
	TRIGGER_AREA *	TrigAreaPtr;

	if( fp )
	{
		fwrite( &NumZones, sizeof( u_int16_t ), 1, fp );

		TrigAreaPtr = Zones;

		for( i = 0; i < NumZones; i++ )
		{
			fwrite( &TrigAreaPtr->state, sizeof( u_int16_t ), 1, fp );
			TrigAreaPtr++;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load TriggerAreas Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadTriggerAreas( FILE * fp )
{
	int				i;
	TRIGGER_AREA *	TrigAreaPtr;
	u_int16_t			NumTrigAreas;

	if( fp )
	{
		fread( &NumTrigAreas, sizeof( u_int16_t ), 1, fp );

		if( NumTrigAreas != NumZones )
		{
			fclose( fp );
			return( NULL );
		}

		TrigAreaPtr = Zones;

		for( i = 0; i < NumZones; i++ )
		{
			fread( &TrigAreaPtr->state, sizeof( u_int16_t ), 1, fp );
			TrigAreaPtr++;
		}
	}

	return( fp );
}

