#include "main.h"
#include "util.h"
#include "render.h"
#include "version.h"
#include "SDL.h"
#ifdef OPENGL
#include "SDL_opengl.h"
#endif

extern render_info_t render_info;

BOOL sdl_init( void )
{
	SDL_version ver;
	SDL_VERSION(&ver);
	DebugPrintf("SDL compile-time version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);
	ver = *SDL_Linked_Version();
	DebugPrintf("SDL runtime version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);

	if( SDL_Init( SDL_INIT_VIDEO  ) < 0 || !SDL_GetVideoInfo() )
	{
		Msg("Failed to initialize sdl: %s\n",SDL_GetError());
		return FALSE;
	}

	return TRUE;
}

// TODO - should this be part of init_renderer?
//		  does sdl enumerate resolutions that are valid for dx?
//		  seems that dx does allot of work that sdl could do...

BOOL sdl_init_window( void )
{
	// set the window icon
	{
		SDL_Surface* icon = NULL;
		icon = SDL_LoadBMP("Data/ProjectX-32x32.bmp");
		if(icon)
		{
			// remove black pixels
			Uint32 colorkey = SDL_MapRGB(icon->format, 0, 0, 0);
			SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);        
			SDL_WM_SetIcon(icon, NULL);
			SDL_FreeSurface(icon);
		}
	}

#ifdef OPENGL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	1  );
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,	1  );
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,	render_info.vsync );
	{
		int mode = render_info.default_mode.bpp;
		if ( mode != 16 && mode != 24 && mode != 32 )
		{
			mode = 24;
			DebugPrintf("main_sdl: invalid bpp (%d) defaulting to 24\n",mode);
		}
		{
			int size = ( mode > 16 ) ? 8 : 5 ;
                	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   size);
        	        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, size);
                	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  size);
	                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, mode);
			DebugPrintf("main_sdl: set pixel sizes to %d\n",size);
			DebugPrintf("main_sdl: set pixel depth to %d\n",mode);
		}
	}
#endif

	// create the window
	{
		int flags = SDL_ANYFORMAT;

		if(render_info.bFullscreen)
			flags |= SDL_FULLSCREEN;
		
#ifdef OPENGL
		flags |= SDL_OPENGL;
#endif

		render_info.screen = SDL_SetVideoMode(
			render_info.default_mode.w,
			render_info.default_mode.h,
			render_info.default_mode.bpp,
			flags
		);
	}

	if(render_info.screen)
	{
		int bpp;
		char driver[64];

		// surface created by sdl
		DebugPrintf("main_sdl: surface { w=%d, h=%d, bpp=%d, Bpp=%d }\n",
			render_info.screen->w,
			render_info.screen->h,
			render_info.screen->format->BitsPerPixel,
			render_info.screen->format->BytesPerPixel
		);

		// actual depth size set by sdl
	        SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &bpp);
	        DebugPrintf("main_sdl: depth buffer is %d bpp\n", bpp);

		// video driver
		if(SDL_VideoDriverName(driver, 64)!=NULL)
			DebugPrintf("main_sd: sdl video driver name: %s\n",driver);
		else
			DebugPrintf("main_sdl: failed to obtain the video driver name.\n");

	}
	else
	{
		Msg("main_sdl: failed to create video surface: %s\n",SDL_GetError());
		return FALSE;
	}

	// window title, icon title (taskbar and other places)
	SDL_WM_SetCaption(ProjectXVersion,"ProjectX");

	//
	return TRUE;
}

