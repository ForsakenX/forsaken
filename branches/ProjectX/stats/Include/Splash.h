

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
