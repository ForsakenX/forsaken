#if GL == 3
#include "render_gl_shared.h"

bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(LVERTEX), GL_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(LVERTEX), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	renderObject->lpNormalBuffer = create_buffer( numNormals * sizeof(NORMAL), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	renderObject->lpNormalBuffer = create_buffer( numNormals * sizeof(NORMAL), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = create_buffer( numIndices * 3 * sizeof(WORD), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = create_buffer( numIndices * 3 * sizeof(WORD), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

// In OpenGL you can only map buffers currently bound to a predefined
// buffer binding point ("target"), so we save the currently bound
// buffer and restore it on unlock.
//
// There is still the restriction that only one buffer of a certain
// type may be locked at the same time. If this is a problem then this
// should be rewritten to use a locally malloc'ed buffer and update
// the real buffer on unlock using glBufferSubData.

static GLuint old_array_buf = 0;
static GLuint old_index_buf = 0;

bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{
	if ( old_array_buf )
	{
		DebugPrintf( "Tried to lock more than one vertex buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_array_buf );
	glBindBuffer( GL_ARRAY_BUFFER, (GLuint) renderObject->lpVertexBuffer );
	*verts = (LVERTEX *) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*verts)
	{
		DebugPrintf("FSLockVertexBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ARRAY_BUFFER, old_array_buf );
	old_array_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals)
{
	if ( old_array_buf )
	{
		DebugPrintf( "Tried to lock more than one vertex buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_array_buf );
	glBindBuffer( GL_ARRAY_BUFFER, (GLuint) renderObject->lpNormalBuffer );
	*normals = (NORMAL *) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*normals)
	{
		DebugPrintf("FSLockNormalBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ARRAY_BUFFER, old_array_buf );
	old_array_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	if ( old_index_buf )
	{
		DebugPrintf( "Tried to lock more than one index buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_index_buf );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, (GLuint) renderObject->lpIndexBuffer );
	*indices = (WORD *) glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*indices)
	{
		DebugPrintf("FSLockIndexBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, old_index_buf );
	old_index_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(TLVERTEX), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{
	return FSLockVertexBuffer( renderObject, (LVERTEX **) verts );
}

/* Draw render object:
 * - if 2D (orthographic), set up appropriately:
 *   - orthographic projection matrix
 *   - ... plus scaling and translation for Y-flipping (T*S*P)
 *   else:
 *   - update mvp if necessary (mvp_needs_update)
 * - for each texture group (renderObject->numTextureGroups)
 *   - group = &renderObject->textureGroups[i]
 *   - if group->colourkey, enable color-keying
 *   - if group->texture, enable texturing and bind
 *     renderObject->textureGroups[group].texture
 *   - draw group->numVerts elements starting at group->startVert
 */

bool draw_render_object( RENDEROBJECT *renderObject, int primitive_type, bool orthographic )
{
	static const struct
	{
		const char *name;
		int components;
		GLenum type;
		GLboolean normalized;
		int offset;
	} normal_attr[] =
	{
		{ "pos",    3, GL_FLOAT,         GL_FALSE, 0  },
		{ "vcolor", 4, GL_UNSIGNED_BYTE, GL_TRUE,  12 }, // 3*float
		{ "vtexc",  2, GL_FLOAT,         GL_FALSE, 16 }, // 3*float + 1*COLOR
		{ NULL,     0, 0,                0,        0  }
	}, ortho_attr[] =
	{
		{ "tlpos",  4, GL_FLOAT,         GL_FALSE, 0 },
		{ "vcolor", 4, GL_UNSIGNED_BYTE, GL_TRUE,  16 }, // 4*float
		{ "vtexc",  2, GL_FLOAT,         GL_FALSE, 20 }, // 4*float + 1*COLOR
		{ NULL,     0, 0,                0,        0  }
	}, *attr;
	//GLuint current_program;
	GLuint u_ortho;
	GLuint u_colorkey;
	GLuint u_enabletex;
	TEXTUREGROUP *group;
	texture_t *texdata;
	int loc;
	int i;

	glBindBuffer( GL_ARRAY_BUFFER, renderObject->lpVertexBuffer );

	if ( renderObject->lpIndexBuffer )
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, renderObject->lpIndexBuffer );
	else
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	//glGetIntegerv( GL_CURRENT_PROGRAM, &current_program );
	// assert( current_program != 0 );

	CHECK_GL_ERRORS;

	// Tell OpenGL about the buffer data layout
	// see the LVERTEX and TLVERTEX definitions inside include/new3d.h
	attr = orthographic ? ortho_attr : normal_attr;
	for ( i=0; attr[i].name; i++ )
	{
		loc = glGetAttribLocation( current_program, attr[i].name );
		if (loc >= 0)
		{
			glVertexAttribPointer(
				loc,
				attr[i].components,
				attr[i].type,
				attr[i].normalized,
				orthographic ? sizeof(TLVERTEX) : sizeof(LVERTEX),
				attr[i].offset
			);
			glEnableVertexAttribArray( loc );
		}
	}

	CHECK_GL_ERRORS;

	// tell it about the normal buffer
	if ( renderObject->lpNormalBuffer )
	{
		glBindBuffer( GL_ARRAY_BUFFER, renderObject->lpNormalBuffer );
		loc = glGetAttribLocation( current_program, "vnormal" );
		if (loc >= 0)
		{
			glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, sizeof(NORMAL), 0 );
			glEnableVertexAttribArray( loc );
		}
	}

	CHECK_GL_ERRORS;

	// Update and use the appropriate model/view/projection matrix
	if ( orthographic )
		ortho_update( current_program );
	else
		mvp_update( current_program );

	// This uniform tells the vertex shader which matrix to use
	if ((u_ortho = glGetUniformLocation(current_program, "orthographic")) >= 0)
		glUniform1i( u_ortho, orthographic ? GL_TRUE : GL_FALSE );

	for ( i = 0; i < renderObject->numTextureGroups; i++ )
	{
		group = &renderObject->textureGroups[i];
		if ( (u_colorkey = glGetUniformLocation(current_program, "colorkeying_enabled")) >= 0 )
			glUniform1i( u_colorkey, group->colourkey ? GL_TRUE : GL_FALSE );
		if ( (u_enabletex = glGetUniformLocation(current_program, "texturing_enabled")) >= 0 )
		{
			glUniform1i( u_enabletex, group->texture ? GL_TRUE : GL_FALSE );
			if ( group->texture )
			{
				texdata = (texture_t *) group->texture;
				glBindTexture( GL_TEXTURE_2D, texdata->id );
			}
		}
		glDrawElementsBaseVertex( primitive_type, group->numTriangles * 3, GL_UNSIGNED_SHORT, group->startIndex * sizeof(WORD), group->startVert );
	}

	CHECK_GL_ERRORS;

	for ( i=0; attr[i].name; i++ )
	{
		loc = glGetAttribLocation( current_program, attr[i].name );
		if (loc >= 0)
			glDisableVertexAttribArray( loc );
	}

	if ( renderObject->lpNormalBuffer )
	{
		loc = glGetAttribLocation( current_program, "vnormal" );
		if (loc >= 0)
			glDisableVertexAttribArray( loc );
	}

	CHECK_GL_ERRORS;

	return true;
}

#endif // GL == 3
