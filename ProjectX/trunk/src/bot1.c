#ifdef BOT1
#include <stdio.h>
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
					ProcessBot1(); // you are here
					ProcessShips()
					{
						ModeControl()
						{
							control_ship(control)
							{
								// clear the last settings
								// player config used to apply key/mouse
								ProcessBot2(control); // you are here
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
static void process(void)
{
	//GLOBALSHIP * me = &Ships[WhoIAm];
	control.forward += 1;
	control.fire_primary = 1;
}
void ProcessBot1( void )
{
	process();
	// rest of control_ship will now clamp you to max movements
	// automatically do bank again like control_ship would have
	if(control.yaw)
	{
		int direction = (control.yaw > 0 ? -1 : 1);
		control.bank += direction * BankAccell * MaxBankAngle * framelag;
	}
}
#endif // BOT1
