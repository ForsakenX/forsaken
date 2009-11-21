
#ifndef VISI_INCLUDED
#define VISI_INCLUDED


#include <math.h>
#include <time.h>
#include "new3d.h"
#include "mload.h"
#include "ships.h"
#include "lights.h"
#include "skin.h"
#include "camera.h"

typedef struct _GROUPLIST
{
	int groups;
	uint16 *group;
} GROUPLIST;

// indirectly visible groups seen by any active camera
uint16 Num_IndirectVisible;
uint16 IndirectVisible[ MAXGROUPS ];

/*
 * fn prototypes
 */
BOOL LightGroupDispGroup( MLOADHEADER	* Mloadheader);

BOOL DispVisipolyModel( MLOADHEADER	* Mloadheader) ;
void FindGroupImIn( MLOADHEADER	* Mloadheader);
uint16 WhichGroupImIn( MLOADHEADER	* Mloadheader , VECTOR * Pos );
BOOL PortalTestTransformClip( MLOADHEADER * Mloadheader , uint16 group );
void FindVisiblePortals( MLOADHEADER * Mloadheader , uint16 group );
void BuildGroupList ( MLOADHEADER * Mloadheader , uint16 group );
void FindVisible( CAMERA *cam, MLOADHEADER	* Mloadheader );
int ClipGroup( CAMERA *cam, uint16 group );
BOOL DisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam );
void InitVisiStats( MLOADHEADER *m );
BOOL OutputVisiStats( MLOADHEADER *m, char *lname );

BOOL FindGroupConnections( MLOADHEADER *m );
void FreeGroupConnections( void );
GROUPLIST *ConnectedGroups( uint16 g );
GROUPLIST *VisibleGroups( uint16 g );
GROUPLIST *IndirectVisibleGroups( uint16 g );
int VisibleOverlap( uint16 g1, uint16 g2, uint16 *overlapping_group );
BOOL GroupsAreVisible( uint16 g1, uint16 g2 );
BOOL GroupsAreConnected( uint16 g1, uint16 g2 );
BOOL GroupsAreIndirectVisible( uint16 g1, uint16 g2 );

uint16 FindClipGroup( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max );
uint16 FindOverlappingVisibleGroups( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max, uint16 * group );
BOOL PointInGroupBoundingBox( MLOADHEADER * Mloadheader, VECTOR * Pos, uint16 group );

#endif	// VISI_INCLUDED
