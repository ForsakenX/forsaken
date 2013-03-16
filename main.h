#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#ifdef WIN32
#define WIN32_EXTRA_LEAN
#endif

/*===================================================================
	Game Status Defines...
===================================================================*/

#define STATUS_UNUSED  0
#define	STATUS_Title									2	// On the Title Screen...
#define	STATUS_StartingMultiplayer						4	// On the Multiplayer Starting Screen...
#define	STATUS_PreStartingMultiplayerSynch				5	// Waiting for everyone to have loaded the right level and be ready
#define	STATUS_StartingMultiplayerSynch					6	// Waiting for everyone to have loaded the right level and be ready
							// to Start...

#define	STATUS_QuitCurrentGame							7	// About to quit a game and return to the title screen..
#define	STATUS_GetPlayerNum								8	// Trying to aquire a Player Number...
#define	STATUS_PlayingDemo								9	// Playing Back a Demo...
#define	STATUS_Normal 									10	// normal in game...
#define	STATUS_LevelEnd									11	// the level has ended tell the host I know the level has ended...
#define	STATUS_BetweenLevels    						12	// in title room between levels

#define	STATUS_ViewingScore								13	// tell the host I am looking at the Score....
#define	STATUS_WaitingAfterScore						14	// waiting for other players or waitng for the host to start...
#define	STATUS_Joining									15	// I Want to join your game....
#define	STATUS_Left										16	// I have left your game...
#define	STATUS_ChangeLevelPostInitView					17	// after a change level init view
#define	STATUS_ChangeLevelPostPlayingDemo				18	// after a change level Playing Demo...

#define	STATUS_StartingSinglePlayer						32	// Starting Single Player in game...
#define	STATUS_PostStartingSinglePlayer					33	// Starting Single Player in game after the level has been loaded...
#define	STATUS_SinglePlayer								34	// Single Player in game...
#define	STATUS_ViewingStats								35	// Viewing Stats Single Player...

#define	STATUS_TitleLoadGameStartingSinglePlayer		36
#define	STATUS_TitleLoadGamePostStartingSinglePlayer	37
#define	STATUS_ChangeLevelPostLoadGame					38
#define	STATUS_InGameLoadGameStartingSinglePlayer		39

#define	STATUS_WaitingToStartSinglePlayer				48	// waiting to start single player game...
#define	STATUS_WaitingToStartMultiPlayerHost			49	// waiting to start multi player game for host...
#define	STATUS_WaitingToStartMultiPlayerClient			50	// waiting to start multi player game for client...
#define	STATUS_WaitingToStartTeamGame					51	// waiting to start multi player game for client...
#define STATUS_QuittingFromTitles						52	// quitting game from title screen
#define STATUS_WaitingToStartDemo						53	// waiting to start demo
#define STATUS_WaitingToStartEndGameSequence			54	// waiting to start end of game demo playback sequence

#define	STATUS_InitView_0								128	// 0 of Preping for a level..
#define	STATUS_InitView_1								129	// 1 of Preping for a level..
#define	STATUS_InitView_2								130	// 2 of Preping for a level..
#define	STATUS_InitView_3								131	// 3 of Preping for a level..
#define	STATUS_InitView_4								132	// 4 of Preping for a level..
#define	STATUS_InitView_5								133	// 5 of Preping for a level..
#define	STATUS_InitView_6								134	// 6 of Preping for a level..
#define	STATUS_InitView_7								135	// 7 of Preping for a level..
#define	STATUS_InitView_8								136	// 8 of Preping for a level..
#define	STATUS_InitView_9								137	// 9 of Preping for a level..
#define	STATUS_InitView_10								138	// 10 of Preping for a level..

#define STATUS_LeftCrashed								254	// I have left and crashed...
#define STATUS_Null										255	// Im not doing anything yet...

#define STATUS_VALID_PLAYER( x ) \
	(x != STATUS_GetPlayerNum && x != STATUS_Joining && x != STATUS_Left && \
	 x != STATUS_LeftCrashed && x != STATUS_Null && x != STATUS_UNUSED)

/*===================================================================
	Some Global Defines...
===================================================================*/

#define START_FOV			(90.0F) // initial horizontal field-of-view (in degrees)
#define MAX_FOV				(120.0F) // max horizontal field-of-view (in degrees)

#define MAXLEVELS			(64)
#define	MAGIC_NUMBER		0x584A5250

#define	GLOBAL_SCALE			(0.25F)

typedef struct
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} rect_t;

typedef struct
{
    long  x;
    long  y;
} point_t;

#undef MAX
#define MAX(x, y) ((x) > (y)) ? (x) : (y)

#undef MIN
#define MIN(x, y) ((x) > (y)) ? (y) : (x)

#undef ZERO_STACK_MEM
#define ZERO_STACK_MEM(x) memset(&x, 0, sizeof(x))

#ifdef WIN32
#include <string.h>
#define strcasecmp	_stricmp
#define strncasecmp _strnicmp
#define strdup _strdup
#include <stdio.h>
#define snprintf	_snprintf
#include <direct.h>
#define chdir		_chdir
// _strupr use strtoupper see util.h
#endif

#ifdef DEBUG_ON
#ifndef _SVID_SOURCE
#define _SVID_SOURCE
#endif
#include <errno.h>
#include <math.h>
#endif

// c99 boolean support
#include <stdbool.h>

// on osx trying to include malloc will not work
// this brings in malloc on linux and osx
#include <stdlib.h>

// xmem also needs strdup to be pulled in first
#include <string.h>

// should be included after malloc/stdlib
#include "xmem.h"

// really crappy support right now for mingw32
// since apparently it doesn't work with u_ types
// support for this coming soon in a newer release
#ifdef WIN32
#include <stdint.h>
typedef uint8_t  u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
#include <windef.h>
#else
// winapi compatibility
typedef u_int32_t DWORD;
#endif
typedef u_int8_t  BYTE;
typedef u_int16_t WORD;
 
#endif	// MAIN_INCLUDED
