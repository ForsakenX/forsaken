#ifdef LUA_BOT
#include "botai_sensors.h"
 
VECTOR FollowTargetPos;
VECTOR MissileOrigDirVector;
bool AllSensorsClear = true;
bool DEBUG_AVOIDANCE = true;
bool FriendlyFire = false;
bool MissileAvoidanceSet = false;
bool ReverseNetwork = true;
bool initialised = false;
float DistWall[NUM_SENSORS];
float IFiredTitan = -1.0F;
float PrevHull = MAX_HULL+1.0F;
float PrevShield = MAX_SHIELD+1.0F;
float TargetShipDistance;
float WhenWillHitSlide[NUM_SENSORS];
float accuracy = 0.0F;
int AvoidMove = -1;
int CurrentNode = -1;
int FollowTargetGroup;
int GettingPickup = -1;
int HomingMissile = -1;
int PrevDeaths = -1;
int PrevHomingMissile = -1;
int PrevKills = -1;
int TargetMine = -1;
int TargetNode = -1;
int TargetShipHealth = -1;
int TargetShipID = -1;
 
char	*	WepMessages[] = {

	LT_PICKUP_TROJAX				,//"Trojax",						// Primary Weapons
	LT_PICKUP_PYROLITE				,//"Pyrolite Rifle",
	LT_PICKUP_TRANSPULSE			,//"Transpulse Cannon",
	LT_PICKUP_SUSSGUN				,//"Suss-Gun",
	LT_PICKUP_LASER					,//"Beam Laser",
	LT_PICKUP_MUG					,//"Mug Missile",					// Secondary Weapons
	LT_PICKUP_MUG_PACK				,//"Mug Missiles",
	LT_PICKUP_SOLARIS				,//"Heatseeker Missile",
	LT_PICKUP_SOLARIS_PACK			,//"Heatseeker Missile Pack",
	LT_PICKUP_THIEF					,//"Thief Missile",
	LT_PICKUP_SCATTER				,//"Scatter Missile",
	LT_PICKUP_GRAVGON				,//"Gravgon Missile",
	LT_PICKUP_MFRL					,//"Rocket Launcher",
	LT_PICKUP_TITAN					,//"TitanStar Missile",
	LT_PICKUP_PURGE					,//"Purge Mine Pack",
	LT_PICKUP_PINE					,//"Pine Mine Pack",
	LT_PICKUP_QUANTUM				,//"Quantum Mine Pack",
	LT_PICKUP_SPIDER				,//"Spider Mine Pack",
	LT_PICKUP_PARASITE				,//"Parasite Mine",
	LT_PICKUP_FLARE					,//"Flare",
	LT_PICKUP_GENAMMO				,//"General Ammo",					// Ammo
	LT_PICKUP_PYROFUEL				,//"Pyrolite Fuel",
	LT_PICKUP_SUSSAMMO				,//"Suss-Gun Ammo",
	LT_PICKUP_POWER_POD				,//"Power Pod",					// Extras
	LT_PICKUP_SHIELD				,//"Shield",
	LT_PICKUP_INVULBERABILITY		,//"Invulnerability",
	LT_PICKUP_EXTRA_LIFE			,//"Extra Life",
	LT_PICKUP_TARGETING_COMPUTER	,//"Targeting Computer",
	LT_PICKUP_SMOKE					,//"Smoke Streamer",
	LT_PICKUP_NITRO					,//"Nitro",
	LT_PICKUP_GOGGLES				,//"Goggles",
	LT_PICKUP_GOLD_BARS				,//"Gold Bars",
	LT_PICKUP_STEALTH				,//"Stealth Mantle",
	LT_PICKUP_CRYSTAL				,//"Crystal",
	LT_PICKUP_ORBITAL				,//"Orbit Pulsar",
	LT_PICKUP_GOLDEN_POWER_POD		,//"Golden Power Pod",
	LT_PICKUP_DNA					,//"DNA",
	"",														// skeleton key
	LT_PICKUP_BOMB					,//"Bomb",
	LT_PICKUP_GOLDEN_IDOL			,//"Golden Idol",
	"",								// flag
	"",
	"",								// flag 1
	"",								// flag 2
	"",								// flag 3
	"",								// flag 4
};
 
void BOTAI_UpdateSensors()
{
	int CurrentDeaths = GetTotalDeaths(WhoIAm);
	int CurrentKills = GetTotalKills(WhoIAm);
	int i;

	// i died or killed someone
	if( CurrentDeaths > PrevDeaths || CurrentKills > PrevKills )
	{
		// mix up the route
		if(Random_Range(2) > 0)
			ReverseNetwork = true;
		else
			ReverseNetwork = false;

		// i died, reset flags 
		if( CurrentDeaths > PrevDeaths )
		{
			//DebugPrintf("i died\n");
			CurrentNode = -1;
			GettingPickup = -1;
			IFiredTitan = -1.0F;
			TargetMine = -1;
			HomingMissile = -1;
			PrevHomingMissile = -1;
			MissileAvoidanceSet = false;
			PrevHull = MAX_HULL+1.0F;
			PrevShield = MAX_SHIELD+1.0F;
		}

		PrevDeaths = CurrentDeaths;
		PrevKills = CurrentKills;
	}

	// i got hit
	if( Ships[WhoIAm].Object.Hull < PrevHull || Ships[WhoIAm].Object.Shield < PrevShield )
	{
		PrevHull = Ships[WhoIAm].Object.Hull;
		PrevShield = Ships[WhoIAm].Object.Shield;

		// debugging stuff
		if(!initialised)
			initialised = true;
		else
			DEBUG_AVOIDANCE = false;
	}

	// -- update sensors
	// up, down, left, right, forward, backward
	DistWall[0] = BOTAI_DistanceToWall(false,false,false,true,false,false); // right
	DistWall[1] = BOTAI_DistanceToWall(false,false,true,false,false,false); // left
	DistWall[2] = BOTAI_DistanceToWall(true,false,false,false,false,false); // up 
	DistWall[3] = BOTAI_DistanceToWall(false,true,false,false,false,false); // down
	DistWall[4] = BOTAI_DistanceToWall(true,false,false,true,false,false); // up right
	DistWall[5] = BOTAI_DistanceToWall(true,false,true,false,false,false); // up left
	DistWall[6] = BOTAI_DistanceToWall(false,true,false,true,false,false); // down right
	DistWall[7] = BOTAI_DistanceToWall(false,true,true,false,false,false); // down left
	DistWall[8] = BOTAI_DistanceToWall(false,false,false,false,true,false); // forward
	DistWall[9] = BOTAI_DistanceToWall(false,false,false,false,false,true); // backward
	DistWall[10] = BOTAI_DistanceToWall(false,false,false,true,false,true); // right backward
	DistWall[11] = BOTAI_DistanceToWall(false,false,true,false,false,true); // left backward
	DistWall[12] = BOTAI_DistanceToWall(true,false,false,false,false,true); // up backward
	DistWall[13] = BOTAI_DistanceToWall(false,true,false,false,false,true); // down backward
	DistWall[14] = BOTAI_DistanceToWall(true,false,false,true,false,true); // up right backward
	DistWall[15] = BOTAI_DistanceToWall(true,false,true,false,false,true); // up left backward
	DistWall[16] = BOTAI_DistanceToWall(false,true,false,true,false,true); // down right backward
	DistWall[17] = BOTAI_DistanceToWall(false,true,true,false,false,true); // down left backward
	DistWall[18] = BOTAI_DistanceToWall(false,false,false,true,true,false); // right forward
	DistWall[19] = BOTAI_DistanceToWall(false,false,true,false,true,false); // left forward
	DistWall[20] = BOTAI_DistanceToWall(true,false,false,false,true,false); // up forward
	DistWall[21] = BOTAI_DistanceToWall(false,true,false,false,true,false); // down forward
	DistWall[22] = BOTAI_DistanceToWall(true,false,false,true,true,false); // up right forward
	DistWall[23] = BOTAI_DistanceToWall(true,false,true,false,true,false); // up left forward
	DistWall[24] = BOTAI_DistanceToWall(false,true,false,true,true,false); // down right forward
	DistWall[25] = BOTAI_DistanceToWall(false,true,true,false,true,false); // down left forward

	AllSensorsClear = true;
	WhenWillHitSlide[0] = BOTAI_WhenWillBulletHitSlide(false,false,false,true,false,false); // r
	WhenWillHitSlide[1] = BOTAI_WhenWillBulletHitSlide(false,false,true,false,false,false); // l
	WhenWillHitSlide[2] = BOTAI_WhenWillBulletHitSlide(true,false,false,false,false,false); // u
	WhenWillHitSlide[3] = BOTAI_WhenWillBulletHitSlide(false,true,false,false,false,false); // d
	WhenWillHitSlide[4] = BOTAI_WhenWillBulletHitSlide(true,false,false,true,false,false); // u r
	WhenWillHitSlide[5] = BOTAI_WhenWillBulletHitSlide(true,false,true,false,false,false); // u l
	WhenWillHitSlide[6] = BOTAI_WhenWillBulletHitSlide(false,true,false,true,false,false); // d r
	WhenWillHitSlide[7] = BOTAI_WhenWillBulletHitSlide(false,true,true,false,false,false); // d l
	WhenWillHitSlide[8] = BOTAI_WhenWillBulletHitSlide(false,false,false,false,true,false); // f
	WhenWillHitSlide[9] = BOTAI_WhenWillBulletHitSlide(false,false,false,false,false,true); // b
	WhenWillHitSlide[10] = BOTAI_WhenWillBulletHitSlide(false,false,false,true,false,true); // r b
	WhenWillHitSlide[11] = BOTAI_WhenWillBulletHitSlide(false,false,true,false,false,true); // l b
	WhenWillHitSlide[12] = BOTAI_WhenWillBulletHitSlide(true,false,false,false,false,true); // u b
	WhenWillHitSlide[13] = BOTAI_WhenWillBulletHitSlide(false,true,false,false,false,true); // d b
	WhenWillHitSlide[14] = BOTAI_WhenWillBulletHitSlide(true,false,false,true,false,true); // u r b
	WhenWillHitSlide[15] = BOTAI_WhenWillBulletHitSlide(true,false,true,false,false,true); // u l b
	WhenWillHitSlide[16] = BOTAI_WhenWillBulletHitSlide(false,true,false,true,false,true); // d r b
	WhenWillHitSlide[17] = BOTAI_WhenWillBulletHitSlide(false,true,true,false,false,true); // d l b
	WhenWillHitSlide[18] = BOTAI_WhenWillBulletHitSlide(false,false,false,true,true,false); // r f
	WhenWillHitSlide[19] = BOTAI_WhenWillBulletHitSlide(false,false,true,false,true,false); // l f
	WhenWillHitSlide[20] = BOTAI_WhenWillBulletHitSlide(true,false,false,false,true,false); // u f
	WhenWillHitSlide[21] = BOTAI_WhenWillBulletHitSlide(false,true,false,false,true,false); // d f
	WhenWillHitSlide[22] = BOTAI_WhenWillBulletHitSlide(true,false,false,true,true,false); // u r f
	WhenWillHitSlide[23] = BOTAI_WhenWillBulletHitSlide(true,false,true,false,true,false); // u l f
	WhenWillHitSlide[24] = BOTAI_WhenWillBulletHitSlide(false,true,false,true,true,false); // d r f
	WhenWillHitSlide[25] = BOTAI_WhenWillBulletHitSlide(false,true,true,false,true,false); // d l f

	// time since i last fired titan
	// used so i don't move into the blast
	IFiredTitan -= framelag;

	BOTAI_GetNearestPickup();
	FriendlyFire = BOTAI_FriendlyFireCheck();
	BOTAI_GetSHIPTarget();

	//DebugPrintf("r: %.1f l: %.1f u: %.1f d: %.1f f: %.1f b: %.1f\n", DistWall[0], DistWall[1], DistWall[2], DistWall[3], DistWall[8], DistWall[9]);
	//DebugPrintf("ur: %.1f ul: %.1f dr: %.1f dl: %.1f\n", DistWall[22], DistWall[23], DistWall[24], DistWall[25]);
	//DebugPrintf("%f %f %f\n", Ships[WhoIAm].Object.Pos.x, Ships[WhoIAm].Object.Pos.y, Ships[WhoIAm].Object.Pos.z); 
	//DebugPrintf("Level: %s\n", ShortLevelNames[LevelNum]);
	//DebugPrintf("nearest node = %d\n", BOTAI_GetNearestNode(&Ships[WhoIAm].Object));

	// if there is a target
	if(TargetShipID > - 1)
	{
		// distance to target
		TargetShipDistance = DistanceVector2Vector(&Ships[WhoIAm].Object.Pos, &Ships[TargetShipID].Object.Pos);

		// health of target
		TargetShipHealth = PlayerHealths[TargetShipID].Shield + PlayerHealths[TargetShipID].Hull;
	}
}
 
bool BOTAI_WillHomingMissileHit(VECTOR * MyPos)
{
	float Cos;
	float Angle;
	VECTOR DirVector;
	VECTOR TmpVec;
	QUATLERP qlerp;
	SECONDARYWEAPONBULLET MissCopy = SecBulls[ HomingMissile ];

	// direction vector from missile to me
	DirVector.x = MyPos->x - MissCopy.Pos.x;
	DirVector.y = MyPos->y - MissCopy.Pos.y;
	DirVector.z = MyPos->z - MissCopy.Pos.z;
	NormaliseVector( &DirVector );

	// angle difference between the missile's current vector and wanted vector
	Cos = DotProduct( &DirVector, &MissCopy.DirVector );

	// set the parameters to perform a linear interpolation on two quaternions
	QuatFrom2Vectors( &qlerp.end, &Forward, &DirVector );
	qlerp.start	= MissCopy.DirQuat;				
	qlerp.crnt	= &MissCopy.DirQuat;	
	qlerp.dir	= QuatDotProduct( &qlerp.start, &qlerp.end );

	// bound angle difference
	if( Cos < -1.0F ) Cos = -1.0F;
	else if ( Cos > 1.0F ) Cos = 1.0F;

	// get angle difference in radians
	Angle = (float) acos( Cos );

	// calculate the amount of angle to turn 
	if( Angle ) qlerp.time = ( ( D2R( MissCopy.TurnSpeed ) * framelag ) / Angle );
	else qlerp.time = 1.0F;
	if( qlerp.time > 1.0F ) qlerp.time = 1.0F;

	// perform quat interpolation
	QuatInterpolate( &qlerp );
	QuatToMatrix( &MissCopy.DirQuat, &MissCopy.Mat );
	ApplyMatrix( &MissCopy.Mat, &Forward, &MissCopy.DirVector );
	ApplyMatrix( &MissCopy.Mat, &SlideUp, &MissCopy.UpVector );

	// will missile hit?
	if(RaytoSphere2(MyPos, SHIP_RADIUS, &MissCopy.Pos, &MissCopy.DirVector, &TmpVec, &TmpVec ))
	{
		DebugPrintf("homing missile will hit\n");
		return true;
	}
	else
	{
		DebugPrintf("safe from homing missile\n");
		return false;
	}
}
 
bool BOTAI_FriendlyFireCheck()
{
	int i;
	VECTOR Move_Dir;
	VECTOR TempVector;

	// no need to check if not a team game
	if(!TeamGame)
		return false;

	// calculate my direction vector
	ApplyMatrix( &Ships[WhoIAm].Object.Mat, &Forward, &Move_Dir );
	NormaliseVector( &Move_Dir );

	// for each player
	for(i = 0; i < MAX_PLAYERS; i++)	
	{
		// that is enabled
		if( i != WhoIAm && Ships[i].enable && Ships[i].Object.Mode == NORMAL_MODE )
		{
			// and on my team
			if(TeamNumber[WhoIAm] != TeamNumber[i])
				continue;

			// are we within visible params?
			if( RaytoSphere2(&Ships[i].Object.Pos, SHIP_RADIUS, &Ships[WhoIAm].Object.Pos, &Move_Dir , &TempVector , &TempVector ) )
				return true;
		}
	}

	return false;
}
 
bool BOTAI_CanICollectPickup( u_int16_t i )
{
	int16_t	PickupEnable = false;

	if( i != (u_int16_t) -1 )
	{
		switch( Pickups[i].Type )
		{
			case PICKUP_Trojax: 
				if( !PrimaryWeaponsGot[ TROJAX ] )
					PickupEnable = true;
				break;

			case PICKUP_Pyrolite:
				if( !PrimaryWeaponsGot[ PYROLITE_RIFLE ] )
					PickupEnable = true;
				break;

			case PICKUP_Transpulse:
				if( !PrimaryWeaponsGot[ TRANSPULSE_CANNON ] )
					PickupEnable = true;
				break;

			case PICKUP_SussGun:
				if( !PrimaryWeaponsGot[ SUSS_GUN ] )
					PickupEnable = true;
				break;

			case PICKUP_Laser:
				if( !PrimaryWeaponsGot[ LASER ] )
					PickupEnable = true;
				break;

			case PICKUP_Mugs:
				if( SecondaryAmmo[ MUGMISSILE ] < 10 )
					PickupEnable = true;
				break;

			case PICKUP_HeatseakerPickup:
				if( SecondaryAmmo[ SOLARISMISSILE ] < 10 )
					PickupEnable = true;
				break;

			case PICKUP_Thief:
				if( !SecondaryAmmo[ THIEFMISSILE ] )
					PickupEnable = true;
				break;

			case PICKUP_Scatter:
				if( SecondaryAmmo[ SCATTERMISSILE ] < 3 )
					PickupEnable = true;
				break;

			case PICKUP_Gravgon:
				if( !SecondaryAmmo[ GRAVGONMISSILE ] )
					PickupEnable = true;
				break;

			case PICKUP_Launcher:
				if( SecondaryAmmo[ MULTIPLEMISSILE ] < 100 )
					PickupEnable = true;
				break;

			case PICKUP_TitanStar:
				if( SecondaryAmmo[ TITANSTARMISSILE ] < 3 )
					PickupEnable = true;
				break;

			case PICKUP_PurgePickup:
				if( SecondaryAmmo[ PURGEMINE ] < 10 )
					PickupEnable = true;
				break;

			case PICKUP_PinePickup:
				if( SecondaryAmmo[ PINEMINE ] < 6 )
					PickupEnable = true;
				break;

			case PICKUP_QuantumPickup:
				if( !SecondaryAmmo[ QUANTUMMINE ] )
					PickupEnable = true;
				break;

			case PICKUP_SpiderPod:
				if( SecondaryAmmo[ SPIDERMINE ] < 6 )
					PickupEnable = true;
				break;

			case PICKUP_GeneralAmmo:
				if( GeneralAmmo <  1000.0F ) // MAXGENERALAMMO )
					PickupEnable = true;
				break;

			case PICKUP_PyroliteAmmo:
				if( PrimaryWeaponsGot[ PYROLITE_RIFLE ] && PyroliteAmmo < 1000.0F ) // MAXPYROLITEAMMO )
					PickupEnable = true;
				break;

			case PICKUP_SussGunAmmo:
				if( PrimaryWeaponsGot[ SUSS_GUN ] && SussGunAmmo < 1000.0F ) //MAXSUSSGUNAMMO )
					PickupEnable = true;
				break;

			case PICKUP_PowerPod:
				if( Ships[ WhoIAm].Object.PowerLevel < 2 )
					PickupEnable = true;
				break;

			case PICKUP_GoldenPowerPod:
				if( Ships[ WhoIAm ].SuperNashramTimer == 0.0F )
					PickupEnable = true;
				break;

			case PICKUP_Inv:
				if( Ships[ WhoIAm ].InvulTimer == 0.0F )
					PickupEnable = true;
				break;

			case PICKUP_Nitro:
				if( NitroFuel < MAX_NITRO_FUEL )
					PickupEnable = true;
				break;

			case PICKUP_Mantle:
				if( Ships[ WhoIAm ].StealthTime == 0.0F )
					PickupEnable = true;
				break;

			case PICKUP_Shield:
				if( Ships[ WhoIAm ].Object.Shield < MAX_SHIELD )
					PickupEnable = true;
				break;

			case PICKUP_Flag:
			case PICKUP_Bounty:
				PickupEnable = true;
				break;

			case PICKUP_Orb:
				if( NumOrbs < MAXMULTIPLES )
					PickupEnable = true;
				break;		 
		}

		if( PickupEnable )
			return true;
		else
			return false;
	}
	else
		return false;
}
 
bool BOTAI_SafeToMove()
{
	if(AllSensorsClear && AvoidMove < 0 && HomingMissile < 0 && IFiredTitan < 0.0F)
		return true;
	else
	{
		if(!AllSensorsClear)
			DebugPrintf("not all clear\n");
		DebugPrintf("avoid = %d\n", AvoidMove);
		return false;
	}
}
 
// rewrite to account for the ship inventory as it collects weapons
int BOTAI_ShipStrength( int i )
{
	int Strength = 0;
	int p;
	float dist;

	if( TargetShipID < 0 )
		return 0;

	if( Ships[i].InvulTimer > 0.0F )
		return 1000000;

	if( Ships[i].Object.Flags & SHIP_Scattered_Bit )
		return 10;

	dist = DistanceVector2Vector( &Ships[WhoIAm].Object.Pos, &Ships[TargetShipID].Object.Pos );

	p = Ships[i].Object.PowerLevel + 1;

	switch( Ships[i].Primary )
	{
		case TROJAX: 
			if( dist > 1500 ) Strength += 70*p;
			else Strength += 90*p;
			break;

		case PYROLITE_RIFLE:
			if( dist < 1000 ) Strength += 100*p;
			else Strength += 10*p;
			break;

		case TRANSPULSE_CANNON:
			if( dist > 1000 ) Strength += 70*p;
			else Strength += 40*p;
			break;

		case SUSS_GUN:
			if( dist > 1000 ) Strength += 10*p;
			else Strength += 40*p;
			break;

		case LASER:
			if( dist > 1500 ) Strength += 80*p;
			else Strength += 60*p;
			break;

		case PULSAR:
			if( dist > 1500 ) Strength += 60*p;
			else Strength += 50*p;
			break;
	}

	if( Ships[i].SuperNashramTimer == 0.0F )
		Strength *= 3;
	else if( Ships[i].Object.Flags & SHIP_Turbo_Bit )
		Strength *= 2;
  
	// secondary code currently useless; only triggered AFTER enemy player has fired the missile
  /*  switch( Ships[i].Secondary )
	{
		case MUGMISSILE:
		case THIEFMISSILE:
			if( dist > 1000 ) Strength += 30;
			else Strength += 60;
			break;

		case SOLARISMISSILE:
		case MULTIPLEMISSILE:
			if( dist > 1500 ) Strength += 150;
			else Strength += 210;
			break;

		case SCATTERMISSILE:
			if( dist > 1500 ) Strength += 180;
			else Strength += 240;
			break;

		case GRAVGONMISSILE:
			Strength += 30;
			break;

		case TITANSTARMISSILE:
			if( dist > 2000 ) Strength += 120;
			else Strength += 300;
			break;
	} */
	if( i == WhoIAm )
		Strength = (int) (Strength * (Ships[WhoIAm].Object.Hull+Ships[WhoIAm].Object.Shield) / (MAX_HULL+MAX_SHIELD));
	else
		Strength = (int) (Strength * (PlayerHealths[i].Hull+PlayerHealths[i].Shield) / (MAX_HULL+MAX_SHIELD));
	return Strength;
}
 
int BOTAI_ComparativeEnemyStrength()
{
	int MyStrength = 0;
	int EnemyStrength = 0;

	if( TargetShipID < 0 )
		return 0;
	else
	{
		MyStrength = BOTAI_ShipStrength(WhoIAm);
		EnemyStrength =  BOTAI_ShipStrength(TargetShipID);
		DebugPrintf("my strength = %d enemy strength = %d\n", MyStrength, EnemyStrength);
		return MyStrength - EnemyStrength;
	}
}
 
float BOTAI_DistanceToWall( bool u, bool d, bool l, bool r, bool f, bool b )
{
	float dist;
	VECTOR temp;
	NORMAL FaceNormal;
	VECTOR Pos_New;
	VECTOR ImpactPoint;
	u_int16_t ImpactGroup;
	VECTOR Slide;

	// calculate the desired vector 
	Slide.x = 0.0F;
	Slide.y = 0.0F;
	Slide.z = 0.0F;

	if(r) Slide.x = 1.0F;	
	else if(l) Slide.x = -1.0F;		
	if(u) Slide.y = 1.0F;	
	else if(d) Slide.y = -1.0F;	
	if(f) Slide.z = 1.0F;	
	else if(b) Slide.z = -1.0F;

	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Slide, &temp );

	temp.x *= MaxColDistance;
	temp.y *= MaxColDistance;
	temp.z *= MaxColDistance;

	// get collision details	
	BackgroundCollide( &MCloadheadert0, &Mloadheader, &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &temp, &ImpactPoint, &ImpactGroup, &FaceNormal, &Pos_New, false, NULL );

	// return the distance
	dist = DistanceVector2Vector(&Ships[WhoIAm].Object.Pos, &ImpactPoint);

	// if movement will put me in a gravgon we want to avoid this
	Pos_New = Ships[WhoIAm].Object.Pos;
	BOTAI_LookAheadPos( 10.0F, &Pos_New, Slide );
	if( BOTAI_CheckForGravgons( &Pos_New ) ) 
		dist = 1.0F;

	return dist;
}
 
bool BOTAI_CheckForGravgons( VECTOR * Pos )
{
	u_int16_t i;
	float DistToGravgon;

	for( i = FirstModelUsed; i != (u_int16_t) -1; i = Models[i].Prev )
	{
		if( Models[i].Func == MODFUNC_Scale )
		{
			DistToGravgon = DistanceVector2Vector( Pos, &Models[i].Pos );

			if( DistToGravgon < ( BALL_RADIUS * Models[i].MaxScale ) + ( SHIP_RADIUS * 8.0F ) )
				return true;
		}
	}
	return false;
}
 
void BOTAI_GetNearestPickup()
{
	int i;
	float dist;
	float nearestDist=BIGDISTANCE;

	GettingPickup = -1;

	// for all pickups
	for(i=0; i<MAXPICKUPS; i++)
	{
		// that are enabled
		if(Pickups[i].Type == (u_int16_t) -1)
			continue;

		// that i can see
		if( BOTAI_ClearLOS( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group, &Pickups[i].Pos ))
		{
			// and that i can collect
			if( BOTAI_CanICollectPickup(i) )
			{
				dist = DistanceVector2Vector(&Ships[WhoIAm].Object.Pos, &Pickups[i].Pos);
				// find the nearest
				if(dist < nearestDist)
				{
					nearestDist = dist;
					GettingPickup = i;
				}
			}
		}
	}
}
 
bool BOTAI_ClearLOS( VECTOR * SPos, u_int16_t Group , VECTOR * Pos )
{
	VECTOR Dir;
	VECTOR Int_Point;
	u_int16_t Int_Group;
	NORMAL Int_Normal;
	VECTOR TempVector;

	Dir.x = Pos->x - SPos->x;
	Dir.y = Pos->y - SPos->y;
	Dir.z = Pos->z - SPos->z;

	return !BackgroundCollide( &MCloadheadert0, &Mloadheader, SPos, Group, &Dir, &Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL );
}

bool BOTAI_ClearLOSNonZero( OBJECT * SObject, VECTOR * Pos , float radius )
{
	VECTOR Dir;
	BGOBJECT * BGObject;

	Dir.x = Pos->x - SObject->Pos.x;
	Dir.y = Pos->y - SObject->Pos.y;
	Dir.z = Pos->z - SObject->Pos.z;

	return !WouldObjectCollide( SObject, &Dir, radius, &BGObject );
}
 
void BOTAI_GetSHIPTarget()
{
	float dist;
	float nearest = BIGDISTANCE;
	int	i;

	TargetShipID = -1;

	// for each player
	for( i = 0; i < MAX_PLAYERS; i++ )
	{
		// that's not me, and must be alive
		if( i != WhoIAm && Ships[i].enable && (Ships[i].Object.Mode == NORMAL_MODE) 
		  ) // && ( !(Ships[i].Object.Flags & SHIP_Stealth) || (Ships[i].Object.Flags & SHIP_Litup) ) )
		  {
			  // and not on my team
			  if(TeamGame && TeamNumber[WhoIAm] == TeamNumber[i])
				  continue;

			  // and that i can hear
			  if( !SoundInfo[Ships[WhoIAm].Object.Group][Ships[i].Object.Group] )
			  {
				  // find the closest
				  dist = DistanceVector2Vector( &Ships[i].Object.Pos, &Ships[WhoIAm].Object.Pos );
				  if( Ships[i].Object.Noise != 0.0F )
					  dist *= ( 1.0F * ( 0.2F * Ships[i].Object.Noise ) );

				  if( dist < nearest )
				  {
					  TargetShipID = i;
					  nearest = dist;         
				  }
			  }
		  }
	}
}

bool BOTAI_InViewCone( VECTOR * Pos, MATRIX * Mat, VECTOR * TPos, float ViewConeCos )
{
	float Cos;
	VECTOR NormVector;
	VECTOR Dir;

	if( ViewConeCos == 1.0F )
		return true;

	Dir.x = TPos->x - Pos->x;
	Dir.y = TPos->y - Pos->y;
	Dir.z = TPos->z - Pos->z;
	NormVector = Dir;
	NormaliseVector( &NormVector );
	ApplyMatrix( Mat, &Forward, &Dir );

	Cos = DotProduct( &NormVector, &Dir );
	if( Cos > ViewConeCos )
		return true;

	return false;
}
 
float BOTAI_WhenWillBulletHitMe(VECTOR * MyPos)
{
	int i;
	float time;
	float shortestTime = BIGDISTANCE;
	float dist;
	VECTOR TempVect;
	VECTOR temp;
	VECTOR TempVector;
	float ShipRadius;
	float Cos;

	// primary weapon bullets
	for(i = 0; i < MAXPRIMARYWEAPONBULLETS; i++)
	{
		// that are active and aren't my own
		if(PrimBulls[i].Used && PrimBulls[i].Owner != WhoIAm)
		{
			// set the collision radius
			switch( PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].ColType )
			{
				case COLTYPE_Transpulse:
					TempVector.x = Ships[ WhoIAm ].Object.Pos.x - PrimBulls[i].Pos.x;
					TempVector.y = Ships[ WhoIAm ].Object.Pos.y - PrimBulls[i].Pos.y;
					TempVector.z = Ships[ WhoIAm ].Object.Pos.z - PrimBulls[i].Pos.z;
					NormaliseVector( &TempVector );
					Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, &PrimBulls[i].Dir ) ) );
					Cos = (float) ( Cos * ( 1.0F - fabs( DotProduct( &TempVector, &PrimBulls[i].UpVector ) ) ) );
					ShipRadius = SHIP_RADIUS + ( PrimBulls[i].ColRadius * Cos );
					break;

				case COLTYPE_Sphere:
				case COLTYPE_Trojax:
					ShipRadius = SHIP_RADIUS + PrimBulls[i].ColRadius;
					break;

				case COLTYPE_Point:
				default:
					ShipRadius = SHIP_RADIUS;
					break;
			}

			// bullet will (currently) eventually collide with this position
			if(RaytoSphere2( MyPos, ShipRadius, &PrimBulls[i].Pos, &PrimBulls[i].Dir, &TempVect, &TempVect ))
			{
				dist = DistanceVector2Vector(MyPos, &PrimBulls[i].Pos);
				time = dist/PrimBulls[i].Speed;
				if(time < shortestTime)
					shortestTime = time;
			}
		}
	}

	// reset homing missile flag when it's gone
	if(HomingMissile > -1)
	{
		if(!SecBulls[HomingMissile].Used)
			MissileAvoidanceSet = false;
	}

	// missiles
	HomingMissile = -1;
	for(i = 0; i< MAXSECONDARYWEAPONBULLETS; i++)
	{
		// ignore inactive missiles or my own
		if(!SecBulls[i].Used || SecBulls[i].Owner == WhoIAm)
			continue;

		// get the distance from me and time to impact
		dist = DistanceVector2Vector(MyPos, &SecBulls[i].Pos);
		time = dist/SecBulls[i].Speed;

		// straight missiles (treat like normal bullets)
		if(SecBulls[i].MoveType == MISMOVE_STRAIGHT )
		{
			// if it will hit me 
			if(RaytoSphere2(MyPos, SHIP_RADIUS, &SecBulls[i].Pos, &SecBulls[i].DirVector, &TempVect, &TempVect))
			{
				if(time < shortestTime)
					shortestTime = time; // find the closest
			}
		}
		// if a homing missile has locked on to me
		else if(SecBulls[i].MoveType == MISMOVE_HOMING && SecBulls[i].Target == WhoIAm)
		{
			// and it's close enough i have to deal with it
			if(dist < 2048.0F * GLOBAL_SCALE)
			{
				HomingMissile = i;
				shortestTime = time; // new
			}
		}
	}

	if(shortestTime < 10000.0F)
		return shortestTime;
	else
		return -1.0F; // nothing will hit
}

float BOTAI_WhenWillBulletHitSlide( bool u, bool d, bool l, bool r, bool f, bool b )
{
	VECTOR NewPos = Ships[WhoIAm].Object.Pos;
	float BulletHitTime;
	float TimeAhead = 11.0F;

	// calculate the new position
	if(u) BOTAI_LookAheadPos( TimeAhead, &NewPos, SlideUp );
	else if(d) BOTAI_LookAheadPos( TimeAhead, &NewPos, SlideDown );
	if(l) BOTAI_LookAheadPos( TimeAhead, &NewPos, SlideLeft );
	else if(r) BOTAI_LookAheadPos( TimeAhead, &NewPos, SlideRight );
	if(f) BOTAI_LookAheadPos( TimeAhead, &NewPos, Forward );
	else if(b) BOTAI_LookAheadPos( TimeAhead, &NewPos, Backward );

	// get time nearest bullet will hit new position
	BulletHitTime = BOTAI_WhenWillBulletHitMe(&NewPos);

	if(BulletHitTime > 0.0F)
		AllSensorsClear = false;

	return BulletHitTime;
}
 
void BOTAI_LookAheadPos( float Time, VECTOR * NewPos, VECTOR SlideDirection )
{
	VECTOR Move_Off;

	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideDirection, &Move_Off );
	NewPos->x += Move_Off.x * MaxMoveSpeed * Time;
	NewPos->y += Move_Off.y * MaxMoveSpeed * Time;
	NewPos->z += Move_Off.z * MaxMoveSpeed * Time;
}

void BOTAI_LookAhead( float Accuracy, VECTOR * SPos, BYTE TargetID, VECTOR * NewPos, float SpeedOfBullet )
{
	float Time;
	float Distance;

	Distance = DistanceVector2Vector( &Ships[TargetID].Object.Pos, SPos );
	Time = ( Distance / SpeedOfBullet );

	// only predict forward if they have nitro
	if(Ships[TargetID].Object.Flags & SHIP_Turbo)
		Accuracy = fabs(Accuracy);

	NewPos->x = Ships[TargetID].Object.Pos.x + ( Ships[TargetID].Move_Off.x * Time * Accuracy );
	NewPos->y = Ships[TargetID].Object.Pos.y + ( Ships[TargetID].Move_Off.y * Time * Accuracy );
	NewPos->z = Ships[TargetID].Object.Pos.z + ( Ships[TargetID].Move_Off.z * Time * Accuracy );
}
 
int BOTAI_CenterCheck( int move )
{
	int a = 0;
	int b = 0;

	switch(move)
	{
		case 0: case 1: a = 0; b = 1; break;
		case 2: case 3: a = 2; b = 3; break;
		case 4: case 7: a = 4; b = 7; break;
		case 5: case 6: a = 5; b = 6; break;
		case 8: case 9: a = 8; b = 9; break;
		case 10: case 19: a = 10; b = 19; break;
		case 11: case 18: a = 11; b = 18; break;
		case 12: case 21: a = 12; b = 21; break;
		case 13: case 20: a = 13; b = 20; break;
		case 14: case 25: a = 14; b = 25; break;
		case 15: case 24: a = 15; b = 24; break;
		case 16: case 23: a = 16; b = 23; break;
		case 17: case 22: a = 17; b = 22; break;
	}

	if( DistWall[a] < DistWall[b] + 100.0F && DistWall[a] > DistWall[b] - 100.0F )
	{
		if( WhenWillHitSlide[a] < 0.0F && WhenWillHitSlide[b] < 0.0F )	
			return a;
	}

	return move;
}
 
#endif
