#ifdef OPENGL

#include "main.h"
#include "util.h"
#include "render.h"
#include "SDL.h"
#include "SDL_opengl.h"

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

	/* everything that d3d9 init did */

	// default the gamma table
		// is this needed ?

	// do any gl object init stuff
		// anything needed ?

	// doesn't sdl do this now ?
		// define back buffer count
		// define refresh rate	
		// select front/back buffer formats
		// enumerate and select a display mode
		//{
			// assign enumeration info to info->Mode
			// assign info->(CurrMode|ThisMode|WindowsDisplay|szClient)
			// set the viewport
		//}

	// enable automated depth stencil
		// doesn't reset_zbuff() already do this?
		// in d3d9 zbuffer being managed *for us* needed to be enabled

	//
	// init render state
	//

	glShadeModel(GL_SMOOTH); // TODO - no gouraud ?
	glDisable(GL_LIGHTING);
	reset_cull();
	reset_trans();
	reset_filtering();

	// wireframe
	//glPolygonMode(GL_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT, GL_LINE);

	set_normal_states();

	return TRUE;
}

void render_cleanup( render_info_t * info )
{
    info->bRenderingIsOK = FALSE;
	if(info->Mode)
		free(info->Mode);
    // TODO - any opengl cleanup required ?
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

HRESULT FSGetViewPort(render_viewport_t *returnViewPort)
{
	return S_OK;
}

HRESULT FSSetViewPort(render_viewport_t *newViewPort)
{
	return S_OK;
}

HRESULT FSSetWorld( RENDERMATRIX *matrix )
{
	return S_OK;
}

HRESULT FSSetProjection( RENDERMATRIX *matrix )
{
	return S_OK;
}

HRESULT FSSetView( RENDERMATRIX *matrix )
{
	return S_OK;
}

HRESULT FSGetWorld(RENDERMATRIX *matrix)
{
	return S_OK;
}

HRESULT FSSetMaterial(RENDERMATERIAL *material)
{
	return S_OK;
}

HRESULT FSBeginScene()
{
	return S_OK;
}

HRESULT FSEndScene()
{
	return S_OK;
}

HRESULT update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{
	return S_OK;
}

void release_texture( LPTEXTURE texture )
{
}

HRESULT FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{
	return S_OK;
}

HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	return S_OK;
}

HRESULT FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	return S_OK;
}

HRESULT FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	return S_OK;
}

HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{
	return S_OK;
}

HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, LPTLVERTEX **verts)
{
	return S_OK;
}

HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject)
{
	return S_OK;
}

HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject)
{
	return S_OK;
}

HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	return S_OK;
}

HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	return S_OK;
}

HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	return S_OK;
}

HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject)
{
	return S_OK;
}

HRESULT draw_line_object(RENDEROBJECT *renderObject)
{
	return S_OK;
}

HRESULT draw_object(RENDEROBJECT *renderObject)
{
	return S_OK;
}

HRESULT draw_2d_object(RENDEROBJECT *renderObject)
{
	return S_OK;
}

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
