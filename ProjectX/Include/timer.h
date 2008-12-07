#ifndef TIMER
#define TIMER

// pukes
#include <Windows.h>

// pukes again
extern void DebugPrintf( const char * format, ... );

// holds all our counters
// non global/static declartions are dirty so run timer_clear on them...
typedef struct timer {
  __int64 last;
  float   best;
  float   worst;
  float   seconds;
} timer;

float timer_run   ( timer* );
void  timer_clear ( timer* );
void  timer_debug ( char*, timer* );

#endif