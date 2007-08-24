/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 165 $
 *
 * $Header: /PcProjectX/Include/models.h 165   24/06/98 11:04 Philipy $
 *
 * $Log: /PcProjectX/Include/models.h $
 * 
 * 165   24/06/98 11:04 Philipy
 * added gateway flag
 * 
 * 164   15/06/98 10:53 Oliverc
 * 
 * 163   11/06/98 16:57 Philipy
 * loads of win98 shareware version stuff
 * 
 * 162   15/04/98 11:46 Collinsd
 * Flygirl now stealths correctly
 * 
 * 161   8/04/98 13:19 Collinsd
 * Bike engines not toggle on/off
 * 
 * 160   26/03/98 12:28 Philipy
 * added water detail & gamma sliders
 * fixed holo object swapping stuff
 * fixed crash bug when selecting room in debug mode
 * 
 * 159   20/03/98 17:23 Collinsd
 * Hopefully fixed crashbug on reactor on nps-sp01
 * 
 * 158   20/03/98 10:19 Collinsd
 * 
 * 157   19/03/98 20:49 Collinsd
 * Added Model and Texture loading optimisation. ( Doesn't load
 * textures/models not used in level/mode ).
 * 
 * 156   19/03/98 11:29 Collinsd
 * Bike appears in missile and pip cameras
 * 
 * 155   11/03/98 21:01 Collinsd
 * Added naked bikers.
 * 
 * 154   3/03/98 18:20 Collinsd
 * CTF Team game
 * 
 * 153   2/03/98 12:02 Collinsd
 * Added ability to allocate comp objects from front end.
 * 
 * 152   27/02/98 16:30 Collinsd
 * Added relinit sfx code.
 * 
 * 151   26/02/98 20:41 Philipy
 * added front end for load game
 * 
 * 150   25/02/98 16:06 Collinsd
 * Save Picture for save game...
 * 
 * 149   23/02/98 21:20 Collinsd
 * Optimised Load/Save Functions.
 * 
 * 148   23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 147   19/02/98 22:01 Collinsd
 * Added flygirl biker.
 * 
 * 146   18/02/98 11:53 Oliverc
 * First feature-complete version of force feedback joystick code
 * 
 * 145   16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 144   13/02/98 13:00 Collinsd
 * Added SoundFX on Enemies.
 * 
 * 143   11/02/98 16:29 Collinsd
 * Added soundfx for bikes/enemies
 * 
 * 142   3/02/98 15:30 Collinsd
 * Added Fireballs for fleshmorph and ability to disable spotfx on models,
 * from within the animation
 * 
 * 141   2/02/98 10:07 Philipy
 * played around with model fading functions
 * 
 * 140   29/01/98 20:20 Oliverc
 * Disabled Tom Cruise (flaming death biker) for both SHAREWARE and
 * SELF_PLAY versions
 * 
 * 139   1/22/98 12:06p Phillipd
 * 
 * 138   20/01/98 16:34 Collinsd
 * Added SpotFX to bikes.
 * 
 * 137   19/01/98 12:54 Collinsd
 * Added Golden Idol.
 * 
 * 136   1/17/98 4:02p Phillipd
 * 
 * 135   16/01/98 16:17 Collinsd
 * Increased Model headers
 * 
 * 134   13/01/98 17:37 Collinsd
 * Added back pyrolite fuel,  Changes secondary to try to get rid of any
 * chance of any link list corruption.
 * 
 * 133   10/01/98 13:03 Collinsd
 * Added bomb pickup and change collectpickup.
 * 
 * 132   9/01/98 11:01 Collinsd
 * Stuff back to normal and goldbars not get carried over levels.
 * 
 * 131   6/01/98 15:39 Collinsd
 * Taken out bike stuff for shareware
 * 
 * 130   5/01/98 16:41 Collinsd
 * 
 * 129   19/12/97 12:35 Collinsd
 * Increased model limit and disabled restart point model.
 * 
 * 128   18/12/97 15:34 Collinsd
 * Fixed Restart Points
 * 
 * 127   12/12/97 17:43 Collinsd
 * Started Beard afterburner.
 * 
 * 126   11/12/97 15:53 Collinsd
 * Fixed enemy mines that didn't hurt you.  Also fixed enemy homing
 * missile.
 * 
 * 125   9/12/97 10:45 Collinsd
 * Done ship burning and change body parts and pickups now explode in
 * direction of ship movement.
 * 
 * 124   5/12/97 14:11 Collinsd
 * Allocated ships fixed.
 * 
 * 123   4/12/97 15:12 Collinsd
 * Ships are now allocated models.
 * 
 * 122   27/11/97 12:32 Collinsd
 * All spotfx timing now work again. Rock and Glass models added.
 * 
 * 121   26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 120   24/11/97 20:04 Collinsd
 * Lasers on models now work properly.  New Primary weapon code.  Looping
 * Anims dont reset when shot.
 * 
 * 119   21/11/97 9:09 Collinsd
 * Taken out all models not used.
 * 
 * 118   19/11/97 9:42 Collinsd
 * Added new level specific loading.
 * 
 * 117   14/11/97 8:35 Collinsd
 * Added goldbar model. ( Collect 10 and get extra life. )
 * 
 * 116   6/11/97 15:26 Collinsd
 * SpotFX on models now have initial delay.  
 * 
 * 115   6/11/97 11:37 Collinsd
 * Added BGObject Lighting, Stopped primary weapons creating lines when
 * outside group and made secondary weapons create debug lines when no
 * collision.
 * 
 * 114   3/11/97 18:27 Collinsd
 * Added flames and shrapnel explosions.  ( Also effect Water ).
 * 
 * 113   30/10/96 10:07 Collinsd
 * Clipping of large objects better.
 * 
 * 112   29/10/97 9:26 Collinsd
 * Added new spotfx and pointdir stuff.
 * 
 * 111   25/10/97 11:19 Collinsd
 * You can now pickup ammo when you have supernasram weapon
 * 
 * 110   24/10/97 17:08 Collinsd
 * Pickups no longer dissapear in single player.
 * 
 * 109   24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 108   23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 107   11/10/97 17:02 Collinsd
 * Added more point/Vector for model stuff. Changed NME Lightning and
 * added fire primary to spotfx.
 * 
 * 106   9/10/97 11:33 Collinsd
 * Added new spotfx and enemies have ambient lights.
 * 
 * 105   4/10/97 15:58 Collinsd
 * Added new spiraling missile and took out old morph code.
 * 
 * 104   24/09/97 16:55 Collinsd
 * Added new bgobject stuff.  Added bounding box calc and display code.
 * and changes bgobject state changes.
 * 
 * 103   17-09-97 4:12p Philipy
 * dynamic loading of textures now possible (for mxa models and screen
 * polys)
 * 
 * 102   9/05/97 4:45p Phillipd
 * 
 * 101   19/08/97 11:57 Collinsd
 * Updated flame guy.
 * 
 * 100   12/08/97 18:11 Collinsd
 * Conditional tom cruise
 * 
 * 99    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 98    24/07/97 17:08 Collinsd
 * Added slick
 * 
 * 97    22-07-97 3:55p Philipy
 * change MODFUNC_ScaleXYZ to include rotation vector
 * 
 * 96    21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 95    16-07-97 4:36p Philipy
 * added cerbero to title models
 * 
 * 94    16-07-97 4:10p Philipy
 * added bikes to title screen models
 * 
 * 93    16/07/97 12:34 Collinsd
 * Adder cerbero bike
 * 
 * 92    10-07-97 6:04p Philipy
 * changed order of title models
 * 
 * 91    9/07/97 16:59 Collinsd
 * Fixed MXA models in titles
 * 
 * 90    7/09/97 3:14p Phillipd
 * Ambient Light Version 1
 * 
 * 89    7/09/97 2:31p Phillipd
 * 
 * 88    9-07-97 2:20p Philipy
 * added StoreTriangles flag to MODELNAME structure
 * 
 * 87    9-07-97 11:38a Philipy
 * added test bike to title models
 * 
 * 86    3/07/97 10:49 Collinsd
 * Merged MXA and MX models and added morphing objects.
 * 
 * 85    1/07/97 16:05 Collinsd
 * Added mofisto bike
 * 
 * 84    19-06-97 3:49p Philipy
 * added new title room models
 * 
 * 83    8/06/97 17:07 Collinsd
 * Done more work on BGObjects/Doors
 * 
 * 82    5/06/97 12:40 Collinsd
 * Reduced texture pages.
 * 
 * 81    19-05-97 8:20p Philipy
 * Changed order of title screen models
 * 
 * 80    18/05/97 21:48 Collinsd
 * Changed max model headers to 256 for extra BGobjects
 * 
 * 79    9-05-97 6:25p Philipy
 * changed order of title screen models (again)
 * 
 * 78    9-05-97 12:54p Philipy
 * Added MODFUNC_RestoreColours to enum
 * 
 * 77    8-05-97 11:28a Philipy
 * changed MAXNUMOFTITLEMODELS to NUMOFTITLEMODELS for clarity
 * 
 * 76    7/05/97 9:55 Collinsd
 * Fixed reloading of title models.
 * 
 * 75    6-05-97 11:58a Philipy
 * Added remaining title room models
 * 
 * 74    30-04-97 3:23p Philipy
 * 
 * 73    4/30/97 11:23a Phillipd
 * 
 * 72    22-04-97 3:25p Collinsd
 * Added player flying out of bike.
 * 
 * 71    4/07/97 12:27p Phillipd
 * 
 * 70    24-03-97 12:54p Collinsd
 * changed detail level of explosion.  Changed process model function
 * 
 * 69    19-03-97 11:30a Collinsd
 * 
 * 68    17-03-97 8:57a Collinsd
 * Poly's, Screen Polys, FmPolys now can create up to 1000 each.
 * 
 * 67    12-03-97 12:40p Collinsd
 * Added new body parts.
 * 
 * 66    4-03-97 6:01p Collinsd
 * Added fog to tracker.
 * 
 * 65    4-03-97 4:50p Collinsd
 * Added Tracker.
 * 
 * 64    25-02-97 10:20a Collinsd
 * Corrected framelag on various bits.
 * 
 * 63    24-02-97 11:21a Collinsd
 * Added gravgon check for restart. and added bounding on object selection
 * depending on mode.
 * 
 * 62    24-02-97 10:23a Collinsd
 * Added component bgobjects.
 * 
 * 61    11-02-97 5:08p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 60    21-01-97 5:34p Collinsd
 * Added the ability to delete bgobjects.
 * 
 * 59    10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 58    6-01-97 11:09a Collinsd
 * All references to timer now replaced with titanstar.
 * 
 * 57    6-01-97 9:11a Collinsd
 * 
 * 56    3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 55    2-01-97 4:22p Collinsd
 * Stealthed orbit pulsars now works.  Orbit pulsar and stealth mantle
 * regeneration bug fixed.
 * 
 * 54    30-12-96 5:22p Collinsd
 * Fixed firerate of orbit pulsar.
 * 
 * 53    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 52    22-12-96 8:53p Collinsd
 * Added invisible shockwave to all missiles. hitting walls.
 * 
 * 51    22-12-96 7:27p Collinsd
 * Increased model count.  Decreased scatter viewcone.  Lensflare now
 * works properly on scatter missiles.  Names are printed after the
 * endscene.
 * 
 * 50    19-12-96 3:19p Collinsd
 * Changed sfx funtion to allow frequency changing.
 * Added Trojax Charging SFX.
 * 
 * 49    20/11/96 14:05 Collinsd
 * Added lower detail version of golden powerpod.
 * 
 * 48    18/11/96 15:10 Collinsd
 * Added new borg girl models. Changed dissapper time for scattered
 * weapons.
 * 
 * 47    14/11/96 17:35 Collinsd
 * added borg girl as pickup. ( TEMP )
 * 
 * 46    13/11/96 15:46 Collinsd
 * Added golden powerpod and ljay.
 * 
 * 45    4/11/96 10:44 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 44    30/10/96 14:34 Collinsd
 * Added stealthmode.
 * 
 * 43    27/10/96 18:38 Collinsd
 * Optimised loads of shit.
 * 
 * 42    10/25/96 2:07p Phillipd
 * 3d panel tested, test for lost packets
 * 
 * 41    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 40    19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 39    18/10/96 17:46 Collinsd
 * Changed all file loading to load from tidy directories.
 * 
 * 38    17/10/96 16:43 Collinsd
 * Taken out redundent models.
 * 
 * 37    11/10/96 11:38 Collinsd
 * Added new models
 * 
 * 36    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 35    9/10/96 12:28 Collinsd
 * Added code to handle low poly, single direction sphere.
 * 
 * 34    8/10/96 10:22 Collinsd
 * Added lower detail transpulse. and new low detail timer.
 * 
 * 33    7/10/96 16:31 Collinsd
 * Added execsize to mx and mxafiles. Also fixed colour keying on models.
 * 
 * 32    3/10/96 12:01 Collinsd
 * Added the beard and borg body bits into models.
 * 
 * 31    30/09/96 14:48 Collinsd
 * Added simplified missiles.
 * Fixed distance 2dpoly's moved out by...
 * 
 * 30    9/26/96 11:21a Phillipd
 * MX load is now operational...
 * 
 * 29    9/25/96 5:53p Phillipd
 * added Simplified Models......And Bike Detail Level now
 * works..
 * 
 * 28    20/09/96 17:12 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 27    19/09/96 8:51 Collinsd
 * Added simplified new models for missiles
 * 
 * 26    18/09/96 17:36 Collinsd
 * Changed lines to be solid. added neds new models
 * 
 * 25    13/09/96 17:04 Collinsd
 * Added quantun fireball mine.
 * 
 * 24    12/09/96 17:51 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 23    10/09/96 16:07 Collinsd
 * Added gravgon effect. (Not finished)
 * 
 * 22    6/09/96 16:20 Collinsd
 * Added extra life pickup, and started pine mine.
 * 
 * 21    6/09/96 9:12 Collinsd
 * Timer missile works now.
 * 
 * 20    4/09/96 20:01 Collinsd
 * Scatter missile and thief missile now work!
 * 
 * 19    2/09/96 14:18 Collinsd
 * Finished shield effect, started on secondary weapons.
 * 
 * 18    30/08/96 17:30 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 17    29/08/96 21:05 Collinsd
 * Inproved shield effect.
 * 
 * 16    29/08/96 17:53 Collinsd
 * Lots more pickups and shield working a little better.
 * 
 * 15    28/08/96 15:03 Collinsd
 * Added 5 more pickups and fixed bug model group update.
 * 
 * 14    28/08/96 11:15 Collinsd
 * 
 * 13    23/08/96 17:22 Collinsd
 * Only Display Models in visible groups.
 * 
 * 12    23/08/96 14:42 Collinsd
 * Added bikes as models
 * 
 * 11    23/08/96 14:20 Collinsd
 * All finished models in.
 * 
 * 10    16/08/96 11:01 Collinsd
 * You now see explosions close to walls and when hit. Pickups and bullets
 * don't dissapear when changing mode.
 * 
 * 9     13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 8     9/08/96 10:18 Collinsd
 * Added powerlevel to primary weapons.
 * Added a few more pickup models.
 * 
 * 7     8/03/96 4:39p Phillipd
 * all Models can be env mapped..
 * Alpha texture formats are detected and reported...
 * But cant load a system Alpha texture into a Video Alpha Texture!!
 * 
 * 6     7/22/96 5:49p Phillipd
 * 
 * 5     7/21/96 2:29p Phillipd
 * 
 * 4     28/06/96 10:05 Collinsd
 * 
 * 3     6/27/96 4:32p Phillipd
 * 
 * 2     6/27/96 4:04p Phillipd
 * 
 * 1     6/27/96 3:52p Phillipd
 * 
 */

/*==========================================================================
 *
 *
 *  File:       models.h
 *  Content:    models.h include file
 *
 *
 ***************************************************************************/

#ifndef MODELS_INCLUDED
#define MODELS_INCLUDED

#include <math.h>
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include "Mload.h"
#include "collision.h"
#include "Ships.h"
#include "Mxload.h"
#include "Mxaload.h"
#include "lights.h"
#include "camera.h"

/*
 * defines
 */
#define MAXNUMOFMODELS		1024
#define NUMOFTITLEMODELS	32
#define NUM_INTERLEVEL_MODELS 17
#define	MAXMODELHEADERS		1024
#define	ORBIT_RADIUS		( 256 * GLOBAL_SCALE )
#define	MAXMODELSPOTFX		10

#define SCAN_WINDOW_WIDTH 20.0F

#define	MODFLAG_Nothing			0
#define	MODFLAG_Clip			1
#define	MODFLAG_Light			2
#define	MODFLAG_AmbientLight	4
#define	MODFLAG_UseClipGroup	8
#define	MODFLAG_RealLight		16
#define	MODFLAG_Stealth			32
#define	MODFLAG_Original		64
#define MODFLAG_SpotFXDisabled	128
#define MODFLAG_ShockwaveHitMe	256
#define MODFLAG_StealthOffset	512

enum {
	BIKE_Lokasenna, 
	BIKE_Beard, 
	BIKE_LAJay, 
	BIKE_ExCop, 
	BIKE_RexHardy,
	BIKE_Foetoid,
	BIKE_NimSoo,
	BIKE_Nutta,
	BIKE_Sceptre,
	BIKE_Jo,
	BIKE_Shark,
	BIKE_HK5,
	BIKE_Nubia,
	BIKE_Mofisto,
	BIKE_Cerbero,
	BIKE_Slick,
	BIKE_FlyGirl,
};

enum {

	MODEL_Heatseaker,			// 0
	MODEL_HeatseakerSmall,		// 1
	MODEL_Gravgon,				// 2
	MODEL_Gravgon_LOD1,			// 3
	MODEL_Trojax,				// 4
	MODEL_Trojax_LOD1,			// 5
	MODEL_Pyrolite,				// 6
	MODEL_Pyrolite_LOD1,		// 7
	MODEL_Transpulse,			// 8
	MODEL_Transpulse_LOD1,		// 9
	MODEL_SussGun,				// 10 
	MODEL_SussGun_LOD1,			// 11 
	MODEL_Laser,				// 12
	MODEL_Laser_LOD1,			// 13
	MODEL_PowerPod,				// 14
	MODEL_PowerPod_LOD1,		// 15
	MODEL_PyroliteAmmo,			// 16
//	MODEL_PyroliteAmmo_LOD1,	// 17
	MODEL_Shield,				// 18
	MODEL_Shield_LOD1,			// 19
	MODEL_SussGunAmmo,			// 20
	MODEL_SussGunAmmo_LOD1,		// 21
	MODEL_GeneralAmmo,			// 22
	MODEL_GeneralAmmo_LOD1,		// 23
	MODEL_Mug,					// 24
	MODEL_MugSmall,				// 25
	MODEL_Mugs,					// 26
	MODEL_Mugs_LOD1,			// 27
	MODEL_Scatter,				// 28
	MODEL_ScatterSmall,			// 29
//	MODEL_Thief,				// 30
//	MODEL_ThiefSmall,			// 31
	MODEL_TitanStar,			// 32
	MODEL_TitanStarSmall,		// 33
	MODEL_Launcher,				// 34
	MODEL_Launcher_LOD1,		// 35
	MODEL_Computer,				// 36
	MODEL_Computer_LOD1,		// 37
	MODEL_Pine,					// 38
	MODEL_Pine_LOD1,			// 39
	MODEL_PinePickup,			// 40
	MODEL_PinePickup_LOD1,  	// 41
	MODEL_QuantumPickup,		// 42
	MODEL_QuantumPickup_LOD1,	// 43
	MODEL_Quantum,				// 44
	MODEL_Quantum_LOD1, 		// 45
	MODEL_Purge,				// 46
	MODEL_Purge_LOD1,   		// 47
//	MODEL_Spider,				// 48
//	MODEL_Spider_LOD1,			// 49
	MODEL_HeatseakerPickup,		// 50
	MODEL_HeatseakerPickup_LOD1,// 51
//	MODEL_Smoke,				// 52
//	MODEL_Smoke_LOD1,			// 53
	MODEL_PurgePickup,			// 54
	MODEL_PurgePickup_LOD1, 	// 55
//	MODEL_Parasite,				// 56
//	MODEL_Parasite_LOD1,		// 57
	MODEL_Nitro,				// 58
	MODEL_Nitro_LOD1,			// 59
//	MODEL_Goggles,				// 60
//	MODEL_Goggles_LOD1,			// 61
	MODEL_Inv,					// 62
	MODEL_Inv_LOD1,				// 63
	MODEL_Gold,					// 64
	MODEL_Mantle,				// 65
	MODEL_Mantle_LOD1,			// 66
//	MODEL_Flare,				// 67
//	MODEL_Flare_LOD1,			// 68
//	MODEL_SpiderPod,			// 69
//	MODEL_SpiderPod_LOD1,		// 70
	MODEL_Crystal,				// 71
	MODEL_Orb,					// 72
	MODEL_Orb_LOD1,  			// 73
	MODEL_ExtraLife,			// 74
	MODEL_ExtraLife_LOD1,		// 75
	MODEL_PineMissile,			// 76
	MODEL_Sphere,				// 77
	MODEL_Sphere_LOD1,			// 78
	MODEL_Sphere_LOD2,			// 79
//	MODEL_Chaf,					// 80
//	MODEL_Chaf_LOD1,			// 81
//	MODEL_Panel,				// 82
	MODEL_GoldenPowerPod,		// 83
	MODEL_GoldenPowerPod_LOD1,	// 84
	MODEL_OrbitPulsar,			// 85
	MODEL_OrbitPulsar_Light,	// 86
	MODEL_TitanStarShrapnel,	// 87
	MODEL_Tracker,				// 88
	MODEL_Ping,					// 89
	MODEL_Brain,				// 90
	MODEL_Ear,					// 91
	MODEL_Eyeball,				// 92
	MODEL_Heart,				// 93
	MODEL_Kidney,				// 94
	MODEL_Leg,					// 95
	MODEL_ExplosionDonut,		// 96
	MODEL_EnemySpiralMissile,	// 97
	MODEL_DNA,					// 98
	MODEL_SkeletonKey,			// 99

	MODEL_Shrapnel,				// 99
	MODEL_Shrapnel_LOD1,		// 99
	MODEL_Shrapnel2,			// 99
	MODEL_Shrapnel2_LOD1,		// 99
	MODEL_Glass,				// 99
	MODEL_Rock,					// 99
	MODEL_Rock2,				// 99

	MODEL_Bomb,					// 99
	MODEL_GoldFigure,			// 99
	MODEL_Flag,					// 99
	MODEL_RedFlag,				// 99
	MODEL_GreenFlag,			// 99
	MODEL_BlueFlag,				// 99
	MODEL_YellowFlag,			// 99

#if !defined( SELF_PLAY ) && !defined( SHAREWARE ) && !defined( WIN98SHAREWARE ) && !defined( GATEWAY )
	MODEL_Tom0,					// 100
	MODEL_Tom1,
	MODEL_Tom2,
	MODEL_Tom3,
	MODEL_Tom4,
	MODEL_Tom5,
	MODEL_Tom6,
	MODEL_Tom7,
	MODEL_Tom8,
	MODEL_Tom9,
	MODEL_Tom10,
	MODEL_Tom11,
	MODEL_Tom12,
	MODEL_Tom13,
	MODEL_Tom14,
	MODEL_Tom15,
	MODEL_Tom16,
	MODEL_Tom17,
	MODEL_Tom18,
	MODEL_Tom19,
	MODEL_Tom20,
	MODEL_Tom21,
	MODEL_Tom22,
	MODEL_Tom23,
	MODEL_Tom24,
	MODEL_Tom25,
	MODEL_Tom26,
#endif

/*--------------- Bikes -----------------*/
#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	MODEL_Borg,					// 127
	MODEL_Borg_LOD1,
	MODEL_Borg_LOD2,
	MODEL_Borg_LOD3,
	MODEL_Borg_LOD4,
	MODEL_Borg_Stealth,
	MODEL_Borg_Stealth_LOD1,
	MODEL_Borg_Stealth_LOD2,
	MODEL_Borg_Stealth_LOD3,
	MODEL_Borg_Stealth_LOD4,
#endif

	MODEL_Beard,				// 137
	MODEL_Beard_LOD1,
	MODEL_Beard_LOD2,
	MODEL_Beard_LOD3,
	MODEL_Beard_LOD4,
	MODEL_Beard_Stealth,
	MODEL_Beard_Stealth_LOD1,
	MODEL_Beard_Stealth_LOD2,
	MODEL_Beard_Stealth_LOD3,
	MODEL_Beard_Stealth_LOD4,
	
#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	MODEL_LaJay,				// 147
	MODEL_LaJay_LOD1,
	MODEL_LaJay_LOD2,
	MODEL_LaJay_LOD3,
	MODEL_LaJay_LOD4,
	MODEL_LaJay_Stealth,
	MODEL_LaJay_Stealth_LOD1,
	MODEL_LaJay_Stealth_LOD2,
	MODEL_LaJay_Stealth_LOD3,
	MODEL_LaJay_Stealth_LOD4,
#endif
	
	MODEL_Excop,				// 157
	MODEL_Excop_LOD1,
	MODEL_Excop_LOD2,
	MODEL_Excop_LOD3,
	MODEL_Excop_LOD4,
	MODEL_Excop_Stealth,
	MODEL_Excop_Stealth_LOD1,
	MODEL_Excop_Stealth_LOD2,
	MODEL_Excop_Stealth_LOD3,
	MODEL_Excop_Stealth_LOD4,
	
	MODEL_Trucker,				// 167
	MODEL_Trucker_LOD1,				 
	MODEL_Trucker_LOD2,
	MODEL_Trucker_LOD3,
	MODEL_Trucker_LOD4,
	MODEL_Trucker_Stealth,
	MODEL_Trucker_Stealth_LOD1,
	MODEL_Trucker_Stealth_LOD2,
	MODEL_Trucker_Stealth_LOD3,
	MODEL_Trucker_Stealth_LOD4,
	
#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	MODEL_Foetoid,				// 177
	MODEL_Foetoid_LOD1,
	MODEL_Foetoid_LOD2,
	MODEL_Foetoid_LOD3,
	MODEL_Foetoid_LOD4,
	MODEL_Foetoid_Stealth,
	MODEL_Foetoid_Stealth_LOD1,
	MODEL_Foetoid_Stealth_LOD2,
	MODEL_Foetoid_Stealth_LOD3,
	MODEL_Foetoid_Stealth_LOD4,
	
	MODEL_JapBird,				// 187
	MODEL_JapBird_LOD1,
	MODEL_JapBird_LOD2,
	MODEL_JapBird_LOD3,
	MODEL_JapBird_LOD4,
	MODEL_JapBird_Stealth,
	MODEL_JapBird_Stealth_LOD1,
	MODEL_JapBird_Stealth_LOD2,
	MODEL_JapBird_Stealth_LOD3,
	MODEL_JapBird_Stealth_LOD4,
	
	MODEL_Nutter,				// 197
	MODEL_Nutter_LOD1,
	MODEL_Nutter_LOD2,
	MODEL_Nutter_LOD3,
	MODEL_Nutter_LOD4,
	MODEL_Nutter_Stealth,
	MODEL_Nutter_Stealth_LOD1,
	MODEL_Nutter_Stealth_LOD2,
	MODEL_Nutter_Stealth_LOD3,
	MODEL_Nutter_Stealth_LOD4,
	
	MODEL_Rhesus,				// 207
	MODEL_Rhesus_LOD1,
	MODEL_Rhesus_LOD2,
	MODEL_Rhesus_LOD3,
	MODEL_Rhesus_LOD4,
	MODEL_Rhesus_Stealth,
	MODEL_Rhesus_Stealth_LOD1,
	MODEL_Rhesus_Stealth_LOD2,
	MODEL_Rhesus_Stealth_LOD3,
	MODEL_Rhesus_Stealth_LOD4,
	
	MODEL_Joe,					// 217
	MODEL_Joe_LOD1,
	MODEL_Joe_LOD2,
	MODEL_Joe_LOD3,
	MODEL_Joe_LOD4,
	MODEL_Joe_Stealth,
	MODEL_Joe_Stealth_LOD1,
	MODEL_Joe_Stealth_LOD2,
	MODEL_Joe_Stealth_LOD3,
	MODEL_Joe_Stealth_LOD4,
	
	MODEL_Shark,				// 227
	MODEL_Shark_LOD1,
	MODEL_Shark_LOD2,
	MODEL_Shark_LOD3,
	MODEL_Shark_LOD4,
	MODEL_Shark_Stealth,
	MODEL_Shark_Stealth_LOD1,
	MODEL_Shark_Stealth_LOD2,
	MODEL_Shark_Stealth_LOD3,
	MODEL_Shark_Stealth_LOD4,
	
	MODEL_HK5,					// 237
	MODEL_HK5_LOD1,
	MODEL_HK5_LOD2,
	MODEL_HK5_LOD3,
	MODEL_HK5_LOD4,
	MODEL_HK5_Stealth,
	MODEL_HK5_Stealth_LOD1,
	MODEL_HK5_Stealth_LOD2,
	MODEL_HK5_Stealth_LOD3,
	MODEL_HK5_Stealth_LOD4,
#endif
	
	MODEL_Nubia,				// 247
	MODEL_Nubia_LOD1,
	MODEL_Nubia_LOD2,
	MODEL_Nubia_LOD3,
	MODEL_Nubia_LOD4,
	MODEL_Nubia_Stealth,
	MODEL_Nubia_Stealth_LOD1,
	MODEL_Nubia_Stealth_LOD2,
	MODEL_Nubia_Stealth_LOD3,
	MODEL_Nubia_Stealth_LOD4,
	
#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	MODEL_Mofisto,				// 257
	MODEL_Mofisto_LOD1,
	MODEL_Mofisto_LOD2,
	MODEL_Mofisto_LOD3,
	MODEL_Mofisto_LOD4,
	MODEL_Mofisto_Stealth,
	MODEL_Mofisto_Stealth_LOD1,
	MODEL_Mofisto_Stealth_LOD2,
	MODEL_Mofisto_Stealth_LOD3,
	MODEL_Mofisto_Stealth_LOD4,
#endif

	MODEL_Cerbero,				// 267
	MODEL_Cerbero_LOD1,
	MODEL_Cerbero_LOD2,
	MODEL_Cerbero_LOD3,
	MODEL_Cerbero_LOD4,
	MODEL_Cerbero_Stealth,
	MODEL_Cerbero_Stealth_LOD1,
	MODEL_Cerbero_Stealth_LOD2,
	MODEL_Cerbero_Stealth_LOD3,
	MODEL_Cerbero_Stealth_LOD4,

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	MODEL_Slick,				// 277
	MODEL_Slick_LOD1,
	MODEL_Slick_LOD2,
	MODEL_Slick_LOD3,
	MODEL_Slick_LOD4,
	MODEL_Slick_Stealth,
	MODEL_Slick_Stealth_LOD1,
	MODEL_Slick_Stealth_LOD2,
	MODEL_Slick_Stealth_LOD3,
	MODEL_Slick_Stealth_LOD4,
#endif

/*---------------------------------------*/

	MODEL_ExtraModels,			// 287
};

enum {

	TITLE_MODEL_Disc1,				//0 - Disc 1 of spinning mechanical device
	TITLE_MODEL_Disc2,				//1 - Disc 2
	TITLE_MODEL_Disc3,				//2 - Disc 3
	TITLE_MODEL_Disc4,				//3 - Disc 4
	TITLE_MODEL_Disc5,				//4 - Disc 5
	TITLE_MODEL_Disc6,				//5 - Disc 6			  
	TITLE_MODEL_Mbot,				//6 - Bottom of mech. device
	TITLE_MODEL_Room,				//7 - Main Room
	TITLE_MODEL_VDU,				//8 - VDU
	TITLE_MODEL_Holopad,			//9 - Holopad
	TITLE_MODEL_Mtop,				//10 - Top of spinning mechanical device
	TITLE_MODEL_MenuTVDummy,				//11 - Top of spinning mechanical device
	TITLE_MODEL_MenuTV,				//11 - Top of spinning mechanical device
	TITLE_MODEL_HoloLight,			//12 - Top of spinning mechanical device
	TITLE_MODEL_Bracket,			//13 - Top of spinning mechanical device

	TITLE_MODEL_Lokasenna, 
	TITLE_MODEL_Beard, 
	TITLE_MODEL_LAJay, 
	TITLE_MODEL_ExCop, 
	TITLE_MODEL_RexHardy,
	TITLE_MODEL_Foetoid,
	TITLE_MODEL_NimSoo,
	TITLE_MODEL_Nutta,
	TITLE_MODEL_Sceptre,
	TITLE_MODEL_Jo,
	TITLE_MODEL_Shark,
	TITLE_MODEL_HK5,
	TITLE_MODEL_Nubia,
	TITLE_MODEL_Mofisto,
	TITLE_MODEL_Cerbero,
	TITLE_MODEL_Slick,

	TITLE_MODEL_ExtraModels,		// ??

};

enum{
	TITLE_MODEL_Crystal = TITLE_MODEL_Bracket + 1,
	TITLE_MODEL_GoldBar,
};
		
enum {								
									
	MODFUNC_Nothing,				
	MODFUNC_Explode,				
	MODFUNC_Scale,					
	MODFUNC_Scale2,
	MODFUNC_BodyPart,
	MODFUNC_OrbitPulsar,
	MODFUNC_ReScale,
	MODFUNC_RealScale,
	MODFUNC_ScaleDonut,
	MODFUNC_Regen,
	MODFUNC_TomCruise,
	MODFUNC_RestoreColours,
	MODFUNC_ScaleXYZandRotate,
	MODFUNC_ProcessSpotFX,
	MODFUNC_SphereZone,
	MODFUNC_LargeShrapnel,
	MODFUNC_SmallShrapnel,
	MODFUNC_Fade,

};

enum {

	MODTYPE_Pickup,
	MODTYPE_Missile,
	MODTYPE_Field,

};

enum {

	MODELTYPE_Static,
	MODELTYPE_Morphing,

};

/*
 * structures
 */
typedef struct MODEL{
	uint16		Next;
	uint16		Prev;
	int16		Type;
	uint16		Flags;
	BYTE		SecWeapon;
	BOOL		Visible;
	uint16		ModelNum;
	uint16		Group;
	uint16		OwnerType;
	uint16		Owner;
	int16		Func;
	float		Scale;
	float		Xscale;
	float		Yscale;
	float		Zscale;
	float		MaxScale;
	float		LifeCount;
	float		AxisRot;
	float		Speed;
	float		TimeInterval;
	float		TimeCount;

	int16		Frame;			// Current Frame
	int16		InterpFrame;	// Wanted Frame
	float		InterpTime;		// Interpolation Time

	QUATLERP	Qlerp;
	QUAT		Quat;
	uint16		Ship;
	VECTOR		Pos;
	VECTOR		OldPos;
	VECTOR		Dir;
	VECTOR		Rot;
	MATRIX		Mat;
	MATRIX		InvMat;

	float		Radius;			// Bounding Radius ( Used for Real Lighting

	int			Red;
	int			Green;
	int			Blue;

	uint16		SpotFXFlags[ MAXMODELSPOTFX ];
	uint16		SpotFXState[ MAXMODELSPOTFX ];
	float		SpotFXDelay[ MAXMODELSPOTFX ];
	float		SpotFXTimeInterval[ MAXMODELSPOTFX ];
	float		SpotFXTimeCount[ MAXMODELSPOTFX ];
	uint32		SpotFXSFX_ID[ MAXMODELSPOTFX ];

	int16		StealthOffset;
	uint16		ClipGroup;
	uint16		TempLines[ 12 ];

	PLANE		IntersectionPlane;

}MODEL;

#define NUM_FADE_FRAMES 120.0F

typedef struct MODELNAME{
	char		Name[128];			// Name of the Model...
	int			LOD;				// How many of the following Model are Levels Of Detail..
	BOOL		Panel;
	BOOL		DoIMorph;			// Morphing (MXA?) TRUE/FALSE
	uint16		ModelIndex;			// MXA or MX Header index
	BOOL		StoreTriangles;		// retain triangle info?
	int			AllocateTpage;		// do we want to allocate space for a T-Page?
	BOOL		LevelSpecific;		// Level Specific?
	BOOL		LoadEnable;			// LoadEnable?

}MODELNAME;

/*
 * fn prototypes
 */

void OnceOnlyInitModel( void );
BOOL PreInitModel( LPDIRECT3DDEVICE lpDev, MODELNAME * ModelNames );
BOOL InitModel( LPDIRECT3DDEVICE lpDev , MODELNAME * ModelNames);
BOOL ModelDisp( uint16 group, LPDIRECT3DDEVICE lpDev, MODELNAME * NamePnt  );
void ProcessModels( void );


void	KillUsedModel( uint16 i );
uint16	FindFreeModel();

BOOL ReleaseModels( );
BOOL ProcessModel( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B );
BOOL ProcessModelExec( LPDIRECT3DEXECUTEBUFFER lpExBuf, int16 NumVerts, float Scale, float MaxScale, int8 R, int8 G, int8 B );
BOOL LightModel( uint16 Model, VECTOR * Pos );
BOOL LightMxModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF );
BOOL LightMxaModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF );
void ShockWave( VECTOR * Pos, float Radius, uint16 OwnerType, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon, uint16 model );
void GravityWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Grav, uint16 Group );
void ExplodeBody( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void ThrowOutRider( uint16 Ship );
void CreateBodyPart( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 ModelNum );
void CreateTomCruise( VECTOR * Pos, uint16 Group );
void MissileShockWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon );
void CreateOrbitPulsar( uint16 Ship );
BOOL LightModel2( uint16 Model, VECTOR * Pos );
BOOL CheckForGravgons( VECTOR * Pos );
BOOL TintModel( uint16 Model, float RF, float GF, float BF, float TF );
BOOL ShadeModel( uint16 Model, MATRIX * Matrix, float ZTrans, float Range );
void CreateExplosionDonut( VECTOR * Pos, VECTOR * Up, uint16 Group );
BOOL ReleaseTitleModels( );
BOOL TintMxaModel( MXALOADHEADER * DstMloadheader, float RF, float GF, float BF, float TF );
BOOL AmbientLightMxaModel( MXALOADHEADER * DstMloadheader, int R, int G, int B, int A , float r , float g , float b);
BOOL AmbientLightMxModel( MXLOADHEADER * DstMloadheader, int R, int G, int B, int A , float rp , float gp , float bp);
void HitMe( uint16 OwnerType, uint16 OwnerID, float Damage, uint8 WeaponType, uint8 Weapon );
BOOL ProcessModel2( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B );

uint16 CreateLine( float x1, float y1, float z1, float x2, float y2, float z2, uint16 Group );
BOOL GetMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					   VECTOR * BottomRight );
BOOL GetMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					    VECTOR * BottomRight );
BOOL CreateMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group );
BOOL CreateMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group );
BOOL ProcessSphereZoneModelExec( LPDIRECT3DEXECUTEBUFFER lpExBuf, int16 NumVerts, uint8 R, uint8 G, uint8 B );
void SetupModelSpotFX( uint16 i );
void CreateModelSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up,
							    uint16 Group, uint8 Weapon, uint16 ModelIndex, uint16 SpotFXIndex );
void UpdateShipModel( uint16 Ship );
void UpdateObjectAmbientColour( OBJECT * Object );
BOOL UpdateMxaModel( MXALOADHEADER * MXAloadheader );
uint32 EngineCode( uint16 OwnerType, uint16 OwnerID, uint32 SoundFX_ID, float Speed );
void KillAttachedSoundFX( uint16 i );
FILE * SaveModels( FILE * fp );
FILE * LoadModels( FILE * fp );
BOOL ReinitSpotFXSFX( uint16 i );
void SetShipsVisibleFlag( void );
void EnableRelavantModels( MODELNAME * ModelNamesPtr );
void KillAttachedSpotFX( uint16 i );
void KillAllBikeEngines( void );
BOOL EngineEnabled( uint16 OwnerType, uint16 Owner );

#endif // MODELS_INCLUDED


