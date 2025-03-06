

/*==========================================================================
 *
 *
 *  File:       Lights.h
 *  Content:    Lights.c include file
 *
 *
 ***************************************************************************/

#ifndef LIGHTS_INCLUDED
#define LIGHTS_INCLUDED

#include <math.h>
#include "main.h"

#include "new3d.h"
#include "ships.h"
#include "mload.h"
#include "mxload.h"
#include "mxaload.h"
#include "collision.h"

/*
 * defines
 */
#define	MAXXLIGHTS	256
#define	POINT_LIGHT	0
#define	SPOT_LIGHT	1
 
#define MIN_LIGHT_SIZE	( 1536.0F * GLOBAL_SCALE )
 
/*
 * structures
 */

 typedef struct XLIGHT{
	int		Type;
	bool	Visible;
	u_int16_t	Index;
	float	r;
	float	g;
	float	b;
	float	Size;
	float	SizeCount;
	float	ColorCount;
	float	CosArc;			// how wide is my light.......
	u_int16_t	Next;			// Next Free
	u_int16_t	Prev;			// Previous used...
	VECTOR	Pos;
	VECTOR	Dir;			// Normalised....
	u_int16_t	Group;
struct XLIGHT * NextVisible;
 }XLIGHT;

 /*
 * fn prototypes
 */

bool	ProcessXLights( MLOADHEADER * Mloadheader );
void	InitXLights();
u_int16_t	FindFreeXLight();
void	KillUsedXLight(u_int16_t);

void	SetLightDie ( u_int16_t light );
bool	XLight1Group( MLOADHEADER * Mloadheader, u_int16_t group );

bool	XLightMxloadHeader( MXLOADHEADER * MXloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix );
bool	XLightMxaloadHeader( MXALOADHEADER * MXAloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix );


void	CreateCellColours( MLOADHEADER * Mloadheader );
COLOR FindNearestCellColour( MLOADHEADER * Mloadheader,VECTOR * Pos, u_int16_t group );
void BuildVisibleLightList( u_int16_t Group );
FILE * SaveXLights( FILE * fp );
FILE * LoadXLights( FILE * fp );

#endif // LIGHTS_INCLUDED


