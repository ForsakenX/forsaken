
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

#include "transexe.h"
#include "models.h"

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

		TransExe[NumOfTransExe].render_lighting_use_only_light_color_and_blend = render_lighting_use_only_light_color_and_blend;
		TransExe[NumOfTransExe].render_lighting_use_only_light_color = render_lighting_use_only_light_color;
		TransExe[NumOfTransExe].render_lighting_point_lights_only = render_lighting_point_lights_only;
		TransExe[NumOfTransExe].render_lighting_enabled = render_lighting_enabled;
		TransExe[NumOfTransExe].render_light_ambience = render_light_ambience;
		TransExe[NumOfTransExe].render_light_ambience_alpha = render_light_ambience_alpha;
		TransExe[NumOfTransExe].render_color_blend_red = render_color_blend_red;
		TransExe[NumOfTransExe].render_color_blend_green = render_color_blend_green;
		TransExe[NumOfTransExe].render_color_blend_blue = render_color_blend_blue;

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

	    render_lighting_enabled = TransExe[i].render_lighting_enabled;
	    render_lighting_use_only_light_color_and_blend = TransExe[i].render_lighting_use_only_light_color_and_blend;
 		  render_lighting_use_only_light_color = TransExe[i].render_lighting_use_only_light_color;
    	render_lighting_point_lights_only = TransExe[i].render_lighting_point_lights_only;
 		  render_light_ambience = TransExe[i].render_light_ambience;
   		render_light_ambience_alpha = TransExe[i].render_light_ambience_alpha;
	    render_color_blend_red = TransExe[i].render_color_blend_red;
 		  render_color_blend_green = TransExe[i].render_color_blend_green;
   		render_color_blend_blue = TransExe[i].render_color_blend_blue;

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
				FSSetWorld(&identity);
			}
			else
			{
				FSSetWorld(&TransExe[i].Matrix);
			}

			if( Display )
					draw_object(&TransExe[i].renderObject);

			render_reset_lighting_variables();
		}
	}
	render_reset_lighting_variables();
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

	    render_lighting_enabled = TransExe[i].render_lighting_enabled;
	    render_lighting_use_only_light_color_and_blend = TransExe[i].render_lighting_use_only_light_color_and_blend;
 		  render_lighting_use_only_light_color = TransExe[i].render_lighting_use_only_light_color;
    	render_lighting_point_lights_only = TransExe[i].render_lighting_point_lights_only;
 		  render_light_ambience = TransExe[i].render_light_ambience;
   		render_light_ambience_alpha = TransExe[i].render_light_ambience_alpha;
	    render_color_blend_red = TransExe[i].render_color_blend_red;
 		  render_color_blend_green = TransExe[i].render_color_blend_green;
   		render_color_blend_blue = TransExe[i].render_color_blend_blue;

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
				FSSetWorld(&identity);
			}
			else
			{
				FSSetWorld(&TransExe[i].Matrix);
			}

			if( Display )
					draw_object(&TransExe[i].renderObject);

			render_reset_lighting_variables();
		}
	}
	render_reset_lighting_variables();
	FSSetWorld(&identity);
}
