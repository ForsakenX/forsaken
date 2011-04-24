/*===================================================================
	File	:	bgobjects.h
	Content	:	BGObjects.c include file
===================================================================*/
#ifndef ENEMIES_INCLUDED
#define ENEMIES_INCLUDED

#include "node.h"
#include "triggers.h"

/*===================================================================
	Defines
===================================================================*/
#define	FRONT_LEFT	0
#define	FRONT_RIGHT	1
#define	BACK_LEFT	2
#define	BACK_RIGHT	3
#define MAXGUNSPERENEMY 5

#define	ENABLEENEMYCOLLISIONS	1

/*===================================================================
	Structures
===================================================================*/
typedef struct FIREPOS {
	int16	NumPoints;
	VECTOR	Points[ 6 ];
	VECTOR	Dir[ 6 ];
} FIREPOS;

#define ENEMY_TAUNT_PAUSE 600.0F;

typedef struct ENEMY {
	BOOL	Used;
	BOOL	Alive;			// Am I Alive or dead
	BOOL	CompCollision;
	OBJECT	Object;
	uint16	Type;
	uint32	AIMoveFlags;
	uint32	AIFlags;
	VECTOR	AI_Angle;
	float	Timer;
	float	ThinkTimer;			// Is It Time for me to have a bit of a think???
	float	AIMoveRestrictClearTime; // how long before I Clear my movement restrictions???
	float	IveBeenHitTimer;	// if this is non zero then I can go for the player even if im out of range...

	void *	TShip;
	void *	TNode;

	void *	NextTNode;		// What node Im Trying to get nearest to....after my current TNode...
	void *	LastTNode;		// What node I was nearest to before my Next Target Node...
	
	float	LastDistance;
	BYTE	LastPrimaryFiredAtMe;
	BYTE	LastSecondaryFiredAtMe;
	float	Viewcone;
	uint16	PickupHeld;				// When I Die do I Release a Pickup???

	VECTOR	TargetingPos;
	VECTOR	TargetingDir;

	VECTOR	MinePos;
	float	MineDistance;
	uint16	MineNum;

	float	AvoidTimer;
	uint16	AvoidType;
	
	uint16	Status;				// Status
	uint16	GenType;			// Generation Type
	uint16	BulletID;			// ID
	uint16	ModelNum;			// ModelNum
	uint16	ModelIndex;			// Model Array Index
	uint16	TriggerMod;			// Trigger
	float	Delay;

	TRIGGERMOD * TriggerModPtr;	// Trigger Mod Ptr

	struct	ENEMY	*	PrevUsed;
	struct	ENEMY	*	NextUsed;
	struct	ENEMY	*	PrevFree;
	struct	ENEMY	*	NextFree;

	struct	ENEMY	*	NextInGroup;// Next in same group ....
	struct	ENEMY	*	PrevInGroup;// Previous in same group ....
	uint16	Index;

	float	PrimaryFireTimer;
	float	SecondaryFireTimer;
	BOOL	ImInNodeTransition;
	BOOL	PickNewNodeNow;
	float	DistanceToPointOnSlope;

struct ENEMY * FormationLink;
	VECTOR	FormationOffset;

	void * SplineNode1;
	void * SplineNode2;
	void * SplineNode3;
	void * SplineNode4;
	NODE	TempSplineNode;


	VECTOR	LastDamagePos;
	VECTOR	LastDamageImpactPos;
	VECTOR	LastDamageDir;
	float	LastDamage;
	float	LastDamageSpeed;

	float	CloakTimer;

	float	GenerationDelay;
	float	MasterGenerationDelay;

	float	SmokeTime;

	int16				FirePosCount;	// if I have more than 1 firing position then cycle between them..

#ifdef DEBUG_ON
	// used for finding out where an enemy is killed from...
	char *	EnemyInFile;
	int		EnemyInLine;
#endif

	float TauntPause;
} ENEMY;

typedef struct AI_BEHAVIOUR{
	uint32	Flags;								// flags to show what stuff I can Do..
	float	Anticipate_Move;					// when targeting the player how far ahead can I predict where he will be....
	float	NetworkRange;						// how far from my net work do I Wander...
	float	Avoid_Primary;						// if player fires at me and I can see it check to see if I should avoid...
	float	Avoid_Secondary;					//
	float	Avoid_Mines;						// If I move towards a mine should I try to avoid it...
	float	IdleTime;							// How long do I Stay Idle...
	float	ScanTime;							// How long do I Scan For...
	float	ThinkTime;							// How often do I Update my Think Status...
	float	ThinkRange;							// How far away does a valid player target have to be before I go To Sleep...
	float	HearRange;							// How far away does a valid player target have to be before I can hear him..
	float	BlindSearchTime;					// If I lose my Target how long will I persue him...
	float	RetreatTime;					// If I lose my Target how long will I persue him...
} AI_BEHAVIOUR;


typedef struct ENEMY_TYPES {

	char *	ModelFilename;		// Model Filename or Component Filename ( .MX/.MXA or .COB )
	BOOL	StealthMode;		// Can use Stealth
	BOOL	LevelSpecific;		// Level Specific
	BOOL	CompCollision;		// Component collision
	int16	ModelNumber;		// -1 None ( Not used so far )
	ANIM_SEQ * AnimSeqs;		// Animation Sequences ( NULL if none )
	FIREPOS * FirePoints;
	VECTOR * AimPos;
	uint16	ControlType;
	uint16	AiMode;				// what initial Ai mode I Have...

	uint16	NumOfGuns;			// How many Guns do I Have???
	uint16	GunType[MAXGUNSPERENEMY];			// What type of gun???
	FIREPOS * GunFirePoints[MAXGUNSPERENEMY];
	VECTOR * GunAimPos[MAXGUNSPERENEMY];

	float	TurnRateAccell;
	float	TurnRateDecell;
	float	MaxTurnRate;
	float	MoveRateAccell;
	float	MoveRateDecell;
	float	MaxMoveRate;
	float	Viewcone;
	float	MinRange;			// minimum range I can dogfight at...
	float	MaxRange;			// maximum range I can dogfight at...
	int16	Shield;
	float	MoveRestrictClearTime;
	float	Radius;
	float	PrimaryFireRate;
	float	SecondaryFireRate;

	int16	PowerLevel;
	BYTE	PrimaryWeaponType;			// which Primaryt weapon do I Fire...
	BYTE	SecondaryWeaponType;		// which Primaryt weapon do I Fire...

//	BOOL	DoIBank;					// When Turning Left and Right Do I Bank???

struct	AI_BEHAVIOUR Behave;
} ENEMY_TYPES;


/*===================================================================
	Defines
===================================================================*/
enum
{
	ENEMY_BeamTurret,			// 0
	ENEMY_DuelTurret,			// 1
	ENEMY_PulseTurret,			// 2
	ENEMY_MissileTurret,		// 3
	ENEMY_SnubTurret,			// 4
	ENEMY_LazerBot,				// 5
	ENEMY_SnubBot,				// 6
	ENEMY_AirMoble,				// 7
	ENEMY_AmmoDump,				// 8
	ENEMY_LeviTank,				// 9
	ENEMY_Max,					// 10
	ENEMY_Mekton,				// 11
	ENEMY_Boss_Nukecrane,		// 12
	ENEMY_Supressor,			// 13
	ENEMY_MineSweeper,			// 14
	ENEMY_Swarm,				// 15
	ENEMY_Shade,				// 16
	ENEMY_MineLayer,			// 17
	ENEMY_Hunter,				// 18
	ENEMY_Legz,					// 19
	ENEMY_GuardBot,				// 20
	ENEMY_Boss_Metatank,		// 21
	ENEMY_Boss_BigGeek,			// 22
	ENEMY_Boss_Avatar,			// 23
	ENEMY_Boss_FleshMorph,		// 24
	ENEMY_Boss_Exogenon,		// 25
	ENEMY_Bike_Lokasenna,  		// 26
	ENEMY_Bike_Beard,       	// 27
	ENEMY_Bike_LAJay,    		// 28
	ENEMY_Bike_ExCop,     		// 29
	ENEMY_Bike_RexHardy,  		// 30
	ENEMY_Bike_Foetoid,    		// 31
	ENEMY_Bike_NimSoo,    		// 32
	ENEMY_Bike_Nutta,      		// 33
	ENEMY_Bike_Sceptre,    		// 34
	ENEMY_Bike_Jo,         		// 35
	ENEMY_Bike_CuvelClark,		// 36
	ENEMY_Bike_HK5,       		// 37
	ENEMY_Bike_Nubia,      		// 38
	ENEMY_Bike_Mofisto,    		// 39
	ENEMY_Bike_Cerbero,    		// 40
	ENEMY_Bike_Slick,      		// 41
	ENEMY_LEADER_Swarm,			// 42
	ENEMY_LEADER_Hunter,		// 43
	ENEMY_LEADER_Mekton,		// 44
	ENEMY_LEADER_SnubBot,		// 45
	ENEMY_LEADER_Legz,			// 46
	ENEMY_LEADER_Shade,			// 47
	ENEMY_LEADER_Supressor,		// 48
	ENEMY_LEADER_LeviTank,		// 49
	ENEMY_LEADER_LazerBot,		// 50
	ENEMY_LEADER_Max,			// 51
	ENEMY_LEADER_AirMoble,		// 52
	ENEMY_Fodder1,				// 53
	ENEMY_Boss_LittleGeek,		// 54
	ENEMY_Bike_FlyGirl,			// 55
	
	ENEMY_LAST,					//	This one should allways be last!!!

};

enum
{
	GUN_Beam,				// 0
	GUN_Duel,				// 1
	GUN_Pulse,				// 2	// this is used for gun...Sunb turret
	GUN_Missile,			// 3
	GUN_Snub,				// 4	// Snub Bot...
	GUN_Levi,				// 5
	GUN_Sec,				// 6
	GUN_Legz,				// 7
	GUN_Pulse_Turret,		// 8
	GUN_Mekton,				// 9
	GUN_MetaTank,			// 10
	GUN_MetaTankMain,		// 11
	GUN_Avatar,				// 12
	GUN_Avatar2,			// 13
};


enum {

	ENEMY_GENTYPE_Initialised,	// Initialised immediatly
	ENEMY_GENTYPE_Time,			// Initialised after time
	ENEMY_GENTYPE_Trigger,		// Initialised by trigger
};


enum {
	ENEMY_CONTROLTYPE_NONE,			// Not Controlled At all...
	ENEMY_CONTROLTYPE_FLY_PLAYER,	// Controlled By Player input..
	ENEMY_CONTROLTYPE_FLY_AI,		// Controlled By Ai input..
	ENEMY_CONTROLTYPE_CRAWL_PLAYER,	// Controlled By Player input..
	ENEMY_CONTROLTYPE_CRAWL_AI,		// Controlled By Ai input..
	ENEMY_CONTROLTYPE_TURRET_PLAYER,// Controlled By Player input..
	ENEMY_CONTROLTYPE_TURRET_AI,	// Controlled By Ai input..
	ENEMY_CONTROLTYPE_SPLINE,		// Controlled By Spline Movement...
	ENEMY_CONTROLTYPE_EXOGENON,		// Boss Specifics for Exogenon...
	ENEMY_CONTROLTYPE_LITTLEGEEK,	// Boss Specifics for LittleGeek...
	ENEMY_CONTROLTYPE_FLESHMORPH,	// Boss Specifics for LittleGeek...
};

enum {

	TURRETSEQ_Closed,			// Closed
	TURRETSEQ_Opening,			// Opening
	TURRETSEQ_Open,				// Open
	TURRETSEQ_Closing,			// Closing
	TURRETSEQ_Fire,				// Fire
};

enum
{
	TURRETCOMP_Base,
	TURRETCOMP_Gun,
};


enum
{
	EXOGENONSEQ_Move_Down,
	EXOGENONSEQ_Idle,
	EXOGENONSEQ_Fire,
	EXOGENONSEQ_Move_Up,
	EXOGENONSEQ_Stop_Up,
};
enum
{
	LITTLEGEEKSEQ_Stop,
	LITTLEGEEKSEQ_Open,
};
enum
{
	FLESHMORPHSEQ_Stop,
	FLESHMORPHSEQ_Pulsate,
	FLESHMORPHSEQ_Attack,
};




#define	ENEMY_STATUS_Nothing	0
#define	ENEMY_STATUS_Enable		1
#define	ENEMY_STATUS_Generating	2

#define	FIRSTENEMYMODEL			0 //MODEL_BeamTurret
#define	MAXENEMIES				256
#define	MAX_ENEMY_TYPES			56

#define	YES_STEALTH_MODE		TRUE
#define	NO_STEALTH_MODE			FALSE

#define	COMPONENTCOLLISION		TRUE
#define	SPHERECOLLISION			FALSE

/*===================================================================
	Prototypes
===================================================================*/
void InitEnemies( void );
ENEMY * FindFreeEnemy( void );



#ifdef DEBUG_ON
void KillUsedEnemyDebugOn( ENEMY * Object, char *in_file, int in_line );
#undef KillUsedEnemy
#define KillUsedEnemy( S )		KillUsedEnemyDebugOn( (S), __FILE__, __LINE__ )
#else
void KillUsedEnemy( ENEMY * Object );
#endif



BOOL PreLoadEnemies( void );
BOOL LoadEnemies( void );
ENEMY * InitOneEnemy( uint16 GenType, VECTOR * Pos, VECTOR * Dir, VECTOR * Up, uint16 Group, uint16 ModelNum, uint16 TriggerMod , uint16 EnemyType , uint32 Network , int32 PickupHeld , uint16 FormationLink , float GenerationDelay);
void ProcessEnemies( void );
void EnableEnemy( uint16 EnemyIndex );
void DisableEnemy( ENEMY * Enemy );
ENEMY * CheckHitEnemy( uint16 OwnerType, uint16 Owner, VECTOR * Pos, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point,
						VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType );
void SetCurAnimSeq( int16 Seq, OBJECT * Object );
BOOL GetLastCompDispMatrix( OBJECT * Object, MATRIX * Matrix, MATRIX * TMatrix, VECTOR * FirePos, int16 BaseIndex );
void SetTurretVector( OBJECT * Object, VECTOR * Vector, int16 BaseIndex );
BOOL Ship2EnemyCollide( uint16 i , VECTOR * Move_Off );
void SetupEnemyGroups( void );
void AddEnemyToGroup( ENEMY * Enemy, uint16 Group );
void RemoveEnemyFromGroup( ENEMY * Enemy, uint16 Group );
void MoveEnemyToGroup( ENEMY * Enemy, uint16 OldGroup, uint16 NewGroup );
BOOL ReadEnemyTxtFile( char *Filename );
void ObjectForceExternalOneOff( OBJECT * Object, VECTOR *force );
BYTE BodgePrimaryWeapon( BYTE Weapon, uint16 Pickup );
uint16 FindDuplicateModel( int8 * Filename, int16 NumModels );
BOOL DamageEnemy( ENEMY * Enemy , float Damage , VECTOR * ImpactPoint , VECTOR * Dir, float Speed ,uint16 Owner , uint16 OwnerType);
void ObjectRotateExternal( OBJECT * Object, VECTOR *Pos, VECTOR *point, VECTOR *dir, float force , float Radius );
void ReleaseAllEnemies( void );
void DoNmeDamagedEffects( ENEMY * Enemy );
void DispUntriggeredNMEs( void );
void AccellDecell(  float *  value ,  float  Decell );

BOOL Enemy_Load( FILE * fp );
BOOL Enemy_Save( FILE * fp );
BOOL Object_Save( FILE * fp , OBJECT * Obj );
BOOL Object_Load( FILE * fp , OBJECT * Obj );
BOOL Gun_Save( BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp );
BOOL Gun_Load( BYTE HowManyGuns , GUNOBJECT * GO , FILE * fp );

#endif


