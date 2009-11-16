
#include "timer.h"
#include "util.h"

// run timer and compute seconds without modifying stats
float timer_peek( timer_t* stats )
{
	float seconds;
	timer_t old = *stats;
	seconds = timer_run( stats );
	*stats = old; // restore it
	return seconds;
}

// send debug info to console
void timer_debug( char* name, timer_t* stats )
{
  DebugPrintf("%s: seconds = %5f, best = %5f, worst = %5f\n",name,stats->seconds,stats->best,stats->worst);
}

// clear the timer
void timer_clear( timer_t* stats )
{
	stats->last    = 0;
	stats->worst   = 0;
	stats->best    = 0;
	stats->seconds = 0.0F;
}

// run timer and compute calculations
float timer_run( timer_t* stats )
{

  // current counter value
  Uint32 time_now;

  // ms between last and now
  Uint32 time_diff;

  // if there is no count_before then we know this is the first run
  if(!stats->last)
  {

    // set the the count_before
    stats->last = SDL_GetTicks();

    // your program should know it's a first run
    return 0.0F;

  }
  
  // get the count_after
  time_now = SDL_GetTicks();

  // calculate ms since last run
  time_diff = time_now - stats->last;

  // calculate the duration in seconds
  stats->seconds = ((float)time_diff) / 1000.0F; // 1000 ms == 1 s

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
