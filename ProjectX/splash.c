/*
 * a v i . c 
 *
 * AVI playback Routines
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 40 $
 *
 * $Header: /PcProjectX/splash.c 40    10/08/98 17:33 Philipy $
 *
 * $Log: /PcProjectX/splash.c $
 * 
 * 40    10/08/98 17:33 Philipy
 * rewrote AVI player
 * 
 * 39    29/04/98 17:54 Philipy
 * fixed splash screen bug when booting in 1024x768
 * 
 * 38    11/04/98 19:32 Collinsd
 * 
 * 37    11/04/98 19:04 Philipy
 * fixed AVI bug
 * fixed stripping bug
 * lobby launched game now goes to next level after stats
 * 
 * 36    10/04/98 19:54 Philipy
 * added time limit for end game demos
 * 
 * 35    8/04/98 20:26 Collinsd
 * 
 * 34    8/04/98 14:34 Philipy
 * renamed end game demos
 * 
 * 33    8/04/98 10:19 Philipy
 * AVI is now looked for locally first, then on CD ( FINAL_RELEASE )
 * 
 * 32    7/04/98 17:50 Philipy
 * removed multiplayer taunts
 * AVI thread now allowed to exit nicely rather than being terminated
 * fixed inter-level bug
 * fixed bug in enemy taunts
 * 
 * 31    7/04/98 11:01 Philipy
 * potentially fixed crash when going from AVI to titles
 * fixed CD audio looping
 * no CD audio in front end unless full install
 * bike features sliders now give correct values
 * 
 * 30    5/04/98 15:01 Philipy
 * started pre AVI CD accesss ( not yet implemented )
 * bike engine freq now done on 5 frame average
 * prevented CD track from playing in titles if set to off
 * NoDynamic SFX does not bike bike computer static anymore
 * water detail slider now only has two levels
 * 
 * 29    4/04/98 22:23 Collinsd
 * Fixed forsakenanykey
 * 
 * 28    4/04/98 19:01 Oliverc
 * Added "Forsaken/Press any key" to attract mode demo playback
 * 
 * 27    4/04/98 17:02 Philipy
 * correct res splash screen now displayed
 * 
 * 26    4/04/98 11:19 Oliverc
 * Attract mode splash demo now plays from CD if not found installed
 * Software screen res now stored separately from HW in registry
 * Registry keys now opened in-game as creation done by installer
 * 
 * 25    3/04/98 18:10 Collinsd
 * Moved position of xmem.h
 * 
 * 24    3/04/98 13:13 Philipy
 * Taunts are now affected by biker volume slider ( taunt volume slider
 * removed )
 * Enemy bikers now give out death cry
 * fixed problem with speech ignoring volume settings
 * fixed end game sequences
 * 
 * 23    30/03/98 20:36 Collinsd
 * Done end game stuff, Added more credits and made sfx for capship non
 * looping.
 * 
 * 22    29/03/98 20:00 Philipy
 * cd path now verified earlier
 * sfx no longer reloaded when changing biker / bike computer
 * mouse sensitivity rounding error fixed
 * 
 * 21    28/03/98 17:33 Philipy
 * corrected some sfx
 * added legal screen
 * fixed mission briefing text bug
 * 
 * 20    28/03/98 14:54 Collinsd
 * Updated
 * 
 * 19    27/03/98 19:06 Philipy
 * water always at max detail for splash screen
 * 
 * 18    26/03/98 9:15 Collinsd
 * ZBuffer 
 * 
 * 17    24/03/98 22:04 Collinsd
 * 
 * 16    24/03/98 21:49 Philipy
 * temporarily prevent splash screen demos for sw version
 * 
 * 15    24/03/98 21:07 Philipy
 * fixed quicktext stuff
 * sfx do not pause when in multiplayer mode
 * rear camera not shown for splash demos
 * 
 * 14    24/03/98 18:01 Collinsd
 * Added ZBuffer clear for title/splash
 * 
 * 13    24/03/98 15:46 Philipy
 * changed water fade stuff
 * 
 * 12    24/03/98 12:14 Collinsd
 * Added credits as splash screen.
 * 
 * 11    20/03/98 10:27 Philipy
 * 
 * 10    19/03/98 20:32 Philipy
 * added different end of game scenarios
 * code now written to config to indicate if secret biker is available
 * 
 * 9     19/03/98 11:29 Philipy
 * implemented new acclaim splash screen plus AVI
 * 
 * 8     17/03/98 15:58 Philipy
 * water fade now rest to 1 on post demo splash
 * 
 * 7     16/03/98 16:40 Philipy
 * fixed buffered key problem
 * added AVI to splash screens
 * 
 * 6     15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 5     7/03/98 15:25 Philipy
 * re-implemented -NoAVI option
 * 
 * 4     23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 3     20/02/98 15:58 Philipy
 * removed splash bitmaps for version
 * 
 * 2     20/02/98 15:29 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 1     20/02/98 12:23 Philipy
 * 
 */

#include "typedefs.h"
#include "splash.h"
#include "title.h"
#include "ddutil.h"
#include <stdio.h>
#include "file.h"
#include "main.h"
#include "sfx.h"
#include "d3dmain.h"
#include "avi.h"
#include "text.h"
#include "credits.h"
#include "xmem.h"
#include "ddsurfhand.h"
#include "d3dappi.h"
#include "showstrm.h"

CRITICAL_SECTION	gPlayKey; // Guards one variable, gbPlay
extern int LogosEnable;
extern BOOL ForsakenAnyKey;
extern int use_data_path;
extern char data_path[];
extern char normdata_path[];

BOOL LoadSplashBitmap( void *VarPtr );
BOOL DisplayBitmap( void *Ptr );
void KillSplashBitmap( void *Ptr );
void ReInitDisplayAfterBitmap( void *Ptr );
void PostGameCompleteSplash( void *Ptr );
BOOL InitSplashDemo( void *Ptr );
BOOL PlaySplashDemo( void *Ptr );
void PostSplashDemo( void *Ptr );
BOOL StartSplashDemo( char *demofile, char *levels );
BOOL InitSplashAVI( void *Ptr );
BOOL PlaySplashAVI( void *Ptr );
void PostSplashAVI( void *Ptr );
BOOL MovieThreadProc (void * pParm);
void InitModeCase(void);
BOOL MainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
void SpecialDestroyGame( void );
void ReleaseLevel(void);
BOOL D3DAppIClearBuffers(void);
BOOL InitAttractDemo( void *Ptr );
char *DemoFileName( char *demoname );
void DebugPrintf( const char * format, ... );

#ifdef SOFTWARE_ENABLE
void CWClearZBuffer( void );
extern	char	CWInTitle;
#endif

extern BYTE	MyGameStatus;
extern float WATER_CELLSIZE;
extern int GameCompleted;

// Possibly not all needed?
extern int32 DemoGameLoops;
extern SLIDER DemoSpeed;
extern float Demoframelag;
extern float Oldframelag;	
extern float framelag; 
extern	BOOL	PauseDemo;
extern BYTE Current_Camera_View;
extern SLIDER DemoEyesSelect;
extern BOOL	PlayDemo;
extern int AVI_Mode;
extern int CameraStatus;
extern	BOOL CreditsToggle;

extern float WaterFade;
extern LIST	DemoList;
extern	FILE	*	DemoFp;
extern BOOL PreventFlips;

#ifdef SOFTWARE_ENABLE
extern	BOOL	SoftwareVersion;
#endif


LPDIRECTDRAWSURFACE lpDDS_NewSplash = NULL;
BOOL LimitedLoad = FALSE;
uint16 CurrentAttractDemo = 0;
HANDLE AVIEvent;

SPLASHSCREENINFO BitmapSplashInfo = {
	LoadSplashBitmap,	// pre splash function
	NULL,	// variable for pre splash function
	DisplayBitmap,	// splash display function
	(void *)&lpDDS_NewSplash,	// variable for splash display function
	KillSplashBitmap,	// post splash function
	NULL,	// variable for post splash function
};

SPLASHSCREENINFO LastBitmapSplashInfo = {
	LoadSplashBitmap,	// pre splash function
	NULL,	// variable for pre splash function
	DisplayBitmap,	// splash display function
	(void *)&lpDDS_NewSplash,	// variable for splash display function
	ReInitDisplayAfterBitmap,	// post splash function
	NULL,	// variable for post splash function
};

SPLASHSCREENINFO LimitedDemoSplashInfo = {
	InitSplashDemo,	// pre splash function
	(void *)&LimitedLoad,	// variable for pre splash function
	PlaySplashDemo,	// splash display function
	(void *)&LimitedLoad,	// variable for splash display function
	PostSplashDemo,	// post splash function
	(void *)&LimitedLoad,	// variable for post splash function
};

SPLASHSCREENINFO DemoSplashInfo = {
	InitSplashDemo,	// pre splash function
	NULL,	// variable for pre splash function
	PlaySplashDemo,	// splash display function
	NULL,	// variable for splash display function
	PostSplashDemo,	// post splash function
	NULL,	// variable for post splash function
};

SPLASHSCREENINFO AVISplashInfo = {
	InitSplashAVI,	// pre splash function
	NULL,	// variable for pre splash function
	PlaySplashAVI,	// splash display function
	NULL,	// variable for splash display function
	PostSplashAVI,	// post splash function
	NULL,	// variable for post splash function
};

SPLASHSCREENINFO AttractSplashInfo = {
	InitAttractDemo,	// pre splash function
	NULL,	// variable for pre splash function
	PlaySplashDemo,	// splash display function
	NULL,	// variable for splash display function
	PostSplashDemo,	// post splash function
	NULL,	// variable for post splash function
};

NEWSPLASHSCREENS NewSplashScreens[MAX_SPLASH_SCREENS] = {
	{ SPLASH_TYPE_Bitmap, SPLASH_Timed | SPLASH_ShowNext, 5000, "le", &BitmapSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Timed  | SPLASH_ShowNext, 15000, "acclaim", &LimitedDemoSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Timed | SPLASH_ShowNext, 15000, "probe", &LimitedDemoSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return, 10000, "main", &AVISplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Dummy, 10000, "fs", &BitmapSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Timed, 20000, "ed1", &DemoSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Timed, 20000, "ed2", &DemoSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return, 10000, "", &AttractSplashInfo },	
	{ SPLASH_TYPE_Bitmap, SPLASH_Return | SPLASH_Timed, 330000, "probe", &LimitedDemoSplashInfo },	
};

BOOL NoAVI = FALSE;
BOOL InSplashDemo = FALSE;
BYTE PreSplash_MyGameStatus;
MENU *PreSplash_Menu = NULL;
MENUITEM *PreSplash_MenuItem = NULL;
int NewCurrentSplashScreen;
char CurrentSplashFile[256];
char *SplashScreenPath = "data\\splash\\";

char *SplashScreenSuffix[ 7 ] = {
	"320200", "320240", "320400", "512384", "640400", "640480", "800600"
};
DWORD SplashStartTime;
DWORD SplashFinishTime;

BOOL LoadSplashBitmap( void *VarPtr )
{
	int currentmode;
    LPDIRECTDRAWPALETTE ddpal;

	/*
	// do not want this as there is no splash screen for this res
	if( d3dappi.szClient.cx >= 1024 && d3dappi.szClient.cy >= 768 )
		currentmode = Mode1024X768;
	else 
	*/
	if( d3dappi.szClient.cx >= 800 && d3dappi.szClient.cy >= 600 )
		currentmode = Mode800X600;
	else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 480 )
		currentmode = Mode640X480;
	else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 400 )
		currentmode = Mode640X400;
	else if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
		currentmode = Mode512X384;
	else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 400 )
		currentmode = Mode320X400;
	else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 240 )
		currentmode = Mode320X240;
	else
		currentmode = Mode320X200;

	if ( currentmode < 0 )
		currentmode = 0;
	do
	{
		strcpy( CurrentSplashFile, SplashScreenPath );
		strcat( CurrentSplashFile, NewSplashScreens[ NewCurrentSplashScreen ].filename );
		strcat( CurrentSplashFile, SplashScreenSuffix[ currentmode ] );

		strcat( CurrentSplashFile, ".bmp" );

		currentmode--;

	}while ( !File_Exists( CurrentSplashFile ) && ( currentmode >= 0 ) );

	lpDDS_NewSplash = DDLoadBitmap( d3dapp->lpDD, CurrentSplashFile, 0, 0 );

	if ( !lpDDS_NewSplash )
	{
		return FALSE;
			
	}else
	{
		ddpal =  DDLoadPalette( d3dapp->lpDD , CurrentSplashFile);
		IDirectDrawSurface3_SetPalette( lpDDS_NewSplash , ddpal );
	}

	return TRUE;
}

void ShowSplashScreen( int num )
{
	// store current game status
	PreSplash_MyGameStatus = MyGameStatus;

	// change game status
	MyGameStatus = STATUS_SplashScreen;

	// store menu status, and set all menus to null ( so that WhichKeyPressed() works )
	PreSplash_Menu = CurrentMenu;
	PreSplash_MenuItem = CurrentMenuItem;
	CurrentMenu = NULL;
	CurrentMenuItem = NULL;

	NewCurrentSplashScreen = num;

	if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_Dummy )
	{
		MyGameStatus = STATUS_FinishedShowingSplashScreen;
	}else
	{
		// call initialization function
		if ( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashFunc )
		{
			if ( !NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashFunc( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashVar ) )
			{
				MyGameStatus = STATUS_FinishedShowingSplashScreen;
			}
		}

		// set finish time if required
		if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_Timed )
		{
			SplashStartTime = timeGetTime();
			SplashFinishTime = SplashStartTime + NewSplashScreens[ num ].time;
		}
	}
}

BOOL DisplayBitmap( void *Ptr )
{
	DDBLTFX fx;
	HRESULT ddrval;
	LPDIRECTDRAWSURFACE *lplpdds;
	LPDIRECTDRAWSURFACE lpdds;

	lplpdds = Ptr;
	lpdds = *lplpdds;

	memset(&fx, 0, sizeof(DDBLTFX));
	fx.dwSize = sizeof(DDBLTFX);

	while( 1 )
	{
		ddrval = IDirectDrawSurface3_Blt( d3dapp->lpBackBuffer, NULL, lpdds, NULL, DDBLT_WAIT , &fx );
		if( ddrval == DD_OK )
			break;
		if( ddrval == DDERR_SURFACELOST )
		{
			IDirectDrawSurface3_Restore(d3dapp->lpFrontBuffer);
			IDirectDrawSurface3_Restore(d3dapp->lpBackBuffer);
			DDReLoadBitmap( lpdds, CurrentSplashFile );
			break;
		}
		if( ddrval != DDERR_WASSTILLDRAWING )
			break;
	}

	return TRUE;
}

void KillSplashBitmap( void *Ptr )
{
	if ( lpDDS_NewSplash )
		ReleaseDDSurf( lpDDS_NewSplash );
}

void ReInitDisplayAfterBitmap( void *Ptr )
{
	KillSplashBitmap( NULL );

	InitScene();
	
	if ( !InitView())
	{
		Msg( "Oct2.c - ReInitDisplayAfterBitmap(): InitView failed\n" );
	}

}

void PostGameCompleteSplash( void *Ptr )
{
	// TEMP  - TAKE OUT WHEN DEMO IS PLAYED!!!
	KillSplashBitmap( NULL );
	
	MenuAbort();
	MenuRestart( &MENU_Start );
				
	InitScene();
	InitView();
	DestroySound( DESTROYSOUND_All );
	InitializeSound( DESTROYSOUND_All );
}

static int old_LogosEnable = 0;

BOOL InitAttractDemo( void *Ptr )
{
#ifdef SOFTWARE_ENABLE
	CWInTitle = 0;	// last minute bodge as witnessed by Dave and Olly
#endif

	InSplashDemo = FALSE;	// will only be set to TRUE if demo launches succesfully )

	strcpy( CurrentSplashFile, DemoFileName( DemoList.item[ CurrentAttractDemo ] ) );

	if ( !File_Exists( CurrentSplashFile )  )
	{
			return FALSE;
	}
	old_LogosEnable = LogosEnable;
	LogosEnable = 1;
 	if ( !StartSplashDemo( CurrentSplashFile, MULTIPLAYER_LEVELS ) )
	{
		LogosEnable = old_LogosEnable;
		return FALSE;
	}

	InSplashDemo = TRUE;
	SetupForsakenAnyKey();
	ForsakenAnyKey = TRUE;

	return TRUE;
}

// flag is set to true ( if given ) to indicate that we are showing water effect splash screen
// and therefore require a limited load, plus a fade in / out effect

float WaterDetailStore;

BOOL InitSplashDemo( void *Ptr )
{
	BOOL *flag;

#ifdef SOFTWARE_ENABLE
	CWInTitle = 0;	// last minute bodge as witnessed by Dave and Olly
#endif

	InSplashDemo = FALSE;	// will only be set to TRUE if demo launches succesfully )
	
	if ( Ptr )
	{
		flag = ( BOOL *)Ptr;
		*flag = TRUE;

		WaterDetailStore = WATER_CELLSIZE;
		WATER_CELLSIZE = 64.0F;
	}
	
	
	strcpy( CurrentSplashFile, SplashScreenPath );
	strcat( CurrentSplashFile, NewSplashScreens[ NewCurrentSplashScreen ].filename );

	strcat( CurrentSplashFile, ".dmo" );

	if ( !File_Exists( CurrentSplashFile )  )
		return FALSE;
	else
	{
	 	if ( !StartSplashDemo( CurrentSplashFile, SPLASH_LEVELS ) )
		{
			return FALSE;
		}
	}

	InSplashDemo = TRUE;
	return TRUE;
}

BOOL PlaySplashDemo( void *Ptr )
{
	LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
	LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;
	DWORD CurrentTime;
	BOOL *flag;

	DisplayCredits();

	flag = (BOOL *)Ptr;

	if ( !DemoGameLoops++ )
	{
		// if this demo is being used for splash screen, timer to start from here...
		SplashStartTime = timeGetTime();
		SplashFinishTime = SplashStartTime + NewSplashScreens[ NewCurrentSplashScreen ].time;
	}

	if( DemoSpeed.value > 8 )
	{
		// slower or normal playback speed...
		Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
	}else{
		Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
	}
	
//		Demoframelag = 10.0F;
	Oldframelag = framelag;

	if( PauseDemo )
	{
		framelag = 0.0F;
	}else{
		framelag *= Demoframelag;
	}

#if 0
	DemoEyesSelect.value = 0;
#endif
	Current_Camera_View = DemoEyesSelect.value;		// which object is currently using the camera view....

//	framelag *= Demoframelag;

	if ( flag && *flag )
	{
		WaterFade = 1.0F;
		CurrentTime = timeGetTime();
		if ( CurrentTime <= ( SplashStartTime + WATER_FADE_TIME ) )
			WaterFade = ( (float)( CurrentTime - SplashStartTime ) / (float)WATER_FADE_TIME );

		if ( CurrentTime > ( SplashFinishTime - WATER_FADE_TIME ) )
			WaterFade = ((float)( SplashFinishTime - CurrentTime ) / (float)WATER_FADE_TIME);

	}

	if( MainGame( lpDev , lpView ) != TRUE )
		return FALSE;

	if ( !PlayDemo )
		return FALSE;

	return TRUE;

}

void PostSplashDemo( void *Ptr )
{
	BYTE tempstatus;
	BOOL *flag;

	ReleaseCredits();
	GameCompleted = GAMECOMPLETE_NotComplete;	// otherwise subsequent demos could display all bikers as current bike
	
	if ( Ptr )
	{
		flag = ( BOOL *)Ptr;
		*flag = FALSE;

		WATER_CELLSIZE = WaterDetailStore;
	}

	if ( DemoFp )
	{
		fclose( DemoFp );
		DemoFp = NULL;
	}

	tempstatus = MyGameStatus;

	D3DAppIClearBuffers();
	WaterFade = 1.0F;

	SpecialDestroyGame();
	StopCompoundSfx();
	if ( InSplashDemo )	// if demo actually was played
	{
		ReleaseView();
		ReleaseLevel();
	}

	PreventFlips = FALSE;
	MyGameStatus = tempstatus;

	LimitedLoad = FALSE;
	InSplashDemo = FALSE;
	LogosEnable = old_LogosEnable;
	ForsakenAnyKey = FALSE;

	InitScene();
	
	if ( !InitView())
	{
		Msg( "Oct2.c - PostSplashDemo(): InitView failed\n" );
	}

	if ( CurrentMenu )
	{
		LastMenu = CurrentMenu;
		VduClear();
		MenuAbort();
	}
	CameraStatus = CAMERA_AtStart;
}

BOOL InitSplashAVI( void *Ptr )
{

	if ( NoAVI )
		return FALSE;

	D3DAppIClearBuffers();
	
	InitModeCase();
#ifdef FINAL_RELEASE
	// look on hard drive, if not there default to CD
	sprintf( CurrentSplashFile, "%ssplash\\%s.avi", data_path, NewSplashScreens[ NewCurrentSplashScreen ].filename );
	if ( !File_Exists ( CurrentSplashFile ) )
	{
		sprintf( CurrentSplashFile, "%ssplash\\%s.avi", normdata_path, NewSplashScreens[ NewCurrentSplashScreen ].filename );
	}
#else
	if( use_data_path )
	{
		// look on data path, if not there default to normal path
		sprintf( CurrentSplashFile, "%ssplash\\%s.avi", data_path, NewSplashScreens[ NewCurrentSplashScreen ].filename );
		if ( !File_Exists ( CurrentSplashFile ) )
		{
			sprintf( CurrentSplashFile, "%ssplash\\%s.avi", normdata_path, NewSplashScreens[ NewCurrentSplashScreen ].filename );
		}
	}else
	{
		// use normal data path
		sprintf( CurrentSplashFile, "%ssplash\\%s.avi", normdata_path, NewSplashScreens[ NewCurrentSplashScreen ].filename );
	}
#endif

//	AVI_Mode = AVI_MODE_FullScreen;
	
	if ( !File_Exists( CurrentSplashFile )  )
		return FALSE;
	else
	{
	    IMultiMediaStream *pMMStream;
		HRESULT hr;
	    hr = OpenMMStream(CurrentSplashFile, d3dappi.lpDD, &pMMStream);
	    if (SUCCEEDED(hr)) 
		{
			RenderStreamToSurface(d3dappi.lpDD, d3dapp->lpFrontBuffer, pMMStream);
			D3DAppIClearBuffers();
			pMMStream->lpVtbl->Release( pMMStream );
		}

	 //	InitAVI( CurrentSplashFile );
	}

	return TRUE;
}

BOOL PlaySplashAVI( void *Ptr )
{
	return FALSE;
	/*
	AVIEvent = CreateEvent( NULL, TRUE,	// requires manual reset
				   FALSE, // initially non-signalled
					NULL ); 

	WaitForSingleObjectEx( AVIEvent, INFINITE, FALSE );

	EnterCriticalSection (&gPlayKey);

	DebugPrintf("about to return FALSE from PlaySplashAVI()\n");
	return FALSE;	// to indicate finished playing
	*/
}

void PostSplashAVI( void *Ptr )
{
	/*
	AviFinished();

	LeaveCriticalSection (&gPlayKey);

	DebugPrintf("about to do ReleaseAVI()\n");
	ReleaseAVI();
	DebugPrintf("ReleaseAVI() done\n");
	*/
	
	InitScene();
	
	if ( !InitView())
	{
		Msg( "Oct2.c - PostSplashDemo(): InitView failed\n" );
	}
}

