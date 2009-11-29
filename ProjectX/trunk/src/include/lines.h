/*==========================================================================
 *  L i n e s . h
 ***************************************************************************/
#ifndef LINES_INCLUDED
#define LINES_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include <math.h>

#include "new3d.h"
#include <time.h>
#include "ships.h"
#include "mload.h"
#include "collision.h"
#include "lights.h"
#include "2dtextures.h"
#include "tload.h"

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
BOOL ExecuteLines( uint16 group, RENDEROBJECT *renderObject );
void KillUsedLine( uint16 i );
uint16 FindFreeLine( void );

#endif


