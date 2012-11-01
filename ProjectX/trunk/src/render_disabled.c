#ifdef RENDER_DISABLED
#include "main.h"
#include "util.h"
#include "render.h"
#include "texture.h"
#include "file.h"
#include <SDL.h>
#include "SDL_opengl.h"
#include <stdio.h>
#include "new3d.h"
#include "lights.h"

void render_mode_wireframe(void){}
void render_mode_points(void){}
void render_mode_fill(void){}
void build_gamma_table( double gamma ){}
void release_texture( LPTEXTURE texture ){}
void render_cleanup( render_info_t * info ){}
void render_set_filter( bool red, bool green, bool blue ){}
void reset_trans( void ){}
void reset_zbuff( void ){}
void disable_zbuff_write( void ){}
void disable_zbuff( void ){}
void cull_none( void ){}
void cull_cw( void ){}
void reset_cull( void ){}
void set_alpha_ignore( void ){}
void unset_alpha_ignore( void ){}
void set_normal_states( void ){}
void set_trans_state_9(){}
void set_alpha_states( void ){}
void set_whiteout_state( void ){}
void render_reset_lighting_variables( void ){}
void do_water_effect( VECTOR * pos, u_int8_t * color ){}
void do_whiteout_effect( VECTOR * pos, COLOR * color ){}
void GetRealLightAmbientWorldSpace( VECTOR * Pos , float * R , float * G , float * B, float * A ){}
void FSReleaseRenderObject(RENDEROBJECT *renderObject)
{
	int i;
	if (renderObject->lpVertexBuffer)
	{
		free(renderObject->lpVertexBuffer);
		renderObject->lpVertexBuffer = NULL;
	}
	if (renderObject->lpIndexBuffer)
	{
		free(renderObject->lpIndexBuffer);
		renderObject->lpIndexBuffer = NULL;
	}
	for (i = 0; i < renderObject->numTextureGroups; i++)
	{
		renderObject->textureGroups[i].numVerts = 0;
		renderObject->textureGroups[i].startVert = 0;

		if (renderObject->textureGroups[i].texture)
		{
			// tload.c calls release_texture
			// we should not release them here
			renderObject->textureGroups[i].texture = NULL;
		}
	}
	renderObject->numTextureGroups = 0;
}

bool FSBeginScene(){ return true; }
bool FSEndScene(){ return true; }
bool create_texture(LPTEXTURE *t, const char *path, u_int16_t *width, u_int16_t *height, int numMips, bool * colorkey){return true;}
bool update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colorkey){return true;}
bool FSCreateTexture(LPTEXTURE *texture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colourkey){return true;}
bool render_mode_select( render_info_t * info ){return true;}
bool render_reset( render_info_t * info ){return true;}
bool render_flip( render_info_t * info ){return true;}
bool FSClear(XYRECT * rect){return true;}
bool FSClearBlack(void){return true;}
bool FSClearDepth(XYRECT * rect){return true;}
bool FSGetViewPort(render_viewport_t *view){return true;}
bool FSSetViewPort(render_viewport_t *view){return true;}
bool FSSetProjection( RENDERMATRIX *matrix ){return true;}
bool FSSetView( RENDERMATRIX *matrix ){return true;}
bool FSSetWorld( RENDERMATRIX *matrix ){return true;}
bool FSGetWorld(RENDERMATRIX *matrix){return true;}
bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return true;}
bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return true;}
bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject){return true;}
bool draw_object(RENDEROBJECT *renderObject){return true;}
bool draw_2d_object(RENDEROBJECT *renderObject){return true;}
bool draw_line_object(RENDEROBJECT *renderObject){return true;}

bool render_init( render_info_t * info )
{
	info->ok_to_render = true;
	return true;
}

bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(LVERTEX) );
	return true;
}

bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	FSCreateVertexBuffer(renderObject, numVertices);
	return true;
}

bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	renderObject->lpNormalBuffer = malloc( numNormals * sizeof(NORMAL) );
	return true; 
}

bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	FSCreateNormalBuffer(renderObject, numNormals); 
	return true;
}

bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = malloc( numIndices * 3 * sizeof(WORD) );
	return true;
}

bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	return FSCreateIndexBuffer(renderObject,numIndices);
}

bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	(*indices) = renderObject->lpIndexBuffer; 
	return true;
}

bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{
	(*verts) = renderObject->lpVertexBuffer; 
	return true;
}

bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals)
{
	(*normals) = renderObject->lpNormalBuffer; 
	return true;
}

bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(TLVERTEX) ); 
	return true;
}

bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{
	*verts = (void*)renderObject->lpVertexBuffer; 
	return true;
}

int render_color_blend_red   = 0;
int render_color_blend_green = 0;
int render_color_blend_blue  = 0;
int render_lighting_enabled = 0;
int render_lighting_point_lights_only = 1;
int render_lighting_use_only_light_color = 0;
int render_lighting_use_only_light_color_and_blend = 0;
int render_light_ambience = 0;
int render_light_ambience_alpha = 255.0f;
int render_lighting_env_water         = 0;
int render_lighting_env_water_level   = 0;
float render_lighting_env_water_red   = 0.0f;
float render_lighting_env_water_green = 0.0f;
float render_lighting_env_water_blue  = 0.0f;
int render_lighting_env_whiteout = 0;

const char * render_error_description( int e ) { return NULL; }

#endif // RENDER_DISABLED
