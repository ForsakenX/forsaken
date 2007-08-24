/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 18 $
 *
 * $Header: /PcProjectX/Include/Lights.h 18    17/02/98 15:00 Collinsd $
 *
 * $Log: /PcProjectX/Include/Lights.h $
 * 
 * 18    17/02/98 15:00 Collinsd
 * Added load/save functions for triggers and xlights..
 * 
 * 17    1/19/98 12:19p Phillipd
 * 
 * 16    1/16/98 2:54p Phillipd
 * 
 * 15    6/11/97 11:37 Collinsd
 * Added BGObject Lighting, Stopped primary weapons creating lines when
 * outside group and made secondary weapons create debug lines when no
 * collision.
 * 
 * 14    7/09/97 2:31p Phillipd
 * 
 * 13    7/08/97 12:51p Phillipd
 * 
 * 12    3/10/97 9:53a Phillipd
 * 
 * 11    3/07/97 9:51a Phillipd
 * 
 * 10    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 9     10/29/96 2:52p Phillipd
 * 
 * 8     19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 7     10/17/96 10:37a Phillipd
 * 
 * 6     10/14/96 2:41p Phillipd
 * Directional Lights are there...allmost
 * 
 * 5     10/10/96 4:18p Phillipd
 * removed mesh.h..no longer used..
 * 
 * 4     8/09/96 2:02p Phillipd
 * 
 * 3     7/21/96 4:27p Phillipd
 * added asynchrinus(??) execution ..so lights happen at the same time as
 * the last group is being displayed...
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

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
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include <time.h>
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "Mxload.h"
#include "Mxaload.h"
#include "collision.h"
#include "ddutil.h"

/*
 * defines
 */
#define	MAXXLIGHTS	256
#define	POINT_LIGHT	0
#define	SPOT_LIGHT	1
 
 
/*
 * structures
 */

 typedef struct XLIGHT{
	int		Type;
	BOOL	Visible;
	uint16	Index;
	float	r;
	float	g;
	float	b;
	float	Size;
	float	SizeCount;
	float	ColorCount;
	float	CosArc;			// how wide is my light.......
	uint16	Next;			// Next Free
	uint16	Prev;			// Previous used...
	VECTOR	Pos;
	VECTOR	Dir;			// Normalised....
	uint16	Group;
struct XLIGHT * NextVisible;
 }XLIGHT;

 /*
 * fn prototypes
 */

BOOL	ProcessXLights( MLOADHEADER * Mloadheader );
void	InitXLights();
uint16	FindFreeXLight();
void	KillUsedXLight(uint16);

//BOOL	DoIEffectThisGroup( MLOADHEADER * MloadHeader , VECTOR * Pos , float size , uint16 group);

void	SetLightDie ( uint16 light );
BOOL	XLight1Group( MLOADHEADER * Mloadheader, uint16 group );

BOOL	XLightMxloadHeader( MXLOADHEADER * MXloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix );
BOOL	XLightMxaloadHeader( MXALOADHEADER * MXAloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix );


void	CreateCellColours( MLOADHEADER * Mloadheader );
D3DCOLOR FindNearestCellColour( MLOADHEADER * Mloadheader,VECTOR * Pos, uint16 group );
void BuildVisibleLightList( uint16 Group );
FILE * SaveXLights( FILE * fp );
FILE * LoadXLights( FILE * fp );

#endif // LIGHTS_INCLUDED


