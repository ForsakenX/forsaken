/*
 * Project X, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 86 $
 *
 * $Header: /PcProjectX/Include/sfx.h 86    13/06/98 14:35 Philipy $
 *
 * $Log: /PcProjectX/Include/sfx.h $
 * 
 * 86    13/06/98 14:35 Philipy
 * only 1 bike computer available for win98 demo version
 * 
 * 85    20/04/98 17:12 Philipy
 * added localisation stuff
 * 
 * 84    8/04/98 14:17 Philipy
 * modified max sound volume
 * 
 * 83    3/04/98 17:02 Philipy
 * cd audio now only retriggered if enabled!
 * added generic pickup sound if bike computer speech is zero
 * 
 * 82    3/04/98 13:13 Philipy
 * Taunts are now affected by biker volume slider ( taunt volume slider
 * removed )
 * Enemy bikers now give out death cry
 * fixed problem with speech ignoring volume settings
 * fixed end game sequences
 * 
 * 81    2/04/98 21:07 Philipy
 * Added taunts ( single & multiplayer, plus enemy biker taunts )
 * added flygirl to front end.
 * sliders for biker, bike computer and taunt speech volume
 * added new sfx for title
 * 
 * 80    30/03/98 17:31 Philipy
 * added cd specific path stuff
 * added new bike computers
 * prevented File_Exists being called every time a dynamic sfx is played
 * 
 * 79    29/03/98 20:00 Philipy
 * cd path now verified earlier
 * sfx no longer reloaded when changing biker / bike computer
 * mouse sensitivity rounding error fixed
 * 
 * 78    28/03/98 17:33 Philipy
 * corrected some sfx
 * added legal screen
 * fixed mission briefing text bug
 * 
 * 77    28/03/98 13:35 Philipy
 * added all biker speech
 * 
 * 76    27/03/98 18:04 Philipy
 * fixed bug where mapped sfx did not play.
 * 
 * 75    3/27/98 12:37p Phillipd
 * 
 * 74    26/03/98 15:24 Philipy
 * re added sfx missing from new batch
 * 
 * 73    25/03/98 21:37 Philipy
 * increased max levelspec sfx
 * 
 * 72    24/03/98 16:20 Philipy
 * added new sfx
 * 
 * 71    19/03/98 15:44 Philipy
 * added ForcePlayPannedSfx function
 * 
 * 70    11/03/98 10:51 Philipy
 * all sfx now paused when single player game paused, not just looping sfx
 * 
 * 69    9/03/98 16:54 Philipy
 * added 'requested' flag to SfxLookup. ( At present all sfx requested )
 * 
 * 68    27/02/98 16:02 Collinsd
 * 
 * 67    27/02/98 16:00 Philipy
 * 
 * 66    25/02/98 15:44 Philipy
 * more efficient use of hw sound mixing buffers
 * 
 * 65    24/02/98 16:56 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 64    21/02/98 16:46 Philipy
 * 
 * 63    21/02/98 16:25 Philipy
 * added text messages for capture flag
 * 
 * 62    21/02/98 13:04 Philipy
 * added in game load / save for sfx
 * 
 * 61    16/02/98 16:53 Collinsd
 * Added Chris's new code.
 * 
 * 60    12/02/98 16:36 Philipy
 * 
 * 59    11/02/98 19:38 Philipy
 * fixed crash bug
 * 
 * 58    10/02/98 19:41 Philipy
 * added support for 2d looping sfx
 * 
 * 57    9/02/98 12:21 Philipy
 * added sound buffer memory managment
 * only one piece of bike computer speech can now play at a time
 * 
 * 56    3/02/98 15:38 Philipy
 * fixed sfx bug
 * 
 * 55    29/01/98 12:16 Philipy
 * added new sfx
 * fixed playing of specific level spec variant sfx
 * 
 * 54    28/01/98 17:16 Philipy
 * added new sfx
 * 
 * 53    28/01/98 10:19 Philipy
 * only one piece of biker speech can play at any one time
 * death cry overides any existing speech
 * 
 * 52    22/01/98 19:14 Philipy
 * fixed re-loading looping sfx while in level
 * biker speech now switchable
 * 
 * 51    21/01/98 18:04 Philipy
 * 
 * 50    21/01/98 17:27 Philipy
 * added new sfx
 * 
 * 49    20/01/98 16:33 Philipy
 * added new sfx fn: PlaySpotFixedSfx
 * -data option now reverts to working dir for sfx if no local sfx dir
 * exists
 * 
 * 48    18/01/98 23:46 Philipy
 * added triggered sfx, tidied up critical sections
 * 
 * 47    1/17/98 10:39a Phillipd
 * 
 * 46    16/01/98 14:43 Philipy
 * added new sfx 
 * 
 * 45    15/01/98 17:01 Philipy
 * added spot sfx stuff.
 * PlayPannedSfx and PlaySpotSfx now return unique uint32 - use StopSfx to
 * stop sound
 * 
 * 44    13/01/98 12:06 Philipy
 * added looping spot sfx support, and changed looping sfx to use static
 * list rather than dynamic linked list
 * 
 * 43    12/01/98 15:41 Collinsd
 * Added sfx
 * 
 * 42    10/01/98 19:31 Philipy
 * bug fixes
 * 
 * 41    7/01/98 9:34 Philipy
 * added title room sfx
 * added ability to select bike computer, biker with sfx loaded
 * 
 * 40    5/01/98 10:38 Philipy
 * speech sfx implemented - currently defaults to 1 biker & computer only,
 * none selectable
 * 
 * 39    30/12/97 14:26 Philipy
 * Changed dynamic sfx linked list to static list
 * 
 * 38    12/22/97 2:01p Phillipd
 * 
 * 37    22/12/97 11:44 Collinsd
 * Added spotfx sound.  and added wesnik weanimator
 * 
 * 36    12/19/97 11:14a Phillipd
 * 
 * 35    12/18/97 5:38p Phillipd
 * 
 * 34    12/18/97 2:46p Phillipd
 * 
 * 33    5/12/97 21:52 Philipy
 * various changes to looping sfx stuff
 * 
 * 32    2/12/97 16:46 Philipy
 * compound sfx buffer is now stopped when quitting game
 * 
 * 31    28/11/97 17:36 Philipy
 * some looping sfx stuff done
 * 
 * 30    27/11/97 12:35 Philipy
 * fixed sound bug on self play demo
 * demo playback speed now given as true percentage
 * 
 * 29    26/11/97 11:48 Philipy
 * implemented dynamic loading of SFX, dynamic allocation of mixing
 * channels.
 * 3D sound currently disabled.
 * 
 * 28    4/08/97 11:58 Collinsd
 * A3D:Sound stuff added
 * 
 * 27    5/31/97 12:35p Phillipd
 * Sfx bug fixed...Now check distance for an overide rather than just
 * checking to see if its the same sfx num...You can quit out of the menus
 * again.....
 * 
 * 26    5/23/97 9:18a Phillipd
 * 
 * 25    5/15/97 11:42a Phillipd
 * 
 * 24    25/04/97 12:52 Collinsd
 * Added Invul sfx
 * 
 * 23    20-03-97 10:11a Collinsd
 * fixed bug in sfx
 * 
 * 22    20-03-97 10:00a Collinsd
 * new sfx
 * 
 * 21    15-02-97 9:32p Collinsd
 * Portals now use variable execute buffers.  They also
 * allocate/deallocate themselves properly now.
 * 
 * 20    13-01-97 5:03p Collinsd
 * Added Temp Door SFX
 * 
 * 19    6-01-97 11:09a Collinsd
 * All references to timer now replaced with titanstar.
 * 
 * 18    19-12-96 3:19p Collinsd
 * Changed sfx funtion to allow frequency changing.
 * Added Trojax Charging SFX.
 * 
 * 17    12/10/96 3:32p Collinsd
 * Added new laser sfx.
 * 
 * 16    12/10/96 11:17a Collinsd
 * Added drop mine sfx.
 * 
 * 15    12/04/96 11:21a Collinsd
 * Added foetoid and new scaled bikes. Added sussgun richochet and sussgun
 * no ammo sfx, new load weapon sfx, and new transpulse/trojax sfx.
 * 
 * 14    12/02/96 1:24p Collinsd
 * Added new cloaking/decloaking sfx.  Also new sussgun fire.
 * 
 * 13    17/11/96 17:28 Collinsd
 * Changed Pulsar & Trojax. Added Trojax Charge sfx to list.
 * 
 * 12    14/11/96 16:40 Collinsd
 * Added incoming sound effect and scattered message if no sound card
 * present.
 * 
 * 11    14/11/96 12:22 Collinsd
 * Added new sfx ( Golden PowerPod, Scattered, Missile Launch )
 * 
 * 10    30/10/96 16:21 Collinsd
 * stealth sfx and regeneration
 * 
 * 9     10/29/96 2:52p Phillipd
 * 
 * 8     22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 7     8/10/96 10:40 Collinsd
 * Added generic pickup sfx, and changed smoke trail.
 * 
 * 6     3/10/96 15:49 Collinsd
 * Added new sfx
 * 
 * 5     18/09/96 10:58 Collinsd
 * Change sfx to 4 channels and distance limit. Also reduced amount of
 * sparks generated by beam laser.
 * 
 * 4     8/08/96 9:13 Collinsd
 * Added Sfx and pickups
 * 
 * 3     7/25/96 10:05a Phillipd
 * 
 * 2     7/24/96 2:42p Phillipd
 * 
 * 1     7/24/96 2:15p Phillipd
 * 
 */
#ifndef	SFX_INCLUDED
#define	SFX_INCLUDED

#include <dsound.h>
#include "d3dmain.h"
#include "dsutil.h"

#include "main.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "object.h"
#include "mydplay.h"
#include "triggers.h"
#include "enemies.h"

/****************************************
defines
*****************************************/
#define MAX_BIKE_COMPUTER_SFX 64 
#define MAX_BIKER_SFX 16 
#define SPOT_SFX_TYPE_Normal 0
#define SPOT_SFX_TYPE_NoPan 1
#define SPOT_SFX_TYPE_Taunt 2

#define MAX_THREADED_SFX 2

#define SFX_TYPE_Normal 1
#define SFX_TYPE_Panned 2
#define SFX_TYPE_Looping 3
#define SFX_TYPE_Taunt 4

#define SFX_BUFFERTYPE_OnceOnly	FALSE
#define SFX_BUFFERTYPE_Looping	TRUE

#define LOOPING_SFX_MIXAHEAD 500	// time from end of sample ( in mS ) that it is not safe to make control changes
									// else you will get a gap when looping.

#define LOOPING_SFX_SCALE 0.75F		// used for max distance of looping sfx ( fraction of max distance used for normal sfx )

#define SFX_MIN_VOLUME  ( DSBVOLUME_MIN / 3) 

#define LOOPING_SFX_PANNING_PROXIMITY 10	// no. of ships radius's distance that you must be away from sound
											// before sound is panned

#define MAX_DISTANCE  ( 24 * 1024 * GLOBAL_SCALE )

#define LOOPING_SFX_FRAME_SKIP 5.0F	// process looping sfx every 5 frames

#define LOOPING_SFX_FixedGroup 0
#define LOOPING_SFX_VariableGroup 1
#define SFX_2D 2

#define TRIGGER_SFX_PAUSE_VALUE 20.0F	// pause between triggered sfx ( 20.0F = 1/3 second )

#define GLOBAL_MAX_SFX ( 0.9F ) 
#define SPEECH_AMPLIFY	( 1.0F / GLOBAL_MAX_SFX )

// sfx is -1 if free
// allocate one variable only - sfx loader can wait if not available

#define SFX_TYPE_Nothing 0
#define SFX_TYPE_Static 1
#define SFX_TYPE_Dynamic 2
#define SFX_TYPE_Compound 3

#define MAX_CONCURRENT_SFX 32
#define MAX_SYNCHRONOUS_DYNAMIC_SFX 16

#define VOL_SCALE_FACTOR 0.6F

#define MIN_SOUNDCARD_HW_MEM 262144		// 256K 

#define MAX_LEVELSPECIFIC_SFX 64

enum {
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

#define SFX_BIKER_START ( SFX_BIKER_GP )
#define SFX_BIKECOMP_START ( SFX_BIKECOMP_AM )
#define SFX_NUM_BIKE_PHRASES ( SFX_BIKER_EX - SFX_BIKER_GP + 1 )
#define SFX_NUM_BIKECOMP_PHRASES ( SFX_Select_Trojax - SFX_BIKECOMP_AM + 1 )

#ifdef WIN98SHAREWARE

#define MAXBIKECOMPTYPES 1

#else 

#if !defined( VERSION_SPANISH ) && !defined( VERSION_ITALIAN ) && !defined( VERSION_GERMAN ) && !defined( VERSION_FRENCH )
#define MAXBIKECOMPTYPES 5
#elif defined( VERSION_SPANISH ) || defined( VERSION_ITALIAN ) || defined( VERSION_FRENCH )
#define MAXBIKECOMPTYPES 3
#elif defined( VERSION_GERMAN )
#define MAXBIKECOMPTYPES 4
#endif

#endif

/*************************************
randomised sound lookup table identifiers
**************************************/
enum {
	SFX_RANDOMISED_LOOKUP_BikeCollideMetal,
	SFX_RANDOMISED_LOOKUP_BikeCollideStone,
	SFX_RANDOMISED_LOOKUP_BikeCollideLargeEnemy,
	SFX_RANDOMISED_LOOKUP_BikeCollideMediumEnemy,
	SFX_RANDOMISED_LOOKUP_BikeCollideSmallEnemy,
	SFX_RANDOMISED_LOOKUP_BikeScrapeMetal,
	SFX_RANDOMISED_LOOKUP_BikeScrapeStone,
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
level specific sfx lookup table identifiers
**************************************/
enum {
	SFX_LEVELSPEC_LOOKUP_00, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_01, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_02, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_03, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_04, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_05, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_06, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_07, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_08, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_09, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_10, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_11, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_12, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_13, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_14, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_15, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_16, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_17, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_18, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_19, //	-	level specific 
	SFX_LEVELSPEC_LOOKUP_20, //	-	level specific 
};

typedef struct _SBUFFER_LIST
{
	struct _SBUFFER_LIST *next;
	IDirectSoundBuffer* buffer;
	IDirectSound3DBuffer* buffer3D;
} SBUFFER_LIST;

typedef struct
{
	int		current_sfx;
	int		current_variant;
	int		compound_buffer_lookup_index;
	DWORD	start_time;
	DWORD	finish_time;
	float	distance;
	UINT	timerID;
	IDirectSoundBuffer *buffer;
	int		SfxHolderIndex;
}COMPOUND_SFX_INFO;

#define MAX_ANY_SFX 64

typedef struct
{
	BOOL Used;
	uint32	UniqueID;
	int SndObjIndex;
	int SfxFlags;
	int SfxBufferIndex;
	int ThreadIndex;
	UINT CompoundSfxTimerID;
	int16 TriggerSfx;
	BOOL OnPause;
	float PauseValue;
} SFX_HOLDER;

typedef struct
{
	int sfx;
	IDirectSoundBuffer *buffer;
	IDirectSound3DBuffer *buffer3D;
	DWORD SampleLength;
	DWORD buffersize;
	int SpotSfxListIndex;
} LOOPING_SFX_PIPE;


typedef struct
{
	int type;
	int SndObjIndex;
	int SndObjBuffer;
	IDirectSoundBuffer *DynamicSfxBuffer;
	int CompoundSfxBufferIndex;
	int LoopingSfxIndex;
} SFX_PLAYING;

typedef struct
{
	BOOL used;
	IDirectSoundBuffer *buffer;
	int SfxHolderIndex;
} SBUFFERLIST;

typedef struct
{
	BOOL SfxToPlay;
	int16 SfxNum;
	int Variant;
	uint16 SfxGroup;
	VECTOR SfxVector;
	VECTOR SfxTempVector;
	float SfxFreq;
	float SfxDistance;
	int SfxType;
//	SPOT_SFX_LIST *node;
	int SpotSfxListIndex;
	int SfxHolderIndex;
	long Vol;
	uint16 Effects;
} SFX_THREAD_INFO;

typedef struct
{
	char file[128];
	int	flags;
}LEVELSPEC_SFX_FILES;

typedef struct{
	uint16 Num_Variants;
	uint16 SndObjIndex;
	BOOL Requested;
} SNDLOOKUP;

#define DESTROYSOUND_All 0
#define DESTROYSOUND_KeepLevelSpecTable 1

//#define	MAX_SFX	128
#define	MAXCHANNELS	4
BOOL InitializeSound( int flags );
void    DestroySound( int flags );
uint32	PlaySfx( int16 Sfx, float Dist );
uint32	PlayPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq );
uint32 ForcePlayPannedSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos, float Freq );
BOOL	SetPosVelDir_Listner( VECTOR * Pos , VECTOR * Velocity , MATRIX * Mat );
void	Update3DSfx(int16 Sfx, uint16 Group , VECTOR * SfxPos , VECTOR * SfxVel );
int AddToSBufferList( IDirectSoundBuffer* buffer, IDirectSound3DBuffer* buffer3D, int SfxHolderIndex );
void CheckSBufferList( void );
void FreeSBufferList( void );
int InitLoopingSfx( int16 Sfx, int variant, uint16 *Group, VECTOR *SfxPos, float Freq, float Volume, int type, int SfxHolderIndex, uint16 Effects, uint32 uid );
void StopLoopingSfx( int index );
void ModifyLoopingSfx( uint32 uid, float Freq, float Volume );
void ProcessLoopingSfx( void );
void StopCompoundSfx( void );
int PlayGenericSfx( GENERIC_SFX_INFO *Info );
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
void GetSfxPathsToTry( int sfx, int *num_paths_to_try, char **data_path_to_use );
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
