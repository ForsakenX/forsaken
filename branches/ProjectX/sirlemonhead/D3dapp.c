
/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dapp.c
 *
 *  Top level D3DApp functions and internal global variables.  See
 *  d3dapp.h for more information.
 *
 *  D3DApp is a collection of helper functions for Direct3D applications.
 *  D3DApp consists of the following files:
 *      d3dapp.h    Main D3DApp header to be included by application
 *      d3dappi.h   Internal header
 *      d3dapp.c    D3DApp functions seen by application.
 *      ddcalls.c   All calls to DirectDraw objects except textures
 *      d3dcalls.c  All calls to Direct3D objects except textures
 *      texture.c   Texture loading and managing texture list
 *      misc.c      Miscellaneous calls
 */

#include "typedefs.h"
#include "d3dappi.h"
#include "main.h"
#include "title.h"

BOOL Wine = FALSE;

extern void SetInputAcquired( BOOL );
extern void SetCursorClip( BOOL );
extern BOOL	DrawPanel;
extern BOOL	DrawCrosshair;
extern uint16 PanelHeight;
extern void ProcessTextItems (void);
extern BOOL cursor_clipped;
extern	int		TexturePalettized;
extern	int		TextureRedBPP;
extern	int		TextureGreenBPP;
extern	int		TextureBlueBPP;
extern	int		TextureAlphaBPP;
extern	int		TextureIndexBPP;
extern int ignore_mouse_input;
extern BOOL CanRenderWindowed;
extern void DebugPrintf( char *fmt, ... );
extern BOOL AppPause(BOOL f);
extern BOOL flush_input;
extern BOOL HideCursor;
extern BOOL ActLikeWindow;

extern d3dmainglobals myglobs;     /* collection of global variables */

/***************************************************************************/
/*                           GLOBAL VARIABLES                              */
/***************************************************************************/
/*
 * All DD and D3D objects which are also available to the application
 * See d3dapp.h for typedef
 */
D3DAppInfo d3dappi;
/*
 * Internal record of the render state.  See d3dapp.h for typedef
 */
D3DAppRenderState d3dapprs; 
/* 
 * Callback functions for D3D device creation and destruction
 */

/*
BOOL(*D3DDeviceDestroyCallback)(LPVOID);
LPVOID D3DDeviceDestroyCallbackContext;
BOOL(*D3DDeviceCreateCallback)(int, int, LPDIRECT3DVIEWPORT*, LPVOID);
LPVOID D3DDeviceCreateCallbackContext;
*/
/*
 * The last error code and string
 */
HRESULT LastError;
char LastErrorString[256];
/*
 * List of texture handles which is copied to D3DAppInfo structure when
 * necessary
 */

void InitModeCase(void);

//LPDIRECTDRAWCLIPPER lpClipper; /* Clipper in windowed case */
//LPDIRECTDRAWPALETTE lpPalette; /* Front buffer's palette */
//PALETTEENTRY ppe[256];         /* Current palette entries */
//PALETTEENTRY Originalppe[256]; /* Windows palette entries at startup */
BOOL bD3DAppInitialized;       /* Is D3DApp initialized? */
BOOL bPrimaryPalettized;       /* Is the front buffer palettized? */
BOOL bPaletteActivate;         /* Is the front buffer's palette valid? */
BOOL bIgnoreWM_SIZE;           /* Ignore this WM_SIZE messages */
BOOL bFullscreen;			   /* Fullscreen flag from cli */
SIZE szLastClient;             /* Dimensions of the last window */
SIZE szBuffers;                /* Current buffer dimensions, not necessarily
                                  the same as the client window */
int CallbackRefCount;          /* How many times DeviceCreateCallback has
                                  been called in a row */

/***************************************************************************/
/*                               FUNCTIONS                                 */
/***************************************************************************/
/*
 * D3DAppCreateFromHWND
 */

#if 0 // bjd
BOOL D3DAppCreateFromHWND(DWORD flags, HWND hwnd,
                          /*BOOL(*DeviceCreateCallback)(int, int,
                                                      LPDIRECT3DVIEWPORT*,
                                                      LPVOID),
                          LPVOID lpCreateContext,
                          BOOL(*DeviceDestroyCallback)(LPVOID),
                          LPVOID lpDestroyContext,*/
                          D3DAppInfo** D3DApp)
{
    int driver, mode, w, h;

    /* 
     * Clean the global varaibles and check the flags
     */

    D3DAppISetDefaults();

    if (flags & D3DAPP_ONLYSYSTEMMEMORY) {
        d3dappi.bOnlySystemMemory = TRUE;
        d3dappi.bOnlyEmulation = TRUE;
    }

	if (flags & D3DAPP_ONLYD3DEMULATION){
        d3dappi.bOnlyEmulation = TRUE;
	}

    /* 
     * Create DirectDraw, remember the Windows display mode and enumerate the
     * display modes
     */

 //   ATTEMPT(D3DAppICreateDD(d3dappi.bOnlyEmulation ? D3DAPP_ONLYDDEMULATION : 0L));
//    ATTEMPT(D3DAppIRememberWindowsMode());
//    ATTEMPT(D3DAppIEnumDisplayModes());

    /*
     * Create Direct3D and enumerate the D3D drivers
     */

    ATTEMPT(D3DAppICreateD3D());
//    ATTEMPT(D3DAppIEnumDrivers());

    /*
     * Set the device creation and destroy callback functions
     */

//    D3DDeviceDestroyCallback		= DeviceDestroyCallback;
//    D3DDeviceDestroyCallbackContext = lpDestroyContext;
//    D3DDeviceCreateCallback			= DeviceCreateCallback;
//    D3DDeviceCreateCallbackContext	= lpCreateContext;

    *D3DApp = &d3dappi;

    d3dappi.hwnd = hwnd;

    /*
     * Choose a driver and display mode.  Using the current window is 
     * prefered, but a fullscreen mode may be selected.  Set the cooperative
     * level and create the front and back buffers for this mode.
     */

	// let the rest of the code decide
    driver = D3DAPP_YOUDECIDE;

	// use window mode if command line says so
	// other wise let the rest of the code decide
	mode = (bFullscreen) ? D3DAPP_YOUDECIDE : D3DAPP_USEWINDOW;

//    ATTEMPT(D3DAppIVerifyDriverAndMode(&driver, &mode));

    D3DAppIGetClientWin(hwnd);

/*
    if (mode == D3DAPP_USEWINDOW) {
        w = d3dappi.szClient.cx;
        h = d3dappi.szClient.cy;
        ATTEMPT(D3DAppISetCoopLevel(hwnd, FALSE));
        ATTEMPT(D3DAppICreateBuffers(hwnd, w, h, D3DAPP_BOGUS, FALSE));
    } else {
        szLastClient = d3dappi.szClient;
        w = d3dappi.Mode[mode].w;
        h = d3dappi.Mode[mode].h;
        d3dappi.szClient.cx = w; d3dappi.szClient.cy = h;
        ATTEMPT(D3DAppISetCoopLevel(hwnd, TRUE));
        ATTEMPT(D3DAppISetDisplayMode(w, h, d3dappi.Mode[mode].bpp));
        d3dappi.CurrMode = mode;
        ATTEMPT(D3DAppICreateBuffers(hwnd, w, h, d3dappi.Mode[mode].bpp, TRUE));
    }
*/
    /*
     * If the front buffer is palettized, initialize its palette
     */

//    ATTEMPT(D3DAppICheckForPalettized());

    /*
     * Create the Z-buffer
     */

//    ATTEMPT(D3DAppICreateZBuffer(w, h, driver));

    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */

    ATTEMPT(D3DAppICreateDevice(driver));
    ATTEMPT(D3DAppIFilterDisplayModes(driver));  /* bThisDriverCanDo flags */
    ATTEMPT(D3DAppICallDeviceCreateCallback(w, h));
    ATTEMPT(D3DAppISetRenderState());

    /*
     * Ready to render
     */

    bD3DAppInitialized = TRUE;
    d3dappi.bRenderingIsOK = TRUE;

    return TRUE;

exit_with_error:
//    D3DAppICallDeviceDestroyCallback();
//    RELEASE(d3dappi.lpD3DDevice);
//    RELEASE(d3dappi.lpZBuffer);
//    RELEASE(lpPalette);
 //   RELEASE(lpClipper);
  //  RELEASE(d3dappi.lpBackBuffer);
//    RELEASE(d3dappi.lpFrontBuffer);
    if (d3dappi.bFullscreen) {
        D3DAppIRestoreDispMode();
//        D3DAppISetCoopLevel(hwnd, FALSE);
    }
    RELEASE(d3dappi.lpD3D);
//    RELEASE(d3dappi.lpDD);
    return FALSE;
}   
#endif

/*
 * D3DAppFullscreen
 */
BOOL D3DAppFullscreen(int mode)
{
	return TRUE;
#if 0 //bjd
    int w, h, bpp;
    BOOL b; /* was already in a fullscreen mode? */

    d3dappi.bRenderingIsOK = FALSE;
    /* 
     * Make sure this is a valid request, otherwise doctor mode so it will
     * work with this driver.
     */
    ATTEMPT(D3DAppIVerifyDriverAndMode(&d3dappi.CurrDriver, &mode));
    /* 
     * Release everything
     */
    ATTEMPT(D3DAppICallDeviceDestroyCallback());
    if (d3dappi.bFullscreen) {
        ATTEMPT(FSClear());
    }
    RELEASE(d3dappi.lpD3DDevice);
    RELEASE(d3dappi.lpZBuffer);
    RELEASE(lpPalette);
    RELEASE(lpClipper);
    RELEASE(d3dappi.lpBackBuffer);
    RELEASE(d3dappi.lpFrontBuffer);
    /*
     * Record information about the current status
     */
    b = d3dappi.bFullscreen;
    w = d3dappi.Mode[mode].w;
    h = d3dappi.Mode[mode].h;
    bpp = d3dappi.Mode[mode].bpp;
    if (!b) {
        /*
         * If this is not a fullscreen mode, we'll need to record the window
         * size for when we return to it.
         */
        szLastClient = d3dappi.szClient;
    }
    /*
     * Set the cooperative level and create front and back buffers
     */
    d3dappi.szClient.cx = w; d3dappi.szClient.cy = h;

	InitModeCase();

    ATTEMPT(D3DAppISetCoopLevel(d3dappi.hwnd, TRUE));
    ATTEMPT(D3DAppISetDisplayMode(w, h, bpp));
    d3dappi.CurrMode = mode;
    ATTEMPT(D3DAppICreateBuffers(d3dappi.hwnd, w, h, bpp, TRUE));
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(D3DAppICheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(D3DAppICreateZBuffer(w, h, d3dappi.CurrDriver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(D3DAppICreateDevice(d3dappi.CurrDriver));
    ATTEMPT(D3DAppICallDeviceCreateCallback(w, h));
    ATTEMPT(D3DAppISetRenderState());
    /* 
     * Set current mode
     */
    d3dappi.CurrMode = mode;
    d3dappi.bRenderingIsOK = TRUE;

    return TRUE;

exit_with_error:
    D3DAppICallDeviceDestroyCallback();
    RELEASE(d3dappi.lpD3DDevice);
    RELEASE(d3dappi.lpZBuffer);
    RELEASE(lpPalette);
    RELEASE(lpClipper);
    RELEASE(d3dappi.lpBackBuffer);
    RELEASE(d3dappi.lpFrontBuffer);
    if (!b) {
        D3DAppIRestoreDispMode();
        D3DAppISetCoopLevel(d3dappi.hwnd, FALSE);
    }
    return FALSE;
#endif
}

/*
 * D3DAppWindow
 */

BOOL D3DAppWindowMode( int mode )
{
    d3dappi.CurrMode = mode;
	d3dappi.ThisMode = d3dappi.Mode[mode];
	return D3DAppWindow(
		d3dappi.Mode[mode].w,
		d3dappi.Mode[mode].h,
		d3dappi.Mode[mode].bpp);
}

BOOL
D3DAppWindow(int w, int h, int bpp)
{
	return TRUE;
#if 0 // bjd
    BOOL b; /* changing from a fullscreen mode? */

    /*
	if (!d3dappi.bIsPrimary) {
        D3DAppISetErrorString("It is not possible to create a D3D window with a hardware DirectDraw device.  Check the bIsPrimary flag before calling D3DAppWindow.");
        return FALSE;
    }
	*/
	if ( !CanRenderWindowed )
	{
		Msg("this video card cannot render to a window\n");
		return FALSE;
	}

    b = d3dappi.bFullscreen;
    /*
     * If asked to set the window size, return it to the last value or use
     * a default value.
     */
    if (w == D3DAPP_YOUDECIDE) {
        w = b ? szLastClient.cx : D3DAPP_DEFAULTWINDOWDIM;
    }
    if (h == D3DAPP_YOUDECIDE) {
        h = b ? szLastClient.cy : D3DAPP_DEFAULTWINDOWDIM;
    }

    /*
     * Release everything
     */
    d3dappi.bRenderingIsOK = FALSE;
    ATTEMPT(D3DAppICallDeviceDestroyCallback());
    if (b) {
        ATTEMPT(FSClear());
    }
    RELEASE(d3dappi.lpD3DDevice);
    RELEASE(d3dappi.lpZBuffer);
//    RELEASE(lpPalette);
    RELEASE(lpClipper);
    RELEASE(d3dappi.lpBackBuffer);
    RELEASE(d3dappi.lpFrontBuffer);
    /* 
     * Restore the display mode if we were in a fullscreen mode
     */
    if (b) {
        D3DAppIRestoreDispMode();
    }
    /* 
     * Set the cooperative level and create front and back buffers
     */
    D3DAppISetCoopLevel(d3dappi.hwnd, FALSE);
    D3DAppISetClientSize(d3dappi.hwnd, w, h, b);
    ATTEMPT(D3DAppICreateBuffers(d3dappi.hwnd, w, h, bpp, FALSE));
    /*
     * If the front buffer is palettized, initialize its palette
     */
    ATTEMPT(D3DAppICheckForPalettized());
    /*
     * Create the Z-buffer
     */
    ATTEMPT(D3DAppICreateZBuffer(szBuffers.cx, szBuffers.cy,
                                 d3dappi.CurrDriver));
    /*
     * Create the D3D device, load the textures, call the device create
     * callback and set a default render state
     */
    ATTEMPT(D3DAppICreateDevice(d3dappi.CurrDriver));
    ATTEMPT(D3DAppICallDeviceCreateCallback(szBuffers.cx, szBuffers.cy));
    ATTEMPT(D3DAppISetRenderState());
    d3dappi.bRenderingIsOK = TRUE;

	// resize window
	// BUG: need to have title bar automatically calculated for
	// for now adding 20 pixels works on my computer...
	SetWindowPos(d3dappi.hwnd, 0, 0, 0, w, h+20, SWP_NOMOVE|SWP_NOZORDER);

    return TRUE;

exit_with_error:
    D3DAppICallDeviceDestroyCallback();
    RELEASE(d3dappi.lpD3DDevice);
    RELEASE(d3dappi.lpZBuffer);
    RELEASE(lpPalette);
    RELEASE(lpClipper);
    RELEASE(d3dappi.lpBackBuffer);
    RELEASE(d3dappi.lpFrontBuffer);
    return FALSE;
#endif
}


/*
 * D3DAppChangeDriver 
 */
BOOL
D3DAppChangeDriver(int driver, DWORD flags)
{
    int mode;

    /*
     * Verify the compatibility of this mode with the specified driver.
     * The mode may change.
     */
    if (d3dappi.bFullscreen)
        mode = d3dappi.CurrMode;
    else
        mode = D3DAPP_USEWINDOW;
    ATTEMPT(D3DAppIVerifyDriverAndMode(&driver, &mode));
    if (driver == D3DAPP_BOGUS || mode == D3DAPP_BOGUS)
        goto exit_with_error;
    /*
     * Update the current driver and set bThisDriverCanDo flags
     */
    d3dappi.CurrDriver = driver;
    ATTEMPT(D3DAppIFilterDisplayModes(driver));
    /*
     * Either call D3DAppWindow or D3DAppFullscreen depending on mode
     */
    if (mode == D3DAPP_USEWINDOW) {
        if (d3dappi.bFullscreen) {
            /*
             * We need to switch to a window.  D3DApp will either use the
             * size of the last window it saw or use a default size.
             */
            ATTEMPT(D3DAppWindow(D3DAPP_YOUDECIDE, D3DAPP_YOUDECIDE, d3dappi.Mode[d3dappi.CurrMode].bpp));
        } else {
            /*
             * We need to recreate the current window.  Don't let D3DApp
             * decide on the size.
             */
            ATTEMPT(D3DAppWindow(d3dappi.szClient.cx, d3dappi.szClient.cy, d3dappi.Mode[d3dappi.CurrMode].bpp));
        }
        /*
         * Change the currently selected mode if it's not compatible with
         * this driver.  Just to make sure that CurrMode is always a mode the
         * current driver can do.
         */

#if 0 // bjd
        if (!(d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth &
              D3DAppIBPPToDDBD(d3dappi.Mode[d3dappi.CurrMode].bpp))){
            ATTEMPT(D3DAppIPickDisplayMode(&d3dappi.CurrMode,
                        d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth));
        }
#endif
        return TRUE;
    } else {
        /*
         * We need to switch to fullscreen or switch fullscreen modes or stay
         * in the same fullscreen mode.  In any of these cases, we call the
         * same function.
         */
        ATTEMPT(D3DAppFullscreen(mode));
        return TRUE;
    }
exit_with_error:
    /*
     * The failed mode setting call would have released everything
     */
    return FALSE;
}


/*
 * D3DAppWindowProc
 */

// Returning false causes program to quit...

extern float HoloLightBrightness;
extern void FadeHoloLight(float Brightness );
extern BOOL MouseExclusive;
extern void DarkenRoom2(float darkness);
extern float RoomDarkness;
extern void ProcessVduItems( MENU *Menu );
extern InitialTexturesSet;
extern void SetGamePrefs( void );

BOOL D3DAppWindowProc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                 UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    *bStopProcessing = FALSE; // when set to true stops processing the message
    if (!bD3DAppInitialized)
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
					if (d3dappi.bRenderingIsOK && !d3dappi.bFullscreen) {
						D3DAppShowBackBuffer(D3DAPP_SHOWALL);
					}
					EndPaint(hwnd, &ps);

			// release mouse so they can interact with message box
			SetCursorClip( FALSE );

			// ask them to confirm clossing
			if( IDYES == MessageBox(
							myglobs.hWndMain,
							"Are you sure you want to exit?",
							"User requested to close application...",
							MB_YESNO |
							MB_ICONQUESTION |
							MB_DEFBUTTON2) // default is no

			// user wants to quit
			// let our code know we're quitting and not failing
			// let the message reach DefWindowProc so it calls CloseWindow
			){
				myglobs.bQuit = 1;
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
				else if ( d3dappi.bFullscreen )
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
			d3dappi.pClientOnPrimary.x = 0;
			d3dappi.pClientOnPrimary.y = 0;

			// this sets the CLIENT AREA POSITION
			d3dappi.pClientOnPrimary.x = (int)(short) LOWORD(lParam);
			d3dappi.pClientOnPrimary.y = (int)(short) HIWORD(lParam);

			// save the WINDOW POSITION
			{
				WINDOWPLACEMENT placement;
				placement.length = sizeof(WINDOWPLACEMENT);
				if(GetWindowPlacement( d3dappi.hwnd, &placement ))
				{
					d3dappi.pWindow.x = placement.rcNormalPosition.left;
					d3dappi.pWindow.y = placement.rcNormalPosition.top;
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
//bjd				ATTEMPT(D3DAppIHandleWM_SIZE(lresult, d3dappi.hwnd, message, wParam, lParam));

				if( MyGameStatus == STATUS_Title )
				{
					FadeHoloLight(HoloLightBrightness);
					DarkenRoom2(RoomDarkness);
					ProcessVduItems( CurrentMenu );
   					InitialTexturesSet = FALSE;
				}

				myglobs.bResized = TRUE;
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
				if ( MouseExclusive ){
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
				else if ( d3dappi.bFullscreen )
					SetCursorClip( TRUE );

			}

            // Set the front buffer's palette
			// what does this do ? when should it be ran ?
/* bjd
            if (bPaletteActivate && bPrimaryPalettized && d3dappi.lpFrontBuffer )
                d3dappi.lpFrontBuffer->lpVtbl->SetPalette(d3dappi.lpFrontBuffer, lpPalette);
*/
            break;

		// this event is same as above but sends TRUE|FALSE
        case WM_ACTIVATEAPP:
			//DebugPrintf("Window is being %s.\n",(wParam?"activated":"de-activated"));
            d3dappi.bAppActive = (BOOL)wParam;
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
            if (d3dappi.bRenderingIsOK && !d3dappi.bFullscreen) {
                D3DAppShowBackBuffer(D3DAPP_SHOWALL);
            }
            EndPaint(hwnd, &ps);
			//*lresult = 1;
			//*bStopProcessing = TRUE;
			break;

        case WM_NCPAINT:
			//DebugPrintf("We are requested to update the window frame.\n");
            // When in fullscreen mode, don't draw the window frame.
            if (d3dappi.bFullscreen) {
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
            //if (d3dappi.bFullscreen) {
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
			if(d3dappi.bFullscreen)
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

exit_with_error:
	DebugPrintf("D3DAppWindowProc called exit_with_error.\n");
	return FALSE;
}


/*
 * D3DAppChangeTextureFormat
 */
BOOL
D3DAppChangeTextureFormat(int format)
{
	return TRUE;
#if 0 //bjd
    /*
     * Release all the textures, change the format and load them again
     */
    d3dappi.bRenderingIsOK = FALSE;
    d3dappi.CurrTextureFormat = format;
    memcpy(&d3dappi.ThisTextureFormat, &d3dappi.TextureFormat[format],
           sizeof(D3DAppTextureFormat));
	TexturePalettized = d3dappi.ThisTextureFormat.bPalettized;
	TextureRedBPP = d3dappi.ThisTextureFormat.RedBPP;
	TextureGreenBPP = d3dappi.ThisTextureFormat.GreenBPP;
	TextureBlueBPP = d3dappi.ThisTextureFormat.BlueBPP;
	TextureAlphaBPP = d3dappi.ThisTextureFormat.AlphaBPP;
	TextureIndexBPP = d3dappi.ThisTextureFormat.IndexBPP;
    d3dappi.bRenderingIsOK = TRUE;
    return TRUE;
#endif
}



/*
 * D3DAppSetRenderState
 */
BOOL
D3DAppSetRenderState(D3DAppRenderState* lpState)
{
    /*
     * If none was provided, reset the current render state.
     */
    if (!lpState)
        lpState = &d3dapprs;
    /*
     * Record this render state and set it.
     */
    if (lpState != &d3dapprs)
        memcpy(&d3dapprs, lpState, sizeof(D3DAppRenderState));
    if (d3dappi.bRenderingIsOK) {
        ATTEMPT(D3DAppISetRenderState());
    }
    return TRUE;

exit_with_error:
    return FALSE;
}

/*
 * D3DAppGetRenderState
 */
BOOL
D3DAppGetRenderState(D3DAppRenderState* lpState)
{
    memcpy(lpState, &d3dapprs, sizeof(D3DAppRenderState));
    return TRUE;
}

/*
 * D3DAppShowBackBuffer
 */

extern BOOL VSync;
BOOL D3DAppShowBackBuffer(DWORD flags)
{
	return TRUE;
#if 0 // bjd
    if (!d3dappi.bRenderingIsOK) {
        D3DAppISetErrorString("Cannot call D3DAppShowBackBuffer while bRenderingIsOK is FALSE.\n");
        return FALSE;
    }

	// requested to clear the screen.
    if ( ! flags )
		FSClear();
		
	// we are paused do not render
	if ( d3dappi.bPaused )
        return TRUE;

	if (d3dappi.bFullscreen) {

		// Flip the back and front buffers
		LastError = d3dappi.lpFrontBuffer->lpVtbl->Flip(
														d3dappi.lpFrontBuffer,
														d3dappi.lpBackBuffer,
														//DDFLIP_STEREO |	// flip between the left/right buffers for 3d
														( VSync ? // command line says?
															DDFLIP_WAIT : // wait for the buffers to flip before continuing
															DDFLIP_NOVSYNC ) // starts filling buffer even if last fill not finished...
														);

		if (LastError == DDERR_SURFACELOST) {

			d3dappi.lpFrontBuffer->lpVtbl->Restore(d3dappi.lpFrontBuffer);
			d3dappi.lpBackBuffer->lpVtbl->Restore(d3dappi.lpBackBuffer);
			FSClear();

		} else if (LastError != DD_OK) {

			D3DAppISetErrorString("Flipping complex display surface failed.\n%s", D3DAppErrorToString(LastError));
			return TRUE; //FALSE;

		}

	} else {
		RECT front;
		RECT buffer;

		// fullscreen back buffer
		SetRect(&buffer, 0, 0, d3dappi.szClient.cx,	d3dappi.szClient.cy);

		// new window mode front buffer
		SetRect(&front,
				d3dappi.pClientOnPrimary.x, d3dappi.pClientOnPrimary.y,
				d3dappi.szClient.cx + d3dappi.pClientOnPrimary.x,
				d3dappi.szClient.cy + d3dappi.pClientOnPrimary.y);

		// Blt the list of rectangles from the back to front buffer
		LastError =	d3dappi.lpFrontBuffer->lpVtbl->Blt(
											d3dappi.lpFrontBuffer,
											&front,
											d3dappi.lpBackBuffer,
											&buffer,
											//DDBLT_ASYNC | // fills fifo's asychrnously
											//DDBLT_DONOTWAIT, // does not wait but returns DDERR_WASSTILLDRAWING
											DDBLT_WAIT, // waits for previous blitter to finish drawing
											NULL
											);

		if ( LastError == DD_OK )
			return TRUE;

		switch(LastError)
		{

		case DDERR_SURFACELOST:
			d3dappi.lpFrontBuffer->lpVtbl->Restore(d3dappi.lpFrontBuffer);
			d3dappi.lpBackBuffer->lpVtbl->Restore(d3dappi.lpBackBuffer);
			FSClear();
			break;

		case DDERR_WASSTILLDRAWING:
			break;
			
		// we will crash now
		default:
			D3DAppISetErrorString("Blt of back buffer to front buffer failed.\n%s", D3DAppErrorToString(LastError));
			return FALSE;
		}
	}

    return TRUE;
#endif
}


/*
 * D3DAppClearScreenOnly
 */
BOOL
D3DAppClearScreenOnly()
{
	d3dappi.lpD3DDevice->lpVtbl->Clear( d3dappi.lpD3DDevice, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	return TRUE;
#if 0 // bjd
    int clearflags;
    D3DRECT dummy;

    if (!d3dappi.bRenderingIsOK)
	{
        D3DAppISetErrorString("Cannot call D3DAppClearBackBuffer while bRenderingIsOK is FALSE.\n");
        return FALSE;
    }
    /*
     * clear just back buffer
     */
	clearflags = D3DCLEAR_TARGET;

	if( clearflags != 0 )
	{
		dummy.x1 = dummy.y1 = 0;
		dummy.x2 = d3dappi.szClient.cx;
		dummy.y2 = d3dappi.szClient.cy;
		LastError =d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
														1, &dummy,
		                                                clearflags);
		if (LastError != D3D_OK)
		{
		    D3DAppISetErrorString("Viewport clear failed.\n%s",
		                          D3DAppErrorToString(LastError));
		    return FALSE;
		}
	}
    return TRUE;
#endif
}


/*
 * D3DAppCheckForLostSurfaces
 */
#define CHECKSURF(x) if (x) {                                               \
                        if (x->lpVtbl->IsLost(x) == DDERR_SURFACELOST) {    \
                            LastError = x->lpVtbl->Restore(x);              \
                            if (LastError != DD_OK) goto exit_with_error;   \
                            b = TRUE;                                       \
                        }                                                   \
                     }
BOOL
D3DAppCheckForLostSurfaces(void)
{
	return TRUE;
#if 0 // bjd
    BOOL b = FALSE;
    /*
     * Check all the surfaces D3DApp owns and restore them if lost.
     */
    CHECKSURF(d3dappi.lpFrontBuffer);
    CHECKSURF(d3dappi.lpBackBuffer);
    CHECKSURF(d3dappi.lpZBuffer);
    if (b) {
        /*
         * If any of the surfaces were lost and restored, clear all the buffers.
         * If this fails, that's fine, just move on.
         */
        FSClear();
    }
    return TRUE;

exit_with_error:
    D3DAppISetErrorString("Restoring of a lost surface failed.\n%s",
                          D3DAppErrorToString(LastError));
    return FALSE;
#endif
}

/*
 * D3DAppPause
 */
BOOL
D3DAppPause(BOOL flag)
{
	return TRUE;
#if 0 // bjd
    static int pausecount;

    /*
     * Keep a count of the number of times D3DAppPause has been called to 
     * prevent pausing more than once in a row.
     */
    if (pausecount != 0) {
        if (flag) {
            ++pausecount;
            return TRUE;
        } else {
            --pausecount;
            if (pausecount != 0)
                return TRUE;
        } 
    }

    d3dappi.bPaused = flag;
    if (!flag) {
        /*
         * Returning from a pause
         */
#if 0 // bjd
        if (d3dappi.bFullscreen && bPrimaryPalettized && lpPalette) {
            /*
             * Set front buffer's palette back to what it was before pause
             */
            LastError = lpPalette->lpVtbl->SetEntries(lpPalette, 0, 0, 256,
                                                      &ppe[0]);
            if (LastError != DD_OK) {
                D3DAppISetErrorString("Setting palette entries during unpause failed.\n%s", D3DAppErrorToString(LastError));
                goto exit_with_error;
            }
		}
#endif
    }
    if (flag && d3dappi.bFullscreen) {
        /*
         * Pausing in a fullscreen mode
         */
#if 0 // bjd
        if (bPrimaryPalettized && lpPalette) {
            /*
             * Save the front buffer's current palette and restore the
             * original Windows palette.
             */
            int i;
            LastError = lpPalette->lpVtbl->GetEntries(lpPalette, 0, 0, 256,
                                                      &ppe[0]);
            if (LastError != DD_OK) {
                D3DAppISetErrorString("Getting palette entries before a pause failed.\n%s", D3DAppErrorToString(LastError));
                goto exit_with_error;
            }
            for (i = 10; i < 246; i++)
                Originalppe[i] = ppe[i];
            LastError = lpPalette->lpVtbl->SetEntries(lpPalette, 0, 0, 256,
                                                      &Originalppe[0]);
            if (LastError != DD_OK) {
                D3DAppISetErrorString("Returning palette entries to defaults failed.\n%s", D3DAppErrorToString(LastError));
                goto exit_with_error;
            }
#endif
        }
        /*
         * Flip to GDI surface (either front or back buffer)
         */
#if 0 // bjd
        if (d3dappi.lpDD) 
		{
//        if (d3dappi.bIsPrimary && d3dappi.lpDD) 
		  {
            LastError = d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);
            if (LastError != DD_OK) 
			{
                D3DAppISetErrorString("Flipping to GDI surface failed.\n%s", D3DAppErrorToString(LastError));
                goto exit_with_error;
            }
          }
        /*
         * Draw the menu and frame
         */
        DrawMenuBar(d3dappi.hwnd);
        RedrawWindow(d3dappi.hwnd, NULL, NULL, RDW_FRAME);
        }
#endif
    return TRUE;
exit_with_error:
    return FALSE;
#endif
}

/*
 * D3DAppCreateSurface
 */

#if 0 //bjd
BOOL
D3DAppCreateSurface(DDSURFACEDESC *ddsd, LPDIRECTDRAWSURFACE *lplpSurf)
{
    return D3DAppICreateSurface(ddsd, lplpSurf);
}
#endif

/*
 * D3DAppLastError
 */
HRESULT
D3DAppLastError(void)
{
    return LastError;  
}

/*
 * D3DAppLastD3DAppISetErrorString
 */
char*
D3DAppLastErrorString(void)
{
    return LastErrorString;
}


/*
 * D3DAppDestroy
 */
BOOL
D3DAppDestroy(void)
{
    /*
     * Destroys all objects including Direct Draw.
     */
    d3dappi.bRenderingIsOK = FALSE;
    d3dappi.hwnd = NULL;
    ATTEMPT(D3DAppICallDeviceDestroyCallback());

    RELEASE(d3dappi.lpD3DDevice);
	RELEASE(d3dappi.lpD3D);
//    RELEASE(d3dappi.lpZBuffer);
//    RELEASE(lpPalette);
//    RELEASE(lpClipper);
//    RELEASE(d3dappi.lpBackBuffer);
//    RELEASE(d3dappi.lpFrontBuffer);
    if (d3dappi.bFullscreen) {
//        D3DAppIRestoreDispMode();
 //       D3DAppISetCoopLevel(d3dappi.hwnd, FALSE);
    }
    D3DAppIReleasePathList();
//    RELEASE(d3dappi.lpDD);
    return TRUE;
exit_with_error:
    return FALSE;
}


/*
 * D3DAppErrorToString
 */
char*
D3DAppErrorToString(HRESULT error)
{
	return "Errors not defined yet.\0";
#if 0 // bjd
    switch(error) 
	{

	    //

        case DD_OK:
            return "No error.\0";

	    // A

        case DDERR_ALREADYINITIALIZED:
            return "This object is already initialized.\0";

	    // B

        case D3DERR_BADMAJORVERSION:
            return "D3DERR_BADMAJORVERSION\0";
        case D3DERR_BADMINORVERSION:
            return "D3DERR_BADMINORVERSION\0";
        case DDERR_BLTFASTCANTCLIP:
            return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";

	    // C

        case D3DERR_CONFLICTINGTEXTUREPALETTE:
            return "D3DERR_CONFLICTINGTEXTUREPALETTE\0";
        case D3DERR_CONFLICTINGTEXTUREFILTER:
            return "D3DERR_CONFLICTINGTEXTUREFILTER\0";
        case D3DERR_CONFLICTINGRENDERSTATE:
            return "D3DERR_CONFLICTINGRENDERSTATE\0";
        case DDERR_CANNOTATTACHSURFACE:
            return "This surface can not be attached to the requested surface.\0";
        case DDERR_CANNOTDETACHSURFACE:
            return "This surface can not be detached from the requested surface.\0";
        case DDERR_CANTCREATEDC:
            return "Windows can not create any more DCs.\0";
        case DDERR_CANTDUPLICATE:
            return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
        case DDERR_CLIPPERISUSINGHWND:
            return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
        case DDERR_COLORKEYNOTSET:
            return "No src color key specified for this operation.\0";
        case DDERR_CURRENTLYNOTAVAIL:
            return "Support is currently not available.\0";
        case D3DERR_COLORKEYATTACHED:
            return "D3DERR_COLORKEYATTACHED\0";

		// D

        case DDERR_DIRECTDRAWALREADYCREATED:
            return "A DirectDraw object representing this driver has already been created for this process.\0";

	    // E

        case DDERR_EXCEPTION:
            return "An exception was encountered while performing the requested operation.\0";
        case DDERR_EXCLUSIVEMODEALREADYSET:
            return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
        case D3DERR_EXECUTE_LOCKED:
            return "D3DERR_EXECUTE_LOCKED\0";
        case D3DERR_EXECUTE_NOT_LOCKED:
            return "D3DERR_EXECUTE_NOT_LOCKED\0";
        case D3DERR_EXECUTE_CREATE_FAILED:
            return "D3DERR_EXECUTE_CREATE_FAILED\0";
        case D3DERR_EXECUTE_DESTROY_FAILED:
            return "D3DERR_EXECUTE_DESTROY_FAILED\0";
        case D3DERR_EXECUTE_LOCK_FAILED:
            return "D3DERR_EXECUTE_LOCK_FAILED\0";
        case D3DERR_EXECUTE_UNLOCK_FAILED:
            return "D3DERR_EXECUTE_UNLOCK_FAILED\0";
        case D3DERR_EXECUTE_FAILED:
            return "D3DERR_EXECUTE_FAILED\0";
        case D3DERR_EXECUTE_CLIPPED_FAILED:
            return "D3DERR_EXECUTE_CLIPPED_FAILED\0";

	    // F

	    // G

        case DDERR_GENERIC:
            return "Generic failure.\0";

	    // H

        case DDERR_HEIGHTALIGN:
            return "Height of rectangle provided is not a multiple of reqd alignment.\0";
        case DDERR_HWNDALREADYSET:
            return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
        case DDERR_HWNDSUBCLASSED:
            return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";

	    // I

        case D3DERR_INVALIDMATRIX:
            return "D3DERR_INVALIDMATRIX.\0";
        case D3DERR_NOTINBEGINSTATEBLOCK:
            return "D3DERR_NOTINBEGINSTATEBLOCK.\0";
        case D3DERR_INBEGINSTATEBLOCK:
            return "D3DERR_INBEGINSTATEBLOCK.\0";
        case D3DERR_INVALIDSTATEBLOCK:
            return "D3DERR_INVALIDSTATEBLOCK.\0";
        case D3DERR_INVALIDVERTEXFORMAT:
            return "D3DERR_INVALIDVERTEXFORMAT.\0";
        case DDERR_IMPLICITLYCREATED:
            return "This surface can not be restored because it is an implicitly created surface.\0";
        case DDERR_INCOMPATIBLEPRIMARY:
            return "Unable to match primary surface creation request with existing primary surface.\0";
        case DDERR_INVALIDCAPS:
            return "One or more of the caps bits passed to the callback are incorrect.\0";
        case DDERR_INVALIDCLIPLIST:
            return "DirectDraw does not support the provided cliplist.\0";
        case DDERR_INVALIDDIRECTDRAWGUID:
            return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
        case DDERR_INVALIDMODE:
            return "DirectDraw does not support the requested mode.\0";
        case DDERR_INVALIDOBJECT:
            return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
        case DDERR_INVALIDPARAMS:
            return "One or more of the parameters passed to the function are incorrect.\0";
        case DDERR_INVALIDPIXELFORMAT:
            return "The pixel format was invalid as specified.\0";
        case DDERR_INVALIDPOSITION:
            return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
        case DDERR_INVALIDRECT:
            return "Rectangle provided was invalid.\0";
		case D3DERR_INVALID_DEVICE:
            return "D3DERR_INVALID_DEVICE.\0";
		case D3DERR_INITFAILED:
            return "D3DERR_INITFAILED.\0";
		case D3DERR_INVALIDCURRENTVIEWPORT:
            return "D3DERR_INVALIDCURRENTVIEWPORT.\0";
		case D3DERR_INVALIDPRIMITIVETYPE:
            return "D3DERR_INVALIDPRIMITIVETYPE.\0";
		case D3DERR_INVALIDVERTEXTYPE:
            return "D3DERR_INVALIDVERTEXTYPE.\0";
		case D3DERR_TEXTURE_BADSIZE:
            return "D3DERR_TEXTURE_BADSIZE.\0";
		case D3DERR_INVALIDRAMPTEXTURE:
            return "D3DERR_INVALIDRAMPTEXTURE.\0";
		case D3DERR_INVALIDPALETTE:
            return "D3DERR_INVALIDPALETTE.\0";
		case D3DERR_INBEGIN:
            return "D3DERR_INBEGIN.\0";

	    // J
	
		// K

		// L

        case D3DERR_LIGHT_SET_FAILED:
            return "D3DERR_LIGHT_SET_FAILED\0";
        case D3DERR_LIGHTHASVIEWPORT:
            return "D3DERR_LIGHTHASVIEWPORT\0";
        case D3DERR_LIGHTNOTINTHISVIEWPORT:
            return "D3DERR_LIGHTNOTINTHISVIEWPORT\0";
        case DDERR_LOCKEDSURFACES:
            return "Operation could not be carried out because one or more surfaces are locked.\0";	

		// M

        case D3DERR_MATRIX_CREATE_FAILED:
            return "D3DERR_MATRIX_CREATE_FAILED\0";
        case D3DERR_MATRIX_DESTROY_FAILED:
            return "D3DERR_MATRIX_DESTROY_FAILED\0";
        case D3DERR_MATRIX_SETDATA_FAILED:
            return "D3DERR_MATRIX_SETDATA_FAILED\0";
        case D3DERR_MATERIAL_CREATE_FAILED:
            return "D3DERR_MATERIAL_CREATE_FAILED\0";
        case D3DERR_MATERIAL_DESTROY_FAILED:
            return "D3DERR_MATERIAL_DESTROY_FAILED\0";
        case D3DERR_MATERIAL_SETDATA_FAILED:
            return "D3DERR_MATERIAL_SETDATA_FAILED\0";
        case D3DERR_MATRIX_GETDATA_FAILED:
            return "D3DERR_MATRIX_GETDATA_FAILED\0";

	    // N

        case D3DERR_NOCURRENTVIEWPORT:
            return "D3DERR_NOCURRENTVIEWPORT\0";
        case D3DERR_VIEWPORTHASNODEVICE:
            return "D3DERR_VIEWPORTHASNODEVICE\0";
        case D3DERR_VIEWPORTDATANOTSET:
            return "D3DERR_VIEWPORTDATANOTSET\0";
        case D3DERR_NOVIEWPORTS:
            return "D3DERR_NOVIEWPORTS\0";
        case D3DERR_NOTINBEGIN:
            return "D3DERR_NOTINBEGIN\0";
        case DDERR_NO3D:
            return "There is no 3D present.\0";
        case DDERR_NOALPHAHW:
            return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
        case DDERR_NOBLTHW:
            return "No blitter hardware present.\0";
        case DDERR_NOCLIPLIST:
            return "No cliplist available.\0";
        case DDERR_NOCLIPPERATTACHED:
            return "No clipper object attached to surface object.\0";
        case DDERR_NOCOLORCONVHW:
            return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
        case DDERR_NOCOLORKEY:
            return "Surface doesn't currently have a color key\0";
        case DDERR_NOCOLORKEYHW:
            return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
        case DDERR_NOCOOPERATIVELEVELSET:
            return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
        case DDERR_NODC:
            return "No DC was ever created for this surface.\0";
        case DDERR_NODDROPSHW:
            return "No DirectDraw ROP hardware.\0";
        case DDERR_NODIRECTDRAWHW:
            return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
        case DDERR_NOEMULATION:
            return "Software emulation not available.\0";
        case DDERR_NOEXCLUSIVEMODE:
            return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
        case DDERR_NOFLIPHW:
            return "Flipping visible surfaces is not supported.\0";
        case DDERR_NOGDI:
            return "There is no GDI present.\0";
        case DDERR_NOHWND:
            return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
        case DDERR_NOMIRRORHW:
            return "Operation could not be carried out because there is no hardware present or available.\0";
        case DDERR_NOOVERLAYDEST:
            return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
        case DDERR_NOOVERLAYHW:
            return "Operation could not be carried out because there is no overlay hardware present or available.\0";
        case DDERR_NOPALETTEATTACHED:
            return "No palette object attached to this surface.\0";
        case DDERR_NOPALETTEHW:
            return "No hardware support for 16 or 256 color palettes.\0";
        case DDERR_NORASTEROPHW:
            return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
        case DDERR_NOROTATIONHW:
            return "Operation could not be carried out because there is no rotation hardware present or available.\0";
        case DDERR_NOSTRETCHHW:
            return "Operation could not be carried out because there is no hardware support for stretching.\0";
        case DDERR_NOT4BITCOLOR:
            return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
        case DDERR_NOT4BITCOLORINDEX:
            return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
        case DDERR_NOT8BITCOLOR:
            return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
        case DDERR_NOTAOVERLAYSURFACE:
            return "Returned when an overlay member is called for a non-overlay surface.\0";
        case DDERR_NOTEXTUREHW:
            return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
        case DDERR_NOTFLIPPABLE:
            return "An attempt has been made to flip a surface that is not flippable.\0";
        case DDERR_NOTFOUND:
            return "Requested item was not found.\0";
        case DDERR_NOTLOCKED:
            return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
        case DDERR_NOTPALETTIZED:
            return "The surface being used is not a palette-based surface.\0";
        case DDERR_NOVSYNCHW:
            return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
        case DDERR_NOZBUFFERHW:
            return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
        case DDERR_NOZOVERLAYHW:
            return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";

	    // O

        case DDERR_OUTOFCAPS:
            return "The hardware needed for the requested operation has already been allocated.\0";
        case DDERR_OUTOFMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OUTOFVIDEOMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OVERLAYCANTCLIP:
            return "The hardware does not support clipped overlays.\0";
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            return "Can only have ony color key active at one time for overlays.\0";
        case DDERR_OVERLAYNOTVISIBLE:
            return "Returned when GetOverlayPosition is called on a hidden overlay.\0";

	    // P

        case DDERR_PALETTEBUSY:
            return "Access to this palette is being refused because the palette is already locked.\0";
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            return "This process already has created a primary surface.\0";

	    // Q

		// R

        case DDERR_REGIONTOOSMALL:
            return "Region passed to Clipper::GetClipList is too small.\0";

	    // S

		case D3DERR_STENCILBUFFER_NOTPRESENT:
			return "D3DERR_STENCILBUFFER_NOTPRESENT\n";
		case D3DERR_SCENE_IN_SCENE:
			return "D3DERR_SCENE_IN_SCENE\n";
		case D3DERR_SCENE_BEGIN_FAILED:
			return "D3DERR_SCENE_BEGIN_FAILED\n";
		case D3DERR_SCENE_END_FAILED:
			return "D3DERR_SCENE_END_FAILED\n";
		case D3DERR_SCENE_NOT_IN_SCENE:
			return "D3DERR_SCENE_NOT_IN_SCENE\n";
        case DDERR_SURFACEALREADYATTACHED:
            return "This surface is already attached to the surface it is being attached to.\0";
        case DDERR_SURFACEALREADYDEPENDENT:
            return "This surface is already a dependency of the surface it is being made a dependency of.\0";
        case DDERR_SURFACEBUSY:
            return "Access to this surface is being refused because the surface is already locked.\0";
        case DDERR_SURFACEISOBSCURED:
            return "Access to surface refused because the surface is obscured.\0";
        case DDERR_SURFACELOST:
            return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
        case DDERR_SURFACENOTATTACHED:
            return "The requested surface is not attached.\0";
        case D3DERR_SETVIEWPORTDATA_FAILED:
            return "D3DERR_SETVIEWPORTDATA_FAILED\0";
        case D3DERR_SURFACENOTINVIDMEM:
            return "D3DERR_SURFACENOTINVIDMEM\0";

	    // T

        case D3DERR_TOOMANYOPERATIONS:
            return "D3DERR_TOOMANYOPERATIONS\0";
        case D3DERR_TOOMANYPRIMITIVES:
            return "D3DERR_TOOMANYPRIMITIVES\0";
        case D3DERR_TOOMANYVERTICES:
            return "D3DERR_TOOMANYVERTICES\0";
        case DDERR_TOOBIGHEIGHT:
            return "Height requested by DirectDraw is too large.\0";
        case DDERR_TOOBIGSIZE:
            return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
        case DDERR_TOOBIGWIDTH:
            return "Width requested by DirectDraw is too large.\0";
        case D3DERR_TEXTURE_NO_SUPPORT:
            return "D3DERR_TEXTURE_NO_SUPPORT\0";
        case D3DERR_TEXTURE_NOT_LOCKED:
            return "D3DERR_TEXTURE_NOT_LOCKED\0";
        case D3DERR_TEXTURE_LOCKED:
            return "D3DERR_TEXTURELOCKED\0";
        case D3DERR_TEXTURE_CREATE_FAILED:
            return "D3DERR_TEXTURE_CREATE_FAILED\0";
        case D3DERR_TEXTURE_DESTROY_FAILED:
            return "D3DERR_TEXTURE_DESTROY_FAILED\0";
        case D3DERR_TEXTURE_LOCK_FAILED:
            return "D3DERR_TEXTURE_LOCK_FAILED\0";
        case D3DERR_TEXTURE_UNLOCK_FAILED:
            return "D3DERR_TEXTURE_UNLOCK_FAILED\0";
        case D3DERR_TEXTURE_LOAD_FAILED:
            return "D3DERR_TEXTURE_LOAD_FAILED\0";
        case D3DERR_TEXTURE_SWAP_FAILED:
            return "D3DERR_TEXTURE_SWAP_FAILED\0";
        case D3DERR_TEXTURE_GETSURF_FAILED:
            return "D3DERR_TEXTURE_GETSURF_FAILED\0";

	    // U

        case D3DERR_UNSUPPORTEDTEXTUREFILTER:
            return "D3DERR_UNSUPPORTEDTEXTUREFILTER\0";
        case D3DERR_UNSUPPORTEDFACTORVALUE:
            return "D3DERR_UNSUPPORTEDFACTORVALUE\0";
        case D3DERR_UNSUPPORTEDCOLOROPERATION:
            return "D3DERR_UNSUPPORTEDCOLOROPERATION\0";
        case D3DERR_UNSUPPORTEDCOLORARG:
            return "D3DERR_UNSUPPORTEDCOLORARG\0";
        case D3DERR_UNSUPPORTEDALPHAOPERATION:
            return "D3DERR_UNSUPPORTEDALPHAOPERATION\0";
        case D3DERR_UNSUPPORTEDALPHAARG:
            return "D3DERR_UNSUPPORTEDALPHAARG\0";
        case DDERR_UNSUPPORTED:
            return "Action not supported.\0";
        case DDERR_UNSUPPORTEDFORMAT:
            return "FOURCC format requested is unsupported by DirectDraw.\0";
        case DDERR_UNSUPPORTEDMASK:
            return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";

	    // V

        case DDERR_VERTICALBLANKINPROGRESS:
            return "Vertical blank is in progress.\0";
        case D3DERR_VERTEXBUFFEROPTIMIZED:
            return "D3DERR_VERTEXBUFFEROPTIMIZED\0";
        case D3DERR_VBUF_CREATE_FAILED:
            return "D3DERR_VBUF_CREATE_FAILED\0";
        case D3DERR_VERTEXBUFFERLOCKED:
            return "D3DERR_VERTEXBUFFERLOCKED\0";
        case D3DERR_VERTEXBUFFERUNLOCKFAILED:
            return "D3DERR_VERTEXBUFFERUNLOCKFAILED\0";

	    // W

        case D3DERR_WRONGTEXTUREFORMAT:
            return "D3DERR_WRONGTEXTUREFORMAT.\0";
        case DDERR_WASSTILLDRAWING:
            return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
        case DDERR_WRONGMODE:
            return "This surface can not be restored because it was created in a different mode.\0";

	    // X

        case DDERR_XALIGN:
            return "Rectangle provided was not horizontally aligned on required boundary.\0";

	    // Y
				
		// Z

        case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY:
            return "D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY.\0";
        case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY:
            return "D3DERR_ZBUFF_NEEDS_VIDEOMEMORY.\0";
        case D3DERR_ZBUFFER_NOTPRESENT:
            return "D3DERR_ZBUFFER_NOTPRESENT.\0";

	    //

        default:
            return "Unrecognized error value.\0";
    }
#endif
}
