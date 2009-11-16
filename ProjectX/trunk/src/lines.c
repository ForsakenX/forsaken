/*===================================================================
*	l i n e s . c
*	All routines to do with lines...
===================================================================*/
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
extern	BOOL	DebugInfo;

/*===================================================================
	Globals
===================================================================*/
LINE	Lines[ MAXLINES ];
uint16	FirstLineUsed;
uint16	FirstLineFree;

/*===================================================================
	Procedure	:	Init Line Structures and Execute buffer
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitLines( void )
{
	int i;

	FirstLineUsed = (uint16) -1;
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
		Lines[i].Prev = (uint16) -1;
	}
	Lines[ MAXLINES-1 ].Next = (uint16) -1;
}

/*===================================================================
	Procedure	:	Find a free Line and move it from the free list to
					the used list
	Input		:	Nothing
	Output		:	uint16	Number of the free Line
===================================================================*/
uint16 FindFreeLine( void )
{
	uint16 i;

	i = FirstLineFree;
	if( i == (uint16) -1 ) return i;
 
	Lines[i].Prev = FirstLineUsed;

	if ( FirstLineUsed != (uint16) -1)
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
	Input		:	uint16		Number of Line to free....
	Output		:	Nothing
===================================================================*/
void KillUsedLine( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;
	
	its_prev = Lines[ i ].Prev;
	its_next = Lines[ i ].Next;

	if( i == FirstLineUsed ) FirstLineUsed = Lines[ i ].Prev;
	if( its_prev != (uint16) -1 ) Lines[ its_prev ].Next = its_next;
	if( its_next != (uint16) -1 ) Lines[ its_next ].Prev = its_prev;

	Lines[i].Prev = (uint16) -1;
	Lines[i].Next = FirstLineFree;
	FirstLineFree	= i;
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	uint16	Group
				:	LPDIRECT3DEXECUTEBUFFER ExecBuffer
				:	uint16	* StartLine
	Output		:	Nothing
===================================================================*/
BOOL LinesDispGroup( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, uint16 * StartLine )
{
	LPLVERTEX	Vert_Ptr;
	D3DCOLOR		color;
	D3DCOLOR		specular;
	int16			Num_Lines;
	uint16			i;

	if ( !DebugInfo )
	{
		*StartLine = (uint16) -1;
		return FALSE; // don't display lines if not debugging
	}

	specular = RGB_MAKE( 255, 255, 255 );
	/* bjd curr driver = 0 use to be software mode
	if(render_info.CurrDriver != 0)	specular = RGB_MAKE( 255, 255, 255 );
	else specular = RGB_MAKE( 128, 128, 128 );
	*/
	
	renderObject->numTextureGroups = 0;

	if( *StartLine != (uint16) -1 )
	{
/*
		memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
//		if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc ) != D3D_OK) return FALSE; // bjd
//		if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc ) != D3D_OK)
//			return FALSE;
		if (FAILED(FSLockVertexBuffer(renderObject, &Vert_Ptr)))
		{
			return FALSE;
		}

		Num_Lines = 0;

		i = *StartLine;

		while( i != (uint16) -1 )
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
				Vert_Ptr->specular = specular;
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
				Vert_Ptr->specular = specular;
//				Vert_Ptr->dwReserved = 0;
				Vert_Ptr++;

				Num_Lines++;

				if( Num_Lines >= 400 ) break;
			}

			i = Lines[ i ].Prev;
		}

/*	old render states passed to d3dline , might be needed later for reference

			OP_STATE_RENDER( 2, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPU, FALSE, lpPointer );
			    STATE_DATA( D3DRENDERSTATE_WRAPV, FALSE, lpPointer );
	   		OP_STATE_LIGHT( 1, lpPointer );
	   		    STATE_DATA( D3DLIGHTSTATE_MATERIAL, 0, lpPointer );
	   		OP_PROCESS_VERTICES( 1, lpPointer );
	   		    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, 0, ( Num_Lines * 2 ), lpPointer );
	   		OP_STATE_RENDER( 1, lpPointer );
	   		    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer );
			OP_LINE_LIST( Num_Lines, lpPointer );

*/

		FSUnlockVertexBuffer(renderObject);

		*StartLine = i;
		
		if( Num_Lines == 0 ) 
			return FALSE;
/*
		memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
		ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
		ExecBuffer_d3dexdata.dwVertexCount = ( Num_Lines * 2 );
		ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
		ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
		if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return FALSE;
*/
		
		renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = Num_Lines; // each line is a triangle
		renderObject->textureGroups[renderObject->numTextureGroups].numVerts = Num_Lines * 2; // 3 verts in a triangle
		renderObject->textureGroups[renderObject->numTextureGroups].startVert = 0;
		renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
		renderObject->textureGroups[renderObject->numTextureGroups].colourkey = FALSE;
		renderObject->numTextureGroups = 1;

	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL ExecuteLines( uint16 group, RENDEROBJECT *renderObject ) 
{
	uint16  i = 0;

	//  if( !render_info.bIsPrimary )
	//		return TRUE;

	i = FirstLineUsed;
	while( i != (uint16) -1 )
	{
		if( LinesDispGroup( group, renderObject, &i ) )
		{
			if (FAILED(draw_line_object(renderObject)))
				return FALSE;
		}
	}

	return TRUE;
}

