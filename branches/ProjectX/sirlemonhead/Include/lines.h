/*==========================================================================
 *  L i n e s . h
 ***************************************************************************/
#ifndef LINES_INCLUDED
#define LINES_INCLUDED

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
#include "ddutil.h"
#include "lights.h"
#include "2dtextures.h"
#include "tload.h"
#include "d3dx9core.h"

/*===================================================================
	Defines
===================================================================*/
#define MAXLINES			2000

/*===================================================================
	Structures
===================================================================*/
typedef struct MY_RGB {

	uint8		R;
	uint8		G;
	uint8		B;

} MY_RGB;

typedef struct LINE {

	uint16		Next;
	uint16		Prev;
	VECTOR		StartPos;			// Start Position
	VECTOR		EndPos;				// End Position
	MY_RGB		StartCol;			// Start Colour
	MY_RGB		EndCol;				// End Colour
	uint8		StartTrans;			// Start Transparency
	uint8		EndTrans;			// End Transparency
	uint16		Group;				// Group ImIn

} LINE;


/*===================================================================
	Protptypes
===================================================================*/
void InitLines( void );
BOOL LinesDispGroup( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, uint16 * StartLine );
void KillUsedLine( uint16 i );
uint16 FindFreeLine( void );

#endif


