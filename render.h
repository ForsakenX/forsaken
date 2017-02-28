#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#include "main.h"
#include "new3d.h"
#include <SDL.h>

extern bool  bSquareOnly;

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
    bool                    fullscreen;				/* in fullscreen exclusive mode? */
    CLIENTSIZE              window_size;			/* dimensions of client win */
    bool                    bPaused;				/* the app is paused */
    bool                    app_active;				/* the app is active */
    bool                    minimized;				/* app window is minimized */
    bool                    ok_to_render;			/* All objects etc. necessary rendering are in ok */
	bool					vsync;					/* vertical sync */
	float					aspect_ratio;			/* screen aspect ratio */
	bool					force_accel;			/* force 3d acelleration on gl */
	bool					wireframe;

#if SDL_VERSION_ATLEAST(2,0,0)
	SDL_Window*             window;
	SDL_Renderer*           renderer;
#else
	SDL_Surface*	        screen;
#endif

	// stereo related options
	bool                  stereo_enabled;
	stereo_mode_t         stereo_mode;
	stereo_right_color_t  stereo_right_color;
	stereo_position_t     stereo_position;
	float                 stereo_eye_sep;
	float                 stereo_focal_dist;
} render_info_t;

typedef bool (*RenderCurrentCameraPt) (void);
bool RenderCurrentCameraInStereo( RenderCurrentCameraPt );


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

bool FSClearBlack(void);
bool FSClear(XYRECT * rect);
bool FSClearDepth(XYRECT * rect);

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
	bool colourkey;
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
	bool			vbLocked;
	int numTextureGroups;
	TEXTUREGROUP textureGroups[MAX_TEXTURE_GROUPS];
} RENDEROBJECT;

typedef struct LEVELRENDEROBJECT
{
	LPVERTEXBUFFER	lpVertexBuffer;
	LPARRAYBUFFER	lpNormalBuffer;
	LPINDEXBUFFER	lpIndexBuffer;
	bool			vbLocked;
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
bool render_mode_select( render_info_t * info );
void render_mode_wireframe(void);
void render_mode_points(void);
void render_mode_fill(void);
void build_gamma_table( double gamma );
bool render_init( render_info_t * info );
void render_cleanup( render_info_t * info );
bool render_reset( render_info_t * info );
void render_set_filter( bool red, bool green, bool blue );
bool render_flip( render_info_t * info );
void reset_trans( void );
void disable_zbuff_write( void );

bool FSGetViewPort(render_viewport_t *returnViewPort);
bool FSBeginScene(void);
bool FSEndScene(void);
bool FSSetViewPort(render_viewport_t *newViewPort);
bool FSGetWorld(RENDERMATRIX *matrix);
bool FSSetWorld( RENDERMATRIX *matrix );
bool FSSetProjection( RENDERMATRIX *matrix );
bool FSSetView( RENDERMATRIX *matrix );

bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals);
bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals);
bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals);
bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject);
bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices);
bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts);
bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject);
bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices);
bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices);
bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject);
bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts);
bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices);

bool FSCreateTexture(LPTEXTURE *texture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colourkey);
bool update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colourkey);
void release_texture( LPTEXTURE texture );

bool draw_render_object( RENDEROBJECT *renderObject, int primitive_type, bool orthographic );
bool draw_line_object(RENDEROBJECT *renderObject);
bool draw_object(RENDEROBJECT *renderObject);
bool draw_2d_object(RENDEROBJECT *renderObject);

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
