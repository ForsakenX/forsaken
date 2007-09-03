/*
 * v i s i .h
 *
 * visipoly Routines
 *
 * Project X, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 20 $
 *
 * $Header: /PcProjectX/Include/visi.h 20    22/01/98 15:06 Oliverc $
 *
 * $Log: /PcProjectX/Include/visi.h $
 * 
 * 20    22/01/98 15:06 Oliverc
 * Added loading of pre-calculated group connectivity, visibility,
 * indirect visibility, and sound attentuation tables
 * 
 * 19    7/11/97 9:24 Collinsd
 * Added point in group bounding box function
 * 
 * 18    6/11/97 19:03 Collinsd
 * Find Overlapping Visible Groups function Added.
 * 
 * 17    30/10/97 11:20 Oliverc
 * Added FindClipGroup() function for clipping objects that span multiple
 * groups
 * 
 * 16    23/10/97 14:22 Oliverc
 * Changed lights to check visibility between displayed group and group
 * light is in
 * 
 * 15    23/10/97 13:53 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 14    26/09/97 11:13 Oliverc
 * Added function to test visible overlap between two groups
 * 
 * 13    25/09/97 16:05 Oliverc
 * Added group connectivity and visibility functions
 * 
 * 12    16/09/97 11:00 Collinsd
 * Added Chris's code
 * 
 * 11    20/08/97 12:18 Oliverc
 * Triangle visibility statistics now kept (min, max, avg) for all
 * background groups and written to .VIS file in level folder iff player
 * using vanilla '-data' command line option to indicate he wants to use
 * PROJXDATA environment variable
 * 
 * 10    15-02-97 9:32p Collinsd
 * Portals now use variable execute buffers.  They also
 * allocate/deallocate themselves properly now.
 * 
 * 9     12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 8     4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 7     10/04/96 10:32a Phillipd
 * 
 * 6     28/08/96 11:15 Collinsd
 * 
 * 5     8/13/96 4:08p Phillipd
 * 
 * 4     22/07/96 14:08 Collinsd
 * Added visipoly display.
 * 
 * 3     7/21/96 4:27p Phillipd
 * added asynchrinus(??) execution ..so lights happen at the same time as
 * the last group is being displayed...
 * 
 * 2     7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 1     7/16/96 10:58a Phillipd
 */

#ifndef VISI_INCLUDED
#define VISI_INCLUDED

#include "typedefs.h"
#include <math.h>
#include <time.h>
#include "New3D.h"
#include "Mload.h"
#include "ships.h"
#include "lights.h"
#include "skin.h"
#include "d3d.h"
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
void  OurVispolyTransformVertices( DWORD VertexCount, D3DTRANSFORMDATA * Data );
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


#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
void  MyVispolyTransformVertices( DWORD VertexCount, D3DTRANSFORMDATA * Data );
BOOL CWDisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam );
void	CWExecute( D3DEXECUTEBUFFERDESC *debDesc, long NumVertsToXform);
void	CWExecute2(	LPDIRECT3DDEVICE lpDev,
					LPDIRECT3DEXECUTEBUFFER execbuf,
					LPDIRECT3DVIEWPORT lpView,
					WORD cwFlags);
/*-------------------------------------------------------------------------*/
#endif

#endif	// VISI_INCLUDED