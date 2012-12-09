#ifndef __New3d_h
#define __New3d_h
#include <math.h>
#include "main.h"

// taken from d3d9.h

typedef DWORD COLOR; // bgra

/*===================================================================
	Defines
===================================================================*/

#define	ourX					0
#define	ourY					1
#define	ourZ					2
#define	EPS					1e-7
#define PI					3.14159265358979323846F
#ifndef M_PI // defined in mingw
#define M_PI				3.14159265358979323846F
#endif
#ifndef M_PI_2 // pi/2
#define M_PI_2				1.57079632679489661923
#endif
#define	D2R(x)				( ( x ) * ( 1.0F / 57.2957F) )		/* Nick Pelling changed */
#define	R2D(x)				( ( x ) * 57.2957F )
#define	SINR(x)				sin( x )
#define	COSR(x)				cos( x )
#define	SIND(x)				( sin( D2R( x ) ) )
#define	COSD(x)				( cos( D2R( x ) ) )
#define ATAND(y, x)			R2D( atan2(y, x) )
#define FMOD( NUM, DIV )	( (NUM) - (DIV) * ( (float) floor( (NUM) / (DIV) ) ) )
#define FDIV( NUM, DIV )	( (DIV) * ( (float) floor( (NUM) / (DIV) ) ) )

/*===================================================================
	Structures
===================================================================*/

// bjd - taken from d3dtypes.h
#define RGBA_MAKE(r, g, b, a)   ((COLOR) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define	RGB_MAKE(r, g, b)    ((COLOR) (((r) << 16) | ((g) << 8) | (b)))

// COLOR is packed bgra
#define RGBA_GETALPHA(rgb)    ((rgb) >> 24)
#define RGBA_GETRED(rgb)    (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)    (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)    ((rgb) & 0xff)

#define RENDERVAL(val)    ((float)val)

/*
	Pre-DX8 vertex formats
	taken from http://www.mvps.org/directx/articles/definitions_for_dx7_vertex_types.htm
*/

typedef struct {
    union {
       float x;
       float dvX;
    };
    union {
       float y;
        float dvY;
    };
    union {
        float z;
        float dvZ;
    };
    union {
        COLOR color;
        COLOR dcColor;
    };
    union {
        float tu;
        float dvTU;
    };
    union {
        float tv;
        float dvTV;
	};
} LVERTEX, *LPLVERTEX;

typedef struct {
    union {
		float x;
        float sx;
        float dvSX;
    };
    union {
		float y;
        float sy;
        float dvSY;
    };
    union {
	    float z;
        float sz;
        float dvSZ;
    };
    union {
		float w;
        float rhw;
        float dvRHW;
    };
    union {
        COLOR color;
        COLOR dcColor;
    };
    union {
        float tu;
        float dvTU;
    };
    union {
        float tv;
        float dvTV;
    };

} TLVERTEX, *LPTLVERTEX;

typedef struct {
    union {
        WORD v1;
        WORD wV1;
    };
    union {
        WORD v2;
        WORD wV2;
    };
    union {
        WORD v3;
        WORD wV3;
    };
//    WORD     wFlags;
} TRIANGLE, *LPTRIANGLE;


/*===================================================================
	2D Vertices
===================================================================*/
typedef struct VERT2D {
	float	x;
	float	y;
} VERT2D;
/*===================================================================
	3D Vertices
===================================================================*/
typedef struct VERT {
	float	x;
	float	y;
	float	z;
} VERT;

/*===================================================================
	3D Normal
===================================================================*/
typedef struct NORMAL {
	union { float nx; float x; };
	union { float ny; float y; };
	union { float nz; float z; };
} NORMAL;

/*===================================================================
	4 X 4 Matrix
===================================================================*/
typedef struct MATRIX {
	float	_11, _12, _13, _14;
	float	_21, _22, _23, _24;
	float	_31, _32, _33, _34;
	float	_41, _42, _43, _44;
} MATRIX;

/*===================================================================
	3 X 3 Matrix
===================================================================*/
typedef struct MATRIX3X3 {
	float	_11, _12, _13;
	float	_21, _22, _23;
	float	_31, _32, _33;
} MATRIX3X3;


/*===================================================================
	Vector
===================================================================*/
typedef struct VECTOR {
	float	x;
	float	y;
	float	z;
} VECTOR;

/*===================================================================
	Short Vector
===================================================================*/
typedef struct SHORTVECTOR {
	int16_t	x;
	int16_t	y;
	int16_t	z;
} SHORTVECTOR;


/*===================================================================
	Plane
===================================================================*/
typedef struct PLANE {
	VECTOR Normal;
	float Offset;
} PLANE;

/*===================================================================
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

u_int16_t	Random_Range( u_int16_t Max );
float Random_Range_Float( float Max );


float	QuickDistance( VECTOR * V );
float	QuickDistance2d( float x , float y );
void	MakeViewMatrix(VECTOR *viewpos, VECTOR *lookpos, VECTOR *up, MATRIX *view);
void CalcViewAxes(VECTOR *viewpos, VECTOR *lookpos, VECTOR *up, VECTOR *vx, VECTOR *vy, VECTOR *vz);
void ScaleMatrix( MATRIX * m, VECTOR * v );
void MatrixFromAxisAndAngle( float angle, VECTOR * axis, MATRIX * rot );
void spline(VECTOR * p, float t, VECTOR * p1, VECTOR * p2, VECTOR * p3, VECTOR * p4);
void MxV( MATRIX * m, VECTOR * v0, VECTOR * v1 );

#endif
