

/*===================================================================
	Includes
===================================================================*/
#include "main.h"
#include	<math.h>
#include	"new3d.h"
#include	"quat.h"
#include <stdio.h>
#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

extern	VECTOR			Forward;
extern	VECTOR			SlideUp;


#if 1
void MakeQuat( float XRot, float YRot, float ZRot , QUAT * qxyz )
{
    float  cosZRot, sinZRot, cosYRot, sinYRot, cosXRot, sinXRot;
    float  half_XRot, half_YRot, half_ZRot;
	/* put angles into radians and divide by two, since all angles in formula
	 *  are (angle/2)
	 */
	
	half_ZRot = ZRot * 0.5F;
	half_YRot = YRot * 0.5F;
	half_XRot = XRot * 0.5F;
	
	cosZRot = (float) COSD(half_ZRot);
	sinZRot = (float) SIND(half_ZRot);
	
	cosYRot = (float) COSD(half_YRot);
	sinYRot = (float) SIND(half_YRot);
	
	cosXRot = (float) COSD(half_XRot);
	sinXRot = (float) SIND(half_XRot);
	
	
	qxyz->x = sinXRot * cosYRot * cosZRot - cosXRot * sinYRot * sinZRot;
	qxyz->y = cosXRot * sinYRot * cosZRot + sinXRot * cosYRot * sinZRot;
	qxyz->z = cosXRot * cosYRot * sinZRot - sinXRot * sinYRot * cosZRot;
	
	qxyz->w = cosXRot * cosYRot * cosZRot + sinXRot * sinYRot * sinZRot;

}	/* q_from_euler */
#else
/*===================================================================
	Procedure	:	Build XYZ Quaternion
	Input		:	float		X Rotation ( Degrees )
				:	float		Y Rotation ( Degrees )
				:	float		Z Rotation ( Degrees )
				:	QUAT	*	Destination Quaternion
	Output		:	Nothing
===================================================================*/
void MakeQuat( float XRot, float YRot, float ZRot, QUAT * qxyz )
{
	QUAT	qx;
	QUAT	qy;
	QUAT	qz;
	QUAT	qxy;

	qx.w = (float) COSD( XRot * 0.5F );
	qx.x = (float) SIND( XRot * 0.5F );
	qx.y = 0.0F;
	qx.z = 0.0F;

	qy.w = (float) COSD( YRot * 0.5F );
	qy.x = 0.0F;
	qy.y = (float) SIND( YRot * 0.5F );
	qy.z = 0.0F;

	qz.w = (float) COSD( ZRot * 0.5F );
	qz.x = 0.0F;
	qz.y = 0.0F;
	qz.z = (float) SIND( ZRot * 0.5F );

	QuatMultiplyX_Y( &qx, &qy, &qxy );
	QuatMultiplyXY_Z( &qxy, &qz, qxyz );
}
#endif
/*===================================================================
	Procedure	:	Multiply 2 Quaternions together creating 3rd
	Input		:	QUAT	*	Quaternion1
				:	QUAT	*	Quaternion2
				:	QUAT	*	New Quaternion
	Output		:	Nothing
===================================================================*/
void QuatMultiply( QUAT * q1, QUAT * q2, QUAT * q1q2 )
{
	float	q1_w, q1_x, q1_y, q1_z;
	float	q2_w, q2_x, q2_y, q2_z;

	q1_w = q1->w;	q1_x = q1->x;	q1_y = q1->y;	q1_z = q1->z;
	q2_w = q2->w;	q2_x = q2->x;	q2_y = q2->y;	q2_z = q2->z;

	q1q2->w = ( (q1_w * q2_w) - (q1_x * q2_x) - (q1_y * q2_y) - (q1_z * q2_z) );
	q1q2->x = ( (q1_w * q2_x) + (q1_x * q2_w) + (q1_y * q2_z) - (q1_z * q2_y) );
	q1q2->y = ( (q1_w * q2_y) + (q1_y * q2_w) + (q1_z * q2_x) - (q1_x * q2_z) );
	q1q2->z = ( (q1_w * q2_z) + (q1_z * q2_w) + (q1_x * q2_y) - (q1_y * q2_x) );
}

/*===================================================================
	Procedure	:	Multiply X Quaternion by Y Quaternion creating
				:	new XY quaternion
	Input		:	QUAT	*	X Quaternion
				:	QUAT	*	Y Quaternion
				:	QUAT	*	New Quaternion
	Output		:	Nothing
===================================================================*/
#ifdef USEINLINE
__inline
#endif
void QuatMultiplyX_Y( QUAT * qx, QUAT * qy, QUAT * qxy )
{
	float	x_w, x_x, x_y, x_z;
	float	y_w, y_x, y_y, y_z;

	x_w = qx->w;	x_x = qx->x;	x_y = qx->y;	x_z = qx->z;
	y_w = qy->w;	y_x = qy->x;	y_y = qy->y;	y_z = qy->z;

	qxy->w = ( ( x_w * y_w ) );
	qxy->x = ( ( x_x * y_w ) );
	qxy->y = ( ( x_w * y_y ) );
	qxy->z = ( ( x_x * y_y ) );
}

/*===================================================================
	Procedure	:	Multiply XY Quaternion by Z Quaternion creating
				:	new XYZ Quaternion
	Input		:	QUAT	*	XY Quaternion
				:	QUAT	*	Z Quaternion
				:	QUAT	*	New Quaternion
	Output		:	Nothing
===================================================================*/
#ifdef USEINLINE
__inline
#endif
void QuatMultiplyXY_Z( QUAT * qxy, QUAT * qz, QUAT * qxyz )
{
	float	xy_w, xy_x, xy_y, xy_z;
	float	z_w, z_x, z_y, z_z;

	xy_w = qxy->w;	xy_x = qxy->x;	xy_y = qxy->y;	xy_z = qxy->z;
	z_w = qz->w;	z_x = qz->x;	z_y = qz->y;	z_z = qz->z;

	qxyz->w = ( ( xy_w * z_w ) - ( xy_z * z_z ) );
	qxyz->x = ( ( xy_x * z_w ) + ( xy_y * z_z ) );
	qxyz->y = ( ( xy_y * z_w ) - ( xy_x * z_z ) );
	qxyz->z = ( ( xy_w * z_z ) + ( xy_z * z_w ) );
}

/*===================================================================
	Procedure	:	Normalise a Quaternion
	Input		:	QUAT	*	Quaternion
	Output		:	Nothing
===================================================================*/
void QuatNormalise( QUAT * q )
{
	float 	len;
	VECTOR	sq;

	sq.x = ( q->x * q->x );
	sq.y = ( q->y * q->y );
	sq.z = ( q->z * q->z );

	len = (float) sqrt( ( sq.x + sq.y + sq.z ) + ( q->w * q->w ) );

	if ( len )
	{
		len = 1 / len;
		q->x = ( q->x * len );
		q->y = ( q->y * len );
		q->z = ( q->z * len );
		q->w = ( q->w * len );
	}			   
}

#if 0
/*===================================================================
	Procedure	:	Create Matrix from Quaternion
	Input		:	QUAT	*	Quaternion
				:	MATRIX	*	Matrix
	Output		:	Nothing
===================================================================*/
void QuatToMatrix( QUAT * q, MATRIX * m )
{
	float	xs, ys, zs;
	float	wx, wy, wz;
	float	xx, xy, xz;
	float	yy, yz, zz;

	xs = q->x;
	ys = q->y;
	zs = q->z;

	wx = ( ( q->w * xs ) * 2 );
	wy = ( ( q->w * ys ) * 2 );
	wz = ( ( q->w * zs ) * 2 );

	xx = ( ( q->x * xs ) * 2 );
	xy = ( ( q->x * ys ) * 2 );
	xz = ( ( q->x * zs ) * 2 );

	yy = ( ( q->y * ys ) * 2 );
	yz = ( ( q->y * zs ) * 2 );
	zz = ( ( q->z * zs ) * 2 );

	m->_11 = ( 1.0F - ( yy + zz ) );
	m->_12 = ( xy - wz );
	m->_13 = ( xz + wy );
	m->_14 = 0.0F;

	m->_21 = ( xy + wz );
	m->_22 = ( 1.0F - ( xx + zz ) );
	m->_23 = ( yz - wx );
	m->_24 = 0.0F;

	m->_31 = ( xz - wy );
	m->_32 = ( yz + wx );
	m->_33 = ( 1.0F - ( xx + yy ) );
	m->_34 = 0.0F;

	m->_41 = 0.0F;
	m->_42 = 0.0F;
	m->_43 = 0.0F;
	m->_44 = 1.0F;
}
#else
/*===================================================================
	Procedure	:	NEW Create Matrix from Quaternion
	Input		:	QUAT	*	Quaternion
				:	MATRIX	*	Matrix
	Output		:	Nothing
===================================================================*/
void QuatToMatrix( QUAT * q, MATRIX * m )
{
	float	xs, ys, zs;
	float	wx, wy, wz;
	float	xx, xy, xz;
	float	yy, yz, zz;

	xs = q->x + q->x;
	ys = q->y + q->y;
	zs = q->z + q->z;

	wx = q->w * xs;
	wy = q->w * ys;
	wz = q->w * zs;

	xx = q->x * xs;
	xy = q->x * ys;
	xz = q->x * zs;

	yy = q->y * ys;
	yz = q->y * zs;
	zz = q->z * zs;

	m->_11 = ( 1.0F - ( yy + zz ) );
	m->_12 = ( xy - wz );
	m->_13 = ( xz + wy );
	m->_14 = 0.0F;

	m->_21 = ( xy + wz );
	m->_22 = ( 1.0F - ( xx + zz ) );
	m->_23 = ( yz - wx );
	m->_24 = 0.0F;

	m->_31 = ( xz - wy );
	m->_32 = ( yz + wx );
	m->_33 = ( 1.0F - ( xx + yy ) );
	m->_34 = 0.0F;

	m->_41 = 0.0F;
	m->_42 = 0.0F;
	m->_43 = 0.0F;
	m->_44 = 1.0F;
}
#endif

/*===================================================================
	Procedure	:	Quaternion Dot Product
	Input		:	QUAT	*	Quaternion1
				:	QUAT	*	Quaternion2
	Output		:	float		Result
===================================================================*/
float QuatDotProduct( QUAT * q1, QUAT * q2 )
{
	return ( ( q1->x * q2->x ) + ( q1->y * q2->y ) +
			 ( q1->z * q2->z ) + ( q1->w * q2->w ) );
}

/*===================================================================
	Procedure	:	Quaternion Interpolation
	Input		:	QUATLERP	*	Quaternion Interpolation Data
	Output		:	Nothing
===================================================================*/
void QuatInterpolate( register QUATLERP * ql )
{
	float	 	alpha;
	float	 	beta;
	QUAT	*	start;
	QUAT	*	end;
	QUAT	*	result;
	
	alpha  =	ql->time;
	start  =	&ql->start;
	end	   =	&ql->end;
	result =	ql->crnt;

  	beta   =	( 1.0F - alpha );

/*===================================================================
	Check which direction we are going around hypersphere ...
===================================================================*/
	if ( ql->dir < 0.0F ) alpha = -alpha;

/*===================================================================
	Interpolate ...
===================================================================*/

	result->x = ( ( beta * start->x ) ) + ( ( alpha * end->x ) );
	result->y = ( ( beta * start->y ) ) + ( ( alpha * end->y ) );
	result->z = ( ( beta * start->z ) ) + ( ( alpha * end->z ) );
	result->w = ( ( beta * start->w ) ) + ( ( alpha * end->w ) );
					 
	QuatNormalise( result );
}

/*===================================================================
	Procedure	:	Create Quaternion from Vector
	Input		:	VECTOR	*	Vector
				:	QUAT	*	New Quaternion
	Output		:	Nothing
===================================================================*/
void QuatFromVector( VECTOR * Tv, QUAT * q )
{
	float	angle;
	VECTOR	Av;

/*===================================================================
	Normalise TARGET Vector
===================================================================*/
	NormaliseVector( Tv );

/*===================================================================
	Create AXIS vector
===================================================================*/
	Av.x = Tv->y;
	Av.y = -Tv->x;
	Av.z = 0.0F;
	NormaliseVector( &Av );

/*===================================================================
	Calculate ANGLE between TARGET and LOOK vectors
===================================================================*/
	angle = (float) ( acos( Tv->z ) / 2 );

/*===================================================================
	Finally build TARGET QUATERNION
===================================================================*/
	q->x = (float) ( sin( angle ) * Av.x );
	q->y = (float) ( sin( angle ) * Av.y );
	q->z = 0.0F;
	q->w = (float) cos( angle );

	QuatNormalise( q );
}

/*===================================================================
	Procedure	:	Create Quaternion from Vector ( Uuuummmmm )
	Input		:	VECTOR	*	Vector
				:	QUAT	*	New Quaternion
	Output		:	Nothing
===================================================================*/
void QuatFromVector2( VECTOR * Tv, QUAT * q )
{
	float	angle;
	VECTOR	Av;

/*===================================================================
	Normalise TARGET Vector
===================================================================*/
	NormaliseVector( Tv );

/*===================================================================
	Create AXIS vector
===================================================================*/
	Av.x = Tv->y;
	Av.y = -Tv->x;
	Av.z = 0.0F;
	NormaliseVector( &Av );

/*===================================================================
	Calculate ANGLE between TARGET and LOOK vectors
===================================================================*/
	angle = (float) ( acos( -Tv->z ) / 2 );

/*===================================================================
	Finally build TARGET QUATERNION
===================================================================*/
	q->x = (float) ( sin( angle ) * Av.x );
	q->y = (float) ( sin( angle ) * Av.y );
	q->z = 0.0F;
	q->w = (float) cos( angle );

	QuatNormalise( q );
}

/*===================================================================
	Procedure	:		Quaternion Spherical Interpolation (Slerp)
	Input		:		double		alpha ( interpolation parameter (0 to 1) )
				:		QUAT	*	a ( start unit quaternions )
				:		QUAT	*	b ( end unit quaternions )
				:		QUAT	*	q ( output interpolated quaternion )
				:		int16_t		spin ( number of extra spin rotations )
	Output		:		Nothing
===================================================================*/
void Quaternion_Slerp( float alpha, QUAT * a, QUAT * b, QUAT * q, int spin )
{
	float	beta;			/* complementary interp parameter */
	float	theta;			/* angle between A and B */
	float	sin_t, cos_t;	/* sine, cosine of theta */
	float	phi;			/* theta plus spins */
	int		bflip;			/* use negation of B? */

	cos_t = QuatDotProduct( a, b );

	/* if B is on opposite hemisphere from A, use -B instead */
 	if( cos_t < 0.0F )
	{
		cos_t = -cos_t;
		bflip = true;
	}
	else
	{
		bflip = false;
	}

	/* if B is (within precision limits) the same as A,
	 * just linear interpolate between A and B.
	 * Can't do spins, since we don't know what direction to spin.
	 */
	if( ( 1.0F - cos_t ) < EPS )
	{
		beta = ( 1.0F - alpha );
 	}
	else
	{				/* normal case */
 		theta	= ( (float) acos( cos_t ) );
 		phi		= ( theta + ( spin * M_PI ) );
 		sin_t	= ( (float) sin( theta ) );
 		beta	= ( (float) sin( theta - ( alpha * phi ) ) / sin_t );
 		alpha	= ( (float) sin( alpha * phi ) / sin_t );
 	}

	if( bflip ) alpha = -alpha;

	/* interpolate */
 	q->x = ( ( beta * a->x ) + ( alpha * b->x ) );
 	q->y = ( ( beta * a->y ) + ( alpha * b->y ) );
 	q->z = ( ( beta * a->z ) + ( alpha * b->z ) );
 	q->w = ( ( beta * a->w ) + ( alpha * b->w ) );
}

/*===================================================================
	Procedure	:		Invert Quaternion
				:		Create it's multiplicative inverse.
	Input		:		QUAT	*	Source Quaternion
				:		QUAT	*	Destin Quaternion
	Output		:		Nothing
===================================================================*/
void QuatInverse( QUAT * s, QUAT * d )
{
    float	sNorm;

	sNorm = ( 1.0F / ( ( s->x * s->x ) +
					   ( s->y * s->y ) +
					   ( s->z * s->z ) +
					   ( s->w * s->w ) ) );

	d->x = -s->x * sNorm;
	d->y = -s->y * sNorm;
	d->z = -s->z * sNorm;
	d->w =  s->w * sNorm;

}

/*===================================================================
	Procedure	:		create a quaternion from two vectors that rotates
    	    	     	v1 to v2 about an axis perpendicular to both
	Input		:		QUAT	*	Destin Quaternion
				:		VECTOR * v1
				:		VECTOR * v2
	Output		:		Nothing
===================================================================*/
void QuatFrom2Vectors( QUAT * destQuat, VECTOR * v1, VECTOR * v2 )
{
	VECTOR	u1, u2;
	VECTOR	axis;					 /* axis of rotation */
	float	theta;				     /* angle of rotation about axis */
	float	theta_complement;
	float	crossProductMagnitude;
/*
** Normalize both vectors and take cross product to get rotation axis. 
*/
	u1 = *v1;
	u2 = *v2;

	NormaliseVector( &u1 );
	NormaliseVector( &u2 );

	CrossProduct( &u1 , &u2, &axis );
/*
** | u1 X u2 | = |u1||u2|sin(theta)
**
** Since u1 and u2 are normalized, 
**
**  theta = arcsin(|axis|)
*/
	crossProductMagnitude = (float) sqrt( DotProduct( &axis , &axis ) );

/*
** Occasionally, even though the vectors are normalized, the magnitude will
** be calculated to be slightly greater than one.  If this happens, just
** set it to 1 or asin() will barf.
*/
	if( crossProductMagnitude > 1.0F )
	   crossProductMagnitude = 1.0F;
/*
** Take arcsin of magnitude of rotation axis to compute rotation angle.
** Since crossProductMagnitude=[0,1], we will have theta=[0,pi/2].
*/
	theta = (float) asin( crossProductMagnitude );
	theta_complement = PI - theta;
/*
** If cos(theta) < 0, use complement of theta as rotation angle.
*/
	if( DotProduct( &u1 , &u2 ) < 0.0F )
	{
		theta = theta_complement;
		theta_complement = PI - theta;
	}

/* if angle is 0, just return identity quaternion   */
	if( theta < EPS )
	{
		destQuat->x = 0.0F;
		destQuat->y = 0.0F;
		destQuat->z = 0.0F;
		destQuat->w = 1.0F;
	}else{
		if( theta_complement < EPS )
		{
			/*
			** The two vectors are opposed.  Find some arbitrary axis vector.
			** First try cross product with x-axis if u1 not parallel to x-axis.
			*/
			if( (u1.y*u1.y + u1.z*u1.z) >= EPS )
			{
				axis.x = 0.0F ;
				axis.y = u1.z ;
				axis.z = u1.y ;
			}else{
				/*
				** u1 is parallel to to x-axis.  Use z-axis as axis of rotation.
				*/
				axis.x = axis.y = 0.0F ;
				axis.z = 1.0F ;
			}
		}
		NormaliseVector( &axis );
		QuatMake( destQuat, axis.x, axis.y, axis.z, theta ) ;
		QuatNormalise( destQuat );
   }
}


/*===================================================================
	Procedure	:		make a quaternion given an axis and an angle (in radians)
				    	notes:
					    - rotation is counter-clockwise when rotation axis vector is 
					    	pointing at you
				    	    - if angle or vector are 0, the identity quaternion is returned.
	Input		:		QUAT	*	Destin Quaternion
				:		float x , y , z , angle
	Output		:		Nothing
===================================================================*/
void
QuatMake( QUAT * destQuat, float x, float y, float z, float angle)
{
    float length, cosA, sinA;
	/* normalize vector */
	length = (float) sqrt( x*x + y*y + z*z );

	/* if zero vector passed in, just return identity quaternion	*/
	if ( length < EPS )
	{
		destQuat->x = 0.0F;
		destQuat->y = 0.0F;
		destQuat->z = 0.0F;
		destQuat->w = 1.0F;
		return;
	}
	x /= length;
	y /= length;
	z /= length;

	cosA = (float) cos(angle * 0.5F);
	sinA = (float) sin(angle * 0.5F);

	destQuat->w = cosA;
	destQuat->x = sinA * x;
	destQuat->y = sinA * y;
	destQuat->z = sinA * z;

}

/*===================================================================
	Procedure	:		Convert GL 4x4 row-major 
    	    	    	rotation matrix to unit quaternion.
	Input		:		QUAT	*	DestQuat
				:		MATRIX	*	SrcMatrix
	Output		:		Nothing
===================================================================*/
void 
MatrixToQuat(QUAT * destQuat, MATRIX * srcMatrix)
{
    float	trace, s;
    int     i, j, k;
	int 	next[3] = {1, 2, 0};
	float	Temp[4][4];
	trace = srcMatrix->_11 + srcMatrix->_22 + srcMatrix->_33;

if (trace > 0.0F)
    {
    s = (float) sqrt(trace + 1.0F);
    destQuat->w = s * 0.5F;
    s = 0.5F / s;
    
    destQuat->x = (srcMatrix->_23 - srcMatrix->_32) * s;
    destQuat->y = (srcMatrix->_31 - srcMatrix->_13) * s;
    destQuat->z = (srcMatrix->_12 - srcMatrix->_21) * s;
    } 
else 
    {

	Temp[0][0]=srcMatrix->_11;Temp[0][1]=srcMatrix->_12;Temp[0][2]=srcMatrix->_13;Temp[0][3]=srcMatrix->_14;
	Temp[1][0]=srcMatrix->_21;Temp[1][1]=srcMatrix->_22;Temp[1][2]=srcMatrix->_23;Temp[1][3]=srcMatrix->_24;
	Temp[2][0]=srcMatrix->_31;Temp[2][1]=srcMatrix->_32;Temp[2][2]=srcMatrix->_33;Temp[2][3]=srcMatrix->_34;
	Temp[3][0]=srcMatrix->_41;Temp[3][1]=srcMatrix->_42;Temp[3][2]=srcMatrix->_43;Temp[3][3]=srcMatrix->_44;

    
	i = 0;
    if (Temp[1][1] > Temp[0][0])
	i = 1;
    if (Temp[2][2] > Temp[i][i])
	i = 2;
    j = next[i];  
    k = next[j];
    
    s = (float) sqrt( (Temp[i][i] - (Temp[j][j]+Temp[k][k])) + 1.0F );
	if( i == 0)
		destQuat->x = s*0.5F;
	if( i == 1)
		destQuat->y = s*0.5F;
	if( i == 2)
		destQuat->z = s*0.5F;
    
    s = 0.5F / s;
    
    destQuat->w = (Temp[j][k] - Temp[k][j]) * s;
	if( j == 0)
		destQuat->x = (Temp[i][j] + Temp[j][i]) * s;
	if( j == 1)
		destQuat->y = (Temp[i][j] + Temp[j][i]) * s;
	if( j == 2)
		destQuat->z = (Temp[i][j] + Temp[j][i]) * s;

	if( k == 0)
		destQuat->x = (Temp[i][k] + Temp[k][i]) * s;
	if( k == 1)
		destQuat->y = (Temp[i][k] + Temp[k][i]) * s;
	if( k == 2)
		destQuat->z = (Temp[i][k] + Temp[k][i]) * s;
    }
}

/*===================================================================
	Procedure	:		Create Quat from dir vector and up vector
	Input		:		VECTOR	*	Direction Vector
				:		VECTOR	*	Up Vector
				:		QUAT	*	Destin Quaternion
	Output		:		Nothing
===================================================================*/
void QuatFromDirAndUp( VECTOR * Dir, VECTOR * Up, QUAT * Quat )
{
	MATRIX		TempMat;
	VECTOR		TempUp;
	QUAT		RotQuat;

	QuatFrom2Vectors( Quat, &Forward, Dir );
	QuatToMatrix( Quat, &TempMat );
	ApplyMatrix( &TempMat, &SlideUp, &TempUp );
	QuatFrom2Vectors( &RotQuat, &TempUp, Up );
	QuatMultiply( &RotQuat, Quat, Quat );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
