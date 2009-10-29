
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
#include	"malloc.h"
#include	"SBufferHand.h"
#include	"file.h"
#include	"XMem.h" 
#include	"Sfx.h"
#include	"util.h"
#include	"net.h"
#include	"lua_common.h"
#include	"d3dappi.h"
#include	"util.h"
#include	"d3dappi.h"

	extern "C" HINSTANCE hInstApp;
	extern BOOL cursor_clipped;
	extern int ignore_mouse_input;
	extern BOOL InitialTexturesSet;
	extern void ProcessVduItems( MENU *Menu );
	extern void DarkenRoom2(float darkness);
	extern float RoomDarkness;
	extern void FadeHoloLight(float Brightness );
	extern float HoloLightBrightness;
	extern void SetGamePrefs( void );
	extern void ReleaseScene(void);
	extern void ReleaseView(void);
	extern BOOL InitScene(void);
	extern BOOL init_renderer( render_info_t * info );
	extern BOOL Debug;
	extern BOOL DebugLog;
	extern BOOL HideCursor;
	extern BOOL TermDInput( void );
	extern BOOL MouseExclusive;
	extern LONGLONG  LargeTime;
	extern LONGLONG  LastTime;
	extern BOOL InitDInput(void);
	extern char * LogFilename;
	extern char *config_name;
	extern render_info_t render_info;
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
	extern BOOL	MoviePlaying;
	extern BOOL SeriousError;
	extern	BOOL	NoSFX;
	extern	float	UV_Fix;
	extern BOOL AllWires;
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
	
	extern BOOL ShowFrameRate;
	extern BOOL QuitRequested;
	extern BOOL ShowInfo;
}


// GLOBAL VARIABLES

BOOL Debug					= FALSE;
HINSTANCE hInstApp;
BOOL ShowFrameRate = TRUE;
BOOL QuitRequested = FALSE;
BOOL ShowInfo = FALSE;

// INTERNAL FUNCTION PROTOTYPES

static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine);

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void CleanUpAndPostQuit(void);

static void InitGlobals(void);

extern "C" void SetInputAcquired( BOOL );
static BOOL RenderLoop(void);

BOOL ParseCommandLine(LPSTR lpCmdLine);

int cliSleep = 0;

render_info_t render_info;


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
	while (!QuitRequested)
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
			if ( render_info.window )
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
        if (render_info.bRenderingIsOK && !render_info.bMinimized && !render_info.bPaused && !QuitRequested)
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

int default_x;
int default_y;
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
		large_icon = (HICON) LoadImage( hInstApp, "AppIcon", IMAGE_ICON, 32, 32, LR_VGACOLOR );
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
		small_icon = (HICON) LoadImage( hInstApp, "AppIcon", IMAGE_ICON, 16, 16, LR_VGACOLOR );
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
    wc.hInstance		= hInstApp;							// window instance
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
		screen_aspect_ratio = (float) (render_info.default_mode.w / render_info.default_mode.h);

    // Create a window with some default settings that may change
	render_info.window = CreateWindow(

         "MainWindow",			// window class name (registered above)
         ProjectXVersion,	// window title

		 WS_TILEDWINDOW, // frame, resizing, caption, overlap, sysmenu, min|max|lower

         0, 0, // start position x,y
		 render_info.default_mode.w,
		 render_info.default_mode.h,

         NULL,				// parent window
         NULL,			    // menu handle
         hInstApp,	// program handle
         NULL				// pointer to pass to WM_CREATE event

	);
	
	if( render_info.window == NULL )
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
		placement.rcNormalPosition.right	= default_x + render_info.default_mode.w;
		placement.rcNormalPosition.bottom	= default_y + render_info.default_mode.h;
		SetWindowPlacement( render_info.window, &placement );
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


extern "C" BOOL InitView( void );
static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine)
{
    ZEROMEM(render_info);

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
    ShowFrameRate	= TRUE;
    ShowInfo		= FALSE;
    hInstApp		= hInstance;

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
	if (!init_renderer( &render_info ))
		return FALSE;

	// load the view
	if (!InitView() )
	{
	    Msg("InitView failed.\n");
		//CleanUpAndPostQuit();
        return FALSE;
	}

	// show the mouse if acting like window
	if ( ActLikeWindow || ! render_info.bFullscreen )
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
	NoCursorClip			= FALSE;

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
			else if ( sscanf( option , "pilot:%s", &config_name )){}

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

// Render the next frame and update the window
extern "C" BOOL RenderScene( void );
static BOOL RenderLoop()
{

    // If all the DD and D3D objects have been initialized we can render
    if ( ! render_info.bRenderingIsOK )
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
		if ((!PlayDemo || ( MyGameStatus != STATUS_PlayingDemo ) ||	DemoShipInit[ Current_Camera_View ]	))
		{
			// this is the actual call to render a frame...
			if (!render_flip(&render_info))
			{
				Msg("RenderLoop: render_flip() failed");
				return FALSE;
			}
		}

		// if there is 3D sound, commit 3D sound processing
		if ( lpDS3DListener )
			if ( lpDS3DListener->CommitDeferredSettings() != DS_OK )
				DebugPrintf("Error commiting 3D\n");

	}

	//
    return TRUE;
}

extern void SetInputAcquired( BOOL );
extern void SetCursorClip( BOOL );
extern BOOL HideCursor;
extern BOOL ActLikeWindow;
extern BOOL MouseExclusive;
extern "C" BOOL render_initialized;
extern "C" BOOL RenderModeReset( void );

BOOL bIgnoreWM_SIZE = FALSE;   /* Ignore this WM_SIZE messages */

BOOL window_proc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                 UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    *bStopProcessing = FALSE; // when set to true stops processing the message

	//
	if (!render_initialized)
        return TRUE;

    /*
     * Look for messages which effect rendering.  In some cases, we will not
     * want the app to continue processing the message, so set the flag and
     * provide a return value in lresult.
     */
    switch(message) {

		// user requested application to close
		case WM_CLOSE:

			// in case the window isn't in the foreground
			// we'd end up with a huge black spot from being previously covered

					// render one frame :]
					BeginPaint(hwnd, &ps);
					render_flip(&render_info);
					EndPaint(hwnd, &ps);

			// release mouse so they can interact with message box
			SetCursorClip( FALSE );

			// ask them to confirm clossing
			if( IDOK == Msg("Are you sure you want to exit?") )
			{
				// user wants to quit
				// let our code know we're quitting and not failing
				// let the message reach DefWindowProc so it calls CloseWindow
				QuitRequested = 1;
			}

			// user says to cancel the close...
			else
			{

				// grab the input again
				if ( ! MouseExclusive ) // will capture if so...
					SetInputAcquired( TRUE );

				// clip if HideCursor requests so...
				if ( HideCursor )
					SetCursorClip( TRUE );

				else if ( MouseExclusive )
					SetCursorClip( TRUE );

				// don't clip if acting like a window
				// and clip if fullscreen
				else if ( ActLikeWindow )
					SetCursorClip( FALSE );
				
				// clip the cursor if we are in fullscreen
				else if ( render_info.bFullscreen )
					SetCursorClip( TRUE );


				// eat the message
				*lresult = 1;
				*bStopProcessing = TRUE;

			}
			break;
			
		// window has been destroyed
		//case WM_DESTROY:
		//	break;

		//case WM_MOVING:
			//DebugPrintf("The window is moving.\n");
        //    break;

        case WM_MOVE:
			//DebugPrintf("Window has been moved (top left corner moved).\n");

			// these must be set to 0 before changing
			render_info.pClientOnPrimary.x = 0;
			render_info.pClientOnPrimary.y = 0;

			// this sets the CLIENT AREA POSITION
			render_info.pClientOnPrimary.x = (int)(short) LOWORD(lParam);
			render_info.pClientOnPrimary.y = (int)(short) HIWORD(lParam);

			// save the WINDOW POSITION
			{
				WINDOWPLACEMENT placement;
				placement.length = sizeof(WINDOWPLACEMENT);
				if(GetWindowPlacement( hwnd, &placement ))
				{
					render_info.pWindow.x = placement.rcNormalPosition.left;
					render_info.pWindow.y = placement.rcNormalPosition.top;
				}
			}
			
			SetGamePrefs();

            break;

        case WM_SIZE: // happens after WM_WINDOWPOSCHANGED
			//DebugPrintf("Window size changed.\n");
			
            if (!bIgnoreWM_SIZE)
			{
				// we should save the size
				// then startup at this size next time

				if( MyGameStatus == STATUS_Title )
				{
					LastMenu = CurrentMenu;	
					VduClear();
				}

				// resize d3d to match the window size..
				// TODO does d3d do the resizing on it's own now?

				if( MyGameStatus == STATUS_Title )
				{
					FadeHoloLight(HoloLightBrightness);
					DarkenRoom2(RoomDarkness);
					ProcessVduItems( CurrentMenu );
   					InitialTexturesSet = FALSE;
				}
			}

			SetGamePrefs();

            break;

		// We have been clicked so activate the application
		// the next case statement will catch the activation message
        case WM_MOUSEACTIVATE:
			//DebugPrintf("Window is defocused and has been clicked.\n");\
			// mouse has activated us and clicked on the client area
			// don't active if a menu is showing
			// cause we turn that off...
			if ( LOWORD( lParam ) == HTCLIENT )
			{
				// of course with exclusive mode this will also clip the mouse
				// but not much we can do because we need to get focus...
				// don't put this under the HideCursor section...
				SetInputAcquired( TRUE );
				SetCursorClip( HideCursor );
				
				if ( MouseExclusive )
				{
					SetCursorClip( TRUE );
					return TRUE;
				}

				// activate the application and remove event from queue
				*lresult = MA_ACTIVATEANDEAT;
				*bStopProcessing = TRUE;
			}
			else
			{
				// hitting title bar in wine causes capture...
				// we don't want that...
				if ( MouseExclusive )
				{
					SetInputAcquired( FALSE );
					SetCursorClip( FALSE );
					*lresult = 0;
					*bStopProcessing = TRUE;
				}
			}
			// do not eat the input on non client hits...
			// you want the close button etc.. to still work...
			break;

		// this seems to be a catch all for click on taskbar entry and title bar...
		// rather not make this cause cursor to clip
        case WM_ACTIVATE: // should recieve keyboard focus if being activated

			// release the mouse on deactivation
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				DebugPrintf("Window has been de-activated.\n");
				// release clip and acquired state
				SetInputAcquired( FALSE );
				SetCursorClip( FALSE );
			}
			// need to set keyboard and mouse focus
			// but don't clip the mouse...
			else
			{
				DebugPrintf("Window has been activated.\n");
				ignore_mouse_input = 15; // for this many reads
				SetInputAcquired( TRUE );

				if ( MouseExclusive )
				{
					SetCursorClip( TRUE );
					return TRUE;
				}

				// i don't think we should ever clip cursor unless in game play
				// this makes it work better in wine emulation mode
				// plus we could use the cursor for on screen shit sometimes ui updates ?

				// show cursor if we are acting like window
				if ( ActLikeWindow )
					SetCursorClip( FALSE );

				// hide cursor in fullscreen
				else if ( render_info.bFullscreen )
					SetCursorClip( TRUE );

			}

            break;

		// this event is same as above but sends TRUE|FALSE
        case WM_ACTIVATEAPP:
			DebugPrintf("Window is being %s.\n",(wParam?"activated":"de-activated"));
            render_info.bAppActive = (BOOL)wParam;
			if(wParam)
				RenderModeReset();
            break;

		// this means the app is about to minimize or maximize
		// we should probably trap maximize messages and go fullscreen...
        case WM_GETMINMAXINFO:
			//DebugPrintf("The size or position of the window is about to change.\n");
            break;

        case WM_PAINT:
			//DebugPrintf("Something has requested that we update/paint our screen.\n");
            // Clear the rectangle and blt the backbuffer image
            BeginPaint(hwnd, &ps);
            render_flip(&render_info);
            EndPaint(hwnd, &ps);
			//*lresult = 1;
			//*bStopProcessing = TRUE;
			break;

        case WM_NCPAINT:
			//DebugPrintf("We are requested to update the window frame.\n");
            // When in fullscreen mode, don't draw the window frame.
            if (render_info.bFullscreen) {
           //     *lresult = 0;
           //     *bStopProcessing = FALSE;
            }
            break;
/*
		// we should have a ShowCursor global
		// then any other part of the code can turn it on/off
		// I'd rather leave cursor showing at all times except real game time...
        case WM_SETCURSOR:
			//DebugPrintf("SETCURSOR: Mouse is within window but mouse is not captured.\n");
            //if (render_info.bFullscreen) {
				//SetCursor(NULL);
                //*lresult = 1;
                //*bStopProcessing = TRUE;
            //}
            break;

		case WM_WINDOWPOSCHANGING: // is changing
			//DebugPrintf("Window size, position, or place in z order is about to change.\n");
			break;

		case WM_CHAR:
			//DebugPrintf("TranslateMessage has generated a CHAR (standard ascii character) out of a WM_KEYDOWN event.\n");
			break;
*/

		case WM_KEYDOWN: // non system key (no alt modifier)
			{
				// if lParam bit 30 is 1 then key is REPEATING
				int repeating = (lParam & (1<<30));

				//DebugPrintf("virtual-key %x is %s.\n", wParam, (repeating?"repeating":"pressed") );

				// PAUSE
				if ( wParam == VK_PAUSE )
				{
					if ( ! repeating ) // only want first event
					{
						DebugPrintf("VK_PAUSE.\n");
						// we need a key to swap in and out of fullscreen
						// and swap in/out of mouse acquire mode...
						SetInputAcquired( ! cursor_clipped );
						SetCursorClip( ! cursor_clipped );
					}
				} 

				// SHIFT + F12
				// fullscreen toggle
				else if ( wParam == VK_F12 && HIWORD( GetKeyState( VK_SHIFT ) ) )
				{
					MenuGoFullScreen( NULL );	
					*lresult = 0; // we are processing the message
					*bStopProcessing = TRUE;
				}
			}
			break;

		// might want to migrate this into ReadInput
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			//DebugPrintf("mouse button event on client area...\n");

			// of course with exclusive mode this will also clip the mouse
			// but not much we can do because we need to get focus...
			// don't put this under the HideCursor section...
			SetInputAcquired( TRUE );

			// only clip if told
			if ( HideCursor || MouseExclusive )
				SetCursorClip( TRUE );

			break;

		// something just put us into idle mode
		// this means that DefWindowProc is blocking the main loop
		// expecting some kinda context menu or dialog to respond with the user action
		case WM_ENTERIDLE:
			switch ( wParam )
			{
			case MSGF_DIALOGBOX:
				DebugPrintf("WM_ENTERIDLE triggered because of: MSGF_DIALOGBOX\n");
				break;
			case MSGF_MENU:
				DebugPrintf("WM_ENTERIDLE triggered because of: MSGF_MENU\n");
				break;
			default:
				DebugPrintf("WM_ENTERIDLE triggered because of: Unknown Reason\n");
			}
			break;

		// we might have to stop certain key strokes here like, alt+ctrl+del
		// or perhaps drop out of screen and loose focus...
		case WM_SYSCOMMAND: // system key (alt modifier)
			//DebugPrintf("A command from the window menu or window buttons has been selected.\n");
			switch( wParam )
			{
			// we have no window menu
			// hitting alt initiates the window menu
			// DefWindowProc blocks expecting the window menu to return some kinda event
			// so we block this call here to stop any menu messages to cause the app to enter 
			case SC_KEYMENU:
				DebugPrintf("WM_SYSCOMMAND SC_KEYMENU 0x%x\n",lParam);
				*lresult = 0;
				*bStopProcessing = TRUE;
				break;
				/*
			case SC_CLOSE:
				DebugPrintf("WM_SYSCOMMAND SC_CLOSE\n");
				break;
			case SC_CONTEXTHELP:
				DebugPrintf("WM_SYSCOMMAND SC_CONTEXTHELP\n");
				break;
			case SC_DEFAULT:
				DebugPrintf("WM_SYSCOMMAND SC_DEFAULT\n");
				break;
			case SC_HOTKEY:
				DebugPrintf("WM_SYSCOMMAND SC_HOTKEY\n");
				break;
			case SC_HSCROLL:
				DebugPrintf("WM_SYSCOMMAND SC_HSCROLL\n");
				break;
			case SC_MAXIMIZE:
				DebugPrintf("WM_SYSCOMMAND SC_MAXIMIZE\n");
				break;
			case SC_MINIMIZE:
				DebugPrintf("WM_SYSCOMMAND SC_MINIMIZE\n");
				break;
			case SC_MONITORPOWER:
				DebugPrintf("WM_SYSCOMMAND SC_MONITORPOWER\n");
				break;
			case SC_MOUSEMENU:
				DebugPrintf("WM_SYSCOMMAND SC_MOUSEMENU\n");
				break;
			case SC_MOVE:
				DebugPrintf("WM_SYSCOMMAND SC_MOVE\n");
				break;
			case SC_NEXTWINDOW:
				DebugPrintf("WM_SYSCOMMAND SC_NEXTWINDOW\n");
				break;
			case SC_PREVWINDOW:
				DebugPrintf("WM_SYSCOMMAND SC_PREVWINDOW\n");
				break;
			case SC_RESTORE:
				DebugPrintf("WM_SYSCOMMAND SC_RESTORE\n");
				break;
			case SC_SCREENSAVE:
				DebugPrintf("WM_SYSCOMMAND SC_SCREENSAVE\n");
				break;
			case SC_SIZE:
				DebugPrintf("WM_SYSCOMMAND SC_SIZE\n");
				break;
			case SC_TASKLIST:
				DebugPrintf("WM_SYSCOMMAND SC_TASKLIST\n");
				break;
			case SC_VSCROLL:
				DebugPrintf("WM_SYSCOMMAND SC_VSCROLL\n");
				break;
			default:
				DebugPrintf("WM_SYSCOMMAND unknown action.\n");
				*/
			}
			break;

		// stuff that should be ignored in fullscreen
		case WM_NCHITTEST:		// mouse on non client area
		case WM_CONTEXTMENU:	// window context menu
			if(render_info.bFullscreen)
			{
				*lresult = 0;
				*bStopProcessing = TRUE;
			}
			break;

		case WM_DEVICECHANGE:
			DebugPrintf("A device has changed.\n");
			break;

		case WM_DISPLAYCHANGE:
			DebugPrintf("Display resolution has changed.\n");
			break;


    }
    return TRUE;
}

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

    BOOL bStop;
    LRESULT lresult;

    // Give D3DApp an opportunity to process any messages
    if (!window_proc(&bStop, &lresult, hWnd, message, wParam, lParam))
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

extern "C" void render_cleanup( render_info_t * info );
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
	TermDInput();
  
	// release the scene
	ReleaseScene();

	// set flag
    QuitRequested = TRUE;

	// we dont control the cursor anymore
	SetInputAcquired( FALSE );
	SetCursorClip( FALSE );

	//
    PostQuitMessage( 0 );
}

extern "C"
int Msg( char * msg, ... )
{
	BOOL was_fullscreen = render_info.bFullscreen;

	char txt[ 1024 ];
	va_list args;
	int res;

	va_start( args, msg );
	vsprintf( txt, msg, args);
	va_end( args );

    if (render_info.bFullscreen)
	{
		// switch to window mode
		// other wise pop up will get stuck behind main window
		MenuGoFullScreen(NULL);
		// push main window to background so popup shows
        SetWindowPos(render_info.window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	// release mouse so they can interact with message box
	SetCursorClip( FALSE );

    res = MessageBox( render_info.window, txt, "Forsaken", MB_OKCANCEL | MB_ICONEXCLAMATION );

    if (was_fullscreen)
	{
		// switch back to fullscreen
		MenuGoFullScreen(NULL);
        SetWindowPos(render_info.window, HWND_TOPMOST, 0, 0, 0, 0,  SWP_NOSIZE | SWP_NOMOVE);
		SetCursorClip( TRUE ); // don't do this in window mode just let them click back on the window
	}

	DebugPrintf( txt );

	// IDCANCEL	Cancel button was selected.
	// IDOK	OK button was selected.
	return res;
}

