
#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED


#include <math.h>
#include <new3d.h>


#define New_EQN_EPS 0.000001F
	float isZero(float x);
	float solveQuadric(float c[3], float s[2]);
	float closest_intersection(float *x, float x_num);
  	float intersect(VECTOR * sphere_pos, float radius , VECTOR * ray_org, VECTOR * ray_dir);

//	vec3 normalAt(vec3& p);
#endif	//SPHERE_INCLUDED
