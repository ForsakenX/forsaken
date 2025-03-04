#ifndef MATH_ERROR
#define MATH_ERROR
#ifdef DEBUG_ON

extern void InitMathErrors( void );
extern int DebugMathErrors( void );

// mingw still appears to support matherr so we'll leave it for now
// not sure if msvc still does but it appears matherr is going away
#if defined(WIN32) && !defined(MINGW)

// note this is no longer really needed either
// it was only here for matherr support and should be removed along with it
// https://stackoverflow.com/questions/29201515/what-does-d-default-source-do
// https://stackoverflow.com/questions/5582211/what-does-define-gnu-source-imply
// https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
#ifndef _SVID_SOURCE
#define _SVID_SOURCE
#endif

#include <errno.h>
#include <math.h>

#else // NOT WIN32

// https://man7.org/linux/man-pages/man3/matherr.3.html
// https://man7.org/linux/man-pages/man7/math_error.7.html
// https://man7.org/linux/man-pages/man3/fenv.3.html
#include <math.h>
#include <errno.h>
#include <fenv.h>

#endif

#endif // DEBUG_ON
#endif // MATH_ERROR
