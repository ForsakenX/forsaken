/*
 * a v i . c 
 *
 * AVI playback Routines
 *
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 10 $
 *
 * $Header: /PcProjectX/Include/Splash.h 10    5/04/98 15:01 Philipy $
 *
 * $Log: /PcProjectX/Include/Splash.h $
 * 
 * 10    5/04/98 15:01 Philipy
 * bike engine freq now done on 5 frame average
 * NoDynamic SFX does not bike bike computer static anymore
 * water detail slider now only has two levels
 * 
 * 9     28/03/98 17:33 Philipy
 * corrected some sfx
 * added legal screen
 * fixed mission briefing text bug
 * 
 * 8     24/03/98 15:46 Philipy
 * changed water fade stuff
 * 
 * 7     24/03/98 12:14 Collinsd
 * Added credits as splash screen.
 * 
 * 6     19/03/98 20:32 Philipy
 * added different end of game scenarios
 * code now written to config to indicate if secret biker is available
 * 
 * 5     16/03/98 16:40 Philipy
 * fixed buffered key problem
 * added AVI to splash screens
 * 
 * 4     15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 3     20/02/98 15:58 Philipy
 * removed splash bitmaps for version
 * 
 * 2     20/02/98 15:30 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 1     20/02/98 12:29 Philipy
 * 
 */

#ifndef	SPLASH_INCLUDED
#define	SPLASH_INCLUDED

#include	"d3d.h"
#include "typedefs.h"

#define SPLASH_TYPE_Bitmap	0
#define SPLASH_TYPE_AVI		1

#define SPLASH_Timed 1
#define SPLASH_Return 2
#define SPLASH_ShowNext 4
#define SPLASH_Dummy 8

#define MAX_SPLASH_SCREENS 16

#define WATER_FADE_TIME 3000

#if 0
enum{
	SPLASHSCREEN_Initial0,
	SPLASHSCREEN_Initial1,
	SPLASHSCREEN_Initial2,
	SPLASHSCREEN_Initial3,
	SPLASHSCREEN_Initial4,
	SPLASHSCREEN_Initial5,
	SPLASHSCREEN_Quitting,
	SPLASHSCREEN_GameComplete,
	SPLASHSCREEN_Credits,
};
#else
enum{
	SPLASHSCREEN_Legal,
	SPLASHSCREEN_Initial0,
	SPLASHSCREEN_Initial1,
	SPLASHSCREEN_Avi,
	SPLASHSCREEN_Quitting,
	SPLASHSCREEN_GameComplete,
	SPLASHSCREEN_SpecialGameComplete,
	SPLASHSCREEN_AttractMode,
	SPLASHSCREEN_Credits,
};
#endif

typedef void (* SplashFunc)(void *);
typedef BOOL (* SplashOKFunc)(void *);

typedef struct{
	SplashOKFunc	PreSplashFunc;
	void		*PreSplashVar;
	SplashOKFunc	SplashDisplayFunc;
	void		*SplashDisplayVar;
	SplashFunc	PostSplashFunc;
	void		*PostSplashVar;
} SPLASHSCREENINFO;

typedef struct{
	uint8	type;	// bitmap or AVI
	int		flags;	// SPLASH_Timed, SPLASH_Return
	DWORD	time;	// milliseconds;
	char	*filename;
	SPLASHSCREENINFO	*splashinfo;
} NEWSPLASHSCREENS;

void ShowSplashScreen( int num );

#endif
