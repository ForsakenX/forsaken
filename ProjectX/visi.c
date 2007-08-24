/*
 * V I S I . C
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 96 $
 *
 * $Header: /PcProjectX/visi.c 96    5/11/98 3:34p Oliverc $
 *
 * $Log: /PcProjectX/visi.c $
 * 
 * 96    5/11/98 3:34p Oliverc
 * Made Z_TRICK work as it should (but left disabled for patch beta 4 as
 * it messes up on translucent external views in certain levels)
 * 
 * 95    20/08/98 4:04p Oliverc
 * Added SetViewportError() debug function (enabled on DEBUG_VIEWPORT
 * build switch)
 * 
 * 94    17/08/98 18:00 Philipy
 * removed loads of unreferenced local variables
 * 
 * 93    24/07/98 16:03 Phillipd
 * 
 * 92    6/11/98 9:29a Phillipd
 * 
 * 91    28/04/98 9:50 Oliverc
 * Enabled debug rays for non-FINAL_RELEASE version
 * 
 * 90    6/04/98 11:32 Collinsd
 * Added show restart zone to debug menu.
 * 
 * 89    3/04/98 18:10 Collinsd
 * Moved position of xmem.h
 * 
 * 88    23/03/98 9:45 Philipy
 * fixed corruption bug in AddIndirectVisible
 * 
 * 87    7/03/98 20:05 Collinsd
 * Bike Glows now on option and debug rays no longer loaded.
 * 
 * 86    3/03/98 17:00 Oliverc
 * New multiplayer CTF mode stuff (1st attempt)
 * 
 * 85    21/02/98 19:44 Oliverc
 * Added optimisation: portals not visible if camera behind them
 * 
 * 84    20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 83    20/02/98 12:30 Oliverc
 * Prototype goal load/release/check/display for capture the flag
 * multiplayer
 * 
 * 82    26/01/98 17:49 Oliverc
 * Moved calculation of clip planes to once per frame instead of once per
 * portal vertex (!) and optimised clip plane test
 * 
 * 81    24/01/98 18:42 Oliverc
 * 2D portal extent now considered empty is min >= max (rather than just
 * >)
 * 
 * 80    1/24/98 5:10p Phillipd
 * 
 * 79    1/23/98 11:10a Phillipd
 * malloc Bug fixed
 * 
 * 78    22/01/98 15:06 Oliverc
 * Added loading of pre-calculated group connectivity, visibility,
 * indirect visibility, and sound attentuation tables
 * 
 * 77    5/01/98 19:04 Collinsd
 * Olly fixed visiblegroups list.
 * 
 * 76    20/12/97 17:41 Oliverc
 * 1st attempt at fixing portal Z-clipping bug
 * 
 * 75    1/12/97 10:51 Oliverc
 * Fixed memory alloced but not freed bugs
 * 
 * 74    11/29/97 4:36p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 73    7/11/97 9:24 Collinsd
 * Added point in group bounding box function
 * 
 * 72    6/11/97 19:03 Collinsd
 * Find Overlapping Visible Groups function Added.
 * 
 * 71    30/10/97 17:00 Collinsd
 * Clipping of BGObjects now works.
 * 
 * 70    30/10/97 11:20 Oliverc
 * Added FindClipGroup() function for clipping objects that span multiple
 * groups
 * 
 * 69    24/10/97 10:37 Philipy
 * Updated debug menus
 * 
 * 68    24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 67    23/10/97 14:22 Oliverc
 * Changed lights to check visibility between displayed group and group
 * light is in
 * 
 * 66    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 65    22/10/97 19:27 Collinsd
 * Fixed bug in debug code....
 * 
 * 64    22/10/97 15:48 Collinsd
 * Ooopss
 * 
 * 63    22/10/97 15:37 Collinsd
 * Added error checking in secondary missile collision.
 * 
 * 62    21/10/97 13:14 Philipy
 * MAXLEVELS is now defined in main.h
 * 
 * 61    15/10/97 19:06 Collinsd
 * Fixed guts comming out bug. and fixed mutually visible group bug.
 * 
 * 60    26/09/97 11:13 Oliverc
 * Added function to test visible overlap between two groups
 * 
 * 59    25/09/97 16:58 Collinsd
 * Added group link list to pickups/secondary added more code for
 * bgobjects.
 * 
 * 58    25/09/97 16:05 Oliverc
 * Added group connectivity and visibility functions
 * 
 * 57    22/09/97 10:40 Collinsd
 * Software version works again. ( Now with trasnsluecency )
 * 
 * 56    18/09/97 11:09 Collinsd
 * Added Chris's 565 stretch.
 * 
 * 55    17/09/97 16:37 Collinsd
 * Blit now works in software.
 * 
 * 54    16/09/97 17:52 Collinsd
 * More of Chris's stuff works.
 * 
 * 53    16/09/97 11:00 Collinsd
 * Added Chris's code
 * 
 * 52    15/09/97 10:46 Oliverc
 * Started to fix portal clipping bug
 * 
 * 51    12/09/97 10:50 Collinsd
 * Added global portal clipping on/off using shift f7
 * 
 * 50    2/09/97 17:37 Oliverc
 * Added debug lines display from .RAY file
 * 
 * 49    20/08/97 12:18 Oliverc
 * Triangle visibility statistics now kept (min, max, avg) for all
 * background groups and written to .VIS file in level folder iff player
 * using vanilla '-data' command line option to indicate he wants to use
 * PROJXDATA environment variable
 * 
 * 48    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 47    16/07/97 11:31 Collinsd
 * Added collision zone display.
 * 
 * 46    15/07/97 10:11 Collinsd
 * Added display of triggerzones on Shift F10 in debug mode.
 * 
 * 45    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 44    5/07/97 16:31 Collinsd
 * Put OPT_ON's around opimisations off
 * 
 * 43    6/24/97 11:12a Phillipd
 * 
 * 42    6/12/97 2:27p Phillipd
 * 
 * 41    5/22/97 9:19a Phillipd
 * 
 * 40    26/04/97 14:49 Collinsd
 * Optimisations now on def.
 * 
 * 39    27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 38    15-02-97 9:32p Collinsd
 * Portals now use variable execute buffers.  They also
 * allocate/deallocate themselves properly now.
 * 
 * 37    14-02-97 11:27a Collinsd
 * Added check for portal display code overflow
 * 
 * 36    13/02/97 18:00 Oliverc
 * Fixed bug in whole level display when outside map
 * 
 * 35    12/02/97 12:54 Oliverc
 * Clears turned on automatically when whole level displayed
 * 
 * 34    11/02/97 21:00 Oliverc
 * Now displays all groups when go outside map
 * 
 * 33    2/03/97 5:16p Phillipd
 * Translusceny is now controlled by global execute buffers.... which is
 * much better...
 * 
 * 32    22-01-97 6:00p Collinsd
 * Added debug node cube.
 * 
 * 31    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 30    12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 29    5/12/96 16:17 Oliverc
 * Integrated new portal format (portalpolys grouped into logical portals
 * and converted into preprojected collisionpolys), BUT bug in portal
 * transition routine remains to be fixed
 * 
 * 28    11/21/96 2:37p Phillipd
 * 
 * 27    4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 26    17/10/96 12:40 Collinsd
 * Added blend types to all transparent exec lists.
 * 
 * 25    10/15/96 2:21p Phillipd
 * 
 * 24    11/10/96 10:31 Oliverc
 * Revised portal clipping to work with odd main window sizes
 * 
 * 23    10/10/96 21:23 Oliverc
 * Fixed portal clipping bug (but not entirely sure why it works...)
 * 
 * 22    10/07/96 11:54a Phillipd
 * 
 * 21    10/06/96 5:04p Phillipd
 * We now have our own text debug print info thing...
 * which speeds up the game by 10%.....
 * 
 * 20    10/05/96 2:02p Phillipd
 * 
 * 19    10/04/96 10:32a Phillipd
 * 
 * 18    9/18/96 5:35p Phillipd
 * 
 * 17    11/09/96 19:00 Collinsd
 * Added tests for RAMP emulation ( Although may not work)
 * Also fixed gravity effect to be dependent on GLOBAL_SCALE
 * 
 * 16    30/08/96 17:31 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 15    28/08/96 11:55 Collinsd
 * Added show skin code + more options for show visi.
 * 
 * 14    28/08/96 11:15 Collinsd
 * 
 * 13    8/16/96 2:43p Phillipd
 * 
 * 12    8/09/96 2:02p Phillipd
 * 
 * 11    8/08/96 5:39p Phillipd
 * 
 * 10    7/29/96 12:16p Phillipd
 * 
 * 9     7/24/96 9:51a Phillipd
 * 
 * 8     23/07/96 18:01 Collinsd
 * Added visipoly line mode and group in mode.
 * 
 * 7     7/23/96 2:38p Phillipd
 * 
 * 6     22/07/96 14:47 Collinsd
 * 
 * 5     22/07/96 14:08 Collinsd
 * Added visipoly display.
 * 
 * 4     7/21/96 4:27p Phillipd
 * added asynchrinus(??) execution ..so lights happen at the same time as
 * the last group is being displayed...
 * 
 * 3     19/07/96 12:34 Oliverc
 * Changed ship <-> background collision routine
 * to track movement of ship through portals
 * 
 * 2     7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 1     7/16/96 10:58a Phillipd
 */
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*	All routines to do with Visipolys...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "mload.h"
#include "camera.h"
#include "visi.h"
#include <float.h>
#include "file.h"
#include "triggers.h"
#include "main.h"
#include "lines.h"
#include "teleport.h"
#include "extforce.h"
#include "goal.h"
#include "title.h"
#include "restart.h"
#include "XMem.h"


#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

extern void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval );

extern float hfov;
extern float screen_aspect_ratio;
extern int outside_map;
extern	BOOL	DoClipping;
extern	CAMERA	CurrentCamera;


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Externals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
extern	BOOL			CTF;
extern	BOOL			CaptureTheFlag;

extern	MATRIX			ProjMatrix;
extern	TLOADHEADER		Tloadheader;
extern	D3DMATRIX		proj;
extern	D3DMATRIX		view;
extern	D3DMATRIXHANDLE hProj;
extern	D3DMATRIXHANDLE hView;
extern	MCLOADHEADER	MCloadheader;
extern	BOOL			UsedStippledAlpha;

extern	DWORD			CurrentSrcBlend;
extern	DWORD			CurrentDestBlend;
extern	DWORD			CurrentTextureBlend;

extern	BSP_NODE *	OldCollideNode;

extern	BOOL			PowerVR;
extern	BOOL			bPolySort;
extern	uint16			GroupTris[ MAXGROUPS ];
extern	int				use_local_data;
extern	LINE			Lines[ MAXLINES ];
extern	char			LevelNames[MAXLEVELS][128];                        
extern	MLOADHEADER		Mloadheader;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Globals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#define MAXPORTVERTS (MAXPORTALSPERGROUP * MAXVERTSPERPORTAL)

#define	MAXGROUPSVISIBLE 16

static int GTabRowSize;

#define GROUP2GROUP_OFFSET( G1, G2 )	( ( (G2) >> 5 ) + ( (G1) * GTabRowSize ) )
#define GROUP2GROUP_MASK( G1, G2 )		( 1 << ( (G2) & 31 ) )

#define DIV_CEIL( N, D ) ( (int32) ( (N) + (D) - 1 ) / (D) )

typedef struct _GROUPRELATION
{
	uint32 *table;
	GROUPLIST list[ MAXGROUPS ];
} GROUPRELATION;


static GROUPRELATION ConnectedGroup;
static GROUPRELATION VisibleGroup;
static GROUPRELATION IndirectVisibleGroup;

static BOOL InitConnections = FALSE;


uint16 Num_IndirectVisible = 0;
uint16 IndirectVisible[ MAXGROUPS ];

static VECTOR clip_top, clip_bottom, clip_left, clip_right; // clipping planes

int		NumOfVertsConsidered= 0;
int		NumOfVertsTouched= 0;

uint16	GroupImIn;
D3DTRANSFORMDATA Data;
DWORD VertexCount = 0;
DWORD Offscreen = 0;
VERT TestVerts[MAXPORTVERTS];
VISPOLVERTEX	VisVerts[MAXPORTVERTS];
uint16	CurrentGroupVisible;						// the real group thats visible
uint16	GroupInVisibleList;							// where it is in the visibility list
uint16	NumGroupsVisible;
uint16	GroupsVisible[MAXGROUPS];
D3DRECT	GroupVisibleExtents[MAXGROUPS];
uint16	NumPortalsVisible;
uint16	PortalsVisible[MAXPORTALSPERGROUP];
D3DRECT	PortalExtents[MAXGROUPS];
D3DVIEWPORT	OldViewPort;
D3DVIEWPORT	PresentViewPort;
MATRIX	VisPolyMatrix = {
				1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 1.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F };
uint16	IsGroupVisible[MAXGROUPS];

struct
{
	uint32 tsum;
	uint32 tmax;
	uint32 tmin;
	uint32 visits;
} VisiStats[ MAXGROUPS ];


typedef struct
{
	uint16 group;
	struct
	{
		double x, y, z;
	} from, to;
} DebugRay;


static void InitDebugRays( void )
{
#if 1
	char fname[ 256 ];
	FILE *rf;
	DebugRay ray;
	uint16 line;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], fname, ".ray" );
	rf = fopen( fname, "rb" );
	if ( !rf )
		return;
	while ( fread( &ray.group, sizeof( ray.group ), 1, rf ) )
	{
		if ( !fread( &ray.from, sizeof( double ), 3, rf ) )
			break;
		if ( !fread( &ray.to, sizeof( double ), 3, rf ) )
			break;
		line = FindFreeLine();
		if ( line != (uint16) -1 )
		{
			Lines[ line ].StartPos.x = (float) ray.from.x;
			Lines[ line ].StartPos.y = (float) ray.from.y;
			Lines[ line ].StartPos.z = (float) ray.from.z;
			Lines[ line ].EndPos.x = (float) ray.to.x;
			Lines[ line ].EndPos.y = (float) ray.to.y;
			Lines[ line ].EndPos.z = (float) ray.to.z;
			Lines[ line ].StartCol.R = 255;
			Lines[ line ].StartCol.G = 64;
			Lines[ line ].StartCol.B = 64;
			Lines[ line ].EndCol.R = 64;
			Lines[ line ].EndCol.G = 255;
			Lines[ line ].EndCol.B = 64;
			Lines[ line ].StartTrans = 255;
			Lines[ line ].EndTrans = 255;
			Lines[ line ].Group = ray.group;
		}
	}
	fclose( rf );
#endif
}


void InitVisiStats( MLOADHEADER *m )
{
	int j;
	GROUP *g;
	VECTOR min, max;

	for ( j = 0; j < m->num_groups; j++ )
	{
		VisiStats[ j ].tsum = 0;
		VisiStats[ j ].tmax = 0;
		VisiStats[ j ].tmin = -1;
		VisiStats[ j ].visits = 0;
		g = &m->Group[ j ];
		min.x = g->center.x - g->half_size.x;
		min.y = g->center.y - g->half_size.y;
		min.z = g->center.z - g->half_size.z;
		max.x = g->center.x + g->half_size.x;
		max.y = g->center.y + g->half_size.y;
		max.z = g->center.z + g->half_size.z;
//		CreateBoundingBox( &min, &max, j );
	}
#ifndef FINAL_RELEASE
	InitDebugRays();
#endif
}


BOOL OutputVisiStats( MLOADHEADER *m, char *lname )
{
	FILE *f;
	char fname[ 256 ];
	int j, k;

	if ( !use_local_data )
		return TRUE;

	Change_Ext( lname, fname, ".vis" );
	f = fopen( fname, "w" );
	if ( !f )
		return FALSE;

	fprintf( f, "Group     TMAX      TAVG      TMIN      Visits    Name\n" );
	for ( j = 0; j < m->num_groups; j++ )
	{
		fprintf( f, "%s%3d %9d %9d %9d %9d      %s\n",
			( VisiStats[ j ].tmax > 1000 ) ? "*" : " ",
			j, VisiStats[ j ].tmax,
			( VisiStats[ j ].visits ) ? VisiStats[ j ].tsum / VisiStats[ j ].visits : 0,
			( VisiStats[ j ].visits ) ? VisiStats[ j ].tmin : 0,
			VisiStats[ j ].visits,
			m->Group[ j ].name );
	}

	fprintf( f, "\n\nGROUP       CONNECTED GROUPS\n" );
	for ( j = 0; j < m->num_groups; j++ )
	{
		fprintf( f, "%-10s %2d", m->Group[ j ].name, ConnectedGroup.list[ j ].groups );
		for ( k = 0; k < ConnectedGroup.list[ j ].groups; k++ )
		{
			fprintf( f, " %s", m->Group[ ConnectedGroup.list[ j ].group[ k ] ].name );
		}
		fprintf( f, "\n" );
	}

	fprintf( f, "\n\nGROUP       VISIBLE GROUPS\n" );
	for ( j = 0; j < m->num_groups; j++ )
	{
		fprintf( f, "%-10s %2d", m->Group[ j ].name, VisibleGroup.list[ j ].groups );
		for ( k = 0; k < VisibleGroup.list[ j ].groups; k++ )
		{
			fprintf( f, " %s", m->Group[ VisibleGroup.list[ j ].group[ k ] ].name );
		}
		fprintf( f, "\n" );
	}


	fclose( f );

	return TRUE;
}


static void RelateGroups( GROUPRELATION *rel, uint16 g1, uint16 g2 )
{
	uint32 *t, mask;

	t = rel->table + GROUP2GROUP_OFFSET( g1, g2 );
	mask = GROUP2GROUP_MASK( g1, g2 );
	if ( !( *t & mask ) )
	{
		*t |= mask;
		rel->list[ g1 ].groups++;
	}
}


static BOOL AreGroupsRelated( GROUPRELATION *rel, uint16 g1, uint16 g2 )
{
	uint32 *t, mask;

	t = rel->table + GROUP2GROUP_OFFSET( g1, g2 );
	mask = GROUP2GROUP_MASK( g1, g2 );
	if ( *t & mask )
		return TRUE;
	else
		return FALSE;
}


static void FindGroupsVisible( GROUPRELATION *vis, uint16 from_group, VISTREE *visible )
{
	int j;

	RelateGroups( vis, from_group, visible->group );
	for ( j = 0; j < visible->num_visible; j++ )
	{
		FindGroupsVisible( vis, from_group, &visible->visible[ j ] );
	}
}


BOOL GroupsAreConnected( uint16 g1, uint16 g2 )
{
	return AreGroupsRelated( &ConnectedGroup, g1, g2 );
}


BOOL GroupsAreVisible( uint16 g1, uint16 g2 )
{
	return AreGroupsRelated( &VisibleGroup, g1, g2 );
}


BOOL GroupsAreIndirectVisible( uint16 g1, uint16 g2 )
{
	if ( IndirectVisibleGroup.table )
		return AreGroupsRelated( &IndirectVisibleGroup, g1, g2 );
	else
	{
		static int failed = 0;

		if ( !failed++ )
			Msg( "No IndirectVisible table loaded\n" );
		return FALSE;
	}
}


BOOL ReadGroupConnections( MLOADHEADER *m, char **pbuf )
{
	uint32 tabsize;
	uint16 g;
	char *buf;
	uint16 *buf16;

	// initialise data structures to reasonable defaults
	for ( g = 0; g < MAXGROUPS; g++ )
	{
		ConnectedGroup.list[ g ].groups = 0;
		ConnectedGroup.list[ g ].group = NULL;
		VisibleGroup.list[ g ].groups = 0;
		VisibleGroup.list[ g ].group = NULL;
		IndirectVisibleGroup.list[ g ].groups = 0;
		IndirectVisibleGroup.list[ g ].group = NULL;
	}

	InitConnections = TRUE;

	// allocate tables
	GTabRowSize = DIV_CEIL( MAXGROUPS, 32 );
	tabsize = m->num_groups * GTabRowSize * sizeof( uint32 );
	if ( !ConnectedGroup.table )
	{
		ConnectedGroup.table = (uint32 *) malloc( tabsize );
		if ( !ConnectedGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for ConnectedGroup.table\n" );
			return FALSE;
		}
	}
	if ( !VisibleGroup.table )
	{
		VisibleGroup.table = (uint32 *) malloc( tabsize );
		if ( !VisibleGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for VisibleGroup.table\n" );
			return FALSE;
		}
	}
	if ( !IndirectVisibleGroup.table )
	{
		IndirectVisibleGroup.table = (uint32 *) malloc( tabsize );
		if ( !IndirectVisibleGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for IndirectVisibleGroup.table\n" );
			return FALSE;
		}
	}
	memset( ConnectedGroup.table, 0, tabsize );
	memset( VisibleGroup.table, 0, tabsize );
	memset( IndirectVisibleGroup.table, 0, tabsize );

	buf = *pbuf;

	// read connectivity table
	memmove( ConnectedGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (uint16 *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		ConnectedGroup.list[ g ].groups = *buf16++;
		if ( ConnectedGroup.list[ g ].groups )
		{
			ConnectedGroup.list[ g ].group = (uint16 *) calloc( ConnectedGroup.list[ g ].groups, sizeof( uint16 ) );
			if ( !ConnectedGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for ConnectedGroup.list[ %d ]\n", g );
				return FALSE;
			}
			memmove( ConnectedGroup.list[ g ].group, buf16, ConnectedGroup.list[ g ].groups * sizeof( uint16 ) );
			buf16 += ConnectedGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	// read visibility summary table
	memmove( VisibleGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (uint16 *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		VisibleGroup.list[ g ].groups = *buf16++;
		if ( VisibleGroup.list[ g ].groups )
		{
			VisibleGroup.list[ g ].group = (uint16 *) calloc( VisibleGroup.list[ g ].groups, sizeof( uint16 ) );
			if ( !VisibleGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for VisibleGroup.list[ %d ]\n", g );
				return FALSE;
			}
			memmove( VisibleGroup.list[ g ].group, buf16, VisibleGroup.list[ g ].groups * sizeof( uint16 ) );
			buf16 += VisibleGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	// read indirect visibility summary table
	memmove( IndirectVisibleGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (uint16 *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		IndirectVisibleGroup.list[ g ].groups = *buf16++;
		if ( IndirectVisibleGroup.list[ g ].groups )
		{
			IndirectVisibleGroup.list[ g ].group = (uint16 *) calloc( IndirectVisibleGroup.list[ g ].groups, sizeof( uint16 ) );
			if ( !IndirectVisibleGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for IndirectVisibleGroup.list[ %d ]\n", g );
				return FALSE;
			}
			memmove( IndirectVisibleGroup.list[ g ].group, buf16, IndirectVisibleGroup.list[ g ].groups * sizeof( uint16 ) );
			buf16 += IndirectVisibleGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	*pbuf = buf;

	return TRUE;
}



BOOL FindGroupConnections( MLOADHEADER *m )
{
	uint32 tabsize;
	uint16 g, p, g2;
	GROUP *group;
	PORTAL *portal;
	uint16 gnum;

	for ( g = 0; g < MAXGROUPS; g++ )
	{
		ConnectedGroup.list[ g ].groups = 0;
		ConnectedGroup.list[ g ].group = NULL;
		VisibleGroup.list[ g ].groups = 0;
		VisibleGroup.list[ g ].group = NULL;
		IndirectVisibleGroup.list[ g ].groups = 0;
		IndirectVisibleGroup.list[ g ].group = NULL;
	}

	InitConnections = TRUE;

	GTabRowSize = (int)ceil(MAXGROUPS/32.0F);
	tabsize = MAXGROUPS * GTabRowSize * sizeof( uint32 );
	if ( !ConnectedGroup.table )
	{
		ConnectedGroup.table = (uint32 *) malloc( tabsize );
		if ( !ConnectedGroup.table )
		{
			Msg( "FindGroupConnections: failed malloc for ConnectedGroup.table\n" );
			return FALSE;
		}
	}
	if ( !VisibleGroup.table )
	{
		VisibleGroup.table = (uint32 *) malloc( tabsize );
		if ( !VisibleGroup.table )
		{
			Msg( "FindGroupConnections: failed malloc for VisibleGroup.table\n" );
			return FALSE;
		}
	}
	IndirectVisibleGroup.table = NULL;
	memset( ConnectedGroup.table, 0, tabsize );
	memset( VisibleGroup.table, 0, tabsize );

	for ( g = 0; g < m->num_groups; g++ )
	{
		group = &m->Group[ g ];
		ConnectedGroup.list[ g ].groups = 0;
		VisibleGroup.list[ g ].groups = 0;
		// make group visible from itself
		RelateGroups( &VisibleGroup, g, g );
		for ( p = 0; p < group->num_portals; p++ )
		{
			portal = &group->Portal[ p ];
			// find groups connected to this one
			RelateGroups( &ConnectedGroup, g, portal->visible.group );
			// find groups visible from this one
			FindGroupsVisible( &VisibleGroup, g, &portal->visible );
		}
		if ( ConnectedGroup.list[ g ].groups )
		{
			ConnectedGroup.list[ g ].group = (uint16 *) calloc( ConnectedGroup.list[ g ].groups, sizeof( uint16 ) );
			if ( !ConnectedGroup.list[ g ].group )
			{
				Msg( "FindGroupConnections: failed X_calloc for ConnectedGroup.list[ %d ]\n", g );
				return FALSE;
			}
			gnum = 0;
			for ( g2 = 0; g2 < m->num_groups; g2++ )
			{
				if ( GroupsAreConnected( g, g2 ) )
				{
					ConnectedGroup.list[ g ].group[ gnum++ ] = g2;
				}
			}
		}
		else
		{
			ConnectedGroup.list[ g ].group = NULL;
		}
		if ( VisibleGroup.list[ g ].groups )
		{
			VisibleGroup.list[ g ].group = (uint16 *) calloc( VisibleGroup.list[ g ].groups, sizeof( uint16 ) );
			if ( !VisibleGroup.list[ g ].group )
			{
				Msg( "FindGroupConnections: failed X_calloc for VisibleGroup.list[ %d ]\n", g );
				return FALSE;
			}
			gnum = 0;
			for ( g2 = 0; g2 < m->num_groups; g2++ )
			{
				if ( GroupsAreVisible( g, g2 ) )
				{
					VisibleGroup.list[ g ].group[ gnum++ ] = g2;
				}
			}
		}
		else
		{
			VisibleGroup.list[ g ].group = NULL;
		}
	}

	return TRUE;
}


void FreeGroupConnections( void )
{
	int g;

	if ( !InitConnections )
		return;
	for ( g = 0; g < MAXGROUPS; g++ )
	{
		ConnectedGroup.list[ g ].groups = 0;
		if ( ConnectedGroup.list[ g ].group )
		{
			free( ConnectedGroup.list[ g ].group );
		}
		ConnectedGroup.list[ g ].group = NULL;
		VisibleGroup.list[ g ].groups = 0;
		if ( VisibleGroup.list[ g ].group )
		{
			free( VisibleGroup.list[ g ].group );
		}
		VisibleGroup.list[ g ].group = NULL;
		IndirectVisibleGroup.list[ g ].groups = 0;
		if ( IndirectVisibleGroup.list[ g ].group )
		{
			free( IndirectVisibleGroup.list[ g ].group );
		}
		IndirectVisibleGroup.list[ g ].group = NULL;
	}
	if ( ConnectedGroup.table )
	{
		free( ConnectedGroup.table );
		ConnectedGroup.table = NULL;
	}
	if ( VisibleGroup.table )
	{
		free( VisibleGroup.table );
		VisibleGroup.table = NULL;
	}
	if ( IndirectVisibleGroup.table )
	{
		free( IndirectVisibleGroup.table );
		IndirectVisibleGroup.table = NULL;
	}
}




GROUPLIST *ConnectedGroups( uint16 g )
{
	return &ConnectedGroup.list[ g ];
}




GROUPLIST *VisibleGroups( uint16 g )
{
	if( g >= Mloadheader.num_groups )
		Msg( "VisibleGroups() Group %d out of range", g );

	if( VisibleGroup.list[ g ].groups > Mloadheader.num_groups )
		Msg( "VisibleGroups.list[%d].groups=%d more than %d groups", g, VisibleGroup.list[ g ].groups, Mloadheader.num_groups );

	if( !VisibleGroup.list[ g ].group )
		Msg( "VisibleGroups.list[%d].group Invalid", g );

	return &VisibleGroup.list[ g ];
}


GROUPLIST *IndirectVisibleGroups( uint16 g )
{
	return &IndirectVisibleGroup.list[ g ];
}



int VisibleOverlap( uint16 g1, uint16 g2, uint16 *overlapping_group )
{
	uint32 *t1, *t2, overlap;
	int j;
	int gnum;
	GROUPLIST *l1, *l2;
	if( g1 > Mloadheader.num_groups || g2 > Mloadheader.num_groups )
		return 0;

	t1 = VisibleGroup.table + GROUP2GROUP_OFFSET( g1, 0 );
	t2 = VisibleGroup.table + GROUP2GROUP_OFFSET( g2, 0 );
	overlap = 0;
	for ( j = 0; j < GTabRowSize; j++ )
	{
		overlap |= *t1++ & *t2++;
	}
	if ( !overlap )
		return 0;
	if ( !overlapping_group )
		return 1;

	l1 = &VisibleGroup.list[ g1 ];
	l2 = &VisibleGroup.list[ g2 ];
	g1 = 0;
	g2 = 0;
	gnum = 0;
	do {
		if ( l1->group[ g1 ] < l2->group[ g2 ] )
		{
			g1++;
		}
		else if ( l1->group[ g1 ] > l2->group[ g2 ])
		{
			g2++;
		}
		else // groups are same
		{
			overlapping_group[ gnum++ ] = l1->group[ g1 ];
			g1++;
			g2++;
		}
	} while ( g1 < l1->groups && g2 < l2->groups );

	return gnum;
}


void InitIndirectVisible( uint16 g )
{
	GROUPLIST *list;
	int j;

	list = &IndirectVisibleGroup.list[ g ];
	for ( j = 0; j < list->groups; j++ )
	{
		IndirectVisible[ j ] = list->group[ j ];
	}
	Num_IndirectVisible = list->groups;
}


void AddIndirectVisible( uint16 g )
{
	static uint16 AddedVisible[ MAXGROUPS ];
	int current, next;
	int added;
	GROUPLIST *add;

	add = &IndirectVisibleGroup.list[ g ];
	current = 0;
	next = 0;
	added = 0;
	while ( current < Num_IndirectVisible || next < add->groups )
	{
		if ( current < Num_IndirectVisible )
		{
			if ( next < add->groups )
			{
				if ( IndirectVisible[ current ] < add->group[ next ] )
				{
					AddedVisible[ added++ ] = IndirectVisible[ current++ ];
				}
				else if ( IndirectVisible[ current ] > add->group[ next ] )
				{
					AddedVisible[ added++ ] = add->group[ next++ ];
				}
				else
				{
					AddedVisible[ added++ ] = IndirectVisible[ current++ ];
					next++;
				}
			}
			else
			{
				AddedVisible[ added++ ] = IndirectVisible[ current++ ];
			}
		}
		else
		{
			AddedVisible[ added++ ] = add->group[ next++ ];
		}
	}
	Num_IndirectVisible = added;
	memmove( IndirectVisible, AddedVisible, Num_IndirectVisible * sizeof( IndirectVisible[ 0 ] ) );
}


#define HUGE_VALUE	(FLT_MAX * 0.5F)



#ifdef USEINLINE
_inline
#endif
 int EmptyXYExtent( EXTENT *e )
{
	return ( e->min.x >= e->max.x ) || ( e->min.y >= e->max.y );
}



static void
MinimiseXYExtent( EXTENT *e1, EXTENT *e2, EXTENT *e )
{
	e->min.x = MAX( e1->min.x, e2->min.x );
	e->min.y = MAX( e1->min.y, e2->min.y );
	e->max.x = MIN( e1->max.x, e2->max.x );
	e->max.y = MIN( e1->max.y, e2->max.y );
}


static void
MaximiseExtent( EXTENT *e1, EXTENT *e2, EXTENT *e )
{
	e->min.x = MIN( e1->min.x, e2->min.x );
	e->min.y = MIN( e1->min.y, e2->min.y );
	e->min.z = MIN( e1->min.z, e2->min.z );
	e->max.x = MAX( e1->max.x, e2->max.x );
	e->max.y = MAX( e1->max.y, e2->max.y );
	e->max.z = MAX( e1->max.z, e2->max.z );
}

extern BOOL ZClearsOn;
extern BOOL g_OddFrame;

static int
Transform2Viewport( CAMERA *cam, VISLIST *v, VERT *wpos, VECTOR *vpos )
{
	int clip;

    VisPolyApplyMatrix( (MATRIX *) &v->viewproj, (VECTOR *) wpos, vpos );
	clip = 0;
#ifdef Z_TRICK
	if ( ZClearsOn ? ( vpos->z > 1.0F ) : g_OddFrame ? ( vpos->z < 0.5F ) : ( vpos->z > 0.5F ) )
#else
	if ( vpos->z > 1.0F )
#endif
	{
		clip |= D3DCLIP_FRONT;
	    VisPolyApplyMatrix( (MATRIX *) &cam->View, (VECTOR *) wpos, vpos );
#if 0
		if ( vpos->x < 0.0F )
			vpos->x = -1.0F;
		else
			vpos->x = 1.0F;
		if ( vpos->y < 0.0F )
			vpos->y = -1.0F;
		else
			vpos->y = 1.0F;
#else
		if ( DotProduct( vpos, &clip_left ) < 0.0F )
		{
			clip |= D3DCLIP_LEFT;
		}
		else if ( DotProduct( vpos, &clip_right ) < 0.0F )
		{
			clip |= D3DCLIP_RIGHT;
		}
		if ( DotProduct( vpos, &clip_top ) < 0.0F )
		{
			clip |= D3DCLIP_TOP;
		}
		else if ( DotProduct( vpos, &clip_bottom ) < 0.0F )
		{
			clip |= D3DCLIP_BOTTOM;
		}
		return clip;
#endif
	}

	if ( vpos->x < -1.0F )
	{
		clip |= D3DCLIP_LEFT;
		vpos->x = (float) v->viewport->dwX;
	}
	else if ( vpos->x > 1.0F )
	{
		clip |= D3DCLIP_RIGHT;
		vpos->x = (float) ( v->viewport->dwX + v->viewport->dwWidth );
	}
	else
	{
		vpos->x = v->viewport->dwX + ( v->viewport->dwWidth * 0.5F ) + (v->viewport->dvScaleX * vpos->x);
	}

   	if ( vpos->y < -1.0F )
	{
		clip |= D3DCLIP_BOTTOM;
		vpos->y = (float) ( v->viewport->dwY + v->viewport->dwHeight );
	}
	else if ( vpos->y > 1.0F )
	{
		clip |= D3DCLIP_TOP;
		vpos->y = (float) v->viewport->dwY;
	}
	else
	{
		vpos->y = v->viewport->dwY + ( v->viewport->dwHeight * 0.5F ) - (v->viewport->dvScaleY * vpos->y);
	}

	return clip;
}


static int
VisiblePortalExtent( CAMERA *cam, VISLIST *v, PORTAL *p, EXTENT *e )
{
	VECTOR pos;
	int vnum;
	int init;
	int clip;
	int clip_any;
	int clip_all;
	MCFACE *pface;
	float d;

	pface = &p->Poly[ 0 ];
	d = pface->nx * cam->Pos.x + pface->ny * cam->Pos.y + pface->nz * cam->Pos.z + pface->D;
	if ( d < 0.0F )
		return 0; // camera behind portal
	init = 0;
	clip_any = 0;
	clip_all = D3DCLIP_LEFT | D3DCLIP_RIGHT | D3DCLIP_TOP | D3DCLIP_BOTTOM | D3DCLIP_FRONT;
	for ( vnum = 0; vnum < p->num_vertices_in_portal; vnum++ )
	{
		clip = Transform2Viewport( cam, v, &p->Verts[ vnum ], &pos );
		clip_any |= clip;
		clip_all &= clip;
		if ( !( clip & D3DCLIP_FRONT ) )
		{
			if ( !init )
			{
				e->min = pos;
				e->max = pos;
				init = 1;
			}
			else
			{
				if ( e->min.x > pos.x )
					e->min.x = pos.x;
				if ( e->min.y > pos.y )
					e->min.y = pos.y;
				if ( e->min.z > pos.z )
					e->min.z = pos.z;
				if ( e->max.x < pos.x )
					e->max.x = pos.x;
				if ( e->max.y < pos.y )
					e->max.y = pos.y;
				if ( e->max.z < pos.z )
					e->max.z = pos.z;
			}
		}
	}
	if ( clip_all )
	{
		return 0; // all vertices clipped by main viewport
	}
	if ( clip_any & D3DCLIP_FRONT )
	{
		*e = v->first_visible->extent; // portal clipped by front plane -> force extents fullscreen
	}
	return 1;
}


static void
ProcessVisiblePortal( CAMERA *cam, VISLIST *v, VISTREE *t, EXTENT *e )
{
	VISGROUP *adj_group;
	PORTAL *p;
	EXTENT extent;
	VISTREE *tvis;
	int vnum;

	if ( !t )
		return;
	p = t->portal;
	if ( !VisiblePortalExtent( cam, v, p, &extent ) )
	{
		return; // portal entirely clipped by main viewport
	}
	MinimiseXYExtent( &extent, e, &extent );
	if ( !EmptyXYExtent( &extent ) )
	{
		adj_group = &v->group[ t->group ];
		if ( adj_group->visible )
		{
			MaximiseExtent( &adj_group->extent, &extent, &adj_group->extent );
		}
		else
		{
			adj_group->extent = extent;
			v->last_visible->next_visible = adj_group;
			v->last_visible = adj_group;
			adj_group->next_visible = NULL;
		}
		adj_group->visible++;
		for ( vnum = t->num_visible, tvis = t->visible; vnum--; tvis++)
		{
			ProcessVisiblePortal( cam, v, tvis, &extent );
		}
	}
}


void
FindVisible( CAMERA *cam, MLOADHEADER *Mloadheader )
{
	static int last_outside = 0;
	static BOOL last_clear = 0;
	VISLIST *v;
	VISGROUP *g;
	D3DRECT clip;
	D3DVIEWPORT *vp;
	VISGROUP *gsort, *gprev, *gnext;
	int j;
	float w, h;

	// calculate clipping planes
	w = (float) tan( hfov );
	h = w / screen_aspect_ratio;
	clip_right.x = w;
	clip_right.y = 0.0F;
	clip_right.z = 2.0F;
	NormaliseVector( &clip_right );
	clip_left.x = clip_right.z;
	clip_left.y = 0.0F;
	clip_left.z = clip_right.x;
	clip_right.x = -clip_left.x;
	clip_right.y = 0.0F;
	clip_right.z = clip_left.z;
	clip_top.x = 0.0F;
	clip_top.y = h;
	clip_top.z = 2.0F;
	NormaliseVector( &clip_top );
	clip_bottom.x = 0.0F;
	clip_bottom.y = clip_top.z;
	clip_bottom.z = clip_top.y;
	clip_top.x = 0.0F;
	clip_top.y = -clip_bottom.y;
	clip_top.z = clip_bottom.z;

	v = &cam->visible;
	v->groups = Mloadheader->num_groups;
	// reset group visibilities
	for ( j = 0; j < v->groups; j++ )
	{
		g = &v->group[ j ];
		g->group = j;
		g->visible = 0;
		g->extent.min.x = HUGE_VALUE;
		g->extent.min.y = HUGE_VALUE;
		g->extent.min.z = HUGE_VALUE;
		g->extent.max.x = -HUGE_VALUE;
		g->extent.max.y = -HUGE_VALUE;
		g->extent.min.z = -HUGE_VALUE;
	}

	// initialise current group
	g = &v->group[ cam->GroupImIn ];
	v->first_visible = g;
	v->last_visible = g;
	v->viewport = &cam->Viewport;
	MatrixMultiply( (MATRIX *) &cam->View, (MATRIX *) &cam->Proj, (MATRIX *) &v->viewproj );
	g->next_visible = NULL;
	g->visible = 1;
	g->extent.min.x = (float) v->viewport->dwX;
	g->extent.min.y = (float) v->viewport->dwY;
	g->extent.min.z = -HUGE_VALUE;
	g->extent.max.x = (float) ( v->viewport->dwX + v->viewport->dwWidth );
	g->extent.max.y = (float) ( v->viewport->dwY + v->viewport->dwHeight );
	g->extent.max.z = HUGE_VALUE;

	// process visible portals
	if ( outside_map )
	{
		if ( !last_outside )
		{
			last_clear = myglobs.bClearsOn;
			myglobs.bClearsOn = TRUE;
		}
		for ( j = 0; j < Mloadheader->num_groups; j++ )
		{
			if ( j != cam->GroupImIn )
			{
				g = &v->group[ j ];
				g->extent = v->first_visible->extent;
				v->last_visible->next_visible = g;
				v->last_visible = g;
				g->next_visible = NULL;
				g->visible++;
			}
		}
	}
	else
	{
		if ( last_outside )
		{
			myglobs.bClearsOn = last_clear;
		}
		for ( j = 0; j < Mloadheader->Group[ cam->GroupImIn].num_portals; j++ )
		{
			ProcessVisiblePortal( cam, v, &Mloadheader->Group[ cam->GroupImIn ].Portal [ j ].visible, &g->extent );
		}
	}
	last_outside = outside_map;

	// set viewport and projection matrix for each visible group
	for ( g = v->first_visible; g; g = g->next_visible )
	{
		clip.x1 = (long) floor( g->extent.min.x );
		clip.y1 = (long) floor( g->extent.min.y );
		clip.x2 = (long) ceil( g->extent.max.x );
		clip.y2 = (long) ceil( g->extent.max.y );
		if ( clip.x1 < 0 )
			clip.x1 = 0;
		if ( clip.y1 < 0 )
			clip.y1 = 0;
		vp = &g->viewport;
		*vp = *v->viewport;
		vp->dwX = clip.x1;
		vp->dwY = clip.y1;
		vp->dwWidth = clip.x2 - clip.x1;
		vp->dwHeight = clip.y2 - clip.y1;
		vp->dwWidth += vp->dwWidth & 1;
		vp->dwHeight += vp->dwHeight & 1;
		if ( vp->dwX + vp->dwWidth > v->viewport->dwX + v->viewport->dwWidth )
		{
			if ( vp->dwX > 0 )
				vp->dwX--;
			else
				vp->dwWidth--;
		}
		if ( vp->dwY + vp->dwHeight > v->viewport->dwY + v->viewport->dwHeight )
		{
			if ( vp->dwY > 0 )
				vp->dwY--;
			else
				vp->dwHeight--;
		}
		vp->dvScaleX = vp->dwWidth * 0.5F;
		vp->dvScaleY = vp->dwHeight * 0.5F;
		g->projection = cam->Proj;
		g->projection._11 = ( cam->Proj._11 * v->viewport->dwWidth ) / vp->dwWidth;
		g->projection._22 = ( cam->Proj._22 * v->viewport->dwHeight ) / vp->dwHeight;
		g->projection._31 = 2.0F * ( ( v->viewport->dwX + v->viewport->dwWidth * 0.5F )
									- ( vp->dwX + vp->dwWidth * 0.5F ) )
							/ vp->dwWidth;
		g->projection._32 = -2.0F * ( ( v->viewport->dwY + v->viewport->dwHeight * 0.5F )
									- ( vp->dwY + vp->dwHeight * 0.5F ) )
							/ vp->dwHeight;
	}

	// sort visible groups in depth order
	v->last_visible = v->first_visible;
	g = v->first_visible->next_visible;
	v->first_visible->next_visible = NULL;
	while ( g )
	{
		gnext = g->next_visible;
		gprev = v->first_visible;
		gsort = v->first_visible->next_visible;
		while ( gsort && g->extent.min.z > gsort->extent.min.z )
		{
			gprev = gsort;
			gsort = gsort->next_visible;
		}
		gprev->next_visible = g;
		g->next_visible = gsort;
		if ( !g->next_visible )
			v->last_visible = g;
		g = gnext;
	}

	NumGroupsVisible = 0;
	for ( g = v->first_visible; g; g = g->next_visible )
	{
		GroupsVisible[ NumGroupsVisible++ ] = g->group;
		IsGroupVisible[ g->group ] = 1;
	}
}


int
ClipGroup( CAMERA *cam, uint16 group )
{
    HRESULT rval;
	VISGROUP *g;

	g = &cam->visible.group[ group ];
	if ( !g->visible )
		return 0;

	if( !DoClipping ) g = &cam->visible.group[ cam->visible.first_visible->group ];
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hProj, &g->projection) != D3D_OK)
		return FALSE;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hView, &cam->View) != D3D_OK)
		return FALSE;

	rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport( d3dapp->lpD3DViewport , &g->viewport );
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
		SetViewportError( "ClipGroup", &g->viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
	
	return 1;
}

BOOL FogOff( void );
BOOL FogOn( float Start , float End );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Disp Visipoly Model
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
DisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam ) 
{
	D3DMATRIX	Tempproj;
	D3DMATRIX	Tempview;
	VISGROUP *g;
	int	i;
	uint32 t;
	//float	Start , End;

	Tempproj = proj;
	Tempview = view;

	NumOfVertsConsidered= 0;
	NumOfVertsTouched= 0;


	OldViewPort.dwSize = sizeof(D3DVIEWPORT);
	d3dapp->lpD3DViewport->lpVtbl->GetViewport( d3dapp->lpD3DViewport , &OldViewPort );
	PresentViewPort = OldViewPort;

	GroupImIn = CurrentCamera.GroupImIn;

	if ( GroupImIn != (uint16) -1 )
	{
		DisplayBSPNode( OldCollideNode );

		CreatePortalExecList( Mloadheader, NumGroupsVisible );
		CreateSkinExecList( &MCloadheader, NumGroupsVisible );

		ShowAllColZones( GroupImIn );
		DisplayTeleportsInGroup( GroupImIn );
		DisplayExternalForcesInGroup( GroupImIn );

		t = 0;
		for ( g = cam->visible.first_visible, i = 0; g; g = g->next_visible, i++ )
		{
		 	ClipGroup( &CurrentCamera, (uint16) g->group );
			CurrentGroupVisible = GroupsVisible[i];
			GroupInVisibleList = i;

#if 0
			if( (CurrentGroupVisible&3) == 0 )
			{
				End = DistanceVector2Vector( &cam->Pos , (VECTOR*)&Mloadheader->Group[CurrentGroupVisible].center);
				Start = End - 256.0F;
				FogOn( Start , End );
			}else{
				FogOff();
			}
#endif

			if ( XLight1Group(  Mloadheader, GroupsVisible[i] ) != TRUE  )
				return FALSE;
   			if ( ExecuteSingleGroupMloadHeader(  Mloadheader, (uint16) g->group ) != TRUE  )
					return FALSE;

			DispGroupTriggerAreas( (uint16) g->group );
			if ( CaptureTheFlag || CTF )
				DisplayGoal( (uint16) g->group );
//			ShowAllColZones( (uint16) g->group );

			t += GroupTris[ g->group ];
		}
		// accumulate visibility stats
		VisiStats[ GroupImIn ].tsum += t;
		if ( VisiStats[ GroupImIn ].tmax < t )
			VisiStats[ GroupImIn ].tmax = t;
		if ( VisiStats[ GroupImIn ].tmin > t )
			VisiStats[ GroupImIn ].tmin = t;
		VisiStats[ GroupImIn ].visits++;
	}else{
		if ( ExecuteMloadHeader ( Mloadheader ) != TRUE)
			return FALSE;
	}

	d3dapp->lpD3DViewport->lpVtbl->SetViewport( d3dapp->lpD3DViewport , &OldViewPort );
	proj = Tempproj;
	view = Tempview;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hProj, &proj) != D3D_OK)
		return FALSE;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hView, &view) != D3D_OK)
		return FALSE;
	
	return TRUE;
}


uint16
FindClipGroup( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max )
{
	uint16 group;
	VISGROUP *vg;
	GROUP *mg;
	uint16 in_groups;
	EXTENT extent, *e;
	float extent_size, min_extent_size;

	if ( outside_map )
		return cam->visible.first_visible->group;

	// find extent of visible groups overlapped by object's bounds
	in_groups = 0;
	for ( vg = cam->visible.first_visible; vg; vg = vg->next_visible )
	{
		mg = &m->Group[ vg->group ];
		if ( min->x <= mg->center.x + mg->half_size.x &&
			 min->y <= mg->center.y + mg->half_size.y &&
			 min->z <= mg->center.z + mg->half_size.z &&
			 max->x >= mg->center.x - mg->half_size.x &&
			 max->y >= mg->center.y - mg->half_size.y &&
			 max->z >= mg->center.z - mg->half_size.z )
		{
			if ( in_groups++ )
				MaximiseExtent( &extent, &vg->extent, &extent );
			else
				extent = vg->extent;
		}
	}

	if ( in_groups )
	{
		// find visible group with smallest enclosing extent
		group = cam->visible.first_visible->group;
		e = &cam->visible.first_visible->extent;
		min_extent_size = ( e->max.x - e->min.x ) * ( e->max.y - e->min.y );
		for ( vg = cam->visible.first_visible; vg; vg = vg->next_visible )
		{
			if ( extent.min.x >= vg->extent.min.x &&
				 extent.min.y >= vg->extent.min.y &&
				 extent.max.x <= vg->extent.max.x &&
				 extent.max.y <= vg->extent.max.y )
			{
				e = &vg->extent;
				extent_size = ( e->max.x - e->min.x ) * ( e->max.y - e->min.y );
				if ( extent_size < min_extent_size )
				{
					min_extent_size = extent_size;
					group = vg->group;
				}
			}
		}
	}
	else // object not overlapping any visible groups
		group = (uint16) -1;

	return group;
}


uint16 FindOverlappingVisibleGroups( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max, uint16 * group )
{
	VISGROUP *vg;
	GROUP *mg;
	uint16 in_groups;

	if ( outside_map )
		return 0;

	// find extent of visible groups overlapped by object's bounds
	in_groups = 0;
	for ( vg = cam->visible.first_visible; vg; vg = vg->next_visible )
	{
		mg = &m->Group[ vg->group ];
		if ( min->x <= mg->center.x + mg->half_size.x &&
			 min->y <= mg->center.y + mg->half_size.y &&
			 min->z <= mg->center.z + mg->half_size.z &&
			 max->x >= mg->center.x - mg->half_size.x &&
			 max->y >= mg->center.y - mg->half_size.y &&
			 max->z >= mg->center.z - mg->half_size.z )
		{
			if ( group )
				group[ in_groups ] = vg->group;
			in_groups++;
		}
	}

	return in_groups;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Is point inside bounding box of group
	Input		:	MLOADHEADER	*	Mloadheader
				:	VECTOR		*	Pos
				:	uint16			Group
	Output		:	FALSE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL PointInGroupBoundingBox( MLOADHEADER * Mloadheader, VECTOR * Pos, uint16 group )
{
	VECTOR	Temp;

	Temp.x = Pos->x - Mloadheader->Group[group].center.x;
	if( Temp.x < 0.0F )	Temp.x *= -1.0F;
	Temp.y = Pos->y - Mloadheader->Group[group].center.y;
	if( Temp.y < 0.0F )	Temp.y *= -1.0F;
	Temp.z = Pos->z - Mloadheader->Group[group].center.z;
	if( Temp.z < 0.0F )	Temp.z *= -1.0F;
	if ( (Temp.x <= ( Mloadheader->Group[group].half_size.x ) ) &&
		 (Temp.y <= ( Mloadheader->Group[group].half_size.y ) ) &&
		 (Temp.z <= ( Mloadheader->Group[group].half_size.z ) ) )
	{
		return TRUE;
	}
	return FALSE;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
