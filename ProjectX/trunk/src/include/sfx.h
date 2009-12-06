#ifndef	SFX_INCLUDED
#define	SFX_INCLUDED

#include "enemies.h"

int NumDupCompoundBuffers;
#define MAX_COMPOUND_SFX 256 // max number of individual sfx that can be stored in a compound buffer
#define MAX_COMPOUND_BUFFERS 16	// max number of mixing channels
#define CompoundSfxBitDepth 16
#define CompoundSfxChannels 1
#define CompoundSfxFrequency 22050 // establish compound sample format...temporarily hard coded for now...
#define CompoundSfxGap 0.1f	// secs

typedef struct
{
	int		current_sfx;
	int		current_variant;
	int		compound_buffer_lookup_index;
	DWORD	start_time;
	DWORD	finish_time;
	float	distance;
	void*	timerID; // SDL_TimerID
	void*	buffer;
	int		SfxHolderIndex;
}COMPOUND_SFX_INFO;

COMPOUND_SFX_INFO CompoundSfxBuffer[MAX_COMPOUND_BUFFERS];


#define SFX_Biker			4	// biker speech
#define SFX_Title	64	// use when biker speech must play ( will cut off any existing speech )
#define SFX_InGame	256	// sfx must be loaded in game, regardless of whether or not SFX_Title is set

#define SPOT_SFX_TYPE_Normal 0
#define SPOT_SFX_TYPE_NoPan 1
#define SPOT_SFX_TYPE_Taunt 2

#define SFX_TYPE_Normal 1
#define SFX_TYPE_Panned 2
#define SFX_TYPE_Looping 3
#define SFX_TYPE_Taunt 4

#define GLOBAL_MAX_SFX ( 0.9F ) 

// sfx is -1 if free
// allocate one variable only - sfx loader can wait if not available

#define SFX_TYPE_Nothing 0
#define SFX_TYPE_Static 1
#define SFX_TYPE_Dynamic 2
#define SFX_TYPE_Compound 3

#define MAX_LEVELSPECIFIC_SFX 64

enum {
	SFX_KILLINGSPREE, //** voices
	SFX_RAMPAGE,
	SFX_DOMINATING,
	SFX_UNSTOPPABLE,
	SFX_WICKEDSICK,
	SFX_GODLIKE,
	SFX_Bubbles,  //**  bubbles rising in water
	SFX_BikeExplode,  //**  bike explodes, as player dies
	//SFX_BigMine,  //  quantum/pine mine drop
	SFX_OtherTeamScored,  //**  crowd boo,  other side scores a goal (in teamplay)
	//SFX_DamageZone,  //  player bike burn damage on contact with acid or lava
	SFX_MyTeamGotFlag,  //**  your side captures flag siren, (in teamplay)
	//SFX_ChaosShieldActive,  //  chaos shield active loop
	SFX_MyTeamScored,  //**  crowd cheer, your side scores a goal (in teamplay)
	//SFX_BikeCollideNoShield,  //  bike collides with another vehicle (when shields depleted) #1
	SFX_SelectStackOption,  //**  select click on selection stack ONLY
	SFX_VDUAmbience,  //  screen ambience for green side VDU screen
	SFX_PickupCrystal,  //**  pickup crystal
	//SFX_DebrisHitsWater,  //  enemy /hull debris hits water surface
	//SFX_EnemyDive,  //  defeated flying enemy plummets to the ground #1
	SFX_WaterDrip,  //**  water drip #1
	//SFX_ElectricArc,  //  level related, electrical arcing. Use INSTEAD of Laser2c
	SFX_ElectricSpark,  //**  electric sparking #1
	SFX_QuantumMineActive,  //  electrical throbbing - QUANTUM MINE ACTIVE
	SFX_EnemyMissile,  //  enemy missile launch
	SFX_EnemyPulsar,  //**  enemy pulsar fire
	SFX_EnemySpawn,  //**  enemy respawns/appears
	SFX_EnemySussGun,  //**  enemy fires suss round
	//SFX_EnemyTranspulse,  //  enemy fires transpulse
	SFX_EnemySpin,  //**  non-boss, flying enemy spins out of control #1
	SFX_BangBang,  //**  generic impact explosion
	SFX_Exp2,  //**  generic detonation type explosion
	SFX_Exp3,  //**  another detonation type, good for level-related/boss stuff
	SFX_Exp4,  //**  another impact explosion
	SFX_Exp5,  //**  non-boss,flying enemy crashes #1
	SFX_Exp6,  //**  non-boss,flying enemy crashes #1
	//SFX_FireImpact,  //**  generic fire impact damage from pyrolite hit
	SFX_MedFireBlast,  //  continuous fire travel ( NOT FIRE LAUNCH)
	//SFX_LightFireLaunch,  //  light fireball/fire launch
	SFX_HeavyFireLaunch,  //  heavier fireball/fire launch
	SFX_FleshmorphHitIndirect,  //  bike collides with fleshmorph / ineffectual impact of weapons on it
	SFX_FleshmorphDeathCry,  //**  fleshmorph defeated death cry
	SFX_FleshmorphTentacleHit,  //  fleshmorph hits bike with tentacle
	SFX_FleshmorphPainCry,  //**  fleshmorf pain cry - successful hit
	//SFX_FleshmorphTentacleAttack,  //  fleshmorph tentacle attack - tentacle shoots out from body
	SFX_PickupGeneralAmmo,  //  pickup general ammo / any secondary weapon
	//SFX_PickupDNA,  //  pickup DNA
	//SFX_PickupKey,  //  pickup any kind of key
	SFX_GoldBarPickup,  //**  pickup gold bars
	SFX_GravgonLoop,  //  gravgon field effect ambience
	//SFX_GravgonImpact,  //  gravgon misile impacts/detonates anywhere
	//SFX_GravgonLaunch,  //  gravgon missile launch
	SFX_Squish,  //**  guts impact #1
	SFX_HoloChange,  //**  biker holograph changes on VDU select screen
	SFX_BikeCollide,  //  bike (with shields depleted)bumps wall(only at high velocity)
	//SFX_HullHit,  //  generic biker/enemy hull damage (shields depleted) #1
	//SFX_BikeScrape,  //  bike scrapes walls/ceiling (shields depleted)
	SFX_Lockon,  //**  incoming beep (now shorter - should solve that prob.)
	SFX_Laser,  //**  laser fire DONT USE FOR LEVEL STUFF -use ELECBOLT
	SFX_LaserHum,  //  constant laser beam hum
	//SFX_TranspulseImpact,  //  laser/transpulse fire impacts wall
	//SFX_TranspulseWaterImpact,  //  laser/transpulse fire impacts water surface
	SFX_OtherTeamGotFlag,  //**  other side gets flag, warning siren (for teamplay)
	SFX_TitlePan,  //**  woosh from stack to/from side green screens ONLY
	SFX_MessageBeep,  //**  bleep accompanying a screen text message
	SFX_MFRLLaunch,  //**  MRFL single rocket launch
	SFX_DropMine,  //**  drop any other mine apart from quantum and pine
	//SFX_MissileLoop,  //  missile travelling loop. -25% pitch for titan, +25% for MFRL
	SFX_GeneralMineActive,  //  any mine (except quantum) active
	SFX_MoveStackOption,  //**  move cursor on selection stack ONLY
	SFX_MissileLaunch,  //**  launch mug/solaris missile
	SFX_GeneralMissileImpact,  //  missile (except gravgon and titan) hit wall/environment
	SFX_GeneralMissileWaterImpact,  //  missile (except gravgon and titan) hit water
	SFX_Error,  //**  negative/error/incorrect selection beep (use anywhere)
	SFX_NitroStart,  //  nitro start
	//SFX_NitroLoop,  //  nitro in use LOOP
	//SFX_NitroStop,  //  nitro stop/turn off
	SFX_Orbit,  //**  orbit pulsar fire
	SFX_PhotonTorpedo,  //**  enemy fires homing photon (slug pulsar?) - currently being used for boss laser
	SFX_PineMineTurn,  //  pine mine turning motion
	SFX_PickupGeneral,  //  pickup sound for any items not covered by specific pickups
	//SFX_PickupPowerPod,  //  pickup power pod
	SFX_PickupShield,  //  pickup shield
	SFX_PlayerGenerate,  //  player generate
	//SFX_TrojaxImpact,  //  pulsar/trojax fire impacts wall
	SFX_Pulsar,  //**  pulsar fire
	SFX_Pyroloop,  //  pyrolite fire loop
	SFX_Pyrostop,  //  stop pyrolite
	SFX_Pyrostart,  //  start pyrolite
	SFX_QuantumMineExplode,  //  quantum mine explosion
	SFX_Respawn,  //**  collectable items respawn
	SFX_RestartPointReached,  //**  restart point reached
	//SFX_scatterImpact,  //  scatter missile hit
	SFX_ScatterLaunch,  //  scatter launch
	SFX_VduClear,  //  green VDU screen change
	SFX_Secret,  //**  secret area found
	SFX_InGameMenuSelect,  //**  positive select beep for green screens
	SFX_ShipHit,  //**  shield hit (new one, with more of a punch to it)
	SFX_TitleAmb,  //  ambience for menu select room on front end
	SFX_ShieldKnock,  //  bike knocks against wall with shields on
	SFX_BigGeekLoop,  //  sonar ping loop for big geek. If you have time, vary pitch according to his depth/distance from you? (maybe I'm asking too much)
	SFX_StackDown,  //**  selection stack comes down on menu screen
	SFX_StakAnimates,  //**  stack movement (now boosted and brightened)
	SFX_Steam,  //**  continuous steam loop - volume varies depending on use
	SFX_DeCloaking,  //**  stealth mantle off
	SFX_Cloaking,  //**  stealth mantle on
	SFX_Submerge,  //**  bike enters water
	SFX_Surface,  //**  bike exits water
	//SFX_SussgunWaterImpact,  //  sussgun fire hits water
	SFX_PickupSussAmmo,  //  pickup sussgun ammo belt
	//SFX_SussgunCartridgeTinkle,  //  spent sussgun ammo cases tinkle #1
	//SFX_SussGun_Dry,  //  sussgun dry fire (is this still applicable?)
	SFX_Suss_Gun,  //**  sussgun fire
	SFX_SussgunImpact,  //  sussgun impacts enemy/bike causing damage #1
	SFX_Ric,  //**  sussgun fire richochet #1
	//SFX_TeleportLoop,  //  teleporter active ambience
	SFX_Teleport,  //**  teleport sound
	SFX_TileSelect,  //  tile selected on selection stack
	//SFX_SingleTileAnimate,  //  single tile slides round (if needed)
	SFX_TitanImpact,  //  titan explosion
	SFX_TitanLaunch,  //  launch titan missile
	SFX_Transpulse,  //**  transpulse fire
	SFX_Trojax,  //**  trojax fire
	SFX_Trojax_Charge,  //**  trojax power up
	SFX_UnderWaterAmb,  //**  underwater ambience
	SFX_UnderwaterOneShot,  //**  ominous 1 shot underwater pulse - play occassionally
	SFX_VduOff,  //  side green VDU screen switches off and retracts
	SFX_VduOn,  //  side green VDU screen flips round and switches on
	SFX_VidText,  //**  text type sound for green VDU screen displayed info
	SFX_WaterDrain,  //  water draining loop
	SFX_WaterFill,  //  water filling loop
	SFX_LoadWeapon,  //**  pickup new primary weapon
	SFX_WeaponSelect,  //  select different weapon
	SFX_ExtraLife,  //**  pickup resnic reanimator / earn extra life from collecting 10 gold

	/***********************************************
	 Bike computer
	************************************************/
	SFX_BIKECOMP_AM	, //	assassin missile
	SFX_BIKECOMP_AP	, //	picking up a weapon which is already present
	SFX_Select_BeamLaser,//	-	beam laser
	SFX_BIKECOMP_BN	, //	bad navigation
	SFX_BIKECOMP_CA	, //	camping
	SFX_BIKECOMP_CD	, //	chaff dispenser
	SFX_Select_Invul,//	-	chaos shield
	SFX_BIKECOMP_DY	, //	destroying yourself
	SFX_Select_Ammo,//	-	extra ammo
	SFX_BIKECOMP_EX	, //	extra, miscellaneous phrases
	SFX_BIKECOMP_FL	, //	flares
	SFX_BIKECOMP_GK	, //	good kill total
	SFX_BIKECOMP_GL	, //	general ammo low
	SFX_Select_GravgonMissile,//	-	gravgon missile
	SFX_Select_GoldenPowerPod,//	-	golden power pod
	SFX_BIKECOMP_HC	, //	hull critical ( #100 )
	SFX_Incoming,//	-	incoming
	SFX_BIKECOMP_IR	, //	IR goggles
	SFX_BIKECOMP_MA	, //	maximum ammo
	SFX_BIKECOMP_MK	, //	many kills in a short time period
	SFX_Select_MFRL,//	-	MRFL
	SFX_Select_MugMissile,//	-	mug
	SFX_BIKECOMP_NK	, //	no kills for a lengthy time period
	SFX_BIKECOMP_NL	, //	nitro low
	SFX_DontHaveThat,//	-	selecting a weapon which is not present
	SFX_Select_Nitro,//	-	nitro
	SFX_Orbital,//	-	orbit pulsar
	SFX_BIKECOMP_PG	, //	petro, gel
	SFX_BIKECOMP_PK	, //	poor kill total
	SFX_BIKECOMP_PL	, //	pyrolite fuel low
	SFX_Select_PineMine,//	-	pine mine
	SFX_Select_PowerPod,//	-	power pod
	SFX_BIKECOMP_PP	, //	plasma pack
	SFX_Select_PurgeMine,//	-	purge mine
	SFX_Select_Pulsar,//	-	pulsar
	SFX_Select_Pyrolite,//	-	pyrolite
	SFX_Select_QuantumMine,//	-	quantum mine
	SFX_ResnicReanimator,//	-	resnic reanimator
	SFX_Select_ScatterMissile,//	-	scatter missile
	SFX_BIKECOMP_SC	, //	shield critical
	SFX_Select_SussGun,//	-	suss-gun
	SFX_Select_Shield,//	-	shield
	SFX_Scattered,//	-	scatter missile impact
	SFX_BIKECOMP_SL	, //	suss gun ammo low
	SFX_BIKECOMP_SM	, //	smoke streamer
	SFX_BIKECOMP_SP	, //	spider mine
	SFX_Select_SolarisMissile,//	-	solaris heatseaker
	SFX_BIKECOMP_ST	, //	stealth mantle
	SFX_Select_TitanStarMissile,//	-	titan star missile
	SFX_Select_Transpulse,//	-	transpulse
	SFX_Select_Trojax,//	-	trojax

	/************************************************
	Biker
	*************************************************/
	SFX_BIKER_GP, //	-	general         
	SFX_BIKER_VP, //	-	victory         
	SFX_BIKER_LP, //	-	losing          
	SFX_BIKER_BW, //	-	big weapon gain 
	SFX_BIKER_LE, //	-	low energy      
	SFX_BIKER_TN, //	-	taunt           
	SFX_BikerPain,	//	-	pain
	SFX_Die,	//	-	death
	SFX_BIKER_EX, //	-	extra           

	/************************************************
	Level Specific	
	*************************************************/
	SFX_LEVELSPEC_Start, //	-	level specific
	SFX_LEVELSPEC_End = SFX_LEVELSPEC_Start + ( MAX_LEVELSPECIFIC_SFX - 1 ),

	MAX_SFX,
};

/*************************************
Biker speech lookup table identifiers
**************************************/
enum {
	SFX_BIKER_LOOKUP_GP, //	-	general        
	SFX_BIKER_LOOKUP_VP, //	-	victory        
	SFX_BIKER_LOOKUP_LP, //	-	losing         
	SFX_BIKER_LOOKUP_BW, //	-	big weapon gain
	SFX_BIKER_LOOKUP_LE, //	-	low energy     
	SFX_BIKER_LOOKUP_TN, //	-	taunt          
	SFX_BIKER_LOOKUP_PN, //	-	pain           
	SFX_BIKER_LOOKUP_DT, //	-	death          
	SFX_BIKER_LOOKUP_EX, //	-	extra          
};

/*************************************
Bike Computer lookup table identifiers
**************************************/
enum {
	SFX_BIKECOMP_LOOKUP_AM,  //	-	assassin missile                        
	SFX_BIKECOMP_LOOKUP_AP,  //	-	picking up a weapon which is already pre
	SFX_BIKECOMP_LOOKUP_BL,  //	-	beam laser                              
	SFX_BIKECOMP_LOOKUP_BN,  //	-	bad navigation                          
	SFX_BIKECOMP_LOOKUP_CA,  //	-	camping                                 
	SFX_BIKECOMP_LOOKUP_CD,  //	-	chaff dispenser                         
	SFX_BIKECOMP_LOOKUP_CS,  //	-	chaos shield                            
	SFX_BIKECOMP_LOOKUP_DY,  //	-	destroying yourself                     
	SFX_BIKECOMP_LOOKUP_EA,  //	-	extra ammo                              
	SFX_BIKECOMP_LOOKUP_EX,  //	-	extra,   miscellaneous phrases            
	SFX_BIKECOMP_LOOKUP_FL,  //	-	flares                                  
	SFX_BIKECOMP_LOOKUP_GK,  //	-	good kill total                         
	SFX_BIKECOMP_LOOKUP_GL,  //	-	general ammo low                        
	SFX_BIKECOMP_LOOKUP_GM,  //	-	gravgon missile                         
	SFX_BIKECOMP_LOOKUP_GP,  //	-	golden power pod                        
	SFX_BIKECOMP_LOOKUP_HC,  //	-	hull critical                           
	SFX_BIKECOMP_LOOKUP_IN,  //	-	incoming                                
	SFX_BIKECOMP_LOOKUP_IR,  //	-	IR goggles                              
	SFX_BIKECOMP_LOOKUP_MA,  //	-	maximum ammo                            
	SFX_BIKECOMP_LOOKUP_MK,  //	-	many kills in a short time period       
	SFX_BIKECOMP_LOOKUP_MR,  //	-	MRFL                                    
	SFX_BIKECOMP_LOOKUP_MU,  //	-	mug                                     
	SFX_BIKECOMP_LOOKUP_NK,  //	-	no kills for a lengthy time period      
	SFX_BIKECOMP_LOOKUP_NL,  //	-	nitro low                               
	SFX_BIKECOMP_LOOKUP_NP,  //	-	selecting a weapon which is not present 
	SFX_BIKECOMP_LOOKUP_NT,  //	-	nitro                                   
	SFX_BIKECOMP_LOOKUP_OP,  //	-	orbit pulsar                            
	SFX_BIKECOMP_LOOKUP_PG,  //	-	petro gel                               
	SFX_BIKECOMP_LOOKUP_PK,  //	-	poor kill total                         
	SFX_BIKECOMP_LOOKUP_PL,  //	-	pyrolite fuel low                       
	SFX_BIKECOMP_LOOKUP_PM,  //	-	pine mine                               
	SFX_BIKECOMP_LOOKUP_PO,  //	-	power pod                               
	SFX_BIKECOMP_LOOKUP_PP,  //	-	plasma pack                             
	SFX_BIKECOMP_LOOKUP_PR,  //	-	purge mine                              
	SFX_BIKECOMP_LOOKUP_PS,  //	-	pulsar                                  
	SFX_BIKECOMP_LOOKUP_PY,  //	-	pyrolite                                
	SFX_BIKECOMP_LOOKUP_QM,  //	-	quantum mine                            
	SFX_BIKECOMP_LOOKUP_RR,  //	-	resnic reanimator                       
	SFX_BIKECOMP_LOOKUP_SA,  //	-	scatter missile                         
	SFX_BIKECOMP_LOOKUP_SC,  //	-	shield critical                         
	SFX_BIKECOMP_LOOKUP_SG,  //	-	suss-gun                                
	SFX_BIKECOMP_LOOKUP_SH,  //	-	shield                                  
	SFX_BIKECOMP_LOOKUP_SI,  //	-	scatter missile impact                  
	SFX_BIKECOMP_LOOKUP_SL,  //	-	suss gun ammo low                       
	SFX_BIKECOMP_LOOKUP_SM,  //	-	smoke streamer                          
	SFX_BIKECOMP_LOOKUP_SO,  //	-	spider mine                             
	SFX_BIKECOMP_LOOKUP_SP,  //	-	solaris heatseaker                      
	SFX_BIKECOMP_LOOKUP_ST,  //	-	stealth mantle                          
	SFX_BIKECOMP_LOOKUP_TI,  //	-	titan star missile                      
	SFX_BIKECOMP_LOOKUP_TR,  //	-	transpulse                              
	SFX_BIKECOMP_LOOKUP_TX,  //	-	trojax  
};

#define SFX_BikeComp		2	// bike computer speech 
#define SFX_BikeCompNoOveride	512
#define SFX_BikerSpeechOveride	32	// use when biker speech must play ( will cut off any existing speech )

#define SFX_Dynamic			1	// sound is loaded up as required
#define SFX_Looping			16	// level specific sfx
#define SFX_LevelSpec		8	// level specific sfx

typedef struct SFXNAME{
	char		*Name;			// Name of the Sfx...
	int			Flags;
	int			Priority;			// for compound sfx
	int			SfxLookup;			// for biker / computer speech
}SFXNAME;

typedef struct
{
	int SfxNum;
	int Variant;
	DWORD StartPos;
	unsigned int Length;
	DWORD Bytes;
} TEMPSFXINFO;

TEMPSFXINFO TempSfxInfo[MAX_COMPOUND_SFX];

typedef struct{
	uint16 Num_Variants;
	uint16 SndObjIndex;
	BOOL Requested;
} SNDLOOKUP;

SNDLOOKUP SndLookup[ MAX_SFX ];

BOOL CompoundSfxAllocated[MAX_SFX];
#define IS_COMPOUND( flags ) ( (!(flags & SFX_Looping)) && (!(flags & SFX_Dynamic)))

#define MAXBIKECOMPTYPES 5

#define DESTROYSOUND_All 0

// functions
void ProcessSoundRoutines (void * pParm);
BOOL InitializeSound( int flags );
void    DestroySound( int flags );
uint32	PlaySfx( int16 Sfx, float Dist );
uint32	PlayPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq );
uint32 ForcePlayPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq );
BOOL SetPosVelDir_Listner( VECTOR * Pos , VECTOR * Velocity , MATRIX * Mat );
void CheckSBufferList( void );
void FreeSBufferList( void );
int InitLoopingSfx( int16 Sfx, int variant, uint16 *Group, VECTOR *SfxPos, float Freq, float Volume, int type, int SfxHolderIndex, uint16 Effects, uint32 uid );
void StopLoopingSfx( int index );
void ModifyLoopingSfx( uint32 uid, float Freq, float Volume );
void ProcessLoopingSfx( void );
void StopCompoundSfx( void );
int16 ReturnSFXIndex( char *file );
uint32 PlaySpotSfx(int16 Sfx, uint16 *Group , VECTOR * SfxPos, float Freq, float Vol, uint16 Effects );
BOOL StopSfx( uint32 uid );
uint32 PlaySfxWithTrigger( int16 Sfx, int16 TriggeredSfx );
uint32 PlayFixedSpotSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq, float Vol, uint16 Effects );
void KillCompoundSfxBuffer( int buffer );
FILE *SaveAllSfx( FILE *fp );
FILE *LoadAllSfx( FILE *fp );
void PauseAllSfx( void );
void ReTriggerSfx( void );
void GetFullSfxPath( char *fullpath, int sfxnum, int variant, int total_variants );
BOOL RestoreSfxData( uint32 id, VECTOR *pos, uint16 *group );
void RequestSfx( int16 sfxnum );
void RequestMainSfx( void );
void RequestTitleSfx( void );
void PreInitSfx( void );
void UpdateSfxForBiker( uint16 biker );
void UpdateSfxForBikeComputer( uint16 bikecomp );
void PlaySpecificBikerSpeech( int16 sfx, uint16 Group, VECTOR *SfxPos, float Freq, int biker, char variant, BOOL update );
void PlayRecievedSpeechTaunt( BYTE player, char variant );
BOOL UpdateTaunt( uint32 uid, uint16 Group, VECTOR *SfxPos );
void ProcessTaunt( void );
void StopTaunt( void );
void StopEnemyBikerTaunt( ENEMY *Enemy );
void PlayEnemyBikerTaunt( ENEMY *Enemy );
void ProcessEnemyBikerTaunt( void );
uint32 PlayPannedSfxWithVolModify(int16 Sfx, uint16 Group, VECTOR * SfxPos, float Freq, float Vol );

#endif
