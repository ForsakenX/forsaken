
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

/*===================================================================
	Includes
===================================================================*/
#include <math.h>
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include <time.h>
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dtextures.h"
#include "tload.h"

/*===================================================================
	Defines
===================================================================*/
#define	EXPLO_DISTANCE		( 256.0F * GLOBAL_SCALE )
#define MAXNUMOF2DPOLYS		2000
#define	MAXVERTSPER2DPOLY	4
#define MaxColDistance		( 64000.0F * GLOBAL_SCALE )
#define	MAXFMPOLYVERTS		700

/*===================================================================
	Faceme Sequences
===================================================================*/
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
	
/*===================================================================
	Faceme Flags
===================================================================*/
#define	FM_FLAG_ZERO		0							// Do Nothing
#define	FM_FLAG_MOVEOUT		1							// Move 2D Sprite toward Camera.
#define	FM_FLAG_DIRCONST	2							// Direction and Up Vector Constant.
#define	FM_FLAG_TWOSIDED	4							// Double Sided Poly
#define	FM_FLAG_DONTCLIP	8							// Don't Clip
#define	FM_FLAG_RELATIVE	16							// Direction and Up Vector Relative to camera.

/*===================================================================
	Make Colour Mode ( MakeColourMode Values )
===================================================================*/
enum
{
	MCM_Normal,					// Actual Colours
	MCM_Stipple,				// Stipple Card Colours
	MCM_Software,				// Software Emulation Colours
};

/*===================================================================
		TPage Info Structure for FmPolys/Polys/ScreenPolys
===================================================================*/
typedef struct TPAGEINFO {

	uint16		FirstPoly;		// Index to first Poly in TPage
	int16		StartVert;		// Filled in by PolyDisp
	int16		NumVerts;		// Filled in by PolyDisp
	int16		NumTris;		// Filled in by PolyDisp

} TPAGEINFO;

/*===================================================================
	FmPoly Structure
===================================================================*/
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


/*===================================================================
	Protptypes
===================================================================*/
void InitFmPoly( void );
void KillUsedFmPoly( uint16 i );
uint16 FindFreeFmPoly();
void FmPolyProcess( void );
void InitFmPolyTPages( void );
void AddFmPolyToTPage( uint16 i, int16 TPage );
void RemoveFmPolyFromTPage( uint16 i, int16 TPage );
BOOL DisplayGroupClippedFmPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, uint16 Group /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL DisplayGroupUnclippedFmPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL FmPolyDispGroupClipped( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextFmPoly );
BOOL FmPolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextFmPoly );
FILE * SaveFmPolys( FILE * fp );
FILE * LoadFmPolys( FILE * fp );

#endif // TWODPOLYS_INCLUDED


