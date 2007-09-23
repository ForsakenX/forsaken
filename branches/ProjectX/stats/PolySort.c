									    
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
extern	BOOL	bPolySort;
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
	
	if( !bPolySort )
		return TRUE;

	if( !Bucket )
	{
		Bucket = ( BUCKETENTRY * ) malloc( BUCKETDEPTH * sizeof(BUCKETENTRY) );
	}
	if( !PolySortPrims )
	{
		PolySortPrims = ( POLYSORTPRIM * ) malloc( MAXPOLYSORTPRIMS * sizeof(POLYSORTPRIM) );
	}

	CurrentPolySortPrim = 0;

	if( !PolySortExec )
	{
		if (MakeExecuteBuffer( &debDesc, d3dapp->lpD3DDevice , &PolySortExec , 32768 ) != TRUE ) return FALSE;
	}

	
	MatrixMultiply( (MATRIX *) &CurrentCamera.View, (MATRIX *) &TempMat, (MATRIX *) &PolySortMatrix );
	memset( Bucket , 0, BUCKETDEPTH * sizeof(BUCKETENTRY) );

	if( !Bucket || !PolySortPrims )
		return FALSE;

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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add a poly to the bucket...
	Input		:		NOTHING
	Output		:		BOOL TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL PolyListExecute( void )
{
	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	int				i;
	BUCKETENTRY		*BucketPnt;
	POLYSORTPRIM	*Prim;
	BOOL			Locked = FALSE;
	LPD3DLVERTEX	PolyVertPnt;
	LPD3DTRIANGLE	PolyFacePnt;
	int				TotalVertCount;
	uint16			StartVert;
	uint16			NumVerts;
    LPVOID			lpBufStart, lpInsStart, lpPointer;
	uint16			Tpage;
    char			*CurrentTpagePnt;

	if( !bPolySort )
		return TRUE;
	
	BucketPnt = &Bucket[0];

	for( i = 0 ; i < BUCKETDEPTH ; i ++ )
	{
		if( BucketPnt->Next )
		{
			Prim = BucketPnt->Next;
			
			while( Prim )
			{
				if( !Locked )
				{
					// Start of a buffer..
					TotalVertCount = 0;
					StartVert = 0;
					NumVerts = 0;
					memset( &debDesc, 0, sizeof( D3DEXECUTEBUFFERDESC ) );
					debDesc.dwSize = sizeof( D3DEXECUTEBUFFERDESC );
					
					if( PolySortExec->lpVtbl->Lock( PolySortExec, &debDesc ) != D3D_OK ) return FALSE;
					lpBufStart = debDesc.lpData;
					lpPointer = lpBufStart;

					PolyVertPnt = (LPD3DLVERTEX) lpPointer;
					CurrentTpagePnt = (char*) lpPointer;
					CurrentTpagePnt += sizeof(D3DLVERTEX) * MAXVERTSPERPOLYSORTEXEC;
					lpInsStart = (LPVOID) CurrentTpagePnt;
					PolyFacePnt = (LPD3DTRIANGLE) (CurrentTpagePnt+POLYLISTTEXCHANGESIZE);
					Tpage = Prim->tpage;
					Locked = TRUE;
				}

//				if( Tpage != Prim->tpage )
				{

				   	OP_STATE_LIGHT( 1, CurrentTpagePnt );
				   	    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Tpage ], CurrentTpagePnt);
				   	OP_PROCESS_VERTICES( 1, CurrentTpagePnt );
				   	    PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, CurrentTpagePnt);
				   	OP_STATE_RENDER( 1, CurrentTpagePnt );
				   	    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Tpage ], CurrentTpagePnt);
				   	OP_TRIANGLE_LIST( ( NumVerts / 3 ), CurrentTpagePnt );
					StartVert = TotalVertCount;
					NumVerts = 0;

					if( TotalVertCount < MAXVERTSPERPOLYSORTEXEC )
					{
						CurrentTpagePnt = (LPVOID) PolyFacePnt;
						PolyFacePnt = (LPD3DTRIANGLE) (CurrentTpagePnt+POLYLISTTEXCHANGESIZE);
						Tpage = Prim->tpage;
					}else{
						Locked = FALSE;

						lpPointer =  (LPVOID) PolyFacePnt;
						OP_EXIT( lpPointer );

						memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
						ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
						ExecBuffer_d3dexdata.dwVertexCount = TotalVertCount;
						ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
						ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
						if( ( PolySortExec->lpVtbl->SetExecuteData( PolySortExec, &ExecBuffer_d3dexdata ) ) != D3D_OK)
							return FALSE;

						PolySortExec->lpVtbl->Unlock( PolySortExec );

						if( d3dapp->lpD3DDevice->lpVtbl->Execute(d3dapp->lpD3DDevice, PolySortExec, d3dapp->lpD3DViewport , D3DEXECUTE_CLIPPED) !=D3D_OK )
							return FALSE;
					}
				}		

				if( Locked )
				{
					*PolyVertPnt++ = Prim->v[0];
					*PolyVertPnt++ = Prim->v[1];
					*PolyVertPnt++ = Prim->v[2];

					PolyFacePnt->v1 = TotalVertCount++;
					PolyFacePnt->v2 = TotalVertCount++;
					PolyFacePnt->v3 = TotalVertCount++;
					PolyFacePnt->wFlags= D3DTRIFLAG_EDGEENABLETRIANGLE;

					PolyFacePnt++;
					NumVerts+=3;
				}

				Prim = Prim->Next;
			}
		}
		BucketPnt++;
	}
	if( Locked )
	{
		// Finish off the last execbuffer....
		OP_STATE_LIGHT( 1, CurrentTpagePnt );
		    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Tpage ], CurrentTpagePnt);
		OP_PROCESS_VERTICES( 1, CurrentTpagePnt );
		    PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, CurrentTpagePnt);
		OP_STATE_RENDER( 1, CurrentTpagePnt );
		    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Tpage ], CurrentTpagePnt);
		OP_TRIANGLE_LIST( ( NumVerts / 3 ), CurrentTpagePnt );
		lpPointer =  (LPVOID) PolyFacePnt;
		OP_EXIT( lpPointer );

		memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
		ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
		ExecBuffer_d3dexdata.dwVertexCount = TotalVertCount;
		ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
		ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
		if( ( PolySortExec->lpVtbl->SetExecuteData( PolySortExec, &ExecBuffer_d3dexdata ) ) != D3D_OK)
			return FALSE;
		PolySortExec->lpVtbl->Unlock( PolySortExec );


		if( d3dapp->lpD3DDevice->lpVtbl->Execute(d3dapp->lpD3DDevice, PolySortExec, d3dapp->lpD3DViewport , D3DEXECUTE_CLIPPED) !=D3D_OK )
			return FALSE;
	}
	return TRUE;
}








