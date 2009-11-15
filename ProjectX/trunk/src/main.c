
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <winuser.h>
#include <direct.h>
#include "stdwin.h"

#include "version.h"
#include "typedefs.h"
#include "render.h"
#include "xmem.h"
#include "sbufferhand.h"
#include "util.h"
#include "file.h"
#include "net.h"
#include "title.h"
#include "lua_common.h"
#include "sfx.h"

#include "SDL.h"
#include "input.h"

#ifdef __WINE__
#define LR_VGACOLOR LR_VGA_COLOR
#endif

//
// GLOBAL VARIABLES
//

BOOL Debug = FALSE;
BOOL ShowFrameRate = TRUE;
BOOL ShowInfo = FALSE;

int cliSleep = 0;

render_info_t render_info;
HINSTANCE hInstApp;

//
// Parses the directory to change to from the command line options
//

static BOOL parse_chdir( char *cli )
{
	LPSTR tmp = "";
    LPSTR option = "";
	char cmdline[256];
	int size;

	size = sizeof(strlen(cli)+1);
	if ( size > sizeof(cmdline) )
	{
		Msg("Command line to long!");
		return FALSE;
	}
	strcpy(cmdline,cli);

	option = strtok(cmdline, " -+'\"");

    while(option != NULL )
	{
		// WARNING:	chdir can only be the LAST option !
		// For running the exe outside the root folder
		// -chdir c:\\Program Files\\ProjectX
		if (!_stricmp(option,"chdir"))
		{
			// get option enclosed in quotes
			// this does not consider space as separator
			option = strtok(NULL, "\"'");

			if (!option)
			{
				Msg("Error using chdir");
				return FALSE;
				break;
			}

			// change to root directory
			// the rest of the command line will be used as the path
			if( _chdir( option ) != 0 )
			{
				// error
				Msg("Could not change to directory: %s", option);
				return FALSE;
			}

			// dont loop anymore were done
			break;
		}
		// get the next token
        option = strtok(NULL, " -+'\"");
	}

	return TRUE;
}

//
// checks for all critical folders
//

#define CRITICAL_FOLDERS 6
static BOOL missing_folders( void )
{
	int x = 0;
	char* folders[CRITICAL_FOLDERS] = {"Configs","Data","Dumps","Pilots","Scripts","Utils"};
	for( x = 0; x < CRITICAL_FOLDERS; x++ )
		if( ! is_folder(folders[x]) )
		{
			Msg("Could not locate the '%s' folder...\n%s\n%s", folders[x],
				"exe is most likely in the wrong directory.",
				"or you just need to create the folder.");
			return TRUE;
		}
	return FALSE;
}


//
// Parse the Command Line
//

extern BOOL NoSFX;
extern BOOL DS;
extern float normal_fov;
extern float UV_Fix;
extern float screen_aspect_ratio;
extern int NetUpdateIntervalCmdLine;
extern DWORD UserTotalCompoundSfxBufferSize;
extern BOOL CustomCompoundBufferSize;
extern char *config_name;
extern int cliSleep;
extern TEXT local_port_str;
extern BOOL AllWires;
extern BOOL SpaceOrbSetup;
extern BOOL NoCompoundSfxBuffer;
extern TEXT TCPAddress;
extern TEXT local_port_str;
extern TEXT host_port_str;
extern BOOL DebugLog;
extern BOOL Debug;
extern uint8 QuickStart;
extern BOOL IpOnCLI;

static BOOL ParseCommandLine(LPSTR lpCmdLine)
{
	
	//
	//  Locals
	//

	LPSTR tmp = "";
    LPSTR option = "";
	char cmdline[256];
	int size;

    //
    //  Set Global Defaults
    //
	
	NoSFX					= FALSE; // turns off sound
	DS						= FALSE;

	NetUpdateIntervalCmdLine	= 0;

	//
	// Get the command line string
	//

	size = sizeof(strlen(lpCmdLine)+1);
	if ( size > sizeof(cmdline) )
	{
		Msg("Command line to long!");
		return FALSE;
	}
	strcpy(cmdline,lpCmdLine);

	//
	// extract and process tokens from command line
	//

	option = strtok(cmdline, " -+'\"");

	// loop over every option
    while(option != NULL )
	{

		// last option
		if (!_stricmp(option,"chdir"))
		{
			// dont loop anymore were done
			break;
		}

		// off only works in full screen...
		// turn on vertical syncing
		else if (!_stricmp(option,"vSync")){
			render_info.vsync = TRUE;
		}

		// debugging information send to Log...
        else if (!_stricmp(option, "log"))
		{
            DebugLog = TRUE;
		}

		// debugging information
        else if (!_stricmp(option, "Debug"))
		{
            Debug = TRUE;
		}
		
		// start in window mode
		else if (!_stricmp(option,"Fullscreen"))
		{
			render_info.bFullscreen = TRUE;
		}

		// start in window mode
		else if (!_stricmp(option,"Window"))
		{
			render_info.bFullscreen = FALSE;
		}

		// display status messages and other information
		else if (!_stricmp(option, "DS"))
		{
			DS = TRUE;
        }

		// turn off sound
		else if (!_stricmp(option, "NoSFX"))
		{
			NoSFX = TRUE;
        }
		
		// jump to the host screen
		else if ( !_stricmp( option, "QuickHost" ) ) 
		{
			QuickStart = QUICKSTART_Start;
		}

		// jump to the join game screen
		else if ( !_stricmp( option, "QuickJoin" ) ) 
		{
			QuickStart = QUICKSTART_Join;
		}

		// set the ip address for game to join
		else if ( !_stricmp( option, "TCP" ) )
		{
			char * port;
			char address[255];

			IpOnCLI = TRUE;

			// extract the address
	        option = strtok(NULL, " ");
			strcpy( address, option );

			// try to find a port in the address
			port = strchr(address,':');

			// if port found assign it
			if( port )
			{
				*port = 0; // separate hostname from port
				strcpy( (char*) host_port_str.text, ++port );
			}
			// other wise set default port
			else
			{
				sprintf( (char*) host_port_str.text, "%d", NETWORK_DEFAULT_PORT );
			}

			// copy in the hostname
			strcpy( (LPSTR)TCPAddress.text, address );
		}

		// supposedly to set wire mode for mxv's...
		else if (!_stricmp(option, "AllWires")) 
		{
            AllWires = TRUE;
        }

		// special override to allow setting up of spaceorb
		else if ( !_stricmp( option, "SetupSpaceOrb" ) )
		{
			SpaceOrbSetup = TRUE;
		}

		// no compound sound buffer
		else if ( !_stricmp( option, "NoCompoundSfxBuffer" ) )
		{
			NoCompoundSfxBuffer = TRUE;
		}

		// use sscanf
		else 
		{

			int num, denom;
			DWORD mem;

			// override local port
			if ( sscanf( option, "port:%s", (char*)&local_port_str.text[0] ) == 1 )
			{
				DebugPrintf("Command Line: local port set to %s\n", local_port_str.text);
			}

			// sleep time for every loop
			else if ( sscanf( option, "sleep:%d", &cliSleep )){}

			// select the pilot
			else if ( sscanf( option , "pilot:%s", config_name )){}

			// ammount of memory to allocate for sound buffer
			else if ( sscanf( option, "CompoundSfxBufferMem:%d", &mem ) == 1 )
			{
				UserTotalCompoundSfxBufferSize = mem;
				CustomCompoundBufferSize = TRUE;
			}

			// set the packets per second
			else if ( sscanf( option, "PPS:%d", &NetUpdateIntervalCmdLine ) == 1 ){}

			// resolution mode
			// must be a valid resolution list in the resolution list in game
			// other wise you will end up with the default highest possible resolution
			// note: if you pick a value not in the list then your window will be 1 size and your resolution another
			else if ( sscanf( option, "mode:%d:%d", &render_info.default_mode.w, &render_info.default_mode.h ) == 1 ){}

			// horizontal refresh rate
			// most monitors normally support 60hz so pass 60 if you get weird stuff happening
			else if ( sscanf( option, "rate:%d", &render_info.default_mode.rate ) == 1 ){}

			// bits per pixel
			// default is 32 bpp
			else if ( sscanf( option, "bpp:%d", &render_info.default_mode.bpp ) == 1 ){}

			// the aspect ratio tells forsaken the ratio between width/height of your monitor
			// for example on an 12"x/7.5"y widescreen the ratio is 8/5
			// a shortcut is to simply alter the base 10
			//		12/7.5*10=120/750
			//		12.25/7.75*100=1225/775
			else if ( sscanf( option, "ratio:%d:%d", &num, &denom ) == 2 )
			{
				if ( num && denom )
					screen_aspect_ratio = (float) num / denom;
			}

			// modifies texture dimentions.. don't now what uv stands for..
			else if ( sscanf( option, "UVFix:%f", &UV_Fix ) == 1 ){}

			// set the horizontal frame of view
			// this is the screen stretching when you go into nitro
			// default is 90... max is 120...
			else if ( sscanf( option, "fov:%f", &normal_fov ) == 1 ){}
        }

		// get the next token
        option = strtok(NULL, " -+'\"");

    }

	return TRUE;
}

//
// Cleans up the application before quiting
//

extern void ReleaseView(void);
extern void DestroySound( int flags );
extern void render_cleanup( render_info_t * info );
extern BOOL TermDInput(void);
extern void ReleaseScene(void);

BOOL QuitRequested = FALSE;
void CleanUpAndPostQuit(void)
{
	// check if this function was ran already
    if (QuitRequested)
		return;

	//
	SDL_Quit();

	// kill stuff
    ReleaseView();

	// stop rendering and destroy objects
	render_cleanup( &render_info );

	// destroy the sound
	DestroySound( DESTROYSOUND_All );

	// destroy direct input
	TermDInput();
  
	// release the scene
	ReleaseScene();

	// set flag
    QuitRequested = TRUE;

	// we dont control the cursor anymore
	input_grab( FALSE );

	//
    PostQuitMessage( 0 );
}

//
// Creates the window
//

extern HINSTANCE hInstApp;

static BOOL InitWindow( void )
{
	SDL_Surface* myVideoSurface = NULL;
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

	// create the window
	// TODO -
	//		look into: SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_HWSURFACE
	//		opengl: for win32 need to redo all the gl init stuff after this
	myVideoSurface = SDL_SetVideoMode(
		render_info.default_mode.w,
		render_info.default_mode.h,
		SDL_GetVideoInfo()->vfmt->BitsPerPixel, 
		0
		);

	if(!myVideoSurface)
	{
		Msg("Failed to create video surface: %s\n",SDL_GetError());
		return FALSE;
	}

	// window title, icon title (taskbar and other places)
	SDL_WM_SetCaption(ProjectXVersion,"ProjectX");

	// hack to get the window handle
	render_info.window = GetActiveWindow();

	//
	return TRUE;
}

//
// Initializes the application
//

#ifdef BREAKPAD
// breakpad running through wine, built for windows doens't work well..
#ifndef __WINE__
extern BOOL breakpad_init( void );
#endif
#endif

extern BOOL InitView( void );
extern LONGLONG LargeTime;
extern LONGLONG LastTime;
extern void GetGamePrefs( void );
extern void SetSoundLevels( int *dummy );
extern BOOL init_renderer( render_info_t * info );
extern void GetDefaultPilot(void);
extern BOOL InitScene(void);
extern BOOL InitDInput(void);
extern BOOL ShowFrameRate;
extern BOOL ShowInfo;
extern BYTE MyGameStatus;

static BOOL AppInit( char * lpCmdLine )
{
    ZEROMEM(render_info);

	// Appears to be a complete fuckup...
	// Only used in two other places...
	// never updated...
	// wtf is the point...
	QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
	LastTime = LargeTime;

#ifdef BREAKPAD
// breakpad running through wine, built for windows doens't work well..
#ifndef __WINE__

	// initialize google breakpad crash reporting
	if(!breakpad_init())
		return FALSE;

	// test breakpad by uncommenting this
	//{ *(int*)0=0; }

#endif
#endif

#ifdef DEBUG_ON

	// special debuggin routines
	XMem_Init();
	XSBuffer_Init();

#endif

	// init sdl
	if( SDL_Init( SDL_INIT_VIDEO  ) < 0 || !SDL_GetVideoInfo() )
	{
		Msg("Failed to initialize sdl: %s\n",SDL_GetError());
		return 0;
	}

	// initialize COM library
	if FAILED( CoInitialize(NULL) )
		return FALSE;

	// setup globals used by application
    ShowFrameRate	= TRUE;
    ShowInfo		= FALSE;

	// parse chdir from command line first
	if(!parse_chdir(lpCmdLine))
		return FALSE;

	// we are now in the skeleton folder
	// now we need to see if we are in right place

	// check for missing folders
	if(missing_folders())
		return FALSE;

	// startup lua
	if( lua_init() != 0 )
		return FALSE;

	// copy game settings from config
	GetGamePrefs();

	// our configs are now loaded
	// now we can check the command line for overrides

	// parse the command line
	if(!ParseCommandLine(lpCmdLine))
		return FALSE;

	// this is just a safety cause if these values are 0
	// then starting in fullscreen and switching to window crashes
	// cause the last window state had a window size of 0
	if ( ! screen_aspect_ratio )
		screen_aspect_ratio = (float) (render_info.default_mode.w / render_info.default_mode.h);

	//
	// create and show the window
	//
	if(!InitWindow())
		return FALSE;

	// appears dinput has to be after init window

	// initialize direct input
	// This requires an application and window handle
	// so it most not come earlier than here
	if (!InitDInput())
	{
		Msg("Failed to initialized Direct Input!");
		return FALSE;
	}
	
	// this needs to come after InitDInput
	// because InitDInput will wipe the joystick settings
	GetDefaultPilot();

// this is where it starts to take so long cause it scans directory for dynamic sound files...

	// start the title scene
	MyGameStatus = STATUS_Title;

	if (!InitScene())
		return FALSE;

	// This  must come after everything above
	if (!init_renderer( &render_info ))
		return FALSE;

	// load the view
	if (!InitView() )
	{
	    Msg("InitView failed.\n");
		//CleanUpAndPostQuit();
        return FALSE;
	}

	// exclusively grab input in fullscreen mode
	input_grab( render_info.bFullscreen );

	//
	SetSoundLevels( NULL );

	// done
	DebugPrintf("AppInit finished...\n");
    return TRUE;

}

//
// Render the next frame and update the window
//

extern BOOL RenderScene( void );
extern void commit_any_sounds( void );

static BOOL RenderLoop()
{

    // If all the DD and D3D objects have been initialized we can render
    if ( !render_info.bRenderingIsOK || render_info.bMinimized || render_info.bPaused || QuitRequested )
		return TRUE;

    // Call the sample's RenderScene to render this frame
    if (!RenderScene())
	{
        Msg("RenderScene failed.\n");
		DebugPrintf("RenderScene: failed.");
        return FALSE;
    }

	if ( quitting )
	{
		quitting = FALSE;
		CleanUpAndPostQuit();
	}

    // Blt or flip the back buffer to the front buffer
	if( !QuitRequested )
	{
#ifdef DEMO_SUPPORT
		if ((!PlayDemo || ( MyGameStatus != STATUS_PlayingDemo ) ||	DemoShipInit[ Current_Camera_View ]	))
#endif
		{
			// this is the actual call to render a frame...
			if (!render_flip(&render_info))
			{
				Msg("RenderLoop: render_flip() failed");
				return FALSE;
			}
		}

		commit_any_sounds();
	}

	//
    return TRUE;
}

//
// The main routine
//

extern int DebugMathErrors( void );
extern void network_cleanup( void );
extern BOOL SeriousError;
extern void CleanUpAndPostQuit(void);

//int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char * cli, int nCmdShow )
int main( int argc, char* argv[] )
{
	int i;
	char cli[500];
    int failcount = 0; // number of times RenderLoop has failed
	
	// application handle
	hInstApp = GetModuleHandle(NULL);

	// build cli string
	strncpy(cli, " ", 500);
	for ( i=1; i<argc; i++ )
	{
		if( strlen(cli) + strlen(argv[i]) -1 > 500 )
		{
			DebugPrintf("Stopped parsing cli options at argc='%s' because greator than 500 characters\n",argv[i]);
			break;
		}
		strcat( cli, " " );
		strcat( cli, argv[i] );
	}

    // Create the window and initialize all objects needed to begin rendering
    if(!AppInit(cli))
		goto FAILURE;

	while( !QuitRequested )
	{
        // Attempt to render a frame, if it fails, take a note.  If
        // rendering fails more than twice, abort execution.
        if( !RenderLoop() )
		{
            ++failcount;

			if( SeriousError )
			{
				CleanUpAndPostQuit();
				break;
			}

			if (failcount == 3) {
				DebugPrintf("Rendering has failed too many times.  Aborting execution.\n");
				CleanUpAndPostQuit();
				break;
			}

        }

		// process system events
		// leave this at the end of the loop cause it wipes out all events
		// we want the rest of the game to have a chance to peep events
		if(!handle_events())
			goto FAILURE;
		
		// call the sound proccesses
		ProcessSoundRoutines( NULL );

		// command line asks us to sleep and free up sys resources a bit...
		if ( cliSleep )
			Sleep( cliSleep );
	}

FAILURE:

	//
	CleanUpAndPostQuit();

	// cleanup networking
	network_cleanup();

	// Uninitialize the COM library
	CoUninitialize();

	// close up lua
	lua_shutdown();

#ifdef DEBUG_ON

	DebugMathErrors();

	if ( UnMallocedBlocks() )
		DebugPrintf( "Un-malloced blocks found!" );

	if ( UnMallocedSBufferBlocks() )
		DebugPrintf( "Un-malloced SBuffer blocks found!" );

#endif

	return 1;
}