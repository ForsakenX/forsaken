/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 11 $
 *
 * $Header: /PcProjectX/Include/sphere.h 11    13/05/98 10:52 Collinsd $
 *
 * $Log: /PcProjectX/Include/sphere.h $
 * 
 * 11    13/05/98 10:52 Collinsd
 * Updated collision with bgobjects and pickups.  Also changed radius of
 * mine detection for regeneration of bikes.
 * 
 * 10    3/04/98 11:55 Collinsd
 * Tidy
 * 
 * 9     4/03/98 21:03 Collinsd
 * Added Quick out check to bgobject collision
 * 
 * 8     24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 7     29/08/96 17:53 Collinsd
 * Lots more pickups and shield working a little better.
 * 
 * 6     15/08/96 17:28 Collinsd
 * Laser now hits ships.  Bug with primary weapons hitting background
 * before ship fixed.
 * 
 * 5     7/03/96 4:14p Phillipd
 * 
 * 4     7/03/96 10:28a Phillipd
 * 
 * 3     7/03/96 9:32a Phillipd
 * 
 * 2     7/02/96 3:43p Phillipd
 * 
 * 1     7/02/96 3:12p Phillipd
 * 
 * 1     7/02/96 3:04p Phillipd
 * 
 * 1     7/02/96 3:04p Phillipd
 * 
 */

#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED


#include <math.h>
#include <new3d.h>

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	R2S_MISSED		0	// Ray does not IntPointsect sphere!
#define	R2S_INSIDE		1	// Vector Started inside Sphere!
#define R2S_BEHIND		2	// Start IntPointsection behind ray start!
#define	R2S_SHORT		3	// Ray was Short!
#define	R2S_COLLISION	4	// Collided!

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Prototypes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float isNotZero(float x);
float solveQuadric(float * c, float * s);
float closest_intersection(float *x, float x_num);
float intersect(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir);
void  SphereNormal( VECTOR * Pos , VECTOR * Point , float radius , NORMAL * Normal );
float RaytoSphere(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir);
BOOL RaytoSphere2( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, VECTOR * inter, VECTOR * inter2 );
int16 RaytoSphere3( VECTOR * SphereCenter, float Radius, VECTOR * RayStart,
				    VECTOR * RayEnd, VECTOR * IntPoint );
BOOL RaytoSphereShort( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, float rlength );

//	vec3 normalAt(vec3& p);
#endif	//SPHERE_INCLUDED
