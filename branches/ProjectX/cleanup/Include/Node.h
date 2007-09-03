/*
 * The X Men, June 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 17 $
 *
 * $Header: /PcProjectX/Include/Node.h 17    5/03/98 12:06 Collinsd $
 *
 * $Log: /PcProjectX/Include/Node.h $
 * 
 * 17    5/03/98 12:06 Collinsd
 * Fixed pickup crushing generating withing crushing object bounds.
 * 
 * 16    19/01/98 9:09 Collinsd
 * Crushed pickups now regernerate.
 * 
 * 15    12/31/97 9:20a Phillipd
 * 
 * 14    11/15/97 1:09p Phillipd
 * 
 * 13    11/12/97 11:39a Phillipd
 * 
 * 12    11/03/97 6:39p Phillipd
 * 
 * 11    10/29/97 12:25p Phillipd
 * 
 * 10    10/21/97 5:07p Phillipd
 * 
 * 9     9/15/97 11:59a Phillipd
 * 
 * 8     9/10/97 9:54a Phillipd
 * 
 * 7     8/15/97 4:33p Phillipd
 * 
 * 6     8/15/97 4:25p Phillipd
 * 
 * 5     8/15/97 4:24p Phillipd
 * 
 * 4     7/31/97 9:09a Phillipd
 * 
 * 3     7/03/97 10:11a Phillipd
 * More Ai and node
 * 
 * 2     6/30/97 10:28a Phillipd
 * 
 * 1     6/25/97 10:44a Phillipd
 * 
 */
#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#define	MAXLINKSPERNODE 16

#define	NODE_DECISION		(1<<0)
#define	NODE_DROPMINES		(1<<1)
#define	NODE_SNIPE_POSITION	(1<<2)
#define	NODE_HIDE_POSITION	(1<<3)
#define	NODE_TERMINATE		(1<<4)

typedef struct NODE
{
	BOOL	LegalGroup;
	uint16	Flags;
	int16	Group;
	VECTOR	Pos;
	VECTOR	SolidPos;
	float	Radius;
	uint16	NodeNum;		// what number in the array I am..
	uint32	NetMask;		// which networks am I in..
	int16	NumOfLinks;
struct	NODE *	NodeLink[MAXLINKSPERNODE];
struct	NODE *	NextNodeInGroup;
} NODE,*LPNODE;

typedef struct _NODENETWORKHEADER
{
	BOOL	State;
	int32	NumOfNodes;
	NODE *	FirstNode;
} NODENETWORKHEADER,*LPNODENETWORKHEADER;


void NodeRelease(void);
BOOL Nodeload( char * Filename );

void FindNearestNode( struct OBJECT * Object );
void UpdateNearestNode( struct OBJECT * Object );
void UpdateNearestNodeSpecial( struct OBJECT * Object );
NODE * WhichNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );
NODE * WhichRetreatNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );

NODE * ChooseAlternateNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );

NODE * FindSuitableSplineNode( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
NODE * FindSuitableSplineNodeRandom( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
float ReturnClosestNode( VECTOR * Pos, VECTOR * NearestNodePos, uint16 * Group, VECTOR * TopLeft, VECTOR * BottomRight );

#endif	// NODE_INCLUDED
