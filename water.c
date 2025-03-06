
/*===================================================================
		Include File...	
===================================================================*/
#include "main.h"
#include <stdio.h>
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "triggers.h"
#include "file.h"
#include "render.h"
#include "new3d.h"
#include "tload.h"
#include "mload.h"
#include "transexe.h"
#include "water.h"
#include "util.h"
#include "oct2.h"

/*===================================================================
		Externs...	
===================================================================*/
extern	MATRIX ProjMatrix;
extern	TLOADHEADER Tloadheader;
extern	float	framelag;
extern	DWORD	CurrentSrcBlend;
extern	DWORD	CurrentDestBlend;
extern	DWORD	CurrentTextureBlend;
extern	RENDERMATRIX view;
extern	RENDERMATRIX identity;
extern	MATRIX	MATRIX_Identity;
extern	RENDERMATRIX  TempWorld;	
extern	RENDERMATRIX identity;
extern	u_int32_t	AnimOncePerFrame;// used for stuff that is displayed more than once in a single frame..
extern TRIGGERMOD	*	TrigMods;

/*===================================================================
		Defines
===================================================================*/
#define	WAT_VERSION_NUMBER	1

/*===================================================================
		Globals...	
===================================================================*/
float WATER_CELLSIZE = 64.0F;

u_int16_t	NumOfWaterObjects = 0;
u_int16_t	WaterTPage = 0;
WATEROBJECT	* FirstWaterObject = NULL;
char	* WaterBuffer = NULL;
char	* OrgWaterBuffer = NULL;
u_int16_t	GroupWaterInfo[MAXGROUPS];

float	GroupDamageInfo[MAXGROUPS];

float	GroupWaterLevel[MAXGROUPS];
float	GroupWaterIntensity_Red[MAXGROUPS];
float	GroupWaterIntensity_Green[MAXGROUPS];
float	GroupWaterIntensity_Blue[MAXGROUPS];

extern	float framelag;
float	Fc[MAXMESHX*MAXMESHY];

void UpdateWaterMesh( WATEROBJECT * WO );
void DisplayWaterObject(WATEROBJECT * WO);
bool InitWaterObject(WATEROBJECT * WO);

VECTOR	WaterNormal = { 0.0F, 1.0F, 0.0F };

WATEROBJECT	* WaterObjectLink[MAXGROUPS];

float WaterFade = 1.0F;
/*===================================================================
	Procedure	:	Pre Water Load..
	Input		:	char * filename....
	Output		:	bool
===================================================================*/
extern int16_t LevelNum;
bool PreWaterLoad( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	u_int16_t * Uint16Pnt;
	u_int32_t * Uint32Pnt;
	char	*	FileNamePnt;
	char buf[256];
	int i;
	u_int32_t	MagicNumber;
	u_int32_t	VersionNumber;
	int8_t		TempFilename[ 256 ];

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
		return true;
	}
	Buffer = malloc( File_Size );
	OrgWaterBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Pre Water Load : Unable to allocate file buffer\n", Filename );
		return( false );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "Pre Water Load Error reading %s\n", Filename );
		return( false );
	}

	Uint32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *Uint32Pnt++;
	VersionNumber = *Uint32Pnt++;
	Buffer = (char *) Uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != WAT_VERSION_NUMBER  ) )
	{
		Msg( "PreWaterLoad() Incompatible water( .WAT ) file %s", Filename );
		return( false );
	}

	Uint16Pnt = (u_int16_t *) Buffer;

	NumOfWaterObjects = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;		
	// get the Tpage sorted out...
	FileNamePnt = (char *) &buf[0];
	while ( ( *FileNamePnt++ = *Buffer++ ) != 0 );

	GetLevelTexturePath( &TempFilename[ 0 ], &buf[0], &ShortLevelNames[ LevelNum ][ 0 ] );
	
	DebugPrintf("loading water texture: %s\n",TempFilename);

	WaterTPage = AddTexture( &Tloadheader , &TempFilename[ 0 ], true , true , true, 0, 0 );		// dont colourkey
	WaterBuffer = Buffer;
	return true;
}

/*===================================================================
	Procedure	:	Water Load..
	Input		:	char * filename....
	Output		:	bool
===================================================================*/
bool WaterLoad( void )
{
	char		*	Buffer;
	char		*	OrgBuffer;
	int			i;
	WATEROBJECT	* WO;
	float * FloatPnt;
	u_int16_t * Uint16Pnt;
	int16_t * int16Pnt;
	u_int16_t NumOfGroups = 0;
	COLOR col;
	COLOR * D3DColourPnt;

	FirstWaterObject = NULL;
	Buffer = WaterBuffer;
	OrgBuffer = OrgWaterBuffer;

	if( !Buffer )
		return true;

	Uint16Pnt = (u_int16_t *) Buffer;
	NumOfGroups = *Uint16Pnt++;
	Buffer = (char*) Uint16Pnt;

	for( i = 0 ; i < NumOfGroups ; i++ )
	{
		Uint16Pnt = (u_int16_t *) Buffer;
		GroupWaterInfo[i] = *Uint16Pnt++;
		Buffer = (char*) Uint16Pnt;

		D3DColourPnt = (COLOR *) Buffer;
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
		return( false );
	}
	
	memset(FirstWaterObject, 0, NumOfWaterObjects * sizeof(FirstWaterObject));	
	
	WO = FirstWaterObject;
	WO->Group = 0;
	WO->AnimOncePerFrame = 0;
	for( i = 0 ; i < NumOfWaterObjects ; i++ )
	{
		WO->Status = WATERSTATUS_STATIC;
		Uint16Pnt = (u_int16_t *) Buffer;
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
			DebugPrintf("Not loading water, either xVerts or yVerts was less than 1\n");
			DebugPrintf("xVerts=%d, yVerts=%d, xSize=%d, ySize=%d, cellSize=%d\n",
				WO->XVerts,WO->YVerts,WO->XSize,WO->YSize,WATER_CELLSIZE);
			return false;
		}

		if( ( WO->XVerts * WO->YVerts ) > 1024)
		{
			Msg( "Water Load : Water is to big\n" );
			return( false );
		}
		if( !InitWaterObject( WO ) )
		{
			Msg( "Water Load : Unable to init water object\n" );
			return( false );

		}
		AddWaterLink(WO);
		Buffer = (char *) FloatPnt;		

		D3DColourPnt = (COLOR *) Buffer;
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

		Uint16Pnt = (u_int16_t *) Buffer;
		WO->Trigger_WhenFilled = *Uint16Pnt++;
		WO->Trigger_WhenDrained = *Uint16Pnt++;
		int16Pnt = (int16_t*) Uint16Pnt;
		WO->Damage = (((float) *int16Pnt++) / 60.0F);
		Buffer = (char *) int16Pnt;

		WO++;
	}
	free( OrgBuffer );
	return true;
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
	Output		:		bool true/false
===================================================================*/
bool InitWaterObject(WATEROBJECT * WO)
{
	int x,y;
	LPTRIANGLE	FacePnt = NULL;
	LPLVERTEX	lpLVERTEX = NULL;
	WORD			*lpIndices = NULL;
	int			i;
	int ntris = 0;
	int vertsCount = 0;

	WO->num_of_verts = WO->XVerts * WO->YVerts;
	WO->Verts = (float*) calloc(  WO->num_of_verts , sizeof(float) );
	WO->Vels = (float*) calloc( WO->num_of_verts , sizeof(float) );

	if (!FSCreateVertexBuffer(&WO->renderObject, WO->num_of_verts))
	{
		DebugPrintf("water FSCreateVertexBuffer failed\n");
		return false;
	}

	// pre-count the number of tris so we know how big to make the index buffer
	// this loop structure was coppied from the way the triangles are assigned further bellow
	for( x = 0 ; x < WO->XVerts-1 ; x++ )
		for( y = 0 ; y < WO->YVerts-1 ; y++ )
			ntris += 2;

	if (!FSCreateIndexBuffer(&WO->renderObject, ntris*3 )) // 3 vertexes in a triangle
	{
		DebugPrintf("water FSCreateIndexBuffer failed\n");
		return false;
	}

	if (!FSLockVertexBuffer(&WO->renderObject, &lpLVERTEX))
	{
		DebugPrintf("water FSLockVertexBuffer failed\n");
		return false;
	}

	if (!(FSLockIndexBuffer(&WO->renderObject, &lpIndices)))
	{
		DebugPrintf("water FSLockIndexBuffer failed\n");
		return false;
	}

	FacePnt =  (LPTRIANGLE) lpIndices;

	for( x = 0 ; x < WO->XVerts ; x++ )
	{
		for( y = 0 ; y < WO->YVerts ; y++ )
		{
			lpLVERTEX->x = -( ((WO->XVerts-1) * WATER_CELLSIZE ) * 0.5F ) + (x*WATER_CELLSIZE);
			lpLVERTEX->z = -( ((WO->YVerts-1) * WATER_CELLSIZE ) * 0.5F ) + (y*WATER_CELLSIZE);
			lpLVERTEX->y = 0.0F;
			lpLVERTEX->tu = 0.0F;
			lpLVERTEX->tv = 0.0F;
			lpLVERTEX->color = RGBA_MAKE(128,128,128,128);
//			lpLVERTEX->dwReserved = 0;
			
			lpLVERTEX++;

			vertsCount++;
		}
	}

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

	if (!(FSUnlockVertexBuffer(&WO->renderObject)))
	{
		DebugPrintf( "water FSUnlockVertexBuffer failed\n");
		return false;
	}
	
	if (!(FSUnlockIndexBuffer(&WO->renderObject)))
	{
		DebugPrintf( "water FSUnlockIndexBuffer failed\n");
		return false ;
	}

	/*	set the data for the execute buffer	*/
	WO->renderObject.numTextureGroups = 1;
	WO->renderObject.textureGroups[0].numTriangles = ntris;
	WO->renderObject.textureGroups[0].numVerts = WO->num_of_verts;
	WO->renderObject.textureGroups[0].startIndex = 0;
	WO->renderObject.textureGroups[0].startVert = 0;
	WO->renderObject.textureGroups[0].texture = Tloadheader.lpTexture[WaterTPage]; 
	WO->renderObject.textureGroups[0].colourkey = Tloadheader.ColourKey[WaterTPage];

	return true;
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif
/*===================================================================
	Procedure	:	Process Water Based on Group..
	Input		:	u_int16_t group
	Output		:	NOTHING
===================================================================*/
void GroupWaterProcessDisplay( u_int16_t group )
{
	WATEROBJECT	* WO;
	u_int16_t i;
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
	LPLVERTEX	lpLVERTEX;
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


	memmove( &Fc[0] , WO->Verts , WO->num_of_verts * sizeof(float) );//memcpy

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
//		return false;

	if (!(FSLockVertexBuffer(/*WO->lpVertexBuffer*/&WO->renderObject, &lpLVERTEX)))
	{
		return;
	}

/*
	lpBufStart = debDesc.lpData;
	lpLVERTEX = ( LPLVERTEX )lpBufStart;
*/
	VertPnt = WO->Verts;
	
	for( x = 0 ; x < WO->XVerts ; x++ )
	{
		for( y = 0 ; y < WO->YVerts ; y++ )
		{
			lpLVERTEX->y = *VertPnt;
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
			lpLVERTEX->tu = (u * WO->uRange) + WO->uTL;
			lpLVERTEX->tv = (v * WO->vRange) + WO->vTL;
			col = 192 + (int) ( dy * 8 );
			if( col > 255 )
				col = 255;
			if( col < 0 )
				col = 0;
			lpLVERTEX->color = RGBA_MAKE( (int)(col * WO->Red) , (int)(col * WO->Green) , (int)(col * WO->Blue) , 128);
			lpLVERTEX++;
			VertPnt++;
		}
	}
	/*	unlock the execute buffer	*/
//	if ( WO->lpExBuf->lpVtbl->Unlock( WO->lpExBuf ) != D3D_OK)
//		return;
	if (!(FSUnlockVertexBuffer(/*WO->lpVertexBuffer*/&WO->renderObject)))
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
	TempWorld = identity;
	TempWorld._41 = Wo->Pos.x;
	TempWorld._42 = Wo->Pos.y + Wo->offset;
	TempWorld._43 = Wo->Pos.z;

	if (!FSSetWorld(&TempWorld))
		return;

	cull_none();

	if (!draw_object(&Wo->renderObject))
		return;

	reset_cull();

	if (!FSSetWorld(&identity))
		return;
}



/*===================================================================
	Procedure	:		Loop around and see if there are any object in this group...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

bool WaterObjectCollide( u_int16_t group , VECTOR *Origin, VECTOR *Offset, VECTOR *CollidePos , float Damage )
{
	WATEROBJECT * Wo;

	Wo = WaterObjectLink[group];

	while( Wo )
	{
		if( SingleWaterObjectCollide( Wo, Origin, Offset, CollidePos ,  Damage) )
		{
			return true;
		}
		Wo = Wo->GroupLink;
	}
	return false;
}
/*===================================================================
	Procedure	:		Am I In The Water..
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

bool InWater( u_int16_t group , VECTOR *OrgPos , float * Damage)
{
	WATEROBJECT * Wo;

	if( GroupWaterInfo[group] == WATERSTATE_ALLWATER )
	{
		*Damage += GroupDamageInfo[group] * framelag;
		return true;
	}

	Wo = WaterObjectLink[group];

	while( Wo )
	{
		if( OrgPos->y < ( Wo->Pos.y + Wo->offset ) )
		{
			*Damage += Wo->Damage * framelag;
			return true;
		}

		Wo = Wo->GroupLink;
	}
	return false;
}




/*===================================================================
	Procedure	:		Collide the Water Object Stuff...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
bool SingleWaterObjectCollide( WATEROBJECT * Wo, VECTOR *Origin, VECTOR *Offset, VECTOR *CollidePos , float Damage)
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
	if( Div == 0.0F ) return false;		/* Reject, Parallel */
	Num = ( ( Origin->x * WaterNormal.x ) +
		    ( Origin->y * WaterNormal.y ) +
		    ( Origin->z * WaterNormal.z ) ) + WaterOffset ; 
	t = -( Num / Div );

/*===================================================================
	Do Polygon collision
===================================================================*/

	if( t < 0.0F ) return false;		/* Intersection behind origin */
	if( t > 1.0F ) return false;		/* Intersection Greater then ray length */

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
		return false;

	if( Damage == 0.0F )
	{
		return true;
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
	
	return true;
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
void TriggerWaterDrain( u_int16_t * Data )
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
void TriggerWaterFill( u_int16_t * Data )
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
	Input		:	u_int16_t		Group
				:	u_int8_t	*	Red;
				:	u_int8_t	*	Green;
				:	u_int8_t	*	Blue;
	Output		:	Nothing
===================================================================*/
void GetWaterColour( u_int16_t Group, u_int8_t * Red, u_int8_t * Green, u_int8_t * Blue )
{
	*Red	= (u_int8_t) ( GroupWaterIntensity_Red[ Group ] * 255.0F );
	*Green	= (u_int8_t) ( GroupWaterIntensity_Green[ Group ] * 255.0F );
	*Blue	= (u_int8_t) ( GroupWaterIntensity_Blue[ Group ] * 255.0F );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif











