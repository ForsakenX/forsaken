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
extern GLOBALSHIP Ships[MAX_PLAYERS+1];
extern BYTE WhoIAm;
extern SHIPCONTROL control;
extern float real_framelag;
extern float framelag;
extern float BankAccell;
extern float MaxBankAngle;
extern float MaxMoveSpeed;
extern float MoveAccell;
extern float MoveDecell;
extern float MaxTurboSpeed;
extern float TurboAccell;
extern float TurboDecell;
extern float MaxTurnSpeed;
extern float TurnAccell;
extern float TurnDecell;
extern float MaxRollSpeed;
extern float RollAccell;
extern float RollDecell;
extern float MaxBankAngle;
extern float BankAccell;
extern float BankDecell;
extern float NitroFuel;
/*

	RenderScene()
	{
		ReadInput(); // key/mouse read here
		switch(MyGameStatus)
		{
		case STATUS_Normal:
			MainGame()
			{
				MainRoutines()
				{
					ProcessShips()
					{
						ModeControl()
						{
							control_ship()
							{
								// clear the last settings
								// player config used to apply key/mouse
								ProcessBot1();
								// clamp to max turn/roll/bank/slide
							}
						}
					}
					// rest of game processing goes here
				}
			}
		break;
		}
	}

 Ships[]
	.Object
		.Pos
		.Quat
		.Group
		.Speed
		.Angle
		.DirVector
		.Shield
		.Hull
		.Energy
		.PowerLevel
	.enable
	.ShipThatLastHitMe
	.NumMultiples
	.StealthTime
	.InvulTimer
	.Invul
	.LastAngle
	.Damage
	.Primary
	.Secondary
	.Pickups
	.Mines
	.LastMove
	.Move_Off
	.OrbAmmo[]
	.PrimPowerLevel
	.PrimID
	.SecID
	.SecWeapon
	.headlights
	.OldPos
	.NextPos
	.OldQuat
	.NextQuat
	.OldBank
	.NextBank
	.SuperNashramTimer
	.ShieldHullCount
	.RealPos
	.RealGroup

 SecBulls[ MAXSECONDARYWEAPONBULLETS ]
  .Used
  .NextInGroup
  .PrevInGroup
  .State
  .Flags
  .Type
  .SecType
  .DropCount
  .MoveType
  .Owner
  .LifeCount
  .ColFlag
  .GroupImIn
  .Target
  .TargetType
  .SpeedWanted
  .SpeedInc
  .Speed
  .TurnSpeed
  .ViewCone
  .DirVector
  .UpVector
  .DropVector
  .NumOldPos
	.DirQuat

 PrimBulls[ PRIMARYWEAPONBULLET ]
  .Used
  .Type
  .Owner
  .GroupImIn
  .line
  .TimeInterval

 Pickups[ MAXPICKUPTYPES ]
	NextInGroup
	PrevInGroup
	Pos
	Group
	Owner
	Type == PICKUP_* (ex: PICKUP_Trojax)
	Mode == PICKUPMODE_Normal

 _Bool PickupsGot[ MAXPICKUPSTYPES ] 
	PICKUP_* (ex: PICKUP_Trojax)

 Mloadheader // level
	->CellSize
	->Group[]
		.name
		.center
		.half_size
		.up
		.num_portals
		.*Portal
			.visible
			.Verts
		.cell_origin[]
		.xcells[]
		.ycells[]
		.zcells[]
		.numofcells[]
		.StartPosInThisGroup

	SHIPCONTROL control;
		.pitch    // aim up/down
		.yaw      // aim left/right
		.roll     // roll
		.right    // go negative for left
		.up       // go negative for down
		.forward  // go negative for back
		.select_primary   = primary + 1 ?
		.select_secondary = secondary + 1 ?

		// booleans

		.turbo
		.fire_primary
		.fire_secondary
		.fire_mine
		.drop_primary
		.drop_secondary
		.drop_shield
		.drop_ammo

*/

lua_State *Lbot;

#define BOT_FILE "bot1.lua"

// put your bot logic here
static void control_bot( SHIPCONTROL * bot )
{
	//GLOBALSHIP * me = &Ships[WhoIAm];
	SHIPCONTROL **botp;
	if (!Lbot)
	{
		Lbot = luaL_newstate();
		luaL_openlibs(Lbot);
		/* FIXME: should be lua calls */
		luaopen_vecmat(Lbot);
		luaopen_controls(Lbot);
		luaopen_object(Lbot);
		luaopen_ships(Lbot);
		luaopen_collision(Lbot);
		luaopen_weapons(Lbot);
		luaopen_bullets(Lbot);
		luaopen_enemies(Lbot);
		DebugPrintf("loading %s\n", BOT_FILE);
		lua_dofile(Lbot, BOT_FILE);
	}
	lua_getglobal(Lbot, "control_bot");
	botp = lua_newuserdata(Lbot, sizeof(void *));
	*botp = bot;
	luaL_getmetatable(Lbot, "SHIPCONTROLPTR");
	lua_setmetatable(Lbot, -2);
	if(lua_pcall(Lbot, 1, 0, 0))
	{
		const char * ptr = lua_tostring(Lbot,-1);
		if(!ptr)
			ptr = "Unknown Error";
		Msg("error lua bot: %s",
			(char*) ptr );
	}
}

#define PASS_IF_SET(prop) \
	if(bot.prop) control.prop = bot.prop

// game calls this each frame
void ProcessBot1( void )
{
	// control bot through this
	// by simply using booleans
	// ex: bot->forward = 1;
	SHIPCONTROL bot; ZEROMEM(bot);
	control_bot(&bot);
	// apply the desired actions using
	// same parameters and constraints
	// a human would have per frame
	// mixing it with local user's inputs
	// TODO: we would want functions to perform
  //       actions not supported here from USERCONFIG
	if ( bot.yaw )
	{
		control.yaw  -= -bot.yaw * TurnAccell * MaxTurnSpeed * framelag;
		control.bank += -bot.yaw * BankAccell * MaxBankAngle * framelag;
	}
	if ( bot.pitch )
	{
		control.pitch -= -bot.pitch * TurnAccell * MaxTurnSpeed * framelag;
	}
	if ( bot.roll )
	{
		control.roll += -bot.roll * RollAccell * MaxRollSpeed * framelag;
	}
	if ( bot.right )
	{
		control.right -= -bot.right * MoveAccell * MaxMoveSpeed * framelag;
	}
	if ( bot.up )
	{
		control.up -= -bot.up * MoveAccell * MaxMoveSpeed * framelag;
	}
	if(bot.turbo && !control.turbo)
		PlaySfx( SFX_NitroStart, 0.66F );
	control.turbo = bot.turbo;
	if( bot.turbo )
	{
		if ( NitroFuel > 0.0F )
		{
			control.forward += TurboAccell * MaxTurboSpeed * framelag;
		}
		else
		{
			control.forward += MoveAccell * MaxMoveSpeed * framelag;
		}
	}
	else if ( bot.forward )
	{
		control.forward += bot.forward * MoveAccell * MaxMoveSpeed * framelag;
	}
	PASS_IF_SET(select_primary);
	PASS_IF_SET(select_secondary);
	PASS_IF_SET(turbo);
	PASS_IF_SET(fire_primary);
	PASS_IF_SET(fire_secondary);
	PASS_IF_SET(fire_mine);
	PASS_IF_SET(drop_primary);
	PASS_IF_SET(drop_secondary);
	PASS_IF_SET(drop_shield);
	PASS_IF_SET(drop_ammo);
	// control_ship will now cap us to max:
	//   move, turn, turbo, roll, and bank
}
#endif // LUA_BOT
