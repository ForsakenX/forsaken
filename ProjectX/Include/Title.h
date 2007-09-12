/*
 * The X Men, October 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 119 $
 *
 * $Header: /PcProjectX/include/Title.h 119   3/09/98 9:31 Philipy $
 *
 * $Log: /PcProjectX/include/Title.h $
 * 
 * 119   3/09/98 9:31 Philipy
 * somw Gamespy fixes
 * added Session ( GUID ) and TCP command line flags
 * added TRACKER_NAME facility
 * 
 * 118   27/08/98 20:12 Philipy
 * manual / auto server mode now selectable from menus
 * text now displayed when server in game & not rendering
 * dynamic PPS setting re-enabled for server
 * 
 * 117   26/08/98 12:07 Philipy
 * message now displayed when server boots you out due to invalid CRC
 * 
 * 116   17/08/98 17:13 Philipy
 * added -ServerChoosesGame & ServerAutoStart command line options
 * 
 * 115   14/08/98 15:25 Philipy
 * added trilinear option to menus
 * fixed level name / shutdown packet in heartbeat
 * 
 * 
 * 114   5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 113   28/07/98 14:44 Philipy
 * all server timeouts now configurable
 * 
 * 112   28/07/98 10:39 Philipy
 * Max players now works properly for server games
 * 
 * 111   23/07/98 15:38 Philipy
 * server now resets if no packets recieved for 5 mins
 * 
 * 110   15/07/98 16:25 Philipy
 * now handles pseudohost quitting in titles & server quitting ( in
 * titiles or in game )
 * 
 * 109   14/07/98 11:15 Philipy
 * various patch bugs
 * pseudohost quitting in titles
 * 
 * 108   3/07/98 17:46 Philipy
 * added quit option when using quickstart
 * 
 * 107   3/07/98 11:55 Philipy
 * heartbeat & quickstart stuff
 * 
 * 106   22/06/98 17:32 Philipy
 * move #includes around for localisations so that title text is available
 * in other files
 * 
 * 105   13/06/98 20:46 Philipy
 * improved lobby support:
 * host now migrates properly
 * you can quit b4 starting game without screwing up session
 * 
 * 104   11/06/98 16:57 Philipy
 * loads of win98 shareware version stuff
 * 
 * 103   9/06/98 11:23 Philipy
 * server now reset if pseudohost does not have any valid levels
 * fixed timed game bug for server based games
 * 
 * 102   8/06/98 12:28 Philipy
 * server levels now sent across to pseudohost. Pseudohost can only pick
 * valid levels
 * 
 * 101   20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 100   11/05/98 15:15 Philipy
 * added session info stuff ( game type, ping etc )
 * 
 * 99    24/04/98 1:37 Philipy
 * weapon order text now comes from local files
 * 
 * 98    20/04/98 17:12 Philipy
 * added localisation stuff
 * 
 * 97    13/04/98 23:38 Philipy
 * fixed inter-level crash bug
 * 
 * 96    8/04/98 18:44 Philipy
 * moved highlight box and slider stuff into processtextitems due to sw
 * version
 * 
 * 95    6/04/98 11:29 Philipy
 * added big packets option
 * upped frequency of some speech sfx
 * re-implemented holo-scanline
 * 
 * 94    29/03/98 20:00 Philipy
 * sfx no longer reloaded when changing biker / bike computer
 * mouse sensitivity rounding error fixed
 * 
 * 93    27/03/98 21:06 Philipy
 * added end game sequence for completing the game with cheating
 * 
 * 92    27/03/98 12:58 Philipy
 * changed cheat mode stuff
 * fixed bug that prevented individual variants of variant sfx to be
 * mapped
 * correct menutv now displayed between levels
 * 
 * 91    26/03/98 12:28 Philipy
 * added water detail & gamma sliders
 * fixed holo object swapping stuff
 * fixed crash bug when selecting room in debug mode
 * 
 * 90    23/03/98 10:37 Oliverc
 * Merged key and button control config
 * 
 * 89    21/03/98 16:18 Philipy
 * added gamma slider ( not yet operational)
 * fixed multiple copies of level in list bug
 * 
 * 88    19/03/98 20:32 Philipy
 * added different end of game scenarios
 * code now written to config to indicate if secret biker is available
 * 
 * 87    11/03/98 10:55 Philipy
 * all sfx now stopped for pause mode
 * correct level info now displayed in briefing screen
 * correct level now entered after completing a previously saved level
 * level timer now accurate
 * 
 * 86    6/03/98 17:37 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 85    5/03/98 21:46 Oliverc
 * Changed save games to use fixed number of slots
 * Added option to delete items from any list (currently enabled only on
 * demos)
 * 
 * 84    4/03/98 12:33 Oliverc
 * CTF mode fully enabled
 * 
 * 83    2/03/98 17:29 Oliverc
 * Added optional validation function to TEXT structure to filter
 * permitted characters for demo filenames
 * All .DMO files now kept in Demos folder
 * 
 * 82    2/03/98 12:10 Oliverc
 * Hidden ".DMO" extension from demo file names
 * and added extra info to .DMO file to specify game type etc
 * 
 * 81    27/02/98 19:11 Philipy
 * fixed load game sfx bug
 * added pseudo dithering for 8 bit saved game pic
 * flygirl now selectable from front end ( no model displayed )
 * 
 * 80    26/02/98 20:41 Philipy
 * added front end for load game
 * 
 * 79    25/02/98 16:19 Oliverc
 * More multiplayer tweaks
 * 
 * 78    24/02/98 16:56 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 77    23/02/98 13:00 Philipy
 * added scan lines for inter-level objects
 * added speech for bike selection in front end
 * 
 * 76    23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 75    20/02/98 15:30 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 74    1/31/98 1:22p Phillipd
 * 
 * 73    27/01/98 11:05 Philipy
 * fixed team game stuff
 * 
 * 72    22/01/98 22:40 Oliverc
 * Force bike in SHAREWARE demo playback to use only those bikes (and
 * corresponding player names) allowed in SHAREWARE version
 * 
 * 71    21/01/98 12:19 Philipy
 * Added attract mode for shareware
 * fixed looping sfx volume bug
 * 
 * 70    20/01/98 12:25 Philipy
 * Fixed level starting bug
 * 
 * 69    14/01/98 17:17 Oliverc
 * Separate DEMO_LEVELS for SHAREWARE version
 * 
 * 68    12/01/98 0:08 Philipy
 * bug fixes
 * added inter-level mission briefing
 * changed splash screen code, + added splash screen on exit
 * 
 * 67    7/01/98 15:03 Oliverc
 * SHAREWARE versions now use built-in level lists instead of external
 * mission.dat and battle.dat files
 * 
 * 66    7/01/98 9:34 Philipy
 * added title room sfx
 * added ability to select bike computer, biker with sfx loaded
 * 
 * 65    30/12/97 18:55 Philipy
 * new cogs
 * 
 * 64    29/12/97 19:53 Philipy
 * various bug fixes
 * 
 * 63    5/12/97 21:56 Philipy
 * fixed sessions list bug
 * 
 * 62    27/11/97 12:35 Philipy
 * fixed sound bug on self play demo
 * demo playback speed now given as true percentage
 * 
 * 61    21/11/97 12:39 Philipy
 * fixed releasing of splash screen
 * 
 * 60    4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 59    16/10/97 18:10 Philipy
 * added CreateTextureSurface function
 * 
 * 58    8/10/97 20:31 Philipy
 * 
 * 57    6-10-97 3:37p Philipy
 * Increased MAXTEXTITEMSPERMENUITEM
 * 
 * 56    17/09/97 9:55 Collinsd
 * Blitting now works in software versions in 320x240 mode.
 * 
 * 55    16/09/97 17:52 Collinsd
 * More of Chris's stuff works.
 * 
 * 54    15-09-97 3:23p Philipy
 * message displayed, then correct menu displayed on new host machine if
 * host migrates.
 * Host can now start another game, having quit 'waiting to start' menu
 * 
 * 53    14-09-97 6:57p Philipy
 * joining existing team game - you can now select which team to go into,
 * and view current scores
 * 
 * 52    5-09-97 4:12p Philipy
 * Changed menus to use buffered keys
 * 
 * 51    5-09-97 11:13a Philipy
 * demo playback stuff from new menus
 * 
 * 49    3-09-97 12:33p Philipy
 * moved Our_CalculateFrameRate to above menu processing so that demos now
 * work.
 * 
 * 48    2-09-97 2:45p Philipy
 * stopped menus from messing up when you press escape
 * 
 * 47    1-09-97 5:35p Philipy
 * changes made for 3 & 4 player team game
 * 
 * 46    26-08-97 4:19p Philipy
 * most new menu versions of current menus now implemented
 * 
 * 45    14-08-97 5:43p Philipy
 * added ability for any number of splash screens & properties
 * 
 * 44    14-08-97 3:55p Philipy
 * 
 * 43    6-08-97 4:47p Philipy
 * Added auto select text flag
 * 
 * 42    6-08-97 11:35a Philipy
 * sliders and lists for VDU
 * 
 * 41    31-07-97 3:47p Philipy
 * started detail level menus, plus additional stuff for self play mode
 * 
 * 40    29-07-97 11:02a Philipy
 * more SELF_PLAY stuff
 * 
 * 39    28-07-97 4:46p Philipy
 * SELF_PLAY option stuff
 * 
 * 38    24-07-97 5:10p Philipy
 * 
 * 37    22-07-97 5:42p Philipy
 * loads of animation stuff switched over to new event system
 * 
 * 36    17-07-97 3:49p Philipy
 * Started to create TITLE_EVENT structure and processes
 * 
 * 35    16-07-97 4:11p Philipy
 * added holo-bikes and scan lines
 * 
 * 34    9-07-97 8:23p Philipy
 * changed so that only one VDU font is loaded at a time
 * 
 * 33    9-07-97 11:37a Philipy
 * changed VDU text stuff, added test bike
 * 
 * 32    25-06-97 9:15a Philipy
 * added 2nd VDU position, started to use screen polys
 * 
 * 31    20-06-97 9:12a Philipy
 * new struct for identifying vertices for exclusion
 * 
 * 30    17-06-97 9:23a Philipy
 * 
 * 29    13-06-97 3:44p Philipy
 * added new menu states
 * 
 * 28    7-06-97 3:05p Philipy
 * increased MAXVDULINES
 * 
 * 27    4-06-97 9:12p Philipy
 * vdu processing functions
 * 
 * 26    21-05-97 9:57a Philipy
 * Set up status for camera
 * 
 * 25    19-05-97 8:19p Philipy
 * Minor changes to structures
 * 
 * 24    15-05-97 7:10a Philipy
 * Changed PARAM and STATUS structures
 * 
 * 23    12-05-97 6:13p Philipy
 * Disc animation stuff
 * 
 * 22    9-05-97 12:54p Philipy
 * 
 * 21    7-05-97 6:11p Philipy
 * Changed MENU structure to include MenuStatus
 * Set up enum for status types
 * 
 * 20    7/05/97 9:57 Collinsd
 * Fixed reloading of title models.
 * 
 * 19    2/24/97 12:17p Phillipd
 * 
 * 18    2/05/97 12:01p Phillipd
 * nicer directplay bug handle..
 * 
 * 17    6-01-97 9:11a Collinsd
 * 
 * 16    20/12/96 15:19 Oliverc
 * Changed debug menu to be on SHIFT-F1
 * 
 * 15    19/12/96 16:20 Oliverc
 * 
 * 14    19/12/96 16:16 Oliverc
 * Added debug options menu with "go to room" function
 * 
 * 13    12/18/96 12:41p Phillipd
 * Copyright screen added..
 * 
 * 12    12/17/96 4:57p Phillipd
 * Version Control Added..
 * 
 * 11    16/12/96 10:48 Oliverc
 * Added programmer-definable expiry date feature
 * 
 * 10    13/12/96 10:56 Oliverc
 * Added in-game help screen
 * 
 * 9     12/12/96 16:22 Oliverc
 * Changed game key reading to use DirectInput
 * 
 * 8     12/10/96 3:54p Phillipd
 * 
 * 7     12/01/96 1:21p Phillipd
 * You can now quit a game go back to the title screen and start or join
 * another....
 * 
 * 6     11/29/96 3:10p Phillipd
 * Full screen from menu toggle...
 * Fixed the mouse lag problem again....
 * 
 * 5     11/20/96 2:42p Phillipd
 * players can now restart and keep there score...
 * 
 * 4     18/11/96 19:30 Oliverc
 * Changed menu toggle/back out key to ESC and stopped ESC from quitting
 * the program
 * 
 * 3     14/11/96 17:37 Oliverc
 * Added in-window in-game menus
 * 
 * 2     11/13/96 9:08a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 1     10/28/96 8:49a Phillipd
 * 
 */

#ifndef TITLE_INCLUDED
#define TITLE_INCLUDED

#if defined( VERSION_SPANISH )
#include "sp-localtitle.h"
#elif defined( VERSION_ITALIAN )
#include "it-localtitle.h"
#elif defined( VERSION_GERMAN )
#include "ge-localtitle.h"
#elif defined( VERSION_FRENCH )
#include "fr-localtitle.h"
#else
#include "localtitle.h"
#endif

#include	"d3d.h"
#include	"typedefs.h"
#include	"new3d.h"
#include	"2dtextures.h"

//	Defines

extern float PseudoHostAck_Timeout;
#define PSEUDOHOST_WAIT_TIME 1800.0F	// 30 secs approx 

#define QUICKSTART_None		0
#define QUICKSTART_Start	1
#define QUICKSTART_Join		2
#define QUICKSTART_Server	3
#define QUICKSTART_SelectSession	4
#define QUICKSTART_Notify	5

#define ALLOWED_BIKETYPES	MAXBIKETYPES

#define MAX_LEVEL_TEXT_LENGTH 512
#define MAX_LEVEL_NAME_LENGTH 128
#define MAX_SESSION_DESC_LENGTH 64

#define DEFAULT_PLAYER_NAME		"Player"

#define DEFAULT_LEVELS			"levels.dat"
#define SINGLEPLAYER_LEVELS		"mission.dat"
#define MULTIPLAYER_LEVELS		"battle.dat"
#define SPLASH_LEVELS			"splash.dat"

#define DEMO_LEVELS				MULTIPLAYER_LEVELS

#define MAXMENULEVELS	16
#define MAXMENUITEMS	128
#define MAXLISTITEMS	128
#define MAXVDULINES		64
#define MAXVDULINELENGTH	64
#define MAXTEXTITEMS	128

#define ERROR_DONTUSE_MENUFUNCS		0
#define ERROR_USE_MENUFUNCS			1
#define ERROR_QUIT					2
#define ERROR_OVERIDE_LOBBY_QUIT	4

#define ATTRACTMODECOUNTVALUE ( 60.0F * 60.0F )

#define	DIFF_Easy 0
#define	DIFF_Norm 1
#define	DIFF_Hard 2
#define	DIFF_Vhard 3

#define CONTROL_Mouse		0
#define CONTROL_Keyboard	1
#define CONTROL_Joystick	2
#define CONTROL_Gamepad		3

// WARNING!! if extending above 7 types, number of bits for game type in GAMEPARAMETERSMSG->GameInfo must be extended
#define GAME_Normal			0
#define GAME_Tag			1
#define GAME_CaptureFlag	2
#define GAME_BountyHunt		3
#define GAME_TeamBounty		4
#define GAME_Team			5
#define GAME_CTF			6

#define GAMECOMPLETE_NotComplete				0
#define GAMECOMPLETE_WithoutAllCrystals			1	// never got to secret level because not all crystals were found ( standard end game sequence )
#define GAMECOMPLETE_GotAllCrystalsSoFar		2	// got all crystals so far ( will now go to secret level )
#define GAMECOMPLETE_WithAllCrystals			3	// with all crystals plus secret level crystal ( secret biker end game sequence )
#define GAMECOMPLETE_WithAllCrystalsExceptLast	4	// with all crystals except secret level crystal ( standard end game sequence )
#define GAMECOMPLETE_Cheated					5	// got to last but one level, but with cheating
#define GAMECOMPLETE_AdditionalLevels			6	// completed all levels ( not original levels )

#define PAN_TIME 1.0F

typedef enum
{
	MENUSTATE_Select,
	MENUSTATE_Keydef,
	MENUSTATE_Keydef2,
	MENUSTATE_Slider,
	MENUSTATE_Slider2,
	MENUSTATE_List,
	MENUSTATE_Text,
	MENUSTATE_Text2,
	MENUSTATE_SelectBiker,
	MENUSTATE_SelectPlayer,
	MENUSTATE_WeaponOrder,
	MENUSTATE_SelectLevelQuick,
	MENUSTATE_SelectLevelSlow,
	MENUSTATE_PlaceTeamMember,
	MENUSTATE_DifficultySet,
	MENUSTATE_SelectSavedGame,
	MENUSTATE_SelectKeydef,
}MENUSTATE;

enum
{
	TEAM_SelectTeam,
  	TEAM_SelectPlayer,
  	TEAM_PlacePlayer,
};

// status of disc stack
enum
{
	MENU_AtStart,
	MENU_AtDiscs,
	MENU_AtLeftVDU,
	MENU_AtRightVDU,
};

// status of the discs
enum
{
	DISC_NOTHING,
	DISC_EXPAND,
	DISC_WIGGLE,
	DISC_ROTATE,
	DISC_RETRACT,
};

enum
{
	DISC_MODE_NONE,
	DISC_MODE_ALL,
	DISC_MODE_ONE,
};

// status of the camera
enum
{
	CAMERA_AtStart,
	CAMERA_AtDiscs,
	CAMERA_AtLeftVDU,
	CAMERA_AtRightVDU,
};

enum
{
	FONT_Small,
	FONT_Medium,
	FONT_Large,
	FONT_Slider,
};

#define NUMSCREENFONTS 15

enum
{
	SLIDER_Normal,
	SLIDER_Value,
	SLIDER_Percent,
	SLIDER_Sensitivity,
	SLIDER_Time,
	SLIDER_Bike,
	SLIDER_Demo,
	SLIDER_User,
};

enum
{
	FONT_320X200_Small,
	FONT_320X200_Medium,
	FONT_320X200_Large,
	FONT_320X240_Small,
	FONT_320X240_Medium,
	FONT_320X240_Large,
	FONT_512X384_Small,
	FONT_512X384_Medium,
	FONT_512X384_Large,
	FONT_640X400_Small,
	FONT_640X400_Medium,
	FONT_640X400_Large,
	FONT_800X600_Small,
	FONT_800X600_Medium,
	FONT_800X600_Large,
};

enum
{
	BIKER_INVALID,
	BIKER_TPAGE_1,
	BIKER_TPAGE_2,
};

enum
{
	WIPE_Done,
 	WIPE_In,
 	WIPE_Out,
	WIPE_WaitingToProcess,
};


#define	MAXTEXTMSG			128		// the number of chars in a message you can send.....
//	Strucures
#define	TEXTFLAG_Nothing			0
#define	TEXTFLAG_CentreX			1		// centre text along x axis of bounding box
#define	TEXTFLAG_CentreY			2		// centre text along y axis of bounding box
#define	TEXTFLAG_BottomY			4		// justify text to bottom of box
#define	TEXTFLAG_Instant			8		// text is displayed instantly
#define	TEXTFLAG_RightX				16		// right justify
#define	TEXTFLAG_NotImplemented		32		// display text dimmed (only works with screenpoly text, ie. 3dfx)
#define	TEXTFLAG_AutoSelect			64		// automatically select this menu item if highlighted
#define TEXTFLAG_RefreshList		128		// not currently used (all lists are continually refreshed)
#define TEXTFLAG_ForceFit			256		// do not wrap text, truncate with ... if neccesary
#define TEXTFLAG_SuppressHighlight	512		// prevent highlight box from appearing around text
#define TEXTFLAG_Unselectable		1024	// do not allow this item to be selected
#define TEXTFLAG_CheckForRefresh	2048	// this text item may change - refresh if neccesary
#define TEXTFLAG_ManualListHighlight 4096   // I want to specify how a list is highlighted manually
											// ( default is pulsing when active, static when not active )
#define TEXTFLAG_CentreYBunched	 8192		// text is bunched up in centre of bounding box	
#define TEXTFLAG_Pulse			 16384		// pulse text on and off
#define TEXTFLAG_TeamItem		 32768		// team select item

#define MAXNUMLINES 64 
#define MAXCHARSPERLINE 128
#define MAXNUMOFVDUPOLYS	512
#define MAXPOINTERS			8

#define MAX_TEXT_LENGTH 64

typedef struct
{
	uint8	*text;						// text string
	uint8	oldtext[MAX_TEXT_LENGTH];	// used for checking if text has changed
	BOOL	highlight;					// item highlighted?
	int		highlighttype;				// highlight type?
	int		xmin;						// 
	int		ymin;						// bounding box for text
	int		xmax;						// (VDU co-ordinates)
	int		ymax;						// 
	int		font;						// font size (small, medium, large)
	int		flags;						// formatting info
	int		num_lines;					// number of lines once formatted
	float	length[MAXNUMLINES];		// length of formatted line (pixels)
	float	height[MAXNUMLINES];		// height of formatted line (pixels)
	int		startpoint[MAXNUMLINES];	// start character of line (in original text string)
	int		endpoint[MAXNUMLINES];		// end character of line

	int		currentchar[MAXNUMLINES];	// used for teletype mode
	float 	currentx[MAXNUMLINES];		// used for teletype mode
	BOOL	boxdone;					// used for teletype mode
	int		num_pointers_to_free;		
	void	*pointer[MAXPOINTERS];		

	float 	currenty;		// possibly replace with local variables?
	float	char1x;
	float	text_y_min;
	float	text_y_max;
	BOOL	force_fit;
	BOOL	finished[MAXNUMLINES];
}TEXTINFO;

typedef struct _MENU MENU;
typedef struct _MENUITEM MENUITEM;
typedef struct _PARAM PARAM;
typedef struct _STATUS STATUS;
typedef struct _TITLE_EVENT TITLE_EVENT;
typedef struct _TITLE_EVENT_TIMER TITLE_EVENT_TIMER;
typedef struct _TEXT TEXT;
typedef struct _SLIDER SLIDER;
typedef struct _LIST LIST;

typedef void (* MenuFunc)( MENU * );
typedef void (* MenuItemFunc)( MENUITEM * );
typedef void (* MenuStatusFunc)(int *);
typedef void (* EventFunc)(TITLE_EVENT *);
typedef void (* TimerFunc)(TITLE_EVENT_TIMER *);
typedef char (* TextTypeFunc)( TEXT *, char );
typedef char *(* SliderUserFunc)( SLIDER * );
typedef BOOL (* SliderSetFunc)( SLIDER * );
typedef BOOL (* ListDeleteFunc) ( LIST *, int );
typedef void (* ListInfoFunc) ( LIST * );

// NOTE!!! CTF type is propagated to clients in 4 bits - cannot have more than 15 types!!
enum
{
	CTF_STANDARD,
	CTF_MUST_CARRY,
	CTF_NO_RETURN,
	CTF_CAN_CARRY,
	CTF_ONE_TOUCH,
	CTF_MAX
};


enum
{
	EVENT_TRIGGERED,
	TIMER_TRIGGERED,
	EXTERNALLY_TRIGGERED,
};

#define EVENT_DONE			TRUE
#define	EVENT_NOT_DONE		FALSE

#define EVENT_ACTIVE		TRUE
#define EVENT_NOT_ACTIVE	FALSE

#define MAXTITLEEVENTS 128
#define MAXTITLETIMERS 32

#define DINPUT_BUFFERSIZE       32      /* Number of buffer elements (for menu screens) */

enum
{
	TITLE_EVENT_STATUS_IDLE,
	TITLE_EVENT_STATUS_ACTIVE,
	TITLE_EVENT_STATUS_FINISHED,
};


// timers...
enum
{
	TITLE_TIMER_NULL,
	TITLE_TIMER_PanToDiscsInit,
	TITLE_TIMER_PanToDiscsFromRight,
	TITLE_TIMER_PanToDiscsFromLeft,
	TITLE_TIMER_PanToRightVDU,
	TITLE_TIMER_InterLevelPan,
	TITLE_TIMER_PanToLeftVDU,
	TITLE_TIMER_SwapBikes,
	TITLE_TIMER_ChooseDiscPan,
	TITLE_TIMER_Visuals,
	TITLE_TIMER_PanToDiscsFromVisuals,
	TITLE_TIMER_NormalPanToRightVDU,
	TITLE_TIMER_NormalPanToDiscsFromRight,
	//TITLE_TIMER_SwapObjects,
	TITLE_TIMER_LowerHolopad,
	TITLE_TIMER_ShrinkObject,
	TITLE_TIMER_ExpandObject,
};

typedef struct _TITLE_EVENT
{
	EventFunc	InitFunc;
	EventFunc	ProcessFunc;	//must set EventStatus to TITLE_EVENT_FINISHED when this function is finished
	EventFunc	ExitFunc;
	int			TriggeredBy;	// the timer that is the trigger
	float		StartTime;	//0 = start, 1 = finish
	float		EndTime;	//0 = start, 1 = finish
	int			Status;
}TITLE_EVENT;

enum
{	
	TITLE_EVENT_TIMER_IDLE,
	TITLE_EVENT_TIMER_ACTIVE,
	TITLE_EVENT_TIMER_FINISHED,
	TITLE_EVENT_TIMER_AWAITINGRESET,
};

typedef struct _TITLE_EVENT_TIMER
{
	float		Interval;		//lifespan of timer (secs)
	float		CurrentTime;	//0 = start, 1 = end
	int			Status;
	TimerFunc	InitFunc;
}TITLE_EVENT_TIMER;

typedef struct _LEVEL_LOOKUP
{
	char		*name;
	int			frame_no;
	FRAME_INFO	**tpage;
	BOOL		rotated;
}LEVEL_LOOKUP;

#define MAXTEXTPERMENUITEM 32

typedef struct
{
	VECTOR start;
	VECTOR end;
	D3DTRIANGLE poly;
} SCANLINES;


typedef struct _MENUITEM
{
	int		x;
	int		y;
	int		xmax;
	int		ymax;
	int		FrameNo;
	uint8 *	StrPnt;
	int		font;
	int		highlightflags;
	void *	Variable;
	void *	Value;
	MenuItemFunc FuncSelect;
	MenuItemFunc FuncDraw;
	TEXTINFO *TextInfo[MAXTEXTPERMENUITEM];
	int numtextitems;
	int EventToTrigger;
}MENUITEM;


typedef struct _MENU
{
	uint8 *			Name;
	MenuFunc		FuncEntry;
	MenuFunc		FuncExit;
	MenuStatusFunc	MenuFuncDraw;
	int				MenuStatus;
	MENUITEM Item[ MAXMENUITEMS ];
}MENU;

typedef struct _SLIDER
{
	int min;
	int max;
	int step;
	int value;
	int oldvalue;
	float dt;
	float timer;
	uint16 poly;
	BOOL redraw_req;
	SliderUserFunc FuncUser;
	SliderSetFunc FuncSet;
	SliderSetFunc FuncRefresh;
} SLIDER;


typedef struct _LIST
{
	int items;
	int display_items;
	int top_item;
	int selected_item;
	uint8 item[MAXLISTITEMS][64];
	int old_top_item;
	int old_selected_item;
	ListDeleteFunc FuncDelete;
	ListInfoFunc FuncInfo;	// used to get info from a particular item
	uint8 old_item[MAXLISTITEMS][64];
	BOOL Static;	// list is 'frozen' & awaiting event before allowing processing
} LIST;


typedef struct _TEXT
{
	int insert_pos;
	int VDU_insert_pos;
	uint8 text[MAXTEXTMSG];
	MenuItemFunc FuncSet;
	int limit_length;
	TextTypeFunc FuncType;
} TEXT;

#define EXCLUDE_ALL -1
#define MAXEXCLUDEDVERTICES 16

typedef struct
{	int Group;
	int ExecBuf;
	int NumVertices;
	int VertexPtr[MAXEXCLUDEDVERTICES];
} EXCLUDEDVERTICES;

// globals

extern TEXT TCPAddress;

extern MENU * CurrentMenu;
extern MENUITEM * CurrentMenuItem;
extern MENU MENU_ForceAbort;
extern MENU MENU_Start;
extern MENU MENU_Copyright;
extern MENU MENU_InGame;
extern MENU MENU_Help;
extern MENU	MENU_DebugHelp;
extern MENU	MENU_GotoRoom;
extern MENU MENU_DemoPlaying;
extern MENU MENU_NEW_JoinWaitingToStart;
extern MENU MENU_NEW_HostWaitingToStart;
extern MENU MENU_NEW_WatchTeamSelect;
extern MENU MENU_NEW_CreateGamePseudoHost;
extern MENU MENU_NEW_ChooseTeamForExistingGame;
extern MENU MENU_NEW_PseudoHostWaitingForAck;
extern MENU MENU_NEW_ChooseSessionToJoin;
extern MENU MENU_NEW_PseudoHostWaitingToStart;

extern MENU MENU_NEW_ChooseConnectionToStart;
extern MENU MENU_NEW_ChooseConnectionToJoin;

extern MENU MENU_NEW_GeneralLoading;

extern MENU	MENU_STATS_Start;

extern BOOL quitting;
extern LIST	SessionsList;
extern LIST	MySessionsList;

MENU *LastMenu;

//	Function Prototypes

BOOL
InitTitle(LPDIRECTDRAW lpDD, LPDIRECT3D lpD3D, LPDIRECT3DDEVICE lpDev, 
           LPDIRECT3DVIEWPORT lpView );
void InitTitleFont(void);
void ReleaseTitle(void);
BOOL DisplayTitle(void);

void MenuBackSpecific( MENU *Menu, BOOL UseExitFuncs );
void	MenuRestart( MENU * Menu );
void	MenuExit( void );
void	MenuAbort( void );
void	MenuChange( MENUITEM * Item );
void	MenuBack();
void	MenuProcess( void );
void	MenuItemDrawName( MENUITEM * Item );
void	MenuDraw( MENU * Menu );
void	SetDifficulty( MENUITEM * Item );
void	MenuItemDrawCursor( MENUITEM * Item );
void	SelectToggle( MENUITEM *Item );
void	DrawToggle( MENUITEM *Item );
void	SelectRadioButton( MENUITEM *Item );
void	DrawRadioButton( MENUITEM *Item );
void	SelectSlider( MENUITEM *Item );
void	DrawSlider( MENUITEM *Item );
void	SelectKey( MENUITEM *Item );
void	DrawKey( MENUITEM *Item );
void	MenuItemBack( MENUITEM *item );
void	SetOurRenderStates( MENUITEM *item );
void	GetOurRenderStates( MENUITEM *item );
void	DecrementSlider( MENUITEM *Item );
void	IncrementSlider( MENUITEM *Item );
void	SliderHome( MENUITEM *Item );
void	SliderEnd( MENUITEM *Item );
void	SliderSet( MENUITEM *Item );
void	SliderCancel( MENUITEM *Item );
void	SelectList( MENUITEM *Item );
void	DrawList( MENUITEM *Item );
void	SelectListPrev( LIST *l );
void	SelectListNext( LIST *l );
void	SelectListNextPage( LIST *l );
void	SelectListPrevPage( LIST *l );
void	SelectListHome( LIST *l );
void	SelectListEnd( LIST *l );
void	SelectListSearch( LIST *l, char *keyword );
void	SelectListItem( LIST *l );
void	CancelListSelection( LIST *l );
void	SelectText( MENUITEM *Item );
void	DrawTextItem( MENUITEM *Item );
void	TextLeft( TEXT *t );
void	TextRight( TEXT *t );
void	TextType( TEXT *t, char c );
void	TextBackspace( TEXT *t );
void	TextDelete( TEXT *t );
void	TextClear( TEXT *t );
void	TextHome( TEXT *t );
void	TextEnd( TEXT *t );
void	TextEnter( MENUITEM *item );
void	TextCancel( TEXT *t );
BOOL	ProcessKeydef( int Key );
BOOL	ProcessText( int Key );
BOOL	ProcessBikerList ( int Key );
BOOL	ProcessList( int Key );
BOOL	ProcessSlider( int Key );
void	ProcessSelect( int Key );

void GetLastGameInfo( void );
void SetLastGameInfo( void );

void MenuGoFullScreen( MENUITEM *Item );
void MenuSelectMode( MENU *Menu );
void MakeModeList( MENU *Menu );
void ChangeStatus_Title ( MENUITEM * Item );
void CreateVersion(void);
BOOL LoadTitleModels(void);
void VduClear ( void );
void UpdateSessions ( int *dummy );
void SelectQuit( MENUITEM *Item );
void PrintErrorMessage (char *ErrorStr, int BackBy, MENU *NewMenu, int type);
void MenuItemBackFromError ( MENUITEM *Item );
void MenuChangeEx( MENU *Menu );
void LoadLevelText( MENU *Menu );
BOOL SetGamma( SLIDER *slider ); 
void InitInGameMenu( MENU *Menu );
void AllowLevelSelect( MENU *Menu );
void InitTitleMessaging( void );
void DrawReadyButton( MENUITEM *Item );
void PlayerConfirm( MENUITEM *Item );
void SelectTeamList( MENUITEM *Item );
void AllowQuitForLobby( MENU *Menu );
BOOL GeneralTimeout( float *timer );
void GetServerPrefs( void );
void SetServerPrefs( void );
LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
void TitleReset( MENUITEM *item );
void SetTitleNotify( char *str );

#endif
