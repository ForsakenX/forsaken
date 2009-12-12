#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#include "main.h"
#include "new3d.h"
#include "SDL.h"

BOOL  bSquareOnly;

typedef struct {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
    float	ScaleX; 
    float	ScaleY; 
} render_viewport_t;

typedef struct {
    int     w;                /* width */
    int     h;                /* height */
    int     bpp;              /* bits per pixel */
} render_display_mode_t;

typedef struct
{
    long cx;
    long cy;
} CLIENTSIZE;

typedef struct {
    int                     NumModes;				/* number of available display modes */
    int                     CurrMode;				/* number of current display mode (only when fullscreen) */
    render_display_mode_t * Mode;					/* desc avail modes */
    render_display_mode_t   ThisMode;				/* description of this mode, identical to Mode[CurrMode] */
    render_display_mode_t   WindowsDisplay;			/* current Windows disply mode */
    render_display_mode_t   default_mode;			/* current Windows disply mode */
    BOOL                    fullscreen;				/* in fullscreen exclusive mode? */
    CLIENTSIZE              window_size;			/* dimensions of client win */
    BOOL                    bPaused;				/* the app is paused */
    BOOL                    app_active;				/* the app is active */
    BOOL                    minimized;				/* app window is minimized */
    BOOL                    ok_to_render;			/* All objects etc. necessary rendering are in ok */
	BOOL					vsync;					/* vertical sync */
	float					aspect_ratio;			/* screen aspect ratio */
	BOOL					force_accel;			/* force 3d acelleration on gl */
	SDL_Surface*	        screen;
} render_info_t;

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

void cull_none( void );
void cull_cw( void );
void reset_cull( void );
void disable_zbuff( void );
void set_alpha_states( void );
void set_normal_states( void );
void reset_zbuff( void );
void reset_filtering( void );
void set_alpha_fx_states( void );

const char * render_error_description( int error );

typedef struct {
    long x1;
    long y1;
    long x2;
    long y2;
} XYRECT;

BOOL FSClearBlack(void);
BOOL FSClear(XYRECT * rect);
BOOL FSClearDepth(XYRECT * rect);

#define MAX_LEVEL_TEXTURE_GROUPS 8

#define MAX_TEXTURE_GROUPS 600

#define INCREASE_TEXTURE_GROUPS( group ) \
	assert( group->numTextureGroups < MAX_TEXTURE_GROUPS ); \
	group->numTextureGroups++;

typedef void * LPTEXTURE;

typedef struct TEXTUREGROUP
{
	int startVert;
	int startIndex;
	int numVerts;
	int numTriangles;
	BOOL colourkey;
	LPTEXTURE texture;
} TEXTUREGROUP;

typedef void * LPVERTEXBUFFER;
typedef void * LPINDEXBUFFER;

// taken from d3d9

typedef struct {
    float r;
    float g;
    float b;
    float a;
} COLORVALUE;

typedef struct {
    COLORVALUE   Diffuse;        /* Diffuse color RGBA */
    COLORVALUE   Ambient;        /* Ambient color RGB */
    COLORVALUE   Specular;       /* Specular 'shininess' */
    COLORVALUE   Emissive;       /* Emissive color RGB */
    float        Power;          /* Sharpness if specular highlight */
} RENDERMATERIAL;

typedef struct RENDEROBJECT
{
	LPVERTEXBUFFER	lpVertexBuffer;
	LPINDEXBUFFER	lpIndexBuffer;
	RENDERMATERIAL	material;
	BOOL			vbLocked;
	int numTextureGroups;
	TEXTUREGROUP textureGroups[MAX_TEXTURE_GROUPS];
} RENDEROBJECT;

typedef struct LEVELRENDEROBJECT
{
	LPVERTEXBUFFER	lpVertexBuffer;
	LPINDEXBUFFER	lpIndexBuffer;
	RENDERMATERIAL	material;
	BOOL			vbLocked;
	int numTextureGroups;
	TEXTUREGROUP textureGroups[MAX_LEVEL_TEXTURE_GROUPS];
} LEVELRENDEROBJECT;

typedef struct {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
    };
} RENDERMATRIX;

#ifdef OPENGL // for stereo
void render_set_filter( BOOL red, BOOL green, BOOL blue );
#endif

BOOL render_flip( render_info_t * info );

BOOL FSGetViewPort(render_viewport_t *returnViewPort);
BOOL FSBeginScene(void);
BOOL FSEndScene(void);
BOOL FSSetViewPort(render_viewport_t *newViewPort);
BOOL FSGetWorld(RENDERMATRIX *matrix);
BOOL FSSetWorld( RENDERMATRIX *matrix );
BOOL FSSetProjection( RENDERMATRIX *matrix );
BOOL FSSetView( RENDERMATRIX *matrix );

BOOL FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
BOOL FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
BOOL FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts);
BOOL FSUnlockVertexBuffer(RENDEROBJECT *renderObject);
BOOL FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
BOOL FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
BOOL FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices);
BOOL FSUnlockIndexBuffer(RENDEROBJECT *renderObject);
BOOL FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts);
BOOL FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices);

BOOL FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey);
BOOL update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey);
void release_texture( LPTEXTURE texture );

BOOL draw_line_object(RENDEROBJECT *renderObject);
BOOL draw_object(RENDEROBJECT *renderObject);
BOOL draw_2d_object(RENDEROBJECT *renderObject);

void FSReleaseRenderObject(RENDEROBJECT *renderObject);

#ifdef __cplusplus
};
#endif

// this is used to read the file because the file is encoded with dwReserved
typedef struct {
    union {
		float     x;             /* Homogeneous coordinates */
		float     dvX;
    };
    union {
		float     y;
		float     dvY;
    };
    union {
		float     z;
		float     dvZ;
    };
    DWORD            dwReserved;
    union {
		COLOR     color;         /* Vertex color */
		COLOR     dcColor;
    };
    union {
		COLOR     specular;      /* Specular component of vertex */
		COLOR     dcSpecular;
    };
    union {
		float     tu;            /* Texture coordinates */
		float     dvTU;
    };
    union {
		float     tv;
		float     dvTV;
    };
} *LPOLDLVERTEX;

/* bjd - remove when done. taken from SDK */
#define CLIP_LEFT                0x00000001L
#define CLIP_RIGHT               0x00000002L
#define CLIP_TOP					0x00000004L
#define CLIP_BOTTOM              0x00000008L
#define CLIP_FRONT               0x00000010L
#define CLIP_BACK                0x00000020L

#endif // RENDER_INCLUDED
