
/*===================================================================
		Include File...	
===================================================================*/
#include <stdio.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"

#include "transexe.h"
#include "models.h"
#include "xmem.h"

/*===================================================================
		Externals...
===================================================================*/
extern RENDERMATRIX identity;
extern MODEL Models[MAXNUMOFMODELS];
extern GLOBALSHIP Ships[MAX_PLAYERS+1];

/*===================================================================
		Globals...
===================================================================*/
int16	NumOfTransExe = 0;
TRANSEXE TransExe[MAXTRANSEXE];


/*===================================================================
	Add Translucent Execute Buffer...
	Input	: LPD3DMATRIX  Matrix...Only needed if ya need to do a compunded view Matrix
			  lpExBuf... Pointer to Execution Buffer
			  int UseIdentity....set to 0 for normal background...1 for Bikes etc..
===================================================================*/
void AddTransExe( /*LPD3DMATRIX Matrix*/RENDERMATRIX *Matrix , /*LPDIRECT3DEXECUTEBUFFER lpExBuf*/RENDEROBJECT *renderObject , int UseIdentity, uint16 Model, uint16 group, int16 NumVerts )
{
	if( NumOfTransExe < MAXTRANSEXE	)
	{
		TransExe[NumOfTransExe].UseIdentity = UseIdentity;
		
		if( UseIdentity == 0 )
		{
			TransExe[NumOfTransExe].Matrix = *Matrix;
		}
//		TransExe[NumOfTransExe].lpExBuf = lpExBuf;
		TransExe[NumOfTransExe].renderObject = (*renderObject);
		TransExe[NumOfTransExe].Model = Model;
		TransExe[NumOfTransExe].NumVerts = NumVerts;
		TransExe[NumOfTransExe].group = group;
		NumOfTransExe++;
	}
}


/*===================================================================
		Execute Translucent Execute Buffers for specific group...
===================================================================*/
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
#ifndef NEW_LIGHTING
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
#endif
					render_lighting_enabled = 1;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
		
					case MODFUNC_Regen:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
#ifdef NEW_LIGHTING
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
#endif
							render_lighting_enabled = 1;
							render_light_ambience_alpha = 0.0f;
							render_light_ambience_alpha_enable = 1;
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
				FSSetWorld(&identity);
			}
			else
			{
				FSSetWorld(&TransExe[i].Matrix);
			}

			if( Display )
					draw_object(&TransExe[i].renderObject);

			render_lighting_enabled = 0;
			render_light_ambience_alpha = 255.0f;
			render_light_ambience_alpha_enable = 0;
		}
	}
	FSSetWorld(&identity);
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
#ifndef NEW_LIGHTING
					if( !LightModel2( Models[ Model ].ModelNum, &Models[ Model ].Pos ) ) Display = FALSE;
#endif
					render_lighting_enabled = 1;
				}

				switch( Models[ Model ].Func )
				{
					case MODFUNC_Explode:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 1, 0, 0 );
						break;
					case MODFUNC_Regen:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 1, 0 );
						break;
		
					case MODFUNC_Scale:
					case MODFUNC_Scale2:
						ProcessModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, Models[ Model ].Scale, Models[ Model ].MaxScale, 0, 0, 1 );
						break;

					case MODFUNC_SphereZone:
						ProcessSphereZoneModelExec( /*TransExe[i].lpExBuf*/&TransExe[i].renderObject, TransExe[i].NumVerts, (uint8) Models[ Model ].Red, (uint8) Models[ Model ].Green, (uint8) Models[ Model ].Blue );
						break;

					case MODFUNC_OrbitPulsar:
						if( ( Ships[ Models[ Model ].Ship ].Object.Flags & SHIP_Stealth ) )
						{
#ifndef NEW_LIGHTING
							if( !LightModel( Model, &Models[ Model ].Pos ) ) Display = FALSE;
#endif
							render_lighting_enabled = 1;
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
				FSSetWorld(&identity);
			}
			else
			{
				FSSetWorld(&TransExe[i].Matrix);
			}

			if( Display )
					draw_object(&TransExe[i].renderObject);

			render_lighting_enabled = 0;
		}
	}
	FSSetWorld(&identity);
}
