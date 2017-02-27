
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
#include "2dpolys.h"
#include "goal.h"
#include "title.h"
#include "visi.h"
#include "text.h"
#include "util.h"

#define GOAL_VERSION_NUMBER		(1)


// externs
extern	bool			PlayDemo;
extern	bool			DebugInfo;
extern	bool			IsHost;
extern	bool			CaptureTheFlag;
extern	bool			CTF;
extern	char			LevelNames[MAXLEVELS][128];
extern	bool			Entry;
extern	bool			Exit;
extern	bool			In;
extern	char		*	TeamName[ MAX_TEAMS ];
extern	u_int32_t			TeamFlagMask[ MAX_TEAMS ];
extern	int8_t			TeamFlagPickup[ MAX_TEAMS ];
extern	bool			TeamFlagAtHome[ MAX_TEAMS ];
extern	int16_t			NumPickupType[ MAXPICKUPTYPES ];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	FRAME_INFO	*	GreyFlare_Header;
extern	float			framelag;
extern	u_int8_t			Colourtrans[MAXFONTCOLOURS][3];
extern	int				TeamCol[ MAX_TEAMS ];
extern	BYTE			TeamNumber[MAX_PLAYERS];
extern	BYTE			GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
extern	u_int16_t			RandomStartPosModify;
extern	int16_t			PickupsGot[ MAXPICKUPTYPES ];
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	u_int32_t			Host_Flags[ MAX_PLAYERS ];

bool ClassifyPointInHull( VECTOR * Pos, TRIGGER_ZONE * Sides, int16_t NumSides, int16_t Side );
bool RayToHull( TRIGGER_ZONE * StartSide , VECTOR * StartPos , VECTOR * EndPos , u_int16_t StartNumSides );
void ShowGoal( GOAL * Goal, u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans );
void TeamGoalsSend( u_int16_t * TeamGoals );


// globals
int NumGoals = 0;
GOAL *Goal = NULL;
int TeamFlag[ MAX_TEAMS ];
int GoalTeamMembers[ MAX_TEAMS ];
int LastTeamMembers[ MAX_TEAMS ];
int16_t Teamgoal[ MAX_TEAMS ];
int LastEnable[ MAX_PLAYERS ];


extern int16_t   LevelNum;
bool GoalLoad( void )
{
	int8_t		Filename[ 256 ];
	char	*	NewExt = ".GOL";
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	u_int16_t			*	u_int16_tpnt;
	u_int32_t			*	u_int32_tpnt;
	int			i, j;
	GOAL * GoalPnt;
	TRIGGER_ZONE * ZonePnt;
	float * floatpnt;
	u_int32_t	MagicNumber;
	u_int32_t	VersionNumber;

	if ( !CaptureTheFlag && !CTF )
		return true;
	
	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &Filename[ 0 ], NewExt );

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

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != GOAL_VERSION_NUMBER  ) )
	{
		Msg( "GoalLoad() Incompatible triggerzone( .ZON ) file %s", Filename );
		return( false );
	}

	u_int16_tpnt = (u_int16_t *) Buffer;
	NumGoals = *u_int16_tpnt++;
	Buffer = (char*) u_int16_tpnt;
	Goal = (GOAL *) calloc( NumGoals, sizeof( GOAL ) );	
	if ( !Goal )
	{
		NumGoals = 0;
		return false;
	}
	GoalPnt = Goal;

	for( i = 0 ; i < NumGoals ; i++ )
	{
		u_int16_tpnt = (u_int16_t *) Buffer;
		GoalPnt->group = *u_int16_tpnt++;
		GoalPnt->type = *u_int16_tpnt++;
		GoalPnt->team = -1;
		GoalPnt->state = GOALSTATE_Off;

		floatpnt = (float * ) u_int16_tpnt;
#ifdef ARM
		memcpy(&GoalPnt->pos, floatpnt, 4*3);
		floatpnt+=3;
		memcpy(&GoalPnt->half_size, floatpnt, 4*3);
		floatpnt+=3;
		memcpy(&GoalPnt->dir, floatpnt, 4*3);
		floatpnt+=3;
		memcpy(&GoalPnt->up, floatpnt, 4*3);
		floatpnt+=3;
		memcpy(&GoalPnt->width, floatpnt++, 4);
		memcpy(&GoalPnt->height, floatpnt++, 4);
		memcpy(&GoalPnt->depth, floatpnt++, 4);
#else
		GoalPnt->pos.x = *floatpnt++;
		GoalPnt->pos.y = *floatpnt++;
		GoalPnt->pos.z = *floatpnt++;
		GoalPnt->half_size.x = *floatpnt++;
		GoalPnt->half_size.y = *floatpnt++;
		GoalPnt->half_size.z = *floatpnt++;
		GoalPnt->dir.x = *floatpnt++;
		GoalPnt->dir.y = *floatpnt++;
		GoalPnt->dir.z = *floatpnt++;
		GoalPnt->up.x = *floatpnt++;
		GoalPnt->up.y = *floatpnt++;
		GoalPnt->up.z = *floatpnt++;
		GoalPnt->width = *floatpnt++;
		GoalPnt->height = *floatpnt++;
		GoalPnt->depth = *floatpnt++;
#endif
		Buffer = (char*) floatpnt;
		if( GoalPnt->type != ZONE_Sphere )
		{
			// convex hull...
			u_int16_tpnt = (u_int16_t*) Buffer;
			GoalPnt->num_sides = *u_int16_tpnt++;
			Buffer = (char*) u_int16_tpnt;
			if ( GoalPnt->num_sides )
				GoalPnt->Zone = (TRIGGER_ZONE*) malloc( GoalPnt->num_sides * sizeof( TRIGGER_ZONE ) );
			else
				GoalPnt->Zone = NULL;
			ZonePnt = GoalPnt->Zone;
			if ( !ZonePnt )
			{
				NumGoals = 0;
				free( Goal );
				Goal = NULL;
				return false;
			}

			floatpnt = (float * ) Buffer;
			
			for( j = 0 ; j < GoalPnt->num_sides ; j++ )
			{
#ifdef ARM
				memcpy(&ZonePnt->normal, floatpnt, 4*3);
				floatpnt+=3;
				memcpy(&ZonePnt->offset, floatpnt++, 4);
#else
				ZonePnt->normal.x = *floatpnt++;
				ZonePnt->normal.y = *floatpnt++;
				ZonePnt->normal.z = *floatpnt++;
				ZonePnt->offset   = *floatpnt++;
#endif
				ZonePnt++;
			}
			Buffer = (char*) floatpnt;
		}
		GoalPnt++;
	}
	
	free( OrgBuffer );

	for ( j = 0; j < MAX_TEAMS; j++ )
	{
		GoalTeamMembers[ j ] = 0;
		Teamgoal[ j ] = -1;
	}

	for ( j = 0; j < MAX_PLAYERS; j++ )
	{
		LastEnable[ j ] = 0;
	}

	return true;
}


void ReleaseGoal( void )
{
	int j;

	if ( !Goal )
		return;

	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].Zone )
		{
			free( Goal[ j ].Zone );
			Goal[ j ].Zone = NULL;
			Goal[ j ].num_sides = 0;
		}
	}
	free( Goal );
	Goal = NULL;
	NumGoals = 0;
}


static void GoalCheck( VECTOR * OldPos , VECTOR * NewPos , GOAL * goal )
{
	bool	OldIn;
	bool	NewIn;

	Entry = false;
	Exit = false;
	In = false;
	if( goal->type == ZONE_Sphere )
	{
   		OldIn = DistanceVector2Vector( &goal->pos , OldPos ) < goal->half_size.x;
   		NewIn = DistanceVector2Vector( &goal->pos , NewPos ) < goal->half_size.x;

   		if( !OldIn && NewIn )
   			Entry = true;
   		if( OldIn && !NewIn )
   			Exit = true;
   		if( OldIn && NewIn )
   		{
   			In = true;
   		}
   	}else{
   		RayToHull( goal->Zone , OldPos , NewPos , goal->num_sides );
   	}
}



GOAL *TeamGoal( int team )
{
	int j;

	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].team == team )
			return &Goal[ j ];
	}

	return NULL;
}


bool GenerateFlagAtHome( int team )
{
	int j;

	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].team == team )
		{
			GOAL *goal;
			u_int16_t i;
			VECTOR Dir = { 0.0F, 0.0F, 0.0F };
			
			goal = &Goal[ j ];
			i = InitOnePickup( &goal->pos, goal->group,
				&Dir, 0.0F, TeamFlagPickup[ team ], WhoIAm,
				++Ships[WhoIAm].PickupIdCount, -1, true,
				-1.0F, (u_int16_t) -1 );
			
			if( ( i != (u_int16_t) -1 )  && ( i != (u_int16_t) -2 ) )
			{
				DropPickupSend( &goal->pos, goal->group,
					&Dir, 0.0F, TeamFlagPickup[ team ], Ships[WhoIAm].PickupIdCount,
					-1, true, -1.0F, (u_int16_t) -1 );
			}
			else
			{
				DebugPrintf( "Unable to generate %s flag\n", TeamName[ team ] );
			}
			return true;
		}
	}
	return false;
}


float NearestGoalDistance( int goal )
{
	int j;
	float d, dmin;
	VECTOR diff;

	dmin = 0.0F;
	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].state != GOALSTATE_On || Goal[ j ].team < 0 || j == goal )
			continue;
		diff.x = Goal[ j ].pos.x - Goal[ goal ].pos.x;
		diff.y = Goal[ j ].pos.y - Goal[ goal ].pos.y;
		diff.z = Goal[ j ].pos.z - Goal[ goal ].pos.z;
		d = 0.1F * VectorLength( &diff ) + SoundInfo[ Goal[ j ].group ][ Goal[ goal ].group ];
		if ( !dmin || d < dmin )
		{
			dmin = d;
		}
	}
	return dmin;
}


int PickTeamGoal( int team )
{
	int j;
	int goal;
	float d, dmax = 0.0f;

	goal = -1;
	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].team < 0 )
		{
			d = NearestGoalDistance( j );
			if ( !d )
			{
				DebugPrintf( "Allocating first team goal at random\n" );
				return RandomStartPosModify % NumGoals;
			}
			else if ( goal < 0 || d > dmax )
			{
				goal = j;
				dmax = d;
				DebugPrintf( "Team %d candidate goal %d (d=%f)\n",
					team, goal, d );
			}
		}
		else if ( Goal[ j ].team == team )
		{
			DebugPrintf( "Reallocating existing team %d goal %d\n",
				team, j );
			return j;
		}
	}
	DebugPrintf( "Allocating team %d goal %d (dmax=%f)\n",
		team, goal, dmax );
	return goal;
}


void SetTeamGoals( u_int16_t *TeamGoals )
{
	int j;

	for ( j = 0; j < MAX_TEAMS; j++ )
	{
		Teamgoal[ j ] = TeamGoals[ j ];
		if ( Teamgoal[ j ] >= 0 && Teamgoal[ j ] < NumGoals )
		{
			if ( Goal[ Teamgoal[ j ] ].team < 0 )
				TeamFlagAtHome[ j ] = false;
			Goal[ Teamgoal[ j ] ].team = j;
		}
		else
		{
			Teamgoal[ j ] = -1;
			TeamFlagAtHome[ j ] = false;
		}
		DebugPrintf( "Teamgoal[ %d(%s) ] = %hd\n",
			j, TeamName[ j ], Teamgoal[ j ] );
	}
	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].team >= 0 && Goal[ j ].team < MAX_TEAMS )
		{
			Goal[ j ].state = ( Teamgoal[ Goal[ j ].team ] == j )
				? GOALSTATE_On : GOALSTATE_Off;
		}
		else
			Goal[ j ].state = GOALSTATE_Off;
	}
	for ( j = 0; j < MAX_TEAMS; j++ )
	{
		DebugPrintf( "Goal[ %d ].team = %d(%s)\n",
			j, Goal[ j ].team,
			( Goal[ j ].team >= 0 ) ? TeamName[ Goal[ j ].team ] : "none" );
	}
}


extern  BYTE          MyGameStatus;
void ProcessGoals( void )
{
	int j, k;
	int NumFlags[ MAX_TEAMS ];
	int NewTeams = 0;

	if( ( ( MyGameStatus != STATUS_Normal ) && ( MyGameStatus != STATUS_PlayingDemo ) ) ||
		( !CaptureTheFlag && !CTF ) || !NumGoals || !Goal ) // paranoia attack...
		return;

	for ( j = 0; j < MAX_TEAMS; j++ )
	{
		TeamFlag[ j ] = 0;
		NumFlags[ j ] = NumPickupType[ TeamFlagPickup[ j ] ];
		LastTeamMembers[ j ] = GoalTeamMembers[ j ];
		GoalTeamMembers[ j ] = 0;
	}
	for ( k = 0; k < MAX_PLAYERS; k++ )
	{
		if ( ( ( GameStatus[ k ] == MyGameStatus && !PlayDemo ) 
			|| ( GameStatus[ k ] == STATUS_Normal && PlayDemo ) )
			&& TeamNumber[ k ] < MAX_TEAMS )
		{
			GoalTeamMembers[ TeamNumber[ k ] ]++;
			if ( Ships[ k ].Object.Flags & SHIP_CarryingFlags )
			{
				TeamFlag[ TeamNumber[ k ] ]++;
				for ( j = 0; j < MAX_TEAMS; j++ )
				{
					if ( Ships[ k ].Object.Flags & TeamFlagMask[ j ] )
					{
						NumFlags[ j ]++;
					}
				}
			}
		}
	}
	if ( IsHost )
	{
		NewTeams = 0;
		for ( j = 0; j < MAX_TEAMS; j++ )
		{
			if ( !LastTeamMembers[ j ] && GoalTeamMembers[ j ] )
			{
				NewTeams++;
				if ( Teamgoal[ j ] < 0 )
				{
					// new team has appeared -> pick a goal for them
					Teamgoal[ j ] = PickTeamGoal( j );
					SetTeamGoals( Teamgoal );
				}
			}
		}
		if ( NewTeams )
		{
			SetTeamGoals( Teamgoal );
			if ( !PlayDemo )
				TeamGoalsSend( Teamgoal );
		}
	}
	for ( j = 0; j < NumGoals; j++ )
	{
		Goal[ j ].state = ( Goal[ j ].team >= 0 && GoalTeamMembers[ Goal[ j ].team ] )
			? GOALSTATE_On : GOALSTATE_Off;
	}
	if ( IsHost && !PlayDemo )
	{
		if ( NewTeams )
		{
			for ( j = 0; j < MAX_TEAMS; j++ )
			{
				if ( !LastTeamMembers[ j ] && GoalTeamMembers[ j ] )
				{
					if ( CTF ) // new team has appeared -> generate their flag
						GenerateFlagAtHome( j );
				}
			}
		}
		else
		{
			int NewPlayers;

			NewPlayers = 0;
			for ( j = 0; j < MAX_PLAYERS; j++ )
			{
				if ( Ships[ j ].enable && !LastEnable[ j ] )
					NewPlayers++;
				LastEnable[ j ] = Ships[ j ].enable;
			}
			if ( NewPlayers )
			{
				TeamGoalsSend( Teamgoal );
			}
		}
	}
	if ( CTF && !PlayDemo )
	{
		for ( j = 0; j < MAX_TEAMS; j++ )
		{
			if ( !GoalTeamMembers[ j ] && LastTeamMembers[ j ] )
			{
				// and old team has disappeared -> kill all their flags
				KillAllPickupsOfType( TeamFlagPickup[ j ], PICKUPKILL_Immediate );
				Ships[ WhoIAm ].Object.Flags &= ~TeamFlagMask[ j ];
				PickupsGot[ TeamFlagPickup[ j ] ] = 0;
				TeamFlagAtHome[ j ] = false;
			}
		}
	}
}


int GoalCheckTeam( VECTOR * OldPos , VECTOR * NewPos, u_int16_t Group, int team )
{
	int j;

	for ( j = 0; j < NumGoals; j++ )
	{
		if( ( Goal[ j ].state == GOALSTATE_On || DebugInfo )
			&& ( Goal[ j ].group == Group || GroupsAreConnected( Goal[ j ].group, Group ) ) )
		{
			GoalCheck( OldPos , NewPos , &Goal[ j ] );
			
			if( Entry )
			{
				if ( Goal[ j ].team == team )
					return GOAL_SCORED;
				else
					return GOAL_WRONG;
			}
			else if ( In )
			{
				if ( Goal[ j ].team == team )
				   return GOAL_SCORED;
			}
		}
	}
	return GOAL_NONE;
}


int DisplayGoal( u_int16_t group )
{
	int j, k;
	int shown;
	u_int8_t c[ 3 ];

	if ( !CaptureTheFlag && !CTF )
		return 0;
	shown = 0;
	for ( j = 0; j < NumGoals; j++ )
	{
		if ( Goal[ j ].group == group
			&& ( ( Goal[ j ].state == GOALSTATE_On ) || DebugInfo ) )
		{
			for ( k = 0; k < 3; k++ )
			{
				c[ k ] = Colourtrans[ TeamCol[ Goal[ j ].team ] ][ k ];
			}
//			DisplayTriggerZone( Goal[ j ].Zone, Goal[ j ].num_sides, group, c[ 0 ], c[ 1 ], c[ 2 ], 128 );
			ShowGoal( &Goal[ j ], c[ 0 ], c[ 1 ], c[ 2 ], 128 );
			shown++;
		}
	}
	return shown;
}

/*===================================================================
	Procedure	:	Show Goal
	Input		:	GOAL	*	Goal
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
				:	u_int8_t		Trans
	Output		:	Nothing
===================================================================*/
float	SparkleTime = 0.1F;

void ShowGoal( GOAL * Goal, u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans )
{
	int16_t	Count;
	u_int16_t	fmpoly;
	VECTOR	Offset;
	VECTOR	Pos;
	QUAT	TempQuat;
	MATRIX	GoalMatrix;

	QuatFromDirAndUp( &Goal->dir, &Goal->up, &TempQuat );
	QuatToMatrix( &TempQuat, &GoalMatrix );

	for( Count = 0; Count < 10; Count++ )
	{
		SparkleTime -= ( framelag * 0.1F );

		if( SparkleTime <= 0.0F )
		{
			SparkleTime = 0.1F;

			Offset.x = (float) ( Random_Range( (u_int16_t) Goal->width ) - ( Goal->width / 2.0F ) );
			Offset.y = (float) ( Random_Range( (u_int16_t) Goal->height ) - ( Goal->height / 2.0F ) );
			Offset.z = (float) ( Random_Range( (u_int16_t) Goal->depth ) - ( Goal->depth / 2.0F ) );

			ApplyMatrix( &GoalMatrix, &Offset, &Pos );

			Pos.x += Goal->pos.x;
			Pos.y += Goal->pos.y;
			Pos.z += Goal->pos.z;

			fmpoly = FindFreeFmPoly();

			if( fmpoly != (u_int16_t ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 20.0F;
				FmPolys[ fmpoly ].Pos = Pos;
				FmPolys[ fmpoly ].SeqNum = FM_GOALSPARKLE;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].R = Red;
				FmPolys[ fmpoly ].G = Green;
				FmPolys[ fmpoly ].B = Blue;
				FmPolys[ fmpoly ].Trans = Trans;
				FmPolys[ fmpoly ].Frm_Info = &GreyFlare_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = 0.0F;
				FmPolys[ fmpoly ].ysize = 0.0F;
				FmPolys[ fmpoly ].Dir.x = ( Random_Range( 10000 ) / 5000.0F ) - 1.0F;
				FmPolys[ fmpoly ].Dir.y = ( Random_Range( 10000 ) / 5000.0F ) - 1.0F;
				FmPolys[ fmpoly ].Dir.z = ( Random_Range( 10000 ) / 5000.0F ) - 1.0F;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				FmPolys[ fmpoly ].Speed = 2.5F;
				FmPolys[ fmpoly ].Group = Goal->group;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		}
	}
}
