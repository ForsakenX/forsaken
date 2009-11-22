#ifdef OPENGL

#include "main.h"
#include "util.h"
#include "render.h"
#include "SDL.h"
#include "SDL_opengl.h"

extern render_info_t render_info;

// TODO - does gl even have such a concept?
BOOL FSBeginScene(){ return TRUE; }
BOOL FSEndScene(){ return TRUE; }

// prototypes
void reset_trans( void );

// TODO - port functions to not use hresult
typedef long HRESULT;

// TODO - all textures should be scaled to square
//		  other wise should get this from gl caps
BOOL bSquareOnly = FALSE;

// these can be done later
BOOL update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}
void release_texture( LPTEXTURE texture ){}
BOOL FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}

// will this even be needed?
void render_gamma_correction( double gamma )
{
}

BOOL init_renderer( render_info_t * info )
{
	DebugPrintf( "gl vendor='%s', renderer='%s', version='%s', shader='%s'\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	// this is way to long for OutputDebugString to show
	//DebugPrintf( "gl vendor='%s', renderer='%s', version='%s', extensions='%s', shaders='%s'\n",
	//	glGetString(GL_EXTENSIONS));

	// reminaing d3d9 initialization steps
	// doesn't sdl do this now?
		// enumerate and select a display mode
			// assign enumeration info to info->Mode
			// assign info->(CurrMode|ThisMode|WindowsDisplay|szClient)
			// set the viewport

	//
	// init render state
	//

	render_gamma_correction(1.0f);
	glShadeModel(GL_SMOOTH); // TODO - is there gouraud ?
	glDisable(GL_LIGHTING);
	reset_cull();
	reset_trans();
	reset_filtering();

	// normally we don't draw back faces
	glPolygonMode(GL_BACK, GL_NONE);
	
	// wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	set_normal_states();

	//
	// everything went ok
	// fill in all the structures
	//

	info->bRenderingIsOK	= TRUE;
	info->NumModes			= 1;
	info->CurrMode			= 0;
	info->Mode				= (render_display_mode_t *) malloc(sizeof(render_display_mode_t));
	info->Mode[0].h			= render_info.default_mode.h;
	info->Mode[0].w			= render_info.default_mode.w;
	info->Mode[0].rate		= render_info.default_mode.rate;
	info->Mode[0].bpp		= render_info.default_mode.bpp;
	info->ThisMode			= info->Mode[ info->CurrMode ];
	info->WindowsDisplay	= info->Mode[ info->CurrMode ];
	info->szClient.cx		= info->ThisMode.w;
	info->szClient.cy		= info->ThisMode.h;
	info->WindowsDisplay.w  = info->ThisMode.w;
	info->WindowsDisplay.h  = info->ThisMode.h;
	info->aspect_ratio		= (float) info->ThisMode.w / (float) info->ThisMode.h;

	{
		render_viewport_t viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = info->ThisMode.w;
		viewport.Height = info->ThisMode.h;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		FSSetViewPort(&viewport);
	}

	return TRUE;
}

void render_cleanup( render_info_t * info )
{
    info->bRenderingIsOK = FALSE;
	if(info->Mode)
		free(info->Mode);
    // TODO - any opengl cleanup required ?
	//		i believe SDL_Quit() handles everything
	//		and at this point we might just be changing resolutions
	//		so sdl again may handle any context cleanup needed
}

BOOL render_mode_select( render_info_t * info )
{
	render_cleanup( info );
	if(!init_renderer( info ))
		return FALSE;
	return TRUE;
}

// TODO - in d3d9 render_flip would detect a lost device
//		lost as in alt+tab (etc) which caused video memory to dump
//		at this point we should set needs_reset = TRUE

static BOOL needs_reset = FALSE;

BOOL render_reset( render_info_t * info )
{
	if(!needs_reset)
		return FALSE;
	if(!render_mode_select( info ))
		return FALSE;
	needs_reset = FALSE;
	return TRUE;
}

BOOL render_flip( render_info_t * info )
{
	SDL_GL_SwapBuffers();
	return TRUE;
}

void reset_trans( void )
{
	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ZERO); // src, dest
}

void reset_zbuff( void )
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE); // depth write
}

void disable_zbuff_write( void )
{
	glDepthMask(GL_FALSE); // depth write
}

void disable_zbuff( void )
{
	glDisable(GL_DEPTH_TEST);
}

void reset_filtering( void )
{
	// texture and mipmap filter
	// TODO - anisotropic texture filtering ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR // interpolates two nearest mip levels
		//GL_LINEAR
		);
}

void cull_none( void )
{
	glDisable(GL_CULL_FACE);
}

void cull_cw( void )
{
	glCullFace(GL_FRONT); // cw is the front for us
}

void reset_cull( void )
{	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
}

void set_alpha_ignore( void )
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,(DWORD)100);
}

void unset_alpha_ignore( void )
{
	glDisable(GL_ALPHA_TEST);
}

void set_normal_states( void )
{
	reset_zbuff();
	reset_trans();
}

void set_trans_state_9()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); // src, dest
}

void set_alpha_states( void )
{
	disable_zbuff_write();
	glEnable(GL_BLEND);
	set_trans_state_9();
}

void set_alpha_fx_states( void )
{
	glEnable(GL_BLEND);
	// TODO - need some type of states here that would create white out affect
}

// TODO - is the stencil buffer ever cleared ?
// TODO - do we even use the stencil buffer ?
// TODO - FSClear is meant to clear current viewport
//        perhaps we can automate and remove need for rect arg ?
// TODO - these functions are clearing the whole screen not the rect !

// clears color/zbuff same time to opaque black
BOOL FSClear(XYRECT * rect)
{	
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	return TRUE;
}

BOOL FSClearDepth(XYRECT * rect)
{
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	return TRUE;
}

BOOL FSClearBlack(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return TRUE;
}

// TODO - conversion from bottom/left to top/left is totaly broken

BOOL FSGetViewPort(render_viewport_t *view)
{
	GLint i[4];
	GLfloat f[2];
	// scalex/y are not modified here
	// xywh
	glGetIntegerv( GL_VIEWPORT, i );
	view->X	= i[0];
	view->Y	= render_info.ThisMode.h - (i[1] + i[3]);
	view->Width	= i[2];
	view->Height = i[3];
	// near,far
	glGetFloatv( GL_DEPTH_RANGE, f );
	view->MinZ = f[0];
	view->MaxZ = f[1];
	return TRUE;
}

BOOL FSSetViewPort(render_viewport_t *view)
{
	// render_viewport_t x/y starts top/left
	// but glViewport starts from bottom/left
	int bottom = render_info.ThisMode.h - (view->Y + view->Height);
	glViewport(	view->X, bottom, (GLint) view->Width, (GLint) view->Height	);
	// sets the min/max depth values to render
	// default is max 1.0f and min 0.0f
	// this is here for compatibility with d3d9
	glDepthRange(view->MinZ,view->MaxZ);
	// i want to know if this is ever changed
	// as most likely we don't need this info in render_viewport_t
	if(view->MaxZ!=1.0f || view->MinZ!=0.0f)
	{
		DebugPrintf("-------------------------------\n");
		DebugPrintf("max/min z used: max=%d min=%d\n",
			view->MaxZ, view->MinZ);
		DebugPrintf("-------------------------------\n");
	}
	// ScaleX|Y are not even part of the d3d9 struct anymore
	// they were part of the old d3d6 viewport settings
	// from testing d3d9 passes the values along untouched
	// probably need some testing here to see how d3d6 worked
	// forsaken still uses these values so we need them
	return TRUE;
}

// TODO - might have to use non tranpose functions in set/get world/view/proj

// i could not get glext.h to work 
static void transpose_matrix( GLfloat m1[4][4], GLfloat m2[4][4] )
{
        int a, b;
        for( a = 0; a < 4; a++ )
                for( b = 0; b < 4; b++ )
                        m2[a][b] = m1[b][a];
}

BOOL FSSetProjection( RENDERMATRIX *matrix )
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&matrix->m);
	return TRUE;
}

GLfloat view_matrix[4][4];
BOOL FSSetView( RENDERMATRIX *matrix )
{
	memcpy(&view_matrix,&matrix->m,sizeof(view_matrix));
	return TRUE;
}

GLfloat world_matrix[4][4];
BOOL FSSetWorld( RENDERMATRIX *matrix )
{	
	memcpy(&world_matrix,&matrix->m,sizeof(world_matrix));
	return TRUE;
}

BOOL FSGetWorld(RENDERMATRIX *matrix)
{
	memcpy(&matrix->m,&world_matrix,sizeof(matrix->m));
	return TRUE;
}

//
// using the concept of index/vertex buffers in opengl is a bit different
// so for now I'm just going going to return a pointer to memory
// then just convert the index/vertex buffer to a pure vertex buffer on draw
//
// the vertex/index pointers could probably end up being the glGen* id
// then internal display lists can be generated for the non dynamic functions
// so we can render the static objects as display lists
//

BOOL FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(LVERTEX) );
	return TRUE;
}
BOOL FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{FSCreateVertexBuffer(renderObject, numVertices); return TRUE;}

BOOL FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = malloc( numIndices * 3 * sizeof(WORD) );
	return TRUE;
}
BOOL FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{return FSCreateIndexBuffer(renderObject,numIndices);}

BOOL FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{(*indices) = renderObject->lpIndexBuffer; return TRUE;}
BOOL FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{(*verts) = renderObject->lpVertexBuffer; return TRUE;}
BOOL FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return TRUE;}
BOOL FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}

BOOL FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(TLVERTEX) ); 
	return TRUE;
}
BOOL FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{(void*)(*verts) = (void*)renderObject->lpVertexBuffer; return TRUE;}

static void set_color( COLOR c )
{
	glColor4f(
		(GLfloat)RGBA_GETRED(c),
		(GLfloat)RGBA_GETGREEN(c),
		(GLfloat)RGBA_GETBLUE(c),
		(GLfloat)RGBA_GETALPHA(c)
	);
}

static void draw_vert( void * _vert, int tlvertex )
{
	LVERTEX * vert = (LVERTEX*) _vert;
	TLVERTEX * tlvert = (TLVERTEX*) _vert;
	if(tlvertex)
	{
		set_color( tlvert->color );
		glTexCoord2f( tlvert->tu, tlvert->tv );
		glVertex4f( tlvert->x, tlvert->y, tlvert->z, tlvert->w );
	}
	else
	{
		set_color( vert->color );
		glTexCoord2f( vert->tu, vert->tv );
		glVertex3f( vert->x, vert->y, vert->z );
	}
}

static void set_material( RENDERMATERIAL * m )
{
	if(!m)return;
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	 (GLfloat*)&m->Diffuse  );
	glMaterialfv( GL_FRONT, GL_AMBIENT,	 (GLfloat*)&m->Ambient  );
	glMaterialfv( GL_FRONT, GL_SPECULAR, (GLfloat*)&m->Specular );
	glMaterialfv( GL_FRONT, GL_EMISSION, (GLfloat*)&m->Emissive );
	// gl specular exponent =~ d3d specular sharpness
	glMaterialf ( GL_FRONT, GL_SHININESS,(GLfloat)m->Power      );
}

static BOOL draw_indexed_list( RENDEROBJECT *renderObject, int primitive_type, BOOL tlvertex )
{
	int group;
	LVERTEX * verts = (LVERTEX*) renderObject->lpVertexBuffer;
	TLVERTEX * tlverts = (TLVERTEX*) renderObject->lpVertexBuffer;
	WORD * indices = (WORD*) renderObject->lpIndexBuffer;

	assert(renderObject->vbLocked == 0);
	
	if(!tlvertex)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf((GLfloat*)&world_matrix);
		glMultMatrixf((GLfloat*)&view_matrix);
	}

	set_material( &renderObject->material );

	for (group = 0; group < renderObject->numTextureGroups; group++)
	{
		int i;
		int startVert  = renderObject->textureGroups[group].startVert;
		int numVerts   = renderObject->textureGroups[group].numVerts;

		/*
		if(renderObject->textureGroups[group].colourkey)
			set_alpha_ignore();
		*/

		if( renderObject->textureGroups[group].texture )
		{
			GLuint texture = *(GLuint*)renderObject->textureGroups[group].texture;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		glBegin(primitive_type);

		// draw vertex list using index list
		if(renderObject->lpIndexBuffer)
		{
			int startIndex = renderObject->textureGroups[group].startIndex;
			int numIndices = renderObject->textureGroups[group].numTriangles * 3;
			for( i = 0; i < numIndices; i++ )
			{
				int indice = indices[ startIndex + i ];
				int vert = startVert + indice;
				draw_vert( &verts[vert], tlvertex );
			}
		}
		// draw only vertex list
		else
		{
			for( i = startVert; i < numVerts; i++ )
				draw_vert( &verts[i], tlvertex );
		}
		
		glEnd();

		if( renderObject->textureGroups[group].texture )
			glDisable(GL_TEXTURE_2D);

		/*
		if(renderObject->textureGroups[group].colourkey)
			unset_alpha_ignore();
			*/
	}

	return TRUE;
}

BOOL draw_object(RENDEROBJECT *renderObject){return draw_indexed_list(renderObject,GL_TRIANGLES,FALSE);}
BOOL draw_2d_object(RENDEROBJECT *renderObject){return draw_indexed_list(renderObject,GL_TRIANGLES,TRUE);}
BOOL draw_line_object(RENDEROBJECT *renderObject){return draw_indexed_list(renderObject,GL_LINES,FALSE);}

void FSReleaseRenderObject(RENDEROBJECT *renderObject)
{
	int i;
	if (renderObject->lpVertexBuffer)
	{
		// TODO - need to destroy buffer gl style
		free(renderObject->lpVertexBuffer);
		renderObject->lpVertexBuffer = NULL;
	}
	if (renderObject->lpIndexBuffer)
	{
		// TODO - need to destroy buffer gl style
		free(renderObject->lpIndexBuffer);
		renderObject->lpIndexBuffer = NULL;
	}
	for (i = 0; i < renderObject->numTextureGroups; i++)
	{
		renderObject->textureGroups[i].numVerts = 0;
		renderObject->textureGroups[i].startVert = 0;

		if (renderObject->textureGroups[i].texture)
		{
			// this is just a pointer to Tloadheader
			// we do not need to worry about releasing it
			renderObject->textureGroups[i].texture = NULL;
		}
	}
}

const char * render_error_description( int e )
{
	GLenum error;
	const GLubyte * str;
	if( ( error = glGetError() ) == GL_NO_ERROR )
		return "";
	str = gluErrorString(error);
	return (const char *) str;
}

#endif // OPENGL
