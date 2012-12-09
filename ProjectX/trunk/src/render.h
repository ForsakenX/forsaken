#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#include "main.h"
#include "new3d.h"
#include <SDL.h>

_Bool  bSquareOnly;

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
} render_display_mode_t;

typedef struct
{
    long cx;
    long cy;
} CLIENTSIZE;

// stereo related enums
typedef enum { STEREO_MODE_COLOR, STEREO_MODE_HALF_HEIGHT, STEREO_MODE_HALF_WIDTH } stereo_mode_t;
typedef enum { ST_CENTER, ST_LEFT, ST_RIGHT } stereo_position_t;
typedef enum { ST_GREEN, ST_BLUE, ST_CYAN } stereo_right_color_t;

typedef struct {
    int                     NumModes;				/* number of available display modes */
    int                     CurrMode;				/* number of current display mode (only when fullscreen) */
    render_display_mode_t * Mode;					/* desc avail modes */
    render_display_mode_t   ThisMode;				/* description of this mode, identical to Mode[CurrMode] */
    render_display_mode_t   WindowsDisplay;			/* current Windows disply mode */
    render_display_mode_t   default_mode;			/* current Windows disply mode */
    _Bool                    fullscreen;				/* in fullscreen exclusive mode? */
    CLIENTSIZE              window_size;			/* dimensions of client win */
    _Bool                    bPaused;				/* the app is paused */
    _Bool                    app_active;				/* the app is active */
    _Bool                    minimized;				/* app window is minimized */
    _Bool                    ok_to_render;			/* All objects etc. necessary rendering are in ok */
	_Bool					vsync;					/* vertical sync */
	float					aspect_ratio;			/* screen aspect ratio */
	_Bool					force_accel;			/* force 3d acelleration on gl */
	_Bool					wireframe;
	SDL_Surface*	        screen;

	// stereo related options
	bool                  stereo_enabled;
	stereo_mode_t         stereo_mode;
	stereo_right_color_t  stereo_right_color;
	stereo_position_t     stereo_position;
	float                 stereo_eye_sep;
	float                 stereo_focal_dist;
} render_info_t;

#undef RELEASE
#ifndef __cplusplus
#define RELEASE(x) {if (x != NULL) {x->lpVtbl->Release(x); x = NULL;}}
#else
#define RELEASE(x) {if (x != NULL) {x->Release(); x = NULL;}}
#endif

void cull_none( void );
void cull_cw( void );
void reset_cull( void );
void disable_zbuff( void );
void set_alpha_states( void );
void set_normal_states( void );
void reset_zbuff( void );
void reset_filtering( void );
void set_alpha_fx_states( void );
void set_whiteout_state( void );
const char * render_error_description( int error );

typedef struct {
    long x1;
    long y1;
    long x2;
    long y2;
} XYRECT;

_Bool FSClearBlack(void);
_Bool FSClear(XYRECT * rect);
_Bool FSClearDepth(XYRECT * rect);

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
	_Bool colourkey;
	LPTEXTURE texture;
} TEXTUREGROUP;

typedef void * LPVERTEXBUFFER;
typedef void * LPARRAYBUFFER;
typedef void * LPINDEXBUFFER;

// taken from d3d9

typedef struct {
    float r;
    float g;
    float b;
    float a;
} COLORVALUE;

typedef struct RENDEROBJECT
{
	LPVERTEXBUFFER	lpVertexBuffer;
	LPARRAYBUFFER	lpNormalBuffer;
	LPINDEXBUFFER	lpIndexBuffer;
	_Bool			vbLocked;
	int numTextureGroups;
	TEXTUREGROUP textureGroups[MAX_TEXTURE_GROUPS];
} RENDEROBJECT;

typedef struct LEVELRENDEROBJECT
{
	LPVERTEXBUFFER	lpVertexBuffer;
	LPARRAYBUFFER	lpNormalBuffer;
	LPINDEXBUFFER	lpIndexBuffer;
	_Bool			vbLocked;
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

extern void render_reset_lighting_variables( void );
extern int render_lighting_use_only_light_color_and_blend;
extern int render_lighting_use_only_light_color;
extern int render_lighting_point_lights_only;
extern int render_lighting_enabled;
extern int render_light_ambience;
extern int render_light_ambience_alpha;
extern int render_color_blend_red;
extern int render_color_blend_green;
extern int render_color_blend_blue;
extern int render_lighting_env_water;
extern int render_lighting_env_water_level;
extern float render_lighting_env_water_red;
extern float render_lighting_env_water_green;
extern float render_lighting_env_water_blue;
extern int render_lighting_env_whiteout;

void render_set_filter( _Bool red, _Bool green, _Bool blue );

_Bool render_flip( render_info_t * info );

_Bool FSGetViewPort(render_viewport_t *returnViewPort);
_Bool FSBeginScene(void);
_Bool FSEndScene(void);
_Bool FSSetViewPort(render_viewport_t *newViewPort);
_Bool FSGetWorld(RENDERMATRIX *matrix);
_Bool FSSetWorld( RENDERMATRIX *matrix );
_Bool FSSetProjection( RENDERMATRIX *matrix );
_Bool FSSetView( RENDERMATRIX *matrix );

_Bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals);
_Bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals);
_Bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals);
_Bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject);
_Bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
_Bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
_Bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts);
_Bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject);
_Bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
_Bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
_Bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices);
_Bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject);
_Bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts);
_Bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices);

_Bool FSCreateTexture(LPTEXTURE *texture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, _Bool * colourkey);
_Bool update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, _Bool * colourkey);
void release_texture( LPTEXTURE texture );

_Bool draw_line_object(RENDEROBJECT *renderObject);
_Bool draw_object(RENDEROBJECT *renderObject);
_Bool draw_2d_object(RENDEROBJECT *renderObject);

void FSReleaseRenderObject(RENDEROBJECT *renderObject);

#ifdef __cplusplus
};
#endif

// this is used to read the file because the file
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
