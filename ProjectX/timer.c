
#include "timer.h"
#include "util.h"

// frequency time does not change
__int64 timer_freq = 0;

// run timer and compute calculations
float timer_run( timer* stats )
{

  // current counter value
  __int64 time_now = 0;

  // difference between last and now
  __int64 time_diff = 0;

  // get the frequency if not already attained
  if(!timer_freq)
    QueryPerformanceFrequency((LARGE_INTEGER *) &timer_freq);

  // if there is no count_before then we know this is the first run
  if(!stats->last)
  {

    // set the the count_before
    QueryPerformanceCounter((LARGE_INTEGER *) &stats->last);

    // your program should know it's a first run
    return 0.0F;

  }
  
  // get the count_after
  QueryPerformanceCounter((LARGE_INTEGER *) &time_now);

  // calculate the count_diff
  time_diff = time_now - stats->last;

  // calculate the duration in seconds
  stats->seconds = (float) time_diff / (float) timer_freq;

  // reset the last value
  stats->last = time_now;

  // calculate the best
  if (stats->best == 0.0F || stats->seconds < stats->best)
    stats->best = stats->seconds;

  // callculate the worst
  if (stats->worst  == 0.0F || stats->seconds > stats->worst)
    stats->worst = stats->seconds;

  // nice feature
  return stats->seconds;

}

// send debug info to console
void timer_debug( char* name, timer* stats )
{
  DebugPrintf("%s: seconds = %5f, best = %5f, worst = %5f\n",name,stats->seconds,stats->best,stats->worst);
}

// clear the timer
void timer_clear( timer* stats )
{
	stats->last    = 0;
	stats->worst   = 0;
	stats->best    = 0;
	stats->seconds = 0.0F;
}
