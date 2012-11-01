
/*===================================================================
		Include File...	
===================================================================*/

#include "main.h"
#include "sphere.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/************************************************************************
*									*
* This method computes the intersection between a ray and the sphere.	*
* It returns the distance between the origin of the ray and the closest *
* point of intersection (or 0.0 if no intersection occurs).		*
*									*
************************************************************************/

float intersect(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir)
{
	float	c[2];			    // coefficients of the quadric equation.
	float	s[2];			    // solutions of the quadric equation
	float	num;
	VECTOR	d;

	d.x = ray_org->x - sphere_pos->x; // vector from the center of the sphere to
	d.y = ray_org->y - sphere_pos->y; // the origin of the ray.                 
	d.z = ray_org->z - sphere_pos->z; 
	
	// compute the coefficients of the resolvent equation
	c[1] = DotProduct( ray_dir , &d);
	c[0] = DotProduct(&d , &d) - radius * radius;
	
//	return the closest intersection point

	num = solveQuadric( (float*) &c, (float*) &s);
  
	return closest_intersection( (float*) &s , num );
}

/************************************************************************
*									*
* This method computes the normal vector to the sphere at the point of	*
* intersection. (NB: normalization is "built-in" for the sphere).	*
*									*
************************************************************************/
void	SphereNormal( VECTOR * Pos , VECTOR * Point , float radius , NORMAL * Normal )
{
	Normal->nx = ( Point->x - Pos->x ) / radius;
	Normal->ny = ( Point->y - Pos->y ) / radius;
	Normal->nz = ( Point->z - Pos->z ) / radius;
}

//vec3 Sphere::normalAt(vec3& p)
//{ return (p - pos) / radius; }

/********************************************************
*							*
* This function determines if a float is small enough	*
* to be zero. The purpose of the subroutine is to try	*
* to overcome precision problems in math routines.	*
*							*
********************************************************/

float isNotZero(float x)
{
	if( ( x > -EPS ) && ( x < EPS ) )
		return 0.0F;
	return x;
}

/********************************************************
*							*
* This function determines the roots of a quadric	*
* equation.						*
* It takes as parameters a pointer to the three		*
* coefficient of the quadric equation (the c[2] is the	*
* coefficient of x2 and so on) and a pointer to the	*
* two element array in which the roots are to be	*
* placed.						*
* It outputs the number of roots found.			*
*							*
********************************************************/

float solveQuadric(float * c, float * s)
{
float p, q, D;
float sqrt_D;

	// normal for: x^2 + px + q
	p = c[1];
	q = c[0];
	D = p * p - q;

	if (!isNotZero(D))
	    {
	    // one double root
	    s[0] = s[1] = -p;
	    return 1.0F;
	    }
	
	if (D < 0.0F)
	    // no real root
	    return 0.0F;

    // two real roots
    sqrt_D = (float) sqrt(D);
    s[0] = sqrt_D - p;
    s[1] = -sqrt_D - p;
    return 2.0F;
}



/************************************************************************
*									*
* This method receives as parameter a list of intersections and the	*
* number of intersections in the list. It scans the list and returns	*
* the closest positive intersection, or 0.0 if no such intersection	*
* exists.								*
*									*
************************************************************************/

float closest_intersection(float * x, float x_num)
{
int	i;
float	x_min;
	x_min = (x_num) ? x[0] : 0.0F;
	for ( i = 1 ; i < x_num ; i++ )
	{
	    if (x[i] < x_min)
		x_min = x[i];
	}
	return x_min;
}




/*===================================================================
	Procedure	:	Perform ray to sphere intersection
	Input		:	VECTOR * sphere_pos
				:	float radius
				:	VECTOR * ray_org
				:	VECTOR * ray_dir
	Output		:	float how far from the ray_org the sphere was
				:	intersected...0.0F if none < 0.0F if there is one
===================================================================*/

float RaytoSphere(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir)
{
	float	s0,s1;			    // solutions of the quadric equation
	VECTOR	d;
	float p, q, D;
	float sqrt_D;

	d.x = ray_org->x - sphere_pos->x; // vector from the center of the sphere to
	d.y = ray_org->y - sphere_pos->y; // the origin of the ray.                 
	d.z = ray_org->z - sphere_pos->z; 
	
	
	// normal for: x^2 + px + q
	p =	( ray_dir->x * d.x ) + ( ray_dir->y * d.y ) + ( ray_dir->z * d.z ); // cos

	q = ( ( d.x * d.x ) + ( d.y * d.y ) + ( d.z * d.z ) ) - radius * radius;

	D = p * p - q ;

	if (D < 0.0F)			// no real root
		return 0.0F;

	// two real roots
	sqrt_D = (float) sqrt(D);
	s0 = sqrt_D - p;
	s1 = -sqrt_D - p;

	if ( s0 < s1 )
		return s0;
	return s1;
}

/*--------------------------------------------------------------------------------------------------------------------------
 * RAY -> SPHERE collision: RADIUS = 256
 *
 * Input:
 *		VECTOR *	sorigin	-	Origin of SPHERE
 *		float		radius	-	Radius of SPHERE
 *		VECTOR *	rorigin	-	Origin of RAY
 *		VECTOR *	rdir	-	Direction of RAY (Normalised)
 *		VECTOR *	inter	-	Intersection
 *		VECTOR *	inter	-	Intersection Back
 *
 * Output:
 *		bool		hit		-	Collision True/False
 *
 *-------------------------------------------------------------------------------------------------------------------------*/
bool RaytoSphere2( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, VECTOR * inter, VECTOR * inter2 )
{
	float		v;
	float		disc;
	float		d, d2;
	float		d0;
	float		d1;
	VECTOR		EO;

	EO.x = (rorigin->x - sorigin->x);
	EO.y = (rorigin->y - sorigin->y);
	EO.z = (rorigin->z - sorigin->z);

	if( VectorLength( &EO ) <= radius ) return false; // Vector Started inside Sphere!

	v = ( ( EO.x * rdir->x ) + ( EO.y * rdir->y ) + ( EO.z * rdir->z ) );

	disc = ( ( radius * radius ) - ( ( ( EO.x * EO.x ) + ( EO.y * EO.y ) + ( EO.z * EO.z ) ) - ( v * v ) ) );

	if ( disc < 0.0F ) return false;

	d  = (float) sqrt( disc );
	d0 = d - v;
	d1 = ( -d ) - v;

	d  = (d0 < d1) ? d0 : d1;
	d2  = (d0 > d1) ? d0 : d1;

	if ( d < 0.0F )	return false;

	inter->x = rorigin->x + (rdir->x * d);
	inter->y = rorigin->y + (rdir->y * d);
	inter->z = rorigin->z + (rdir->z * d);

	inter2->x = rorigin->x + (rdir->x * d2);
	inter2->y = rorigin->y + (rdir->y * d2);
	inter2->z = rorigin->z + (rdir->z * d2);

	return true;
}

/*===================================================================
	Procedure	:	Ray To Sphere Collision
	Input		:	VECTOR *	SphereCenter
				:	float		Radius
				:	VECTOR *	RayStart
				:	VECTOR *	RayEnd
				:	VECTOR *	IntPoint
	Output		:	int16_t		Type Collision
===================================================================*/
int16_t RaytoSphere3( VECTOR * SphereCenter, float Radius, VECTOR * RayStart,
				    VECTOR * RayEnd, VECTOR * IntPoint )
{
	VECTOR		EO;
	float		v, disc, d, d0, d1;
	VECTOR		RayDir;
	float		RayLength;

	EO.x = ( RayStart->x - SphereCenter->x );
	EO.y = ( RayStart->y - SphereCenter->y );
	EO.z = ( RayStart->z - SphereCenter->z );

	if( VectorLength( &EO ) < Radius ) return( R2S_INSIDE ); // Vector Started inside Sphere!

	RayDir.x = ( RayEnd->x - RayStart->x );
	RayDir.y = ( RayEnd->y - RayStart->y );
	RayDir.z = ( RayEnd->z - RayStart->z );
	RayLength = VectorLength( &RayDir );
	NormaliseVector( &RayDir );

	v = DotProduct( &EO, &RayDir );

	disc = ( ( Radius * Radius ) - DotProduct( &EO, &EO ) - ( v * v ) );

	if ( disc < 0.0F ) return( R2S_MISSED );				// Ray does not IntPointsect sphere!

	d  = (float) sqrt( disc );
	d0 = d - v;
	d1 = ( -d ) - v;

	d = (d0 < d1) ? d0 : d1;

	if( d < 0.0F ) return( R2S_BEHIND );					// Start IntPointsection behind ray start!
	if( d > RayLength ) return( R2S_SHORT );				// Ray was Short!

	IntPoint->x = RayStart->x + ( RayDir.x * d );
	IntPoint->y = RayStart->y + ( RayDir.y * d );
	IntPoint->z = RayStart->z + ( RayDir.z * d );

	return( R2S_COLLISION );								// Collided!
}

/*===================================================================
	Procedure	:	Ray To Sphere Short
	Input		:	VECTOR *	SphereCenter
				:	float		Radius
				:	VECTOR *	RayStart
				:	VECTOR *	RayDirection
				:	float		RayLength
	Output		:	bool		True/False
===================================================================*/
bool RaytoSphereShort( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, float rlength )
{
	float		v;
	float		disc;
	float		d, d0, d1;
	float		EO_LS;								// EO Length Squared
	VECTOR		EO;

	EO.x = ( rorigin->x - sorigin->x );
	EO.y = ( rorigin->y - sorigin->y );
	EO.z = ( rorigin->z - sorigin->z );

	EO_LS = ( EO.x * EO.x ) + ( EO.y * EO.y ) + ( EO.z * EO.z );

	if( EO_LS <= ( radius * radius ) ) return( true ); // Started inside?

	v = ( ( EO.x * rdir->x ) + ( EO.y * rdir->y ) + ( EO.z * rdir->z ) );

	disc = ( ( radius * radius ) - ( EO_LS - ( v * v ) ) );

	if( disc < 0.0F ) return( false );				// Ray does not hit sphere

	d  = (float) sqrt( disc );
	d0 = d - v;
	d1 = ( -d ) - v;

	d  = (d0 < d1) ? d0 : d1;

	if ( d < 0.0F )	return( false );				// Ray Opposit Dir
	if( d > rlength ) return( false );				// Ray was Short!

	return true;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
