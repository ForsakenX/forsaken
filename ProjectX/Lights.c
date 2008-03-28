
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*	l i g h t s . c
*	All routines to do with Lights...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "Lights.h"
#include "water.h"
#include "visi.h"
#include "text.h"
#include "main.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

#define MIN_LIGHT_SIZE	( 1536.0F * GLOBAL_SCALE )

#undef TESTING_SUBTRACTIVE

#define	USE_SPECULAR
#undef USE_SPECULAR

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern int CurrentLoadingStep;
extern	BOOL	ShowPlaneRGB;
extern	float	WhiteOut;

extern	uint16	NumGroupsVisible;
extern	uint16	GroupsVisible[MAXGROUPS];
extern	int16	ShowPortal;

extern	int		NumOfVertsConsidered;
extern	int		NumOfVertsTouched;

extern	float	framelag;
extern	BOOL	PowerVR;
extern	float	SoundInfo[MAXGROUPS][MAXGROUPS];
extern	uint16	GroupWaterInfo[MAXGROUPS];
extern	float	GroupWaterLevel[MAXGROUPS];
extern	float	GroupWaterIntensity_Red[MAXGROUPS];
extern	float	GroupWaterIntensity_Green[MAXGROUPS];
extern	float	GroupWaterIntensity_Blue[MAXGROUPS];
extern BOOL	BrightShips;

extern	CAMERA	CurrentCamera;
void PrintInitViewStatus( BYTE Status );
void DebugPrintf( const char * format, ... );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
D3DCOLOR	GroupColours[ 8 ] = {

	RGBA_MAKE( 0,128,0,255 ),
	RGBA_MAKE( 0,0,128,255 ),
	RGBA_MAKE( 0,128,128,255 ),
	RGBA_MAKE( 128,128,0,255 ),
	RGBA_MAKE( 128,0,128,255 ),
	RGBA_MAKE( 128,128,128,255 ),
	RGBA_MAKE( 64,128,64,255 ),
	RGBA_MAKE( 128,64,64,255 )
};

XLIGHT * FirstLightVisible = NULL;
XLIGHT	XLights[MAXXLIGHTS];
uint16	FirstXLightUsed;
uint16	FirstXLightFree;

WORD	status;		
DWORD	chop_status;		


D3DCOLOR WorkOutAverageLight( VECTOR * Pos , MLOADHEADER * Mloadheader , uint16 group , uint16 execbuf );


__inline
void FLOAT2INT( int * I, float F )
{
	__asm
	{
		fld		F
		mov		esi, I
		fistp	[esi]
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Floating Point Cull Mode
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#ifdef USEASM
_inline
void	start_chop(void)
{
__asm
	{
		fstcw word ptr status
		mov eax,dword ptr status
		or eax,3072
		mov dword ptr chop_status,eax
		fldcw word ptr chop_status
	}
}

_inline
void	end_chop(void)
{
__asm
	{
		fldcw word ptr status
	}
}
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Does this light fall in a group...
	Input		:	nothing
	Output		:	FALSE/TRUE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#ifdef USEINLINE
_inline
#endif
BOOL	DoIEffectThisGroup1( MLOADHEADER * Mloadheader , VECTOR * Pos , float size , uint16 group)
{
	VECTOR	Temp;
	Temp.x = Pos->x - Mloadheader->Group[group].center.x;
	if( Temp.x < 0.0F )	Temp.x *= -1.0F;
	Temp.y = Pos->y - Mloadheader->Group[group].center.y;
	if( Temp.y < 0.0F )	Temp.y *= -1.0F;
	Temp.z = Pos->z - Mloadheader->Group[group].center.z;
	if( Temp.z < 0.0F )	Temp.z *= -1.0F;
	if ( (Temp.x <= ( Mloadheader->Group[group].half_size.x + size ) ) &&
		 (Temp.y <= ( Mloadheader->Group[group].half_size.y + size ) ) &&
		 (Temp.z <= ( Mloadheader->Group[group].half_size.z + size ) ) )
	{
		return TRUE;
	}
	return FALSE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set up And Init all XLights
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	InitXLights()
{
	uint16	i;
	FirstXLightUsed = (uint16) -1;
	FirstXLightFree = 0;
	for( i = 0 ; i < MAXXLIGHTS ; i++ )
	{
		XLights[i].Index = i;
		XLights[i].Next = i + 1;
		XLights[i].Prev = (uint16) -1;
		XLights[i].Type = POINT_LIGHT;
	}

	XLights[MAXXLIGHTS-1].Next = (uint16) -1;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find a free light and move it from the free list to
					the used list
	Input		:	nothing
	Output		:	uint16 number of light free....
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	FindFreeXLight()
{
	uint16 i;

	i = FirstXLightFree;
	
	if ( i == (uint16) -1)
		return i;
 
	XLights[i].Prev = FirstXLightUsed;
	if ( FirstXLightUsed != (uint16) -1)
	{
		XLights[FirstXLightUsed].Next = i;
	}
	FirstXLightUsed = i;
	FirstXLightFree = XLights[i].Next;
	XLights[i].Type = POINT_LIGHT;
	XLights[i].Visible = TRUE;

	return i ;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill a used light and move it from the used list to
					the free list
	Input		:	uint16 number of light free....
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	KillUsedXLight( uint16 light )
{
	uint16	its_prev;
	uint16	its_next;
	
	its_prev = XLights[light].Prev;
	its_next = XLights[light].Next;

	if ( light == FirstXLightUsed )
		FirstXLightUsed = XLights[light].Prev;

	if( its_prev != (uint16) -1)
		XLights[its_prev].Next = its_next;

	if( its_next != (uint16) -1)
		XLights[its_next].Prev = its_prev;

	XLights[light].Prev = (uint16) -1;
	XLights[light].Next = FirstXLightFree;
	FirstXLightFree	= light;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process all XLights
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL	ProcessXLights( MLOADHEADER * Mloadheader )
{
	uint16	light;
	uint16	oldlight;

	light = FirstXLightUsed;
	while( light != (uint16 ) -1 )
	{
		oldlight = XLights[light].Prev;

		if(	XLights[light].SizeCount != 0.0F )
		{
			XLights[light].Size	-= XLights[light].SizeCount * framelag;
			XLights[light].r -= XLights[light].ColorCount * framelag;
			XLights[light].g -= XLights[light].ColorCount * framelag;
			XLights[light].b -= XLights[light].ColorCount * framelag;
			if(	XLights[light].r <= 0.0F )
				XLights[light].r = 0.0F;
			if(	XLights[light].g <= 0.0F )
				XLights[light].g = 0.0F;
			if(	XLights[light].b <= 0.0F )
				XLights[light].b = 0.0F;
			if( XLights[light].Size	<= 0.0F)
				KillUsedXLight(light);
		}
		light = oldlight;
	}

	return TRUE;
}




/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*		cause a light to go red and get smaller then die...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void SetLightDie ( uint16 light )
{				
	XLights[light].SizeCount = 50.0F;
	XLights[light].ColorCount = 2.0F;
	XLights[light].r = 255.0F;
	XLights[light].g = 0.0F;
	XLights[light].b = 0.0F;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Xlight 1 Group Only...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float	cral = 0.0F;


BOOL	XLight1Group( MLOADHEADER * Mloadheader, uint16 group )
{
	XLIGHT * XLightPnt;
	float	blf;
	float	glf;
	float	rlf;
	D3DEXECUTEBUFFERDESC	debDesc;
	VECTOR	Temp;
	VECTOR	CellIndex;
	float	distance;
	int		execbuf;
	int		vert;
    LPD3DLVERTEX	lpPointer;
	LPD3DLVERTEX	lpD3DLVERTEX;
	LPD3DLVERTEX	lpD3DLVERTEX2;
	D3DCOLOR col;
	VERTEXCELL * VertexCellPnt;
	uint16 * VertexIndexPnt;
	uint16 * OrgVertexIndexPnt;
	int	Cell;
	int	CellIndex_x;
	int	CellIndex_y;
	int	CellIndex_z;
	int	CellRange_x;
	int	CellRange_y;
	int	CellRange_z;
	int	Cellx;
	int	Celly;
	int	Cellz;
	int		NumOfxCells;
	int		NumOfyCells;
	int		NumOfzCells;
	float	CellSize;
	float	Size,OSize;
	float	SizeX2;
	float	x,y,z;
	float	Posx,Posy,Posz;
	float	Dirx,Diry,Dirz;
	float	Cosa,CosArc;
	float	rlen;
	float	intense;
	uint32	tempiR;
	uint32	tempiG;
	uint32	tempiB;
	uint32	tempiA;
	float	centerx;  
	float	centery;  
	float	centerz;  
	float	half_sizex;
	float	half_sizey;
	float	half_sizez;
	uint32 inc;
	uint32 carry;
	uint32 clamp;
	uint32 r,g,b,intWhiteOut;
	POLYANIM * PolyAnim;
	int i,e;
	uint32 * uint32Pnt;
	TANIMUV * TanimUV;
	float	intensity;

	intWhiteOut = (int)WhiteOut;
	if( intWhiteOut >= 256 )
	{
		intWhiteOut = (256 - (intWhiteOut-256) );
	}

	
	CellSize = Mloadheader->CellSize;
	execbuf = Mloadheader->Group[group].num_execbufs;
	while( execbuf--)
	{
		memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		/*	lock the execute buffer	*/
		if ( Mloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( Mloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
			return FALSE ;
		lpPointer = (LPD3DLVERTEX) debDesc.lpData;

		VertexCellPnt = Mloadheader->Group[group].vertex_cell_pnt[execbuf];
		OrgVertexIndexPnt = Mloadheader->Group[group].vertex_index_pnt[execbuf];

		NumOfxCells = Mloadheader->Group[group].xcells[execbuf];
		NumOfyCells = Mloadheader->Group[group].ycells[execbuf];
		NumOfzCells = Mloadheader->Group[group].zcells[execbuf];

		if( Mloadheader->Group[group].num_animating_polys[execbuf] != 0 )
		{

			PolyAnim = Mloadheader->Group[group].polyanim[execbuf];

			for( i = 0 ; i < Mloadheader->Group[group].num_animating_polys[execbuf] ; i++ )
			{
				
				if( PolyAnim->currentframe != PolyAnim->newframe )
				{
					// something has changed....

					uint32Pnt = PolyAnim->vert;
					for( e = 0 ; e < PolyAnim->vertices ; e++ )
					{

						lpD3DLVERTEX = lpPointer+ *uint32Pnt++;
						TanimUV = PolyAnim->UVs;
						TanimUV += e + (PolyAnim->vertices * PolyAnim->newframe);
						lpD3DLVERTEX->tu = TanimUV->u;
						lpD3DLVERTEX->tv = TanimUV->v;
					}

					PolyAnim->currentframe = PolyAnim->newframe;
				}
				PolyAnim++;
			}
		}
		
		
		
		lpD3DLVERTEX = lpPointer;
		if( ( myglobs.rstate.FillMode == D3DFILL_WIREFRAME )&& ShowPortal )
		{
			vert = Mloadheader->Group[group].num_verts_per_execbuf[execbuf];
			while( vert --)
			{
				lpD3DLVERTEX->color = GroupColours[ group % 8 ];
				lpD3DLVERTEX++;
			}
		}else{
			lpD3DLVERTEX2 = Mloadheader->Group[group].org_vertpnt[execbuf];
			
			vert = Mloadheader->Group[group].num_verts_per_execbuf[execbuf];
			
			
			if( GroupWaterInfo[group] == WATERSTATE_NOWATER || ShowPlaneRGB )
			{
				if( WhiteOut == 0.0F )
				{
#ifdef	USEASM
					__asm
					{
						mov		eax, DWORD PTR ShowPlaneRGB
						or		eax, eax
						jnz		useplanergb
						mov		esi, [lpD3DLVERTEX2]
						add		esi,16
						jmp		go
useplanergb:
						mov		esi, [lpD3DLVERTEX2]
						add		esi,12
go:
						mov		edi, [lpD3DLVERTEX]
						add		edi,16
						mov		ecx, vert
clear:					mov		eax, [esi]
						add		esi, 32
						mov		[edi],eax
						add		edi, 32
						dec		ecx
						jnz		clear
					}
#else	//USEASM
					if ( ShowPlaneRGB )
					{
						while( vert --)
						{
							lpD3DLVERTEX->color = (D3DCOLOR) lpD3DLVERTEX2->dwReserved;
							lpD3DLVERTEX++;		
							lpD3DLVERTEX2++;		
						}
					}
					else
					{
						while( vert --)
						{
							lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
							lpD3DLVERTEX++;		
							lpD3DLVERTEX2++;		
						}
					}
#endif	//USEASM
					
				}else{
					//	Special Lighting effects
					while( vert --)
					{
						
						x = (float)((int) (lpD3DLVERTEX2->x * 0.35F) % 360);
						y = (float)((int) (lpD3DLVERTEX2->y * 0.35F) % 360);
						z = (float)((int) (lpD3DLVERTEX2->z * 0.35F) % 360);
						//					distance = (float) sqrt( (x*x) + (y*y) + (z*z) );
						col = lpD3DLVERTEX2->color;
						col &= 0xffff;
						//					tal = 128.0F + 127.0F * (float) sin( (distance+cral) * (PI / 180.0F ) );
						b = (int)  ( ( sin( D2R( x + cral ) ) + sin( D2R( y + cral ) ) + sin( D2R( z + cral ) ) ) * 127.0F * 0.3333333F + 128.0F ) ;
						b+= intWhiteOut;
						if( b > 255 )
							b = 255;
						col |= (b<<24)+(b<<16);
						lpD3DLVERTEX->color = col;
						lpD3DLVERTEX++;		
						lpD3DLVERTEX2++;		
						
					}
				}
			}else if( GroupWaterInfo[group] == WATERSTATE_ALLWATER )
			{
				// ****************** Full Water Effect ********************************
				lpD3DLVERTEX2 = Mloadheader->Group[group].org_vertpnt[execbuf];
				lpD3DLVERTEX = lpPointer;
				vert = Mloadheader->Group[group].num_verts_per_execbuf[execbuf];
				//	Special Lighting effects
				while( vert --)
				{
					x = (float)((int) (lpD3DLVERTEX2->x * 0.35F) % 360);
					y = (float)((int) (lpD3DLVERTEX2->y * 0.35F) % 360);
					z = (float)((int) (lpD3DLVERTEX2->z * 0.35F) % 360);
					col = lpD3DLVERTEX2->color;
					r = RGBA_GETRED(col);
					g = RGBA_GETGREEN(col);
					b = RGBA_GETBLUE(col);

					r >>=2;
					g >>=2;
					b >>=2;
					intensity = (float) ( ( sin( D2R( x + cral ) ) + sin( D2R( y + cral ) ) + sin( D2R( z + cral ) ) ) * 127.0F * 0.3333333F + 128.0F ) ;
					r += (int) (GroupWaterIntensity_Red[group] * intensity);
					if( r > 255 )
						r = 255;
					g += (int) (GroupWaterIntensity_Green[group] * intensity);
					if( g > 255 )
						g = 255;
					b += (int) (GroupWaterIntensity_Blue[group] * intensity);
					if( b > 255 )
						b = 255;
					lpD3DLVERTEX->color = RGBA_MAKE( r ,g ,b , 128 );
					lpD3DLVERTEX++;		
					lpD3DLVERTEX2++;		
				}
				// ****************** End of Water Effect ******************************
			}else{
				// ****************** Partial Water Effect ******************************
				lpD3DLVERTEX2 = Mloadheader->Group[group].org_vertpnt[execbuf];
				lpD3DLVERTEX = lpPointer;
				vert = Mloadheader->Group[group].num_verts_per_execbuf[execbuf];
				//	Special Lighting effects
				while( vert --)
				{
					if( lpD3DLVERTEX2->y < GroupWaterLevel[group] )
					{
						x = (float)((int) (lpD3DLVERTEX2->x * 0.35F) % 360);
						y = (float)((int) (lpD3DLVERTEX2->y * 0.35F) % 360);
						z = (float)((int) (lpD3DLVERTEX2->z * 0.35F) % 360);
						col = lpD3DLVERTEX2->color;
						r = RGBA_GETRED(col);
						g = RGBA_GETGREEN(col);
						b = RGBA_GETBLUE(col);
						
						r >>=2;
						g >>=2;
						b >>=2;
						intensity = (float) ( ( sin( D2R( x + cral ) ) + sin( D2R( y + cral ) ) + sin( D2R( z + cral ) ) ) * 127.0F * 0.3333333F + 128.0F ) ;
						r += (int) (GroupWaterIntensity_Red[group] * intensity);
						if( r > 255 )
							r = 255;
						g += (int) (GroupWaterIntensity_Green[group] * intensity);
						if( g > 255 )
							g = 255;
						b += (int) (GroupWaterIntensity_Blue[group] * intensity);
						if( b > 255 )
							b = 255;
						lpD3DLVERTEX->color = RGBA_MAKE( r ,g ,b , 128 );
					}else{
  						lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
					}
					lpD3DLVERTEX++;		
					lpD3DLVERTEX2++;		
				}
				// ****************** End of Water Effect ******************************
			}
		}
		if( WhiteOut == 0.0F )
		{

			centerx		=	Mloadheader->Group[group].center.x;
			centery		=	Mloadheader->Group[group].center.y;
			centerz		=	Mloadheader->Group[group].center.z;
			half_sizex	=	Mloadheader->Group[group].half_size.x;
			half_sizey	=	Mloadheader->Group[group].half_size.y;
			half_sizez	=	Mloadheader->Group[group].half_size.z;
			
			

			XLightPnt = FirstLightVisible;
			while( XLightPnt )
			{
				if( GroupsAreVisible( group, XLightPnt->Group ) )
				{
					Posx = XLightPnt->Pos.x;
					Posy = XLightPnt->Pos.y;
					Posz = XLightPnt->Pos.z;
					OSize  = XLightPnt->Size;
			
					Temp.x = Posx - centerx;
					if( Temp.x < 0.0F )	Temp.x *= -1.0F;
					Temp.y = Posy - centery;
					if( Temp.y < 0.0F )	Temp.y *= -1.0F;
					Temp.z = Posz - centerz;
					if( Temp.z < 0.0F )	Temp.z *= -1.0F;
					if ( (Temp.x <= ( half_sizex + OSize ) ) &&
						 (Temp.y <= ( half_sizey + OSize ) ) &&
						 (Temp.z <= ( half_sizez + OSize ) ) )
					{
						SizeX2 = OSize * OSize;
						Size = 1 / SizeX2;
						rlf	= XLightPnt->r; 
						glf	= XLightPnt->g; 
						blf	= XLightPnt->b;
						if(!d3dapp->CurrDriver || PowerVR ) // is it ramp mode..Or PowerVr Card...
						{
							rlf = ( rlf+glf+blf ) * 0.33333F;
							glf = rlf;
							blf = glf;
						}
						if( XLightPnt->Type == SPOT_LIGHT )
						{
							Dirx = XLightPnt->Dir.x;
							Diry = XLightPnt->Dir.y;
							Dirz = XLightPnt->Dir.z;
							CosArc = XLightPnt->CosArc;
						}
				
					
						CellIndex.x = Posx - Mloadheader->Group[group].cell_origin[execbuf].x;
						CellIndex.y = Posy - Mloadheader->Group[group].cell_origin[execbuf].y;
						CellIndex.z = Posz - Mloadheader->Group[group].cell_origin[execbuf].z;
						CellIndex_x = (int) floor( (CellIndex.x - OSize) * CellSize );
						CellIndex_y = (int) floor( (CellIndex.y - OSize) * CellSize );
						CellIndex_z = (int) floor( (CellIndex.z - OSize) * CellSize );
						CellRange_x = (int) floor( (CellIndex.x + OSize) * CellSize );
						CellRange_y = (int) floor( (CellIndex.y + OSize) * CellSize );
						CellRange_z = (int) floor( (CellIndex.z + OSize) * CellSize );
					
					
						if( CellIndex_x < 0 )
						{
							CellIndex_x = 0;
						}else{
							if( CellIndex_x >= NumOfxCells )
								CellIndex_x = NumOfxCells-1;
						}
						if( CellIndex_y < 0 )
						{
							CellIndex_y = 0;
						}else{
							if( CellIndex_y >= NumOfyCells )
								CellIndex_y = NumOfyCells-1;
						}
						if( CellIndex_z < 0 )
						{
							CellIndex_z = 0;
						}else{
							if( CellIndex_z >= NumOfzCells )
								CellIndex_z = NumOfzCells-1;
						}
					
						if( CellRange_x < 0 )
						{
							CellRange_x = 0;
						}else{
							if( CellRange_x >= NumOfxCells  )
								CellRange_x = NumOfxCells-1;
						}
						if( CellRange_y < 0 )
						{
							CellRange_y = 0;
						}else{
							if( CellRange_y >=NumOfyCells  )
								CellRange_y = NumOfyCells-1;
						}
						if( CellRange_z < 0 )
						{
							CellRange_z = 0;
						}else{
							if( CellRange_z >= NumOfzCells )
								CellRange_z = NumOfzCells-1;
						}
					
						switch( XLightPnt->Type )
						{
						case POINT_LIGHT:
							Cellz = CellIndex_z;
#ifdef	USEASM
							start_chop();
#endif	//USEASM
			
							while( Cellz <= CellRange_z )
							{			   
								Celly = CellIndex_y;
								while( Celly <= CellRange_y )
								{
									Cell = ( CellIndex_x + NumOfxCells *
										   ( Celly + NumOfyCells *
										     Cellz ) );
									Cellx = CellRange_x - CellIndex_x + 1;
									
									while( Cellx-- )
									{
										VertexIndexPnt = OrgVertexIndexPnt+VertexCellPnt[Cell].start_vert_in_cell;
										vert = VertexCellPnt[Cell].num_verts_in_cell;
			
										while( vert-- )
										{
//											NumOfVertsConsidered++;
											lpD3DLVERTEX = lpPointer + *VertexIndexPnt++;
											/* find the distance from vert to light */
											x = lpD3DLVERTEX->x;
											y = lpD3DLVERTEX->y;
											z = lpD3DLVERTEX->z;
											x -= Posx;
											y -= Posy;
											z -= Posz;
			
			
											distance = (x*x) + (y*y) + (z*z);
//											distance = 0.0F;			
											if ( distance <  SizeX2  )					// float
											{
												//											NumOfVertsTouched++;
												distance = 1.0F - ( distance * Size );	// float
#ifdef	USEASM
__asm
{
#if 1		
														fld		rlf				;float load rlf
														fmul	distance		;float mul distance 2
														fld		blf				;float load blf
														fmul	distance		;float mul distance	1
														fld		glf				;float load glf
														fmul	distance		;float mul distance	0
														fxch	st(2)
														fistp	tempiR			;float int store tempiR
														fxch	st(1)
														fistp	tempiG			;float int store tempiG
														fxch	st(0)
														fistp	tempiB			;float int store tempiG
			
 												mov	esi , [lpD3DLVERTEX];set up the pointer
												mov	ecx , [esi+16]		;int ecx = col   get the color

												mov edi , ecx			; edi = col..
												and ecx , 0x00ffffff	; and out the alpha
												and edi , 0xff000000	; keep the alpha for later
												
												mov ebx , tempiR		;move the red into inc
												mov edx , ecx			;edx = col
												shl ebx , 8				;shift it up 8 bits
												or  ebx , tempiG		;or in the green
												shl ebx , 8				;shift it up 8 bits
												or  ebx , tempiB		;or in the Blue
			
			
#else		
			
			
														fld rlf					;float load rlf        
														fmul distance			;float mul distance    
 												mov	esi , [lpD3DLVERTEX];set up the pointer
														fistp tempiR			;float int store tempiR
												mov	ecx , [esi+16]		;int ecx = col   get the color
														fld glf					;float load glf
														fmul distance			;float mul distance

												mov edi , ecx			; edi = col..
												and ecx , 0x00ffffff	; and out the alpha
												and edi , 0xff000000	; keep the alpha for later

												mov ebx , tempiR		;move the red into inc
														fistp	tempiG			;float int store tempiG
												shl ebx , 8				;shift it up 8 bits
														fld blf					;float load blf
														fmul distance			;float mul distance
												or  ebx , tempiG		;or in the green
												mov edx , ecx			;edx = col
														fistp	tempiB			;float int store tempiG
												shl ebx , 8				;shift it up 8 bits
												or  ebx , tempiB		;or in the Blue
#endif
												add ecx , ebx			;ecx = col + inc
												xor edx , ebx			;edx = col ^ inc
												xor edx , ecx			;edx = ( col + inc ) ^ ( col ^ inc )
												and edx	, 0x01010100	;edx = carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100
												mov eax , edx			;eax = carry
												shr	edx , 8				;edx = ( carry >> 8 )
												sub eax , edx			;eax = clamp = carry - ( carry >> 8 )
												sub ecx , edx			;ecx = (col + inc) - carry
												or  ecx , eax			;col = ecx | clamp
			
												or  ecx , edi			; or back in the alpha..

												mov [esi+16] , ecx		;int put the color back
												// carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
												// clamp = carry - ( carry >> 8 );
												// col = ( col + inc - carry ) | clamp;
											}

#else	//USEASM


#ifdef	USE_SPECULAR
												col = lpD3DLVERTEX->specular;
#else		
												col = lpD3DLVERTEX->color;
#endif		
#ifdef TESTING_SUBTRACTIVE
												tempiA = col & 0xff000000;
												// for subtraction, simply add the negative
												tempiR = ( (int) -( rlf * distance ) ) & 0xFF;
												tempiG = ( (int) -( glf * distance ) ) & 0xFF;
												tempiB = ( (int) -( blf * distance ) ) & 0xFF;
												// so far so much nearly the same...
												inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
												carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
												col = col + inc - carry;
												// set the carry also for those channels where we are subtracting 0 (this seems wrong!)
												// where 0 components are detected by inverting the inc
												// adding 1, and checking the carry out
												inc = inc ^ 0x00FFFFFF;
												carry |= ( ( inc + 0x00010101) ^ ( inc ^ 0x00010101 ) ) & 0x01010100;
												// ...and now it's nearly the same as for additive saturation
												clamp = carry - ( carry >> 8 );
												col = ( col & clamp ) & 0x00ffffff;
												col |= tempiA;
#else
												tempiA = col & 0xff000000;
												tempiR = (int) ( rlf * distance );
												tempiG = (int) ( glf * distance );
												tempiB = (int) ( blf * distance );
												inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
												carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
												clamp = carry - ( carry >> 8 );
												col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
												col |= tempiA;
#endif
			
#ifdef	USE_SPECULAR
												lpD3DLVERTEX->specular = col;
#else		
												lpD3DLVERTEX->color = col;
#endif		
			
#endif	//USEASM
											}
										}
			
										Cell++;
									}
									Celly++;
								}
								Cellz++;
							}
#ifdef USEASM
							end_chop();
#endif		
							break;
						case SPOT_LIGHT:
							Cellz = CellIndex_z;
							while( Cellz <= CellRange_z )
							{
								Celly = CellIndex_y;
								while( Celly <= CellRange_y )
								{
									Cell = (uint16) ( CellIndex_x + NumOfxCells *
										            ( Celly + NumOfyCells *
										              Cellz ) );
									Cellx = CellRange_x - CellIndex_x + 1;
									
									while( Cellx-- )
									{
										VertexIndexPnt = OrgVertexIndexPnt+VertexCellPnt[Cell].start_vert_in_cell;
										vert = VertexCellPnt[Cell].num_verts_in_cell;
										while( vert-- )
										{
//											NumOfVertsConsidered++;
//											lpD3DLVERTEX = ((LPD3DLVERTEX ) lpPointer) + *VertexIndexPnt++;
											lpD3DLVERTEX = lpPointer + *VertexIndexPnt++;
											/* find the distance from vert to light */
											x = lpD3DLVERTEX->x - Posx;
											y = lpD3DLVERTEX->y - Posy;
											z = lpD3DLVERTEX->z - Posz;
											//distance = (float) sqrt( (x*x) + (y*y) + (z*z)); 
											distance = (x*x) + (y*y) + (z*z); 
					
											if ( distance <  SizeX2  )
											{
//												NumOfVertsTouched++;
							
												if( distance > 0.0F )
												{
													distance = (float) sqrt( distance );
													rlen = 1.0F / distance;
													x *= rlen;
													y *= rlen;
													z *= rlen;
												}
												Cosa = ( ( x*Dirx ) + ( y*Diry ) + ( z*Dirz ) );
												
												if( distance > 0.5F * OSize )
												{
													if ( Cosa > CosArc )
													{
														// Lights Are Go....
														intense = ( ( OSize - distance ) / ( 0.75F * OSize ) ) * ( ( Cosa - CosArc ) / ( 1.0F - CosArc ) );
													}else{
														continue;
													}
												}else if ( distance > MIN_LIGHT_SIZE ) {
													float cosarc2;
					
													cosarc2 = CosArc * ( 1.0F - ( ( OSize * 0.5F - distance ) / ( OSize * 0.5F - MIN_LIGHT_SIZE ) ) );
													if ( Cosa > cosarc2 )
													{
														intense = ( ( OSize - distance ) / ( OSize - MIN_LIGHT_SIZE ) ) * ( ( Cosa - cosarc2 ) / ( 1.0F - cosarc2 ) );
													}else{
														continue;
													}
												}else {
													if ( Cosa > 0.0F )
													{
														intense = 1.0F;
													}else{
														intense = 1.0F + Cosa;
													}
												}
												
#ifdef	USE_SPECULAR
												col =  lpD3DLVERTEX->specular;			// int
#else		
												col =  lpD3DLVERTEX->color;				// int
#endif		

												tempiA = col & 0xff000000;
												tempiR = (int) ( rlf * intense );
												tempiG = (int) ( glf * intense );
												tempiB = (int) ( blf * intense );
												inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
												carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
												clamp = carry - ( carry >> 8 );
												col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
												col |= tempiA;
			
#ifdef	USE_SPECULAR
												lpD3DLVERTEX->specular = col;
#else		
												lpD3DLVERTEX->color = col;
#endif		
											}
										}
										Cell++;
									}
									Celly++;
								}
								Cellz++;
							}
							break;
					
						}
					}
				}
				XLightPnt = XLightPnt->NextVisible;
			}
		}
		/*	unlock the execute buffer	*/
		if ( Mloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
			return FALSE;
	}
	
	return TRUE;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Xlight Mxloadheader...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL	XLightMxloadHeader( MXLOADHEADER * MXloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix )
{
	XLIGHT * XLightPnt;
	D3DEXECUTEBUFFERDESC	debDesc;
	VECTOR	Temp;
	float	distance;
	int		group;
	int		execbuf;
	int		vert;
    LPD3DLVERTEX	lpPointer;

	LPD3DLVERTEX	lpD3DLVERTEX;
	LPD3DLVERTEX	lpD3DLVERTEX2;
	D3DCOLOR col;
	float	Size,OSize;
	float	SizeX2;
	float	x,y,z;
	float	Posx,Posy,Posz;
	float	Dirx,Diry,Dirz;
	float	Cosa,CosArc;
	float	rlen;
	float	intense;
	int	tempiR;
	int	tempiG;
	int	tempiB;
	int	tempiA;
	uint32 inc;
	uint32 carry;
	uint32 clamp;
	float	blf;
	float	glf;
	float	rlf;

	group = MXloadheader->num_groups;
	while( group--)
	{
	
		execbuf = MXloadheader->Group[group].num_execbufs;
		while( execbuf--)
		{
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			/*	lock the execute buffer	*/
			if ( MXloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( MXloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
				return FALSE ;
			lpPointer = (LPD3DLVERTEX) debDesc.lpData;
		
			lpD3DLVERTEX = lpPointer;
			vert = MXloadheader->Group[group].num_verts_per_execbuf[execbuf];
			lpD3DLVERTEX2 = MXloadheader->Group[group].org_vertpnt[execbuf];
			while( vert --)
			{
#ifdef	USE_SPECULAR
				lpD3DLVERTEX->specular = 0;
				lpD3DLVERTEX++;		
#else	
				lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
				lpD3DLVERTEX++;		
				lpD3DLVERTEX2++;		
#endif
			}
		

			XLightPnt = FirstLightVisible;
			while( XLightPnt )
			{
				{
					
					Posx = XLightPnt->Pos.x;
					Posy = XLightPnt->Pos.y;
					Posz = XLightPnt->Pos.z;
					OSize  = XLightPnt->Size;
		
					Temp.x = Posx - Pos->x;
					if( Temp.x < 0.0F )	Temp.x *= -1.0F;
					Temp.y = Posy - Pos->y;
					if( Temp.y < 0.0F )	Temp.y *= -1.0F;
					Temp.z = Posz - Pos->z;
					if( Temp.z < 0.0F )	Temp.z *= -1.0F;
					if ( (Temp.x <= ( Radius + OSize ) ) &&
						 (Temp.y <= ( Radius + OSize ) ) &&
						 (Temp.z <= ( Radius + OSize ) ) )
					{
						SizeX2 = OSize * OSize;
						Size = 1 / SizeX2;
						rlf	= XLightPnt->r; 
						glf	= XLightPnt->g; 
						blf	= XLightPnt->b;

						if(!d3dapp->CurrDriver || PowerVR ) // is it ramp mode..Or PowerVr Card...
						{
							rlf = ( rlf+glf+blf ) * 0.33333F;
							glf = rlf;
							blf = glf;
						}
						if( XLightPnt->Type == SPOT_LIGHT )
						{
							Dirx = XLightPnt->Dir.x;
							Diry = XLightPnt->Dir.y;
							Dirz = XLightPnt->Dir.z;
							CosArc = XLightPnt->CosArc;
						}
					
						lpD3DLVERTEX = lpPointer;
						vert = MXloadheader->Group[group].num_verts_per_execbuf[execbuf];
		
						switch( XLightPnt->Type )
						{
						case POINT_LIGHT:
		
							while( vert-- )
							{
								/* find the distance from vert to light */

								ApplyMatrix( Matrix, (VECTOR*) lpD3DLVERTEX, &Temp );
								
								x = Temp.x - Posx;
								y = Temp.y - Posy;
								z = Temp.z - Posz;
								distance = (x*x) + (y*y) + (z*z);
		
								if ( distance <  SizeX2  )					// float
								{
									distance = 1.0F - ( distance * Size );	// float
#ifdef	USE_SPECULAR
									col =  lpD3DLVERTEX->specular;			// int
#else
									col =  lpD3DLVERTEX->color;				// int
#endif		
									tempiA = col & 0xff000000;
									tempiR = (int) ( rlf * distance );
									tempiG = (int) ( glf * distance );
									tempiB = (int) ( blf * distance );
									inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
									carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
									clamp = carry - ( carry >> 8 );
									col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
									col |= tempiA;

#ifdef	USE_SPECULAR
									lpD3DLVERTEX->specular = col;
#else
									lpD3DLVERTEX->color = col;
#endif		
								}
								lpD3DLVERTEX++;
							}
							break;
						case SPOT_LIGHT:
							while( vert-- )
							{
								/* find the distance from vert to light */
								ApplyMatrix( Matrix, (VECTOR*) lpD3DLVERTEX, &Temp );

								x = Temp.x - Posx;
								y = Temp.y - Posy;
								z = Temp.z - Posz;

								distance = (x*x) + (y*y) + (z*z);
		
								if ( distance <  SizeX2  )
								{
									if( distance > 0.0F )
									{
										distance = (float) sqrt( distance );
										rlen = 1.0F / distance;
										x *= rlen;
										y *= rlen;
										z *= rlen;
									}
									Cosa = ( ( x*Dirx ) + ( y*Diry ) + ( z*Dirz ) );
									
									if( distance > 0.5F * OSize )
									{
										if ( Cosa > CosArc )
										{
											// Lights Are Go....
											intense = ( ( OSize - distance ) / ( 0.75F * OSize ) ) * ( ( Cosa - CosArc ) / ( 1.0F - CosArc ) );
										}else{
											goto PLOP;
										}
									}else if ( distance > MIN_LIGHT_SIZE ) {
										float cosarc2;
		
										cosarc2 = CosArc * ( 1.0F - ( ( OSize * 0.5F - distance ) / ( OSize * 0.5F - MIN_LIGHT_SIZE ) ) );
										if ( Cosa > cosarc2 )
										{
											intense = ( ( OSize - distance ) / ( OSize - MIN_LIGHT_SIZE ) ) * ( ( Cosa - cosarc2 ) / ( 1.0F - cosarc2 ) );
										}else{
											goto PLOP;
										}
									}else {
										if ( Cosa > 0.0F )
										{
											intense = 1.0F;
										}else{
											intense = 1.0F + Cosa;
										}
									}
									
#ifdef	USE_SPECULAR
									col =  lpD3DLVERTEX->specular;			// int
#else		
									col =  lpD3DLVERTEX->color;				// int
#endif		
									tempiA = col & 0xff000000;
									tempiR = (int) ( rlf * intense );
									tempiG = (int) ( glf * intense );
									tempiB = (int) ( blf * intense );
									inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
									carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
									clamp = carry - ( carry >> 8 );
									col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
									col |= tempiA;

#ifdef	USE_SPECULAR
									lpD3DLVERTEX->specular = col;
#else		
									lpD3DLVERTEX->color = col;
#endif		
								}
PLOP:
								lpD3DLVERTEX++;
							}
							break;
						}
					}
				}
				XLightPnt = XLightPnt->NextVisible;
			}
			/*	unlock the execute buffer	*/
			if ( MXloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( MXloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return FALSE;
		}
	}
	
	return TRUE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Xlight Mxloadheader...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL	XLightMxaloadHeader( MXALOADHEADER * MXloadheader , VECTOR * Pos , float Radius , MATRIX * Matrix )
{
	XLIGHT * XLightPnt;
	D3DEXECUTEBUFFERDESC	debDesc;
	VECTOR	Temp;
	float	distance;
	int		group;
	int		execbuf;
	int		vert;
    LPD3DLVERTEX	lpPointer;

	LPD3DLVERTEX	lpD3DLVERTEX;
	LPD3DLVERTEX	lpD3DLVERTEX2;
	D3DCOLOR col;
	float	Size,OSize;
	float	SizeX2;
	float	x,y,z;
	float	Posx,Posy,Posz;
	float	Dirx,Diry,Dirz;
	float	Cosa,CosArc;
	float	rlen;
	float	intense;
	int	tempiR;
	int	tempiG;
	int	tempiB;
	int	tempiA;
	uint32 inc;
	uint32 carry;
	uint32 clamp;
	float	blf;
	float	glf;
	float	rlf;

	group = MXloadheader->num_groups;
	while( group--)
	{
	
		execbuf = MXloadheader->Group[group].num_execbufs;
		while( execbuf--)
		{
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			/*	lock the execute buffer	*/
			if ( MXloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( MXloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
				return FALSE ;
			lpPointer = (LPD3DLVERTEX) debDesc.lpData;
		
			lpD3DLVERTEX = lpPointer;
			vert = MXloadheader->Group[group].num_verts_per_execbuf[execbuf];
			lpD3DLVERTEX2 = MXloadheader->Group[group].org_vertpnt[execbuf];
			while( vert --)
			{
#ifdef	USE_SPECULAR
				lpD3DLVERTEX->specular = 0;
				lpD3DLVERTEX++;		
#else	
				lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
				lpD3DLVERTEX++;		
				lpD3DLVERTEX2++;		
#endif
			}
		

			XLightPnt = FirstLightVisible;
			while( XLightPnt )
			{
				{
					
					Posx = XLightPnt->Pos.x;
					Posy = XLightPnt->Pos.y;
					Posz = XLightPnt->Pos.z;
					OSize  = XLightPnt->Size;
		
					Temp.x = Posx - Pos->x;
					if( Temp.x < 0.0F )	Temp.x *= -1.0F;
					Temp.y = Posy - Pos->y;
					if( Temp.y < 0.0F )	Temp.y *= -1.0F;
					Temp.z = Posz - Pos->z;
					if( Temp.z < 0.0F )	Temp.z *= -1.0F;
					if ( (Temp.x <= ( Radius + OSize ) ) &&
						 (Temp.y <= ( Radius + OSize ) ) &&
						 (Temp.z <= ( Radius + OSize ) ) )
					{
						SizeX2 = OSize * OSize;
						Size = 1 / SizeX2;
						rlf	= XLightPnt->r; 
						glf	= XLightPnt->g; 
						blf	= XLightPnt->b;

						if(!d3dapp->CurrDriver || PowerVR ) // is it ramp mode..Or PowerVr Card...
						{
							rlf = ( rlf+glf+blf ) * 0.33333F;
							glf = rlf;
							blf = glf;
						}
						if( XLightPnt->Type == SPOT_LIGHT )
						{
							Dirx = XLightPnt->Dir.x;
							Diry = XLightPnt->Dir.y;
							Dirz = XLightPnt->Dir.z;
							CosArc = XLightPnt->CosArc;
						}
					
						lpD3DLVERTEX = lpPointer;
						vert = MXloadheader->Group[group].num_verts_per_execbuf[execbuf];
		
						switch( XLightPnt->Type )
						{
						case POINT_LIGHT:
		
							while( vert-- )
							{
								/* find the distance from vert to light */

								ApplyMatrix( Matrix, (VECTOR*) lpD3DLVERTEX, &Temp );
								
								x = Temp.x - Posx;
								y = Temp.y - Posy;
								z = Temp.z - Posz;
								distance = (x*x) + (y*y) + (z*z);
		
								if ( distance <  SizeX2  )					// float
								{
									distance = 1.0F - ( distance * Size );	// float
#ifdef	USE_SPECULAR
									col =  lpD3DLVERTEX->specular;			// int
#else
									col =  lpD3DLVERTEX->color;				// int
#endif		
									tempiA = col & 0xff000000;
									tempiR = (int) ( rlf * distance );
									tempiG = (int) ( glf * distance );
									tempiB = (int) ( blf * distance );
									inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
									carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
									clamp = carry - ( carry >> 8 );
									col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
									col |= tempiA;

#ifdef	USE_SPECULAR
									lpD3DLVERTEX->specular = col;
#else
									lpD3DLVERTEX->color = col;
#endif		
								}
								lpD3DLVERTEX++;
							}
							break;
						case SPOT_LIGHT:
							while( vert-- )
							{
								/* find the distance from vert to light */
								ApplyMatrix( Matrix, (VECTOR*) lpD3DLVERTEX, &Temp );
								
								x = Temp.x - Posx;
								y = Temp.y - Posy;
								z = Temp.z - Posz;
								//distance = (float) sqrt( (x*x) + (y*y) + (z*z)); 
								distance = (x*x) + (y*y) + (z*z); 
		
								if ( distance <  SizeX2  )
								{
									if( distance > 0.0F )
									{
										distance = (float) sqrt( distance );
										rlen = 1.0F / distance;
										x *= rlen;
										y *= rlen;
										z *= rlen;
									}
									Cosa = ( ( x*Dirx ) + ( y*Diry ) + ( z*Dirz ) );
									
									if( distance > 0.5F * OSize )
									{
										if ( Cosa > CosArc )
										{
											// Lights Are Go....
											intense = ( ( OSize - distance ) / ( 0.75F * OSize ) ) * ( ( Cosa - CosArc ) / ( 1.0F - CosArc ) );
										}else{
											goto PLOP2;
										}
									}else if ( distance > MIN_LIGHT_SIZE ) {
										float cosarc2;
		
										cosarc2 = CosArc * ( 1.0F - ( ( OSize * 0.5F - distance ) / ( OSize * 0.5F - MIN_LIGHT_SIZE ) ) );
										if ( Cosa > cosarc2 )
										{
											intense = ( ( OSize - distance ) / ( OSize - MIN_LIGHT_SIZE ) ) * ( ( Cosa - cosarc2 ) / ( 1.0F - cosarc2 ) );
										}else{
											goto PLOP2;
										}
									}else {
										if ( Cosa > 0.0F )
										{
											intense = 1.0F;
										}else{
											intense = 1.0F + Cosa;
										}
									}
									
#ifdef	USE_SPECULAR
									col =  lpD3DLVERTEX->specular;			// int
#else		
									col =  lpD3DLVERTEX->color;				// int
#endif		
									tempiA = col & 0xff000000;
									tempiR = (int) ( rlf * intense );
									tempiG = (int) ( glf * intense );
									tempiB = (int) ( blf * intense );
									inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
									carry = ( ( col + inc ) ^ ( col ^ inc ) ) & 0x01010100;
									clamp = carry - ( carry >> 8 );
									col = ( ( col + inc - carry ) | clamp) & 0x00ffffff;
									col |= tempiA;

#ifdef	USE_SPECULAR
									lpD3DLVERTEX->specular = col;
#else		
									lpD3DLVERTEX->color = col;
#endif		
								}
PLOP2:
								lpD3DLVERTEX++;
							}
							break;
						}
					}
				}
				XLightPnt = XLightPnt->NextVisible;
			}
			/*	unlock the execute buffer	*/
			if ( MXloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( MXloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return FALSE;
		}
	}
	
	return TRUE;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set Xlight Mxloadheader...
					Set all the ->color valus to be the same....
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL	SetColorMXAloadHeader( MXALOADHEADER * MXAloadheader , D3DCOLOR Col )
{
    LPD3DLVERTEX	lpPointer;
	LPD3DLVERTEX	lpD3DLVERTEX;
	D3DEXECUTEBUFFERDESC	debDesc;
	int		group;
	int		execbuf;
	int		vert;
	group = MXAloadheader->num_groups;
	while( group--)
	{
		execbuf = MXAloadheader->Group[group].num_execbufs;
		while( execbuf--)
		{
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			/*	lock the execute buffer	*/
			if ( MXAloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( MXAloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
				return FALSE ;
			lpPointer = (LPD3DLVERTEX) debDesc.lpData;
		
			lpD3DLVERTEX = lpPointer;
			vert = MXAloadheader->Group[group].num_verts_per_execbuf[execbuf];
			while( vert --)
			{
				lpD3DLVERTEX->color = Col;
				lpD3DLVERTEX++;		
			}
			/*	unlock the execute buffer	*/
			if ( MXAloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( MXAloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return FALSE;
		}
	}
	return TRUE;
}

extern	int FontHeight;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Make all the Cell Ambient Colours..
	Input		:	MLOADHEADER * Mloadheader
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	CreateCellColours( MLOADHEADER * Mloadheader )
{
	uint16	group, total_groups;
	uint16	execbuf;
	VECTOR	Pos;

	D3DCOLOR * ColourPnt;
	uint16 * OrgVertexIndexPnt;
	float	CellSize;
	int		x,y,z;
	int		num_of_cells = 0;
	CellSize = 1.0F / Mloadheader->CellSize;
	group = Mloadheader->num_groups;
	total_groups = group;
	while( group--)
	{
		DrawLoadingBox( CurrentLoadingStep, total_groups - group, total_groups );
		//D3DAppShowBackBuffer(TRUE);
		//D3DAppClearScreenOnly();
		PrintInitViewStatus( MyGameStatus );
		Printuint16( group, 0,0,1);
		execbuf = 0;//Mloadheader->Group[group].num_execbufs;
//		while( execbuf--)
		{
			OrgVertexIndexPnt = Mloadheader->Group[group].vertex_index_pnt[execbuf];

			num_of_cells += Mloadheader->Group[group].numofcells[execbuf];

			ColourPnt = Mloadheader->Group[group].colour_cell_pnt[execbuf];

			Pos.z = Mloadheader->Group[group].cell_origin[execbuf].z + ( CellSize * 0.5F );
			for( z = 0 ; z < Mloadheader->Group[group].zcells[execbuf] ; z++ )
			{
				Pos.y = Mloadheader->Group[group].cell_origin[execbuf].y + ( CellSize * 0.5F );
				for( y = 0 ; y < Mloadheader->Group[group].ycells[execbuf] ; y++ )
				{	
					Pos.x = Mloadheader->Group[group].cell_origin[execbuf].x + ( CellSize * 0.5F );
					for( x = 0 ; x < Mloadheader->Group[group].xcells[execbuf] ; x++ )
					{
						*ColourPnt++ = WorkOutAverageLight( &Pos , Mloadheader , group , execbuf );
						Pos.x += CellSize;
					}
					Pos.y += CellSize;

				}
				Pos.z += CellSize;
			}
		}
	}
	DebugPrintf( "There are %d Lightcells in this level\n",num_of_cells );
	CurrentLoadingStep++;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Make all the Cell Ambient Colours..
	Input		:	MLOADHEADER * Mloadheader
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

D3DCOLOR WorkOutAverageLight( VECTOR * Pos , MLOADHEADER * Mloadheader , uint16 group , uint16 execbuf )
{
	float	r,g,b;
	float	R,G,B;
	D3DCOLOR Colour;
	LPD3DLVERTEX	lpD3DLVERTEX;
	float	CellSize;
	float	Distance;
	int		i;
	VECTOR	vPos;
	float	Weight;
	float	TotalWeight;

	r = g = b = 0.0F;
	TotalWeight = 0;

	CellSize = 1.0F / Mloadheader->CellSize;

	for ( execbuf = 0; execbuf < Mloadheader->Group[group].num_execbufs; execbuf++ )
	{
		lpD3DLVERTEX = Mloadheader->Group[group].org_vertpnt[execbuf];
		
		for( i = 0 ; i < Mloadheader->Group[group].num_verts_per_execbuf[execbuf] ; i++ )
		{
			vPos.x = lpD3DLVERTEX->x;
			vPos.y = lpD3DLVERTEX->y;
			vPos.z = lpD3DLVERTEX->z;
			
			Distance = DistanceVector2Vector( &vPos , Pos);
			Weight = CellSize / Distance;
			Weight *= Weight;
//			if ( Weight > 1.0F )
//				Weight = 1.0F;
			Colour = lpD3DLVERTEX->color;
			
			R = RGBA_GETRED(Colour) * Weight;
			G = RGBA_GETGREEN(Colour) * Weight;
			B = RGBA_GETBLUE(Colour) * Weight;
			
			
			r += R;
			g += G;
			b += B;
			TotalWeight += Weight;
			lpD3DLVERTEX++;
		}
	}

	r /= TotalWeight;
	g /= TotalWeight;
	b /= TotalWeight;
	if( r > 255 )
		r = 255;
	if( g > 255 )
		g = 255;
	if( b > 255 )
		b = 255;
	
	return RGBA_MAKE( (int) (255.0F - r) , (int) (255.0F - g) , (int) (255.0F - b) , 0 );
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find Nearest Cell Colour..
	Input		:	MLOADHEADER * Mloadheader
	Output		:	D3DCOLOR
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
D3DCOLOR FindNearestCellColour( MLOADHEADER * Mloadheader,VECTOR * Pos, uint16 group )
{
	D3DCOLOR Col;
	D3DCOLOR * colpnt;
	VECTOR	CellIndex;
	int		Cell;
	int		CellIndex_x;
	int		CellIndex_y;
	int		CellIndex_z;
	int		NumOfxCells;
	int		NumOfyCells;
	int		NumOfzCells;
	float	CellSize;
	int		r,g,b;

	
	r=g=b=0;
	CellSize = Mloadheader->CellSize;
	NumOfxCells = Mloadheader->Group[group].xcells[0];
	NumOfyCells = Mloadheader->Group[group].ycells[0];
	NumOfzCells = Mloadheader->Group[group].zcells[0];

	CellIndex.x = Pos->x - Mloadheader->Group[group].cell_origin[0].x;
	CellIndex.y = Pos->y - Mloadheader->Group[group].cell_origin[0].y;
	CellIndex.z = Pos->z - Mloadheader->Group[group].cell_origin[0].z;
	CellIndex_x = (int) floor( CellIndex.x * CellSize );
	CellIndex_y = (int) floor( CellIndex.y * CellSize );
	CellIndex_z = (int) floor( CellIndex.z * CellSize );
	if( CellIndex_x < 0 )
	{
		CellIndex_x = -1;
	}else{
		if( CellIndex_x >= NumOfxCells )
			CellIndex_x = -1;
	}
	if( CellIndex_y < 0 )
	{
		CellIndex_y = -1;
	}else{
		if( CellIndex_y >= NumOfyCells )
			CellIndex_y = -1;
	}
	if( CellIndex_z < 0 )
	{
		CellIndex_z = -1;
	}else{
		if( CellIndex_z >= NumOfzCells )
			CellIndex_z = -1;
	}
	
	if( CellIndex_x != -1 && CellIndex_y != -1 && CellIndex_z != -1 )
	{
		
		Cell = ( CellIndex_x + NumOfxCells *
			( CellIndex_y + NumOfyCells *
			CellIndex_z ) );
		
		colpnt = Mloadheader->Group[group].colour_cell_pnt[0];
		colpnt += Cell;
		Col = *colpnt;
		
		r = RGBA_GETRED(Col);
		g = RGBA_GETGREEN(Col);
		b = RGBA_GETBLUE(Col);
	}

	if( BrightShips && (MyGameStatus == STATUS_Normal) )
	{
		r >>= 1;
		g >>= 1;
		b >>= 1;

	}
	return RGBA_MAKE( r, g, b, 0 );
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Build a list of the lights that are currently potentially visible...
	Input		:	Group the Current camera is in..
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void BuildVisibleLightList( uint16 Group )
{
	int		light;
	XLIGHT * XLightPnt;

	light = FirstXLightUsed;
	FirstLightVisible = NULL;

	while( light != (uint16 ) -1 )
	{
		XLightPnt = &XLights[light];

		if( (Group == (uint16) -1) || (XLightPnt->Visible && VisibleOverlap( Group, XLightPnt->Group , NULL ) ) )
		{
			XLightPnt->NextVisible = FirstLightVisible;
			FirstLightVisible = XLightPnt;
		}
		light = XLights[light].Prev;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Save XLights Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SaveXLights( FILE * fp )
{
	int		i;
	uint16	TempIndex = (uint16) -1;

	if( fp )
	{
		fwrite( &FirstXLightUsed, sizeof( uint16 ), 1, fp );
		fwrite( &FirstXLightFree, sizeof( uint16 ), 1, fp );
		if( FirstLightVisible  ) fwrite( &FirstLightVisible->Index, sizeof( uint16 ), 1, fp );
		else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
		
		for( i = 0; i < MAXXLIGHTS; i++ )
		{
			fwrite( &XLights[ i ].Type, sizeof( int ), 1, fp );
			fwrite( &XLights[ i ].Visible, sizeof( BOOL ), 1, fp );
			fwrite( &XLights[ i ].r, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].g, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].b, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].Size, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].SizeCount, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].CosArc, sizeof( float ), 1, fp );
			fwrite( &XLights[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &XLights[ i ].Prev, sizeof( uint16 ), 1, fp );
			fwrite( &XLights[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &XLights[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &XLights[ i ].Group, sizeof( uint16 ), 1, fp );
			if( XLights[ i ].NextVisible ) fwrite( &XLights[ i ].NextVisible->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Load XLights Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * LoadXLights( FILE * fp )
{
	int		i;
	uint16	TempIndex;

	if( fp )
	{
		fread( &FirstXLightUsed, sizeof( uint16 ), 1, fp );
		fread( &FirstXLightFree, sizeof( uint16 ), 1, fp );
		fread( &TempIndex, sizeof( uint16 ), 1, fp );
		if( TempIndex != (uint16) -1 ) FirstLightVisible = &XLights[ TempIndex ];
		else FirstLightVisible = NULL;
		
		for( i = 0; i < MAXXLIGHTS; i++ )
		{
			fread( &XLights[ i ].Type, sizeof( int ), 1, fp );
			fread( &XLights[ i ].Visible, sizeof( BOOL ), 1, fp );
			fread( &XLights[ i ].r, sizeof( float ), 1, fp );
			fread( &XLights[ i ].g, sizeof( float ), 1, fp );
			fread( &XLights[ i ].b, sizeof( float ), 1, fp );
			fread( &XLights[ i ].Size, sizeof( float ), 1, fp );
			fread( &XLights[ i ].SizeCount, sizeof( float ), 1, fp );
			fread( &XLights[ i ].CosArc, sizeof( float ), 1, fp );
			fread( &XLights[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &XLights[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &XLights[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &XLights[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &XLights[ i ].Group, sizeof( uint16 ), 1, fp );
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) XLights[ i ].NextVisible = &XLights[ TempIndex ];
			else XLights[ i ].NextVisible = NULL;
		}
	}

	return( fp );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

