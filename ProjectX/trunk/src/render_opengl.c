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

	glPolygonMode(GL_BACK, GL_NONE);
	
	// wireframe
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


// TODO - is it good form to clean up and set matrix mode to GL_MODELVIEW ?
// load the given matrix to be the current project matrix
// it looks like forsaken already does all the matrix math on it's own
// and the d3d9 api basicaly says it's "loaded" not multiplied against the identity or anything
BOOL FSSetProjection( RENDERMATRIX *matrix )
{
	GLfloat transposed[4][4];
	transpose_matrix(matrix->m,transposed);
	glMatrixMode(GL_PROJECTION);
	//glLoadTransposeMatrixf(&matrix->m);
	//glLoadMatrixf((GLfloat*)transposed[0]);
	glLoadMatrixf((GLfloat*)transposed);
	return TRUE;
}

// i believe this maps to normal model view transformations
// we probably don't want to load identity here and simply want to multiply against the current
BOOL FSSetView( RENDERMATRIX *matrix )
{
	GLfloat transposed[4][4];
	transpose_matrix(matrix->m,transposed);
	glMatrixMode(GL_MODELVIEW);
	//glMultTransposeMatrixf(&matrix->m);
	glMultMatrixf((GLfloat*)transposed);
	return TRUE;
}

// i believe the given matrix should represent a location from the identify
// it's used to jump to a location of an object previously stored
BOOL FSSetWorld( RENDERMATRIX *matrix )
{
	GLfloat transposed[4][4];
	transpose_matrix(matrix->m,transposed);
	glMatrixMode(GL_MODELVIEW);
	//glLoadTransposeMatrixf(&matrix->m);
	glLoadMatrixf((GLfloat*)transposed);
	return TRUE;
}

// i believe this should return a matrix representing the current matrix after multiplications
// meaning that we could return here at any time by loading this matrix
BOOL FSGetWorld(RENDERMATRIX *matrix)
{
	GLfloat f[16];
	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, f);
	// TODO - this copy may not work properly based on col/row major
	memcpy(&matrix->m,f,sizeof(matrix->m));
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

HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(LVERTEX) );
	return TRUE;
}
HRESULT FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{FSCreateVertexBuffer(renderObject, numVertices); return TRUE;}

HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = malloc( numIndices * 3 * sizeof(WORD) );
	return TRUE;
}
HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{return FSCreateIndexBuffer(renderObject,numIndices);}

HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{(*indices) = renderObject->lpIndexBuffer; return TRUE;}
HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{(*verts) = renderObject->lpVertexBuffer; return TRUE;}
HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return TRUE;}
HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}

HRESULT FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(TLVERTEX) ); 
	return TRUE;
}
HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{(void*)(*verts) = (void*)renderObject->lpVertexBuffer; return TRUE;}
HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}


// these should go next
// maybe just draw a triangle or something ?
HRESULT draw_line_object(RENDEROBJECT *renderObject){return TRUE;}
HRESULT draw_2d_object(RENDEROBJECT *renderObject){return TRUE;}
HRESULT draw_object(RENDEROBJECT *renderObject)
{  
	glBegin(GL_TRIANGLES);
		glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
		glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
	glEnd();
	return TRUE;
}

// these can be done later
HRESULT update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}
void release_texture( LPTEXTURE texture ){}
HRESULT FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}

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

const char * render_error_description( HRESULT hr )
{
	GLenum error;
	const GLubyte * str;
	if( ( error = glGetError() ) == GL_NO_ERROR )
		return "";
	str = gluErrorString(error);
	return (const char *) str;
}

#endif // OPENGL
