									    
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"

#include "d3dmain.h"
#include "d3ddemo.h"
#include "exechand.h"
#include "d3dappi.h"
#include "mload.h"

#include "camera.h"

#include "PolySort.h"
#include "XMem.h"





/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	CAMERA	CurrentCamera;
extern	TLOADHEADER	Tloadheader;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BUCKETENTRY * Bucket = NULL;

POLYSORTPRIM * PolySortPrims = NULL;
MATRIX	PolySortMatrix;

int	CurrentPolySortPrim = 0;

D3DEXECUTEBUFFERDESC debDesc;
LPDIRECT3DEXECUTEBUFFER PolySortExec = NULL;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Function	:	Apply Matrix to a Vector
	Input		:	MATRIX	*	Matrix
				:	VECTOR	*	Vert
				:	VECTOR	*	New Vert
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float FindZ( MATRIX * m, VECTOR * v0 )
{
	float w;
	float z;
	z = ( ( m->_13 * v0->x ) + ( m->_23 * v0->y ) + ( m->_33 * v0->z ) +m->_43);
	w = ( ( m->_14 * v0->x ) + ( m->_24 * v0->y ) + ( m->_34 * v0->z ) +m->_44);
	z /= w;
	return z;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Init The PolySort..
	Input		:		NOTHING
	Output		:		BOOL FALSE/TRUE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define PolySortNear	(25.0F)
#define PolySortFar		(4096.0F)

BOOL	InitPolySort( void )
{
	D3DMATRIX	TempMat;
	TempMat = CurrentCamera.Proj;

	TempMat._33 = D3DVAL(PolySortFar/(PolySortFar-PolySortNear));
	TempMat._34 = D3DVAL(-PolySortFar*PolySortNear/(PolySortFar-PolySortNear));

	return TRUE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Release The PolySort..
	Input		:		NOTHING
	Output		:		NOTHING
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	ReleasePolySort( void )
{

	if( Bucket )
	{
		free( Bucket );
		Bucket = NULL;
	}
	if( PolySortPrims )
	{
		free( PolySortPrims);
		PolySortPrims = NULL;
	}
	if( PolySortExec )
	{
		XRELEASE(PolySortExec );
		PolySortExec = NULL;
	}

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add a poly to the bucket...
	Input		:		NOTHING
	Output		:		NOTHING
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	AddToPolySort( D3DLVERTEX * v1 , D3DLVERTEX * v2 , D3DLVERTEX * v3 , uint16 tpage )
{
	BUCKETENTRY * BucketPnt;
	POLYSORTPRIM * Prim;
	float	Zdist;
	int		Zindex;
	VECTOR	Vector;
	if( CurrentPolySortPrim >= MAXPOLYSORTPRIMS )
		return;

	Prim = &PolySortPrims[CurrentPolySortPrim];

	Prim->v[0] = *v1;
	Prim->v[1] = *v2;
	Prim->v[2] = *v3;
	Prim->tpage = tpage;

	Vector.x = v1->x;
	Vector.y = v1->y;
	Vector.z = v1->z;

//	Zdist = FindZ( &PolySortMatrix, &Vector );
	Zdist =	DistanceVector2Vector(&Vector , &CurrentCamera.Pos);
	if( Zdist < 0.0F)
		Zdist *= -1.0F;
	if( Zdist > 4096.0F )
		Zdist = 4096.0F;

	Zdist = (1.0F / 4096.0F ) * Zdist;
	
//	if( Zdist < 0.0F )
//		Zdist *= -1.0F;
//	if( Zdist > 1.0F )
//		Zdist = 1.0F;
	Zindex = (int) ( (BUCKETDEPTH-1) * Zdist );

	Zindex = BUCKETDEPTH-1 - Zindex;
	BucketPnt = &Bucket[Zindex];
	Prim->Next = BucketPnt->Next;
	BucketPnt->Next = Prim;
	CurrentPolySortPrim++;
}


#define POLYLISTTEXCHANGESIZE ( (4 * sizeof(D3DINSTRUCTION) ) + sizeof(D3DSTATE) + sizeof(D3DPROCESSVERTICES) + sizeof(D3DSTATE) )









