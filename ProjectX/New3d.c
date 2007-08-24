/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 32 $
 *
 * $Header: /PcProjectX/New3d.c 32    4/22/98 10:46a Phillipd $
 *
 * $Log: /PcProjectX/New3d.c $
 * 
 * 32    4/22/98 10:46a Phillipd
 * 
 * 31    4/09/98 11:30a Phillipd
 * 
 * 30    19/03/98 20:32 Philipy
 * added different end of game scenarios
 * code now written to config to indicate if secret biker is available
 * 
 * 29    16/03/98 12:19 Collinsd
 * Random Pickups option added.  Sphere dislpay of trigger areas
 * 
 * 28    11/03/98 16:08 Oliverc
 * Fixed bug in NormaliseVector() that made it return crap for near-zero
 * (but non-zero) length vectors
 * 
 * 27    3/04/98 12:27p Phillipd
 * 
 * 26    11/06/97 5:39p Phillipd
 * 
 * 25    11/04/97 10:36a Phillipd
 * 
 * 24    10/29/97 12:25p Phillipd
 * 
 * 23    10/25/97 3:49p Phillipd
 * 
 * 22    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 21    16/09/97 10:59 Collinsd
 * Added Chris's code
 * 
 * 20    6/08/97 15:22 Oliverc
 * Fixed integer div 0 bug in Random_Range()
 * 
 * 19    5/07/97 16:31 Collinsd
 * Put OPT_ON's around opimisations off
 * 
 * 18    26/04/97 14:49 Collinsd
 * Optimisations now on def.
 * 
 * 17    2/27/97 3:46p Phillipd
 * 
 * 16    27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 15    24-02-97 10:23a Collinsd
 * Added component bgobjects.
 * 
 * 14    10/30/96 2:55p Phillipd
 * 
 * 13    29/09/96 17:31 Collinsd
 * Assembler DotProduct dosn't seem to work, so i've if'd the C one back
 * in.
 * 
 * 12    9/26/96 3:43p Phillipd
 * 
 * 11    9/13/96 4:46p Phillipd
 * 
 * 10    9/10/96 9:00a Phillipd
 * 
 * 9     4/09/96 20:01 Collinsd
 * Scatter missile and thief missile now work!
 * 
 * 8     21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 7     7/26/96 11:46a Phillipd
 * 
 * 6     8/07/96 10:53 Collinsd
 * Added random number function.
 * 
 * 5     6/07/96 16:36 Collinsd
 * Explosion moved away from wall, toward camera.
 * 
 * 4     6/27/96 12:40p Phillipd
 * 
 * 3     6/27/96 12:23p Phillipd
 * removed d3dvertex types from routine prototypes
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

#include "typedefs.h"
#include "New3D.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Globals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	Seed1 = 0x1234;
uint16	Seed2 = 0x4321;

MATRIX	MATRIX_Identity = {
			1.0F, 0.0F, 0.0F, 0.0F,
			0.0F, 1.0F, 0.0F, 0.0F,
			0.0F, 0.0F, 1.0F, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Build Rotation Matrix
	Input		:	float		X Rotations ( Degrees )
				:	float		Y Rotations ( Degrees )
				:	float		Z Rotations ( Degrees )
				:	MATRIX	*	New Matrix
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void BuildRotMatrix( float xa, float ya, float za, MATRIX * m )
{
	float	c0, c1, c2, s0, s1, s2, c0s1, s0s1;

	c0 = (float) COSD( xa );
	c1 = (float) COSD( ya );
	c2 = (float) COSD( za );

	s0 = (float) SIND( xa );
	s1 = (float) SIND( ya );
	s2 = (float) SIND( za );

	c0s1 = c0 * s1;
	s0s1 = s0 * s1;

	m->_11 = c1 * c2;
	m->_12 = -c1 * s2;
	m->_13 = -s1;
	m->_14 = 0.0F;

	m->_21 = ( ( c0 * s2 ) - ( s0s1 * c2 ) );
	m->_22 = ( ( s0s1 * s2 ) + ( c0 * c2 ) );
	m->_23 = -s0 * c1;
	m->_24 = 0.0F;

	m->_31 = ( ( c0s1 * c2 ) + ( s0 * s2 ) );
	m->_32 = ( ( s0 * c2 ) - ( c0s1 * s2 ) );
	m->_33 = c0 * c1;
	m->_34 = 0.0F;

	m->_41 = 0.0F;
	m->_42 = 0.0F;
	m->_43 = 0.0F;
	m->_44 = 1.0F;

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Matrix Multiplication
	Input		:	MATRIX	*	Matrix 1
				:	MATRIX	*	Matrix 2
				:	MATRIX	*	Result Matrix
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void MatrixMultiply( MATRIX * m0, MATRIX * m1, MATRIX * m0m1 )
{
	float	a, b, c, d, e, f, g, h, i;
	float	A, B, C, D, E, F, G, H, I;
	float   j, k, l, m;
	float   J, K, L, M;
	float	n, o, p;
	float	N, O, P;

	a = m0->_11; b = m0->_12; c = m0->_13;
	d = m0->_21; e = m0->_22; f = m0->_23;
	g = m0->_31; h = m0->_32; i = m0->_33;

	A = m1->_11; B = m1->_12; C = m1->_13;
	D = m1->_21; E = m1->_22; F = m1->_23;
	G = m1->_31; H = m1->_32; I = m1->_33;

	j = m0->_41; k = m0->_42; l = m0->_43; m = m0->_44;
	J = m1->_41; K = m1->_42; L = m1->_43; M = m1->_44;

	n = m0->_14; o = m0->_24; p = m0->_34;
	N = m1->_14; O = m1->_24; P = m1->_34;


	m0m1->_11 = ( A * a + D * b + G * c + J * n );
	m0m1->_12 = ( B * a + E * b + H * c + K * n );
	m0m1->_13 = ( C * a + F * b + I * c + L * n );
	m0m1->_14 = ( N * a + O * b + P * c + M * n );

	m0m1->_21 = ( A * d + D * e + G * f + J * o );
	m0m1->_22 = ( B * d + E * e + H * f + K * o );
	m0m1->_23 = ( C * d + F * e + I * f + L * o );
	m0m1->_24 = ( N * d + O * e + P * f + M * o );


	m0m1->_31 = ( A * g + D * h + G * i + J * p );
	m0m1->_32 = ( B * g + E * h + H * i + K * p );
	m0m1->_33 = ( C * g + F * h + I * i + L * p );
	m0m1->_34 = ( N * g + O * h + P * i + M * p );


	m0m1->_41 = ( j * A + k * D + l * G + m * J );
	m0m1->_42 = ( j * B + k * E + l * H + m * K );
	m0m1->_43 = ( j * C + k * F + l * I + m * L );
	m0m1->_44 = ( j * N + k * O + l * P + m * M );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Apply Matrix to a Vector
	Input		:	MATRIX	*	Matrix
				:	VECTOR	*	Vert
				:	VECTOR	*	New Vert
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
//#ifndef USEASM
#if 1
void ApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 )
{
	float x = m->_41;
	float y = m->_42;
	float z = m->_43;

	x += ( ( m->_11 * v0->x ) + ( m->_12 * v0->y ) + ( m->_13 * v0->z ) );
	y += ( ( m->_21 * v0->x ) + ( m->_22 * v0->y ) + ( m->_23 * v0->z ) );
	z += ( ( m->_31 * v0->x ) + ( m->_32 * v0->y ) + ( m->_33 * v0->z ) );

	v1->x = x;
	v1->y = y;
	v1->z = z;
}
#else
// assumes v[W] == 1.0f
//__declspec(naked) void MatMultVec3x4(Vector res, Matrix m, Vector v)
__declspec(naked) void ApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 )
{
//	res[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3];
//	res[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3];
//	res[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3];
	__asm
	{
		mov			eax, [esp + 12]
		mov			ecx, [esp + 4]
		mov			edx, [esp + 8]
		fld			dword ptr [ecx]
		fmul		dword ptr [edx]
		fld			dword ptr [ecx + 4]
		fmul		dword ptr [edx + 4]
		fld			dword ptr [ecx + 8]
		fmul		dword ptr [edx + 8]
		fld			dword ptr [ecx + 16]
		fmul		dword ptr [edx]
		fld			dword ptr [ecx + 16 + 4]
		fmul		dword ptr [edx + 4]
		fld			dword ptr [ecx + 16 + 8]
		fmul		dword ptr [edx + 8]
		fxch		st(5)
		fadd		dword ptr [ecx + 12]
		fxch		st(4)
		faddp		st(3), st
		fxch		st(1)
		fadd		dword ptr [ecx + 16 + 12]
		fxch		st(4)
		fld			dword ptr [ecx + 32]
		fmul		dword ptr [edx]
		fld			dword ptr [ecx + 32 + 4]
		fmul		dword ptr [edx + 4]
		fld			dword ptr [ecx + 32 + 8]
		fmul		dword ptr [edx + 8]
		fxch		st(4)
		faddp		st(3), st
		fxch		st(1)
		fadd		dword ptr [ecx + 32 + 12]
		fxch		st(5)
		faddp		st(4), st
		faddp		st(2), st
		faddp		st(4), st
		fxch		st(1)
		fstp		dword ptr [eax]
		faddp		st(1), st
		fxch		st(1)
		fstp		dword ptr [eax + 4]
		fstp		dword ptr [eax + 8]
		ret
	}
}

#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Apply Matrix to a Vector
	Input		:	MATRIX	*	Matrix
				:	VECTOR	*	Vert
				:	VECTOR	*	New Vert
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void VisPolyApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 )
{
	float w;
	v1->x = ( ( m->_11 * v0->x ) + ( m->_21 * v0->y ) + ( m->_31 * v0->z ) +m->_41 );
	v1->y = ( ( m->_12 * v0->x ) + ( m->_22 * v0->y ) + ( m->_32 * v0->z ) +m->_42);
	v1->z = ( ( m->_13 * v0->x ) + ( m->_23 * v0->y ) + ( m->_33 * v0->z ) +m->_43);
	w	  = ( ( m->_14 * v0->x ) + ( m->_24 * v0->y ) + ( m->_34 * v0->z ) +m->_44);
	v1->x /= w;
	v1->y /= w;
	v1->z /= w;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Add XYZ Translation to Matrix
	Input		:	float		X Translation
				:	float		Y Translation
				:	float		Z Translation
				:	MATRIX	*	Matrix to add translation
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddMatrixTrans( float xt, float yt, float zt, MATRIX * m )
{
	m->_41 = xt;
	m->_42 = yt;
	m->_43 = zt;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate the Cross product of 2 Vectors
	Input		:	VECTOR	*	Vector1
				:	VECTOR	*	Vector2
				:	VECTOR	*	New Vector
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#ifndef USEASM
void CrossProduct( VECTOR * a, VECTOR * b, VECTOR * ab )
{
	ab->x = ( a->y * b->z ) - ( a->z * b->y );
	ab->y = ( a->z * b->x ) - ( a->x * b->z );
	ab->z = ( a->x * b->y ) - ( a->y * b->x );
}
#else
__declspec(naked) void CrossProduct(VECTOR * v, VECTOR * v1, VECTOR * v2)
{
	// v[X] = v1[Y]*v2[Z] - v1[Z]*v2[Y] = A - B
	// v[Y] = v1[Z]*v2[X] - v1[X]*v2[Z] = C - D
	// v[Z] = v1[X]*v2[Y] - v1[Y]*v2[X] = E - F
	__asm
	{
		mov			eax, [esp + 12]
		mov			ecx, [esp + 4]
		mov			edx, [esp + 8]

		fld			dword ptr [ecx + 4]
		// v1[Y]
		fmul		dword ptr [edx + 8]
		// v1[Y]*v2[Z] (2)
		fld			dword ptr [ecx + 8]
		// v1[Z]
		// v1[Y]*v2[Z] (1)
		fmul		dword ptr [edx + 4]
		// v1[Z]*v2[Y] (2)
		// v1[Y]*v2[Z] (0)
		fld			dword ptr [ecx + 8]
		// v1[Z]
		// v1[Z]*v2[Y] (1)
		// v1[Y]*v2[Z] (0)
		fmul		dword ptr [edx]
		// v1[Z]*v2[X] (2)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fld			dword ptr [ecx]
		// v1[X]
		// v1[Z]*v2[X] (1)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fmul		dword ptr [edx + 8]
		// v1[X]*v2[Z] (2)
		// v1[Z]*v2[X] (0)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fld			dword ptr [ecx]
		// v1[X]
		// v1[X]*v2[Z] (1)
		// v1[Z]*v2[X] (0)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fmul		dword ptr [edx + 4]
		// v1[X]*v2[Y] (2)
		// v1[X]*v2[Z] (0)
		// v1[Z]*v2[X] (0)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fld			dword ptr [ecx + 4]
		// v1[Y]
		// v1[X]*v2[Y] (1)
		// v1[X]*v2[Z] (0)
		// v1[Z]*v2[X] (0)
		// v1[Z]*v2[Y] (0)
		// v1[Y]*v2[Z] (0)
		fmul		dword ptr [edx]
		// v1[Y]*v2[X] = F (2)
		// v1[X]*v2[Y] = E (0)
		// v1[X]*v2[Z] = D (0)
		// v1[Z]*v2[X] = C (0)
		// v1[Z]*v2[Y] = B (0)
		// v1[Y]*v2[Z] = A (0)
		fxch		st(5)
		// v1[Y]*v2[Z] = A (0)
		// v1[X]*v2[Y] = E (0)
		// v1[X]*v2[Z] = D (0)
		// v1[Z]*v2[X] = C (0)
		// v1[Z]*v2[Y] = B (0)
		// v1[Y]*v2[X] = F (2)
		fsubrp	st(4), st
		// v1[X]*v2[Y] = E (0)
		// v1[X]*v2[Z] = D (0)
		// v1[Z]*v2[X] = C (0)
		// A - B (2)
		// v1[Y]*v2[X] = F (1)
		fxch		st(2)
		// v1[Z]*v2[X] = C (0)
		// v1[X]*v2[Z] = D (0)
		// v1[X]*v2[Y] = E (0)
		// A - B (2)
		// v1[Y]*v2[X] = F (1)
		fsubrp	st(1), st
		// C - D (2)
		// v1[X]*v2[Y] = E (0)
		// A - B (1)
		// v1[Y]*v2[X] = F (0)
		fxch		st(1)
		// v1[X]*v2[Y] = E (0)
		// C - D (2)
		// A - B (1)
		// v1[Y]*v2[X] = F (0)
		fsubrp	st(3), st
		// C - D (1)
		// A - B (0)
		// E - F (2)
		fxch		st(1)
		// A - B (0)
		// C - D (1)
		// E - F (2)
		// ******** 1 clock stall (needs result 1 cycle in advance) ********
		fstp		dword ptr [eax]
		fstp		dword ptr [eax + 4]
		fstp		dword ptr [eax + 8]

		ret
	}
}
#endif
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate the Dod product of 2 VECTORS
	Input		:	VECTOR * a
				:	VECTOR * b
	Output		:	float		Dot Product
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#ifndef USEASM
float DotProduct( VECTOR * a , VECTOR * b ) 
{
	return( ( a->x * b->x ) +
		    ( a->y * b->y ) +
		    ( a->z * b->z ) );
}
#else
__declspec(naked) float DotProduct( VECTOR * a, VECTOR * b)
{
	// a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z]
	__asm
	{
		mov			ecx, [esp + 4]
		mov			edx, [esp + 8]
		fld			dword ptr [ecx]
		fmul		dword ptr [edx]
		fld			dword ptr [ecx + 4]
		fmul		dword ptr [edx + 4]
		fld			dword ptr [ecx + 8]
		fmul		dword ptr [edx + 8]
		fxch		st(2)
		faddp		st(1), st
		faddp		st(1), st
		ret
	}
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Reflect Vector off a surface normal
	Input		:	VECTOR	*	Vector
				:	NORMAL	*	Normal
				:	VECTOR	*	New Vector
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReflectVector( VECTOR * old, NORMAL * normal, VECTOR * new1 )
{
   float        lambda;

   lambda= ( -2 * ( (normal->nx * old->x ) + (normal->ny * old->y ) + (normal->nz * old->z ) ) );

   if( fabs( lambda ) < EPS )
   {
        new1->x = old->x;
        new1->y = old->y;
        new1->z = old->z;
   }
   else
   {
        new1->x = ( old->x + ( lambda * normal->nx ) );
        new1->y = ( old->y + ( lambda * normal->ny ) );
        new1->z = ( old->z + ( lambda * normal->nz ) );
   }
}




/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Normalises Vector V
	Input		:	VECTOR	*	Vector
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void NormaliseVector( VECTOR *  v )
{
    float	inv_mod;

    inv_mod = (float) sqrt( ( v->x * v->x ) + ( v->y * v->y ) + ( v->z * v->z ) );
	if ( !inv_mod )
		return;
	inv_mod = 1.0F / inv_mod;

    v->x = v->x * inv_mod;
    v->y = v->y * inv_mod;
    v->z = v->z * inv_mod;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate the Distance between a VERT and a VECTOR
	Input		:	VERT	*	VERTEX1
				:	VECTOR	*  VECTOR
	Output		:	float		Dot Product
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float VectorLength( VECTOR * v ) 
{
	return( (float) sqrt( ( v->x * v->x ) + ( v->y * v->y ) + ( v->z * v->z ) ) ); 
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate the Distance between a VERT and a VECTOR
	Input		:	VERT	*	VERTEX1
				:	VECTOR	*  VECTOR
	Output		:	float		Dot Product
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float DistanceVert2Vector( VERT *  a , VECTOR * b ) 
{
	float x;
	float y;
	float z;
	
	x = a->x - b->x;
	y = a->y - b->y;
	z = a->z - b->z;

	return( (float) sqrt( (double)((x*x) + (y*y) + (z*z)) ) ); 
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate the Distance between a VERT and a VECTOR
	Input		:	VECTOR	*  VECTOR
				:	VECTOR	*  VECTOR
	Output		:	float		Dot Product
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float DistanceVector2Vector( VECTOR *  a , VECTOR * b ) 
{
	float x;
	float y;
	float z;
	
	x = a->x - b->x;
	y = a->y - b->y;
	z = a->z - b->z;

	return( (float) sqrt( (double)((x*x) + (y*y) + (z*z)) ) ); 
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Matrix Transpose
	Input		:	MATRIX	*	Matrix 1
				:	MATRIX	*	Matrix 2
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void MatrixTranspose( MATRIX * m1, MATRIX * m2 )
{
	MATRIX	T;

#if 1
	T = *m1;

#else
	T._11 = m1->_11;	T._12 = m1->_12;	T._13 = m1->_13;	T._14 = m1->_14;
	T._21 = m1->_21;	T._22 = m1->_22;	T._23 = m1->_23;	T._24 = m1->_24;
	T._31 = m1->_31;	T._32 = m1->_32;	T._33 = m1->_33;	T._34 = m1->_34;
	T._41 = m1->_41;	T._42 = m1->_42;	T._43 = m1->_43;	T._44 = m1->_44;
#endif

	m2->_11 = T._11;	m2->_12 = T._21;	m2->_13 = T._31;	m2->_14 = T._41;
	m2->_21 = T._12;	m2->_22 = T._22;	m2->_23 = T._32;	m2->_24 = T._42;
	m2->_31 = T._13;	m2->_32 = T._23;	m2->_33 = T._33;	m2->_34 = T._43;
	m2->_41 = T._14;	m2->_42 = T._24;	m2->_43 = T._34;	m2->_44 = T._44;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Calculate Random Number within range
	Input		:	uint16		Max
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	Random_Range( uint16 Max )
{
	uint16	bit1, bit2;

	if ( !Max )
		return 0;
	Seed1	^=	Seed2;
	Seed1	+=	7;
	Seed2	-=	0x5347;
	Seed2	^=	0x7c9f;
	bit1	=	( Seed1 >> 15 );
	bit2	=	( Seed2 << 15 );
	Seed1	<<=	1;
	Seed2	>>=	1;
	Seed1	|=	bit1;
	Seed2	|=	bit2;

	return( ( Seed1 & 32767 ) % Max );
}


float Random_Range_Float( float Max )
{
	float Rand;
	Rand = (float) Random_Range( 32767 );
	Rand /= 32767.0F;
	return (Max * Rand);
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Quick Distancecalc... 8 % error...
	Input		:	VECTOR * 
	Output		:	float
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float	QuickDistance( VECTOR * V )
{
	float	Max,Med,Min;

	Max = V->x;

	if( V->y > Max )
	{
		Med = Max;
		Max = V->y;
	}else{
		Med = V->y;
	}
	if( V->z > Max)
	{
		Min = Med;
		Med = Max;
		Max = V->z;
	}else{
		if(	V->z > Med )
		{
			Min = Med;
			Med = V->z;
		}else{
			Min = V->z;
		}
	}
	return( Max + ( Med * 0.34375F ) + ( Min * 0.25F ) ) ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Quick Distancecalc... 8 % error...
	Input		:	float , float
	Output		:	float
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float	QuickDistance2d( float x , float y )
{
	float Min;

	x = (float) fabs( x );
	y = (float) fabs( y );

	if( x > y )
	{
		Min = y;
	}else{
		Min = x;
	}
	return( x + y - ( 0.5f * Min ) );
}

void MakeViewMatrix(VECTOR *viewpos, VECTOR *lookpos, VECTOR *up, MATRIX *view)
{
	VECTOR vx, vy, vz;

	CalcViewAxes(viewpos, lookpos, up, &vx, &vy, &vz);
	view->_11 = vx.x;
	view->_12 = vy.x;
	view->_13 = vz.x;
	view->_14 = 0.0F;
	view->_21 = vx.y;
	view->_22 = vy.y;
	view->_23 = vz.y;
	view->_24 = 0.0F;
	view->_31 = vx.z;
	view->_32 = vy.z;
	view->_33 = vz.z;
	view->_34 = 0.0F;
	view->_41 = 0.0F;
	view->_42 = 0.0F;
	view->_43 = 0.0F;
	view->_44 = 1.0F;
//	view->_41 = vx.x * -viewpos->x + vx.y * -viewpos->y + vx.z * -viewpos->z;
//	view->_42 = vy.x * -viewpos->x + vy.y * -viewpos->y + vy.z * -viewpos->z;
//	view->_43 = vz.x * -viewpos->x + vz.y * -viewpos->y + vz.z * -viewpos->z;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Calculate View Axes
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction Vector
				:	VECTOR	*	Up Vector
				:	VECTOR	*	New X Axis Vector
				:	VECTOR	*	New Y Axis Vector
				:	VECTOR	*	New Z Axis Vector
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CalcViewAxes( VECTOR * viewpos, VECTOR * lookpos, VECTOR * up,
				   VECTOR * vx, VECTOR * vy, VECTOR * vz )
{

/*	Calculate camera z axis as normalised vector from viewpos to lookpos */

	vz->x = (lookpos->x - viewpos->x);
	vz->y = (lookpos->y - viewpos->y);
	vz->z = (lookpos->z - viewpos->z);
	NormaliseVector( vz );

/*	Calculate camera x axis as normalised cross product of camera z axis and up vector */

	CrossProduct( up, vz, vx);
	NormaliseVector( vx );

/*	Calculate camera y axis as normalised cross product of camera z and x axes */

	CrossProduct( vz, vx, vy);
	NormaliseVector( vy );
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Scale a matrix
	Input		:	MATRIX	*	Matrix
				:	VECTOR	*	Scale Vector
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ScaleMatrix( MATRIX * m, VECTOR * v )
{
	m->_11 = ( m->_11 * v->x );
	m->_12 = ( m->_12 * v->y );
	m->_13 = ( m->_13 * v->z );

	m->_21 = ( m->_21 * v->x );
	m->_22 = ( m->_22 * v->y );
	m->_23 = ( m->_23 * v->z );

	m->_31 = ( m->_31 * v->x );
	m->_32 = ( m->_32 * v->y );
	m->_33 = ( m->_33 * v->z );
}

/* rotate_matrix

   compute rotation matrix required

   to rotate a vector about given axis by given angle

*/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Create matrix to rotate around arbitary axis
	Input		:	float		Angle
				:	VECTOR		Axis
				:	MATRIX	*	Matrix
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void MatrixFromAxisAndAngle( float angle, VECTOR * axis, MATRIX * rot )
{

	MATRIX	rotx, rotangle;
	float	xz, xyz, s, c;

	xz	= (float) sqrt( axis->x * axis->x + axis->z * axis->z );
	xyz	= (float) sqrt( axis->x * axis->x + axis->y * axis->y + axis->z * axis->z ) ;

	if( xz != 0.0F )
	{
		rotx._11 = ( axis->x / xyz );
		rotx._12 = -( axis->x * axis->y ) / ( xz * xyz );
		rotx._13 = -( axis->z / xz );
		rotx._14 = 0.0F;
		
		rotx._21 = ( axis->y / xyz );
		rotx._22 = ( xz / xyz );
		rotx._23 = 0.0F;
		rotx._24 = 0.0F;
		
		rotx._31 = ( axis->z / xyz );
		rotx._32 = -( axis->y * axis->z ) / ( xz * xyz );
		rotx._33 = ( axis->x / xz );
		rotx._34 = 0.0F;
		
		rotx._41 = 0.0F;
		rotx._42 = 0.0F;
		rotx._43 = 0.0F;
		rotx._44 = 1.0F;
	}
	else
	{
		rotx._11 = 0.0F;
		rotx._12 = -( axis->y / xyz );
		rotx._13 = 0.0F;
		rotx._14 = 0.0F;
		
		rotx._21 = ( axis->y / xyz );
		rotx._22 = 0.0F;
		rotx._23 = 0.0F;
		rotx._24 = 0.0F;
		
		rotx._31 = 0.0F;
		rotx._32 = 0.0F;
		rotx._33 = 1.0F;
		rotx._34 = 0.0F;
		
		rotx._41 = 0.0F;
		rotx._42 = 0.0F;
		rotx._43 = 0.0F;
		rotx._44 = 1.0F;
	}

	c = (float) cos( angle );
	s = (float) sin( angle );

   	rotangle._11 = 1.0F;
   	rotangle._12 = 0.0F;
   	rotangle._13 = 0.0F;
   	rotangle._14 = 0.0F;
   	
   	rotangle._21 = 0.0F;
   	rotangle._22 = c;
   	rotangle._23 = s;
   	rotangle._24 = 0.0F;
   	
   	rotangle._31 = 0.0F;
   	rotangle._32 = -s;
   	rotangle._33 = c;
   	rotangle._34 = 0.0F;
   	
   	rotangle._41 = 0.0F;
   	rotangle._42 = 0.0F;
   	rotangle._43 = 0.0F;
   	rotangle._44 = 1.0F;

	MatrixMultiply( &rotx, &rotangle, rot );
	MatrixTranspose( &rotx, &rotx );
	MatrixMultiply( rot, &rotx, rot );
}

#ifdef SOFTWARE_ENABLE
/*-----------------------------------------------------------------------------
	Chris Walsh's Code
-----------------------------------------------------------------------------*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Apply Matrix to a Vector
	Input		:	MATRIX	*	Matrix
				:	VECTOR	*	Vert
				:	VECTOR	*	New Vert
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CWVisPolyApplyMatrix( MATRIX * m, VECTOR * v0, VECTOR * v1 )
{
	float w;
	v1->x = ( ( m->_11 * v0->x ) + ( m->_21 * v0->y ) + ( m->_31 * v0->z ) +m->_41 );
	v1->y = ( ( m->_12 * v0->x ) + ( m->_22 * v0->y ) + ( m->_32 * v0->z ) +m->_42);
	v1->z = ( ( m->_13 * v0->x ) + ( m->_23 * v0->y ) + ( m->_33 * v0->z ) +m->_43);
	w	  = ( ( m->_14 * v0->x ) + ( m->_24 * v0->y ) + ( m->_34 * v0->z ) +m->_44);
 //	v1->x /= w;
 //	v1->y /= w;
	v1->z = w;	//cwxcwxcwx was /=w
}
/*---------------------------------------------------------------------------*/
#endif


/*
 * Calculates a point along a B-Spline curve defined by four points. p
 * n output, contain the point. t                                Position
 * along the curve between p2 and p3.  This position is a float between 0
 * and 1. p1, p2, p3, p4    Points defining spline curve. p, at parameter
 * t along the spline curve
 */
void 
spline(VECTOR * p, float t, VECTOR * p1, VECTOR * p2, VECTOR * p3, VECTOR * p4)
{
#if 1
    float t2, t3, m1, m2, m3, m4;

    t2 = t * t;
    t3 = t2 * t;

    m1 = ( -1.0F * t3) + ( 2.0F  * t2) + ( -1.0F * t);
    m2 = ( 3.0F  * t3) + ( -5.0F * t2) + 2.0F;
    m3 = ( -3.0F * t3) + ( 4.0F  * t2) + t;
    m4 = t3 + ( -1.0F * t2);

    m1 *= 0.5;
    m2 *= 0.5;
    m3 *= 0.5;
    m4 *= 0.5;

    p->x = p1->x * m1 + p2->x * m2 + p3->x * m3 + p4->x * m4;
    p->y = p1->y * m1 + p2->y * m2 + p3->y * m3 + p4->y * m4;
    p->z = p1->z * m1 + p2->z * m2 + p3->z * m3 + p4->z * m4;
#else
    double t2, t3;
    float m1, m2, m3, m4;

    t2 = (double)(t * t);
    t3 = t2 * (double)t;

    m1 = (float)((-1.0 * t3) + (2.0 * t2) + (-1.0 * (double)t));
    m2 = (float)((3.0 * t3) + (-5.0 * t2) + (0.0 * (double)t) + 2.0);
    m3 = (float)((-3.0 * t3) + (4.0 * t2) + (1.0 * (double)t));
    m4 = (float)((1.0 * t3) + (-1.0 * t2) + (0.0 * (double)t));

    m1 /= (float)2.0;
    m2 /= (float)2.0;
    m3 /= (float)2.0;
    m4 /= (float)2.0;

    p->x = p1->x * m1 + p2->x * m2 + p3->x * m3 + p4->x * m4;
    p->y = p1->y * m1 + p2->y * m2 + p3->y * m3 + p4->y * m4;
    p->z = p1->z * m1 + p2->z * m2 + p3->z * m3 + p4->z * m4;
#endif

}




#ifdef OPT_ON
#pragma optimize( "", off )
#endif
