
#include "typedefs.h"
#include <stdio.h>
#include "sfx.h"
#include "new3d.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mload.h"
#include "networking.h"
#include <objbase.h>
#include <cguid.h>
#include "title.h"
#include "text.h"
#include "main.h"
#include "dinput.h"
#include "controls.h"
#include "ships.h"
#include "vfw.h"
#include "config.h"
#include "XMem.h"
#include "SBufferHand.h"
#include "util.h"

void SetBufferFreq( IDirectSoundBuffer *Buffer, float Freq );

#ifdef DEBUG_ON
#define DSLoadSoundBuffer(A, B, C) DSLoadSoundBuffer(A, B, C, __FILE__, __LINE__)
#define DSLoadCompoundSoundBuffer(A, B, C) DSLoadCompoundSoundBuffer(A, B, C, __FILE__, __LINE__)
#endif

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/****************************************
globals
*****************************************/
ENEMY *EnemyTaunter;
BYTE Taunter = 0xFF;
uint32 TauntID = 0;
BOOL TauntUpdatable = FALSE;
BOOL NoCompoundSfxBuffer = FALSE;
//float TauntDist = 0.0F;
char CurrentTauntVariant;
SFX_HOLDER	SfxHolder[ MAX_ANY_SFX ];
BOOL		NoSFX = FALSE;
BOOL		Sound3D = FALSE;
float		GlobalSoundAttenuation = 0.8F;
BOOL CompoundSfxAllocated[MAX_SFX];

int NumDupCompoundBuffers;

BOOL bSoundEnabled = FALSE;

LPDIRECTSOUND           lpDS = NULL;
LPDIRECTSOUND3DLISTENER	lpDS3DListener;
LPDIRECTSOUNDBUFFER		glpPrimaryBuffer;
DSCAPS					DSCaps;
int	MaxCompoundSfx;
uint32 CurrentBikerSpeech = 0;
uint32 CurrentBikeCompSpeech = 0;

float	LastDistance[MAX_SFX];

int Num_SndObjs;


int CurrentLevel = 16;
SNDOBJ *SndObjs[MAX_SFX];
SBUFFERLIST SBufferList[ MAX_SYNCHRONOUS_DYNAMIC_SFX ];

// buffer linked list pointers...
SBUFFER_LIST *SBufferList_Start = NULL;
SBUFFER_LIST *SBufferList_Current = NULL;

SNDOBJ *SndObjList_Static_Start;

SPOT_SFX_LIST SpotSfxList[ MAX_LOOPING_SFX ];

SFX_THREAD_INFO SfxThreadInfo[MAX_THREADED_SFX];

int sfxref = 0;
int dupbufref = 0;
char TauntPath[ 128 ];

DWORD CompoundSfxTimeStamp[64];
DWORD CompoundSfxMaxLag = 0;

BOOL BikerSpeechPlaying = FALSE;
BOOL FreeHWBuffers;

COMPOUND_SFX_INFO CompoundSfxBuffer[MAX_COMPOUND_BUFFERS];

TEMPSFXINFO TempSfxInfo[MAX_COMPOUND_SFX];

SFX_PLAYING SfxPlaying[ MAX_CONCURRENT_SFX ];

SNDLOOKUP SndLookup[ MAX_SFX ];

uint32 SfxUniqueID = 1;

char *CompoundSfxFilename = "sfx\\Compound.wav";

#define MAX_SFX_VARIANTS 16
char *SfxFullPath[ MAX_SFX ][ MAX_SFX_VARIANTS];

/****************************************
Externals
*****************************************/
extern 	ENEMY Enemies[];
extern SLIDER BikeCompSpeechSlider;
extern SLIDER BikerSpeechSlider;
extern USERCONFIG	*player_config;
extern VECTOR	SlideUp;
extern VECTOR	Forward;
extern GLOBALSHIP	Ships[MAX_PLAYERS+1];
extern BYTE	Current_Camera_View;		// which object is currently using the camera view....
extern float	SoundInfo[MAXGROUPS][MAXGROUPS];
extern SLIDER  SfxSlider; 
extern DWORD CompoundSfxDataRate;

extern DWORD CompoundSfxBitDepth;
extern DWORD CompoundSfxFrequency;
extern DWORD CompoundSfxChannels;
extern USERCONFIG *player_config;

extern float framelag;	
extern LEVEL_LOOKUP LevelLookup[];
extern char	ShortLevelNames[MAXLEVELS][32];
extern LIST	BikeList;
extern LIST BikeComputerList;

/****************************************
Fn Prototypes
*****************************************/
BOOL Init_SoundGlobals(void);
void FreeSBufferList( void );
void InitSfxHolders( void );
void SetPannedBufferParams( IDirectSoundBuffer *pDSB, IDirectSound3DBuffer *pDSB3D, VECTOR *SfxPos, float Freq, VECTOR *Temp, float Distance, long Volume, uint16 Effects );
int FindFreeBufferSpace( SNDOBJ *SndObj, float Distance );

SFXNAME		Sfx_Filenames[MAX_SFX] =
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
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_AM  }, //	-	assassin missile
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_AP  }, //	-	picking up a weapon which is already present
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_BL  }, //	-	beam laser
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_BN  }, //	-	bad navigation
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CA  }, //	-	camping
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CD  }, //	-	chaff dispenser
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_CS  }, //	-	chaos shield
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_DY  }, //	-	destroying yourself
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_EA  }, //	-	extra ammo
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_EX  }, //	-	extra, miscellaneous phrases
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_FL  }, //	-	flares
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GK  }, //	-	good kill total
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GL  }, //	-	general ammo low
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GM  }, //	-	gravgon missile
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_GP  }, //	-	golden power pod
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_HC  }, //	-	hull critical
	{ NULL , SFX_Dynamic | SFX_BikeComp | SFX_BikeCompNoOveride, 0, SFX_BIKECOMP_LOOKUP_IN  }, //	-	incoming
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_IR  }, //	-	IR goggles
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MA  }, //	-	maximum ammo
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MK  }, //	-	many kills in a short time period
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MR  }, //	-	MRFL
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_MU  }, //	-	mug
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NK  }, //	-	no kills for a lengthy time period
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NL  }, //	-	nitro low
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NP  }, //	-	selecting a weapon which is not present
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_NT  }, //	-	nitro
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_OP  }, //	-	orbit pulsar
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PG  }, //	-	petro gel
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PK  }, //	-	poor kill total
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PL  }, //	-	pyrolite fuel low
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PM  }, //	-	pine mine
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PO  }, //	-	power pod
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PP  }, //	-	plasma pack
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PR  }, //	-	purge mine
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PS  }, //	-	pulsar
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_PY  }, //	-	pyrolite
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_QM  }, //	-	quantum mine
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_RR  }, //	-	resnic reanimator
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SA  }, //	-	scatter missile
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SC  }, //	-	shield critical
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SG  }, //	-	suss-gun
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SH  }, //	-	shield
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SI  }, //	-	scatter missile impact
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SL  }, //	-	suss gun ammo low
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SM  }, //	-	smoke streamer
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SO  }, //	-	spider mine
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_SP  }, //	-	solaris heatseaker
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_ST  }, //	-	stealth mantle
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TI  }, //	-	titan star missile
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TR  }, //	-	transpulse
	{ NULL , SFX_Dynamic | SFX_BikeComp, 0, SFX_BIKECOMP_LOOKUP_TX  }, //	-	trojax

	/************************************************
	Biker
	*************************************************/
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_GP  }, //	-	general 
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_VP  }, //	-	victory
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_LP  }, //	-	losing
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_BW  }, //	-	big weapon gain
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_LE  }, //	-	low energy
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_TN  }, //	-	taunt
	{ NULL, SFX_Dynamic | SFX_Biker, 0, SFX_BIKER_LOOKUP_PN  }, //	-	pain
	{ NULL, SFX_Dynamic | SFX_Biker | SFX_BikerSpeechOveride, 0, SFX_BIKER_LOOKUP_DT  }, //	-	death
	{ NULL, SFX_Dynamic | SFX_Biker | SFX_Title, 0, SFX_BIKER_LOOKUP_EX  }, //	-	extra 
  
	{ NULL, SFX_LevelSpec, 0, 0 }, // first level spec sfx

};

//LEVELSPEC_SFX_FILES LevelSpecificEffects[ MAX_LEVELSPECIFIC_SFX ];

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

// if there is 3D sound, commit 3D sound processing
void commit_any_sounds( void )
{
	if ( lpDS3DListener )
		if ( IDirectSound3DListener_CommitDeferredSettings(lpDS3DListener) != DS_OK )
			DebugPrintf("Error commiting 3D\n");
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

	if ( ( SfxHolder[ index ].SfxFlags == SFX_HOLDERTYPE_Dynamic ) || ( SfxHolder[ index ].SfxFlags == SFX_HOLDERTYPE_Taunt ) )
		SBufferList[ SfxHolder[ index ].SfxBufferIndex ].used = FALSE;

	if( SfxHolder[ index ].SfxFlags == SFX_HOLDERTYPE_Taunt )
	{
		TauntID = 0;
		Taunter = 0xFF;
		TauntUpdatable = FALSE;
		EnemyTaunter = NULL;
	}
	
	SfxHolder[ index ].Used = FALSE;
}

/****************************************
	Procedure	:		TimerProc ( Callback function )
	description	:		stops a compound buffer after required portion has been played
	Input		:		UINT uID:	unique ID of timer calling function
						UINT uMsg:	not used
						DWORD dwUser:	low 2 bytes = compound buffer number
										high 2 bytes = buffer type flags
	Output		:		none
*****************************************/
void CALLBACK TimerProc( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 )
{
	if (dwUser != (DWORD)-1 )
	{
		if ( CompoundSfxBuffer )
		{
			if ( CompoundSfxBuffer[dwUser].buffer )
			{
				// current_sfx could have been set to -1 in KillCompoundSfx ( inside critical section )
				// safe to ignore, since timer was killed off
				if ( CompoundSfxBuffer[dwUser].current_sfx != -1 )																
				{
					IDirectSoundBuffer_Stop( CompoundSfxBuffer[dwUser].buffer );
					if ( IDirectSoundBuffer_SetFrequency( CompoundSfxBuffer[dwUser].buffer, DSBFREQUENCY_ORIGINAL ) != DS_OK )
					{
						DebugPrintf("set frequency failed ( 1 )\n");
					}
					SndObjs[ SndLookup[ CompoundSfxBuffer[ dwUser ].current_sfx ].SndObjIndex + CompoundSfxBuffer[ dwUser ].current_variant ]->CompoundBufferLookup[ CompoundSfxBuffer[dwUser].compound_buffer_lookup_index ] = -1;
					CompoundSfxBuffer[dwUser].current_sfx = -1;
					FreeSfxHolder( CompoundSfxBuffer[ dwUser ].SfxHolderIndex );
				}
			}
		}
	}
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

/****************************************
	Procedure	: ProcessSoundRoutines		
	description	: runs continuously while sfx are active - loads sound buffers when SfxThreadInfo contains valid information
	Input		: void *pParm - not used
	Output		: none
*****************************************/
void ProcessSoundRoutines (void * pParm)
{
	IDirectSoundBuffer* TempSndBuf;
	int i;
	char *file;

	for ( i = 0; i < MAX_THREADED_SFX; i++ )
	{
		if (SfxThreadInfo[ i ].SfxToPlay)
		{

			switch( SfxThreadInfo[ i ].SfxType )
			{
			case SFX_TYPE_Looping:
				file = SfxFullPath[ SfxThreadInfo[ i ].SfxNum ][ SfxThreadInfo[ i ].Variant ];
				break;
			case SFX_TYPE_Taunt:
				file = TauntPath;
				break;
			default:
				file = SfxFullPath[ SfxThreadInfo[ i ].SfxNum ][ Random_Range( SndLookup[ SfxThreadInfo[ i ].SfxNum ].Num_Variants )];
			}

			//DebugPrintf( "ProcessSoundRoutines SfxTypeLooping: %s, file: %s\n",
			//			(SfxThreadInfo[i].SfxType == SFX_TYPE_Looping ? "true" : "false"),	file);

			// create temporary sound buffer - will have volume, frequency & pan facilities 
			// - located in sw ( any hardware will have been used by now )
			// - static ( not streamed )
			TempSndBuf = DSLoadSoundBuffer(lpDS, file, DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE);
			if ( !TempSndBuf )
			{
				DebugPrintf( "Sfx Load failed...in %s \n", file );
				SfxThreadInfo[ i ].SfxToPlay = FALSE;
				SfxThreadInfo[ i ].SfxType = 0;
				continue;
			}

			if ( SfxThreadInfo[ i ].SfxType == SFX_TYPE_Normal )
			{
				// set buffer parameters & play
				IDirectSoundBuffer_SetVolume( TempSndBuf, ( SfxThreadInfo[ i ].Vol > 0 ) ? 0 : SfxThreadInfo[ i ].Vol );
				IDirectSoundBuffer_Play(TempSndBuf, 0, 0, 0);

				// add to list of dynamic buffers
				SfxHolder[ SfxThreadInfo[ i ].SfxHolderIndex ].SfxBufferIndex = AddToSBufferList( TempSndBuf, NULL, SfxThreadInfo[ i ].SfxHolderIndex );

				// if unable to store buffer, free up sfx holder
				if ( SfxHolder[ SfxThreadInfo[ i ].SfxHolderIndex ].SfxBufferIndex < 0 )
				{
					FreeSfxHolder( SfxThreadInfo[ i ].SfxHolderIndex );
					DebugPrintf("Unable to store buffer(1)!\n");
				}
			}

			if ( ( SfxThreadInfo[ i ].SfxType == SFX_TYPE_Panned ) || ( SfxThreadInfo[ i ].SfxType == SFX_TYPE_Taunt ) )
			{
				// set buffer parameters & play
				SetPannedBufferParams( TempSndBuf, NULL, &SfxThreadInfo[ i ].SfxVector, SfxThreadInfo[ i ].SfxFreq, &SfxThreadInfo[ i ].SfxTempVector, SfxThreadInfo[ i ].SfxDistance, SfxThreadInfo[ i ].Vol, SfxThreadInfo[ i ].Effects );
				IDirectSoundBuffer_Play( TempSndBuf, 0, 0, 0 );

				// add to list of dynamic buffers
				SfxHolder[ SfxThreadInfo[ i ].SfxHolderIndex ].SfxBufferIndex = AddToSBufferList( TempSndBuf, NULL, SfxThreadInfo[ i ].SfxHolderIndex );
				
				if ( SfxHolder[ SfxThreadInfo[ i ].SfxHolderIndex ].SfxBufferIndex < 0 )
				{
					// if unable to store buffer, free up sfx holder
					FreeSfxHolder( SfxThreadInfo[ i ].SfxHolderIndex );
					DebugPrintf("Unable to store buffer(2)!\n");
				}
			}

			if ( SfxThreadInfo[ i ].SfxType == SFX_TYPE_Looping )
			{
				DSBCAPS dsbcaps; 

				DebugPrintf( "-- adding SpotSfx %d onto SpotSfxList, buffer is %s\n",
								SfxThreadInfo[ i ].SpotSfxListIndex,
								(TempSndBuf)?"GOOD":"BAD");

				// get caps of buffer...
				dsbcaps.dwSize = sizeof( DSBCAPS );
				IDirectSoundBuffer_GetCaps( TempSndBuf, &dsbcaps );

				SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].buffer = TempSndBuf;
				SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].buffer3D = NULL;
				SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].buffersize = dsbcaps.dwBufferBytes;

				SetBufferFreq( 
							SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].buffer,
							SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].freq );

				// indicate that thread info index is no longer valid
				SpotSfxList[ SfxThreadInfo[ i ].SpotSfxListIndex ].SfxThreadInfoIndex = -1;
			}

			// mark current thread info as free
			SfxThreadInfo[ i ].SfxToPlay = FALSE;
			SfxThreadInfo[ i ].SfxType = 0;
		}
	}
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
	Input		: uint16 sfx: Sfx index ( SfxFilenames )
				  char *sfxname: Sfx name
	Output		: TRUE / FALSE
	notes		: <name> exists if <path>/name.wav or <path>/name01.wav exists
				  this implies that sfx with variants must be numbered 01 to x.
*****************************************/
BOOL SfxExists( uint16 sfx, char *name )
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
		return TRUE;

	// get pointer to extension
	if ( ! (ptr = strstr(fullpath,".wav")))
	  return FALSE;

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
		return TRUE;

	// failure
	return FALSE;

}


BOOL IsDigit( char check )
{
	if ( ( check >= 48 ) && ( check <= 57 ) )
		return TRUE;
	else
		return FALSE;
}

/****************************************
	Procedure	: ReturnSFXIndex
	description	: returns an index to Sfx_Filenames for the requested sfx.
				  if sfx does not already exist in table, sfx is added as 
				  a level specific sfx at the end of the table.
	Input		:  char *file: Sfx name
	Output		: int16: index no.
*****************************************/

int16 ReturnSFXIndex( char *file )
{
	int16 i, level_spec_flags, filelen;
	BOOL exists = FALSE;
	char *suffix;
	char filetocheck[ 256 ];
	BOOL found;

	// if zero length string, return error
	if( !file || !file[0] )
		return -1;

	// remove .wav suffix if given
	suffix = strstr( file, ".wav" );
	if ( suffix )
		*suffix = 0;

	// check all non level specific sfx for file
	for( i = 0; i < SFX_LEVELSPEC_Start; i++ )
		if ( Sfx_Filenames[ i ].Name && ( !_stricmp( Sfx_Filenames[ i ].Name, file ) ) )
			if ( SfxExists( i, file ) )
			{
				RequestSfx( i );
				return i;
			}
			else
				return -1;

	// check all existing level specific sfx for file
	for ( i = SFX_LEVELSPEC_Start; i <= SFX_LEVELSPEC_End; i++ )
		//if ( LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file[0] )
		if ( Sfx_Filenames[ i ].Name )
		{
			//if ( !_stricmp( LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file, file ) )
			if ( !_stricmp( Sfx_Filenames[ i ].Name, file ) )
			{
				RequestSfx( i );
				return i;
			}
		}else
			break;
		
	// look up sfx in table of possible level spec sfx ( neccesary in order to get correct flags )...
	i = 0;
	level_spec_flags = SFX_LevelSpec;
	found = FALSE;

	while (!( LevelSpecificSfxLookup[ i ].flags & SFX_End ))
	{
		if ( !_stricmp( LevelSpecificSfxLookup[ i ].file, file ) )
		{
			if ( SfxExists( SFX_LEVELSPEC_Start , file ) )	
			{
				level_spec_flags |= LevelSpecificSfxLookup[ i ].flags;
				found = TRUE;
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
				if ( !_stricmp( LevelSpecificSfxLookup[ i ].file, filetocheck ) )
				{
					if ( SfxExists( SFX_LEVELSPEC_Start , file ) )
					{
						level_spec_flags |= LevelSpecificSfxLookup[ i ].flags;
						found = TRUE;
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

BOOL CreateSndObj( char *file, int sfxnum, int flags )
{
	BOOL BuffersMissing = FALSE;
	int i;

	SndObjs[ Num_SndObjs ] = SndObjCreate( lpDS, file, flags , DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY, sfxnum );

	for ( i = 0; i < MAX_DUP_BUFFERS; i++ )
	{
		if ( !SndObjs[ Num_SndObjs ]->Dup_Buffer[ i ] )
		{
			BuffersMissing = TRUE;
		}
	}

	// if sound buffer could not be loaded for some reason ( & has not already been loaded into hw )...
	if ( ( !SndObjs[ Num_SndObjs ] || BuffersMissing ) && 
		!( SndObjs[ Num_SndObjs ] && SndObjs[ Num_SndObjs ]->CompoundBuffer ) )
  	{
		SndLookup[ sfxnum ].Num_Variants = 0;
		return FALSE;
	}

	Num_SndObjs++;

	return TRUE;
}

/****************************************
	Procedure	: LoadSfx 
	description	: Fills in SndLookup info for the given sfx.
	              If static, also loads sfx ( or sfx's if variants exist )
				  & stores info in SndObjs
	Input		: int sfxnum
	Output		: none
*****************************************/
void LoadSfx( int sfxnum )
{
	char fullpath[256];
	char filename[256] = "";
	int flags, i;

	flags = Sfx_Filenames[ sfxnum ].Flags;

	/*	
	// get correct flags...
	if ( flags & SFX_LevelSpec )
	{
	 	if ( CurrentLevel < 0 )
			return;

		flags = LevelSpecificEffects[ Sfx_Filenames[ sfxnum ].SfxLookup ].flags;
	}
	else		   
	{
		// only load in level specific and biker sfx for title room...
		if ( ( ( MyGameStatus == STATUS_Title ) || ( MyGameStatus == STATUS_BetweenLevels ) ) &&
			(!(flags & SFX_Biker)) )
			return;
	}
	*/



	if ( !SndLookup[ sfxnum ].Num_Variants )
		return;

	if ( SndLookup[ sfxnum ].Num_Variants == 1 )
	{
		// load just one sfx
		if (!( flags & SFX_Dynamic ))
		{
			GetFullSfxPath( fullpath, sfxnum, 1, 1 );

			if ( !CreateSndObj( fullpath, sfxnum, flags ))
			{
				DebugPrintf( "Error loading sfx %d - marking sound as invalid\n", sfxnum, SndLookup[ sfxnum ].Num_Variants );
				return;
			}
		}
	}else
	{
		for ( i = 0; i < SndLookup[ sfxnum ].Num_Variants; i++ )
		{
			// if static, load & store SndObj...
			if (!( flags & SFX_Dynamic ) )
			{
				GetFullSfxPath( fullpath, sfxnum, i, SndLookup[ sfxnum ].Num_Variants );

				if ( !CreateSndObj( fullpath, sfxnum, flags ))
				{
					DebugPrintf( "Error loading sfx %d variant %d - marking sound as invalid\n", sfxnum, SndLookup[ sfxnum ].Num_Variants );
					return;
				}
			}
		}
	}

}


#define NUM_ESSENTIAL_SFX 26

int16 EssentialSfx[ NUM_ESSENTIAL_SFX ] = {
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

int LoadSfxToHW( void )
{
	DSCAPS DSCaps;
	DSBCAPS DSBCaps;
	DWORD FreeMem;
	DWORD flags;
	DWORD buffers_before;
	int AllocatedCompoundSfx;
	int j, i;
	DWORD dwSizeWritten;
	LPWAVEFORMATEX lpwaveinfo; 

   	if ( !MAX_COMPOUND_SFX )
		return 0;

	if ( NoCompoundSfxBuffer )
		return 0;

   	/*
	if ( ( MyGameStatus == STATUS_Title ) || ( MyGameStatus == STATUS_BetweenLevels ) )
	{
		return 0;
	}
	*/

	AllocatedCompoundSfx = 0;
	
	NumDupCompoundBuffers = 0;

	memset( CompoundSfxAllocated, 0, MAX_SFX * sizeof ( BOOL ) );

	memset (&DSCaps, 0, sizeof (DSCAPS));
	DSCaps.dwSize = sizeof(DSCAPS);
	IDirectSound_GetCaps( lpDS, &DSCaps );

	if ( DSCaps.dwMaxContigFreeHwMemBytes > MIN_SOUNDCARD_HW_MEM )
	{
		FreeMem = DSCaps.dwMaxContigFreeHwMemBytes;
		flags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCHARDWARE | DSBCAPS_CTRLFREQUENCY; 
		DebugPrintf("Loading compound sfx buffer in HW\n");
	}else
	{
		FreeMem = 1;	// so that it equates to TRUE
		flags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		DebugPrintf("Loading compound sfx buffer in SW\n");
	}

	// if we have hardware mixing channels & hardware mem & not in titles
	if ( FreeMem && ( DSCaps.dwFreeHwMixingStaticBuffers >= MIN_COMPOUND_BUFFERS ) )
	{
		buffers_before = DSCaps.dwFreeHwMixingStaticBuffers;

		// load first compound buffer
		CompoundSfxBuffer[0].buffer = DSLoadCompoundSoundBuffer(lpDS, flags, &AllocatedCompoundSfx );

		// if buffer succesfully loaded...
		if ( CompoundSfxBuffer[0].buffer )
		{
			/*
			// get caps after creating buffer, to check that buffer is using hw mixing channel
			memset (&DSCaps, 0, sizeof (DSCAPS));
			DSCaps.dwSize = sizeof(DSCAPS);
			IDirectSound_GetCaps( lpDS , &DSCaps );
			*/

			memset (&DSBCaps, 0, sizeof( DSBCAPS ) );
			DSBCaps.dwSize = sizeof( DSBCAPS );
			IDirectSoundBuffer_GetCaps( CompoundSfxBuffer[ 0 ].buffer, &DSBCaps );

			//if ( buffers_before == DSCaps.dwFreeHwMixingStaticBuffers)
			if (!( DSBCaps.dwFlags & DSBCAPS_LOCHARDWARE ))
			{
   				// no point in using compound buffer, since HW mixing channels are not being used
				DebugPrintf("Crap sound driver detected - Buffers stored are not using HW mixing channels, therefor compound buffer will not be created\n");

				// kill this buffer...
				SoundBufferRelease( &CompoundSfxBuffer[0].buffer );
				DebugPrintf("Releasing sound buffer %s %d\n", __FILE__, __LINE__ );

				// mark all newly allocate sfx as not part of compound buffer...
				for ( j = 0; j < AllocatedCompoundSfx; j++ )
				{
					CompoundSfxAllocated[ TempSfxInfo[j].SfxNum ] = FALSE;
				}
			}else
			{
				// get size of WAVEFORMAT structure to be returned ( is variable size, as stated in DSound docs ) & allocate memory for it
				IDirectSoundBuffer_GetFormat( CompoundSfxBuffer[0].buffer, NULL, 0, &dwSizeWritten );
				lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );

				// get data rate of compound buffer
				IDirectSoundBuffer_GetFormat( CompoundSfxBuffer[0].buffer, lpwaveinfo, dwSizeWritten, 0 );
				CompoundSfxDataRate = lpwaveinfo->nAvgBytesPerSec; 

				free(lpwaveinfo);

				CompoundSfxBuffer[0].current_sfx = -1;

				// duplicate for rest of buffers ( limit number of buffers to MAX_COMPOUND_BUFFERS )
				NumDupCompoundBuffers = DSCaps.dwFreeHwMixingStaticBuffers;

				if ( NumDupCompoundBuffers > MAX_COMPOUND_BUFFERS )
					NumDupCompoundBuffers = MAX_COMPOUND_BUFFERS;

				for (j = 1; j < NumDupCompoundBuffers; j++)
				{
					if ( !SoundBufferDuplicate(lpDS, CompoundSfxBuffer[0].buffer, &CompoundSfxBuffer[j].buffer ) )
					{	
						DebugPrintf("unable to duplicate more than %d compound buffers\n",j);
				
						// if insufficient buffers created...
						if ( j < MIN_COMPOUND_BUFFERS )
						{
							// free all created buffers
							for ( i = 0; i < j; i++ )
							{
								 IDirectSoundBuffer_Release( CompoundSfxBuffer[ i ].buffer );
							}

							// return 0 to indicate no hw buffers
							return 0;
						}

						break;
					}

					CompoundSfxBuffer[j].current_sfx = -1;
				}

				NumDupCompoundBuffers = j;

				memset (&DSCaps, 0, sizeof (DSCAPS));
				DSCaps.dwSize = sizeof(DSCAPS);
				IDirectSound_GetCaps( lpDS , &DSCaps );
			}
		}else
		{
			DebugPrintf("unable to create compound buffer\n");
		}
	}else
	{
		DebugPrintf("not loading compound sfx buffer becuase not enough mixing channels or not enough free memory in sound driver.\n");
	}

	return AllocatedCompoundSfx;
}

void PauseAllSfx( void )
{
	uint16 i;
	SPOT_SFX_LIST temp;
	
	// looping sfx info has to be stored for later retriggering
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( SpotSfxList[ i ].used )
		{
			temp = SpotSfxList[ i ];
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

BOOL RestoreSfxData( uint32 id, VECTOR *pos, uint16 *group )
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
				return TRUE;
			}else
			{
				Msg("trying to restore group & pos addresses for non-variable looping sfx!\n");
				return FALSE;
			}
		}
	}

	return FALSE;
}

void ReTriggerSfx( void )
{
#if 0
	uint16 i, j;
	uint32 tempuid[ MAX_LOOPING_SFX ];

	SfxUniqueID = 1;

	// find highest unique id
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( TempSpotSfxList[ i ].uid > SfxUniqueID )
		{
			SfxUniqueID = TempSpotSfxList[ i ].uid;
		}
	}

	memset( tempuid, 0, sizeof( uint32 ) * MAX_LOOPING_SFX );

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
	uint32 tempuid;
	uint16 i, j;

	// re-trigger all existing looping sfx...
	for ( i = 0; i < MAX_LOOPING_SFX; i++ )
	{
		if ( SpotSfxList[ i ].used )
		{
			temp = SpotSfxList[ i ];
			SpotSfxList[ i ].used = FALSE;
			
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
			}else
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
	}else
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
		SndLookup[ i ].SndObjIndex = 0;
		SndLookup[ i ].Requested = FALSE;
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

void RequestSfx( int16 sfxnum )
{
	SndLookup[ sfxnum ].Requested = TRUE;
}

uint16 BikeSpeechVariants[ SFX_NUM_BIKE_PHRASES ][ MAXBIKETYPES ];
uint16 BikeCompVariants[ SFX_NUM_BIKECOMP_PHRASES ][ MAXBIKECOMPTYPES ];

void UpdateSfxForBiker( uint16 biker )
{
	int i, j;
	char file[ 128 ];
	
	for( i = 0; i < SFX_NUM_BIKE_PHRASES; i++ )
	{
		SndLookup[ i + SFX_BIKER_START ].Num_Variants = BikeSpeechVariants[ i ][ biker ];

		for ( j = 0; j < MAX_SFX_VARIANTS; j++ )
		{
			if( SfxFullPath[ i + SFX_BIKER_START ][ j ] )
			{
				free( SfxFullPath[ i + SFX_BIKER_START ][ j ] );
				SfxFullPath[ i + SFX_BIKER_START ][ j ] = NULL;
			}
		}

		for( j = 0; j < SndLookup[ i + SFX_BIKER_START ].Num_Variants; j++ )
		{
			GetFullSfxPath( file, i + SFX_BIKER_START, j, SndLookup[ i + SFX_BIKER_START ].Num_Variants );
			SfxFullPath[ i + SFX_BIKER_START ][ j ] = ( char * )malloc( strlen( file ) + 1 );
			strcpy( SfxFullPath[ i + SFX_BIKER_START ][ j ], file );
		}

	}
}

void UpdateSfxForBikeComputer( uint16 bikecomp )
{
	int i, j;
	char file[ 128 ];

	for( i = 0; i < SFX_NUM_BIKECOMP_PHRASES; i++ )
	{
		SndLookup[ i + SFX_BIKECOMP_START ].Num_Variants = BikeCompVariants[ i ][ bikecomp ];

		for ( j = 0; j < MAX_SFX_VARIANTS; j++ )
		{
			if( SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] )
			{
				free( SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] );
				SfxFullPath[ i + SFX_BIKECOMP_START ][ j ] = NULL;
			}
		}

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
		// all speech must be dynamic!!!
		if ( ( Sfx_Filenames[ i ].Flags & SFX_Biker ) || ( Sfx_Filenames[ i ].Flags & SFX_BikeComp ) )
			Sfx_Filenames[ i ].Flags |= SFX_Dynamic;
		
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
	uint16 i, current;

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
	uint16 i;
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
BOOL InitializeSound( int flags )
{
	int i;
	int j;
	uint16 Num_Sfx = 0;
	int AllocatedCompoundSfx;

#if 0
	// neede for locking sound buffer
	DSCAPS DSCaps;
	LPVOID lpvAudioPtr1;
	DWORD dwAudioBytes1;
	LPVOID lpvAudioPtr2;  
	DWORD dwAudioBytes2;  
	DWORD dwFlags = DSBLOCK_ENTIREBUFFER;
#endif
	
	// check sound is not already initialised
	if ( bSoundEnabled )
	{
		DebugPrintf("Tried to init sound before destroying!\n");
		return FALSE;
	}
	
	DebugPrintf("Initializing sound.\n");

	// check sound is valid
	if( NoSFX || !Init_SoundGlobals() )
	{
		bSoundEnabled = FALSE;
		return TRUE;
	}
	else
	{
		bSoundEnabled = TRUE;
	}

	PreInitSfx();

	FreeHWBuffers = TRUE;

	Num_Sfx = 0;

	PreProcessSfx();
	
	// initialise SndObjs
	Num_SndObjs = 0;
	memset (SndObjs, 0, sizeof( SNDOBJ * ) * MAX_SFX);

	// initialise taunt stuff
	Taunter = 0xFF;
	TauntID = 0;
	TauntUpdatable = FALSE;
	EnemyTaunter = NULL;
	//TauntDist = 0.0F;

	// try to load hw sfx
	AllocatedCompoundSfx = LoadSfxToHW();
	
	// load sfx
	for( Num_Sfx = 0; Num_Sfx < MAX_SFX; Num_Sfx++ )
		if ( SndLookup[ Num_Sfx ].Requested )
		{
			SndLookup[ Num_Sfx ].SndObjIndex = Num_SndObjs;
			LoadSfx( Num_Sfx );
		}

	// store start/stop info for all compound sfx....( not for title )
	for (j = 0; j < AllocatedCompoundSfx; j++)
	{
		SndObjs[ SndLookup[ TempSfxInfo[j].SfxNum ].SndObjIndex + TempSfxInfo[j].Variant ]->StartPos = TempSfxInfo[j].StartPos;
		SndObjs[ SndLookup[ TempSfxInfo[j].SfxNum ].SndObjIndex + TempSfxInfo[j].Variant ]->Length = TempSfxInfo[j].Length;
		SndObjs[ SndLookup[ TempSfxInfo[j].SfxNum ].SndObjIndex + TempSfxInfo[j].Variant ]->Bytes = TempSfxInfo[j].Bytes;
	}

	for ( i = 0; i < MAX_THREADED_SFX; i++ )
	{
		SfxThreadInfo[ i ].SfxToPlay = FALSE;
	}

	InitSfxHolders();

	// re-initialise looping sfx list ( if already in level, we need to restart all existing looping sfx )
	if ( flags & DESTROYSOUND_KeepLevelSpecTable)
	{
		ReTriggerSfx();
	}else
	{
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
		{
			SpotSfxList[ i ].used = FALSE;
		}
	}


#if 0
	InitUsedMemList();

	memset (&DSCaps, 0, sizeof (DSCAPS));
	DSCaps.dwSize = sizeof(DSCAPS);
	IDirectSound_GetCaps( lpDS, &DSCaps );

   	
	if ( CompoundBufferInSysMem && AllocatedCompoundSfx )	// if no hw mem exists, any compound buffer will be in sys mem
	{
		// lock compound buffer...
		IDirectSoundBuffer_Lock( CompoundSfxBuffer[ 0 ].buffer, 0, 0, &lpvAudioPtr1, &dwAudioBytes1,
			&lpvAudioPtr2, &dwAudioBytes2, dwFlags );
		
		MakeRegionPresent( ( BYTE * )lpvAudioPtr1, (UINT)dwAudioBytes1 );

		IDirectSoundBuffer_Unlock( CompoundSfxBuffer[ 0 ].buffer, lpvAudioPtr1, 0, lpvAudioPtr2, 0 );

	}
#endif

	return TRUE;
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

void StopCompoundSfx( void )
{
	int i;

	if ( !lpDS || !bSoundEnabled || !NumDupCompoundBuffers )
		return;

	for ( i = 0; i < NumDupCompoundBuffers; i++ )
	{
		if ( CompoundSfxBuffer[ i ].current_sfx != -1 )
		{
		 	IDirectSoundBuffer_Stop( CompoundSfxBuffer[ i ].buffer );
			CompoundSfxBuffer[ i ].current_sfx = -1;
		}
	}
}

/*===================================================================
	Procedure	:	Destroy all sound fx...
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DestroySound( int flags )
{
	int i, j;

	ClearLevelSpecSfx();

	if ( !lpDS || !bSoundEnabled )
		return;

	for ( i = 0; i < MAX_SFX; i++ )
		for( j = 0; j < MAX_SFX_VARIANTS; j++ )
			if( SfxFullPath[ i ][ j ] )
				free( SfxFullPath[ i ][ j ] );

	FreeSBufferList();


	// kill off compound buffers ( HW & SW ) - only if no flags specified
	// kill all sfx timers...

	for ( i = 0; i < NumDupCompoundBuffers; i++ )
	{
		if ( CompoundSfxBuffer[ i ].current_sfx != -1 )
		{
			IDirectSoundBuffer_Stop( CompoundSfxBuffer[ i ].buffer );
			timeKillEvent( CompoundSfxBuffer[ i ].timerID ); 
		}

		SoundBufferRelease( &CompoundSfxBuffer[ i ].buffer );
		CompoundSfxBuffer[ i ].buffer = NULL;
	}
	NumDupCompoundBuffers = 0;
		
	// free all original buffers...
	for (i = 0; i < Num_SndObjs; i++)
	{
		if (SndObjs[i])
		{
			SndObjDestroy(SndObjs[i]);
			SndObjs[i] = NULL;
		}
	}

	// clear all sfx holders...
	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		FreeSfxHolder( i );
	}
	
	if ( !( flags & DESTROYSOUND_KeepLevelSpecTable) )
	{
		// clear level specific sfx table...
		for ( i = SFX_LEVELSPEC_Start; i <= SFX_LEVELSPEC_End; i++ )
		{
			//LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].file[0] = 0;
			//LevelSpecificEffects[ i - SFX_LEVELSPEC_Start ].flags = 0;

			if ( Sfx_Filenames[ i ].Name )
			{
				free( Sfx_Filenames[ i ].Name );
				Sfx_Filenames[ i ].Name = NULL;
			}
			Sfx_Filenames[ i ].Flags = 0;
		}
	}

	Sfx_Filenames[ SFX_LEVELSPEC_Start ].Flags = SFX_LevelSpec;

	// release primary buffer...
	SoundBufferRelease( &glpPrimaryBuffer );

	IDirectSound_Release(lpDS);
	bSoundEnabled = FALSE;
}

int FindFreeBufferSpace( SNDOBJ *SndObj, float Distance )
{
	int free_buffer, i;
	DWORD age[MAX_DUP_BUFFERS], oldest, dwStatus;
	clock_t current_time;

	current_time = clock();

	for (i = 0; i < MAX_DUP_BUFFERS; i++)
	{
		
		IDirectSoundBuffer_GetStatus( SndObj->Dup_Buffer[i], &dwStatus );

		// if buffer not playing...
		if (!( dwStatus & DSBSTATUS_PLAYING ))
			return i;
		
		age[i] = ((current_time - SndObj->Buffer_TimeStamp[i]) +
			  ((DWORD)SndObj->Buffer_Dist[i]) );
	}

	// if we get here, we must stop an existing buffer...

	// find buffer to stop...
	free_buffer = 0;
	oldest = age[0];
	for (i = 1; i < MAX_DUP_BUFFERS; i++)
	{
		if (age[i] > oldest)
		{
			oldest = age[i];
			free_buffer = i;
		}
	}

	if (Distance > oldest)
		return -1;			// sound is too far away to overwrite any existing sounds...

	// stop buffer...
	IDirectSoundBuffer_Stop(SndObj->Dup_Buffer[free_buffer]);
	// reset current position to start of buffer
	IDirectSoundBuffer_SetCurrentPosition( SndObj->Dup_Buffer[free_buffer], 0 );
	// reset frequency
	if ( IDirectSoundBuffer_SetFrequency( SndObj->Dup_Buffer[free_buffer], DSBFREQUENCY_ORIGINAL ) != DS_OK )
	{
		DebugPrintf("set frequency failed ( 2 )\n");
	}

	FreeSfxHolder( SndObj->SfxHolderIndex[ free_buffer ] );

	return free_buffer;
}

	
void KillCompoundSfxBuffer( int buffer )
{
		
	// kill timer
	timeKillEvent( CompoundSfxBuffer[ buffer ].timerID ); 

	// stop buffer
	IDirectSoundBuffer_Stop( CompoundSfxBuffer[ buffer ].buffer );
	if ( IDirectSoundBuffer_SetFrequency( CompoundSfxBuffer[ buffer ].buffer, DSBFREQUENCY_ORIGINAL ) != DS_OK )
	{
		DebugPrintf("set frequency failed ( 3 )\n");
	}

	SndObjs[ SndLookup[ CompoundSfxBuffer[ buffer ].current_sfx ].SndObjIndex ]->CompoundBufferLookup[ CompoundSfxBuffer[ buffer ].compound_buffer_lookup_index ] = -1;

	CompoundSfxBuffer[ buffer ].current_sfx = -1;

	FreeSfxHolder( CompoundSfxBuffer[ buffer ].SfxHolderIndex );

}

BOOL StartPannedSfx(int16 Sfx, uint16 *Group , VECTOR * SfxPos, float Freq, int type, int HolderIndex, float VolModify, uint16 Effects, BOOL OverideDistanceCheck )
{
	VECTOR	Temp;
	float	Distance, MaxDistance;
	float	Modify;
	int free_buffer;
	int i;
	DWORD age, temp_age;
	long Volume;
	int sndobj_index;
	uint16 offset;
	int flags;
	//DWORD dwCurrentPlayPos;

	if( !bSoundEnabled )
		return FALSE;

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
		return FALSE;
	
	switch ( SndLookup[ Sfx ].Num_Variants )
	{
	case 0:
		//DebugPrintf("Sfx.c: PlaySfx() - sfx #%d does not exist!\n", Sfx);
		return FALSE;
	case 1:
		sndobj_index = SndLookup[ Sfx ].SndObjIndex;
		offset = 0;
		break;
	default:
		offset = Random_Range( SndLookup[ Sfx ].Num_Variants );
		sndobj_index = SndLookup[ Sfx ].SndObjIndex + offset;
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
			}else
			{
				return FALSE;
			}

		}else
			CurrentBikerSpeech = SfxHolder[ HolderIndex ].UniqueID;
	}

	if ( flags & SFX_BikeComp )
	{
		if ( CurrentBikeCompSpeech )
		{
			if( flags & SFX_BikeCompNoOveride )
			{
				return FALSE;
			}else
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

		SfxHolder[ HolderIndex ].SfxBufferIndex = InitLoopingSfx( Sfx, offset, Group, SfxPos, Freq, VolModify, type, HolderIndex, Effects, SfxHolder[ HolderIndex ].UniqueID );
		if ( SfxHolder[ HolderIndex ].SfxBufferIndex < 0 )
			return FALSE;

		return TRUE;
	}
	
	if ( type != SFX_2D )
	{
		// work out sound distance...
		if( Ships[ Current_Camera_View ].Object.Group != (uint16) -1 )
		{
			Modify= SoundInfo[Ships[ Current_Camera_View ].Object.Group][*Group];
		}else{
			Modify = 0.0F;
		}
		if( Modify < 0.0F )
			return FALSE;

		Temp.x = SfxPos->x - Ships[ Current_Camera_View ].Object.Pos.x;
		Temp.y = SfxPos->y - Ships[ Current_Camera_View ].Object.Pos.y;
		Temp.z = SfxPos->z - Ships[ Current_Camera_View ].Object.Pos.z;

		Distance = (float) sqrt( ( Temp.x * Temp.x ) + ( Temp.y * Temp.y ) + ( Temp.z * Temp.z ) );

		Distance += Modify;

		MaxDistance = 24 * 1024 * GLOBAL_SCALE;

		//Distance += (MaxDistance - Distance) - (MaxDistance - Distance) * GlobalSoundAttenuation;

		if( Distance >= MaxDistance ) 
			return FALSE;

		// check to see if another sfx of the same type has already been started...if it has but is further away..override it...
		if( !OverideDistanceCheck && (Distance+25.0F) >= LastDistance[Sfx] ) 
			return FALSE;
	}else
		Distance = 0.0F;

	LastDistance[Sfx] = Distance;

	if ( !( flags & SFX_Dynamic ) )
	{
		if ( SndObjs[ sndobj_index ]->CompoundBuffer )
		{
			// check for free buffer index...
			int buffer_lookup = -1;
			DWORD time_left = 0;
			int oldest = -1;
			DWORD current_time;
			int buffer, lastkilledbuffer;

			current_time = GetTickCount();
			age = 0;

			for (i = 0; i < MAX_DUP_BUFFERS; i++)
			{
				// if free buffer found...
				if (SndObjs[ sndobj_index ]->CompoundBufferLookup[ i ] < 0)
				{
					buffer_lookup = i;
					//DebugPrintf("free buffer %d found\n", i);
					break;
				}

				// if this buffer has less time left than the last...
				temp_age = ((current_time - CompoundSfxBuffer[ SndObjs[ sndobj_index ]->CompoundBufferLookup[ i ] ].start_time) +
						  ((DWORD)CompoundSfxBuffer[ SndObjs[ sndobj_index ]->CompoundBufferLookup[ i ] ].distance) );

				if ( temp_age > age )
				{
					age = temp_age;
					oldest = i;
				}

			}

			// if none, free up oldest...
			if (buffer_lookup < 0)	// buffer_lookup not found...
			{
				if( oldest == -1 )
				{
					// all soundfx are same age and just started,also same distance...so dont kill any...
					return FALSE;
				}else{
					lastkilledbuffer = SndObjs[ sndobj_index ]->CompoundBufferLookup[ oldest ];
					KillCompoundSfxBuffer( SndObjs[ sndobj_index ]->CompoundBufferLookup[ oldest ] );
					SndObjs[ sndobj_index ]->CompoundBufferLookup[ oldest ] = -1;
					buffer_lookup = oldest;
				}

			}

			// find free buffer...
			oldest = 0;
			temp_age = 0;
			buffer = -1;
			for (i = 0; i < NumDupCompoundBuffers; i++)
			{
				//if ( i == lastkilledbuffer )
				//	continue;
				
				if ( CompoundSfxBuffer[i].current_sfx < 0 )
				{
					buffer = i;
					break;
				}

				// if this buffer has less time left than last...
				if ( (CompoundSfxBuffer[ i ].finish_time - current_time) > temp_age )
				{
					temp_age = CompoundSfxBuffer[ i ].finish_time;
					oldest = i;	
				}
			}

			// if none, free up oldest
			if (buffer < 0)
			{
				// or if all same age kill the first.......oldest = 0....
				KillCompoundSfxBuffer( oldest );
				buffer = oldest;
			}

			// play sound in buffer, store buffer num in SndObjs->CompoundSfxBuffer...

			// set start pos

			if ( CompoundSfxBuffer[buffer].buffer->lpVtbl->SetCurrentPosition( CompoundSfxBuffer[buffer].buffer, SndObjs[ sndobj_index ]->StartPos ) != DS_OK )
				DebugPrintf("error setting position\n");
			/*
		   	if ( IDirectSoundBuffer_GetCurrentPosition( CompoundSfxBuffer[buffer].buffer, &dwCurrentPlayPos, NULL ) != DS_OK )
				DebugPrintf("error getting position\n");
			else
				DebugPrintf("position set at %d\n", dwCurrentPlayPos);
			*/
			Volume = ( 0 - (long) ( Distance * 0.6F ) );	// Scale it down by a factor...

			Volume = SFX_MIN_VOLUME - (long)( (float)( SFX_MIN_VOLUME - Volume ) * VolModify * GlobalSoundAttenuation );

			if ( type != SFX_2D )
				SetPannedBufferParams( CompoundSfxBuffer[ buffer ].buffer, NULL, SfxPos, Freq, &Temp, Distance, Volume, Effects );
			else
				IDirectSoundBuffer_SetVolume( CompoundSfxBuffer[ buffer ].buffer , ( Volume > 0 ) ?  0 : Volume );
			  
			//DebugPrintf("about to play: start %d buffer %d\n", SndObjs[ sndobj_index ]->StartPos, buffer); 
			IDirectSoundBuffer_Play( CompoundSfxBuffer[ buffer ].buffer, 0, 0, 0 );

			current_time = GetTickCount();
			CompoundSfxBuffer[ buffer ].finish_time = current_time + SndObjs[ sndobj_index ]->Length;
			CompoundSfxBuffer[ buffer ].start_time = current_time;
			
			if ( !Freq || ( Freq == 1.0F ) )
			{
				CompoundSfxBuffer[ buffer ].finish_time = current_time + SndObjs[ sndobj_index ]->Length;
				CompoundSfxBuffer[ buffer ].timerID = timeSetEvent( SndObjs[ sndobj_index ]->Length, SndObjs[ sndobj_index ]->Length, TimerProc, buffer, TIME_ONESHOT ); 
			}
			else
			{
				DWORD BytesPerSec;
				DWORD Length;
				LPWAVEFORMATEX lpwaveinfo;
				DWORD dwSizeWritten, OrigFreq;
	
				// get original frequency of buffer
				IDirectSoundBuffer_GetFormat( CompoundSfxBuffer[buffer].buffer, NULL, 0, &dwSizeWritten );
				lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
				IDirectSoundBuffer_GetFormat( CompoundSfxBuffer[buffer].buffer, lpwaveinfo, dwSizeWritten, 0 );
				OrigFreq = lpwaveinfo->nSamplesPerSec; 
				free(lpwaveinfo);

				// get new frequency
				OrigFreq = (DWORD)( (float)OrigFreq * Freq );

				// calculate length based on given frequency...
				BytesPerSec = (CompoundSfxBitDepth * CompoundSfxChannels / 8) * OrigFreq;
				Length = ( SndObjs[ sndobj_index ]->Bytes * 1000 ) / BytesPerSec;

				CompoundSfxBuffer[ buffer ].finish_time = current_time + Length;
				CompoundSfxBuffer[ buffer ].timerID = timeSetEvent( Length, Length, TimerProc, buffer, TIME_ONESHOT ); 
			}

			if ( !CompoundSfxBuffer[ buffer ].timerID ) 
			{
				// timer not created for some reason - stop buffer to prevent multiple sounds playing!
				IDirectSoundBuffer_Stop( CompoundSfxBuffer[buffer].buffer );
			}else
			{
				CompoundSfxBuffer[ buffer ].current_sfx = Sfx;
				CompoundSfxBuffer[ buffer ].current_variant = offset; 
				CompoundSfxBuffer[ buffer ].compound_buffer_lookup_index = buffer_lookup;
				CompoundSfxBuffer[ buffer ].distance = Distance;
				SndObjs[ sndobj_index ]->CompoundBufferLookup[ buffer_lookup ] = buffer;


				SfxHolder[ HolderIndex ].SfxBufferIndex = buffer;
				SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Compound;
				SfxHolder[ HolderIndex ].SndObjIndex = sndobj_index;
				CompoundSfxBuffer[ buffer ].SfxHolderIndex = HolderIndex; 


			}

			return TRUE;
		}

		// if we get here, sound must be played in sw

		// find free buffer space - if none, kill off relevent buffer
		free_buffer = FindFreeBufferSpace( SndObjs[ sndobj_index ], Distance );
		if (free_buffer < 0)
		{
			//SfxHolder[ HolderIndex ].Used = FALSE;	// mark holder as free			
			return FALSE;	// no buffer returned
		}

		
		
		SfxHolder[ HolderIndex ].SfxBufferIndex = free_buffer;
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Static;
		SfxHolder[ HolderIndex ].SndObjIndex = sndobj_index;
		SndObjs[ sndobj_index ]->SfxHolderIndex[ free_buffer ] = HolderIndex;


		Volume = ( 0 - (long) ( Distance * 0.6F ) );	// Scale it down by a factor...
		Volume = SFX_MIN_VOLUME - (long)( ((float)( SFX_MIN_VOLUME - Volume )) * GlobalSoundAttenuation * VolModify );
	   	if ( type != SFX_2D )
			SetPannedBufferParams( SndObjs[ sndobj_index ]->Dup_Buffer[free_buffer], SndObjs[ sndobj_index ]->Dup_3DBuffer[free_buffer], SfxPos, Freq, &Temp, Distance, Volume, Effects );
		else
			IDirectSoundBuffer_SetVolume( SndObjs[ sndobj_index ]->Dup_Buffer[free_buffer] , ( Volume > 0 ) ? 0 : Volume );
	
		// sfx can only be looping at this point if if is 2D - sfx will just continue to play until stopped.
		if ( flags & SFX_Looping )
			IDirectSoundBuffer_Play( SndObjs[ sndobj_index ]->Dup_Buffer[free_buffer], 0, 0, DSBPLAY_LOOPING );
		else
			IDirectSoundBuffer_Play( SndObjs[ sndobj_index ]->Dup_Buffer[free_buffer], 0, 0, 0 );

		SndObjs[ sndobj_index ]->Buffer_TimeStamp[free_buffer] = clock();
		SndObjs[ sndobj_index ]->Buffer_Dist[free_buffer] = Distance;
		return TRUE;
	}
	
	// if we get here, either no buffers exist for the sound, or we are unable to duplicate a buffer.
	// in either case, we must dynamically load the sound.

	Volume = ( 0 - (long) ( Distance * 0.6F ) );	// Scale it down by a factor...
	Volume = SFX_MIN_VOLUME - (long)( (float)( SFX_MIN_VOLUME - Volume ) * VolModify * GlobalSoundAttenuation );


	if ( Effects == SPOT_SFX_TYPE_Taunt )
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Taunt;
	else
		SfxHolder[ HolderIndex ].SfxFlags = SFX_HOLDERTYPE_Dynamic;

	SfxHolder[ HolderIndex ].SndObjIndex = sndobj_index;
	SfxHolder[ HolderIndex ].SfxBufferIndex = -1;

	for ( i = 0; i < MAX_THREADED_SFX; i++ )
	{
		if ( !SfxThreadInfo[ i ].SfxToPlay )
		{

			SfxThreadInfo[ i ].SfxToPlay = TRUE;
			SfxThreadInfo[ i ].SfxNum = Sfx;
			SfxThreadInfo[ i ].Vol = Volume;
			SfxThreadInfo[ i ].SfxHolderIndex = HolderIndex;
			SfxThreadInfo[ i ].Effects = Effects;
			SfxHolder[ HolderIndex ].ThreadIndex = i;
	
			if ( type != SFX_2D )
			{
				SfxThreadInfo[ i ].SfxVector = *SfxPos;
				SfxThreadInfo[ i ].SfxFreq = Freq;
				SfxThreadInfo[ i ].SfxTempVector = Temp;
				SfxThreadInfo[ i ].SfxDistance = Distance;

				if ( Effects == SPOT_SFX_TYPE_Taunt )
				{
					SfxThreadInfo[ i ].SfxType = SFX_TYPE_Taunt;
				}else
				{
					SfxThreadInfo[ i ].SfxType = SFX_TYPE_Panned;
				}
			}else
			{
				SfxThreadInfo[ i ].SfxType = SFX_TYPE_Normal;
			}

			break;
		}
	}

	return TRUE;

}

void InitSfxHolders( void )
{
	int i;

	for( i = 0; i < MAX_ANY_SFX; i++ )
		SfxHolder[ i ].Used = FALSE;

	SfxUniqueID = 1;
	BikerSpeechPlaying = FALSE;
}

int FindFreeSfxHolder( void )
{
	int i;

	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( !SfxHolder[ i ].Used )
		{
			SfxHolder[ i ].Used = TRUE;

			SfxHolder[ i ].UniqueID = SfxUniqueID++;
			SfxHolder[ i ].OnPause = FALSE;

			SfxHolder[ i ].SndObjIndex = -1;
			SfxHolder[ i ].SfxFlags = -1;
			SfxHolder[ i ].SfxBufferIndex = -1;
			SfxHolder[ i ].ThreadIndex = -1;
			SfxHolder[ i ].CompoundSfxTimerID = 0;
			SfxHolder[ i ].TriggerSfx = -1;
			SfxHolder[ i ].PauseValue = 0.0F;
			return i;
		}
	}
	return -1;
}

uint32 PlaySfxWithTrigger( int16 Sfx, int16 TriggeredSfx )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %d - no free sfx holders\n");
	}
	else
	{
		if ( !StartPannedSfx( Sfx, NULL, NULL, 0.0F, SFX_2D, index, 1.0F, 0, FALSE ) )
		{
			/*
			SfxHolder[ index ].Used = FALSE;
			*/
			FreeSfxHolder( index );
		}
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
uint32 PlaySfx( int16 Sfx, float Vol )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	index = FindFreeSfxHolder();

	if ( index < 0 )
		DebugPrintf("Unable to play sfx %d - no free sfx holders\n");
	else
		if ( !StartPannedSfx( Sfx, NULL, NULL, 0.0F, SFX_2D, index, Vol, 0, FALSE ) )
		{
			/*
			SfxHolder[ index ].Used = FALSE;
			*/
			FreeSfxHolder( index );
		}


	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
		return SfxHolder[ index ].UniqueID;

	return 0;
}

uint32 PlayGeneralPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq, BOOL OverideDistanceCheck, uint16 Effects, float VolModify )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;

	//DebugPrintf("started playing sfx at %d\n", timeGetTime() );

	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %d - no free sfx holders\n");
	}
	else
	{
		if ( !StartPannedSfx( Sfx, &Group , SfxPos, Freq, LOOPING_SFX_FixedGroup, index, VolModify, Effects, OverideDistanceCheck ) )
		{
			/*
			SfxHolder[ index ].Used = FALSE;
			*/
			FreeSfxHolder( index );
		}
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
	{
		//DebugPrintf("finished playing sfx at %d\n", timeGetTime() );
		return SfxHolder[ index ].UniqueID;
	}

	//DebugPrintf("sfx not played at %d\n", timeGetTime() );
	return 0;
}

uint32 ForcePlayPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, TRUE, SPOT_SFX_TYPE_Normal, 1.0F );
}
uint32 PlayPannedSfx(int16 Sfx, uint16 Group, VECTOR * SfxPos, float Freq )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, FALSE, SPOT_SFX_TYPE_Normal, 1.0F );
}
uint32 PlayPannedSfxWithVolModify(int16 Sfx, uint16 Group, VECTOR * SfxPos, float Freq, float Vol )
{
	return PlayGeneralPannedSfx( Sfx, Group, SfxPos, Freq, FALSE, SPOT_SFX_TYPE_Normal, Vol );
}

uint32 PlaySpotSfx(int16 Sfx, uint16 *Group , VECTOR * SfxPos, float Freq, float Vol, uint16 Effects )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;
	
	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %d - no free sfx holders\n");
	}
	else
	{
		if ( !StartPannedSfx( Sfx, Group , SfxPos, Freq, LOOPING_SFX_VariableGroup, index, Vol, Effects, FALSE ) )
		{
			/*
			SfxHolder[ index ].Used = FALSE;
			*/
			FreeSfxHolder( index );
		}
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
	{
		return SfxHolder[ index ].UniqueID;
	}

	return 0;
}

uint32 PlayFixedSpotSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq, float Vol, uint16 Effects )
{
	int index;

	if ( !GlobalSoundAttenuation || !bSoundEnabled )
		return 0;
	
	index = FindFreeSfxHolder();
	if ( index < 0 )
	{
		DebugPrintf("Unable to play sfx %d - no free sfx holders\n");
	}
	else
	{
		if ( !StartPannedSfx( Sfx, &Group , SfxPos, Freq, LOOPING_SFX_FixedGroup, index, Vol, Effects, FALSE ) )
		{
			/*
			SfxHolder[ index ].Used = FALSE;
			*/
			FreeSfxHolder( index );
		}
	}

	// if sound was played, return unique ID
	if ( SfxHolder[ index ].Used )
	{
		return SfxHolder[ index ].UniqueID;
	}

	return 0;
}

int GetSfxHolderIndex( uint32 uid )
{
	int i;
	
	for( i = 0; i < MAX_ANY_SFX; i++ )
		if ( SfxHolder[ i ].Used && ( uid == SfxHolder[ i ].UniqueID ) )
			return i;

	return -1;
}

BOOL StopSfx( uint32 uid )
{
	int i;

	if ( !uid )
		return FALSE;

	if ( !bSoundEnabled )
		return FALSE;

	i = GetSfxHolderIndex( uid );
	if ( i != -1 )
	{
		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Compound )
		{
			KillCompoundSfxBuffer( SfxHolder[ i ].SfxBufferIndex );
		}

		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Looping )
		{
			StopLoopingSfx( SfxHolder[ i ].SfxBufferIndex ); 
		}

		if ( ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Dynamic ) || ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Taunt ) )

		{
			if ( SfxHolder[ i ].SfxBufferIndex < 0)
				SfxThreadInfo[ SfxHolder[ i ].ThreadIndex ].SfxToPlay = FALSE;
			else
			{
				SoundBufferRelease( &SBufferList[ SfxHolder[ i ].SfxBufferIndex ].buffer );
			}
		}

		if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Static )
		{
			IDirectSoundBuffer_Stop( SndObjs[ SfxHolder[ i ].SndObjIndex ]->Dup_Buffer[ SfxHolder[ i ].SfxBufferIndex ] );
			IDirectSoundBuffer_SetCurrentPosition( SndObjs[ SfxHolder[ i ].SndObjIndex ]->Dup_Buffer[ SfxHolder[ i ].SfxBufferIndex ], 0 );
		}

		FreeSfxHolder( i );
		return TRUE;
	}
	
	//DebugPrintf("Unable to stop sfx!!!\n");
	return FALSE;
}

void SetBufferFreq( IDirectSoundBuffer *Buffer, float Freq )
{
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, OrigFreq;

	// BUG:  Appears Buffer pointer goes bad or is passed in as NULL
	if(!Buffer)
	{
		DebugPrintf("BUG: SetBufferFreq() Buffer passed in was null\n");
		return;
	}

	if ( !Freq || ( Freq == 1.0F ) )
	{
		OrigFreq = DSBFREQUENCY_ORIGINAL; 
	}else
	{
		// get original frequency of buffer
		IDirectSoundBuffer_GetFormat( Buffer, NULL, 0, &dwSizeWritten );
		lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
		IDirectSoundBuffer_GetFormat( Buffer, lpwaveinfo, dwSizeWritten, 0 );
		OrigFreq = lpwaveinfo->nSamplesPerSec; 
		free(lpwaveinfo);
	
		// work out new frequency
		OrigFreq = (DWORD)( (float)OrigFreq * Freq );

		if ( OrigFreq < DSBFREQUENCY_MIN )
		{
			OrigFreq = DSBFREQUENCY_MIN;
		}
		if ( OrigFreq > DSBFREQUENCY_MAX )
		{
			OrigFreq = DSBFREQUENCY_MAX;
		}

	}

	// set frequency
	if ( IDirectSoundBuffer_SetFrequency( Buffer, OrigFreq ) != DS_OK )
	{
		DebugPrintf("set frequency failed ( 4 )\n");
	}

}

/*===================================================================
	Procedure	:	Play Panned Sfx
	Input		:	int16	Sfx Number
				:	MATRIX * Matrix
				:	VECTOR * Pos
	Output		:	Nothing
===================================================================*/
void SetPannedBufferParams( IDirectSoundBuffer *pDSB, IDirectSound3DBuffer *pDSB3D, VECTOR *SfxPos, float Freq, VECTOR *Temp, float Distance, long Volume, uint16 Effects )
{
	VECTOR	Temp2;
	float	nz;
	float	sx, sxmod;
	long	Pan;
	float mindist = SHIP_RADIUS * 6.0F;
	float sxmin = 100.0F;
	float sxmax = 10000.0F;
	float currentdist;

	if (!pDSB)
		return;

	if ( Distance && (!( Effects & SPOT_SFX_TYPE_NoPan )) )
	{
		ApplyMatrix( &Ships[ Current_Camera_View ].Object.FinalInvMat , Temp, &Temp2 );

		currentdist = (float) sqrt( ( Temp2.x * Temp2.x ) + ( Temp2.z * Temp2.z ) );
		
		if( currentdist )
		{
			nz = Temp2.z / currentdist;

			sxmod = ( Temp2.x < 0.0F ) ? -1.0F : 1.0F;

			sx = ( currentdist > mindist ) ? 10000.0F : ( ( ( currentdist * ( sxmax - sxmin ) ) / mindist ) + sxmin );

			sx *= sxmod;

			Pan = (long) ( ( 1.0F - fabs(nz) )* ( sx ) );
		}else{
			Pan = 0;
		}
		//DebugPrintf("nz: %f\tsx: %f\tsxmod: %f\tPan: %d\n", nz, sx, sxmod, Pan);
	}else
		Pan = 0;
    
	if (pDSB)
    {
		if( !Sound3D || !pDSB3D )
		{

			IDirectSoundBuffer_SetPan(pDSB, Pan );
			if ( IDirectSoundBuffer_SetVolume( pDSB , ( Volume > 0 ) ? 0 : Volume ) != DS_OK )
			{
				DebugPrintf("unable to set volume\n" );
			}

			SetBufferFreq( pDSB, Freq );

		}else{
			//DebugPrintf("Creating 3D info\n");
			IDirectSound3DBuffer_SetPosition( pDSB3D , SfxPos->x / 128.0F ,
											  SfxPos->y / 128.0F, SfxPos->z / 128.0F,
											  DS3D_IMMEDIATE);
		
			IDirectSound3DBuffer_SetMaxDistance( pDSB3D , 100.0F, DS3D_IMMEDIATE); 
			IDirectSound3DBuffer_SetMinDistance( pDSB3D ,   1.0F, DS3D_IMMEDIATE); 

		}
    }
}

/****************************************************************************
FUNCTION:   Init_SoundGlobals

PURPOSE:    Initializes the tiErree main global variables. After this is done,
            we should have allocated:
            a. A DirectSound Object
            b. A DirectSound3DListener Object
            c. A Primary Buffer.

            These are all global.
*****************************************************************************/
BOOL Init_SoundGlobals(void)
{
	DSBUFFERDESC  dsbdesc;
	int			  iErr = 0;
#ifdef DEBUG_ON
	char *from_file = __FILE__;
	int from_line = __LINE__;
#endif

		lpDS = NULL;

		// Attempt to initialize with DirectSound.
		// First look for DSOUND.DLL using CoCreateInstance.
		iErr = CoCreateInstance(&CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER,
									 &IID_IDirectSound, (void **) &lpDS);
		
		if ((iErr >= DS_OK)	&& (lpDS)) // Found DSOUND.DLL
			iErr = IDirectSound_Initialize(lpDS, NULL);	// Try to init DS.

		if (iErr < DS_OK)
		{
			DebugPrintf("returning FALSE from Init_SoundGlobals at point 1\n");
			return(FALSE);	// Failed to get DirectSound, so no sound-system available.
		}
		else
		{
			// Succeeded in getting DirectSound.
			// Check to see if there is 3D acceleration.
			DSCAPS	dsCaps;
			dsCaps.dwSize = sizeof(DSCAPS);
			IDirectSound_GetCaps(lpDS, &dsCaps);
			// Allow 3D sound only if acceleration exists.
			Sound3D = ((dsCaps.dwMaxHw3DAllBuffers > 0) ? TRUE : FALSE);
		}

	// TEMP!! no 3D sound for now
	Sound3D = FALSE;

	// If here, got a valid sound-interface...
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	dsbdesc.dwBufferBytes = 0; //dwBufferBytes and lpwfxFormat must be set this way.
	dsbdesc.lpwfxFormat = NULL;
	
	// Set control-level of DS. (To normal, default.)
	if (IDirectSound_SetCooperativeLevel(lpDS, render_info.window ,DSSCL_EXCLUSIVE /*DSSCL_NORMAL*/) >= DS_OK)    
	{
		// Create primary buffer.
		if ( MakeSoundBuffer( lpDS, &dsbdesc, &glpPrimaryBuffer, NULL ) )
		{
			
			DWORD dwSizeWritten;
			LPWAVEFORMATEX lpwaveinfo;
			
			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, NULL, 0, &dwSizeWritten );
			lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, lpwaveinfo, dwSizeWritten, 0 );
			
			lpwaveinfo->nChannels = 2;
			lpwaveinfo->nSamplesPerSec = 22050;
			lpwaveinfo->wBitsPerSample = 16;
			lpwaveinfo->nBlockAlign = 4;
			lpwaveinfo->nAvgBytesPerSec = lpwaveinfo->nSamplesPerSec * lpwaveinfo->nBlockAlign;

			if ( IDirectSoundBuffer_SetFormat( glpPrimaryBuffer, lpwaveinfo ) != DS_OK )
			{
				free(lpwaveinfo);
				return FALSE;
			}

			IDirectSoundBuffer_GetFormat( glpPrimaryBuffer, lpwaveinfo, dwSizeWritten, 0 );
			DebugPrintf("using primary buffer format: wFormatTag %d, nChannels %d, nSamplesPerSec %d, nAvgBytesPerSec %d, nBlockAlign %d, wBitsPerSample %d\n",
				lpwaveinfo->wFormatTag, lpwaveinfo->nChannels, lpwaveinfo->nSamplesPerSec, lpwaveinfo->nAvgBytesPerSec, lpwaveinfo->nBlockAlign, lpwaveinfo->wBitsPerSample );
			
			free(lpwaveinfo);

			// If no 3D, we are done.
			if (!Sound3D)
				return(TRUE);

			// If 3D, need to get listener interface.
			if (IDirectSoundBuffer_QueryInterface(glpPrimaryBuffer, &IID_IDirectSound3DListener,
															  (void **) &lpDS3DListener) >= DS_OK)
			{
				// Start primary-buffer looped-playing to reduce overhead on secondary-play calls.
				glpPrimaryBuffer->lpVtbl->Play(glpPrimaryBuffer, 0, 0, DSBPLAY_LOOPING);
				return(TRUE);
			}
			else
			{
				// Failed to get 3D, so we only have 2D-controls. Release listener if valid, and go with 2D.
				if (lpDS3DListener)
				{
					IDirectSound3DListener_Release(lpDS3DListener);
					lpDS3DListener = NULL;
				}
				
				// Flag all 3D as off.
				Sound3D = FALSE;

				return(TRUE);	// Still succeed initialization, but with 2D.
			}
		}
	}

	// If here, failed to initialize sound system in some way. (Either in SetCoopLevel, or creating primary-buffer.)
	IDirectSound_Release(lpDS);
	lpDS = NULL;
	
	DebugPrintf("returning FALSE from Init_SoundGlobals at point 2\n");

	return(FALSE);
}


/*===================================================================
	Procedure	:	Set the position velocity and direction of the Listner..
	Input		:	VECTOR * Pos
				:	VECTOR * Velocity
				:	MATRIX * Matrix
	Output		:	Nothing
===================================================================*/
BOOL SetPosVelDir_Listner( VECTOR * Pos , VECTOR * Velocity , MATRIX * Mat )
{
	VECTOR UpVector;
	VECTOR ForwardVector;
	if(DS_OK != IDirectSound3DListener_SetPosition(lpDS3DListener, Pos->x / 128.0F, Pos->y / 128.0F, Pos->z / 128.0F, DS3D_DEFERRED))
		return FALSE;
    //if(DS_OK != IDirectSound3DListener_SetVelocity(lpDS3DListener, Velocity->x / 128.0F,
	//											   Velocity->y / 128.0F, Velocity->z / 128.0F, DS3D_DEFERRED))
    if(DS_OK != IDirectSound3DListener_SetVelocity(lpDS3DListener, Velocity->x, Velocity->y, Velocity->z, DS3D_DEFERRED))
		return FALSE;

	ApplyMatrix( Mat, &SlideUp, &UpVector );			/* Calc Direction Vector */
	ApplyMatrix( Mat, &Forward, &ForwardVector );			/* Calc Direction Vector */
	if(DS_OK != IDirectSound3DListener_SetOrientation(lpDS3DListener, ForwardVector.x, ForwardVector.y, ForwardVector.z,
																	  UpVector.x, UpVector.y, UpVector.z, DS3D_DEFERRED))
		return FALSE;
	if (DS_OK != IDirectSound3DListener_CommitDeferredSettings(lpDS3DListener))
		return FALSE;
	return TRUE;
}

void SetSoundLevels( int *dummy )
{
#if 0
	float vol;
	DWORD volbits;
	MMRESULT mmr;
#endif
	// set sfx att.
	//GlobalSoundAttenuation = SfxSlider.value / 12.5F;
	GlobalSoundAttenuation = SfxSlider.value / ( SfxSlider.max / GLOBAL_MAX_SFX );
}

int FindFreeSBufferListNode( void )
{
	int i;
	
	for ( i = 0; i < MAX_SYNCHRONOUS_DYNAMIC_SFX; i++ )
		if ( SBufferList[ i ].used == FALSE )
			return i;

	return -1;
}

void FreeUpSBufferListNode( int node )
{
	SBufferList[ node ].used = FALSE;
}

int AddToSBufferList( IDirectSoundBuffer* buffer, IDirectSound3DBuffer* buffer3D, int SfxHolderIndex )
{
	int index;
	
	index = FindFreeSBufferListNode();

	if ( index == -1 )
	{
		// Ahhhh! - no room to store dynamic sfx, therefore must kill off prematurly
		DebugPrintf("unable to play dynamic sfx - need to increase size of SBufferList\n");
		IDirectSoundBuffer_Stop( buffer );
		SoundBufferRelease( &buffer );
		return -1;
	}

	SBufferList[ index ].used = TRUE;
	SBufferList[ index ].buffer = buffer;
	SBufferList[ index ].SfxHolderIndex = SfxHolderIndex;
	return index;
}

void CheckSBufferList( void )
{
	int i;
	DWORD dwStatus;

	if ( !bSoundEnabled )
		return;

	for( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( SfxHolder[ i ].Used )
		{
			if ( SfxHolder[ i ].OnPause )
			{
				SfxHolder[ i ].PauseValue -= framelag;
				if ( SfxHolder[ i ].PauseValue < 0.0F )
				{
					PlaySfx( SfxHolder[ i ].TriggerSfx, 1.0F );
					//SfxHolder[ i ].Used = FALSE;
					FreeSfxHolder( i );
				}
			}else
			{
				if ( SfxHolder[ i ].SfxFlags != SFX_Looping )	// never want to kill off looping sfx here!!
				{
					if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Static )
					{
		 				// if buffer is not playing...
						IDirectSoundBuffer_GetStatus( SndObjs[  SfxHolder[ i ].SndObjIndex ]->Dup_Buffer[ SfxHolder[ i ].SfxBufferIndex ] , &dwStatus );
						if ( !(dwStatus & DSBSTATUS_PLAYING) )
						{
							if ( SfxHolder[ i ].TriggerSfx != -1 )
							{
								// we want to trigger an additional sfx...
								SfxHolder[ i ].OnPause = TRUE;
								SfxHolder[ i ].PauseValue = TRIGGER_SFX_PAUSE_VALUE;

								// if was biker / bikecomp speech, reset to zero
								CheckSpeech( i );

							}else
							{
								//SfxHolder[ i ].Used = FALSE;
								FreeSfxHolder( i );
							}
						}
					}

					// must check SfxBufferIndex is valid, because it might not have been filled in by thread yet
					if ( (( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Dynamic ) || ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Taunt )) && ( SfxHolder[ i ].SfxBufferIndex != -1 ) )
					{
						// get status of buffer
						IDirectSoundBuffer_GetStatus( SBufferList[ SfxHolder[ i ].SfxBufferIndex ].buffer, &dwStatus );
							
						// if current node used and not playing, release & mark as unused
						if ( !(dwStatus & DSBSTATUS_PLAYING) )
						{
							SoundBufferRelease( &SBufferList[ SfxHolder[ i ].SfxBufferIndex ].buffer );
							//DebugPrintf("Released sfx %d\n", SfxHolder[ i ].UniqueID);

							if ( SfxHolder[ i ].TriggerSfx != -1 )
							{
								// we want to trigger an additional sfx...
								SfxHolder[ i ].OnPause = TRUE;
								SfxHolder[ i ].PauseValue = TRIGGER_SFX_PAUSE_VALUE;

								// if was biker / bikecomp speech, reset to zero
								CheckSpeech( i );
							}else
							{
								//SfxHolder[ i ].Used = FALSE;
								FreeSfxHolder( i );
							}
						}
					}
				}
			}
		}
	}
}

void FreeSBufferList( void )
{
	int i;
	for ( i = 0; i < MAX_SYNCHRONOUS_DYNAMIC_SFX; i++ )
	{
		if ( SBufferList[ i ].used )
		{
			IDirectSoundBuffer_Stop( SBufferList[ i ].buffer );
			SoundBufferRelease( &SBufferList[ i ].buffer );
			SBufferList[ i ].used = FALSE;
		}
	}
}


// globals used in functions below:
// SPOT_SFX_LIST *LoopingSfxListStart - set to NULL in InitializeSound

int InitLoopingSfx( int16 Sfx, int variant, uint16 *Group, VECTOR *SfxPos, float Freq, float Volume, int type, int SfxHolderIndex, uint16 Effects, uint32 uid )
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

	SpotSfxList[ index ].buffer = NULL;
	SpotSfxList[ index ].buffer3D = NULL;

	SpotSfxList[ index ].bufferloaded = FALSE;
	SpotSfxList[ index ].buffersize = 0;
	SpotSfxList[ index ].distance = 0.0F;
	SpotSfxList[ index ].used = TRUE;
	SpotSfxList[ index ].SfxHolderIndex = SfxHolderIndex;
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
	
	if (!( flags & SFX_Dynamic ))
	{
		if ( SpotSfxList[ index ].buffer )
		{
			IDirectSoundBuffer_Stop( SpotSfxList[ index ].buffer );
			SpotSfxList[ index ].buffer = NULL;
		}
	}

	if ( flags & SFX_Dynamic )
	{
		if ( SpotSfxList[ index ].SfxThreadInfoIndex != -1 )
		{
			SfxThreadInfo[ SpotSfxList[ index ].SfxThreadInfoIndex ].SfxToPlay = FALSE;
		}

		if ( SpotSfxList[ index ].buffer )
		{
			DebugPrintf("- looping sound %d never stopped\n", SpotSfxList[ index ].sfxindex);
			SoundBufferRelease( &SpotSfxList[ index ].buffer );
			SpotSfxList[ index ].buffer = NULL;
		}
	}

	SpotSfxList[ index ].used = FALSE;

	DebugPrintf("stopping looping sfx %d\n", SpotSfxList[ index ].sfxindex);
}

void ModifyLoopingSfx( uint32 uid, float Freq, float Volume )
{
	int index, LoopingSfxIndex;
	LPWAVEFORMATEX lpwaveinfo;
	DWORD dwSizeWritten, OrigFreq;

	index = GetSfxHolderIndex( uid );

	if ( index != -1 )
	{
		LoopingSfxIndex = SfxHolder[ index ].SfxBufferIndex;
		if ( ( LoopingSfxIndex >= 0 ) && ( LoopingSfxIndex < MAX_LOOPING_SFX ) )
		{
			if ( Freq && ( Freq != SpotSfxList[ LoopingSfxIndex ].freq ))
			{
				SpotSfxList[ LoopingSfxIndex ].freq = Freq;

				// if buffer exists, set frequency...
				if ( SpotSfxList[ LoopingSfxIndex ].bufferloaded && SpotSfxList[ LoopingSfxIndex ].buffer )
				{
	   				if ( !CheckSBufferValid( SpotSfxList[ LoopingSfxIndex ].buffer ) )
					{
						Msg("trying to modify invalid sound buffer\n");
						return;
					}
					
					// get original frequency of buffer
					IDirectSoundBuffer_GetFormat( SpotSfxList[ LoopingSfxIndex ].buffer, NULL, 0, &dwSizeWritten );
					lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
					IDirectSoundBuffer_GetFormat( SpotSfxList[ LoopingSfxIndex ].buffer, lpwaveinfo, dwSizeWritten, 0 );
					OrigFreq = lpwaveinfo->nSamplesPerSec; 
					free(lpwaveinfo);
			
					// work out new frequency
					OrigFreq = (DWORD)( (float)OrigFreq * Freq );

					if ( OrigFreq < DSBFREQUENCY_MIN )
					{
						OrigFreq = DSBFREQUENCY_MIN;
					}
					if ( OrigFreq > DSBFREQUENCY_MAX )
					{
						OrigFreq = DSBFREQUENCY_MAX;
					}

					if ( IDirectSoundBuffer_SetFrequency( SpotSfxList[ LoopingSfxIndex ].buffer, OrigFreq ) != DS_OK )
					{
						DebugPrintf("set frequency failed ( 5 )\n");
					}
				}
			}

			if ( Volume && ( Volume != SpotSfxList[ LoopingSfxIndex ].vol ))
			{
				SpotSfxList[ LoopingSfxIndex ].vol = Volume;
			}
		}else
		{
			Msg("invalid looping sfx index! ( %d )\n", LoopingSfxIndex);
		}

	}
}

void FindFreeLoopingSfxBuffer( int index )
{
	int i;
	DWORD dwStatus;
	float furthest;
	int furthest_index;

	furthest = 0.0F;
	furthest_index = 0;

	for ( i = 0; i < MAX_DUP_BUFFERS; i++ )
	{								
		IDirectSoundBuffer_GetStatus( SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->Dup_Buffer[ i ], &dwStatus );

		// if buffer not playing...
		if (!( dwStatus & DSBSTATUS_PLAYING ))
		{
			//DebugPrintf("- assigned non playing buffer to looping sfx %d\n", SpotSfxList[ index ].sfxindex);
			SpotSfxList[ index ].buffer = SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->Dup_Buffer[ i ];
			SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ i ] = index;
			return;
		}

		if ( SpotSfxList[ SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ i ] ].distance > furthest )
		{
			furthest = SpotSfxList[ SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ i ] ].distance;
			furthest_index = i;
		}
	}

	// if furthest sfx is nearer than current sfx, do not allocate buffer for current sfx
	if ( SpotSfxList[ index ].distance > furthest )
	{
		//DebugPrintf("- furthest is closer than current so cannot find free buffer %d\n", SpotSfxList[ index ].sfxindex);
		return;
	}

	//DebugPrintf("- stopping furthest sound buffer and assigning it to current looping sfx buffer %d\n", SpotSfxList[ index ].sfxindex);

	IDirectSoundBuffer_Stop ( SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->Dup_Buffer[ furthest_index ] );
	
	SpotSfxList[ SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ furthest_index ] ].buffer = NULL;
	SpotSfxList[ SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ furthest_index ] ].bufferloaded = FALSE;

	SpotSfxList[ index ].buffer = SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->Dup_Buffer[ furthest_index ];
	SndObjs[ SndLookup[ SpotSfxList[ index ].sfxindex ].SndObjIndex + SpotSfxList[ index ].variant ]->looping_sfx_index[ furthest_index ] = index;

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
	Print4x5Text( "bufferloaded",	X_OFFSET, LINE_HEIGHT * line++, 2 );
	Print4x5Text( "buffer ptr",		X_OFFSET, LINE_HEIGHT * line++, 2 );
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
		sprintf( buf, "%d%", SpotSfxList[ i ].type );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", *SpotSfxList[ i ].group );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].fixedgroup );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%1.1f", SpotSfxList[ i ].freq );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%1.1f", SpotSfxList[ i ].vol );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%s", SpotSfxList[ i ].bufferloaded ? "Y" : "N" );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%s", SpotSfxList[ i ].buffer ? "Y" : "N" );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%4.0f", SpotSfxList[ i ].distance );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].SfxHolderIndex );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].SfxThreadInfoIndex );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].Effects );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
		sprintf( buf, "%d", SpotSfxList[ i ].uid );
		Print4x5Text( buf , DATA_OFFSET + X_OFFSET + i * GAP_SIZE , LINE_HEIGHT * line++, 2 );
	}
}
#endif

void ProcessLoopingSfx( void )
{
	float	Modify;
	VECTOR	Temp;
	float	Distance, MaxDistance;
	BOOL InRange = FALSE;
	int i, j, flags;
	long	Volume;
	static float FrameSkip = 0.0F;
	DWORD dwSizeWritten;
	LPWAVEFORMATEX lpwaveinfo;
	DWORD datarate, safezone;
	VECTOR Pos;

	// print debug info
	// only looping sound i know of is the bike engines
	//PrintLoopingSfxDebug();

	// process looping sfx every few frames
	// i tried disabling this and saw no performance hit
	// we might find it better to just leave this running
	// or at least run it every few milliseconds
	// which is standard across varrying computers
	// that have diff fps
	FrameSkip += framelag;
	if (FrameSkip < LOOPING_SFX_FRAME_SKIP) return;
	else FrameSkip -= LOOPING_SFX_FRAME_SKIP;
	
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
		
		// work out if sound in range, get parameters
		if( Ships[ Current_Camera_View ].Object.Group != (uint16) -1 )
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
			//Distance += (MaxDistance - Distance) - (MaxDistance - Distance) * GlobalSoundAttenuation * SpotSfxList[ i ].vol;

			if( Distance >= MaxDistance ) 
				InRange = FALSE;
			else
				InRange = TRUE;
		}
		else
		{
			InRange = FALSE;
		}

		SpotSfxList[ i ].distance = Distance;

		// special checks added by methods to fix sound bugs
		if( !(flags & SFX_Dynamic) )
		{
			if ( SpotSfxList[i].bufferloaded && !SpotSfxList[i].buffer )
			{
				DebugPrintf("- looping sfx sound %d, bufferloaded=true but buffer pointer is BAD\n", SpotSfxList[ i ].sfxindex);
				DebugPrintf("- setting bufferloaded to false\n");
				SpotSfxList[i].bufferloaded = FALSE;
			}
			if ( !SpotSfxList[i].bufferloaded && SpotSfxList[i].buffer )
			{
				DWORD dwStatus;
				DebugPrintf("- looping sfx sound %d, bufferloaded=false but buffer pointer is GOOD\n", SpotSfxList[ i ].sfxindex);
				IDirectSoundBuffer_GetStatus( SpotSfxList[ i ].buffer, &dwStatus );
				if (dwStatus & DSBSTATUS_PLAYING)
				{
					DebugPrintf("- and buffer is still playing...\n");
					DebugPrintf("- stopping buffer now...\n");
					StopLoopingSfx(i);
				}
			}
		}

		// if not in range and currently loaded then stop the sound ... 
		if ( !InRange && SpotSfxList[ i ].bufferloaded )
		{
			if ( flags & SFX_Dynamic )
			{
			 	if ( SpotSfxList[ i ].buffer )
				{
					//DebugPrintf("Releasing dynamic looping sfx %d\n", SpotSfxList[ i ].sfxindex);
			
					// kill off buffer(s)
					if ( SpotSfxList[ i ].buffer3D )
					{
						IDirectSound3DBuffer_Release( SpotSfxList[ i ].buffer3D );
						SpotSfxList[ i ].buffer3D = NULL;
					}

					SoundBufferRelease( &SpotSfxList[ i ].buffer );
					SpotSfxList[ i ].buffer = NULL;
				}
			}
			else
			{
				//DebugPrintf("Stopping looping sfx %d\n", SpotSfxList[ i ].sfxindex);
				if(SpotSfxList[ i ].buffer == NULL)
				{
					DebugPrintf("ProcessLoopSFX() Attempt to call IDirectSoundBuffer_Stop() on bad pointer....\n");
				}
				else
				{
					//DebugPrintf("ProcessLoopSFX() IDirectSoundBuffer_Stop() buffer is not NULL ...\n");
					IDirectSoundBuffer_Stop( SpotSfxList[ i ].buffer );
				}
				SpotSfxList[ i ].buffer = NULL;
			}
			SpotSfxList[ i ].bufferloaded = FALSE;
		}

		// if in range & not loaded then find a free slot for the sound
		if ( InRange && !SpotSfxList[ i ].bufferloaded )
		{

			if ( flags & SFX_Dynamic )
			{
				DebugPrintf("Loading dynamic looping sfx %d\n", SpotSfxList[ i ].sfxindex);

				// load buffer(s) in sfx thread

				for ( j = 0; j < MAX_THREADED_SFX; j++ )
				{
					if ( !SfxThreadInfo[ j ].SfxToPlay )
					{
						SfxThreadInfo[ j ].SfxToPlay = TRUE;
						SfxThreadInfo[ j ].SfxNum = SpotSfxList[ i ].sfxindex;
						SfxThreadInfo[ j ].Variant = SpotSfxList[ i ].variant;
						SfxThreadInfo[ j ].SfxType = SFX_TYPE_Looping;
						SfxThreadInfo[ j ].SpotSfxListIndex = i;

						SpotSfxList[ i ].SfxThreadInfoIndex = j;
						SpotSfxList[ i ].bufferloaded = TRUE;

						break;
					}
				}


			}
			else
			{
				//DebugPrintf("assigning buffer to looping sfx %d\n", SpotSfxList[ i ].sfxindex);

				FindFreeLoopingSfxBuffer( i );

				if ( SpotSfxList[ i ].buffer )
				{
					SetBufferFreq( SpotSfxList[ i ].buffer, SpotSfxList[ i ].freq );
					SpotSfxList[ i ].bufferloaded = TRUE;
				}
			}

		}

		// if in range, and buffer already loaded
		if ( InRange && SpotSfxList[ i ].bufferloaded && SpotSfxList[ i ].buffer)
		{
			DWORD dwCurrentPlayCursor;
			HRESULT hres;
			DWORD dwStatus;

			// check sfx is not in 'safe zone'
			//currenttime = GetTickCount();

			// get current buffer position...
			hres = IDirectSoundBuffer_GetCurrentPosition(SpotSfxList[ i ].buffer, &dwCurrentPlayCursor, NULL);
			if ( hres != DS_OK )
				DebugPrintf("error getting current looping sound play position\n");

			// get buffer format
			IDirectSoundBuffer_GetFormat( SpotSfxList[ i ].buffer, NULL, 0, &dwSizeWritten );
			lpwaveinfo = (LPWAVEFORMATEX)malloc( dwSizeWritten );
			IDirectSoundBuffer_GetFormat( SpotSfxList[ i ].buffer, lpwaveinfo, dwSizeWritten, 0 );
			datarate = lpwaveinfo->nAvgBytesPerSec; 
			free(lpwaveinfo);

			// work out safe zone ( bytes from end )
			safezone = ( datarate * LOOPING_SFX_MIXAHEAD ) / 1000; 
			
			if ( dwCurrentPlayCursor < ( SpotSfxList[ i ].buffersize - safezone ) )
			{
				if( !Sound3D || !SpotSfxList[ i ].buffer3D )
				{
					//DebugPrintf("- adjusting looping sound volumne based on distance.\n");

					// adjust buffer parameters
					//Volume = ( 0 - (long) ( Distance * 0.6F ) );	// Scale it down by a factor...

					Volume = (long)(( GlobalSoundAttenuation * Distance / MaxDistance ) * SFX_MIN_VOLUME);
		
					Volume = SFX_MIN_VOLUME - (long)( (float)( SFX_MIN_VOLUME - Volume ) * SpotSfxList[ i ].vol * GlobalSoundAttenuation );

					SetPannedBufferParams( SpotSfxList[ i ].buffer, NULL, &Pos, SpotSfxList[ i ].freq, &Temp, Distance, Volume, SpotSfxList[ i ].Effects );

					//if ( Distance <= SHIP_RADIUS * LOOPING_SFX_PANNING_PROXIMITY )
					//	IDirectSoundBuffer_SetPan( SpotSfxList[ i ].buffer, 0 );
			
				}
				else
				{
					// would do 3D stuff here...
				}

				//DebugPrintf("- playing looping sound %d\n", SpotSfxList[ i ].sfxindex);

				IDirectSoundBuffer_GetStatus( SpotSfxList[ i ].buffer, &dwStatus );
				if (!(dwStatus & DSBSTATUS_PLAYING))
					IDirectSoundBuffer_Play( SpotSfxList[ i ].buffer, 0, 0, DSBPLAY_LOOPING );

			}//else
			 //	DebugPrintf("in safe zone...( current play cursor = %d, buffer size = %d )\n", dwCurrentPlayCursor, tempnode->buffersize);
		}
	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

FILE *SaveAllSfx( FILE *fp )
{
	uint16 num_active_sfx = 0;
	int16 i;

	if( fp )
	{
		// work out number of sfx to save...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
			if ( SpotSfxList[ i ].used )
				num_active_sfx++;

		// write out number of sfx...
		fwrite( &num_active_sfx, sizeof( uint16 ), 1, fp );

		// write out sfx data...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
			if ( SpotSfxList[ i ].used )
				fwrite( &SpotSfxList[ i ], sizeof( SPOT_SFX_LIST ), 1, fp );

		fwrite( &SfxUniqueID, sizeof( uint32 ), 1, fp );
	}

	return( fp );
}

FILE *LoadAllSfx( FILE *fp )
{
	uint16 num_active_sfx = 0;
	int16 i;
	
	if( fp )
	{
		// work out number of sfx to load...
		fread( &num_active_sfx, sizeof( uint16 ), 1, fp );

		// invalidate existing sfx data, stopping any current sfx...
		for ( i = 0; i < MAX_LOOPING_SFX; i++ )
		{
			if ( SpotSfxList[ i ].used )
			{
				StopSfx( SpotSfxList[ i ].uid );
			}

			SpotSfxList[ i ].used = FALSE;
		}
		
		// read in sfx data...
		for ( i = 0; i < num_active_sfx; i++ )
		{
			fread( &SpotSfxList[ i ], sizeof( SPOT_SFX_LIST ), 1, fp );
		}

		fread( &SfxUniqueID, sizeof( uint32 ), 1, fp );
		
		ReTriggerSfx();
	}


	return( fp );
}

void PlaySpecificBikerSpeech( int16 sfx, uint16 Group, VECTOR *SfxPos, float Freq, int biker, char variant, BOOL update )
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

	TauntID =  PlayGeneralPannedSfx( sfx, Group, SfxPos, Freq, FALSE, SPOT_SFX_TYPE_Taunt, 1.0F );
	TauntUpdatable = update;
}

float ReturnDistanceVolumeVector( VECTOR *sfxpos, uint16 sfxgroup, VECTOR *listenerpos, uint16 listenergroup, long *vol, VECTOR *sfxvector )
{
	VECTOR	Temp;
	float Modify;
	float dist;

	Temp.x = sfxpos->x - listenerpos->x;
	Temp.y = sfxpos->y - listenerpos->y;
	Temp.z = sfxpos->z - listenerpos->z;

	if( listenergroup != (uint16) -1 )
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
			*vol = SFX_MIN_VOLUME - (long)( ((float)( SFX_MIN_VOLUME - *vol )) * GlobalSoundAttenuation );
		}
		if( sfxvector )
		{
			*sfxvector = Temp;
		}
		return dist;
	}
}

BOOL UpdateTaunt( uint32 uid, uint16 Group, VECTOR *SfxPos )
{
	int i;
	float	Distance;
	long Volume;
	float VolModify;
	VECTOR sfxvector;

	if ( !uid )
		return FALSE;

	if ( !bSoundEnabled )
		return FALSE;

	i = GetSfxHolderIndex( uid );
	if( i == -1 )
	{
		return FALSE;
	}

	Distance = ReturnDistanceVolumeVector( SfxPos, Group, &Ships[ Current_Camera_View ].Object.Pos, Ships[ Current_Camera_View ].Object.Group, NULL, &sfxvector );
//	TauntDist = Distance;

	if( Distance < 0.0F ) 
	{
	 	StopSfx( uid );
		return FALSE;
	}

	if ( SfxHolder[ i ].SfxFlags == SFX_HOLDERTYPE_Taunt )
	{
		if ( SfxHolder[ i ].SfxBufferIndex >= 0)
		{
			VolModify = ( (float)BikerSpeechSlider.value / (float)BikerSpeechSlider.max ) * SPEECH_AMPLIFY;	// when multiplied with max value for GlobalSoundAttenuation, gives 1.0F;
			Volume = 0;
			Volume = SFX_MIN_VOLUME - (long)( (float)( SFX_MIN_VOLUME - Volume ) * VolModify * GlobalSoundAttenuation );

			// set buffer parameters
			SetPannedBufferParams( SBufferList[ SfxHolder[ i ].SfxBufferIndex ].buffer,
				NULL,
				SfxPos, 
				0.0F, 
				&sfxvector, 
				Distance,
				Volume, 
				SPOT_SFX_TYPE_Taunt );
		}
	}else
	{
		DebugPrintf("Holder type not SFX_HOLDERTYPE_Taunt\n");
	}

	return TRUE;
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
			TauntUpdatable = FALSE;
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
				TauntUpdatable = FALSE;
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

void PlayUpdatableBikerTaunt( VECTOR *pos, uint16 Group, uint16 bike, char variant )
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
	PlaySpecificBikerSpeech( SFX_BIKER_TN, Group, pos, 0.0F, bike, variant, TRUE );	// TRUE means update
}

void PlayRecievedSpeechTaunt( BYTE player, char variant )
{
	uint16 Group;
	VECTOR *Pos;
	int Taunter = player;
	if( Taunter != 0xFF )
	{
		Group = Ships[ Taunter ].Object.Group;
		Pos = &Ships[ Taunter ].Object.Pos;
		PlaySpecificBikerSpeech( SFX_BIKER_TN, Group, Pos, 0.0F, Ships[ Taunter ].BikeNum, variant, TRUE );	// TRUE means update
	}
}

void StopTaunt( void )
{
	// do not stop taunt if it is your own. ( this will be automatically cut out on death by death cry )
	if ( TauntID && ( Taunter != 0xFF ))
		StopSfx( TauntID );
}

int EnemyBikeNum( uint16 type )
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
		return (uint16)-1;
	}
}

void StopEnemyBikerTaunt( ENEMY *Enemy )
{
	uint16 bike;

	bike = EnemyBikeNum( Enemy->Type );
	if( bike == (uint16)-1 )
		return;

	if ( TauntID )
		StopSfx( TauntID );

	PlaySpecificBikerSpeech( SFX_Die, Enemy->Object.Group, &Enemy->Object.Pos, 0.0F, bike, -1, FALSE );
}

void PlayEnemyBikerTaunt( ENEMY *Enemy )
{
	uint16 bike;

	bike = EnemyBikeNum( Enemy->Type );
	if( bike == (uint16)-1 )
		return;

	EnemyTaunter = Enemy;

	PlayUpdatableBikerTaunt( &EnemyTaunter->Object.Pos, EnemyTaunter->Object.Group, bike, -1 );
}





