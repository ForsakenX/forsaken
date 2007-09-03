/*
 * The X Men, July 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 23 $
 *
 * $Header: /PcProjectX/Transexe.c 23    24/10/97 10:01 Collinsd $
 *
 * $Log: /PcProjectX/Transexe.c $
 * 
 * 23    24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 22    23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 21    16/09/97 10:59 Collinsd
 * Added Chris's code
 * 
 * 20    4/08/97 15:36 Collinsd
 * Fixed trans for models.
 * 
 * 19    31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 18    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 17    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 16    6/24/97 5:11p Phillipd
 * 
 * 15    6/24/97 11:12a Phillipd
 * 
 * 14    17-04-97 6:00p Collinsd
 * Models now use processexecmodel()
 * 
 * 13    4/07/97 12:27p Phillipd
 * 
 * 12    24-03-97 12:54p Collinsd
 * changed detail level of explosion.  Changed process model function
 * 
 * 11    19-03-97 11:31a Collinsd
 * 
 * 10    21-01-97 5:34p Collinsd
 * Added the ability to delete bgobjects.
 * 
 * 9     2-01-97 4:22p Collinsd
 * Stealthed orbit pulsars now works.  Orbit pulsar and stealth mantle
 * regeneration bug fixed.
 * 
 * 8     12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 7     4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 6     27/10/96 18:38 Collinsd
 * Optimised loads of shit.
 * 
 * 5     17/10/96 16:43 Collinsd
 * Taken out redundent models.
 * 
 * 4     17/10/96 14:44 Collinsd
 * Explosion and gravgon effects now work properly.
 * 
 * 3     8/29/96 5:38p Phillipd
 * 
 * 2     7/29/96 12:16p Phillipd
 * 
 * 1     7/29/96 11:24a Phillipd
 * 
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include File...	
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

#include "Transexe.h"
#include "models.h"

#ifdef SOFTWARE_ENABLE
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Chris's Code
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	CWExecute2(	LPDIRECT3DDEVICE lpDev,
					LPDIRECT3DEXECUTEBUFFER execbuf,
					LPDIRECT3DVIEWPORT lpView,
					WORD cwFlags);
extern	BOOL	SoftwareVersion;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern D3DMATRIX identity;
extern D3DMATRIXHANDLE hWorld;
extern	MODEL	Models[MAXNUMOFMODELS];
extern	GLOBALSHIP	Ships[MAX_PLAYERS];

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16	NumOfTransExe = 0;
TRANSEXE TransExe[MAXTRANSEXE];




/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Add Translucent Execute Buffer...
		Input	: LPD3DMATRIX  Matrix...Only needed if ya need to do a compunded view Matrix
				  lpExBuf... POinter to Execution Buffer
				  int UseIdentity....set to 0 for normal background...1 for Bikes etc..
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddTransExe( LPD3DMATRIX  Matrix , LPDIRECT3DEXECUTEBUFFER lpExBuf , int UseIdentity, uint16 Model, uint16 group, int16 NumVerts )
{
	if( NumOfTransExe < MAXTRANSEXE	)
	{
		TransExe[NumOfTransExe].UseIdentity = UseIdentity;
		
		if( UseIdentity == 0 )
		{
			TransExe[NumOfTransExe].Matrix = *Matrix;
		}
		TransExe[NumOfTransExe].lpExBuf = lpExBuf;
		TransExe[NumOfTransExe].Model = Model;
		TransExe[NumOfTransExe].NumVerts = NumVerts;
		TransExe[NumOfTransExe].group = group;
		NumOfTransExe++;
	}
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Execute Translucent Execute Buffers for specific group...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ExecuteTransExe( uint16 group )
{
	int16	i;
	uint16	Model;
	BOOL	Display;

	for( i = 0 ; i < NumOfTransExe ; i++ )
	{
		if ( TransExe[i].group == group )
		{
			Display = TRUE;

			if( TransExe[i].Model != (uint16) -1 )
			{
				Model = TransExe[i].Model;
				if( !( Models[ Model ].Flags & MODFLAG_Clip ) )
					continue; // do not display fields clipped

				if( ( Models[ Model ].Flags & MODFLAG_Light ) )
				{
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
		
					case MODFUNC_Regen:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
						}
						break;

					case MODFUNC_ScaleDonut:
//						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );
						break;
					
					case MODFUNC_Nothing:
					default:
						break;
				}
			}

			if( TransExe[i].UseIdentity	== 1 )
			{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
			}else{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &TransExe[i].Matrix);
			}

			if( Display )
			{
#ifdef SOFTWARE_ENABLE
				if( SoftwareVersion )
				{
					CWExecute2( d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
				}
				else
#endif
				{
					d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
				}
			}
		}
	}
	d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
	
}


void ExecuteTransExeUnclipped( uint16 group )
{
	int16	i;
	uint16	Model;
	BOOL	Display;

	for( i = 0 ; i < NumOfTransExe ; i++ )
	{
		if ( TransExe[i].group == group )
		{
			Display = TRUE;

			if( TransExe[i].Model != (uint16) -1 )
			{
				Model = TransExe[i].Model;
				if( ( Models[ Model ].Flags & MODFLAG_Clip ) )
					continue; // do not display clipped models here

				if( ( Models[ Model ].Flags & MODFLAG_Light ) )
				{
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
					case MODFUNC_Regen:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
		
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
						}
						break;

					case MODFUNC_ScaleDonut:
//						ProcessModelExec( TransExe[i].lpExBuf, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 1, 1 );
						break;
					
					case MODFUNC_Nothing:
					default:
						break;
				}
			}
			else
				continue; // do not display non-model translucencies

			if( TransExe[i].UseIdentity	== 1 )
			{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
			}else{
				d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &TransExe[i].Matrix);
			}

			if( Display )
			{
#ifdef SOFTWARE_ENABLE
				if( SoftwareVersion )
				{
					CWExecute2( d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
				}
				else
#endif
				{
					d3dappi.lpD3DDevice->lpVtbl->Execute(d3dappi.lpD3DDevice, TransExe[i].lpExBuf , d3dappi.lpD3DViewport, D3DEXECUTE_CLIPPED);
				}
			}
		}
	}
	d3dappi.lpD3DDevice->lpVtbl->SetMatrix(d3dappi.lpD3DDevice, hWorld, &identity);
	
}



