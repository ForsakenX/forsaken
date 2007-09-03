/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 68 $
 *
 * $Header: /PcProjectX/Include/2dPolys.h 68    7/04/98 22:13 Collinsd $
 *
 * $Log: /PcProjectX/Include/2dPolys.h $
 * 
 * 68    7/04/98 22:13 Collinsd
 * Hack for chris's goals.  CWClear screen added.
 * 
 * 67    20/02/98 16:29 Collinsd
 * Added score zone display.
 * 
 * 66    16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 65    29/01/98 12:06 Collinsd
 * Added fireball, blue photon, tentacle and changed spotfx laser.
 * 
 * 64    28/01/98 9:04 Collinsd
 * Done more work on Afterburners.
 * 
 * 63    3/01/98 19:14 Collinsd
 * Added Splash for secondary weapons
 * 
 * 62    30/12/97 10:45 Collinsd
 * Added enemy damage modifier as variable.
 * 
 * 61    20/12/97 15:48 Collinsd
 * Restart points deactivated and startpoint loading added ollypad.
 * 
 * 60    12/12/97 17:43 Collinsd
 * Started Beard afterburner.
 * 
 * 59    9/12/97 10:45 Collinsd
 * Done ship burning and change body parts and pickups now explode in
 * direction of ship movement.
 * 
 * 58    6/12/97 19:22 Collinsd
 * Added burning effect on deathmode.
 * 
 * 57    25/11/97 12:56 Collinsd
 * Added Photon Missile
 * 
 * 56    18/11/97 11:50 Collinsd
 * Added Drip and Fixed Enemy damage calculation.
 * 
 * 55    17/11/97 19:09 Collinsd
 * Added enemy generation effect.  New Compobject format and relative flag
 * for fmpolys.
 * 
 * 54    15/11/97 15:20 Collinsd
 * Added Bubbles
 * 
 * 53    3/11/97 18:27 Collinsd
 * Added flames and shrapnel explosions.  ( Also effect Water ).
 * 
 * 52    29/10/97 9:26 Collinsd
 * Added new spotfx and pointdir stuff.
 * 
 * 51    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 50    16/10/97 17:56 Collinsd
 * Added new pointdir effect for models.
 * 
 * 49    10/10/97 12:12 Collinsd
 * Added flame and coloured sparks.
 * 
 * 48    9/10/97 11:33 Collinsd
 * Added new spotfx and enemies have ambient lights.
 * 
 * 47    8/10/97 16:32 Collinsd
 * Added spotfx and started lightning bolt.
 * 
 * 46    8/08/97 9:55 Collinsd
 * Change TPage info structure.
 * 
 * 45    7/08/97 21:25 Collinsd
 * Change FmPolys to use tpage link list.
 * 
 * 44    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 43    24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 42    17/05/97 16:01 Collinsd
 * Added PVR polys
 * 
 * 41    17-03-97 8:57a Collinsd
 * Poly's, Screen Polys, FmPolys now can create up to 1000 each.
 * 
 * 40    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 39    12/09/96 5:25p Collinsd
 * Started smoke when hull damaged ( still need positions )
 * 
 * 38    21/11/96 16:12 Collinsd
 * Added code to reduce packet loss for pickups.
 * Changed Trojax to be constant dir.
 * 
 * 37    19/11/96 11:06 Collinsd
 * Added new blood graphics.
 * 
 * 36    4/11/96 16:35 Collinsd
 * Fixed fmpolys/polys/lines exec lists.
 * 
 * 35    29/10/96 14:49 Collinsd
 * Added big explosion flash.
 * 
 * 34    24/10/96 15:16 Collinsd
 * No message if sfx, mine now collide immediatly if not owner
 * 
 * 33    24/10/96 12:50 Collinsd
 * Backface culling optimisation
 * 
 * 32    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 31    10/10/96 4:18p Phillipd
 * removed mesh.h..no longer used..
 * 
 * 30    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 29    7/10/96 13:52 Collinsd
 * Added new smoke trails
 * 
 * 28    1/10/96 17:43 Collinsd
 * Reduced pulsar to half.  Tidied up primary weapons.
 * deleted redundant code in 2dpolys/primary weapons.
 * 
 * 27    30/09/96 14:48 Collinsd
 * Added simplified missiles.
 * Fixed distance 2dpoly's moved out by...
 * 
 * 26    20/09/96 17:12 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 25    9/10/96 9:00a Phillipd
 * 
 * 24    30/08/96 17:30 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 23    16/08/96 15:18 Collinsd
 * Pickups now have bubble
 * 
 * 22    16/08/96 11:01 Collinsd
 * You now see explosions close to walls and when hit. Pickups and bullets
 * don't dissapear when changing mode.
 * 
 * 21    13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 20    2/08/96 9:28 Collinsd
 * Added double sided 2D polys, Static 2D polys.
 * Added transpulse cannon and pyrolite weapon.
 * Weapons should work correctly over network.
 * 
 * 19    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 18    25/07/96 17:44 Collinsd
 * Trojax weapon now has variable sizes. Primary weapon collision with
 * ships now creates the proper explosion
 * 
 * 17    25/07/96 10:08 Collinsd
 * Added variable speed to trojax weapon.
 * 
 * 16    24/07/96 12:59 Collinsd
 * Started on primary weapons.
 * 
 * 15    23/07/96 15:40 Collinsd
 * 
 * 14    22/07/96 17:46 Collinsd
 * 
 * 13    21/07/96 20:42 Collinsd
 * Added plasma sparks. ( Crap at the moment ! ).
 * 
 * 12    21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 11    19/07/96 16:56 Collinsd
 * Now have seperate explosion for Ship and Background.
 * Increased Max 2DPolys, and have framelag compensated fireing of the
 * primary and secondary weapons.
 * 
 * 10    19/07/96 12:36 Collinsd
 * New Explosions and better smoke trails.
 * 
 * 9     17/07/96 19:15 Collinsd
 * Fixed bug in missiles/ improved 2D sprites handling
 * 
 * 8     17/07/96 15:28 Collinsd
 * Missiles/2DPolys altered to work better
 * 
 * 7     10/07/96 17:10 Collinsd
 * Smoke trails added to missiles, explosions improved.
 * 
 * 6     8/07/96 10:54 Collinsd
 * Added rotations to FmPolys.
 * 
 * 5     6/07/96 20:49 Collinsd
 * FmPolys now caluate position of explosion constantly.
 * 
 * 4     4/07/96 16:09 Collinsd
 * 
 * 3     4/07/96 10:34 Collinsd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *
 *  File:       2dpolys.h
 *  Content:    2dpolys.h include file
 *
 *
 ***************************************************************************/

#ifndef TWODPOLYS_INCLUDED
#define TWODPOLYS_INCLUDED

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Includes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <math.h>
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include <time.h>
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "ddutil.h"
#include "lights.h"
#include "2dtextures.h"
#include "tload.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	EXPLO_DISTANCE		( 256.0F * GLOBAL_SCALE )
#define MAXNUMOF2DPOLYS		2000
#define	MAXVERTSPER2DPOLY	4
#define MaxColDistance		( 64000.0F * GLOBAL_SCALE )
#define	MAXFMPOLYVERTS		700

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Faceme Sequences
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	FM_ZERO					0  	   				// Do Nothing
#define	FM_ANIM					1  	   				// Animate then die (Wall Explo).
#define	FM_ANIM2				2  	   				// Animate then die (Ship Explo).
#define	FM_SMOKE_TRAIL			3  	   				// Move and animate X2 then die.
#define	FM_TROJAX_EXP			4  	   				// Trojax Explosion.
#define	FM_TROJAX				5  	   				// Trojax 1 Bullet
#define FM_PULSAR_EXP			6  	   				// Pulsar Explosion
#define FM_PULSAR				7  	   				// Pulsar
#define FM_SHRAPNEL_EXP			8  	   				// Shrapnel Explosion
#define	FM_FLAME				9  	   				// Flame Sequence
#define	FM_STATIC				10 	   				// Static Sequence
#define	FM_ARC					11 	   				// Arc Sequence
#define	FM_ARC2					12 	   				// Arc Glow Sequence
#define	FM_ARC_SPARK			13 	   				// Arc Spark Sequence
#define	FM_BITS					14 	   				// Bits Sequence
#define	FM_ARC_RING				15 	   				// Arc Explosion Ring
#define	FM_FLARE				16 	   				// Arc Explosion Flare
#define	FM_GRAVGON_TRAIL		17 	   				// Gravgon Smoke Trail
#define	FM_SOLARIS_TRAIL		18 	   				// Solaris Smoke Trail
#define	FM_NEW_TRAIL			19 	   				// New Smoke Trail
#define	FM_PICKUP_REGEN			20 	   				// Regenerating Pickup?
#define	FM_BLOOD				21 	   				// Blood?
#define	FM_BLOODSPLAT			22 	   				// Blood Splat?
#define	FM_BLOODDRIBBLE			23 	   				// Blood Dribble?
#define	FM_FLASH				24 	   				// Flash?
#define	FM_BLACK_SMOKE			25 	   				// Black Smoke?
#define	FM_NMEBULLET1			26 	   				// NME Bullet1?
#define	FM_MUZZLEFLASH			27 	   				// Muzzle Flash?
#define	FM_MUSHROOM				28 	   				// Mushroom Explosion?
#define	FM_SMOKE_RING			29 	   				// Smoke Ring?
#define	FM_SPOTFX_SMOKE			30 	   				// Smoke?
#define	FM_SPOTFX_FLAME			31 	   				// Flame?
#define	FM_SPOTFX_STEAM			32 	   				// Steam?
#define	FM_SPOTFX_SPARKS		33 	   				// Sparks?
#define	FM_SPOTFX_SPARKSFACEME	34					// Sparks FaceMe?
#define	FM_SPOTFX_GRAVGON_TRAIL	35					// Gravgon Trail?
#define	FM_SPOTFX_GLOW			36					// Glow?
#define	FM_NME_TRAIL			37					// NME Trail?
#define	FM_NME_VAPOUR_TRAIL		38					// NME Vapour Trail?
#define	FM_SPOTFX_REALSPARKS	39					// Real Sparks?
#define	FM_SPOTFX_SHRAPNEL_TRAIL 40					// Shrapnel Trail?
#define	FM_SPOTFX_BUBBLE		41					// Bubble?
#define	FM_NMEGEN_EFFECT		42					// Enemy Generation Effect?
#define	FM_NMEGEN_FLARE			43					// Enemy Generation Flare?
#define	FM_SPOTFX_DRIP			44					// Drip?
#define	FM_PHOTON				45					// Photon Torpedo?
#define	FM_SPOTFX_BURNING		46					// Burning?
#define	FM_ANIMSPEED			47					// Anim using animspeed
#define	FM_SPOTFX_BEARD_AFTERBURNER	48				// Beard Afterburner
#define	FM_RESTART_INACTIVE		49					// Restart Point Inactive
#define	FM_RESTART_ACTIVE		50					// Restart Point Active
#define	FM_SPOTFX_WATERSPLASH	51					// Water Splash
#define	FM_HK5_TRAIL			52					// Hk5 Trail
#define	FM_BLUEPHOTON			53					// Blue Photon Torpedo?
#define	FM_FIREBALL				54					// Fireball
#define	FM_SPARKLE				55					// Sparkle?
#define	FM_GOALSPARKLE			56					// Hacky goal Sparkle?
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Faceme Flags
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	FM_FLAG_ZERO		0							// Do Nothing
#define	FM_FLAG_MOVEOUT		1							// Move 2D Sprite toward Camera.
#define	FM_FLAG_DIRCONST	2							// Direction and Up Vector Constant.
#define	FM_FLAG_TWOSIDED	4							// Double Sided Poly
#define	FM_FLAG_DONTCLIP	8							// Don't Clip
#define	FM_FLAG_RELATIVE	16							// Direction and Up Vector Relative to camera.

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Make Colour Mode ( MakeColourMode Values )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
enum
{
	MCM_Normal,					// Actual Colours
	MCM_Stipple,				// Stipple Card Colours
	MCM_PowerVR,				// PowerVR Card Colours
	MCM_Software,				// Software Emulation Colours
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		TPage Info Structure for FmPolys/Polys/ScreenPolys
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct TPAGEINFO {

	uint16		FirstPoly;		// Index to first Poly in TPage
	int16		StartVert;		// Filled in by PolyDisp
	int16		NumVerts;		// Filled in by PolyDisp
	int16		NumTris;		// Filled in by PolyDisp

} TPAGEINFO;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	FmPoly Structure
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct FMPOLY{

	uint16			Next;
	uint16			Prev;
	uint16			NextInTPage;
	uint16			PrevInTPage;
	float			LifeCount;			// LifeCount
	VECTOR			Pos;				// Position
	uint8			Start_R;			// Start Colour
	uint8			Start_G;
	uint8			Start_B;
	uint8			R;					// Colour
	uint8			G;
	uint8			B;
	uint8			Trans;				// Amount of transparency
	uint8			OnceOnly;			// Once Only
	float			Rot;				// Rotation Angle
	float			RotSpeed;			// Rotation Speed
	float			Frame;				// Animation Frame
	float			AnimSpeed;			// Animation Speed
	int16			Flags;				// Display Flags
	FRAME_INFO	**	Frm_Info;			// Offset Info
	int16			SeqNum;				// Sequence Number
	VECTOR			Dir;				// Current Direction
	VECTOR			DirVector;			// Direction to face
	VECTOR			UpVector;			// Up Vector to face
	MATRIX			Mat;				// Matrix ( Trojax );
	uint16			Group;				// GroupImIn
	float			Speed;				// Speed
	float			UpSpeed;			// Speed
	float			xsize;				// in world space
	float			ysize;				// in world space
	
}FMPOLY;


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Protptypes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitFmPoly( void );
void KillUsedFmPoly( uint16 i );
uint16 FindFreeFmPoly();
void FmPolyProcess( void );
BOOL PVR_FmPolyDispGroup( uint16 Group, uint16 * Next );
void InitFmPolyTPages( void );
void AddFmPolyToTPage( uint16 i, int16 TPage );
void RemoveFmPolyFromTPage( uint16 i, int16 TPage );
BOOL DisplayGroupClippedFmPolys( LPDIRECT3DEXECUTEBUFFER ExecBuff, uint16 Group, LPDIRECT3DDEVICE D3D_Device, LPDIRECT3DVIEWPORT D3D_ViewPort );
BOOL DisplayGroupUnclippedFmPolys( LPDIRECT3DEXECUTEBUFFER ExecBuff, LPDIRECT3DDEVICE D3D_Device, LPDIRECT3DVIEWPORT D3D_ViewPort );
BOOL FmPolyDispGroupClipped( uint16 Group, LPDIRECT3DEXECUTEBUFFER ExecBuffer, int16 * TPage, uint16 * NextFmPoly );
BOOL FmPolyDispGroupUnclipped( LPDIRECT3DEXECUTEBUFFER ExecBuffer, int16 * TPage, uint16 * NextFmPoly );
FILE * SaveFmPolys( FILE * fp );
FILE * LoadFmPolys( FILE * fp );

#endif // TWODPOLYS_INCLUDED


