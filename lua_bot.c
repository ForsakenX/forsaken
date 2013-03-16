#ifdef LUA_BOT
#include "botai_main.h"
#include "botai_sensors.h"
#include "botai_actions.h"
#include "botai_path.h"
 
// sensors.c
extern int TargetShipID;
extern VECTOR FollowTargetPos;
extern int FollowTargetGroup;
 
void ProcessBot1();
void control_bot();

lua_State *Lbot;

#define PASS_IF_SET(prop) \
	if(bot.prop) control.prop = bot.prop

// game calls this each frame
void ProcessBot1()
{
	ZERO_STACK_MEM(bot);
	control_bot();
	if ( bot.yaw )
	{
		control.yaw  -= -bot.yaw * TurnAccell * MaxTurnSpeed * framelag;
		control.bank += -bot.yaw * BankAccell * MaxBankAngle * framelag;
	}
	if ( bot.pitch )
		control.pitch -= -bot.pitch * TurnAccell * MaxTurnSpeed * framelag;
	if ( bot.roll )
		control.roll += -bot.roll * RollAccell * MaxRollSpeed * framelag;
	if ( bot.right )
		control.right -= -bot.right * MoveAccell * MaxMoveSpeed * framelag;
	if ( bot.up )
		control.up -= -bot.up * MoveAccell * MaxMoveSpeed * framelag;
	control.turbo = bot.turbo;
	if( bot.turbo )
	{
		if ( NitroFuel > 0.0F )
			control.forward += TurboAccell * MaxTurboSpeed * framelag;
		else
			control.forward += MoveAccell * MaxMoveSpeed * framelag;
	}
	else if ( bot.forward )
		control.forward += bot.forward * MoveAccell * MaxMoveSpeed * framelag;

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
}

void control_bot()
{
	BOTAI_ClearActions();
	BOTAI_UpdateSensors();
	//BOTAI_AvoidTitans(); // not working properly
	//	BOTAI_AvoidHomingMissiles(); // currently dealt with in AvoidBullets()
	BOTAI_AvoidBullets();
	BOTAI_SelectWeapon( false );

	if( BOTAI_SafeToMove() )
	{
		if( BOTAI_FireTitan() )
			return;
	}

	BOTAI_AimAtTargetShip();
	BOTAI_ShootAtTargetShip();

	if( !BOTAI_SafeToMove() )
	{
		DebugPrintf("not safe to move\n");
		return;
	}

	if( BOTAI_ShootEnemyMines() )
		return;

	// if there is a ship target
	if( TargetShipID > -1 )
	{
		// try and collect any nearby pickups (sliding only so we keep aim on target)
		if(	BOTAI_CollectNearestPickup(true) )
			return;

		// maintain distance if i can see the target ship - replace later with dogfight etc.
		if( BOTAI_MaintainDistanceToTargetShip() )	
			return;

		// i have a target but i can't see it and it's safe to move so lets try and get a line of sight
		FollowTargetPos = Ships[TargetShipID].Object.Pos;
		FollowTargetGroup = Ships[TargetShipID].Object.Group;
		BOTAI_FollowNodeNetwork(true, true);
	}   

	// no ship targets and it's safe to move
	else
	{
		// collect any nearby pickups
		if(	BOTAI_CollectNearestPickup(false) )
			return;

		// no nearby pickups so roam around
		DebugPrintf("following node network...\n");
		BOTAI_FollowNodeNetwork(false, false);
	}
}

#endif // LUA_BOT
