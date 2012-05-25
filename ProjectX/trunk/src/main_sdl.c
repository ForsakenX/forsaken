#include "main.h"
#include "util.h"
#include "render.h"
#include "version.h"
#include <SDL.h>
#ifdef OPENGL
#include "SDL_opengl.h"
#endif

extern render_info_t render_info;
extern BOOL render_init( render_info_t * info );

BOOL sdl_init( void )
{
	SDL_version ver;
	SDL_VERSION(&ver);
	DebugPrintf("SDL compile-time version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);
	ver = *SDL_Linked_Version();
	DebugPrintf("SDL runtime version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 || !SDL_GetVideoInfo() )
	{
		Msg("Failed to initialize sdl: %s\n",SDL_GetError());
		return FALSE;
	}

	return TRUE;
}

/////////////////////////
//
// Video Initialization
//
/////////////////////////

/*
	You can't trust ListModes.
	Modes are standard and a list is updated at:
		http://en.wikipedia.org/wiki/Display_resolution.
	Default mode should match the current desktop.
	BPP should be left alone to match whatever the desktop is at.
*/

#define NUMBER_MODES 19
render_display_mode_t video_modes[NUMBER_MODES] = {
{0,0}, // current video mode of the desktop
{640,480},
{800,600}, // default window mode
{1024,768},
{1152,864},
{1280,600},
{1280,720},
{1280,768},
{1280,800},
{1280,854},
{1280,960},
{1280,1024},
{1366,768},
{1440,900},
{1600,900},
{1600,1200},
{1680,1050},
{1920,1080},
{1920,1200} 
};

static void init_video_modes( Uint32 flags )
{
	int i;
	render_info.Mode               = video_modes;
	render_info.NumModes           = NUMBER_MODES;

	// in full screen then default to desktop resolution
	if( flags & SDL_FULLSCREEN ) render_info.CurrMode = 0;

	// in window mode default to 800x600 
	else render_info.CurrMode = 2;

	// try to find the users preferred mode
	for( i = 1; i < render_info.NumModes; i++ ) // skip mode 0
	{
		if( render_info.Mode[i].w == render_info.default_mode.w &&
		    render_info.Mode[i].h == render_info.default_mode.h )
		{
			render_info.CurrMode = i;
			DebugPrintf("init_video_modes: found requested resolution of %dx%d\n",
				render_info.default_mode.w,
				render_info.default_mode.h);
			break;
		}
	}

	render_info.ThisMode           = render_info.Mode[ render_info.CurrMode ];
	render_info.window_size.cx     = render_info.ThisMode.w;
	render_info.window_size.cy     = render_info.ThisMode.h;
	render_info.WindowsDisplay     = render_info.Mode[ render_info.CurrMode ];
	render_info.WindowsDisplay.w   = render_info.ThisMode.w;
	render_info.WindowsDisplay.h   = render_info.ThisMode.h;
}

static void set_window_icon( void )
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

static void set_opengl_settings( void )
{
#ifdef OPENGL
	// BPP should be left alone to match whatever the desktop is at.

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	1  );

	// this causes issues on at least one card I've seen.
	// best to leave this as an option.  if someone says they have
	// low frame rate then suggest them to pass -forceaccel on cli
	if(render_info.force_accel)
	{
		DebugPrintf("main_sdl: enabling accelerated visual\n");
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,	1  );
	}

	DebugPrintf("vsync set to %d\n",render_info.vsync);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,	render_info.vsync );

#endif
}

static void print_info( void )
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

static Uint32 create_video_flags( void )
{
	Uint32 flags = SDL_ANYFORMAT;	

#ifdef OPENGL
	flags |= SDL_OPENGL;
#endif

	if(render_info.fullscreen)
		flags |= SDL_FULLSCREEN;

	return flags;
}

static BOOL create_video_surface( Uint32 flags )
{
#ifndef RENDER_DISABLED
	render_info.screen = SDL_SetVideoMode(
		render_info.ThisMode.w,
		render_info.ThisMode.h,
		0, // BPP should be left alone to match whatever the desktop is at.
		flags
	);

	if(!render_info.screen)
	{
		Msg("main_sdl: failed to create video surface: %s\n",SDL_GetError());
		return FALSE;
	}
	
	print_info();
#endif

	return TRUE;
}

static void set_window_title( void )
{
	// window title, icon title (taskbar and other places)
	SDL_WM_SetCaption(ProjectXVersion,"ProjectX");
}

// TODO - we need a ui control for this
static void set_aspect_ratio( void )
{
	render_info.aspect_ratio = (float) render_info.ThisMode.w / (float) render_info.ThisMode.h;
	DebugPrintf("aspect ratio: %d:%d\n",render_info.ThisMode.w,render_info.ThisMode.h);
}

BOOL sdl_init_video( void )
{
	Uint32 flags = create_video_flags();

	set_window_icon();

	set_opengl_settings();

	init_video_modes( flags );

	set_aspect_ratio();

	if(!create_video_surface( flags ))
		return FALSE;

#ifdef WIN32
	// on windows the 2nd time you try to set video mode
	// you need to call the opengl states again after wards
	// as stated on http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode
	// this fixes a bug where the screen goes black on mode change
	set_opengl_settings();
#endif

	set_window_title();

	if (!render_init( &render_info ))
		return FALSE;

	return TRUE;
}

