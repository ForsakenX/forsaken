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
/*

	RenderScene()
	{
		ReadInput(); // key/mouse read here
		switch(STATUS)
		{
		case STATUS_Normal:
			MainGame()
			{
				MainRoutines()
				{
					ProcessBot(); // you are here
					ProcessShips()
					{
						ModeControl()
						{
							control_ship()
							{
								// applies key/mouse inputs against
								// player config to perform actions
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

*/
void ProcessBot1( void )
{
	// your ship
	GLOBALSHIP * me = &Ships[WhoIAm];
	// move your own ship
	me->Object.Speed.z = 1;
}
#endif // BOT1
