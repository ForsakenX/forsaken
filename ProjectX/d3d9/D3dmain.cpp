
// Includes

#include "typedefs.h"
#include "Local.h"
#include "resource.h"
#include "d3dmain.h"
#include "main.h"
#include "dsound.h"
#include "dbt.h"
#include <direct.h>
#include "version.h"
#include "client/windows/handler/exception_handler.h"

// load up C externals

extern "C" {

#include	"title.h"
#include	"stdwin.h"
#include	"d3dapp.h"
#include	"malloc.h"
#include	"SBufferHand.h"
#include	"file.h"
#include	"XMem.h" 
#include	"d3dapp.h"
#include	"Sfx.h"
#include	"util.h"
#include	"net.h"
#include	"lua_common.h"
#include	"d3dappi.h"

	extern void ReleaseScene(void);
	extern void ReleaseView(void);
	extern BOOL InitScene(void);
	extern BOOL init_renderer(HWND hwnd, BOOL fullscreen);
	extern BOOL Debug;
	extern BOOL DebugLog;
	extern BOOL HideCursor;
	extern BOOL Wine;
	extern BOOL TermDInput( void );
	extern BOOL MouseExclusive;
	extern LONGLONG  LargeTime;
	extern LONGLONG  LastTime;
	extern BOOL InitDInput(void);
	extern char * LogFilename;
	extern BOOL bFullscreen;
	extern char *config_name;
	extern D3DAppInfo d3dappi;
	extern BOOL NoCursorClip;
	extern void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval );
	extern	int NetUpdateIntervalCmdLine;
	extern void GetGamePrefs( void );
	extern BYTE	Current_Camera_View;		// which object is currently using the camera view....
	extern BOOL PlayDemo;
	extern BOOL DemoShipInit[];
	extern BOOL flush_input;
	extern int ddchosen3d;
	extern int default_x;
	extern int default_y;
	extern int default_width;
	extern int default_height;
	extern int default_bpp;
	extern BOOL	MoviePlaying;
	extern BOOL SeriousError;
	extern	BOOL	NoSFX;
	extern	float	UV_Fix;
	extern BOOL AllWires;
//	extern BOOL CanDoStrechBlt;
	extern float normal_fov;
	extern float screen_aspect_ratio;
	extern	BOOL LockOutWindows;
	extern	BOOL DS;
	extern BOOL SpaceOrbSetup;
	extern BOOL NoCompoundSfxBuffer;
	extern long UseDDrawFlip;
	extern TEXT local_port_str;
	extern TEXT host_port_str;

	extern LPDIRECTSOUND3DLISTENER	lpDS3DListener;

	extern DWORD UserTotalCompoundSfxBufferSize;
	extern BOOL CustomCompoundBufferSize;

	extern uint8 QuickStart;
	extern BOOL IpOnCLI;

	int DebugMathErrors( void );
	void OnceOnlyRelease( void );

	void RemoveDynamicSfx( void );
	void FillStatusTab( void );

	BOOL ActLikeWindow = FALSE;

	void GetDefaultPilot(void);
	void SetSoundLevels( int *dummy );
}


// GLOBAL VARIABLES

BOOL render_initialized;

d3dmainglobals myglobs;     // collection of global variables

BOOL Debug					= FALSE;

// INTERNAL FUNCTION PROTOTYPES

static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine);

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void CleanUpAndPostQuit(void);

static void InitGlobals(void);

extern "C" void SetInputAcquired( BOOL );
static BOOL RenderLoop(void);

extern "C" BOOL VSync = FALSE;

BOOL ParseCommandLine(LPSTR lpCmdLine);

int cliSleep = 0;


/****************************************************************************/
/*                            WinMain                                       */
/****************************************************************************/
/*
 * Initializes the application then enters a message loop which calls sample's
 * RenderScene until a quit message is received.
 */

extern "C" void network_cleanup( void );
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    int failcount = 0; // number of times RenderLoop has failed
    hPrevInstance;
    MSG msg;

    // Create the window and initialize all objects needed to begin rendering
    if(!AppInit(hInstance, lpCmdLine))
		goto FAILURE;

    // Main game loop...
	while (!myglobs.bQuit)
	{

		// dispatches any messages that are in the queue and removes them
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// if the window got the WM_QUIT message
			if (msg.message == WM_QUIT)
			{
				// quit
				CleanUpAndPostQuit();
				break;
			}

			// we have a valid window handle
			if ( myglobs.hWndMain )
			{

				// translates virtual-key messages into character messages
				// posts them to the message queue
				// to be read the next time PeekMessage is called
				// this does not modify the original message
				// it simply creates new translated messages
				TranslateMessage(&msg);

				// dispatches a message to a window procedure
				DispatchMessage(&msg);

			}
		}

        // Render if app is not minimized, not about to quit, not paused and D3D initialized
        if (d3dappi.bRenderingIsOK && !d3dappi.bMinimized && !d3dappi.bPaused && !myglobs.bQuit)
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
        }

		// call the sound proccesses
		ProcessSoundRoutines( NULL );

		// command line asks us to sleep and free up sys resources a bit...
		if ( cliSleep )
			Sleep( cliSleep );

    }

FAILURE:

	// cleanup networking
	network_cleanup();

	// free the cursor to move outside the window
	ClipCursor( NULL );

	// show the cursor
	ReallyShowCursor( TRUE );

	// Uninitialize the COM library
	CoUninitialize();

	// close up lua
	lua_shutdown();

#ifdef DEBUG_ON

	//
	// debug messages
	//

	DebugMathErrors();

	if ( UnMallocedBlocks() )
		DebugPrintf( "Un-malloced blocks found!" );

//	if ( UnMallocedDDSurfBlocks() )
//		DebugPrintf( "Un-malloced DDSurf blocks found!" );

	if ( UnMallocedSBufferBlocks() )
		DebugPrintf( "Un-malloced SBuffer blocks found!" );

#endif

	//
    return msg.wParam;

}


/****************************************************************************/
/*             D3DApp Initialization and callback functions                 */
/****************************************************************************/

static BOOL InitWindow( void )
{
	HICON small_icon;
	HICON large_icon;
    WNDCLASSEX wc;

//// START UGLY

// It's supposed to be able to dyanmically load the right an icon
// from a resource holding multiple sizes

// For now I just manually
// Search for local ProjectX.ico and fall back to embedded image in exe
// 32x32 for large and 16x16 for small but falls back to any one found

	// load the application icons
	large_icon = (HICON) LoadImage(
		NULL,					// hInstance (not needed when load from file)
		"ProjectX.ico",			// image name / file path
		IMAGE_ICON,				// used as icon
		32,32,					// x,y height of the icon (0 == use actual)
		LR_LOADFROMFILE |		// load the icon from file
		LR_VGACOLOR				// use true color
	);

	// if failed to load the icon file then use the embedded icon in the exe
	// see above for explanation of fields
	if ( ! large_icon )
	{
		DebugPrintf("Failed to load large icon from ProjectX.ico.\n");
		large_icon = (HICON) LoadImage( myglobs.hInstApp, "AppIcon", IMAGE_ICON, 32, 32, LR_VGACOLOR );
		if( ! large_icon )
			DebugPrintf("Failed to load large AppIcon from executable.\n");
	}
	
	// load the application icons
	small_icon = (HICON) LoadImage(
		NULL,					// hInstance (not needed when load from file)
		"ProjectX.ico",			// image name / file path
		IMAGE_ICON,				// used as icon
		16,16,					// x,y height of the icon (0 == use actual)
		LR_LOADFROMFILE |		// load the icon from file
		LR_VGACOLOR				// use true color
	);

	// if failed to load the icon file then use the embedded icon in the exe
	// see above for explanation of fields
	if ( ! small_icon )
	{
		DebugPrintf("Failed to load small icon from ProjectX.ico.\n");
		small_icon = (HICON) LoadImage( myglobs.hInstApp, "AppIcon", IMAGE_ICON, 16, 16, LR_VGACOLOR );
		if( ! small_icon )
			DebugPrintf("Failed to load small AppIcon from executable.\n");
	}

	if ( ! small_icon && large_icon )
		small_icon = large_icon;
	if ( ! large_icon && small_icon )
		large_icon = small_icon;


//////// END UGLY


	// Setup the window class
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW |					// redraw window if horizontal width changes
						CS_DBLCLKS;									// send my window double click messages
    wc.lpfnWndProc		= WindowProc;								// processer for window messages
    wc.cbClsExtra		= 0;										// extra bytes to initialize
	wc.cbWndExtra		= 0;										// extra bytes to initialize
    wc.hInstance		= myglobs.hInstApp;							// window instance
    wc.hIcon			= large_icon;								// handle to icons
    wc.hCursor			= LoadCursor( NULL, IDC_ARROW );			// the cursor for mouse over window
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);		//
    wc.lpszClassName	= "MainWindow";								// class name
	wc.hIconSm			= small_icon;								// the small icon
	wc.lpszMenuName		= NULL;

	// Register the window class
    if (!RegisterClassEx(&wc))
	{
		Msg("RegistrClass failed: %d", GetLastError());
		DebugPrintf("RegistrClass failed: %d", GetLastError());
        return FALSE;
	}
	
	// this is just a safety cause if these values are 0
	// then starting in fullscreen and switching to window crashes
	// cause the last window state had a window size of 0
	
	if ( ! screen_aspect_ratio )
		screen_aspect_ratio = (float) (default_width / default_height);

    // Create a window with some default settings that may change
	myglobs.hWndMain = CreateWindow(

         "MainWindow",			// window class name (registered above)
         ProjectXVersion,	// window title

		 WS_TILEDWINDOW, // frame, resizing, caption, overlap, sysmenu, min|max|lower

         0, 0, // start position x,y
		 default_width,
		 default_height,

         NULL,				// parent window
         NULL,			    // menu handle
         myglobs.hInstApp,	// program handle
         NULL				// pointer to pass to WM_CREATE event

	);
	
	if( myglobs.hWndMain == NULL )
	{
		Msg("CreateWindow Failed: %d\n",GetLastError());
        DebugPrintf("CreateWindow Failed: %d\n",GetLastError());
        return FALSE;
    }

	// restore window position
	{
		WINDOWPLACEMENT placement;
		placement.length  = sizeof(WINDOWPLACEMENT);
		placement.showCmd = SW_SHOWNORMAL;
		placement.rcNormalPosition.left		= default_x;
		placement.rcNormalPosition.top		= default_y;
		placement.rcNormalPosition.right	= default_x + default_width;
		placement.rcNormalPosition.bottom	= default_y + default_height;
		SetWindowPlacement( myglobs.hWndMain, &placement );
	}

	//
	return TRUE;
}

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

BOOL parse_chdir( char *cli )
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

static void wchar_to_char( char * dst_ptr, const wchar_t * src_ptr )
{
	while( *src_ptr != 0 ) // while not end of source
		*dst_ptr++ = (char) *src_ptr++; // conver to char
	*dst_ptr = 0; // finish string
}

static bool breakpad_callback(
	const wchar_t* dump_path,
	const wchar_t* minidump_id,
	void* context,
	EXCEPTION_POINTERS* exinfo,
	MDRawAssertionInfo* assertion,
	bool succeeded 
)
{
	// if writing to dump file succeeded
	if(succeeded)
	{    
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		char path[256];
		char dir[256];
		char id[256];

		wchar_to_char( &dir[0], &dump_path[0] );
		wchar_to_char( &id[0],  &minidump_id[0] );

		sprintf(path, "%s\\%s.dmp", dir, id);
		
		ZeroMemory( &si, sizeof(si) );
		ZeroMemory( &pi, sizeof(pi) );
		si.cb = sizeof(si);

		if(!CreateProcess( "Utils/crash_reporter.exe", path,
							NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ))
		{
			Msg("%s\n%s\n%s\n%s",
				"Error launching crash reporting tool!",
				"Crash report saved to: ", path,
				"Please email it to fskn.methods@gmail.com");
		}
	}

	// if we succeeded creating a dump
	// then stop any other handlers from running
	return succeeded;
}

static BOOL breakpad_init( void )
{
	using namespace google_breakpad;
	static ExceptionHandler * e;
	wstring path(L"Dumps");
	e = new ExceptionHandler(
		path, 
		NULL, 
		(google_breakpad::ExceptionHandler::MinidumpCallback) &breakpad_callback, 
		NULL, 
		ExceptionHandler::HANDLER_ALL 
	);//, MiniDumpNormal, pipe_name, custom_info );
	DebugPrintf("initialized breakpad\n");
	return TRUE;
}

static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	// Appears to be a complete fuckup...
	// Only used in two other places...
	// never updated...
	// wtf is the point...
	QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
	LastTime = LargeTime;

	// initialize google breakpad crash reporting
	if(!breakpad_init())
		return FALSE;

	// test breakpad by uncommenting this
	//{ *(int*)0=0; }

#ifdef DEBUG_ON

	// special debuggin routines
	XMem_Init();
	XSBuffer_Init();

#endif

	// initialize COM library
	if FAILED( CoInitialize(NULL) )
		return FALSE;

	// setup globals used by application
    memset(&myglobs, 0, sizeof(myglobs));
    myglobs.bShowFrameRate	= TRUE;
    myglobs.bShowInfo		= FALSE;
    myglobs.hInstApp		= hInstance;

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

	// create and show the window
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

    ZEROMEM(d3dappi);

	if (!init_renderer(myglobs.hWndMain, bFullscreen))
		return FALSE;

	render_initialized = TRUE;

	// show the mouse if acting like window
	if ( ActLikeWindow || ! d3dappi.bFullscreen )
	{
		DebugPrintf("AppInit setting mouse clip for fullscreen mode.\n");
		if ( MouseExclusive )
			SetInputAcquired( FALSE );
		SetCursorClip( FALSE );
	}

	SetSoundLevels( NULL );

	DebugPrintf("AppInit finished...\n");

	// done
    return TRUE;

}

//
// Parse the Command Line
//

BOOL ParseCommandLine(LPSTR lpCmdLine)
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
	Wine					= FALSE;
	NoCursorClip			= FALSE;
	VSync					= FALSE;

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
			VSync = TRUE;
		}

		// treate mouse like it's window mode regardless of fullscreen
		// used for wine desktop emulation mode...
		else if (!_stricmp(option, "ActLikeWindow")){
			ActLikeWindow = TRUE;
		}

		// don't exclusivly grab the mouse
		else if (!_stricmp(option, "MouseNonExclusive")){
			MouseExclusive = FALSE;
		}

		// never clip the cursor...
		else if (!_stricmp(option, "NoCursorClip")){
			NoCursorClip = TRUE;
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
			bFullscreen = TRUE;
		}

		// start in window mode
		else if (!_stricmp(option,"Window"))
		{
			bFullscreen = FALSE;
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

		// don't try to scale blitted text
		else if ( !_stricmp( option, "NoBlitTextScaling" ) )
		{
//			CanDoStrechBlt = FALSE;
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
			float fnum;
			DWORD mem;

			// override local port
			if ( sscanf( option, "port:%s", (char*)&local_port_str.text[0] ) == 1 )
			{
				DebugPrintf("Command Line: local port set to %s\n", local_port_str.text);
			}

			// sleep time for every loop
			else if ( sscanf( option, "sleep:%d", &cliSleep ))
			{
				// set directly
			}

			// select the pilot
			else if ( sscanf( option , "pilot:%s", &config_name ))
			{
				// sccanf writes to the config_name variable
			}

			// ammount of memory to allocate for sound buffer
			else if ( sscanf( option, "CompoundSfxBufferMem:%d", &mem ) == 1 )
			{
				UserTotalCompoundSfxBufferSize = mem;
				CustomCompoundBufferSize = TRUE;
			}

			// set the packets per second
			else if ( sscanf( option, "PPS:%d", &num ) == 1 )
			{
				NetUpdateIntervalCmdLine = num;
			}

			// resolution mode
			// although you can set this to any value you like
			// the values *should* (for now) be valid resolutions for your device
			// other wise you will end up with the default resolution (lowest)
			// BUT your window size will be set to the value you picked...
			// this means the window and resolution will be out of sync and doesn't look pretty
			// we should figure out how to stretch/scale the resolution on the window
			// just like when you resize the window
			// or just make the window default to the selected resolution...
			else if ( sscanf( option, "mode:%d:%d", &default_width, &default_height ) == 1 )
			{
				// values set directly by sscanf
			}

			// bits per pixel
			// by default the game picks 16bbps cause that renders the best
			// use bbp32 to get 32bbp/sec
			else if ( sscanf( option, "bpp:%d", &num ) == 1 )
			{
				default_bpp = num;
			}

			// modifies texture dimentions.. don't now what uv stands for..
			else if ( sscanf( option, "UVFix:%f", &fnum ) == 1 )
			{
				UV_Fix = fnum;
			}

			// set the horizontal frame of view
			// this is the screen stretching when you go into nitro
			// default is 90... max is 120...
			else if ( sscanf( option, "fov:%d", &num ) == 1 )
			{
				normal_fov = (float) num;
			}

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
        }

		// get the next token
        option = strtok(NULL, " -+'\"");

    }

	return TRUE;
}

// Render the next frame and update the window
extern "C" BOOL RenderScene( void );
static BOOL RenderLoop()
{

    // If all the DD and D3D objects have been initialized we can render
    if ( ! d3dappi.bRenderingIsOK )
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
	if( !myglobs.bQuit )
	{
		if ((!PlayDemo || ( MyGameStatus != STATUS_PlayingDemo ) ||	DemoShipInit[ Current_Camera_View ]	))
		{
			// this is the actual call to render a frame...
			if (!FlipBuffers())
			{
				Msg("RenderLoop: FlipBuffers() failed");
				return FALSE;
			}
		}

		// if there is 3D sound, commit 3D sound processing
		if ( lpDS3DListener )
			if ( lpDS3DListener->CommitDeferredSettings() != DS_OK )
				DebugPrintf("Error commiting 3D\n");

	}

	// Reset the resize flag
	myglobs.bResized = FALSE;

	//
    return TRUE;
}

/*************************************************************************
  Windows message handlers
 *************************************************************************/

/*
 * WindowProc
 * Main window message handler.
 */

long
FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

    BOOL bStop;
    LRESULT lresult;

    // Give D3DApp an opportunity to process any messages
    if (!D3DAppWindowProc(&bStop, &lresult, hWnd, message, wParam, lParam))
	{
		// quit
        CleanUpAndPostQuit();
        return 0;
    }

	// if bStop is set by D3DApp, dont process message and return lresult
    if (bStop)
        return lresult;

	//
	if ( quitting )
	{
		DebugPrintf("about to CleanUpAndPostQuit ( from WindowProc )\n");
		quitting = FALSE;
		CleanUpAndPostQuit();
	}

	// default processing for any messages not processed
    return DefWindowProc(hWnd, message, wParam, lParam);
}



/****************************************************************************/
/*          Initialization, error reporting and release functions.          */
/****************************************************************************/

/*
 * CleanUpAndPostQuit
 * Release all D3D objects, post a quit message and set the bQuit flag
 */

extern "C" void render_cleanup( void );
void CleanUpAndPostQuit(void)
{
	// check if this function was ran already
    if (myglobs.bQuit)
		return;

	// kill stuff
    ReleaseView();

	// stop rendering and destroy objects
	render_cleanup();

	// destroy the sound
	DestroySound( DESTROYSOUND_All );

	// destroy direct input
	TermDInput();
  
	// release the scene
	ReleaseScene();

	// set flag
    myglobs.bQuit = TRUE;

	// we dont control the cursor anymore
	SetInputAcquired( FALSE );
	SetCursorClip( FALSE );

	//
    PostQuitMessage( 0 );
}

extern "C"
int Msg( char * msg, ... )
{
	char txt[ 1024 ];
	va_list args;
	int res;

	va_start( args, msg );
	vsprintf( txt, msg, args);
	va_end( args );

    if (d3dappi.bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    res = MessageBox( NULL, txt, "Forsaken", MB_OKCANCEL | MB_ICONEXCLAMATION );

    if (d3dappi.bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_TOPMOST, 0, 0, 0, 0,  SWP_NOSIZE | SWP_NOMOVE);

	DebugPrintf( txt );

	return res;
}

