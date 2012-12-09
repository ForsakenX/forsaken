/*==========================================================================
 *  L i n e s . h
 ***************************************************************************/
#ifndef LINES_INCLUDED
#define LINES_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include "main.h"
#include <math.h>

#include "new3d.h"
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

	u_int8_t		R;
	u_int8_t		G;
	u_int8_t		B;

} MY_RGB;

typedef struct LINE {

	u_int16_t		Next;
	u_int16_t		Prev;
	VECTOR		StartPos;			// Start Position
	VECTOR		EndPos;				// End Position
	MY_RGB		StartCol;			// Start Colour
	MY_RGB		EndCol;				// End Colour
	u_int8_t		StartTrans;			// Start Transparency
	u_int8_t		EndTrans;			// End Transparency
	u_int16_t		Group;				// Group ImIn

} LINE;


/*===================================================================
	Protptypes
===================================================================*/
void InitLines( void );
bool ExecuteLines( u_int16_t group, RENDEROBJECT *renderObject );
void KillUsedLine( u_int16_t i );
u_int16_t FindFreeLine( void );

#endif


