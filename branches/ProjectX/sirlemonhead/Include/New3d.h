
#ifndef __New3d_h
#define __New3d_h
#include <math.h>
#include "d3ddemo.h"
#include "typedefs.h"

/*/*===================================================================
	Defines
===================================================================*/
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

/*/*===================================================================
	Structures
===================================================================*/

/*/*===================================================================
	2D Vertices
===================================================================*/
typedef struct VERT2D {
	float	x;
	float	y;
} VERT2D;
/*/*===================================================================
	3D Vertices
===================================================================*/
typedef struct VERT {
	float	x;
	float	y;
	float	z;
} VERT;

/*/*===================================================================
	3D Normal
===================================================================*/
typedef struct NORMAL {
	float	nx;
	float	ny;
	float	nz;
} NORMAL;

/*/*===================================================================
	4 X 4 Matrix
===================================================================*/
typedef struct MATRIX {
	float	_11, _12, _13, _14;
	float	_21, _22, _23, _24;
	float	_31, _32, _33, _34;
	float	_41, _42, _43, _44;
} MATRIX;

/*/*===================================================================
	3 X 3 Matrix
===================================================================*/
typedef struct MATRIX3X3 {
	float	_11, _12, _13;
	float	_21, _22, _23;
	float	_31, _32, _33;
} MATRIX3X3;


/*/*===================================================================
	Vector
===================================================================*/
typedef struct VECTOR {
	float	x;
	float	y;
	float	z;
} VECTOR;

/*/*===================================================================
	Short Vector
===================================================================*/
typedef struct SHORTVECTOR {
	int16	x;
	int16	y;
	int16	z;
} SHORTVECTOR;


/*/*===================================================================
	Plane
===================================================================*/
typedef struct PLANE {
	VECTOR Normal;
	float Offset;
} PLANE;

/*/*===================================================================
	Prototypes
===================================================================*/
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

#endif
