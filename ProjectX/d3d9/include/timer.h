#ifndef TIMER
#define TIMER

/*
	setup:

			timer t;
			timer_clear( & t );

	time passed between two points:

			timer_run( &t );
			sleep(10);
			duration = timer_run( &t );

	peeking at the time without modifing last time:

			duration = timer_peek( &t );

*/


// pukes
#include <windows.h>

// holds all our counters
// non global/static declartions are dirty so run timer_clear on them...
typedef struct timer {
  __int64 last;
  float   best;
  float   worst;
  float   seconds;
} timer;

float timer_run   ( timer* );
float timer_peek  ( timer* );
void  timer_clear ( timer* );
void  timer_debug ( char*, timer* );

#endif
