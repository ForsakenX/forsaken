
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

void FindNearestNode( OBJECT * Object );
void UpdateNearestNode( OBJECT * Object );
void UpdateNearestNodeSpecial( OBJECT * Object );
NODE * WhichNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );
NODE * WhichRetreatNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );

NODE * ChooseAlternateNode( uint32 Network , NODE * NodeFrom , NODE * NodeTo );

NODE * FindSuitableSplineNode( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
NODE * FindSuitableSplineNodeRandom( uint32 Network, NODE * NodeFrom , NODE *Node1 , NODE *Node2 , NODE *Node3 , NODE *Node4 );
float ReturnClosestNode( VECTOR * Pos, VECTOR * NearestNodePos, uint16 * Group, VECTOR * TopLeft, VECTOR * BottomRight );

#endif	// NODE_INCLUDED
