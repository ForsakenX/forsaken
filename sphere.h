

#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED


#include <math.h>
#include <new3d.h>

/*===================================================================
	Defines
===================================================================*/
#define	R2S_MISSED		0	// Ray does not IntPointsect sphere!
#define	R2S_INSIDE		1	// Vector Started inside Sphere!
#define R2S_BEHIND		2	// Start IntPointsection behind ray start!
#define	R2S_SHORT		3	// Ray was Short!
#define	R2S_COLLISION	4	// Collided!

/*===================================================================
	Prototypes
===================================================================*/
float isNotZero(float x);
float solveQuadric(float * c, float * s);
float closest_intersection(float *x, float x_num);
float intersect(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir);
void  SphereNormal( VECTOR * Pos , VECTOR * Point , float radius , NORMAL * Normal );
float RaytoSphere(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir);
bool RaytoSphere2( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, VECTOR * inter, VECTOR * inter2 );
int16_t RaytoSphere3( VECTOR * SphereCenter, float Radius, VECTOR * RayStart,
				    VECTOR * RayEnd, VECTOR * IntPoint );
bool RaytoSphereShort( VECTOR * sorigin, float radius, VECTOR * rorigin, VECTOR * rdir, float rlength );

//	vec3 normalAt(vec3& p);
#endif	//SPHERE_INCLUDED
