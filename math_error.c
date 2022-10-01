// see math_error.h for an explain of some details
#ifdef DEBUG_ON
#if defined(WIN32) && !defined(MINGW)
#include "main.h"
#include "new3d.h"
#include "util.h"
int acos_exceptions = 0;
int sqrt_exceptions = 0;
int pow_exceptions = 0;
int unhandled_exceptions = 0;
int total_exceptions = 0;
#define MAX_EXCEPTION_TYPES (6)
int type_exceptions[ MAX_EXCEPTION_TYPES ];
int unknown_exceptions = 0;
static char * MathErrStrings[] = { 
	"Domain",
	"Singularity",
	"Overflow",
	"Underflow",
	"Total Loss of significance",
	"Partial Loss of significance",
};
#ifdef WIN32
int     __cdecl _matherr(struct _exception * except)
#else
int matherr(struct exception * except)
#endif
{
	total_exceptions++;
	DebugPrintf( "matherr(#%d) %s( %lf, %lf ) = %lf %s\n",
		total_exceptions,
		except->name, except->arg1, except->arg2,
		except->retval, MathErrStrings[ except->type - 1 ] );
	if ( except->type > 0 && except->type <= MAX_EXCEPTION_TYPES )
		type_exceptions[ except->type - 1 ]++;
	else
		unknown_exceptions++;
	switch( except->type )
	{
#ifdef WIN32
		case _DOMAIN:
#else
		case DOMAIN:
#endif
			if( !strcasecmp( except->name, "acos" ) )
			{
				if( ( except->arg1 < -1.0 ) && ( except->arg1 > -1.0001 ) )
				{
					except->retval = PI;
					acos_exceptions++;
					return( 1 );
				}

				if( ( except->arg1 > 1.0 ) && ( except->arg1 < 1.0001 ) )
				{
					except->retval = 0;
					acos_exceptions++;
					return( 1 );
				}
			}
			else if ( !strcasecmp( except->name, "sqrt" ) )
			{
				if ( except->arg1 < 0.0 )
				{
					sqrt_exceptions++;
					return 0; // not handled, needs fixing...
				}
			}
	  		break;
#ifdef WIN32
		case _SING:
		case _OVERFLOW:
		case _PLOSS:
		case _TLOSS:
			break;
		case _UNDERFLOW:
#else
		case SING:
		case OVERFLOW:
		case PLOSS:
		case TLOSS:
			break;
		case UNDERFLOW:
#endif
			if ( !strcasecmp( except->name, "pow" ) )
			{
				if ( except->arg1 < 1.0 && except->arg2 > 0.0 )
				{
					pow_exceptions++;
					return 0; // not handled, but don't care
				}
			}
			break;

		default:
			break;
	}
	return 0;
}

int DebugMathErrors( void )
{
	int j;
	if ( !total_exceptions )
		return 0;
	DebugPrintf( "Math Exception Summary\n" );
	DebugPrintf( "======================\n" );
	for ( j = 0; j < MAX_EXCEPTION_TYPES; j++ )
	{
		if ( type_exceptions[ j ] )
		{
			DebugPrintf( "%d %s\n",
				type_exceptions[ j ], MathErrStrings[ j ] );
		}
	}
	DebugPrintf( "===\n" );
	DebugPrintf( "%d total (%d unknown)\n",
		total_exceptions, unknown_exceptions );
	DebugPrintf( "(%d acos, %d sqrt, %d pow, %d unhandled)\n",
		acos_exceptions, sqrt_exceptions, pow_exceptions, unhandled_exceptions );
	return total_exceptions;
}

void InitMathErrors(void)
{
#if defined(DEBUG_ON) && defined(_SVID_)
  _LIB_VERSION = _SVID_;
#endif
}

#else // NOT WIN32

// matherr was deprecated
// https://man7.org/linux/man-pages/man7/math_error.7.html
// https://man7.org/linux/man-pages/man3/fenv.3.html

#include "math_error.h"
#include "util.h"

void InitMathErrors(void)
{
}

//#define TEST_MATH_ERRORS
#ifdef TEST_MATH_ERRORS
#include <float.h>
void TestMathErrors(void)
{
  // https://en.cppreference.com/w/c/numeric/fenv/fetestexcept
  DebugPrintf("1.0/0.0 = %f\n", 1.0/0.0); // FE_DIVBYZERO
  DebugPrintf("1.0/10.0 = %f\n", 1.0/10.0); // FE_INEXACT
  DebugPrintf("sqrt(-1) = %f\n", sqrt(-1)); // FE_INVALID
  DebugPrintf("DBL_MAX*2.0 = %f\n", DBL_MAX*2.0); // FE_INEXACT FE_OVERFLOW
  DebugPrintf("nextafter(DBL_MIN/pow(2.0,52),0.0) = %.1f\n",
    nextafter(DBL_MIN/pow(2.0,52),0.0)); // FE_INEXACT FE_UNDERFLOW
}
#endif

// https://en.cppreference.com/w/c/numeric/fenv/fetestexcept
int DebugMathErrors( void )
{
#ifdef TEST_MATH_ERRORS
	TestMathErrors();
#endif

	if(!fetestexcept(FE_ALL_EXCEPT))
		return 0;

	DebugPrintf( "======================\n" );
	DebugPrintf( "Math Exception Summary\n" );
	DebugPrintf( "======================\n" );

	if(fetestexcept(FE_DIVBYZERO))
		DebugPrintf("FE_DIVBYZERO\n");

	if(fetestexcept(FE_INEXACT))
		DebugPrintf("FE_INEXACT\n");

	if(fetestexcept(FE_INVALID))
		DebugPrintf("FE_INVALID\n");

	if(fetestexcept(FE_OVERFLOW))
		DebugPrintf("FE_OVERFLOW\n");

	if(fetestexcept(FE_UNDERFLOW))
		DebugPrintf("FE_UNDERFLOW\n");

	DebugPrintf( "======================\n" );

	feclearexcept(FE_ALL_EXCEPT);

	return 1;
}

#endif // NOT WIN32
#endif // DEBUG_ON
