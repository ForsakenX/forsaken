#ifndef MAIN_SDL_INCLUDED
#define MAIN_SDL_INCLUDED

#include <SDL.h>
#include "render.h"

bool sdl_init_video( void );
bool sdl_init( void );
void sdl_render_present( render_info_t * info );

#endif
