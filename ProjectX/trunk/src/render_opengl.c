#ifdef OPENGL

#include "main.h"
#include "util.h"
#include "render.h"
#include "SDL.h"
#include "SDL_opengl.h"

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
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);

	set_normal_states();

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return TRUE;
}

// TODO - how do i query for these values ?
//        can i simply return my last Set ?
BOOL FSGetViewPort(render_viewport_t *returnViewPort)
{
	// scalex/y are not modified here
	return TRUE;
}

BOOL FSSetViewPort(render_viewport_t *view)
{
	// render_viewport_t x/y starts top/left
	// but glViewport starts from bottom/left
	int bottom = view->Y + view->Height;
	glViewport(
		view->X, bottom, 
		(GLint) view->Width, (GLint) view->Height
	);
	// sets the min/max depth values to render
	// default is max 1.0f and min 0.0f
	// this is here for compatibility with d3d9
	glDepthRange(view->MinZ,view->MaxZ);
	// i want to know if this is ever changed
	// as most likely we don't need this info in render_viewport_t
	if(view->MaxZ!=1.0f || view->MinZ!=0.0f)
	{
		DeubgPrintf("-------------------------------\n");
		DebugPrintf("max/min z used: max=%d min=%d\n",
			view->MaxZ, view->MinZ);
		DeubgPrintf("-------------------------------\n");
	}
	// ScaleX|Y are not even part of the d3d9 struct anymore
	// they were part of the old d3d6 viewport settings
	// from testing d3d9 passes the values along untouched
	// probably need some testing here to see how d3d6 worked
	// forsaken still uses these values so we need them
	return TRUE;
}

// these can be done later
HRESULT update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}
void release_texture( LPTEXTURE texture ){}
HRESULT FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{return S_OK;}

// probably needs to be done second
BOOL FSGetWorld(RENDERMATRIX *matrix){return TRUE;}
BOOL FSSetWorld( RENDERMATRIX *matrix ){return TRUE;}
BOOL FSSetProjection( RENDERMATRIX *matrix ){return TRUE;}
BOOL FSSetView( RENDERMATRIX *matrix ){return TRUE;}
BOOL FSSetMaterial(RENDERMATERIAL *material){return TRUE;}

// these should probably be done first
HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices){return TRUE;}
HRESULT FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices){return TRUE;}
HRESULT FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices){return TRUE;}
HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts){return TRUE;}
HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, LPTLVERTEX **verts){return TRUE;}
HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}
HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}
HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices){return TRUE;}
HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices){return TRUE;}
HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices){return TRUE;}
HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return TRUE;}

// probably last thing to do
HRESULT draw_object(RENDEROBJECT *renderObject){return TRUE;}
HRESULT draw_line_object(RENDEROBJECT *renderObject){return TRUE;}
HRESULT draw_2d_object(RENDEROBJECT *renderObject){return TRUE;}

void FSReleaseRenderObject(RENDEROBJECT *renderObject)
{
	int i;
	if (renderObject->lpVertexBuffer)
	{
		// TODO - need to destroy buffer gl style
		renderObject->lpVertexBuffer = NULL;
	}
	if (renderObject->lpIndexBuffer)
	{
		// TODO - need to destroy buffer gl style
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
