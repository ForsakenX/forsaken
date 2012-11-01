/*===================================================================
*	l i n e s . c
*	All routines to do with lines...
===================================================================*/
#include "main.h"
#include <stdio.h>
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "lines.h"
#include "camera.h"

extern	CAMERA	CurrentCamera;
extern	bool	DebugInfo;

/*===================================================================
	Globals
===================================================================*/
LINE	Lines[ MAXLINES ];
u_int16_t	FirstLineUsed;
u_int16_t	FirstLineFree;

/*===================================================================
	Procedure	:	Init Line Structures and Execute buffer
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitLines( void )
{
	int i;

	FirstLineUsed = (u_int16_t) -1;
	FirstLineFree = 0;
	
	for( i=0; i < MAXLINES; i++ )
	{
		Lines[i].StartPos.x = 0.0F;
		Lines[i].StartPos.y = 0.0F;
		Lines[i].StartPos.z = 0.0F;
		Lines[i].EndPos.x = 0.0F;
    	Lines[i].EndPos.y = 0.0F;
		Lines[i].EndPos.z = 0.0F;
		Lines[i].StartCol.R = 0;
		Lines[i].StartCol.G = 0;
		Lines[i].StartCol.B = 0;
		Lines[i].EndCol.R = 0;
		Lines[i].EndCol.G = 0;
		Lines[i].EndCol.B = 0;
		Lines[i].Next = i + 1;
		Lines[i].Prev = (u_int16_t) -1;
	}
	Lines[ MAXLINES-1 ].Next = (u_int16_t) -1;
}

/*===================================================================
	Procedure	:	Find a free Line and move it from the free list to
					the used list
	Input		:	Nothing
	Output		:	u_int16_t	Number of the free Line
===================================================================*/
u_int16_t FindFreeLine( void )
{
	u_int16_t i;

	i = FirstLineFree;
	if( i == (u_int16_t) -1 ) return i;
 
	Lines[i].Prev = FirstLineUsed;

	if ( FirstLineUsed != (u_int16_t) -1)
	{
		Lines[ FirstLineUsed ].Next = i;
	}

	FirstLineUsed = i;
	FirstLineFree = Lines[i].Next;

	return i ;
}

/*===================================================================
	Procedure	:	Kill a used Line and move it from the used list
				:	to the free list
	Input		:	u_int16_t		Number of Line to free....
	Output		:	Nothing
===================================================================*/
void KillUsedLine( u_int16_t i )
{
	u_int16_t	its_prev;
	u_int16_t	its_next;
	
	its_prev = Lines[ i ].Prev;
	its_next = Lines[ i ].Next;

	if( i == FirstLineUsed ) FirstLineUsed = Lines[ i ].Prev;
	if( its_prev != (u_int16_t) -1 ) Lines[ its_prev ].Next = its_next;
	if( its_next != (u_int16_t) -1 ) Lines[ its_next ].Prev = its_prev;

	Lines[i].Prev = (u_int16_t) -1;
	Lines[i].Next = FirstLineFree;
	FirstLineFree	= i;
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	u_int16_t	Group
				:	LPDIRECT3DEXECUTEBUFFER ExecBuffer
				:	u_int16_t	* StartLine
	Output		:	Nothing
===================================================================*/
bool LinesDispGroup( u_int16_t Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, u_int16_t * StartLine )
{
	LPLVERTEX	Vert_Ptr;
	COLOR		color;
	int16_t			Num_Lines;
	u_int16_t			i;

	if ( !DebugInfo )
	{
		*StartLine = (u_int16_t) -1;
		return false; // don't display lines if not debugging
	}

	renderObject->numTextureGroups = 0;

	if( *StartLine != (u_int16_t) -1 )
	{
		if (!(FSLockVertexBuffer(renderObject, &Vert_Ptr)))
		{
			return false;
		}

		Num_Lines = 0;

		i = *StartLine;

		while( i != (u_int16_t) -1 )
		{
			if( Lines[ i ].Group == Group )
			{
				/* bjd curr driver = 0 use to be software mode */
				//if(render_info.CurrDriver != 0)
				{
						color = RGBA_MAKE( Lines[ i ].StartCol.R, Lines[ i ].StartCol.G,
										   Lines[ i ].StartCol.B, Lines[ i ].StartTrans );
				}
				/*
				else
				{
					color = RGBA_MAKE( 128, 128, 128, 255 );
				}
				*/
	
				Vert_Ptr->x = Lines[ i ].StartPos.x;
				Vert_Ptr->y = Lines[ i ].StartPos.y;
				Vert_Ptr->z = Lines[ i ].StartPos.z;
				Vert_Ptr->tu = 0.0F;
				Vert_Ptr->tv = 0.0F;
				Vert_Ptr->color = color;
//				Vert_Ptr->dwReserved = 0;
				Vert_Ptr++;
	
				/* bjd curr driver = 0 use to be software mode */
				//if(render_info.CurrDriver != 0)
				{
						color = RGBA_MAKE( Lines[ i ].EndCol.R, Lines[ i ].EndCol.G,
										   Lines[ i ].EndCol.B, Lines[ i ].EndTrans );
				}
				/*
				else
				{
					color = RGBA_MAKE( 128, 128, 128, 255 );
				}
				*/
	
				Vert_Ptr->x = Lines[ i ].EndPos.x;
				Vert_Ptr->y = Lines[ i ].EndPos.y;
				Vert_Ptr->z = Lines[ i ].EndPos.z;
				Vert_Ptr->tu = 0.0F;
				Vert_Ptr->tv = 0.0F;
				Vert_Ptr->color = color;
//				Vert_Ptr->dwReserved = 0;
				Vert_Ptr++;

				Num_Lines++;

				if( Num_Lines >= 400 ) break;
			}

			i = Lines[ i ].Prev;
		}

/*	old render states passed to d3dline , might be needed later for reference

			OP_STATE_RENDER( 2, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPU, false, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPV, false, lpPointer );
	   		OP_PROCESS_VERTICES( 1, lpPointer );
	   		    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, 0, ( Num_Lines * 2 ), lpPointer );
	   		OP_STATE_RENDER( 1, lpPointer );
	   		    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer );
			OP_LINE_LIST( Num_Lines, lpPointer );

*/

		FSUnlockVertexBuffer(renderObject);

		*StartLine = i;
		
		if( Num_Lines == 0 ) 
			return false;
		
		renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = Num_Lines; // each line is a triangle
		renderObject->textureGroups[renderObject->numTextureGroups].numVerts = Num_Lines * 2; // 3 verts in a triangle
		renderObject->textureGroups[renderObject->numTextureGroups].startVert = 0;
		renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
		renderObject->textureGroups[renderObject->numTextureGroups].colourkey = false;
		renderObject->numTextureGroups = 1;

	}
	else
	{
		return false;
	}

	return true;
}


bool ExecuteLines( u_int16_t group, RENDEROBJECT *renderObject ) 
{
	u_int16_t  i = 0;

	//  if( !render_info.bIsPrimary )
	//		return true;

	i = FirstLineUsed;
	while( i != (u_int16_t) -1 )
	{
		if( LinesDispGroup( group, renderObject, &i ) )
		{
			if (!draw_line_object(renderObject))
				return false;
		}
	}

	return true;
}

