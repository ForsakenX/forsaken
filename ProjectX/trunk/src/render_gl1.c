#if GL == 1
#include "render_gl_shared.h"
#include "new3d.h"
#include "lights.h"

//
// using the concept of index/vertex buffers in opengl is a bit different
// so for now I'm just going going to return a pointer to memory
// then just convert the index/vertex buffer to a pure vertex buffer on draw
//
// the vertex/index pointers could probably end up being the glGen* id
// then internal display lists can be generated for the non dynamic functions
// so we can render the static objects as display lists
//

bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(LVERTEX) );
	return true;
}
bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{FSCreateVertexBuffer(renderObject, numVertices); return true;}

bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{ renderObject->lpNormalBuffer = malloc( numNormals * sizeof(NORMAL) ); return true; }

bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{FSCreateNormalBuffer(renderObject, numNormals); return true;}

bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = malloc( numIndices * 3 * sizeof(WORD) );
	return true;
}
bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{return FSCreateIndexBuffer(renderObject,numIndices);}

bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{(*indices) = renderObject->lpIndexBuffer; return true;}

bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{(*verts) = renderObject->lpVertexBuffer; return true;}
bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return true;}
bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return true;}

bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals)
{(*normals) = renderObject->lpNormalBuffer; return true;}
bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject){return true;}

bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(TLVERTEX) ); 
	return true;
}
bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{*verts = (void*)renderObject->lpVertexBuffer; return true;}

static void set_color( COLOR c )
{
	// COLOR is the value loaded from the files
	// it's packed as uchar[4] (bgra) and glColor expects (rgba)
	// so we flip the red/blue values with each other
	c = (c & 0xff00ff00) | ((c & 0x00ff0000) >> 16) | ((c & 0x000000ff) << 16);
	glColor4ubv((GLubyte*)&c);
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

void render_reset_lighting_variables( void )
{
	render_color_blend_red   = 0;
	render_color_blend_green = 0;
	render_color_blend_blue  = 0;
	render_lighting_enabled = 0;
	render_lighting_point_lights_only = 1;
	render_lighting_use_only_light_color = 0;
	render_lighting_use_only_light_color_and_blend = 0;
	render_light_ambience = 0;
	render_light_ambience_alpha = 255.0f;
	render_lighting_env_water         = 0;
	render_lighting_env_water_level   = 0;
	render_lighting_env_water_red   = 0.0f;
	render_lighting_env_water_green = 0.0f;
	render_lighting_env_water_blue  = 0.0f;
	render_lighting_env_whiteout = 0;
}

void do_water_effect( VECTOR * pos, COLOR * color )
{
	u_int32_t r,g,b;
	int x,y,z;
	float intensity, seconds;
	static float speed = 71.0f;
	if( render_lighting_env_water == 2 && pos->y >= render_lighting_env_water_level )
		return;
	r = color[2] >> 2;
	g = color[1] >> 2;
	b = color[0] >> 2;
	x = (float)((int)(pos->x * 0.35f) % 360);
	y = (float)((int)(pos->y * 0.35f) % 360);
	z = (float)((int)(pos->z * 0.35f) % 360);
	seconds = SDL_GetTicks() / 1000.0f;
	intensity = (float) (
		( 
			sin( D2R( x + seconds * speed ) ) +  // cral = seconds * speed
			sin( D2R( y + seconds * speed ) ) + 
			sin( D2R( z + seconds * speed ) ) 
		) * 127.0F * 0.3333333F + 128.0F 
	);
	r += render_lighting_env_water_red   * intensity;
	g += render_lighting_env_water_green * intensity;
	b += render_lighting_env_water_blue  * intensity;
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	color[2] = (u_int8_t) r;
	color[1] = (u_int8_t) g;
	color[0] = (u_int8_t) b;
}

void do_whiteout_effect( VECTOR * pos, COLOR * color )
{
	int x,y,z,intensity;
	float seconds;
	static float speed = 71.0f;
	x = (float)((int)(pos->x * 0.35f) % 360);
	y = (float)((int)(pos->y * 0.35f) % 360);
	z = (float)((int)(pos->z * 0.35f) % 360);
	seconds = SDL_GetTicks() / 1000.0f;
	intensity = (int) (
		( 
			sin( D2R( x + seconds * speed ) ) +  // cral = seconds * speed
			sin( D2R( y + seconds * speed ) ) + 
			sin( D2R( z + seconds * speed ) ) 
		) * 127.0F * 0.3333333F + 128.0F 
	);
	intensity += render_lighting_env_whiteout;
	if(intensity > 255) intensity = 255;
	*color &= 0xffff;
	*color |= ( intensity << 24 ) + ( intensity << 16 );
}

extern XLIGHT * FirstLightVisible;
void GetRealLightAmbientWorldSpace( VECTOR * Pos , float * R , float * G , float * B, float * A )
{
	VECTOR ray;
	float rlen, rlen2, lsize2, intensity, cosa, cosarc2;
	XLIGHT * LightPnt = FirstLightVisible;

	*R = *G = *B = render_light_ambience;
	*A = render_light_ambience_alpha;
	
	while( LightPnt )
	{
		ray.x = Pos->x - LightPnt->Pos.x;
		ray.y = Pos->y - LightPnt->Pos.y;
		ray.z = Pos->z - LightPnt->Pos.z;

		rlen2 = (
			ray.x * ray.x +
			ray.y * ray.y +
			ray.z * ray.z
		);

		lsize2 = LightPnt->Size * LightPnt->Size;
		
		if( rlen2 < lsize2 )
		{
			if(render_lighting_point_lights_only || LightPnt->Type == POINT_LIGHT)
			{
				intensity = 1.0F - rlen2 / (int) lsize2; 
			}
			else if(LightPnt->Type == SPOT_LIGHT)
			{
				if( rlen2 > 0.0F )
				{
					rlen = (float) sqrt( rlen2 );
					ray.x /= rlen;
					ray.y /= rlen;
					ray.z /= rlen;
				}
				
				cosa = ( 
					ray.x * LightPnt->Dir.x +
					ray.y * LightPnt->Dir.y + 
					ray.z * LightPnt->Dir.z
				);
				
				if ( rlen2 > lsize2 * 0.5F )
				{
					if ( cosa > LightPnt->CosArc )
						intensity = (
							( ( lsize2 - rlen2 ) / ( 0.75F * lsize2 ) ) *
							( ( cosa - LightPnt->CosArc ) / ( 1.0F - LightPnt->CosArc )	));
					else
						goto NEXT_LIGHT;
				}
				else if ( rlen2 > MIN_LIGHT_SIZE ) 
				{
					cosarc2 = LightPnt->CosArc * (
							1.0F - 
							( lsize2 * 0.5F - rlen2 ) / 
							( lsize2 * 0.5F - MIN_LIGHT_SIZE ) 
					);
					if ( cosa > cosarc2 )
						intensity = (
							( ( lsize2 - rlen2 ) / ( lsize2 - MIN_LIGHT_SIZE ) ) *
							( ( cosa - cosarc2 ) / ( 1.0F - cosarc2 ) ));
					else
						goto NEXT_LIGHT;
				}
				else 
				{
					intensity = ( cosa > 0.0F ) ? 1.0F : 1.0F + cosa ;
				}
			}
			else
			{
				DebugPrintf("Unknown light type %d\n",
					LightPnt->Type);
				goto NEXT_LIGHT;
			}

			*R += LightPnt->r * intensity;
			*G += LightPnt->g * intensity;
			*B += LightPnt->b * intensity;
			*A += 255.0f * intensity;
		}
		
NEXT_LIGHT:

		LightPnt = LightPnt->NextVisible;
	}

	if( *R > 255.0F ) *R = 255.0F;
	if( *G > 255.0F ) *G = 255.0F;
	if( *B > 255.0F ) *B = 255.0F;
	if( *A > 255.0F ) *A = 255.0F;
}

#define MINUS( X, Y )\
	tmp = X;\
	tmp -= Y;\
	X = (tmp < 0) ? 0 :\
		(tmp > 255) ? 255 : tmp

#define ADD( X, Y )\
	MINUS( X, -Y )

// color = (vert + light) - blend
// where blend = 255 - color
#define MIX_COLOR_BLEND_LIGHT( COLOR, BLEND, LIGHT )\
	ADD( COLOR, LIGHT );\
	MINUS( COLOR, BLEND )

void light_vert( LVERTEX * vert, u_int8_t * color ) 
{
	int tmp;
	float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
	VECTOR world, v = {vert->x,vert->y,vert->z};
	MxV( &world_matrix, &v, &world );
	if( render_lighting_env_whiteout )
		do_whiteout_effect( &world, color );
	else if( render_lighting_env_water )
		do_water_effect( &world, color );
#ifndef LIGHT_EVERYTHING
	if( render_lighting_enabled )
#endif
		GetRealLightAmbientWorldSpace( &world, &r, &g, &b, &a );
	if(render_lighting_use_only_light_color)
	{
		color[0] = b;
		color[1] = g;
		color[2] = r;
		color[3] = a;
	}
	else if(render_lighting_use_only_light_color_and_blend)
	{
		color[0] = b;
		color[1] = g;
		color[2] = r;
		color[3] = a;
		ADD( color[0], render_color_blend_blue );
		ADD( color[1], render_color_blend_green );
		ADD( color[2], render_color_blend_red );
	}
	else
	{
		MIX_COLOR_BLEND_LIGHT( color[0], render_color_blend_blue,  b );
		MIX_COLOR_BLEND_LIGHT( color[1], render_color_blend_green, g );
		MIX_COLOR_BLEND_LIGHT( color[2], render_color_blend_red,   r );
	}
}

static void draw_vert( void * _vert, bool orthographic )
{
	LVERTEX * vert = (LVERTEX*) _vert;
	TLVERTEX * tlvert = (TLVERTEX*) _vert;
	if(orthographic)
	{
		set_color( tlvert->color );
		glTexCoord2f( tlvert->tu, tlvert->tv );
		glVertex2f( tlvert->x, tlvert->y );
	}
	else
	{
#ifdef NEW_LIGHTING
		COLOR c = vert->color;
		light_vert( vert, (u_int8_t*) &c );
		set_color( c );
#else
		set_color( vert->color );
#endif
		glTexCoord2f( vert->tu, vert->tv );
		glVertex3f( vert->x, vert->y, vert->z );
	}
}

// accept fragment if alpha value is greater than x
// alpha values are from 0-255 (8bit units)
// glAlphaFunc expects the number as a fraction
// if your images have an alpha value less than x
// then they will be ignored during rendering !!!

static void set_alpha_ignore( void )
{
	float x = 100.f;
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,(x/255.0f));
}

static void unset_alpha_ignore( void )
{
	glDisable(GL_ALPHA_TEST);
}

bool draw_render_object( RENDEROBJECT *renderObject, int primitive_type, bool orthographic )
{
	int group;
	LVERTEX * verts = (LVERTEX*) renderObject->lpVertexBuffer;
	TLVERTEX * tlverts = (TLVERTEX*) renderObject->lpVertexBuffer;
	WORD * indices = (WORD*) renderObject->lpIndexBuffer;

	//assert(renderObject->vbLocked == 0);
	
	if(orthographic)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, (double)render_info.ThisMode.w, 0.0, (double)render_info.ThisMode.h);
		// These next two steps allow us to specify screen location from top/left offets
		// invert the y axis, down is positive
		glScalef(1, -1, 1);
		// and move the origin from the bottom left corner to the upper left corner
		glTranslatef(0.0f, -((float)render_info.ThisMode.h), 0.0f);
	}

	for (group = 0; group < renderObject->numTextureGroups; group++)
	{
		int i;
		int startVert  = renderObject->textureGroups[group].startVert;
		int numVerts   = renderObject->textureGroups[group].numVerts;

		if(renderObject->textureGroups[group].colourkey)
			set_alpha_ignore();

		if( renderObject->textureGroups[group].texture )
		{
			GLuint texture = *(GLuint*)renderObject->textureGroups[group].texture;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		glBegin(primitive_type);

		// draw vertex list using index list
		if(renderObject->lpIndexBuffer)
		{
			int startIndex = renderObject->textureGroups[group].startIndex;
			int numIndices = renderObject->textureGroups[group].numTriangles * 3;
			for( i = 0; i < numIndices; i++ )
			{
				int indice = indices[ startIndex + i ];
				int vert = startVert + indice;
				if(orthographic)
					draw_vert( &tlverts[vert], orthographic );
				else
					draw_vert( &verts[vert], orthographic );
			}
		}
		// draw using only vertex list
		else
		{
			for( i = startVert; i < numVerts; i++ )
				if(orthographic)
					draw_vert( &tlverts[i], orthographic );
				else
					draw_vert( &verts[i], orthographic );
		}
		
		glEnd();

		if( renderObject->textureGroups[group].texture )
			glDisable(GL_TEXTURE_2D);

		if(renderObject->textureGroups[group].colourkey)
			unset_alpha_ignore();
	}

	if(orthographic)
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	return true;
}

#endif // GL == 1
