/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 21 $
 *
 * $Header: /PcProjectX/Include/New3d.h 21    1/29/98 9:12a Phillipd $
 *
 * $Log: /PcProjectX/Include/New3d.h $
 * 
 * 21    1/29/98 9:12a Phillipd
 * 
 * 20    11/06/97 5:39p Phillipd
 * 
 * 19    29/10/97 17:24 Collinsd
 * Added new destructable bgobject.
 * 
 * 18    10/25/97 3:50p Phillipd
 * 
 * 17    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 16    17/10/97 10:45 Collinsd
 * Moved FMOD to new3d.h
 * 
 * 15    16/09/97 11:00 Collinsd
 * Added Chris's code
 * 
 * 14    10-07-97 6:05p Philipy
 * added PLANE type
 * 
 * 13    15-05-97 7:10a Philipy
 * changed TAND macro to ATAND (arctangent)
 * 
 * 12    14-05-97 10:05a Philipy
 * added TAND macro
 * 
 * 11    24-02-97 10:23a Collinsd
 * Added component bgobjects.
 * 
 * 10    10/30/96 2:55p Phillipd
 * 
 * 9     9/10/96 9:00a Phillipd
 * 
 * 8     21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 7     7/26/96 11:46a Phillipd
 * 
 * 6     8/07/96 10:53 Collinsd
 * Added random number function.
 * 
 * 5     6/27/96 12:40p Phillipd
 * 
 * 4     6/27/96 12:23p Phillipd
 * removed d3dvertex types from routine prototypes
 * 
 * 3     25/06/96 14:10 Collinsd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

#ifndef __New3d_h
#define __New3d_h
#include <math.h>
#include "d3ddemo.h"
#include "typedefs.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	X					0
#define	Y					1
#define	Z					2
#define	EPS					1e-7
#define PI					3.14159265358979323846F
#define M_PI				3.14159265358979323846F
#define	D2R(x)				( ( x ) * ( 1.0F / 57.2957F) )		/* Nick Pelling changed */
#define	R2D(x)				( ( x ) * 57.2957F )
#define	SINR(x)				sin( x )
#define	COSR(x)				cos( x )
#define	SIND(x)				( sin( D2R( x ) ) )
#define	COSD(x)				( cos( D2R( x ) ) )
#define ATAND(y, x)			R2D( atan2(y, x) )
#define FMOD( NUM, DIV )	( (NUM) - (DIV) * ( (float) floor( (NUM) / (DIV) ) ) )
#define FDIV( NUM, DIV )	( (DIV) * ( (float) floor( (NUM) / (DIV) ) ) )

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Structures
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	2D Vertices
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct VERT2D {
	float	x;
	float	y;
} VERT2D;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	3D Vertices
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct VERT {
	float	x;
	float	y;
	float	z;
} VERT;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	3D Normal
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct NORMAL {
	float	nx;
	float	ny;
	float	nz;
} NORMAL;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	4 X 4 Matrix
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct MATRIX {
	float	_11, _12, _13, _14;
	float	_21, _22, _23, _24;
	float	_31, _32, _33, _34;
	float	_41, _42, _43, _44;
} MATRIX;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	3 X 3 Matrix
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct MATRIX3X3 {
	float	_11, _12, _13;
	float	_21, _22, _23;
	float	_31, _32, _33;
} MATRIX3X3;


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Vector
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct VECTOR {
	float	x;
	float	y;
	float	z;
} VECTOR;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Short Vector
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct SHORTVECTOR {
	int16	x;
	int16	y;
	int16	z;
} SHORTVECTOR;


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Plane
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef struct PLANE {
	VECTOR Normal;
	float Offset;
} PLANE;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Prototypes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float DotProduct( VECTOR * a , VECTOR * b ); 

void CrossProduct( VECTOR * a, VECTOR * b, VECTOR * ab );

void NormaliseVector( VECTOR *  v );


void BuildRotMatrix( float xa, float ya, float za, MATRIX * m );
void MatrixMultiply( MATRIX * m0, MATRIX * m1, MATRIX * m0m1 );
void ApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 );
void AddMatrixTrans( float xt, float yt, float zt, MATRIX * m );

void ReflectVector( VECTOR * old, NORMAL * normal, VECTOR * new1 );

float VectorLength( VECTOR * v ) ;

float DistanceVert2Vector( VERT *  a , VECTOR * b ) ;
float DistanceVector2Vector( VECTOR *  a , VECTOR * b );


void VisPolyApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 );

void MatrixTranspose( MATRIX * m1, MATRIX * m2 );

uint16	Random_Range( uint16 Max );
float Random_Range_Float( float Max );


float	QuickDistance( VECTOR * V );
float	QuickDistance2d( float x , float y );
void	MakeViewMatrix(VECTOR *viewpos, VECTOR *lookpos, VECTOR *up, MATRIX *view);
void CalcViewAxes(VECTOR *viewpos, VECTOR *lookpos, VECTOR *up, VECTOR *vx, VECTOR *vy, VECTOR *vz);
void ScaleMatrix( MATRIX * m, VECTOR * v );
void MatrixFromAxisAndAngle( float angle, VECTOR * axis, MATRIX * rot );
void spline(VECTOR * p, float t, VECTOR * p1, VECTOR * p2, VECTOR * p3, VECTOR * p4);

#ifdef SOFTWARE_ENABLE
/*-----------------------------------------------------------------------------
	Chris Walsh's Code
-----------------------------------------------------------------------------*/
void CWVisPolyApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 );
/*---------------------------------------------------------------------------*/
#endif

#endif
