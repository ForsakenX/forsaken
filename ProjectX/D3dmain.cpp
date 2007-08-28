/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 177 $
 *
 * $Header: /PcProjectX/D3dmain.cpp 177   11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/D3dmain.cpp $
//
//177   11/11/98 16:00 Philipy
//various fixes for warnings / errors when compiling under VC6
//
//171   3/09/98 14:25 Philipy
//tidied up window mode slightly
//( no resize allowed, removed menu bar )
//
//170   3/09/98 9:31 Philipy
//somw Gamespy fixes
//added Session ( GUID ) and TCP command line flags
//added TRACKER_NAME facility
//
//169   27/08/98 20:12 Philipy
//manual / auto server mode now selectable from menus
//text now displayed when server in game & not rendering
//dynamic PPS setting re-enabled for server
//
//168   25/08/98 17:38 Philipy
//added gamespy support
//tracker config now selectable from start server game menu, & stored in
//reg
//
//166   17/08/98 17:13 Philipy
//added -ServerChoosesGame & ServerAutoStart command line options
//
//165   14/08/98 15:25 Philipy
//added trilinear option to menus
//fixed level name / shutdown packet in heartbeat
//
//
//163   10/08/98 17:33 Philipy
//rewrote AVI player
//
//162   5/08/98 11:04 Philipy
//added AutoStart facility ( joins game based on GUID in registery )
//upped patch version to 1.02
//
//161   9/07/98 12:43 Philipy
//few minor fixes for patch release
//
//160   8/07/98 11:42 Philipy
//re-enabled -NoSplash command line option
//
//159   7/07/98 18:05 Philipy
//added lobby autostart code ( when all players have recieved init msg )
//added num primary weapons menu option ( propergated to other players &|
//server )
//extracted new title text for localisation
//
//158   3/07/98 17:46 Philipy
//added quit option when using quickstart
//
//157   3/07/98 11:53 Philipy
//heartbeat & quickstart stuff
//
//151   8/04/98 20:47 Philipy
//title text messages now properly initialised
//holo-glaire removed for sw version
//compound buffer size can now be set in command line and opt file
//prevented "level select disabled" from appearing at start of
//multiplayer game
//
//150   8/04/98 10:15 Philipy
//Title CD track now only started after valid CD check
//
//149   7/04/98 11:00 Philipy
//potentially fixed crash when going from AVI to titles
//fixed CD audio looping
//no CD audio in front end unless full install
//bike features sliders now give correct values
//
//148   4/04/98 14:22 Philipy
//mode scaling stuff is now calculated rather than based on fixed values
//added -NoBlitTextScaling option to ReadIni and command line options
//
//146   3/04/98 19:15 Philipy
//blitted text now scales properly
//added -NoBlitTextScaling flag for cards that can't do a strech blit
//properly ( Riva )
//
//139   28/03/98 17:33 Philipy
//corrected some sfx
//added legal screen
//fixed mission briefing text bug
//
//135   15/03/98 18:40 Philipy
//added water effect splash screen
//fixed bug with end game sequence
//implemented attract mode
//text macros now saved in config
//
//131   7/03/98 15:25 Philipy
//re-implemented -NoAVI option
//
//129   6/03/98 16:51 Philipy
//
//126   20/02/98 15:28 Philipy
//re-implented AVI
//splash screens can now play demos and AVIs
//
//125   9/02/98 12:21 Philipy
//added sound buffer memory managment
//only one piece of bike computer speech can now play at a time
//
//122   1/27/98 12:07p Philipy
//fixed mode changing bug
//
//121   26/01/98 18:23 Philipy
//fixed video memory leaks
//splash screens now display after release view, and call InitScene,
//InitView after  completion
//
//120   24/01/98 17:38 Philipy
//fixed multiplayer join-quit-join bug
//fixed attract mode loading wrong level
//
//118   21/01/98 12:19 Philipy
//Added attract mode for shareware
//fixed looping sfx volume bug
//
//117   18/01/98 23:36 Philipy
//added -NoDynamicSfx option
//
//113   13/01/98 12:05 Philipy
//put ifdefs around initial splash screen for shareware
//
//112   12/01/98 0:08 Philipy
//bug fixes
//added inter-level mission briefing
//changed splash screen code, + added splash screen on exit
//
//110   9/01/98 11:13 Philipy
//CD nows plays last track
//CD now replays current track from seperate  ( low priority ) thread -
//but still causes pause
//loading bar now displayed when loading
//
//95    2/12/97 11:52 Philipy
//boot demo stuff
//
//91    26/11/97 11:48 Philipy
//implemented dynamic loading of SFX, dynamic allocation of mixing
//channels.
//3D sound currently disabled.
//
//85    4/11/97 16:26 Philipy
//AVI now plays for stats screens
//implemented scrolling messages (not enabled)
//
//84    27/10/97 15:38 Philipy
//added in-game cd track playing
//
//83    23/10/97 16:49 Philipy
//added tggle (number key 1) for playing AVI on texture.
//(no texture conversion yet, could appear corrupted)
//
//80    21/10/97 13:08 Philipy
//AVI now no longer sends main game thread to sleep
//WindowProc now handles CD track finishing
 * 
 */

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

#include "typedefs.h"
#include "demo_id.h"

#include "resource.h"
#include "d3dmain.h"
#include "main.h"
#include "dsound.h"
#ifdef PCIDENT
#include "finger.h"
#define PCIDENT_FILE "data\\offsets.dat"
#endif
#include "dbt.h"

#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
 int MakeCodeWritable( void );
/*-------------------------------------------------------------------------*/
#endif

extern "C" {

#include	"title.h"
//#include	"cplay.h"
#include	"stdwin.h"
#include	"cdaudio.h" 
#include	"malloc.h"
#include	"Exechand.h" 
#include	"DDSurfhand.h" 
#include	"SBufferHand.h" 
#include	"Readini.h" 
#include	"file.h" 
#include	"splash.h"
#include	"XMem.h" 

#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
extern CWmain(void);	// Main render loop on chris' stuff....
extern	BOOL	SoftwareVersion;
/*-------------------------------------------------------------------------*/
#endif
	extern BOOL ZClearsOn;
	extern BOOL AllowServer;
	extern void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval );
	extern BOOL CheckDirectPlayVersion;
	extern HKEY ghCondemnedKey;     // Condemned registry key handle
#ifdef LOBBY_DEBUG
	extern HKEY ghLobbyKey;     // lobby registry key handle
#endif
	extern	int DPlayUpdateIntervalCmdLine;
	extern int ValidInstall( void );
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
	extern int use_level_path;
	extern char level_path[];
	extern int use_data_path;
	extern int use_local_data;
	extern char data_path[];
	extern int LogosEnable;
	extern BOOL	MoviePlaying;
	extern BOOL SeriousError;
	extern BOOL	E3DemoHost;
	extern BOOL	E3DemoClient;
	extern	BOOL	PowerVR_Overide;
	extern	BOOL	E3DemoLoop;
	extern	BOOL	Is3Dfx;
	extern	BOOL	Is3Dfx2;
	extern	BOOL	TriLinear;
	extern	BOOL	NoSFX;
	extern	int		TextureMemory;
	extern	BOOL	NoTextureScaling;
	extern	BOOL	MipMap;
	extern	BOOL	TripleBuffer;
	extern	BOOL	PolygonText;
	extern	BOOL	UseSendAsync;
	extern	float	UV_Fix;
	extern BOOL	ECTSDemo;
	extern BOOL AllWires;
	extern BOOL NoAVI;
	extern BOOL CanDoStrechBlt;

	extern float normal_fov;
	extern float screen_aspect_ratio;
	extern	BOOL	CreateBatchFile;
	extern	BOOL	CreateLogFile;
	extern	BOOL LockOutWindows;
	extern	BOOL	DplayRecieveThread;
	extern BOOL PreventFlips;
	extern	BOOL DS;
	extern	BOOL NoDebugMsgs;
	extern BOOL SpaceOrbSetup;
	extern BOOL NoCompoundSfxBuffer;
	extern long UseDDrawFlip;
	extern BOOL NoSplash;

	extern LPDIRECTSOUND3DLISTENER	lpDS3DListener;
	extern BOOL	OptFileOnCommandline;
	extern BOOL	DeviceOnCommandline;

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
void	OnceOnlyInit( void );
void	OnceOnlyRelease( void );
void DebugPrintf( const char * format, ... );

void MovieRedraw (HWND);      // ID_MOVE_REDRAW
void MovieStop (HWND);
void MoviePlay (HWND hwnd);
void AviFinished( void );
void ShowSplashScreen( int num );
void RemoveDynamicSfx( void );
void FillStatusTab( void );

void	GetDeviceGuid( void );
extern LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);

HRESULT GUIDFromString( char *lpStr, GUID * pGuid);

}

/*
 * GLOBAL VARIABLES
 */
D3DAppInfo* d3dapp;         /* Pointer to read only collection of DD and D3D
                               objects maintained by D3DApp */
d3dmainglobals myglobs;     /* collection of global variables */

HKEY ghLobbyKey = NULL;     // lobby registry key handle

static UINT CancelAutoPlayMessage;

// AVI Specific stuff...
int AVI_bpp = 16;
int AVI_ZoomMode = 0;


HANDLE AVIThreadControlEvent;




/*
 *  INTERNAL FUNCTION PROTOTYPES
 */
static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine);
static BOOL CreateD3DApp(LPSTR lpCmdLine);
static BOOL BeforeDeviceDestroyed(LPVOID lpContext);
static BOOL AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT* lpViewport,
                               LPVOID lpContext);
void CleanUpAndPostQuit(void);
static void InitGlobals(void);
static BOOL AppPause(BOOL f);
void ReportD3DAppError(void);
static BOOL RenderLoop(void);
static BOOL RestoreSurfaces();
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam,
                           LPARAM lParam );

/****************************************************************************/
/*                            WinMain                                       */
/****************************************************************************/
/*
 * Initializes the application then enters a message loop which calls sample's
 * RenderScene until a quit message is received.
 */
int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
        int nCmdShow)
{
	HRESULT hr;
    int failcount = 0; /* number of times RenderLoop has failed */
    MSG msg;
    HACCEL hAccelApp;
    hPrevInstance;
	UINT	param1;
	UINT	param2;

	// sets the minimum amount of memory allocated froma single malloc.....
	//_amblksiz = 1024;

#ifdef DEBUG_ON
	XMem_Init();
	XExec_Init();
	DDSurf_Init();
	XSBuffer_Init();
#endif
	FillStatusTab();

	MyGameStatus = STATUS_Title;

	AVIThreadControlEvent = CreateEvent(NULL, FALSE, FALSE, NULL ); 
			
	// initialize COM library
	hr = CoInitialize(NULL);
	if FAILED(hr)
		goto FAILURE;
	/*
     * Create the window and initialize all objects needed to begin rendering
     */

    if(!AppInit(hInstance, lpCmdLine))
	{
		ClipCursor( NULL );
		ReallyShowCursor( TRUE );
        return FALSE;
	}
    hAccelApp = LoadAccelerators(hInstance, "AppAccel");    

//	if( ModeCase == -1 )
	{
//		D3DAppFullscreen(d3dapp->CurrMode);
	}

#if 0	// no movie will play...	
#if 1
	PlayVideoInWindow(myglobs.hWndMain);
#else  	
	PlayMovie( hInstance,
               hPrevInstance,
               0, myglobs.hWndMain );
#endif
#endif

	while (!myglobs.bQuit) {
        /* 
         * Monitor the message queue until there are no pressing
         * messages
         */
#if 1

//	This will disable windows key and alt-tab and ctrl-alt-del
		if( LockOutWindows )
		{
			param1 = WM_NULL;
			param2 = WM_KEYFIRST;
		}else{
			param1 = 0;
			param2 = 0;
		}
		if (PeekMessage(&msg, NULL, param1, param2, PM_REMOVE))
		{
            if (msg.message == WM_QUIT)
			{
                CleanUpAndPostQuit();
                break;
            }
			if (!d3dapp->bPaused ||
				!myglobs.hWndMain
				|| !TranslateAccelerator(myglobs.hWndMain, hAccelApp, &msg)
				)
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg);
			}
        }
#endif
		/* 
         * If the app is not minimized, not about to quit, not paused and D3D
         * has been initialized, we can render
         */
        if (d3dapp->bRenderingIsOK && !d3dapp->bMinimized && !d3dapp->bPaused
            && !myglobs.bQuit) {
            /*
             * If were are not in single step mode or if we are and the
             * bDrawAFrame flag is set, render one frame
             */
            if (!(myglobs.bSingleStepMode && !myglobs.bDrawAFrame)) {
                /* 
                 * Attempt to render a frame, if it fails, take a note.  If
                 * rendering fails more than twice, abort execution.
                 */
                if( !RenderLoop() )
				{
                    ++failcount;

					if( SeriousError )
					{
						CleanUpAndPostQuit();
						break;
					}

//                  if (failcount == 3) {
//	                  Msg("Rendering has failed too many times.  Aborting execution.\n");
//                    CleanUpAndPostQuit();
//                    break;
//					}
                }
            }
            /*
             * Reset the bDrawAFrame flag if we are in single step mode
             */
            if (myglobs.bSingleStepMode)
                myglobs.bDrawAFrame = FALSE;
        }
    }
	ClipCursor( NULL );
	ReallyShowCursor( TRUE );
	if ( ghCondemnedKey )
		RegCloseKey( ghCondemnedKey );
#ifdef LOBBY_DEBUG
	if ( ghLobbyKey )
		RegCloseKey( ghLobbyKey );
#endif

FAILURE:
	// Uninitialize the COM library
	CoUninitialize();
#ifdef DEBUG_ON
	DebugMathErrors();
	if ( UnMallocedBlocks() )
	{
		DebugPrintf( "Un-malloced blocks found!" );
	}
	if ( UnMallocedExecBlocks() )
	{
		DebugPrintf( "Un-malloced Exec blocks found!" );
	}
	if ( UnMallocedDDSurfBlocks() )
	{
		DebugPrintf( "Un-malloced DDSurf blocks found!" );
	}
	if ( UnMallocedSBufferBlocks() )
	{
		DebugPrintf( "Un-malloced SBuffer blocks found!" );
	}
#endif

    return msg.wParam;
}

/****************************************************************************/
/*             D3DApp Initialization and callback functions                 */
/****************************************************************************/
/*
 * AppInit
 * Creates the window and initializes all objects necessary to begin rendering
 */
static BOOL
AppInit(HINSTANCE hInstance, LPSTR lpCmdLine)
{
    WNDCLASS wc;

    /*
     * Initialize the global variables
     */
    InitGlobals();
    myglobs.hInstApp = hInstance;
    /*
     * Register the window class
     */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, "AppIcon");
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//  wc.lpszMenuName = "AppMenu";
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Example";
    if (!RegisterClass(&wc))
        return FALSE;
    /*
     * Create a window with some default settings that may change
     */

	myglobs.hWndMain = CreateWindowEx(
         WS_EX_APPWINDOW,
         "Example",
         "Forsaken",
         WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
         WS_THICKFRAME | WS_MINIMIZEBOX,
         0, 0,								// start position of Window..
         START_WIN_SIZE_X, START_WIN_SIZE_Y,
         NULL,                              /* parent window */
         NULL,                              /* menu handle */
         hInstance,                         /* program handle */
         NULL);                             /* create parms */  

    if (!myglobs.hWndMain){
        Msg("CreateWindowEx failed");
        return FALSE;
    }
    /*
     * Display the window
     */
    ShowWindow(myglobs.hWndMain, SW_SHOWNORMAL);
    UpdateWindow(myglobs.hWndMain);
    /* 
     * Have the example initialize it components which remain constant
     * throughout execution
     */

	OnceOnlyInit();

	if (!InitScene())
        return FALSE;
    /*
     * Call D3DApp to initialize all DD and D3D objects necessary to render.
     * D3DApp will call the device creation callback which will initialize the
     * viewport and the sample's execute buffers.
     */
    if (!CreateD3DApp(lpCmdLine))
        return FALSE;

#ifdef EXTERNAL_DEMO
    myglobs.bShowFrameRate = FALSE;
#else

	myglobs.bShowFrameRate = ( ECTSDemo || E3DemoHost || E3DemoClient || E3DemoLoop ) ? FALSE : TRUE;
#endif

    return TRUE;
}

extern "C" {
extern char *config_name;
}
/*
 * CreateD3DApp
 * Create all DirectDraw and Direct3D objects necessary to begin rendering.
 * Add the list of D3D drivers to the file menu.
 */
static BOOL
CreateD3DApp(LPSTR lpCmdLine)
{
	char optname[256];
	char optfile[256];
    HMENU hmenu;
    int i;
    LPSTR option;
    BOOL bOnlySystemMemory, bOnlyEmulation;
    DWORD flags;
    Defaults defaults;
#if defined( SHAREWARE_MAGAZINE ) || defined( SHAREWARE_RETAIL ) || defined( SHAREWARE_INTERNET ) || defined( SHAREWARE_TCP )
	BOOL game_ok = TRUE;
#else
	BOOL game_ok = FALSE;
#endif
	char * DataPath;
	char *cmdlineptr;
	char tempcmdline[ 256 ];
	char buf[ 256 ];


#ifdef SOFTWARE_ENABLE
/*---------------------------------------------------------------------------
	Chris Walsh's code
---------------------------------------------------------------------------*/
	int	Writeable;
/*-------------------------------------------------------------------------*/
#endif

    /*
     * Parse the command line in search of one of the following options:
     *     systemmemory  All surfaces should be created in system memory.
     *                   Hardware DD and D3D devices are disabled, but
     *                   debugging during the Win16 lock becomes possible.
     *     emulation     Do not use hardware DD or D3D devices.
     */
    bOnlySystemMemory = FALSE;
    bOnlyEmulation = FALSE;

	E3DemoHost = FALSE;
	E3DemoClient = FALSE;
#if defined( ECTS )
	LogosEnable = 1;
#endif
#ifdef SELF_PLAY
	E3DemoLoop = TRUE;
#ifdef BOOT_DEMO
	LogosEnable = 2;
#else
	LogosEnable = 1;
#endif
#else
	E3DemoLoop = FALSE;
#endif
	PowerVR_Overide = FALSE;
	Is3Dfx = FALSE;
	Is3Dfx2 = FALSE;
	TriLinear = FALSE;
	NoSFX = FALSE;
	TextureMemory = 0;
	MipMap = FALSE;
	TripleBuffer = FALSE;
	ECTSDemo = FALSE;
	NoTextureScaling = FALSE;
	DplayRecieveThread = FALSE;
	PolygonText = FALSE;
	DS = FALSE;
	NoSplash = FALSE;
	SessionGuidExists = FALSE;
	UseSendAsync = FALSE;

	DPlayUpdateIntervalCmdLine = 0;

	cmdlineptr = lpCmdLine;

	// get around dplay bug of not propagating command line when lobby launched
	// if app was launched from launcher, -xmen is always specified and so command line cannot be empty
	if ( !lpCmdLine || !lpCmdLine[ 0 ] )
	{
		DWORD dwType;
		DWORD dwDataSize = 256;
		
		// get command line from registery...

		// open key...
		if ( RegOpenKeyEx(REGISTRY_ROOT_KEY, REGISTRY_LOBBY_KEY, 0, /*KEY_ALL_ACCESS*/KEY_QUERY_VALUE, &ghLobbyKey	) == ERROR_SUCCESS )
		{
	    	// get command line...
			if ( RegQueryValueEx(ghLobbyKey, "CommandLine", NULL, &dwType, ( unsigned char * )tempcmdline, &dwDataSize) == ERROR_SUCCESS )
			{
				cmdlineptr = tempcmdline;
			}
		}
	}

    option = strtok(cmdlineptr, " -+");
    while(option != NULL )   {

        if (!_stricmp(option, "systemmemory")) {
            bOnlySystemMemory = TRUE;
#ifdef SOFTWARE_ENABLE
			SoftwareVersion = TRUE;
#endif
        } else if (!_stricmp(option, "emulation")) {
            bOnlyEmulation = TRUE;
        } else if (!_stricmp(option, "xmen")) {
            game_ok = TRUE;
        } else if (!_stricmp(option, "E3DemoHost")) {
			E3DemoHost = TRUE;
        } else if (!_stricmp(option, "DS")) {
			DS = TRUE;
        } else if (!_stricmp(option, "E3DemoClient")) {
			E3DemoClient = TRUE;
        } else if (!_stricmp(option, "E3DemoLoop")) {
			E3DemoLoop = TRUE;
        } else if (!_stricmp(option, "PowerVR")) {
			PowerVR_Overide = TRUE;
        } else if (!_stricmp(option, "3Dfx")) {
			Is3Dfx = TRUE;
        } else if (!_stricmp(option, "SendAsync")) {
			UseSendAsync = TRUE;
        } else if (!_stricmp(option, "3Dfx2")) {
			Is3Dfx2 = TRUE;
			TriLinear = TRUE;
        } else if (!_stricmp(option, "TriLinear")) {
			TriLinear = TRUE;
        } else if (!_stricmp(option, "NoSFX")) {
			NoSFX = TRUE;
        } else if (!_stricmp(option, "PolyText")) {
			PolygonText = TRUE;
        } else if (!_stricmp(option, "DplayThread")) {
			DplayRecieveThread = TRUE;
        } else if (!_stricmp(option, "NoTextureScaling")) {
			NoTextureScaling = TRUE;
        } else if (!_stricmp(option, "MipMap")) {
			MipMap = TRUE;
        } else if (!_stricmp(option, "TripleBuffer")) {
			TripleBuffer = TRUE;
        } else if (!_stricmp(option, "ECTS")) {
			ECTSDemo = TRUE;
        } else if (!_stricmp(option, "BatchFile")) {
            CreateBatchFile = TRUE;
        } else if (!_stricmp(option, "LogFile")) {
            CreateLogFile = TRUE;
        } else if (!_stricmp(option, "AllWires")) {
            AllWires = TRUE;
        } else if (!_stricmp(option, "NoDynamicSfx")) {
            RemoveDynamicSfx();
        } else if (!_stricmp(option, "NoAVI")) {
            NoAVI = TRUE;								 
        } else if (!_stricmp(option, "NoDebug")) {
            NoDebugMsgs = TRUE;
        } else if (!_stricmp(option, "Debug")) {
            NoDebugMsgs = FALSE;
		} else if ( !_stricmp( option, "SetupSpaceOrb" ) ) {
			SpaceOrbSetup = TRUE;
		} else if ( !_stricmp( option, "NoBlitTextScaling" ) ) {
			CanDoStrechBlt = FALSE;
		} else if ( !_stricmp( option, "NoCompoundSfxBuffer" ) ) {
			NoCompoundSfxBuffer = TRUE;
		} else if ( !_stricmp( option, "AnyDPlayVersion" ) ) {
			CheckDirectPlayVersion = FALSE;
		} else if ( !_stricmp( option, "QuickHost" ) ) {
			NoSplash = TRUE;
			QuickStart = QUICKSTART_Start;
		} else if ( !_stricmp( option, "NoSplash" ) ) {
			NoSplash = TRUE;
		}else if ( !_stricmp( option, "Server" ) ) {
			AllowServer = TRUE;	 
		} else if ( !_stricmp( option, "ServerAutoStart" ) ) {
			QuickStart = QUICKSTART_Server;
			ServerChoosesGameType = TRUE;
			NoSplash = TRUE;
		} else if ( !_stricmp( option, "QuickJoin" ) ) {
			NoSplash = TRUE;
			QuickStart = QUICKSTART_Join;
		}else if ( !_stricmp( option, "ForceHeartbeat" ) ) {
			ForceHeartbeat = TRUE;	 
		}else if ( !_stricmp( option, "session" ) ) {
	        option = strtok(NULL, "{}");
			sprintf( buf, "{%s}", option );
			if ( GUIDFromString( buf, &autojoin_session_guid ) == S_OK )
			{
				SessionGuidExists = TRUE;
			}
		}else if ( !_stricmp( option, "TCP" ) ) {
			bTCP = TRUE;
	        option = strtok(NULL, " ");
			strcpy( (LPSTR)TCPAddress.text, option );
		} else if ( !_stricmp( option, "AutoStart" ) ) {
			NoSplash = TRUE;
			QuickStart = QUICKSTART_SelectSession;
#ifdef Z_TRICK
		} else if ( !_stricmp( option, "NoZClear" ) ) {
			ZClearsOn = FALSE;
#endif
#ifdef SOFTWARE_ENABLE
		} else if ( !_stricmp( option, "UseDDrawFlip" ) ) {
			UseDDrawFlip = TRUE;
#endif
        } else {
#if 1
			int num, denom;
			float fnum;
			DWORD mem;

			if ( sscanf( option, "dev%d", &num ) == 1 )
			{
				ddchosen3d = num;
				DeviceOnCommandline = TRUE;
			}
			if ( sscanf( option, "CompoundSfxBufferMem%d", &mem ) == 1 )
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
			else if ( sscanf( option, "lev:%s", level_path ) == 1 )
			{
				use_level_path = 1;
			}
			else if ( sscanf( option, "opt:%s", optname ) == 1 )
			{
				if ( strstr( optname, ".opt" ) || strstr( optname, ".OPT" ) )
					sprintf( optfile, "%s", optname );
				else
					sprintf( optfile, "opt\\%s.opt", optname );
				ReadIniFile( optfile );
			}
			else if ( !strcmp( option, "data" ) )
			{
				DataPath = getenv( "PROJXDATA" );
				if( DataPath )
				{
					use_data_path = 1;
					use_local_data = 1;
					strcpy( &data_path[ 0 ], DataPath );
					strcat( &data_path[ 0 ], "\\" );
				}
			}
			else if ( sscanf( option, "data:%s", data_path ) == 1 )
			{
				strcat( &data_path[ 0 ], "\\" );
				use_data_path = 1;
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
			else
			{
				config_name = option;
			}
#else
            Msg("Invalid command line options given.\nLegal options: -systemmemory, -emulation\n");
            return FALSE;
#endif
        }
        option = strtok(NULL, " -+");
    }
#ifndef SELF_PLAY
	/*
#ifdef PCIDENT
	if ( !game_ok || !AllowGame() || !ValidFingerPrint( PCIDENT_FILE ) )
#else
	if ( !game_ok || !AllowGame() )
#endif
	{
        Msg( "Fatal initialization error\n" );
        return FALSE;
	}
	*/
	GetGamePrefs();
	GetDeviceGuid();

	SoundInit();	// cd audio
	OpenCD();

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
#endif
	if ( !default_bpp )
		default_bpp = 16;
    /*
     * Set the flags to pass to the D3DApp creation based on command line
     */
    flags = ((bOnlySystemMemory) ? D3DAPP_ONLYSYSTEMMEMORY : 0) | 
            ((bOnlyEmulation) ? (D3DAPP_ONLYD3DEMULATION |
                                 D3DAPP_ONLYDDEMULATION) : 0);

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
    strcpy(defaults.Name, "D3D ProjectX Demo");
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
#ifndef FINAL_RELEASE
	DebugPrintf( "CancelAutoPlayMessage=0x%08X\n", CancelAutoPlayMessage );
#endif

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
	}else
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
/*
 * RenderLoop
 * Render the next frame and update the window
 */
static BOOL
RenderLoop()
{
    /*
     * If all the DD and D3D objects have been initialized we can render
     */
    if (d3dapp->bRenderingIsOK) {
        /*
         * Restore any lost surfaces
         */
        if (!RestoreSurfaces())
		{
            /*
             * Restoring surfaces sometimes fails because the surfaces cannot
             * yet be restored.  If this is the case, the error will show up
             * somewhere else and we should return success here to prevent
             * unnecessary error's being reported.
             */
            return TRUE;
        }
        /*
         * Call the sample's RenderScene to render this frame
         */
        {
            if (!RenderScene(d3dapp->lpD3DDevice, d3dapp->lpD3DViewport)) {
                Msg("RenderScene failed.\n");
                return FALSE;
            }

			if ( quitting )
			{
				quitting = FALSE;
				DebugPrintf("about to CleanUpAndPostQuit ( from RenderLoop )\n");
				CleanUpAndPostQuit();
			}
        }
        /*
         * Blt or flip the back buffer to the front buffer
         */
		if( !myglobs.bQuit )
		{
			if ( ( !PlayDemo ||
				!( MyGameStatus == STATUS_AttractMode || MyGameStatus == STATUS_PlayingDemo ) ||
				DemoShipInit[ Current_Camera_View ] )
				&& !PreventFlips )
			{
				if (!D3DAppShowBackBuffer(myglobs.bResized ? D3DAPP_SHOWALL : NULL))
				{
					ReportD3DAppError();
					return FALSE;
				}
			}

			// if there is 3D sound, commit 3D sound processing
			if ( lpDS3DListener )
			{
				//DebugPrintf("Commiting 3D settings\n");
				if ( lpDS3DListener->CommitDeferredSettings() != DS_OK )
					DebugPrintf("Error commiting 3D\n");
			}
		}
		/*
		 * Reset the resize flag
	     */
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
    if (!D3DAppCheckForLostSurfaces()) {
            return FALSE;
    }
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
#ifndef FINAL_RELEASE
		DebugPrintf( "AutoPlay cancelled\n" );
#endif
		return 1L;
	}

    switch( message ) {

		// some i/o device has changed state -- make sure the CD is still valid
		case WM_DEVICECHANGE:
#ifndef FINAL_RELEASE
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
#endif
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
			if (wParam == MCI_NOTIFY_SUCCESSFUL)
			{
				DebugPrintf("CD audio track finished\n");
				PlayAudioTrack();
			}
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
 * InitGlobals
 * Called once at program initialization to initialize global variables.
 */
static void
InitGlobals(void)
{
    d3dapp = NULL;
    memset(&myglobs.rstate, 0, sizeof(myglobs.rstate));
    memset(&myglobs, 0, sizeof(myglobs));
    myglobs.bClearsOn = FALSE;
    myglobs.bShowFrameRate = TRUE;
    myglobs.bShowInfo = FALSE;
}

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

