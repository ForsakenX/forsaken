/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 107 $
 *
 * $Header: /PcProjectX/Include/Primary.h 107   14/09/98 12:42 Phillipd $
 *
 * $Log: /PcProjectX/Include/Primary.h $
 * 
 * 107   14/09/98 12:42 Phillipd
 * 
 * 106   26/06/98 10:55 Collinsd
 * Server Pickups and weapons works pretty well, death mode pickups thrown
 * out, as well as scattered.  Quitting/Crash players throw out all
 * pickups.
 * 
 * 105   22/05/98 15:06 Collinsd
 * Server Pickup work
 * 
 * 104   5/04/98 19:49 Collinsd
 * Added GlobalFramelagAddition.
 * 
 * 103   20/03/98 17:23 Collinsd
 * Hopefully fixed crashbug on reactor on nps-sp01
 * 
 * 102   12/03/98 21:52 Collinsd
 * Added cheatmodes for primary and seconadry weapons.
 * 
 * 101   27/02/98 18:37 Collinsd
 * Primary next/prev sfx added
 * 
 * 100   26/02/98 15:27 Collinsd
 * Altered minimum ammo
 * 
 * 99    26/02/98 12:59 Collinsd
 * Ammo thrown out when player low on ammo.
 * 
 * 98    16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 97    14/02/98 10:50 Collinsd
 * Fixed Bullets hitting ships/mines not in same group ( Pandora's box )
 * 
 * 96    10/02/98 11:40 Collinsd
 * Added code for descent/forsaken collision
 * 
 * 95    30/01/98 17:13 Collinsd
 * 
 * 94    30/01/98 10:58 Collinsd
 * Added Correction code for laser direction
 * 
 * 93    30/01/98 9:13 Collinsd
 * Added Laser for exogenon and added corruption checking for componented
 * objects.
 * 
 * 92    1/16/98 3:59p Phillipd
 * 
 * 91    30/12/97 10:45 Collinsd
 * Added enemy damage modifier as variable.
 * 
 * 90    29/12/97 20:19 Collinsd
 * Added enemy damage modifier.
 * 
 * 89    5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 88    26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 87    24/11/97 20:04 Collinsd
 * Lasers on models now work properly.  New Primary weapon code.  Looping
 * Anims dont reset when shot.
 * 
 * 86    15/11/97 16:57 Collinsd
 * Secondary weapons now effect water, and bubbles hitting surface effect
 * water.
 * 
 * 85    10/11/97 11:12 Collinsd
 * Added new graphjcs ( transpulse, trojax .... )
 * 
 * 84    7/11/97 14:51 Collinsd
 * Fixed enemy gun fire points.
 * 
 * 83    3/11/97 18:27 Collinsd
 * Added flames and shrapnel explosions.  ( Also effect Water ).
 * 
 * 82    8/10/97 16:32 Collinsd
 * Added spotfx and started lightning bolt.
 * 
 * 81    6/10/97 14:41 Collinsd
 * Added enemy pulsar, trojax, sussgun, transpulse, laser ( Same as
 * players but different colours ).
 * 
 * 80    29/09/97 11:35 Collinsd
 * New bgobject stuff enabled. Pickups/Secondary and Enemies now have
 * group link lists.  Flags for secondary bullets being targeted and hit
 * now added.
 * 
 * 79    9/10/97 9:54a Phillipd
 * 
 * 78    1/09/97 11:37 Collinsd
 * Enemies target ships and ships target enemies/mines when homing.
 * 
 * 77    11/08/97 17:21 Collinsd
 * Added visible group checks.
 * 
 * 76    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 75    24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 74    22/07/97 17:23 Collinsd
 * 
 * 73    21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 72    6/24/97 11:13a Phillipd
 * 
 * 71    16-04-97 4:38p Collinsd
 * Added reflecting bullets for all weapons except laser.
 * Cheat mode now has invul effect.
 * 
 * 70    9-04-97 8:51a Collinsd
 * Added invulnerability
 * 
 * 69    7-04-97 3:34p Collinsd
 * Added supernashram powerup.
 * 
 * 68    4-04-97 11:48a Collinsd
 * Changed over to 3 power pods for good!!!!!
 * 
 * 67    2-04-97 6:39p Collinsd
 * Uses 3 powerpods.
 * 
 * 66    2-04-97 11:54a Collinsd
 * Added Jo bike
 * 
 * 65    16-03-97 8:07p Collinsd
 * Changed missiles
 * 
 * 64    11-03-97 10:11a Collinsd
 * Orbit pular using general ammo bug fixed.
 * 
 * 63    10-03-97 12:48p Collinsd
 * Transpulse loses speed and size on rebound.
 * 
 * 62    9-03-97 9:55p Collinsd
 * Changed primary weapons, Added screen polys indicating how many orbit
 * pulsars,
 * 
 * 61    5-03-97 5:00p Collinsd
 * 
 * 60    6-01-97 9:11a Collinsd
 * 
 * 59    3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 58    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 57    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 56    21-12-96 5:24p Collinsd
 * Added drop primary/drop secondary weapons.
 * 
 * 55    20-12-96 5:06p Collinsd
 * Added debug weapon feature.
 * 
 * 54    19-12-96 4:17p Collinsd
 * Added lighting when charging thr trojax.
 * 
 * 53    19-12-96 3:19p Collinsd
 * Changed sfx funtion to allow frequency changing.
 * Added Trojax Charging SFX.
 * 
 * 52    19-12-96 11:45a Collinsd
 * Added better ship collision for Trojax and Transpulse.
 * 
 * 51    11/28/96 4:00p Collinsd
 * Started hull damage effect
 * 
 * 50    14/11/96 9:18 Collinsd
 * Altered pyrolite a bit.
 * 
 * 49    31/10/96 16:30 Collinsd
 * Primary/Secondary now included ship updates.  Stealth regeneration
 * fixed
 * 
 * 48    26/10/96 18:04 Collinsd
 * Added recoil vector to ihityou.
 * 
 * 47    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 46    19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 45    16/10/96 12:38 Collinsd
 * Shock waves now affect mines.
 * Fixed bug in ship/mine collision
 * 
 * 44    10/10/96 4:18p Phillipd
 * removed mesh.h..no longer used..
 * 
 * 43    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 42    9/10/96 12:28 Collinsd
 * Added code to handle low poly, single direction sphere.
 * 
 * 41    9/10/96 10:02 Collinsd
 * Old Sfx back, new gravgon trail.
 * 
 * 40    7/10/96 20:55 Collinsd
 * Collision now works with new data.
 * 
 * 39    7/10/96 13:52 Collinsd
 * Added new smoke trails
 * 
 * 38    5/10/96 20:42 Collinsd
 * Added ammo usage to primary weapons.  Cleaned up
 * primary/secondary weapon code.  Added primary lights detail variable.
 * 
 * 37    1/10/96 17:43 Collinsd
 * Reduced pulsar to half.  Tidied up primary weapons.
 * deleted redundant code in 2dpolys/primary weapons.
 * 
 * 36    20/09/96 17:13 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 35    17/09/96 13:58 Collinsd
 * Added autoselection of primary and secondary weapons.
 * Also you can pickup anything if F12 enabled.
 * 
 * 34    13/09/96 12:59 Collinsd
 * Added shield effect to external views.
 * 
 * 33    12/09/96 17:52 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 32    7/09/96 20:21 Collinsd
 * Collision with pine mine now added.  Trojax power
 * fixed, now does proper damage.  Transpulse speeded
 * up and made more lethal.
 * 
 * 31    2/09/96 14:18 Collinsd
 * Finished shield effect, started on secondary weapons.
 * 
 * 30    29/08/96 17:53 Collinsd
 * Lots more pickups and shield working a little better.
 * 
 * 29    28/08/96 11:15 Collinsd
 * 
 * 28    23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 27    21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 26    16/08/96 17:29 Collinsd
 * Damage variables now in..
 * 
 * 25    15/08/96 17:28 Collinsd
 * Laser now hits ships.  Bug with primary weapons hitting background
 * before ship fixed.
 * 
 * 24    15/08/96 9:54 Collinsd
 * Added glow to pickups and started primary wepon powerups
 * 
 * 23    13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 22    9/08/96 10:18 Collinsd
 * Added powerlevel to primary weapons.
 * Added a few more pickup models.
 * 
 * 21    8/08/96 9:13 Collinsd
 * Added Sfx and pickups
 * 
 * 20    5/08/96 20:04 Collinsd
 * Network for pickups now works,
 * 
 * 19    4/08/96 17:04 Collinsd
 * All  primary weapons are now in. ( no power ups )
 * 
 * 18    2/08/96 19:47 Collinsd
 * Added sussgun lines
 * 
 * 17    2/08/96 16:13 Collinsd
 * Fixed primary weapon net loop bug.  Oooops...
 * 
 * 16    2/08/96 9:28 Collinsd
 * Added double sided 2D polys, Static 2D polys.
 * Added transpulse cannon and pyrolite weapon.
 * Weapons should work correctly over network.
 * 
 * 15    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 14    25/07/96 17:44 Collinsd
 * Trojax weapon now has variable sizes. Primary weapon collision with
 * ships now creates the proper explosion
 * 
 * 13    25/07/96 10:08 Collinsd
 * Added variable speed to trojax weapon.
 * 
 * 12    24/07/96 12:59 Collinsd
 * Started on primary weapons.
 * 
 * 11    23/07/96 15:40 Collinsd
 * 
 * 10    21/07/96 20:42 Collinsd
 * Added plasma sparks. ( Crap at the moment ! ).
 * 
 * 9     21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 8     19/07/96 16:56 Collinsd
 * Now have seperate explosion for Ship and Background.
 * Increased Max 2DPolys, and have framelag compensated fireing of the
 * primary and secondary weapons.
 * 
 * 7     7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 6     6/07/96 22:25 Collinsd
 * Added pulse to 2dPoly's.
 * 
 * 5     4/07/96 17:03 Collinsd
 * 
 * 4     7/04/96 9:49a Phillipd
 * 
 * 3     7/03/96 2:45p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *
 *  File:       primary.h
 *  Content:    primary include file
 *
 *
 ***************************************************************************/

#ifndef PRIMARY_INCLUDED
#define PRIMARY_INCLUDED

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Defines
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#define	MAXPRIMARYWEAPONBULLETS	256
#define MAXPRIMARYWEAPONS		6

#define	MAXENEMYPRIMARYWEAPONS	10
#define	TOTALPRIMARYWEAPONS		( MAXPRIMARYWEAPONS + 1 + MAXENEMYPRIMARYWEAPONS )

#define	MAXPOWERLEVELS			3

#define	MAXGENERALAMMO		2000.0F
#define	MAXPYROLITEAMMO		3000.0F
#define	MAXSUSSGUNAMMO		3000.0F
#define	MAXMULTIPLEAMMO		1000.0F

#define	MINIMUM_TOTAL_AMMO	100.0F

#define	GENERALAMMOPERPOD	500.0F
#define	PYROLITEAMMOPERPOD	500.0F
#define	SUSSGUNAMMOPERPOD	500.0F

#define	PULSAR				0
#define	TROJAX				1
#define	PYROLITE_RIFLE		2
#define	TRANSPULSE_CANNON	3
#define	SUSS_GUN			4
#define	LASER				5
#define	ORBITPULSAR			6
#define	NME_BULLET1			7
#define	NME_PULSAR			8
#define	NME_TROJAX			9
#define	NME_PYROLITE		10
#define	NME_TRANSPULSE		11
#define	NME_SUSS_GUN		12
#define	NME_LASER			13
#define	NME_LIGHTNING		14
#define	FLAMES				15
#define	NME_POWERLASER		16

#define	NO_PRIMARY			128

#define	ORBITPULSAR_FMSIZE	( ( 2.0F * GLOBAL_SCALE ) * 0.5F )

#define	PULSAR_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	PULSAR_FMSIZE		( ( 5.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILSTART	( ( 40.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILEND		( ( 20.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILPOWERMUL	( ( 240.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_TAILADDMIN	( ( 376.0F * GLOBAL_SCALE ) * 0.5F )
#define	PULSAR_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 0.5F )

#define	TROJAX_COLRADIUS	( ( 512.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_SIZE			( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPSIZE		( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPMAXSIZE	( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	TROJAX_EXPINC		( ( 0.333F * GLOBAL_SCALE ) * 1.0F )
  
#define	PYROLITE_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	PYROLITE_POWERMUL	( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	PYROLITE_ADDMIN		( ( 0.75F * GLOBAL_SCALE ) * 1.0F )

#define	TRANSPULSE_COLRADIUS ( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_POWERMUL	( ( 2.4F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_ADDMIN	( ( 4.2F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_POWERMUL		( ( 3.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_ADDMIN			( ( 6.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARC_LENGTH			( ( 12.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_SPARKSIZE ( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_INC	( ( 0.333F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_MINSIZE	( ( 1.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_EXP_MAXSIZE	( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_FLARE_SIZE ( ( 10.0F * GLOBAL_SCALE ) * 1.0F )
#define	TRANSPULSE_BOUNCES	( 1 )
#define	TRANSPULSE_VIEWCONE	( 160.0F / 2.0F )

#define	ARCTRAIL_POWERMUL	( ( 360.0F * GLOBAL_SCALE ) * 1.0F )
#define	ARCTRAIL_ADDMIN		( ( 580.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_POWERMUL	( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_ADDMIN		( ( 0.5F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_FLARESIZE	( ( 4.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUN_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	SUSSGUNLIFE_POWERMUL ( 10.0F )
#define	SUSSGUNLIFE_ADDMIN	 ( 10.0F )

#define	LASER_COLRADIUS		( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_WIDTH_POWERMUL ( ( 8.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_WIDTH_ADDMIN	( ( 24.0F * GLOBAL_SCALE ) * 1.0F )
#define	LASER_TEMP_POWERMUL ( 0.5F )
#define	LASER_TEMP_ADDMIN	( 0.5F )

#define	NMEBUL1_COLRADIUS	( ( 0.0F * GLOBAL_SCALE ) * 1.0F )
#define	NMEBUL1_FMSIZE		( ( 5.0F * GLOBAL_SCALE ) * 2.0F )
#define	NMEBUL1_TAILSTART	( ( 40.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILEND		( ( 20.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILPOWERMUL ( ( 240.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_TAILADDMIN	( ( 376.0F * GLOBAL_SCALE ) * 0.5F )
#define	NMEBUL1_EXPSIZE		( ( 8.0F * GLOBAL_SCALE ) * 0.5F )

#define	SPARKSIZE			( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	SHIP_EXPLO_SIZE		( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	NORM_EXPLO_SIZE		( ( 16.0F * GLOBAL_SCALE ) * 1.0F )

#define	SMOKE_TRAIL_SIZE	( ( 2.0F * GLOBAL_SCALE ) * 1.0F )
#define	GRAVGON_TRAIL_SIZE	( ( 7.0F * GLOBAL_SCALE ) * 1.0F )
#define	GRAVGON_TRAIL_INC	( ( 0.4F * GLOBAL_SCALE ) * 1.0F )
#define	SOLARIS_TRAIL_SIZE	( ( 7.0F * GLOBAL_SCALE ) * 1.0F )
#define	NEW_TRAIL_SIZE		( ( 7.0F * GLOBAL_SCALE ) * 1.0F )

#define	PLASER_COLRADIUS	( ( 200.0F * GLOBAL_SCALE ) * 1.0F )
#define	PLASER_WIDTH_POWERMUL ( ( 16.0F * GLOBAL_SCALE ) * 1.0F )
#define	PLASER_WIDTH_ADDMIN	( ( 48.0F * GLOBAL_SCALE ) * 1.0F )

#define	GRAVGON_SCALE_DEC	0.075F

#define	PICKUP_REGEN_SIZE	( ( 8.0F * GLOBAL_SCALE ) * 1.0F )

#define	BLOODSIZE			( ( 4.0F * GLOBAL_SCALE ) * 1.0F )

#define	FLAMES_COLRADIUS	( ( 500.0F * GLOBAL_SCALE ) * 1.0F )

#define	PULSAR2SHOTPOWERLEVEL	( 1 )

#define	COLTYPE_Point		0
#define	COLTYPE_Sphere		1
#define	COLTYPE_Trojax		2
#define	COLTYPE_Transpulse	3

#define	WEPTYPE_Primary		0
#define	WEPTYPE_Secondary	1
#define	WEPTYPE_Various		2

#define	OWNER_NOBODY		0
#define	OWNER_SHIP			1
#define	OWNER_ENEMY			2
#define	OWNER_BGOBJECT		3
#define	OWNER_MINE			4
#define	OWNER_MODELSPOTFX	5

#define	COLPERS_Forsaken	0					// Forsaken Collision ( My Perspective )
#define	COLPERS_Descent		1					// Descent Collision ( Your Perspective )
#define	COLPERS_Server		2					// Server decides who gets hit....

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct PRIMARYWEAPONATTRIB {

	float		Speed[ MAXPOWERLEVELS ];		// how fast do I move
	float		LifeCount;						// how long can I last..
	uint16		FireDelay[ MAXPOWERLEVELS ];	// how fast can I fire
	float		AmmoUsage[ MAXPOWERLEVELS ];	// how much ammo do i use per shot...
	float		Damage[ MAXPOWERLEVELS ];		// how much damage can i inflict...
	float		ColRadius[ MAXPOWERLEVELS ];	// Collision Radius...
	uint16		ColType;						// Collision Type...
	float		lightsize;						// how big is the light
	float		r[ MAXPOWERLEVELS ];			// what color does it emit....
	float		g[ MAXPOWERLEVELS ];			// what color does it emit....
	float		b[ MAXPOWERLEVELS ];			// what color does it emit....
	int16		FmSeq;							// Faceme Sequence
	FRAME_INFO ** FmFrmInfo;					// Faceme FrameInfo

} PRIMARYWEAPONATTRIB;

typedef struct PRIMARYWEAPONBULLET {
	BOOL		Used;
	uint16		Next;							// link list.....	
	uint16		Prev;							// link list.....
	uint16		Type;							// which type of bullet am I
	uint16		OwnerType;						// who fired me...
	uint16		Owner;							// who fired me...
	void *		EnemyGun;						// Only used by enemies.
	uint16		ID;								// Id from that ship..
	int8		Weapon;							// Weapon type that fired me..
	int16		PowerLevel;						// Power level the weapon has..
	float		TrojPower;						// Trojax Power
	float		LifeCount;						// how long do I live.....
	float		Speed;							// how fast do I move
	float		ColRadius;						// how big is my collide sphere
	uint16		ColType;						// what type of collision
	VECTOR		Offset;							// offset relative to ship that fired me...
	VECTOR		Pos;							// where am I
	VECTOR		Dir;							// where am i going
	VECTOR		LocalDir;						// Local Direction
	VECTOR		UpVector;						// UpVector
	VECTOR		ColStart;						// where I was when started going straight.
	float		ColDist;						// Length from Collision to ColStart.
	uint16		ColFlag;						// have I checked if im going to collide with something
	VERT		ColPoint;						// where am I going to collide
	NORMAL		ColPointNormal;					// if I reflect use this when I do
	uint16		ColGroup;						// where am I going to collide
	uint16		GroupImIn;						// which group am I in...
	MATRIX		Mat;							// and a Matrix for when Im displayed...
	uint16		line;							// attached line
	uint16		fmpoly;							// if Im 2D then I need a Face Me Poly..
	int16		numfmpolys;						// Number of linked fmpolys
	uint16		poly;							// first attached poly
	int16		numpolys;						// Number of attached polys
	uint16		light;							// if I produce Light then I Need A Light
	float		lightsize;						// how big is the light
	float		r;								// what color does it emit....
	float		g;								// what color does it emit....
	float		b;								// what color does it emit....
	uint16		Bounces;						// Number of bounces
	float		TimeInterval;
	float		TimeCount;
	int16		FirePoint;						// gun...
	int16		SpotFX;
	float		FramelagAddition;

} PRIMARYWEAPONBULLET;							// sizes and colours of 2d polys and Lights are set up depoending on type...


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Prototypes
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void	InitPrimBulls(void);
uint16	FindFreePrimBull(void);
void	KillUsedPrimBull( uint16 i );
void	FirePrimary(void);
void	ProcessPrimaryBullets( void );
void	CleanUpPrimBull( uint16 i, BOOL LightFlag  );
uint16 InitOnePrimBull( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag );
void	CreateTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group );
void	CreateNmeTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group );
void	CreatePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmeShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateFireExplosion( VECTOR * Pos, uint16 Group );
void	CreateNmeArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNmeLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateNmePowerLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateNmeLightningPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup );
void	CreateTransTrail( uint16 i );
void	DispPowerLevel( void );
void	RestoreAmmo( void );
void	RestoreWeapons( void );
void	InitAmmoUsed( void );
void	CreateShieldEffect( VECTOR * Pos, VECTOR * IntPos, VECTOR * IntPos2, uint16 ShipHit, int16 Num,
							 uint8 Red, uint8 Green, uint8 Blue );
uint16	CheckHitShip( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType );
uint16	CheckHitSecondary( VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType );
void	PrimarySelectControl( void );
void	CreateDebugLine( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void	EnablePrimaryLights( void );
void	DisablePrimaryLights( void );
void	FirePrimaryWeapons( uint8 Ship );
void	CreateColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSmallColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CheckCharging( uint16 Ship, uint32 OldFlags, uint32 NewFlags );
void	GivemeAllWeapons( void );
void	LoseAllWeapons( void );
void	DropPrimary( void );
void	GetDeathString( BYTE WeaponType, BYTE Weapon, char * DstString );
void	DropShield( void );
void	DropAmmo( void );
BOOL ChangeTranspulseDir( uint16 i, VECTOR * Pos, VECTOR * MoveOffset, VECTOR * Dir );
void	ChangePulsarDir( uint16 i, VECTOR * Dir );
void	ChangeTrojaxDir( uint16 i, VECTOR * Dir );
void	ChangeSussgunDir( uint16 i, VECTOR * Dir );
void	ChangeOrbitPulsarDir( uint16 i, VECTOR * Dir );
void	ReflectLaser( uint16 i, VECTOR * Dir );
void	GivemeSuperNashram( void );
void	LoseSuperNashram( void );
void	CreateInvEffect( uint16 ShipHit, int16 Num, uint8 Red, uint8 Green, uint8 Blue );
void	CreateSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateMushroomExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateNewExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void	CreateSteam( VECTOR * Pos, VECTOR * Dir, uint16 Group );
uint16	EnemyFirePrimary( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag, void * Gun );
void CreatePowerLaserSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue );
void GetLaserLocalVector( uint16 i, VECTOR * LocalVector );
void CreateScaleExplosion( VECTOR * Pos, uint16 Group, float GlobalScale );
BOOL ValidGroupCollision( VECTOR * StartPos, uint16 StartGroup, VECTOR * IntPoint, VECTOR * ObjPos, uint16 ObjGroup );
int16 GetAmmoPickupToGen( void );
FILE * SavePrimBulls( FILE * fp );
FILE * LoadPrimBulls( FILE * fp );
uint8 GetNextValidPrimaryWeapon( void );
uint8 GetPrevValidPrimaryWeapon( void );
int8 ConvPrimToNormWeapon( int8 Primary );
void TogglePrimaryWeapons( int8 Primary );
void KillPrimBullsByOwner(  uint16 OwnerType, uint16 Owner );
void HostGivemeSuperNashram( uint16 Player );
void HostLoseSuperNashram( uint16 Player );
void HostRestoreWeapons( int16 Player );
void HostRestoreAmmo( int16 Player );
float GetPrimaryAmmoUsage( int16 Ship, int8 Weapon );

#endif // PRIMARY_INCLUDED


