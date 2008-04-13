
/*
 *  Copyright (C) 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dmain.cpp
 *
 *  Each of the Direct3D samples must be linked with this file.  It contains
 *  the code which allows them to run in the Windows environment.
 *
 *  A window is created using d3dmain.res which allows the user to select the
 *  Direct3D driver to use and change the render options.  The D3DApp
 *  collection of functions is used to initialize DirectDraw, Direct3D and
 *  keep surfaces and D3D devices available for rendering.
 *
 *  Frame rate and a screen mode information buffer is Blt'ed to the screen
 *  by functions in stats.cpp.
 *
 *  Each sample is executed through the functions: InitScene, InitView,
 *  RenderScene, ReleaseView, ReleaseScene and OverrideDefaults, as described
 *  in d3ddemo.h.
 */


// Includes

#include "typedefs.h"
#include "Local.h"
#include "resource.h"
#include "d3dmain.h"
#include "main.h"
#include "dsound.h"
#include "dbt.h"
#include <direct.h>
#include "getdxver.h"
#include "dplay.h"

// load up C externals

extern "C" {

#include	"title.h"
#include	"stdwin.h"
#include	"d3dapp.h"
#include	"malloc.h"
#include	"Exechand.h" 
#include	"DDSurfhand.h" 
#include	"SBufferHand.h"
#include	"file.h"
#include	"XMem.h" 
#include	"d3dapp.h"
#include	"Sfx.h"

#ifdef MANUAL_SESSIONDESC_PROPAGATE
	extern LPDPSESSIONDESC2 glpdpSD_copy;
#endif

	extern BOOL TermDInput( void );

	extern LONGLONG  LargeTime;
	extern LONGLONG  LastTime;
	extern void InitValidPickups();
	extern void InitPolyText( void );
	extern BOOL InitDInput(void);
	extern char * LogFilename;
	extern BOOL bFullscreen;
	extern char *config_name;
	extern D3DAppInfo d3dappi;
	extern BOOL InitRegistry();
	extern BOOL CloseRegistry();
	extern LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);
	extern LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
	extern LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);

	extern BOOL ZClearsOn;
	extern BOOL AllowServer;
	extern void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval );
	extern BOOL CheckDirectPlayVersion;
	extern	int DPlayUpdateIntervalCmdLine;
	extern void GetGamePrefs( void );
	extern int ScreenWidth;
	extern int ScreenHeight;
	extern int ScreenBPP;
	extern BYTE	Current_Camera_View;		// which object is currently using the camera view....
	extern BOOL PlayDemo;
	extern BOOL DemoShipInit[];
	extern BOOL flush_input;
	extern int ddchosen3d;
	extern int default_width;
	extern int default_height;
	extern int default_bpp;
	extern BOOL	MoviePlaying;
	extern BOOL SeriousError;
	extern	BOOL	PowerVR_Overide;
	extern	BOOL	Is3Dfx;
	extern	BOOL	Is3Dfx2;
	extern	BOOL	TriLinear;
	extern	BOOL	NoSFX;
	extern	int		TextureMemory;
	extern	BOOL	NoTextureScaling;
	extern	BOOL	MipMap;
	extern  BOOL	DontColourKey;
	extern	BOOL	TripleBuffer;
	extern	BOOL	PolygonText;
	extern	BOOL	UseSendAsync;
	extern	float	UV_Fix;
	extern BOOL AllWires;
	extern BOOL CanDoStrechBlt;

	extern BOOL RecordDemoToRam;

	extern float normal_fov;
	extern float screen_aspect_ratio;
	extern	BOOL LockOutWindows;
	extern	BOOL	DplayRecieveThread;
	extern BOOL PreventFlips;
	extern	BOOL DS;
	extern	BOOL Debug;
	extern BOOL SpaceOrbSetup;
	extern BOOL NoCompoundSfxBuffer;
	extern long UseDDrawFlip;
	extern BOOL NoSplash;

	extern LPDIRECTSOUND3DLISTENER	lpDS3DListener;

	extern DWORD UserTotalCompoundSfxBufferSize;
	extern BOOL CustomCompoundBufferSize;

	extern uint8 QuickStart;
	extern BOOL ForceHeartbeat;

	extern GUID autojoin_session_guid;
	extern BOOL	SessionGuidExists;
	extern BOOL bTCP;
	
	int PreferredWidth, PreferredHeight;

	int DebugMathErrors( void );
	void OnceOnlyRelease( void );
	void DebugPrintf( const char * format, ... );

	void RemoveDynamicSfx( void );
	void FillStatusTab( void );

	HRESULT GUIDFromString( char *lpStr, GUID * pGuid);

}


// GLOBAL VARIABLES

D3DAppInfo* d3dapp = NULL;  // Pointer to read only collection of DD and D3D objects maintained by D3DApp

d3dmainglobals myglobs;     // collection of global variables

static UINT CancelAutoPlayMessage;

BOOL Debug					= FALSE;
BOOL DeviceOnCommandline	= FALSE;
BOOL bOnlySystemMemory		= FALSE;
BOOL bOnlyEmulation			= FALSE;

// INTERNAL FUNCTION PROTOTYPES

static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine);
static BOOL CreateD3DApp(void);
static BOOL BeforeDeviceDestroyed(LPVOID lpContext);
static BOOL AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lpViewport, LPVOID lpContext);

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void ReportD3DAppError(void);
void CleanUpAndPostQuit(void);

static void InitGlobals(void);
static BOOL AppPause(BOOL f);
static BOOL RenderLoop(void);
static BOOL RestoreSurfaces();

BOOL ParseCommandLine(LPSTR lpCmdLine);


/****************************************************************************/
/*                            WinMain                                       */
/****************************************************************************/
/*
 * Initializes the application then enters a message loop which calls sample's
 * RenderScene until a quit message is received.
 */


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    int failcount = 0; // number of times RenderLoop has failed
    hPrevInstance;
	UINT param1;
	UINT param2;
    MSG msg;

#ifdef DEBUG_ON

	XMem_Init();
	XExec_Init();
	DDSurf_Init();
	XSBuffer_Init();

#endif

	// initialize COM library
	if FAILED( CoInitialize(NULL) )
		goto FAILURE;

    // Create the window and initialize all objects needed to begin rendering
    if(!AppInit(hInstance, lpCmdLine))
		goto FAILURE;

    // Monitor the message queue until there are no pressing  messages
	while (!myglobs.bQuit)
	{

		//	This will disable windows key and alt-tab and ctrl-alt-del
		if( LockOutWindows )
		{
			param1 = WM_NULL;
			param2 = WM_KEYFIRST;
		}else{
			param1 = 0;
			param2 = 0;
		}

		// if retval is 1 there is a message
		// dispatches incoming sent messages
		// checks the thread message queue for a posted message
		// and retrieves the message (if any exist).
		// if there is a message process it.
		while(PeekMessage(&msg, NULL, param1, param2, PM_REMOVE))
		{
			// if the window got the WM_QUIT message
			if (msg.message == WM_QUIT)
			{
				// quit
				CleanUpAndPostQuit();
				break;
			}

			if
			(
				! d3dapp->bPaused  ||  // not paused
				! myglobs.hWndMain		// window handel is not bad
			)
			{

				// translates virtual-key messages into character messages
				// posts them to the calling thread's message queue
				// to be read the next time the thread calls the PeekMessage
				TranslateMessage(&msg);

				// dispatches a message to a window procedure
				DispatchMessage(&msg);

			}
		}

        // Render if app is not minimized, not about to quit, not paused and D3D initialized
        if (d3dapp->bRenderingIsOK && !d3dapp->bMinimized && !d3dapp->bPaused && !myglobs.bQuit)
		{

            // NOT in single step mode
			// OR  in single step mode with bDrawAFrame flag is set
            if (!(myglobs.bSingleStepMode && !myglobs.bDrawAFrame))

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

            // Reset the bDrawAFrame flag if we are in single step mode
            if (myglobs.bSingleStepMode)
                myglobs.bDrawAFrame = FALSE;

        }
    }

FAILURE:

	// free the cursor to move outside the window
	ClipCursor( NULL );

	// show the cursor
	ReallyShowCursor( TRUE );

	// Uninitialize the COM library
	CoUninitialize();

	// close up the registry 
	CloseRegistry();

#ifdef DEBUG_ON

	//
	// debug messages
	//

	DebugMathErrors();

	if ( UnMallocedBlocks() )
		DebugPrintf( "Un-malloced blocks found!" );

	if ( UnMallocedExecBlocks() )
		DebugPrintf( "Un-malloced Exec blocks found!" );

	if ( UnMallocedDDSurfBlocks() )
		DebugPrintf( "Un-malloced DDSurf blocks found!" );

	if ( UnMallocedSBufferBlocks() )
		DebugPrintf( "Un-malloced SBuffer blocks found!" );

#endif

	//
    return msg.wParam;

}


/****************************************************************************/
/*             D3DApp Initialization and callback functions                 */
/****************************************************************************/


static BOOL
AppInit(HINSTANCE hInstance, LPSTR lpCmdLine)
{

    WNDCLASS wc;
	DWORD dwPlatform, dwVersion;

	//
	QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
	LastTime = LargeTime;

	// check direct x version
	GetDXVersion( &dwVersion, &dwPlatform );
	if ( dwVersion < 0x600 )
	{
		DebugPrintf( "DirectX version less than 6\n" );
		Msg("You need to install Direct X 6 or later.");
		return FALSE;
	}

	// globals
    memset(&myglobs.rstate, 0, sizeof(myglobs.rstate));
    memset(&myglobs, 0, sizeof(myglobs));
    myglobs.bClearsOn		= FALSE;
    myglobs.bShowFrameRate	= TRUE;
    myglobs.bShowInfo		= FALSE;
    myglobs.hInstApp		= hInstance;

	// parse the command line
	if(!ParseCommandLine(lpCmdLine))
		return FALSE;

	// Register the window class
	wc.style			=	CS_HREDRAW |  // redraw window if horizontal width changes
							CS_VREDRAW;	  // redraw window if vertical width changes
    wc.lpfnWndProc		= WindowProc;								// processer for window messages
    wc.cbClsExtra		= 0;										//
	wc.cbWndExtra		= 0;										//
    wc.hInstance		= hInstance;								// window instance
    wc.hIcon			= LoadIcon( hInstance, "AppIcon");			// window icon
    wc.hCursor			= LoadCursor( NULL, IDC_ARROW );			// the cursor for mouse over window
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);		//
    wc.lpszClassName	= "ProjectX";								// class name

    if (!RegisterClass(&wc))
	{
		Msg("RegistrClass failed: %d", GetLastError());
		DebugPrintf("RegistrClass failed: %d", GetLastError());
        return FALSE;
	}
    
    // Create a window with some default settings that may change
	myglobs.hWndMain = CreateWindow(

         "ProjectX",  // window class name 
         "ProjectX",  // window title

	     WS_OVERLAPPED	| // title bar and a border
		 WS_THICKFRAME	| // add resizing border order window
		 WS_MINIMIZEBOX,  // add minimize, fullscreen, close widgets

         0, 0, // start position x,y

         START_WIN_SIZE_X, START_WIN_SIZE_Y, // start size

         NULL,       // parent window
         NULL,       // menu handle
         hInstance,  // program handle
         NULL		 // create parms

	);
	
	if( myglobs.hWndMain == NULL )
	{
		Msg("CreateWindow Failed: %d\n",GetLastError());
        DebugPrintf("CreateWindow Failed: %d\n",GetLastError());
        return FALSE;
    }

    // Display the window
    ShowWindow(myglobs.hWndMain, SW_SHOWNORMAL);

	// force a repaint ( probably just to show it right away )
	UpdateWindow(myglobs.hWndMain);
	
	//
	InitPolyText();

	// create the valid pickups global
	InitValidPickups();

#ifdef SCROLLING_MESSAGES

	// scrolling messages
	InitStatsMessages();

#endif

	// connect or setup registry
	InitRegistry();

    // starting screen
	MyGameStatus = STATUS_Title;

	// start the scene
	if (!InitScene())
		return FALSE;

	// initialize direct input
	if (!InitDInput())
	{
		DebugPrintf( "Failed on InitDInput()\n" );
		Msg("Failed to initialized Direct Input!");
		return FALSE;
	}

	// and extract game settings
	GetGamePrefs();

    // Call D3DApp to initialize all DD and D3D objects necessary to render.
    // D3DApp will call the device creation callback which will initialize the
    // viewport and the sample's execute buffers.
    if (!CreateD3DApp())
        return FALSE;

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

    LPSTR option = "";
	char cmdline[256];
	char buf[256];
	int size;

    //
    //  Set Global Defaults
    //
	
	DeviceOnCommandline			= FALSE;
    bOnlySystemMemory				= FALSE;
    bOnlyEmulation					= FALSE;
	PowerVR_Overide					= FALSE;
	Is3Dfx								= FALSE;
	Is3Dfx2								= FALSE;
	TriLinear								= TRUE;
	NoSFX								= FALSE;
	TextureMemory					= 0;
	MipMap								= TRUE;
	TripleBuffer							= FALSE;
	NoTextureScaling					= FALSE;
	DplayRecieveThread				= FALSE;
	PolygonText						= FALSE;
	DS										= FALSE;
	NoSplash							= TRUE;
	SessionGuidExists					= FALSE;
	UseSendAsync						= TRUE;
	bFullscreen							= TRUE;
	DPlayUpdateIntervalCmdLine	= 0;

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

	option = strtok(cmdline, " -+");

	// loop over every option
    while(option != NULL )
	{


		/***************************************************************\
		|
		|  Change Directory
		|  
		|    Description:  Changes to the given directory.
		|    Purpose:      When Running the exe outside the root folder
		|    Synatx:       -chdir <dir>
		|    Example:      -chdir c:\\Program Files\\ProjectX
		|    
		|    WARNING:	   Must be LAST option !!!
		|
		\***************************************************************/


		if (!_stricmp(option,"chdir"))
		{

			// get the pointer to "chdir" in the command line
			option = strstr(lpCmdLine,"chdir");

			// move pointer to the space after "-chdir"
			option = &option[ (strlen("chdir")+1) ];

			if (!option)
			{
				Msg("Error using chdir");
				break;
			}

			// change to root directory
			// the rest of the command line will be used as the path
			if( _chdir( option ) != 0 )

				// error
				Msg("Could not change to directory: %s", option);

			// dont loop anymore were done
			break;

		}

        else if (!_stricmp(option, "Debug"))
		{
            Debug = TRUE;
		}

		else if (!_stricmp(option,"NoFullScreen"))
		{
			bFullscreen = FALSE;
		}

		else if (!_stricmp(option,"DontColourKey")) 
		{
			DontColourKey = TRUE;
		}

		else if (!_stricmp(option, "RecordDemoToRam")) 
		{
			RecordDemoToRam = TRUE;
		}

		else if (!_stricmp(option, "systemmemory")) 
		{
			bOnlySystemMemory = TRUE;
        }

		else if (!_stricmp(option, "emulation")) 
		{
            bOnlyEmulation = TRUE;
        }

		else if (!_stricmp(option, "DS"))
		{
			DS = TRUE;
        } 

		else if (!_stricmp(option, "PowerVR"))
		{
			PowerVR_Overide = TRUE;
        }

		else if (!_stricmp(option, "3Dfx"))
		{
			Is3Dfx = TRUE;
        }

		else if (!_stricmp(option, "SendAsync"))
		{
			UseSendAsync = TRUE;
        }

		else if (!_stricmp(option, "3Dfx2"))
		{
			Is3Dfx2 = TRUE;
			TriLinear = TRUE;
        }

		else if (!_stricmp(option, "NoTriLinear")) 
		{
			TriLinear = FALSE;
        }

		else if (!_stricmp(option, "NoSFX"))
		{
			NoSFX = TRUE;
        }

		else if (!_stricmp(option, "PolyText"))
		{
			PolygonText = TRUE;
        }

		else if (!_stricmp(option, "DplayThread"))
		{
			DplayRecieveThread = TRUE;
        }

		else if (!_stricmp(option, "NoTextureScaling")) 
		{
			NoTextureScaling = TRUE;
        }

		else if (!_stricmp(option, "NoMipMap"))
		{
			MipMap = FALSE;
        }

		else if (!_stricmp(option, "TripleBuffer")) 
		{
			TripleBuffer = TRUE;
        }

		else if (!_stricmp(option, "AllWires")) 
		{
            AllWires = TRUE;
        }

		else if (!_stricmp(option, "NoDynamicSfx")) 
		{
            RemoveDynamicSfx();
        } 

		else if ( !_stricmp( option, "SetupSpaceOrb" ) )
		{
			SpaceOrbSetup = TRUE;
		} 

		else if ( !_stricmp( option, "NoBlitTextScaling" ) )
		{
			CanDoStrechBlt = FALSE;
		}

		else if ( !_stricmp( option, "NoCompoundSfxBuffer" ) )
		{
			NoCompoundSfxBuffer = TRUE;
		}

		else if ( !_stricmp( option, "AnyDPlayVersion" ) )
		{
			CheckDirectPlayVersion = FALSE;
		}

		else if ( !_stricmp( option, "QuickHost" ) ) 
		{
			NoSplash = TRUE;
			QuickStart = QUICKSTART_Start;
		}

		else if ( !_stricmp( option, "NoSplash" ) ) 
		{
			NoSplash = TRUE;
		}

		else if ( !_stricmp( option, "QuickJoin" ) ) 
		{
			NoSplash = TRUE;
			QuickStart = QUICKSTART_Join;
		}

		else if ( !_stricmp( option, "ForceHeartbeat" ) )
		{
			ForceHeartbeat = TRUE;	 
		}

		else if ( !_stricmp( option, "session" ) )
		{
	        option = strtok(NULL, "{}");
			sprintf( buf, "{%s}", option );
			if ( GUIDFromString( buf, &autojoin_session_guid ) == S_OK )
				SessionGuidExists = TRUE;
		}

		else if ( !_stricmp( option, "TCP" ) )
		{
			bTCP = TRUE;
	        option = strtok(NULL, " ");
			strcpy( (LPSTR)TCPAddress.text, option );
		}

		else if ( !_stricmp( option, "AutoStart" ) )
		{
			NoSplash = TRUE;
			QuickStart = QUICKSTART_SelectSession;
		}

#ifdef Z_TRICK
		else if ( !_stricmp( option, "NoZClear" ) )
		{
			ZClearsOn = FALSE;
		}
#endif

		// use sscanf
		else 
		{

			int num, denom;
			float fnum;
			DWORD mem;

			if ( sscanf( option, "dev%d", &num ) == 1 )
			{
				ddchosen3d = num;
				DeviceOnCommandline = TRUE;
			}

			else if ( sscanf( option , "pilot:%s", &config_name ))
			{
				//
			}

			else if ( sscanf( option, "CompoundSfxBufferMem%d", &mem ) == 1 )
			{
				UserTotalCompoundSfxBufferSize = mem;
				CustomCompoundBufferSize = TRUE;
			}

			else if ( sscanf( option, "PPS%d", &num ) == 1 )
			{
				DPlayUpdateIntervalCmdLine = num;
			}

			else if ( sscanf( option, "w%d", &num ) == 1 )
			{
				default_width = num;
			}

			else if ( sscanf( option, "h%d", &num ) == 1 )
			{
				default_height = num;
			}

			else if ( sscanf( option, "bpp%d", &num ) == 1 )
			{
				default_bpp = num;
			}

			else if ( sscanf( option, "pw%d", &num ) == 1 )
			{
				PreferredWidth = num;
			}

			else if ( sscanf( option, "ph%d", &num ) == 1 )
			{
				PreferredHeight = num;
			}

			else if ( sscanf( option, "TextureMemory%d", &num ) == 1 )
			{
				TextureMemory = num;
			}

			else if ( sscanf( option, "UVFix%f", &fnum ) == 1 )
			{
				UV_Fix = fnum;
			}

			else if ( sscanf( option, "fov%d", &num ) == 1 )
			{
				normal_fov = (float) num;
			}

			else if ( sscanf( option, "screen%d:%d", &num, &denom ) == 2 )
			{
				if ( num && denom )
					screen_aspect_ratio = (float) num / denom;
			}
        }

		// get the next token
        option = strtok(NULL, " -+");

    }

	return TRUE;
}

/*
 * CreateD3DApp
 * Create all DirectDraw and Direct3D objects necessary to begin rendering.
 * Add the list of D3D drivers to the file menu.
 */

static BOOL CreateD3DApp(void)
{

    DWORD flags;
    Defaults defaults;

	// setup the width and height
	if ( !default_width && !default_height && !default_bpp )
	{
		if ( ScreenWidth || ScreenHeight )
		{
			default_width = ScreenWidth;
			default_height = ScreenHeight;
		}
		else
		{
			default_width = PreferredWidth;
			default_height = PreferredHeight;
		}
		default_bpp = ScreenBPP;
	}


	// setup default bits per pixel
	if ( !default_bpp )
		default_bpp = 16;


	// Set the flags to pass to the D3DApp creation based on command line
    flags = ( (bOnlySystemMemory) ? D3DAPP_ONLYSYSTEMMEMORY : 0 ) | 
            ( (bOnlyEmulation)    ? (D3DAPP_ONLYD3DEMULATION |	D3DAPP_ONLYDDEMULATION) : 0 );

    /*
     * Create all the DirectDraw and D3D objects neccesary to render.  The
     * AfterDeviceCreated callback function is called by D3DApp to create the
     * viewport and the example's execute buffers.
     */

    if (!D3DAppCreateFromHWND(flags, myglobs.hWndMain, AfterDeviceCreated,
                              NULL, BeforeDeviceDestroyed, NULL, &d3dapp)) {
        ReportD3DAppError();
        return FALSE;
    }

	/*
     * Allow the sample to override the default render state and other
     * settings
     */

    if (!D3DAppGetRenderState(&defaults.rs)) {
        ReportD3DAppError();
        return FALSE;
    }

    strcpy(defaults.Name, "ProjectX");
    defaults.bTexturesDisabled = FALSE;
    defaults.bResizingDisabled = myglobs.bResizingDisabled;
    defaults.bClearsOn = myglobs.bClearsOn;

    myglobs.bClearsOn = defaults.bClearsOn;
    myglobs.bResizingDisabled = defaults.bResizingDisabled;

    /*
     * Apply any changes to the render state
     */

    memcpy(&myglobs.rstate, &defaults.rs, sizeof(D3DAppRenderState));
    if (!D3DAppSetRenderState(&myglobs.rstate)) {
        ReportD3DAppError();
        return FALSE;
    }


	SetWindowText(myglobs.hWndMain, defaults.Name);

	CancelAutoPlayMessage = RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));

	DebugPrintf( "CancelAutoPlayMessage=0x%08X\n", CancelAutoPlayMessage );

    return TRUE;
}

/*
 * AfterDeviceCreated
 * D3DApp will call this function immediately after the D3D device has been
 * created (or re-created).  D3DApp expects the D3D viewport to be created and
 * returned.  The sample's execute buffers are also created (or re-created)
 * here.
 */

BOOL SplashOnceOnly = TRUE;

static BOOL
AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lplpViewport, LPVOID lpContext)
{

	LPDIRECT3DVIEWPORT lpD3DViewport;
    HRESULT rval;

    // Create the D3D viewport object
    rval = d3dapp->lpD3D->CreateViewport(&lpD3DViewport, NULL);
    if (rval != D3D_OK) {
        Msg("Create D3D viewport failed.\n%s", D3DAppErrorToString(rval));
        CleanUpAndPostQuit();
        return FALSE;
    }

    // Add the viewport to the D3D device
    rval = d3dapp->lpD3DDevice->AddViewport(lpD3DViewport);
    if (rval != D3D_OK) {
        Msg("Add D3D viewport failed.\n%s", D3DAppErrorToString(rval));
        CleanUpAndPostQuit();
        return FALSE;
    }
    
    // Setup the viewport for a reasonable viewing area

    D3DVIEWPORT viewData;
    memset(&viewData, 0, sizeof(D3DVIEWPORT));

    viewData.dwSize = sizeof(D3DVIEWPORT);
    viewData.dwX = viewData.dwY = 0;
    viewData.dwWidth = w;
    viewData.dwHeight = h;
    viewData.dvScaleX = viewData.dwWidth / (float)2.0;
    viewData.dvScaleY = viewData.dwHeight / (float)2.0;
    viewData.dvMaxX = (float)D3DDivide(D3DVAL(viewData.dwWidth),
                                       D3DVAL(2 * viewData.dvScaleX));
    viewData.dvMaxY = (float)D3DDivide(D3DVAL(viewData.dwHeight),
                                       D3DVAL(2 * viewData.dvScaleY));

    rval = lpD3DViewport->SetViewport(&viewData);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
		SetViewportError( "AfterDeviceCreated", &viewData, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        CleanUpAndPostQuit();
        return FALSE;
    }

    // Return the viewport to D3DApp so it can use it
    *lplpViewport = lpD3DViewport;

	// load the view
	if (!InitView() )
	{
	    Msg("InitView failed.\n");
		CleanUpAndPostQuit();
        return FALSE;
	}

    return TRUE;
}

/*
 * BeforeDeviceDestroyed
 * D3DApp will call this function before the current D3D device is destroyed
 * to give the app the opportunity to destroy objects it has created with the
 * DD or D3D objects.
 */

static BOOL
BeforeDeviceDestroyed(LPVOID lpContext)
{
    // Release all objects (ie execute buffers) created by InitView
    ReleaseView();

    // Since we created the viewport it is our responsibility to release it
    d3dapp->lpD3DViewport->Release();

	//
    return TRUE;
}

// Render the next frame and update the window
static BOOL RenderLoop()
{

    // If all the DD and D3D objects have been initialized we can render
    if ( ! d3dapp->bRenderingIsOK)
		return TRUE;

    // Restore any lost surfaces
    if (!RestoreSurfaces())
	{
        // Restoring surfaces sometimes fails because the surfaces cannot
        // yet be restored.  If this is the case, the error will show up
        // somewhere else and we should return success here to prevent
        // unnecessary error's being reported.
        return TRUE;
    }

    // Call the sample's RenderScene to render this frame
    if (!RenderScene(d3dapp->lpD3DDevice, d3dapp->lpD3DViewport))
	{
        Msg("RenderScene failed.\n");
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
		if (
			(
				! PlayDemo ||
				( MyGameStatus != STATUS_PlayingDemo ) ||
				DemoShipInit[ Current_Camera_View ]
			) &&
			! PreventFlips
		)
		{
			if (!D3DAppShowBackBuffer(myglobs.bResized ? D3DAPP_SHOWALL : NULL))
			{
				Msg("!D3DAppShowBackBuffer");
				ReportD3DAppError();
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


//
// AppPause
// Pause and unpause the application
//

static BOOL
AppPause(BOOL f)
{
    /*
     * Flip to the GDI surface and halt rendering
     */
    if (!D3DAppPause(f))
        return FALSE;
	// toggle cursor clipping when pausing/unpausing
	cursorclipped = (d3dapp) ? !d3dapp->bPaused : 0;
	SetCursorClip();
    /*
     * When returning from a pause, reset the frame rate count
     */
    if (!f) {
		flush_input = TRUE; // and flush any mouse input that occurred while paused
    }
    return TRUE;
}

/*
 * RestoreSurfaces
 * Restores any lost surfaces.  Returns TRUE if all surfaces are not lost and
 * FALSE if one or more surfaces is lost and can not be restored at the
 * moment.
 */
static BOOL
RestoreSurfaces()
{
    /*
     * Have D3DApp check all the surfaces it's in charge of
     */
    if (!D3DAppCheckForLostSurfaces())
            return FALSE;
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
		// print error message
        ReportD3DAppError();

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

	//
	if( CancelAutoPlayMessage && (message == CancelAutoPlayMessage) )
	{
		// return 1 to cancel AutoPlay
		// return 0 to allow AutoPlay
		DebugPrintf( "AutoPlay cancelled\n" );
		return 1L;
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
void
CleanUpAndPostQuit(void)
{
	// unpause d3d
	AppPause( FALSE );

	// check if this function was ran allready
    if (myglobs.bQuit)
		return;

	// tell d3d to stop and report any errors
    if (!D3DAppDestroy())
        ReportD3DAppError();

	// release direct play copy
#ifdef MANUAL_SESSIONDESC_PROPAGATE
    if ( glpdpSD_copy )
    free ( glpdpSD_copy );
#endif
  
	// destroy the sound
	DestroySound( DESTROYSOUND_All );

	// destroy direct input
	TermDInput();
  
	// release the scene
	ReleaseScene();

	// set flag
    myglobs.bQuit = TRUE;

	// we dont control the cursor anymore
	cursorclipped = FALSE;

	//
	SetCursorClip();

	//
    PostQuitMessage( 0 );
}

/*
 * ReportD3DAppError
 * Reports an error during a d3d app call.
 */
void
ReportD3DAppError(void)
{
    Msg("%s", D3DAppLastErrorString());
}

/* Msg
 * Message output for error notification.
 */
void __cdecl
Msg( LPSTR fmt, ... )
{
    char buff[256];

#ifndef	WATCOM
    wvsprintf(&buff[0], fmt, (char *)(&fmt+1));
#endif
    lstrcat(buff, "\r\n");
    AppPause(TRUE);
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
    MessageBox( NULL, buff, "Forsaken", MB_OK );
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);

	AddCommentToLog( buff );

    AppPause(FALSE);
}


/* RetryMsg
 * Message output for error notification with option to retry or cancel
 */
int __cdecl
RetryMsg( LPSTR fmt, ... )
{
    char buff[256];
	int result;

#ifndef	WATCOM
    wvsprintf(&buff[0], fmt, (char *)(&fmt+1));
#endif
    lstrcat(buff, "\r\n");
    AppPause(TRUE);
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
    result = MessageBox( NULL, buff, "Forsaken Request", MB_RETRYCANCEL | MB_ICONEXCLAMATION | MB_SYSTEMMODAL );
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);

	AddCommentToLog( buff );

    AppPause(FALSE);

	return result != IDCANCEL;
}


extern "C"
int __cdecl MsgBox( int type, char *msg, ... )
{
	char txt[ 1024 ];
	va_list args;
	int res;

	va_start( args, msg );
	vsprintf( txt, msg, args);
	va_end( args );

    AppPause(TRUE);
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
    res = MessageBox( NULL, txt, "Forsaken", type );
    if (d3dapp && d3dapp->bFullscreen)
        SetWindowPos(myglobs.hWndMain, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
    AppPause(FALSE);

	return res;
}

