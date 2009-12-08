#ifdef OPENGL

#include "main.h"
#include "util.h"
#include "render.h"
#include "texture.h"
#include "file.h"
#include "SDL.h"
#include "SDL_opengl.h"

extern render_info_t render_info;

// capabilities of the opengl system

static struct
{
	float anisotropic; // anisotropic filtering for viewing textures at a flat angle
} caps;

static void detect_caps( void )
{
	//check whether anisotropic filtering extension is supported
	caps.anisotropic = 0.0f;
	if(strstr((char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &caps.anisotropic );
	DebugPrintf("render: anisotropic filtering support = %s\n",caps.anisotropic?"true":"false");
}

// unused in opengl
BOOL FSBeginScene(){ return TRUE; }
BOOL FSEndScene(){ return TRUE; }

// prototypes
void reset_trans( void );

// TODO - gl scales textures to be square automatically
//        maybe this should be removed ?
// TODO - all textures should be scaled to square
//		  other wise should get this from gl caps
BOOL bSquareOnly = FALSE;

//
// Texture Routines
//

uchar_t gamma_table[256];

void build_gamma_table( double gamma )
{
	double k;
	int i;

#ifndef DEBUG_ON
	if (gamma <= 0)
		gamma = 1.0;
#endif

	k = 255.0/pow(255.0, 1.0/gamma);
	
	for (i = 0; i <= 255; i++)
	{
		gamma_table[i] = (uchar_t)(k*(pow((double)i, 1.0/gamma)));
		if( i && !gamma_table[i] )
			gamma_table[i] = 1;
	}
}

void release_texture( LPTEXTURE texture ){
	if(!texture)
		return;
	glDeleteTextures( 1, texture );
	free(texture);
	texture = NULL;
}

BOOL create_texture(LPTEXTURE *t, const char *path, uint16 *width, uint16 *height, int numMips, BOOL * colorkey)
{
	GLuint * id = NULL;
	texture_image_t image;

	Change_Ext( path, image.path, ".PNG" );
	if( ! File_Exists( (char*) image.path ) )
	{
		DebugPrintf("Could not find texture file: %s\n",path);
		return TRUE;
	}

	if(load_image( &image, numMips )!=0)
	{
		DebugPrintf("couldn't load image\n");
		return FALSE;
	}

	// return values
	*width  = (uint16) image.w;
	*height = (uint16) image.h;
	(*colorkey) = (BOOL) image.colorkey;

	// employ colour key and gamma correction
	{
		int y, x;
		int size = 4;
		int pitch = size*image.w;
		for (y = 0; y < image.h; y++)
		{
			for (x = 0; x < image.w; x++)
			{
				// move to the correct offset in the data
				// y is the row and pitch is the size of a row
				// (x*size) is the length of each pixel data (column)
				DWORD index = (y*pitch)+(x*size);

				// image.data is packed in rgba
				image.data[index]   = (char) gamma_table[ (uchar_t) image.data[index]];	   // red
				image.data[index+1] = (char) gamma_table[ (uchar_t) image.data[index+1]];  // green
				image.data[index+2] = (char) gamma_table[ (uchar_t) image.data[index+2]];  // blue
				image.data[index+3] = (char) gamma_table[ (uchar_t) image.data[index+3]];  // alpha

				// colour key
				if( image.colorkey && (image.data[index] + image.data[index+1] + image.data[index+2]) == 0 )
					image.data[index+3] = 0; // alpha - pixel will not be rendered do to alpha value tests

			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// create a new opengl texture
	if( ! *t )
	{
		id = malloc(sizeof(GLuint));
		*t = (void*)id;
		glGenTextures(1, id);
		glBindTexture(GL_TEXTURE_2D, *id);
	}
	// updates an existing texture
	else
	{
		id = *t;
		glBindTexture(GL_TEXTURE_2D, *id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.w, image.h, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
	}

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// when texture area is large, bilinear filter the original
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// the texture wraps over at the edges (repeat)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// anisotropic settings
	if(caps.anisotropic)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, caps.anisotropic);

	// generates full range of mipmaps and scales to nearest power of 2
	if(gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.w, image.h, GL_RGBA, GL_UNSIGNED_BYTE, image.data) != 0)
	{
	   render_error_description(0);
	   return FALSE;
	}

	DebugPrintf( "Created texture: file=%s, width=%d, height=%d, colorkey=%s\n", 
		image.path, image.w, image.h, (image.colorkey ? "true" : "false") );

	destroy_image( &image );

	return TRUE;
}

BOOL update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colorkey)
{
	create_texture(&dstTexture, fileName, width, height, numMips, colorkey);
	return TRUE;
}

BOOL FSCreateTexture(LPTEXTURE *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{	
	return create_texture(texture, fileName, width, height, numMips, colourkey);
}

BOOL init_renderer( render_info_t * info )
{
	GLboolean b;
	glGetBooleanv(GL_STEREO,&b);

	DebugPrintf( "gl vendor='%s', renderer='%s', version='%s', shader='%s', stereo='%s'\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION),
		(b)?"true":"false");

	// this is way to long for OutputDebugString to show
	//DebugPrintf( "gl vendor='%s', renderer='%s', version='%s', extensions='%s', shaders='%s'\n",
	//	glGetString(GL_EXTENSIONS));

	// reminaing d3d9 initialization steps
	// doesn't sdl do this now?
		// enumerate and select a display mode
			// assign enumeration info to info->Mode
			// assign info->(CurrMode|ThisMode|WindowsDisplay|szClient)
			// set the viewport

	//
	// init render state
	//

	detect_caps();
	build_gamma_table(1.0f);
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH); // TODO - is there gouraud ?
	glDisable(GL_LIGHTING);
	reset_cull();
	reset_trans();

	// normally we don't draw back faces
	glPolygonMode(GL_BACK, GL_NONE);
	
	// wireframe mode
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);

	//
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//
	set_normal_states();

	//
	// everything went ok
	// fill in all the structures
	//

	// TODO - this should all be part of init_sdl_window

	info->bRenderingIsOK	= TRUE;
	info->NumModes			= 1;
	info->CurrMode			= 0;
	info->Mode				= (render_display_mode_t *) malloc(sizeof(render_display_mode_t));
	info->Mode[0].h			= render_info.default_mode.h;
	info->Mode[0].w			= render_info.default_mode.w;
	info->Mode[0].rate		= render_info.default_mode.rate;
	info->Mode[0].bpp		= render_info.default_mode.bpp;
	info->ThisMode			= info->Mode[ info->CurrMode ];
	info->WindowsDisplay	= info->Mode[ info->CurrMode ];
	info->szClient.cx		= info->ThisMode.w;
	info->szClient.cy		= info->ThisMode.h;
	info->WindowsDisplay.w  = info->ThisMode.w;
	info->WindowsDisplay.h  = info->ThisMode.h;

	if(!info->aspect_ratio)
	{
		info->aspect_ratio = (float) info->ThisMode.w / (float) info->ThisMode.h;
		DebugPrintf("render: aspect ratio set to %d:%d\n",info->ThisMode.w,info->ThisMode.h);
	}

	{
		render_viewport_t viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = info->ThisMode.w;
		viewport.Height = info->ThisMode.h;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		FSSetViewPort(&viewport);
	}

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
	render_error_description(0);
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

// accept fragment if alpha value is greater than x
// alpha values are from 0-255 (8bit units)
// glAlphaFunc expects the number as a fraction
// if your images have an alpha value less than x
// then they will be ignored during rendering !!!

void set_alpha_ignore( void )
{
	float x = 100.f;
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,(x/255.0f));
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
	int width = rect->x2 - rect->x1;
	int height = rect->y2 - rect->y1;
	int x = rect->x1;
	int y = render_info.ThisMode.h - rect->y1 - height;
	// here we employ a stencil buffer so that we
	// only clear the desired part of the screen
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, width, height);
	//
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//
	glDisable(GL_SCISSOR_TEST);
	return TRUE;
}

BOOL FSClearBlack(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return TRUE;
}

BOOL FSClearDepth(XYRECT * rect)
{
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	return TRUE;
}

BOOL FSGetViewPort(render_viewport_t *view)
{
	GLint i[4];
	GLfloat f[2];
	// scalex/y are not modified here
	// xywh
	glGetIntegerv( GL_VIEWPORT, i );
	view->X	= i[0];
	view->Y	= render_info.ThisMode.h - (i[1] + i[3]);
	view->Width	= i[2];
	view->Height = i[3];
	// near,far
	glGetFloatv( GL_DEPTH_RANGE, f );
	view->MinZ = f[0];
	view->MaxZ = f[1];
	return TRUE;
}

// TODO - we can probably use glScalef and glTranslatef
//        to invert the viewport dimentions

BOOL FSSetViewPort(render_viewport_t *view)
{
	// render_viewport_t x/y starts top/left
	// but glViewport starts from bottom/left
	int bottom = render_info.ThisMode.h - (view->Y + view->Height);
	glViewport(	view->X, bottom, (GLint) view->Width, (GLint) view->Height	);
	// sets the min/max depth values to render
	// default is max 1.0f and min 0.0f
	// this is here for compatibility with d3d9
	glDepthRange(view->MinZ,view->MaxZ);
	// i want to know if this is ever changed
	// as most likely we don't need this info in render_viewport_t
	if(view->MaxZ!=1.0f || view->MinZ!=0.0f)
	{
		DebugPrintf("-------------------------------\n");
		DebugPrintf("max/min z used: max=%d min=%d\n",
			view->MaxZ, view->MinZ);
		DebugPrintf("-------------------------------\n");
	}
	// ScaleX|Y are not even part of the d3d9 struct anymore
	// they were part of the old d3d6 viewport settings
	// from testing d3d9 passes the values along untouched
	// probably need some testing here to see how d3d6 worked
	// forsaken still uses these values so we need them
	return TRUE;
}

GLfloat proj_matrix[4][4];
BOOL FSSetProjection( RENDERMATRIX *matrix )
{
	memcpy(&proj_matrix,&matrix->m,sizeof(proj_matrix));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&matrix->m);
	return TRUE;
}

//
// d3d stored the world/view matrixes
// and then multiplied them together before rendering
// in the following order: world * view * projection
// opengl handles only world and projection
// so we must emulate the behavior of world*view
//
// although we multiply the arguments backwards view*world
// other wise instead of an object rotating you end up turning the world
// causing pickups etc.. to fly around the entire level wicked fast!
//

GLfloat view_matrix[4][4];
GLfloat world_matrix[4][4];

static void reset_modelview( void )
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&view_matrix);
	glMultMatrixf((GLfloat*)&world_matrix);
}

BOOL FSSetView( RENDERMATRIX *matrix )
{
	memcpy(&view_matrix,&matrix->m,sizeof(view_matrix));
	reset_modelview();
	return TRUE;
}

BOOL FSSetWorld( RENDERMATRIX *matrix )
{	
	memcpy(&world_matrix,&matrix->m,sizeof(world_matrix));
	reset_modelview();
	return TRUE;
}

BOOL FSGetWorld(RENDERMATRIX *matrix)
{
	memcpy(&matrix->m,&world_matrix,sizeof(matrix->m));
	return TRUE;
}

//
// using the concept of index/vertex buffers in opengl is a bit different
// so for now I'm just going going to return a pointer to memory
// then just convert the index/vertex buffer to a pure vertex buffer on draw
//
// the vertex/index pointers could probably end up being the glGen* id
// then internal display lists can be generated for the non dynamic functions
// so we can render the static objects as display lists
//

BOOL FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(LVERTEX) );
	return TRUE;
}
BOOL FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{FSCreateVertexBuffer(renderObject, numVertices); return TRUE;}

BOOL FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = malloc( numIndices * 3 * sizeof(WORD) );
	return TRUE;
}
BOOL FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{return FSCreateIndexBuffer(renderObject,numIndices);}

BOOL FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{(*indices) = renderObject->lpIndexBuffer; return TRUE;}
BOOL FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{(*verts) = renderObject->lpVertexBuffer; return TRUE;}
BOOL FSUnlockIndexBuffer(RENDEROBJECT *renderObject){return TRUE;}
BOOL FSUnlockVertexBuffer(RENDEROBJECT *renderObject){return TRUE;}

BOOL FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = malloc( numVertices * sizeof(TLVERTEX) ); 
	return TRUE;
}
BOOL FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{*verts = (void*)renderObject->lpVertexBuffer; return TRUE;}

static void set_color( COLOR c )
{
	// COLOR is the value loaded from the files
	// it's packed as uchar[4] (bgra) and glColor expects (rgba)
	// so we flip the red/blue values with each other
	c = (c & 0xff00ff00) | ((c & 0x00ff0000) >> 16) | ((c & 0x000000ff) << 16);
	glColor4ubv(&c);
}

static void draw_vert( void * _vert, BOOL orthographic )
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
		set_color( vert->color );
		glTexCoord2f( vert->tu, vert->tv );
		glVertex3f( vert->x, vert->y, vert->z );
	}
}

static void set_material( RENDERMATERIAL * m )
{
	if(!m)return;
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	 (GLfloat*)&m->Diffuse  );
	glMaterialfv( GL_FRONT, GL_AMBIENT,	 (GLfloat*)&m->Ambient  );
	glMaterialfv( GL_FRONT, GL_SPECULAR, (GLfloat*)&m->Specular );
	glMaterialfv( GL_FRONT, GL_EMISSION, (GLfloat*)&m->Emissive );
	// gl specular exponent =~ d3d specular sharpness
	glMaterialf ( GL_FRONT, GL_SHININESS,(GLfloat)m->Power      );
}

static BOOL draw_render_object( RENDEROBJECT *renderObject, int primitive_type, BOOL orthographic )
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

	set_material( &renderObject->material );

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

	return TRUE;
}

BOOL draw_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_TRIANGLES,FALSE);}
BOOL draw_2d_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_TRIANGLES,TRUE);}
BOOL draw_line_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_LINES,FALSE);}

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
			// this is just a pointer to Tloadheader
			// we do not need to worry about releasing it
			renderObject->textureGroups[i].texture = NULL;
		}
	}
}

//
// loop and clear out all gl errors
// it will does not use the argument
// and the last argument is returned
//

const char * render_error_description( int e )
{
	GLenum error;
	const GLubyte * str = NULL;
	while( ( error = glGetError() ) != GL_NO_ERROR )
	{
		str = gluErrorString(error);
		DebugPrintf("render error: %s\n",str);
	}
	return (const char *) str;
}

#endif // OPENGL
