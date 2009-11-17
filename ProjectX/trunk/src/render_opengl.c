#ifdef OPENGL

#include "main.h"
#include "util.h"
#include "render.h"

// TODO - port functions to not use hresult
typedef long HRESULT;

void render_gamma_correction( double gamma )
{
}

BOOL init_renderer( render_info_t * info )
{
	// TODO - all textures should be scaled to square
	//		  other wise should get this from gl caps
	bSquareOnly = FALSE;
	return TRUE;
}

void render_cleanup( render_info_t * info )
{
}

BOOL render_mode_select( render_info_t * info )
{
	return TRUE;
}

BOOL render_reset( render_info_t * info )
{
	return TRUE;
}

BOOL render_flip( render_info_t * info )
{
	return TRUE;
}

void reset_trans( void )
{
}

void reset_zbuff( void )
{
}

void disable_zbuff_write( void )
{
}

void disable_zbuff( void )
{
}

void linear_filtering( void )
{
}

void anisotropic_filtering( void )
{
}

void disable_filtering( void )
{
}

void default_filtering( void )
{
}

void reset_filtering( void )
{
}

void cull_none( void )
{
}

void cull_cw( void )
{
}

void reset_cull( void )
{
}

void set_alpha_ignore( void )
{
}

void set_normal_states( void )
{
}

void set_alpha_states( void )
{
}

void set_alpha_fx_states( void )
{
}

BOOL FSClear(DWORD Count, XYRECT* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil)
{
	return TRUE;
}

BOOL FSClearBlack(void)
{
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

HRESULT FSSetWorld( D3DMATRIX *matrix )
{
	return lpD3DDevice->SetTransform(D3DTS_WORLD, matrix);
}

HRESULT FSSetProjection( D3DMATRIX *matrix )
{
	return lpD3DDevice->SetTransform(D3DTS_PROJECTION, matrix);
}

HRESULT FSSetView( D3DMATRIX *matrix )
{
	return lpD3DDevice->SetTransform(D3DTS_VIEW, matrix);
}

HRESULT FSGetWorld(D3DMATRIX *matrix)
{
	return S_OK;
}

HRESULT FSSetMaterial(const RENDERMATERIAL *material)
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

HRESULT create_texture(LPTEXTURE *texture, const char *path, uint16 *width, uint16 *height, int numMips, BOOL * colorkey, D3DPOOL pool)
{
	return S_OK;
}

HRESULT update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{
	return S_OK;
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

HRESULT draw_render_object( RENDEROBJECT *renderObject, BOOL transformed /*aka 2d*/, D3DPRIMITIVETYPE primitive_type )
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
}

const char * render_error_description( HRESULT hr )
{
	return "";
}

#endif // OPENGL
