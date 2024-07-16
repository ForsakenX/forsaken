#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include "main.h"
#include "object.h"

#define	MAXLINKSPERNODE 16

#define	NODE_DECISION		(1<<0)
#define	NODE_DROPMINES		(1<<1)
#define	NODE_SNIPE_POSITION	(1<<2)
#define	NODE_HIDE_POSITION	(1<<3)
#define	NODE_TERMINATE		(1<<4)

typedef struct NODE
{
	bool	LegalGroup;
	u_int16_t	Flags;
	int16_t	Group;
	VECTOR	Pos;
	VECTOR	SolidPos;
	float	Radius;
	u_int16_t	NodeNum;		// what number in the array I am..
	u_int32_t	NetMask;		// which networks am I in..
	int16_t	NumOfLinks;
struct	NODE *	NodeLink[MAXLINKSPERNODE];
struct	NODE *	NextNodeInGroup;
} NODE,*LPNODE;

typedef struct _NODENETWORKHEADER
{
	bool	State;
	int32_t	NumOfNodes;
	NODE *	FirstNode;
} NODENETWORKHEADER,*LPNODENETWORKHEADER;


void NodeRelease(void);
bool Nodeload( char * Filename );

void FindNearestNode( OBJECT * Object );
void UpdateNearestNode( OBJECT * Object );
void UpdateNearestNodeSpecial( OBJECT * Object );
NODE * WhichNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo );
NODE * WhichRetreatNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo );

NODE * ChooseAlternateNode( u_int32_t Network , NODE * NodeFrom , NODE * NodeTo );

NODE * FindSuitableSplineNode( u_int32_t Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
NODE * FindSuitableSplineNodeRandom( u_int32_t Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
float ReturnClosestNode( VECTOR * Pos, VECTOR * NearestNodePos, u_int16_t * Group, VECTOR * TopLeft, VECTOR * BottomRight );

#endif	// NODE_INCLUDED
