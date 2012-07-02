#if defined(OPENGL1) || defined(OPENGL3)
#include "render_gl_shared.h"

// CHECK_GL_ERRORS will set this when called
GLenum render_last_gl_error = GL_NO_ERROR;

// description of last render error
const char * render_error_description( int e )
{
	CHECK_GL_ERRORS;
	if(render_last_gl_error != GL_NO_ERROR)
		return (const char *) gluErrorString(render_last_gl_error);
	else
		return NULL;
}

#endif // THIS FILE
