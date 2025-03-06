
/*===================================================================
*	All routines to do with Visipolys...
===================================================================*/

#include <stdio.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
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
#include "util.h"
#include "water.h"
#include "render.h"

extern render_info_t render_info;

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

extern void SetViewportError( char *where, render_viewport_t *vp );

extern float hfov;
extern int outside_map;
extern	bool	DoClipping;
extern	CAMERA	CurrentCamera;

/*===================================================================
		Externals...	
===================================================================*/

extern	bool			CTF;
extern	bool			CaptureTheFlag;

extern	MATRIX			ProjMatrix;
extern	TLOADHEADER		Tloadheader;
extern	RENDERMATRIX		proj;
extern	RENDERMATRIX		view;
extern	MCLOADHEADER	MCloadheader;

extern	DWORD			CurrentSrcBlend;
extern	DWORD			CurrentDestBlend;
extern	DWORD			CurrentTextureBlend;

extern	BSP_NODE *	OldCollideNode;

extern	u_int16_t			GroupTris[ MAXGROUPS ];
extern	LINE			Lines[ MAXLINES ];
extern	char			LevelNames[MAXLEVELS][128];                        
extern	MLOADHEADER		Mloadheader;

/*===================================================================
		Globals...	
===================================================================*/

#define MAXPORTVERTS (MAXPORTALSPERGROUP * MAXVERTSPERPORTAL)

#define	MAXGROUPSVISIBLE 16

static int GTabRowSize;

#define GROUP2GROUP_OFFSET( G1, G2 )	( ( (G2) >> 5 ) + ( (G1) * GTabRowSize ) )
#define GROUP2GROUP_MASK( G1, G2 )		( 1 << ( (G2) & 31 ) )

#define DIV_CEIL( N, D ) ( (int32_t) ( (N) + (D) - 1 ) / (D) )

typedef struct _GROUPRELATION
{
	u_int32_t *table;
	GROUPLIST list[ MAXGROUPS ];
} GROUPRELATION;


static GROUPRELATION ConnectedGroup;
static GROUPRELATION VisibleGroup;
static GROUPRELATION IndirectVisibleGroup;

static bool InitConnections = false;


u_int16_t Num_IndirectVisible = 0;
u_int16_t IndirectVisible[ MAXGROUPS ];

static VECTOR clip_top, clip_bottom, clip_left, clip_right; // clipping planes

int		NumOfVertsConsidered= 0;
int		NumOfVertsTouched= 0;

u_int16_t	GroupImIn;
/* bjd - CHECK
D3DTRANSFORMDATA Data;
*/
DWORD VertexCount = 0;
DWORD Offscreen = 0;
VERT TestVerts[MAXPORTVERTS];
VISPOLVERTEX	VisVerts[MAXPORTVERTS];
u_int16_t	CurrentGroupVisible;						// the real group thats visible
u_int16_t	GroupInVisibleList;							// where it is in the visibility list
u_int16_t	NumGroupsVisible;
u_int16_t	GroupsVisible[MAXGROUPS];
XYRECT	GroupVisibleExtents[MAXGROUPS];
u_int16_t	NumPortalsVisible;
u_int16_t	PortalsVisible[MAXPORTALSPERGROUP];
XYRECT	PortalExtents[MAXGROUPS];
MATRIX	VisPolyMatrix = {
				1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 1.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F };
u_int16_t	IsGroupVisible[MAXGROUPS];

struct
{
	u_int32_t tsum;
	u_int32_t tmax;
	u_int32_t tmin;
	u_int32_t visits;
} VisiStats[ MAXGROUPS ];


typedef struct
{
	u_int16_t group;
	struct
	{
		double x, y, z;
	} from, to;
} DebugRay;

extern int16_t LevelNum;
static void InitDebugRays( void )
{
#if 1
	char fname[ 256 ];
	FILE *rf;
	DebugRay ray;
	u_int16_t line;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], fname, ".ray" );
	rf = file_open( fname, "rb" );
	if ( !rf )
		return;
	while ( fread( &ray.group, sizeof( ray.group ), 1, rf ) )
	{
		if ( !fread( &ray.from, sizeof( double ), 3, rf ) )
			break;
		if ( !fread( &ray.to, sizeof( double ), 3, rf ) )
			break;
		line = FindFreeLine();
		if ( line != (u_int16_t) -1 )
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
	LVLGROUP *g;
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


bool OutputVisiStats( MLOADHEADER *m, char *lname )
{
	FILE *f;
	char fname[ 256 ];
	int j, k;

	if ( strlen(lname) == 0 )
	{
		DebugPrintf("Passed empty level name to OutputVisiStats!\n");
		return false;
	}

	Change_Ext( lname, fname, ".vis" );

	f = file_open( fname, "w" );
	if ( !f )
		return false;

	fprintf( f, "Group     TMAX      TAVG      TMIN      Visits    Name\n" );
	for ( j = 0; j < m->num_groups; j++ )
	{
		fprintf( f, "%s%3d %9d %9lu %9lu %9d      %s\n",
			( VisiStats[ j ].tmax > 1000 ) ? "*" : " ",
			j,
			(int)VisiStats[ j ].tmax,
			(int)( VisiStats[ j ].visits ) ? VisiStats[ j ].tsum / VisiStats[ j ].visits : 0,
			(int)( VisiStats[ j ].visits ) ? VisiStats[ j ].tmin : 0,
			(int)VisiStats[ j ].visits,
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

	return true;
}


static void RelateGroups( GROUPRELATION *rel, u_int16_t g1, u_int16_t g2 )
{
	u_int32_t *t, mask;

	t = rel->table + GROUP2GROUP_OFFSET( g1, g2 );
	mask = GROUP2GROUP_MASK( g1, g2 );
	if ( !( *t & mask ) )
	{
		*t |= mask;
		rel->list[ g1 ].groups++;
	}
}


static bool AreGroupsRelated( GROUPRELATION *rel, u_int16_t g1, u_int16_t g2 )
{
	u_int32_t *t, mask;

	t = rel->table + GROUP2GROUP_OFFSET( g1, g2 );
	mask = GROUP2GROUP_MASK( g1, g2 );
	if ( *t & mask )
		return true;
	else
		return false;
}


static void FindGroupsVisible( GROUPRELATION *vis, u_int16_t from_group, VISTREE *visible )
{
	int j;

	RelateGroups( vis, from_group, visible->group );
	for ( j = 0; j < visible->num_visible; j++ )
	{
		FindGroupsVisible( vis, from_group, &visible->visible[ j ] );
	}
}


bool GroupsAreConnected( u_int16_t g1, u_int16_t g2 )
{
	return AreGroupsRelated( &ConnectedGroup, g1, g2 );
}


bool GroupsAreVisible( u_int16_t g1, u_int16_t g2 )
{
	return AreGroupsRelated( &VisibleGroup, g1, g2 );
}


bool GroupsAreIndirectVisible( u_int16_t g1, u_int16_t g2 )
{
	if ( IndirectVisibleGroup.table )
		return AreGroupsRelated( &IndirectVisibleGroup, g1, g2 );
	else
	{
		static int failed = 0;

		if ( !failed++ )
			Msg( "No IndirectVisible table loaded\n" );
		return false;
	}
}


bool ReadGroupConnections( MLOADHEADER *m, char **pbuf )
{
	u_int32_t tabsize;
	u_int16_t g;
	char *buf;
	u_int16_t *buf16;

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

	InitConnections = true;

	// allocate tables
	GTabRowSize = DIV_CEIL( MAXGROUPS, 32 );
	tabsize = m->num_groups * GTabRowSize * sizeof( u_int32_t );
	if ( !ConnectedGroup.table )
	{
		ConnectedGroup.table = (u_int32_t *) malloc( tabsize );
		if ( !ConnectedGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for ConnectedGroup.table\n" );
			return false;
		}
	}
	if ( !VisibleGroup.table )
	{
		VisibleGroup.table = (u_int32_t *) malloc( tabsize );
		if ( !VisibleGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for VisibleGroup.table\n" );
			return false;
		}
	}
	if ( !IndirectVisibleGroup.table )
	{
		IndirectVisibleGroup.table = (u_int32_t *) malloc( tabsize );
		if ( !IndirectVisibleGroup.table )
		{
			Msg( "ReadGroupConnections: failed malloc for IndirectVisibleGroup.table\n" );
			return false;
		}
	}
	memset( ConnectedGroup.table, 0, tabsize );
	memset( VisibleGroup.table, 0, tabsize );
	memset( IndirectVisibleGroup.table, 0, tabsize );

	buf = *pbuf;

	// read connectivity table
	memmove( ConnectedGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (u_int16_t *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		ConnectedGroup.list[ g ].groups = *buf16++;
		if ( ConnectedGroup.list[ g ].groups )
		{
			ConnectedGroup.list[ g ].group = (u_int16_t *) calloc( ConnectedGroup.list[ g ].groups, sizeof( u_int16_t ) );
			if ( !ConnectedGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for ConnectedGroup.list[ %d ]\n", g );
				return false;
			}
			memmove( ConnectedGroup.list[ g ].group, buf16, ConnectedGroup.list[ g ].groups * sizeof( u_int16_t ) );
			buf16 += ConnectedGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	// read visibility summary table
	memmove( VisibleGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (u_int16_t *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		VisibleGroup.list[ g ].groups = *buf16++;
		if ( VisibleGroup.list[ g ].groups )
		{
			VisibleGroup.list[ g ].group = (u_int16_t *) calloc( VisibleGroup.list[ g ].groups, sizeof( u_int16_t ) );
			if ( !VisibleGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for VisibleGroup.list[ %d ]\n", g );
				return false;
			}
			memmove( VisibleGroup.list[ g ].group, buf16, VisibleGroup.list[ g ].groups * sizeof( u_int16_t ) );
			buf16 += VisibleGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	// read indirect visibility summary table
	memmove( IndirectVisibleGroup.table, buf, tabsize );
	buf += tabsize;

	buf16 = (u_int16_t *) buf;
	for ( g = 0; g < m->num_groups; g++ )
	{
		IndirectVisibleGroup.list[ g ].groups = *buf16++;
		if ( IndirectVisibleGroup.list[ g ].groups )
		{
			IndirectVisibleGroup.list[ g ].group = (u_int16_t *) calloc( IndirectVisibleGroup.list[ g ].groups, sizeof( u_int16_t ) );
			if ( !IndirectVisibleGroup.list[ g ].group )
			{
				Msg( "ReadGroupConnections: failed X_calloc for IndirectVisibleGroup.list[ %d ]\n", g );
				return false;
			}
			memmove( IndirectVisibleGroup.list[ g ].group, buf16, IndirectVisibleGroup.list[ g ].groups * sizeof( u_int16_t ) );
			buf16 += IndirectVisibleGroup.list[ g ].groups;
		}
	}
	buf = (char *) buf16;

	*pbuf = buf;

	return true;
}



bool FindGroupConnections( MLOADHEADER *m )
{
	u_int32_t tabsize;
	u_int16_t g, p, g2;
	LVLGROUP *group;
	PORTAL *portal;
	u_int16_t gnum;

	for ( g = 0; g < MAXGROUPS; g++ )
	{
		ConnectedGroup.list[ g ].groups = 0;
		ConnectedGroup.list[ g ].group = NULL;
		VisibleGroup.list[ g ].groups = 0;
		VisibleGroup.list[ g ].group = NULL;
		IndirectVisibleGroup.list[ g ].groups = 0;
		IndirectVisibleGroup.list[ g ].group = NULL;
	}

	InitConnections = true;

	GTabRowSize = (int)ceil(MAXGROUPS/32.0F);
	tabsize = MAXGROUPS * GTabRowSize * sizeof( u_int32_t );
	if ( !ConnectedGroup.table )
	{
		ConnectedGroup.table = (u_int32_t *) malloc( tabsize );
		if ( !ConnectedGroup.table )
		{
			Msg( "FindGroupConnections: failed malloc for ConnectedGroup.table\n" );
			return false;
		}
	}
	if ( !VisibleGroup.table )
	{
		VisibleGroup.table = (u_int32_t *) malloc( tabsize );
		if ( !VisibleGroup.table )
		{
			Msg( "FindGroupConnections: failed malloc for VisibleGroup.table\n" );
			return false;
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
			ConnectedGroup.list[ g ].group = (u_int16_t *) calloc( ConnectedGroup.list[ g ].groups, sizeof( u_int16_t ) );
			if ( !ConnectedGroup.list[ g ].group )
			{
				Msg( "FindGroupConnections: failed X_calloc for ConnectedGroup.list[ %d ]\n", g );
				return false;
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
			VisibleGroup.list[ g ].group = (u_int16_t *) calloc( VisibleGroup.list[ g ].groups, sizeof( u_int16_t ) );
			if ( !VisibleGroup.list[ g ].group )
			{
				Msg( "FindGroupConnections: failed X_calloc for VisibleGroup.list[ %d ]\n", g );
				return false;
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

	return true;
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




GROUPLIST *ConnectedGroups( u_int16_t g )
{
	return &ConnectedGroup.list[ g ];
}




GROUPLIST *VisibleGroups( u_int16_t g )
{
	if( g >= Mloadheader.num_groups )
		Msg( "VisibleGroups() Group %d out of range", g );

	if( VisibleGroup.list[ g ].groups > Mloadheader.num_groups )
		Msg( "VisibleGroups.list[%d].groups=%d more than %d groups", g, VisibleGroup.list[ g ].groups, Mloadheader.num_groups );

	if( !VisibleGroup.list[ g ].group )
		Msg( "VisibleGroups.list[%d].group Invalid", g );

	return &VisibleGroup.list[ g ];
}


GROUPLIST *IndirectVisibleGroups( u_int16_t g )
{
	return &IndirectVisibleGroup.list[ g ];
}



int VisibleOverlap( u_int16_t g1, u_int16_t g2, u_int16_t *overlapping_group )
{
	u_int32_t *t1, *t2, overlap;
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


void InitIndirectVisible( u_int16_t g )
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


void AddIndirectVisible( u_int16_t g )
{
	static u_int16_t AddedVisible[ MAXGROUPS ];
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
__inline
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

static int
Transform2Viewport( CAMERA *cam, VISLIST *v, VERT *wpos, VECTOR *vpos )
{
	int clip;

    VisPolyApplyMatrix( (MATRIX *) &v->viewproj, (VECTOR *) wpos, vpos );
	clip = 0;
	if ( vpos->z > 1.0F )
	{
		clip |= CLIP_FRONT;
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
			clip |= CLIP_LEFT;
		}
		else if ( DotProduct( vpos, &clip_right ) < 0.0F )
		{
			clip |= CLIP_RIGHT;
		}
		if ( DotProduct( vpos, &clip_top ) < 0.0F )
		{
			clip |= CLIP_TOP;
		}
		else if ( DotProduct( vpos, &clip_bottom ) < 0.0F )
		{
			clip |= CLIP_BOTTOM;
		}
		return clip;
#endif
	}

	if ( vpos->x < -1.0F )
	{
		clip |= CLIP_LEFT;
		vpos->x = (float) v->viewport->X;
	}
	else if ( vpos->x > 1.0F )
	{
		clip |= CLIP_RIGHT;
		vpos->x = (float) ( v->viewport->X + v->viewport->Width );
	}
	else
	{
		vpos->x = v->viewport->X + ( v->viewport->Width * 0.5F ) + (v->viewport->ScaleX * vpos->x);
	}

   	if ( vpos->y < -1.0F )
	{
		clip |= CLIP_BOTTOM;
		vpos->y = (float) ( v->viewport->Y + v->viewport->Height );
	}
	else if ( vpos->y > 1.0F )
	{
		clip |= CLIP_TOP;
		vpos->y = (float) v->viewport->Y;
	}
	else
	{
		vpos->y = v->viewport->Y + ( v->viewport->Height * 0.5F ) - (v->viewport->ScaleY * vpos->y);
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
	clip_all = CLIP_LEFT | CLIP_RIGHT | CLIP_TOP | CLIP_BOTTOM | CLIP_FRONT;
	for ( vnum = 0; vnum < p->num_vertices_in_portal; vnum++ )
	{
		clip = Transform2Viewport( cam, v, &p->Verts[ vnum ], &pos );
		clip_any |= clip;
		clip_all &= clip;
		if ( !( clip & CLIP_FRONT ) )
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
	if ( clip_any & CLIP_FRONT )
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

void FindVisible( CAMERA *cam, MLOADHEADER *Mloadheader )
{
	VISLIST *v;
	VISGROUP *g;
	XYRECT clip;
	render_viewport_t *vp;
	VISGROUP *gsort, *gprev, *gnext;
	int j;
	float w, h;

	// helper variable used for stereo adjustment
	float lr;

	// calculate clipping planes
	w = (float) tan( hfov );
	h = w / render_info.aspect_ratio;
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
	g->extent.min.x = (float) v->viewport->X;
	g->extent.min.y = (float) v->viewport->Y;
	g->extent.min.z = -HUGE_VALUE;
	g->extent.max.x = (float) ( v->viewport->X + v->viewport->Width );
	g->extent.max.y = (float) ( v->viewport->Y + v->viewport->Height );
	g->extent.max.z = HUGE_VALUE;

	// process visible portals
	if ( outside_map )
	{
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
		for ( j = 0; j < Mloadheader->Group[ cam->GroupImIn].num_portals; j++ )
		{
			ProcessVisiblePortal( cam, v, &Mloadheader->Group[ cam->GroupImIn ].Portal [ j ].visible, &g->extent );
		}
	}

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
		vp->X = clip.x1;
		vp->Y = clip.y1;
		vp->Width = clip.x2 - clip.x1;
		vp->Height = clip.y2 - clip.y1;
		vp->Width += vp->Width & 1;
		vp->Height += vp->Height & 1;
		if ( vp->X + vp->Width > v->viewport->X + v->viewport->Width )
		{
			if ( vp->X > 0 )
				vp->X--;
			else
				vp->Width--;
		}
		if ( vp->Y + vp->Height > v->viewport->Y + v->viewport->Height )
		{
			if ( vp->Y > 0 )
				vp->Y--;
			else
				vp->Height--;
		}
		vp->ScaleX = vp->Width * 0.5F;
		vp->ScaleY = vp->Height * 0.5F;
		g->projection = cam->Proj;
		g->projection._11 = ( cam->Proj._11 * v->viewport->Width ) / vp->Width;
		g->projection._22 = ( cam->Proj._22 * v->viewport->Height ) / vp->Height;
		lr = 2.0F * ( ( v->viewport->X + v->viewport->Width * 0.5F ) - ( vp->X + vp->Width * 0.5F ) );

		switch( render_info.stereo_position )
		{
		case ST_LEFT:
			lr -= 0.5f * render_info.stereo_focal_dist / render_info.stereo_eye_sep;
			break;
		case ST_RIGHT:
			lr += 0.5f * render_info.stereo_focal_dist / render_info.stereo_eye_sep;
		}

		g->projection._31 = lr / vp->Width;
		g->projection._32 = -2.0F * ( ( v->viewport->Y + v->viewport->Height * 0.5F )
									- ( vp->Y + vp->Height * 0.5F ) )
							/ vp->Height;
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

int ClipGroup( CAMERA *cam, u_int16_t group )
{
	VISGROUP *g;

	g = &cam->visible.group[ group ];
	if ( !g->visible )
		return 0;

	if( !DoClipping )
		g = &cam->visible.group[ cam->visible.first_visible->group ];

	if (!FSSetProjection(&g->projection))
		return false;
	if (!FSSetView(&cam->View))
		return false;

    if (!FSSetViewPort(&g->viewport)) {
#ifdef DEBUG_VIEWPORT
		SetViewportError( "ClipGroup", &g->viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return false;
    }
	
	return 1;
}

/*===================================================================
		Disp Visipoly Model
===================================================================*/
extern	float	WhiteOut;
extern	u_int16_t	GroupWaterInfo[MAXGROUPS];
extern	float	GroupWaterLevel[MAXGROUPS];
extern	float	GroupWaterIntensity_Red[MAXGROUPS];
extern	float	GroupWaterIntensity_Green[MAXGROUPS];
extern	float	GroupWaterIntensity_Blue[MAXGROUPS];
bool
DisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam ) 
{
	RENDERMATRIX	Tempproj;
	RENDERMATRIX	Tempview;
	VISGROUP *g;
	int	i, group;
	u_int32_t t;
	render_viewport_t OldViewPort;

	Tempproj = proj;
	Tempview = view;

	NumOfVertsConsidered= 0;
	NumOfVertsTouched= 0;

	FSGetViewPort(&OldViewPort);

	GroupImIn = CurrentCamera.GroupImIn;

	if ( GroupImIn != (u_int16_t) -1 )
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
		 	ClipGroup( &CurrentCamera, (u_int16_t) g->group );
		  CurrentGroupVisible = GroupsVisible[i];
			GroupInVisibleList = i;
			group = GroupsVisible[i];

			if ( XLight1Group(  Mloadheader, GroupsVisible[i] ) != true  )
				return false;

 			if ( ExecuteSingleGroupMloadHeader(  Mloadheader, (u_int16_t) g->group ) != true  )
				return false;

#ifdef NEW_LIGHTING
			render_reset_lighting_variables();
#endif

			DispGroupTriggerAreas( (u_int16_t) g->group );
			if ( CaptureTheFlag || CTF )
				DisplayGoal( (u_int16_t) g->group );
//			ShowAllColZones( (u_int16_t) g->group );

			t += GroupTris[ g->group ];
		}
		// accumulate visibility stats
		VisiStats[ GroupImIn ].tsum += t;
		if ( VisiStats[ GroupImIn ].tmax < t )
			VisiStats[ GroupImIn ].tmax = t;
		if ( VisiStats[ GroupImIn ].tmin > t )
			VisiStats[ GroupImIn ].tmin = t;
		VisiStats[ GroupImIn ].visits++;
	}
	else
	{
		if ( ExecuteMloadHeader ( Mloadheader ) != true)
			return false;
	}

    if (!FSSetViewPort(&OldViewPort)) 
	{
#ifdef DEBUG_VIEWPORT
		SetViewportError( "ClipGroup", &g->viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return false;
    }

	proj = Tempproj;
	view = Tempview;

	if (!FSSetProjection(&proj))
		return false;
	if (!FSSetView(&view))
		return false;
	
	return true;
}


u_int16_t
FindClipGroup( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max )
{
	u_int16_t group;
	VISGROUP *vg;
	LVLGROUP *mg;
	u_int16_t in_groups;
	EXTENT extent, *e=NULL; ZERO_STACK_MEM(extent);
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
		group = (u_int16_t) -1;

	return group;
}


u_int16_t FindOverlappingVisibleGroups( CAMERA *cam, MLOADHEADER *m, VECTOR *min, VECTOR *max, u_int16_t * group )
{
	VISGROUP *vg;
	LVLGROUP *mg;
	u_int16_t in_groups;

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

/*===================================================================
	Procedure	:	Is point inside bounding box of group
	Input		:	MLOADHEADER	*	Mloadheader
				:	VECTOR		*	Pos
				:	u_int16_t			Group
	Output		:	false/true
===================================================================*/
bool PointInGroupBoundingBox( MLOADHEADER * Mloadheader, VECTOR * Pos, u_int16_t group )
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
		return true;
	}
	return false;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
