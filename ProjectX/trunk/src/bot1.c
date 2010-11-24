#ifdef BOT1
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
#include "xmem.h"
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

 Pickups[ MAXPICKUPTYPES ]
	NextInGroup
	PrevInGroup
	Pos
	Group
	Owner
	Type == PICKUP_* (ex: PICKUP_Trojax)
	Mode == PICKUPMODE_Normal

 BOOL PickupsGot[ MAXPICKUPSTYPES ] 
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
		DebugPrintf("loading %s\n", BOT_FILE);
		luaL_dofile(Lbot, BOT_FILE);
	}
	DebugPrintf("running Lua bot code\n", BOT_FILE);
	lua_getglobal(Lbot, "control_bot");
	botp = lua_newuserdata(Lbot, sizeof(void *));
	*botp = bot;
	luaL_getmetatable(Lbot, "SHIPCONTROLPTR");
	lua_setmetatable(Lbot, -2);
	lua_call(Lbot, 1, 0);
}

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
	control.select_primary   = bot.select_primary;
	control.select_secondary = bot.select_secondary;
	control.turbo            = bot.turbo;
	control.fire_primary     = bot.fire_primary;
	control.fire_secondary   = bot.fire_secondary;
	control.fire_mine        = bot.fire_mine;
	control.drop_primary     = bot.drop_primary;
	control.drop_secondary   = bot.drop_secondary;
	control.drop_shield      = bot.drop_shield;
	control.drop_ammo        = bot.drop_ammo;
	// control_ship will now cap us to max:
	//   move, turn, turbo, roll, and bank
}
#endif // BOT1
