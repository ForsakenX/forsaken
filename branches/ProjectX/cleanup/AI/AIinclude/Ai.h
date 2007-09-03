/*
 * a i . h
 *
 * AI Header File
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 53 $
 *
 * $Header: /PcProjectX/Ai/AIInclude/Ai.h 53    21/02/98 14:29 Collinsd $
 *
 * $Log: /PcProjectX/Ai/AIInclude/Ai.h $
 * 
 * 53    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 52    2/03/98 4:29p Phillipd
 * Dans modifications to enemies. ( CHECKED IN BY DAVE )
 * 
 * 51    1/30/98 11:16a Phillipd
 * 
 * 50    1/28/98 5:16p Phillipd
 * 
 * 49    1/27/98 2:58p Phillipd
 * 
 * 48    1/24/98 5:10p Phillipd
 * 
 * 47    1/23/98 9:57a Phillipd
 * 
 * 46    1/16/98 2:54p Phillipd
 * 
 * 45    1/13/98 12:25p Phillipd
 * 
 * 44    1/08/98 4:18p Phillipd
 * 
 * 43    1/06/98 10:01a Phillipd
 * 
 * 42    12/15/97 2:43p Phillipd
 * 
 * 41    12/04/97 4:55p Phillipd
 * 
 * 40    11/25/97 4:13p Phillipd
 * 
 * 39    11/25/97 3:13p Phillipd
 * 
 * 38    11/25/97 2:37p Phillipd
 * 
 * 37    11/22/97 1:23p Phillipd
 * 
 * 36    11/20/97 11:44a Phillipd
 * 
 * 35    11/17/97 10:13a Phillipd
 * 
 * 34    11/12/97 5:00p Phillipd
 * 
 * 33    11/11/97 11:01a Phillipd
 * 
 * 32    11/07/97 12:20p Phillipd
 * 
 * 31    10/29/97 12:25p Phillipd
 * 
 * 30    10/25/97 3:50p Phillipd
 * 
 * 29    10/24/97 5:05p Phillipd
 * 
 * 28    10/22/97 12:14p Phillipd
 * 
 * 27    10/21/97 5:07p Phillipd
 * 
 * 26    9/15/97 11:24a Phillipd
 * 
 * 25    9/10/97 9:54a Phillipd
 * 
 * 24    9/03/97 3:50p Phillipd
 * 
 * 23    2/09/97 9:06 Collinsd
 * Added one off avoid into enemy flags
 * 
 * 22    9/02/97 8:52a Phillipd
 * 
 * 21    1/09/97 14:18 Collinsd
 * Added enemy targeted pos, dir and flag.
 * 
 * 20    9/01/97 12:17p Phillipd
 * 
 * 19    8/31/97 1:05p Phillipd
 * 
 * 18    8/29/97 12:27p Phillipd
 * 
 * 17    8/27/97 3:43p Phillipd
 * 
 * 16    8/27/97 9:21a Phillipd
 * 
 * 15    8/22/97 10:16a Phillipd
 * 
 * 14    8/19/97 12:59p Phillipd
 * 
 * 13    8/17/97 2:58p Phillipd
 * 
 * 12    8/17/97 1:33p Phillipd
 * 
 * 11    8/17/97 1:17p Phillipd
 * 
 * 10    8/15/97 4:33p Phillipd
 * 
 * 9     8/15/97 4:25p Phillipd
 * 
 * 8     8/15/97 4:24p Phillipd
 * 
 * 7     7/31/97 9:09a Phillipd
 * 
 * 6     7/21/97 10:28a Phillipd
 * 
 * 5     7/18/97 10:17a Phillipd
 * 
 * 4     7/12/97 3:34p Phillipd
 * 
 * 3     7/07/97 2:12p Phillipd
 * 
 * 2     7/03/97 10:11a Phillipd
 * More Ai and node
 * 
 * 1     6/30/97 10:35a Phillipd
 *
 */
 
#ifndef AI_H
#define AI_H

//--------------------------------------------------------------------------
// Define AI modes
//--------------------------------------------------------------------------

enum
{
	AIMODE_PLAYER,					// Controlled by PLAYER
	AIMODE_FOLLOWPATH,				// Follow node path
	AIMODE_IDLE,					// Do nothing for a while
	AIMODE_SCAN,					// Look around for a target
	AIMODE_MOVETOTARGET,			// Move towards selected target   
	AIMODE_FIRE,					// Fire at PLAYER
	AIMODE_DOGFIGHT,				// Spar with the player		Move in swrily motion...
	AIMODE_KILLMINE,				// Attack And kill a Mine....
	AIMODE_RETREAT,					// Try and get away from PLAYER
	AIMODE_FORMATION,				// Stay in Formation and attack whan necessary...

	AIMODE_DEATH_CRASHNBURN,		// Fall to the ground and Shatter...
	AIMODE_DEATH_PINGOFF,			// Ping off in the dir of the bullet that killed me spinning as I go...
	AIMODE_DEATH_PSYCHO,			// Twist and turn and shoot and explode...

	AIMODE_DEATH_DEADINTHEWATER,	// Float or stop occasionally Spark or explode...
	
	AIMODE_STRAFE,					// Do a passing Run at the player firing as I go..
	AIMODE_GETBEHIND,				// Get behind the player then start dog fighting him again...
	AIMODE_AVOIDHOMING,				// Avoid a Homing Missile..
	AIMODE_FINDCOVER,				// Find a 'safe' spot for a while
	AIMODE_UNLOAD,					// Unload all at PLAYER
	AIMODE_BACKOFF,					// Back off firing at PLAYER
	AIMODE_STALK,					// Stalk PLAYER from a distance
	AIMODE_FLOCK,					// Flock around a point
};
enum
{
	AIMODE_EXOGENON_IDLE,
	AIMODE_EXOGENON_MOVEDOWN,
	AIMODE_EXOGENON_FIRE,
	AIMODE_EXOGENON_MOVEUP,
	AIMODE_EXOGENON_SCAN,
	AIMODE_EXOGENON_SWEEP,
};

enum
{
	AIMODE_FLESHMORPH_IDLE,
	AIMODE_FLESHMORPH_MOVEFORWARDS,
	AIMODE_FLESHMORPH_STOPFORWARD,
	AIMODE_FLESHMORPH_MOVEBACKWARDS,
	AIMODE_FLESHMORPH_STOPBACKWARD,
};



// Ai Status.... Enemy->AIFlags
#define AI_ITHINK						(1<<0)
#define AI_PLAYERINRANGE				(1<<1)
#define AI_ANYPLAYERINRANGE				(1<<2)
#define AI_ICANSEEPLAYER				(1<<3)
#define AI_IHAVESEENPLAYER				(1<<4)
#define AI_FRIENDLYFIRE					(1<<5)		// If I fire at my target I might cause friendly fire....
#define AI_PLAYERCANSEEME				(1<<6)
#define AI_PLAYERISTARGETINGME			(1<<7)
#define AI_PLAYERHASFIREDATME			(1<<8)
#define AI_IVEBEENHITBYPLAYER			(1<<9)
#define AI_ICANHEARPLAYER				(1<<10)
#define	AI_PLAYERHASFIREDPRIMARY		(1<<11)
#define	AI_PLAYERHASFIREDSECONDARY		(1<<12)
#define	AI_TARGETED						(1<<13)
#define	AI_TARGETED_ONEOFF				(1<<14)
#define	AI_MINEISCLOSE					(1<<15)
#define	AI_MINEAVOID					(1<<16)



// Behaviour
#define AI_BEHAVIOUR_LEAVENETWORK		(1<<0)		// Can I Leave My Network to persue Target...
#define AI_BEHAVIOUR_NOFRIENDLYFIRE		(1<<1)		// Do I Care About Friendly Fire...
#define AI_BEHAVIOUR_AVOID_PRIMARY		(1<<2)
#define AI_BEHAVIOUR_AVOID_SECONDARY	(1<<3)
#define AI_BEHAVIOUR_AVOID_MINES		(1<<4)
#define AI_BEHAVIOUR_ANTICIPATE_MOVE	(1<<5)
#define AI_BEHAVIOUR_ATTACK_ONSITE		(1<<6)		// If I See the player Should I Attack him...
#define AI_BEHAVIOUR_ATTACK_PROVOKE		(1<<7)		// If the Player Hits me should I Hit Back...
#define AI_BEHAVIOUR_ATTACK_FIND		(1<<8)		// Can I Find the player if I have allready attacked him...
#define AI_BEHAVIOUR_NETWORKRANGE		(1<<9)		// Do I Have a max range that I can Wander from my network???
#define AI_BEHAVIOUR_WEAPONKNOWLEDGE	(1<<10)		// Can I Take advantage of certain weapon charcteristics..ie suss gun limited range??
#define AI_BEHAVIOUR_ICANTPITCH			(1<<11)		// I cant turn in the X..So I must try and get level with the player by sliding up and down...
#define AI_BEHAVIOUR_ICANTYAW			(1<<12)		// I cant turn in the Y..So I must try and get level with the player by sliding left and right...
#define AI_BEHAVIOUR_ATTACKMINES		(1<<13)		// If I see a Mine Should I Attack It..
#define AI_BEHAVIOUR_AVOIDMINES			(1<<14)		// If I see a Mine Should I Avoid It..
#define AI_BEHAVIOUR_DROPMINES			(1<<15)		// If My Nearest Node is a mine dropping node then I can Drop Mines..
#define AI_BEHAVIOUR_RAMMINES			(1<<16)		// If I find a mine ram the bastard....
#define AI_BEHAVIOUR_RETREAT			(1<<17)		// If I Get hurt bad I should run away..
#define AI_BEHAVIOUR_DONTSTOPANDTURN	(1<<18)		// should I stop when I turn...
#define AI_BEHAVIOUR_PRECISECONTROL		(1<<19)		// No bouncy inertial shit...Robotic and Menacing movements...
#define AI_BEHAVIOUR_CLOAK				(1<<20)		// I Can Cloak...
#define AI_BEHAVIOUR_NOBOB				(1<<21)		// I Dont Bob...
#define AI_BEHAVIOUR_NOTURN				(1<<22)		// I Dont Turn..
#define AI_BEHAVIOUR_RANDSECONDARYFIREPOS	(1<<23)		// Fire my secondary bullets randomly...


#define	AI_CONTROL_FORWARD	(1<<0)
#define	AI_CONTROL_BACK		(1<<1)
#define	AI_CONTROL_LEFT		(1<<2)
#define	AI_CONTROL_RIGHT	(1<<3)
#define	AI_CONTROL_UP		(1<<4)
#define	AI_CONTROL_DOWN		(1<<5)
#define	AI_CONTROL_TURNLEFT		(1<<6)
#define	AI_CONTROL_TURNRIGHT	(1<<7)
#define	AI_CONTROL_TURNUP		(1<<8)
#define	AI_CONTROL_TURNDOWN		(1<<9)
#define	AI_CONTROL_FIREPRIMARY	(1<<10)


#define	AI_CONTROL_NO_FORWARD	(1<<11)
#define	AI_CONTROL_NO_BACK		(1<<12)
#define	AI_CONTROL_NO_LEFT		(1<<13)
#define	AI_CONTROL_NO_RIGHT		(1<<14)
#define	AI_CONTROL_NO_UP		(1<<15)
#define	AI_CONTROL_NO_DOWN		(1<<16)

#define	AI_CONTROL_COLLISION	(1<<17)




// Avoidance Types....
#define AVOID_LEFT				(AI_CONTROL_LEFT)
#define AVOID_RIGHT				(AI_CONTROL_RIGHT)
#define AVOID_UP				(AI_CONTROL_UP)
#define AVOID_DOWN				(AI_CONTROL_DOWN)
#define AVOID_FORWARD			(AI_CONTROL_FORWARD)
#define AVOID_BACK				(AI_CONTROL_BACK)



//--------------------------------------------------------------------------
// Define AI mode timer values
//--------------------------------------------------------------------------

#define ONE_SECOND				(60.0F)

#define RESET_IDLE_TIME			(ONE_SECOND * 1.0F)
#define RESET_SCAN_TIME			(ONE_SECOND * 0.25F)
#define RESET_VALIDATE_TIME		(ONE_SECOND * 0.25F)
#define RESET_FIRE_TIME			(ONE_SECOND * 0.25F)

#define PINGOFFTIME				(ONE_SECOND * 3.0F)

//--------------------------------------------------------------------------
// Define Primary Weapon Data...
//--------------------------------------------------------------------------
typedef struct PRIMARYWEAPONKNOWLEDGE{
	float	MinDistance;		// How close should I get to make the weapon less effective....
	float	AccuracyAngle;		// How accurate does the player have to be to hit me.... 
}PRIMARYWEAPONKNOWLEDGE;



//--------------------------------------------------------------------------
// Define TARGETTING data structure
//--------------------------------------------------------------------------

typedef struct {

	VECTOR			Pos;									// Initial search position
	uint16			Group;									// Initial Search Group..
	float			PosDist;								// Distance of 'Pos' from targetting SHIP
	float			Nearest;								// Distance to nearest target
	uint32			Flags;								// Flags - see below
	void	*		SObject;								// Pointer to targetting SHIP   
	void	*		TObject;								// Pointer to target
	uint32			TFlags;								// Flags - see below
} TARGETDATA;

//--------------------------------------------------------------------------
// Define Aim data structure
//--------------------------------------------------------------------------

typedef struct {
	int32	Flags;								// Flags - Say wether to turn left right etc...
	VECTOR	Angle;								// The difference in x y z...
}AIMDATA;
//--------------------------------------------------------------------------
// Define TARGETTING flags / macros
//--------------------------------------------------------------------------

#define	_TARGET_CURRENT		(1 << 0)				// Target in current GROUP only
#define	_TARGET_ENEMIES		(1 << 1)				// Target ENEMIES
#define	_TARGET_PLAYERS		(1 << 2)				// Target PLAYERS
#define	_TARGET_NODES		(1 << 3)				// Target NODES
#define	_TARGET_PICKUPS		(1 << 4)				// Target PICKUPS
#define	_TARGET_TRIGGERS	(1 << 5)				// Target TRIGGERS
#define	_TARGET_MINES		(1 << 6)				// Target MINES

#define	SET_TARGET_CURRENT	(Tinfo->Flags |= _TARGET_CURRENT)
#define	SET_TARGET_ENEMIES	(Tinfo->Flags |= _TARGET_ENEMIES)
#define	SET_TARGET_PLAYERS	(Tinfo->Flags |= _TARGET_PLAYERS)
#define	SET_TARGET_NODES	(Tinfo->Flags |= _TARGET_NODES)
#define	SET_TARGET_PICKUPS	(Tinfo->Flags |= _TARGET_PICKUPS)
#define	SET_TARGET_TRIGGERS	(Tinfo->Flags |= _TARGET_TRIGGERS)
#define	SET_TARGET_MINES	(Tinfo->Flags |= _TARGET_MINES)

#define	TARGET_CURRENT			(Tinfo->Flags & _TARGET_CURRENT)
#define	TARGET_ENEMIES			(Tinfo->Flags & _TARGET_ENEMIES)
#define	TARGET_PLAYERS			(Tinfo->Flags & _TARGET_PLAYERS)
#define	TARGET_NODES			(Tinfo->Flags & _TARGET_NODES)
#define	TARGET_PICKUPS			(Tinfo->Flags & _TARGET_PICKUPS)
#define	TARGET_TRIGGERS			(Tinfo->Flags & _TARGET_TRIGGERS)
#define	TARGET_MINES			(Tinfo->Flags & _TARGET_MINES)

#define 	BIGDISTANCE				(32000.0F)

#define 	END_OF_TYPES			((int8) 255)

//--------------------------------------------------------------------------
// External data
//--------------------------------------------------------------------------

extern TARGETDATA *				Tinfo;

extern	GUNTYPE	GunTypes[];
extern	ENEMY_TYPES	EnemyTypes[ MAX_ENEMY_TYPES ];
extern	float	framelag;
extern	float	SoundInfo[MAXGROUPS][MAXGROUPS];
extern	GLOBALSHIP              Ships[MAX_PLAYERS+1];
void DebugPrintf( const char * format, ... );
extern	PRIMARYWEAPONKNOWLEDGE PrimaryKnowledge[];

//--------------------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------------------

// AILIB.C

void AI_GetDistToNearestTarget( register ENEMY * Enemy );
BOOL AI_InViewCone( VECTOR * Pos, MATRIX * Mat ,VECTOR * TPos, float ViewConeCos );
BOOL AI_ClearLOS( VECTOR * SPos, uint16 Group , VECTOR * Pos );
BOOL AI_ClearLOSNonZero( OBJECT * SObject, VECTOR * Pos , float radius );
BOOL AI_ClearLOSNonZeroNonObject( VECTOR * SPos, uint16 Group , VECTOR * Pos , float radius );
void AI_SetIDLE( register ENEMY * Enemy );
void AI_SetSCAN( register ENEMY * Enemy );
void AI_SetMOVETOTARGET( ENEMY * Enemy );
void AI_SetFIREATTARGET( ENEMY * Enemy );
void AI_AimAtTarget( MATRIX * InvMat , VECTOR * SPos, VECTOR * TPos );
void AI_GetNearestNODETarget( OBJECT * Object );
void AI_SetMOVEFOLLOWPATH( ENEMY * Enemy );
void AI_LookAhead( float Accuracy, VECTOR * SPos ,OBJECT * TObject , VECTOR * NewPos , float SpeedOfBullet );
void AI_SetFOLLOWPATH( register ENEMY * Enemy );
void AI_SetDOGFIGHT( register ENEMY * Enemy );
void AI_SetKILLMINE( register ENEMY * Enemy );
void AI_GetNearestMINETarget( void );
void AI_SetRETREAT( register ENEMY * Enemy );
void AI_SetFORMATION( ENEMY * Enemy );

// AIMAIN.C
void AI_AIR( ENEMY * Enemy );
void AI_TURRET( ENEMY * Enemy );
void AI_CRAWL( ENEMY * Enemy );
void AI_THINK( register ENEMY * Enemy , BOOL OverideThinkTime , BOOL JustCheckPlayerRange );
void AI_AVOIDCHECK( register ENEMY * Enemy );
void AI_SPLINE( ENEMY * Enemy );
void AI_EXOGENON( ENEMY * Enemy );
void AI_LITTLEGEEK( ENEMY * Enemy );
void AI_FLESHMORPH( ENEMY * Enemy );
BOOL Enemy2EnemyFriendlyFireCheck( ENEMY * SEnemy );

// AIIDLE.C
void AI_AIR_IDLE( ENEMY * Enemy );
void AI_TURRET_IDLE( ENEMY * Enemy );
void AI_CRAWL_IDLE( ENEMY * Enemy );

// AISCAN.C
void AI_AIR_SCAN( ENEMY * Enemy );
void AI_TURRET_SCAN( ENEMY * Enemy );
void AI_CRAWL_SCAN( ENEMY * Enemy );
void AI_DO_SCAN( register ENEMY * Enemy );

// AIMOVET.C
void AI_AIR_MOVETOTARGET( ENEMY * Enemy );

// AIDOGFIGHT.C
void AI_AIR_DOGFIGHT( register ENEMY * Enemy );

// AIKILLMINE.C
void AI_AIR_KILLMINE( register ENEMY * Enemy );


// AIFIRE.C
void AI_TURRET_FIREATTARGET( ENEMY * Enemy );
void AI_UPDATEGUNS( register ENEMY * Enemy );

// AIFOLLOW.C
void AI_AIR_FOLLOWPATH( register ENEMY * Enemy );
void AI_CRAWL_FOLLOWPATH( register ENEMY * Enemy );


// AIRETREAT.C
void AI_AIR_RETREAT( register ENEMY * Enemy );

// AIFORMATION.C
void AI_AIR_FORMATION( register ENEMY * Enemy );
void AI_LITTLEGEEK_FORMATION( register ENEMY * Enemy );

// AISPLINE.C
void AI_SPLINE_FOLLOWPATH( register ENEMY * Enemy );


// AICRASHANDBURN.C
void AI_AIR_DEATH_CRASHANDBURN( register ENEMY * Enemy );
// AIPINGOFF.C
void AI_AIR_DEATH_PINGOFF( register ENEMY * Enemy );
// AIPSYCHO.C
void AI_AIR_DEATH_PSYCHO( register ENEMY * Enemy );


// AIEXOGENON.c
void AI_EXOGENON_IDLE( ENEMY * Enemy );
void AI_EXOGENON_MOVEDOWN( ENEMY * Enemy );
void AI_EXOGENON_FIRE( ENEMY * Enemy );
void AI_EXOGENON_MOVEUP( ENEMY * Enemy );
void AI_EXOGENON_SCAN( ENEMY * Enemy );
void AI_EXOGENON_SWEEP( ENEMY * Enemy );

// AIFLESHMORPH.c
void AI_FLESHMORPH_IDLE( register ENEMY * Enemy );
void AI_FLESHMORPH_MOVEFORWARDS( register ENEMY * Enemy );
void AI_FLESHMORPH_STOPFORWARD( register ENEMY * Enemy );
void AI_FLESHMORPH_MOVEBACKWARDS( register ENEMY * Enemy );
void AI_FLESHMORPH_STOPBACKWARD( register ENEMY * Enemy );



#endif // AI_H
//--------------------------------------------------------------------------
 
 