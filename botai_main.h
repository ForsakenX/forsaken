#ifdef LUA_BOT
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "config.h"
#include "controls.h"
#include "ships.h"
#include "triggers.h"
#include "pickups.h"
#include "primary.h"
#include "secondary.h"
#include "mxaload.h"
#include "lights.h"
#include "sphere.h"
#include "title.h"
#include "trigarea.h"
#include "node.h"
#include "models.h"
#include "enemies.h"
#include "extforce.h"
#include "teleport.h"
#include "water.h"
#include "spotfx.h"
#include "screenpolys.h"
#include "restart.h"
#include "goal.h"
#include "visi.h"
#include "text.h"
#include "local.h"
#include "util.h"
#include "timer.h"
#include "ai.h"

extern bool TeamGame;
extern BYTE	TeamNumber[MAX_PLAYERS];
extern BYTE WhoIAm;
extern MCLOADHEADER	MCloadheader;
extern MCLOADHEADER	MCloadheadert0;
extern MLOADHEADER Mloadheader;
extern MODEL Models[MAXNUMOFMODELS];
extern PICKUP Pickups[ MAXPICKUPS ];
extern PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern PRIMARYWEAPONBULLET PrimBulls[ MAXPRIMARYWEAPONBULLETS ];
extern SECONDARYWEAPONBULLET SecBulls[ MAXSECONDARYWEAPONBULLETS ];
extern SHIPCONTROL control;
extern SHIPHEALTHMSG PlayerHealths[ MAX_PLAYERS+1 ];
extern VECTOR Backward;
extern VECTOR Forward;
extern VECTOR SlideDown;
extern VECTOR SlideLeft;
extern VECTOR SlideRight;
extern VECTOR SlideUp;
extern float BankAccell;
extern float BankAccell;
extern float BankDecell;
extern float GeneralAmmo;
extern float MaxBankAngle;
extern float MaxBankAngle;
extern float MaxMoveSpeed;
extern float MaxRollSpeed;
extern float MaxTurboSpeed;
extern float MaxTurnSpeed;
extern float MoveAccell;
extern float MoveDecell;
extern float NitroFuel;
extern float PowerLevel;
extern float PyroliteAmmo;
extern float RollAccell;
extern float RollDecell;
extern float SussGunAmmo;
extern float TurboAccell;
extern float TurboDecell;
extern float TurnAccell;
extern float TurnDecell;
extern float framelag;
extern float real_framelag;
extern int16_t LevelNum;
extern int16_t LaserOverheated;
extern int16_t NumOrbs;
extern int16_t PickupsGot[ MAXPICKUPTYPES ];
extern int16_t PrimaryWeaponsGot [ MAXPRIMARYWEAPONS ];
extern int16_t SecondaryAmmo[ MAXSECONDARYWEAPONS ];
extern int16_t SecondaryWeaponsGot [ MAXSECONDARYWEAPONS ];
extern u_int16_t FirstModelUsed;
extern char ShortLevelNames[MAXLEVELS][32];

extern bool CanPlayerCollectPickup( u_int16_t i, u_int16_t Player );
extern bool RaytoSphere2( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, VECTOR * inter, VECTOR * inter2 ); 
extern void DebugPrintf( const char * format, ... );
extern int GetTotalKills(int Killer);
extern int GetTotalDeaths(int Victim);

SHIPCONTROL bot;

#define BOT_FILE "bot1.lua"
#define MIN_WALLDIST 100.0F
#define NUM_SENSORS 26
#define BALL_RADIUS	( 256 * GLOBAL_SCALE )
#endif // LUA_BOT
