
/*===================================================================
		Include File...	
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "triggers.h"
#include "d3dmain.h"
#include <d3d9.h>
#include "d3dapp.h"
#include "d3dmacs.h"
#include "typedefs.h"
#include "file.h"
#include "d3dappi.h"
#include "new3d.h"
#include "Tload.h"
#include "Mload.h"
#include "Transexe.h"
#include "water.h"
#include "magic.h"
#include "XMem.h"
#include "util.h"

/*===================================================================
		Externs...	
===================================================================*/
extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern	D3DMATRIXHANDLE hWorld;
extern	BOOL	UsedStippledAlpha;
extern	float	framelag;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	BOOL	CanCullFlag;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	D3DMATRIXHANDLE hView;
extern	D3DMATRIX view;
extern	D3DMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	D3DMATRIXHANDLE hWorld;
extern	D3DMATRIX  TempWorld;	
extern	D3DMATRIX identity;
extern	BOOL	CanCullFlag;
extern	uint32				AnimOncePerFrame;					// used for stuff that is displayed more than once in a single frame..
extern TRIGGERMOD	*	TrigMods;

/*===================================================================
		Defines
===================================================================*/
#define	WAT_VERSION_NUMBER	1

/*===================================================================
		Globals...	
===================================================================*/
float WATER_CELLSIZE = 64.0F;

uint16	NumOfWaterObjects = 0;
uint16	WaterTPage = 0;
WATEROBJECT	* FirstWaterObject = NULL;
char	* WaterBuffer = NULL;
char	* OrgWaterBuffer = NULL;
uint16	GroupWaterInfo[MAXGROUPS];

float	GroupDamageInfo[MAXGROUPS];

float	GroupWaterLevel[MAXGROUPS];
float	GroupWaterIntensity_Red[MAXGROUPS];
float	GroupWaterIntensity_Green[MAXGROUPS];
float	GroupWaterIntensity_Blue[MAXGROUPS];

extern	float framelag;
float	Fc[MAXMESHX*MAXMESHY];

void UpdateWaterMesh( WATEROBJECT * WO );
void DisplayWaterObject(WATEROBJECT * WO);
BOOL InitWaterObject(WATEROBJECT * WO);

VECTOR	WaterNormal = { 0.0F, 1.0F, 0.0F };

WATEROBJECT	* WaterObjectLink[MAXGROUPS];

float WaterFade = 1.0F;
/*===================================================================
	Procedure	:	Pre Water Load..
	Input		:	char * filename....
	Output		:	BOOL
===================================================================*/
BOOL PreWaterLoad( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	uint16 * Uint16Pnt;
	uint32 * Uint32Pnt;
	char	*	FileNamePnt;
	char buf[256];
	int i;
	uint32	MagicNumber;
	uint32	VersionNumber;
	int8		TempFilename[ 256 ];

	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		GroupDamageInfo[i] = 0.0F;
		GroupWaterInfo[i] = WATERSTATE_NOWATER;
		WaterObjectLink[i] = NULL;
	}

	NumOfWaterObjects = 0;
	FirstWaterObject = NULL;
	WaterBuffer = NULL;

	File_Size = Get_File_Size( Filename );

	if( !File_Size )
	{
		// dont need water...
		return TRUE;
	}
	Buffer = malloc( File_Size );
	OrgWaterBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Pre Water Load : Unable to allocate file buffer\n", Filename );
		return( FALSE );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "Pre Water Load Error reading %s\n", Filename );
		return( FALSE );
	}

	Uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != WAT_VERSION_NUMBER  ) )
	{
		Msg( "PreWaterLoad() Incompatible water( .WAT ) file %s", Filename );
		return( FALSE );
	}

	Uint16Pnt = (uint16 *) Buffer;

	NumOfWaterObjects = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		
	// get the Tpage sorted out...
	FileNamePnt = (char *) &buf[0];
	while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );

	if( !_stricmp( "default.ppm", (char *) &buf[0] ) ) sprintf( &TempFilename[ 0 ], "data\\textures\\%s", (char *) &buf[0] );
	else sprintf( &TempFilename[ 0 ], "data\\levels\\%s\\textures\\%s", &ShortLevelNames[ LevelNum ][ 0 ], (char *) &buf[0] );

	WaterTPage = AddTexture( &Tloadheader , &TempFilename[ 0 ], TRUE , TRUE , TRUE, 0, 0 );		// dont colourkey
	WaterBuffer = Buffer;
	return TRUE;
}
/*===================================================================
	Procedure	:	Water Load..
	Input		:	char * filename....
	Output		:	BOOL
===================================================================*/
BOOL WaterLoad( void )
{
	char		*	Buffer;
	char		*	OrgBuffer;
	int			i;
	WATEROBJECT	* WO;
	float * FloatPnt;
	uint16 * Uint16Pnt;
	int16 * int16Pnt;
	uint16 NumOfGroups = 0;
	D3DCOLOR col;
	D3DCOLOR * D3DColourPnt;

	FirstWaterObject = NULL;
	Buffer = WaterBuffer;
	OrgBuffer = OrgWaterBuffer;

	if( !Buffer )
		return TRUE;

	Uint16Pnt = (uint16 *) Buffer;
	NumOfGroups = *Uint16Pnt++;
	Buffer = (char*) Uint16Pnt;

	for( i = 0 ; i < NumOfGroups ; i++ )
	{
		Uint16Pnt = (uint16 *) Buffer;
		GroupWaterInfo[i] = *Uint16Pnt++;
		Buffer = (char*) Uint16Pnt;

		D3DColourPnt = (D3DCOLOR *) Buffer;
		col = *D3DColourPnt++;

		GroupWaterIntensity_Red[i] = RGBA_GETRED(col) * ONEOVER255;
		GroupWaterIntensity_Green[i] = RGBA_GETGREEN(col) * ONEOVER255;
		GroupWaterIntensity_Blue[i] = RGBA_GETBLUE(col) * ONEOVER255;

		Buffer = (char*) D3DColourPnt;

	}

	
	FirstWaterObject = (WATEROBJECT*) malloc( NumOfWaterObjects * sizeof(WATEROBJECT) );
	if( !FirstWaterObject )
	{
		Msg( "Water Load : Unable to allocate buffer" );
		return( FALSE );
	}
	
	
	WO = FirstWaterObject;
	for( i = 0 ; i < NumOfWaterObjects ; i++ )
	{
		WO->Status = WATERSTATUS_STATIC;
		Uint16Pnt = (uint16 *) Buffer;
		WO->Group = *Uint16Pnt++;
		FloatPnt = (float*) Uint16Pnt;

		WO->Pos.x = *FloatPnt++;
		WO->Pos.y = *FloatPnt++;
		WO->Pos.z = *FloatPnt++;
		WO->offset = 0.0F;
		GroupWaterLevel[WO->Group] = WO->Pos.y + WATER_CELLSIZE;

		WO->XSize = *FloatPnt++;
		WO->YSize = *FloatPnt++;

		WO->uTL = *FloatPnt++;	// Uv coords...	Top Left...
		WO->vTL = *FloatPnt++;	// Uv coords...	Top Left...
		WO->uBR = *FloatPnt++;	// Uv coords...	Bottom Right...
		WO->vBR = *FloatPnt++;	// Uv coords...	Bottom Right...

		WO->uRange = WO->uBR - WO->uTL;
		WO->vRange = WO->vBR - WO->vTL;

		WO->XVerts = ((int) (WO->XSize / WATER_CELLSIZE))+1;
		WO->YVerts = ((int) (WO->YSize / WATER_CELLSIZE))+1;

		if ( WO->XVerts < 1 || WO->YVerts < 1 )
		{
			DebugPrintf("Not loading water, either xVerts or yVerts was less than 1: %d, %d\n",WO->XSize,WO->YSize);
			return FALSE;
		}

		if( ( WO->XVerts * WO->YVerts ) > 1024)
		{
			Msg( "Water Load : Water is to big\n" );
			return( FALSE );
		}
		if( !InitWaterObject( WO ) )
		{
			Msg( "Water Load : Unable to init water object\n" );
			return( FALSE );

		}
		AddWaterLink(WO);
		Buffer = (char *) FloatPnt;		

		D3DColourPnt = (D3DCOLOR *) Buffer;
		col = *D3DColourPnt++;
		WO->Red = RGBA_GETRED(col) * ONEOVER255;
		WO->Green = RGBA_GETGREEN(col) * ONEOVER255;
		WO->Blue = RGBA_GETBLUE(col) * ONEOVER255;
		Buffer = (char*) D3DColourPnt;
		
		FloatPnt = (float*) Buffer;

		WO->MaxLevel = *FloatPnt++;
		WO->MinLevel = *FloatPnt++;
		WO->FillRate = *FloatPnt++ / 60.0F;
		WO->DrainRate = *FloatPnt++ / 60.0F;
		WO->Density = *FloatPnt++;
		WO->MaxWaveSize = *FloatPnt++;
		Buffer = (char *) FloatPnt;		

		Uint16Pnt = (uint16 *) Buffer;
		WO->Trigger_WhenFilled = *Uint16Pnt++;
		WO->Trigger_WhenDrained = *Uint16Pnt++;
		int16Pnt = (int16*) Uint16Pnt;
		WO->Damage = (((float) *int16Pnt++) / 60.0F);
		Buffer = (char *) int16Pnt;

		WO++;
	}
	free( OrgBuffer );
	return TRUE;
}

/*===================================================================
	Procedure	:	Water Release..
	Input		:	NOTHING
	Output		:	NOTHING
===================================================================*/
void WaterRelease( void )
{
	int			i;
	WATEROBJECT	* WO;

	WO = FirstWaterObject;
	if( !WO )
		return;
	for( i = 0 ; i < NumOfWaterObjects ; i++ )
	{
		free(WO->Verts);
		free(WO->Vels);

		FSReleaseRenderObject(&WO->renderObject);

		WO++;
	}

	if( FirstWaterObject )
	{
		free(FirstWaterObject);
		FirstWaterObject = NULL;
	}
	NumOfWaterObjects = 0;
}
/*===================================================================
	Procedure	:		Add a Water to the group link list..
	Input		:		WATEROBJECT * WO
	Output		:		Nothing
===================================================================*/
void AddWaterLink(WATEROBJECT * WO)
{
	if( !WaterObjectLink[WO->Group] )
	{
		WaterObjectLink[WO->Group] = WO;
		WO->GroupLink = NULL;
	}else{
		WO->GroupLink = WaterObjectLink[WO->Group];
		WaterObjectLink[WO->Group] = WO;
	}
}

/*===================================================================
	Procedure	:		Init Everything to do with a Water Mesh...
	Input		:		Nothing
	Output		:		BOOL TRUE/FALSE
===================================================================*/
BOOL InitWaterObject(WATEROBJECT * WO)
{
//	D3DEXECUTEDATA			d3dExData;
//	D3DEXECUTEBUFFERDESC	debDesc;
//    LPVOID lpBufStart, lpInsStart, lpPointer;
	int x,y;
	LPD3DTRIANGLE	FacePnt = NULL;
	LPD3DLVERTEX	lpD3DLVERTEX = NULL;
	WORD			*lpIndices = NULL;
	int				start_index = 0;
	int			i;
	int ntris = 0;
	int vertsCount = 0;

	WO->num_of_verts = WO->XVerts * WO->YVerts;
	WO->Verts = (float*) calloc(  WO->num_of_verts , sizeof(float) );
	WO->Vels = (float*) calloc( WO->num_of_verts , sizeof(float) );

	if (FAILED(FSCreateVertexBuffer(&WO->renderObject, WO->num_of_verts)))
	{
		return FALSE;
	}

	// pre-count the number of tris so we know how big to make the index buffer
	// this loop structure was coppied from the way the triangles are assigned further bellow
	for( x = 0 ; x < WO->XVerts-1 ; x++ )
		for( y = 0 ; y < WO->YVerts-1 ; y++ )
			ntris += 2;

	if (FAILED(FSCreateIndexBuffer(&WO->renderObject, ntris*3 ))) // 3 vertexes in a triangle
	{
		return FALSE;
	}

	if (FAILED(FSLockVertexBuffer(/*WO->lpD3DVertexBuffer*/&WO->renderObject, &lpD3DLVERTEX)))
	{
		return FALSE;
	}

	if (FAILED(FSLockIndexBuffer(&WO->renderObject, &lpIndices)))
	{
		return FALSE;
	}

	FacePnt =  (LPD3DTRIANGLE) lpIndices;

/*
	lpBufStart = debDesc.lpData;
	lpPointer = lpBufStart;
	lpD3DLVERTEX = (LPD3DLVERTEX ) lpPointer;
*/

	for( x = 0 ; x < WO->XVerts ; x++ )
	{
		for( y = 0 ; y < WO->YVerts ; y++ )
		{
			lpD3DLVERTEX->x = -( ((WO->XVerts-1) * WATER_CELLSIZE ) * 0.5F ) + (x*WATER_CELLSIZE);
			lpD3DLVERTEX->z = -( ((WO->YVerts-1) * WATER_CELLSIZE ) * 0.5F ) + (y*WATER_CELLSIZE);
			lpD3DLVERTEX->y = 0.0F;
			lpD3DLVERTEX->tu = 0.0F;
			lpD3DLVERTEX->tv = 0.0F;
			lpD3DLVERTEX->color = RGBA_MAKE(128,128,128,128);
			lpD3DLVERTEX->specular = RGBA_MAKE(128,128,128,128);
//			lpD3DLVERTEX->dwReserved = 0;
			
			lpD3DLVERTEX++;

			vertsCount++;
		}
	}
/*	
	lpPointer = (void * )  lpD3DLVERTEX;			
	lpInsStart = lpPointer;
	OP_STATE_LIGHT(1, lpPointer);
		STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[WaterTPage], lpPointer);

//	OP_STATE_RENDER(2, lpPointer);
//	    STATE_DATA( D3DRENDERSTATE_WRAPU, TRUE, lpPointer );
//	    STATE_DATA( D3DRENDERSTATE_WRAPV, TRUE, lpPointer );
		
	OP_PROCESS_VERTICES(1, lpPointer);
		PROCESSVERTICES_DATA(D3DPROCESSVERTICES_TRANSFORM, 0, WO->XVerts*WO->YVerts, lpPointer);
	OP_STATE_RENDER(1, lpPointer);
		STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[WaterTPage], lpPointer);
	if (QWORD_ALIGNED(lpPointer))
		OP_NOP(lpPointer);
	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE, lpPointer );
	}
	OP_TRIANGLE_LIST( (short) (WO->XVerts-1)*(WO->YVerts-1)*2, lpPointer);
				

	FacePnt = (LPD3DTRIANGLE ) lpPointer;
*/

	/*	copy the faces data into the execute buffer	*/

	for( x = 0 ; x < WO->XVerts-1 ; x++ )
	{
		for( y = 0 ; y < WO->YVerts-1 ; y++ )
		{
			i = x * WO->YVerts + y;
			FacePnt->v1 = i+WO->YVerts+1;
			FacePnt->v2 = i+WO->YVerts;
			FacePnt->v3 = i;
//			FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			FacePnt++;
			FacePnt->v1 = i+1;
			FacePnt->v2 = i+WO->YVerts+1;
			FacePnt->v3 = i;
//			FacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
			FacePnt++;
		}
	}

/*
	lpPointer = (LPVOID) FacePnt;
	if( CanCullFlag )
	{
		OP_STATE_RENDER( 1, lpPointer);
		    STATE_DATA( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW, lpPointer );
	}
//	OP_STATE_RENDER(2, lpPointer);
//	    STATE_DATA( D3DRENDERSTATE_WRAPU, FALSE, lpPointer );
//	    STATE_DATA( D3DRENDERSTATE_WRAPV, FALSE, lpPointer );
	OP_EXIT(lpPointer);
*/			
	/*	unlock the execute buffer	*/
//	if ( WO->lpExBuf->lpVtbl->Unlock( WO->lpExBuf ) != D3D_OK)
//		return FALSE;

	if (FAILED(FSUnlockVertexBuffer(/*WO->lpD3DVertexBuffer*/&WO->renderObject)))
	{
		return FALSE;
	}
	
	if (FAILED(FSUnlockIndexBuffer(&WO->renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

	/*	set the data for the execute buffer	*/
	WO->renderObject.numTextureGroups = 1;
	WO->renderObject.textureGroups[0].numTriangles = ntris;
	WO->renderObject.textureGroups[0].numVerts = WO->num_of_verts;
	WO->renderObject.textureGroups[0].startIndex = 0;
	WO->renderObject.textureGroups[0].startVert = 0;
	WO->renderObject.textureGroups[0].texture = Tloadheader.lpTexture[WaterTPage];
	WO->renderObject.textureGroups[0].colourkey = Tloadheader.ColourKey[WaterTPage];
	WO->renderObject.material = Tloadheader.lpMat[WaterTPage];

/*
	memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
	d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
	d3dExData.dwVertexCount = (WO->XVerts*WO->YVerts);
	d3dExData.dwInstructionOffset = (ULONG) ((char *)lpInsStart - (char *)lpBufStart);
	d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
	if ( WO->lpExBuf->lpVtbl->SetExecuteData(WO->lpExBuf, &d3dExData) != D3D_OK)
		return FALSE;
*/

	return TRUE;
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif
/*===================================================================
	Procedure	:	Process Water Based on Group..
	Input		:	uint16 group
	Output		:	NOTHING
===================================================================*/
void GroupWaterProcessDisplay( uint16 group )
{
	WATEROBJECT	* WO;
	uint16 i;
	float r, g, b;

	WO = FirstWaterObject;

	for( i = 0 ; i < NumOfWaterObjects ; i++ )
	{
		if( (WO->Group==group) && (WO->AnimOncePerFrame != AnimOncePerFrame) )
		{
			WO->AnimOncePerFrame = AnimOncePerFrame;

			// store orig water rgb
			r = WO->Red; g = WO->Green; b = WO->Blue; 
			// apply fade value ( should be 0 to 1 )
			WO->Red *= WaterFade; WO->Green *= WaterFade; WO->Blue *= WaterFade;

			UpdateWaterMesh( WO );

			// restore orig water rgb
			WO->Red = r; WO->Green = g; WO->Blue = b;
		}
		if( WO->Group==group )
		{
			// Display Water...
			DisplayWaterObject( WO );
		}
		WO++;
	}

}


/*===================================================================
	Procedure	:	Update a Water Mesh...
	Input		:	WATEROBJECT * WO
	Output		:	NOTHING
===================================================================*/
void UpdateWaterMesh( WATEROBJECT * WO )
{
	int x,y;
// 	D3DEXECUTEBUFFERDESC	debDesc;
	LPD3DLVERTEX	lpD3DLVERTEX;
//  LPVOID lpBufStart;
	int col;
	float dx, dy;
	float u, v;
	float * floatpnt;
	float	Tick,Damp;
	float temp;
	float temp2;
	float * VelsPnt;
	float * VertPnt;
	float	org;

	Damp = (float)pow( 1.0 - 0.01, framelag * framelag);
	Tick = 0.04F * framelag;


	memcpy( &Fc[0] , WO->Verts , WO->num_of_verts * sizeof(float) );

	VelsPnt = WO->Vels;
	VelsPnt += WO->YVerts + 1;
	VertPnt = WO->Verts;
	VertPnt += WO->YVerts + 1;

	for (x = 1; x< WO->XVerts-1; x++)
	{
		for (y=1; y<WO->YVerts-1; y++)
		{
			floatpnt = &Fc[0];
			floatpnt += ((x-1) * WO->YVerts ) + (y-1);
			temp2 = *floatpnt++;
			temp = *floatpnt++;
			temp2 += *floatpnt;
			floatpnt += WO->YVerts-2;

			temp += *floatpnt++;
			org = *floatpnt++;
			temp += *floatpnt;

			floatpnt += WO->YVerts-2;
			temp2 += *floatpnt++;
			temp += *floatpnt++;
			temp2 += *floatpnt;

			temp += temp2 * ROOTTWO;
			temp *= SPECIALROOTTWO;

			*VelsPnt += Tick * (temp - org);  // Accel 
			*VelsPnt *= Damp;						// Damping
			*VertPnt++ += *VelsPnt++ * framelag;
		}
		VelsPnt += 2;
		VertPnt += 2;
	}

/*
	memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
	debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/

	/*	lock the execute buffer	*/
//	if ( WO->lpExBuf->lpVtbl->Lock( WO->lpExBuf, &debDesc ) != D3D_OK)
//		return; // bjd
//	if (FSLockExecuteBuffer(WO->lpExBuf, &debDesc ) != D3D_OK)
//		return FALSE;

	if (FAILED(FSLockVertexBuffer(/*WO->lpD3DVertexBuffer*/&WO->renderObject, &lpD3DLVERTEX)))
	{
		return;
	}

/*
	lpBufStart = debDesc.lpData;
	lpD3DLVERTEX = ( LPD3DLVERTEX )lpBufStart;
*/
	VertPnt = WO->Verts;
	
	for( x = 0 ; x < WO->XVerts ; x++ )
	{
		for( y = 0 ; y < WO->YVerts ; y++ )
		{
			lpD3DLVERTEX->y = *VertPnt;
			if( x > 0 && x < WO->XVerts-1 && y > 0 && y < WO->YVerts-1 )
			{
				dx = *(VertPnt+WO->YVerts) - *(VertPnt-WO->YVerts) + 0.25F
					* ( *(VertPnt+WO->YVerts-1) + *(VertPnt+WO->YVerts+1) - *(VertPnt-WO->YVerts-1) - *(VertPnt-WO->YVerts+1) );
				dy = *(VertPnt+1) - *(VertPnt-1) + 0.25F
					* ( *(VertPnt+WO->YVerts+1) + *(VertPnt-WO->YVerts+1) - *(VertPnt+WO->YVerts-1) - *(VertPnt-WO->YVerts-1) );
			}else{
				dx = 0.0F;
				dy = 0.0F;
			}
			u = (float) ( x + dx * 0.025F) / WO->XVerts;
			v = 1.0F - (float) ( y + dy * 0.025F) / WO->YVerts;
			lpD3DLVERTEX->tu = (u * WO->uRange) + WO->uTL;
			lpD3DLVERTEX->tv = (v * WO->vRange) + WO->vTL;
			col = 192 + (int) ( dy * 8 );
			if( col > 255 )
				col = 255;
			if( col < 0 )
				col = 0;
			lpD3DLVERTEX->color = RGBA_MAKE( (int)(col * WO->Red) , (int)(col * WO->Green) , (int)(col * WO->Blue) , 128);
			lpD3DLVERTEX++;
			VertPnt++;
		}
	}
	/*	unlock the execute buffer	*/
//	if ( WO->lpExBuf->lpVtbl->Unlock( WO->lpExBuf ) != D3D_OK)
//		return;
	if (FAILED(FSUnlockVertexBuffer(/*WO->lpD3DVertexBuffer*/&WO->renderObject)))
	{
		return;
	}
}

/*===================================================================
	Procedure	:		Display the Water Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void DisplayWaterObject(WATEROBJECT * Wo)
{										 
//	LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
	TempWorld = identity;
	TempWorld._41 = Wo->Pos.x;
	TempWorld._42 = Wo->Pos.y + Wo->offset;
	TempWorld._43 = Wo->Pos.z;

//	if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &TempWorld) != D3D_OK)
//	if (FSSetMatrix(hWorld, &TempWorld) != D3D_OK)
//		return;

	if (FAILED(FSSetMatrix(D3DTS_WORLD, &TempWorld)))
	{
		return;
	}

	/*	Execute it	*/
//	if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, Wo->lpExBuf, d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)

//	if (FSExecuteBuffer(Wo->lpExBuf, d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
//			return;

	cull_none();

	if (FAILED(draw_object(&Wo->renderObject)))
		return;

	reset_cull();

//	if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &identity) != D3D_OK)
//	if (FSSetMatrix(hWorld, &identity) != D3D_OK)
//		return;

	if (FAILED(FSSetMatrix(D3DTS_WORLD, &identity)))
	{
		return;
	}
}



/*===================================================================
	Procedure	:		Loop around and see if there are any object in this group...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

BOOL WaterObjectCollide( uint16 group , VECTOR *Origin, VECTOR *Offset, VECTOR *CollidePos , float Damage )
{
	WATEROBJECT * Wo;

	Wo = WaterObjectLink[group];

	while( Wo )
	{
		if( SingleWaterObjectCollide( Wo, Origin, Offset, CollidePos ,  Damage) )
		{
			return TRUE;
		}
		Wo = Wo->GroupLink;
	}
	return FALSE;
}
/*===================================================================
	Procedure	:		Am I In The Water..
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

BOOL InWater( uint16 group , VECTOR *OrgPos , float * Damage)
{
	WATEROBJECT * Wo;

	if( GroupWaterInfo[group] == WATERSTATE_ALLWATER )
	{
		*Damage += GroupDamageInfo[group] * framelag;
		return TRUE;
	}

	Wo = WaterObjectLink[group];

	while( Wo )
	{
		if( OrgPos->y < ( Wo->Pos.y + Wo->offset ) )
		{
			*Damage += Wo->Damage * framelag;
			return TRUE;
		}

		Wo = Wo->GroupLink;
	}
	return FALSE;
}




/*===================================================================
	Procedure	:		Collide the Water Object Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
BOOL SingleWaterObjectCollide( WATEROBJECT * Wo, VECTOR *Origin, VECTOR *Offset, VECTOR *CollidePos , float Damage)
{
	float	WaterOffset;
	float	Div;
	float	Num;
	float	t;
	VECTOR	Delta;
	float	WaterX;
	float	WaterY;
	int		IWaterX;
	int		IWaterY;
	float	dx;
	float	dy;
	float	Impact;
	float	NewDamage;
	float * Vels;
	float * Verts;
	VECTOR Pos;

	Pos = Wo->Pos;
	Pos.y += Wo->offset;


	
	WaterOffset = -DotProduct( &WaterNormal, &Pos );
/*===================================================================
	Calculate T
===================================================================*/
	Div = ( Offset->x * WaterNormal.x) + 
		  ( Offset->y * WaterNormal.y) + 
		  ( Offset->z * WaterNormal.z);
	if( Div == 0.0F ) return FALSE;		/* Reject, Parallel */
	Num = ( ( Origin->x * WaterNormal.x ) +
		    ( Origin->y * WaterNormal.y ) +
		    ( Origin->z * WaterNormal.z ) ) + WaterOffset ; 
	t = -( Num / Div );

/*===================================================================
	Do Polygon collision
===================================================================*/

	if( t < 0.0F ) return FALSE;		/* Intersection behind origin */
	if( t > 1.0F ) return FALSE;		/* Intersection Greater then ray length */

	CollidePos->x = ( Origin->x + ( Offset->x * t ) );
	CollidePos->y = ( Origin->y + ( Offset->y * t ) );
	CollidePos->z = ( Origin->z + ( Offset->z * t ) );

	Delta.x = CollidePos->x - Pos.x;
	Delta.y = CollidePos->y - Pos.y;
	Delta.z = CollidePos->z - Pos.z;

	// ideally, now we'd use the WaterNormal (and other parameters, such as water orientation in the plane)
	// to find out the correct WaterX and Y positions.  for now, though... [assuming WaterNormal is (0,1,0)]
	WaterX = ( Delta.x + ( (Wo->XVerts * WATER_CELLSIZE ) * 0.5F ) ) / WATER_CELLSIZE;
	WaterY = ( Delta.z + ( (Wo->YVerts * WATER_CELLSIZE ) * 0.5F ) ) / WATER_CELLSIZE;


	IWaterX = (int) floor( WaterX );
	IWaterY = (int) floor( WaterY );
	if ( IWaterX < 0 || IWaterX >= Wo->XVerts || IWaterY < 0 || IWaterY >= Wo->YVerts )
		return FALSE;

	if( Damage == 0.0F )
	{
		return TRUE;
	}
	else
	{
		NewDamage = Damage / 32.0F;
		if( NewDamage < 0.75F )
			NewDamage = 0.75F;
	}
	
	Impact = WATER_IMPACT * NewDamage;
	if( CollidePos->y > Origin->y )
		// shooting from below.....
		Impact *= -0.25F;

	dx = 1.0F - (float) ( WaterX - floor( WaterX ) );
	dy = 1.0F - (float) ( WaterY - floor( WaterY ) );

	Vels = Wo->Vels;

	Verts = Wo->Verts;
	
#if 1
	Verts += (IWaterX * Wo->YVerts ) + IWaterY;
	Vels += (IWaterX * Wo->YVerts ) + IWaterY;
	if( *Verts < ( 1024.0F * GLOBAL_SCALE ) && 
		*Verts > -( 1024.0F * GLOBAL_SCALE ) )
	{
		*Vels = Impact * ( dx + dy );
	}
	Verts++;
	Vels++;
	if ( IWaterY + 1 < Wo->YVerts)
	{
		if( *Verts < ( 1024.0F * GLOBAL_SCALE ) && 
			*Verts  > -( 1024.0F * GLOBAL_SCALE ) )
		{
			*Vels = Impact * ( dx + ( 1 - dy ) );
		}
	}
	if ( IWaterX + 1 < Wo->XVerts )
	{
		Verts += Wo->YVerts-1;
		Vels += Wo->YVerts-1;

		if( *Verts < ( 1024.0F * GLOBAL_SCALE ) && 
			*Verts > -( 1024.0F * GLOBAL_SCALE ) )
		{
			*Vels = Impact * ( ( 1 - dx ) + dy );
		}
		if ( IWaterY + 1 < Wo->YVerts )
		{
			Verts++;
			Vels++;
			if( *Verts < ( 1024.0F * GLOBAL_SCALE ) && 
				*Verts > -( 1024.0F * GLOBAL_SCALE ) )
			{
				*Vels = Impact * ( ( 1 - dx ) + ( 1 - dy ) );
			}
		}
	}
#else	
	if( *(Verts +((IWaterX) * Wo->YVerts ) + (IWaterY) ) < ( 1024.0F * GLOBAL_SCALE ) && 
		*(Verts +((IWaterX) * Wo->YVerts ) + (IWaterY) ) > -( 1024.0F * GLOBAL_SCALE ) )
	{
		*(Vels+((IWaterX) * Wo->YVerts ) + (IWaterY) ) = Impact * ( dx + dy );
	}
	if ( IWaterX + 1 < Wo->XVerts )
	{
		if( *(Verts +((IWaterX+1) * Wo->YVerts ) + (IWaterY) ) < ( 1024.0F * GLOBAL_SCALE ) && 
			*(Verts +((IWaterX+1) * Wo->YVerts ) + (IWaterY) ) > -( 1024.0F * GLOBAL_SCALE ) )
		{
			*(Vels+((IWaterX+1) * Wo->YVerts ) + (IWaterY) ) = Impact * ( ( 1 - dx ) + dy );
		}
		if ( IWaterY + 1 < Wo->YVerts )
		{
			if( *(Verts +((IWaterX+1) * Wo->YVerts ) + (IWaterY+1) ) < ( 1024.0F * GLOBAL_SCALE ) && 
				*(Verts +((IWaterX+1) * Wo->YVerts ) + (IWaterY+1) ) > -( 1024.0F * GLOBAL_SCALE ) )
			{
				*(Vels+((IWaterX+1) * Wo->YVerts ) + (IWaterY+1) ) = Impact * ( ( 1 - dx ) + ( 1 - dy ) );
			}
		}
	}
	if ( IWaterY + 1 < Wo->YVerts)
	{
		if( *(Verts +((IWaterX) * Wo->YVerts ) + (IWaterY+1) ) < ( 1024.0F * GLOBAL_SCALE ) && 
			*(Verts +((IWaterX) * Wo->YVerts ) + (IWaterY+1) ) > -( 1024.0F * GLOBAL_SCALE ) )
		{
			*(Vels+((IWaterX) * Wo->YVerts ) + (IWaterY+1) ) = Impact * ( dx + ( 1 - dy ) );
		}
	}
#endif
	
	return TRUE;
}


/*===================================================================
	Procedure	:	Water Process...
	Input		:	NOTHING
	Output		:	NOTHING
===================================================================*/
void WaterProcess( void )
{
	int			i;
	WATEROBJECT	* WO;

	WO = FirstWaterObject;
	if( !WO )
		return;
	for( i = 0 ; i < NumOfWaterObjects ; i++ )
	{
		if( WO->Status == WATERSTATUS_DRAINING )
		{
			WO->offset -= WO->DrainRate * framelag;
			if( WO->offset <= WO->MinLevel )
			{
				WO->offset = WO->MinLevel;
				WO->Status = WATERSTATUS_STATIC;
				if( WO->Trigger_WhenDrained != 0xffff )
					ApplyTriggerMod( &TrigMods[WO->Trigger_WhenDrained] );
			}
			GroupWaterLevel[WO->Group] = WO->Pos.y + WO->offset + WATER_CELLSIZE;
		}else if( WO->Status == WATERSTATUS_FILLING )
		{

			WO->offset += WO->FillRate * framelag;
			if( WO->offset >= WO->MaxLevel )
			{
				WO->offset = WO->MaxLevel;
				WO->Status = WATERSTATUS_STATIC;
				if( WO->Trigger_WhenFilled != 0xffff )
					ApplyTriggerMod( &TrigMods[WO->Trigger_WhenFilled] );
			}
			GroupWaterLevel[WO->Group] = WO->Pos.y + WO->offset + WATER_CELLSIZE;
		}

		WO++;
	}
}

/*===================================================================
	Procedure	:	Trigger water to drain...
	Input		:	NOTHING
	Output		:	NOTHING
===================================================================*/
void TriggerWaterDrain( uint16 * Data )
{
	WATEROBJECT	* WO;

	WO = FirstWaterObject;
	if( !WO )
		return;
	WO += *Data;
	WO->Status = WATERSTATUS_DRAINING;
}
/*===================================================================
	Procedure	:	Trigger water to fill...
	Input		:	NOTHING
	Output		:	NOTHING
===================================================================*/
void TriggerWaterFill( uint16 * Data )
{
	WATEROBJECT	* WO;

	WO = FirstWaterObject;
	if( !WO )
		return;
	WO += *Data;
	WO->Status = WATERSTATUS_FILLING;
}


/*===================================================================
	Procedure	:	Get Water Colour in group
	Input		:	uint16		Group
				:	uint8	*	Red;
				:	uint8	*	Green;
				:	uint8	*	Blue;
	Output		:	Nothing
===================================================================*/
void GetWaterColour( uint16 Group, uint8 * Red, uint8 * Green, uint8 * Blue )
{
	*Red	= (uint8) ( GroupWaterIntensity_Red[ Group ] * 255.0F );
	*Green	= (uint8) ( GroupWaterIntensity_Green[ Group ] * 255.0F );
	*Blue	= (uint8) ( GroupWaterIntensity_Blue[ Group ] * 255.0F );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif











