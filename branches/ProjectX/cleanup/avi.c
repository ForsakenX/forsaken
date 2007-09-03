/*
 * a v i . c 
 *
 * AVI playback Routines
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 21 $
 *
 * $Header: /PcProjectX/avi.c 21    11/04/98 19:04 Philipy $
 *
 * $Log: /PcProjectX/avi.c $
 * 
 * 21    11/04/98 19:04 Philipy
 * fixed AVI bug
 * fixed stripping bug
 * lobby launched game now goes to next level after stats
 * 
 * 20    7/04/98 17:50 Philipy
 * removed multiplayer taunts
 * AVI thread now allowed to exit nicely rather than being terminated
 * fixed inter-level bug
 * fixed bug in enemy taunts
 * 
 * 19    3/20/98 10:36a Phillipd
 * 
 * 18    19/03/98 20:29 Philipy
 * added debug messages to try and track down AVI crash bug
 * 
 * 17    19/03/98 11:23 Philipy
 * implemented new acclaim splash screen plus AVI
 * 
 * 16    7/03/98 15:25 Philipy
 * re-implemented -NoAVI option
 * 
 * 15    20/02/98 15:28 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 14    1/27/98 2:58p Phillipd
 * 
 * 13    23/01/98 11:24 Collinsd
 * Added override for solid screen poly bilinear
 * 
 * 12    12/01/98 0:08 Philipy
 * bug fixes
 * added inter-level mission briefing
 * changed splash screen code, + added splash screen on exit
 * 
 * 11    14/11/97 17:34 Philipy
 * 
 * 10    5/11/97 9:59 Oliverc
 * Add new collision function WouldObjectCollide()
 * 
 * 9     4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 8     27/10/97 10:39 Philipy
 * if'd out AVI display on poly routines
 * compile with AVI_UsePolys if needed
 * 
 * 7     23/10/97 16:49 Philipy
 * added tggle (number key 1) for playing AVI on texture.
 * (no texture conversion yet, could appear corrupted)
 * 
 * 6     23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 5     21/10/97 13:06 Philipy
 * AVI now no longer sends main game thread to sleep
 * 
 * 4     17/10/97 15:13 Philipy
 * 
 * 3     17/10/97 14:23 Philipy
 */

#include "main.h"

#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "d3dmain.h"
#include "d3dapp.h"
#include "AVIresource.h"
#include "vfw.h"
#include "aviresource.h"
#include "ddraw.h"
#include "2dtextures.h"
#include "controls.h"
#include "dinput.h"
#include "mload.h"
#include "camera.h"
#include "screenpolys.h"
#include "tload.h"
#include "avi.h"

LPDIRECTDRAWSURFACE		Lp_AVI_DDSurface;
BOOL ExitAVIThread = FALSE;

AVISTREAMINFO VidInfo;
int AVI_Letterbox;	// the height of top and bottom borders 
#define AVI_FullScreenHeight 240	// what the AVI full screen height would be if black borders were included.
int AVI_Mode;

extern  FRAME_INFO	*	AVI_Poly_Header;
extern  FRAME_INFO	*	AVI_Poly_Header2;
extern	D3DAppInfo d3dappi;
extern VECTOR View, Look, Up;
extern CAMERA	CurrentCamera;
extern	D3DVIEWPORT viewport;
extern SCRPOLY	ScrPolys[];
extern TLOADHEADER	Tloadheader;
extern	D3DMATRIX view;
extern float framelag;	
extern	BOOL	BilinearSolidScrPolys;
extern BOOL bSoundEnabled;

#ifdef SOFTWARE_ENABLE
extern BOOL	SoftwareVersion;
#endif

extern LPDIRECT3DEXECUTEBUFFER lpD3DNormCmdBuf;
extern LPDIRECT3DEXECUTEBUFFER RenderBufs[];

/***********************************
AVI functions
***********************************/
BOOL MovieFileOpen (HWND hwnd, LPSTR lpszPathName);
void MovieSetOptionsBitdepth (HWND, int);
void MovieSetOptionsResolution (HWND hwnd, int id, int xres, int yres);
void MovieSetOptionsPalette (HWND, int);
void MovieInitialize (HWND hwnd);
void MoviePlay (HWND hwnd);
void MoviePause (HWND hwnd);
void MovieTerminate (HWND hwnd);
void MovieFileClose (HWND hwnd);
void MovieUpdateStatusBarContents (HWND hwnd);

void InitFont( BOOL OverridePolytext );
void DebugPrintf( const char * format, ... );
void Build_View();
BOOL	ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer );

/***********
AVI
************/
BOOL PlayingAVI = FALSE;

/*************************************************************************
AVI Stuff
*************************************************************************/

BOOL AVIfileOpen (HWND hwnd, char * file)
{
   DWORD		DDSurfWidth;
   DWORD		DDSurfHeight;
   DDSURFACEDESC       ddsd;
    char szDrive[ _MAX_DRIVE ] ;
	char szDir[ _MAX_DIR ];
	char szFname[ _MAX_FNAME ];
	char szExt[ _MAX_EXT ];
	DWORD currenttime;

	MovieInitialize (myglobs.hWndMain);
	
	if (file != NULL )
	{
		_splitpath( file, szDrive, szDir, szFname, szExt );
		// Try to open the Movie
		if (!MovieFileOpen (hwnd, file))
      		return FALSE;

		// Initialize Menus
		MovieSetOptionsBitdepth (hwnd, d3dapp->ThisMode.bpp);  // set bitdepth option to last set
		MovieSetOptionsPalette(hwnd,ID_OPTIONS_PALETTE_NATIVE); // set to native palette
		//
	}
	// set the letterbox size...
	switch (AVI_Mode)
	{
	case AVI_MODE_Letterbox:
		AVI_Letterbox = (d3dapp->ThisMode.h - (VidInfo.rcFrame.bottom - VidInfo.rcFrame.top) * d3dapp->ThisMode.h / AVI_FullScreenHeight) / 2;
		break;
	default:
		AVI_Letterbox = 0;
	}

	//create a direct draw surface...
   	DDSurfWidth = VidInfo.rcFrame.right -  VidInfo.rcFrame.left;
   	DDSurfHeight = VidInfo.rcFrame.bottom -  VidInfo.rcFrame.top;
   
	ZeroMemory(&ddsd, sizeof(ddsd)); 
	ddsd.dwSize = sizeof(ddsd); 

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
	ddsd.dwWidth = DDSurfWidth; 
	ddsd.dwHeight = DDSurfHeight; 

	if (d3dapp->lpDD->lpVtbl->CreateSurface(d3dapp->lpDD, &ddsd, &Lp_AVI_DDSurface, NULL) != DD_OK) 
	{
		Msg("Title.c: AVIfileOpen() unable to create Direct Draw Surface");
		exit(1);
	}

	currenttime = timeGetTime();
	while ( timeGetTime() < ( currenttime + 1000 ) );
	
	MoviePlay ( hwnd );

	return TRUE;
}

void AVIShutDown ( HWND hwnd )
{
	MovieFileClose (hwnd);
	DebugPrintf("about to do MovieTerminate()\n");
	MovieTerminate (hwnd);
	DebugPrintf("about to release AVI surface\n");
	RELEASE (Lp_AVI_DDSurface);
}

uint16 AVIScreenPoly[2];

void InitAVI( char *FileNamePtr )
{
	D3DAppIClearBuffers();
	
	d3dappi.bRenderingIsOK = TRUE;
	InitFont(FALSE);
	
	if (!AVIfileOpen (myglobs.hWndMain, FileNamePtr))
		AviFinished();
	else
	{
		if ( bSoundEnabled )
			DestroySound( DESTROYSOUND_All );
		PlayingAVI = TRUE;
	}
}

void ReleaseAVI( void )
{
	AVIShutDown ( myglobs.hWndMain );
}

void AviFinished( void )
{
	ExitAVIThread = TRUE;
}

