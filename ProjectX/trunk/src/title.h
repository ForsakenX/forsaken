#ifndef TITLE_INCLUDED
#define TITLE_INCLUDED

#include "localtitle.h"

#include "new3d.h"
#include "2dtextures.h"

#define QUICKSTART_None		0
#define QUICKSTART_Start	1
#define QUICKSTART_Join		2
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

#define	DIFF_Easy 0
#define	DIFF_Norm 1
#define	DIFF_Hard 2
#define	DIFF_Vhard 3

#define CONTROL_Mouse		0
#define CONTROL_Keyboard	1
#define CONTROL_Joystick	2
#define CONTROL_Gamepad		3

// order of this list must match GameTypeNameTable
#define MAX_GAMETYPE		5
typedef enum
{
GAME_Normal,			// 0 free for all
GAME_FlagChase,			// 1
GAME_BountyHunt,		// 2
GAME_TeamBountyHunt,	// 3
GAME_Team,				// 4
GAME_CTF,				// 5
} game_t;

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
	// this is the only font file we have
	FONT_512X384_Small,
	FONT_512X384_Medium,
	FONT_512X384_Large,
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


#define	MAXTEXTMSG			512		// the number of chars in a message you can send.....
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
	u_int8_t	*text;						// text string
	u_int8_t	oldtext[MAX_TEXT_LENGTH];	// used for checking if text has changed
	bool	highlight;					// item highlighted?
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
	bool	boxdone;					// used for teletype mode
	int		num_pointers_to_free;		
	void	*pointer[MAXPOINTERS];		

	float 	currenty;		// possibly replace with local variables?
	float	char1x;
	float	text_y_min;
	float	text_y_max;
	bool	force_fit;
	bool	finished[MAXNUMLINES];
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
typedef bool (* SliderSetFunc)( SLIDER * );
typedef bool (* ListDeleteFunc) ( LIST *, int );
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

#define EVENT_DONE			true
#define	EVENT_NOT_DONE		false

#define EVENT_ACTIVE		true
#define EVENT_NOT_ACTIVE	false

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

struct _TITLE_EVENT
{
	EventFunc	InitFunc;
	EventFunc	ProcessFunc;	//must set EventStatus to TITLE_EVENT_FINISHED when this function is finished
	EventFunc	ExitFunc;
	int			TriggeredBy;	// the timer that is the trigger
	float		StartTime;	//0 = start, 1 = finish
	float		EndTime;	//0 = start, 1 = finish
	int			Status;
};

enum
{	
	TITLE_EVENT_TIMER_IDLE,
	TITLE_EVENT_TIMER_ACTIVE,
	TITLE_EVENT_TIMER_FINISHED,
	TITLE_EVENT_TIMER_AWAITINGRESET,
};

struct _TITLE_EVENT_TIMER
{
	float		Interval;		//lifespan of timer (secs)
	float		CurrentTime;	//0 = start, 1 = end
	int			Status;
	TimerFunc	InitFunc;
};

typedef struct _LEVEL_LOOKUP
{
	char		*name;
	int			frame_no;
	FRAME_INFO	**tpage;
	bool		rotated;
}LEVEL_LOOKUP;

#define MAXTEXTPERMENUITEM 32

typedef struct
{
	VECTOR start;
	VECTOR end;
	TRIANGLE poly;
} SCANLINES;


struct _MENUITEM
{
	int		x;
	int		y;
	int		xmax;
	int		ymax;
	int		FrameNo;
	u_int8_t *	StrPnt;
	int		font;
	int		highlightflags;
	void *	Variable;
	void *	Value;
	MenuItemFunc FuncSelect;
	MenuItemFunc FuncDraw;
	TEXTINFO *TextInfo[MAXTEXTPERMENUITEM];
	int numtextitems;
	int EventToTrigger;
};


struct _MENU
{
	u_int8_t *			Name;
	MenuFunc		FuncEntry;
	MenuFunc		FuncExit;
	MenuStatusFunc	MenuFuncDraw;
	int				MenuStatus;
	MENUITEM Item[ MAXMENUITEMS ];
};

struct _SLIDER
{
	int min;
	int max;
	int step;
	int value;
	int oldvalue;
	float dt;
	float timer;
	u_int16_t poly;
	bool redraw_req;
	SliderUserFunc FuncUser;
	SliderSetFunc FuncSet;
	SliderSetFunc FuncRefresh;
};


struct _LIST
{
	int items;
	int display_items;
	int top_item;
	int selected_item;
	u_int8_t item[MAXLISTITEMS][64];
	int old_top_item;
	int old_selected_item;
	ListDeleteFunc FuncDelete;
	ListInfoFunc FuncInfo;	// used to get info from a particular item
	u_int8_t old_item[MAXLISTITEMS][64];
	bool Static;	// list is 'frozen' & awaiting event before allowing processing
};


struct _TEXT
{
	int insert_pos;
	int VDU_insert_pos;
	char text[MAXTEXTMSG];
	MenuItemFunc FuncSet;
	int limit_length;
	TextTypeFunc FuncType;
};

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
extern MENU MENU_InGame;
extern MENU MENU_InGameSingle;
extern MENU MENU_Help;
extern MENU	MENU_DebugHelp;
extern MENU	MENU_GotoRoom;
extern MENU MENU_DemoPlaying;
extern MENU MENU_NEW_JoinWaitingToStart;
extern MENU MENU_NEW_HostWaitingToStart;
extern MENU MENU_NEW_WatchTeamSelect;
extern MENU MENU_NEW_ChooseTeamForExistingGame;
extern MENU MENU_NEW_PseudoHostWaitingForAck;
extern MENU MENU_NEW_Joining;
extern MENU MENU_NEW_PseudoHostWaitingToStart;
extern MENU MENU_NEW_ChooseConnectionToJoin;
extern MENU MENU_NEW_GeneralLoading;

extern MENU	MENU_STATS_Start;

extern bool quitting;

MENU *LastMenu;

//	Function Prototypes

bool InitTitle();
void ReleaseTitle(void);
bool DisplayTitle(void);

void MenuBackSpecific( MENU *Menu, bool UseExitFuncs );
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
void	SelectMatureContentToggle( MENUITEM *Item );
void	DrawMatureContentToggle( MENUITEM *Item );
void	SelectColourToggle( MENUITEM *Item );
void	SelectToggle( MENUITEM *Item );
void	DrawToggle( MENUITEM *Item );
void	SelectRadioButton( MENUITEM *Item );
void	DrawRadioButton( MENUITEM *Item );
void	SelectSlider( MENUITEM *Item );
void	DrawSlider( MENUITEM *Item );
void	SelectKey( MENUITEM *Item );
void	DrawKey( MENUITEM *Item );
void	MenuItemBack( MENUITEM *item );
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
bool	ProcessKeydef( int Key );
bool	ProcessText( int Key );
bool	ProcessBikerList ( int Key );
bool	ProcessList( int Key );
bool	ProcessSlider( int Key );
void	ProcessSelect( int Key );

void MenuGoFullScreen( MENUITEM *Item );
void MenuSelectMode( MENU *Menu );
void MakeModeList( MENU *Menu );
bool LoadTitleModels(void);
void VduClear ( void );
void UpdateSessions ( int *dummy );
void SelectQuit( MENUITEM *Item );
void PrintErrorMessage (char *ErrorStr, int BackBy, MENU *NewMenu, int type);
void MenuItemBackFromError ( MENUITEM *Item );
void MenuChangeEx( MENU *Menu );
void LoadLevelText( MENU *Menu );
bool SetGamma( SLIDER *slider );
void InitInGameMenu( MENU *Menu );
void InitTitleMessaging( void );
void DrawReadyButton( MENUITEM *Item );
void PlayerConfirm( MENUITEM *Item );
void SelectTeamList( MENUITEM *Item );
bool GeneralTimeout( float *timer );
void TitleReset( MENUITEM *item );
void SetTitleNotify( char *str );

#endif
