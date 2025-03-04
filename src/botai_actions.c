#ifdef LUA_BOT
#include "botai_sensors.h"
#include "botai_actions.h"

// sensors.c
extern VECTOR FollowTargetPos;
extern VECTOR MissileOrigDirVector;
extern bool AllSensorsClear;
extern bool DEBUG_AVOIDANCE;
extern bool FriendlyFire;
extern bool MissileAvoidanceSet;
extern bool ReverseNetwork;
extern bool initialised;
extern float DistWall[NUM_SENSORS];
extern float IFiredTitan;
extern float PrevHull;
extern float PrevShield;
extern float TargetShipDistance;
extern float WhenWillHitSlide[NUM_SENSORS];
extern float accuracy;
extern int AvoidMove;
extern int CurrentNode;
extern int FollowTargetGroup;
extern int GettingPickup;
extern int HomingMissile;
extern int PrevDeaths;
extern int PrevHomingMissile;
extern int PrevKills;
extern int TargetMine;
extern int TargetNode;
extern int TargetShipHealth;
extern int TargetShipID;
extern char	* WepMessages[];

void BOTAI_ClearActions()
{
	BOTAI_SetAction( &bot.forward, 0.0F, "ClearActions(): forward" );
	BOTAI_SetAction( &bot.up, 0.0F, "ClearActions(): up" );
	BOTAI_SetAction( &bot.right, 0.0F, "ClearActions(): right" );
	BOTAI_SetAction( &bot.pitch, 0.0F, "ClearActions(): pitch" );
	BOTAI_SetAction( &bot.yaw, 0.0F, "ClearActions(): yaw" );
	BOTAI_SetAction( &bot.roll, 0.0F, "ClearActions(): roll" );
	BOTAI_SetAction( &bot.turbo, 0.0F, "ClearActions(): turbo" );
	BOTAI_SetAction( &bot.fire_primary, 0.0F, "ClearActions(): fire_primary" );
	BOTAI_SetAction( &bot.fire_secondary, 0.0F, "ClearActions(): fire_secondary" );
}

bool BOTAI_SetAction( float * action, float value, char * str )
{
	// enforce bounds
	if( value < -1.0F )
		value = -1.0F;
	else if( value > 1.0F )
		value = 1.0F;

	// try and set action
	if( *action != 0.0F && value != 0.0F )
	{
		DebugPrintf("%s already set\n", str);
		return false;
	}
	else
	{
		DebugPrintf("%s set\n", str);
		*action = value;
		return true;
	}
}

bool BOTAI_FireTitan()
{
	int i;
	int wall;
	float dist;
	float closest = BIGDISTANCE;
	VECTOR TempVector;
	VECTOR Move_Dir;
	VECTOR ColPnt;
	u_int16_t TmpGroup;

	// no target to fire at
	if( TargetShipID < 0 )
		return false;

	// do we have the titan? 
	if( TITANSTARMISSILE != Ships[WhoIAm].Secondary && SecondaryAmmo[TITANSTARMISSILE] <= 0)
		return false;
	else
		Ships[WhoIAm].Secondary = TITANSTARMISSILE;

	// too close to target or too far away
	dist = DistanceVector2Vector(&Ships[WhoIAm].Object.Pos, &Ships[TargetShipID].Object.Pos);
	if(dist < 500.0F || dist > 2000.0F)
	{
		//DebugPrintf("too close or too far to target to fire titan\n");
		return false;
	}

	// for each r/l/u/d wall sensor
	for(i = 0; i < 4; i++)
	{
		// identify the closest wall
		if(DistWall[i] < closest)
		{
			closest = DistWall[i];
			wall = i;
		}
	}

	// slide to the wall
	if(closest > 150.0F)
	{
		switch(wall)
		{
			case 0:
				BOTAI_SetAction( &bot.right, 1.0F, "FireTitan() right" );
				break;
			case 1:
				BOTAI_SetAction( &bot.right, -1.0F, "FireTitan() left" );
				break;
			case 2:
				BOTAI_SetAction( &bot.up, 1.0F, "FireTitan() up" );
				break;
			case 3:
				BOTAI_SetAction( &bot.up, -1.0F, "FireTitan() down" );
				break;
		}
	}
	// aim towards wall
	else
	{
		// calculate collision point
		ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Move_Dir );
		Move_Dir.x *= MaxColDistance;
		Move_Dir.y *= MaxColDistance;
		Move_Dir.z *= MaxColDistance;
		BackgroundCollide( &MCloadheadert0, &Mloadheader, &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Move_Dir, (VECTOR *) &ColPnt, &TmpGroup, &TempVector, &TempVector, false, NULL ); 

		// distance between target and collision point should be less than distance between me and target
		if( DistanceVector2Vector(&Ships[TargetShipID].Object.Pos, &ColPnt) > TargetShipDistance 
				// and target needs to be reachable by the collision point
				|| ( !BOTAI_ClearLOS(&Ships[TargetShipID].Object.Pos, Ships[TargetShipID].Object.Group, &ColPnt) )
				// and i have line of sight to the target
				|| ( !BOTAI_ClearLOS(&Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Ships[TargetShipID].Object.Pos) ) )
		{
			//DebugPrintf("aiming at target\n");
			BOTAI_AimAtTarget( &Ships[WhoIAm].Object.FinalInvMat, &Ships[WhoIAm].Object.Pos, &Ships[TargetShipID].Object.Pos);
		}
		// can see target so aim front sensor towards wall
		else if(DistWall[8] > 500.0F)
		{	
			//DebugPrintf("aiming toward wall\n");
			switch(wall)
			{
				case 0:
					BOTAI_SetAction( &bot.yaw, 1.0F, "FireTitan() yaw right" );
					break;
				case 1:
					BOTAI_SetAction( &bot.yaw, -1.0F, "FireTitan() yaw left" );
					break;
				case 2:
					BOTAI_SetAction( &bot.pitch, -1.0F, "FireTitan() pitch up" );
					break;
				case 3:
					BOTAI_SetAction( &bot.pitch, 1.0F, "FireTitan() pitch down" );
					break;
			}
			// helps to break alternating between too close and too far
			BOTAI_SetAction( &bot.forward, -1.0F, "FireTitan() reverse" );

		}
		// can see target but i'm too close to wall
		else if(DistWall[8] < 400.0F)
		{	
			//DebugPrintf("aiming away from wall\n");
			switch(wall)
			{
				case 0:
					BOTAI_SetAction( &bot.yaw, -1.0F, "FireTitan() yaw left" );
					break;
				case 1:
					BOTAI_SetAction( &bot.yaw, 1.0F, "FireTitan() yaw right" );
					break;
				case 2:
					BOTAI_SetAction( &bot.pitch, 1.0F, "FireTitan() pitch down" );
					break;
				case 3:
					BOTAI_SetAction( &bot.pitch, -1.0F, "FireTitan() pitch up" );
					break;
			}
			// helps to break alternating between too close and too far
			BOTAI_SetAction( &bot.forward, -1.0F, "FireTitan() reverse" );
		}
		// just right
		else
		{
			//DebugPrintf("chocs away old boy!\n");
			Ships[WhoIAm].Secondary = TITANSTARMISSILE;
			BOTAI_SetAction( &bot.fire_secondary, 1.0F, "FireTitan() fire secondary" );
			BOTAI_SetAction( &bot.forward, -1.0F, "FireTitan() reverse" );
			BOTAI_SetAction( &bot.right, -1.0F, "FireTitan() left" );
			IFiredTitan = 180.0F;
		}
	}

	return true;
}

void BOTAI_FireMissiles()
{
	// don't fire missiles if the target isn't in front of me and don't shoot directly into a wall
	//if(!BOTAI_InViewCone( &Ships[WhoIAm].Object.Pos, &Ships[WhoIAm].Object.FinalMat, &Ships[TargetShipID].Object.Pos, (float) COSD( 45.0F ) ) && DistWall[8] < 200.0F)
	//		return;


	// don't fire missiles unless we have a line of sight to the target
	if( TargetShipID < 0 )
		return;
	if( !BOTAI_ClearLOS(&Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Ships[TargetShipID].Object.Pos))
		return;

	// set order of preference
	if(SCATTERMISSILE == Ships[WhoIAm].Secondary || SecondaryAmmo[SCATTERMISSILE] > 0)
		Ships[WhoIAm].Secondary = SCATTERMISSILE;
	else if(SOLARISMISSILE == Ships[WhoIAm].Secondary || SecondaryAmmo[SOLARISMISSILE] > 0)
		Ships[WhoIAm].Secondary = SOLARISMISSILE;
	else if(MULTIPLEMISSILE == Ships[WhoIAm].Secondary || SecondaryAmmo[MULTIPLEMISSILE] > 0)
		Ships[WhoIAm].Secondary = MULTIPLEMISSILE;
	else if(MUGMISSILE == Ships[WhoIAm].Secondary || SecondaryAmmo[MUGMISSILE] > 0)
		Ships[WhoIAm].Secondary = MUGMISSILE;

	// fire missiles, but don't fire titan here
	if(TITANSTARMISSILE != Ships[WhoIAm].Secondary)
		BOTAI_SetAction( &bot.fire_secondary, 1.0F, "FireMissiles() fire secondary" );
}

#define TOL 50.0F
/* triple-chords to target pos in a straight line; returns true if reached target */
bool BOTAI_MoveToTargetNew(VECTOR * TPos)
{
	VECTOR Slide;
	VECTOR TriChordVector;
	VECTOR WantedVector;
	VECTOR NormVector;
	VECTOR DirVector;
	VECTOR FwdDirVector;
	float xAngle;
	float yAngle;
	float Cos;

	////DebugPrintf("moving to %f %f %f\n", TPos->x, TPos->y, TPos->z);

	// -- calculate direction vector from my position to target 
	NormVector.x = TPos->x - Ships[WhoIAm].Object.Pos.x;
	NormVector.y = TPos->y - Ships[WhoIAm].Object.Pos.y;
	NormVector.z = TPos->z - Ships[WhoIAm].Object.Pos.z;

	DirVector = NormVector;

	// apply that vector to my matrix to make direction relative to my direction and rotation 
	//ApplyMatrix( &Ships[WhoIAm].Object.FinalInvMat, &NormVector, &DirVector );
	NormaliseVector( &DirVector );
	//DebugPrintf("straight x = %f y = %f z = %f\n", DirVector.x, DirVector.y, DirVector.z);

	// calculate the desired vector
	// forward right
	Slide.x = 1.0F;
	Slide.y = 0.0F;
	Slide.z = 1.0F;


	// calculate the triple chord direction vector 
	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Slide, &TriChordVector );
	NormaliseVector( &TriChordVector );
	//DebugPrintf("trivec x = %f y = %f z = %f\n", TriChordVector.x, TriChordVector.y, TriChordVector.z);


	// difference between wanted vector and slide vector
	WantedVector.x = DirVector.x - TriChordVector.x;
	WantedVector.y = DirVector.y - TriChordVector.y;
	WantedVector.z = DirVector.z - TriChordVector.z;
	////DebugPrintf("W x = %f y = %f z = %f\n", WantedVector.x, WantedVector.y, WantedVector.z);

	// angle between trichord vector and wanted vector, == 1.0F when aligned perfectly
	Cos = DotProduct( &DirVector, &TriChordVector );
	////DebugPrintf("Cos = %f\n", Cos);

	return false;

	// left/right angle
	xAngle = (float) acos( WantedVector.x );
	xAngle = 90.0F - R2D( xAngle );
	if( WantedVector.z < 0.0F )
		xAngle = 180.0F - xAngle;
	if( xAngle > 180.0F )
		xAngle -= 360.0F;
	////DebugPrintf("X Angle %f\n", xAngle);

	// up/down angle
	yAngle = (float) acos( WantedVector.y );
	yAngle = 90.0F - R2D( yAngle );
	yAngle *= -1.0F;
	if( yAngle > 180.0F )
		yAngle -= 360.0F;
	////DebugPrintf("Y Angle %f\n", yAngle);

	if( xAngle > 0.0F )
		BOTAI_SetAction( &bot.yaw, 1.0F, "MoveToTargetNew() yaw right" );
	else if( xAngle < 0.0F )
		BOTAI_SetAction( &bot.yaw, -1.0F, "MoveToTargetNew() yaw left" );

	if( yAngle < 0.0F && ( WantedVector.z > 0.0F ))
		BOTAI_SetAction( &bot.pitch, -1.0F, "MoveToTargetNew() pitch up" );
	else if( yAngle > 0.0F && ( WantedVector.z > 0.0F ))
		BOTAI_SetAction( &bot.pitch, 1.0F, "MoveToTargetNew() pitch down" );


	// Reached Target
	if(Ships[WhoIAm].Object.Pos.x < TPos->x + TOL && Ships[WhoIAm].Object.Pos.x > TPos->x - TOL
			&& Ships[WhoIAm].Object.Pos.y < TPos->y + TOL && Ships[WhoIAm].Object.Pos.y > TPos->y - TOL
			&& Ships[WhoIAm].Object.Pos.z < TPos->z + TOL && Ships[WhoIAm].Object.Pos.z > TPos->z - TOL)
		return true;
	else
		return false;
}

/* (buggy) always triple chords up/right/forward to target */
bool BOTAI_MoveToTarget(VECTOR * TPos)
{
	VECTOR NormVector;
	VECTOR DirVector;
	float xAngle;
	float yAngle;

	//DebugPrintf("moving to %f %f %f\n", TPos->x, TPos->y, TPos->z);


	// set slide movements and check if at target pos
	if(BOTAI_SlideToTarget(TPos))
		return true;

	// adjust the aim

	// -- Calculate angles
	NormVector.x = TPos->x - Ships[WhoIAm].Object.Pos.x;
	NormVector.y = TPos->y - Ships[WhoIAm].Object.Pos.y;
	NormVector.z = TPos->z - Ships[WhoIAm].Object.Pos.z;
	ApplyMatrix( &Ships[WhoIAm].Object.FinalInvMat, &NormVector, &DirVector );
	NormaliseVector( &DirVector );
	////DebugPrintf("x = %f y = %f z = %f\n", DirVector.x, DirVector.y, DirVector.z);

	// left/right angle
	xAngle = (float) acos( DirVector.x );
	xAngle = 90.0F - R2D( xAngle );
	if( DirVector.z < 0.0F )
		xAngle = 180.0F - xAngle;
	if( xAngle > 180.0F )
		xAngle -= 360.0F;
	////DebugPrintf("X Angle %f\n", xAngle);

	// up/down angle
	yAngle = (float) acos( DirVector.y );
	yAngle = 90.0F - R2D( yAngle );
	yAngle *= -1.0F;
	if( yAngle > 180.0F )
		yAngle -= 360.0F;
	////DebugPrintf("Y Angle %f\n", yAngle);

	// -- angle aim
	// sliding right
	if( bot.right == 1.0F)
	{
		// aim left
		if(xAngle < 30.0F)
			BOTAI_SetAction( &bot.yaw, -1.0F, "MoveToTarget() yaw left" );
		// aim right
		else if(xAngle > 40.0F)
			BOTAI_SetAction( &bot.yaw, 1.0F, "MoveToTarget() yaw right" );
	}

	// sliding left
	else if( bot.right == -1.0F)
	{
		// aim left
		if(xAngle < -40.0F)
			BOTAI_SetAction( &bot.yaw, -1.0F, "MoveToTarget() yaw left" );
		// aim right
		else if(xAngle > -30.0F)
			BOTAI_SetAction( &bot.yaw, 1.0F, "MoveToTarget() yaw right" );
	}

	// sliding up
	if(bot.up == 1.0F)
	{
		// aim up
		if(yAngle < -40.0F)
			BOTAI_SetAction( &bot.pitch, -1.0F, "MoveToTarget() pitch up" );
		// aim down
		else if(yAngle > -30.0F)
			BOTAI_SetAction( &bot.pitch, 1.0F, "MoveToTarget() pitch down" );
	}

	// sliding down
	else if(bot.up == -1.0F)
	{
		// aim up
		if(yAngle > 30.0F)
			BOTAI_SetAction( &bot.pitch, -1.0F, "MoveToTarget() pitch up" );
		// aim down
		else if(yAngle < 40.0F)
			BOTAI_SetAction( &bot.pitch, 1.0F, "MoveToTarget() pitch down" );
	}

	return false;
}

/* Slides to target pos without aiming at it; returns true if reached target */
bool BOTAI_SlideToTarget(VECTOR * TPos)
{
	VECTOR NormVector;
	VECTOR DirVector;
	float xAngle;
	float yAngle;

	//DebugPrintf("sliding to target\n");

	// -- Calculate angles
	NormVector.x = TPos->x - Ships[WhoIAm].Object.Pos.x;
	NormVector.y = TPos->y - Ships[WhoIAm].Object.Pos.y;
	NormVector.z = TPos->z - Ships[WhoIAm].Object.Pos.z;
	ApplyMatrix( &Ships[WhoIAm].Object.FinalInvMat, &NormVector, &DirVector );
	NormaliseVector( &DirVector );
	////DebugPrintf("x = %f y = %f z = %f\n", DirVector.x, DirVector.y, DirVector.z);

	// left/right angle
	xAngle = (float) acos( DirVector.x );
	xAngle = 90.0F - R2D( xAngle );
	if( DirVector.z < 0.0F )
		xAngle = 180.0F - xAngle;
	if( xAngle > 180.0F )
		xAngle -= 360.0F;
	////DebugPrintf("X Angle %f\n", xAngle);

	// up/down angle
	yAngle = (float) acos( DirVector.y );
	yAngle = 90.0F - R2D( yAngle );
	yAngle *= -1.0F;
	if( yAngle > 180.0F )
		yAngle -= 360.0F;
	////DebugPrintf("Y Angle %f\n", yAngle);
	////DebugPrintf("z %f\n", DirVector.z);


	// -- execute slide movements
	// sliding right
	if(xAngle > 0.0F && DistWall[0] > 100.0F)
		BOTAI_SetAction( &bot.right, 1.0F, "SlideToTarget() right" );
	// sliding left
	else if(xAngle < 0.0F && DistWall[1] > 100.0F)
		BOTAI_SetAction( &bot.right, -1.0F, "SlideToTarget() left" );

	// sliding up
	if(DistWall[2] > 100.0F) 
	{
		if(yAngle < 0.0F )
			BOTAI_SetAction( &bot.up, 1.0F, "SlideToTarget() up" );
	}
	// sliding down
	if(DistWall[3] > 100.0F) 
	{
		if(yAngle > 0.0F)
			BOTAI_SetAction( &bot.up, -1.0F, "SlideToTarget() down" );
	}

	// forward
	if(DirVector.z > 0.0F && DistWall[8] > 100.0F) 
		BOTAI_SetAction( &bot.forward, 1.0F, "SlideToTarget() forward" );
	// backward
	else if(DirVector.z < 0.0F && DistWall[9] > 100.0F )
		BOTAI_SetAction( &bot.forward, -1.0F, "SlideToTarget() reverse" );


	// Reached Target
	if(Ships[WhoIAm].Object.Pos.x < TPos->x + TOL && Ships[WhoIAm].Object.Pos.x > TPos->x - TOL
			&& Ships[WhoIAm].Object.Pos.y < TPos->y + TOL && Ships[WhoIAm].Object.Pos.y > TPos->y - TOL
			&& Ships[WhoIAm].Object.Pos.z < TPos->z + TOL && Ships[WhoIAm].Object.Pos.z > TPos->z - TOL)
		return true;
	else
		return false;
}

bool BOTAI_ShootEnemyMines()
{
	int i;
	float dist = 0.0F;
	float TeamDist = 0.0F;
	float nearest = BIGDISTANCE;
	float mindist = BIGDISTANCE;

	TargetMine = -1;

	// search all secondary bullets
	for(i = 0; i< MAXSECONDARYWEAPONBULLETS; i++)
	{
		// for mines that aren't my own
		if(SecBulls[i].SecType == SEC_MINE && SecBulls[i].Owner !=WhoIAm && SecBulls[i].Used)
		{
			// that i can see
			if( BOTAI_ClearLOS( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &SecBulls[i].Pos ))
			{
				// find the nearest
				dist = DistanceVector2Vector(&Ships[WhoIAm].Object.Pos, &SecBulls[i].Pos);
				if(dist < nearest)
				{
					TargetMine = i;
					nearest = dist;
				}
			}
		}
	}

	// if i have a mine target
	if(TargetMine > -1)
	{
		// don't hit any team members 
		if( TeamGame )
		{
			// i will hit team mate with the bullet
			if( BOTAI_FriendlyFireCheck() )
				return false;

			// team mate will endure shockwave damage
			for(i = 0; i < MAX_PLAYERS; i++)
			{
				if(TeamNumber[WhoIAm] != TeamNumber[i])
					continue;

				TeamDist = DistanceVector2Vector(&Ships[i].Object.Pos, &SecBulls[TargetMine].Pos);
				if( TeamDist < dist )
					return false;
			}
		}

		// aim at mine
		BOTAI_AimAtTarget(&Ships[WhoIAm].Object.FinalInvMat, &Ships[WhoIAm].Object.Pos, &SecBulls[TargetMine].Pos);

		// try not to get too close to mines
		switch(SecBulls[TargetMine].Weapon)
		{
			// ideal safest distance
			case QUANTUMMINE:
				dist = (BALL_RADIUS * QUANTUM_SHOCKWAVE) + SHIP_RADIUS;
				break;
			default:
				dist = (BALL_RADIUS * PURGE_SHOCKWAVE) + SHIP_RADIUS;
				break;
		}

		// however, if i only have short range weapons then i need to be a bit closer to hit it
		BOTAI_SelectWeapon( true );
		switch( Ships[WhoIAm].Primary )
		{
			case PYROLITE_RIFLE: 
			case SUSS_GUN:
				if( dist > 1300.0F )
					dist = 1300.0F;
				mindist = dist;
				break;
		}

		//DebugPrintf("i have a mine target at %f distance\n", nearest);
		//DebugPrintf("min dist = %f\n", (BALL_RADIUS * QUANTUM_SHOCKWAVE) + SHIP_RADIUS);

		// back up to safest distance or until i hit a wall
		if( nearest < dist && DistWall[9] > 100.0F )
			BOTAI_SetAction( &bot.forward, -1.0F, "ShootEnemyMines() reverse" );
		// too far away from the mine with the current weapon i have so move forward
		else if( nearest > mindist && DistWall[8] > 100.0F )
			BOTAI_SetAction( &bot.forward, 1.0F, "ShootEnemyMines() forward" );
		// at an ok distance 
		else
		{
			// release trojax
			if(Ships[WhoIAm].Primary == TROJAX && PowerLevel > 10.0F)
				BOTAI_SetAction( &bot.fire_primary, 0.0F, "ShootEnemyMines() fire primary off" );
			// shoot primary
			else
				BOTAI_SetAction( &bot.fire_primary, 1.0F, "ShootEnemyMines() fire primary" );
		}
		return true;
	}
	else
		return false;
}

bool BOTAI_CollectNearestPickup(bool SlideOnly)
{
	// Pickup identified
	if(GettingPickup > -1)
	{
		// Make sure we search for nodes after
		CurrentNode = -1;

		// if we lost LOS 
		if(!BOTAI_ClearLOS( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Pickups[GettingPickup].Pos ) || Pickups[GettingPickup].Type == (u_int16_t) -1)
		{
			DebugPrintf("lost pickup los %s\n", WepMessages[ Pickups[GettingPickup].Type ]);
			GettingPickup = -1;
			return false;
		}

		// otherwise, move to the target and collect
		if(!SlideOnly)
		{
			DebugPrintf("moving to pickup %s\n", WepMessages[ Pickups[GettingPickup].Type ]);
			if( BOTAI_MoveToTarget(&Pickups[GettingPickup].Pos) )
				GettingPickup = -1;
		}
		// try and slide near it 
		else
		{
			DebugPrintf("sliding to pickup %s\n", WepMessages[ Pickups[GettingPickup].Type ]);
			if( BOTAI_SlideToTarget(&Pickups[GettingPickup].Pos) )
				GettingPickup = -1;
		}

		return true;
	}
	else
		return false;
}

bool BOTAI_AimAtTarget( MATRIX * InvMat , VECTOR * SPos, VECTOR * TPos )
{
	VECTOR WantedDir;
	VECTOR TempDir;
	float Angle;
	float OnTarget = true;

	WantedDir.x = ( TPos->x - SPos->x );
	WantedDir.y = ( TPos->y - SPos->y );
	WantedDir.z = ( TPos->z - SPos->z );
	ApplyMatrix( InvMat, &WantedDir, &TempDir );

	NormaliseVector( &TempDir );
	Angle = (float) acos( TempDir.x );
	Angle = 90.0F - R2D( Angle );

	if( TempDir.z < 0.0F )
		Angle = 180.0F - Angle;

	if( Angle > 180.0F )
		Angle -= 360.0F;

	if( Angle > 0.0F )
	{
		BOTAI_SetAction( &bot.yaw, 1.0F, "AimAtTarget() yaw right" );
		if( Angle > 3.0F )
			OnTarget = false;
	}
	else if( Angle < 0.0F )
	{
		BOTAI_SetAction( &bot.yaw, -1.0F, "AimAtTarget() yaw left" );
		if( Angle < -3.0F )
			OnTarget = false;
	}

	Angle = (float) acos( TempDir.y );
	Angle = 90.0F - R2D( Angle );
	Angle *= -1.0F;

	if( Angle > 180.0F )
		Angle -= 360.0F;

	if( Angle < 0.0F && ( TempDir.z > 0.0F ))
	{
		BOTAI_SetAction( &bot.pitch, -1.0F, "AimAtTarget() pitch up" );
		if(Angle < -3.0F)
			OnTarget = false;
	}
	else if( Angle > 0.0F && ( TempDir.z > 0.0F ))
	{
		BOTAI_SetAction( &bot.pitch, 1.0F, "AimAtTarget() pitch down" );
		if(Angle > 3.0F)
			OnTarget = false;
	}

	return OnTarget;
}

void BOTAI_AvoidHomingMissiles()
{
	VECTOR NormVector;
	VECTOR DirVector;
	float xAngle;
	float yAngle;

	if(HomingMissile > -1)
	{
		// -- decide what movement to perform
		// new missile
		//if(!MissileAvoidanceSet)
		{
			//DebugPrintf("set details for missile %d\n", HomingMissile);
			MissileAvoidanceSet = true;
			NormVector.x = SecBulls[HomingMissile].Pos.x - Ships[WhoIAm].Object.Pos.x;
			NormVector.y = SecBulls[HomingMissile].Pos.y - Ships[WhoIAm].Object.Pos.y;
			NormVector.z = SecBulls[HomingMissile].Pos.z - Ships[WhoIAm].Object.Pos.z;
			ApplyMatrix( &Ships[WhoIAm].Object.FinalInvMat, &NormVector, &DirVector );
			NormaliseVector( &DirVector );
			MissileOrigDirVector = DirVector;
		}


		// left/right angle
		xAngle = (float) acos( MissileOrigDirVector.x );
		xAngle = 90.0F - R2D( xAngle );
		if( MissileOrigDirVector.z < 0.0F )
			xAngle = 180.0F - xAngle;
		if( xAngle > 180.0F )
			xAngle -= 360.0F;
		//DebugPrintf("x = %f\n", xAngle);

		// up/down angle
		yAngle = (float) acos( MissileOrigDirVector.y );
		yAngle = 90.0F - R2D( yAngle );
		yAngle *= -1.0F;
		if( yAngle > 180.0F )
			yAngle -= 360.0F;
		//DebugPrintf("y = %f\n", yAngle);

		// -- execute slide movements

		// target is directly in front or behind me
		if((xAngle > -22.5F && xAngle < 22.5F) || (xAngle < -157.5F || xAngle > 157.5F))
		{
			// slide either left or right, doesn't matter
			BOTAI_SetAction( &bot.right, 1.0F, "AvoidHomingMissiles() right" );
		}
		// target is directly to the side of me
		else if((xAngle > 67.5F && xAngle < 112.5F) || (xAngle < -67.5F && xAngle > -112.5F))
		{
			// forward or backward, doesn't matter
			BOTAI_SetAction( &bot.forward, 1.0F, "AvoidHomingMissiles() forward" );
		}

		// target is to my front left 
		else if(xAngle > -67.5F && xAngle < -22.5F)
		{
			// move forward right 
			BOTAI_SetAction( &bot.forward, 1.0F, "AvoidHomingMissiles() forward" );
			BOTAI_SetAction( &bot.right, 1.0F, "AvoidHomingMissiles() right" );
		}

		// target is to my rear left 
		else if(xAngle > -157.5F && xAngle < -112.5F)
		{
			// move back left
			BOTAI_SetAction( &bot.forward, -1.0F, "AvoidHomingMissiles() reverse" );
			BOTAI_SetAction( &bot.forward, -1.0F, "AvoidHomingMissiles() left" );
		}

		// target is to my front right
		else if(xAngle > 22.5F && xAngle < 67.5F)
		{
			// move front left 
			BOTAI_SetAction( &bot.forward, 1.0F, "AvoidHomingMissiles() forward" );
			BOTAI_SetAction( &bot.right, -1.0F, "AvoidHomingMissiles() left" );
		}

		// target is to my rear right 
		else if(xAngle > 112.5F && xAngle < 157.5F)
		{
			// move back right 
			BOTAI_SetAction( &bot.forward, -1.0F, "AvoidHomingMissiles() reverse" );
			BOTAI_SetAction( &bot.right, 1.0F, "AvoidHomingMissiles() right" );
		}

		if(yAngle > 0.0F)
			BOTAI_SetAction( &bot.up, -1.0F, "AvoidHomingMissiles() down" );
		else
			BOTAI_SetAction( &bot.up, 1.0F, "AvoidHomingMissiles() up" );
	}
}

void BOTAI_AvoidBullets()
{
	int i = 0;
	float dist = 0.0F;
	float longest = 0.0F;

	// bullet will hit us if we stay in current position
	if(BOTAI_WhenWillBulletHitMe(&Ships[WhoIAm].Object.Pos) > -1.0F)
	{
		if(AvoidMove < 0)
		{
			// find perfect spot
			for(i = 0; i < NUM_SENSORS; i++)
			{
				if(WhenWillHitSlide[i] > -1.0F)
					DebugPrintf("will hit %d\n", i);

				// nothing will hit and there's space to move
				if(WhenWillHitSlide[i] < 0.0F && DistWall[i] > MIN_WALLDIST)	
				{
					// move to the one with most room
					if(DistWall[i] > dist)
					{	
						//DebugPrintf("using %d instead of %d because %f > %f\n", i, AvoidMove, DistWall[i], dist);
						AvoidMove = i;// BOTAI_CenterCheck(i);
						DebugPrintf("using %d\n", AvoidMove);
						dist = DistWall[i];
					}
				}
			}

			// couldn't find perfect so try safest
			if(AvoidMove < 0)
			{
				for(i = 0; i < NUM_SENSORS; i++)
				{
					// something will hit but i have space and some time
					if(WhenWillHitSlide[i] > longest && DistWall[i] > MIN_WALLDIST)
					{
						AvoidMove = i;
						longest = WhenWillHitSlide[i];
					}
				}
				//if(DEBUG_AVOIDANCE)
				DebugPrintf("couldn't find perfect spot: using %d\n", AvoidMove);
			}
			//else if(DEBUG_AVOIDANCE)
			DebugPrintf("found perfect spot: %d\n", AvoidMove);
		}

		// carry out the movement
		switch(AvoidMove)
		{
			case -1: 
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				break;
			case 0:
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				break;
			case 1: 
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				break;
			case 2: 
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				break;
			case 3: 
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				break;
			case 4:
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				break;
			case 5: 
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				break;
			case 6:
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				break;
			case 7:
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				break;
			case 8: 
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 9:
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 10:
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 11:
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				break;
			case 12:
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				break;
			case 13:
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				break;
			case 14:
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 15:
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 16:
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 17:
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.forward, -1.0F, "AvoidBullets() reverse" );
				break;
			case 18:
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 19:
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 20:
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 21:
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 22:
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 23:
				BOTAI_SetAction( &bot.up, 1.0F, "AvoidBullets() up" );
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 24:
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				BOTAI_SetAction( &bot.right, 1.0F, "AvoidBullets() right" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;
			case 25:
				BOTAI_SetAction( &bot.up, -1.0F, "AvoidBullets() down" );
				BOTAI_SetAction( &bot.right, -1.0F, "AvoidBullets() left" );
				BOTAI_SetAction( &bot.forward, 1.0F, "AvoidBullets() forward" );
				break;

			default: DebugPrintf("invalid move: %d\n", AvoidMove); // should never get here
					 break;
		}
	}
	else
		AvoidMove = -1;

	if(AvoidMove < 0 && DEBUG_AVOIDANCE)
		DebugPrintf("safe......................\n");
}

void BOTAI_SelectWeapon( bool MineTarget )
{
	if( MineTarget )
	{
		if( PrimaryWeaponsGot[ LASER ] && !LaserOverheated) 	
			Ships[WhoIAm].Primary = LASER;
		else if( GeneralAmmo > 0.0F )
			Ships[WhoIAm].Primary = PULSAR;
		else if( PrimaryWeaponsGot[ PYROLITE_RIFLE ] && PyroliteAmmo > 0.0F )
			Ships[WhoIAm].Primary = PYROLITE_RIFLE;
		else if( PrimaryWeaponsGot[ SUSS_GUN ] && SussGunAmmo > 0.0F )
			Ships[WhoIAm].Primary = SUSS_GUN;
		else
			Ships[WhoIAm].Primary = PULSAR;
	}

	else if(TargetShipID > -1)
	{
		// target is weak and i have lasers, finish him!
		if(TargetShipHealth < 64 && PrimaryWeaponsGot[ LASER ] && !LaserOverheated)
			Ships[WhoIAm].Primary = LASER;
		// use pyro if they're close
		else if(PrimaryWeaponsGot[ PYROLITE_RIFLE ] && TargetShipDistance < 1000.0F)
			Ships[WhoIAm].Primary = PYROLITE_RIFLE;
		// use lasers if they're far away or if they're closer and i don't have trojax
		else if( (PrimaryWeaponsGot[ LASER ] && !LaserOverheated && TargetShipDistance > 1500.0F)
				|| (PrimaryWeaponsGot[ LASER ] && !LaserOverheated && !PrimaryWeaponsGot[ TROJAX ]) )
			Ships[WhoIAm].Primary = LASER;
		// use trojax
		else if(PrimaryWeaponsGot[ TROJAX ])
			Ships[WhoIAm].Primary = TROJAX;
		// last choice pulsars
		else
			Ships[WhoIAm].Primary = PULSAR;
	}
}

void BOTAI_AimAtTargetShip()
{
	VECTOR NewPos;

	if(TargetShipID > -1)
	{
		// make a linear prediction
		BOTAI_LookAhead( accuracy, &Ships[WhoIAm].Object.Pos, TargetShipID, &NewPos, PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ]);

		// aim at the target
		if(BOTAI_AimAtTarget( &Ships[WhoIAm].Object.FinalInvMat, &Ships[WhoIAm].Object.Pos, &NewPos ))
			accuracy = Random_Range_Float(2.0F) - 1.0F;
	}
}

void BOTAI_ShootAtTargetShip()
{
	if(TargetShipID > -1)
	{
		// if clear line of sight, shoot primary at target
		if( BOTAI_ClearLOS(&Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Ships[TargetShipID].Object.Pos))
		{
			// reset node network flag
			CurrentNode = -1;

			// fire primary if i wont hit any team members
			if(!FriendlyFire) 
			{		
				// tap shot trojax
				if(Ships[WhoIAm].Primary == TROJAX && PowerLevel > 10.0F)
					BOTAI_SetAction( &bot.fire_primary, 0.0F, "ShootAtTargetShip() fire primary off" );
				// shoot primary
				else
					BOTAI_SetAction( &bot.fire_primary, 1.0F, "ShootAtTargetShip() fire primary" );
			}
		}

		// shoot any missiles we have
		if(!FriendlyFire)
			BOTAI_FireMissiles();	
	}
}

bool BOTAI_MaintainDistanceToTargetShip()
{
	float DesiredDistance;

	if( TargetShipID < 0 )
		return false;

	// if i can see the target ship
	if( BOTAI_ClearLOS(&Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Ships[TargetShipID].Object.Pos) )
	{
		// set the distance wanted for short range weapons
		if(Ships[WhoIAm].Primary == PYROLITE_RIFLE || Ships[WhoIAm].Primary == SUSS_GUN)
			DesiredDistance = 800.0F;
		// all other weapons
		//else if( BOTAI_ComparativeEnemyStrength() > 0 ) // get in their face if we're stronger
		//	DesiredDistance = 500.0F;
		else
			DesiredDistance = 1500.0F;

		// too close so move back
		if(TargetShipDistance < DesiredDistance && DistWall[9] > 200.0F)
			BOTAI_SetAction( &bot.forward, -1.0F, "MaintainDistanceToTargetShip() reverse" );
		// too far so move forward
		else if (TargetShipDistance > DesiredDistance && DistWall[8] > 200.0F)
			BOTAI_SetAction( &bot.forward, 1.0F, "MaintainDistanceToTargetShip() forward" );

		// move right towards the middle
		if( DistWall[0] > DistWall[1] && DistWall[0] > 100.0F )
			BOTAI_SetAction( &bot.right, 1.0F, "MaintainDistanceToTargetShip() right" );
		// move left towards the middle
		else if( DistWall[1] > 100.0F )
			BOTAI_SetAction( &bot.right, -1.0F, "MaintainDistanceToTargetShip() left" );

		// move up towards the middle
		if( DistWall[2] > DistWall[3] && DistWall[2] > 100.0F )
			BOTAI_SetAction( &bot.up, 1.0F, "MaintainDistanceToTargetShip() up" );
		// move down towards the middle
		else if( DistWall[3] > 100.0F )
			BOTAI_SetAction( &bot.up, -1.0F, "MaintainDistanceToTargetShip() down" );

		return true;
	}
	else
		return false;
}
 

#endif
