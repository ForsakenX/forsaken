/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 124 $
 *
 * $Header: /PcProjectX/2dPolys.c 124   7/04/98 22:13 Collinsd $
 *
 * $Log: /PcProjectX/2dPolys.c $
 * 
 * 124   7/04/98 22:13 Collinsd
 * Hack for chris's goals.  CWClear screen added.
 * 
 * 123   3/04/98 11:39 Collinsd
 * Tidy
 * 
 * 122   30/03/98 10:20 Collinsd
 * SOFTWARE_ENABLE # fixed.
 * 
 * 121   28/03/98 15:25 Collinsd
 * SoftwareVersion Transparency fixed.
 * 
 * 120   18/03/98 14:06 Collinsd
 * Bubbles now work through groups.
 * 
 * 119   23/02/98 21:20 Collinsd
 * Optimised Load/Save Functions.
 * 
 * 118   20/02/98 16:29 Collinsd
 * Added score zone display.
 * 
 * 117   16/02/98 14:33 Collinsd
 * Started LoadSave
 * 
 * 116   2/02/98 10:06 Collinsd
 * Reduced poly's on splash and splash does not effect water.
 * 
 * 115   30/01/98 9:12 Collinsd
 * Added Laser for exogenon and added corruption checking for componented
 * objects.
 * 
 * 114   29/01/98 12:06 Collinsd
 * Added fireball, blue photon, tentacle and changed spotfx laser.
 * 
 * 113   28/01/98 11:05 Collinsd
 * Decreased Drip Effect
 * 
 * 112   28/01/98 9:04 Collinsd
 * Done more work on Afterburners.
 * 
 * 111   20/01/98 16:34 Collinsd
 * Added SpotFX to bikes.
 * 
 * 110   5/01/98 10:39 Collinsd
 * Water Splash is now tinted by water colour.
 * 
 * 109   3/01/98 19:14 Collinsd
 * Added Splash for secondary weapons
 * 
 * 108   20/12/97 17:45 Collinsd
 * Added new restart point.
 * 
 * 107   20/12/97 15:48 Collinsd
 * Restart points deactivated and startpoint loading added ollypad.
 * 
 * 106   19/12/97 14:16 Collinsd
 * 
 * 105   13/12/97 15:06 Collinsd
 * Added light to explosion and slowed down the light on sparks.
 * 
 * 104   12/12/97 17:43 Collinsd
 * Started Beard afterburner.
 * 
 * 103   9/12/97 10:45 Collinsd
 * Done ship burning and change body parts and pickups now explode in
 * direction of ship movement.
 * 
 * 102   6/12/97 19:22 Collinsd
 * Added burning effect on deathmode.
 * 
 * 101   1/12/97 15:05 Collinsd
 * Framelagged new transpulse/trojax explosion
 * 
 * 100   25/11/97 12:56 Collinsd
 * Added Photon Missile
 * 
 * 99    18/11/97 11:50 Collinsd
 * Added Drip and Fixed Enemy damage calculation.
 * 
 * 98    17/11/97 19:09 Collinsd
 * Added enemy generation effect.  New Compobject format and relative flag
 * for fmpolys.
 * 
 * 97    15/11/97 16:57 Collinsd
 * Secondary weapons now effect water, and bubbles hitting surface effect
 * water.
 * 
 * 96    15/11/97 15:20 Collinsd
 * Added Bubbles
 * 
 * 95    10/11/97 11:12 Collinsd
 * Added new graphjcs ( transpulse, trojax .... )
 * 
 * 94    3/11/97 18:27 Collinsd
 * Added flames and shrapnel explosions.  ( Also effect Water ).
 * 
 * 93    29/10/97 9:26 Collinsd
 * Added new spotfx and pointdir stuff.
 * 
 * 92    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 91    16/10/97 17:55 Collinsd
 * Added new pointdir effect for models.
 * 
 * 90    10/10/97 12:12 Collinsd
 * Added flame and coloured sparks.
 * 
 * 89    9/10/97 11:33 Collinsd
 * Added new spotfx and enemies have ambient lights.
 * 
 * 88    8/10/97 16:32 Collinsd
 * Added spotfx and started lightning bolt.
 * 
 * 87    16/09/97 10:59 Collinsd
 * Added Chris's code
 * 
 * 86    8/08/97 9:55 Collinsd
 * Change TPage info structure.
 * 
 * 85    7/08/97 21:25 Collinsd
 * Change FmPolys to use tpage link list.
 * 
 * 84    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 83    24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 82    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 81    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 80    5/07/97 16:31 Collinsd
 * Put OPT_ON's around opimisations off
 * 
 * 79    6/24/97 11:10a Phillipd
 * Object Stuff is added.....
 * 
 * 78    18/06/97 17:00 Collinsd
 * Added PowerVR Trans
 * 
 * 77    6/12/97 2:27p Phillipd
 * bPolySort added as opposed to just basing it on PowerVR
 * 
 * 76    10/06/97 14:27 Collinsd
 * Added ACTUAL_TRANS to use transluecency values.
 * 
 * 75    5/06/97 15:48 Collinsd
 * Added edge enable flags
 * 
 * 74    17/05/97 20:15 Collinsd
 * Add PowerVR code for polys and screenpolys as well.......
 * 
 * 73    5/17/97 6:14p Phillipd
 * 
 * 72    17/05/97 16:01 Collinsd
 * Added PVR polys
 * 
 * 71    26/04/97 14:49 Collinsd
 * Optimisations now on def.
 * 
 * 70    17-03-97 8:57a Collinsd
 * Poly's, Screen Polys, FmPolys now can create up to 1000 each.
 * 
 * 69    12-03-97 4:47p Collinsd
 * Added recoil and new shrapnel model.
 * 
 * 68    27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 67    25-02-97 12:05p Collinsd
 * Framelaged pyrolite rifle.
 * 
 * 66    25-02-97 10:20a Collinsd
 * Corrected framelag on various bits.
 * 
 * 65    2/03/97 5:16p Phillipd
 * Translusceny is now controlled by global execute buffers.... which is
 * much better...
 * 
 * 64    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 63    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 62    12/17/96 12:56p Collinsd
 * Added team missile stuff. and fixed translucency on 3DFX.
 * 
 * 61    12/09/96 5:25p Collinsd
 * Started smoke when hull damaged ( still need positions )
 * 
 * 60    21/11/96 16:12 Collinsd
 * Added code to reduce packet loss for pickups.
 * Changed Trojax to be constant dir.
 * 
 * 59    19/11/96 11:06 Collinsd
 * Added new blood graphics.
 * 
 * 58    4/11/96 16:35 Collinsd
 * Fixed fmpolys/polys/lines exec lists.
 * 
 * 57    3/11/96 16:40 Collinsd
 * fixed groups visible.
 * 
 * 56    29/10/96 14:49 Collinsd
 * Added big explosion flash.
 * 
 * 55    24/10/96 15:16 Collinsd
 * No message if sfx, mine now collide immediatly if not owner
 * 
 * 54    24/10/96 12:50 Collinsd
 * Backface culling optimisation
 * 
 * 53    23/10/96 19:12 Collinsd
 * Execute buffer for fmpolys/polys now 64k
 * 
 * 52    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 51    17/10/96 12:40 Collinsd
 * Added blend types to all transparent exec lists.
 * 
 * 50    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 49    9/10/96 10:02 Collinsd
 * Old Sfx back, new gravgon trail.
 * 
 * 48    7/10/96 13:52 Collinsd
 * Added new smoke trails
 * 
 * 47    5/10/96 21:34 Collinsd
 * Added drop pickup mode. speeded up pickups even more now 0.9% of
 * processing
 * 
 * 46    10/05/96 2:02p Phillipd
 * 
 * 45    4/10/96 10:47 Collinsd
 * Added visible group pickup as well as using proper ray for movement of
 * ship picking up.
 * 
 * 44    1/10/96 17:43 Collinsd
 * Reduced pulsar to half.  Tidied up primary weapons.
 * deleted redundant code in 2dpolys/primary weapons.
 * 
 * 43    20/09/96 17:12 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 42    17/09/96 13:58 Collinsd
 * Added autoselection of primary and secondary weapons.
 * Also you can pickup anything if F12 enabled.
 * 
 * 41    15/09/96 14:48 Collinsd
 * Scaled primary weapons, changed defauls ammo levels.
 * 
 * 40    13/09/96 12:59 Collinsd
 * Added shield effect to external views.
 * 
 * 39    11/09/96 19:00 Collinsd
 * Added tests for RAMP emulation ( Although may not work)
 * Also fixed gravity effect to be dependent on GLOBAL_SCALE
 * 
 * 38    11/09/96 16:17 Collinsd
 * now use destalpha
 * 
 * 37    11/09/96 14:19 Collinsd
 * Added global scale to hopefully everything that needs it.
 * 
 * 36    11/09/96 10:31 Collinsd
 * Now use global scale for polys.
 * 
 * 35    9/10/96 9:00a Phillipd
 * 
 * 34    30/08/96 17:30 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 33    8/21/96 5:49p Phillipd
 * 
 * 32    21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 31    16/08/96 15:18 Collinsd
 * Pickups now have bubble
 * 
 * 30    16/08/96 11:01 Collinsd
 * You now see explosions close to walls and when hit. Pickups and bullets
 * don't dissapear when changing mode.
 * 
 * 29    13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 28    8/08/96 9:13 Collinsd
 * Added Sfx and pickups
 * 
 * 27    4/08/96 17:04 Collinsd
 * All  primary weapons are now in. ( no power ups )
 * 
 * 26    2/08/96 15:28 Collinsd
 * Added pickups
 * 
 * 25    2/08/96 9:28 Collinsd
 * Added double sided 2D polys, Static 2D polys.
 * Added transpulse cannon and pyrolite weapon.
 * Weapons should work correctly over network.
 * 
 * 24    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 23    29/07/96 16:43 Collinsd
 * Fixed bug in missile hit & changed transpulse.
 * 
 * 22    25/07/96 17:44 Collinsd
 * Trojax weapon now has variable sizes. Primary weapon collision with
 * ships now creates the proper explosion
 * 
 * 21    25/07/96 10:08 Collinsd
 * Added variable speed to trojax weapon.
 * 
 * 20    24/07/96 12:59 Collinsd
 * Started on primary weapons.
 * 
 * 19    23/07/96 15:26 Collinsd
 * Added new offset file loading/format.
 * 
 * 18    21/07/96 20:42 Collinsd
 * Added plasma sparks. ( Crap at the moment ! ).
 * 
 * 17    21/07/96 12:52 Collinsd
 * Now all works with Dan's new tloader
 * 
 * 16    21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 15    19/07/96 16:56 Collinsd
 * 
 * 14    19/07/96 12:36 Collinsd
 * New Explosions and better smoke trails.
 * 
 * 13    17/07/96 15:28 Collinsd
 * Missiles/2DPolys altered to work better
 * 
 * 12    15/07/96 14:30 Collinsd
 * Transparency & Translucency work (With hardware)
 * 
 * 11    11/07/96 12:50 Collinsd
 * Drop missiles started
 * 
 * 10    10/07/96 17:10 Collinsd
 * Smoke trails added to missiles, explosions improved.
 * 
 * 9     8/07/96 10:54 Collinsd
 * Added rotations to FmPolys.
 * 
 * 8     6/07/96 22:25 Collinsd
 * Added pulse to 2dPoly's.
 * 
 * 7     6/07/96 20:49 Collinsd
 * FmPolys now caluate position of explosion constantly.
 * 
 * 6     6/07/96 16:36 Collinsd
 * Explosion moved away from wall, toward camera.
 * 
 * 5     5/07/96 17:59 Collinsd
 * Faceme poly's now use boxtoppm data
 * 
 * 4     4/07/96 16:09 Collinsd
 * 
 * 3     6/27/96 12:23p Phillipd
 * removed d3dvertex types from routine prototypes
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*	2 d p o l y s . c
*	All routines to do with 2d always face you polygons...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include	<stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "2dpolys.h"
#include "camera.h"

#include "Primary.h"
#include "polysort.h"
#include "spotfx.h"
#include "water.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

#define	FMPOLYSCALE	32.0F

#ifdef SOFTWARE_ENABLE
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Chris's Code
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	CHRISFLAG	8192
void	CWExecute2(	LPDIRECT3DDEVICE lpDev,
					LPDIRECT3DEXECUTEBUFFER execbuf,
					LPDIRECT3DVIEWPORT lpView,
					WORD cwFlags);
extern	BOOL	SoftwareVersion;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Externs
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	CAMERA	CurrentCamera;
extern	D3DMATRIX			view;
extern	MATRIX				Identity;
extern	MATRIX				ProjMatrix;
extern	VECTOR				Camera_Dir;
extern	VECTOR				Forward;
extern	VECTOR				SlideUp;
extern	VECTOR				SlideLeft;
extern	D3DMATERIALHANDLE	hMat2;
extern	D3DTEXTUREHANDLE	bTex;
extern	BYTE				WhoIAm;
extern	TLOADHEADER			Tloadheader;
extern	FRAME_INFO	*		Troj_Exp_Header;
extern	FRAME_INFO	*		Shrapnel_Header;
extern	float				framelag;
extern	BOOL				UsedStippledAlpha;
extern	MATRIX				MATRIX_Identity;
extern	DWORD				CurrentSrcBlend;
extern	DWORD				CurrentDestBlend;
extern	DWORD				CurrentTextureBlend;
extern	BOOL				CanCullFlag;
extern	BOOL				PowerVR;
extern	BOOL				bPolySort;
extern	MLOADHEADER			Mloadheader;
extern	MCLOADHEADER		MCloadheader;
extern	MCLOADHEADER		MCloadheadert0;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Globals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FMPOLY		FmPolys[ MAXNUMOF2DPOLYS ];
uint16		FirstFmPolyUsed;
uint16		FirstFmPolyFree;
uint32		TotalFmPolysInUse = 0;
int16		MakeColourMode = MCM_Normal;
TPAGEINFO	FmPolyTPages[ MAXTPAGESPERTLOAD + 1 ];

void DebugPrintf( const char * format, ... );
void FadeColour( uint8 * Colour, uint8 WantedColour, float Speed );

#define RGBA_MAKE2(r, g, b, a)   ((D3DCOLOR) (( (DWORD) ((a) & 0xff) << 24) | ( (DWORD) ((r) & 0xff) << 16) | ( (DWORD) ((g) & 0xff) << 8) | (DWORD) ((b) & 0xff)))

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init Faceme poly structures
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitFmPoly( void )
{
	int i;

	FirstFmPolyUsed = (uint16) -1;
	FirstFmPolyFree = 0;
	
	for( i=0;i<MAXNUMOF2DPOLYS;i++)
	{
		memset( &FmPolys[i], 0, sizeof( FMPOLY ) );

		FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[i].R = 255;
		FmPolys[i].G = 255;
		FmPolys[i].B = 255;
#if ACTUAL_TRANS
		FmPolys[i].Trans = 128;
#else
		FmPolys[i].Trans = 255;
#endif
		FmPolys[i].DirVector.x = 0.0F;
		FmPolys[i].DirVector.y = 0.0F;
		FmPolys[i].DirVector.z = 1.0F;
		FmPolys[i].UpVector.x = 0.0F;
		FmPolys[i].UpVector.y = 1.0F;
		FmPolys[i].UpVector.z = 0.0F;

		FmPolys[i].NextInTPage = (uint16) -1;
		FmPolys[i].PrevInTPage = (uint16) -1;

		FmPolys[i].Next = i + 1;
		FmPolys[i].Prev = (uint16) -1;
	}
	FmPolys[MAXNUMOF2DPOLYS-1].Next = (uint16) -1;

	InitFmPolyTPages();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find a free FmPoly and move it from the free list to
					the used list
	Input		:	Nothing
	Output		:	uint16	Number of the free FmPoly
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 FindFreeFmPoly( void )
{
	uint16 i;

	i = FirstFmPolyFree;
	if( i == (uint16) -1 ) return i;
 
	FmPolys[i].Prev = FirstFmPolyUsed;

	if ( FirstFmPolyUsed != (uint16) -1)
	{
		FmPolys[FirstFmPolyUsed].Next = i;
	}

	FirstFmPolyUsed = i;
	FirstFmPolyFree = FmPolys[i].Next;

	TotalFmPolysInUse++;

	return i ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill a used FmPoly and move it from the used list to
					the free list
	Input		:	uint16		Number of FmPoly to free....
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void KillUsedFmPoly( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;
	
	its_prev = FmPolys[i].Prev;
	its_next = FmPolys[i].Next;

	if ( i == FirstFmPolyUsed )
		FirstFmPolyUsed = FmPolys[i].Prev;

	if( its_prev != (uint16) -1)
		FmPolys[its_prev].Next = its_next;

	if( its_next != (uint16) -1)
		FmPolys[its_next].Prev = its_prev;

	TotalFmPolysInUse--;

	RemoveFmPolyFromTPage( i, GetTPage( *FmPolys[i].Frm_Info, 0 ) );

	FmPolys[i].Prev = (uint16) -1;
	FmPolys[i].Next = FirstFmPolyFree;
	FmPolys[i].LifeCount = 0.0F;
	FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
	FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
	FmPolys[i].Rot = 0.0F;
	FmPolys[i].RotSpeed = 0.0F;
	FmPolys[i].R = 255;
	FmPolys[i].G = 255;
	FmPolys[i].B = 255;
	FmPolys[i].Trans = 255;
	FmPolys[i].Flags = 0;
	FmPolys[i].Frame = 0.0F;
	FmPolys[i].SeqNum = 0;
	FmPolys[i].Dir.x = 0.0F;
	FmPolys[i].Dir.y = 0.0F;
	FmPolys[i].Dir.z = 0.0F;
   	FmPolys[i].DirVector.x = 0.0F;
   	FmPolys[i].DirVector.y = 0.0F;
   	FmPolys[i].DirVector.z = 1.0F;
   	FmPolys[i].UpVector.x = 0.0F;
   	FmPolys[i].UpVector.y = 1.0F;
   	FmPolys[i].UpVector.z = 0.0F;
	FmPolys[i].Speed = 0.0F;
	FmPolys[i].Frm_Info = NULL;
	FirstFmPolyFree	= i;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process FmPoly's
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#if 0

#define	NMEGEN_START		( 5.0F * ANIM_SECOND )
#define	NMEGEN_STAGE1		( 2.0F * ANIM_SECOND )
#define	NMEGEN_STAGE2		( 1.0F * ANIM_SECOND )
#define	NMEGEN_END			( 0.0F * ANIM_SECOND )
#define	NMEGEN_STAGE0_LEN	( NMEGEN_START - NMEGEN_STAGE1 )
#define	NMEGEN_STAGE1_LEN	( NMEGEN_STAGE1 - NMEGEN_STAGE2 )
#define	NMEGEN_STAGE2_LEN	( NMEGEN_STAGE2 - NMEGEN_END )

#else

#define	NMEGEN_START		( 2.0F * ANIM_SECOND )
#define	NMEGEN_STAGE1		( 0.8F * ANIM_SECOND )
#define	NMEGEN_STAGE2		( 0.4F * ANIM_SECOND )
#define	NMEGEN_END			( 0.0F * ANIM_SECOND )
#define	NMEGEN_STAGE0_LEN	( NMEGEN_START - NMEGEN_STAGE1 )
#define	NMEGEN_STAGE1_LEN	( NMEGEN_STAGE1 - NMEGEN_STAGE2 )
#define	NMEGEN_STAGE2_LEN	( NMEGEN_STAGE2 - NMEGEN_END )

#endif

static	int16	GlobCount;
static	uint8	ShadeTable[ 16 ] = { 255, 200, 128, 104, 96, 92, 80, 77, 74, 72, 69, 66, 64, 64, 64, 64 };
static	uint8	SmokeRingShadeTable[ 5 ] = { 255, 200, 150, 100, 50 };

void FmPolyProcess( void )
{
	uint16	i;
	uint16	nextprim;
	float	Speed;
	float	R, G, B, T, FadeDec;
	VECTOR	Rotation;
	float	OldSize;
	float	Damage;
	VECTOR	TempDir, TempVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	MATRIX	UpMatrix;
	MATRIX	InvUpMatrix;
	QUAT	TempQuat;
	VECTOR	OldPos;

	GlobCount += (int16) framelag;								// Use Timer!!!

	i = FirstFmPolyUsed;

	while( i != (uint16) -1 )
	{
		nextprim = FmPolys[i].Prev;

		FmPolys[i].LifeCount -= framelag;
		if( FmPolys[i].LifeCount < 0.0F ) FmPolys[i].LifeCount = 0.0F;

		switch( FmPolys[i].SeqNum )
		{
		   	case FM_ANIMSPEED:							// Anim using animspeed
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_ANIM:								// Explosion?
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );
		   		FmPolys[i].Frame += ( framelag / 3.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;
		
		   	case FM_ANIM2:								// Ship Explosion?
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );
		   		FmPolys[i].Frame += ( framelag / 4.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;
		
		   	case FM_SMOKE_TRAIL:						// Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );
		
		   		FmPolys[i].R = ShadeTable[ (int16) FmPolys[i].Frame ];
		   		FmPolys[i].G = ShadeTable[ (int16) FmPolys[i].Frame ];
		   		FmPolys[i].B = ShadeTable[ (int16) FmPolys[i].Frame ];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 4.0F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}
		
		   		FmPolys[i].Frame += ( framelag / 2.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;
		
		   	case FM_TROJAX_EXP:								// Explosion?
				FadeDec = ( 8.0F * framelag );
				R = ( ( (float) FmPolys[i].R ) - FadeDec );
				G = ( ( (float) FmPolys[i].G ) - FadeDec );
				B = ( ( (float) FmPolys[i].B ) - FadeDec );
				T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
				if( R < 0.0F ) R = 0.0F;
				if( G < 0.0F ) G = 0.0F;
				if( B < 0.0F ) B = 0.0F;
				if( T < 0.0F ) T = 0.0F;
				FmPolys[i].R = (uint8) R;
				FmPolys[i].G = (uint8) G;
				FmPolys[i].B = (uint8) B;
				FmPolys[i].Trans = (uint8) T;

	   			FmPolys[i].xsize += ( TROJAX_EXPINC * framelag );
	   			FmPolys[i].ysize += ( TROJAX_EXPINC * framelag );
	   			if( FmPolys[i].xsize >= TROJAX_EXPMAXSIZE )
	   			{
	   				KillUsedFmPoly( i );
	   			}
		   		break;
		
		   	case FM_TROJAX:								// Trojax bullet
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		if( FmPolys[ i ].Rot > 360.0F ) FmPolys[ i ].Rot -= 360.0F;
		   		else if( FmPolys[ i ].Rot < 0.0F ) FmPolys[ i ].Rot += 360.0F;
				Rotation.x = (float) sin( D2R( FmPolys[ i ].Rot ) );
				Rotation.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
				Rotation.z = 0.0F;
				ApplyMatrix( &FmPolys[ i ].Mat, &Rotation, &FmPolys[ i ].UpVector );
		   		break;
		
		   	case FM_PULSAR_EXP:							// Pulsar Explosion?
		   		FmPolys[i].Frame += ( framelag / 4.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;
		
		   	case FM_PULSAR:
		   		break;

			case FM_NMEBULLET1:
		   		FmPolys[i].Frame += ( framelag / 3.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			FmPolys[i].Frame -= ( ( FmPolys[i].Frame / (*FmPolys[i].Frm_Info)->Num_Frames )
											* (*FmPolys[i].Frm_Info)->Num_Frames );
		   		}
		   		break;
		 				
		   	case FM_SHRAPNEL_EXP:						// Shrapnel Explosion?
				if( FmPolys[i].LifeCount < 995.0F )
				{
					if( FmPolys[i].Frm_Info != &Shrapnel_Header )
					{
						RemoveFmPolyFromTPage( i, GetTPage( *FmPolys[i].Frm_Info, (int16) FmPolys[i].Frame ) );
				   		FmPolys[i].Frm_Info = &Shrapnel_Header;
						AddFmPolyToTPage( i, GetTPage( *FmPolys[i].Frm_Info, 0 ) );
						FmPolys[i].Frame = 0.0F;
						FmPolys[i].xsize = SUSSGUN_EXPSIZE;
						FmPolys[i].ysize = SUSSGUN_EXPSIZE;
						FmPolys[i].R = 128;
						FmPolys[i].G = 128;
						FmPolys[i].B = 128;
					}
					else
					{
				   		FmPolys[i].Frame += framelag;
				   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
				   		{
				   			KillUsedFmPoly( i );
				   		}
					}
				}
		   		break;
		
		   	case FM_FLAME:								// Flame Thrower?
				FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		if( FmPolys[ i ].Rot > 360.0F ) FmPolys[ i ].Rot -= 360.0F;
		   		else if( FmPolys[ i ].Rot < 0.0F ) FmPolys[ i ].Rot += 360.0F;
				
				FmPolys[i].Frame += framelag;
		   		if( FmPolys[i].Frame > 12.0F ) FmPolys[i].Frame = 12.0F;
		   		break;
	
		   	case FM_STATIC:								// Static Obj Don't move/rotate
		   		break;

		   	case FM_ARC:								// Arc?
		   		break;

		   	case FM_ARC2:								// Arc Glow?
		   		break;

		   	case FM_ARC_SPARK:							// Arc Sparks?
		   		FmPolys[i].Frame += ( framelag / 5.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
		   		else
		   		{
			   		if( FmPolys[i].Speed > 0.0F )
			   		{
			   			FmPolys[i].Speed -= 0.5F;
			   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
			   		}

					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		   		}
		   		break;

		   	case FM_BITS:								// Bits?
		   		FmPolys[i].Frame += ( framelag / 6.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

		   	case FM_ARC_RING:							// Arc Explosion Ring?
				FadeDec = ( 8.0F * framelag );
				R = ( ( (float) FmPolys[i].R ) - FadeDec );
				G = ( ( (float) FmPolys[i].G ) - FadeDec );
				B = ( ( (float) FmPolys[i].B ) - FadeDec );
				T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
				if( R < 0.0F ) R = 0.0F;
				if( G < 0.0F ) G = 0.0F;
				if( B < 0.0F ) B = 0.0F;
				if( T < 0.0F ) T = 0.0F;
				FmPolys[i].R = (uint8) R;
				FmPolys[i].G = (uint8) G;
				FmPolys[i].B = (uint8) B;
				FmPolys[i].Trans = (uint8) T;

	   			FmPolys[i].xsize += ( TRANSPULSE_EXP_INC * framelag );
	   			FmPolys[i].ysize += ( TRANSPULSE_EXP_INC * framelag );
	   			if( FmPolys[i].xsize >= TRANSPULSE_EXP_MAXSIZE )
	   			{
	   				KillUsedFmPoly( i );
	   			}
		   		break;

		   	case FM_FLARE:								// Arc Explosion Flare?
				if( FmPolys[i].LifeCount == 0.0F )
				{
	   				KillUsedFmPoly( i );
				}
				break;

		   	case FM_GRAVGON_TRAIL:						// Gravgon Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( ( FmPolys[i].Frame * 5.0F ) * 8.0F ) ) );
		
		   		FmPolys[i].R = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
				FmPolys[i].xsize += ( GRAVGON_TRAIL_INC * framelag );
				FmPolys[i].ysize += ( GRAVGON_TRAIL_INC * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}
		
		   		FmPolys[i].Frame += ( 0.1F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_SOLARIS_TRAIL:						// Solaris Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( ( FmPolys[i].Frame * 3.0F ) * 8.0F ) ) );
		
		   		FmPolys[i].R = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.166F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_NEW_TRAIL:								// New Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( ( FmPolys[i].Frame * 3.0F ) * 8.0F ) ) );
		
		   		FmPolys[i].R = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16) ( FmPolys[i].Frame * 3.0F )];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}
		
		   		FmPolys[i].Frame += ( 0.166F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_PICKUP_REGEN:							// Regenerating Pickup?
		   		FmPolys[i].Frame += ( framelag / 6.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_BLOOD:									// Blood?
		   		FmPolys[i].Frame += ( framelag / 8.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

		   	case FM_BLOODSPLAT:								// Blood Splat?
		   		FmPolys[i].Frame += ( framelag / 8.0F ); //6.0F;
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FadeDec = ( 255.0F / ( (*FmPolys[i].Frm_Info)->Num_Frames * 8.0F ) ) * framelag;
					R = ( ( (float) FmPolys[i].R ) - FadeDec );
					G = ( ( (float) FmPolys[i].G ) - FadeDec );
					B = ( ( (float) FmPolys[i].B ) - FadeDec );
					T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
					if( R < 0.0F ) R = 0.0F;
					if( G < 0.0F ) G = 0.0F;
					if( B < 0.0F ) B = 0.0F;
					if( T < 0.0F ) T = 0.0F;
 					FmPolys[i].R = (uint8) R;
					FmPolys[i].G = (uint8) G;
					FmPolys[i].B = (uint8) B;
					FmPolys[i].Trans = (uint8) T;
				}
		   		break;

		   	case FM_BLOODDRIBBLE:							// Blood Splat?
		   		FmPolys[i].Frame += ( framelag / 8.0F ); //6.0F;
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
		   		break;

			case FM_FLASH:								// Flash?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( 0.1F * framelag );
					if( FmPolys[i].xsize < 0.0F ) FmPolys[i].xsize = 0.0F;
					FmPolys[i].ysize -= ( 0.1F * framelag );
					if( FmPolys[i].ysize < 0.0F ) FmPolys[i].ysize = 0.0F;
				}
				break;

			case FM_MUZZLEFLASH:						// Muzzle Flash?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				break;


		   	case FM_BLACK_SMOKE:						// Black Smoke?
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );
		
				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}
		
		   		FmPolys[i].Frame += ( framelag / 2.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_MUSHROOM:								// Mushroom Explosion?
		   		FmPolys[i].Frame += ( framelag / 6.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_SMOKE_RING:								// Smoke Ring?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = (uint8) 255;
		   		FmPolys[i].R = SmokeRingShadeTable[ (int16) FmPolys[i].Frame ];
		   		FmPolys[i].G = SmokeRingShadeTable[ (int16) FmPolys[i].Frame ];
		   		FmPolys[i].B = SmokeRingShadeTable[ (int16) FmPolys[i].Frame ];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_SPOTFX_SMOKE:							// Smoke?
		   		FmPolys[i].Trans = ( 128 - ( (int16) ( FmPolys[i].Frame * 25.0F ) ) );

				Speed = (float) ( 64.0F - ( (float) ( FmPolys[i].Frame * 12.0F ) ) );
				FmPolys[i].R = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
				FmPolys[i].xsize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.01F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 4.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.075F;
		   			if( FmPolys[i].UpSpeed > 4.0F ) FmPolys[i].UpSpeed = 4.0F;
		   		}
		
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_SPOTFX_STEAM:							// STEAM?
		   		FmPolys[i].Trans = (uint8) 255;

				Speed = SmokeRingShadeTable[ (int16) FmPolys[i].Frame ];
				FmPolys[i].R = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				FmPolys[i].xsize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}
		
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_SPOTFX_FLAME:							// Flame?
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( OldSize >= ( 3.5F * GLOBAL_SCALE ) ) && ( FmPolys[i].xsize < ( 3.5F * GLOBAL_SCALE ) ) )
				{
					CreateSpotFXFlameSmoke( &FmPolys[i].Pos, &FmPolys[i].Dir, FmPolys[i].Group );
				}

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= 0.2F;
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += 0.2F;
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}
			
		   			FmPolys[i].Frame += ( framelag / 2.0F );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );
			
		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_SPARKS:							// Sparks?
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

			case FM_SPOTFX_SPARKSFACEME:
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_GRAVGON_TRAIL:						// Gravgon Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( ( FmPolys[i].Frame * 5.0F ) * 8.0F ) ) );
		
		   		FmPolys[i].R = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16) ( FmPolys[i].Frame * 5.0F )];
		
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
				FmPolys[i].xsize += ( GRAVGON_TRAIL_INC * framelag );
				FmPolys[i].ysize += ( GRAVGON_TRAIL_INC * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed += ( 0.1F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.1F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_SPOTFX_GLOW:							// SpotFX Glow?
				if( FmPolys[i].OnceOnly )
				{
					FmPolys[i].OnceOnly--;
				}
				else
				{
		   			KillUsedFmPoly( i );
				}
				break;

		   	case FM_NME_TRAIL:								// Nme Trail?
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( ( 0.175F * GLOBAL_SCALE ) * framelag );
					FmPolys[i].ysize -= ( ( 0.175F * GLOBAL_SCALE ) * framelag );

					Speed = ( FmPolys[i].Speed * framelag );
					FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
			
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 2.0F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}
				}
		   		break;

			case FM_NME_VAPOUR_TRAIL:						// Vapour Trail?
		   		FmPolys[i].Trans = ( 128 - ( (int16) ( FmPolys[i].Frame * 25.0F ) ) );

				Speed = (float) ( 64.0F - ( (float) ( FmPolys[i].Frame * 12.0F ) ) );
				FmPolys[i].R = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
				FmPolys[i].xsize += ( ( 0.05F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.05F * GLOBAL_SCALE ) * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.01F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;

		   	case FM_SPOTFX_SHRAPNEL_TRAIL:						// Shrapnel Trail?
		   		FmPolys[i].Trans = (uint8) 255;

				Speed = SmokeRingShadeTable[ (int16) FmPolys[i].Frame ];
				FmPolys[i].R = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (uint8) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				FmPolys[i].xsize += ( ( 0.25F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.25F * GLOBAL_SCALE ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		
		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}
		
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		
		   		}
		   		break;


		   	case FM_SPOTFX_REALSPARKS:						// Real Sparks?
		   		FmPolys[i].Frame += ( framelag * 0.1F );
				FmPolys[i].Frame = FMOD( FmPolys[i].Frame, (*FmPolys[i].Frm_Info)->Num_Frames );

				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ FmPolys[i].Group ].up, &SlideUp );
					QuatToMatrix( &TempQuat, &UpMatrix );
					MatrixTranspose( &UpMatrix, &InvUpMatrix );

	     			TempDir.x = ( FmPolys[i].Dir.x * framelag );
	     			TempDir.y = ( FmPolys[i].Dir.y * framelag );
  	     			TempDir.z = ( FmPolys[i].Dir.z * framelag );
		  
					if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &FmPolys[i].Pos,
	     								  FmPolys[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     								  &FmPolys[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     			{
						FmPolys[i].Pos = Int_Point;

	     				TempVector = FmPolys[i].Dir;
	     				NormaliseVector( &TempVector );
	     				Speed = VectorLength( &FmPolys[i].Dir );
	     				ReflectVector( &TempVector, &Int_Normal, &FmPolys[i].Dir );
	     				Speed *= 0.5F;
		  
						if( Speed < ( 5.0F * GLOBAL_SCALE ) )
	     				{
		  					KillUsedFmPoly( i );
	     					break;
	     				}

						FmPolys[i].Dir.x *= Speed;
	     				FmPolys[i].Dir.y *= Speed;
	     				FmPolys[i].Dir.z *= Speed;
	     			}
					else
					{
		     			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * framelag );
		     			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * framelag );
		     			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * framelag );
					}

					ApplyMatrix( &UpMatrix, &FmPolys[i].Dir, &TempDir );

	     			if( TempDir.x )
	     			{
	     				if( TempDir.x > 0.0F )
	     				{
	     					TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     				}
	     				else
	     				{
	     					TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     				}
	     			}
	     			if( TempDir.z )
	     			{
	     				if( TempDir.z > 0.0F )
	     				{
	     					TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     				}
	     				else
	     				{
	     					TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     				}
	     			}
	     			TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

					ApplyMatrix( &InvUpMatrix, &TempDir, &FmPolys[i].Dir );
				}
	 			break;

			case FM_SPOTFX_BUBBLE:							// Bubble?
				OldPos = FmPolys[i].Pos;

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
	   			FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.10F ) ) * framelag );
	   			FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.05F ) ) * framelag );
	   			FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.15F ) ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
	   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.10F ) ) * framelag );
	   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.05F ) ) * framelag );
	   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.15F ) ) * framelag );
			
	   			if( FmPolys[i].Speed > 0.0F )
	   			{
	   				FmPolys[i].Speed -= ( 0.4F * framelag );
	   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
	   			}

	   			if( FmPolys[i].UpSpeed < 4.0F )
	   			{
	   				FmPolys[i].UpSpeed += ( 0.4F * framelag );
	   				if( FmPolys[i].UpSpeed > 4.0F ) FmPolys[i].UpSpeed = 4.0F;
	   			}

				TempDir.x = ( FmPolys[i].Pos.x - OldPos.x );
				TempDir.y = ( FmPolys[i].Pos.y - OldPos.y );
				TempDir.z = ( FmPolys[i].Pos.z - OldPos.z );
				FmPolys[i].Group = MoveGroup( &Mloadheader, &OldPos, FmPolys[i].Group, &TempDir );

				if( ( FmPolys[i].LifeCount == 0.0F ) ||	( !InWater( FmPolys[i].Group, &FmPolys[i].Pos, &Damage ) ) )
				{
					TempDir.x = ( FmPolys[i].Pos.x - OldPos.x );
					TempDir.y = ( FmPolys[i].Pos.y - OldPos.y );
					TempDir.z = ( FmPolys[i].Pos.z - OldPos.z );
					WaterObjectCollide( FmPolys[i].Group, &OldPos, &TempDir, &Int_Point, 8 );
		   			KillUsedFmPoly( i );
				}
		   		break;

			case FM_NMEGEN_EFFECT:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE1 )			// 0 - 3
					{
			   			FmPolys[ i ].Rot -= ( ( 360.0F / NMEGEN_STAGE0_LEN ) * framelag ); // 1 Rot in 3 seconds
			   			FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
						FmPolys[ i ].R = (uint8) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].G = (uint8) ( 49.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 49.0F / NMEGEN_STAGE0_LEN ) ) );
					}
					else
					{
			   			FmPolys[ i ].Rot -= ( ( 360.0F / ( NMEGEN_STAGE1_LEN + NMEGEN_STAGE2_LEN ) ) * framelag );	// 1 Rot in 2 Seconds
			   			FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

						if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE2 )		// 3 - 4
						{
							FmPolys[ i ].R = (uint8) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 83.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].G = (uint8) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 206.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].B = (uint8) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 255.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].xsize -= ( ( ( 6.0F * GLOBAL_SCALE ) / NMEGEN_STAGE1_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
						else												// 4 - 5
						{
							FmPolys[ i ].xsize -= ( ( ( 10.0F * GLOBAL_SCALE ) / NMEGEN_STAGE2_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
					}
				}
				break;

			case FM_NMEGEN_FLARE:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE1 )			// 0 - 3
					{
						FmPolys[ i ].R = (uint8) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].G = (uint8) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].B = (uint8) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].xsize += ( ( ( 14.0F * GLOBAL_SCALE ) / NMEGEN_STAGE0_LEN ) * framelag );
						FmPolys[ i ].ysize = FmPolys[ i ].xsize;
					}
					else
					{
						if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE2 )		// 3 - 4
						{
							FmPolys[ i ].xsize -= ( ( ( 6.0F * GLOBAL_SCALE ) / NMEGEN_STAGE1_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
						else												// 4 - 5
						{
							FmPolys[ i ].xsize -= ( ( ( 8.0F * GLOBAL_SCALE ) / NMEGEN_STAGE2_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
					}
				}
				break;

			case FM_SPOTFX_DRIP:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].Speed += ( 0.1F * framelag );
					if( FmPolys[i].Speed > ( 40.0F * GLOBAL_SCALE ) ) FmPolys[i].Speed = ( 40.0F * GLOBAL_SCALE );

					Speed = ( FmPolys[i].Speed * framelag );

					TempDir.x = ( FmPolys[i].Dir.x * Speed );
					TempDir.y = ( FmPolys[i].Dir.y * Speed );
					TempDir.z = ( FmPolys[i].Dir.z * Speed );

					if( WaterObjectCollide( FmPolys[i].Group, &FmPolys[i].Pos, &TempDir, &Int_Point, 32 ) )
					{
						CreateSpotFXDripSplash( &Int_Point, &SlideUp, FmPolys[i].Group, FmPolys[i].R, FmPolys[i].G, FmPolys[i].B );
			   			KillUsedFmPoly( i );
					}
					else
					{
		     			if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &FmPolys[i].Pos,
		     								  FmPolys[i].Group, &TempDir, (VECTOR *) &Int_Point,
			 								  &FmPolys[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     				{
							CreateSpotFXDripSplash( &Int_Point, &SlideUp, FmPolys[i].Group, FmPolys[i].R, FmPolys[i].G, FmPolys[i].B );
				   			KillUsedFmPoly( i );
						}
						else
						{
			   				FmPolys[i].Pos.x += TempDir.x;
			   				FmPolys[i].Pos.y += TempDir.y;
			   				FmPolys[i].Pos.z += TempDir.z;
						}
					}
				}
				break;

			case FM_PHOTON:
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

				FmPolys[i].ysize = (float) ( ( sin( FmPolys[ i ].Rot ) * 4.0F ) );
				break;

			case FM_BLUEPHOTON:
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

				FmPolys[i].ysize = (float) ( ( sin( FmPolys[ i ].Rot ) * 6.0F ) );
				break;

		   	case FM_SPOTFX_BURNING:
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}
			
		   			FmPolys[i].Frame += ( framelag / 2.0F );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );
			
		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_BEARD_AFTERBURNER:
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize -= ( ( 0.1F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.1F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}
			
			   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
			   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
			   		{
		   				KillUsedFmPoly( i );
					}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}

		   		break;

		   	case FM_RESTART_INACTIVE:							// Restart Point Inactive
				FadeColour( &FmPolys[i].R, FmPolys[i].Start_R, 5.0F );
				FadeColour( &FmPolys[i].G, FmPolys[i].Start_G, 5.0F );
				FadeColour( &FmPolys[i].B, FmPolys[i].Start_B, 5.0F );

				FmPolys[ i ].Rot += ( FmPolys[ i ].RotSpeed * framelag );
				FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
				break;

		   	case FM_RESTART_ACTIVE:								// Restart Point Active
				FadeColour( &FmPolys[i].R, FmPolys[i].Start_R, 5.0F );
				FadeColour( &FmPolys[i].G, FmPolys[i].Start_G, 5.0F );
				FadeColour( &FmPolys[i].B, FmPolys[i].Start_B, 5.0F );

				FmPolys[ i ].Rot += ( FmPolys[ i ].RotSpeed * framelag );
				FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
		   		break;

		   	case FM_SPOTFX_WATERSPLASH:							// Water Splash?
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ FmPolys[i].Group ].up, &SlideUp );
					QuatToMatrix( &TempQuat, &UpMatrix );
					MatrixTranspose( &UpMatrix, &InvUpMatrix );

	     			TempDir.x = ( FmPolys[i].Dir.x * framelag );
	     			TempDir.y = ( FmPolys[i].Dir.y * framelag );
  	     			TempDir.z = ( FmPolys[i].Dir.z * framelag );
		  
					if( WaterObjectCollide( FmPolys[i].Group, &FmPolys[i].Pos, &TempDir, &Int_Point, 0 ) )
					{
			   			KillUsedFmPoly( i );
					}
					else
					{
		     			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * framelag );
		     			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * framelag );
			 			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * framelag );

						ApplyMatrix( &UpMatrix, &FmPolys[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &FmPolys[i].Dir );
					}
				}
	 			break;

		   	case FM_HK5_TRAIL:								// Hk5 Trail?
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( ( 0.25F * GLOBAL_SCALE ) * framelag );
					FmPolys[i].ysize -= ( ( 0.25F * GLOBAL_SCALE ) * framelag );

					Speed = ( FmPolys[i].Speed * framelag );
					FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
			
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.125F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}
				}
		   		break;

		   	case FM_FIREBALL:
		   		FmPolys[i].Trans = ( 248 - ( (int16) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag ); 
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}
			
			   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );
			
		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

			case FM_SPARKLE:						// Sparkle?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
			   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
			   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
			   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

					Speed = FMOD( FmPolys[i].LifeCount, 20.0F );

					if( Speed > 10.0F )
					{
						FmPolys[i].xsize = ( ( 20.0F - Speed ) * 0.05F );
					}
					else
					{
						FmPolys[i].xsize = ( Speed * 0.05F );
					}
					FmPolys[i].ysize = FmPolys[i].xsize;
				}
				break;

			case FM_GOALSPARKLE:						// Sparkle?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
			   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
			   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
			   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

					Speed = FMOD( FmPolys[i].LifeCount, 20.0F );

					if( Speed > 10.0F )
					{
						FmPolys[i].xsize = ( ( 20.0F - Speed ) * 0.05F );
					}
					else
					{
						FmPolys[i].xsize = ( Speed * 0.05F );
					}
					FmPolys[i].ysize = FmPolys[i].xsize;
				}
				break;

		   	default:
		   		break;
		}
		i = nextprim;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	uint16						Group
				:	uint16						Next FmPoly to process (updated)
	Output		:	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL PVR_FmPolyDispGroup( uint16 Group, uint16 * Next )
{
	LPD3DLVERTEX	Temp_Ptr;
	LPD3DLVERTEX	FmPolyVertPnt;
	uint16			i;
	uint16			TotalVertCount;
	VECTOR			XVector;
	VECTOR			YVector;
	VECTOR			ZVector;
	VECTOR			Xoff;
	VECTOR			Yoff;
	D3DCOLOR		color;
	D3DCOLOR		specular;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	VECTOR			Xsize;
	VECTOR			Ysize;
	VECTOR			NewVector;
	VECTOR			NewPos;
	VECTOR			Rotation;
	int16			Bitcount;
	D3DLVERTEX		TempVert;
	FMPOLY	*		TempPtr;
	float			Length;
	MATRIX			MatrixCopy;
	D3DLVERTEX		Verts[ 8 ];

	if(d3dapp->CurrDriver != 0)	specular = RGB_MAKE( 255, 255, 255 );
	else specular = RGB_MAKE( 128, 128, 128 );
		
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Copy Verts into execution list
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	i = *Next;
	TotalVertCount = 0;

	while( i != (uint16) -1 )
	{
		if( TotalVertCount > MAXFMPOLYVERTS ) break;

//		if( !( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) )
		{
			if( FmPolys[i].Group == Group )
			{
				Rotation.x = (float) sin( D2R( FmPolys[ i ].Rot ) );
				Rotation.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
				Rotation.z = 0.0F;
		
				color = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 128 );
		
				if( FmPolys[ i ].Flags & FM_FLAG_DIRCONST )
				{
					YVector = FmPolys[ i ].UpVector;					/* Calc Up Vector */
				   	ZVector = FmPolys[ i ].DirVector;					/* Calc Forward Vector */
				}
				else
				{
					MatrixCopy = CurrentCamera.Mat;
					MatrixCopy._41 = 0.0F;
					MatrixCopy._42 = 0.0F;
					MatrixCopy._43 = 0.0F;
					ApplyMatrix( &MatrixCopy, &Rotation, &YVector );	/* Calc Up Vector */
					ApplyMatrix( &MatrixCopy, &Forward, &ZVector );		/* Calc Forward Vector */
				}
				CrossProduct( &YVector, &ZVector, &XVector );			/* Calc Left Vector */
		
				if( FmPolys[ i ].Frm_Info != NULL )
				{
					TempPtr = &FmPolys[i];
		
					if( FmPolys[ i ].Frame >= (*FmPolys[ i ].Frm_Info)->Num_Frames ) FmPolys[i].Frame = 0.0F;
		
					Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16) FmPolys[ i ].Frame );
					Off_Ptr = ( (*FmPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		
					TotalVertCount += 4;
		
					for( Bitcount = 0; Bitcount < Bit_Ptr->numbits; Bitcount++ )
					{
						FmPolyVertPnt = (LPD3DLVERTEX) &Verts[ 0 ];

						Box_Ptr = ( (*FmPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );
		
						Xoff.x = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.x );
						Xoff.y = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.y );
						Xoff.z = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.z );
						Yoff.x = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.x );
						Yoff.y = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.y );
						Yoff.z = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.z );
		
						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
		
						if( Off_Ptr->box & 0x2000 )
						{
							Xsize.x = -Xsize.x;
							Xsize.y = -Xsize.y;
							Xsize.z = -Xsize.z;
						}
		
						if( Off_Ptr->box & 0x8000 )
						{
							Ysize.x = -Ysize.x;
							Ysize.y = -Ysize.y;
							Ysize.z = -Ysize.z;
						}
		
						Temp_Ptr = FmPolyVertPnt;
		
						if( ( FmPolys[ i ].Flags & FM_FLAG_MOVEOUT ) )
						{
							NewVector.x = ( FmPolys[ i ].Pos.x - CurrentCamera.Pos.x );
							NewVector.y = ( FmPolys[ i ].Pos.y - CurrentCamera.Pos.y );
							NewVector.z = ( FmPolys[ i ].Pos.z - CurrentCamera.Pos.z );
		
							Length = VectorLength( &NewVector );
		
							NormaliseVector( &NewVector );
							if( Length >= ( EXPLO_DISTANCE + 110.0F ) )
							{
								Length = EXPLO_DISTANCE;
							}
							else
							{
								Length -= 110.0F;
								if( Length < 32.0F ) Length = 32.0F;
							}
		
							NewPos.x = ( FmPolys[ i ].Pos.x + ( -Length * NewVector.x ) );
							NewPos.y = ( FmPolys[ i ].Pos.y + ( -Length * NewVector.y ) );
							NewPos.z = ( FmPolys[ i ].Pos.z + ( -Length * NewVector.z ) );
		
							FmPolyVertPnt->x = ( NewPos.x + Xoff.x + Yoff.x );
							FmPolyVertPnt->y = ( NewPos.y + Xoff.y + Yoff.y );
							FmPolyVertPnt->z = ( NewPos.z + Xoff.z + Yoff.z );
						}
						else
						{
							FmPolyVertPnt->x = ( FmPolys[i].Pos.x + Xoff.x + Yoff.x );
							FmPolyVertPnt->y = ( FmPolys[i].Pos.y + Xoff.y + Yoff.y );
							FmPolyVertPnt->z = ( FmPolys[i].Pos.z + Xoff.z + Yoff.z );
						}
		
						FmPolyVertPnt->tu = Box_Ptr->u1;
						FmPolyVertPnt->tv = Box_Ptr->v1;
						FmPolyVertPnt->color = color;
						FmPolyVertPnt->specular = specular;
						FmPolyVertPnt->dwReserved = 0;
						FmPolyVertPnt++;
							
						FmPolyVertPnt->x = ( Temp_Ptr->x + Xsize.x );
						FmPolyVertPnt->y = ( Temp_Ptr->y + Xsize.y );
						FmPolyVertPnt->z = ( Temp_Ptr->z + Xsize.z );
						FmPolyVertPnt->tu = Box_Ptr->u2;
						FmPolyVertPnt->tv = Box_Ptr->v1;
						FmPolyVertPnt->color = color;
						FmPolyVertPnt->specular = specular;
						FmPolyVertPnt->dwReserved = 0;
						FmPolyVertPnt++;
							
						FmPolyVertPnt->x = ( Temp_Ptr->x + Xsize.x - Ysize.x );
						FmPolyVertPnt->y = ( Temp_Ptr->y + Xsize.y - Ysize.y );
						FmPolyVertPnt->z = ( Temp_Ptr->z + Xsize.z - Ysize.z );
						FmPolyVertPnt->tu = Box_Ptr->u2;
						FmPolyVertPnt->tv = Box_Ptr->v2;
						FmPolyVertPnt->color = color;
						FmPolyVertPnt->specular = specular;
						FmPolyVertPnt->dwReserved = 0;
						FmPolyVertPnt++;
						
						FmPolyVertPnt->x = ( Temp_Ptr->x - Ysize.x );
						FmPolyVertPnt->y = ( Temp_Ptr->y - Ysize.y );
						FmPolyVertPnt->z = ( Temp_Ptr->z - Ysize.z );
						FmPolyVertPnt->tu = Box_Ptr->u1;
						FmPolyVertPnt->tv = Box_Ptr->v2;
						FmPolyVertPnt->color = color;
						FmPolyVertPnt->specular = specular;
						FmPolyVertPnt->dwReserved = 0;
						FmPolyVertPnt++;
		
						if( ( Off_Ptr->box & 0xf000 ) == 0x2000 || ( Off_Ptr->box & 0xf000 ) == 0x8000 )
						{
							memcpy( &TempVert, FmPolyVertPnt-4, sizeof( D3DLVERTEX ) );
							memcpy( FmPolyVertPnt-4, FmPolyVertPnt-2, sizeof( D3DLVERTEX ) );
							memcpy( FmPolyVertPnt-2, &TempVert, sizeof( D3DLVERTEX ) );
						}

						AddToPolySort( &Verts[ 0 ], &Verts[ 1 ], &Verts[ 2 ], Box_Ptr->tpage );
						AddToPolySort( &Verts[ 0 ], &Verts[ 2 ], &Verts[ 3 ], Box_Ptr->tpage );
						
						if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag )
						{
							memcpy( ( FmPolyVertPnt + 0 ), ( FmPolyVertPnt - 4 ), sizeof( D3DLVERTEX ) );
							memcpy( ( FmPolyVertPnt + 1 ), ( FmPolyVertPnt - 1 ), sizeof( D3DLVERTEX ) );
							memcpy( ( FmPolyVertPnt + 2 ), ( FmPolyVertPnt - 2 ), sizeof( D3DLVERTEX ) );
							memcpy( ( FmPolyVertPnt + 3 ), ( FmPolyVertPnt - 3 ), sizeof( D3DLVERTEX ) );
							FmPolyVertPnt += 4;
							TotalVertCount += 4;

							AddToPolySort( &Verts[ 4 ], &Verts[ 5 ], &Verts[ 6 ], Box_Ptr->tpage );
							AddToPolySort( &Verts[ 4 ], &Verts[ 6 ], &Verts[ 7 ], Box_Ptr->tpage );

						}
						Off_Ptr++;

					}
				}
			}
		}
		i = FmPolys[i].Prev;
	}

	*Next = i;

	if( TotalVertCount == 0 ) return FALSE;

	return TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init FmPoly TPage Groups
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitFmPolyTPages( void )
{
	uint16	i;

	for( i = 0; i < MAXTPAGESPERTLOAD; i++ )
	{
		FmPolyTPages[ i ].FirstPoly = (uint16) -1;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Add FmPoly To TPage Link List
	Input		:	uint16		FmPoly Index
				:	uint16		TPage to add to
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddFmPolyToTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	FmPolys[ i ].PrevInTPage = (uint16) -1;
	FmPolys[ i ].NextInTPage = FmPolyTPages[ TPage ].FirstPoly;

	if( FmPolys[ i ].NextInTPage != (uint16) -1 )
	{
		FmPolys[ FmPolys[ i ].NextInTPage ].PrevInTPage = i;
	}

	FmPolyTPages[ TPage ].FirstPoly = i;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Remove FmPoly From TPage Link List
	Input		:	uint16		FmPoly Index
				:	uint16		TPage to add to
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RemoveFmPolyFromTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	if( FmPolys[ i ].PrevInTPage != (uint16) -1 )
	{
		FmPolys[ FmPolys[ i ].PrevInTPage ].NextInTPage = FmPolys[ i ].NextInTPage;
	}
	else
	{
		FmPolyTPages[ TPage ].FirstPoly = FmPolys[ i ].NextInTPage;
	}

	if( FmPolys[ i ].NextInTPage != (uint16) -1 )
	{
		FmPolys[ FmPolys[ i ].NextInTPage ].PrevInTPage = FmPolys[ i ].PrevInTPage;
	}

	FmPolys[ i ].PrevInTPage = (uint16) -1;
	FmPolys[ i ].NextInTPage = (uint16) -1;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display all group clipped FmPolys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	uint16						Group
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL DisplayGroupClippedFmPolys( LPDIRECT3DEXECUTEBUFFER ExecBuff, uint16 Group,
								 LPDIRECT3DDEVICE D3D_Device, LPDIRECT3DVIEWPORT D3D_ViewPort )
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = FmPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !FmPolyDispGroupClipped( Group, ExecBuff, &TPage, &i ) )
			return( TRUE );

#ifdef SOFTWARE_ENABLE
		if( SoftwareVersion )
		{
			CWExecute2( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED );
		}
		else
#endif
		{
			if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
				return FALSE;
		}
	}

	return( FALSE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display all non group clipped FmPolys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL DisplayGroupUnclippedFmPolys( LPDIRECT3DEXECUTEBUFFER ExecBuff,
								   LPDIRECT3DDEVICE D3D_Device, LPDIRECT3DVIEWPORT D3D_ViewPort )
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = FmPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !FmPolyDispGroupUnclipped( ExecBuff, &TPage, &i ) )
			return( TRUE );

#ifdef SOFTWARE_ENABLE
		if( SoftwareVersion )
		{
			CWExecute2( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED );
		}
		else
#endif
		{
			if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
				return FALSE;
		}
	}

	return( FALSE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	uint16						Group
				:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current FmPoly
	Output		:	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL FmPolyDispGroupClipped( uint16 Group, LPDIRECT3DEXECUTEBUFFER ExecBuffer, int16 * TPage, uint16 * NextFmPoly )
{
	uint16			i;
	int16			Count;
	int16			BitCount;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	VECTOR			TempVector;
	VECTOR			XVector;
	VECTOR			YVector;
	VECTOR			ZVector;
	VECTOR			Xoff;
	VECTOR			Yoff;
	VECTOR			Xsize;
	VECTOR			Ysize;
	VECTOR			NewPos;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
 	float			Distance;
	MATRIX			MatrixCopy;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	FmPolyVertPnt;
	LPD3DTRIANGLE	FmPolyFacePnt;
    LPVOID			lpBufStart, lpInsStart, lpPointer;
	MATRIX			TempMatrix;
	QUAT			TempQuat;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Find out how may verts involved in Exec Buffer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	TotalVerts = 0;

	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == *TPage ) i = *NextFmPoly;
		else i = FmPolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXFMPOLYVERTS ) )
		{
			if( !( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) && ( FmPolys[i].Group == Group ) )
			{
				if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
				{
					Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16) FmPolys[ i ].Frame );
	
					if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag ) NumTris += ( 4 * Bit_Ptr->numbits );
					else NumTris += ( 2 * Bit_Ptr->numbits );
	
					NumVerts += ( 4 * Bit_Ptr->numbits );
				}
			}
			
			i = FmPolys[ i ].NextInTPage;
		}

		FmPolyTPages[ Count ].StartVert = StartVert;
		FmPolyTPages[ Count ].NumVerts = NumVerts;
		FmPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXFMPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	if(d3dapp->CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Lock Exec Buffer and get ready to fill in...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		
	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
		
	lpBufStart = ExecBuffer_debdesc.lpData;
	FmPolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	lpPointer = (LPVOID) ( FmPolyVertPnt + TotalVerts );
	lpInsStart = lpPointer;

	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE, lpPointer );
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = FmPolyTPages[ Count ].StartVert;
		NumVerts = FmPolyTPages[ Count ].NumVerts;
		NumTris = FmPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
	 
	   		FmPolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextFmPoly;
			else i = FmPolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXFMPOLYVERTS ) )
			{
				if( !( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) && ( FmPolys[i].Group == Group ) )
				{
					if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
					{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Create Colour Value for polys
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						switch( MakeColourMode )
						{
							case MCM_Normal:
#if ACTUAL_TRANS
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
#else
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 255 );
#endif
								break;
		
							case MCM_Stipple:
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans / 2 );
								break;
			
							case MCM_PowerVR:
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 128 );
								break;
			
							case MCM_Software:
#ifdef SOFTWARE_ENABLE
								if( SoftwareVersion )
								{
									Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
								}
								else
								{
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
								}
#else
								Colour = RGBA_MAKE2( 128, 128, 128, 255 );
#endif
								break;
						}
			
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Set Dir and Up Vectors
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						if( FmPolys[ i ].Flags & FM_FLAG_DIRCONST )
						{
							YVector = FmPolys[ i ].UpVector;					/* Calc Up Vector */
						   	ZVector = FmPolys[ i ].DirVector;					/* Calc Forward Vector */
						}
						else
						{
							YVector.x = (float) sin( D2R( FmPolys[ i ].Rot ) ); /* Calc Up Vector */
							YVector.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
							YVector.z = 0.0F;
							ZVector = Forward;

							if( FmPolys[ i ].Flags & FM_FLAG_RELATIVE )
							{
								QuatFromDirAndUp( &FmPolys[i].DirVector, &FmPolys[i].UpVector, &TempQuat );
								QuatToMatrix( &TempQuat, &TempMatrix );
								ApplyMatrix( &TempMatrix, &YVector, &YVector );
								ApplyMatrix( &TempMatrix, &ZVector, &ZVector );
							}
			
							MatrixCopy = CurrentCamera.Mat;
							MatrixCopy._41 = 0.0F;
							MatrixCopy._42 = 0.0F;
							MatrixCopy._43 = 0.0F;
							ApplyMatrix( &MatrixCopy, &YVector, &YVector );		/* Calc Up Vector */
							ApplyMatrix( &MatrixCopy, &ZVector, &ZVector );		/* Calc Forward Vector */
						}
			
						CrossProduct( &YVector, &ZVector, &XVector );			/* Calc Left Vector */
			
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Setup FrameInfo description pointers
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16) FmPolys[ i ].Frame );
						Off_Ptr = ( (*FmPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
			
						for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
						{
							Box_Ptr = ( (*FmPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );
			
							Xoff.x = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.x );
							Xoff.y = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.y );
							Xoff.z = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.z );
							Yoff.x = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.x );
							Yoff.y = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.y );
							Yoff.z = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.z );
					
							switch( Off_Ptr->box & BOX_BOTHFLIP )
							{
								case BOX_NOFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_XFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_YFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_BOTHFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
							}
			
/*-------------- Errrrmmmmmmm ---------------*/
							if( ( FmPolys[ i ].Flags & FM_FLAG_MOVEOUT ) )
							{
								TempVector.x = ( FmPolys[ i ].Pos.x - CurrentCamera.Pos.x );
								TempVector.y = ( FmPolys[ i ].Pos.y - CurrentCamera.Pos.y );
								TempVector.z = ( FmPolys[ i ].Pos.z - CurrentCamera.Pos.z );
								Distance = VectorLength( &TempVector );
								NormaliseVector( &TempVector );
			
								if( Distance >= ( EXPLO_DISTANCE + 110.0F ) )
								{
									Distance = EXPLO_DISTANCE;
								}
								else
								{
									Distance -= 110.0F;
									if( Distance < 32.0F ) Distance = 32.0F;
								}
					
								NewPos.x = ( FmPolys[ i ].Pos.x + ( -Distance * TempVector.x ) );
								NewPos.y = ( FmPolys[ i ].Pos.y + ( -Distance * TempVector.y ) );
								NewPos.z = ( FmPolys[ i ].Pos.z + ( -Distance * TempVector.z ) );
							}
							else
							{
								NewPos = FmPolys[i].Pos;
							}
/*-------------------------------------------*/
					
							NewPos.x += ( Xoff.x + Yoff.x );
							NewPos.y += ( Xoff.y + Yoff.y );
							NewPos.z += ( Xoff.z + Yoff.z );
			
							FmPolyVertPnt->x = NewPos.x;						// v1
							FmPolyVertPnt->y = NewPos.y;
							FmPolyVertPnt->z = NewPos.z;
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
								
							FmPolyVertPnt->x = ( NewPos.x + Xsize.x );			// v2
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
								
							FmPolyVertPnt->x = ( NewPos.x + Xsize.x - Ysize.x ); // v3
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
							
							FmPolyVertPnt->x = ( NewPos.x - Ysize.x );			// v4
							FmPolyVertPnt->y = ( NewPos.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
	  
#ifdef SOFTWARE_ENABLE
  							if( ( FmPolys[ i ].SeqNum == FM_GOALSPARKLE ) &&  SoftwareVersion )
							{
					   			FmPolyFacePnt->v1 = ( StartVert + 0 );
					   			FmPolyFacePnt->v2 = ( StartVert + 1 );
					   			FmPolyFacePnt->v3 = ( StartVert + 2 );
								FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 | CHRISFLAG );
					   			FmPolyFacePnt++;
					   			FmPolyFacePnt->v1 = ( StartVert + 0 );
					   			FmPolyFacePnt->v2 = ( StartVert + 2 );
					   			FmPolyFacePnt->v3 = ( StartVert + 3 );
					   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 | CHRISFLAG );
					   			FmPolyFacePnt++;
				
								if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag )
								{
						   			FmPolyFacePnt->v1 = ( StartVert + 0 );
									FmPolyFacePnt->v2 = ( StartVert + 3 );
						   			FmPolyFacePnt->v3 = ( StartVert + 2 );
						   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 | CHRISFLAG  );
						   			FmPolyFacePnt++;
						   			FmPolyFacePnt->v1 = ( StartVert + 0 );
						   			FmPolyFacePnt->v2 = ( StartVert + 2 );
						   			FmPolyFacePnt->v3 = ( StartVert + 1 );
						   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 | CHRISFLAG  );
						   			FmPolyFacePnt++;
								}
							}
							else
#endif
							{
					   			FmPolyFacePnt->v1 = ( StartVert + 0 );
					   			FmPolyFacePnt->v2 = ( StartVert + 1 );
					   			FmPolyFacePnt->v3 = ( StartVert + 2 );
					   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
					   			FmPolyFacePnt++;
					   			FmPolyFacePnt->v1 = ( StartVert + 0 );
					   			FmPolyFacePnt->v2 = ( StartVert + 2 );
					   			FmPolyFacePnt->v3 = ( StartVert + 3 );
					   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
					   			FmPolyFacePnt++;
				
								if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag )
								{
						   			FmPolyFacePnt->v1 = ( StartVert + 0 );
									FmPolyFacePnt->v2 = ( StartVert + 3 );
						   			FmPolyFacePnt->v3 = ( StartVert + 2 );
						   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
						   			FmPolyFacePnt++;
						   			FmPolyFacePnt->v1 = ( StartVert + 0 );
						   			FmPolyFacePnt->v2 = ( StartVert + 2 );
						   			FmPolyFacePnt->v3 = ( StartVert + 1 );
						   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
						   			FmPolyFacePnt++;
								}
							}
			
							StartVert += 4;
							Off_Ptr++;
						}
					}
				}
		
				i = FmPolys[ i ].NextInTPage;
			}

	   		lpPointer = ( LPVOID ) FmPolyFacePnt;
		}

		if( StartVert >= MAXFMPOLYVERTS ) break;
	}

	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW, lpPointer );
	}

	OP_EXIT( lpPointer );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		UnLock Exec Buffer and set data description
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );

	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );

	*TPage = Count;
	*NextFmPoly = i;

	return( TRUE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current FmPoly
	Output		:	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL FmPolyDispGroupUnclipped( LPDIRECT3DEXECUTEBUFFER ExecBuffer, int16 * TPage, uint16 * NextFmPoly )
{
	uint16			i;
	int16			Count;
	int16			BitCount;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	VECTOR			TempVector;
	VECTOR			XVector;
	VECTOR			YVector;
	VECTOR			ZVector;
	VECTOR			Xoff;
	VECTOR			Yoff;
	VECTOR			Xsize;
	VECTOR			Ysize;
	VECTOR			NewPos;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
 	float			Distance;
	MATRIX			MatrixCopy;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	FmPolyVertPnt;
	LPD3DTRIANGLE	FmPolyFacePnt;
    LPVOID			lpBufStart, lpInsStart, lpPointer;
	MATRIX			TempMatrix;
	QUAT			TempQuat;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Find out how may verts involved in Exec Buffer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	TotalVerts = 0;

	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == *TPage ) i = *NextFmPoly;
		else i = FmPolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXFMPOLYVERTS ) )
		{
			if( ( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) )
			{
				if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
				{
					Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16) FmPolys[ i ].Frame );
	
					if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag ) NumTris += ( 4 * Bit_Ptr->numbits );
					else NumTris += ( 2 * Bit_Ptr->numbits );
	
					NumVerts += ( 4 * Bit_Ptr->numbits );
				}
			}
			
			i = FmPolys[ i ].NextInTPage;
		}

		FmPolyTPages[ Count ].StartVert = StartVert;
		FmPolyTPages[ Count ].NumVerts = NumVerts;
		FmPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXFMPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	if(d3dapp->CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Lock Exec Buffer and get ready to fill in...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		
	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
		
	lpBufStart = ExecBuffer_debdesc.lpData;
	FmPolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	lpPointer = (LPVOID) ( FmPolyVertPnt + TotalVerts );
	lpInsStart = lpPointer;

	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE, lpPointer );
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = FmPolyTPages[ Count ].StartVert;
		NumVerts = FmPolyTPages[ Count ].NumVerts;
		NumTris = FmPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
	 
	   		FmPolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextFmPoly;
			else i = FmPolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXFMPOLYVERTS ) )
			{
				if( ( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) )
				{
					if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
					{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Create Colour Value for polys
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						switch( MakeColourMode )
						{
							case MCM_Normal:
#if ACTUAL_TRANS
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
#else
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 255 );
#endif
								break;
		
							case MCM_Stipple:
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans / 2 );
								break;
			
							case MCM_PowerVR:
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 128 );
								break;
			
							case MCM_Software:
#ifdef SOFTWARE_ENABLE
								if( SoftwareVersion )
								{
									Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
								}
								else
								{
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
								}
#else
								Colour = RGBA_MAKE2( 128, 128, 128, 255 );
#endif
								break;
						}
			
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Set Dir and Up Vectors
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						if( FmPolys[ i ].Flags & FM_FLAG_DIRCONST )
						{
							YVector = FmPolys[ i ].UpVector;					/* Calc Up Vector */
						   	ZVector = FmPolys[ i ].DirVector;					/* Calc Forward Vector */
						}
						else
						{
							YVector.x = (float) sin( D2R( FmPolys[ i ].Rot ) ); /* Calc Up Vector */
							YVector.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
							YVector.z = 0.0F;
							ZVector = Forward;

							if( FmPolys[ i ].Flags & FM_FLAG_RELATIVE )
							{
								QuatFromDirAndUp( &FmPolys[i].DirVector, &FmPolys[i].UpVector, &TempQuat );
								QuatToMatrix( &TempQuat, &TempMatrix );
								ApplyMatrix( &TempMatrix, &YVector, &YVector );
								ApplyMatrix( &TempMatrix, &ZVector, &ZVector );
							}
			
							MatrixCopy = CurrentCamera.Mat;
							MatrixCopy._41 = 0.0F;
							MatrixCopy._42 = 0.0F;
							MatrixCopy._43 = 0.0F;
							ApplyMatrix( &MatrixCopy, &YVector, &YVector );		/* Calc Up Vector */
							ApplyMatrix( &MatrixCopy, &ZVector, &ZVector );		/* Calc Forward Vector */
						}
			
						CrossProduct( &YVector, &ZVector, &XVector );			/* Calc Left Vector */
			
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Setup FrameInfo description pointers
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16) FmPolys[ i ].Frame );
						Off_Ptr = ( (*FmPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
			
						for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
						{
							Box_Ptr = ( (*FmPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );
			
							Xoff.x = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.x );
							Xoff.y = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.y );
							Xoff.z = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.z );
							Yoff.x = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.x );
							Yoff.y = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.y );
							Yoff.z = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.z );
					
							switch( Off_Ptr->box & BOX_BOTHFLIP )
							{
								case BOX_NOFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_XFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_YFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
			
								case BOX_BOTHFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
							}
			
/*-------------- Errrrmmmmmmm ---------------*/
							if( ( FmPolys[ i ].Flags & FM_FLAG_MOVEOUT ) )
							{
								TempVector.x = ( FmPolys[ i ].Pos.x - CurrentCamera.Pos.x );
								TempVector.y = ( FmPolys[ i ].Pos.y - CurrentCamera.Pos.y );
								TempVector.z = ( FmPolys[ i ].Pos.z - CurrentCamera.Pos.z );
								Distance = VectorLength( &TempVector );
								NormaliseVector( &TempVector );
			
								if( Distance >= ( EXPLO_DISTANCE + 110.0F ) )
								{
									Distance = EXPLO_DISTANCE;
								}
								else
								{
									Distance -= 110.0F;
									if( Distance < 32.0F ) Distance = 32.0F;
								}
					
								NewPos.x = ( FmPolys[ i ].Pos.x + ( -Distance * TempVector.x ) );
								NewPos.y = ( FmPolys[ i ].Pos.y + ( -Distance * TempVector.y ) );
								NewPos.z = ( FmPolys[ i ].Pos.z + ( -Distance * TempVector.z ) );
							}
							else
							{
								NewPos = FmPolys[i].Pos;
							}
/*-------------------------------------------*/
					
							NewPos.x += ( Xoff.x + Yoff.x );
							NewPos.y += ( Xoff.y + Yoff.y );
							NewPos.z += ( Xoff.z + Yoff.z );
			
							FmPolyVertPnt->x = NewPos.x;						// v1
							FmPolyVertPnt->y = NewPos.y;
							FmPolyVertPnt->z = NewPos.z;
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
								
							FmPolyVertPnt->x = ( NewPos.x + Xsize.x );			// v2
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
								
							FmPolyVertPnt->x = ( NewPos.x + Xsize.x - Ysize.x ); // v3
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
							
							FmPolyVertPnt->x = ( NewPos.x - Ysize.x );			// v4
							FmPolyVertPnt->y = ( NewPos.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
							FmPolyVertPnt->specular = Specular;
							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;
			
					   		FmPolyFacePnt->v1 = ( StartVert + 0 );
					   		FmPolyFacePnt->v2 = ( StartVert + 1 );
					   		FmPolyFacePnt->v3 = ( StartVert + 2 );
					   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
					   		FmPolyFacePnt++;
					   		FmPolyFacePnt->v1 = ( StartVert + 0 );
					   		FmPolyFacePnt->v2 = ( StartVert + 2 );
					   		FmPolyFacePnt->v3 = ( StartVert + 3 );
					   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
					   		FmPolyFacePnt++;
			
							if( ( FmPolys[i].Flags & FM_FLAG_TWOSIDED ) && !CanCullFlag )
							{
						   		FmPolyFacePnt->v1 = ( StartVert + 0 );
								FmPolyFacePnt->v2 = ( StartVert + 3 );
						   		FmPolyFacePnt->v3 = ( StartVert + 2 );
						   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
						   		FmPolyFacePnt++;
						   		FmPolyFacePnt->v1 = ( StartVert + 0 );
						   		FmPolyFacePnt->v2 = ( StartVert + 2 );
						   		FmPolyFacePnt->v3 = ( StartVert + 1 );
						   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
						   		FmPolyFacePnt++;
							}
			
							StartVert += 4;
							Off_Ptr++;
						}
					}
				}
		
				i = FmPolys[ i ].NextInTPage;
			}

	   		lpPointer = ( LPVOID ) FmPolyFacePnt;
		}

		if( StartVert >= MAXFMPOLYVERTS ) break;
	}

	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW, lpPointer );
	}

	OP_EXIT( lpPointer );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		UnLock Exec Buffer and set data description
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );

	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );

	*TPage = Count;
	*NextFmPoly = i;

	return( TRUE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fade Colour to wanted colour
	Input		:	uint8	*	Colour
				:	uint8		Wanted Colour
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FadeColour( uint8 * Colour, uint8 WantedColour, float Speed )
{
	float	WanCol, CurCol;

	WanCol = (float) WantedColour;
	CurCol = (float) *Colour;

	if( CurCol < WanCol )
	{
		CurCol += Speed;
		if( CurCol > WanCol ) CurCol = WanCol;
	}

	if( CurCol > WanCol )
	{
		CurCol -= Speed;
		if( CurCol < WanCol ) CurCol = WanCol;
	}

	*Colour = (uint8) CurCol;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Save FmPolys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SaveFmPolys( FILE * fp )
{
	uint16 i;
	int16	Frm_Info_Index;

	if( fp )
	{
		fwrite( &TotalFmPolysInUse, sizeof( uint32 ),1 ,fp );
		fwrite( &FirstFmPolyUsed, sizeof( FirstFmPolyUsed ), 1, fp );
		fwrite( &FirstFmPolyFree, sizeof( FirstFmPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fwrite( &FmPolyTPages[ i ].FirstPoly, sizeof( uint16 ), 1, fp );
		}

		i = FirstFmPolyUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &FmPolys[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &FmPolys[ i ].Prev, sizeof( uint16 ), 1, fp );
			fwrite( &FmPolys[ i ].NextInTPage, sizeof( uint16 ), 1, fp );
			fwrite( &FmPolys[ i ].PrevInTPage, sizeof( uint16 ), 1, fp );
			fwrite( &FmPolys[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].Start_R, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].Start_G, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].Start_B, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].R, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].G, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].B, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].Trans, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].OnceOnly, sizeof( uint8 ), 1, fp );
			fwrite( &FmPolys[ i ].Rot, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].RotSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Frame, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Flags, sizeof( int16 ), 1, fp );
			Frm_Info_Index = Get_Frm_Info_Index( FmPolys[ i ].Frm_Info );
			fwrite( &Frm_Info_Index, sizeof( int16 ), 1, fp );
			fwrite( &FmPolys[ i ].SeqNum, sizeof( int16 ), 1, fp );
			fwrite( &FmPolys[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &FmPolys[ i ].Group, sizeof( uint16 ), 1, fp );
			fwrite( &FmPolys[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].UpSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].xsize, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].ysize, sizeof( float ), 1, fp );
			i = FmPolys[ i ].Prev;
		}


		i = FirstFmPolyFree;

		while( i != (uint16) -1 )
		{
			fwrite( &FmPolys[ i ].Next, sizeof( uint16 ), 1, fp );
			i = FmPolys[ i ].Next;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Load FmPolys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * LoadFmPolys( FILE * fp )
{
	uint16 i;
	int16	Frm_Info_Index;

	if( fp )
	{
		fread( &TotalFmPolysInUse, sizeof( uint32 ),1 ,fp );
		fread( &FirstFmPolyUsed, sizeof( FirstFmPolyUsed ), 1, fp );
		fread( &FirstFmPolyFree, sizeof( FirstFmPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fread( &FmPolyTPages[ i ].FirstPoly, sizeof( uint16 ), 1, fp );
		}

		i = FirstFmPolyUsed;

		while( i != (uint16) -1 )
		{
			fread( &FmPolys[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &FmPolys[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &FmPolys[ i ].NextInTPage, sizeof( uint16 ), 1, fp );
			fread( &FmPolys[ i ].PrevInTPage, sizeof( uint16 ), 1, fp );
			fread( &FmPolys[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].Start_R, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].Start_G, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].Start_B, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].R, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].G, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].B, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].Trans, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].OnceOnly, sizeof( uint8 ), 1, fp );
			fread( &FmPolys[ i ].Rot, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].RotSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Frame, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Flags, sizeof( int16 ), 1, fp );
			fread( &Frm_Info_Index, sizeof( int16 ), 1, fp );
			FmPolys[ i ].Frm_Info = Get_Frm_Info_Address( Frm_Info_Index );
			fread( &FmPolys[ i ].SeqNum, sizeof( int16 ), 1, fp );
			fread( &FmPolys[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &FmPolys[ i ].Group, sizeof( uint16 ), 1, fp );
			fread( &FmPolys[ i ].Speed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].UpSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].xsize, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].ysize, sizeof( float ), 1, fp );
			i = FmPolys[ i ].Prev;
		}

		i = FirstFmPolyFree;

		while( i != (uint16) -1 )
		{
			memset( &FmPolys[i], 0, sizeof( FMPOLY ) );
			FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
			FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
			FmPolys[i].R = 255;
			FmPolys[i].G = 255;
			FmPolys[i].B = 255;
#if ACTUAL_TRANS
			FmPolys[i].Trans = 128;
#else
			FmPolys[i].Trans = 255;
#endif
			FmPolys[i].DirVector.x = 0.0F;
			FmPolys[i].DirVector.y = 0.0F;
			FmPolys[i].DirVector.z = 1.0F;
			FmPolys[i].UpVector.x = 0.0F;
			FmPolys[i].UpVector.y = 1.0F;
			FmPolys[i].UpVector.z = 0.0F;
			FmPolys[i].NextInTPage = (uint16) -1;
			FmPolys[i].PrevInTPage = (uint16) -1;
			FmPolys[i].Prev = (uint16) -1;

			fread( &FmPolys[ i ].Next, sizeof( uint16 ), 1, fp );
			i = FmPolys[ i ].Next;
		}
	}

	return( fp );
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif