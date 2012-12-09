/*==========================================================================
 *  File:       p o l y s . h
 ***************************************************************************/
#ifndef POLYS_INCLUDED
#define POLYS_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include "main.h"
#include <math.h>

#include "new3d.h"
#include "quat.h"
#include "ships.h"
#include "mload.h"
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

	u_int8_t	R;
	u_int8_t	G;
	u_int8_t	B;

} POLY_RGB;

typedef struct POLY {

	u_int16_t			Next;
	u_int16_t			Prev;

	u_int16_t			NextInTPage;
	u_int16_t			PrevInTPage;

	VECTOR			Pos1;				// Position 1
	VECTOR			Pos2;				// Position	2
	VECTOR			Pos3;				// Position	3
	VECTOR			Pos4;				// Position	4
	POLY_RGB		Col1;				// Colour 1
	POLY_RGB		Col2;				// Colour 2
	POLY_RGB		Col3;				// Colour 3
	POLY_RGB		Col4;				// Colour 4
	u_int8_t			Trans1;				// Amount of transparency 1
	u_int8_t			Trans2;				// Amount of transparency 2
	u_int8_t			Trans3;				// Amount of transparency 3
	u_int8_t			Trans4;				// Amount of transparency 4
	int16_t			Flags;				// Flags
	float			Scale;				// Scale Factor
	float			Frame;				// Animation Frame
	float			AnimSpeed;			// Animation Speed
	FRAME_INFO	**	Frm_Info;			// Offset Info
	int16_t			SeqNum;				// Process Sequence
	u_int16_t			Group;				// Group
	float			TimeStep;			// Time step for interpolation
	QUATLERP		Qlerp;				// Quaternion Interpolation 
	QUAT			Quat;				// Quat
	u_int16_t			Ship;				// Ship

} POLY;

/*===================================================================
	Prototypes
===================================================================*/
void InitPolys( void );
void KillUsedPoly( u_int16_t i );
u_int16_t FindFreePoly( void );
void ProcessPolys( void );
void DoAfterBurnerEffects( void );
void InitAfterBurners( void );
void InitPolyTPages( void );
void AddPolyToTPage( u_int16_t i, int16_t TPage );
void RemovePolyFromTPage( u_int16_t i, int16_t TPage );
bool DisplayGroupClippedPolys( RENDEROBJECT *renderObject, u_int16_t Group );
bool DisplayGroupUnclippedPolys( RENDEROBJECT *renderObject );
bool PolyDispGroupClipped( u_int16_t Group, RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly );
bool PolyDispGroupUnclipped( RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly );

bool DisplaySolidGroupClippedPolys( RENDEROBJECT *renderObject, u_int16_t Group );
bool DisplaySolidGroupUnclippedPolys( RENDEROBJECT *renderObject );
bool SolidPolyDispGroupClipped( u_int16_t Group, RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly );
bool SolidPolyDispGroupUnclipped( RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly );

FILE * SavePolys( FILE * fp );
FILE * LoadPolys( FILE * fp );

#endif


