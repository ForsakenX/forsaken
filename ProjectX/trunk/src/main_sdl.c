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
	SDL_Surface* icon = NULL;

	// set the window icon
	icon = SDL_LoadBMP("Data/ProjectX-32x32.bmp");
	if(icon)
	{
		// remove black pixels
		Uint32 colorkey = SDL_MapRGB(icon->format, 0, 0, 0);
		SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);        
		SDL_WM_SetIcon(icon, NULL);
		SDL_FreeSurface(icon);
	}

#ifdef OPENGL
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,		8  );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,			32 );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1  );
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,	1  );
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,		render_info.vsync );
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

	if(!render_info.screen)
	{
		Msg("Failed to create video surface: %s\n",SDL_GetError());
		return FALSE;
	}

	// print video driver name
	{
		char video_driver_name[64];
		if(SDL_VideoDriverName(video_driver_name, 64)!=NULL)
			DebugPrintf("sdl video driver name: %s\n",video_driver_name);
		else
			DebugPrintf("Failed to obtain the video driver name.\n");
	}

	// window title, icon title (taskbar and other places)
	SDL_WM_SetCaption(ProjectXVersion,"ProjectX");

	//
	return TRUE;
}