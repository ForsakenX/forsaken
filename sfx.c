#include "main.h"
#include "render.h"
#include "sound.h"
#include "sfx.h"

#ifndef SOUND_SUPPORT

float LastDistance[MAX_SFX];
bool  bSoundEnabled = false;
char  CurrentTauntVariant;
bool  NoSFX = true;

bool InitializeSound( int flags ){return 1;}

void PlayEnemyBikerTaunt( ENEMY *Enemy ){}
u_int32_t PlaySfx( int16_t Sfx, float Vol ){return 0;}
void PlayRecievedSpeechTaunt( BYTE player, char variant ){}
u_int32_t PlaySfxWithTrigger( int16_t Sfx, int16_t TriggeredSfx ){return 0;}
u_int32_t PlayPannedSfx(int16_t Sfx, u_int16_t Group, VECTOR * SfxPos, float Freq ){return 0;}
u_int32_t ForcePlayPannedSfx(int16_t Sfx, u_int16_t Group , VECTOR * SfxPos, float Freq ){return 0;}
u_int32_t PlaySpotSfx(int16_t Sfx, u_int16_t *Group , VECTOR * SfxPos, float Freq, float Vol, u_int16_t Effects ){return 0;}
u_int32_t PlayFixedSpotSfx(int16_t Sfx, u_int16_t Group , VECTOR * SfxPos, float Freq, float Vol, u_int16_t Effects ){return 0;}
u_int32_t PlayPannedSfxWithVolModify(int16_t Sfx, u_int16_t Group, VECTOR * SfxPos, float Freq, float Vol ){return 0;}

void StopEnemyBikerTaunt( ENEMY *Enemy ){}
bool StopSfx( u_int32_t uid ){return 0;}
void StopTaunt( void ){}

void DestroySound( int flags ){}

void PauseAllSfx( void ){}

void UpdateSfxForBiker( u_int16_t biker ){}
void UpdateSfxForBikeComputer( u_int16_t bikecomp ){}

void ProcessLoopingSfx( void ){}
void ProcessEnemyBikerTaunt( void ){}

bool RestoreSfxData( u_int32_t id, VECTOR *pos, u_int16_t *group ){return 0;}
int16_t ReturnSFXIndex( char *file ){return -1;}
void ReTriggerSfx( void ){}

void ModifyLoopingSfx( u_int32_t uid, float Freq, float Volume ){}

void CheckSBufferList( void ){}

bool SetPosVelDir_Listner( VECTOR * Pos , VECTOR * Velocity , MATRIX * Mat ){return 0;}
void SetSoundLevels( int *dummy ){}

float ReturnDistanceVolumeVector( VECTOR *sfxpos, u_int16_t sfxgroup, VECTOR *listenerpos, u_int16_t listenergroup, long *vol, VECTOR *sfxvector ){return 0;}

FILE *SaveAllSfx( FILE *fp ){return fp;}
FILE *LoadAllSfx( FILE *fp ){return fp;}

#else

#include "main.h"
#include "quat.h"
#include "compobjects.h"
#include "object.h"
#include "networking.h"
#include "object.h"
#include "mload.h"
#include "title.h"
#include "text.h"
#include "controls.h"
#include "ships.h"
#include "config.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

extern render_info_t render_info;

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

typedef struct SFXNAME{
	char		*Name;			// Name of the Sfx...
	int			Flags;
	int			Priority;			// for compound sfx
	int			SfxLookup;			// for biker / computer speech
}SFXNAME;

typedef struct{
	u_int16_t Num_Variants;
	bool Requested;
} SNDLOOKUP;

SNDLOOKUP SndLookup[ MAX_SFX ];

/****************************************
globals
*****************************************/
ENEMY *EnemyTaunter;
BYTE Taunter = 0xFF;
u_int32_t TauntID = 0;
bool TauntUpdatable = false;
//float TauntDist = 0.0F;
char CurrentTauntVariant;
#define MAX_ANY_SFX 256 // openal soft max is 256 so this feels good

SFXNAME	Sfx_Filenames[MAX_SFX] =
{
	/************************************************
	Generic
	*************************************************/

	{ "killingspree", 0, 0, -1 },  // voices
	{ "rampage", 0, 0, -1 },  
	{ "dominating", 0, 0, -1 },  
	{ "unstoppable", 0, 0, -1 },  
	{ "wickedsick", 0, 0, -1 },  
	{ "godlike", 0, 0, -1 }, 
	{ "doublekill", 0, 0, -1 }, 
	{ "firstblood", 0, 0, -1 }, 
 	{ "combowhore", 0, 0, -1 }, 
	{ "hattrick", 0, 0, -1 }, 
	{ "holyshit", 0, 0, -1 }, 
	{ "humiliation", 0, 0, -1 }, 
	{ "impressive", 0, 0, -1 }, 
	{ "ludicrouskill", 0, 0, -1 }, 
	{ "megakill", 0, 0, -1 }, 
	{ "monsterkill", 0, 0, -1 }, 
	{ "multikill", 0, 0, -1 }, 
	{ "play", 0, 0, -1 }, 
	{ "teamkiller", 0, 0, -1 }, 
	{ "triplekill", 0, 0, -1 }, 
	{ "ultrakill", 0, 0, -1 }, 
	{ "airbubb1", 0, 0, -1 },  // bubbles rising in water
	{ "bikexpl", 0, 0, -1 },  // bike explodes, as player dies
	//{ "bminelay", 0, 0, -1 },  // quantum/pine mine drop
	{ "boo", 0, 0, -1 },  // crowd boo,  other side scores a goal (in teamplay)
	//{ "burnhiss", 0, 0, -1 },  // player bike burn damage on contact with acid or lava
	{ "captflag", 0, 0, -1 },  // your side captures flag siren, (in teamplay)
	//{ "chaosact", SFX_Looping, 0, -1 },  // chaos shield active loop
	{ "cheer", 0, 0, -1 },  // crowd cheer, your side scores a goal (in teamplay)
	//{ "collide", 0, 0, -1 },  // bike collides with another vehicle (when shields depleted) #1
	{ "compsel", SFX_Title, 0, -1 },  // select click on selection stack ONLY
	{ "compwrk", SFX_Looping | SFX_Title | SFX_InGame, 0, -1 },  // screen ambience for green side VDU screen
	{ "cryspkup", 0, 0, -1 },  // pickup crystal
	//{ "dbhitwat", 0, 0, -1 },  // enemy /hull debris hits water surface
	//{ "dive", 0, 0, -1 },  // defeated flying enemy plummets to the ground #1
	{ "dripwat", 0, 0, -1 },  // water drip #1
	//{ "elecbolt", 0, 0, -1 },  // level related, electrical arcing. Use INSTEAD of Laser2c
	{ "elecspk", 0, 0, -1 },  // electric sparking #1
	{ "electhrb", SFX_Looping, 0, -1 },  // electrical throbbing - QUANTUM MINE ACTIVE
	{ "enemisln", 0, 0, -1 },  // enemy missile launch
	{ "enempuls", 0, 0, -1 },  // enemy pulsar fire
	{ "enemspwn", 0, 0, -1 },  // enemy respawns/appears
	{ "enemsuss", 0, 0, -1 },  // enemy fires suss round
	//{ "enemtrnp", 0, 0, -1 },  // enemy fires transpulse
	{ "enmspin", 0, 0, -1 },  // non-boss, flying enemy spins out of control #1
	{ "exp1", 0, 0, -1 },  // generic impact explosion
	{ "exp2", 0, 0, -1 },  // generic detonation type explosion
	{ "exp3", 0, 0, -1 },  // another detonation type, good for level-related/boss stuff
	{ "exp4", 0, 0, -1 },  // another impact explosion
	{ "exp5", 0, 0, -1 },  // non-boss,flying enemy crashes #1
	{ "exp6", 0, 0, -1 },  // non-boss,flying enemy crashes #1
	//{ "fireimpc", 0, 0, -1 },  // generic fire impact damage from pyrolite hit
	{ "firembls", SFX_Looping, 0, -1 },  // continuous fire travel ( NOT FIRE LAUNCH)
	//{ "firesh1", 0, 0, -1 },  // light fireball/fire launch
	{ "firesh2", 0, 0, -1 },  // heavier fireball/fire launch
	{ "fmorfchh", 0, 0, -1 },  // bike collides with fleshmorph / ineffectual impact of weapons on it
	{ "fmorfdie", 0, 0, -1 },  // fleshmorph defeated death cry
	{ "fmorfhit", 0, 0, -1 },  // fleshmorph hits bike with tentacle
	{ "fmorfpai", 0, 0, -1 },  // fleshmorf pain cry - successful hit
	//{ "fmorftat", 0, 0, -1 },  // fleshmorph tentacle attack - tentacle shoots out from body
	{ "genrammo", 0, 0, -1 },  // pickup general ammo / any secondary weapon
	//{ "getdna", 0, 0, -1 },  // pickup DNA
	//{ "getkey", 0, 0, -1 },  // pickup any kind of key
	{ "goldpkup", 0, 0, -1 },  // pickup gold bars
	{ "gravgfld", SFX_Looping, 0, -1 },  // gravgon field effect ambience
	//{ "gravghit", 0, 0, -1 },  // gravgon misile impacts/detonates anywhere
	//{ "gravgln", 0, 0, -1 },  // gravgon missile launch
	{ "guts", 0, 0, -1 },  // guts impact #1
	{ "holochng", SFX_Title, 0, -1 },  // biker holograph changes on VDU select screen
	{ "hullbump", 0, 0, -1 },  // bike (with shields depleted)bumps wall(only at high velocity)
	//{ "hullhit", 0, 0, -1 },  // generic biker/enemy hull damage (shields depleted) #1
	//{ "hullscrp", 0, 0, -1 },  // bike scrapes walls/ceiling (shields depleted)
	{ "incoming", 0, 0, -1 },  // incoming beep (now shorter - should solve that prob.)
	{ "laser2c", 0, 0, -1 },  // laser fire DONT USE FOR LEVEL STUFF -use ELECBOLT
	{ "laserbm", SFX_Looping, 0, -1 },  // constant laser beam hum
	//{ "lashitwl", 0, 0, -1 },  // laser/transpulse fire impacts wall
	//{ "lashitwt", 0, 0, -1 },  // laser/transpulse fire impacts water surface
	{ "loseflag", 0, 0, -1 },  // other side gets flag, warning siren (for teamplay)
	{ "menuwsh", SFX_Title, 0, -1 },  // woosh from stack to/from side green screens ONLY
	{ "message", 0, 0, -1 },  // bleep accompanying a screen text message
	{ "mfrln", 0, 0, -1 },  // MRFL single rocket launch
	{ "minelay", 0, 0, -1 },  // drop any other mine apart from quantum and pine
	//{ "misltrav", SFX_Looping, 0, -1 },  // missile travelling loop. -25% pitch for titan, +25% for MFRL
	{ "mnacthum", SFX_Looping, 0, -1 },  // any mine (except quantum) active
	{ "movesel", SFX_Title, 0, -1 },  // move cursor on selection stack ONLY
	{ "mslaun2", 0, 0, -1 },  // launch mug/solaris missile
	{ "mslhitwl", 0, 0, -1 },  // missile (except gravgon and titan) hit wall/environment
	{ "mslhitwt", 0, 0, -1 },  // missile (except gravgon and titan) hit water
	{ "negative", SFX_Title | SFX_InGame, 0, -1 },  // negative/error/incorrect selection beep (use anywhere)
	{ "nitrogo", 0, 0, -1 },  // nitro start
	//{ "nitromov", SFX_Looping, 0, -1 },  // nitro in use LOOP
	//{ "nitrstop", 0, 0, -1 },  // nitro stop/turn off
	{ "orbitpls", 0, 0, -1 },  // orbit pulsar fire
	{ "photon2", 0, 0, -1 },  // enemy fires homing photon (slug pulsar?)
	{ "pinemove", SFX_Looping, 0, -1 },  // pine mine turning motion
	{ "pkupmisc", 0, 0, -1 },  // pickup sound for any items not covered by specific pickups
	//{ "pkuppowp", 0, 0, -1 },  // pickup power pod
	{ "pkupshld", 0, 0, -1 },  // pickup shield
	{ "playergn", 0, 0, -1 },  // player generate
	//{ "plshitwl", 0, 0, -1 },  // pulsar/trojax fire impacts wall
	{ "pulsfire", 0, 0, -1 },  // pulsar fire
//	{ "pyroloop", SFX_Looping, 0, -1 },  // pyrolite fire loop
	{ "pyroloop", 0, 0, -1 },  // pyrolite fire loop
	{ "pyrostop", 0, 0, -1 },  // stop pyrolite
	{ "pyrostrt", 0, 0, -1 },  // start pyrolite
	{ "quantexp", 0, 0, -1 },  // quantum mine explosion
	{ "respawn", 0, 0, -1 },  // collectable items respawn
	{ "restartp", 0, 0, -1 },  // restart point reached
	//{ "scathit", 0, 0, -1 },  // scatter missile hit
	{ "scattln", 0, 0, -1 },  // scatter launch
	{ "scrchang", SFX_Title, 0, -1 },  // green VDU screen change
	{ "secret", SFX_Title | SFX_InGame, 0, -1 },  // secret area found
	{ "select", SFX_Title, 0, -1 },  // positive select beep for green screens
	{ "shieldht", 0, 0, -1 },  // shield hit (new one, with more of a punch to it)
	{ "shipamb", SFX_Looping | SFX_Title, 0, -1 },  // ambience for menu select room on front end
	{ "shldknck", 0, 0, -1 },  // bike knocks against wall with shields on
	{ "sonar", SFX_Looping, 0, -1 },  // sonar ping loop for big geek. If you have time, vary pitch according to his depth/distance from you? (maybe I'm asking too much)
	{ "stakdown", SFX_Title, 0, -1 },  // selection stack comes down on menu screen
	{ "stakmove", SFX_Title, 0, -1 },  // stack movement (now boosted and brightened)
	{ "steammcn", SFX_Looping, 0, -1 },  // continuous steam loop - volume varies depending on use
	{ "stmantof", 0, 0, -1 },  // stealth mantle off
	{ "stmanton", 0, 0, -1 },  // stealth mantle on
	{ "submerge", 0, 0, -1 },  // bike enters water
	{ "surface", 0, 0, -1 },  // bike exits water
	//{ "sushitwt", 0, 0, -1 },  // sussgun fire hits water
	{ "sussammo", 0, 0, -1 },  // pickup sussgun ammo belt
	//{ "susscas", 0, 0, -1 },  // spent sussgun ammo cases tinkle #1
	//{ "sussdry2", 0, 0, -1 },  // sussgun dry fire (is this still applicable?)
	{ "sussgnf3", 0, 0, -1 },  // sussgun fire
	{ "sussimp", 0, 0, -1 },  // sussgun impacts enemy/bike causing damage #1
	{ "sussric", 0, 0, -1 },  // sussgun fire richochet #1
	//{ "telepact", SFX_Looping, 0, -1 },  // teleporter active ambience
	{ "teleport", 0, 0, -1 },  // teleport sound
	{ "tilesel", 0, 0, -1 },  // tile selected on selection stack
	//{ "tileslid", SFX_Title, 0, -1 },  // single tile slides round (if needed)
	{ "titanexp", 0, 0, -1 },  // titan explosion
	{ "titanln", 0, 0, -1 },  // launch titan missile
	{ "tpfire2", 0, 0, -1 },  // transpulse fire
	{ "trojfire", 0, 0, -1 },  // trojax fire
	{ "trojpup", 0, 0, -1 },  // trojax power up
	{ "unwatamb", SFX_Looping, 0, -1 },  // underwater ambience
	{ "uwtsnd01", 0, 0, -1 },  // ominous 1 shot underwater pulse - play occassionally
	{ "vdu_off", SFX_Title, 0, -1 },  // side green VDU screen switches off and retracts
	{ "vdu_on", SFX_Title, 0, -1 },  // side green VDU screen flips round and switches on
	{ "vidtext", SFX_Title | SFX_InGame, 0, -1 },  // text type sound for green VDU screen displayed info
	{ "watrdrai", SFX_Looping, 0, -1 },  // water draining loop
	{ "watrfill", SFX_Looping, 0, -1 },  // water filling loop
	{ "weapload", 0, 0, -1 },  // pickup new primary weapon
	{ "weapslct", 0, 0, -1 },  // select different weapon
	{ "xtralife", 0, 0, -1 },  // pickup resnic reanimator / earn extra life from collecting 10 gold

	/***********************************************
	 Bike computer
	 ************************************************/
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_AM  }, //	-	assassin missile
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_AP  }, //	-	picking up a weapon which is already present
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_BL  }, //	-	beam laser
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_BN  }, //	-	bad navigation
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CA  }, //	-	camping
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CD  }, //	-	chaff dispenser
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CS  }, //	-	chaos shield
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_DY  }, //	-	destroying yourself
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_EA  }, //	-	extra ammo
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_EX  }, //	-	extra, miscellaneous phrases
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_FL  }, //	-	flares
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GK  }, //	-	good kill total
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GL  }, //	-	general ammo low
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GM  }, //	-	gravgon missile
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GP  }, //	-	golden power pod
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_HC  }, //	-	hull critical
	{ NULL , SFX_BikeComp | SFX_BikeCompNoOveride, 0, SFX_BIKECOMP_LOOKUP_IN  }, //	-	incoming
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_IR  }, //	-	IR goggles
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MA  }, //	-	maximum ammo
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MK  }, //	-	many kills in a short time period
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MR  }, //	-	MRFL
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MU  }, //	-	mug
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NK  }, //	-	no kills for a lengthy time period
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NL  }, //	-	nitro low
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NP  }, //	-	selecting a weapon which is not present
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NT  }, //	-	nitro
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_OP  }, //	-	orbit pulsar
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PG  }, //	-	petro gel
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PK  }, //	-	poor kill total
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PL  }, //	-	pyrolite fuel low
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PM  }, //	-	pine mine
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PO  }, //	-	power pod
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PP  }, //	-	plasma pack
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PR  }, //	-	purge mine
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PS  }, //	-	pulsar
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PY  }, //	-	pyrolite
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_QM  }, //	-	quantum mine
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_RR  }, //	-	resnic reanimator
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SA  }, //	-	scatter missile
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SC  }, //	-	shield critical
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SG  }, //	-	suss-gun
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SH  }, //	-	shield
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SI  }, //	-	scatter missile impact
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SL  }, //	-	suss gun ammo low
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SM  }, //	-	smoke streamer
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SO  }, //	-	spider mine
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SP  }, //	-	solaris heatseaker
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_ST  }, //	-	stealth mantle
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TI  }, //	-	titan star missile
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TR  }, //	-	transpulse
	{ NULL , SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TX  }, //	-	trojax

	/************************************************
	Biker
	*************************************************/
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_GP  }, //	-	general 
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_VP  }, //	-	victory
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_LP  }, //	-	losing
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_BW  }, //	-	big weapon gain
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_LE  }, //	-	low energy
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_TN  }, //	-	taunt
	{ NULL, SFX_Biker, 0, SFX_BIKER_LOOKUP_PN  }, //	-	pain
	{ NULL, SFX_Biker | SFX_BikerSpeechOveride, 0, SFX_BIKER_LOOKUP_DT  }, //	-	death
	{ NULL, SFX_Biker | SFX_Title, 0, SFX_BIKER_LOOKUP_EX  }, //	-	extra 
  
	{ NULL, SFX_LevelSpec, 0, 0 }, // first level spec sfx

};

typedef struct
{
        bool Used;
        u_int32_t  UniqueID;
        int SndObjIndex;
        int SfxFlags;
        int16_t TriggerSfx;
        bool OnPause;
        float PauseValue;
	int SfxBufferIndex;
	sound_source_t * source;
} SFX_HOLDER;

SFX_HOLDER	SfxHolder[ MAX_ANY_SFX ];
bool		NoSFX = false;
float		GlobalSoundAttenuation = 0.8F;

bool bSoundEnabled = false;
u_int32_t CurrentBikerSpeech = 0;
u_int32_t CurrentBikeCompSpeech = 0;

float	LastDistance[MAX_SFX];

int CurrentLevel = 16;

/////
// sound buffer storage
// reusable buffers for sources
/////

sound_buffer_t * sound_buffers[MAX_SFX];

sound_buffer_t * sound_buffer_create( char *file, int sfxnum )
{
	sound_buffer_t * buffer;

	if( sound_buffers[ sfxnum ] )
		return sound_buffers[ sfxnum ];
	
	buffer = sound_load( file );

	if ( ! buffer )
  	{
		SndLookup[ sfxnum ].Num_Variants = 0;
		DebugPrintf("failed to load sfxnum %d from %s\n",sfxnum,file);
		return NULL;
	}
	
	sound_buffers[ sfxnum ] = buffer;

	return buffer;
}

// TODO 
//   openal will not really delete the buffer until you delete all the sources...
//   make a table of sources and tell everything else to point to id's in that table ?
//   that way we can simply iterate one list... 

// use this if you want sound to reinit
void sound_buffer_delete( int sfx )
{
	sound_release_buffer(sound_buffers[sfx]);
	sound_buffers[sfx] = NULL;
}

sound_source_t * sound_source_create( char *file, int sfxnum )
{
	sound_buffer_t * buffer;
	if(!file) return NULL;
	buffer = sound_buffer_create(file,sfxnum);
	if(!buffer)
	{
		DebugPrintf("sound: failed to create sound buffer %s for sfx %d\n",
			file, sfxnum);
		return NULL;
	}
	return sound_source(buffer);
}

////////
// End
////////

typedef struct _SPOT_SFX_LIST
{
	bool     used;       // is sfx in use?
	int16_t    sfxindex;   // sfx num, from enum list
	int      variant;    // sfx num, from enum list
	int      flags;
	VECTOR * pos;        // current sfx position vector
	VECTOR   fixedpos;
	int      type;       // fixed or variable group?
	u_int16_t * group;      // current sfx group num
	u_int16_t   fixedgroup; // current sfx group num
	float    freq;       // frequency ( 0 for original frequency )
	float    vol;        // vol ( 0 = zero volume, 1 = full volume )
	void   * source;     // source address
	float    distance;
	int      SfxHolderIndex;
	u_int16_t   Effects;
	u_int32_t   uid;
} SPOT_SFX_LIST;

#define MAX_LOOPING_SFX 64
SPOT_SFX_LIST SpotSfxList[ MAX_LOOPING_SFX ];

int sfxref = 0;
int dupbufref = 0;
char TauntPath[ 128 ];

bool BikerSpeechPlaying = false;

u_int32_t SfxUniqueID = 1;

#define MAX_SFX_VARIANTS 16
char *SfxFullPath[ MAX_SFX ][ MAX_SFX_VARIANTS ];

/****************************************
Externals
*****************************************/
extern 	ENEMY Enemies[];
extern SLIDER BikeCompSpeechSlider;
extern SLIDER BikerSpeechSlider;
extern USERCONFIG	*player_config;
extern VECTOR	SlideUp;
extern VECTOR	Forward;
extern float	SoundInfo[MAXGROUPS][MAXGROUPS];
extern SLIDER  SfxSlider; 
extern USERCONFIG *player_config;

extern float framelag;	
extern LEVEL_LOOKUP LevelLookup[];
extern LIST	BikeList;
extern LIST BikeComputerList;

/****************************************
Fn Prototypes
*****************************************/
void InitSfxHolders( void );

typedef struct
{
	char file[128];
	int	flags;
}LEVELSPEC_SFX_FILES;
//LEVELSPEC_SFX_FILES LevelSpecificEffects[ MAX_LEVELSPECIFIC_SFX ];

#define SFX_End	128	// end of sfx batch
LEVELSPEC_SFX_FILES LevelSpecificSfxLookup[ 256 ] = {
	{ "acidpool", SFX_Looping },  // bubbling ambience for slime/acid pools (are there any?)
	{ "aircond1", SFX_Looping },  // normal air conditioning ventilator air rush
	{ "aircond2", SFX_Looping },  // larger air conditioning ventilator air rush
	{ "aircond3", SFX_Looping },  // smaller & faster air conditioning
	{ "airmoeng", SFX_Looping },  // engine loop for airmobile enemy
	{ "airpress", 0 },  // piston sigh for switch changing states
	{ "ammodump", SFX_Looping },  // factory type ambience for ammodump emplacement
	{ "areahum1", SFX_Looping },  // low heavy mains hum tone -  use for big, powered rooms only.
	{ "avatreng", SFX_Looping },  // engine loop for final avatar boss
	{ "bikeengn", SFX_Looping },  // hunter enemy engine ALSO USE THIS FOR MAIN BIKENG
	{ "boathorn", 0 },  // far off foghorn sound (one off -as above)
	{ "boteng", SFX_Looping },  // caterpillar-tracked, 'bot' enemy engine loop
	{ "compamb1", SFX_Looping },  // computer console ambience. Numerous bleeping at various pitches
	{ "compamb2", SFX_Looping },  // computer console ambience. Many rapid beeps, hum in bkgrnd
	{ "compamb3", SFX_Looping },  // computer wall machinery ambience
	{ "compamb4", SFX_Looping },  // computer console ambience activity
	{ "cranemov", SFX_Looping },  // crane movement
	{ "crysthum", SFX_Looping },  // ambience for crystal structures
	{ "crysthum", SFX_Looping },  // ambience for crystal structures
	{ "elecexpl", 0 },  // electrical explosion
	{ "eled1mov", SFX_Looping },  // light motorized metallic door sliding
	{ "eled1stp", 0 },  // light motorized metallic door stop
	{ "eled1str", 0 },  // light motorized metallic door starts
	{ "eled2mov", SFX_Looping },  // heavy motorized door sliding
	{ "eled2stp", 0 },  // heavy motorized door stop
	{ "eled2str", 0 },  // heavy motorized door starts
	{ "eled3mov", SFX_Looping },  // light motorized door 2 sliding
	{ "eled3stp", 0 },  // light motorized door 2 stop
	{ "eled3str", 0 },  // light motorized door 2 str
	{ "eled4mov", SFX_Looping },  // heavy motorized door 2 sliding
	{ "eled4stp", 0 },  // heavy motorized door 2 stop
	{ "eled4str", 0 },  // heavy motorized door 2 starts
	{ "emergency", SFX_Looping },  // 'emergency' warning (female) announcement
	{ "ffdorcls", 0 },  // forcefield door closes, i.e. powers up
	{ "ffdorcns", SFX_Looping },  // forcefield door closed, constant hum
	{ "ffdoropn", 0 },  // forcefield door opens, i.e. powers down
	{ "ffield1", SFX_Looping },  // smooth pulsing forcefield/energy sound
	{ "ffield2", SFX_Looping },  // solar panels activated
	{ "ffield3", SFX_Looping },  // brighter pulsing forcefield/energy/laser beam sound
	{ "fireup", 0 },  // single flame up on surface of lava #1
	{ "flaphits", 0 },  // metal flap hits wall
	{ "fmorfbro", 0 },  // fleshmorph big attack roar - accompanies 'head back' move
	{ "fmorfhdr", 0 },  // fleshmorph hammers against containment door
	{ "fmorfmov", SFX_Looping },  // fleshmorph squelching, slimy movement loop
	{ "fmorfsro", 0 },  // dleshmorph small attack roar - to accompany lesser attacks
	{ "forkleng", SFX_Looping },  // forklift truck moves
	{ "frklftgo", SFX_Looping },  // forklift's forks elevate
	{ "frklften", 0 },  // forlift's forks finish elevating
	{ "gearturn", 0 },  // winch chain sound / cogs turning on doors
	{ "geekengn", SFX_Looping },  // 'voyage to the bottom of the sea' type engine for big geek
	{ "geiger", SFX_Looping },  // rapid geiger counter click loop,  for high radio act. Areas
	{ "glassexp", 0 },  // glass panel/console/box explodes
	{ "gndoorcl", 0 },  // GENERIC DOOR CLOSES
	{ "gndoorop", 0 },  // GENERIC DOOR OPENS
	{ "gndoorst", 0 },  // GENERIC DOOR STOPS
	{ "grateop1", 0 },  // metallic grates/small doors squeak open
	{ "grsldstp", 0 },  // green and red switch stops
	{ "grsldstr", 0 },  // 'green n' red' slider switch starts closing
	{ "gtretmov", SFX_Looping },  // mounted/emplaced gun turret moves in elevation/rotation
	{ "gtretstp", 0 },  // mounted/emplaced gun turret stops elevation/rotation
	{ "gtretstr", 0 },  // mounted/emplaced gun turret starts elevation/rotation
	{ "hmetslam", 0 },  // metal slamming  - doors, weights (mainly specific)
	{ "hollwam1", SFX_Looping },  // hollow tunnel  ambience- play quietly for best effect
	{ "huntreng", SFX_Looping },  // hunter enemy engine ALSO USE THIS FOR MAIN BIKENG
	{ "indshum1", SFX_Looping },  // medium powered area ambience 1
	{ "indshum2", SFX_Looping },  // lighter powered ambience 2-more like roomtone than ambience
	{ "indshum3", SFX_Looping },  // heavier powered room ambience- machinery/generator overtones
	{ "indshum4", SFX_Looping },  // lighter, subtle pulsing industrial area tone
	{ "intaler1", SFX_Looping },  // robotic 'intruder alert' announcement
	{ "keypadpr", 0 },  // general electronic keypad press
	{ "largefan", SFX_Looping },  // large fan blades loop - pitch up/down to suit fan speed
	{ "lavaloop", SFX_Looping },  // lava ambience
	{ "legzfoot", 0 },  // legz robot leg movement & footstep
	{ "leviteng", SFX_Looping },  // levitank engine loop
	{ "lift1lp", SFX_Looping },  // lift in motion
	{ "lift1stp", 0 },  // lift stops		 allocatememory
	{ "lift1str", 0 },  // lift starts    (could be used for doors instead)
	{ "litesh", 0 },  // strip light flickers #1
	{ "machamb1", SFX_Looping },  // industrial machinery active/ in motion
	{ "mainhumh", SFX_Looping },  // flourescent hum - for larger powered lights
	{ "mainhuml", SFX_Looping },  // flourescent hum - for small powered lights
	{ "maxeng", SFX_Looping },  // max enemy engine loop
	{ "metateng", SFX_Looping },  // metatank engine loop - this is same as rumble1 - big
	{ "methinge", 0 },  // metal flap swings open
	{ "metpexpl", 0 },  // metal panel explodes
	{ "mineleng", SFX_Looping },  // minelayer engine loop
	{ "mineseng", SFX_Looping },  // minesweeper engine loop
	{ "mtrtspin", SFX_Looping },  // missile turret emplacment spinning.
	{ "mtscrape", SFX_Looping },  // docking tunnel doors scrape movement??
	{ "nearwatr", SFX_Looping },  // water vicinity ambience (running water loop)
	{ "potexp", 0 },  // pot explodes (temple levels only?)
	{ "pumpamb1", SFX_Looping },  // heavy waterpump ambience - map this where there are pipes
	{ "pumpamb2", SFX_Looping },  // big, slow, very heavy pump ambience - again, map near pipes
	{ "rockfall", 0 },  // big rocks dislodge and start to fall (for 'tank bridge' area)
	{ "rockht", 0 },  // big rock impact ground #1
	{ "roomtone1", SFX_Looping },  // general powered room tone, air conditioning in background
	{ "rumble1", SFX_Looping },  // low rumble of space station movement (constant?)
	{ "safedial", SFX_Looping },  // safe dials whirr round (vary pitch for speed for turning)
	{ "sfindamb", SFX_Looping },  // soft, pulsing energy ambience / hi-tech powered room
	{ "shadeeng", SFX_Looping },  // shade enemy engine loop
	{ "shipcrk1", 0 },  // distant ship hull scraping (one off sound - play occassionally)
	{ "shipcrk2", 0 },  // ships' bows creaking (one off - as above)
	{ "shtlidle", SFX_Looping },  // space shuttle idling/trafficing sound
	{ "siren01", SFX_Looping },  // continuous wailing alarm siren
	{ "siren02", SFX_Looping },  // repeating single tone siren (geared towards ship levels)
	{ "siren03", SFX_Looping },  // repeating buzzer alarm siren
	{ "siren04", SFX_Looping },  // repeating klaxon siren
	{ "siren05", SFX_Looping },  // new one - goes well with 'emergency' speech
	{ "smallfan", SFX_Looping },  // small spinning ventilator fan
	{ "smallfn2", SFX_Looping },  // large spinning ventilator fan
	{ "smallfn3", SFX_Looping },  // ventilator fan - slightly rattling blade (more conspicuous)
	{ "smallfn4", SFX_Looping },  // ventilator fan - very rattley blade (very conspicuous)
	{ "smkdorcl", 0 },  // smoke door closing (vanishing)
	{ "smkdorop", 0 },  // smoke door opening (reappearing)
	{ "spshappr", 0 },  // space ship desceending to launch pad - will need accurate timing
	{ "spshland", 0 },  // spaceship landing thud
	{ "stndrmov", SFX_Looping },  // heavy stone door moving
	{ "stndrstp", 0 },  // heavy stone door stop
	{ "stndrstr", 0 },  // heavy stone door start
	{ "stnswitch", 0 },  // stone switch depress
	{ "stnswtch", 0 },  // stone switch 'clack'
	{ "subgenlp", SFX_Looping },  // generator loop (for the subway level, but has other uses)
	{ "subgenst", 0 },  // generator start (motor winds up to running speed)
	{ "subgstop", 0 },  // for power stopping? Alternatively, use:---
	{ "suppreng", SFX_Looping },  // suppressor enemy engine loop
	{ "swarmeng", SFX_Looping },  // swarm enemy engine
	{ "switch01", 0 },  // fairly slow metallic switch
	{ "switch02", 0 },  // quick metallic switch
	{ "switch03", 0 },  // slow metallic switch
	{ "switch04", 0 },  // metallic button press
	{ "switch05", 0 },  // small button/lever switch
	{ "switch06", 0 },  // small electronic/synthetic switch
	{ "switch07", 0 },  // small motorized lever
	{ "switch08", 0 },  // small electronic/synthetic switch
	{ "tankeng", SFX_Looping },  // caterpillar-tracked tank enemy engine
	{ "tcovercl", 0 },  // gun turret cover closes
	{ "tcoverop", 0 },  // gun turret cover open
	{ "thermal", 0 },  // single heavy air gust - volcano use, also big fan/air tube levels
	{ "throbamb", SFX_Looping },  // throbbing magnetic hum ambience, generator, machine motion
	{ "thrstbrn", SFX_Looping },  // thruster jet engine loop for space ship - if this is too weak, use firembls
	{ "torchbrn", SFX_Looping },  // firetorch burning loop
	{ "trainmov", 0 },  // re-powered up train moves
	{ "tubertat", SFX_Looping },  // metallic sliding, rotating tunnel (can't remember which level
	{ "wallfall", 0 },  // wall collapses / doors/flapsbreaks
	{ "windesol", SFX_Looping },  // desolate wind
	{ "windhol1", SFX_Looping },  // hollow wind, calm -good for long tunnels
	{ "windhol2", SFX_Looping },  // heavier hollow wind -better for area ambience
	{ "windhol3", SFX_Looping },  // smoother less hollow wind sound

	{ "shrtrumb", SFX_Looping },
	{ "longrumb", 0 },
	{ "elecbzt", 0 },
	{ "firesbrn", SFX_Looping },
	{ "elechumh", SFX_Looping },
	{ "shrphmt04", 0 },
	{ "shrplrk", 0 },
	{ "shteammbm", 0 },
	{ "gasleak", SFX_Looping },

	{ "", SFX_End },
};

char *BikeComputerSpeechNames[MAXBIKECOMPTYPES] = {
	"P3B",	//	-	Phil3B
	"BRE",	//	-	Brenda
	"LAN",	//	-	Lani-1
	"LEP",	//	-	Lepracom
	"ROA",	//	-	Roadster
};
#define MAX_BIKE_COMPUTER_SFX 64 

char *BikeComputerSpeechEffects[MAX_BIKE_COMPUTER_SFX] = {
	"AM",	//  -	assassin missile
	"AP",	//  -	picking up a weapon which is already present
	"BL",	//  -	beam laser
	"BN",	//  -	bad navigation
	"CA",	//  -	camping
	"CD",	//  -	chaff dispenser
	"CS",	//  -	chaos shield
	"DY",	//  -	destroying yourself
	"EA",	//  -	extra ammo
	"EX",	//  -	extra, miscellaneous phrases
	"FL",	//  -	flares
	"GK",	//  -	good kill total
	"GL",	//  -	general ammo low
	"GM",	//  -	gravgon missile
	"GP",	//  -	golden power pod
	"HC",	//  -	hull critical
	"IN",	//  -	incoming
	"IR",	//  -	IR goggles
	"MA",	//  -	maximum ammo
	"MK",	//  -	many kills in a short time period
	"MR",	//  -	MFRL
	"MU",	//  -	mug
	"NK",	//  -	no kills for a lengthy time period
	"NL",	//  -	nitro low
	"NP",	//  -	selecting a weapon which is not present
	"NT",	//  -	nitro
	"OP",	//  -	orbit pulsar
	"PG",	//  -	petro//  -gel
	"PK",	//  -	poor kill total
	"PL",	//  -	pyrolite fuel low
	"PM",	//  -	pine mine
	"PO",	//  -	power pod
	"PP",	//  -	plasma pack
	"PR",	//  -	purge mine
	"PS",	//  -	pulsar
	"PY",	//  -	pyrolite
	"QM",	//  -	quantum mine
	"RR",	//  -	resnic reanimator
	"SA",	//  -	scatter missile
	"SC",	//  -	shield critical
	"SG",	//  -	suss//  -gun
	"SH",	//  -	shield
	"SI",	//  -	scatter missile impact
	"SL",	//  -	suss gun ammo low
	"SM",	//  -	smoke streamer
	"SP",	//  -	spider mine
	"SO",	//  -	solaris heatseaker
	"ST",	//  -	stealth mantle
	"TI",	//  -	titan star missile
	"TR",	//  -	transpulse
	"TX",	//  -	trojax
	NULL,
};

char *BikerSpeechNames[MAXBIKETYPES] = {
	"LOK",	//	-	Lokasenna
	"BRD",	//	-	Beard
	"JAY",	//	-	L.A. Jay
	"COP",	//	-	Ex-Cop
	"REX",	//	-	Rex Hardy
	"FOE",	//	-	Foetoid
	"NIM",	//	-	Nim Soo Sun
	"NEP",	//	-	Nutta
	"SEP",	//	-	Sceptre
	"JOE",	//	-	Jo
	"CUV",	//	-	Cuvel Clark
	"HK5",	//	-	HK 5
	"NUB",	//	-	Nubia
	"MEP",	//	-	Mofisto
	"CER",	//	-	Cerbero
	"EAR",	//	-	Slick
	"BEL",	//	-	Flygirl
};

char *BikerSpeechEffects[MAX_BIKE_COMPUTER_SFX] = {
	"GP",	//	-	general 
	"VP",	//	-	victory
	"LP",	//	-	losing
	"BW",	//	-	big weapon gain
	"LE",	//	-	low energy
	"TN",	//	-	taunt
	"PN",	//	-	pain
	"DT",	//	-	death
	"EX",	//	-	extra 
};

char *GenericSfxPath = {
	"data\\sound\\generic\\",
};
char *BikeCompSfxPath = {
	"data\\sound\\BikeComp\\",
};
char *BikerSfxPath = {
	"data\\sound\\Biker\\",
};
char *LevelSpecSfxPath = {
	"data\\sound\\Mapped\\",
};

void GetSfxPath( int sfxnum, char *path )
{

	if ( sfxnum > SFX_LEVELSPEC_Start )
		sfxnum = SFX_LEVELSPEC_Start;

	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_BikeComp )
	{
		strcpy( path, BikeCompSfxPath );
		return;
	}

	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_Biker )
	{
		strcpy( path, BikerSfxPath );
		return;
	}

	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_LevelSpec )
	{
		strcpy( path, LevelSpecSfxPath );
		return;
	}

	strcpy( path, GenericSfxPath );

}

bool SetPosVelDir_Listner( VECTOR * Pos , VECTOR * Velocity , MATRIX * Mat )
{
	VECTOR UpVector;
	VECTOR ForwardVector;

	if(!sound_listener_position(
		Pos->x / 128.0F,
		Pos->y / 128.0F,
		Pos->z / 128.0F
	)) return false;

	if(!sound_listener_velocity(
		Velocity->x, // / 128.0F,
		Velocity->y, // / 128.0F,
		Velocity->z // / 128.0F,
	)) return false;

	ApplyMatrix( Mat, &SlideUp, &UpVector ); /* Calc Direction Vector */
	ApplyMatrix( Mat, &Forward, &ForwardVector ); /* Calc Direction Vector */

	if(!sound_listener_orientation(
		ForwardVector.x,
		ForwardVector.y,
		ForwardVector.z,
		UpVector.x,
		UpVector.y,
		UpVector.z
	)) return false;

	return true;
}

void CheckSpeech( int index )
{
	if ( CurrentBikerSpeech )
	{
		if ( SfxHolder[ index ].UniqueID == CurrentBikerSpeech )
			CurrentBikerSpeech = 0;
	}

	if ( CurrentBikeCompSpeech )
	{
		if ( SfxHolder[ index ].UniqueID == CurrentBikeCompSpeech )
			CurrentBikeCompSpeech = 0;
	}
}

#define SFX_HOLDERTYPE_Static 0
#define SFX_HOLDERTYPE_Dynamic 2
#define SFX_HOLDERTYPE_Looping 3
#define SFX_HOLDERTYPE_Taunt 4
/****************************************
	Procedure	:		FreeSfxHolder
	Input		:		int index:	index to SfxHolder
	Output		:		none
	description	:		Marks an SfxHolder as unused
						If Sfx being played was biker speech, reset CurrentBikerSpeech
*****************************************/
void FreeSfxHolder( int index ) 
{
	CheckSpeech( index );

	if( SfxHolder[ index ].SfxFlags == SFX_HOLDERTYPE_Taunt )
	{
		TauntID = 0;
		Taunter = 0xFF;
		TauntUpdatable = false;
		EnemyTaunter = NULL;
	}

	if(SfxHolder[index].source)
	{
		sound_release_source(SfxHolder[index].source);
		SfxHolder[index].source = NULL;
	}
	
	SfxHolder[ index ].Used = false;
}

/****************************************
	Procedure	:		GetSfxFileNamePrefix
	description	:		gets a prefix to the given sfx, based on what the sfx type is
	Input		:		int sfxnum:	sfx index ( in Sfx_Filenames table )
	Output		:		prefix placed in *file
	notes		:		BikeCompSpeechSlider can be zero ( volume ) to indicate no bike computer
						
*****************************************/
void GetSfxFileNamePrefix( int sfxnum, char *file )
{
	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_BikeComp )
	{
		strcpy( file, BikeComputerSpeechNames[ player_config->bikecomp ] );
		strcat( file, "_" );
		strcat( file, BikeComputerSpeechEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ] );
		return;
	}				

	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_Biker )
	{
		strcpy( file, BikerSpeechNames[ player_config->bike ] );
		strcat( file, "_" );
		strcat( file, BikerSpeechEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ] );
		return;
	}
/*
	if ( Sfx_Filenames[ sfxnum ].Flags & SFX_LevelSpec )
	{
		if ( LevelSpecificEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ].file )
			strcpy( file, LevelSpecificEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ].file );
		return;
	}
*/

	strcpy( file, Sfx_Filenames[ sfxnum ].Name);
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

/****************************************
	Procedure	: ClearLevelSpecSfx		
	description	: removes current level specific sfx from LevelSpecificEffects table
	Input		: none
	Output		: none
*****************************************/
void ClearLevelSpecSfx( void )
{
	int i;

	for ( i = SFX_LEVELSPEC_Start; i <= SFX_LEVELSPEC_End; i++ )
	{
		/*
		LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file[0] = 0;
		LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].flags = 0;
		*/

		if( Sfx_Filenames[ i ].Name )
		{
			free( Sfx_Filenames[ i ].Name );
			Sfx_Filenames[ i ].Name = NULL;
		}
		Sfx_Filenames[ i ].Flags = 0;
	}

	Sfx_Filenames[ SFX_LEVELSPEC_Start ].Flags = SFX_LevelSpec;
}


/****************************************
	Procedure	: SfxExists
	description	: checks to see if sfx exists in correct directory
	Input		: u_int16_t sfx: Sfx index ( SfxFilenames )
				  char *sfxname: Sfx name
	Output		: true / false
	notes		: <name> exists if <path>/name.wav or <path>/name01.wav exists
				  this implies that sfx with variants must be numbered 01 to x.
*****************************************/
bool SfxExists( u_int16_t sfx, char *name )
{
	char fullpath[128] = "";
	char dirname[128]  = "";
	char *ptr;

	// try to find the sfx file
	// using the standard <name>.wav format

	GetSfxPath( sfx , dirname );  // get path to sfx file
	strcat( fullpath, dirname );  // set the path
	strcat( fullpath, name    );  // add the name
	strcat( fullpath, ".wav"  );  // add the extension

	// success !
	if(File_Exists(fullpath))
		return true;

	// get pointer to extension
	if ( ! (ptr = strstr(fullpath,".wav")))
	  return false;

	// remove the extension
	*ptr = 0;

	// try to find the sfx file
	// using the <name>01.wav format

	// add the 01
	strcat( fullpath, "01" );

	// add the extension
	strcat( fullpath, ".wav" );

	// if file exists
	if( File_Exists ( fullpath ) )

		// success
		return true;

	// failure
	return false;

}


bool IsDigit( char check )
{
	if ( ( check >= 48 ) && ( check <= 57 ) )
		return true;
	else
		return false;
}

/****************************************
	Procedure	: ReturnSFXIndex
	description	: returns an index to Sfx_Filenames for the requested sfx.
				  if sfx does not already exist in table, sfx is added as 
				  a level specific sfx at the end of the table.
	Input		:  char *file: Sfx name
	Output		: int16_t: index no.
*****************************************/

int16_t ReturnSFXIndex( char *file )
{
	int16_t i, level_spec_flags, filelen;
	char *suffix;
	char filetocheck[ 256 ];
	bool found;

	// if zero length string, return error
	if( !file || !file[0] )
		return -1;

	// remove .wav suffix if given
	suffix = strstr( file, ".wav" );
	if ( suffix )
		*suffix = 0;

	// check all non level specific sfx for file
	for( i = 0; i < SFX_LEVELSPEC_Start; i++ )
	{
		if ( Sfx_Filenames[ i ].Name && ( !strcasecmp( Sfx_Filenames[ i ].Name, file ) ) )
		{
			if ( SfxExists( i, file ) )
			{
				RequestSfx( i );
				return i;
			}
			else
			{
				return -1;
			}
		}
	}

	// check all existing level specific sfx for file
	for ( i = SFX_LEVELSPEC_Start; i <= SFX_LEVELSPEC_End; i++ )
		//if ( LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file[0] )
		if ( Sfx_Filenames[ i ].Name )
		{
			//if ( !strcasecmp( LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file, file ) )
			if ( !strcasecmp( Sfx_Filenames[ i ].Name, file ) )
			{
				RequestSfx( i );
				return i;
			}
		}else
			break;
		
	// look up sfx in table of possible level spec sfx ( neccesary in order to get correct flags )...
	i = 0;
	level_spec_flags = SFX_LevelSpec;
	found = false;

	while (!( LevelSpecificSfxLookup[ i ].flags & SFX_End ))
	{
		if ( !strcasecmp( LevelSpecificSfxLookup[ i ].file, file ) )
		{
			if ( SfxExists( SFX_LEVELSPEC_Start , file ) )	
			{
				level_spec_flags |= LevelSpecificSfxLookup[ i ].flags;
				found = true;
			}
			break;
		}

		i++;
	}

	if ( !found )
	{
		// if no flags found, try again minus suffix digits ( if any )
		filelen = strlen( file );
		if ( ( IsDigit( file[ filelen - 1 ] ) ) && ( IsDigit( file[ filelen - 2 ] ) ) )
		{
		    strcpy( filetocheck, file );
			filetocheck[ filelen - 2 ] = 0;
			i = 0;
			while (!( LevelSpecificSfxLookup[ i ].flags & SFX_End ))
			{
				if ( !strcasecmp( LevelSpecificSfxLookup[ i ].file, filetocheck ) )
				{
					if ( SfxExists( SFX_LEVELSPEC_Start , file ) )
					{
						level_spec_flags |= LevelSpecificSfxLookup[ i ].flags;
						found = true;
					}
					break;
				}

				i++;
			}
		}

		// if still not flags...
		if ( !found )
		{
			Msg("Level specific sfx %s not found in LevelSpecificSfxLookup - please tell Phil\n", file);
			return -1;
		}
	}


	// add to end of level specific table
	for ( i = SFX_LEVELSPEC_Start; i <= SFX_LEVELSPEC_End; i++ )
	{
	//		if (!( LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file[0] ))
		if ( !Sfx_Filenames[ i ].Name  )
		{
			//LevelSpecificEffects[ Sfx_Filenames[ i ].SfxLookup ].flags = level_spec_flags;
			Sfx_Filenames[ i ].Flags = level_spec_flags;
			//strcpy( LevelSpecificEffects[ Sfx_Filenames[ i ].SfxLookup ].file, file );
			Sfx_Filenames[ i ].Name = (char *)malloc( strlen( file ) + 1 );
			strcpy( Sfx_Filenames[ i ].Name, file );
			RequestSfx( i );
			return i;
		}
	}

	DebugPrintf("Too many sfx!! - Please tell Phil\n");

	return -1;
}

#define NUM_ESSENTIAL_SFX 26

int16_t EssentialSfx[ NUM_ESSENTIAL_SFX ] = {
	SFX_BIKECOMP_AP,				//	-	picking up a weapon which is already present
	SFX_Select_BeamLaser,			//	-	beam laser
	SFX_Select_Invul,				//	-	chaos shield
	SFX_Select_Ammo,				//	-	extra ammo
	SFX_Select_GravgonMissile,		//	-	gravgon missile
	SFX_Select_GoldenPowerPod,		//	-	golden power pod
	SFX_Incoming,					//	-	incoming
	SFX_Select_MFRL,				//	-	MRFL
	SFX_Select_MugMissile,			//	-	mug
	SFX_DontHaveThat,				//	-	selecting a weapon which is not present
	SFX_Select_Nitro,				//	-	nitro
	SFX_Orbital,					//	-	orbit pulsar
	SFX_Select_PineMine,			//	-	pine mine
	SFX_Select_PowerPod,			//	-	power pod
	SFX_Select_PurgeMine,			//	-	purge mine
	SFX_Select_Pulsar,				//	-	pulsar
	SFX_Select_QuantumMine,			//	-	quantum mine
	SFX_Select_ScatterMissile,		//	-	scatter missile
	SFX_Select_SussGun,				//	-	suss-gun
	SFX_Select_Shield,				//	-	shield
	SFX_Scattered,					//	-	scatter missile impact
	SFX_Select_SolarisMissile,		//	-	solaris heatseaker
	SFX_BIKECOMP_ST,				//	-	stealth mantle
	SFX_Select_TitanStarMissile,	//	-	titan star missile
	SFX_Select_Transpulse,			//	-	transpulse
	SFX_Select_Trojax,				//	-	trojax
};

void PauseAllSfx( void )
{
	u_int16_t i;
	SPOT_SFX_LIST temp;
	
	// looping sfx info has to be stored for later retriggering
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( SpotSfxList[ i ].used )
		{
			temp = SpotSfxList[ i ];
			// StopSfx will free the source
			// so we can't leave the old pointer here 
			temp.source = NULL;
			StopSfx( SpotSfxList[ i ].uid );
			SpotSfxList[ i ] = temp;
		}
	}

	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( SfxHolder[ i ].Used )
		{
			StopSfx( SfxHolder[ i ].UniqueID );
		}
	}
}

#define LOOPING_SFX_SCALE 0.75F		// used for max distance of looping sfx ( fraction of max distance used for normal sfx )
#define LOOPING_SFX_FRAME_SKIP 5.0F	// process looping sfx every 5 frames
#define LOOPING_SFX_FixedGroup 0
#define LOOPING_SFX_VariableGroup 1

bool RestoreSfxData( u_int32_t id, VECTOR *pos, u_int16_t *group )
{
	int i;

	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( SpotSfxList[ i ].uid == id )
		{
			if ( SpotSfxList[ i ].type == LOOPING_SFX_VariableGroup )
			{
				SpotSfxList[ i ].pos = pos;
				SpotSfxList[ i ].group = group;
				return true;
			}
			else
			{
				Msg("trying to restore group & pos addresses for non-variable looping sfx!\n");
				return false;
			}
		}
	}

	return false;
}

void ReTriggerSfx( void )
{
#if 0
	u_int16_t i, j;
	u_int32_t tempuid[ MAX_LOOPING_SFX ];

	SfxUniqueID = 1;

	// find highest unique id
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( TempSpotSfxList[ i ].uid > SfxUniqueID )
		{
			SfxUniqueID = TempSpotSfxList[ i ].uid;
		}
	}

	memset( tempuid, 0, sizeof( u_int32_t ) * MAX_LOOPING_SFX );

	// re-trigger all existing looping sfx...
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( TempSpotSfxList[ i ].used )
		{
			if ( TempSpotSfxList[ i ].type == LOOPING_SFX_FixedGroup )
			{
				tempuid[ i ] = PlayFixedSpotSfx( temp.sfxindex, temp.fixedgroup, &temp.fixedpos, temp.freq, temp.vol, temp.Effects );
			}

			if ( TempSpotSfxList[ i ].type == LOOPING_SFX_VariableGroup )
			{

				tempuid[ i ] = PlaySpotSfx( temp.sfxindex, temp.group, temp.pos,	temp.freq, temp.vol, temp.Effects );
			}

		}
	}

	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( tempuid[ i ] )
		{
			// change uid of given sfx holder to match old one...
			for ( j = 0; j < MAX_ANY_SFX; j++ )
			{
				if ( SfxHolder[ j ].Used && ( SfxHolder[ j ].UniqueID == tempuid[ i ] ) )
				{
					SfxHolder[ j ].UniqueID = TempSpotSfxList[ j ].uid;
					SpotSfxList[ SfxHolder[ j ].SfxBufferIndex ].uid = TempSpotSfxList[ j ].uid;
					break;
				}
			}
		}
	}
#else
	SPOT_SFX_LIST temp;
	u_int32_t tempuid;
	u_int16_t i, j;

	// re-trigger all existing looping sfx...
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( SpotSfxList[ i ].used )
		{
			temp = SpotSfxList[ i ];
			SpotSfxList[ i ].used = false;
			
			tempuid = 0;
			
			if ( temp.type == LOOPING_SFX_FixedGroup )
			{
				tempuid = PlayFixedSpotSfx( temp.sfxindex, temp.fixedgroup, &temp.fixedpos, temp.freq, temp.vol, temp.Effects );
			}

			if ( temp.type == LOOPING_SFX_VariableGroup )
			{
				tempuid = PlaySpotSfx( temp.sfxindex, temp.group, temp.pos,	temp.freq, temp.vol, temp.Effects );
			}

			if ( tempuid )
			{
				// change uid of given sfx holder to match old one...
				for ( j = 0; j < MAX_ANY_SFX; j++ )
				{
					if ( SfxHolder[ j ].Used && ( SfxHolder[ j ].UniqueID == tempuid ) )
					{
						SfxHolder[ j ].UniqueID = temp.uid;
						SpotSfxList[ SfxHolder[ j ].SfxBufferIndex ].uid = temp.uid;
						break;
					}
				}
			}
			else
			{
				DebugPrintf("Unable to retrigger looping sfx after reloading sfx!!\n");
			}
		}
	}
#endif
}

void GetFullBikeSfxPath( char *fullpath, int sfxnum, int variant, int total_variants, int bike )
{
	char filename[256];
	char numstr[3];

	if ( ( variant > total_variants ) && total_variants )
	{
		DebugPrintf("GetFullSfxPath: invalid parameters\n");
		fullpath[ 0 ] = 0;
		return;
	}

	// name of speach file
	strcpy( filename, BikerSpeechNames[ bike ] );

	// add _
	strcat( filename, "_" );

	// 
	strcat( filename, BikerSpeechEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ] );

	// create filename with no num extension
	if ( ( total_variants == 0 ) || ( total_variants == 1 ) )
	{
		GetSfxPath( sfxnum , fullpath );
		strcat( fullpath, filename );
		strcat( fullpath, ".wav" );
	}

	// create filename with num extension
	else
	{
		GetSfxPath( sfxnum , fullpath );
		strcat( fullpath, filename );
		sprintf( numstr, "%02d", variant + 1 );	// file convention states that variant sounds start at 01.
		strcat( fullpath, numstr );
		strcat( fullpath, ".wav" );
	}

}

void GetFullBikeCompSfxPath( char *fullpath, int sfxnum, int variant, int total_variants, int bikecomp )
{
	char filename[256];
	char numstr[3];

	if ( ( variant > total_variants ) && total_variants )
	{
		DebugPrintf("GetFullSfxPath: invalid parameters\n");
		fullpath[ 0 ] = 0;
		return;
	}


	strcpy( filename, BikeComputerSpeechNames[ bikecomp ] );
	strcat( filename, "_" );
	strcat( filename, BikeComputerSpeechEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ] );

	if ( ( total_variants == 0 ) || ( total_variants == 1 ) )
	{
		// create filename with no num extension
		GetSfxPath( sfxnum, fullpath );
		strcat( fullpath, filename );
		strcat( fullpath, ".wav" );
	}else
	{
		// create filename with num extension
		GetSfxPath( sfxnum, fullpath );
		strcat( fullpath, filename );
		sprintf( numstr, "%02d", variant + 1 );	// file convention states that variant sounds start at 01.
		strcat( fullpath, numstr );
		strcat( fullpath, ".wav" );
	}

}


void GetFullSfxPath( char *fullpath, int sfxnum, int variant, int total_variants )
{
	char filename[256];
	char numstr[3];
	
	if ( ( variant > total_variants ) && total_variants )
	{
		DebugPrintf("GetFullSfxPath: invalid parameters\n");
		fullpath[ 0 ] = 0;
		return;
	}

	GetSfxFileNamePrefix( sfxnum, filename );

	if ( ( total_variants == 0 ) || ( total_variants == 1 ) )
	{
		// create filename with no num extension
		GetSfxPath( sfxnum , fullpath );
		strcat( fullpath, filename );
		strcat( fullpath, ".wav" );
	}
	else
	{
		// create filename with num extension
		GetSfxPath( sfxnum , fullpath );
		strcat( fullpath, filename );
		sprintf( numstr, "%02d", variant + 1 );	// file convention states that variant sounds start at 01.
		strcat( fullpath, numstr );
		strcat( fullpath, ".wav" );
	}

}

extern BYTE MyGameStatus;
void PreInitSfx( void )
{
	int i;
	
	for( i = 0; i < MAX_SFX; i++ )
	{
		SndLookup[ i ].Num_Variants = 0;
		SndLookup[ i ].Requested = false;
	}

	switch( MyGameStatus )
	{
	case STATUS_Title:
	case STATUS_BetweenLevels:
		RequestTitleSfx();
		break;
	default:
		RequestMainSfx();
		break;
	}
}

void RequestSfx( int16_t sfxnum )
{
	SndLookup[ sfxnum ].Requested = true;
}

#define SFX_BIKER_START ( SFX_BIKER_GP )
#define SFX_BIKECOMP_START ( SFX_BIKECOMP_AM )
#define SFX_NUM_BIKE_PHRASES ( SFX_BIKER_EX - SFX_BIKER_GP + 1 )
#define SFX_NUM_BIKECOMP_PHRASES ( SFX_Select_Trojax - SFX_BIKECOMP_AM + 1 )
u_int16_t BikeSpeechVariants[ SFX_NUM_BIKE_PHRASES ][ MAXBIKETYPES ];
u_int16_t BikeCompVariants[ SFX_NUM_BIKECOMP_PHRASES ][ MAXBIKECOMPTYPES ];

void UpdateSfxForBiker( u_int16_t biker )
{
	int i, j;
	char file[ 128 ];

	for( i = 0; i < SFX_NUM_BIKE_PHRASES; i++ )
	{
		SndLookup[ i + SFX_BIKER_START ].Num_Variants = BikeSpeechVariants[ i ][ biker ];

		// cleanup old biker settings
		for ( j = 0; j < MAX_SFX_VARIANTS; j++ )
		{
			if( SfxFullPath[ i + SFX_BIKER_START ][ j ] )
			{
				free( SfxFullPath[ i + SFX_BIKER_START ][ j ] );
				SfxFullPath[ i + SFX_BIKER_START ][ j ] = NULL;
			}
			sound_buffer_delete(i+SFX_BIKER_START);
		}

		// apply new biker settings
		for( j = 0; j < SndLookup[ i + SFX_BIKER_START ].Num_Variants; j++ )
		{
			GetFullSfxPath( file, i + SFX_BIKER_START, j, SndLookup[ i + SFX_BIKER_START ].Num_Variants );
			SfxFullPath[ i + SFX_BIKER_START ][ j ] = ( char * )malloc( strlen( file ) + 1 );
			strcpy( SfxFullPath[ i + SFX_BIKER_START ][ j ], file );
		}
	}
}

void UpdateSfxForBikeComputer( u_int16_t bikecomp )
{
	int i, j;
	char file[ 128 ];

	for( i = 0; i < SFX_NUM_BIKECOMP_PHRASES; i++ )
	{
		SndLookup[ i + SFX_BIKECOMP_START ].Num_Variants = BikeCompVariants[ i ][ bikecomp ];

		// cleanup old settings
		for ( j = 0; j < MAX_SFX_VARIANTS; j++ )
		{
			if( SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] )
			{
				free( SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] );
				SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] = NULL;
			}
			sound_buffer_delete(i+SFX_BIKECOMP_START);
		}

		// create new settings
		for( j = 0; j < SndLookup[ i + SFX_BIKECOMP_START ].Num_Variants; j++ )
		{
			GetFullSfxPath( file, i + SFX_BIKECOMP_START, j, SndLookup[ i + SFX_BIKECOMP_START ].Num_Variants );
			SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] = ( char * )malloc( strlen( file ) + 1 );
			strcpy( SfxFullPath[ i + SFX_BIKECOMP_START ][ j ], file );
		}
	}
}


void GetBikeVariants( void )
{
	int bike, i, j;
	char filename[256];
	
	for ( i = 0; i < MAX_SFX; i++ )
	{
		if ( i >= SFX_LEVELSPEC_Start )
			break;

		if ( Sfx_Filenames[ i ].Flags & SFX_Biker )
		{

			for( bike = 0; bike < MAXBIKETYPES; bike++ )
			{

				BikeSpeechVariants[ i - SFX_BIKER_START ][ bike ] = 0;

				// try filename with no num extension
				GetFullBikeSfxPath( filename, i, 1, 0, bike );

				if ( File_Exists ( filename ) )
				{
					BikeSpeechVariants[ i - SFX_BIKER_START ][ bike ]++; 
					continue;
				}

				// try filename for variants
				j = 0;
				while( 1 )
				{

					if ( BikeSpeechVariants[ i - SFX_BIKER_START ][ bike ] < 2 )
						GetFullBikeSfxPath( filename, i, j++, 2, bike );
					else
						GetFullBikeSfxPath( filename, i, j++, BikeSpeechVariants[ i - SFX_BIKER_START ][ bike ], bike );

					if ( File_Exists( filename ) )
						BikeSpeechVariants[ i - SFX_BIKER_START ][ bike ]++; 
					else
						break;

				}
			}
		}
	}
}

void GetBikeCompVariants( void )
{
	int bikecomp, i, j;
	char filename[256];
	
	for ( i = 0; i < MAX_SFX; i++ )
	{
		if ( i >= SFX_LEVELSPEC_Start )
			break;

		if ( Sfx_Filenames[ i ].Flags & SFX_BikeComp )
		{

			for( bikecomp = 0; bikecomp < MAXBIKECOMPTYPES; bikecomp++ )
			{
				BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ] = 0;

				// try filename with no num extension
				GetFullBikeCompSfxPath( filename, i, 1, 0, bikecomp );

				if ( File_Exists ( filename ) )
				{
					BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ]++; 
					continue;
				}

				// try filename for variants
				j = 0;
				while( 1 )
				{

					if ( BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ] &&
						 BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ] < 2 )
						GetFullBikeCompSfxPath( filename, i, j++, 2, bikecomp );
					else
						GetFullBikeCompSfxPath( filename, i, j++, BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ], bikecomp );

					if ( File_Exists( filename ) )
						BikeCompVariants[ i - SFX_BIKECOMP_START ][ bikecomp ]++; 
					else
						break;

				}
			}
		}
	}
}

void PreProcessSfx( void )
{
	int i, j;
	char filename[256];
	char fullpath[256];

	DebugPrintf("Detecting valid sound files.\n");

	memset( SfxFullPath, 0, sizeof( SfxFullPath ) );

	for ( i = 0; i < MAX_SFX; i++ )
	{
		if ( ( i >= SFX_LEVELSPEC_Start ) && !Sfx_Filenames[ i ].Name )
			break;
		
		if ( ! SndLookup[ i ].Requested )
			continue;

		SndLookup[ i ].Num_Variants = 0; 

		GetSfxFileNamePrefix( i, filename );
	
		// if no filename given, sound is left marked with zero variants
		if ( !filename[0] )
			continue;

		// try filename with no num extension
		GetFullSfxPath( fullpath, i, 1, 0 );

		if ( File_Exists ( fullpath ) )
		{
			SndLookup[ i ].Num_Variants++;
			SfxFullPath[ i ][ 0 ] = (char *)malloc( strlen( fullpath ) + 1 );
			strcpy( SfxFullPath[ i ][ 0 ], fullpath );
			continue;
		}

		// try filename for variants
		j = 0;
		while( 1 )
		{
			if ( SndLookup[ i ].Num_Variants < 2 )
				GetFullSfxPath( fullpath, i, j, 2 );
			else
				GetFullSfxPath( fullpath, i, j, SndLookup[ i ].Num_Variants );

			if ( File_Exists( fullpath ) )
			{
				SndLookup[ i ].Num_Variants++; 

				SfxFullPath[ i ][ j ] = (char *)malloc( strlen( fullpath ) + 1 );
				strcpy( SfxFullPath[ i ][ j ], fullpath );
				j++;
			}
			else
				break;
		}

	}

	GetBikeVariants();
	GetBikeCompVariants();
}

void RequestMainSfx( void )
{
	u_int16_t i, current;
	current = 0;
	for ( i = 0; i < MAX_SFX; i++ )
	{
		if( (!( Sfx_Filenames[ i ].Flags & SFX_Title )) || (Sfx_Filenames[ i ].Flags & SFX_InGame) )
		{
			RequestSfx( i );
		}
	}

	// if multiplayer, un-request enemy sfx

	// if not team game, un-request flag & scoring sfx
}

void RequestTitleSfx( void )
{
	u_int16_t i;
	for ( i = 0; i < MAX_SFX; i++ )
		if( Sfx_Filenames[ i ].Flags & SFX_Title )
			RequestSfx( i );
}

/****************************************
	Procedure	: InitializeSound
	description	: 
	Input		: none
	Output		: none
*****************************************/
#define DESTROYSOUND_KeepLevelSpecTable 1
bool InitializeSound( int flags )
{
	int i;
	u_int16_t Num_Sfx = 0;

	// check sound is not already initialised
	if ( bSoundEnabled )
	{
		DebugPrintf("Tried to init sound before destroying!\n");
		return false;
	}
	
	DebugPrintf("Initializing sound.\n");

	// check sound is valid
	if( NoSFX || !sound_init() )
	{
		bSoundEnabled = false;
		return true;
	}
	else
	{
		bSoundEnabled = true;
	}

	PreInitSfx();

	Num_Sfx = 0;

	PreProcessSfx();

	// null globals which could have dirty memory
	for( i = 0; i < MAX_SFX; i++ )
		sound_buffers[i] = NULL;
	for( i = 0; i < MAX_ANY_SFX; i++ )
		SfxHolder[i].source = NULL;
	for( i = 0; i < MAX_LOOPING_SFX; i++ )
		SpotSfxList[i].source = NULL;

	// initialise taunt stuff
	Taunter = 0xFF;
	TauntID = 0;
	TauntUpdatable = false;
	EnemyTaunter = NULL;
	//TauntDist = 0.0F;

	InitSfxHolders();

	// re-initialise looping sfx list ( if already in level, we need to restart all existing looping sfx )
	if ( flags & DESTROYSOUND_KeepLevelSpecTable)
	{
		ReTriggerSfx();
	}
	else
	{
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
			SpotSfxList[ i ].used = false;
	}

	return true;
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:	Destroy all sound fx...
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DestroySound( int flags )
{
	int i, j;

	DebugPrintf("DestroySound\n");

	ClearLevelSpecSfx();

	if ( !bSoundEnabled )
		return;

	for ( i = 0; i < MAX_SFX; i++ )
	{
		for( j = 0; j < MAX_SFX_VARIANTS; j++ )
		{
			if( SfxFullPath[ i ][ j ] )
				free( SfxFullPath[ i ][ j ] );
		}
	}

	for ( i=0; i < MAX_LOOPING_SFX; i++ )
	{
		if(SpotSfxList[ i ].source)
		{
			sound_release_source(SpotSfxList[ i ].source);
			SpotSfxList[ i ].source = NULL;
		}
		SpotSfxList[i].used = false;
	}
	
	// clear all sfx holders...
	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		FreeSfxHolder( i );
	}
		
	for (i = 0; i < MAX_SFX; i++)
	{
		if (sound_buffers[i])
		{
			sound_release_buffer(sound_buffers[i]);
			sound_buffers[i] = NULL;
		}
	}

	bSoundEnabled = false;
}

void SetPannedSourceParams( void* sound_source, VECTOR *SfxPos, float Freq, VECTOR *Temp, float Distance, long Volume, u_int16_t Effects );

#define SPEECH_AMPLIFY	( 1.0F / GLOBAL_MAX_SFX )
#define SFX_2D 2

bool StartPannedSfx(int16_t Sfx, u_int16_t *Group , VECTOR * SfxPos, float Freq, int type, int HolderIndex, float VolModify, u_int16_t Effects, bool OverideDistanceCheck )
{
	VECTOR	Temp;
	float	Distance, MaxDistance;
	float	Modify;
	long Volume;
	int sndobj_index;
	u_int16_t offset;
	int flags;

	if( !bSoundEnabled )
{
		return false;
}

	flags = Sfx_Filenames[ Sfx ].Flags;
/*
	if ( flags & SFX_LevelSpec )
		flags = LevelSpecificEffects[ Sfx_Filenames[ Sfx ].SfxLookup ].flags;
*/

	if( flags & SFX_Biker )
    	VolModify *= ( (float)BikerSpeechSlider.value / (float)BikerSpeechSlider.max ) * SPEECH_AMPLIFY;	// when multiplied with max value for GlobalSoundAttenuation, gives 1.0F;

	if( flags & SFX_BikeComp ) 
    	VolModify *= ( (float)BikeCompSpeechSlider.value / (float)BikeCompSpeechSlider.max ) * SPEECH_AMPLIFY;	// when multiplied with max value for GlobalSoundAttenuation, gives 1.0F;

	if ( !VolModify )
		return false;
	
	switch ( SndLookup[ Sfx ].Num_Variants )
	{
	case 0:
		//DebugPrintf("Sfx.c: PlaySfx() - sfx #%d does not exist!\n", Sfx);
		return false;
	case 1:
		sndobj_index = Sfx;
		offset = 0;
		break;
	default:
		offset = Random_Range( SndLookup[ Sfx ].Num_Variants );
		sndobj_index = Sfx + offset;
		break;
	}

	if ( flags & SFX_Biker )
	{
		if ( CurrentBikerSpeech )
		{
			if ( flags & SFX_BikerSpeechOveride )
			{
				StopSfx( CurrentBikerSpeech );
				CurrentBikerSpeech = SfxHolder[ HolderIndex ].UniqueID;
			}
			else
			{
				return false;
			}
		}
		else
		{
			CurrentBikerSpeech = SfxHolder[ HolderIndex ].UniqueID;
		}
	}

	if ( flags & SFX_BikeComp )
	{
		if ( CurrentBikeCompSpeech )
		{
			if( flags & SFX_BikeCompNoOveride )
			{
				return false;
			}
			else
			{
				StopSfx( CurrentBikeCompSpeech );
			}
		}
		CurrentBikeCompSpeech = SfxHolder[ HolderIndex ].UniqueID;
	}

	SfxHolder[ HolderIndex ].TriggerSfx = -1;

	// do not allow looping 2D sfx...
	if ( ( flags & SFX_Looping ) && ( type != SFX_2D ) )
	{
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Looping;
		SfxHolder[ HolderIndex ].SndObjIndex = sndobj_index;
		SfxHolder[ HolderIndex ].SfxBufferIndex = InitLoopingSfx(
			Sfx, offset, Group, SfxPos, Freq, VolModify, type, HolderIndex, Effects, SfxHolder[ HolderIndex ].UniqueID 
		);
		if ( SfxHolder[ HolderIndex ].SfxBufferIndex < 0 )
			return false;
		return true;
	}
	
	// TODO - all this volume hacking should be removed for openal

	if ( type != SFX_2D )
	{
		// work out sound distance...
		if( Ships[ Current_Camera_View ].Object.Group != (u_int16_t) -1 )
		{
			Modify = SoundInfo[Ships[ Current_Camera_View ].Object.Group][*Group];
		}
		else
		{
			Modify = 0.0F;
		}
		if( Modify < 0.0F )
			return false;

		Temp.x = SfxPos->x - Ships[ Current_Camera_View ].Object.Pos.x;
		Temp.y = SfxPos->y - Ships[ Current_Camera_View ].Object.Pos.y;
		Temp.z = SfxPos->z - Ships[ Current_Camera_View ].Object.Pos.z;

		Distance = (float) sqrt( ( Temp.x * Temp.x ) + ( Temp.y * Temp.y ) + ( Temp.z * Temp.z ) );

		Distance += Modify;

		MaxDistance = 24 * 1024 * GLOBAL_SCALE;

		//Distance += (MaxDistance - Distance) - (MaxDistance - Distance) * GlobalSoundAttenuation;

		if( Distance >= MaxDistance ) 
			return false;

		// check to see if another sfx of the same type has already been started...if it has but is further away..override it...
		if( !OverideDistanceCheck && (Distance+25.0F) >= LastDistance[Sfx] ) 
			return false;
	}
	else
	{
		Distance = 0.0F;
	}

	LastDistance[Sfx] = Distance;

	//DebugPrintf("sfx passing %d\n",Sfx);

	Volume = ( 0 - (long) ( Distance * 0.6F ) );	// Scale it down by a factor...
	Volume = sound_minimum_volume - (long)( (float)( sound_minimum_volume - Volume ) * VolModify * GlobalSoundAttenuation );

	if ( Effects == SPOT_SFX_TYPE_Taunt )
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Taunt;
	else
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Dynamic;

	SfxHolder[ HolderIndex ].SndObjIndex = sndobj_index;
	SfxHolder[ HolderIndex ].SfxBufferIndex = -1;

	//
	//
	//

	{
		char * file;
		int _type = SFX_TYPE_Normal;

		if ( Effects == SPOT_SFX_TYPE_Taunt )
		{
			_type = SFX_TYPE_Taunt;
			file = TauntPath;
		}
		else
		{
			_type = SFX_TYPE_Panned;
			file = SfxFullPath[ Sfx ][ Random_Range( SndLookup[ Sfx ].Num_Variants )];
		}
		
		if( ! (SfxHolder[ HolderIndex ].source = sound_source_create( file, Sfx )) )
			return false;

		if ( _type == SFX_TYPE_Normal )
		{
			sound_volume( SfxHolder[ HolderIndex ].source, Volume );
		}
  
		if ( ( _type == SFX_TYPE_Panned ) || ( _type == SFX_TYPE_Taunt ) )
		{
			SetPannedSourceParams( 
				SfxHolder[ HolderIndex ].source, 
				SfxPos, 
				Freq, 
				&Temp, 
				Distance, 
				Volume, 
				Effects 
			);
		}

        if ( flags & SFX_Looping )
                sound_play_looping( SfxHolder[ HolderIndex ].source );
        else
                sound_play( SfxHolder[ HolderIndex ].source );
	}

	return true;
}

void InitSfxHolders( void )
{
	int i;

	for( i = 0; i < MAX_ANY_SFX; i++ )
		SfxHolder[ i ].Used = false;

	SfxUniqueID = 1;
	BikerSpeechPlaying = false;
}

// TODO we should probably find the farthest sound and kill it? 
// isn't this done already in looping sfx or somewhere else ?
int FindFreeSfxHolder( void )
{
	int i;

	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( !SfxHolder[ i ].Used )
		{
			SfxHolder[ i ].Used = true;
			SfxHolder[ i ].UniqueID = SfxUniqueID++;
			SfxHolder[ i ].OnPause = false;
			SfxHolder[ i ].SndObjIndex = -1;
			SfxHolder[ i ].SfxFlags = -1;
			SfxHolder[ i ].SfxBufferIndex = -1;
			SfxHolder[ i ].TriggerSfx = -1;
			SfxHolder[ i ].PauseValue = 0.0F;
			return i;
		}
	}
	return -1;
}

u_int32_t PlaySfxWithTrigger( int16_t Sfx, int16_t TriggeredSfx )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %hd - no free sfx holders\n",Sfx);
		return 0;
	}
	else
	{
		if ( !StartPannedSfx( Sfx, NULL, NULL, 0.0F, SFX_2D, index, 1.0F, 0, false ) )
			FreeSfxHolder( index );
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
	{
		SfxHolder[ index ].TriggerSfx = TriggeredSfx;
		return SfxHolder[ index ].UniqueID;
	}

	return 0;
}


// note Dist is ignored - all 2D sfx assumed to be at zero distance
u_int32_t PlaySfx( int16_t Sfx, float Vol )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %hd - no free sfx holders\n",Sfx);
		return 0;
	}
	else
	{
		if ( !StartPannedSfx( Sfx, NULL, NULL, 0.0F, SFX_2D, index, Vol, 0, false ) )
			FreeSfxHolder( index );
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
		return SfxHolder[ index ].UniqueID;

	return 0;
}

u_int32_t PlayGeneralPannedSfx(int16_t Sfx, u_int16_t Group , VECTOR * SfxPos, float Freq, bool OverideDistanceCheck, u_int16_t Effects, float VolModify )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	//DebugPrintf("started playing sfx at %d\n", SDL_GetTicks() );

	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %hd - no free sfx holders\n",Sfx);
		return 0;
	}
	else
	{
		if ( !StartPannedSfx( Sfx, &Group , SfxPos, Freq, LOOPING_SFX_FixedGroup, index, VolModify, Effects, OverideDistanceCheck ) )
			FreeSfxHolder( index );
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
	{
		//DebugPrintf("finished playing sfx at %d\n", SDL_GetTicks() );
		return SfxHolder[ index ].UniqueID;
	}

	//DebugPrintf("sfx not played at %d\n", SDL_GetTicks() );
	return 0;
}

u_int32_t ForcePlayPannedSfx(int16_t Sfx, u_int16_t Group , VECTOR * SfxPos, float Freq )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, true, SPOT_SFX_TYPE_Normal, 1.0F );
}
u_int32_t PlayPannedSfx(int16_t Sfx, u_int16_t Group, VECTOR * SfxPos, float Freq )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, false, SPOT_SFX_TYPE_Normal, 1.0F );
}
u_int32_t PlayPannedSfxWithVolModify(int16_t Sfx, u_int16_t Group, VECTOR * SfxPos, float Freq, float Vol )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, false, SPOT_SFX_TYPE_Normal, Vol );
}

u_int32_t PlaySpotSfx(int16_t Sfx, u_int16_t *Group , VECTOR * SfxPos, float Freq, float Vol, u_int16_t Effects )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;
	
	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %hd - no free sfx holders\n",Sfx);
		return 0;
	}
	else
	{
		if ( !StartPannedSfx( Sfx, Group , SfxPos, Freq, LOOPING_SFX_VariableGroup, index, Vol, Effects, false ) )
			FreeSfxHolder( index );
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
		return SfxHolder[ index ].UniqueID;

	return 0;
}

u_int32_t PlayFixedSpotSfx(int16_t Sfx, u_int16_t Group , VECTOR * SfxPos, float Freq, float Vol, u_int16_t Effects )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;
	
	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %hd - no free sfx holders\n",Sfx);
		return 0;
	}
	else
	{
		if ( !StartPannedSfx( Sfx, &Group , SfxPos, Freq, LOOPING_SFX_FixedGroup, index, Vol, Effects, false ) )
			FreeSfxHolder( index );
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
		return SfxHolder[ index ].UniqueID;

	return 0;
}

int GetSfxHolderIndex( u_int32_t uid )
{
	int i;
	for( i = 0; i < MAX_ANY_SFX; i++ )
		if ( SfxHolder[ i ].Used && ( uid == SfxHolder[ i ].UniqueID ) )
			return i;
	return -1;
}

bool StopSfx( u_int32_t uid )
{
	int i;

	if ( !uid )
		return false;

	if ( !bSoundEnabled )
		return false;

	i = GetSfxHolderIndex( uid );
	if ( i != -1 )
	{
		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Looping )
			StopLoopingSfx( SfxHolder[ i ].SfxBufferIndex ); 

		FreeSfxHolder( i );
		return true;
	}
	
	//DebugPrintf("Unable to stop sfx!!!\n");
	return false;
}

/*===================================================================
	Procedure	:	Play Panned Sfx
	Input		:	int16_t	Sfx Number
				:	MATRIX * Matrix
				:	VECTOR * Pos
	Output		:	Nothing
===================================================================*/
void SetPannedSourceParams( void* sound_source, VECTOR *SfxPos, float Freq, VECTOR *Temp, float Distance, long Volume, u_int16_t Effects )
{
	if(!sound_source) return;
	if( Sound3D )
	{
		if(!SfxPos)
		{
			DebugPrintf("SetPannedSourceParams given SfxPos=0\n");
			return;
		}
		sound_position(
			sound_source,
			SfxPos->x / 128.0F,
			SfxPos->y / 128.0F,
			SfxPos->z / 128.0F,
			1.0f,	// min distance
			100.0f  // max distance
		);
	}
	else
	{
		VECTOR	Temp2;
		float	nz;
		float	sx, sxmod;
		long	Pan = 0;
		float mindist = SHIP_RADIUS * 6.0F;
		float sxmin = 100.0F;
		float sxmax = 10000.0F;
		float currentdist;
		if ( Distance && (!( Effects & SPOT_SFX_TYPE_NoPan )) )
		{
			ApplyMatrix( &Ships[ Current_Camera_View ].Object.FinalInvMat , Temp, &Temp2 );

			// we only care about x plane because we only have left/right speakers in panning
			// z is probably used to adjust for field of view (as things get farther away they
			// move towards the center of screen)
			currentdist = (float) sqrt( ( Temp2.x * Temp2.x ) + ( Temp2.z * Temp2.z ) );
		
			if( currentdist )
			{
				nz = Temp2.z / currentdist;

				sxmod = ( Temp2.x < 0.0F ) ? -1.0F : 1.0F;

				sx = ( currentdist > mindist ) ? 10000.0F :
					( ( ( currentdist * ( sxmax - sxmin ) ) / mindist ) + sxmin );

				sx *= sxmod;

				// the bigger z is than x becomes smaller hence sound shrinks into center of
				// view with distance
				Pan = (long) ( ( 1.0F - fabs(nz) )* ( sx ) );
			}
			else
			{
				Pan = 0;
			}
			//DebugPrintf("nz: %f\tsx: %f\tsxmod: %f\tPan: %d\n", nz, sx, sxmod, Pan);
		}
		sound_pan(sound_source, Pan);
		sound_volume( sound_source , Volume );
		sound_set_pitch( sound_source, Freq );
	}
}

void SetSoundLevels( int *dummy )
{
	GlobalSoundAttenuation = SfxSlider.value / ( SfxSlider.max / GLOBAL_MAX_SFX );
}

#define TRIGGER_SFX_PAUSE_VALUE 20.0F	// pause between triggered sfx ( 20.0F = 1/3 second )
void CheckSBufferList( void )
{
	int i;

	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( ! SfxHolder[ i ].Used )
			continue;

		if ( SfxHolder[ i ].OnPause )
		{
			SfxHolder[ i ].PauseValue -= framelag;
			if ( SfxHolder[ i ].PauseValue < 0.0F )
			{
				PlaySfx( SfxHolder[ i ].TriggerSfx, 1.0F );
				FreeSfxHolder( i );
			}
			continue;
		}

		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Looping )	// never want to kill off looping sfx here!!
			continue;

		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Static )
		{
			if(!sound_is_playing( SfxHolder[ i ].source ))
			{
				if ( SfxHolder[ i ].TriggerSfx != -1 )
				{
					// we want to trigger an additional sfx...
					SfxHolder[ i ].OnPause = true;
					SfxHolder[ i ].PauseValue = TRIGGER_SFX_PAUSE_VALUE;

					// if was biker / bikecomp speech, reset to zero
					CheckSpeech( i );
				}
				else
				{
					FreeSfxHolder( i );
				}
			}
		}

		if ( (( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Dynamic ) || ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Taunt )) )
		{
			// if current node used and not playing, release & mark as unused
			if(!sound_is_playing( SfxHolder[ i ].source ))
			{
				//DebugPrintf("Released sfx %d\n", SfxHolder[ i ].UniqueID);
				if ( SfxHolder[ i ].TriggerSfx != -1 )
				{
					// we want to trigger an additional sfx...
					SfxHolder[ i ].OnPause = true;
					SfxHolder[ i ].PauseValue = TRIGGER_SFX_PAUSE_VALUE;

					// if was biker / bikecomp speech, reset to zero
					CheckSpeech( i );
				}
				else
				{
					FreeSfxHolder( i );
				}
			}
		}
	}
}

// globals used in functions below:
// SPOT_SFX_LIST *LoopingSfxListStart - set to NULL in InitializeSound

int InitLoopingSfx( int16_t Sfx, int variant, u_int16_t *Group, VECTOR *SfxPos, float Freq, float Volume, int type, int SfxHolderIndex, u_int16_t Effects, u_int32_t uid )
{
	int index, i;

	if ( !bSoundEnabled )
		return -1;

	//DebugPrintf("Initialising looping sfx %d\n", Sfx);

	index = -1;
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( !SpotSfxList[ i ].used )
		{
			index = i;
			break;
		}
	}

	if ( index < 0 )
	{
	 	DebugPrintf("No free looping sfx list places!\n");
		return -1;
	}
	
	SpotSfxList[ index ].sfxindex = Sfx;
	SpotSfxList[ index ].type = type;
	SpotSfxList[ index ].variant = variant;

	if ( type == LOOPING_SFX_FixedGroup )
	{
		SpotSfxList[ index ].fixedgroup = *Group;
		SpotSfxList[ index ].fixedpos = *SfxPos;
	}
	if ( type == LOOPING_SFX_VariableGroup )
	{
		SpotSfxList[ index ].group = Group;
		SpotSfxList[ index ].pos = SfxPos;
	}

	SpotSfxList[ index ].freq = Freq;
	SpotSfxList[ index ].vol = Volume;
	SpotSfxList[ index ].source = NULL;
	SpotSfxList[ index ].distance = 0.0F;
	SpotSfxList[ index ].used = true;
	SpotSfxList[ index ].Effects = Effects;
	SpotSfxList[ index ].uid = uid;	
	return index;
}

void StopLoopingSfx( int index )
{
	int flags;

	flags = Sfx_Filenames[ SpotSfxList[ index ].sfxindex ].Flags;
/*
	if ( flags & SFX_LevelSpec )
		flags = LevelSpecificEffects[ Sfx_Filenames[ SpotSfxList[ index ].sfxindex ].SfxLookup ].flags;
*/

	if ( SpotSfxList[ index ].source )
	{
		//DebugPrintf("StopLoopingSfx - looping sound %d never stopped\n", SpotSfxList[ index ].sfxindex);
		sound_release_source( SpotSfxList[ index ].source );
		SpotSfxList[ index ].source = NULL;
	}

	SpotSfxList[ index ].used = false;

	DebugPrintf("stopping looping sfx %d\n", SpotSfxList[ index ].sfxindex);
}

void ModifyLoopingSfx( u_int32_t uid, float Freq, float Volume )
{
	int index, LoopingSfxIndex;
	index = GetSfxHolderIndex( uid );
	if ( index != -1 )
	{
		LoopingSfxIndex = SfxHolder[ index ].SfxBufferIndex;
		if ( ( LoopingSfxIndex >= 0 ) && ( LoopingSfxIndex < MAX_LOOPING_SFX ) )
		{
			if ( Freq && ( Freq != SpotSfxList[ LoopingSfxIndex ].freq ))
			{
				SpotSfxList[ LoopingSfxIndex ].freq = Freq;

				// if exists, set frequency...
				if ( SpotSfxList[ LoopingSfxIndex ].source )
					sound_set_pitch( SpotSfxList[ LoopingSfxIndex ].source, (float)Freq );
			}
			if ( Volume && ( Volume != SpotSfxList[ LoopingSfxIndex ].vol ))
				SpotSfxList[ LoopingSfxIndex ].vol = Volume;
		}
	}
}

#if 1
#define START_LINE 5
#define GAP_SIZE 50
#define X_OFFSET 5
#define LINE_HEIGHT 8
#define DATA_OFFSET 100

void PrintLoopingSfxDebug( void )
{
	int line, i;
	char buf[128];

	line = START_LINE;

	// print field names...
	Print4x5Text( "sfxindex",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "variant",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "flags",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "pos x",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "pos y",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "pos z",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "fixedpos x",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "fixedpos y",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "fixedpos z",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "type",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "var group",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "fixed group",	X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "freq",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "vol",			X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "source ptr",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "distance",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "holder index",	X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "thread index",	X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "effects",		X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "uid",			X_OFFSET, LINE_HEIGHT * line++, 2 ); 

	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( !SpotSfxList[ i ].used )
			continue;
	
		line = START_LINE;

		sprintf( buf, "%d", SpotSfxList[ i ].sfxindex );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].variant );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].flags );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].pos->x );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].pos->y );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].pos->z );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].fixedpos.x );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].fixedpos.y );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].fixedpos.z );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].type );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", *SpotSfxList[ i ].group );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].fixedgroup );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%1.1f", SpotSfxList[ i ].freq );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%1.1f", SpotSfxList[ i ].vol );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%s", SpotSfxList[ i ].source ? "Y" : "N" );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].distance );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].SfxHolderIndex );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].Effects );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", (int)SpotSfxList[ i ].uid );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
	}
}
#endif

void ProcessLoopingSfx( void )
{
	float	Modify = 0.0f, Distance = 0.0f, MaxDistance = 0.0f;
	int i=0, flags=0;
	long Volume = 0;
	bool InRange = false;
	VECTOR Temp = {0.0f,0.0f,0.0f};
	VECTOR Pos = {0.0f,0.0f,0.0f};

	if(!bSoundEnabled)
		return;

	// print debug info
	// only looping sound i know of is the bike engines
	//PrintLoopingSfxDebug();

	// process all looping sfx
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( !SpotSfxList[ i ].used )
			continue;

		flags = Sfx_Filenames[ SpotSfxList[ i ].sfxindex ].Flags;
/*
		if ( flags & SFX_LevelSpec )
			flags = LevelSpecificEffects[ Sfx_Filenames[ SpotSfxList[ i ].sfxindex ].SfxLookup ].flags;
*/
		

//
// Sound3D
//
//           in/out of range will not be needed if we implement a sound stealing algorithm
//           so if we need to create a new sound and we are the max sounds then we can find
//           the least significant sound based on distance/volume (attenuation?) and kill 
//           that sound so we can fire off the new one. unless the new one is worse.
//
//           volume hacking based on distance calculations will not be needed
//

		// work out if sound in range, get parameters
		if( Ships[ Current_Camera_View ].Object.Group != (u_int16_t) -1 )
		{
			Modify = 0.0F;	// just in case type is invalid...
			
			if ( SpotSfxList[ i ].type == LOOPING_SFX_FixedGroup ) 
			{
			 	Pos = SpotSfxList[ i ].fixedpos; 
				Modify = SoundInfo[Ships[ Current_Camera_View ].Object.Group][SpotSfxList[ i ].fixedgroup];
			}
			if ( SpotSfxList[ i ].type == LOOPING_SFX_VariableGroup ) 
			{
			 	Pos = *( SpotSfxList[ i ].pos ); 
				Modify = SoundInfo[Ships[ Current_Camera_View ].Object.Group][*(SpotSfxList[ i ].group)];
			}
		}
		else
		{
			Modify = 0.0F;
		}

		if( Modify != -1.0F )
		{
			Temp.x = Pos.x - Ships[ Current_Camera_View ].Object.Pos.x;
			Temp.y = Pos.y - Ships[ Current_Camera_View ].Object.Pos.y;
			Temp.z = Pos.z - Ships[ Current_Camera_View ].Object.Pos.z;

			Distance = (float) sqrt( ( Temp.x * Temp.x ) + ( Temp.y * Temp.y ) + ( Temp.z * Temp.z ) );
			Distance += Modify;
			MaxDistance = 24 * 1024 * GLOBAL_SCALE * LOOPING_SFX_SCALE;
			if( Distance >= MaxDistance ) 
				InRange = false;
			else
				InRange = true;
		}
		else
		{
			InRange = false;
		}

		SpotSfxList[ i ].distance = Distance;

		// if not in range
		if ( !InRange )
		{
			// if sound loaded then free it
		 	if ( SpotSfxList[ i ].source )
			{
				DebugPrintf("ProcessLoopingSfx: Releasing dynamic looping sfx %d\n", SpotSfxList[ i ].sfxindex);
				sound_release_source( SpotSfxList[ i ].source );
				SpotSfxList[ i ].source = NULL;
			}
			continue;
		}

		// if in range & not loaded then find a free slot for the sound
		if ( !SpotSfxList[ i ].source )
		{
			char * file = SfxFullPath[ SpotSfxList[i].sfxindex ][ SpotSfxList[i].variant ];

			// TODO - this should probably be done right away in InitLoopingSfx()
			int index = SpotSfxList[ i ].sfxindex +	SpotSfxList[ i ].variant;

			if( ! (SpotSfxList[ i ].source = sound_source_create( file, index )) )
				continue;

			sound_set_pitch( SpotSfxList[ i ].source, SpotSfxList[ i ].freq );
		}

		// if in range, and source already loaded
		if ( SpotSfxList[ i ].source)
		{
			Volume = (long)(( GlobalSoundAttenuation * Distance / MaxDistance ) * sound_minimum_volume);
			Volume = sound_minimum_volume - (long)( (float)( sound_minimum_volume - Volume ) * SpotSfxList[ i ].vol * GlobalSoundAttenuation );

			SetPannedSourceParams(
				SpotSfxList[ i ].source,
				&Pos, 
				SpotSfxList[ i ].freq,
				&Temp,
				Distance,
				Volume,
				SpotSfxList[ i ].Effects 
			);

			//DebugPrintf("ProcessLoopingSfx: playing looping sound %d\n", SpotSfxList[ i ].sfxindex);

			if( ! sound_is_playing( SpotSfxList[ i ].source ) )
				sound_play_looping(SpotSfxList[ i ].source);
		}
	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

FILE *SaveAllSfx( FILE *fp )
{
	u_int16_t num_active_sfx = 0;
	int16_t i;

	if( fp )
	{
		// work out number of sfx to save...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
			if ( SpotSfxList[ i ].used )
				num_active_sfx++;

		// write out number of sfx...
		fwrite( &num_active_sfx, sizeof( u_int16_t ), 1, fp );

		// write out sfx data...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
			if ( SpotSfxList[ i ].used )
				fwrite( &SpotSfxList[ i ], sizeof( SPOT_SFX_LIST ), 1, fp );

		fwrite( &SfxUniqueID, sizeof( u_int32_t ), 1, fp );
	}

	return( fp );
}

// TODO: this code loads pointers !!!!!!!!!!
// apparrently this is somehow possible if you save/load from same binary
FILE *LoadAllSfx( FILE *fp )
{
	u_int16_t num_active_sfx = 0;
	int16_t i;
	
	if( fp )
	{
		// work out number of sfx to load...
		fread( &num_active_sfx, sizeof( u_int16_t ), 1, fp );

		// invalidate existing sfx data, stopping any current sfx...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
		{
			if ( SpotSfxList[ i ].used )
				StopSfx( SpotSfxList[ i ].uid );
			SpotSfxList[ i ].used = false;
		}
		
		// read in sfx data...
		for ( i = 0; i < num_active_sfx; i++ )
			fread( &SpotSfxList[ i ], sizeof( SPOT_SFX_LIST ), 1, fp );

		fread( &SfxUniqueID, sizeof( u_int32_t ), 1, fp );
		
		ReTriggerSfx();
	}

	return( fp );
}

void PlaySpecificBikerSpeech( int16_t sfx, u_int16_t Group, VECTOR *SfxPos, float Freq, int biker, char variant, bool update )
{
	char *last_slash;
	char numstr[ 3 ];

	if ( !( Sfx_Filenames[ sfx ].Flags & SFX_Biker ) )
		return;

	if ( TauntID )
		return;

	// if no biker speech loaded, path ptr will be NULL
	if ( !SfxFullPath[ sfx ][ 0 ] )
		return;

	strcpy( TauntPath, SfxFullPath[ sfx ][ 0 ] );

	last_slash = strrchr( TauntPath, 92 );	// look for last slash
	if( !last_slash )
	{
		DebugPrintf("Unable to find last slash in %s\n", TauntPath);
		return;
	}
	
	last_slash++;
	//GetSfxFileNamePrefix( sfx, last_slash );
	strcpy( last_slash, BikerSpeechNames[ biker ] );
	strcat( last_slash, "_" );
	strcat( last_slash, BikerSpeechEffects[ Sfx_Filenames[ sfx ].SfxLookup ] );

	if ( variant != -1 )
	{
		sprintf( numstr, "%02d", variant + 1 );	// file convention states that variant sounds start at 01.
		strcat( last_slash, numstr );
	}else
	{
		if( BikeSpeechVariants[ sfx - SFX_BIKER_START ][ biker ] > 1 )
		{
			CurrentTauntVariant = (char)Random_Range( BikeSpeechVariants[ sfx - SFX_BIKER_START ][ biker ] );

			sprintf( numstr, "%02d", CurrentTauntVariant + 1 );	// file convention states that variant sounds start at 01.
			strcat( TauntPath, numstr );
		}
	}
	
	strcat( TauntPath, ".wav" );

	TauntID =  PlayGeneralPannedSfx( sfx, Group, SfxPos, Freq, false, SPOT_SFX_TYPE_Taunt, 1.0F );
	TauntUpdatable = update;
}

#define MAX_DISTANCE  ( 24 * 1024 * GLOBAL_SCALE )
float ReturnDistanceVolumeVector( VECTOR *sfxpos, u_int16_t sfxgroup, VECTOR *listenerpos, u_int16_t listenergroup, long *vol, VECTOR *sfxvector )
{
	VECTOR	Temp;
	float Modify;
	float dist;

	Temp.x = sfxpos->x - listenerpos->x;
	Temp.y = sfxpos->y - listenerpos->y;
	Temp.z = sfxpos->z - listenerpos->z;

	if( listenergroup != (u_int16_t) -1 )
	{
		Modify= SoundInfo[ listenergroup ][ sfxgroup ];
	}else{
		Modify = 0.0F;
	}
	if ( Modify < 0.0F )
		return -1.0F;

	dist = (float) sqrt( ( Temp.x * Temp.x ) + ( Temp.y * Temp.y ) + ( Temp.z * Temp.z ) );
	dist += Modify;

	if ( dist > MAX_DISTANCE )
		return -1.0F;
	else
	{
		if( vol )
		{
			*vol = ( 0 - (long) ( dist * 0.6F ) );	// Scale it down by a factor...
			*vol = sound_minimum_volume - (long)( ((float)( sound_minimum_volume - *vol )) * GlobalSoundAttenuation );
		}
		if( sfxvector )
		{
			*sfxvector = Temp;
		}
		return dist;
	}
}

bool UpdateTaunt( u_int32_t uid, u_int16_t Group, VECTOR *SfxPos )
{
	int i;
	float	Distance;
	long Volume;
	float VolModify;
	VECTOR sfxvector;

	if ( !uid )
		return false;

	if ( !bSoundEnabled )
		return false;

	i = GetSfxHolderIndex( uid );
	if( i == -1 )
	{
		return false;
	}

	Distance = ReturnDistanceVolumeVector( SfxPos, Group, &Ships[ Current_Camera_View ].Object.Pos, Ships[ Current_Camera_View ].Object.Group, NULL, &sfxvector );
//	TauntDist = Distance;

	if( Distance < 0.0F ) 
	{
	 	StopSfx( uid );
		return false;
	}

	if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Taunt )
	{
		VolModify = ( (float)BikerSpeechSlider.value / (float)BikerSpeechSlider.max ) * SPEECH_AMPLIFY;	// when multiplied with max value for GlobalSoundAttenuation, gives 1.0F;
		Volume = 0;
		Volume = sound_minimum_volume - (long)( (float)( sound_minimum_volume - Volume ) * VolModify * GlobalSoundAttenuation );

		// set source parameters
		DebugPrintf("UpdateTaunt:\n");
		SetPannedSourceParams( 
			SfxHolder[ i ].source,
			SfxPos, 
			0.0F, 
			&sfxvector, 
			Distance,
			Volume, 
			SPOT_SFX_TYPE_Taunt 
		);
	}
	else
	{
		DebugPrintf("Holder type not SFX_HOLDERTYPE_Taunt\n");
	}

	return true;
}

#define TAUNT_PAUSE 5.0F
float ProcessTauntPause = TAUNT_PAUSE;
void ProcessTaunt( void )
{
	if ( !TauntID || !TauntUpdatable || Taunter == 0xFF )
	{
		return;							 
	}

	ProcessTauntPause -= framelag;
	
	if ( ProcessTauntPause < 0.0F )
	{
		ProcessTauntPause = TAUNT_PAUSE;
		if ( !UpdateTaunt( TauntID, Ships[ Taunter ].Object.Group, &Ships[ Taunter ].Object.Pos ) )
		{
			TauntID = 0;	// shouldn't be needed
			Taunter = 0xFF;
			TauntUpdatable = false;
		}
	}
}

void ProcessEnemyBikerTaunt( void )
{
	if ( !TauntID  || !TauntUpdatable )
	{
		return;							 
	}

	ProcessTauntPause -= framelag;
	
	if ( ProcessTauntPause < 0.0F )
	{
		ProcessTauntPause = TAUNT_PAUSE;
		if( EnemyTaunter )
		{
			if ( !UpdateTaunt( TauntID, EnemyTaunter->Object.Group, &EnemyTaunter->Object.Pos ) )
			{
				TauntID = 0;	// shouldn't be needed
				TauntUpdatable = false;
			}
		}
	}
}

#define TAUNT_OVERIDE_DISTANCE_FACTOR 2.0F
#define MAX_TAUNT_DISTANCE 2240.0F 

extern BYTE GameStatus[MAX_PLAYERS + 1];

void SendBikerTaunt()
{
	float dist;
	int i;

	// for every player in the game
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		// excluding myself and those who aren't in normal mode
		if((GameStatus[ i ] ==  0x0a/*STATUS_NORMAL*/) && (i != WhoIAm))
		{
			// if they are close to me
			dist = ReturnDistanceVolumeVector( &Ships[ i ].Object.Pos, Ships[ i ].Object.Group, &Ships[ Current_Camera_View ].Object.Pos, Ships[ Current_Camera_View ].Object.Group, NULL, NULL );
			if( (dist >= 0.0F) && ( dist <= MAX_TAUNT_DISTANCE ))
				// send them speech taunt
				SendGameMessage(MSG_TEXTMSG, Ships[i].network_player, WhoIAm, TEXTMSGTYPE_SpeechTaunt, 0);
		}
	}
}

void PlayUpdatableBikerTaunt( VECTOR *pos, u_int16_t Group, u_int16_t bike, char variant )
{
	float dist;

	dist = ReturnDistanceVolumeVector( pos, Group, &Ships[ Current_Camera_View ].Object.Pos, Ships[ Current_Camera_View ].Object.Group, NULL, NULL );
	if( dist < 0.0F )
		return;
/*
	if ( TauntID && ( dist < ( TauntDist / TAUNT_OVERIDE_DISTANCE_FACTOR ) ) )
	{
		StopSfx( TauntID );
	}
*/
	
	if ( dist > MAX_TAUNT_DISTANCE )
	{
		return;
	}

//	TauntDist = dist;
	ProcessTauntPause = TAUNT_PAUSE;
	PlaySpecificBikerSpeech( SFX_BIKER_TN, Group, pos, 0.0F, bike, variant, true );	// true means update
}

void PlayRecievedSpeechTaunt( BYTE player, char variant )
{
	u_int16_t Group;
	VECTOR *Pos;
	int Taunter = player;
	if( Taunter != 0xFF )
	{
		Group = Ships[ Taunter ].Object.Group;
		Pos = &Ships[ Taunter ].Object.Pos;
		PlaySpecificBikerSpeech( SFX_BIKER_TN, Group, Pos, 0.0F, Ships[ Taunter ].BikeNum, variant, true );	// true means update
	}
}

void StopTaunt( void )
{
	// do not stop taunt if it is your own. ( this will be automatically cut out on death by death cry )
	if ( TauntID && ( Taunter != 0xFF ))
		StopSfx( TauntID );
}

int EnemyBikeNum( u_int16_t type )
{
	switch( type )
	{
	case ENEMY_Bike_Lokasenna:     
			return 0; 
	case ENEMY_Bike_Beard:         
			return 1;
	case ENEMY_Bike_LAJay:    	   
			return 2;
	case ENEMY_Bike_ExCop:     	   
			return 3;
	case ENEMY_Bike_RexHardy:  	   
			return 4;
	case ENEMY_Bike_Foetoid:       
			return 5;
	case ENEMY_Bike_NimSoo:    	   
			return 6;
	case ENEMY_Bike_Nutta:         
			return 7;
	case ENEMY_Bike_Sceptre:       
			return 8;
	case ENEMY_Bike_Jo:        	   
			return 9;
	case ENEMY_Bike_CuvelClark:	   
			return 10;
	case ENEMY_Bike_HK5:       	   
			return 11;
	case ENEMY_Bike_Nubia:         
			return 12;
	case ENEMY_Bike_Mofisto:       
			return 13;
	case ENEMY_Bike_Cerbero:       
			return 14;
	case ENEMY_Bike_Slick:         
			return 15;
	case ENEMY_Bike_FlyGirl:	   
			return 16;
	default:
		return (u_int16_t)-1;
	}
}

void StopEnemyBikerTaunt( ENEMY *Enemy )
{
	u_int16_t bike;

	bike = EnemyBikeNum( Enemy->Type );
	if( bike == (u_int16_t)-1 )
		return;

	if ( TauntID )
		StopSfx( TauntID );

	PlaySpecificBikerSpeech( SFX_Die, Enemy->Object.Group, &Enemy->Object.Pos, 0.0F, bike, -1, false );
}

void PlayEnemyBikerTaunt( ENEMY *Enemy )
{
	u_int16_t bike;

	bike = EnemyBikeNum( Enemy->Type );
	if( bike == (u_int16_t)-1 )
		return;

	EnemyTaunter = Enemy;

	PlayUpdatableBikerTaunt( &EnemyTaunter->Object.Pos, EnemyTaunter->Object.Group, bike, -1 );
}


#endif
