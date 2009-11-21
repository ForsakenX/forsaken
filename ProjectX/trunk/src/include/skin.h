#ifndef SKIN_INCLUDED
#define SKIN_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include <math.h>

#include "new3d.h"
#include <time.h>
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
BOOL CreateSkinExecList( MCLOADHEADER * MCloadheader, int16 NumVisible );
BOOL MakeNewSkinExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16	NumVerts, int16 NumTris );
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
BOOL MakeNewPortalExec( LPLVERTEX Verts, LPTRIANGLE Tris, int16 NumVerts, int16 NumTris );
BOOL CreatePortalExecList( MLOADHEADER * Mloadheader, int16 NumVisible );
void ReleasePortalExecs( void );
void InitPortalExecs( void );
int GimmeTriggerZoneVertices( TRIGGER_ZONE * plane_ptr, TRIGGER_ZONE * space_ptr, int NumSides, int side, VECTOR * out_ptr );
int GimmeColZoneVertices( ZONESIDE * plane_ptr, ZONESIDE * space_ptr, int NumSides, int side, VECTOR * out_ptr );
void DisplayTriggerZone( TRIGGER_ZONE * Sides, int16 NumSides, uint16 Group,
						 uint8 Red, uint8 Green, uint8 Blue, uint8 Trans );
void DisplayColZone( ZONESIDE * Sides, int16 NumSides, uint16 Group, int16 Sensitive );
void DisplaySphereZone( VECTOR * Pos, float Radius, uint16 Group,
					   uint8 Red, uint8 Green, uint8 Blue, uint8 Trans );
void KillAllSphereZones( void );
void DisplayZoneDirection( VECTOR * Pos, VECTOR * Dir, uint16 Group );
#endif


