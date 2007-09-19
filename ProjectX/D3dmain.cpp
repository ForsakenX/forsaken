
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


#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
 int MakeCodeWritable( void );
/*-------------------------------------------------------------------------*/
#endif


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
#include	"splash.h"
#include	"XMem.h" 
#include	"d3dapp.h"

#ifdef SOFTWARE_ENABLE
	/*---------------------------------------------------------------------------
		Chris Walsh's code
	---------------------------------------------------------------------------*/
	extern CWmain(void);	// Main render loop on chris' stuff....
	extern	BOOL	SoftwareVersion;
	/*-------------------------------------------------------------------------*/
#endif

	extern LONGLONG  LargeTime;
	extern LONGLONG  LastTime;
	extern void SetupCharTransTable( void );
	extern void InitValidPickups();
	extern void InitPolyText( void );
	extern BOOL InitDInput(void);
	extern char * LogFilename;
	extern char * BatchFilename;
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
	extern int LogosEnable;
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
	extern BOOL NoAVI;
	extern BOOL CanDoStrechBlt;

	extern BOOL RecordDemoToRam;

	extern float normal_fov;
	extern float screen_aspect_ratio;
	extern	BOOL	CreateBatchFile;
	extern	BOOL	CreateLogFile;
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
	extern BOOL ServerChoosesGameType;
	extern BOOL ForceHeartbeat;

	extern GUID autojoin_session_guid;
	extern BOOL	SessionGuidExists;
	extern BOOL bTCP;
	
	int PreferredWidth, PreferredHeight;

	int DebugMathErrors( void );
	void OnceOnlyRelease( void );
	void DebugPrintf( const char * format, ... );

	void MovieRedraw (HWND);      // ID_MOVE_REDRAW
	void MovieStop (HWND);
	void MoviePlay (HWND hwnd);
	void AviFinished( void );
	void ShowSplashScreen( int num );
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

// AVI Specific stuff...

int AVI_bpp = 16;
int AVI_ZoomMode = 0;
HANDLE AVIThreadControlEvent;


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
    HACCEL hAccelApp;
    hPrevInstance;
	UINT param1;
	UINT param2;
    MSG msg;

	// sets the minimum amount of memory allocated from a single malloc.....
	//_amblksiz = 1024;

#ifdef DEBUG_ON

	XMem_Init();
	XExec_Init();
	DDSurf_Init();
	XSBuffer_Init();

#endif

	// fill status tab on window 
	FillStatusTab();

	// create a thread for AVI
	AVIThreadControlEvent = CreateEvent(NULL, FALSE, FALSE, NULL ); 
			
	// initialize COM library
	if FAILED( CoInitialize(NULL) )
		goto FAILURE;

    // Create the window and initialize all objects needed to begin rendering
    if(!AppInit(hInstance, lpCmdLine))
		goto FAILURE;

	// look for APPACCEL in D3dmain.rc
	// keystroke or combination of keystrokes
	// that generates a WM_COMMAND or WM_SYSCOMMAND
	// message for an application.
    hAccelApp = LoadAccelerators( hInstance , "AppAccel" );

	//if( ModeCase == -1 )
	//	D3DAppFullscreen(d3dapp->CurrMode);

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

		// dispatches incoming sent messages
		// checks the thread message queue for a posted message
		// and retrieves the message (if any exist).
		if (PeekMessage(&msg, NULL, param1, param2, PM_REMOVE))
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
				! myglobs.hWndMain ||  // 
				// processes accelerator keys for menu commands.
				// does not return until the window procedure has processed the message.
				! TranslateAccelerator( myglobs.hWndMain, hAccelApp, &msg )
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

	// debug messages

	DebugMathErrors();

	if ( UnMallocedBlocks() )
		DebugPrintf( "Un-malloced blocks found!" );

	if ( UnMallocedExecBlocks() )
		DebugPrintf( "Un-malloced Exec blocks found!" );

	if ( UnMallocedDDSurfBlocks() )
		DebugPrintf( "Un-malloced DDSurf blocks found!" );

	if ( UnMallocedSBufferBlocks() )
		DebugPrintf( "Un-malloced SBuffer blocks found!" );

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

	// refresh log files
	if ( Debug )
	{
	  DeleteFile( LogFilename   );
	  DeleteFile( BatchFilename );
	}

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
	wc.style			= CS_HREDRAW | CS_VREDRAW;					//
    wc.lpfnWndProc		= WindowProc;								// processer for window messages
    wc.cbClsExtra		= 0;										//
	wc.cbWndExtra		= 0;										//
    wc.hInstance		= hInstance;								// window instance
    wc.hIcon			= LoadIcon( hInstance, "AppIcon");			// window icon
    wc.hCursor			= LoadCursor( NULL, IDC_ARROW );			// the cursor for mouse over window
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);		//
    //wc.lpszMenuName		= "AppMenu";								// Menu & Accelorator Name
    wc.lpszClassName	= "ProjectX";								// class name

    if (!RegisterClass(&wc))
	{
        DebugPrintf("RegistryClass failed");
        return FALSE;
	}
    
    // Create a window with some default settings that may change
	if( ! (myglobs.hWndMain = CreateWindowEx(

         WS_EX_APPWINDOW,	//

         "ProjectX",  // window class name 

         "ProjectX",  // window title

	     WS_OVERLAPPED	| //
		 WS_CAPTION		| //
		 //WS_SYSMENU		| // put a menu on the window
		 WS_THICKFRAME	| // add resizing border order window
		 WS_MINIMIZEBOX,  // add minimize, fullscreen, close widgets

		 
         0, 0, // start position

         START_WIN_SIZE_X, START_WIN_SIZE_Y, // start size

         NULL,       // parent window
         NULL,       // menu handle
         hInstance,  // program handle
         NULL		 // create parms

	)))
	{
        DebugPrintf("CreateWindowEx failed");
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

	// character translation table
	SetupCharTransTable();


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
    bOnlySystemMemory			= FALSE;
    bOnlyEmulation				= FALSE;
	LogosEnable					= 0;
	PowerVR_Overide				= FALSE;
	Is3Dfx						= FALSE;
	Is3Dfx2						= FALSE;
	TriLinear					= TRUE;
	NoSFX						= FALSE;
	TextureMemory				= 0;
	MipMap						= TRUE;
	TripleBuffer				= FALSE;
	NoTextureScaling			= FALSE;
	DplayRecieveThread			= FALSE;
	PolygonText					= FALSE;
	DS							= FALSE;
	NoSplash					= TRUE;
	SessionGuidExists			= FALSE;
	UseSendAsync				= FALSE;
	bFullscreen					= TRUE;
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
#ifdef SOFTWARE_ENABLE
			SoftwareVersion = TRUE;
#endif
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

		else if (!_stricmp(option, "NoAVI")) 
		{
            NoAVI = TRUE;
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

#ifdef SOFTWARE_ENABLE
		else if ( !_stricmp( option, "UseDDrawFlip" ) )
		{
			UseDDrawFlip = TRUE;
			
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

			else if ( sscanf( option, "logos%d", &num ) == 1 )
			{
				LogosEnable = num;
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

    HMENU hmenu;
    int i;
    DWORD flags;
    Defaults defaults;


#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
	int	Writeable;
/*-------------------------------------------------------------------------*/
#endif


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


#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
	if( SoftwareVersion )
	{
		Writeable = MakeCodeWritable();	
		CWmain();
	}
/*-------------------------------------------------------------------------*/
#endif

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
     * Add the the list of display modes D3DApp found to the mode menu
     */

    hmenu = GetSubMenu(GetMenu(myglobs.hWndMain), 4);
    for (i = 0; i < d3dapp->NumModes; i++) {
        char ach[80];
        wsprintf(ach,"%dx%dx%d", d3dapp->Mode[i].w, d3dapp->Mode[i].h,
                 d3dapp->Mode[i].bpp);
        AppendMenu(hmenu, MF_STRING, MENU_FIRST_MODE+i, ach);
    }

    /*
     * Add the list of D3D drivers D3DApp foudn to the file menu
     */

    hmenu = GetSubMenu(GetSubMenu(GetMenu(myglobs.hWndMain), 0),6);
    for (i = 0; i < d3dapp->NumDrivers; i++) {
        InsertMenu(hmenu, 6 + i, MF_BYPOSITION | MF_STRING,
                   MENU_FIRST_DRIVER + i, d3dapp->Driver[i].Name);
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

    OverrideDefaults(&defaults);

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
    HMENU hmenu;
	LPDIRECT3DVIEWPORT lpD3DViewport;
    int i;
    char ach[20];
    HRESULT rval;

    /*
     * Create the D3D viewport object
     */
    rval = d3dapp->lpD3D->CreateViewport(&lpD3DViewport, NULL);
    if (rval != D3D_OK) {
        Msg("Create D3D viewport failed.\n%s", D3DAppErrorToString(rval));
        CleanUpAndPostQuit();
        return FALSE;
    }
    /*
     * Add the viewport to the D3D device
     */
    rval = d3dapp->lpD3DDevice->AddViewport(lpD3DViewport);
    if (rval != D3D_OK) {
        Msg("Add D3D viewport failed.\n%s", D3DAppErrorToString(rval));
        CleanUpAndPostQuit();
        return FALSE;
    }
    /*
     * Setup the viewport for a reasonable viewing area
     */
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
    /*
     * Return the viewport to D3DApp so it can use it
     */
    *lplpViewport = lpD3DViewport;

	// if showing splash screen, post-splash function is responsible for init view
	if ( SplashOnceOnly && !NoSplash )
	{
		SplashOnceOnly = FALSE;
		ShowSplashScreen( SPLASHSCREEN_Legal );
	}
	else
	{
		if (!InitView() )
		{
		    Msg("InitView failed.\n");
			CleanUpAndPostQuit();
	        return FALSE;
		}
	}

    /*
     * Add the list of texture formats found by D3DApp to the texture menu
     */
    hmenu = GetSubMenu(GetMenu(myglobs.hWndMain), 3);
    for (i = 0; i < d3dapp->NumTextureFormats; i++) {
        if (d3dapp->TextureFormat[i].bPalettized) {
            wsprintf(ach, "%d-bit Palettized",
                     d3dapp->TextureFormat[i].IndexBPP);
        } else {
            wsprintf(ach, "%d%d%d%d RGBA", d3dapp->TextureFormat[i].RedBPP,
                     d3dapp->TextureFormat[i].GreenBPP,
                     d3dapp->TextureFormat[i].BlueBPP,
                     d3dapp->TextureFormat[i].AlphaBPP);
        }
        AppendMenu(hmenu, MF_STRING, MENU_FIRST_FORMAT + i, ach);
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
    HMENU hmenu;
    int i;
    /*
     * Release all objects (ie execute buffers) created by InitView
     */
    ReleaseView();
    /*
     * Since we created the viewport it is our responsibility to release
     * it.  Use D3DApp's pointer to it since we didn't save one.
     */
    d3dapp->lpD3DViewport->Release();
    /*
     * Delete the list of texture formats from the texture menu because
     * they are likely to change
     */
    hmenu = GetSubMenu(GetMenu(myglobs.hWndMain), 3);
    for (i = 0; i < d3dapp->NumTextureFormats; i++) {
        DeleteMenu(hmenu, MENU_FIRST_FORMAT + i, MF_BYCOMMAND);
    }
    return TRUE;
}

/****************************************************************************/
/*                            Rendering loop                                */
/****************************************************************************/

// Render the next frame and update the window

static BOOL RenderLoop()
{

    // If all the DD and D3D objects have been initialized we can render

    if (d3dapp->bRenderingIsOK)
	{

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
			if (( !PlayDemo || !( MyGameStatus == STATUS_AttractMode || MyGameStatus == STATUS_PlayingDemo ) || DemoShipInit[ Current_Camera_View ] ) && !PreventFlips )
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
    }

    return TRUE;
}

/*
 * AppPause
 * Pause and unpause the application
 */
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
 * AppAbout
 * About box message handler
 */

BOOL
FAR PASCAL AppAbout(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK)
        EndDialog(hwnd, TRUE);
      break;

    case WM_INITDIALOG:
      return TRUE;
  }
  return FALSE;
}

/*
 * WindowProc
 * Main window message handler.
 */

long
FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam,
                           LPARAM lParam )
{
    int i;
    BOOL bStop;
    LRESULT lresult;

    /*
     * Give D3DApp an opportunity to process any messages it MUST see in order
     * to perform it's function.
     */

    if (!D3DAppWindowProc(&bStop, &lresult, hWnd, message, wParam, lParam)) {
        ReportD3DAppError();
        CleanUpAndPostQuit();
        return 0;
    }

    /* 
     * If bStop is set by D3DApp, the app should not process the message but
     * return lresult.
     */

    if (bStop)
        return lresult;

	if ( quitting )
	{
		quitting = FALSE;
		DebugPrintf("about to CleanUpAndPostQuit ( from WindowProc )\n");
		CleanUpAndPostQuit();
	}

	if( CancelAutoPlayMessage && message == CancelAutoPlayMessage )
	{
		// return 1 to cancel AutoPlay
		// return 0 to allow AutoPlay
		DebugPrintf( "AutoPlay cancelled\n" );
		return 1L;
	}

    switch( message ) {

		// some i/o device has changed state
		case WM_DEVICECHANGE:

			DebugPrintf( "DeviceChange detected\n" );
			switch( wParam )
			{
			case DBT_DEVICEARRIVAL:
				DebugPrintf( "ARRIVAL\n" );
				break;
			case DBT_DEVICEQUERYREMOVE:
				DebugPrintf( "QUERYREMOVE\n" );
				break;
			case DBT_DEVICEQUERYREMOVEFAILED:
				DebugPrintf( "QUERYREMOVEFAILED\n" );
				break;
			case DBT_DEVICEREMOVECOMPLETE:
				DebugPrintf( "REMOVECOMPLETE\n" );
				break;
			case DBT_DEVICEREMOVEPENDING:
				DebugPrintf( "REMOVEPENDING\n" );
				break;
			case DBT_DEVICETYPESPECIFIC:
				DebugPrintf( "TYPESPECIFIC\n" );
				break;
			}

		break;

        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:

			if (GetCapture() != NULL )
			{
				SetCapture(myglobs.hWndMain);
			}
			if ( message != WM_MOUSEMOVE && d3dapp->bPaused )
			{
				AppPause( FALSE );
			}
			return 1;
            break;

		case WM_SYSKEYUP:
        case WM_KEYUP:
			if ( !d3dapp->bPaused )
			{
				i = My_Key_Up( wParam,lParam);
				if ( !i )
					break;
				if ( i < 0 )
					CleanUpAndPostQuit();
				if ( i == 2 )
				{
					AppPause( !d3dapp->bPaused );
				}
				return 1;
			}
			break;

		case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
//			if( MoviePlaying )
			if ( 0 )
			{
					switch( wParam )
					{
						case VK_ESCAPE:
							{

								CleanUpAndPostQuit();
								return 1;
							}
					}
			}else{

				if ( !d3dapp->bPaused )
				{
					My_Key_Down( wParam,lParam);
					return 1;
				}
			}
			break;
		case WM_SYSCHAR:
		case WM_SYSDEADCHAR:
			if ( !d3dapp->bPaused )
			{
				return 1; // ignore menu mnemonic (ALT + key) keypresses
			}
			break;
		case WM_MENUCHAR:
			if ( d3dapp->bPaused )
			{
				AppPause( FALSE ); // unpause if sent invalid menu hotkey
				return 1;
			}
			break;
        /*
         * Pause and unpause the app when entering/leaving the menu
         */
        case WM_ENTERMENULOOP:
            AppPause(TRUE);
            break;
        case WM_EXITMENULOOP:
            AppPause(FALSE);
            break;
		// unpause app when window resized or moved
		case WM_WINDOWPOSCHANGED:
			if ( d3dapp && !d3dapp->bFullscreen )
				AppPause(FALSE);
			break;
        case WM_DESTROY:
            myglobs.hWndMain = NULL;
            CleanUpAndPostQuit();
            break;
        case WM_INITMENUPOPUP:
            /*
             * Check and enable the appropriate menu items
             */
            if (d3dapp->ThisDriver.bDoesZBuffer) {
                EnableMenuItem((HMENU)wParam, MENU_ZBUFFER, MF_ENABLED);
                CheckMenuItem((HMENU)wParam, MENU_ZBUFFER, myglobs.rstate.bZBufferOn ? MF_CHECKED : MF_UNCHECKED);
            } else {
                EnableMenuItem((HMENU)wParam, MENU_ZBUFFER, MF_GRAYED);
                CheckMenuItem((HMENU)wParam, MENU_ZBUFFER, MF_UNCHECKED);
            }
            CheckMenuItem((HMENU)wParam, MENU_STEP, (myglobs.bSingleStepMode) ? MF_CHECKED : MF_UNCHECKED);
            EnableMenuItem((HMENU)wParam, MENU_GO, (myglobs.bSingleStepMode) ? MF_ENABLED : MF_GRAYED);
            CheckMenuItem((HMENU)wParam, MENU_FLAT, (myglobs.rstate.ShadeMode == D3DSHADE_FLAT) ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_GOURAUD, (myglobs.rstate.ShadeMode == D3DSHADE_GOURAUD) ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_PHONG, (myglobs.rstate.ShadeMode == D3DSHADE_PHONG) ? MF_CHECKED : MF_UNCHECKED);
            EnableMenuItem((HMENU)wParam, MENU_PHONG, MF_GRAYED);
            CheckMenuItem((HMENU)wParam, MENU_CLEARS, myglobs.bClearsOn ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_POINT, (myglobs.rstate.FillMode == D3DFILL_POINT) ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_WIREFRAME, (myglobs.rstate.FillMode == D3DFILL_WIREFRAME) ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_SOLID, (myglobs.rstate.FillMode == D3DFILL_SOLID) ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_DITHERING, myglobs.rstate.bDithering ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_SPECULAR, myglobs.rstate.bSpecular ? MF_CHECKED : MF_UNCHECKED);
            EnableMenuItem((HMENU)wParam, MENU_SPECULAR, MF_ENABLED);
            CheckMenuItem((HMENU)wParam, MENU_FOG, myglobs.rstate.bFogEnabled ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_ANTIALIAS, myglobs.rstate.bAntialiasing ? MF_CHECKED : MF_UNCHECKED);
            if (d3dapp->ThisDriver.bDoesTextures) {
                CheckMenuItem((HMENU)wParam, MENU_TEXTURE_TOGGLE, (!d3dapp->bTexturesDisabled) ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_TEXTURE_TOGGLE, MF_ENABLED);
                EnableMenuItem((HMENU)wParam, MENU_TEXTURE_SWAP, (d3dapp->bTexturesDisabled) ? MF_GRAYED : MF_ENABLED);
                CheckMenuItem((HMENU)wParam, MENU_PERSPCORRECT, myglobs.rstate.bPerspCorrect ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_PERSPCORRECT, (d3dapp->bTexturesDisabled) ? MF_GRAYED : MF_ENABLED);
                CheckMenuItem((HMENU)wParam, MENU_POINT_FILTER, (myglobs.rstate.TextureFilter == D3DFILTER_NEAREST) ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_POINT_FILTER, (d3dapp->bTexturesDisabled) ? MF_GRAYED : MF_ENABLED);
                CheckMenuItem((HMENU)wParam, MENU_LINEAR_FILTER, (myglobs.rstate.TextureFilter == D3DFILTER_LINEAR) ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_LINEAR_FILTER, (d3dapp->bTexturesDisabled) ? MF_GRAYED : MF_ENABLED);
                for (i = 0; i < d3dapp->NumTextureFormats; i++) {
                    CheckMenuItem((HMENU)wParam, MENU_FIRST_FORMAT + i, (i == d3dapp->CurrTextureFormat) ? MF_CHECKED : MF_UNCHECKED);
                    EnableMenuItem((HMENU)wParam, MENU_FIRST_FORMAT + i, (d3dapp->bTexturesDisabled) ? MF_GRAYED : MF_ENABLED);
                }
            } else {
                EnableMenuItem((HMENU)wParam, MENU_TEXTURE_SWAP, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_TEXTURE_TOGGLE, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_POINT_FILTER, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_LINEAR_FILTER, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_PERSPCORRECT, MF_GRAYED);
            }
            if (d3dapp->bIsPrimary) {
                CheckMenuItem((HMENU)wParam, MENU_FULLSCREEN, d3dapp->bFullscreen ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_FULLSCREEN, d3dapp->bFullscreen && !d3dapp->ThisDriver.bCanDoWindow ? MF_GRAYED : MF_ENABLED);
                EnableMenuItem((HMENU)wParam, MENU_NEXT_MODE, (!d3dapp->bFullscreen) ? MF_GRAYED : MF_ENABLED);
                EnableMenuItem((HMENU)wParam, MENU_PREVIOUS_MODE, (!d3dapp->bFullscreen) ? MF_GRAYED : MF_ENABLED);
            } else {
                EnableMenuItem((HMENU)wParam, MENU_FULLSCREEN, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_NEXT_MODE, MF_GRAYED);
                EnableMenuItem((HMENU)wParam, MENU_PREVIOUS_MODE, MF_GRAYED);
            }
            for (i = 0; i < d3dapp->NumModes; i++) {
                CheckMenuItem((HMENU)wParam, MENU_FIRST_MODE + i, (i == d3dapp->CurrMode) ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem((HMENU)wParam, MENU_FIRST_MODE + i, (d3dapp->Mode[i].bThisDriverCanDo) ? MF_ENABLED : MF_GRAYED);
            }
            for (i = 0; i < d3dapp->NumDrivers; i++) {
                CheckMenuItem((HMENU)wParam, MENU_FIRST_DRIVER + i, (i == d3dapp->CurrDriver) ? MF_CHECKED : MF_UNCHECKED);
            }
            for (i = 0; i < NumLevels; i++) {
                CheckMenuItem((HMENU)wParam, MENU_FIRST_LEVEL + i, (i == NewLevelNum) ? MF_CHECKED : MF_UNCHECKED);
            }
			// BikeDetail Level Setting...
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL6, BikeDetail == 0  ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL5, BikeDetail == 1  ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL4, BikeDetail == 2  ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL3, BikeDetail == 3  ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL2, BikeDetail == 4  ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_BIKEDETAIL1, BikeDetail == 5  ? MF_CHECKED : MF_UNCHECKED);

            CheckMenuItem((HMENU)wParam, MENU_PLIGHTDETAIL1, PrimaryLightDetail ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_PLIGHTDETAIL2, !PrimaryLightDetail ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_SLIGHTDETAIL1, SecondaryLightDetail ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_SLIGHTDETAIL2, !SecondaryLightDetail ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_PKLIGHTDETAIL1, PickupLightDetail ? MF_CHECKED : MF_UNCHECKED);
            CheckMenuItem((HMENU)wParam, MENU_PKLIGHTDETAIL2, !PickupLightDetail ? MF_CHECKED : MF_UNCHECKED);
            break;
        case WM_GETMINMAXINFO:
            /*
             * Some samples don't like being resized, such as those which use
             * screen coordinates (TLVERTEXs).
             */
            if (myglobs.bResizingDisabled) {
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = START_WIN_SIZE_X;
                ((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = START_WIN_SIZE_Y;
                ((LPMINMAXINFO)lParam)->ptMinTrackSize.x = START_WIN_SIZE_X;
                ((LPMINMAXINFO)lParam)->ptMinTrackSize.y = START_WIN_SIZE_Y;
                return 0;
            }
            break;
		case MM_MCINOTIFY:
			DebugPrintf("MCINOTIFY message %d\n",wParam);
			break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case MENU_ABOUT:
                    AppPause(TRUE);
                    DialogBox(myglobs.hInstApp, "AppAbout", myglobs.hWndMain, (DLGPROC)AppAbout);
                    AppPause(FALSE);
                    break;
                case MENU_EXIT:
                    CleanUpAndPostQuit();
                    break;
                case MENU_STEP:
                    /*
                     * Begin single step more or draw a frame if in single
                     * step mode
                     */
                    if (!myglobs.bSingleStepMode) {
                        myglobs.bSingleStepMode = TRUE;
                        myglobs.bDrawAFrame = TRUE;
                    } else if (!myglobs.bDrawAFrame) {
                        myglobs.bDrawAFrame = TRUE;
                    }
                    break;
                case MENU_GO:
                    /*
                     * Exit single step mode
                     */
                    if (myglobs.bSingleStepMode) {
                        myglobs.bSingleStepMode = FALSE;
                    }
                    break;
                /*
                * Bike Detail Selection..
                */
                case MENU_BIKEDETAIL6:
					BikeDetail = 0;
                    break;
                case MENU_BIKEDETAIL5:
					BikeDetail = 1;
                    break;
                case MENU_BIKEDETAIL4:
					BikeDetail = 2;
                    break;
                case MENU_BIKEDETAIL3:
					BikeDetail = 3;
                    break;
                case MENU_BIKEDETAIL2:
					BikeDetail = 4;
                    break;
                case MENU_BIKEDETAIL1:
					BikeDetail = 5;
                    break;

                /*
                * Lights Detail Selection..
                */
                case MENU_PLIGHTDETAIL1:
					EnablePrimaryLights();
					PrimaryLightDetail = TRUE;
                    break;
                case MENU_PLIGHTDETAIL2:
					DisablePrimaryLights();
					PrimaryLightDetail = FALSE;
                    break;
                case MENU_SLIGHTDETAIL1:
					EnableSecondaryLights();
					SecondaryLightDetail = TRUE;
                    break;
                case MENU_SLIGHTDETAIL2:
					DisableSecondaryLights();
					SecondaryLightDetail = FALSE;
                    break;
                case MENU_PKLIGHTDETAIL1:
					EnablePickupLights();
					PickupLightDetail = TRUE;
                    break;
                case MENU_PKLIGHTDETAIL2:
					DisablePickupLights();
					PickupLightDetail = FALSE;
                    break;


                case MENU_STATS:
                    /*
                     * Toggle output of frame rate and window info
                     */
                    if ((myglobs.bShowFrameRate) && (myglobs.bShowInfo)) {
                        myglobs.bShowFrameRate = FALSE;
                        myglobs.bShowInfo = FALSE;
                        break;
                    }
                    if ((!myglobs.bShowFrameRate) && (!myglobs.bShowInfo)) {
                        myglobs.bShowFrameRate = TRUE;
                        break;
                    }
                    myglobs.bShowInfo = TRUE;
                    break;
                case MENU_FULLSCREEN:
                    if (d3dapp->bFullscreen) {
                        /*
                         * Return to a windowed mode.  Let D3DApp decide which
                         * D3D driver to use in case this one cannot render to
                         * the Windows display depth
                         */
                        if (!D3DAppWindow(D3DAPP_YOUDECIDE, D3DAPP_YOUDECIDE)) {
                            ReportD3DAppError();
                            CleanUpAndPostQuit();
                            break;
                        }
                    } else {
                        /*
                         * Enter the current fullscreen mode.  D3DApp may
                         * resort to another mode if this driver cannot do
                         * the currently selected mode.
                         */
                        if (!D3DAppFullscreen(d3dapp->CurrMode)) {
                            ReportD3DAppError();
                            CleanUpAndPostQuit();
                            break;
                        }
                    }
                    break;
                /*
                 * Texture filter method selection
                 */
                case MENU_POINT_FILTER:
                    if (myglobs.rstate.TextureFilter == D3DFILTER_NEAREST)
                        break;
                    myglobs.rstate.TextureFilter = D3DFILTER_NEAREST;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_LINEAR_FILTER:
                    if (myglobs.rstate.TextureFilter == D3DFILTER_LINEAR)
                        break;
                    myglobs.rstate.TextureFilter = D3DFILTER_LINEAR;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                /* 
                 * Shading selection
                 */
                case MENU_FLAT:
                    if (myglobs.rstate.ShadeMode == D3DSHADE_FLAT)
                        break;
                    myglobs.rstate.ShadeMode = D3DSHADE_FLAT;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_GOURAUD:
                    if (myglobs.rstate.ShadeMode == D3DSHADE_GOURAUD)
                        break;
                    myglobs.rstate.ShadeMode = D3DSHADE_GOURAUD;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_PHONG:
                    if (myglobs.rstate.ShadeMode == D3DSHADE_PHONG)
                        break;
                    myglobs.rstate.ShadeMode = D3DSHADE_PHONG;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                /*
                 * Fill mode selection
                 */
                case MENU_POINT:
                    if (myglobs.rstate.FillMode == D3DFILL_POINT)
                        break;
                    myglobs.rstate.FillMode = D3DFILL_POINT;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_WIREFRAME:
                    if (myglobs.rstate.FillMode == D3DFILL_WIREFRAME)
                        break;
                    myglobs.rstate.FillMode = D3DFILL_WIREFRAME;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_SOLID:
                    if (myglobs.rstate.FillMode == D3DFILL_SOLID)
                        break;
                    myglobs.rstate.FillMode = D3DFILL_SOLID;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_PERSPCORRECT:
                    /*
                     * Toggle perspective correction
                     */
                    myglobs.rstate.bPerspCorrect =
                        !myglobs.rstate.bPerspCorrect;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_CLEARS:
                    /*
                     * Toggle the clearing the the back buffer and Z-buffer
                     * and set the resized flag to clear the entire window
                     */
                    myglobs.bClearsOn = !myglobs.bClearsOn;
                    if (myglobs.bClearsOn)
                        myglobs.bResized = TRUE;
                    break;
                case MENU_ZBUFFER:
                    /*
                     * Toggle the use of a Z-buffer
                     */
                    myglobs.rstate.bZBufferOn = !myglobs.rstate.bZBufferOn;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_DITHERING:
                    /*
                     * Toggle dithering
                     */
                    myglobs.rstate.bDithering = !myglobs.rstate.bDithering;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_SPECULAR:
                    /*
                     * Toggle specular highlights
                     */
                    myglobs.rstate.bSpecular = !myglobs.rstate.bSpecular;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_FOG:
                    /*
                     * Toggle fog
                     */
                    myglobs.rstate.bFogEnabled = !myglobs.rstate.bFogEnabled;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_ANTIALIAS:
                    /*
                     * Toggle anti-aliasing
                     */
                    myglobs.rstate.bAntialiasing =
                        !myglobs.rstate.bAntialiasing;
                    if (!D3DAppSetRenderState(&myglobs.rstate)) {
                        ReportD3DAppError();
                        break;
                    }
                    break;
                case MENU_TEXTURE_TOGGLE:
                    /*
                     * Release the sample's execute buffers, toggle the
                     * texture disabled state and recreate them.
                     */
                    break;
                case MENU_TEXTURE_SWAP:
                    /*
                     * Swap textures using the load command
                     */
					break;
                    /*
                     * Just in case we have a texture background
                     */
                    myglobs.bResized = TRUE;
                    break;
                case MENU_NEXT_MODE:
                    /*
                     * Enter the next usable fullscreen mode
                     */
                    i = d3dapp->CurrMode;
                    do {
                        ++i;
                        if (i >= d3dapp->NumModes)
                            i = 0;
                        if (!d3dapp->Mode[i].bThisDriverCanDo)
                            continue;
                        else {
                            if (!D3DAppFullscreen(i)) {
                                ReportD3DAppError();
                                CleanUpAndPostQuit();
                            }
                            break;
                        }
                    } while(i != d3dapp->CurrMode);
                    break;
                case MENU_PREVIOUS_MODE:
                    /*
                     * Enter the previous usable fullscreen mode
                     */
                    i = d3dapp->CurrMode;
                    do {
                        --i;
                        if (i < 0)
                            i = d3dapp->NumModes - 1;
                        if (!d3dapp->Mode[i].bThisDriverCanDo)
                            continue;
                        else {
                            if (!D3DAppFullscreen(i)) {
                                ReportD3DAppError();
                                CleanUpAndPostQuit();
                            }
                            break;
                        }
                    } while(i != d3dapp->CurrMode);
                    break;
/*
				case ID_MOVIE_REDRAW:
					MovieRedraw (myglobs.hWndMain);
					break;
				case ID_MOVIE_STOP:
					AviFinished();
					break;
				case ID_MOVIE_PLAY:
					MoviePlay (myglobs.hWndMain);
					break;
*/
            }
            if (   LOWORD(wParam) >= MENU_FIRST_FORMAT
                && LOWORD(wParam) < MENU_FIRST_FORMAT +
                    D3DAPP_MAXTEXTUREFORMATS
                && d3dapp->CurrTextureFormat !=
                    LOWORD(wParam) - MENU_FIRST_FORMAT) {
                /*
                 * Release the sample's execute buffers, change the texture
                 * format and recreate the view.
                 */
                ReleaseView();
                if (!D3DAppChangeTextureFormat(LOWORD(wParam) -
                                               MENU_FIRST_FORMAT)) {
                    ReportD3DAppError();
                    CleanUpAndPostQuit();
                }
                {
                    if (!InitView()) {
                        Msg("InitView failed.\n");
                        CleanUpAndPostQuit();
                        break;
                    }
                }
            }
            if (   LOWORD(wParam) >= MENU_FIRST_DRIVER
                && LOWORD(wParam) < MENU_FIRST_DRIVER + D3DAPP_MAXD3DDRIVERS
                && d3dapp->CurrDriver != LOWORD(wParam) - MENU_FIRST_DRIVER) {
                /*
                 * Change the D3D driver
                 */
                if (!D3DAppChangeDriver(LOWORD(wParam) - MENU_FIRST_DRIVER,
                                        NULL)) {
                    ReportD3DAppError();
                    CleanUpAndPostQuit();
                }
            }

            if (   LOWORD(wParam) >= MENU_FIRST_LEVEL
                && LOWORD(wParam) < MENU_FIRST_LEVEL + 16
				&& NewLevelNum == LevelNum
                && NewLevelNum != LOWORD(wParam) - MENU_FIRST_LEVEL )
			{
				//	Change the Level
				NewLevelNum = LOWORD(wParam) - MENU_FIRST_LEVEL;
            }

            if (   LOWORD(wParam) >= MENU_FIRST_MODE
                && LOWORD(wParam) < MENU_FIRST_MODE+100) {
                /*
                 * Switch to the selected fullscreen mode
                 */
                if (!D3DAppFullscreen(LOWORD(wParam) - MENU_FIRST_MODE)) {
                    ReportD3DAppError();
                    CleanUpAndPostQuit();
                }
            }
            /*
             * Whenever we receive a command in single step mode, draw a frame
             */
            if (myglobs.bSingleStepMode)
                myglobs.bDrawAFrame = TRUE;
			
			// automatic unpause after every command (except when exiting)
			if ( !myglobs.bQuit )
			{
				AppPause( FALSE );
			}

            return 0L;
    }
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
	AppPause( FALSE );
    if (myglobs.bQuit)
	{
     	DebugPrintf("myglobs.bQuit was TRUE\n");
		return;
	}
    if (!D3DAppDestroy())
        ReportD3DAppError();

	OnceOnlyRelease();
  
	ReleaseScene();
    myglobs.bQuit = TRUE;
	cursorclipped = FALSE;
	SetCursorClip();
    PostQuitMessage( 0 );

    DebugPrintf("done CleanUpAndPostQuit\n");
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

#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/

  int MakeCodeWritable( void )
  {
        int ReturnValue  = 0;

        HMODULE OurModule = GetModuleHandle(0);
        BYTE *pBaseOfImage = 0;

        if((GetVersion() & 0xC0000000) == 0x80000000)
        {
        // We're on Win32s, so get the real pointer
        HMODULE Win32sKernel = GetModuleHandle("W32SKRNL.DLL");

        typedef DWORD __stdcall translator( DWORD );
        translator *pImteFromHModule =
                (translator
*)GetProcAddress(Win32sKernel,"_ImteFromHModule@4");
        translator *pBaseAddrFromImte =
                (translator
*)GetProcAddress(Win32sKernel,"_BaseAddrFromImte@4");

        if(pImteFromHModule && pBaseAddrFromImte)
        {
                DWORD Imte = (*pImteFromHModule)((DWORD)OurModule);
                pBaseOfImage = (BYTE *)(*pBaseAddrFromImte)(Imte);
        }
        }
        else
        {
        pBaseOfImage = (BYTE *)OurModule;
        }

        if(pBaseOfImage)
        {
        IMAGE_OPTIONAL_HEADER *pHeader = (IMAGE_OPTIONAL_HEADER *)
                (pBaseOfImage + ((IMAGE_DOS_HEADER
*)pBaseOfImage)->e_lfanew +
                sizeof(IMAGE_NT_SIGNATURE) + sizeof(IMAGE_FILE_HEADER));

        DWORD OldRights;

       
if(VirtualProtect(pBaseOfImage+pHeader->BaseOfCode,pHeader->SizeOfCode,
                        PAGE_READWRITE,&OldRights))
        {
                ReturnValue = 1;
        }
        }

        return ReturnValue;
   }
#endif

/*************************************************************************
AVI Stuff
*************************************************************************/

// error: end proces
//
void EndProcess (int errcode)
{
	if (errcode == ERROR_NOT_ENOUGH_MEMORY)
	{
		//MessageBox (ghWnd, "Error: Not enough memory", "Error",
		//            MB_APPLMODAL|MB_ICONSTOP|MB_OK);
		Msg("AVI error: Not enough memory");
	}
	Msg("AVI error");
	ExitProcess (errcode);
}

