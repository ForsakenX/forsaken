#ifndef TRANSEXE_INCLUDED
#define TRANSEXE_INCLUDED

#include "render.h"
#include "new3d.h"

#define MAXTRANSEXE 128

typedef struct _TRANSEXE{
	int	UseIdentity;
	RENDERMATRIX  Matrix;
	RENDEROBJECT renderObject;
	int16_t		NumVerts;
	u_int16_t		Model;
	u_int16_t		group;
	int render_lighting_use_only_light_color_and_blend;
	int render_lighting_use_only_light_color;
	int render_lighting_point_lights_only;
	int render_lighting_enabled;
	int render_light_ambience;
	int render_light_ambience_alpha;
	int render_color_blend_red;
	int render_color_blend_green;
	int render_color_blend_blue;
}TRANSEXE;

extern int16_t	NumOfTransExe;

/*
 * fn prototypes
 */

void AddTransExe( RENDERMATRIX *Matrix , RENDEROBJECT *renderObject, int UseIdentity, u_int16_t Model, u_int16_t group, int16_t NumVerts );
void ExecuteTransExe( u_int16_t group );
void ExecuteTransExeUnclipped( u_int16_t group );

#endif	// TRANSEXE_INCLUDED
