#include "typedefs.h"
#include "d3dappi.h"
#include "main.h"
#include "title.h"

BOOL Wine = FALSE;

extern void SetInputAcquired( BOOL );
extern void SetCursorClip( BOOL );
extern BOOL	DrawPanel;
extern void ProcessTextItems (void);
extern BOOL cursor_clipped;
extern int ignore_mouse_input;
extern BOOL CanRenderWindowed;
extern void DebugPrintf( char *fmt, ... );
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
 * List of texture handles which is copied to D3DAppInfo structure when
 * necessary
 */

void InitModeCase(void);

BOOL bD3DAppInitialized;       /* Is D3DApp initialized? */
BOOL bIgnoreWM_SIZE = FALSE;   /* Ignore this WM_SIZE messages */
BOOL bFullscreen = TRUE;	   /* Fullscreen flag from cli */
SIZE szLastClient;             /* Dimensions of the last window */
SIZE szBuffers;                /* Current buffer dimensions, not necessarily
                                  the same as the client window */

/***************************************************************************/
/*                               FUNCTIONS                                 */
/***************************************************************************/

/*
 * D3DAppFullscreen
 */
BOOL D3DAppFullscreen(int mode)
{
	return TRUE;
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

BOOL D3DAppWindow(int w, int h, int bpp)
{
	return TRUE;
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
					FlipBuffers();
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
				// TODO does d3d do the resizing on it's own now?

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
            FlipBuffers();
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
}
