
#ifndef BSP_INCLUDED
#define BSP_INCLUDED

#include "main.h"

/*
 * Defines
 */
#define TOLER (0.03125)	//0.0000076F 
#define INSIDE 1
#define OUTSIDE -1
#define ONPLANE 0
 

#define MAX_BSP_MODELS	(2)
/*
 * Structures
 */
 typedef struct BSP_RAWNODE
 {
	VECTOR	Normal;
	float	Offset;
	int		Front;
	int		Back;
	int		Colour;
 }BSP_RAWNODE;

typedef struct BSP_NODE
{
    VECTOR	Normal;
    float	Offset;
	int		Colour;
struct	BSP_NODE * Parent;
struct	BSP_NODE * Front;
struct	BSP_NODE * Back;
}BSP_NODE;
 
typedef struct BSP_TREE
{
   int	NumNodes;
   BSP_NODE * Root;
}BSP_TREE;

typedef struct BSP_HEADER
{
	bool	State;
    int NumGroups;
    BSP_TREE Bsp_Tree[MAXGROUPS];
}BSP_HEADER;


typedef struct _BSP_PORTAL
{
	u_int16_t		group;
	VECTOR		normal;
	float		offset;
	BSP_TREE	bsp;
} BSP_PORTAL;

typedef struct _BSP_PORTAL_GROUP
{
	int			portals;
	BSP_PORTAL	*portal;
} BSP_PORTAL_GROUP;


typedef struct _BSP_PORTAL_HEADER
{
	bool		state;
	int			groups;
	BSP_PORTAL_GROUP	group[ MAXGROUPS ];
} BSP_PORTAL_HEADER;


/*
 * global vars
 */
extern	BSP_HEADER Bsp_Header[];
extern	BSP_PORTAL_HEADER Bsp_Portal_Header;


 /*
 * fn prototypes
 */
void Bspfree( void );
bool Bspload( char * Filename,  BSP_HEADER *Bsp_Header );
bool InBSPGroup( u_int16_t group, VECTOR *pos );
bool RayCollide( BSP_HEADER *Bsp_Header, VECTOR *StartPos, VECTOR *Dir, VECTOR *ImpactPoint, VECTOR *ImpactNormal, float *ImpactOffset ,u_int16_t group );

bool PointInSpaceRecursive( VECTOR *Pos );

bool PointInsideSkin( VECTOR *Pos, u_int16_t Group );
#endif	// BSP_INCLUDED


