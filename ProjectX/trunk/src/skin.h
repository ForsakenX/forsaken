#ifndef SKIN_INCLUDED
#define SKIN_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include <math.h>

#include "new3d.h"
#include "ships.h"
#include "mload.h"
#include "collision.h"
#include "lights.h"
#include "2dtextures.h"
#include "2dpolys.h"
#include "tload.h"
#include "bsp.h"
#include "trigarea.h"

/*===================================================================
	Defines
===================================================================*/
#define	MAXSKINEXECS		MAXGROUPS
#define	MAXPORTALEXECS		MAXGROUPS
#define	MAX_TRIS_PER_EXEC	300

/*===================================================================
	Prototypes
===================================================================*/
bool CreateSkinExecList( MCLOADHEADER * MCloadheader, int16_t NumVisible );
bool MakeNewSkinExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16_t	NumVerts, int16_t NumTris );
void ReleaseSkinExecs( void );
void DisplayBSPNode( BSP_NODE * Node );
void DisplayNewBSPNode( BSP_NODE * Node );
int PlaneToCubeIntersectionShape( VECTOR * min_ptr, VECTOR * max_ptr, VECTOR * vec_ptr, float offset, VECTOR * result_ptr );
void CreateLineBox( void );
int GimmeNodeVertices( BSP_NODE * plane_ptr, BSP_NODE * space_ptr, int side, VECTOR * out_ptr );
void DisplayNodeVolume( BSP_NODE * Node );
void KillNodeCubeLines( void );
void KillBoxLines( void );
void InitSkinExecs( void );
bool MakeNewPortalExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16_t NumVerts, int16_t NumTris );
bool CreatePortalExecList( MLOADHEADER * Mloadheader, int16_t NumVisible );
void ReleasePortalExecs( void );
void InitPortalExecs( void );
int GimmeTriggerZoneVertices( TRIGGER_ZONE * plane_ptr, TRIGGER_ZONE * space_ptr, int NumSides, int side, VECTOR * out_ptr );
int GimmeColZoneVertices( ZONESIDE * plane_ptr, ZONESIDE * space_ptr, int NumSides, int side, VECTOR * out_ptr );
void DisplayTriggerZone( TRIGGER_ZONE * Sides, int16_t NumSides, u_int16_t Group,
						 u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans );
void DisplayColZone( ZONESIDE * Sides, int16_t NumSides, u_int16_t Group, int16_t Sensitive );
void DisplaySphereZone( VECTOR * Pos, float Radius, u_int16_t Group,
					   u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans );
void KillAllSphereZones( void );
void DisplayZoneDirection( VECTOR * Pos, VECTOR * Dir, u_int16_t Group );
#endif


