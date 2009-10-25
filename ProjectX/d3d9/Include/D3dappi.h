#ifndef __D3DAPPI_H__
#define __D3DAPPI_H__

/*
 * INCLUDED HEADERS
 */

#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <d3d9.h>
#include "d3dapp.h"
#include "d3dmain.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MACROS
 */
#undef ATTEMPT
#define ATTEMPT(x) if (!(x)) goto exit_with_error
#undef RELEASE
#ifndef __cplusplus
#define RELEASE(x) {if (x != NULL) {x->lpVtbl->Release(x); x = NULL;}}
#else
#define RELEASE(x) {if (x != NULL) {x->Release(); x = NULL;}}
#endif
#undef MAX
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#undef MIN
#define MIN(x, y) ((x) > (y)) ? (y) : (x)
#undef ZEROMEM
#define ZEROMEM(x) memset(&x, 0, sizeof(x))

/*
 * GLOBAL VARIABLES
 *    see d3dapp.c for descriptions
 */
extern render_info_t d3dappi;
extern BOOL bIgnoreWM_SIZE;

#ifdef __cplusplus
};
#endif

/*
 * INTERNAL FUNCTION PROTOTYPES
 */

void cull_none( void );
void cull_cw( void );
void reset_cull( void );
BOOL init_render_states(void);
void disable_zbuff( void );
void set_alpha_states( void );
void set_normal_states( void );
void reset_zbuff( void );
void reset_filtering( void );
void set_alpha_fx_states( void );

const char * render_error_description( HRESULT hr );

#define FSColourKeyBlack 0xFF000000 // pass this as colour key for black as transparent
BOOL FSClearBlack(void);
BOOL FSClear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LEVEL_TEXTURE_GROUPS 8

#define MAX_TEXTURE_GROUPS 600

#define INCREASE_TEXTURE_GROUPS( group ) \
	assert( group->numTextureGroups < MAX_TEXTURE_GROUPS ); \
	group->numTextureGroups++;

typedef struct TEXTUREGROUP
{
	int startVert;
	int startIndex;
	int numVerts;
	int numTriangles;
	BOOL colourkey;
	LPDIRECT3DTEXTURE9 texture;
} TEXTUREGROUP;

/* bjd - move this stuff somewhere more appropriate */
typedef struct RENDEROBJECT
{
	LPDIRECT3DVERTEXBUFFER9 lpD3DVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	lpD3DIndexBuffer;
	D3DMATERIAL9 material;
	/*BOOL*/int				vbLocked;
	int numTextureGroups;

	TEXTUREGROUP textureGroups[MAX_TEXTURE_GROUPS];

	/* add whatever else needed.. */
} RENDEROBJECT;

/* bjd - move this stuff somewhere more appropriate */
typedef struct LEVELRENDEROBJECT
{
	LPDIRECT3DVERTEXBUFFER9 lpD3DVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	lpD3DIndexBuffer;
	D3DMATERIAL9 material;
	/*BOOL*/int				vbLocked;
	int numTextureGroups;

	TEXTUREGROUP textureGroups[MAX_LEVEL_TEXTURE_GROUPS];

	/* add whatever else needed.. */
} LEVELRENDEROBJECT;

typedef struct RENDERSTATE
{
	int blah; // temp
} RENDERSTATE;

HRESULT FSGetViewPort(render_viewport_t *returnViewPort);
HRESULT FSBeginScene();
HRESULT FSEndScene();
HRESULT FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, D3DLVERTEX **verts);
HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject);
HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices);
HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject);
void FSReleaseRenderObject(RENDEROBJECT *renderObject);
HRESULT draw_object(RENDEROBJECT *renderObject);
HRESULT FSSetViewPort(render_viewport_t *newViewPort);
HRESULT FSGetViewport(render_viewport_t *returnViewPort);
HRESULT FSSetViewPort(render_viewport_t *newViewPort);
HRESULT FSSetMatrix(D3DTRANSFORMSTATETYPE type, const D3DMATRIX *matrix);
HRESULT FSGetMatrix(D3DTRANSFORMSTATETYPE type, D3DMATRIX *matrix);
HRESULT FSSetMaterial(const D3DMATERIAL9 *material);
HRESULT FSCreateTexture(LPDIRECT3DTEXTURE9 *texture, const char *fileName, int width, int height, int numMips, BOOL * colourkey);
HRESULT update_texture_from_file(LPDIRECT3DTEXTURE9 dstTexture, const char *fileName, int width, int height, int numMips, BOOL * colourkey);
HRESULT draw_line_vertex_buffer(RENDEROBJECT *renderObject);
HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject);
HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, D3DTLVERTEX **verts);
HRESULT FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
HRESULT draw_line_object(RENDEROBJECT *renderObject);
HRESULT draw_object(RENDEROBJECT *renderObject);
HRESULT draw_2d_object(RENDEROBJECT *renderObject);

#ifdef __cplusplus
};
#endif

typedef struct _OLDD3DLVERTEX {
    union {
    D3DVALUE     x;             /* Homogeneous coordinates */
    D3DVALUE     dvX;
    };
    union {
    D3DVALUE     y;
    D3DVALUE     dvY;
    };
    union {
    D3DVALUE     z;
    D3DVALUE     dvZ;
    };
    DWORD            dwReserved;
    union {
    D3DCOLOR     color;         /* Vertex color */
    D3DCOLOR     dcColor;
    };
    union {
    D3DCOLOR     specular;      /* Specular component of vertex */
    D3DCOLOR     dcSpecular;
    };
    union {
    D3DVALUE     tu;            /* Texture coordinates */
    D3DVALUE     dvTU;
    };
    union {
    D3DVALUE     tv;
    D3DVALUE     dvTV;
    };
} OLDD3DLVERTEX, *LPOLDD3DLVERTEX;

/* bjd - remove when done. taken from SDK */
#define D3DCLIP_LEFT                0x00000001L
#define D3DCLIP_RIGHT               0x00000002L
#define D3DCLIP_TOP					0x00000004L
#define D3DCLIP_BOTTOM              0x00000008L
#define D3DCLIP_FRONT               0x00000010L
#define D3DCLIP_BACK                0x00000020L

#endif // __D3DAPPI_H__
