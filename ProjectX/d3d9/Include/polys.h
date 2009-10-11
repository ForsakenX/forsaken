/*==========================================================================
 *  File:       p o l y s . h
 ***************************************************************************/
#ifndef POLYS_INCLUDED
#define POLYS_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include <math.h>
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include "quat.h"
#include <time.h>
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dtextures.h"
#include "2dpolys.h"
#include "tload.h"

/*===================================================================
	Defines
===================================================================*/
#define MAXPOLYS			2000
#define	MAXPOLYVERTS		800

#define	POLY_FLAG_NOTHING	0
#define	POLY_FLAG_TWOSIDED	1
#define	POLY_FLAG_DONTCLIP	2
#define	POLY_FLAG_SOLID		4

#define	POLY_NOTHING		0
#define	POLY_LASER			1
#define	POLY_PULSAR_TRAIL	2
#define	POLY_SHIELD			3
#define	POLY_AFTERBURNER	4
#define	POLY_SCATTER_TRAIL	5
#define	POLY_INVBITS		6
#define	POLY_POWERLASER		7

/*===================================================================
	Structures
===================================================================*/
typedef	struct POLY_RGB {

	uint8	R;
	uint8	G;
	uint8	B;

} POLY_RGB;

typedef struct POLY {

	uint16			Next;
	uint16			Prev;

	uint16			NextInTPage;
	uint16			PrevInTPage;

	VECTOR			Pos1;				// Position 1
	VECTOR			Pos2;				// Position	2
	VECTOR			Pos3;				// Position	3
	VECTOR			Pos4;				// Position	4
	POLY_RGB		Col1;				// Colour 1
	POLY_RGB		Col2;				// Colour 2
	POLY_RGB		Col3;				// Colour 3
	POLY_RGB		Col4;				// Colour 4
	uint8			Trans1;				// Amount of transparency 1
	uint8			Trans2;				// Amount of transparency 2
	uint8			Trans3;				// Amount of transparency 3
	uint8			Trans4;				// Amount of transparency 4
	int16			Flags;				// Flags
	float			Scale;				// Scale Factor
	float			Frame;				// Animation Frame
	float			AnimSpeed;			// Animation Speed
	FRAME_INFO	**	Frm_Info;			// Offset Info
	int16			SeqNum;				// Process Sequence
	uint16			Group;				// Group
	float			TimeStep;			// Time step for interpolation
	QUATLERP		Qlerp;				// Quaternion Interpolation 
	QUAT			Quat;				// Quat
	uint16			Ship;				// Ship

} POLY;

/*===================================================================
	Prototypes
===================================================================*/
void InitPolys( void );
void KillUsedPoly( uint16 i );
uint16 FindFreePoly( void );
void ProcessPolys( void );
void DoAfterBurnerEffects( void );
void InitAfterBurners( void );
void InitPolyTPages( void );
void AddPolyToTPage( uint16 i, int16 TPage );
void RemovePolyFromTPage( uint16 i, int16 TPage );
BOOL DisplayGroupClippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, uint16 Group
								/*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL DisplayGroupUnclippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject
								   /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL PolyDispGroupClipped( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly );
BOOL PolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly );

BOOL DisplaySolidGroupClippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, uint16 Group
								/*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL DisplaySolidGroupUnclippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject
								   /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ); // bjd
BOOL SolidPolyDispGroupClipped( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly );
BOOL SolidPolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly );

FILE * SavePolys( FILE * fp );
FILE * LoadPolys( FILE * fp );

#endif


