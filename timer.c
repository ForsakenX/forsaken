
#include "main.h"
#include "timer.h"
#include "util.h"

// run timer and compute seconds without modifying stats
float timer_peek( px_timer_t* stats )
{
	float seconds;
	px_timer_t old = *stats;
	seconds = timer_run( stats );
	*stats = old; // restore it
	return seconds;
}

// send debug info to console
void timer_debug( char* name, px_timer_t* stats )
{
  DebugPrintf("%s: seconds = %5f, millis = %d, best = %5f, worst = %5f\n",
	name,stats->seconds,stats->millis,stats->best,stats->worst);
}

// clear the timer
void timer_clear( px_timer_t* stats )
{
	stats->last    = 0;
	stats->worst   = 0;
	stats->best    = 0;
	stats->seconds = 0.0F;
}

// run timer and compute calculations
float timer_run( px_timer_t* stats )
{

  // current counter value
  u_int32_t time_now;

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
  stats->millis = time_now - stats->last;

  // calculate the duration in seconds
  stats->seconds = ((float)stats->millis) / 1000.0F; // 1000 ms == 1 s

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
