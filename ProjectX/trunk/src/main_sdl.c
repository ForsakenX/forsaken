#include "main.h"
#include "util.h"
#include "render.h"
#include "version.h"
#include "SDL.h"
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

static BOOL enumerate_video_modes( Uint32 flags )
{
	SDL_Rect** modes;

	render_info.NumModes = 0;
	if(render_info.Mode)
	{
		free(render_info.Mode);
		render_info.Mode = NULL;
	}

	modes = SDL_ListModes(NULL, flags);

	if (modes == (SDL_Rect**)0) 
	{
		Msg("No video modes available for given format!\n");
		return FALSE;
	}

	if (modes != (SDL_Rect**)-1) 
	{
		int i;
		int count = 0;
		DebugPrintf("resolutions restricted to following set:\n");
		for (i=0; modes[i]; i++)
		{
			DebugPrintf("valid resolution: %d x %d\n", modes[i]->w, modes[i]->h);
			if( modes[i]->w < 640 || modes[i]->h < 480 )
				continue;
			count++;
		}
		//
		render_info.NumModes	= count;
		render_info.Mode		= (render_display_mode_t *) malloc(sizeof(render_display_mode_t)*render_info.NumModes);
		render_info.CurrMode	= 0;
		//
		count = 0;
		for (i=0; modes[i]; i++)
		{
			if( modes[i]->w < 640 || modes[i]->h < 480 )
				continue;
			render_info.Mode[count].w		= modes[i]->w;
			render_info.Mode[count].h		= modes[i]->h;
			render_info.Mode[count].bpp		= render_info.default_mode.bpp;
			count++;
		}
		//
		{
			int mode = 0;
			int desired = 0;
			int lowest = 0;
			for( i = 0; i < render_info.NumModes; i++ )
			{
				if( render_info.Mode[i].w == render_info.default_mode.w &&
					render_info.Mode[i].h == render_info.default_mode.h	)
							desired = i;
				if( render_info.Mode[i].w < render_info.Mode[lowest].w &&
					render_info.Mode[i].h < render_info.Mode[lowest].h	)
							lowest = i;
			}
			render_info.CurrMode = lowest;
			if(desired)
				render_info.CurrMode = desired;
		}
	}

	// build a hard coded list of supported formats
	// user can always pass in their own mode on the command line

	else
	{
		render_info.NumModes			= 6;
		render_info.Mode				= (render_display_mode_t *) malloc(sizeof(render_display_mode_t)*render_info.NumModes);
		render_info.CurrMode			= 0;
		// first mode is user selected
		render_info.Mode[0].w			= render_info.default_mode.w;
		render_info.Mode[0].h			= render_info.default_mode.h;
		render_info.Mode[0].bpp			= render_info.default_mode.bpp;
		// 640x480
		render_info.Mode[1].w			= 640;
		render_info.Mode[1].h			= 480;
		render_info.Mode[1].bpp			= render_info.default_mode.bpp;
		// 800x600
		render_info.Mode[2].w			= 800;
		render_info.Mode[2].h			= 600;
		render_info.Mode[2].bpp			= render_info.default_mode.bpp;
		// 1024x768
		render_info.Mode[3].w			= 1024;
		render_info.Mode[3].h			= 768;
		render_info.Mode[3].bpp			= render_info.default_mode.bpp;
		// 1280x800
		render_info.Mode[4].w			= 1280;
		render_info.Mode[4].h			= 800;
		render_info.Mode[4].bpp			= render_info.default_mode.bpp;
		// 1280x1024
		render_info.Mode[5].w			= 1280;
		render_info.Mode[5].h			= 1024;
		render_info.Mode[5].bpp			= render_info.default_mode.bpp;
	}

	render_info.ThisMode			= render_info.Mode[ render_info.CurrMode ];
	render_info.WindowsDisplay		= render_info.Mode[ render_info.CurrMode ];
	render_info.window_size.cx		= render_info.ThisMode.w;
	render_info.window_size.cy		= render_info.ThisMode.h;
	render_info.WindowsDisplay.w    = render_info.ThisMode.w;
	render_info.WindowsDisplay.h    = render_info.ThisMode.h;

	return TRUE;
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
	int size;
	int mode = render_info.default_mode.bpp;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	1  );
	// this causes issues on at least one card I've seen.
	// best to leave this as an option.  if someone says they have
	// low frame rate then suggest them to pass -forceaccel on cli
	if(render_info.force_accel)
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,	1  );
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,	render_info.vsync );
	if ( mode != 16 && mode != 24 && mode != 32 )
	{
		mode = 24;
		DebugPrintf("main_sdl: invalid bpp (%d) defaulting to 24\n",mode);
	}
	size = ( mode > 16 ) ? 8 : 5 ;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   size);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, size);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  size);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, mode);
	DebugPrintf("main_sdl: set pixel sizes to %d\n",size);
	DebugPrintf("main_sdl: set pixel depth to %d\n",mode);
#endif
}

static print_info( void )
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
	render_info.screen = SDL_SetVideoMode(
		render_info.ThisMode.w,
		render_info.ThisMode.h,
		render_info.ThisMode.bpp,
		flags
	);

	if(!render_info.screen)
	{
		Msg("main_sdl: failed to create video surface: %s\n",SDL_GetError());
		return FALSE;
	}
	
	print_info();

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

	if(!enumerate_video_modes( flags ))
		return FALSE;

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

