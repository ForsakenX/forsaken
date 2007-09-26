
#ifdef SOFTWARE_ENABLE // top most software enable



#define ZBUFFER
//#define NEWGOUR	// for gouraud remap code.


//#define INTERZ		// Interleaved ZBuffer.



#ifdef SOFTWARE_ENABLE

#define SPANS
extern void dofpu();
extern char CkClut[];
extern void SWRendView();

//void dofpu(){}


#include "colord.h"	// Select 32-level or 256-level 8-bit gouraud.
#include "cwdefine.h"

#define ZCLIPCLIP
#define MINOT 1





#ifdef ZBUFFER
	extern	long Zoff0, Zoff1, Zoff2, Zoff3;
#endif


#define		MAXRGB	(32<<16)

	extern void SetVTables();

/*
#define			CLAMPRGB 	if (polyptr->r0>MAXRGB) polyptr->r0=MAXRGB;\
							if (polyptr->r1>MAXRGB) polyptr->r1=MAXRGB;\
							if (polyptr->r2>MAXRGB) polyptr->r2=MAXRGB;\
							if (polyptr->g0>MAXRGB) polyptr->g0=MAXRGB;\
							if (polyptr->g1>MAXRGB) polyptr->g1=MAXRGB;\
							if (polyptr->g2>MAXRGB) polyptr->g2=MAXRGB;\
							if (polyptr->b0>MAXRGB) polyptr->b0=MAXRGB;\
							if (polyptr->b1>MAXRGB) polyptr->b1=MAXRGB;\
							if (polyptr->b2>MAXRGB) polyptr->b2=MAXRGB 

*/

#define CLAMPRGB //polyptr->r0 = polyptr->r0


/*
	===================================================================================================================


										Software version C file.

															   

			I will try to keep all software version stuff in this file from now on.

	


	===================================================================================================================
*/




// Externs, mainly taken from VISI.C

#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "mload.h"
#include "camera.h"
#include "visi.h"
#include <float.h>
#include "file.h"
#include "triggers.h"
#include "pickups.h"
#include "lines.h"
#include "main.h"
#include "goal.h"


#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

extern int outside_map;
extern	BOOL	DoClipping;
extern	CAMERA	CurrentCamera;

BOOL	RenderCurrentCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Externals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
extern	MATRIX			ProjMatrix;
extern	TLOADHEADER		Tloadheader;
extern	D3DMATRIX		proj;
extern	D3DMATRIX		view;
extern	D3DMATRIXHANDLE hProj;
extern	D3DMATRIXHANDLE hView;
extern	MCLOADHEADER	MCloadheader;
extern	BOOL			UsedStippledAlpha;

extern	DWORD			CurrentSrcBlend;
extern	DWORD			CurrentDestBlend;
extern	DWORD			CurrentTextureBlend;

extern	BSP_NODE *	OldCollideNode;

extern	BOOL			PowerVR;
extern	BOOL			bPolySort;
extern	uint16			GroupTris[ MAXGROUPS ];
extern	int				use_local_data;
extern	LINE			Lines[ MAXLINES ];
extern	char			LevelNames[MAXLEVELS][128];                        


extern char * ScreenTable[];
extern long Vmode;							//0==8bit, 1==16bit.
extern	D3DMATRIXHANDLE hWorld;


extern	D3DVIEWPORT	OldViewPort;
extern	D3DVIEWPORT	PresentViewPort;
extern	MATRIX	VisPolyMatrix;
extern	uint16	CurrentGroupVisible;						// the real group thats visible
extern	uint16	GroupsVisible[MAXGROUPS];
extern	uint16	GroupInVisibleList;							// where it is in the visibility list
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Globals...	
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#define MAXPORTVERTS (MAXPORTALSPERGROUP * MAXVERTSPERPORTAL)

#define	MAXGROUPSVISIBLE 16

#define GROUP2GROUP_OFFSET( G1, G2 )	( ( (G1) >> 5 ) + ( (G2) * MAXGROUPS / 32 ) )
#define GROUP2GROUP_MASK( G1, G2 )		( 1 << ( (G1) & 31 ) )



 
#define ZCLIP_NEAR 32.0F  //24.0F //32.0F
#define ZCLIP_NEARL 32 //24 //32













/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Chris Walsh's Code
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SoftwareVersion = FALSE;
char TransparentFlag=0;	// 1-Semitransparencies ON. Changed through renderstates.
DWORD	NumVertsToXform;		//CW
WORD SurfWidth, SurfHeight;	// Dimensions of current mode.
WORD SurfBpp;				// bits per pixel, current mode.
char DrawnYet = 0;
long LineWidth;			//CW
char *ScreenBase;		//CW
extern char * ScreenTable[];
extern long Vmode;							//0==8bit, 1==16bit.
extern	D3DMATRIXHANDLE hWorld;



long PortalLeft,PortalRight,PortalTop,PortalBot;

extern char Transparent;


VISPOLVERTEX XformedVerts[5000];	// Max for 5000 verts xformed, shouldn't 
									// overflow.
//VISPOLVERTEX XformedVerts2[5000];	// Max for 5000 verts xformed, shouldn't 
//									// overflow: ONLY USED FOR PROCESSVERTICES BLOCKS.
D3DTRANSFORMDATA CWXformData;		//cw
	
long MaxDotsConsidered=0;	//cw

 
typedef struct {
	char	* Next;								// ( 4) Next poly to draw (NULL==None).
	short	x0,x1,x2;	// -12 bytes per poly saved.
	short	y0,y1,y2;
	long	u0,u1,u2;	//long
	long	v0,v1,v2;	//long
	unsigned long r0,r1,r2;
	unsigned long g0,g1,g2;
	unsigned long b0,b1,b2;
	float z0,z1,z2;
	short Material;					// shrink.
	char stlevel;					// shrink.
	char channel;		//long					// Polytype, shrink.
} CWPOLYGON;	//26 longs = 26*4 = 3.5? cache lines.

CWPOLYGON ScanPoly;



			 
extern long SemiTRLevel;



CWPOLYGON ChrissPoly;

CWPOLYGON * polyptr;

extern char ScanType;
extern void DrawPolyFT4( CWPOLYGON * p);
extern long PolyType;


//#include "ot.h"	// CW: OTag include file...


extern void ClrOT();
extern void DrawOT();
  
#define MAX_DISTANCE 65536*2					// Up to 1024 polygon slots.
extern CWPOLYGON * ObjTable[ MAX_DISTANCE ];				// 4K object table, (1024 dists).
  
extern CWPOLYGON * PolygonHeap;	// Addr of next space to add poly to heap.
extern CWPOLYGON * ZBPolyHeap;	// Addr of next space to add poly to heap.
extern long Highestz;


char SemiTROn = 0;

long rendpitch;	// Bytes per line for ASM routines.
long screenheight;	// Height of screen in pixels.
long screenwidth;
long widthshl16;
float SurfWidthOver2;
float SurfHeightOver2;
float MinusSurfHeightOver2;

long Scanning;



/*
	======================================


		Execute an execute buffer.


	======================================

*/









extern long zRW;
long RenderingSmall, RenderingSmallXOff, RenderingSmallYOff;











/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Disp Visipoly Model
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
BOOL
DisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam ) 
{
	D3DMATRIX	Tempproj;
	D3DMATRIX	Tempview;
	VISGROUP *g;
	int	i;
	uint32 t;

	Tempproj = proj;
	Tempview = view;

	NumOfVertsConsidered= 0;
	NumOfVertsTouched= 0;


	OldViewPort.dwSize = sizeof(D3DVIEWPORT);
	d3dapp->lpD3DViewport->lpVtbl->GetViewport( d3dapp->lpD3DViewport , &OldViewPort );
	PresentViewPort = OldViewPort;

	GroupImIn = CurrentCamera.GroupImIn;

	if ( GroupImIn != (uint16) -1 )
	{
		DisplayBSPNode( OldCollideNode );

		CreatePortalExecList( Mloadheader, NumGroupsVisible );
		CreateSkinExecList( &MCloadheader, NumGroupsVisible );

		t = 0;
		for ( g = cam->visible.first_visible, i = 0; g; g = g->next_visible, i++ )
		{
		 	ClipGroup( &CurrentCamera, (uint16) g->group );
			CurrentGroupVisible = GroupsVisible[i];
			GroupInVisibleList = i;
			if ( XLight1Group(  Mloadheader, GroupsVisible[i] ) != TRUE  )
				return FALSE;
   			if ( ExecuteSingleGroupMloadHeader(  Mloadheader, (uint16) g->group ) != TRUE  )
					return FALSE;


			DispGroupTriggerAreas( g->group );
			ShowAllColZones( g->group );

			t += GroupTris[ g->group ];
		}
		// accumulate visibility stats
		VisiStats[ GroupImIn ].tsum += t;
		if ( VisiStats[ GroupImIn ].tmax < t )
			VisiStats[ GroupImIn ].tmax = t;
		if ( VisiStats[ GroupImIn ].tmin > t )
			VisiStats[ GroupImIn ].tmin = t;
		VisiStats[ GroupImIn ].visits++;
	}else{
		if ( ExecuteMloadHeader ( Mloadheader ) != TRUE)
			return FALSE;
	}

	d3dapp->lpD3DViewport->lpVtbl->SetViewport( d3dapp->lpD3DViewport , &OldViewPort );
	proj = Tempproj;
	view = Tempview;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hProj, &proj) != D3D_OK)
		return FALSE;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hView, &view) != D3D_OK)
		return FALSE;
	
	return TRUE;

			//executesinglegroupmloadheader

			if( Mloadheader->Group[group].exec_type[i]&HASTRANSPARENCIES )
			{
				if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
				AddTransExe( &Matrix , Mloadheader->Group[group].lpExBuf[i] , 0, (uint16) -1, group, Mloadheader->Group[ group ].num_verts_per_execbuf[i] );

			}else{
				if (d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, Mloadheader->Group[group].lpExBuf[i], d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED) != D3D_OK)
					return FALSE;
			}


}
*/



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Chris Walsh's Code
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
CWDisplayBackground( MLOADHEADER	* Mloadheader, CAMERA *cam ) 
{
	D3DMATRIX				Tempproj;
	D3DMATRIX				Tempview;
	//HRESULT					ddrval;			//CW
	VISGROUP				*g;
	int						i;
	int						execbuf;			//CW
	D3DEXECUTEBUFFERDESC	debDesc;	//CW
	char					Drawn;

	D3DMATRIX			Matrix;




	zRW = 0;		// Just Z write for background.
	// Here is where we do the setvtables for the stuff...............
	SetVTables();	// ================================================================================
	
	
	Tempproj = proj;
	Tempview = view;

				 
	// Now it can start looking at the execute buffer..

 


	OldViewPort.dwSize = sizeof(D3DVIEWPORT);
	d3dapp->lpD3DViewport->lpVtbl->GetViewport( d3dapp->lpD3DViewport , &OldViewPort );



	PresentViewPort = OldViewPort;

	GroupImIn = CurrentCamera.GroupImIn;

	if ( GroupImIn != (uint16) -1 )
	{
		
		for ( g = cam->visible.first_visible, i = 0; g; g = g->next_visible, i++ )
		{
	 	    ClipGroup( &CurrentCamera, (uint16) g->group );	// And set matrices.
	 		CurrentGroupVisible = GroupsVisible[i];
	 		GroupInVisibleList = i;

			if ( CaptureTheFlag || CTF )
				DisplayGoal( (uint16) g->group );

			if ( XLight1Group(  Mloadheader, CurrentGroupVisible ) != TRUE  )	return FALSE;
		   
			//if ( ExecuteSingleGroupMloadHeader(  Mloadheader, (uint16) g->group ) != TRUE  )
			//		return FALSE;

			for (execbuf=0; execbuf != Mloadheader->Group[GroupsVisible[i]].num_execbufs; execbuf++) //while (execbuf--)
			{

				if ( Mloadheader->Group[g->group].exec_type[execbuf]&HASTRANSPARENCIES)
				{
					if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
					AddTransExe( &Matrix , Mloadheader->Group[g->group].lpExBuf[execbuf] , 0, (uint16) -1, g->group, Mloadheader->Group[ g->group ].num_verts_per_execbuf[execbuf] );
						
				}
				else
				{
	 				CWExecute2( d3dappi.lpD3DDevice, Mloadheader->Group[GroupsVisible[i]].lpExBuf[execbuf], &PresentViewPort , D3DEXECUTE_CLIPPED );
				}
  			}


		}
	}
	else
	{
			// i DONT THINK I NEED THIS?????
				if ( ExecuteMloadHeader ( Mloadheader ) != TRUE)
					return FALSE;
	}



//	Transparent = 0;
//===========================


	d3dapp->lpD3DViewport->lpVtbl->SetViewport( d3dapp->lpD3DViewport , &OldViewPort );
	proj = Tempproj;
	view = Tempview;
	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hProj, &proj) != D3D_OK)
		return FALSE;

	if (d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hView, &view) != D3D_OK)
		return FALSE;


// With Z buffer stuff we need to flush out all the polys NOW and then get on with rendering the rest of the stuff..

// So that means set vtables, followed by a SWRendView();



	SWRendView();
//	Highestz = 1;				//cw: for next time round.

	d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );

	zRW = 1;		// All things from now need Z testing.
	
	return TRUE;
}





























//========================================      C    R     A     P       ==================================
//========================================      C    R     A     P       ==================================
//========================================      C    R     A     P       ==================================


#if 0


// executemloadheader

			   /*	if( Mloadheader->Group[GroupsVisible[i]].exec_type[i]&HASTRANSPARENCIES )
				{
					if (d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &Matrix) != D3D_OK) return FALSE;
					AddTransExe( &Matrix , Mloadheader->Group[GroupsVisible[i]].lpExBuf[i] , 0, (uint16) -1, GroupsVisible[i], Mloadheader->Group[ GroupsVisible[i] ].num_verts_per_execbuf[i] );
				   	
				}
				else*/
				{	

					//1. LOCK the execute buffer.
					memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
					debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

					if ( Mloadheader->Group[GroupsVisible[i]].lpExBuf[execbuf]->lpVtbl->Lock(
							Mloadheader->Group[GroupsVisible[i]].lpExBuf[execbuf],
							&debDesc ) != D3D_OK)	return FALSE ;

					//2. Get how many verts there are in this execute buffer.
					NumVertsToXform = Mloadheader->Group[GroupsVisible[i]].num_verts_per_execbuf[ execbuf ];

					//3. Set up a D3DTRANSFORMDATA structure...
						//CWXformData.dwSize = sizeof (D3DTRANSFORMDATA);	//???????????? WHY ?????????
						CWXformData.lpIn = debDesc.lpData;	// Verts IN.
						CWXformData.dwInSize = sizeof (D3DLVERTEX);
						CWXformData.lpOut = &XformedVerts[0];	// Verts OUT.
						//CWXformData.dwOutSize = sizeof( D3DLVERTEX );
			 			//CWXformData.dwClip = 0;
						CWXformData.dwClipIntersection = 0;
						//CWXformData.dwClipUnion = 0;


					//4. Transform vertices.
						MyVispolyTransformVertices( NumVertsToXform, &CWXformData );

			 			Drawn = 0;
					
						CWExecute( &debDesc, NumVertsToXform );	// Execute, using the xformed verts.

					//5. Unlock the execute buffer.

						if ( Mloadheader->Group[GroupsVisible[i]].lpExBuf[execbuf]->lpVtbl->Unlock(
							Mloadheader->Group[GroupsVisible[i]].lpExBuf[execbuf] ) != D3D_OK)	return FALSE;
//#endif



//#if 0
//=====================================
	if ( GroupImIn != (uint16) -1 )
	{
		for ( g = cam->visible.first_visible, i = 0; g; g = g->next_visible, i++ )
		{
		 	ClipGroup( &CurrentCamera, (uint16) g->group );
			CurrentGroupVisible = GroupsVisible[i];
			GroupInVisibleList = i;
			if ( XLight1Group(  Mloadheader, GroupsVisible[i] ) != TRUE  )
				return FALSE;

   			if ( ExecuteSingleGroupMloadHeader(  Mloadheader, (uint16) g->group ) != TRUE  )
					return FALSE;


			DispGroupTriggerAreas( g->group );
			ShowAllColZones( g->group );

			t += GroupTris[ g->group ];
		}
		// accumulate visibility stats
		VisiStats[ GroupImIn ].tsum += t;
		if ( VisiStats[ GroupImIn ].tmax < t )
			VisiStats[ GroupImIn ].tmax = t;
		if ( VisiStats[ GroupImIn ].tmin > t )
			VisiStats[ GroupImIn ].tmin = t;
		VisiStats[ GroupImIn ].visits++;
	}else{
		if ( ExecuteMloadHeader ( Mloadheader ) != TRUE)
			return FALSE;
	}


//=====================================




				}

#endif




//========================================      C    R     A     P       ==================================
//========================================      C    R     A     P       ==================================
//========================================      C    R     A     P       ==================================



































#ifdef ZBUFFER

short ZBuffer[640*500];	// 1/z buffer... 0-0xffff.
#endif



/*

	This routine LOCKS the back buffer, decides on the VMode (ie:8/16 bit).

  Then it renders to the back buffer.

*/



long  ZFlag;
float ZCLIPNEARF = 32.0F;

extern long zwrite0;
extern long zwrite1;
extern long zwrite2, zwrite2RW, zwrite2RWT;
extern long zwrite3;
extern long zwrite4;
extern long zwrite5;
extern long zwrite6;
extern long zwrite7;
extern long zwrite8;
extern long zwrite9;
extern long zwritea;
extern long zwriteb;
extern long zwritec;
extern long zwrited;
extern long zwritee;
extern long zwritef;
extern long zwrite10;
extern long zwrite11;
extern long zwrite12;


extern long zwriteeRW;
extern long zwritefRW;
extern long zwrite10RW;
extern long zwrite11RW;
extern long zwrite12RW;
extern long zwrite13RW;



extern long zwriteeRWT;
extern long zwritefRWT;
extern long zwrite10RWT;
extern long zwrite11RWT;
extern long zwrite12RWT;
extern long zwrite13RWT;




//pc16 semitr
extern long zwrite6MR;
extern long zwrite7MR;
extern long zwrite8MR;



extern long zread0, zread1;
extern long zread0MR, zread1MR;
extern long zread2MR, zread3MR;
extern long zread4MR, zread5MR;


extern long zwrite0RW, zwrite1RW,Ztest0RW, Ztest1RW;
extern long zwrite0RWT, zwrite1RWT,Ztest0RWT, Ztest1RWT;






extern long Ztest2RW, Ztest3RW;
extern long Ztest2RWT, Ztest3RWT;
extern long Ztest14RW, Ztest15RW;
extern long Ztest16RW, Ztest17RW;

extern long Ztest14RWT, Ztest15RWT;
extern long Ztest16RWT, Ztest17RWT;


extern Ztest6RW, Ztest7RW;
extern Ztest8RW, Ztest9RW;
extern ZtestaRW, ZtestbRW;


extern Ztest6RWT, Ztest7RWT;
extern Ztest8RWT, Ztest9RWT;
extern ZtestaRWT, ZtestbRWT;


extern ZtestcRW, ZtestdRW;
extern ZtesteRW, ZtestfRW;
extern Ztest10RW, Ztest11RW;



extern ZtestcRWT, ZtestdRWT;
extern ZtesteRWT, ZtestfRWT;
extern Ztest10RWT, Ztest11RWT;



extern Ztest12RW, Ztest13RW;
extern Ztest12RWT, Ztest13RWT;




extern long ztest4RW, ztest5RW;
extern long ztest4RWT, ztest5RWT;
extern long zwrite8RW, zwrite7RW, zwrite6RW;
extern long zwrite8RWT, zwrite7RWT, zwrite6RWT;

extern long zwrite9RW;
extern long zwriteaRW;
extern long zwritebRW;
extern long zwritecRW;
extern long zwritedRW;



extern long zwrite9RWT;
extern long zwriteaRWT;
extern long zwritebRWT;
extern long zwritecRWT;
extern long zwritedRWT;



extern long zwrite4RW, zwrite5RW, zwrite5RWT;


extern void ClrOT();

void CWAfterModeSwitch()
{
HRESULT ddrval;

	SetVTables();
	ddrval = d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL);


	ClrOT();
	
}

void SetVTables()
{



DDSURFACEDESC			BBddsd;	//CW		// Surfdesc of back-buffer.
long					t;					//CW
HRESULT ddrval;

/* FIRST WE LOCK THE BACK BUFFER */
	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);


// Unlock the cunt first.................
//	ddrval = d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL);

									 
	ddrval = d3dappi.lpBackBuffer->lpVtbl->Lock( d3dappi.lpBackBuffer, NULL, &BBddsd,0,NULL);
	//if (ddrval != D3D_OK ) Msg(" fuck that!");


	rendpitch = BBddsd.lPitch;

 	SurfWidth  = BBddsd.dwWidth;						// Get width.
	SurfHeight = BBddsd.dwHeight;						// Get height.

	if (RenderingSmall)
	{
		SurfWidth>>=2;
		SurfHeight>>=2;

	}


	screenwidth = SurfWidth-1; //BBddsd.dwWidth-1;	// eg:319 wide.
	widthshl16 = screenwidth<<16;

	screenheight = SurfHeight-1;

	SurfBpp    = BBddsd.ddpfPixelFormat.dwRGBBitCount;  // Get bits-per-pixel.

	ScreenBase = BBddsd.lpSurface;	// CW: get ptr to screen.

	if (SurfBpp==16) Vmode = 1; else Vmode = 0;	// Q: What about 24/32 bit modes???


	SurfWidthOver2 = SurfWidth/2;
	SurfHeightOver2 = SurfHeight/2;



//	ZFlag ^= 1;



// Set SMC for different Zbuffer modes.

/*
	if (!ZFlag)
	{
		__asm
		{
			push	eax
 			mov	byte ptr [Ztest0],0x7d		// jg
			mov	byte ptr [Ztest1],0x7c		// jle

 			//mov	byte ptr [ Zadd0+1 ],0xc6	// ddz add +
			//mov	byte ptr [ Zadd1+1 ],0xc6	// ddz add +
			//mov	byte ptr [ Zadd2 ],0x03		// ldz add +

 			pop		eax
		}
	}
	else
	{	
		__asm
		{
			push	eax
			mov	byte ptr [Ztest0],0x7e //7e		// jl
			mov	byte ptr [Ztest1],0x7f //7d		// jg

			//mov	byte ptr [ Zadd0+1 ],0xee	// ddz add -
			//mov	byte ptr [ Zadd1+1 ],0xee	// ddz add -
			//mov	byte ptr [ Zadd2 ],0x2b		// ldz add -


			pop		eax
		}
	}	

*/



// Also here,set the offset from the screen to the Zbuffer.

#ifdef ZBUFFER


//	#ifndef INTERZ
	__asm
	{
		push	eax
		push	ebx

		lea	eax,ZBuffer
		sub	eax,[ScreenBase]

							   
	// SMC....

	//	mov		[ Zoff0 + 2],eax
	//	mov		[ Zoff1 + 2],eax

	//	mov		[ Zoff2 + 2],eax
	//	inc		eax
	//	mov		[ Zoff3 + 2],eax


		cmp	eax,dword ptr [zwrite0+2]
		je	short Done_Smc

		mov	dword ptr [zwrite0+2],eax		// AFFN flat write dword.

		mov	dword ptr [zwrite2+2],eax		// AFFN gouraud 1 write dword.
		mov	dword ptr [zwrite3+3],eax		// AFFN gouraud 1 write single.


		mov	dword ptr [zwrite4+2],eax		// AFFN gouraud 3 write dword.
		mov	dword ptr [zwrite5+3],eax		// AFFN gouraud 3 write single.
		
		mov	dword ptr [zwrite6+2],eax		// PC16 flat write dword main 16s lp.
		mov	dword ptr [zwrite7+3],eax		// PC16 flat write word last15 16s lp.
		mov	dword ptr [zwrite8+3],eax		// PC16 flat write word last15 16s lp.
		mov	dword ptr [zwrite9+2],eax		// PC16 gouraud3 write dword main16s lp.
		mov	dword ptr [zwritea+2],eax		// PC16 gouraud3 write dword last15s lp.
		mov	dword ptr [zwriteb+3],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwritec+2],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwrited+3],eax		// PC16 gouraud3 write word last15 :: 1.



//pc16 semitr NEW
		
		mov	dword ptr [zwrite6MR+2],eax		// PC16 flat write dword main 16s lp.
		mov	dword ptr [zwrite7MR+3],eax		// PC16 flat write word last15 16s lp.
		mov	dword ptr [zwrite8MR+3],eax		// PC16 flat write word last15 16s lp.









		mov	dword ptr [zwritee+2],eax		// PC16 gouraud1 write dword 16's lp
		mov	dword ptr [zwritef+2],eax		// PC16 gouraud1 write dword last15's lp
		mov	dword ptr [zwrite10+2],eax		// PC16 gouraud1 write dword last15's lp


		mov	dword ptr [zwrite11+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite12+3],eax		// PC16 gouraud1 write Word last15's lp


		mov	dword ptr [zread0+2],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zread1+2],eax		// PC16 gouraud1 write Word last15's lp


// pc16 semitr.....

		mov	dword ptr [zread0MR+2],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zread1MR+2],eax		// PC16 gouraud1 write Word last15's lp

		mov	dword ptr [zread2MR+2],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zread3MR+2],eax		// PC16 gouraud1 write Word last15's lp

		mov	dword ptr [zread4MR+2],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zread5MR+2],eax		// PC16 gouraud1 write Word last15's lp



// affn gour1 r+w.........
// --------------------------
		mov	dword ptr [zwrite2RW+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite2RWT+3],eax		// PC16 gouraud1 write Word last15's lp






// Z read write routines.
		mov	ebx,eax

		sub	ebx,2												/// WHY ?????????????????  fuck knows.........
	 	mov	dword ptr [Ztest0RW+2],ebx		// word

	 	mov	dword ptr [Ztest0RWT+2],ebx		// word
		mov	dword ptr [Ztest1RW+2],ebx		// word
		mov	dword ptr [Ztest1RWT+2],ebx		// word

	 	mov	dword ptr [Ztest2RW+2],ebx		// word
	 	mov	dword ptr [Ztest3RW+2],ebx		// word


	 	mov	dword ptr [Ztest2RWT+2],ebx		// word
	 	mov	dword ptr [Ztest3RWT+2],ebx		// word



	 	mov	dword ptr [ztest4RW+2],ebx		// word
	 	mov	dword ptr [ztest5RW+2],ebx		// word

	 	mov	dword ptr [ztest4RWT+2],ebx		// word
	 	mov	dword ptr [ztest5RWT+2],ebx		// word






		mov	dword ptr [zwrite4RW+3],eax		// AFFN gouraud 3 write dword.
		mov	dword ptr [zwrite5RW+3],eax		// AFFN gouraud 3 write single.
		mov	dword ptr [zwrite5RWT+3],eax		// AFFN gouraud 3 write single.
		

// PC16 FLAT...

		mov	ebx,eax
	 //	add	ebx,2

		mov	dword ptr [zwrite6RW+3],eax		// AFFN gouraud 3 write dword.
		mov	dword ptr [zwrite7RW+3],eax		// AFFN gouraud 3 write single.
		mov	dword ptr [zwrite8RW+3],eax		// AFFN gouraud 3 write single.

		mov	dword ptr [zwrite6RWT+3],eax		// AFFN gouraud 3 write dword.
		mov	dword ptr [zwrite7RWT+3],eax		// AFFN gouraud 3 write single.
		mov	dword ptr [zwrite8RWT+3],eax		// AFFN gouraud 3 write single.


 
		mov	dword ptr [Ztest6RW+2],ebx
		mov	dword ptr [Ztest7RW+2],ebx

		mov	dword ptr [Ztest8RW+2],ebx
		mov	dword ptr [Ztest9RW+2],ebx

		mov	dword ptr [ZtestaRW+2],ebx
 		mov	dword ptr [ZtestbRW+2],ebx


 
		mov	dword ptr [Ztest6RWT+2],ebx
		mov	dword ptr [Ztest7RWT+2],ebx

		mov	dword ptr [Ztest8RWT+2],ebx
		mov	dword ptr [Ztest9RWT+2],ebx

		mov	dword ptr [ZtestaRWT+2],ebx
 		mov	dword ptr [ZtestbRWT+2],ebx



// pc16grou3
 





		sub	ebx,2




		
 		mov	dword ptr [Ztest12RW+2],ebx
 		mov	dword ptr [Ztest13RW+2],ebx

		mov	dword ptr [Ztest14RW+2],ebx
 		mov	dword ptr [Ztest15RW+2],ebx

		mov	dword ptr [Ztest16RW+2],ebx
 		mov	dword ptr [Ztest17RW+2],ebx



		
 		mov	dword ptr [Ztest12RWT+2],ebx
 		mov	dword ptr [Ztest13RWT+2],ebx
		mov	dword ptr [Ztest14RWT+2],ebx
 		mov	dword ptr [Ztest15RWT+2],ebx
		mov	dword ptr [Ztest16RWT+2],ebx
 		mov	dword ptr [Ztest17RWT+2],ebx




		mov	dword ptr [ZtestcRW+2],ebx
		mov	dword ptr [ZtestdRW+2],ebx
		mov	dword ptr [ZtesteRW+2],ebx
		mov	dword ptr [ZtestfRW+2],ebx
		mov	dword ptr [Ztest10RW+2],ebx
		mov	dword ptr [Ztest11RW+2],ebx



		mov	dword ptr [ZtestcRWT+2],ebx
		mov	dword ptr [ZtestdRWT+2],ebx
		mov	dword ptr [ZtesteRWT+2],ebx
		mov	dword ptr [ZtestfRWT+2],ebx
		mov	dword ptr [Ztest10RWT+2],ebx
		mov	dword ptr [Ztest11RWT+2],ebx




 
// PC16 GOUR1...........
		mov	dword ptr [zwriteeRW+2],eax		// PC16 gouraud1 write dword 16's lp
		mov	dword ptr [zwritefRW+2],eax		// PC16 gouraud1 write dword last15's lp
		mov	dword ptr [zwrite10RW+2],eax		// PC16 gouraud1 write dword last15's lp
		mov	dword ptr [zwrite11RW+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite12RW+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite13RW+3],eax		// PC16 gouraud1 write Word last15's lp

		mov	dword ptr [zwriteeRWT+2],eax		// PC16 gouraud1 write dword 16's lp
		mov	dword ptr [zwritefRWT+2],eax		// PC16 gouraud1 write dword last15's lp
		mov	dword ptr [zwrite10RWT+2],eax		// PC16 gouraud1 write dword last15's lp
		mov	dword ptr [zwrite11RWT+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite12RWT+3],eax		// PC16 gouraud1 write Word last15's lp
		mov	dword ptr [zwrite13RWT+3],eax		// PC16 gouraud1 write Word last15's lp




// PC16 GOUR3...........
 		mov	dword ptr [zwrite9RW+3],eax		// PC16 gouraud3 write dword main16s lp.
		mov	dword ptr [zwriteaRW+2],eax		// PC16 gouraud3 write dword last15s lp.
		mov	dword ptr [zwritebRW+3],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwritecRW+3],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwritedRW+3],eax		// PC16 gouraud3 write word last15 :: 1.

 		mov	dword ptr [zwrite9RWT+3],eax		// PC16 gouraud3 write dword main16s lp.
		mov	dword ptr [zwriteaRWT+2],eax		// PC16 gouraud3 write dword last15s lp.
		mov	dword ptr [zwritebRWT+3],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwritecRWT+3],eax		// PC16 gouraud3 write word last15 :: 1.
		mov	dword ptr [zwritedRWT+3],eax		// PC16 gouraud3 write word last15 :: 1.



		




// sol16.asm
		//mov	dword ptr [zwrite0RW+2],eax		// dword
		mov	dword ptr [zwrite1RW+3],eax		// word
		mov	dword ptr [zwrite1RWT+3],eax		// word




		add	eax,4

	 

		mov	dword ptr [zwrite1+3],eax		// AFFN flat write single.
Done_Smc:
		pop		ebx
		pop		eax

	}
//	#endif


#endif


	if (RenderingSmall)
	{
		ScreenBase+= LineWidth*RenderingSmallYOff;
		ScreenBase+= RenderingSmallXOff * 2;

	}


	LineWidth = BBddsd.lPitch;									// lPitch should be ==320, but just in case...
	for (t=0; t!=SurfHeight; t++)
	{
		ScreenTable[t]=ScreenBase; ScreenBase+=LineWidth;		
		#ifdef INTERZ
			ScreenBase+=LineWidth;
		#endif
	}
	
										  
}







// OPT ON--------------->
#pragma optimize( "gty", on )



//    MyVispolyTransformVertices( PVPtr->wStart, PVPtr->wDest, PVPtr->dwCount, &CWXformData );

//void  MyVispolyTransformVertices( DWORD VertexCount, D3DTRANSFORMDATA * Data )
void  MyVispolyTransformVertices( long VSource, long VDest, DWORD VertexCount, D3DTRANSFORMDATA * Data )
{
	VERT			TempVert;	
	DWORD	i;

	MATRIX	WorldMatrix;
	MATRIX	TempMatrix;
	MATRIX	Temp2Matrix;
	VECTOR	Trans;
	VECTOR	Trans2;
	D3DMATRIX	Tempproj;
	D3DMATRIX	Tempview;
	float w;

   	D3DVIEWPORT CWViewPort;
	//float	OneOverZ;
	long		iz;	// Used to insert into OT list.

	D3DLVERTEX		*VertSrc;
	VISPOLVERTEX	*VertDest;
	float OneOverZ;
	DWORD Flags;
	float x,y,z;
	float	TopY,BotY,LeftX,RightX;





	Tempproj = proj;
	Tempview = view;

//	SurfWidthOver2 = (long)(SurfWidth/2);
//	SurfHeightOver2 = (long)(SurfHeight/2);



	VertSrc = (D3DLVERTEX *) (Data->lpIn);
	VertSrc += VSource;

	VertDest = (VISPOLVERTEX*) (Data->lpOut);
	VertDest += VDest;


	// Set portal clipping params.
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CWViewPort.dwSize = sizeof(D3DVIEWPORT);
	d3dapp->lpD3DViewport->lpVtbl->GetViewport( d3dapp->lpD3DViewport , &CWViewPort );

 

	if (RenderingSmall)
	{
		LeftX = 0;
		RightX = SurfWidth;
		TopY = 0;
		BotY = SurfHeight;

	}
	else
	{
		LeftX = floor( ( (float) CWViewPort.dwX / d3dapp->szClient.cx ) * SurfWidth );
		TopY = floor( ( (float) CWViewPort.dwY / d3dapp->szClient.cy ) * SurfHeight );
		RightX = ceil( ( ( (float) CWViewPort.dwX + CWViewPort.dwWidth ) / d3dapp->szClient.cx ) * SurfWidth );
		BotY = ceil( ( ( (float) CWViewPort.dwY + CWViewPort.dwHeight ) / d3dapp->szClient.cy ) * SurfHeight );
	}


// Setup vispolymatrix.

	//cw: commented out wed 25/3/98:     PresentViewPort = OldViewPort;

    PresentViewPort = OldViewPort;

	Trans.x = -CurrentCamera.Pos.x;
	Trans.y = -CurrentCamera.Pos.y;
	Trans.z = -CurrentCamera.Pos.z;

	ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );			/* Set Translation */

	TempMatrix  = CurrentCamera.Mat;


	TempMatrix._41 = Trans2.x;
	TempMatrix._42 = Trans2.y;												 
	TempMatrix._43 = Trans2.z;

	d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hWorld, &WorldMatrix);

  //	d3dappi.lpD3DDevice->lpVtbl->GetMatrix(d3dappi.lpD3DDevice, hProj, &ProjMatrix);





	MatrixMultiply( &WorldMatrix, &TempMatrix, &Temp2Matrix );
	MatrixMultiply( &Temp2Matrix, &ProjMatrix, &VisPolyMatrix );





	MinusSurfHeightOver2 = -SurfHeightOver2;




	dofpu();	// 24-bit (Low-precision) mode.


//========================= optimise this bit ====================

#if 1		  
			i = VertexCount;


			__asm
			{
				pushad

				mov		esi,[ VertSrc ]
				mov		edi,[ VertDest ]
				mov		ecx,[ i ]


EachVert:
				lea		eax,[ VisPolyMatrix ]

// Calc Z first...

				fld		[esi]D3DLVERTEX.x			// x
				fmul	[ eax ]MATRIX._14			// _14*x
				fld		[esi]D3DLVERTEX.y 			// y     _14*x
				fmul	[ eax ]MATRIX._24			// _24*y _14*x
				fld		[esi]D3DLVERTEX.z 			// z     _21*y _11*x
				fmul	[ eax ]MATRIX._34			// _34*z _24*y _14*x
		
				fld		[ eax ]MATRIX._44			// m44   _34z  _24y  _14x
				fadd	st,st(3)
				fxch	st(1)
				fadd	st,st(2)
				fxch	st(1)	
				fadd	st,st(1)
				

			//	fadd	st,st(3)
			//	fadd	st,st(2)
			//	fadd	st,st(1)
			

				fxch	st(3)
				//fstp	[ edi ]VISPOLVERTEX.z	// _34z  _24y  _14x
			
				fcompp								// clear 2 fp regs.
				fstp	st							// Z


// calc wx,wy....

				fld		[esi]D3DLVERTEX.x			// x
				fmul	[ eax ]MATRIX._11			// _11*x
				fld		[esi]D3DLVERTEX.y 			// y     _11*x
				fmul	[ eax ]MATRIX._21			// _21*y _11*x
				fld		[esi]D3DLVERTEX.z 			// z     _21*y _11*x
				fmul	[ eax ]MATRIX._31			// _31*z _21*y _11*x

				fld		[esi]D3DLVERTEX.x			// x     _31*z _21*y _11*x
				fmul	[ eax ]MATRIX._12			// _12*x _31*z _21*y _11*x
				fld		[esi]D3DLVERTEX.y			// y     _12*x _31*z _21*y _11*x
				fmul	[ eax ]MATRIX._22			// _22*y _12*x _31*z _21*y _11*x
				fld		[esi]D3DLVERTEX.z			// z     _22*y _12*x _31*z _21*y _11*x
				fmul	[ eax ]MATRIX._32			// _32*z _22*y _12*x _31*z _21*y _11*x

				fxch	st(5)  						// _11*x _22*y _12*x _31*z _21*y _32*z
				fadd	st,st(4)  					// 
				fadd	st,st(3)  					// X     _22*y _12*x _31*z _21*y _32*z
				fadd	[ eax ]MATRIX._41

				fxch	st(2)  						// _12*x _22*y X     _31*z _21*y _32*z

				fadd	st,st(1)
				fadd	st,st(5)  					// Y     _22*y X _31*z _21*y _32*z
				fadd	[ eax ]MATRIX._42

				
				fxch	st(5)						// _32*z _22*y X _31*z _21*y Y
			
				
		// clear crap...
				fcompp								// clear 2 fp regs.
				fstp	[ edi ]VISPOLVERTEX.wx				
				fcompp								// clear 2 fp regs.
				fstp	[ edi ]VISPOLVERTEX.wy				

// Now TOS is Z....


	// if vertdest->z < ZCLIP_NEAR
			//
				fcom	[ ZCLIPNEARF ]
				fnstsw	ax
				fstp	[edi]VISPOLVERTEX.z
				and	ah,41h
				jz SHORT InView

// Behind player, just set flags, then onto next....
				mov	eax,[Data]
				mov		[edi]VISPOLVERTEX.dwFlags, D3DCLIP_FRONT

// now data->ClipIntersection &= ebx;

				mov		edx,[eax]D3DTRANSFORMDATA.dwClipIntersection
				and		edx,D3DCLIP_FRONT
				mov		[eax]D3DTRANSFORMDATA.dwClipIntersection,edx
				jmp		SHORT NextVert

InView:
// On screen: Calc the X and Y screen coords.
				fld1
				fdiv	[ edi ]VISPOLVERTEX.z		// 1/z


	;; !!!! Massive delay !!!!!!

// x = SurfWidthOver2* VertDest->wx * OneOverZ  + SurfWidthOver2

				fld 	[ SurfWidthOver2 ]			//sw/2             1/z
				fmul	[edi]VISPOLVERTEX.wx		//sw/2*Vd->wx      1/z
				fld 	[ SurfWidthOver2 ]			//sw/2             sw/2*vd->wx      1/z
				fld		st(2)	 					//1/z              sw/2             sw/2*vd->wx    1/z
				fmul	st,st(2)					//1/z*sw/2*vd->wx  sw/2             sw/2*vd->wx    1/z
				fxch	st(2)	
				fstp	st							//sw/2             1/z*sw/2*vd->wx  1/z

				
  				fld 	[MinusSurfHeightOver2]		//-h/2             sw/2             1/z*sw/2*vd->wx  1/z
				fmul	[edi]VISPOLVERTEX.wy		//-h/2*Vd->wy      sw/2             1/z*sw/2*vd->wx  1/z
				fld 	[ SurfHeightOver2 ]	 		//h/2              -h/2*Vd->wy      sw/2             1/z*sw/2*vd->wx      1/z
				fxch	st(4)						//1/z              -h/2*Vd->wy      sw/2             1/z*sw/2*vd->wx      h/2
				fmul	st,st(1)					//1/z*-h/2*Vd->wy  -h/2*Vd->wy      sw/2             1/z*sw/2*vd->wx      h/2
				fxch	st(2)						//sw/2             -h/2*Vd->wy      1/z*-h/2*Vd->wy  1/z*sw/2*vd->wx      h/2

				fadd	st,st(3)					//X                -h/2*Vd->wy      1/z*-h/2*Vd->wy  1/z*sw/2*vd->wx      h/2

				fxch	st(2)  						//1/z*-h/2*Vd->wy  -h/2*Vd->wy      X				 1/z*sw/2*vd->wx      h/2
				fadd	st,st(4)  					//Y                -h/2*Vd->wy      X				 1/z*sw/2*vd->wx      h/2
				fxch	st(2)						//X                -h/2*Vd->wy      Y				 1/z*sw/2*vd->wx      h/2

	//			fstp	[edi]VISPOLVERTEX.x			//X KAK y KAK KAK
	//			fstp	st
	//			fstp	[edi]VISPOLVERTEX.y
	//			fcompp
			
		   //		fxch	st(1)						// kak x y kak kak
		   //		fstp	st						// x y kak kak
		   //		fxch	st(2)					// kak y x kak
		   //		fstp	st
		   //		fxch	st(2)					// kak x y
		   //		fstp	st

					fxch	st(3)					// kak kak y x kak
					fcompp
					fxch	st(2)
					fstp	st						// x y

				
				

				
// Now check for Left/Right/Top/Bot and set flags.....

			  //	fld		[ edi ]VISPOLVERTEX.y
			  //	fld		[ edi ]VISPOLVERTEX.x

				fcom	[ LeftX ]
				xor	ebx,ebx	//mov		ebx,0						// Flags = 0;
				fnstsw	ax
				and		ah,41h
				jz		SHORT CheckRight

				or		ebx,D3DCLIP_LEFT
				jmp		SHORT CheckY

CheckRight:		fcom	[ RightX ]
				fnstsw	ax
				and		ah,41h
				jnz		SHORT CheckY
	
				or		ebx,D3DCLIP_RIGHT
	

CheckY:			fxch	st(1)					// y x
				fcom	[ TopY ]
				fnstsw	ax
				and		ah,41h
				jz		SHORT CheckBot

				or		ebx,D3DCLIP_TOP
				jmp		SHORT WriteFlags
			
CheckBot:		fcom	[ BotY ]
				fnstsw	ax
				and		ah,41h
				jnz		SHORT WriteFlags

				or		ebx,D3DCLIP_BOTTOM

NotInView:
WriteFlags:
				mov	eax,[Data]

				mov		[edi]VISPOLVERTEX.dwFlags,ebx
				fstp	[edi]VISPOLVERTEX.y
				fstp	[edi]VISPOLVERTEX.x

// now data->ClipIntersection &= ebx;
				mov		edx,[eax]D3DTRANSFORMDATA.dwClipIntersection
				and		edx,ebx
				mov		[eax]D3DTRANSFORMDATA.dwClipIntersection,edx

NextVert:
				add		esi,TYPE D3DLVERTEX
				add		edi,TYPE VISPOLVERTEX

				dec		ecx
				test	ecx,ecx
				jns		EachVert


				popad


			}

}
#endif



#pragma optimize( "", off )


/*
	================================================================



	Assuming here that	D3DEXECUTE_UNCLIPPED means no xforms.



	================================================================


  */

#pragma optimize( "gty", on )


void  MyVispolyCopyVerts( long VSource, long VDest, DWORD VertexCount, D3DTRANSFORMDATA * Data )
//void  MyVispolyCopyVerts( DWORD VertexCount, D3DTRANSFORMDATA * Data )
{
	D3DLVERTEX		*VertSrc;
	VISPOLVERTEX	*VertDest;
	VERT			TempVert;	
	DWORD	i;
	DWORD	Flags;

	MATRIX	WorldMatrix;
	MATRIX	TempMatrix;
	MATRIX	Temp2Matrix;
	VECTOR	Trans;
	VECTOR	Trans2;
	D3DMATRIX	Tempproj;
	D3DMATRIX	Tempview;
   	D3DVIEWPORT CWViewPort;

	char charbuf[160];


	Tempproj = proj;
	Tempview = view;



//	sprintf( &charbuf[0],"copying verts unclipped, s=%d, d=%d, no=%d", VSource, VDest, VertexCount);
//	Msg(&charbuf[0]);


	VertSrc = (D3DLVERTEX *) Data->lpIn;
	VertSrc += VSource;

	VertDest = (VISPOLVERTEX*) Data->lpOut;
	VertDest += VDest;

	for( i = 0 ; i<VertexCount ; i++ )
	{



//		sprintf( &charbuf[0], "v%d = %f,%f", i,VertSrc->x, VertSrc->y );
 ///		Msg(&charbuf[0]);
		

	// !! Rewrite this in asm, PLEASE?

		//TempVert.x = VertSrc->x;
		//TempVert.y = VertSrc->y;
		//TempVert.z = VertSrc->z;

		VertDest->x = VertSrc->x;
		VertDest->y = VertSrc->y;
		VertDest->z = 33;

	  	VertDest->dwFlags = 0;
	  	VertSrc++;
		VertDest++;
	}
}




#pragma optimize( "", off )




// OPTIMIZE ON ------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>

#pragma optimize( "gty", on )

float WARPALLOWANCE = (1.0/8.0F);	//8.5
float WARPALLOWANCESTR = (1.0F);


long FSixtyFive = 65536; //65536;
float Half = 0.5F;
float BfCull;


















/*

	Remap the gouraud values to a new set depending on the GlBright parameter.

*/


long GBright=32;
long GRemap[256];

void	AdjustGouraudRemap()
{

#ifdef NEWGOUR
// We need to create a 256 byte table that says for each input gourad which output we get.
long n;
long i;


	

	for (n =0 ; n != 31 ; n++)
	{
		// for normal gouraud, i=n, lets just try a scaler on it for now.
			i = (n*GBright)/32; if (i>31) i=31;

			GRemap[ n ] = i<<16;
		
	}

#endif



}


void	CWExecute( D3DEXECUTEBUFFERDESC *debDesc, long NumVertsToXform, WORD cwFlags )
{


			   //		if (cwFlags==D3DEXECUTE_CLIPPED)
			   //			MyVispolyTransformVertices( execdata.dwVertexCount, &CWXformData );
			   //		else
			   //			MyVispolyCopyVerts( execdata.dwVertexCount, &CWXformData );



D3DLVERTEX	* VertBase;	// Used for uv lookup.
long		CurrentMat,Material;
long		ESrcAdd;			//cw
D3DTRIANGLE *TriPtr;
D3DSTATE	* StatePtr;
D3DBRANCH *		BranchPtr;
D3DPROCESSVERTICES *PVPtr;

long		n;
long		OffPlane;
unsigned	long bright;	// CW: temp for working out flat/gouraud values.
long		iz;	// Used to insert into OT list.
long		ZClip;
float		dz,newdz,ReconX,ReconY,ReconDelta,ReconU,ReconV,ReconX2,ReconY2,ReconU2,ReconV2;
float		ReconR,ReconG,ReconB;
float		ReconR2,ReconG2,ReconB2;
float		R0,G0,B0,R1,G1,B1,R2,G2,B2;
float		dx01,dy01,dz01,warp;
//#define		OFF_R 1
//#define		OFF_L 2
//#define		OFF_TOP 4
//#define		OFF_BOT 8
long		Clip,Clip2;
long		a;
long		st;
BOOL		Culling;	// TRUE - CCW culling on.
long		Trans;
long		NeedZB;

// Used during decoding verts..
// =============================
VISPOLVERTEX * XF0ptr;
VISPOLVERTEX * XF1ptr;
VISPOLVERTEX * XF2ptr;
D3DLVERTEX	* VB0ptr;
D3DLVERTEX	* VB1ptr;
D3DLVERTEX	* VB2ptr;
char		* ESrc;
long		NeedGour;


WORD		v0index;	//cw: temp. tri vertex indices.
WORD		v1index;

WORD		v2index;
WORD		* WordESrc;

WORD		DataUnits;
short		shite;

char		ECommand;
char		CommandSize;
char		NullTex;



//char		charbuf[160];



			Culling = TRUE;


  //		SurfWidthOver2  = (SurfWidth/2);
  //		SurfHeightOver2 = (SurfHeight/2);


					
			VertBase = (D3DLVERTEX *)debDesc->lpData;		// Base of verts for uv lookup.
				   
			ESrc = (char *) debDesc->lpData;					// Start of execute buffer.
			ESrc += (NumVertsToXform*sizeof(D3DLVERTEX));	// Jump over verts: pt to commands.






/*
	==================================================

		MAIN DECODE LOOP

	==================================================

*/

decode:				ECommand = *ESrc++;
					NeedZB = 0;
					NeedGour = 0;
			   

					if ( ECommand == D3DOP_PROCESSVERTICES )
					{

						
						//Msg("ProcessVertices");
						// show how many of the fuckers there are....

					 	CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc += 2;  				
					 	PVPtr = (D3DPROCESSVERTICES *) ESrc;

					 	for (n=0 ; n!=DataUnits; n++)
						{
						  	if ( PVPtr->dwFlags == D3DPROCESSVERTICES_COPY ) // == D3DEXECUTE_CLIPPED)
							{
						  		MyVispolyCopyVerts( PVPtr->wStart, PVPtr->wDest, PVPtr->dwCount, &CWXformData );
								Culling = FALSE;
							}
							else
								MyVispolyTransformVertices( PVPtr->wStart, PVPtr->wDest, PVPtr->dwCount, &CWXformData );

							PVPtr++;

						}

						if (DataUnits!=1)
						{
						  	ESrcAdd = DataUnits * sizeof( D3DPROCESSVERTICES ); //*DataUnits; //CommandSize;		  // Skip last triangle block.
						}
						else
						{
						  	ESrcAdd = sizeof( D3DPROCESSVERTICES ); //*DataUnits; //CommandSize;		  // Skip last triangle block.
						}


						ESrc += ESrcAdd;
						goto decode;

					}


					if ( ECommand == D3DOP_BRANCHFORWARD )
					{
					 	CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc+=2;  							//point at branch instruction.
					 	BranchPtr = (D3DBRANCH *) ESrc;
					  	ESrcAdd = sizeof( D3DBRANCH ); //*DataUnits		  // Skip last triangle block.
						ESrc += ESrcAdd;

			// EXIT if the dwOffset is 0....
						
				   	    if (BranchPtr->dwOffset==0 && (CWXformData.dwClipIntersection)) return;	// Exit, && all verts off && FALSE, then...

						if ((BranchPtr->dwOffset!=0) && (CWXformData.dwClipIntersection)) Msg("eh?");

						goto decode;


					}



					if (ECommand == D3DOP_STATERENDER)
					{
					 	CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc+=2;  						//point at triangle.
					
						// Check for it being a new material to use...
					 	StatePtr = (D3DSTATE *) ESrc;

						NullTex = 0;

					   	for ( n = 0 ; n != DataUnits ; n++)
						{
							if (StatePtr->drstRenderStateType == D3DRENDERSTATE_TEXTUREHANDLE)
							{
								CurrentMat = 0;
			

									if (StatePtr->dwArg[0] == 0) { NullTex = 1; } // Msg("Null texture");

			    				  	for (Material = 0; Material!=128; Material++)
									{
								   		if (Tloadheader.hTex[Material]==StatePtr->dwArg[0])
										{
											 CurrentMat = Material;		
											 goto gotmat;
										}
									}
									Msg(" Couldnt find texture!");
																
							}
							else
							if(StatePtr->drstRenderStateType == D3DRENDERSTATE_BLENDENABLE)
							{
								if (StatePtr->dwArg[0])	TransparentFlag = 1;
								else
														TransparentFlag = 0; //!!!!!!!!!!!!!!!!!!!!!!!! 0!!!!!!!!
							}
							else
							if(StatePtr->drstRenderStateType == D3DRENDERSTATE_CULLMODE)
							{
								if (StatePtr->dwArg[0]== D3DCULL_NONE) Culling = FALSE; //Msg("oi, culling switched off.");
								else
									if (StatePtr->dwArg[0]== D3DCULL_CW) Culling = TRUE; //Msg("oi, culling switched to CW.");
								else
									if (StatePtr->dwArg[0]== D3DCULL_CCW) Culling = TRUE; //Msg("oi, culling switched to CCW.");
						
							}
			gotmat:
						//CurrentMat = 1;

							StatePtr++;
						}
					  	ESrcAdd = DataUnits*CommandSize;		  // Skip last triangle block.
						ESrc += ESrcAdd;
						goto decode;
					}


					
					
					if (ECommand == D3DOP_EXIT) return; //goto DoneDecode;
				
					
					
					// Check for a triangle...
					if (ECommand ==  D3DOP_TRIANGLE)
					{
						CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;

						DataUnits = *WordESrc;
						ESrc+=2;  						//point at triangle.

					// Draw each tri..Addprim each time...
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						TriPtr = (D3DTRIANGLE *) ESrc;

					 	for (n=0;n!=DataUnits; n++)
						{
							// Check for FAN / STRIP flags..


						   	if ((TriPtr->wFlags & 31) == D3DTRIFLAG_START)
							{
						 		v0index = TriPtr->wV1;
								v1index = TriPtr->wV2;
								v2index = TriPtr->wV3;
								goto GotType;
							}

							if ((TriPtr->wFlags & 31) == D3DTRIFLAG_EVEN)
							{
								v1index = v2index;
								v2index = TriPtr->wV3;
								goto GotType;
							}

							if ((TriPtr->wFlags & 31) == D3DTRIFLAG_ODD)
							{
								v0index = v2index;
								v2index = TriPtr->wV3;
							}




GotType:
							if (TriPtr->wFlags & 32768 ) Trans = 64; else Trans=0;
							if (TriPtr->wFlags & 16384 ) NeedZB = 1; else NeedZB = 0;

							if (TriPtr->wFlags & 8192) NeedGour = 1; else NeedGour = 0;


					 	// !! Need to check here for dump using
						// the len in the exec buffer: sometimes whole
						// bunch of polys can be rejected...

						//if (culling)
						//{
						//	culling--;
						//	goto NextPoly;
						//}

 





// Set up some ptrs so I dont have to keep doing array[].this etc...


						XF0ptr = &XformedVerts[v0index];
						XF1ptr = &XformedVerts[v1index];
						XF2ptr = &XformedVerts[v2index];
						
						VB0ptr = VertBase+v0index;
						VB1ptr = VertBase+v1index;
						VB2ptr = VertBase+v2index;










							   
// Reject if all off 1 plane...


						if ( (XF0ptr->dwFlags) &   (XF1ptr->dwFlags) &  (XF2ptr->dwFlags) ) goto NextPoly;


						OffPlane =  (XF0ptr->dwFlags) |
								    (XF1ptr->dwFlags) |
								    (XF2ptr->dwFlags) ;


			   //			if ((OffPlane & D3DCLIP_FRONT)==0) goto NoZclipping;
			   			if (OffPlane & D3DCLIP_FRONT) goto DoTheZclipping;







/*
	=======================================================================================================================

								No ZClipping.


	========================================================================================================================
*/


NoZclipping:



//						if	( Culling && (
//							  (		(XF0ptr->x - XF1ptr->x) * (XF2ptr->y - XF1ptr->y) ) -
//							  (		(XF0ptr->y - XF1ptr->y) * (XF2ptr->x - XF1ptr->x) )
//								> 0))		goto NextPoly;



									
  						// Poly visible..						  					 



			//----------------------
						// Sort this out...


					if (Culling)
					{

							__asm
							{
								push	eax
								push	ebx
								push	ecx


								mov	eax, [ XF0ptr ]
								mov	ebx, [ XF1ptr ]
								mov	ecx, [ XF2ptr ]

					// Backface-cull...
					// ----------------
								fld		[ eax ]VISPOLVERTEX.x	// x0
								fsub	[ ebx ]VISPOLVERTEX.x	// x0-x1
								fld		[ ecx ]VISPOLVERTEX.y	// y2    x0-x1
								fsub	[ ebx ]VISPOLVERTEX.y	// y2-y1 x0-x1
								fld		[ eax ]VISPOLVERTEX.y	// y0    y2-y1 x0-x1
								fsub	[ ebx ]VISPOLVERTEX.y	// y0-y1 y2-y1 x0-x1
								fld		[ ecx ]VISPOLVERTEX.x	// x2    y0-y1 y2-y1 x0-x1
								fsub	[ ebx ]VISPOLVERTEX.x	// x2-x1 y0-y1 y2-y1 x0-x1

								fxch	st(3)					// x0-x1 y0-y1 y2-y1 x2-x1
								fmul	st,st(2)		 		// A     y0-y1 y2-y1 x2-x1

								fxch	st(1)					// y0-y1 A     y2-y1 x2-x1
								fmul	st,st(3)		 		// B     A     y2-y1 x2-x1

								fxch	st(3)					// x2-x1 A     y2-y1 B
								fstp	st						// A     y2-y1 B
	
								fxch	st(1)					// y2-y1 A     B
								fstp	st						// A     B

								fsub	st,st(1)					// A-B   B
								fxch	st(1)					// B     A-B
								fstp	st						// A-B
								fstp	[ BfCull ]

								pop		ecx
								pop		ebx
								pop		eax

							}

					 		if (BfCull>0) goto NextPoly;	// hehe, you SHITTY compiler...


					 }


		
						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else		
							polyptr = PolygonHeap++;	// Addr of this poly.

							
						if (NullTex)
						{

							polyptr->u0 = 0;
							polyptr->u1 = 0;
							polyptr->u2 = 0;
							polyptr->v0 = 0;
							polyptr->v1 = 0;
							polyptr->v2 = 0;
							polyptr->Material = 1;


						}
						else
						{
								


							__asm
							{
								push	eax
								push	ebx
								push	ecx
								push	edi


						  //		mov	eax, [ XF0ptr ]
						  //		mov	ebx, [ XF1ptr ]

						  //		mov	ecx, [ XF2ptr ]
								mov	edi,[ polyptr ]
	
						// XY coords: float->long...


								
						//		fld [ eax ]VISPOLVERTEX.x
								//fadd	[Half]
						//		fld	[ eax ]VISPOLVERTEX.y
								//fadd	[Half]

						//		fld [ ebx ]VISPOLVERTEX.x
								//fadd	[Half]

						//		fld	[ ebx ]VISPOLVERTEX.y
								//fadd	[Half]


						//		fld [ ecx ]VISPOLVERTEX.x

								//fadd	[Half]

						//		fld	[ ecx ]VISPOLVERTEX.y		// y2 x2 y1 x1 y0 x0
								//fadd	[Half]

						
					   //			fistp [edi]CWPOLYGON.y2
					   //	   		fistp [edi]CWPOLYGON.x2

					   //			fistp [edi]CWPOLYGON.y1
					   //	   		fistp [edi]CWPOLYGON.x1

								mov	eax,[ VB0ptr ]
								mov	ebx,[ VB1ptr ]
								mov	ecx,[ VB2ptr ]

					   //			fistp [edi]CWPOLYGON.y0
					   //	   		fistp [edi]CWPOLYGON.x0

						// Texture coords....
		
								fld	 [ eax ]D3DLVERTEX.tu			// tu0
								fimul [ FSixtyFive ]		 		// tu0*65k 
								fld  [ eax ]D3DLVERTEX.tv			// tv0 tu065k 
								fimul [ FSixtyFive ]		 		// tv065k tu065k 
								fld	 [ ebx ]D3DLVERTEX.tu			// tu1 tv065k tu065k 
								fimul [ FSixtyFive ]		 		// tu165k tv065k tu065k
								fld	 [ ebx ]D3DLVERTEX.tv			// tv1 tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tv165k tu165k tv065k tu065k
								fld	 [ ecx ]D3DLVERTEX.tu			// tu2 tv165k tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tu265k tv165k tu165k tv065k tu065k
								fld	 [ ecx ]D3DLVERTEX.tv			// tv2 tu265k tv165k tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tv265k tu265k tv165k tu165k tv065k tu065k

								fxch	st(5)						// tu065k tu265k tv165k tu165k tv065k tv265k
								fistp	[edi]CWPOLYGON.u0			// tu265k tv165k tu165k tv065k tv265k 
								fxch	st(3)						// tv065k tv165k tu165k tu265k tv265k 
								fistp	[edi]CWPOLYGON.v0			// tv165k tu165k tu265k tv265k
	
								mov	eax,[ CurrentMat]

								fistp	[edi]CWPOLYGON.v1
								shl		eax,6
								fistp	[edi]CWPOLYGON.u1
								fistp	[edi]CWPOLYGON.u2
								fistp	[edi]CWPOLYGON.v2
								
								mov		[ edi ]CWPOLYGON.Material,ax
								pop		edi
								pop		ecx
								pop		ebx
								pop		eax

							}

					}


							polyptr->x0 = (long)XF0ptr->x;
							polyptr->x1 = (long)XF1ptr->x;
							polyptr->x2 = (long)XF2ptr->x;

							polyptr->y0 = (long)XF0ptr->y;
							polyptr->y1 = (long)XF1ptr->y;
							polyptr->y2 = (long)XF2ptr->y;


							 // Test material.
							 // 	polyptr->Material = CurrentMat<<6; //64;

							// Test brightness for 8-bit.

							// If in RAMP and FLAT SHADED.

							  
							// If GOURAUD...

					  			{
											// SLOW BUT 100% ACCURATE.

											register unsigned long	c0,c1,c2;

											c0 = VB0ptr->color;
											c1 = VB1ptr->color;
											c2 = VB2ptr->color;


								// Red...

											polyptr->r0 = ( c0 >> 3 ) & 0x1f0000;
											polyptr->r1 = ( c1 >> 3 ) & 0x1f0000;
											polyptr->r2 = ( c2 >> 3 ) & 0x1f0000;

											polyptr->g0 = (c0<<5) & 0x1f0000; //bright<<16;
											polyptr->g1 = (c1<<5) & 0x1f0000; //bright<<16;
											polyptr->g2 = (c2<<5) & 0x1f0000; //bright<<16;

											polyptr->b0 = (c0<<13) & 0x1f0000; //bright<<16;
											polyptr->b1 = (c1<<13) & 0x1f0000; //bright<<16;
											polyptr->b2 = (c2<<13) & 0x1f0000; //bright<<16;

											polyptr->channel = 2;	// Gouraud.



									#ifdef NEWGOUR
											polyptr->r0 = GRemap[ (polyptr->r0) >> 16 ];
											polyptr->r1 = GRemap[ (polyptr->r1) >> 16 ];
											polyptr->r2 = GRemap[ (polyptr->r2) >> 16 ];

											polyptr->g0 = GRemap[ (polyptr->g0) >> 16 ];
											polyptr->g1 = GRemap[ (polyptr->g1) >> 16 ];
											polyptr->g2 = GRemap[ (polyptr->g2) >> 16 ];

											polyptr->b0 = GRemap[ (polyptr->b0) >> 16 ];
											polyptr->b1 = GRemap[ (polyptr->b1) >> 16 ];
											polyptr->b2 = GRemap[ (polyptr->b2) >> 16 ];
									#endif

								   			if (TransparentFlag)
											{
												 polyptr->channel = 3;	// Gouraud semitr.


												st =  (c0 & 0xff000000)>>24;
												polyptr->stlevel = st;
													
											}
								
												// Check for r0=g0=b0=r1=g1=b1=r2=g2=b2, ie: FLAT SHADED.
						
											bright = polyptr->r0;
									
								// What were doing here is checking if the color of each point is the same.


									   		if   ( polyptr->g0==bright && polyptr->b0==bright &&
									   				polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright &&
									   				polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright )

											//if (
											//	((c0&0xffffff) == (c1&0xffffff) ) &&
											//	((c1&0xffffff) == (c2&0xffffff)  )
											//	)
											{
												polyptr->channel = TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
											}
						
												


							// In AFFN semitr we must check also for a fade on the source graphic.
							// Easily done though, just adjust the palette it reads from...
							// ie: (Normal contribution * shade ) /32


					 			}
				#if 0
								else
								{


								// 8-bit
								// ========

														bright = (long) ((VertBase+v0index)->color)&0xff;
														polyptr->r0 = bright>>3; 

														bright =  (long)  	((VertBase+v1index)->color)&0xff;
														polyptr->r1 = bright>>3; 

														bright =  (long)  	((VertBase+v2index)->color)&0xff;
														polyptr->r2 = bright>>3; 


							  						  
													// Only do flat if theyre all the same.
													// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

														if (!( (polyptr->r0==polyptr->r1) && (polyptr->r0==polyptr->r2) ))
															goto needgour8;

												
														  
													// OK, so we dont need gouraud, work out the flat-tint to use.
													//	if (polyptr->r0 >=32-1) polyptr->r0 = 32-1;			
													//	if (polyptr->r0 <=1) polyptr->r0 = 1;

														polyptr->channel = TransparentFlag;
														goto DoAddPrim;

					needgour8:						// Oh dear, its in 8-bit and we need gouraud shading...
							 							polyptr->r0<<=16;	// Up to 31.
														polyptr->r1<<=16;
														polyptr->r2<<=16;


													if (TransparentFlag)
														polyptr->channel = 3;
													else
							  							polyptr->channel = 2; //2;	// 3=gouraudsemitr.
													// WORK OUT semitr level....



													if (TransparentFlag)
													{
														st =  (((VertBase+v0index)->color)>>24)&0xff;
														//st += (((VertBase+v1index)->color)>>24)&0xff;
														//st += (((VertBase+v2index)->color)>>24)&0xff;
														//st/=3;// OUCH!	0-255.
														
														polyptr->stlevel = st>>6;
													}

													// Lets make this Pc16 gouraud for now...
													
							  
								}
				#endif


//~~~~~~~~~~
// ADDPRIM
//~~~~~~~~~~
DoAddPrim:				   


							if (TransparentFlag)
								 iz = 1;
							else
								iz = (long)( (XF0ptr->z + XF1ptr->z + XF2ptr->z)*0.2); //333333);


							//iz >>= 1;	// Speedup??

							//if (iz<MINOT) iz=MINOT;

							//if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

											
	 						// OPTIONALLY SET PC16 if close enough.




								if (NeedGour)
								{
									if ((polyptr->channel==3)) polyptr->channel=2+64;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
								}
						
						  		if (iz<=1300)	// 1100 1300
								{


							  			if  (
												( XF0ptr->z == XF1ptr->z )
												&&
												( XF0ptr->z == XF2ptr->z )
											) goto Force_AFFN;
		

					// Now this is the good bit, here we only do AFFN if its warped enough.
					// This is just 6 muls at most to determine its PC16...


										if (TransparentFlag)
									
	
 										{					
										// Transparent.
										//==============
											float	z0,z1,z2;

												 
												z0 = 1.0F / XF0ptr->z;
												z1 = 1.0F / XF1ptr->z;


												dx01 = XF1ptr->x - XF0ptr->x;
												dy01 = XF1ptr->y - XF0ptr->y;
												dz01 = z1-z0; //(1.0F/XformedVerts[v1index].z) - (1.0F/XformedVerts[v0index].z);
												warp = dz01*dx01*dy01; //(dx01*dy01*dz01);
												if ( (warp > WARPALLOWANCESTR ) || (warp < -WARPALLOWANCESTR ) ) goto Force_PC16;

												z2 = 1.0F / XF2ptr->z;
					

												dx01 = XF2ptr->x - XF1ptr->x;
												dy01 = XF2ptr->y - XF1ptr->y;
												dz01 = z2-z1; //(1.0F/XformedVerts[v2index].z) - (1.0F/XformedVerts[v1index].z);
												warp = dz01*dx01*dy01; //(dx01*dy01*dz01);
												if ( ( warp > WARPALLOWANCESTR ) || (warp < -WARPALLOWANCESTR ) ) goto Force_PC16;

					
												dx01 = XF0ptr->x - XF2ptr->x;
												dy01 = XF0ptr->y - XF2ptr->y;
												dz01 = z0-z2; //(1.0F/XformedVerts[v0index].z) - (1.0F/XformedVerts[v2index].z);
												warp = dz01*dx01*dy01; //(dx01*dy01*dz01;
												if ( ( warp > WARPALLOWANCESTR ) || (warp < -WARPALLOWANCESTR ) ) goto Force_PC16;

										// Make it affn, but constant RGB.
												//polyptr->r0 >>=1;
												//polyptr->r1 >>=1;
												//polyptr->r2 >>=1;
										 		goto Force_AFFN;

										}
										else
										{	
										// Not transparent.
										// ================= 
												register float	z0,z1,z2;
		
												

												z0 = 1.0F / XF0ptr->z;
												z1 = 1.0F / XF1ptr->z;
												z2 = 1.0F / XF2ptr->z;


												dx01 = polyptr->x1 - polyptr->x0;
												dy01 = polyptr->y1 - polyptr->y0;
												dz01 = z1-z0; //(1.0F/XformedVerts[v1index].z) - (1.0F/XformedVerts[v0index].z);
												warp = dx01*dy01*dz01; //(dx01*dy01*dz01);
												if ( (warp > WARPALLOWANCE ) || (warp < -WARPALLOWANCE ) ) goto Force_PC16;

					
												dx01 = polyptr->x2 - polyptr->x1;
												dy01 = polyptr->y2 - polyptr->y1;
												dz01 = z2-z1; //(1.0F/XformedVerts[v2index].z) - (1.0F/XformedVerts[v1index].z);
												warp = dx01*dy01*dz01; //(dx01*dy01*dz01);
												if ( ( warp > WARPALLOWANCE ) || (warp < -WARPALLOWANCE ) ) goto Force_PC16;

					
												dx01 = polyptr->x0 - polyptr->x2;
												dy01 = polyptr->y0 - polyptr->y2;
												dz01 = z0-z2; //(1.0F/XformedVerts[v0index].z) - (1.0F/XformedVerts[v2index].z);
												warp = dx01*dy01*dz01; //(dx01*dy01*dz01;
												if ( ( warp > WARPALLOWANCE ) || (warp < -WARPALLOWANCE ) ) goto Force_PC16;
												goto Force_AFFN;
										}




Force_PC16:							polyptr->channel += 4; // PC16 gouraud.

								}


Force_AFFN:							polyptr->z0 = XF0ptr->z;
									polyptr->z1 = XF1ptr->z;
									polyptr->z2 = XF2ptr->z;




									polyptr->channel |= Trans;		// Set color key bit.






// Optimise this bit, ADDPRIM.............


/*
							__asm
							{
							
								push	eax
								mov		eax,[ iz ]

								shl		eax,2			// Dwords...
								push	ebx
																	  
								add		eax,offset ObjTable
								mov		ebx,[ polyptr ]

								push	ecx
							//
							
							//
							//
								mov		ecx,[ eax ]


								test	ecx,ecx
								jne	short NOTNULL
ISNULL:
								mov		[ ebx ]CWPOLYGON.Next,0
								mov		[ eax ],ebx
								jmp		short	AP_Exit



NOTNULL:						mov		[ ebx ]CWPOLYGON.Next, eax
								mov		[ eax ],ebx

AP_Exit:
								pop	ecx
								pop	ebx
								pop	eax
							}

*/


					if (!NeedZB)
					{
							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
		
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
					}
		




		//				goto NextPoly;
		
							TriPtr++; continue;

					








/*
		============================================================================================================


												 Handle ZCLIP


		============================================================================================================

*/

DoTheZclipping:

			if (TransparentFlag)
			{
				long c0;

				c0 = (VB0ptr->color);

				st =  (c0 & 0xff000000)>>24;
					
			}




// Basicaly, reconstruct the points to be on the z==0 plane.
// Then do a clip on the reconstructed points.

// Decide what the Zclip status are for 012.
			ZClip=0;
		  //	if (XformedVerts[v0index].dwFlags&D3DCLIP_FRONT) ZClip+=1;
		  //	if (XformedVerts[v1index].dwFlags&D3DCLIP_FRONT) ZClip+=2;
		  //	if (XformedVerts[v2index].dwFlags&D3DCLIP_FRONT) ZClip+=4;

		  	if (XF0ptr->dwFlags & D3DCLIP_FRONT) ZClip+=1;
		  	if (XF1ptr->dwFlags & D3DCLIP_FRONT) ZClip+=2;
		  	if (XF2ptr->dwFlags & D3DCLIP_FRONT) ZClip+=4;



		#if 0

			if (Vmode==0)
			{
			// 8-bit.
			// =======
			 	a= (VertBase+v0index)->color&0xff;
				a>>=3;
				a<<=16;
				R0 =  a;
				
				a= (VertBase+v1index)->color&0xff;
				a>>=3;
				a<<=16;
				R1 =  a;

				a= (VertBase+v2index)->color&0xff;
				a>>=3;
				a<<=16;
				R2 =  a;
			}
			else
		#endif

			{


			// 16-bit
			// =======


					// SLOW BUT 100% ACCURATE.

					register unsigned long	c0,c1,c2;

					c0 = VB0ptr->color;
					c1 = VB1ptr->color;
					c2 = VB2ptr->color;


					R0 = ( c0 >> 3 ) & 0x1f0000;
					R1 = ( c1 >> 3 ) & 0x1f0000;
					R2 = ( c2 >> 3 ) & 0x1f0000;

					G0 = (c0<<5) & 0x1f0000; //bright<<16;
					G1 = (c1<<5) & 0x1f0000; //bright<<16;
					G2 = (c2<<5) & 0x1f0000; //bright<<16;

					B0 = (c0<<13) & 0x1f0000; //bright<<16;
					B1 = (c1<<13) & 0x1f0000; //bright<<16;
					B2 = (c2<<13) & 0x1f0000; //bright<<16;
			}
 
									  

	   		switch(ZClip)
			{


		/*
			=====================================

						Case 0
		
			=====================================

		*/

			case 0:
						break;




		/*
			=====================================

						Case 1
		
			=====================================

		*/
			case 1:
				 		dz = XF1ptr->z - XF0ptr->z;
						newdz = XF1ptr->z-ZCLIP_NEAR;

						ReconX = XF0ptr->wx - XF1ptr->wx;
						ReconDelta = ReconX/dz;
						ReconX = XF1ptr->wx + (newdz*ReconDelta);
					   
						ReconY = XF0ptr->wy - XF1ptr->wy;
						ReconDelta = ReconY/dz;
						ReconY = XF1ptr->wy + (newdz*ReconDelta);
																		 
						ReconR = (R0-R1);
						ReconDelta = ReconR/dz;
						ReconR = newdz * ReconDelta;
						ReconR+=R1;

					//	if (Vmode)
					//	{
							ReconG = (G0-G1);
							ReconDelta = ReconG/dz;
							ReconG = newdz * ReconDelta;
							ReconG+=G1;

							ReconB = (B0-B1);
							ReconDelta = ReconB/dz;
							ReconB = newdz * ReconDelta;
							ReconB+=B1;
					//	}
					
						ReconU = VB0ptr->tu  -  VB1ptr->tu;
						ReconDelta = ReconU/dz;
						ReconU = VB1ptr->tu + (newdz*ReconDelta);

						ReconV = VB0ptr->tv  -  VB1ptr->tv;
						ReconDelta = ReconV/dz;
						ReconV = VB1ptr->tv + (newdz*ReconDelta);

					   
						// Project the new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);


						if (  (Culling &&		(	( ReconX - XF1ptr->x ) *	(XF2ptr->y - XF1ptr->y) ) >
							 		(	( ReconY - XF1ptr->y ) *	(XF2ptr->x - XF1ptr->x) )  ) )
									 goto NextPoly;	//continue;	 // conflict with break???


			
						Clip = 0;
						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;

						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;


				#ifdef ZCLIPCLIP

						if (	Clip &
								XF1ptr->dwFlags &
								XF2ptr->dwFlags  ) goto notp1;
				#endif
						 
						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else
							polyptr = PolygonHeap++;	// Addr of this poly.

						polyptr->x0 = ReconX; polyptr->y0 = ReconY; polyptr->z0 = ZCLIP_NEAR;
						polyptr->u0 = (long)((ReconU*65536.0F));polyptr->v0 = (long)((ReconV*65536.0F));	//reconu/v
						polyptr->r0 = (long)(ReconR);


						polyptr->x1 = XF1ptr->x; polyptr->y1 = XF1ptr->y; polyptr->z1 = XF1ptr->z;
						polyptr->u1 = (long)(( VB1ptr->tu*65536.0F)); polyptr->v1 = (long)(( VB1ptr->tv*65536.0F));
						polyptr->r1 = (long)R1;

						polyptr->x2 =  XF2ptr->x; polyptr->y2 = XF2ptr->y ; polyptr->z2 = XF2ptr->z;
						polyptr->u2 = (long)(( VB2ptr->tu*65536.0F)); polyptr->v2 = (long)(( VB2ptr->tv*65536.0F));
						polyptr->r2 = (long)R2;	

						polyptr->Material = CurrentMat*64;


				   //		if (Vmode)
				   //		{
							polyptr->g0 = (long)ReconG;
							polyptr->g1 = (long)G1;
							polyptr->g2 = (long)G2;	
							polyptr->b0 = (long)ReconB;
							polyptr->b1 = (long)B1;
							polyptr->b2 = (long)B2;	
							CLAMPRGB;
				   //		}


						



						iz = (long) (( ZCLIP_NEAR + XF1ptr->z + XF2ptr->z) *0.3333333F);
						if (iz<MINOT) iz=MINOT;
						if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;


						if (TransparentFlag) iz = 1;
		


	// Addprim..

						if (!NeedZB)
						{

							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
				
						}


 
						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

					 //		if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.

								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							if (TransparentFlag)	polyptr->stlevel = st;

							polyptr->channel |= Trans;		// Set color key bit.




//	NOW DO POLYGON r,r2,2...
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
notp1:
		
					  
						dz = XformedVerts[v2index].z - XformedVerts[v0index].z;
						newdz = XformedVerts[v2index].z-ZCLIP_NEAR;

						ReconX2 = XformedVerts[v0index].wx - XformedVerts[v2index].wx;
						ReconDelta = ReconX2/dz;
						ReconX2 = XformedVerts[v2index].wx + (newdz*ReconDelta);
					   
						ReconY2 = XformedVerts[v0index].wy - XformedVerts[v2index].wy;
						ReconDelta = ReconY2/dz;
						ReconY2 = XformedVerts[v2index].wy + (newdz*ReconDelta);

		
						ReconR2 = (R0 - R2);
						ReconDelta = ReconR2/dz;
						ReconR2 = newdz * ReconDelta;
						ReconR2+=R2;

					 //	if (Vmode)
					 //	{
							ReconG2 = (G0 - G2);
							ReconDelta = ReconG2/dz;
							ReconG2 = newdz * ReconDelta;
							ReconG2+=G2;

							ReconB2 = (B0 - B2);
							ReconDelta = ReconB2/dz;
							ReconB2 = newdz * ReconDelta;
							ReconB2+=B2;
					 //	}



						ReconU2 = (VertBase+v0index)->tu - (VertBase+v2index)->tu;
						ReconDelta = ReconU2/dz;
						ReconU2 = (VertBase+v2index)->tu + (newdz*ReconDelta);

						ReconV2 = (VertBase+v0index)->tv- (VertBase+v2index)->tv;
						ReconDelta = ReconV2/dz;
						ReconV2 = (VertBase+v2index)->tv + (newdz*ReconDelta);

				 		// Project the new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);


					// We must test the clip status of the new point and
					// see if it caused a set of points to go off 1 plane..

						Clip2 = 0;
						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;
						 

				#ifdef ZCLIPCLIP

		 				if (	Clip & Clip2 &
		 					XformedVerts[v2index].dwFlags) break;
				#endif

						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else
							polyptr = PolygonHeap++;	// Addr of this poly.

						polyptr->x0 = ReconX2; polyptr->y0 = ReconY2; polyptr->z0 = ZCLIP_NEARL;
						polyptr->u0 = (long)(ReconU2*65536.0F);		polyptr->v0 = (long)(ReconV2*65536.0F);
						polyptr->r0 = (long)ReconR2;

						polyptr->x1 = ReconX; polyptr->y1 = ReconY; polyptr->z1 = ZCLIP_NEARL;
						polyptr->u1 = (long)(ReconU*65536.0F); 		polyptr->v1 = (long)(ReconV*65536.0F);
						polyptr->r1 = (long)ReconR;

						polyptr->x2 = XformedVerts[v2index].x; polyptr->y2 = XformedVerts[v2index].y; polyptr->z2 = XformedVerts[v2index].z;
						polyptr->u2 = (long) (((VertBase+v2index)->tu)*65536.0F);	polyptr->v2 = (long) (((VertBase+v2index)->tv)*65536.0F);
						polyptr->r2 = (long)R2;	



					 //	if (Vmode)
					 //	{
							polyptr->g0 = (long)ReconG2;
							polyptr->g1 = (long)ReconG;
							polyptr->g2 = (long)G2;	

							polyptr->b0 = (long)ReconB2;
							polyptr->b1 = (long)ReconB;
							polyptr->b2 = (long)B2;	
							CLAMPRGB;

					 //	}
					
			
			 			iz = (long) (( ZCLIP_NEAR + ZCLIP_NEAR + XformedVerts[v2index].z) *0.3333333F);
						if (iz<MINOT) iz=MINOT;

						if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

						if (TransparentFlag) iz = 1;



	// Addprim..
						if (!NeedZB)
						{
							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
						}
					

 
						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

		   //					if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
		   //					if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.

								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							if (TransparentFlag)	polyptr->stlevel = st;


							polyptr->channel |= Trans;		// Set color key bit.


						polyptr->Material = CurrentMat*64;

						break;




		/*
			=====================================

						Case 2
		
			=====================================

		*/


			case 2:	// 010: Point 1 behind, 0+2 in front: CLIP


			// 010: Point 1 behind, 0+2 in front: CLIP
			// So reconstruct line 21 (clipping 1).: PRECON_1
			// reconstruct line 01 (clipping 1)      PRECON_2
			// Add triangles 1,PRECON1,2 and 2,1,PRECON2
			// In this instance I can calculate the normal for
			// point precon01 and do the normal on 1,2,precon1.
			// if it faces away, drop the polygon, ie:continue.

						dz = XformedVerts[v2index].z - XformedVerts[v1index].z;
						newdz = XformedVerts[v2index].z-ZCLIP_NEAR;

						ReconX = XformedVerts[v1index].wx - XformedVerts[v2index].wx;
						ReconDelta = ReconX/dz;
						ReconX = XformedVerts[v2index].wx + (newdz*ReconDelta);
					   
						ReconY = XformedVerts[v1index].wy - XformedVerts[v2index].wy;
						ReconDelta = ReconY/dz;
						ReconY = XformedVerts[v2index].wy + (newdz*ReconDelta);

						ReconR = (R1 - R2);
						ReconDelta = ReconR/dz;
						ReconR = newdz * ReconDelta;
						ReconR+=R2;
	
					 //	if (Vmode)
					 //	{
							ReconG = (G1 - G2);
							ReconDelta = ReconG/dz;
							ReconG = newdz * ReconDelta;
							ReconG+=G2;

							ReconB = (B1 - B2);
							ReconDelta = ReconB/dz;
							ReconB = newdz * ReconDelta;
							ReconB+=B2;
					 //	}

						ReconU = ((VertBase+v1index)->tu)  -  ((VertBase+v2index)->tu);
						ReconDelta = ReconU/dz;
						ReconU = ((VertBase+v2index)->tu) + (newdz*ReconDelta);

						ReconV = ((VertBase+v1index)->tv)  -  ((VertBase+v2index)->tv);
						ReconDelta = ReconV/dz;
						ReconV = ((VertBase+v2index)->tv) + (newdz*ReconDelta);


				   		   

			//project new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);


	   	   
				  //		if	(  (	(XformedVerts[v0index].x - XformedVerts[v1index].x) * (XformedVerts[v2index].y - XformedVerts[v1index].y)) -
				  //			   (    (XformedVerts[v0index].y - XformedVerts[v1index].y) * (XformedVerts[v2index].x - XformedVerts[v1index].x))
				  //				> 0)		goto NextPoly;
						


	 					if (  (Culling &&	(	( XformedVerts[ v0index ].x - ReconX) *	(  XformedVerts[  v2index ].y - ReconY) ) >
	   				     		(	( XformedVerts[ v0index ].y - ReconY) *	(  XformedVerts[ v2index ].x - ReconX) )  ) )
 									 goto NextPoly;


			// We must test the clip status of the new point and
			// see if it caused a set of points to go off 1 plane..

						Clip = 0;
						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;
						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;
						 
				#ifdef ZCLIPCLIP

		   				if (	Clip &
		   						XformedVerts[v0index].dwFlags &
		   						XformedVerts[v2index].dwFlags) goto notp2;
				#endif

						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.

						polyptr->x0 = XformedVerts[v0index].x; polyptr->y0 = XformedVerts[v0index].y;  polyptr->z0 = XformedVerts[v0index].z;
						polyptr->u0 = (long)((VertBase+v0index)->tu*65536.0F);polyptr->v0 = (long)((VertBase+v0index)->tv*65536.0F);
						polyptr->r0 = (long)R0;

						polyptr->x1 = ReconX; polyptr->y1 = ReconY; polyptr->z1 = ZCLIP_NEARL;
						polyptr->u1 = (long)(ReconU*65536.0F);	polyptr->v1 = (long)(ReconV*65536.0F);
						polyptr->r1= (long)ReconR;
	

						polyptr->x2 = XformedVerts[v2index].x; polyptr->y2 = XformedVerts[v2index].y; polyptr->z2 = XformedVerts[v2index].z;
						polyptr->u2 = (long)((VertBase+v2index)->tu*65536.0F);polyptr->v2 = (long)((VertBase+v2index)->tv*65536.0F);
						polyptr->r2= (long)R2;	

				   //		if (Vmode)
				   //		{
							polyptr->g0 = (long)G0; polyptr->g1= (long)ReconG; polyptr->g2= (long)G2;	
							polyptr->b0 = (long)B0; polyptr->b1= (long)ReconB; polyptr->b2= (long)B2;	
							CLAMPRGB;


				   //		}

	 					iz = (long) (( XformedVerts[v0index].z +  ZCLIP_NEAR + XformedVerts[v2index].z) *0.3333333F);
						if (iz<MINOT) iz=MINOT;

						if (iz>8191) iz=8191;

						if (TransparentFlag) iz = 1;


						if (!NeedZB)
						{
					// Addprim..
							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
						}



						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

	  //						if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
	  //						if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.

								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.


						polyptr->Material = CurrentMat*64;

							if (TransparentFlag)	polyptr->stlevel = st;


		
				//	NOW DO POLYGON r,r2,2...
				// ~~~~~~~~~~~~~~~~~~~~~~~~~~
notp2:
			
			
						dz = XformedVerts[v0index].z - XformedVerts[v1index].z;
						newdz = XformedVerts[v0index].z-ZCLIP_NEAR;
		
						ReconX2 = XformedVerts[v1index].wx - XformedVerts[v0index].wx;
						ReconDelta = ReconX2/dz;
						ReconX2 = XformedVerts[v0index].wx + (newdz*ReconDelta);
						   
						ReconY2 = XformedVerts[v1index].wy - XformedVerts[v0index].wy;
						ReconDelta = ReconY2/dz;
						ReconY2 = XformedVerts[v0index].wy + (newdz*ReconDelta);
			
						ReconR2 = (R1 - R0);
						ReconDelta = ReconR2/dz;
						ReconR2 = newdz * ReconDelta;
						ReconR2+=R0;
		
				   //		if (Vmode)
				   //		{
							ReconG2 = (G1 - G0);
							ReconDelta = ReconG2/dz;
							ReconG2 = newdz * ReconDelta;
							ReconG2+=G0;
						
							ReconB2 = (B1 - B0);
							ReconDelta = ReconB2/dz;
							ReconB2 = newdz * ReconDelta;
							ReconB2+=B0;
				   //		}

						
						ReconU2 = (VertBase+v1index)->tu - (VertBase+v0index)->tu;
						ReconDelta = ReconU2/dz;
						ReconU2 = (VertBase+v0index)->tu + (newdz*ReconDelta);

						ReconV2 = (VertBase+v1index)->tv- (VertBase+v0index)->tv;
						ReconDelta = ReconV2/dz;
						ReconV2 = (VertBase+v0index)->tv + (newdz*ReconDelta);

				 		// Project the new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);
														   

			// We must test the clip status of the new point and
			// see if it caused a set of points to go off 1 plane..

						Clip2 = 0;
						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;
						 
				#ifdef ZCLIPCLIP

				 		if (	Clip & Clip2 &
				 				XformedVerts[v0index].dwFlags) break;
				#endif


						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.

					  //	polyptr = PolygonHeap++;	// Addr of this poly.

						polyptr->x0 = ReconX2; polyptr->y0 = ReconY2; polyptr->z0 = ZCLIP_NEARL;
						polyptr->x1 = ReconX; polyptr->y1 = ReconY; polyptr->z1 = ZCLIP_NEARL;
						polyptr->x2 = XformedVerts[v0index].x; polyptr->y2 = XformedVerts[v0index].y; polyptr->z2 = XformedVerts[v0index].z;

						polyptr->u0 = (long)(ReconU2*65536.0F);
						polyptr->v0 = (long)(ReconV2*65536.0F);

						polyptr->u1 = (long)(ReconU*65536.0F);
						polyptr->v1 = (long)(ReconV*65536.0F);

						polyptr->u2 = (long)((VertBase+v0index)->tu*65536.0F);
						polyptr->v2 = (long)((VertBase+v0index)->tv*65536.0F);

						polyptr->r0 = (long)ReconR2;
						polyptr->r1 = (long)ReconR;
						polyptr->r2 = (long)R0;	

				   //		if (Vmode)
				   //		{
							polyptr->g0 = (long)ReconG2;
							polyptr->g1 = (long)ReconG;
							polyptr->g2 = (long)G0;	

							polyptr->b0 = (long)ReconB2;
							polyptr->b1 = (long)ReconB;
							polyptr->b2 = (long)B0;	

							CLAMPRGB;

				   //		}


									
	 					iz = (long) (( ZCLIP_NEAR + ZCLIP_NEAR + XformedVerts[v0index].z) *0.3333333F);
						if (iz<MINOT) iz=MINOT;

						if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;
						if (TransparentFlag) iz = 1;


						if (!NeedZB)
						{
					// Addprim..
							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
						}					

						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

 //							if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}



							polyptr->channel |= Trans;		// Set color key bit.


							polyptr->Material = CurrentMat*64;

							if (TransparentFlag)	polyptr->stlevel = st;


						break;




		/*
			=====================================

						Case 3
		
			=====================================

		*/

case 3:	// 011: Point 0+1 behind, 2 in front: CLIP


						dz = XformedVerts[v2index].z - XformedVerts[v0index].z;
						newdz = XformedVerts[v2index].z-ZCLIP_NEAR;

						ReconX = XformedVerts[v0index].wx - XformedVerts[v2index].wx;
						ReconDelta = ReconX/dz;
						ReconX = XformedVerts[v2index].wx + (newdz*ReconDelta);
					   
						ReconY = XformedVerts[v0index].wy - XformedVerts[v2index].wy;
						ReconDelta = ReconY/dz;
						ReconY = XformedVerts[v2index].wy + (newdz*ReconDelta);


		// 011: Point 0+1 behind, 2 in front: CLIP
		// just 1 tri using 2 reprojected points.
						ReconR = (R0 - R2);
						ReconDelta = ReconR/dz;
						ReconR = newdz * ReconDelta;
						ReconR+=R2;

				   //		if (Vmode)
				   //		{
							ReconG = (G0 - G2);
							ReconDelta = ReconG/dz;
							ReconG = newdz * ReconDelta;
							ReconG+=G2;	

							ReconB = (B0 - B2);
							ReconDelta = ReconB/dz;
							ReconB = newdz * ReconDelta;
							ReconB+=B2;
				   //		}
						
						ReconU = ((VertBase+v0index)->tu)  -  ((VertBase+v2index)->tu);
						ReconDelta = ReconU/dz;
						ReconU = ((VertBase+v2index)->tu) + (newdz*ReconDelta);

						ReconV = ((VertBase+v0index)->tv)  -  ((VertBase+v2index)->tv);
						ReconDelta = ReconV/dz;
						ReconV = ((VertBase+v2index)->tv) + (newdz*ReconDelta);


						// Project the new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);




		//## calc 2nd point of the tri.

						//newdz = vert2->z-1024; SAME!


						dz = XformedVerts[v2index].z - XformedVerts[v1index].z;

						ReconX2 = XformedVerts[v1index].wx - XformedVerts[v2index].wx;
						ReconDelta = ReconX2/dz;
						ReconX2 = XformedVerts[v2index].wx + (newdz*ReconDelta);
					   
						ReconY2 = XformedVerts[v1index].wy - XformedVerts[v2index].wy;
						ReconDelta = ReconY2/dz;
						ReconY2 = XformedVerts[v2index].wy + (newdz*ReconDelta);


						ReconR2 = (R1 - R2);
						ReconDelta = ReconR2/dz;
						ReconR2 = newdz * ReconDelta;
						ReconR2+=R2;
				   //		if (Vmode)
				   //		{
							ReconG2 = (G1 - G2);
							ReconDelta = ReconG2/dz;
							ReconG2 = newdz * ReconDelta;
							ReconG2+=G2;

							ReconB2 = (B1 - B2);
							ReconDelta = ReconB2/dz;
							ReconB2 = newdz * ReconDelta;
							ReconB2+=B2;	
				   //		}


												
						ReconU2 = ((VertBase+v1index)->tu)  -  ((VertBase+v2index)->tu);
						ReconDelta = ReconU2/dz;
						ReconU2 = ((VertBase+v2index)->tu) + (newdz*ReconDelta);

						ReconV2 = ((VertBase+v1index)->tv)  -  ((VertBase+v2index)->tv);
						ReconDelta = ReconV2/dz;
						ReconV2 = ((VertBase+v2index)->tv) + (newdz*ReconDelta);


						// Project the new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);


			   	   
				  //		if	(  (	(XformedVerts[v0index].x - XformedVerts[v1index].x) * (XformedVerts[v2index].y - XformedVerts[v1index].y)) -
				  //			   (    (XformedVerts[v0index].y - XformedVerts[v1index].y) * (XformedVerts[v2index].x - XformedVerts[v1index].x))
				  //				> 0)		goto NextPoly;



 						if (  (Culling &&	(	(  ReconX - ReconX2) *	( XformedVerts[ v2index ].y - ReconY2) ) >
	  							(	(  ReconY - ReconY2) *	( XformedVerts[ v2index ].x - ReconX2) )  ) )
										 goto NextPoly;


			
						Clip = 0;
						Clip2=0;

						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;
						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;


						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;
						 
				#ifdef ZCLIPCLIP

			   			if (	Clip & Clip2 &
			   				XformedVerts[v2index].dwFlags) break;
				#endif


						 
						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.
				   
					 //	polyptr = PolygonHeap++;	// Addr of this poly.

						polyptr->x0 = ReconX; polyptr->y0 = ReconY; polyptr->z0 = ZCLIP_NEARL;
						polyptr->u0 = ReconU*65536.0F;polyptr->v0 = ReconV*65536.0F;
						polyptr->r0 = (long)ReconR;

						polyptr->x1 = ReconX2; polyptr->y1 = ReconY2; polyptr->z1 = ZCLIP_NEARL;
						polyptr->u1 = ReconU2*65536.0F;polyptr->v1 = ReconV2*65536.0F;
					    polyptr->r1 = (long)ReconR2;

						polyptr->x2 = XformedVerts[v2index].x; polyptr->y2 = XformedVerts[v2index].y; polyptr->z2 = XformedVerts[v2index].z;
						polyptr->u2 = (VertBase+v2index)->tu*65536.0F;polyptr->v2 = (VertBase+v2index)->tv*65536.0F;
						polyptr->r2 = (long)R2;	
	
				 //		if (Vmode)
				 //		{
							polyptr->g0 = (long)ReconG;    polyptr->g1 = (long)ReconG2;	polyptr->g2= (long)G2;	
							polyptr->b0 = (long)ReconB;    polyptr->b1 = (long)ReconB2;	polyptr->b2= (long)B2;	
							CLAMPRGB;

				 //		}


						
					 	iz = (long) (( ZCLIP_NEAR + ZCLIP_NEAR + XformedVerts[v2index].z) *0.3333333F);
						if (iz<MINOT) iz=MINOT;

						if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;


						if (TransparentFlag) iz = 1;


						if (!NeedZB)
						{
					// Addprim..
							if ( ObjTable[ iz ] == NULL )
							{
								polyptr -> Next = NULL;
								ObjTable[ iz ] = polyptr;
							}
							else
							{
								polyptr -> Next = (char *)ObjTable[ iz ];
								ObjTable[ iz ] = polyptr;
							}
							if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
						}
					
						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

 //							if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.


						polyptr->Material = CurrentMat*64;

				   		if (TransparentFlag)	polyptr->stlevel = st;


						break;










		/*
			=====================================

						Case 4
		
			=====================================

		*/


case 4:	// 100:	Point 2 behind, 0+1 in front: CLIP


			// 100: Point 2 behind, 0+1 in front: CLIP
			// So reconstruct line 12 (clipping 2).: PRECON_1
			// reconstruct line 02 (clipping 2)      PRECON_2
			// Add triangles 1,PRECON1,2 and 2,1,PRECON2
			// In this instance I can calculate the normal for
			// point precon01 and do the normal on 1,2,precon1.
			// if it faces away, drop the polygon, ie:continue.



	
						dz = XformedVerts[v0index].z - XformedVerts[v2index].z;
						newdz = XformedVerts[v0index].z-ZCLIP_NEAR;

						ReconX = XformedVerts[v2index].wx - XformedVerts[v0index].wx;
						ReconDelta = ReconX/dz;
						ReconX = XformedVerts[v0index].wx + (newdz*ReconDelta);
					   
						ReconY = XformedVerts[v2index].wy - XformedVerts[v0index].wy;
						ReconDelta = ReconY/dz;
						ReconY = XformedVerts[v0index].wy + (newdz*ReconDelta);



							ReconR = (R2 - R0);
							ReconDelta = ReconR/dz;
							ReconR = newdz * ReconDelta;
							ReconR+=R0;
	
				   //			if (Vmode)
				   //			{
								ReconG = (G2 - G0);
								ReconDelta = ReconG/dz;
								ReconG = newdz * ReconDelta;
								ReconG+=G0;

								ReconB = (B2 - B0);
								ReconDelta = ReconB/dz;
								ReconB = newdz * ReconDelta;
								ReconB+=B0;

				   //			}


						
						ReconU = ((VertBase+v2index)->tu)  -  ((VertBase+v0index)->tu);
						ReconDelta = ReconU/dz;
						ReconU = ((VertBase+v0index)->tu) + (newdz*ReconDelta);

						ReconV = ((VertBase+v2index)->tv)  -  ((VertBase+v0index)->tv);
						ReconDelta = ReconV/dz;
						ReconV = ((VertBase+v0index)->tv) + (newdz*ReconDelta);


						// Project the new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);

			  	   
				  //		if	(  (	(XformedVerts[v0index].x - XformedVerts[v1index].x) * (XformedVerts[v2index].y - XformedVerts[v1index].y)) -
				  //			   (    (XformedVerts[v0index].y - XformedVerts[v1index].y) * (XformedVerts[v2index].x - XformedVerts[v1index].x))
				  //				> 0)		goto NextPoly;


			
														   
						if (  (Culling && (	( XformedVerts[ v0index].x - XformedVerts[ v1index ].x) *	( ReconY - XformedVerts[ v1index ].y) ) >
  				     		(	( XformedVerts[ v0index ].y - XformedVerts[ v1index ].y) *	( ReconX - XformedVerts[ v1index ].x) )  ) )
											 goto NextPoly;
			



						Clip = 0;
						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;
						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;

				#ifdef ZCLIPCLIP

				 		if (	Clip &
				 			XformedVerts[v0index].dwFlags &
				 			XformedVerts[v1index].dwFlags) goto notp3; //break;
				#endif


						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.


			   //		polyptr = PolygonHeap++;	// Addr of this poly.

	polyptr->x0 = XformedVerts[v0index].x; polyptr->y0 = XformedVerts[v0index].y; polyptr->z0 = XformedVerts[v0index].z;polyptr->u0 = (VertBase+v0index)->tu*65536.0F;polyptr->v0 = (VertBase+v0index)->tv*65536.0F;
	polyptr->x1 = XformedVerts[v1index].x; polyptr->y1 = XformedVerts[v1index].y; polyptr->z1 = XformedVerts[v1index].z;polyptr->u1 = (VertBase+v1index)->tu*65536.0F;polyptr->v1 = (VertBase+v1index)->tv*65536.0F;
	polyptr->x2 = ReconX; polyptr->y2 = ReconY; polyptr->z2 = ZCLIP_NEARL;		polyptr->u2 = ReconU*65536.0F;	polyptr->v2 = ReconV*65536.0F;

		polyptr->r0= (long)R0; polyptr->r1= (long)R1; polyptr->r2= (long)ReconR;	
		//if (Vmode)
		//{
			polyptr->g0= (long)G0;polyptr->g1= (long)G1;polyptr->g2= (long)ReconG;	
			polyptr->b0= (long)B0;polyptr->b1= (long)B1;polyptr->b2= (long)ReconB;	
			CLAMPRGB;
		//}



	 	iz = (long) (( XformedVerts[v0index].z + XformedVerts[v1index].z + ZCLIP_NEAR) *0.3333333F);
		if (iz<MINOT) iz=MINOT;

		if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

		if (TransparentFlag) iz = 1;


// Addprim..
		if (!NeedZB)
		{
			if ( ObjTable[ iz ] == NULL )
			{
				polyptr -> Next = NULL;
				ObjTable[ iz ] = polyptr;
			}
			else
			{
				polyptr -> Next = (char *)ObjTable[ iz ];
				ObjTable[ iz ] = polyptr;
			}
			if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
		}
	

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.


//							if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.


							if (TransparentFlag)	polyptr->stlevel = st;


							polyptr->Material = CurrentMat*64;


//	NOW DO POLYGON r,r2,2...
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
notp3:





	
					dz = XformedVerts[v1index].z - XformedVerts[v2index].z;
					newdz = XformedVerts[v1index].z-ZCLIP_NEAR;

					ReconX2 = XformedVerts[v2index].wx - XformedVerts[v1index].wx;
					ReconDelta = ReconX2/dz;
					ReconX2 = XformedVerts[v1index].wx + (newdz*ReconDelta);
					   
					ReconY2 = XformedVerts[v2index].wy - XformedVerts[v1index].wy;
					ReconDelta = ReconY2/dz;
					ReconY2 = XformedVerts[v1index].wy + (newdz*ReconDelta);



						ReconR2 = (R2 - R1);
						ReconDelta = ReconR2/dz;
						ReconR2 = newdz * ReconDelta;
						ReconR2+=R1;	

					 //	if (Vmode)
					 //	{
							ReconG2 = (G2 - G1);
							ReconDelta = ReconG2/dz;
							ReconG2 = newdz * ReconDelta;
							ReconG2+=G1;	

							ReconB2 = (B2 - B1);
							ReconDelta = ReconB2/dz;
							ReconB2 = newdz * ReconDelta;
							ReconB2+=B1;	
					 //	}


					
						
						ReconU2 = ((VertBase+v2index)->tu)  -  ((VertBase+v1index)->tu);
						ReconDelta = ReconU2/dz;
						ReconU2 = ((VertBase+v1index)->tu) + (newdz*ReconDelta);

						ReconV2 = ((VertBase+v2index)->tv)  -  ((VertBase+v1index)->tv);
						ReconDelta = ReconV2/dz;
						ReconV2 = ((VertBase+v1index)->tv) + (newdz*ReconDelta);




						// Project the new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);

														   
						Clip2 = 0;
						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;
	
						#ifdef ZCLIPCLIP
				   			if (	Clip & Clip2 &
				   				XformedVerts[v1index].dwFlags) break;
						#endif


						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.


				 //	polyptr = PolygonHeap++;	// Addr of this poly.

					polyptr->x0 = ReconX2; polyptr->y0 = ReconY2; polyptr->z0 = ZCLIP_NEARL;
					polyptr->x1 = ReconX; polyptr->y1 = ReconY; polyptr->z1 = ZCLIP_NEARL;
					polyptr->x2 = XformedVerts[v1index].x; polyptr->y2 = XformedVerts[v1index].y; polyptr->z2 = XformedVerts[v1index].z;

					polyptr->u0 = ReconU2*65536.0F;
					polyptr->u1 = ReconU*65536.0F;
					polyptr->u2 = (VertBase+v1index)->tu*65536.0F;

					polyptr->v0 = ReconV2*65536.0F;
					polyptr->v1 = ReconV*65536.0F;
					polyptr->v2 = (VertBase+v1index)->tv*65536.0F;

					polyptr->r0 = (long)ReconR2;
					polyptr->r1 = (long)ReconR;
					polyptr->r2 = (long)R1;	
					//if (Vmode)
					//	{
							polyptr->g0 = (long)ReconG2;
							polyptr->g1 = (long)ReconG;
							polyptr->g2 = (long)G1;		

							polyptr->b0 = (long)ReconB2;
							polyptr->b1 = (long)ReconB;
							polyptr->b2 = (long)B1;	
							CLAMPRGB;

				    //	}





	 	iz = (long) (( ZCLIP_NEAR + XformedVerts[v1index].z + ZCLIP_NEAR) *0.3333333F);
		if (iz<MINOT) iz=MINOT;

		if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

		if (TransparentFlag) iz = 1;


		if (!NeedZB)
		{

// Addprim..
			if ( ObjTable[ iz ] == NULL )
			{
				polyptr -> Next = NULL;
				ObjTable[ iz ] = polyptr;
			}
			else
			{
				polyptr -> Next = (char *)ObjTable[ iz ];
				ObjTable[ iz ] = polyptr;
			}
			if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
		}


						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.



//							if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.

	
							polyptr->Material = CurrentMat*64;


							if (TransparentFlag)	polyptr->stlevel = st;


//##############################################
//##############################################
//##############################################

					break;


case 5:	// 101: Point 0+2 behind, 1 in front: CLIP

//###############################################################
//###############################################################
//###############################################################
//###############################################################
//###############################################################
//###############################################################

	// 101: Point 0+2 behind, 1 in front: CLIP
	// just 1 tri using 2 reprojected points.


	
	
						dz = XformedVerts[v1index].z - XformedVerts[v0index].z;
						newdz = XformedVerts[v1index].z-ZCLIP_NEAR;

						ReconX = XformedVerts[v0index].wx - XformedVerts[v1index].wx;
						ReconDelta = ReconX/dz;
						ReconX = XformedVerts[v1index].wx + (newdz*ReconDelta);
					   
						ReconY = XformedVerts[v0index].wy - XformedVerts[v1index].wy;
						ReconDelta = ReconY/dz;
						ReconY = XformedVerts[v1index].wy + (newdz*ReconDelta);


							ReconR = (R0 - R1);
							ReconDelta = ReconR/dz;
							ReconR = newdz * ReconDelta;
							ReconR+=R1;

					  //		if (Vmode)
					  //		{
								ReconG = (G0 - G1);
								ReconDelta = ReconG/dz;
								ReconG = newdz * ReconDelta;
								ReconG+=G1;

								ReconB = (B0 - B1);
								ReconDelta = ReconB/dz;
								ReconB = newdz * ReconDelta;
								ReconB+=B1;

					  //		}




						
						ReconU = ((VertBase+v0index)->tu)  -  ((VertBase+v1index)->tu);
						ReconDelta = ReconU/dz;
						ReconU = ((VertBase+v1index)->tu) + (newdz*ReconDelta);

						ReconV = ((VertBase+v0index)->tv)  -  ((VertBase+v1index)->tv);
						ReconDelta = ReconV/dz;
						ReconV = ((VertBase+v1index)->tv) + (newdz*ReconDelta);


				  		// Project the new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);

														   
	  
		//## calc 2nd point of the tri.


						dz = XformedVerts[v1index].z - XformedVerts[v2index].z;
						//newdz = vert1->z-1024; SAME!

						ReconX2 = XformedVerts[v2index].wx - XformedVerts[v1index].wx;
						ReconDelta = ReconX2/dz;
						ReconX2 = XformedVerts[v1index].wx + (newdz*ReconDelta);
					   
						ReconY2 = XformedVerts[v2index].wy - XformedVerts[v1index].wy;
						ReconDelta = ReconY2/dz;
						ReconY2 = XformedVerts[v1index].wy + (newdz*ReconDelta);


							ReconR2 = (R2 - R1);
							ReconDelta = ReconR2/dz;
							ReconR2 = newdz * ReconDelta;
							ReconR2+=R1;
					   //		if (Vmode)
					   //		{
								ReconG2 = (G2 - G1);
								ReconDelta = ReconG2/dz;
								ReconG2 = newdz * ReconDelta;
								ReconG2+=G1;

								ReconB2 = (B2 - B1);
								ReconDelta = ReconB2/dz;
								ReconB2 = newdz * ReconDelta;
								ReconB2+=B1;
					   //		}


						
						ReconU2 = ((VertBase+v2index)->tu)  -  ((VertBase+v1index)->tu);
						ReconDelta = ReconU2/dz;
						ReconU2 = ((VertBase+v1index)->tu) + (newdz*ReconDelta);

						ReconV2 = ((VertBase+v2index)->tv)  -  ((VertBase+v1index)->tv);
						ReconDelta = ReconV2/dz;
						ReconV2 = ((VertBase+v1index)->tv) + (newdz*ReconDelta);


// COULD BE THIS???

 		// Project the new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);


	   	   				  //		if	(  (	(XformedVerts[v0index].x - XformedVerts[v1index].x) * (XformedVerts[v2index].y - XformedVerts[v1index].y)) -
				  //			   (    (XformedVerts[v0index].y - XformedVerts[v1index].y) * (XformedVerts[v2index].x - XformedVerts[v1index].x))
				  //				> 0)		goto NextPoly;


						if ( (Culling &&	(	( ReconX - XformedVerts[ v1index ].x) *	(ReconY2 - XformedVerts[ v1index ].y) ) >
  	 		     				(	( ReconY - XformedVerts[ v1index ].y) *	(ReconX2 - XformedVerts[ v1index ].x) )  ) )
											 goto NextPoly;

			
						Clip = 0;
						Clip2=0;


						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;

						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;
						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;

				#ifdef ZCLIPCLIP

						if (	Clip & Clip2 &
							XformedVerts[v1index].dwFlags) break;
				#endif


													  												  

						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.

					//	polyptr = PolygonHeap++;	// Addr of this poly.

	polyptr->x0 = ReconX; polyptr->y0 = ReconY; polyptr->z0 = ZCLIP_NEARL;		polyptr->u0 = ReconU*65536.0F;polyptr->v0 = ReconV*65536.0F;
	polyptr->x1 = XformedVerts[v1index].x; polyptr->y1 = XformedVerts[v1index].y; polyptr->z1 = XformedVerts[v1index].z;polyptr->u1 = (VertBase+v1index)->tu*65536.0F;polyptr->v1 = (VertBase+v1index)->tv*65536.0F;
	polyptr->x2 = ReconX2; polyptr->y2 = ReconY2; polyptr->z2 = ZCLIP_NEARL;		polyptr->u2 = ReconU2*65536.0F;polyptr->v2 = ReconV2*65536.0F;

	polyptr->r0= (long)ReconR;	polyptr->r1= (long)R1;	polyptr->r2= (long)ReconR2;	

 //	if (Vmode)
 //	{
		polyptr->g0= (long)ReconG;	polyptr->g1= (long)G1;polyptr->g2= (long)ReconG2;	
		polyptr->b0= (long)ReconB;	polyptr->b1= (long)B1;polyptr->b2= (long)ReconB2;	
		CLAMPRGB;

 //	}

	

	 	iz = (long) (( ZCLIP_NEAR + ZCLIP_NEAR + XformedVerts[v1index].z) *0.3333333F);
		if (iz<MINOT) iz=MINOT;

		if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

		if (TransparentFlag) iz = 1;


// Addprim..
		if (!NeedZB)
		{
			if ( ObjTable[ iz ] == NULL )
			{
				polyptr -> Next = NULL;
				ObjTable[ iz ] = polyptr;
			}
			else
			{
				polyptr -> Next = (char *)ObjTable[ iz ];
				ObjTable[ iz ] = polyptr;
			}
			if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
		}	

						

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.

//							if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.


							polyptr->Material = CurrentMat*64;

							if (TransparentFlag)	polyptr->stlevel = st;


					break;
//##############################################
//##############################################
//##############################################


case 6:	// 110: Point 1+2 behind, 0 in front: CLIP


	// 110: Point 1+2 behind, 0 in front: CLIP
	// just 1 tri using 2 reprojected points.

		
						dz = XformedVerts[v0index].z - XformedVerts[v1index].z;
						newdz = XformedVerts[v0index].z-ZCLIP_NEAR;

						ReconX = XformedVerts[v1index].wx - XformedVerts[v0index].wx;
						ReconDelta = ReconX/dz;
						ReconX = XformedVerts[v0index].wx + (newdz*ReconDelta);
					   
						ReconY = XformedVerts[v1index].wy - XformedVerts[v0index].wy;
						ReconDelta = ReconY/dz;
						ReconY = XformedVerts[v0index].wy + (newdz*ReconDelta);


						ReconR = (R1 - R0);
						ReconDelta = ReconR/dz;
						ReconR = newdz * ReconDelta;
						ReconR+=R0;
				
			   //			if (Vmode)
			   //			{
							ReconG = (G1 - G0);
							ReconDelta = ReconG/dz;
							ReconG = newdz * ReconDelta;
							ReconG+=G0;

							ReconB = (B1 - B0);
							ReconDelta = ReconB/dz;
							ReconB = newdz * ReconDelta;
							ReconB+=B0;

			   //			}

						
						ReconU = ((VertBase+v1index)->tu)  -  ((VertBase+v0index)->tu);
						ReconDelta = ReconU/dz;
						ReconU = ((VertBase+v0index)->tu) + (newdz*ReconDelta);

						ReconV = ((VertBase+v1index)->tv)  -  ((VertBase+v0index)->tv);
						ReconDelta = ReconV/dz;
						ReconV = ((VertBase+v0index)->tv) + (newdz*ReconDelta);


		// Project the new point.
						ReconX	= SurfWidthOver2 + ((SurfWidthOver2*ReconX)/ZCLIP_NEAR);
						ReconY	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY)/ZCLIP_NEAR);

		//## calc 2nd point of the tri.




						dz = XformedVerts[v0index].z - XformedVerts[v2index].z;

						ReconX2 = XformedVerts[v2index].wx - XformedVerts[v0index].wx;
						ReconDelta = ReconX2/dz;
						ReconX2 = XformedVerts[v0index].wx + (newdz*ReconDelta);
					   
						ReconY2 = XformedVerts[v2index].wy - XformedVerts[v0index].wy;
						ReconDelta = ReconY2/dz;
						ReconY2 = XformedVerts[v0index].wy + (newdz*ReconDelta);
						
						
						ReconR2 = (R2 - R0);
						ReconDelta = ReconR2/dz;
						ReconR2 = newdz * ReconDelta;
						ReconR2+=R0;

				//		if (Vmode)
				//		{
							ReconG2 = (G2 - G0);
							ReconDelta = ReconG2/dz;
							ReconG2 = newdz * ReconDelta;
							ReconG2+=G0;

							ReconB2 = (B2 - B0);
							ReconDelta = ReconB2/dz;
							ReconB2 = newdz * ReconDelta;
							ReconB2+=B0;
				//		}

						ReconU2 = ((VertBase+v2index)->tu)  -  ((VertBase+v0index)->tu);
						ReconDelta = ReconU2/dz;
						ReconU2 = ((VertBase+v0index)->tu) + (newdz*ReconDelta);

						ReconV2 = ((VertBase+v2index)->tv)  -  ((VertBase+v0index)->tv);
						ReconDelta = ReconV2/dz;
						ReconV2 = ((VertBase+v0index)->tv) + (newdz*ReconDelta);


						// project new point.
						ReconX2	= SurfWidthOver2 + ((SurfWidthOver2*ReconX2)/ZCLIP_NEAR);
						ReconY2	= SurfHeightOver2 + ((-SurfHeightOver2*ReconY2)/ZCLIP_NEAR);



//!!!!! Dodgy !!!!!

				  //		if	(  (	(XformedVerts[v0index].x - XformedVerts[v1index].x) * (XformedVerts[v2index].y - XformedVerts[v1index].y)) -
				  //			   (    (XformedVerts[v0index].y - XformedVerts[v1index].y) * (XformedVerts[v2index].x - XformedVerts[v1index].x))
				  //				> 0)		goto NextPoly;



					if	(	(Culling &&	(	( XformedVerts[ v0index ].x - ReconX) *	( ReconY2 - ReconY) ) >
  								(	( XformedVerts[ v0index ].y - ReconY) *	( ReconX2 - ReconX) )  ) )
											 goto NextPoly;



						Clip = 0;
						Clip2=0;


						if (ReconX2 >= SurfWidth)	Clip2 |= D3DCLIP_RIGHT;
						else	if (ReconX2 < 0.0F)	Clip2 |= D3DCLIP_LEFT;
						if (ReconY2 >= SurfHeight)	Clip2 |= D3DCLIP_BOTTOM;
						else	if (ReconY2 < 0.0F)	Clip2 |= D3DCLIP_TOP;

						if (ReconX >= SurfWidth)	Clip |= D3DCLIP_RIGHT;
						else	if (ReconX < 0.0F)	Clip |= D3DCLIP_LEFT;
						if (ReconY >= SurfHeight)	Clip |= D3DCLIP_BOTTOM;
						else	if (ReconY < 0.0F)	Clip |= D3DCLIP_TOP;


				#ifdef ZCLIPCLIP
			
			   			if (	Clip & Clip2 &
			   					XformedVerts[v0index].dwFlags ) break;
				#endif



						if (NeedZB)
							polyptr = ZBPolyHeap++;
						else	
							polyptr = PolygonHeap++;	// Addr of this poly.


				   //		polyptr = PolygonHeap++;	// Addr of this poly.

	polyptr->x0 = ReconX; polyptr->y0 = ReconY; polyptr->z0 = ZCLIP_NEARL;		polyptr->u0 = ReconU*65536.0F;polyptr->v0 = ReconV*65536.0F;
	polyptr->x1 = XformedVerts[v0index].x; polyptr->y1 = XformedVerts[v0index].y; polyptr->z1 = XformedVerts[v0index].z;polyptr->u1 = (VertBase+v0index)->tu*65536.0F;polyptr->v1 = (VertBase+v0index)->tv*65536.0F;
	polyptr->x2 = ReconX2; polyptr->y2 = ReconY2; polyptr->z2 = ZCLIP_NEARL;   	polyptr->u2 = ReconU2*65536.0F;polyptr->v2 = ReconV2*65536.0F;

		polyptr->r0= (long)ReconR;	polyptr->r1= (long)R0; polyptr->r2= (long)ReconR2;	
	   //	if (Vmode)
	   //	{
			polyptr->g0= (long)ReconG;	polyptr->g1= (long)G0; polyptr->g2= (long)ReconG2;	
			polyptr->b0= (long)ReconB;	polyptr->b1= (long)B0; polyptr->b2= (long)ReconB2;	
			CLAMPRGB;

	   //	}




	 	iz = (long) (( ZCLIP_NEARL + ZCLIP_NEARL + XformedVerts[v0index].z) *0.3333333F);
		if (iz<MINOT) iz=MINOT;

		if (iz>8191) iz=8191; 	//if (iz >= MAX_DISTANCE) continue;

		if (TransparentFlag) iz = 1;


// Addprim..
		if (!NeedZB)
		{
			if ( ObjTable[ iz ] == NULL )
			{
				polyptr -> Next = NULL;
				ObjTable[ iz ] = polyptr;
			}
			else
			{
				polyptr -> Next = (char *)ObjTable[ iz ];
				ObjTable[ iz ] = polyptr;
			}
			if (iz>Highestz) Highestz=iz;	//erm, i doubt it somehow.
		}

						bright = polyptr->r0;
						
						if (
							(	polyptr->g0==bright && polyptr->b0==bright) &&
							(	polyptr->r1==bright && polyptr->g1==bright && polyptr->b1==bright) &&
							(	polyptr->r2==bright && polyptr->g2==bright && polyptr->b2==bright) )
							polyptr->channel = 4+TransparentFlag;	// 0/1 for AFFN flat, AFFN flat semitr.
						else
							polyptr->channel = 6+TransparentFlag;	// Always PC16 if Zclipped.


//						if ((polyptr->channel==3)) polyptr->channel=1;	// grdsemitr -> semitr remap.
//							if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.


								if (NeedGour)
								{
									if ((polyptr->channel==7)) polyptr->channel=2+64+4;	// grdsemitr-> gouraud remap + CK.
								}	
								else
								{
									if ((polyptr->channel==7)) polyptr->channel=5;	// grdsemitr -> semitr remap.
								}


							polyptr->channel |= Trans;		// Set color key bit.

							polyptr->Material = CurrentMat*64;

							if (TransparentFlag)	polyptr->stlevel = st;


		break;
//##############################################
//##############################################
//##############################################



case 7:
				break;

			}




// Reject if facing away....
					 


NextPoly:						TriPtr++;		// Next triangle in block.
							

						}

					   	//ESrcAdd = DataUnits*sizeof(D3DTRIANGLE);		  // Skip last triangle block.
						ESrc += DataUnits*sizeof(D3DTRIANGLE);
						goto decode;


					}
					else
					{
						CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc+=2;

						ESrcAdd = CommandSize * DataUnits;
						ESrc+=ESrcAdd;
					}

					goto decode;


DoneDecode:
					return;
}


// OPTIMIZE OFF ------------------------->>>>>>>>>>>>>>>>>>>>>>>



#pragma optimize( "", off )







void	CWExecute2(	LPDIRECT3DDEVICE lpDev,
					LPDIRECT3DEXECUTEBUFFER execbuf,
					LPDIRECT3DVIEWPORT lpView,
					WORD cwFlags)
{
D3DEXECUTEBUFFERDESC debDesc;
D3DEXECUTEDATA execdata;
char	charbuf[160];


			//1. LOCK the execute buffer.
				memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
				debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

				if ( execbuf->lpVtbl->Lock(	execbuf, &debDesc ) != D3D_OK)	return; // FALSE ;


			//2. Get execute data.

				memset(&execdata, 0, sizeof (D3DEXECUTEDATA));
				execdata.dwSize = sizeof(D3DEXECUTEDATA);
				if ( execbuf->lpVtbl->GetExecuteData( execbuf, &execdata) !=D3D_OK ) return;


				//Msg("got execute data..");
			// Print it...

				//sprintf(&charbuf[0],"#pts %d voffset %d instroffset %d",
				//	execdata.dwVertexCount,
				//	execdata.dwVertexOffset,
				//	execdata.dwInstructionOffset);
				//Msg(&charbuf[0]);


				//	Msg("locked execbuffer OK..");


			//3. Set up a D3DTRANSFORMDATA structure...
					CWXformData.dwSize = sizeof (D3DTRANSFORMDATA);							//???????????? WHY ?????????
					CWXformData.lpIn = (char *)debDesc.lpData + execdata.dwVertexOffset;	// Verts IN.
					CWXformData.dwInSize = sizeof (D3DLVERTEX);
					CWXformData.lpOut = &XformedVerts[0];	// Verts OUT.
					CWXformData.dwOutSize = sizeof( D3DLVERTEX );
			 		CWXformData.dwClip = 0;
					CWXformData.dwClipIntersection = D3DCLIP_LEFT | D3DCLIP_RIGHT | D3DCLIP_TOP | D3DCLIP_BOTTOM | D3DCLIP_FRONT;
					CWXformData.dwClipUnion = 0;

					// Test for already xformed verts....
			 //		if (cwFlags==D3DEXECUTE_CLIPPED)
			 //			MyVispolyTransformVertices( execdata.dwVertexCount, &CWXformData );
			 //		else
			 //			MyVispolyCopyVerts( execdata.dwVertexCount, &CWXformData );

					CWExecute( &debDesc, execdata.dwVertexCount, cwFlags );


			//4. Unlock the execute buffer.

					if ( execbuf->lpVtbl->Unlock(
						execbuf) != D3D_OK)	return; // FALSE;





}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/


#ifdef OPT_ON
#pragma optimize( "", off )
#endif











long ZShow;





















/*
		===========================================================================================================================

			FAST FLIPPING ROUTINE, USES FAST COPY LOOP TO TRANSFER THE MEMORY.

			COULD USE FPU COPY HERE, BUT IS IT WORTH IT ?


			WE CAN ASSUME HERE THAT THE SOURCE IS IN SYSTEM, DEST IS IN VRAM.

		===========================================================================================================================

*/



long UseDDrawFlip=0;




//#define USE_DDRAW_FLIP


char	flgxor;

char *	FlipSrc, *FlipDst;			// Address of each surf.
long	FBpitch, BBpitch;	// pitch for both surfs.
long	FlipXSize, FlipYSize;		// XY size to copy.
char *SrcBase, *DstBase;

void CWFlip()
{
DDSURFACEDESC	BBddsd;	// Back buffer surface descriptor.
DDSURFACEDESC	FBddsd;	// Front buffer surface descriptor.
HRESULT			ddrval;	// Crap from ddraw.


//=======================================================================================================================
//=======================================================================================================================
//=======================================================================================================================


// Clear the 1 pixel line on the right of the back buffer. Shit I know, but I want to finish this....

// Back buffer.

	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);
	ddrval = d3dappi.lpBackBuffer->lpVtbl->Lock( d3dappi.lpBackBuffer, NULL, &BBddsd,0,NULL);

	BBpitch   = BBddsd.lPitch;
	FlipXSize = BBddsd.dwWidth;						// Get width.
	FlipYSize = BBddsd.dwHeight;					// Get height.

	if (Vmode) FlipXSize <<= 1;						// 2 bytes/pixel check.


	SrcBase = BBddsd.lpSurface;

	_asm
	{
		pushad

		mov	edi,[ SrcBase ]
		mov	edx, [ FlipYSize ]
		add	edi,[ FlipXSize ]
		sub	edi,4
		mov	eax,0 //x7fff
			 
	linelp:
	   	mov	[ edi ],eax
		add	edi,[ BBpitch ]
		dec	edx
		jne short linelp

		popad
		
	}

	d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );



//=======================================================================================================================
//=======================================================================================================================
//=======================================================================================================================







		if (UseDDrawFlip)
		{

			ddrval = d3dappi.lpFrontBuffer->lpVtbl->Flip(d3dappi.lpFrontBuffer,
														d3dappi.lpBackBuffer,
														0          );
				
			if (ddrval == DDERR_SURFACELOST)
			{
			 d3dappi.lpFrontBuffer->lpVtbl->Restore(d3dappi.lpFrontBuffer);
			 d3dappi.lpBackBuffer->lpVtbl->Restore(d3dappi.lpBackBuffer);
			 //D3DAppIClearBuffers();
			}
			else if (ddrval != DD_OK)
			{																					   
			   return; // TRUE; //FALSE;
			}

			return; // TRUE;
		}
	
	

// First we lock both the surfaces.
// =================================


// Back buffer.

	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);
	ddrval = d3dappi.lpBackBuffer->lpVtbl->Lock( d3dappi.lpBackBuffer, NULL, &BBddsd,0,NULL);

// Front buffer.

	memset( &FBddsd,0,sizeof(DDSURFACEDESC)	);
    FBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&FBddsd,d3dappi.lpFrontBuffer);
	ddrval = d3dappi.lpFrontBuffer->lpVtbl->Lock( d3dappi.lpFrontBuffer, NULL, &FBddsd,0,NULL);


// Now all we need are the XY dimensions of each, and the pitch to skip from Y to Y+1 in each.



	BBpitch = BBddsd.lPitch;
	FBpitch = FBddsd.lPitch;

	FlipXSize = BBddsd.dwWidth;						// Get width.
	FlipYSize = BBddsd.dwHeight;					// Get height.

	if (Vmode) FlipXSize<<=1;						// 2 bytes/pixel check.


	SrcBase = BBddsd.lpSurface;
 	DstBase = FBddsd.lpSurface;

	
	
// Alternately display ZBuffer or screen every 32 frames.

//	if (ZShow&128) SrcBase = &ZBuffer[0];
	


	FlipXSize >>= 3;		// Get number of dwords.
	if (FlipXSize==0) goto stoperrors;



 
				__asm

				{
								push	ebp

								mov	ebx,[ BBpitch ]
								mov	ebp,[ FBpitch ]

								mov	edx, [ FlipYSize ]

					allscans:	mov	esi,[ SrcBase ]
								mov	edi,[ DstBase ]
				
								mov	ecx,[ FlipXSize ]
								;------
					
					scanout:
								mov	eax,[esi]
								mov	ebx,[esi+4]

								mov	[edi],eax
								mov	[edi+4],ebx

								add	esi,8
								add	edi,8

								dec	ecx
								jne SHORT scanout

								mov	eax,[ SrcBase ]
								mov	ebx,[ BBpitch ]

								mov	ecx,[ DstBase ]
								add	eax,ebx

								add	ecx,ebp
								mov	[ SrcBase ],eax

								mov	[ DstBase ],ecx
								dec	edx

								jne SHORT allscans

								pop	ebp

								


				}


stoperrors:




#if 0
// Clear ZBuffer.
//=================

	__asm
	{

		push	eax
		push	edi
		push	ecx


			mov	ecx,8000
			mov	eax,0
			lea	edi,ZBuffer

		lp:	mov [edi],eax
			mov [edi+4],eax
			mov [edi+8],eax
			mov [edi+12],eax
			add	edi,16
			dec	ecx
			jne SHORT lp

		pop	ecx
		pop	edi
		pop	eax
	}
#endif



// Now we unlock both the surfaces.
// ================================

	d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );
	d3dappi.lpFrontBuffer->lpVtbl->Unlock( d3dappi.lpFrontBuffer, NULL );



}


















/*


	Clear the ZBuffer in s/w version.







*/



































/*
	====================================================================


	Clear the Z buffer.





	====================================================================
*/






void CWClearZBuffer()
{

DDSURFACEDESC	BBddsd;	// Back buffer surface descriptor.
DDSURFACEDESC	FBddsd;	// Front buffer surface descriptor.
HRESULT			ddrval;	// Crap from ddraw.


// First we lock both the surfaces.
// =================================


// Back buffer.

	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);


// Now all we need are the XY dimensions of each, and the pitch to skip from Y to Y+1 in each.
	

	BBpitch = BBddsd.lPitch;

	FlipXSize = BBddsd.dwWidth;						// Get width.
	FlipYSize = BBddsd.dwHeight;					// Get height.

	FlipXSize<<=1;						// 2 bytes/pixel.


 	DstBase = &ZBuffer[0];

	
	
// Alternately display ZBuffer or screen every 32 frames.

//	if (ZShow&128) SrcBase = &ZBuffer[0];
	


	FlipXSize >>= 2;		// Get number of dwords.
	if (FlipXSize)
	{



 
				__asm

				{
								push	ebp

								mov	ebp,[ BBpitch ]
								mov	edx, [ FlipYSize ]
								mov	ebx,0

					allscans2:	
								mov	edi,[ DstBase ]
				
								mov	ecx,[ FlipXSize ]
								
					
					scanout2:  	mov	[edi],ebx
   								add	edi,4

								dec	ecx
								jne SHORT scanout2


								mov	ecx,[ DstBase ]
								add	ecx,ebp
								mov	[ DstBase ],ecx
								dec	edx

								jne SHORT allscans2

								pop	ebp

								


				}

	}

}



















void CWClearScreen()
{

DDSURFACEDESC	BBddsd;	// Back buffer surface descriptor.
DDSURFACEDESC	FBddsd;	// Front buffer surface descriptor.
HRESULT			ddrval;	// Crap from ddraw.


// First we lock both the surfaces.
// =================================


// Back buffer.

	memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
    BBddsd.dwSize = sizeof(DDSURFACEDESC);
	ddrval = D3DAppIGetSurfDesc(&BBddsd,d3dappi.lpBackBuffer);


// Now all we need are the XY dimensions of each, and the pitch to skip from Y to Y+1 in each.
	
	BBpitch = BBddsd.lPitch;

	FlipXSize = BBddsd.dwWidth;						// Get width.
	FlipYSize = BBddsd.dwHeight;					// Get height.

	FlipXSize<<=1;						// 2 bytes/pixel.




//========

									 
	ddrval = d3dappi.lpBackBuffer->lpVtbl->Lock( d3dappi.lpBackBuffer, NULL, &BBddsd,0,NULL);
	//if (ddrval != D3D_OK ) Msg(" fuck that!");

	if (RenderingSmall)
	{
		SurfWidth>>=2;
		SurfHeight>>=2;

	}

	DstBase = BBddsd.lpSurface;	// CW: get ptr to screen.

//========


	FlipXSize >>= 2;		// Get number of dwords.
	if (FlipXSize)
	{



 
				__asm

				{
								push	ebp

								mov	ebp,[ BBpitch ]
								mov	edx, [ FlipYSize ]
								mov	ebx,0

					allscans2:	
								mov	edi,[ DstBase ]
				
								mov	ecx,[ FlipXSize ]
								
					
					scanout2:  	mov	[edi],ebx
   								add	edi,4

								dec	ecx
								jne SHORT scanout2


								mov	ecx,[ DstBase ]
								add	ecx,ebp
								mov	[ DstBase ],ecx
								dec	edx

								jne SHORT allscans2

								pop	ebp

								


				}

	}

	d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );


}
























/*
	====================================================================



	Render a SMALL camera.




	====================================================================
*/





BOOL CWRenderSmallCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
   //	Msg("Rendering rear-view");
   //	if (zRW) Msg("ZRW"); else Msg("ZW");
		zRW = 0;

	/*	sprintf( &charbuf[0]," xyTL=%d,%d WH=%d,%d",
					CurrentCamera.Viewport.dwX,
					CurrentCamera.Viewport.dwY,
					CurrentCamera.Viewport.dwWidth,
					CurrentCamera.Viewport.dwHeight );
		Msg( &charbuf[0] );
	*/

	// Subtract the TL to make the screen at origin (0,0)..
		

	RenderingSmallXOff = CurrentCamera.Viewport.dwX;
	RenderingSmallYOff = CurrentCamera.Viewport.dwY;
	CurrentCamera.Viewport.dwX -= RenderingSmallXOff;
	CurrentCamera.Viewport.dwY -= RenderingSmallYOff;
	RenderingSmall = 1;

	if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
	{
		RenderingSmall = 0;
		return FALSE;
	}
	else
	{
		RenderingSmall = 0;
	}

	return( TRUE );
}





















/*
	====================================================================


	Move a texture to VRAM.


	====================================================================
*/





BOOL CWMovePPMtoVRAM( TLOADHEADER *Tloadheader, short n, LPDIRECTDRAWSURFACE lpSrcTextureSurf)
{


    LPDIRECT3DTEXTURE lpSrcTexture = NULL;


//-- sw vars ------

	DDSURFACEDESC	BBddsd;
	LPDIRECTDRAWPALETTE SurfPal;
	long x,y,col;	// sw version.
	char *Src, *Dst;
	extern char *TextureAddr[];
	extern long TextureHeights[];
	extern long TextureWidths[];
	long rendpitch, SurfWidth, SurfHeight;
	char charbuf[160];
	HRESULT ddrval;
	PALETTEENTRY TempPalette[256];
	extern	void	MakeClut( long n, unsigned char * Pal, long ck );
	long CK;
	DDCOLORKEY CWCK;

//------------------

	if (SoftwareVersion)
	{

		Dst = TextureAddr[ n*64 ];
		
		// Fill the "VRAM" Texture with shite for now (RED!!).
	





		// Need to lock the source surface to get the data......

		memset( &BBddsd,0,sizeof(DDSURFACEDESC)	);
		BBddsd.dwSize = sizeof(DDSURFACEDESC);
		ddrval = D3DAppIGetSurfDesc(&BBddsd,lpSrcTextureSurf);

		ddrval = lpSrcTextureSurf->lpVtbl->Lock( lpSrcTextureSurf, NULL, &BBddsd,0,NULL);
		if (ddrval != D3D_OK) Msg ("failed lock on moveppm");


		rendpitch = BBddsd.lPitch;

 		SurfWidth  = BBddsd.dwWidth;						// Get width.
		SurfHeight = BBddsd.dwHeight;						// Get height.


		Src = BBddsd.lpSurface;	// CW: get ptr to screen.



	// Now do the copy................................
	// ====================================================


	// These MUST be the same size so thats what we copy.

			for ( y=0; y<SurfHeight; y++)
			{
				for ( x=0; x<SurfWidth; x++)
				{
					*(Dst+(y*256)+x ) = *(Src+(y*rendpitch)+x); //0x7c00;
				}
			}
			
		

		ddrval = lpSrcTextureSurf->lpVtbl->Unlock( lpSrcTextureSurf, NULL );
		if (ddrval != D3D_OK) Msg ("failed unlock on moveppm");



//============================================================================================================

// Create CLUT number N here.....


// Now we know that in 16-bit mode the surfaces are still 8-bit, we can make up the CLUT by using the palette
// associated with the surface.
// We must decompose it into RGB then make the palette up..
// We assume here that the palette is 6:6:6 as usual..

	   	ddrval = lpSrcTextureSurf->lpVtbl->GetPalette( lpSrcTextureSurf, &SurfPal );
		if (ddrval!=DD_OK)
			Msg("MovePPM : Problem getting the palette from Texture surface.");


// read 256 palette entries...


//		ddrval = Tloadheader.lpTextureSurf[n]->lpVtbl->GetEntries( SurfPal, 0, 0, 256, &TempPalette[0] );
		ddrval = SurfPal->lpVtbl->GetEntries( SurfPal, 0, 0, 256, &TempPalette[0] );
		if (ddrval!=DD_OK)
			Msg("MovePPM : Problem getting palette ENTRIES from Texture surface.");
		
	
// Now we have the palette, display it.


		//CK = 0;
		//ddrval = lpSrcTextureSurf->lpVtbl->GetColorKey( lpSrcTextureSurf,DDCKEY_SRCBLT , &CWCK );

	   //	if (ddrval != DD_OK ) Msg(" Problem getting colorkey information for surface.");
		

		//if (ddrval == DDERR_NOCOLORKEY || ddrval == DDERR_NOCOLORKEYHW ) CK = 0;
		//else
		//{
		//	if ( CWCK.dwColorSpaceLowValue == CWCK.dwColorSpaceHighValue ) CK= 1; else CK = 0;
		//}




	  	MakeClut( n, &TempPalette[0], 1 );		// Add this clut to the bunch, also makes 32 fades of it too.
 

		CkClut[ n ] = 1;	// Assume theyre always colorkeyed.

//============================================================================================================
			

		return TRUE;

	}
	
	
}
























/*
	=============================================================================================



	Scan each triangle in this execute buffer and mark each polygon as having holes or not...



	=============================================================================================
*/


long HasHoles;


void	CWScan( D3DEXECUTEBUFFERDESC *debDesc, long NumVertsToXform )
{
D3DLVERTEX	* VertBase;	// Used for uv lookup.
long		CurrentMat,Material;
long		ESrcAdd;			//cw
D3DTRIANGLE *TriPtr;
D3DSTATE	* StatePtr;
D3DBRANCH *		BranchPtr;
D3DPROCESSVERTICES *PVPtr;

long		n;
long		OffPlane;
unsigned	long bright;	// CW: temp for working out flat/gouraud values.
long		iz;	// Used to insert into OT list.
long		ZClip;
float		dz,newdz,ReconX,ReconY,ReconDelta,ReconU,ReconV,ReconX2,ReconY2,ReconU2,ReconV2;
float		ReconR,ReconG,ReconB;
float		ReconR2,ReconG2,ReconB2;
float		R0,G0,B0,R1,G1,B1,R2,G2,B2;
float		dx01,dy01,dz01,warp;
long		Clip,Clip2;
long		a;
long		st;
BOOL		Culling;	// TRUE - CCW culling on.

// Used during decoding verts..
// =============================
VISPOLVERTEX * XF0ptr;
VISPOLVERTEX * XF1ptr;
VISPOLVERTEX * XF2ptr;
D3DLVERTEX	* VB0ptr;
D3DLVERTEX	* VB1ptr;
D3DLVERTEX	* VB2ptr;
char		* ESrc;


float	u0,u1,u2,v0,v1,v2,CentreU,CentreV;
float	Dist0, Dist1, Dist2, NewDist0, NewDist1, NewDist2;

float	UvMove = 1.0F / (256.0F * 4.0F); // 1/4 pixel move for UV fixing.




WORD		v0index;	//cw: temp. tri vertex indices.
WORD		v1index;

WORD		v2index;
WORD		* WordESrc;

WORD		DataUnits;
short		shite;

char		ECommand;
char		CommandSize;

			Culling = TRUE;

			Scanning = 1;




					
			VertBase = (D3DLVERTEX *)debDesc->lpData;		// Base of verts for uv lookup.
				   
			ESrc = (char *) debDesc->lpData;					// Start of execute buffer.
			ESrc += (NumVertsToXform*sizeof(D3DLVERTEX));	// Jump over verts: pt to commands.






/*
	==================================================

		MAIN DECODE LOOP

	==================================================

*/

decode:				ECommand = *ESrc++;

			   

					if ( ECommand == D3DOP_PROCESSVERTICES )
					{
					}


					if ( ECommand == D3DOP_BRANCHFORWARD )
					{
	 				}



					if (ECommand == D3DOP_STATERENDER)
					{
					 	CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc+=2;  						//point at triangle.
					
						// Check for it being a new material to use...
					 	StatePtr = (D3DSTATE *) ESrc;

					   	for ( n = 0 ; n != DataUnits ; n++)
						{
							if (StatePtr->drstRenderStateType == D3DRENDERSTATE_TEXTUREHANDLE)
							{


								CurrentMat = 0;
				

			    				  	for (Material = 0; Material!=128; Material++)
									{
								   		if (Tloadheader.hTex[Material]==StatePtr->dwArg[0])
										{
											 CurrentMat = Material;		
											 goto gotmat;
										}
									}
									Msg(" Couldnt find texture!");
																
							}
							else
							if(StatePtr->drstRenderStateType == D3DRENDERSTATE_BLENDENABLE)
							{
								if (StatePtr->dwArg[0])	TransparentFlag = 1;
								else
														TransparentFlag = 0; //!!!!!!!!!!!!!!!!!!!!!!!! 0!!!!!!!!
							}
							else
							if(StatePtr->drstRenderStateType == D3DRENDERSTATE_CULLMODE)
							{
								if (StatePtr->dwArg[0]== D3DCULL_NONE) Culling = FALSE; //Msg("oi, culling switched off.");
								else
									if (StatePtr->dwArg[0]== D3DCULL_CW) Culling = TRUE; //Msg("oi, culling switched to CW.");
								else
									if (StatePtr->dwArg[0]== D3DCULL_CCW) Culling = TRUE; //Msg("oi, culling switched to CCW.");
						
							}
			gotmat:
						//CurrentMat = 1;

							StatePtr++;
						}
					  	ESrcAdd = DataUnits*CommandSize;		  // Skip last triangle block.
						ESrc += ESrcAdd;
						goto decode;
					}



					
					
					if (ECommand == D3DOP_EXIT) return; //goto DoneDecode;
				
					
					
					// Check for a triangle...
					if (ECommand ==  D3DOP_TRIANGLE)
					{
						CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;

						DataUnits = *WordESrc;
						ESrc+=2;  						//point at triangle.

					// Draw each tri..Addprim each time...
					// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						TriPtr = (D3DTRIANGLE *) ESrc;

					 	for (n=0;n!=DataUnits; n++)
						{
							// Check for FAN / STRIP flags..

							if ((TriPtr->wFlags & 31) == D3DTRIFLAG_START)
							{
						 		v0index = TriPtr->wV1;
								v1index = TriPtr->wV2;
								v2index = TriPtr->wV3;
								goto GotType;
							}

							if ((TriPtr->wFlags & 31) == D3DTRIFLAG_EVEN)
							{
								v1index = v2index;
								v2index = TriPtr->wV3;
								goto GotType;
							}

							if ((TriPtr->wFlags & 31) == D3DTRIFLAG_ODD)
							{
								v0index = v2index;
								v2index = TriPtr->wV3;
							}

GotType:

// Set up some ptrs so I dont have to keep doing array[].this etc...

// At this point, TriPtr->wFlags is the thing we want to update, we want to use 32768 to mean its transparent.
// =============================================================================================================



// Set up a temporary polygon to scan..... XYZ coords are FIXED, always the same.
// UV coords and texture information comes from the polygon.
// We ignore the RGB values and just use flat-shaded....


						XF0ptr = &XformedVerts[v0index];
						XF1ptr = &XformedVerts[v1index];
						XF2ptr = &XformedVerts[v2index];
						
						VB0ptr = VertBase+v0index;
						VB1ptr = VertBase+v1index;
						VB2ptr = VertBase+v2index;



						polyptr = &ScanPoly;

							__asm
							{
								push	eax
								push	ebx
								push	ecx
								push	edi

						  		mov	edi,[ polyptr ]
						
								mov	eax,[ VB0ptr ]
								mov	ebx,[ VB1ptr ]
								mov	ecx,[ VB2ptr ]

						// Texture coords....
		
								fld	 [ eax ]D3DLVERTEX.tu			// tu0
								fimul [ FSixtyFive ]		 		// tu0*65k 
								fld  [ eax ]D3DLVERTEX.tv			// tv0 tu065k 
								fimul [ FSixtyFive ]		 		// tv065k tu065k 
								fld	 [ ebx ]D3DLVERTEX.tu			// tu1 tv065k tu065k 
								fimul [ FSixtyFive ]		 		// tu165k tv065k tu065k
								fld	 [ ebx ]D3DLVERTEX.tv			// tv1 tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tv165k tu165k tv065k tu065k
								fld	 [ ecx ]D3DLVERTEX.tu			// tu2 tv165k tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tu265k tv165k tu165k tv065k tu065k
								fld	 [ ecx ]D3DLVERTEX.tv			// tv2 tu265k tv165k tu165k tv065k tu065k
								fimul [ FSixtyFive ]		 		// tv265k tu265k tv165k tu165k tv065k tu065k

								fxch	st(5)						// tu065k tu265k tv165k tu165k tv065k tv265k
								fistp	[edi]CWPOLYGON.u0			// tu265k tv165k tu165k tv065k tv265k 
								fxch	st(3)						// tv065k tv165k tu165k tu265k tv265k 
								fistp	[edi]CWPOLYGON.v0			// tv165k tu165k tu265k tv265k
	
								mov	eax,[ CurrentMat]

								fistp	[edi]CWPOLYGON.v1
								shl		eax,6
								fistp	[edi]CWPOLYGON.u1
								fistp	[edi]CWPOLYGON.u2
								fistp	[edi]CWPOLYGON.v2
								
								mov		[ edi ]CWPOLYGON.Material,ax
								pop		edi
								pop		ecx
								pop		ebx
								pop		eax

							}


			// Set up the FIXED XY coords, for a large texture we want to make sure we catch em all, so make it 128x128 triangle sized.
			// ========================================================================================================================

							polyptr->x0 = 32;
							polyptr->x1 = 32;
							polyptr->x2 = 0;
							polyptr->y0 = 0;
							polyptr->y1 = 32;
							polyptr->y2 = 32;

							polyptr->z0 = 64;
							polyptr->z1 = 64;
							polyptr->z2 = 64;



			// Set up the RGB values.
							polyptr->r0 = 31<<16;
							polyptr->r1 = 31<<16;
							polyptr->r2 = 31<<16;
			// we will be using a flat-tinted thing so dont worry about the G,B parts...







// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================


/*
	Scale the UV's so they are 1/16th pixel closer to the centroid of the triangle in UV space.
	===========================================================================================

*/



							u0 = VB0ptr->tu;
							u1 = VB1ptr->tu;
							u2 = VB2ptr->tu;

							v0 = VB0ptr->tv;
							v1 = VB1ptr->tv;
							v2 = VB2ptr->tv;



								


// Find the centroid........
	
							CentreU = (u0+u1+u2)/3.0F;
							CentreV = (v0+v1+v2)/3.0F;


							u0 -= CentreU;
							v0 -= CentreV;

							u1 -= CentreU;
							v1 -= CentreV;

							u2 -= CentreU;
							v2 -= CentreV;


// find the dist of each UV pair from the centroid...........

							Dist0 = sqrt( (u0*u0) + (v0*v0) );
							Dist1 = sqrt( (u1*u1) + (v1*v1) );
							Dist2 = sqrt( (u2*u2) + (v2*v2) );

// Now we need this dist to be 1/16th of a pixel less. 

// So we find the NewDist0, NewDist1, NewDist2;

							NewDist0 = Dist0 - (UvMove);	// UvMove is amount to move in by.
							NewDist1 = Dist1 - (UvMove);	// UvMove is amount to move in by.
							NewDist2 = Dist2 - (UvMove);	// UvMove is amount to move in by.

					// Now we scale all the UVs, then add back in the original centroid...


							u0 *= (NewDist0/Dist0);
							u1 *= (NewDist1/Dist1);
							u2 *= (NewDist2/Dist2);

							v0 *= (NewDist0/Dist0);
							v1 *= (NewDist1/Dist1);
							v2 *= (NewDist2/Dist2);


							u0 += CentreU; u1 += CentreU; u2 += CentreU;
							v0 += CentreV; v1 += CentreV; v2 += CentreV;



			// Now put the UV's back into the execute buffer.

							VB0ptr->tu = u0;
							VB1ptr->tu = u1;
							VB2ptr->tu = u2;

							VB0ptr->tv = v0;
							VB1ptr->tv = v1;
							VB2ptr->tv = v2;







							



// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================
// ========================================================================================================================================================









			// Here is where we would TEST the poly...


// Lock the back buffer, set vtables, draw, unlock...


// Check if the CLUT for this texture had a 0,0,0 as entry #0.........
	
// Texture is CurrentMat...find clut number.............




// propogate bit 15 to bit 14....

							if ( TriPtr->wFlags & 32768 ) { TriPtr->wFlags|=16384; TriPtr->wFlags&=0x7fff; }


							if ( CkClut[ CurrentMat ] )
							{
																										 
									PolyType = 0;

									HasHoles = 0;

									DrawPolyFT416( polyptr );

									if (HasHoles) {
												  //	 Msg(" This has colorkeying");
													TriPtr->wFlags |= 32768;
													}

							}

NextPoly:					TriPtr++;		// Next triangle in block.
							

						}

						ESrc += DataUnits*sizeof(D3DTRIANGLE);
						goto decode;


					}
					else
					{
						CommandSize = *ESrc++;
						WordESrc = (WORD *)ESrc;
						DataUnits = *WordESrc;
						ESrc+=2;

						ESrcAdd = CommandSize * DataUnits;
						ESrc+=ESrcAdd;
					}

						goto decode;


DoneDecode:

					Scanning = 0;


					return;
}

















/*
	==========================================================================================


	Scan an execute buffer and flag each polygon as having holes or not.


	Updates the execute buffer.

	
	(This is the same format as an execute command for d3d.)



	==========================================================================================
*/

void	CWScanExecuteBuffer(LPDIRECT3DEXECUTEBUFFER execbuf	)
{
D3DEXECUTEBUFFERDESC debDesc;
D3DEXECUTEDATA execdata;
char	charbuf[160];


			//1. LOCK the execute buffer.
				memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
				debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

				if ( execbuf->lpVtbl->Lock(	execbuf, &debDesc ) != D3D_OK)	return; // FALSE ;


			//2. Get execute data.

				memset(&execdata, 0, sizeof (D3DEXECUTEDATA));
				execdata.dwSize = sizeof(D3DEXECUTEDATA);
				if ( execbuf->lpVtbl->GetExecuteData( execbuf, &execdata) !=D3D_OK ) return;

			//3. Set up a D3DTRANSFORMDATA structure...
					// Test for already xformed verts....


					SetVTables();

					CWScan( &debDesc, execdata.dwVertexCount);

			   		d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );


			//4. Unlock the execute buffer.



					Scanning = 0;

					if ( execbuf->lpVtbl->Unlock(
						execbuf) != D3D_OK)	return; // FALSE;

}




#if 0




/*
	=====================================================================

	Allocate ZBuffer and tell sw version internally that its been allocated.
																			

	=====================================================================
*/


long ZBSize;
short *ZBAddr;

void CWAllocateZBuffer()
{
	
// Find size of back buffer, lpitch * height * 2...
// ================================================



	ZBAddr = malloc( ZBSize );
	ZBAlloced = 1;
}





/*
	=====================================================================

	Free up the ZBuffer, called from CWTidyUp();

	=====================================================================
*/

void CWFreeZBuffer()
{
	ZBAlloced = 0;
}




/*
	=====================================================================


	Tidy up the software version internal stuff.

	Call this before any important actions, ie: level end, exit game, load saved game, etc.


	=====================================================================
*/


#endif




#endif



#endif // top most software enable
