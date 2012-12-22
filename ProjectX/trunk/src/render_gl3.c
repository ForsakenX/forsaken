#ifdef OPENGL3
#include "render_gl_shared.h"

struct texture
{
	GLuint id;
	// Possibly later: GLuint bump_id;
};

// Fallback shaders that emulate original Forsaken rendering.

// Things a vertex shader must take into account:
// - orthographic a.k.a. pretransformed a.k.a. 2D mode:
//   - "orthographic" uniform var
//   - "ortho_proj" uniform matrix
//   - different vertex layout (LVERTEX vs TLVERTEX)
// - vertex colors are in BGRA format, not RGBA

static const char *default_vertex_shader =
	"#version 150\n"
	"\n"
	"uniform bool orthographic;\n"
	"\n"
	"uniform mat4 mvp;\n"
	"uniform mat4 ortho_proj;\n"
	"\n"
	"in vec3 pos;\n"
	"in vec4 tlpos;\n"
	"in vec4 vcolor;\n"
	"in vec2 vtexc;\n"
	"\n"
	"out vec4 color;\n"
	"out vec2 texc;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"    if (orthographic)\n"
	"    {\n"
	"        gl_Position = ortho_proj * tlpos;\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        gl_Position = mvp * vec4(pos, 1.0);\n"
	"    }\n"
	"    color = vcolor.bgra;\n"
	"    texc = vtexc;\n"
	"}\n"
;

// Things a fragment shader must take into account:
// - color-keying (discard if alpha <= 100/255; don't ask)
//   - "colorkeying_enabled" uniform var
// - texturing (possibly disabled)
//   - "texturing_enabled" uniform var

static const char *default_fragment_shader =
	"#version 150\n"
	"\n"
	"uniform bool colorkeying_enabled;\n"
	"uniform bool texturing_enabled;\n"
	"uniform sampler2D tex;\n"
	"\n"
	"in vec4 color;\n"
	"in vec2 texc;\n"
	"\n"
	"out vec4 fcolor;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"    vec2 dx;\n"
	"    vec2 dy;\n"
	"\n"
	"    // This *may* be needed - see:\n"
	"    // http://www.opengl.org/wiki/GLSL_Sampler\n"
	"    // but I'm not sure because it's not really clear\n"
	"    // on just how 'uniform' the control flow has to be\n"
	"    dx = dFdx(texc);\n"
	"    dy = dFdy(texc);\n"
	"    if ( texturing_enabled )\n"
	"        fcolor = textureGrad(tex, texc, dx, dy) * color;\n"
	"    else\n"
	"        fcolor = color;\n"
	"    if ( colorkeying_enabled && fcolor.a <= (100.0/255.0) )\n"
	"        discard;\n"
	"}\n"
;

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

// poly modes

void render_mode_wireframe(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void render_mode_points(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void render_mode_fill(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// unused in opengl
bool FSBeginScene(){ return true; }
bool FSEndScene(){ return true; }

// prototypes
void reset_trans( void );

// TODO	- should get this from gl caps ?
bool bSquareOnly = true;

//
// Texture Routines
//

u_int8_t gamma_table[256];

void build_gamma_table( double gamma )
{
	double k;
	int i;

	DebugPrintf("build_gamma_table( %f )\n",gamma);

#ifndef DEBUG_ON
	if (gamma <= 0)
		gamma = 1.0;
#endif

	k = 255.0/pow(255.0, 1.0/gamma);
	
	for (i = 0; i <= 255; i++)
	{
		gamma_table[i] = (u_int8_t)(k*(pow((double)i, 1.0/gamma)));
		if( i && !gamma_table[i] )
			gamma_table[i] = 1;
	}
}

void release_texture( LPTEXTURE texture ){
	struct texture *texdata;
	if(!texture)
		return;
	texdata = (struct texture *) texture;
	glDeleteTextures( 1, &texdata->id );
	CHECK_GL_ERRORS;
	free(texture);
}

bool create_texture(LPTEXTURE *t, const char *path, u_int16_t *width, u_int16_t *height, int numMips, bool * colorkey)
{
	struct texture *texdata;
	texture_image_t image;

	Change_Ext( path, image.path, ".PNG" );
	if( ! File_Exists( (char*) image.path ) )
	{
		DebugPrintf("Could not find texture file: %s\n",path);
		return true;
	}

	if(load_image( &image, numMips )!=0)
	{
		DebugPrintf("couldn't load image\n");
		return false;
	}

	// return values
	*width  = (u_int16_t) image.w;
	*height = (u_int16_t) image.h;
	(*colorkey) = (bool) image.colorkey;

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
				image.data[index]   = (char) gamma_table[ (u_int8_t) image.data[index]];	   // red
				image.data[index+1] = (char) gamma_table[ (u_int8_t) image.data[index+1]];  // green
				image.data[index+2] = (char) gamma_table[ (u_int8_t) image.data[index+2]];  // blue
				image.data[index+3] = (char) gamma_table[ (u_int8_t) image.data[index+3]];  // alpha

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
		texdata = malloc(sizeof(struct texture));
		glGenTextures(1, &texdata->id);
		glBindTexture(GL_TEXTURE_2D, texdata->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
		CHECK_GL_ERRORS;
	}
	// updates an existing texture
	else
	{
		texdata = (struct texture *) *t;
		glBindTexture(GL_TEXTURE_2D, texdata->id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.w, image.h, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
		CHECK_GL_ERRORS;
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

	// generate mipmaps
	glGenerateMipmap( GL_TEXTURE_2D );

	if ( render_error_description(0) )
		return false;

	*t = (LPTEXTURE) texdata;

	DebugPrintf( "Created texture: file=%s, width=%d, height=%d, colorkey=%s\n", 
		image.path, image.w, image.h, (image.colorkey ? "true" : "false") );

	destroy_image( &image );

	return true;
}

bool update_texture_from_file(LPTEXTURE dstTexture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colorkey)
{
	create_texture(&dstTexture, fileName, width, height, numMips, colorkey);
	return true;
}

bool FSCreateTexture(LPTEXTURE *texture, const char *fileName, u_int16_t *width, u_int16_t *height, int numMips, bool * colourkey)
{	
	return create_texture(texture, fileName, width, height, numMips, colourkey);
}

static print_info( void )
{
	GLboolean b;
	glGetBooleanv(GL_STEREO,&b);

	DebugPrintf( "gl vendor='%s', renderer='%s', version='%s', shader='%s', stereo='%s'\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION),
		(b)?"true":"false");

	DebugPrintf( "extensions='%s'\n", glGetString(GL_EXTENSIONS));
}

GLuint vertex_shader = 0;
GLuint fragment_shader = 0;
GLuint current_program = 0;

static GLuint new_shader( GLenum type, const char *src, char **log )
{
	GLuint shader;
	int compile_ok;
	GLsizei log_size;
	static char *info_log = NULL;

	if ( info_log )
		free( info_log );

	shader = glCreateShader( type );
	glShaderSource( shader, 1, &src, NULL );
	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compile_ok );
	if ( compile_ok == GL_FALSE )
	{
		if ( log )
		{
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_size );
			info_log = malloc( log_size );
			glGetShaderInfoLog( shader, log_size, NULL, info_log );
			*log = info_log;
			return 0;
		}
	}

	CHECK_GL_ERRORS;

	if ( log )
		*log = NULL;

	return shader;
}

static bool update_shader_program( char **log )
{
	int link_ok;
	GLsizei log_size;
	static char *info_log = NULL;

	if ( info_log )
		free( info_log );

	if ( current_program )
	{
		glUseProgram( 0 );
		glDeleteProgram( current_program );
	}

	current_program = glCreateProgram();
	glAttachShader( current_program, vertex_shader );
	glAttachShader( current_program, fragment_shader );
	glLinkProgram( current_program );
	glGetProgramiv( current_program, GL_LINK_STATUS, &link_ok );
	if ( link_ok == GL_FALSE )
	{
		if ( log )
		{
			glGetProgramiv( current_program, GL_INFO_LOG_LENGTH, &log_size );
			info_log = malloc( log_size );
			glGetProgramInfoLog( current_program, log_size, NULL, info_log );
			*log = info_log;
			return false;
		}
	}

	glUseProgram( current_program );
	CHECK_GL_ERRORS;

	if ( log )
		*log = NULL;

	return true;
}

static void set_default_shaders( void )
{
	char *info_log;

	// Clean up the existing shaders if any
	if ( vertex_shader )
	{
		glDeleteShader( vertex_shader );
		CHECK_GL_ERRORS;
		vertex_shader = 0;
	}
	if ( fragment_shader )
	{
		glDeleteShader( fragment_shader );
		CHECK_GL_ERRORS;
		fragment_shader = 0;
	}

	vertex_shader = new_shader( GL_VERTEX_SHADER, default_vertex_shader, &info_log );
	if ( ! vertex_shader )
	{
		DebugPrintf( "Failed to compile vertex shader! Info log:\n-----\n%s\n-----\n", info_log );
		return;
	}
	fragment_shader = new_shader( GL_FRAGMENT_SHADER, default_fragment_shader, &info_log );
	if ( ! fragment_shader )
	{
		DebugPrintf( "Failed to compile fragment shader! Info log:\n-----\n%s\n-----\n", info_log );
		return;
	}
	if ( update_shader_program( &info_log ) == false )
	{
		DebugPrintf( "Failed to build shader program! Info log:\n-----\n%s\n-----\n", info_log );
		return;
	}
}

static set_defaults( void )
{
	build_gamma_table(1.0f); // 1.0f means no gamma change
	set_default_shaders();
	reset_cull();
	reset_trans();
	set_normal_states(); // default preset render mode
	CHECK_GL_ERRORS;
}

static resize_viewport( int width, int height )
{
	render_viewport_t viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	FSSetViewPort(&viewport);
}

bool render_init( render_info_t * info )
{
	print_info();
	detect_caps();
	set_defaults();
	resize_viewport(info->ThisMode.w, info->ThisMode.h);
	if(info->wireframe)
		render_mode_wireframe();
	info->ok_to_render = true;
	return true;
}

void render_cleanup( render_info_t * info )
{
	info->ok_to_render = false;
	// ???
}

extern bool sdl_init_video( void );
bool render_mode_select( render_info_t * info )
{
	render_cleanup( info );
	if(!sdl_init_video())
		return false;
	//if(!render_init( info ))
	//	return false;
	return true;
}

// TODO - in d3d9 render_flip would detect a lost device
//		lost as in alt+tab (etc) which caused video memory to dump
//		at this point we should set needs_reset = true

static bool needs_reset = false;

bool render_reset( render_info_t * info )
{
	if(!needs_reset)
		return false;
	if(!render_mode_select( info ))
		return false;
	needs_reset = false;
	return true;
}

void render_set_filter( bool red, bool green, bool blue )
{
	glColorMask(red?1:0, green?1:0, blue?1:0, 1);
}

bool render_flip( render_info_t * info )
{
	sdl_render_present(info);
	render_error_description(0);
	return true;
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

extern float framelag;

// 16.7 ~ 1/85 * 71 / 0.05;
// 85 is my fps, 71 is the framelag multiplier, 0.05 is a suitable
// alpha for that fps.

void set_whiteout_state( void )
{
	// higher = more white; < 1.0 makes it darker
	float whiteness = 5.0f;

	float src_a = framelag / 16.7f;
	float dst_a = src_a / whiteness;

	glEnable(GL_BLEND);
	glBlendFunc(GL_CONSTANT_ALPHA,GL_ONE_MINUS_CONSTANT_COLOR); // src, dest
	glBlendColor(dst_a, dst_a, dst_a, src_a); // src, dest
}

// TODO - is the stencil buffer ever cleared ?
// TODO - do we even use the stencil buffer ?
// TODO - FSClear is meant to clear current viewport
//        perhaps we can automate and remove need for rect arg ?

// clears color/zbuff same time to opaque black
bool FSClear(XYRECT * rect)
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
	return true;
}

bool FSClearBlack(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

bool FSClearDepth(XYRECT * rect)
{
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	return true;
}

bool FSGetViewPort(render_viewport_t *view)
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
	return true;
}

// The orthographic projection matrix depends on the viewport
// dimensions. We don't update it right away but mark it `dirty'
// and have the drawing routine update it when it's needed.

bool ortho_matrix_needs_update;

bool FSSetViewPort(render_viewport_t *view)
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

	ortho_matrix_needs_update = true;
	return true;
}

void ortho_update ( GLuint current_program )
{
	MATRIX m;
	float left, right, bottom, top, near, far;
	GLuint u_ortho_matrix;

	if ( ortho_matrix_needs_update && ( u_ortho_matrix = glGetUniformLocation( current_program, "ortho_proj" ) ) >= 0 )
	{
		left = 0.0f;
		right = render_info.ThisMode.w;
		bottom = 0.0f;
		top = render_info.ThisMode.h;
		near = -1.0f;
		far = 1.0f;
		memset(&m, 0, sizeof(MATRIX));
		m._11 = 2.0f/(right-left);
		// vertical flip ...
		m._22 = (2.0f/(top-bottom)) * -1.0f;
		m._33 = -2.0f/(far-near);
		m._14 = -(right+left)/(right-left);
		// ... and translate to make top left the origin
		m._24 = -(top+bottom)/(top-bottom) + 2.0f;
		m._34 = -(far+near)/(far-near);
		m._44 = 1.0f;
		glUniformMatrix4fv( u_ortho_matrix, 1, GL_TRUE, &m );
		CHECK_GL_ERRORS;
		ortho_matrix_needs_update = false;
	}
}

// These matrices are eventually combined (multiplied together) at
// render time. We update them locally and mark the combined
// model-view-projection (MVP) matrix as dirty.
//
// The drawing routine should call mvp_update which will update the
// MVP matrix if needed.

bool mvp_needs_update;

MATRIX proj_matrix;

bool FSSetProjection( RENDERMATRIX *matrix )
{
	memmove(&proj_matrix,&matrix->m,sizeof(proj_matrix));//memcpy
	mvp_needs_update = true;
	return true;
}

MATRIX view_matrix;
MATRIX world_matrix;

bool FSSetView( RENDERMATRIX *matrix )
{
	memmove(&view_matrix,&matrix->m,sizeof(view_matrix));//memcpy
	mvp_needs_update = true;
	return true;
}

bool FSSetWorld( RENDERMATRIX *matrix )
{	
	memmove(&world_matrix,&matrix->m,sizeof(world_matrix));//memcpy
	mvp_needs_update = true;
	return true;
}

bool FSGetWorld(RENDERMATRIX *matrix)
{
	memmove(&matrix->m,&world_matrix,sizeof(matrix->m));//memcpy
	return true;
}

// This kinda sucks: MATRIX and RENDERMATRIX are supposed to be stored
// in row-major order (see include/new3d.h and render.h respectively),
// but the above matrices appear to be delivered to us already in
// column-major order, which is in fact the way gl{Load,Mult}Matrixf
// expect them (see render_opengl.c).
//
// To compensate for this, we need to multiply matrices in *reverse*
// order. The resulting matrix will also be in column-major order,
// hence the transpose parameter should be GL_FALSE.
//
// See also: http://en.wikipedia.org/wiki/Transpose#Properties

static void mvp_update( GLuint current_program )
{
	MATRIX mvp;
	GLuint u_mvp;

	if ( mvp_needs_update && ( u_mvp = glGetUniformLocation( current_program, "mvp" ) ) >= 0 )
	{
		MatrixMultiply( &world_matrix, &view_matrix, &mvp );
		MatrixMultiply( &mvp,          &proj_matrix, &mvp );
		glUniformMatrix4fv( u_mvp, 1, GL_FALSE, &mvp );
		CHECK_GL_ERRORS;
		mvp_needs_update = false;
	}
}

//
// These functions map nicely to OpenGL's Vertex Buffer Objects (VBOs).
//

static LPVERTEXBUFFER _create_buffer( int size, GLenum type, GLenum gettype, GLenum usage )
{
	// Need to bind the new buffer in order to specify its parameters,
	// so temporarily store the old binding in a variable.
	GLuint oldvbo;
	GLuint vbo;

	glGenBuffers( 1, &vbo );
	glGetIntegerv( gettype, &oldvbo );
	glBindBuffer( type, vbo );
	glBufferData( type, size, NULL, usage );
	// Restore old binding
	glBindBuffer( type, oldvbo );

	CHECK_GL_ERRORS;

	return (LPVERTEXBUFFER) vbo;
}

#define create_buffer( size, type, usage ) \
	_create_buffer( size, type, type ## _BINDING, usage )

bool FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(LVERTEX), GL_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(LVERTEX), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSCreateNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	renderObject->lpNormalBuffer = create_buffer( numNormals * sizeof(NORMAL), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicNormalBuffer(RENDEROBJECT *renderObject, int numNormals)
{
	renderObject->lpNormalBuffer = create_buffer( numNormals * sizeof(NORMAL), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = create_buffer( numIndices * 3 * sizeof(WORD), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW );
	return true;
}
bool FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	renderObject->lpIndexBuffer = create_buffer( numIndices * 3 * sizeof(WORD), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

// In OpenGL you can only map buffers currently bound to a predefined
// buffer binding point ("target"), so we save the currently bound
// buffer and restore it on unlock.
//
// There is still the restriction that only one buffer of a certain
// type may be locked at the same time. If this is a problem then this
// should be rewritten to use a locally malloc'ed buffer and update
// the real buffer on unlock using glBufferSubData.

static GLuint old_array_buf = 0;
static GLuint old_index_buf = 0;

bool FSLockVertexBuffer(RENDEROBJECT *renderObject, LVERTEX **verts)
{
	if ( old_array_buf )
	{
		DebugPrintf( "Tried to lock more than one vertex buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_array_buf );
	glBindBuffer( GL_ARRAY_BUFFER, (GLuint) renderObject->lpVertexBuffer );
	*verts = (LVERTEX *) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*verts)
	{
		DebugPrintf("FSLockVertexBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockVertexBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ARRAY_BUFFER, old_array_buf );
	old_array_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSLockNormalBuffer(RENDEROBJECT *renderObject, NORMAL **normals)
{
	if ( old_array_buf )
	{
		DebugPrintf( "Tried to lock more than one vertex buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &old_array_buf );
	glBindBuffer( GL_ARRAY_BUFFER, (GLuint) renderObject->lpNormalBuffer );
	*normals = (NORMAL *) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*normals)
	{
		DebugPrintf("FSLockNormalBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockNormalBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ARRAY_BUFFER, old_array_buf );
	old_array_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	if ( old_index_buf )
	{
		DebugPrintf( "Tried to lock more than one index buffer at once\n" );
		return false;
	}
	glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_index_buf );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, (GLuint) renderObject->lpIndexBuffer );
	*indices = (WORD *) glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY );
	if(!*indices)
	{
		DebugPrintf("FSLockIndexBuffer: glMapBuffer returned NULL\n");
		return false;
	}
	CHECK_GL_ERRORS;
	return true;
}

bool FSUnlockIndexBuffer(RENDEROBJECT *renderObject)
{
	bool ret = ( glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) == GL_TRUE );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, old_index_buf );
	old_index_buf = 0;
	CHECK_GL_ERRORS;
	return ret;
}

bool FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
	renderObject->lpVertexBuffer = create_buffer( numVertices * sizeof(TLVERTEX), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW );
	return true;
}

bool FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, TLVERTEX **verts)
{
	return FSLockVertexBuffer( renderObject, (LVERTEX **) verts );
}

/* Draw render object:
 * - if 2D (orthographic), set up appropriately:
 *   - orthographic projection matrix
 *   - ... plus scaling and translation for Y-flipping (T*S*P)
 *   else:
 *   - update mvp if necessary (mvp_needs_update)
 * - for each texture group (renderObject->numTextureGroups)
 *   - group = &renderObject->textureGroups[i]
 *   - if group->colourkey, enable color-keying
 *   - if group->texture, enable texturing and bind
 *     renderObject->textureGroups[group].texture
 *   - draw group->numVerts elements starting at group->startVert
 */

static bool draw_render_object( RENDEROBJECT *renderObject, GLenum primitive_type, bool orthographic )
{
	static const struct
	{
		const char *name;
		int components;
		GLenum type;
		GLboolean normalized;
		int offset;
	} normal_attr[] =
	{
		{ "pos",    3, GL_FLOAT,         GL_FALSE, 0  },
		{ "vcolor", 4, GL_UNSIGNED_BYTE, GL_TRUE,  12 }, // 3*float
		{ "vtexc",  2, GL_FLOAT,         GL_FALSE, 16 }, // 3*float + 1*COLOR
		{ NULL,     0, 0,                0,        0  }
	}, ortho_attr[] =
	{
		{ "tlpos",  4, GL_FLOAT,         GL_FALSE, 0 },
		{ "vcolor", 4, GL_UNSIGNED_BYTE, GL_TRUE,  16 }, // 4*float
		{ "vtexc",  2, GL_FLOAT,         GL_FALSE, 20 }, // 4*float + 1*COLOR
		{ NULL,     0, 0,                0,        0  }
	}, *attr;
	//GLuint current_program;
	GLuint u_ortho;
	GLuint u_colorkey;
	GLuint u_enabletex;
	TEXTUREGROUP *group;
	struct texture *texdata;
	int loc;
	int i;

	glBindBuffer( GL_ARRAY_BUFFER, renderObject->lpVertexBuffer );

	if ( renderObject->lpIndexBuffer )
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, renderObject->lpIndexBuffer );
	else
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	//glGetIntegerv( GL_CURRENT_PROGRAM, &current_program );
	// assert( current_program != 0 );

	CHECK_GL_ERRORS;

	// Tell OpenGL about the buffer data layout
	// see the LVERTEX and TLVERTEX definitions inside include/new3d.h
	attr = orthographic ? ortho_attr : normal_attr;
	for ( i=0; attr[i].name; i++ )
	{
		loc = glGetAttribLocation( current_program, attr[i].name );
		if (loc >= 0)
		{
			glVertexAttribPointer(
				loc,
				attr[i].components,
				attr[i].type,
				attr[i].normalized,
				orthographic ? sizeof(TLVERTEX) : sizeof(LVERTEX),
				attr[i].offset
			);
			glEnableVertexAttribArray( loc );
		}
	}

	CHECK_GL_ERRORS;

	// tell it about the normal buffer
	if ( renderObject->lpNormalBuffer )
	{
		glBindBuffer( GL_ARRAY_BUFFER, renderObject->lpNormalBuffer );
		loc = glGetAttribLocation( current_program, "vnormal" );
		if (loc >= 0)
		{
			glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, sizeof(NORMAL), 0 );
			glEnableVertexAttribArray( loc );
		}
	}

	CHECK_GL_ERRORS;

	// Update and use the appropriate model/view/projection matrix
	if ( orthographic )
		ortho_update( current_program );
	else
		mvp_update( current_program );

	// This uniform tells the vertex shader which matrix to use
	if ((u_ortho = glGetUniformLocation(current_program, "orthographic")) >= 0)
		glUniform1i( u_ortho, orthographic ? GL_TRUE : GL_FALSE );

	for ( i = 0; i < renderObject->numTextureGroups; i++ )
	{
		group = &renderObject->textureGroups[i];
		if ( (u_colorkey = glGetUniformLocation(current_program, "colorkeying_enabled")) >= 0 )
			glUniform1i( u_colorkey, group->colourkey ? GL_TRUE : GL_FALSE );
		if ( (u_enabletex = glGetUniformLocation(current_program, "texturing_enabled")) >= 0 )
		{
			glUniform1i( u_enabletex, group->texture ? GL_TRUE : GL_FALSE );
			if ( group->texture )
			{
				texdata = (struct texture *) group->texture;
				glBindTexture( GL_TEXTURE_2D, texdata->id );
			}
		}
		glDrawElementsBaseVertex( primitive_type, group->numTriangles * 3, GL_UNSIGNED_SHORT, group->startIndex * sizeof(WORD), group->startVert );
	}

	CHECK_GL_ERRORS;

	for ( i=0; attr[i].name; i++ )
	{
		loc = glGetAttribLocation( current_program, attr[i].name );
		if (loc >= 0)
			glDisableVertexAttribArray( loc );
	}

	if ( renderObject->lpNormalBuffer )
	{
		loc = glGetAttribLocation( current_program, "vnormal" );
		if (loc >= 0)
			glDisableVertexAttribArray( loc );
	}

	CHECK_GL_ERRORS;

	return true;
}

bool draw_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_TRIANGLES,false);}
bool draw_2d_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_TRIANGLES,true);}
bool draw_line_object(RENDEROBJECT *renderObject){return draw_render_object(renderObject,GL_LINES,false);}

void FSReleaseRenderObject(RENDEROBJECT *renderObject)
{
	int i;
	if (renderObject->lpVertexBuffer)
	{
		glDeleteBuffers( 1, &renderObject->lpVertexBuffer );
		renderObject->lpVertexBuffer = NULL;
	}
	if (renderObject->lpNormalBuffer)
	{
		glDeleteBuffers( 1, &renderObject->lpNormalBuffer );
		renderObject->lpNormalBuffer = NULL;
	}
	if (renderObject->lpIndexBuffer)
	{
		glDeleteBuffers( 1, &renderObject->lpIndexBuffer );
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

#endif // OPENGL3
