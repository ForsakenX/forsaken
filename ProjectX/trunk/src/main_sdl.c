#include "main.h"
#include "util.h"
#include "version.h"
#include "main_sdl.h"
#ifdef GL
#include "SDL_opengl.h"
#endif

extern render_info_t render_info;
extern bool render_init( render_info_t * info );

bool sdl_init( void )
{
	SDL_version ver;

	SDL_VERSION(&ver);
	DebugPrintf("SDL compile-time version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);

#if SDL_VERSION_ATLEAST(2,0,0)
	SDL_GetVersion(&ver);
#else
	ver = *SDL_Linked_Version();
#endif
	DebugPrintf("SDL runtime version: %u.%u.%u\n", ver.major, ver.minor, ver.patch);

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0
#if !SDL_VERSION_ATLEAST(2,0,0)
		|| !SDL_GetVideoInfo()
#endif
	)
	{
		Msg("Failed to initialize sdl: %s\n",SDL_GetError());
		return false;
	}

	return true;
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

static void init_video_modes( u_int32_t window_flags )
{
	int i;
	render_info.Mode               = video_modes;
	render_info.NumModes           = NUMBER_MODES;

	// in full screen then default to desktop resolution
	if( window_flags &
#if SDL_VERSION_ATLEAST(2,0,0)
		SDL_WINDOW_FULLSCREEN
#else
		SDL_FULLSCREEN
#endif
	) render_info.CurrMode = 0;

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
// TODO
#if !SDL_VERSION_ATLEAST(2,0,0)
	SDL_Surface* icon = NULL;
	icon = SDL_LoadBMP("Data/ProjectX-32x32.bmp");
	if(icon)
	{
		// remove black pixels
		u_int32_t colorkey = SDL_MapRGB(icon->format, 0, 0, 0);
		SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);        
		SDL_WM_SetIcon(icon, NULL);
		SDL_FreeSurface(icon);
	}
#endif
}

static void set_opengl_settings( void )
{
#ifdef GL
	// BPP should be left alone to match whatever the desktop is at.

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	  1);

#if SDL_VERSION_ATLEAST(2,0,0)
#if GL == 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	// TODO - this isn't only mac osx specific is it ?
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
#endif

	// this causes issues on at least one card I've seen.
	// best to leave this as an option.  if someone says they have
	// low frame rate then suggest them to pass -forceaccel on cli
	if(render_info.force_accel)
	{
		DebugPrintf("main_sdl: enabling accelerated visual\n");
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,	1  );
	}

// this is now done during creation of the renderer
#if !SDL_VERSION_ATLEAST(2,0,0)
	DebugPrintf("vsync set to %d\n",render_info.vsync);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,	render_info.vsync );
#endif

#endif
}

static void print_info( void )
{
	int bpp;
	char driver[64];

// TODO
#if !SDL_VERSION_ATLEAST(2,0,0)
	// surface created by sdl
	DebugPrintf("main_sdl: surface { w=%d, h=%d, bpp=%d, Bpp=%d }\n",
		render_info.screen->w,
		render_info.screen->h,
		render_info.screen->format->BitsPerPixel,
		render_info.screen->format->BytesPerPixel
	);
#endif

	// actual depth size set by sdl
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &bpp);
	DebugPrintf("main_sdl: depth buffer is %d bpp\n", bpp);

// TODO
#if !SDL_VERSION_ATLEAST(2,0,0)
	// video driver
	if(SDL_VideoDriverName(driver, 64)!=NULL)
		DebugPrintf("main_sd: sdl video driver name: %s\n",driver);
	else
		DebugPrintf("main_sdl: failed to obtain the video driver name.\n");
#endif
}

#if !SDL_VERSION_ATLEAST(2,0,0)
static u_int32_t create_video_flags( void )
{
	u_int32_t flags = SDL_ANYFORMAT;

#ifdef GL
	flags |= SDL_OPENGL;
#endif

	if(render_info.fullscreen)
		flags |= SDL_FULLSCREEN;

	return flags;
}
#endif

#if SDL_VERSION_ATLEAST(2,0,0)
static u_int32_t create_renderer_flags( void )
{
	// doubt we want to support   SDL_RENDERER_SOFTWARE
	// we might need to add       SDL_RENDERER_TARGETTEXTURE
	u_int32_t renderer_flags = SDL_RENDERER_ACCELERATED;

	DebugPrintf("vsync set to %d\n",render_info.vsync);
	if(render_info.vsync)
		renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

	return renderer_flags;
}
static u_int32_t create_window_flags( void )
{
	u_int32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE; // TODO - we really want resizable ?

	if(render_info.fullscreen)
		window_flags |= SDL_WINDOW_FULLSCREEN;

#ifdef GL
	window_flags |= SDL_WINDOW_OPENGL;
#endif

	return window_flags;
}
#endif



static bool create_video_surface( u_int32_t window_flags, u_int32_t renderer_flags )
{
#ifndef RENDER_DISABLED
  #if SDL_VERSION_ATLEAST(2,0,0)
	render_info.window = SDL_CreateWindow(
		"ProjectX",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		render_info.ThisMode.w,
		render_info.ThisMode.h,
		window_flags
		);
	if(!render_info.window)
	{
		Msg("main_sdl: failed to create window: %s\n",SDL_GetError());
		return false;
	}
	render_info.renderer = SDL_CreateRenderer(
		render_info.window,
		-1,
		renderer_flags
		);
	if(!render_info.renderer)
	{
		Msg("main_sdl: failed to create renderer: %s\n",SDL_GetError());
		return false;
	}

  #else
	render_info.screen = SDL_SetVideoMode(
		render_info.ThisMode.w,
		render_info.ThisMode.h,
		0, // BPP should be left alone to match whatever the desktop is at.
		window_flags // on older sdl only 1 set of flags was used
	);

	if(!render_info.screen)
	{
		Msg("main_sdl: failed to create video surface: %s\n",SDL_GetError());
		return false;
	}
  #endif
	
	print_info();
#endif

	return true;
}

static void set_window_title( void )
{
// this is now done in CreateWindow
#if !SDL_VERSION_ATLEAST(2,0,0)
	// window title, icon title (taskbar and other places)
	SDL_WM_SetCaption(PXVersion,"ProjectX");
#endif
}

// TODO - we need a ui control for this
static void set_aspect_ratio( void )
{
	render_info.aspect_ratio = (float) render_info.ThisMode.w / (float) render_info.ThisMode.h;
	DebugPrintf("aspect ratio: %d:%d\n",render_info.ThisMode.w,render_info.ThisMode.h);
}

bool sdl_init_video( void )
{
#if SDL_VERSION_ATLEAST(2,0,0)
	u_int32_t renderer_flags = create_renderer_flags();
	u_int32_t window_flags   = create_window_flags();
#else
	// old sdl had one set of flags
	u_int32_t window_flags   = create_video_flags();
	u_int32_t renderer_flags = window_flags;
#endif

	set_window_icon();

	set_opengl_settings();

	init_video_modes( window_flags );

	set_aspect_ratio();

	if(!create_video_surface( window_flags, renderer_flags ))
		return false;

#ifdef WIN32
	// on windows the 2nd time you try to set video mode
	// you need to call the opengl states again after wards
	// as stated on http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode
	// this fixes a bug where the screen goes black on mode change
	set_opengl_settings();
#endif

	set_window_title();

	if (!render_init( &render_info ))
	{
		Msg("render_init() returned false");
		return false;
	}

	return true;
}

void sdl_render_present( render_info_t * info )
{
#if SDL_VERSION_ATLEAST(2,0,0)
	SDL_RenderPresent(info->renderer);
#else
	SDL_GL_SwapBuffers();
#endif
}
