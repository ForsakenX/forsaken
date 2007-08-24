#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "demo_id.h"


const char	FingerPrint[128] = "bubbles " FINGERPRINT;

#ifdef DISABLE_IP_CHECKING
const char ipcheck[] = "noip";
#else
const char ipcheck[] = "ipon";
#endif

const char last_compiled[] = __DATE__ " at " __TIME__;
const char last_modified[] = __FILE__ " last modified " __TIMESTAMP__;


time_t compile_time( void )
{
	time_t t;
	char *compile_date = __DATE__;
	char *compile_time = __TIME__;
	char *test_date;
	struct tm *test, compile_tm;
	int day, month, year, hour, minute, second;

	memset( &compile_tm, 0, sizeof( compile_tm ) );
	/*
		__DATE__	The compilation date of the current source file.
					The date is a string literal of the form Mmm dd yyyy.
					The month name Mmm is the same as for dates generated
					by the library function asctime declared in TIME.H. 
	*/
	time( &t );
	test = localtime( &t );
	if ( !test )
		return 0;
	compile_tm.tm_isdst = test->tm_isdst;
	for ( month = 0; month < 12; month++ )
	{
		test->tm_mon = month;
		test_date = asctime( test );
		/*
			The string result produced by asctime contains exactly 26 characters
			and has the form Wed Jan 02 02:03:55 1980\n\0.
			A 24-hour clock is used. All fields have a constant width.
			The newline character and the null character occupy the last
			two positions of the string.
			asctime uses a single, statically allocated buffer to hold the return string.
			Each call to this function destroys the result of the previous call.
		*/
		if ( !_strnicmp( test_date + 4, compile_date, 3 ) )
			break; // found month
	}
	if ( month < 12 )
		compile_tm.tm_mon = month;
	if ( sscanf( compile_date + 4, "%d %d", &day, &year ) == 2 )
	{
		compile_tm.tm_mday = day;
		compile_tm.tm_year = year - 1900;
	}
	if ( sscanf( compile_time, "%d:%d:%d", &hour, &minute, &second ) == 3 )
	{
		compile_tm.tm_hour = hour;
		compile_tm.tm_min = minute;
		compile_tm.tm_sec = second;
	}
	t = mktime( &compile_tm );

	return t;
}
