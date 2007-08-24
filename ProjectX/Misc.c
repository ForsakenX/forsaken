/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 18 $
 *
 * $Header: /PcProjectX/Misc.c 18    11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/Misc.c $
 * 
 * 18    11/11/98 16:00 Philipy
 * various fixes for warnings / errors when compiling under VC6
 * 
 * 17    5/01/98 19:58 Oliverc
 * Current screen mode, texture format and sound volume settings now saved
 * to registry on exit and restored on startup
 * 
 * 16    16/09/97 11:16 Oliverc
 * Changed "HAL driver required" message to something a bit more
 * informative
 * 
 * 15    1-08-97 11:03a Philipy
 * fixed start-up res problem
 * 
 * 14    31/07/97 15:57 Oliverc
 * Added special SELF_PLAY features, including disabling critical unused
 * code and setting default values appropriate to demo attract mode
 * 
 * 13    5/21/97 10:57a Phillipd
 * no need to specifically go to full screen ..it should go full screen
 * first time you run
 * automatically...
 * 
 * 12    12/14/96 5:04p Phillipd
 * 
 * 11    11/13/96 9:08a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 10    10/23/96 4:24p Phillipd
 * Lots of crap taken out of D3dapp and its associated functions and
 * files....
 * 
 * 9     10/23/96 9:57a Phillipd
 * 
 * 8     14/10/96 15:02 Oliverc
 * Added command line options to select default DirectDraw device and
 * fullscreen video mode
 * 
 * 7     10/14/96 12:41p Phillipd
 * 
 * 6     9/19/96 3:33p Phillipd
 * 
 * 4     8/21/96 4:17p Phillipd
 * 
 * 3     8/09/96 4:29p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*
 *  Copyright (C) 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: misc.c
 *
 *  Miscellaneous functions not involving DD and D3D.  Part of D3DApp.
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

int default_width	= 0;
int default_height	= 0;
int default_bpp		= 0;


/***************************************************************************/
/*                          Setting Defaults                               */
/***************************************************************************/
/*
 * D3DAppISetDefaults
 * Set all the global variables to their default values.  Do not reset the
 * image files.
 */
void
D3DAppISetDefaults(void)
{
    ZEROMEM(d3dappi);
    ZEROMEM(d3dapprs);
    d3dapprs.bZBufferOn = TRUE;
    d3dapprs.bPerspCorrect = TRUE;
    d3dapprs.ShadeMode = D3DSHADE_GOURAUD;
    d3dapprs.TextureFilter = D3DFILTER_LINEAR;
    d3dapprs.TextureBlend = D3DTBLEND_MODULATE;
    d3dapprs.FillMode = D3DFILL_SOLID;
    d3dapprs.bDithering = TRUE;
    d3dapprs.bSpecular = FALSE;
    d3dapprs.bAntialiasing = FALSE;
    d3dapprs.bFogEnabled = FALSE;
    d3dapprs.FogColor = RGB_MAKE(0, 0, 0);
    d3dapprs.FogMode = D3DFOG_LINEAR;
    d3dapprs.FogStart = D3DVAL(256.0*1.0F);
    d3dapprs.FogEnd = D3DVAL(1024.0*3.0F);

    lpClipper = NULL;
    lpPalette = NULL;
    bPrimaryPalettized = FALSE;
    bPaletteActivate = FALSE;
    bIgnoreWM_SIZE = FALSE;
    ZEROMEM(ppe);
    ZEROMEM(Originalppe);
    LastError = DD_OK;
    ZEROMEM(LastErrorString);
    D3DDeviceDestroyCallback = NULL;
    D3DDeviceDestroyCallbackContext = NULL;
    D3DDeviceCreateCallback = NULL;
    D3DDeviceCreateCallbackContext = NULL;
}

/***************************************************************************/
/*                Calling Device Create And Destroy Callbacks              */
/***************************************************************************/
BOOL
D3DAppICallDeviceDestroyCallback(void)
{
    if (D3DDeviceDestroyCallback) {
        if (CallbackRefCount) {
            --CallbackRefCount;
            return (D3DDeviceDestroyCallback)(D3DDeviceDestroyCallbackContext);
        }
    }
    return TRUE;
}

BOOL
D3DAppICallDeviceCreateCallback(int w, int h)
{
    if (D3DDeviceCreateCallback) {
        ++CallbackRefCount;
        return (D3DDeviceCreateCallback)(w, h, &d3dappi.lpD3DViewport,
                                         D3DDeviceCreateCallbackContext);
    }
    return TRUE;
}

/***************************************************************************/
/*            Choosing and verifying the driver and display mode           */
/***************************************************************************/
/*
 * D3DAppIPickDriver
 * Choose a driver from the list of available drivers which can render to one
 * of the given depths.  Hardware is prefered.  Mono-lighting drivers are
 * prefered over RGB.
 */
BOOL
D3DAppIPickDriver(int* driver, DWORD depths)
{
    int i, j;
    j = 0;
    for (i = 0; i < d3dappi.NumDrivers; i++)
        if (d3dappi.Driver[i].Desc.dwDeviceRenderBitDepth & depths)
            break;
    if (i >= d3dappi.NumDrivers) {
        *driver = D3DAPP_BOGUS;
        return TRUE;
    }
    j = i;
    for (i = 0; i < d3dappi.NumDrivers; i++) {
        if (d3dappi.Driver[i].Desc.dwDeviceRenderBitDepth & depths) {
            if (d3dappi.Driver[i].bIsHardware &&
                                              !d3dappi.Driver[j].bIsHardware)
                                                  j = i;
            else if (d3dappi.Driver[i].bIsHardware ==
                                             d3dappi.Driver[j].bIsHardware) {
                if (d3dappi.Driver[i].Desc.dcmColorModel & D3DCOLOR_MONO &&
                    !(d3dappi.Driver[j].Desc.dcmColorModel & D3DCOLOR_MONO))
                        j = i;
            }
        }
    }
    if (j >= d3dappi.NumDrivers)
        *driver = D3DAPP_BOGUS;
    else
        *driver = j;
    return TRUE;
}

/*
 * D3DAppIFilterDisplayModes
 * Set the bThisDriverCanDo flag for each display mode if the given driver
 * can render in that depth.  Also checks to make sure there is enough
 * total video memory for front/back/z-buffer in video memory if it's a
 * hardware device.
 */
BOOL
D3DAppIFilterDisplayModes(int driver)
{
    int i;
    DWORD depths = d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth;
    DWORD TotVidMem = D3DAppTotalVideoMemory();
    for (i = 0; i < d3dappi.NumModes; i++) {
        d3dappi.Mode[i].bThisDriverCanDo = FALSE;
        if (!(D3DAppIBPPToDDBD(d3dappi.Mode[i].bpp) & depths))
            continue;
        if (d3dappi.Driver[driver].bIsHardware && TotVidMem &&
            TotVidMem < (unsigned)3 * d3dappi.Mode[i].w * d3dappi.Mode[i].h *
                        d3dappi.Mode[i].bpp / 8)
            continue;
        d3dappi.Mode[i].bThisDriverCanDo = TRUE;
            
    }
    d3dappi.ThisMode.bThisDriverCanDo =
                             d3dappi.Mode[d3dappi.CurrMode].bThisDriverCanDo;
    return TRUE;
}

/*
 * D3DAppIPickDisplayMode
 * Pick a display mode of one of the given depths.  640x480x16 is prefered.
 */
BOOL
D3DAppIPickDisplayMode(int *mode, DWORD depths)
{
    int i, j;
#if 1
	int wmin, hmin, bppmin, default_mode;

	wmin = hmin = bppmin = 9999;
	default_mode = 0;
    for (i = 0; i < d3dappi.NumModes; i++)
	{
        if ( ( D3DAppIBPPToDDBD(d3dappi.Mode[i].bpp) & depths ) &&
			d3dappi.Mode[i].bpp >= 16 &&
			bppmin >= d3dappi.Mode[i].bpp &&
			wmin >= d3dappi.Mode[i].w &&
			hmin >= d3dappi.Mode[i].h )
		{
			wmin = d3dappi.Mode[i].w;
			hmin = d3dappi.Mode[i].h;
			bppmin = d3dappi.Mode[i].bpp;
			default_mode = i;
		}
	}
	j = default_mode;
	wmin = hmin = bppmin = 9999;
#else
    for (i = 0; i < d3dappi.NumModes; i++)
        if (D3DAppIBPPToDDBD(d3dappi.Mode[i].bpp) & depths)
            break;
    j = i;
#endif
#if 1
    for (i = 0; i < d3dappi.NumModes; i++) {
#else
    for (; i < d3dappi.NumModes; i++) {
#endif
        if (!(D3DAppIBPPToDDBD(d3dappi.Mode[i].bpp) & depths))
            continue;
#if 1
		if ( ( !default_bpp || d3dappi.Mode[i].bpp == default_bpp ) &&
			 ( !default_width ||d3dappi.Mode[i].w == default_width ) &&
			 ( !default_height || d3dappi.Mode[i].h == default_height ) )
		{
			if ( !default_width || !default_height || !default_bpp )
			{
				if ( ( default_width || wmin >= d3dappi.Mode[i].w ) &&
					 ( default_height || hmin >= d3dappi.Mode[i].h ) &&
					 ( default_bpp || bppmin >= d3dappi.Mode[i].bpp ) )
				{
					if ( !default_width )
						wmin = d3dappi.Mode[i].w;
					if ( !default_height )
						hmin = d3dappi.Mode[i].h;
					if ( !default_bpp )
						bppmin = d3dappi.Mode[i].bpp;
					j = i;
				}
			}
			else
			{
				j = i;
				break;
			}
		}
#else
        if (d3dappi.Mode[i].w == default_width && d3dappi.Mode[i].h == default_height &&
            d3dappi.Mode[i].bpp == default_bpp) {
//        if (d3dappi.Mode[i].w == 320 && d3dappi.Mode[i].h == 200 &&
//            d3dappi.Mode[i].bpp == 16) {
            j = i;
            break;
        }
#endif
    }
    if (j >= d3dappi.NumModes)
        *mode = D3DAPP_BOGUS;
    else
        *mode = j;
    return TRUE;
}

BOOL	FirstTime = TRUE;
/*
 * D3DAppIVerifyDriverAndMode
 * Verifies the selected driver and mode combination.  If the driver is
 * specified, the mode will be changed to accomodate the driver if it's not
 * compatible.  If the driver is not specified, one will be selected which is
 * compatible with the specified mode.  If neither are specified, a suitable
 * pair will be returned.
 */
BOOL
D3DAppIVerifyDriverAndMode(int* lpdriver, int* lpmode)
{
    DWORD depths;
    int driver, mode, i;
    driver = *lpdriver; mode = *lpmode;

    if (mode == D3DAPP_USEWINDOW && !d3dappi.bIsPrimary) {
        D3DAppISetErrorString("Cannot render to a window when the DirectDraw device is not the primary.\n");
        goto exit_with_error;
    }

    /*
     * If I've been ask to choose a driver, choose one which is compatible
     * with the specified mode.
     */
    if (driver == D3DAPP_YOUDECIDE) {   
        if (mode == D3DAPP_USEWINDOW) {
            /*
             * I must find a driver for this display depth
             */
            depths = D3DAppIBPPToDDBD(d3dappi.WindowsDisplay.bpp);
            ATTEMPT(D3DAppIPickDriver(&driver, depths));
            if (driver == D3DAPP_BOGUS) {
                D3DAppISetErrorString("Cannot find a D3D device driver which is compatible with the current display depth.\n");
                goto exit_with_error;
            }
            /*
             * I don't need to go through the mode selection since I've
             * verified it here
             */
            goto ret_ok;
        } else if (mode == D3DAPP_YOUDECIDE) {
            /*
             * I'm free to choose any driver which can use even one
             * supported depth
             */
            if (d3dappi.bIsPrimary)
                depths = D3DAppIBPPToDDBD(d3dappi.WindowsDisplay.bpp);
            else
                depths = 0;
            for (i = 0; i < d3dappi.NumModes; i++)
                depths |= D3DAppIBPPToDDBD(d3dappi.Mode[i].bpp);
            ATTEMPT(D3DAppIPickDriver(&driver, depths));
            if (driver == D3DAPP_BOGUS) {
                D3DAppISetErrorString("Cannot find a D3D device driver which is compatible with the current display depth or any supported fullscreen mode.\n");
            }
            /*
             * The mode will be chosen in the next section
             */
        } else {
            /*
             * Must pick a driver which uses the given mode depth
             */
            ATTEMPT(D3DAppIPickDriver(&driver,
                                  D3DAppIBPPToDDBD(d3dappi.Mode[mode].bpp)));
            if (driver == D3DAPP_BOGUS) {
                D3DAppISetErrorString("Cannot find a D3D device driver which is compatible with the specified fullscreen mode.\n");
                goto exit_with_error;
            }
            /*
             * I don't need to go through the mode selection since I've
             * verified it here
             */
            goto ret_ok;
        }
    }

#ifdef SELF_PLAY
	if ( driver < 2 ) // only permit HAL for self-playing demo for hardware card manufacturers
	{
		D3DAppISetErrorString( "No D3D hardware driver found\n" );
		goto exit_with_error;
	}
#endif
    /* 
     * At this stage, I have a driver I want to match the mode to.
     */
    if (mode == D3DAPP_YOUDECIDE) {
        /*
         * If it's my choice, I prefer windowed over fullscreen
         */
        if (d3dappi.bIsPrimary && !FirstTime ) {
            if (D3DAppIBPPToDDBD(d3dappi.WindowsDisplay.bpp) & 
                    d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth) {
                mode = D3DAPP_USEWINDOW;
                goto ret_ok;
            }
        }
		FirstTime = FALSE;
        /*
         * Either this is not a primary DD device or the driver cannot use
         * the Windows display depth
         */
        ATTEMPT(D3DAppIPickDisplayMode(&mode,
                        d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth));
        if (mode == D3DAPP_BOGUS) {
            D3DAppISetErrorString("The selected D3D device driver is not compatible with the current display depth or any supported fullscreen modes.\n");
            goto exit_with_error;
        }
        goto ret_ok;
    } else if (mode == D3DAPP_USEWINDOW) {
        /*
         * Check to see if this driver can use the Windows display depth
         */
        if (D3DAppIBPPToDDBD(d3dappi.WindowsDisplay.bpp) &
                d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth) {
            goto ret_ok;
        } else {
            /*
             * Since it cannot, call this function again to choose any 
             * display mode which is compatible 
             */
            mode = D3DAPP_YOUDECIDE;
            ATTEMPT(D3DAppIVerifyDriverAndMode(&driver, &mode));
            if (driver == D3DAPP_BOGUS)
                goto exit_with_error;
            goto ret_ok;
        }
    } else {
        /*
         * Check to see if this driver can use the specified fullscreen mode
         */
        if (D3DAppIBPPToDDBD(d3dappi.Mode[mode].bpp) &
                d3dappi.Driver[driver].Desc.dwDeviceRenderBitDepth) {
            goto ret_ok;
        } else {
            /*
             * Since it cannot, call this function again to choose any
             * display mode which is compatible
             */
            mode = D3DAPP_YOUDECIDE;
            ATTEMPT(D3DAppIVerifyDriverAndMode(&driver, &mode));
            if (driver == D3DAPP_BOGUS)
                goto exit_with_error;
            goto ret_ok;
        }
    }

ret_ok:
    *lpdriver = driver; *lpmode = mode;
    return TRUE;
exit_with_error:
    return FALSE;
}


/***************************************************************************/
/*                     Getting and Setting Window Attribs                  */
/***************************************************************************/
/*
 * D3DAppISetClientSize
 * Set the client size of the given window.  A WM_SIZE message is generated,
 * but ignored.
 */
void
D3DAppISetClientSize(HWND hwnd, int w, int h, BOOL bReturnFromFullscreen)
{
    RECT rc;

    bIgnoreWM_SIZE = TRUE;
    if (bReturnFromFullscreen) {
        SetRect(&rc, 0, 0, w, h);
        AdjustWindowRectEx(&rc, GetWindowLong(hwnd, GWL_STYLE),
                           GetMenu(hwnd) != NULL,
                           GetWindowLong(hwnd, GWL_EXSTYLE));
        SetWindowPos(hwnd, NULL, 0, 0, rc.right-rc.left,
                     rc.bottom-rc.top,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    } else {
        /*
         * This is the only way to set the client size correctly if the menu
         * is stacked, so do it unless we are returning from a fullscreen
         * mode.
         */
        SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, w + ( h << 16) );
        GetWindowRect(hwnd, &rc);
        SetWindowPos(hwnd, NULL, 0, 0, rc.right-rc.left,
                     rc.bottom-rc.top,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
    bIgnoreWM_SIZE = FALSE;
    d3dappi.pClientOnPrimary.x = d3dappi.pClientOnPrimary.y = 0;
    ClientToScreen(hwnd, &d3dappi.pClientOnPrimary);
    d3dappi.szClient.cx = w; d3dappi.szClient.cy = h;
}

/*
 * D3DAppIGetClientWin
 * Gets the client window size and sets it in the D3DAppInfo structure
 */
void
D3DAppIGetClientWin(HWND hwnd)
{
    RECT rc;
    if (!d3dappi.bFullscreen) {
        d3dappi.pClientOnPrimary.x = d3dappi.pClientOnPrimary.y = 0;
        ClientToScreen(hwnd, &d3dappi.pClientOnPrimary);
        GetClientRect(hwnd, &rc);
        d3dappi.szClient.cx = rc.right;
        d3dappi.szClient.cy = rc.bottom;
    } else {
        /*
         * In the fullscreen case, we must be careful because if the window
         * frame has been drawn, the client size has shrunk and this can
         * cause problems, so it's best to report the entire screen.
         */
        d3dappi.pClientOnPrimary.x = d3dappi.pClientOnPrimary.y = 0;
        d3dappi.szClient.cx = d3dappi.ThisMode.w;
        d3dappi.szClient.cy = d3dappi.ThisMode.h;
    }
}


/***************************************************************************/
/*                              Error reporting                            */
/***************************************************************************/

/*
 * D3DAppISetErrorString
 * Set the global variable which records the last error string.
 */
void __cdecl
D3DAppISetErrorString( LPSTR fmt, ... )
{
    char buff[256];

    buff[0] = 0;
    wvsprintf(&buff[0], fmt, (char *)(&fmt+1));
    lstrcat(buff, "\r\n");
    strcpy(LastErrorString, buff);
}

/* dpf
 * Debug printf.  Very useful for fullscreen exclusive mode or when surfaces
 * are in video memory.
 */
void __cdecl
dpf( LPSTR fmt, ... )
{
    char buff[256];

    lstrcpy(buff, "D3DApp: ");
    wvsprintf(&buff[lstrlen(buff)], fmt, (char *)(&fmt+1));
    lstrcat(buff, "\r\n");
    OutputDebugString(buff);
}
