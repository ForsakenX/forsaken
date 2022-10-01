#include "main.h"
#include "main_sdl.h"
#include <stdio.h>
#include <string.h>
#include <search.h>
#include "version.h"
#include "render.h"
#include "util.h"
#include "file.h"
#include "net.h"
#include "title.h"
#include "lua_common.h"
#include "sfx.h"
#include <SDL.h>
#include "input.h"
#include "sound.h"

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef __WINE__
#define LR_VGACOLOR LR_VGA_COLOR
#endif

//
// GLOBAL VARIABLES
//

bool Debug = true;
bool ShowFrameRate = false;
bool ShowInfo = false;

int cliSleep = 0;

render_info_t render_info;

//
// Parses the directory to change to from the command line options
//

static bool parse_chdir( char *cli )
{
    char * option;
	char cmdline[256];
	size_t size;

	size = strlen(cli)+1;
	if ( size > sizeof(cmdline) )
	{
		Msg("Command line to long!");
		return false;
	}
	strcpy(cmdline,cli);

	option = strtok(cmdline, " -+'\"");

    while(option != NULL )
	{
		// WARNING:	chdir can only be the LAST option !
		// For running the exe outside the root folder
		// -chdir c:\\Program Files\\ProjectX
		if (!strcasecmp(option,"chdir"))
		{
			// get option enclosed in quotes
			// this does not consider space as separator
			option = strtok(NULL, "\"'");

			if (!option)
			{
				Msg("Error using chdir");
				return false;
				break;
			}

			// change to root directory
			// the rest of the command line will be used as the path
			if( chdir( option ) != 0 )
			{
				// error
				Msg("Could not change to directory: %s", option);
				return false;
			}

			// dont loop anymore were done
			break;
		}
		// get the next token
        option = strtok(NULL, " -+'\"");
	}

	return true;
}

//
// checks for all critical folders
//

#define CRITICAL_FOLDERS 4
static bool missing_folders( void )
{
	int x = 0;
	char* folders[CRITICAL_FOLDERS] = {"Configs","Data","Pilots","Scripts"};
	for( x = 0; x < CRITICAL_FOLDERS; x++ )
		if( ! is_folder(folders[x]) )
		{
			Msg("Could not locate the '%s' folder...\n%s\n%s", folders[x],
				"exe is most likely in the wrong directory.",
				"or you just need to create the folder.");
			return true;
		}
	return false;
}


//
// Parse the Command Line
//

extern bool NoSFX;
extern float normal_fov;
extern float UV_Fix;
extern int NetUpdateIntervalCmdLine;
extern char *config_name;
extern int cliSleep;
extern TEXT local_port_str;
extern bool SpaceOrbSetup;
extern TEXT TCPAddress;
extern TEXT local_port_str;
extern TEXT host_port_str;
extern bool DebugLog;
extern u_int8_t QuickStart;
extern bool IpOnCLI;

static bool ParseCommandLine(char* lpCmdLine)
{
	
	//
	//  Locals
	//

    char * option;
	char cmdline[256];
	size_t size;

    //
    //  Set Global Defaults
    //
	
	NoSFX					= false; // turns off sound
	Debug					= false; // turns it off now

	NetUpdateIntervalCmdLine	= 0;

	//
	// Get the command line string
	//

	size = strlen(lpCmdLine)+1;
	if ( size > sizeof(cmdline) )
	{
		Msg("Command line to long!");
		return false;
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
		if (!strcasecmp(option,"chdir"))
		{
			// dont loop anymore were done
			break;
		}

		// sdl + opengl setting
		else if (!strcasecmp(option,"ForceAccel")){
			render_info.force_accel = true;
		}

		// off only works in full screen...
		// turn on vertical syncing
		else if (!strcasecmp(option,"VSync")){
			render_info.vsync = true;
		}

		// debugging information send to Log...
        else if (!strcasecmp(option, "log"))
		{
            DebugLog = true;
		}

		// debugging information
        else if (!strcasecmp(option, "Debug"))
		{
            Debug = true;
		}
		
		// start in window mode
		else if (!strcasecmp(option,"Fullscreen"))
		{
			render_info.fullscreen = true;
		}

		// start in window mode
		else if (!strcasecmp(option,"Window"))
		{
			render_info.fullscreen = false;
		}

		// turn off sound
		else if (!strcasecmp(option, "NoSFX"))
		{
			NoSFX = true;
        }
		
		// jump to the host screen
		else if ( !strcasecmp( option, "QuickHost" ) ) 
		{
			QuickStart = QUICKSTART_Start;
		}

		// jump to the join game screen
		else if ( !strcasecmp( option, "QuickJoin" ) ) 
		{
			QuickStart = QUICKSTART_Join;
		}

		// set the ip address for game to join
		else if ( !strcasecmp( option, "TCP" ) )
		{
			char * port;
			char address[255];

			IpOnCLI = true;

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
			strcpy( (char*)TCPAddress.text, address );
		}

		// supposedly to set wire mode for mxv's...
		else if (!strcasecmp(option, "wireframe")) 
		{
            render_info.wireframe = true;
        }

		// special override to allow setting up of spaceorb
		else if ( !strcasecmp( option, "SetupSpaceOrb" ) )
		{
			SpaceOrbSetup = true;
		}

		// use sscanf
		else 
		{
			int w,h;

			// override local port
			if ( sscanf( option, "port:%s", (char*)&local_port_str.text[0] ) )
			{
				DebugPrintf("Command Line: local port set to %s\n", local_port_str.text);
			}

			// sleep time for every loop
			else if ( sscanf( option, "sleep:%d", &cliSleep )){}

			// select the pilot
			else if ( sscanf( option , "pilot:%s", config_name )){}

			// set the packets per second
			else if ( sscanf( option, "PPS:%d", &NetUpdateIntervalCmdLine ) ){}

			// resolution mode
			// must be a valid resolution list in the resolution list in game
			// other wise you will end up with the default highest possible resolution
			// note: if you pick a value not in the list then your window will be 1 size and your resolution another
			else if ( sscanf( option, "mode:%d:%d", &render_info.default_mode.w, &render_info.default_mode.h ) ){}

			// let user fix up aspect
			else if ( sscanf( option, "aspect:%d:%d", &w, &h ) )
			{
				DebugPrintf("cli: aspect ratio set to %d:%d\n",w,h);
				render_info.aspect_ratio = (float) w / (float) h;
			}

			// modifies texture dimentions.. don't now what uv stands for..
			else if ( sscanf( option, "UVFix:%f", &UV_Fix ) ){}

			// set the horizontal frame of view
			// this is the screen stretching when you go into nitro
			// default is 90... max is 120...
			else if ( sscanf( option, "fov:%f", &normal_fov ) ){}

			//
			else {
				DebugPrintf("cli: unknown option: %s\n",option);
			}
        }

		// get the next token
        option = strtok(NULL, " -+'\"");

    }

	return true;
}

//
// Cleans up the application before quiting
//

extern void ReleaseView(void);
extern void DestroySound( int flags );
extern void render_cleanup( render_info_t * info );
extern void ReleaseScene(void);

bool QuitRequested = false;
void CleanUpAndPostQuit(void)
{
	// check if this function was ran already
    if (QuitRequested)
		return;

	// kill stuff
    ReleaseView();

	// stop rendering and destroy objects
	render_cleanup( &render_info );

	// destroy the sound
	DestroySound( DESTROYSOUND_All );

	// destroy direct input
	joysticks_cleanup();
  
	// release the scene
	ReleaseScene();

	// set flag
    QuitRequested = true;

	// we dont control the cursor anymore
	input_grab( false );

	// close up lua
	lua_shutdown();

	// cleanup networking
	network_cleanup();

#ifdef SOUND_SUPPORT

	// cleanup sound system
	sound_destroy();

#endif

	// should come last
	SDL_Quit();
}

//
// Initializes the application
//

#ifdef BREAKPAD
// breakpad running through wine, built for windows doens't work well..
#ifndef __WINE__
extern bool breakpad_init( void );
#endif
#endif

extern bool InitView( void );
extern void GetGamePrefs( void );
extern void SetSoundLevels( int *dummy );
extern void GetDefaultPilot(void);
extern bool InitScene(void);
extern BYTE MyGameStatus;

#include "mload.h"
extern RENDEROBJECT Portal_Execs[ MAXGROUPS ];
extern RENDEROBJECT Skin_Execs[ MAXGROUPS ];
extern RENDEROBJECT RenderBufs[4];

static bool AppInit( char * lpCmdLine )
{
#ifdef DEBUG_ON
	InitMathErrors();
#endif

	ZERO_STACK_MEM(render_info);
	ZERO_STACK_MEM(RenderBufs);
	ZERO_STACK_MEM(Portal_Execs);
	ZERO_STACK_MEM(Skin_Execs);

	render_info.vsync = false;

#ifdef DXMOUSE
	if(!dx_init_mouse())
	{
		DebugPrintf("Could not init dx mouse\n");
		return false;
	}
#endif

#ifdef BREAKPAD
// breakpad running through wine, built for windows doens't work well..
#ifndef __WINE__

	// initialize google breakpad crash reporting
	if(!breakpad_init())
		return false;

	// test breakpad by uncommenting this
	//{ *(int*)0=0; }

#endif
#endif

#ifdef DEBUG_ON

	// special debuggin routines
	XMem_Init();

#endif

	//
	if(!sdl_init())
		return false;

	// parse chdir from command line first
	if(!parse_chdir(lpCmdLine))
		return false;

	// we are now in the skeleton folder
	// now we need to see if we are in right place

	// check for missing folders
	if(missing_folders())
		return false;

	// startup lua
	if( lua_init() != 0 )
		return false;

	// copy game settings from config
	GetGamePrefs();

	// our configs are now loaded
	// now we can check the command line for overrides

	// parse the command line
	if(!ParseCommandLine(lpCmdLine))
		return false;

	//
	// create and show the window
	//
	if(!sdl_init_video())
	{
		Msg("sdl_init_video() returned false");
		return false;
	}

	// appears dinput has to be after init window

	// initialize direct input
	// This requires an application and window handle
	// so it most not come earlier than here
	if (!joysticks_init())
	{
		Msg("Failed to initialized joysticks!");
		return false;
	}
	
	// this needs to come after joysticks_init
	// because joysticks_init will wipe the joystick settings
	GetDefaultPilot();

// this is where it starts to take so long cause it scans directory for dynamic sound files...

	// start the title scene
	MyGameStatus = STATUS_Title;

	if (!InitScene())
		return false;

	// load the view
	if (!InitView() )
	{
	    Msg("InitView failed.\n");
		//CleanUpAndPostQuit();
        return false;
	}

	// exclusively grab input in fullscreen mode
	input_grab( render_info.fullscreen );

	//
	SetSoundLevels( NULL );

	// done
	DebugPrintf("AppInit finished...\n");
    return true;

}

//
// Render the next frame and update the window
//

extern bool RenderScene( void );

static bool RenderLoop()
{
    if ( !render_info.ok_to_render || render_info.minimized || render_info.bPaused || QuitRequested )
		return true;

    // Call the sample's RenderScene to render this frame
    if (!RenderScene())
	{
        Msg("RenderScene failed.\n");
        return false;
    }

	if ( quitting )
	{
		quitting = false;
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
				Msg("RenderLoop: render_flip() failed\n");
				return false;
			}
		}
	}

	//
    return true;
}

//
// The main routine
//

extern void network_cleanup( void );
extern bool SeriousError;
extern void CleanUpAndPostQuit(void);

int main( int argc, char* argv[] )
{
	int i;
	char cli[500];
    int failcount = 0; // number of times RenderLoop has failed

	// build cli string
	// TODO - cli parsing should be updated
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
	DebugPrintf("cli: %s\n",cli);

    // Create the window and initialize all objects needed to begin rendering
    if(!AppInit(cli))
		goto FAILURE;

	while( !QuitRequested )
	{
		// process system events
		if(!handle_events())
			goto FAILURE;

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

		// command line asks us to sleep and free up sys resources a bit...
		if ( cliSleep )
			SDL_Delay( cliSleep );

#if defined(DEBUG_ON) && defined(DEBUG_ALL_MATH_ERRORS)
		DebugMathErrors();
#endif
	}

	DebugPrintf("exit(0)\n");

	return 0;

FAILURE:

	//
	CleanUpAndPostQuit();

#ifdef DEBUG_ON

	DebugMathErrors();

	if ( UnMallocedBlocks() )
		DebugPrintf( "Un-malloced blocks found!\n" );

#endif

	DebugPrintf("exit(1)\n");

	return 1;
}
