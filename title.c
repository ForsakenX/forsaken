
/*===================================================================
	Include Files...	
===================================================================*/

#define SAVEGAME_SLOTS
#ifdef WIN32
#include <windows.h>
#endif

#include "main.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "sfx.h"
#include "text.h"
#include "input.h"
#include "controls.h"
#include "config.h"
#include "title.h"
#include "multiplayer.h"
#include "2dtextures.h"
#include "mload.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"
#include "models.h"
#include "camera.h"
#include "models.h"
#include "polys.h"
#include "2dpolys.h"
#include "screenpolys.h"
#include "local.h"
#include "lines.h"
#include "loadsave.h"

#include "net.h"
#include "restart.h"
#include "version.h"
#include "demo.h"
#include "singleplayer.h"
#include "util.h"
#include "lua_config.h"
#include "net_tracker.h"
#include "lua_games.h"
#include "timer.h"
#include "render.h"
#include "file.h"
#include "oct2.h"
#include "tload.h"


#define MAX_SAVEGAME_SLOTS		16
#define MAX_PILOTNAME_LENGTH	(MAX_PLAYER_NAME_LENGTH - 1)

//#pragma optimize( "gty", on )

extern render_info_t render_info;
extern float MaxMessageTime;
extern void SetViewportError( char *where, render_viewport_t *vp );
extern bool ShowNamesAnyway;

bool SpaceOrbSetup = false;
void DefaultJoystickSettings( USERCONFIG *u );
void ReInitJoysticks( void );

#define MAX_KEYDEF_PAGES	(4)
int CurrentPage = 0;
int MaxPage = 0;
bool MissileCameraEnable;
void MenuItemSelect( MENUITEM * item );
void GetLevelName( char *buf, int bufsize, int level );
void MenuItemDrawPageName( MENUITEM *Item );
void InitKeyDef( MENU *Menu );
void InitKeyDefPage( MENU *Menu );
void DrawKeyDefPage( MENUITEM *item );
void DrawKeyDefHelp1( MENUITEM *item );
void DrawKeyDefHelp2( MENUITEM *item );
void DrawKeyDefHelp3( MENUITEM * Item );
void DrawFlatMenuKeyLeftArrowText( MENUITEM *Item );
void DrawFlatMenuKeyLeftArrow( MENUITEM *Item );
void DrawFlatMenuKeyRightArrowText( MENUITEM *Item );
void DrawFlatMenuKeyRightArrow( MENUITEM *Item );
void DrawFlatMenuKeyNum( MENUITEM *Item );
static void KeyDelete( DEFKEY *kdef, int key );
static void KeyDeleteRepeating( DEFKEY *key );
static void KeySelect( DEFKEY *kdef, int key );

bool DeletePilot( LIST *l, int item );
void MoveConfigFile( MENU *Menu );

extern void ShowNodeNetwork( u_int32_t NetMask );
void ProcessTextItems (void);

bool	Last_SWMonoChrome = false;

/*===================================================================
		Externals ...
===================================================================*/
extern bool WaitingToQuit;
extern int16_t	NumPrimaryPickups;
extern float FlashTextActive;
extern float GlobalSoundAttenuation;
extern float WATER_CELLSIZE;
extern bool flush_input;
extern double	Gamma;
extern bool MyUseShortPackets;
extern bool UseShortPackets;
extern bool MyResetKillsPerLevel;
extern bool TintBikeTeamColor;
extern int CrystalsFound;
extern	int16_t	NumGoldBars;
extern int Secrets;
extern int TotalSecrets;
extern	int16_t		NumInitEnemies;
extern	int16_t		NumKilledEnemies;

extern MODELNAME *TitleModelSet;

int GameCompleted = GAMECOMPLETE_NotComplete;

#define NUM_TITLE_LOOPS 2
#define TITLE_LOOP_DiscAmbience 0
#define TITLE_LOOP_VduAmbience 1
u_int32_t TitleLoopId[ NUM_TITLE_LOOPS ];

bool JoystickInput = true;

bool CTF = false;
bool NeedFlagAtHome = true;
bool CanCarryOwnFlag = false;
bool OwnFlagTeleportsHome = true;

bool ShowWeaponsPossessedOnHUD;
bool ShowClockOnHUD;
bool ShowKPMOnHUD;
bool ShowSpeedOnHUD;
bool ShowPlayersOnHUD;
bool ShowPlayersOnHUDbyKills;
bool ShowPlayerHealthAboveBikes;
bool ScaleFontPlayerHealthAboveBikes;
bool ShowPlayerHealthByScores;

int	NetUpdateIntervalCmdLine = 0;
int	GoalScore = 5;
int BountyBonusInterval = 10;

float LevelTimeTaken;

bool NoMenuBack = false;

int16_t DummyTextureIndex;
extern FRAME_INFO	*	Title_LevelPics_Header;
extern FRAME_INFO	*	Title_TVFrame_Header;
extern char MissionTextNames[MAXLEVELS][128];
extern char MissionTextPics[MAXLEVELS][128];
extern int32_t ColPerspective;
extern int32_t MyColPerspective;
extern bool	JustExitedMenu;
extern bool	PickupValid[ MAXPICKUPTYPES ];
extern bool	MyPickupValid[ MAXPICKUPTYPES ];
extern bool LockOutWindows;
extern SLIDER BikerSpeechSlider;
extern int16_t Lives;
extern BYTE	GameStatus[];	// Game Status for every Ship...
extern bool bSoundEnabled;
extern bool ShowTrigZones;
extern bool ShowEFZones;
extern bool ShowTeleports;
extern bool ShowBoundingBoxes;
extern bool ShowColZones;

#ifdef DEMO_SUPPORT
extern float DemoAvgFps;
extern float DemoTotalTime;
extern int32_t DemoGameLoops;
#endif

extern	int16_t actual_volume;
extern	bool RearCameraActive;
char *ShipAxisSeperateText[NUM_SHIP_AXIS_ACTIONS * 2];
extern char *JoystickPOVDirections[];
extern char *ShipAxisText[];
extern int ShipAxisLookup[];

#ifdef DINPUTJOY
extern DIJOYSTATE2		js[ INPUT_BUFFERS ][ MAX_JOYSTICKS ];
#endif

extern char *ShipActionText[];
extern TLOADHEADER	Tloadheader;
extern int	TeamMembers[MAX_TEAMS];
extern	int16_t	ShowPortal;
extern float VduScaleX, VduScaleY;
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	POLY   			Polys[MAXPOLYS];
extern  SCRPOLY			ScrPolys[ MAXNUMOFSCRPOLYS ];
extern	LINE			Lines[ MAXLINES ];
extern	MXALOADHEADER	MxaModelHeaders[ MAXMXAMODELHEADERS ];
extern	FRAME_INFO	*	Title_Fonts_Header;
extern	FRAME_INFO	*	Title_Chars1_Header;
extern	FRAME_INFO	*	Title_Chars2_Header;
extern	OFF_FILES Title_OffsetFiles[];
extern	CAMERA	CurrentCamera;
void Build_View();
extern	RENDEROBJECT RenderBufs[4];
extern	int16_t		NumLevels;

#define REGISTRY_EXPIRE_TIME	"ServiceID"
#define DEFAULT_CONFIG_FILE		DEFAULT_PLAYER_NAME".txt"

char	*config_name = NULL;
char	old_config[256] = "";
char	biker_config[256] = DEFAULT_CONFIG_FILE;
char	biker_name[256] = DEFAULT_PLAYER_NAME;
char	bike_name[16];
char	biker_text[512];
bool	quitting = false;
int16_t	SelectedBike = 0; //MODEL_Borg;
int16_t	NumTitleModelsToLoad;
float TriggerInitialAnimation = 0.0F;
bool	Our_CalculateFrameRate(void);
extern	u_int16_t			FirstFmPolyUsed;
extern	u_int16_t			FirstPolyUsed;
extern	u_int16_t			FirstLineUsed;
extern	u_int16_t			FirstScrPolyUsed;
extern USERCONFIG *player_config;
extern	float framelag;
extern	BYTE	MyGameStatus;
extern	int		FontWidth;
extern	int		FontHeight;
extern	int		FontSourceWidth;
extern	int		FontSourceHeight;
bool	ShowWeaponKills = false;
extern	float	ModeScaleX;
extern	float	ModeScaleY;
extern	bool					HostDuties;
extern	int16_t	BikeModels[ MAXBIKETYPES ];
extern	MLOADHEADER Mloadheader;
extern int16_t		LevelNum;
extern int16_t		NumLevels;
extern	bool                    IsHost;
extern	RENDERMATRIX view;
extern	bool ClearBuffers( void );
extern	MATRIX	MATRIX_Identity;
extern	render_viewport_t viewport;
extern	MODEL	Models[];
u_int16_t	BackgroundModel[NUMOFTITLEMODELS];
extern	TLOADHEADER Tloadheader;
extern	float	LastDistance[];
extern	MXLOADHEADER ModelHeaders[];
extern	MODELNAME	TitleModelNames[]; 
extern	MODELNAME	InterLevelModelNames[];
extern	char *PrimaryDescription[];
extern	char *SecondaryDescription[];
extern	bool	ShowUntriggeredNMEs;
extern	bool	BilinearSolidScrPolys;
extern	bool	MyRandomPickups;
extern  bool bIgnoreWM_SIZE;

/*===================================================================
		Mode changing stuff..
===================================================================*/
LIST	ModeList = { 0 };
int		WhichMode[ MAXLISTITEMS ];
SLIDER GammaSlider = {50, 300, 10, 100, 0, 0.0F, 0.0F, 0, false, NULL, NULL, SetGamma };

/*===================================================================
		Level changing stuff..
===================================================================*/
LIST	LevelList = { 0 };
char SelectedLevel[128];
char SelectedLevelText[MAX_LEVEL_TEXT_LENGTH];

/*===================================================================
		Joystick stuff..
===================================================================*/
char	NoBtnText[] = "not currently assigned";
char	MultiBtnText[] = "multiple buttons assigned";
char	NoPOVText[] = "No POV selected";
char	JoyMode1[] = "use the cursor keys to select an action, then press enter";
char	JoyMode2[] = "Press a joystick button to assign to this action";
LIST	JoystickList = { 0 };
char    JoystickBtnText[64];
char	AxisExtremeLeftText[64];
char	AxisExtremeRightText[64];
LIST ShipActionList = { 0 };
typedef struct
{
	int type;
	int num;
	int dir;
}PSEUDOJOYBTN;
#define BTNTYPE_Button 0
#define BTNTYPE_POV 1
int CurrentJoyPOV;
int	CurrentJoyPOVDirection;
LIST JoystickAxisList = { 0 };
LIST AxisActionList = { 0 };
bool AxisInvert = false;
bool FineControl = false;
SLIDER DeadzoneSlider = {0, 100, 5, 30 };
SLIDER SensitivitySlider = {0, 100, 5, 50 };
SLIDER AxisSlider;
SLIDER AxisActionSlider;
int AxisMap[MAX_JOYSTICK_AXIS];
int JoystickMap[MAX_JOYSTICKS];
#define NUM_AXIS_TEST_SCRPOLYS 2
u_int16_t AxisTestScrPoly[NUM_AXIS_TEST_SCRPOLYS];
#define AXISTEST_XPos 20
#define AXISTEST_YPos 150
#define AXISTEST_Width 160
#define AXISTEST_Height 10
#define AXISTEST_PtrWidth 30
float AxisPtr_xmin, AxisPtr_xmax, AxisPtr_ymin, AxisPtr_ymax, AxisPtr_mid;
#define JOYCONFIG_SelectAction 0
#define JOYCONFIG_AssignButton 1
int JoystickConfigState;

bool InTitleRoom;

char DemoAvgFpsText[128];
char DemoTotalFramesText[128];

/*===================================================================
		Error message stuff..
===================================================================*/
char ErrorMessage[256];
char NotifyMessage[ 256 ];
int	ErrorMoveBackBy;
MENU * ErrorNewMenu;
bool IgnoreMenuFuncs = false;

/*===================================================================
		Fucntions without a header..
===================================================================*/
void InitBattleMenu( MENU *Menu );
void InitInGameLevelSelect( MENU *menu );
void CheckCheats( int key );
char *CTF_Type( SLIDER *s );
void InGameSaveASinglePlayerGame( MENUITEM *item );
char TypeFileName( TEXT *t, char c );
void LoadLevelText( MENU *Menu );
void ToggleBikeEngines( MENUITEM *Item );
void DrawHelpKey( MENUITEM * Item );
int GetFontInfo( MENUITEM * Item );
void DrawBikeLabel ( MENUITEM * Item );
void DrawNameVar( MENUITEM * Item );
void DrawCenteredNameVar( MENUITEM * Item );
void ExitDetailLevels( MENU *Menu );
void NewInitBikeMenu ( MENU *Menu);
void InitBikerMenu( MENU *Menu );
void InitBikeComputerMenu( MENU *Menu );
void ExitBikerMenu( MENU *Menu );
void InitBikeListVDU( MENU *Menu );
void InitLevelSelectVDU( MENUITEM *Item );
void InitBikeList( MENU *Menu );
void InitControls( MENU *Menu );
void ExitControls( MENU *Menu );
void InitVDUPilotMenu ( MENU *Menu );
void InitPilotMenu( MENU *Menu );
void SelectPilot( MENUITEM *item );
void InitPilotName( MENU *menu );
void SetPilotName( MENUITEM *item );
void SetPilotNameInGame( MENUITEM *item );
void InitPilotReName( MENU *menu );
void RenamePilotName( MENUITEM *item );
void InitStartMenu( MENU *Menu );
void ExitInGameMenu( MENU *Menu );
void InitMultiplayerHostVDUPeerPeer( MENU *Menu );
void ExitMultiplayerHostVDUPeerPeer( MENU *Menu );
void RedrawMultiplayerHostVDUPeerPeer( int * i );
void InitSinglePlayerGame( MENU *Menu );
void InitTitleLoad( MENU *Menu );
void ExitTitleLoad( MENU *Menu );
void InitBikerName( char *name );
void InitMultiplayerHost( MENU *Menu );
void SetAutolevel( MENUITEM *item );
void SelectQuitCurrentGame( MENUITEM *Item );
void ExitBikeSelection( MENUITEM * menu );
void ExitBikeComputerSelection( MENUITEM * item );
void InitDebugMenu( MENU *Menu );
void InitDebugMode( MENU *Menu );
void InitRoomList( void );
void SetRoomName( MENUITEM *item );
void ChooseRoom( MENUITEM *item );
void DebugModeChanged( MENUITEM *item );
void SavePickups( MENUITEM *item );
void ShowNodeToggle( MENUITEM *item );
void ShowStartPointsToggle( MENUITEM *item );
void InitDemoList( MENU * Menu );
void GetGamePrefs( void );
void SetGamePrefs( void );
void InitLoadSavedGameList( MENU * Menu );
void WatchPlayerSelected( MENUITEM *Item);

char *SearchKey( char c );
void PauseDemoToggle( MENUITEM *Item );

bool InitLevels( char *levels_list );
void InitLevelSelect( MENU *Menu );
void ExitLevelSelect( MENU * Menu );
void GoToStats( MENUITEM *Item );
void GetSavedGameData( void );

bool SetFOV( float fov );
bool TintModelVertices( u_int16_t Model, float percent, EXCLUDEDVERTICES *Exclude );
bool MakeTranslucent( u_int16_t Model );
void GetExtremeOffsets( u_int16_t Model, PLANE *plane, float *minoffset, float *maxoffset );
bool TintOneVertex( u_int16_t Model, u_int16_t Group, u_int16_t ExecBuf, int VertexNo, float tr, float tg, float tb, float ta );

bool CompareVectors(VECTOR vector1, VECTOR vector2);
void SetDiscStatus(MENU *menu);
void PlaceObjects (void);
void HighlightDisc(int disc);
void ResetDisc(int disc);
void TintDisc(u_int16_t Model);
void ExpandDiscs(void);
void SetUpRotate(int disc);
void RotateDiscs(int disc, int DifficultyLevel);
void WiggleDiscs(void);
void SetTextures (MENU *menu);
void InitDifficultyLevel ( MENU *Menu );
void DifficultySet (MENUITEM *item);
void ProcessVduItems( MENU * Menu );
void KillTextInfo ( TEXTINFO *TextInfo );

void SetInitialTitleParams();
void RotateHoloLight(void);
void PlotBikeScanLine(void);
void PulsateVDU(void);
void MorphHoloLight(void);

bool DeleteSavedGame( LIST *l, int item );
bool DeleteDemo( LIST *l, int item );
bool ListDelete( LIST *l, int item );

char *AxisName( SLIDER *s );
char *AxisActionName( SLIDER *s );
bool SetAxisSlider( SLIDER *s );
bool SetAxisActionSlider( SLIDER *s );

void InitHostMenu( MENU *Menu );
void HostListPlayerSelected( MENUITEM *Item );
void InitWatchList( MENU *Menu );

/**************************
new text functions
***************************/
bool DrawTextEditCursor( TEXTINFO *TextInfo, int font );
void GetTextPos(TEXTINFO *TextInfo, float *xpos, float *ypos, int line, float totalheight);
bool DisplayTextCharacter(TEXTINFO *TextInfo, int line, int pos, int font, float totalheight);
void Print3Dots(TEXTINFO *TextInfo, float totalheight);
void DisplayTextItem (TEXTINFO *TextInfo);
void GetFinalChars( TEXTINFO *TextInfo, int currentpos, int length);
void PrintTextItem (TEXTINFO *TextInfo);
bool FormatTextItem(int start, int end, TEXTINFO *TextInfo, bool *newline);
int GetScreenFont(int fontsize);
bool Plot2dBox (TEXTINFO *TextInfo);
void InitScreenFonts (void);
TEXTINFO *InitTextInfo( void );
void DrawFlatMenuListText ( MENUITEM *Item );
void DrawLeftArrow( MENUITEM *Item );
void DrawRightArrow( MENUITEM *Item );
void DrawFlatMenuList( MENUITEM *Item );
void DrawSplitMenuList( MENUITEM *Item );
void AddToFlatMenuList( MENUITEM *Item );
void RedrawFlatMenuList( MENUITEM *Item );
void DrawFlatMenuText( MENUITEM *Item );
void DrawFlatTextStatus( MENUITEM *Item );
void DrawFlatMenuItem( MENUITEM *Item );
void DrawConditionalText( MENUITEM *Item );
void DrawConditionalName( MENUITEM *Item );
TEXTINFO * DrawFlatToggleStatus( MENUITEM *Item );
void UpdateSlider (MENUITEM *Item);
void DrawFlatSliderStatus( MENUITEM *Item );
void SelectFlatRadioButton( MENUITEM *Item );
void SelectFlatMenuToggle( MENUITEM *Iem );
void SelectMultiToggle( MENUITEM *Item );
void DrawFlatMenuToggle( MENUITEM *Item );

void DrawMessagesToggle( MENUITEM *Item);	// message colour toggle - in game

int MatureContent;
int HUDColour;
int InGameMenuColour;
int KillMessageColour;
int MilestoneMessagesColour;
int SystemMessageColour;
int FlagMessageColour;
int PlayerMessageColour;
int PickupMessageColour;
int TauntMessageColour;
int MyMessageColour;

void DrawColToggle( MENUITEM *Item);			// collisoin perspective - in game
void RedrawFlatMenuKey( MENUITEM *Item);
void SelectFlatMenuKey( MENUITEM *Item );
void CheckKeysForChanges( void );
void DrawFlatMenuKeyText( MENUITEM *Item );
void DrawFlatMenuKey( MENUITEM *Item );
void DrawFlatRadioButton( MENUITEM *Item );
void ProcessFlatSliders(void);
void RedrawFlatMenuSlider( MENUITEM *Item );
SLIDER *GetSlider( MENUITEM *Item, int *slidertype );
void DrawFlatMenuSlider( MENUITEM *Item );
void DrawFlatMenuName (MENUITEM *Item);
void DrawGeneralWeaponText( MENUITEM *Item );
void DrawGeneralWeapons( MENUITEM *Item );
void DrawSecondaryWeapons( MENUITEM *Item );
void StoreWeaponOrder( MENU *Menu );
void DrawPrimaryWeapons( MENUITEM *Item );
void GetBikeDetails(int Bike, MENUITEM *item);
void SelectFlatMenutext( MENUITEM *Item );
void SendTitleMessage(MENUITEM *Item);
void PrepareNextLevelStart( MENU *Menu );
void HostAboutToStart( MENUITEM *Item );
void ExitSoundMenu( MENU *Menu );
void MakeUnselectable( MENUITEM *Item );
void SendQuickText( MENUITEM *Item );
void SendQuickTextWhisper( MENUITEM *Item );
void InitCrystalsFoundText( MENU *Menu );
void SelectGoldBarModel( MENU *Menu );
void AfterSpecialMessage( MENUITEM *Item );
void NextLevelOrGameComplete( MENUITEM *Item );
bool SetUpLines (u_int16_t Model, PLANE plane, VECTOR *rot);
void KillLines(void);
void KillDerivedPlane(void);
void KillPlane(void);
void SwapBikes(void);
bool LoadBike(void);
void GetVduTextFormattingInfo (TEXT *t);
void RestoreDefaultControlSettings( MENUITEM *Item );
void PlayCursorSfx( void );
bool ChangeLevel( void );
void AddBikeArrow( char *name, int num );
void CheckMenuTimer( void );

/********************
Event Functions
*********************/
float GetEventCurrentTime(TITLE_EVENT *TitleEvent);
void InitEventGeneralPan(TITLE_EVENT *TitleEvent);
void InitEventDiscPan(TITLE_EVENT *TitleEvent);
void EventDiscPan(TITLE_EVENT *TitleEvent);
void ExitEventDiscPanFromRight(TITLE_EVENT *TitleEvent);
void ProcessEvents(void);
void ProcessTimers(void);
void SinPan2(VECTOR ViewFrom, VECTOR ViewTo, VECTOR *CurrentView,
			 VECTOR LookFrom, VECTOR LookTo, VECTOR *CurrentLook, float Time);
void InitEventRightVDUPan(TITLE_EVENT *TitleEvent);
void EventRightVDUPan(TITLE_EVENT *TitleEvent);
void ExitEventVDUPan(TITLE_EVENT *TitleEvent);
void InitEventLeftVDUPan(TITLE_EVENT *TitleEvent);
void EventLeftVDUPan(TITLE_EVENT *TitleEvent);
void ResetAllEvents(void);
void InitEventRotateOutRightVDU(TITLE_EVENT *TitleEvent);
void EventRotateOutRightVDU(TITLE_EVENT *TitleEvent);
void ExitEventRotateOutVDU(TITLE_EVENT *TitleEvent);
void RotateVDU2(float time, float start, float end);
void RotateBracket(float time, float start, float end);
void InitEventRotateInRightVDU(TITLE_EVENT *TitleEvent);
void EventRotateInRightVDU(TITLE_EVENT *TitleEvent);
void InitEventRotateOutLeftVDU(TITLE_EVENT *TitleEvent);
void EventRotateOutLeftVDU(TITLE_EVENT *TitleEvent);
void InitEventRotateInLeftVDU(TITLE_EVENT *TitleEvent);
void EventRotateInLeftVDU(TITLE_EVENT *TitleEvent);

void InitEventDarkenRoom(TITLE_EVENT *TitleEvent);
void EventDarkenRoom(TITLE_EVENT *TitleEvent);

void InitEventLightenRoom(TITLE_EVENT *TitleEvent);
void EventLightenRoom(TITLE_EVENT *TitleEvent);

void InitEventFadeOnHoloLight(TITLE_EVENT *TitleEvent);
void TriggerBikeSpeech( TITLE_EVENT *TitleEvent );
void EventFadeOnHoloLight(TITLE_EVENT *TitleEvent);

void InitEventFadeOffHoloLight(TITLE_EVENT *TitleEvent);
void ExitEventFadeOffHoloLight(TITLE_EVENT *TitleEvent);
void EventFadeOffHoloLight(TITLE_EVENT *TitleEvent);

void FadeHoloLight(float Brightness);

void InitEventRaiseHoloPad(TITLE_EVENT *TitleEvent);
void EventRaiseHoloPad(TITLE_EVENT *TitleEvent);
void InitEventLowerHoloPad(TITLE_EVENT *TitleEvent);
void EventLowerHoloPad(TITLE_EVENT *TitleEvent);
void InitEventJudderHoloPad(TITLE_EVENT *TitleEvent);
void ExitEventDiscPanInit( TITLE_EVENT *TitleEvent );
void InitEventLowerStack( TITLE_EVENT *TitleEvent );
void EventJudderHoloPad(TITLE_EVENT *TitleEvent);
void EventDiscPanInit(TITLE_EVENT *TitleEvent);
void InitEventDiscPanFromRightInit(TITLE_EVENT *TitleEvent);
void EventLowerStack(TITLE_EVENT *TitleEvent);
void ExitEventLowerStack(TITLE_EVENT *TitleEvent);
void InitEventJudderStack(TITLE_EVENT *TitleEvent);
void EventJudderStack(TITLE_EVENT *TitleEvent);
void InitEventExpandBike(TITLE_EVENT *TitleEvent);
void InitEventExpandCrystal(TITLE_EVENT *TitleEvent);
void ExitEventExpandBike(TITLE_EVENT *TitleEvent);
void DarkenRoom2(float darkness);
void DarkenRoomForStart( MENU *Menu );
void RotateDiscStack(float theta);

void InitEventShrinkBike(TITLE_EVENT *TitleEvent);

void KillBikeChar(MENU *Menu);
void KillLevelPic( MENU *Menu );
void LoadBikeChar(MENUITEM *Item);
void LoadLevelPic(MENUITEM *Item);
void LoadBikeCharPic(MENUITEM *Item);
void KillBikeCharPic( MENU *Menu );
bool LoadGeneralPic(float xmin, float ymin, float xmax, float ymax, FRAME_INFO **header, int *frame, u_int16_t *scrpoly, bool *displayed);
bool DisplayPicOnTV( FRAME_INFO **header, int *frame, u_int16_t *poly, bool *displayed, float zoffset );
void LoadSavedGamePic( char *file );

void ExitEventRotateInRightVDU(TITLE_EVENT *TitleEvent);
void ExitEventRaiseHolopad(TITLE_EVENT *TitleEvent);
void ExitEventJudderHoloPad(TITLE_EVENT *TitleEvent);


void InitStartDiscPanTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void InitGeneralTimer( TITLE_EVENT_TIMER *TitleEventTimer );
void InitRightDiscPanTimer(TITLE_EVENT_TIMER *TitleEventTimer);
void InitLeftDiscPanTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void InitRightVDUPanTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void InitLeftVDUPanTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void ChooseDiscTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void InitVisualsTimer (TITLE_EVENT_TIMER *TitleEventTimer);
void NewMenuSelectMode( MENUITEM *Item );
bool ProcessSlider2( int Key );
void SelectConnectionToStart (MENUITEM *Item);
void SelectConnectionToJoin (MENUITEM *Item);
void GetInitialPlayers ( MENU *menu );
void DrawReadyStatus( MENUITEM *Item );
void InitJoystickList ( MENU *Menu );
void SelectConfigureAxisMenu ( MENUITEM *Item );
void InitJoystickAxisConfig ( MENU *Menu);
void SelectVDUList ( MENUITEM *Item );
void InitShipActionList( void );
void AssignShipActionToJoyAxis( MENUITEM *Item);
void CheckJoyAxis( int *dummy );
void ExitJoySetup( MENU *Menu );
void ChooseJoyAxis( MENUITEM *Item );
void SetAxis( MENU *Menu );
void SetAxisInv( MENUITEM *Item );
void SetAxisFineControl( MENUITEM *Item );
void SetAxisDeadzone( MENUITEM *Item );
void SetAxisSensitivity( MENUITEM *Item );
void UpdateAxisPtr( float pos );
void InitModeCase(void);
void SetSoundLevels( int *dummy );
void ExitMouseSetup( MENU *Menu );
void InitAvgFrameRateGlobals( MENU *Menu );
void GoToNextLevel( MENUITEM *Item );
void TitleLoadGame( MENUITEM *Item );
void StartTimer( int timer );
void SaveMacros( MENUITEM *Item );
bool SetWaterDetail( SLIDER *slider );
bool SetTextScale( SLIDER *slider );
bool SetNumPrimaryPickups( SLIDER *slider );
void InitHostWaitingToStart( MENU *Menu );
void BackToJoinSession( MENUITEM *Item );

/***********************************
Highlight box functions
***********************************/
u_int16_t PlotHighlightPoly (float xmin, float ymin, float xmax, float ymax, u_int8_t r, u_int8_t g, u_int8_t b);
void ProcessHighlightBox(void);
void DrawHighlightBox(TEXTINFO *TextInfo, u_int8_t r, u_int8_t g, u_int8_t b);
bool ProcessWeaponOrder ( int Key );

/***********************************
text messaging
***********************************/
void AddTitleMessage(LPTEXTMSG LpTextMsg);
void GetTitleMessage(void);

/*===================================================================
	MultiPlayer Stuff...
===================================================================*/

extern BYTE	TeamNumber[MAX_PLAYERS];
extern bool	ShowStartPoints;

enum
{
	BIKE_FEATURE_Speed,
	BIKE_FEATURE_Accel,
	BIKE_FEATURE_Handling,
	BIKE_FEATURE_Power,
	BIKE_FEATURE_Shield,
	BIKE_FEATURE_Hull,
	BIKE_FEATURE_Missiles,
	BIKE_FEATURE_Ammo,
};

#define TITLE_TEXT_MSGS_STORED 6
char TitleMessage[TITLE_TEXT_MSGS_STORED][MAXTEXTMSG];
char TeamCurrentScore[MAX_TEAMS][64];

void SelectKeyDef( MENUITEM *Item );
void DrawKeyDef( MENUITEM *Item );

int SpeedFeat			= BIKE_FEATURE_Speed;
int AccelFeat			= BIKE_FEATURE_Accel;
int HandleFeat			= BIKE_FEATURE_Handling;
int PowerFeat			= BIKE_FEATURE_Power;
int ShieldFeat			= BIKE_FEATURE_Shield;
int HullFeat			= BIKE_FEATURE_Hull;
int MissileFeat			= BIKE_FEATURE_Missiles;
int AmmoFeat			= BIKE_FEATURE_Ammo;
int	OriginalSelection	= 0;
int	OriginalSliderValue;
int Team1				= 0;
int Team2				= 1;
int Team3				= 2;
int Team4				= 3;
int FramesIgnored		= 0;
int	DifficultyLevel		= DIFF_Norm;
int	ControlMethod		= CONTROL_Mouse;
int	GameType			= GAME_Normal;

// these are really only meant for displaying in create game menu
// the order of this list must match the order of game_t
char* GameTypeNameTable[7] = {
	LT_MENU_NEW_CreateGame9,  /*"free for all"*/
	LT_MENU_NEW_CreateGame12, /*"flag chase"*/
	LT_MENU_NEW_CreateGame13, /*"bounty hunt"*/
	LT_MENU_NEW_CreateGame14, /*"Team bounty hunt"*/
	LT_MENU_NEW_CreateGame10, /*"team game"*/
	LT_MENU_NEW_CreateGame11  /*"capture the flag"*/
};
char GameTypeName[128] = "";

float CharWidth;
float Pulse = 0.0F;

BYTE TeamNumber[MAX_PLAYERS];
BYTE HostGamePlayersWhoIAm[ MAX_PLAYERS ];

LIST TeamList[MAX_TEAMS];
LIST LoadSavedGameList		= { 0 };
LIST PlayersList					= { 0 };
LIST PilotList						= { 0 };
LIST HostPlayersList				= { 0 };
LIST DemoList						= { 0, 8 };	// list of all demo file in the current dir...
LIST BikeList						= { MAXBIKETYPES, 8, 0, 1, { "Lokasenna", "Beard", "L.A. Jay", "Ex-Cop", "Rex Hardy", "Foetoid", "Nim Soo Sun", "Nutta", "Sceptre", "Jo", "Cuvel Clark", "HK 5", "Nubia", "Mofisto", "Cerbero", "Slick", "FlyGirl" }, 0, 0 };
LIST BikeComputerList			= { MAXBIKECOMPTYPES, 8, 0, 1, { "phil 3b", "brenda", "lani-1", "Lepracom", "Roadster" },  0, 0 };
LIST TestList						= { 2, 12, 0, 0, { "test1", "test2" }, 0, 0 };
LIST RoomList						= { 0, 10 };
LIST *CurrentList					= NULL;
TEXT DemoGameName			= { 0, 0, "yourdemo", NULL, MAX_DEMONAME_LENGTH, TypeFileName };
TEXT TitlePlayerMessage		= { 0, 0, "", SendTitleMessage, MAXTEXTMSG, NULL };
TEXT PilotNameInGame			= { 0, 0, DEFAULT_PLAYER_NAME, SetPilotNameInGame, MAX_PILOTNAME_LENGTH, TypeFileName };
TEXT PilotName					= { 0, 0, DEFAULT_PLAYER_NAME, SetPilotName, MAX_PILOTNAME_LENGTH, TypeFileName };
TEXT PilotReName				= { 0, 0, DEFAULT_PLAYER_NAME, RenamePilotName, MAX_PILOTNAME_LENGTH, TypeFileName };
TEXT RoomName					= { 0, 0, "room", SetRoomName };
TEXT MacroText1					= { 0, 0, LT_MacroText1/*"you ugly son of a bitch..."*/, SaveMacros };
TEXT MacroText2					= { 0, 0, LT_MacroText2/*"Time to die..."*/, SaveMacros };
TEXT MacroText3					= { 0, 0, LT_MacroText3/*"I will tear your soul apart..."*/, SaveMacros };
TEXT MacroText4					= { 0, 0, "", NULL };
TEXT QuickText					= { 0, 0, "", SendQuickText };
TEXT QuickTextWhisper			= { 0, 0, "", SendQuickTextWhisper };
TEXT local_port_str				= { 0, 0, "", NULL };
TEXT host_port_str				= { 0, 0, "", NULL };
TEXT GamesLength				= { 0, 0, "", NULL };
TEXT TCPAddress					= { 0, 0, "", NULL};
TEXT OriginalText;

SLIDER SensitivityXSlider				= { 0, 16, 1, 5, 0, 0.0F };
SLIDER SensitivityYSlider				= { 0, 16, 1, 5, 0, 0.0F };
SLIDER WaterDetailSlider				= { 1, 2, 1, 2, 0, 0.0F, 0, 0, false, NULL, SetWaterDetail };
SLIDER NumPrimaryPickupsSlider		= { 1, (MAX_PLAYERS*2), 1, 1, 0, 0.0F, 0, 0, false, NULL, SetNumPrimaryPickups };
SLIDER PacketsSlider						= { 1, 100, 1, 5, 0, 0.0F };
SLIDER MyPacketsSlider						= { 1, 100, 1, 5, 0, 0.0F };
SLIDER PseudoHostTimeoutSlider1	= { 1, 10, 1, 2, 0, 0.0F };
SLIDER PseudoHostTimeoutSlider2	= { 1, 20, 1, 5, 0, 0.0F };
SLIDER NumOfPlayersSlider				= { 1, MAX_PLAYERS, 1, 1, 0, 0.0F };
SLIDER TimeLimit							= { 0, 30, 1, 0, 0, 0.0F };
SLIDER MyTimeLimit							= { 0, 30, 1, 0, 0, 0.0F };
SLIDER MaxPlayersSlider				= { 1, MAX_PLAYERS, 1, MAX_PLAYERS, 0, 0.0F };
SLIDER MaxKillsSlider						= { 0, 255, 1, 0, 0, 0.0F };
SLIDER GoalScoreSlider					= { 1, 10, 1, 5, 0, 0.0F };
SLIDER BountyBonusSlider				= { 1, 30, 1, 10, 0, 0.0F };
SLIDER CTFSlider							= { 0, CTF_MAX - 1, 1, CTF_STANDARD, -1, 0.0F, 0.0F, 0, false, CTF_Type };
SLIDER DemoSpeed						= { 1, 16, 1, 8, 0, 0.0F };
SLIDER SfxSlider							= { 0, 10, 1, 10, 0, 0.0F };
SLIDER BikerSpeechSlider				= { 0, 10, 1, 8, 0, 0.0F };
SLIDER BikeCompSpeechSlider			= { 0, 10, 1, 8, 0, 0.0F };
SLIDER DemoEyesSelect				= { 0, MAX_PLAYERS, 1, 0, 0, 0.0F };
SLIDER FlagSfxSlider						= { 0, 10, 1, 10, 0, 0.0F };
SLIDER WatchPlayerSelect				= { 0, MAX_PLAYERS+1, 1, 0, 0, 0.0F }; // which player's pov to watch
SLIDER TextScaleSlider				= { 0, 5, 1, 1, 0, 0.0F, 0.0F, 0, false, NULL, NULL, SetTextScale };

bool ShowTeamInfo;
bool MenuFrozen					= false;
bool NoTeamSelect				= false;
bool UseNewMenus				= true;
bool GameRestricted				= false;
bool Autoleveling				= true;
bool DebugInfo					= false;
bool GodMode					= false;
bool TexturesEnabled			= true;
bool ShowPlaneRGB				= false;
bool PlayDemo					= false;
bool PauseDemo					= false;
bool RecordDemo					= false;
bool BrightShips;
bool MyBrightShips;
bool BikeExhausts;
bool ScreenSaving				= true;
bool ShowNode					= false;
bool NodeCube					= false;
bool OldNodeCube				= false;
bool NodeCubeType				= false;
bool TeamGame					= false;
bool BountyBonus;
bool BikeEnginesOn;
bool ToggleTest;

bool	CanSelectBike[ MAXBIKETYPES ] =
{
	true,	// Borg,
	true,	// Beard,
	true,	// LaJay,
	true,	// Excop,
	true,	// Trucker,
	true,	// Foetoid,
	true,	// JapBird,
	true,	// Nutter,
	true,	// Rhesus,
	true,	// Joe,
	true,	// Shark,
	true,	// HK5,
	true,	// Nubia,
	true,	// Mofisto,
	true,	// Cerbero,
	true,	// Slick,
	true,	// FlyGirl,
};
int		AllowedBike[ ALLOWED_BIKETYPES ] =
{
	0,	// Borg,
	1,	// Beard,
	2,	// LaJay,
	3,	// Excop,
	4,	// Trucker,
	5,	// Foetoid,
	6,	// JapBird,
	7,	// Nutter,
	8,	// Rhesus,
	9,	// Joe,
	10,	// Shark,
	11,	// HK5,
	12,	// Nubia,
	13,	// Mofisto,
	14,	// Cerbero,
	15,	// Slick,
	16,	// FlyGirl,
};

#define BIKEFEATURESLIDER( VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7, VALUE8 ) \
	{ 0, 10, 1, (VALUE1), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE2), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE3), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE4), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE5), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE6), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE7), 0, 0.0F, 0.0F, 0, false },\
	{ 0, 10, 1, (VALUE8), 0, 0.0F, 0.0F, 0, false }

SLIDER BikeFeatureSlider[MAXBIKETYPES][8] = {
	{BIKEFEATURESLIDER(  4,	 5,	 6, 10,	 5,	5, 10, 10)},	// Lokasenna
	{BIKEFEATURESLIDER(  5,	 5,	 5, 10,	 5,	5, 10, 10)},    // Beard
	{BIKEFEATURESLIDER(  8,	 8,	 8, 10,	 3,	2, 10, 10)},    // L.A. Jay
	{BIKEFEATURESLIDER(  6,	 4,	 6, 10,	 6,	3, 10, 10)},	// Ex-Cop
	{BIKEFEATURESLIDER(  2,	 2,	 4, 10,	10,	5, 10, 10)},	// Rex Hardy
	{BIKEFEATURESLIDER(  5,	 4,	 4, 10,	 7,	5, 10, 10)},	// Foetoid
	{BIKEFEATURESLIDER(  5,	 5,	 4, 10,	 6,	5, 10, 10)},	// Nim Soo
	{BIKEFEATURESLIDER( 10,	 6,	 6, 10,	 1,	2, 10, 10)},	// Nutta
	{BIKEFEATURESLIDER(  2,	 2,	 4, 10,	 9,	5, 10, 10)},	// Sceptre
	{BIKEFEATURESLIDER(  5,	 9,	 6, 10,	 3,	3, 10, 10)},	// Jo
	{BIKEFEATURESLIDER( 10,	10,	10, 10,	 0,	0, 10, 10)},	// Cuvel Clark
	{BIKEFEATURESLIDER(  3,	 3,	 3, 10,	 8,	4, 10, 10)},	// HK-5
	{BIKEFEATURESLIDER(  6,	 8,	 6, 10,	 3,	4, 10, 10)},	// Nubia
	{BIKEFEATURESLIDER(  3,	 3,	 5, 10,	 5,	5, 10, 10)},	// Mofisto
	{BIKEFEATURESLIDER(  5,	 5,	 5, 10,	 5,	5, 10, 10)},	// Cerbero
	{BIKEFEATURESLIDER(  5,	 6,	 8, 10,	 6,	4, 10, 10)},	// Slick
	{BIKEFEATURESLIDER(  5,	 6,	 8, 10,	 6,	4, 10, 10)},	// flygirl
};

USERCONFIG	Config;

MENUSTATE MenuState			= MENUSTATE_Select;
MENUITEM *KeyItem				= NULL;
MENUITEM *SliderItem			= NULL;
MENUITEM *CurrentListItem	= NULL;
MENUITEM *TextItem			= NULL;
MENUITEM *PreListItem;
MENU *PreListMenu;

/*===================================================================
		Menus...
===================================================================*/

DEFKEY KDleft						= { &Config.left, 0 };
DEFKEY KDright					= { &Config.right, 0 };
DEFKEY KDup						= { &Config.up, 0 };
DEFKEY KDdown					= { &Config.down, 0 };
DEFKEY KDmove_left				= { &Config.move_left, 0 };
DEFKEY KDmove_right			= { &Config.move_right, 0 };
DEFKEY KDmove_up				= { &Config.move_up, 0 };
DEFKEY KDmove_down			= { &Config.move_down, 0 };
DEFKEY KDroll_left				= { &Config.roll_left, 0 };
DEFKEY KDroll_right				= { &Config.roll_right, 0 };
DEFKEY KDmove					= { &Config.move, 0 };
DEFKEY KDroll						= { &Config.roll, 0 };
DEFKEY KDmove_forward		= { &Config.move_forward, 0 };
DEFKEY KDmove_backward		= { &Config.move_backward, 0 };
DEFKEY KDturbo					= { &Config.turbo, 0 };
DEFKEY KDcruise_faster			= { &Config.cruise_faster, 0 };
DEFKEY KDcruise_slower		= { &Config.cruise_slower, 0 };
DEFKEY KDfire_primary			= { &Config.fire_primary, 0 };
DEFKEY KDfire_secondary		= { &Config.fire_secondary, 0 };
DEFKEY KDfire_mine				= { &Config.fire_mine, 0 };
DEFKEY KDdrop_primary			= { &Config.drop_primary, 0 };
DEFKEY KDdrop_secondary		= { &Config.drop_secondary, 0 };
DEFKEY KDdrop_shield			= { &Config.drop_shield, 0 };
DEFKEY KDdrop_ammo			= { &Config.drop_ammo, 0 };
DEFKEY KDselect_primary[]	= {
	{ &Config.select_primary[ PULSAR ], 0 },
	{ &Config.select_primary[ TROJAX ], 0 },
	{ &Config.select_primary[ PYROLITE_RIFLE ], 0 },
	{ &Config.select_primary[ TRANSPULSE_CANNON ], 0 },
	{ &Config.select_primary[ SUSS_GUN ], 0 },
	{ &Config.select_primary[ LASER ], 0 },
};
DEFKEY KDselect_next_primary		= { &Config.select_next_primary, 0 };
DEFKEY KDselect_prev_primary		= { &Config.select_prev_primary, 0 };
DEFKEY KDselect_next_secondary	= { &Config.select_next_secondary, 0 };
DEFKEY KDselect_prev_secondary	= { &Config.select_prev_secondary, 0 };
DEFKEY KDselect_secondary[] = {
	{ &Config.select_secondary[ MUGMISSILE ], 0 },
	{ &Config.select_secondary[ SOLARISMISSILE ], 0 },
	{ &Config.select_secondary[ THIEFMISSILE ], 0 },
	{ &Config.select_secondary[ SCATTERMISSILE ], 0 },
	{ &Config.select_secondary[ GRAVGONMISSILE ], 0 },
	{ &Config.select_secondary[ MULTIPLEMISSILE ], 0 },
	{ &Config.select_secondary[ TITANSTARMISSILE ], 0 },
	{ &Config.select_secondary[ PURGEMINE ], 0 },
	{ &Config.select_secondary[ PINEMINE ], 0 },
	{ &Config.select_secondary[ QUANTUMMINE ], 0 },
	{ &Config.select_secondary[ SPIDERMINE ], 0 },
};
DEFKEY KDsend_msg				= { &Config.send_msg, 0 };
DEFKEY KDheadlights				= { &Config.headlights, 0 };
DEFKEY KDfull_rearview			= { &Config.full_rear_view, 0 };
DEFKEY KDshow_messages		= { &Config.show_messages, 0 };
DEFKEY KDshow_stats			= { &Config.show_stats, 0 };
DEFKEY KDshow_networkinfo	= { &Config.show_networkinfo, 0 };

#ifdef PLAYER_SPEECH_TAUNTS
DEFKEY KDsend_speech = { &Config.send_speech, 0 };
#endif

#define KDEF( PAGE, X, Y, LABEL, KEY )\
		{ (X) +-100, (Y),0, 0, 0, (LABEL), 0, 0, NULL, (void *)(PAGE), NULL, MenuItemDrawPageName, NULL, 0 },\
		{ (X) + 208, (Y), 0, 0, 0, (LABEL), 0, 0, (void *)&(KEY), (void *)(PAGE), SelectKeyDef, DrawKeyDef, NULL, 0 }

#define KEYDEF( X, Y, LABEL, KEY )\
		{ (X) +   0, (Y),0, 0, 0, (LABEL), 0, 0, NULL, NULL, NULL, MenuItemDrawName, NULL, 0 },\
		{ (X) + 128, (Y), 0, 0, 0, (LABEL), 0, 0, KEY.key[ 0 ], NULL, SelectKey, DrawKey, NULL, 0 },\
		{ (X) + 198, (Y), 0, 0, 0, (LABEL), 0, 0, KEY.key[ 1 ], NULL, SelectKey, DrawKey , NULL, 0}

#define VDUKDEF( XMIN, YMIN, XMAX, YMAX, LABEL, KEY )\
		{ XMIN, YMIN, XMAX, YMAX, 0 , (LABEL), FONT_Small, TEXTFLAG_CentreY, NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0 },\
		{ XMAX + 3, YMIN, XMAX + 25, YMAX, 0, (LABEL), FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_CentreY, (void *)&(KEY), NULL, NULL, DrawFlatMenuKeyNum, NULL, 0 },\
		{ XMAX + 30, YMIN, XMAX + 120, YMAX, 0, (LABEL), FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)&(KEY), NULL, SelectFlatMenuKey, DrawFlatMenuKey, NULL, 0 }

#define VDUKEYDEF( XMIN, YMIN, XMAX, YMAX, LABEL, KEY )\
		{ XMIN, YMIN, XMAX, YMAX, 0 , (LABEL), FONT_Small, TEXTFLAG_CentreY, NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0 },\
		{ XMAX + 5, YMIN, XMAX + 55, YMAX, 0, (LABEL), FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, KEY.key[ 0 ], NULL, SelectFlatMenuKey, DrawFlatMenuKey, NULL, 0 },\
		{ XMAX + 60, YMIN, XMAX + 110, YMAX, 0, (LABEL), FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, KEY.key[ 1 ], NULL, SelectFlatMenuKey, DrawFlatMenuKey, NULL, 0 }


#define HELPKEY( X, Y, KEY, MSG )\
		{ (X), (Y), 0, 0, 0, (MSG), 0, 0, NULL, (KEY), NULL, DrawHelpKey, NULL, 0 }

#define OLDMENUITEM( X, Y, STR, VAR, VAL, SELECTFUNC, DRAWFUNC )\
		{ (X), (Y), 0, 0, 0, (STR), 0, 0, (VAR), (VAL), (SELECTFUNC), (DRAWFUNC), NULL, 0 }

MENU	MENU_NotYet = {
	"Not Implemented Yet", NULL, NULL, NULL, 0,
	{
		{ 200, 144, 0, 0, 0, "Go Back", 0, 0, NULL, NULL, MenuItemBack, MenuItemDrawName, NULL, 0 },
		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_BetweenLevels = {
	"", PrepareNextLevelStart, NULL, NULL, 0,
	{
		{  0,  0, 200,  20, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_ForceFit, (void *)SelectedLevel, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 36, 20, 164,  72, 0, "", 0, 0, NULL, NULL, NULL, LoadLevelPic, NULL, 0 },
		{ 10, 80, 190, 155, 0, "", FONT_Small, TEXTFLAG_ForceFit , (void *)SelectedLevelText, NULL, NULL, DrawFlatMenuName, NULL, 0  },

		{ -1, -1,   0,   0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

char CrystalsFoundText[ 64 ];
char GoldBarsFoundText[ 64 ];
char SecretsFoundText[ 64 ];
char EnemiesKilledText[ 64 ];
char TimeTakenText[ 64 ];
char GameCompleteText[ 256 ];
			       
char *GameCompleteWithoutCrystals			= LT_GameCompleteWithoutCrystals			/* "well done. You completed all the levels but you didn't get all the crystals. You'll have to do it all over again if you want to be properly rewarded."*/;
char *GameCompleteGotAllCrystalsSoFar		= LT_GameCompleteGotAllCrystalsSoFar		/* "well done, you got all the crystals. Take an extra life for the secret level. Find the crystal here for a special award."*/;
char *GameCompleteWithAllCrystals			= LT_GameCompleteWithAllCrystals			/* "well done, you got all the crystals. You get the secret biker."*/; 
char *GameCompleteWithAllCrystalsExceptLast = LT_GameCompleteWithAllCrystalsExceptLast	/* "well you got through the secret level, but you didn't get the final crystal. Better luck next time."*/;
char *GameCompleteWithCheating				= LT_GameCompleteWithCheating				/* "well, you completed the game but you cheated, didn't you. If you want to see the secret level and have a chance to get a special reward, you'll have to do it properly!"*/;
char *GameCompleteAdditionalLevels			= LT_GameCompleteAdditionalLevels			/* "congratulations. You have completed all the currently installed additional levels. Look out for further mission packs!!"*/;

MENU MENU_NEW_SpecialMessage = {
	"", NULL, NULL, NULL, TITLE_TIMER_LowerHolopad,
	{
		{ 30,   0, 170, 150, 0, "", FONT_Medium, TEXTFLAG_CentreYBunched,  (void *)&GameCompleteText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{  0, 150, 200, 170, 0, LT_MENU_NEW_SpecialMessage0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, AfterSpecialMessage, DrawFlatMenuItem, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_NumberOfSecretsEnemys = {
	"", NULL, NULL, NULL, TITLE_TIMER_LowerHolopad,
	{
		{ 30,   0, 170,  50, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreYBunched,  (void *)&SecretsFoundText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 30,  50, 170, 100, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreYBunched,  (void *)&EnemiesKilledText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 30, 100, 170, 150, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreYBunched,  (void *)&TimeTakenText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{  0, 150, 200, 170, 0, LT_MENU_NEW_NumberOfSecretsEnemys0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NextLevelOrGameComplete, DrawFlatMenuItem, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_NumberOfGoldBars = {
	"", SelectGoldBarModel, NULL, NULL, TITLE_TIMER_InterLevelPan,
	{
		{ 30,   0, 170, 150, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreYBunched,  (void *)&GoldBarsFoundText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{  0, 150, 200, 170, 0, LT_MENU_NEW_NumberOfGoldBars0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_NumberOfSecretsEnemys, MenuChange, DrawFlatMenuItem, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_NumberOfCrystals = {
	"", InitCrystalsFoundText, NULL, NULL, TITLE_TIMER_InterLevelPan,
	{
		{ 30,   0, 170, 150, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreYBunched,  (void *)&CrystalsFoundText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{  0, 150, 200, 170, 0, LT_MENU_NEW_NumberOfCrystals0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_NumberOfGoldBars, MenuChange, DrawFlatMenuItem, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/* select single player level */

MENU	MENU_NEW_StartSinglePlayer = {
	"", InitSinglePlayerGame, NULL, NULL, TITLE_TIMER_NormalPanToRightVDU,
	{

		{  0,  0, 200, 20, 0,
			LT_MENU_NEW_StartSinglePlayer0/*"single player Game"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,
			NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 0, 50, 200, 50, 0,
			LT_MENU_NEW_StartSinglePlayer2/*"select level"*/, FONT_Medium, TEXTFLAG_CentreY | TEXTFLAG_CentreX,
			NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0 } ,

		{ 0, 90, 200, 50, 0,
			"", /* level name */ FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY |
											TEXTFLAG_AutoSelect | TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit,
			(void *)SelectedLevel, NULL, InitLevelSelectVDU, DrawFlatMenuName, NULL, 0 } ,

		{ 0, 200, 200, 50, 0,
			LT_MENU_NEW_StartSinglePlayer1/*"start"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,
			NULL, &MENU_NEW_BetweenLevels, MenuChange, DrawFlatMenuItem, NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Error = {
	"", NULL, NULL, NULL, 0,
	{
		{ 0,   0, 200, 150, 0, "", FONT_Small, TEXTFLAG_BottomY | TEXTFLAG_CentreX | TEXTFLAG_CentreY, (void *)ErrorMessage, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 0, 150, 200, 170, 0, LT_MENU_NEW_Error0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, MenuItemBackFromError, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Notify = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{ 0,   0, 200, 150, 0, "", FONT_Small, TEXTFLAG_BottomY | TEXTFLAG_CentreX | TEXTFLAG_CentreY, (void *)NotifyMessage, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 0, 150, 200, 170, 0, LT_MENU_NEW_Error0/*ok*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, TitleReset, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};


MENU	MENU_NEW_GeneralLoading = {
	"", NULL, NULL, NULL, 0,
	{
		{ 0, 0, 200, 150, 0, LT_MENU_NEW_GeneralLoading0/*loading..."*/, FONT_Medium, TEXTFLAG_BottomY | TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_HostWaitingToStart = {
	"", InitHostWaitingToStart, BailMultiplayerFrontEnd, UpdateSessions, TITLE_TIMER_PanToLeftVDU,
	{
		{   0,   0, 200,  10, 0, LT_MENU_NEW_HostWaitingToStart0/*"waiting to start..."*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{   5,  15,  90,  25, 0, LT_MENU_NEW_HostWaitingToStart1/*"start"*/, FONT_Medium, TEXTFLAG_CentreY,  NULL, &MENU_NEW_GeneralLoading, HostAboutToStart/*GoToSynchup*/, DrawFlatMenuItem, NULL, 0  },
		{   5,  27, 100,  37, SLIDER_Value, LT_MENU_NEW_HostWaitingToStart3/*"Num of Players"*/, FONT_Small, TEXTFLAG_CentreY, &NumOfPlayersSlider, NULL, NULL, DrawFlatMenuSlider, NULL, 0 },
		{   5,  40, 100, 125, 0, "", FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, &PlayersList, NULL , NULL , DrawFlatMenuList, NULL, 0 } ,
		{   5, 112, 200, 119, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[5], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{   5, 119, 200, 126, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[4], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{   5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{   5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{   5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{   5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,

		{   5, 160,  25, 160, 0, LT_MENU_NEW_HostWaitingToStart4/*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_SelectLevel = {
	"", LoadLevelText, KillLevelPic, NULL, 0,
	{
		{  0,  0, 200,  20, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_ForceFit, (void *)SelectedLevel, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 30, 20, 168,  76, 0, "", 0, 0, NULL, NULL, NULL, LoadLevelPic, NULL, 0 },
		{ 10, 84, 190, 155, 0, "", FONT_Small, TEXTFLAG_ForceFit , (void *)SelectedLevelText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
	 
		{ -1, -1, 0, 0, 0,"", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENUITEM SlowLevelSelectItem = { 0, 0, 0, 0, 0, "", 0, 0, NULL, &MENU_NEW_SelectLevel, NULL, NULL, NULL, 0 };

MENU	MENU_NEW_ValidPickups_Second_Page = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{  0,   0, 200,  20, 0, LT_MENU_NEW_ValidPickups0	/*"allowed pickups"*/,	FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,	NULL,				DrawFlatMenuItem,	NULL, 0 },
		
		{ 10,  28, 200,  36, 0, LT_MENU_NEW_ValidPickups24	/*"missiles"*/,			FONT_Small,		TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,	NULL,				DrawFlatMenuItem,	NULL, 0 } ,

		{ 10,  44, 120,  52, 0, LT_MENU_NEW_ValidPickups6	/*mug*/,				FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Mugs ],				NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  52, 120,  60, 0, LT_MENU_NEW_ValidPickups7	/*"solaris"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_HeatseakerPickup ],	NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  60, 120,  68, 0, LT_MENU_NEW_ValidPickups8	/*"scatter"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Scatter ],			NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  68, 120,  76, 0, LT_MENU_NEW_ValidPickups9	/*"gravgon"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Gravgon ],			NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  76, 120,  84, 0, LT_MENU_NEW_ValidPickups10	/*"mfrl"*/,				FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Launcher ],			NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  84, 120,  92, 0, LT_MENU_NEW_ValidPickups11	/*"titan"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_TitanStar ],			NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  92, 120, 100, 0, LT_MENU_NEW_ValidPickups20	/*"thief missle"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Thief ],				NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		
		{ 10, 108, 200, 116, 0, LT_MENU_NEW_ValidPickups25	/*"mines"*/,			FONT_Small,		TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,	NULL,				DrawFlatMenuItem,	NULL, 0 } ,

		{ 10, 124, 120, 132, 0, LT_MENU_NEW_ValidPickups12	/*"purge mine"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_PurgePickup ],		NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 132, 120, 140, 0, LT_MENU_NEW_ValidPickups13	/*"pine mine"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_PinePickup ],		NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 140, 120, 148, 0, LT_MENU_NEW_ValidPickups14	/*"quantum mine"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_QuantumPickup ],		NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 148, 120, 156, 0, LT_MENU_NEW_ValidPickups21	/*"spider mine"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_SpiderPod ],			NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_ValidPickups = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{  0,   0, 200,  20, 0, LT_MENU_NEW_ValidPickups0				/*"allowed pickups"*/,	FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,								NULL,				DrawFlatMenuItem,	NULL, 0 } ,
		
		{ 10,  28, 200,  36, 0, LT_MENU_NEW_ValidPickups22				/*"primary weapons"*/,	FONT_Small,		TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,								NULL,				DrawFlatMenuItem,	NULL, 0 } ,
		{ 10,  44, 120,  52, 0, LT_MENU_NEW_ValidPickups1				/*"trojax"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Trojax ],				NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  52, 120,  60, 0, LT_MENU_NEW_ValidPickups2				/*"pyrolite"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Pyrolite ],			NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  60, 120,  68, 0, LT_MENU_NEW_ValidPickups3				/*"transpulse"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Transpulse ],			NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  68, 120,  76, 0, LT_MENU_NEW_ValidPickups4				/*"suss gun"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_SussGun ],				NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10,  76, 120,  84, 0, LT_MENU_NEW_ValidPickups5				/*laser*/,				FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Laser ],				NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		
		{ 10,  92, 200, 100, 0, LT_MENU_NEW_ValidPickups23				/*"power-ups"*/,		FONT_Small,		TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,										NULL,								NULL,				DrawFlatMenuItem,	NULL, 0 } ,
		{ 10, 108, 120, 116, 0, LT_MENU_NEW_ValidPickups15				/*"invulnerability"*/,	FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Inv ],					NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 116, 120, 124, 0, LT_MENU_NEW_ValidPickups16				/*"nitro"*/,			FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Nitro ],				NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 124, 120, 132, 0, LT_MENU_NEW_ValidPickups17				/*"stealth mantle"*/,	FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Mantle ],				NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 132, 120, 140, 0, LT_MENU_NEW_ValidPickups18				/*"orbit pulsar"*/,		FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_Orb ],					NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		{ 10, 140, 120, 148, 0, LT_MENU_NEW_ValidPickups19				/*"golden power pod"*/, FONT_Small,		TEXTFLAG_CentreY,						&MyPickupValid[ PICKUP_GoldenPowerPod ],		NULL,								SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 } ,
		
		{ 10, 156, 120, 164, 0, LT_MENU_NEW_MoreMultiplayerOptions22	/*"next page"*/,		FONT_Small,		TEXTFLAG_CentreY,						NULL,										&MENU_NEW_ValidPickups_Second_Page, MenuChange,			DrawFlatMenuItem,	NULL, 0 } ,
		
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/*
MENU	MENU_NEW_MoreMultiplayerOptions = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{  0,   0, 200,  20, 0,				"Other options",													FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,		NULL,						NULL,					NULL,						DrawFlatMenuItem,		NULL, 0 } ,

		{ 10,  86, 100,  93, 0,				LT_MENU_NEW_MoreMultiplayerOptions7  /* "bike exhausts"          , FONT_Small,  TEXTFLAG_CentreY,							&BikeExhausts,				NULL,					SelectFlatMenuToggle,		DrawFlatMenuToggle,		NULL, 0 } ,
		{ 10,  93, 100, 100, 0,				LT_MENU_NEW_MoreMultiplayerOptions8  /* "bright bikes"           , FONT_Small,  TEXTFLAG_CentreY,							&MyBrightShips,				NULL,					SelectFlatMenuToggle,		DrawFlatMenuToggle,		NULL, 0 } ,
		
		{  10, 116,  85, 116, 0,			LT_MENU_NEW_MoreMultiplayerOptions14/*"demo"        , FONT_Small,		TEXTFLAG_CentreY,													&RecordDemo,						NULL,								SelectFlatMenuToggle,	DrawFlatMenuToggle,		NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};
*/

void GameTypeRefresh( int * i )
{
	BountyBonusInterval = ( BountyBonus ) ? BountyBonusSlider.value : 0;
}

MENU MENU_NEW_GameType = {
	"",NULL,NULL,GameTypeRefresh,TITLE_TIMER_PanToLeftVDU,
	{		

		{   0,   0, 200,   0, 0,	LT_MENU_NEW_CreateGame8  /*"game type"*/,				FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0 } ,

		{  10,   8, 200,  16, 0,	LT_MENU_NEW_CreateGame9	 /*"free for all"*/,			FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_Normal,		SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{  10,  16, 200,  24, 0,	LT_MENU_NEW_CreateGame10 /*"team game"*/,				FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_Team,			SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{  10,  24, 200,  32, 0,	LT_MENU_NEW_CreateGame11 /*"capture the flag"*/,		FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_CTF,			SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{  10,  32, 200,  40, 0,	LT_MENU_NEW_CreateGame12 /*"flag chase"*/,				FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_FlagChase,	SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{  10,  40, 200,  48, 0,	LT_MENU_NEW_CreateGame13 /*"bounty hunt"*/,				FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_BountyHunt,	SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{  10,  48, 200,  56, 0,	LT_MENU_NEW_CreateGame14 /*"Team bounty hunt"*/,		FONT_Small,		TEXTFLAG_CentreY,	&GameType,	(void *)GAME_TeamBountyHunt,	SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,

		{  10,  68, 200,  68, 0,			"Game Type Options",												FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,		NULL,						NULL,					NULL,						DrawFlatMenuItem,		NULL, 0 } ,

		{  10,  92, 120,  92, 0,			LT_MENU_NEW_MoreMultiplayerOptions10 /* "Bounty bonus"           */, FONT_Small, TEXTFLAG_CentreY,							&BountyBonus,				NULL,					SelectFlatMenuToggle,		DrawFlatMenuToggle,		NULL, 0 } ,
		{  10, 100, 120, 100, SLIDER_Value,	LT_MENU_NEW_MoreMultiplayerOptions12 /* "bounty bonus interval"  */, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&BountyBonusSlider,			NULL,					SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{  10, 108, 120, 108, SLIDER_Value,	LT_MENU_NEW_MoreMultiplayerOptions11 /* "flag capture score"     */, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&GoalScoreSlider,			NULL,					SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{  10, 116, 120, 116, SLIDER_User,	LT_MENU_NEW_MoreMultiplayerOptions13 /* "own flag rule"          */, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&CTFSlider,					NULL,					SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_NetworkOptions = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{  0, 16, 200, 16, 0,			"Network Options",															FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0 } ,

		{ 10, 32,  85, 32, 0,			LT_MENU_NEW_MoreMultiplayerOptions2/*"short packets"*/,						FONT_Small,	TEXTFLAG_CentreY,							&MyUseShortPackets,			NULL,						SelectFlatMenuToggle,	DrawFlatMenuToggle,		NULL, 0 } ,
		{ 10, 40,  85, 40, SLIDER_Value,LT_MENU_NEW_MoreMultiplayerOptions4/*"packet rate"*/,						FONT_Small,	TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,		&MyPacketsSlider,				NULL,						SelectSlider,			DrawFlatMenuSlider,		NULL, 0 } ,

		{ 10, 56,  85, 56, 0,			LT_MENU_NEW_MoreMultiplayerOptions1a /*target collision perspective"*/,		FONT_Small, TEXTFLAG_CentreY,							&MyColPerspective,			(void *)COLPERS_Descent,	SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,
		{ 10, 64,  85, 64, 0,			LT_MENU_NEW_MoreMultiplayerOptions2a /*"shooter collision perspective"*/,	FONT_Small, TEXTFLAG_CentreY,							&MyColPerspective,			(void *)COLPERS_Forsaken,	SelectFlatRadioButton,	DrawFlatRadioButton,	NULL, 0 } ,

		{ 10, 78,  85, 78, 0,			"enable tracker",															FONT_Small, TEXTFLAG_CentreY,							&tracker_enabled,			NULL,						SelectFlatMenuToggle,	DrawFlatMenuToggle,		NULL, 0 } ,

		{ 10, 88,  85, 88, 0,			"local port",																FONT_Small,	TEXTFLAG_ForceFit | TEXTFLAG_CentreY,		&local_port_str,			NULL,						SelectFlatMenutext,		DrawFlatMenuText,		NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/* create */
MENU	MENU_NEW_CreateGame = {

	"",
	InitMultiplayerHostVDUPeerPeer,
	ExitMultiplayerHostVDUPeerPeer,
	RedrawMultiplayerHostVDUPeerPeer,
	TITLE_TIMER_PanToLeftVDU,

	{
		{   0,  15, 200,  15, 0,			LT_MENU_NEW_CreateGame0  /*"Create Multiplayer Game"*/, FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,								NULL,								NULL,								NULL,					DrawFlatMenuItem,		NULL, 0 } ,
		{  10,  36,  90,  36, 0,			LT_MENU_NEW_CreateGame1  /*"Start game"*/,				FONT_Small,		TEXTFLAG_CentreY,													NULL,								&MENU_NEW_HostWaitingToStart,		StartAHostSession,		DrawFlatMenuItem,		NULL, 0 } ,

		{  10,  51,  85,  51, 0,			LT_MENU_NEW_CreateGame8  /*"game type"*/,				FONT_Small,		TEXTFLAG_CentreY,													NULL,								&MENU_NEW_GameType,					MenuChange,				DrawFlatMenuItem,		NULL, 0 } ,
		{  90,  51, 200,  51, 0,			"",														FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit,	(void *)GameTypeName,				NULL,								NULL,					DrawFlatMenuName,		NULL, 0 } ,

		{  10,  59,  50,  59, 0,			LT_MENU_NEW_CreateGame4  /*"level"*/,					FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_AutoSelect,								NULL,								NULL,								InitLevelSelectVDU,		DrawFlatMenuItem,		NULL, 0 } ,
		{  90,  59, 200,  59, 0,			"",														FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit,	(void *)SelectedLevel,				NULL,								NULL,					DrawFlatMenuName,		NULL, 0 } ,

		{  10,  75,  85,  75, SLIDER_Value, LT_MENU_NEW_CreateGame5  /*"player limit"*/,			FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_AutoSelect,								&MaxPlayersSlider,					NULL,								SelectSlider,			DrawFlatMenuSlider,		NULL, 0 } ,
		{  10,  83,  85,  83, SLIDER_Value, LT_MENU_NEW_CreateGame6  /*"score limit"*/,				FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_AutoSelect,								&MaxKillsSlider,					NULL,								SelectSlider,			DrawFlatMenuSlider,		NULL, 0 } ,
		{  10,  91,  85,  91, SLIDER_Time,	LT_MENU_NEW_CreateGame7  /*"time limit"*/,				FONT_Small,		TEXTFLAG_CentreY | TEXTFLAG_AutoSelect,								&MyTimeLimit,						NULL,								SelectSlider,			DrawFlatMenuSlider,		NULL, 0 } ,

		{  10, 107,  85, 107, 0,			"Reset Kills"            /*"Reset Kills"*/,				FONT_Small,		TEXTFLAG_CentreY,													&MyResetKillsPerLevel,				NULL,								SelectFlatMenuToggle,	DrawFlatMenuToggle,		NULL, 0 } ,
		{  10, 115,  85, 115, SLIDER_Value,	LT_MENU_NEW_MoreMultiplayerOptions21/*"num weapons"	*/, FONT_Small,		TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,								&NumPrimaryPickupsSlider,			NULL,								SelectSlider,			DrawFlatMenuSlider,		NULL, 0 } ,
		{  10, 124,  85, 124, 0,			LT_MENU_NEW_MoreMultiplayerOptions20/*"randomize"   */, FONT_Small,		TEXTFLAG_CentreY,													&MyRandomPickups,					NULL,								SelectFlatMenuToggle,	DrawFlatMenuToggle,		NULL, 0 } ,

		{  10, 139, 180, 139, 0,			LT_MENU_NEW_MoreMultiplayerOptions19/*"pickups" */,		FONT_Small,		TEXTFLAG_CentreY,													NULL,								&MENU_NEW_ValidPickups,				MenuChange,				DrawFlatMenuItem,		NULL, 0 } ,
		{  10, 147, 100, 147, 0,			"Network Options",										FONT_Small,		TEXTFLAG_CentreY,													NULL,								&MENU_NEW_NetworkOptions,			MenuChange,				DrawFlatMenuItem,		NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENUITEM MENU_ITEM_IdiotText = 
		{ 0, 150, 200, 170, 0, "", FONT_Small, TEXTFLAG_CentreY,  NULL, NULL, NULL, NULL, NULL, 0  };

MENU	MENU_NEW_PrimaryOrder = {
	"", NULL, StoreWeaponOrder, NULL, 0,
	{
		{ 10, 0, 190, 35, 0, LT_MENU_NEW_PrimaryOrder0 /*"primary weapon selection preferences"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 5, 50, 95, 180, 0, "", FONT_Small, TEXTFLAG_CentreY  | TEXTFLAG_ForceFit,  &Config.primary_order, NULL, NULL, DrawPrimaryWeapons, NULL, 0  },
		{ 5, 35, 95, 50, 0, LT_MENU_NEW_PrimaryOrder1 /*"auto"*/, FONT_Medium, TEXTFLAG_CentreY ,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 105, 35, 195, 50, 0, LT_MENU_NEW_PrimaryOrder2 /*"manual"*/, FONT_Medium, TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_SecondaryOrder = {
	"", NULL, StoreWeaponOrder, NULL, 0,
	{
		{ 10, 0, 190, 35, 0, LT_MENU_NEW_SecondaryOrder0 /*"secondary weapon selection preferences"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 5, 50, 95, 180, 0, "", FONT_Small, TEXTFLAG_CentreY | TEXTFLAG_ForceFit,  &Config.primary_order, NULL, NULL, DrawSecondaryWeapons, NULL, 0  },
		{ 5, 35, 95, 50, 0, LT_MENU_NEW_SecondaryOrder1 /*"auto"*/, FONT_Medium, TEXTFLAG_CentreY ,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 105, 35, 195, 50, 0, LT_MENU_NEW_SecondaryOrder2 /*"manual"*/, FONT_Medium, TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};



MENU	MENU_NEW_SecondaryWeaponKeys = {
#ifdef VDUKDEF
	"", InitKeyDef, NULL, NULL, 0,
#else
	"", InitControls, ExitControls, NULL, 0,
#endif
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_SecondaryWeaponKeys0 /*"secondary weapon"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

#ifdef VDUKDEF
		VDUKDEF( 3, 20, 89, 28, LT_MENU_NEW_SecondaryWeaponKeys1 /*"fire secondary"*/, KDfire_secondary ),
		VDUKDEF( 3, 28, 89, 36, LT_MENU_NEW_SecondaryWeaponKeys2 /*"drop mine"*/, KDfire_mine ),

		VDUKDEF( 3, 42, 89, 50, LT_MENU_NEW_SecondaryWeaponKeys3 /*"mug missile"*/, KDselect_secondary[ MUGMISSILE ] ),
		VDUKDEF( 3, 50, 89, 58, LT_MENU_NEW_SecondaryWeaponKeys4 /*"solaris missile"*/, KDselect_secondary[ SOLARISMISSILE ] ),
		VDUKDEF( 3, 58, 89, 66, LT_MENU_NEW_SecondaryWeaponKeys5 /*"scatter missile"*/, KDselect_secondary[ SCATTERMISSILE ] ),
		VDUKDEF( 3, 66, 89, 74, LT_MENU_NEW_SecondaryWeaponKeys6 /*"gravgon missile"*/, KDselect_secondary[ GRAVGONMISSILE ] ),
		VDUKDEF( 3, 74, 89, 82, LT_MENU_NEW_SecondaryWeaponKeys7 /*"mfrl"*/, KDselect_secondary[ MULTIPLEMISSILE ] ),
		VDUKDEF( 3, 82, 89, 90, LT_MENU_NEW_SecondaryWeaponKeys8 /*"Titan Missile"*/, KDselect_secondary[ TITANSTARMISSILE ] ),
		VDUKDEF( 3, 90, 89, 98, LT_MENU_NEW_SecondaryWeaponKeys29 /*"Thief Missile"*/, KDselect_secondary[ THIEFMISSILE ] ),
		
		VDUKDEF( 3,  98, 89, 106, LT_MENU_NEW_SecondaryWeaponKeys9 /*"purge mine"*/, KDselect_secondary[ PURGEMINE ] ),
		VDUKDEF( 3, 106, 89, 114, LT_MENU_NEW_SecondaryWeaponKeys10 /*"pine mine"*/, KDselect_secondary[ PINEMINE ] ),
		VDUKDEF( 3, 114, 89, 122, LT_MENU_NEW_SecondaryWeaponKeys11 /*"quantum mine"*/, KDselect_secondary[ QUANTUMMINE ] ),
		VDUKDEF( 3, 122, 89, 130, LT_MENU_NEW_SecondaryWeaponKeys30 /*"spider mine"*/, KDselect_secondary[ SPIDERMINE ] ),

		VDUKDEF( 3, 136, 89, 144, LT_MENU_NEW_SecondaryWeaponKeys12 /*"Next weapon"*/, KDselect_next_secondary ),
		VDUKDEF( 3, 144, 89, 152, LT_MENU_NEW_SecondaryWeaponKeys13 /*"Prev weapon"*/, KDselect_prev_secondary ),

		VDUKDEF( 3, 158, 89, 166, LT_MENU_NEW_SecondaryWeaponKeys14 /*"drop weapon"*/, KDdrop_secondary ),
#else
		VDUKEYDEF( 3, 20, 89, 28, LT_MENU_NEW_SecondaryWeaponKeys15 /*"fire secondary:"*/, (void *)&Config.fire_secondary ),
		VDUKEYDEF( 3, 28, 89, 36, LT_MENU_NEW_SecondaryWeaponKeys16 /*"drop mine:"*/, (void *)&Config.fire_mine ),

		VDUKEYDEF( 3, 42, 89, 50, LT_MENU_NEW_SecondaryWeaponKeys17 /*"mug missile:"*/, (void *)&Config.select_secondary[ MUGMISSILE ] ),
		VDUKEYDEF( 3, 50, 89, 58, LT_MENU_NEW_SecondaryWeaponKeys18 /*"solaris missile:"*/, (void *)&Config.select_secondary[ SOLARISMISSILE ] ),
		VDUKEYDEF( 3, 58, 89, 66, LT_MENU_NEW_SecondaryWeaponKeys19 /*"scatter missile:"*/, (void *)&Config.select_secondary[ SCATTERMISSILE ] ),
		VDUKEYDEF( 3, 66, 89, 74, LT_MENU_NEW_SecondaryWeaponKeys20 /*"gravgon missile:"*/, (void *)&Config.select_secondary[ GRAVGONMISSILE ] ),
		VDUKEYDEF( 3, 74, 89, 82, LT_MENU_NEW_SecondaryWeaponKeys21 /*"mfrl:"*/, (void *)&Config.select_secondary[ MULTIPLEMISSILE ] ),
		VDUKEYDEF( 3, 82, 89, 90, LT_MENU_NEW_SecondaryWeaponKeys22 /*"Titan Missile:"*/, (void *)&Config.select_secondary[ TITANSTARMISSILE ] ),
		VDUKEYDEF( 3, 90, 89, 98, LT_MENU_NEW_SecondaryWeaponKeys29 /*"Thief Missile"*/, (void *)&Config.select_secondary[ THIEFMISSILE ] ),
		
		VDUKEYDEF( 3, 98, 89, 106, LT_MENU_NEW_SecondaryWeaponKeys23 /*"purge mine:"*/, (void *)&Config.select_secondary[ PURGEMINE ] ),
		VDUKEYDEF( 3, 106, 89, 114, LT_MENU_NEW_SecondaryWeaponKeys24 /*"pine mine:"*/, (void *)&Config.select_secondary[ PINEMINE ] ),
		VDUKEYDEF( 3, 114, 89, 122, LT_MENU_NEW_SecondaryWeaponKeys25 /*"quantum mine:"*/, (void *)&Config.select_secondary[ QUANTUMMINE ] ),
		VDUKEYDEF( 3, 122, 89, 130, LT_MENU_NEW_SecondaryWeaponKeys30 /*"spider mine"*/, (void *)&Config.select_secondary[ SPIDERMINE ] ),

		VDUKEYDEF( 3, 136, 89, 144, LT_MENU_NEW_SecondaryWeaponKeys26 /*"Next weapon:"*/, (void *)&Config.select_next_secondary ),
		VDUKEYDEF( 3, 144, 89, 152, LT_MENU_NEW_SecondaryWeaponKeys27 /*"Prev weapon:"*/, (void *)&Config.select_prev_secondary ),

		VDUKEYDEF( 3, 158, 89, 166, LT_MENU_NEW_SecondaryWeaponKeys28 /*"drop weapon:"*/, (void *)&Config.drop_secondary ),
#endif		
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_PrimaryWeaponKeys = {
#ifdef VDUKDEF
	"", InitKeyDef, NULL, NULL, 0,
#else
	"", InitControls, ExitControls, NULL, 0,
#endif
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_PrimaryWeaponKeys0 /*"primary weapon"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
#ifdef VDUKDEF
		VDUKDEF( 3, 20, 89, 28, LT_MENU_NEW_PrimaryWeaponKeys1 /*"fire primary"*/, KDfire_primary ),

		VDUKDEF( 3, 34, 89, 42, LT_MENU_NEW_PrimaryWeaponKeys2 /*"pulsar"*/, KDselect_primary[ PULSAR ] ),
		VDUKDEF( 3, 42, 89, 50, LT_MENU_NEW_PrimaryWeaponKeys3 /*"trojax"*/, KDselect_primary[ TROJAX ] ),
		VDUKDEF( 3, 50, 89, 58, LT_MENU_NEW_PrimaryWeaponKeys4 /*"pyrolite"*/, KDselect_primary[ PYROLITE_RIFLE ] ),
		VDUKDEF( 3, 58, 89, 66, LT_MENU_NEW_PrimaryWeaponKeys5 /*"transpulse"*/, KDselect_primary[ TRANSPULSE_CANNON ] ),
		VDUKDEF( 3, 66, 89, 74, LT_MENU_NEW_PrimaryWeaponKeys6 /*"suss gun"*/, KDselect_primary[ SUSS_GUN ] ),
		VDUKDEF( 3, 74, 89, 82, LT_MENU_NEW_PrimaryWeaponKeys7 /*"laser"*/, KDselect_primary[ LASER ] ),

		VDUKDEF( 3, 88, 89, 96, LT_MENU_NEW_PrimaryWeaponKeys8 /*"Next weapon"*/, KDselect_next_primary ),
		VDUKDEF( 3, 96, 89, 104, LT_MENU_NEW_PrimaryWeaponKeys9 /*"Prev weapon"*/, KDselect_prev_primary ),

		VDUKDEF( 3, 110, 89, 118, LT_MENU_NEW_PrimaryWeaponKeys10 /*"drop weapon"*/, KDdrop_primary ),

		VDUKDEF( 3, 124, 89, 132, LT_MENU_NEW_PrimaryWeaponKeys11 /*"send msg"*/, KDsend_msg ),
#else
		VDUKEYDEF( 3, 20, 89, 28, LT_MENU_NEW_PrimaryWeaponKeys12 /*"fire primary:"*/, (void *)&Config.fire_primary ),

		VDUKEYDEF( 3, 34, 89, 42, LT_MENU_NEW_PrimaryWeaponKeys13 /*"pulsar:"*/, (void *)&Config.select_primary[ PULSAR ] ),
		VDUKEYDEF( 3, 42, 89, 50, LT_MENU_NEW_PrimaryWeaponKeys14 /*"trojax:"*/, (void *)&Config.select_primary[ TROJAX ] ),
		VDUKEYDEF( 3, 50, 89, 58, LT_MENU_NEW_PrimaryWeaponKeys15 /*"suss gun:"*/, (void *)&Config.select_primary[ SUSS_GUN ] ),
		VDUKEYDEF( 3, 58, 89, 66, LT_MENU_NEW_PrimaryWeaponKeys16 /*"pyrolite:"*/, (void *)&Config.select_primary[ TRANSPULSE_CANNON ] ),
		VDUKEYDEF( 3, 66, 89, 74, LT_MENU_NEW_PrimaryWeaponKeys17 /*"transpulse:"*/, (void *)&Config.select_primary[ PYROLITE_RIFLE ] ),
		VDUKEYDEF( 3, 74, 89, 82, LT_MENU_NEW_PrimaryWeaponKeys18 /*"laser:"*/, (void *)&Config.select_primary[ LASER ] ),

		VDUKEYDEF( 3, 88, 89, 96, LT_MENU_NEW_PrimaryWeaponKeys19 /*"Next weapon:"*/, (void *)&Config.select_next_primary ),
		VDUKEYDEF( 3, 96, 89, 104, LT_MENU_NEW_PrimaryWeaponKeys20 /*"Prev weapon:"*/, (void *)&Config.select_prev_primary ),

		VDUKEYDEF( 3, 110, 89, 118, LT_MENU_NEW_PrimaryWeaponKeys21 /*"drop weapon:"*/, (void *)&Config.drop_primary ),
#endif		
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Weapons = {
	"", InitControls, ExitControls, NULL, TITLE_TIMER_NormalPanToRightVDU,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_Weapons0 /*"weapons"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 10, 45, 200, 65, 0, LT_MENU_NEW_Weapons1 /*"primary weapon keys"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, &MENU_NEW_PrimaryWeaponKeys, MenuChange, DrawFlatMenuItem, NULL, 0  },
		{ 10, 70, 200, 90, 0, LT_MENU_NEW_Weapons2 /*"secondary weapon keys"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, &MENU_NEW_SecondaryWeaponKeys, MenuChange, DrawFlatMenuItem, NULL, 0  },
		{ 10, 100, 200, 120, 0, LT_MENU_NEW_Weapons3 /*"primary weapon preferences"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, &MENU_NEW_PrimaryOrder, MenuChange, DrawFlatMenuItem, NULL, 0  },
		{ 10, 125, 200, 145, 0, LT_MENU_NEW_Weapons4 /*"secondary weapon preferences"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, &MENU_NEW_SecondaryOrder, MenuChange, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_ConfigureKeyboard = {
#ifdef VDUKDEF
	"", InitKeyDef, NULL, NULL, 0,
#else
	"", InitControls, ExitControls, NULL, 0,
#endif
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_ConfigureKeyboard0 /*"movement controls"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

#ifdef VDUKDEF
		VDUKDEF( 3, 15, 80, 23, LT_MENU_NEW_ConfigureKeyboard1 /*"rotate left"*/, KDleft ),
		VDUKDEF( 3, 23, 80, 31, LT_MENU_NEW_ConfigureKeyboard2 /*"rotate right"*/, KDright ),
		VDUKDEF( 3, 31, 80, 39, LT_MENU_NEW_ConfigureKeyboard3  /*"rotate up"*/, KDup ),
		VDUKDEF( 3, 39, 80, 47, LT_MENU_NEW_ConfigureKeyboard4 /*"rotate down"*/, KDdown ),

		VDUKDEF( 3, 50, 80, 58, LT_MENU_NEW_ConfigureKeyboard5 /*"slide left"*/, KDmove_left ),
		VDUKDEF( 3, 58, 80, 66, LT_MENU_NEW_ConfigureKeyboard6 /*"slide right"*/, KDmove_right ),
		VDUKDEF( 3, 66, 80, 74, LT_MENU_NEW_ConfigureKeyboard7 /*"slide up"*/, KDmove_up ),
		VDUKDEF( 3, 74, 80, 82, LT_MENU_NEW_ConfigureKeyboard8 /*"slide down"*/, KDmove_down ),

		VDUKDEF( 3, 85, 80, 93, LT_MENU_NEW_ConfigureKeyboard9 /*"roll left"*/, KDroll_left ),
		VDUKDEF( 3, 93, 80, 101, LT_MENU_NEW_ConfigureKeyboard10 /*"roll right"*/, KDroll_right ),

		VDUKDEF( 3, 104, 80, 112, LT_MENU_NEW_ConfigureKeyboard11 /*"slide mode"*/, KDmove ),
		VDUKDEF( 3, 112, 80, 120, LT_MENU_NEW_ConfigureKeyboard12 /*"roll mode"*/, KDroll ),

		VDUKDEF( 3, 123, 80, 131, LT_MENU_NEW_ConfigureKeyboard13 /*"forward"*/, KDmove_forward ),
		VDUKDEF( 3, 131, 80, 139, LT_MENU_NEW_ConfigureKeyboard14 /*"reverse"*/, KDmove_backward ),
		VDUKDEF( 3, 139, 80, 147, LT_MENU_NEW_ConfigureKeyboard15 /*"nitro"*/, KDturbo ),

		VDUKDEF( 3, 150, 80, 158, LT_MENU_NEW_ConfigureKeyboard16 /*"cruise faster"*/, KDcruise_faster ),
		VDUKDEF( 3, 158, 80, 166, LT_MENU_NEW_ConfigureKeyboard17 /*"cruise slower"*/, KDcruise_slower ),
#else
		VDUKEYDEF( 3, 15, 80, 23, LT_MENU_NEW_ConfigureKeyboard18 /*"rotate left:"*/, (void *)&Config.left ),
		VDUKEYDEF( 3, 23, 80, 31, LT_MENU_NEW_ConfigureKeyboard19 /*"rotate right:"*/, (void *)&Config.right ),
		VDUKEYDEF( 3, 31, 80, 39, LT_MENU_NEW_ConfigureKeyboard20 /*"rotate up:"*/, (void *)&Config.up ),
		VDUKEYDEF( 3, 39, 80, 47, LT_MENU_NEW_ConfigureKeyboard21 /*"rotate down:"*/, (void *)&Config.down ),

		VDUKEYDEF( 3, 50, 80, 58, LT_MENU_NEW_ConfigureKeyboard22 /*"slide left:"*/, (void *)&Config.move_left ),
		VDUKEYDEF( 3, 58, 80, 66, LT_MENU_NEW_ConfigureKeyboard23 /*"slide right:"*/, (void *)&Config.move_right ),
		VDUKEYDEF( 3, 66, 80, 74, LT_MENU_NEW_ConfigureKeyboard24 /*"slide up"*/, (void *)&Config.move_up ),
		VDUKEYDEF( 3, 74, 80, 82, LT_MENU_NEW_ConfigureKeyboard25 /*"slide down:"*/, (void *)&Config.move_down ),

		VDUKEYDEF( 3, 85, 80, 93, LT_MENU_NEW_ConfigureKeyboard26 /*"roll left:"*/, (void *)&Config.roll_left ),
		VDUKEYDEF( 3, 93, 80, 101, LT_MENU_NEW_ConfigureKeyboard27 /*"roll right:"*/, (void *)&Config.roll_right ),

		VDUKEYDEF( 3, 104, 80, 112, LT_MENU_NEW_ConfigureKeyboard28 /*"slide mode:"*/, (void *)&Config.move ),
		VDUKEYDEF( 3, 112, 80, 120, LT_MENU_NEW_ConfigureKeyboard29 /*"roll mode:"*/, (void *)&Config.roll ),

		VDUKEYDEF( 3, 123, 80, 131, LT_MENU_NEW_ConfigureKeyboard30 /*"forward:"*/, (void *)&Config.move_forward ),
		VDUKEYDEF( 3, 131, 80, 139, LT_MENU_NEW_ConfigureKeyboard31 /*"reverse:"*/, (void *)&Config.move_backward ),
		VDUKEYDEF( 3, 139, 80, 147, LT_MENU_NEW_ConfigureKeyboard32 /*"nitro:"*/, (void *)&Config.turbo ),

		VDUKEYDEF( 3, 150, 80, 158, LT_MENU_NEW_ConfigureKeyboard33 /*"cruise faster:"*/, (void *)&Config.cruise_faster ),
		VDUKEYDEF( 3, 158, 80, 166, LT_MENU_NEW_ConfigureKeyboard34 /*"cruise slower:"*/, (void *)&Config.cruise_slower ),

		VDUKEYDEF( 3, 170, 80, 178, LT_MENU_NEW_ConfigureKeyboard35 /*"send msg:"*/, (void *)&Config.send_msg ),
#endif
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU MENU_NEW_ConfigJoyAxis = {
	"", InitJoystickAxisConfig, SetAxis, CheckJoyAxis, 0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_ConfigJoyAxis0 /*"setup axis"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 0, 0, 20 , 20, 0, "", FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &JoystickAxisList, NULL, NULL, DrawLeftArrow, NULL, 0 } ,
		{ 180, 0, 200 , 20, 0, "", FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &JoystickAxisList, NULL, NULL, DrawRightArrow, NULL, 0 } ,

		{ 5, 25, 85, 45, SLIDER_User, LT_MENU_NEW_ConfigJoyAxis1 /*"select axis:"*/, FONT_Medium, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,
						&AxisSlider, NULL, SelectSlider, DrawFlatMenuSlider, NULL, 0  },

		{ 5, 50, 85, 65, SLIDER_User, LT_MENU_NEW_ConfigJoyAxis2 /*"select action:"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY | TEXTFLAG_ForceFit,
						&AxisActionSlider, NULL, SelectSlider, DrawFlatMenuSlider, NULL, 0  },

		{ 5, 70, 85, 80, 0, LT_MENU_NEW_ConfigJoyAxis3 /*"invert axis"*/, FONT_Small, TEXTFLAG_CentreY, &AxisInvert, SetAxisInv, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0  },

		{ 5, 85, 85, 95, 0, LT_MENU_NEW_ConfigJoyAxis4 /*"sensitivity"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &SensitivitySlider, SetAxisSensitivity, SelectSlider, DrawFlatMenuSlider, NULL, 0  },

		{ 5, 100, 85, 110, 0, LT_MENU_NEW_ConfigJoyAxis5 /*"deadzone"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &DeadzoneSlider, SetAxisDeadzone, SelectSlider, DrawFlatMenuSlider, NULL, 0  },
		
		{ 5, 115, 85, 125, 0, LT_MENU_NEW_ConfigJoyAxis6 /*"fine control"*/, FONT_Small, TEXTFLAG_CentreY, &FineControl, SetAxisFineControl, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0  },

		{ 0, 130, 200, 140, 0, LT_MENU_NEW_ConfigJoyAxis7 /*"test:"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 5, 140, 100, 150, 0, "", FONT_Small, TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh,  (void *)AxisExtremeLeftText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 100, 140, 195, 150, 0, "", FONT_Small, TEXTFLAG_RightX | TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh,  (void *)AxisExtremeRightText, NULL, NULL, DrawFlatMenuName, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};


/* joystick */
MENU	MENU_NEW_SetupJoystick = {
	"", InitJoystickList, ExitJoySetup, NULL, 0,
	{

		/* title */
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_SetupJoystick0 /*"choose joystick"*/,
		FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL,
		DrawFlatMenuItem, NULL, 0  },

		{ 10, 30, 200, 100, 0,
		LT_MENU_NEW_SetupJoystick1 /*"joystick list"*/, FONT_Small,
		TEXTFLAG_SuppressHighlight | TEXTFLAG_CentreY | TEXTFLAG_ForceFit | 
		TEXTFLAG_AutoSelect,
        &JoystickList, SelectConfigureAxisMenu, SelectList, DrawFlatMenuList, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}

};

MENUITEM AxisScreenItem = { 100, 110, 180, 130, 0, LT_AxisScreenItem/*"configure Axis"*/, FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_ConfigJoyAxis, MenuChange, DrawFlatMenuItem, NULL, 0  };

MENU	MENU_NEW_SetupMouse = {
	"", NULL, ExitMouseSetup, NULL, 0,
	{
		{ 10, 85, 100, 95, SLIDER_Sensitivity, LT_MENU_NEW_SetupMouse0 /*"X sensitivity"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,  &SensitivityXSlider, NULL, SelectSlider,	DrawFlatMenuSlider, NULL, 0  },
		{ 10, 95, 100, 105, SLIDER_Sensitivity, LT_MENU_NEW_SetupMouse1 /*"Y sensitivity"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,  &SensitivityYSlider, NULL, SelectSlider,	DrawFlatMenuSlider, NULL, 0  },
		{ 10, 115, 100, 125, 0, LT_MENU_NEW_SetupMouse2 /*"invert x"*/, FONT_Small, TEXTFLAG_CentreY,  &Config.invert_turn, NULL, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0  },
		{ 10, 125, 100, 135, 0, LT_MENU_NEW_SetupMouse3 /*"invert y"*/, FONT_Small, TEXTFLAG_CentreY,  &Config.invert_pitch, NULL, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}

};

/* controls */
MENU	MENU_NEW_Controls = {
	"", InitControls, ExitControls, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		

		{ 0, 0, 200, 20, 0, LT_MENU_NEW_Controls0 /*"controls"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 10, 30, 200, 40, 0, LT_MENU_NEW_Controls1 /*"configure mouse"*/, FONT_Small, TEXTFLAG_CentreY, NULL, &MENU_NEW_SetupMouse, MenuChange,	DrawFlatMenuItem, NULL, 0  },

		{ 10, 45, 200, 55, 0, LT_MENU_NEW_Controls2 /*"configure joystick"*/, FONT_Small, TEXTFLAG_CentreY, NULL,
		  &MENU_NEW_SetupJoystick, MenuChange, DrawFlatMenuItem, NULL, 0  },



		{ 10, 60, 200, 70, 0, LT_MENU_NEW_Controls3 /*"configure keys and buttons"*/, FONT_Small, TEXTFLAG_CentreY, NULL, &MENU_NEW_ConfigureKeyboard, MenuChange, DrawFlatMenuItem, NULL, 0  },
		{ 10, 75, 100, 85, 0, LT_MENU_NEW_Controls4 /*"autoleveling"*/, FONT_Small, TEXTFLAG_CentreY,  &Autoleveling, SetAutolevel, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0  },
		{ 10, 90, 200, 100, 0, LT_MENU_NEW_Controls5 /*"restore defaults"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, NULL, RestoreDefaultControlSettings, DrawFlatMenuItem, NULL, 0  },
		{ 10, 105, 200, 115, 0, LT_MENU_NEW_Controls6 /*"back"*/, FONT_Small, TEXTFLAG_CentreY,  NULL, NULL, MenuItemBack, DrawFlatMenuItem, NULL, 0  },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

// Out-of-game menu
extern bool ShowInfo;
MENU	MENU_NEW_DetailLevels = {
	"", NULL, ExitDetailLevels, NULL, 0,
	{
		{ 0, 10, 200, 10, 0, LT_MENU_NEW_DetailLevels0/*"DETAIL LEVELS"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 10, 40, 150, 40, 0, "MATURE CONTENT", FONT_Small, TEXTFLAG_CentreY,  &MatureContent,	NULL,	SelectMultiToggle,	DrawFlatMenuToggle, NULL, 0 },
		{ 10, 56, 150, 56, 0, LT_MENU_NEW_DetailLevels4/*"Team Info"*/, FONT_Small, TEXTFLAG_CentreY,		&ShowTeamInfo,			NULL,	SelectFlatMenuToggle,	DrawFlatMenuToggle, NULL, 0 },
		{ 10, 72, 95, 72, 0, LT_MENU_NEW_DetailLevels8/*"Water Detail"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,		&WaterDetailSlider,		NULL,	SelectSlider,	DrawFlatMenuSlider, NULL, 0 },
		{ 10, 86, 150, 86, 0, LT_MENU_NEW_DetailLevels14/*"Show Extra Info "*/, FONT_Small, TEXTFLAG_CentreY, &ShowInfo, NULL, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0 },
		{ 10, 102, 150, 102, 0, LT_MENU_NEW_DetailLevels15/*"Show Weapon Kills"*/, FONT_Small, TEXTFLAG_CentreY, &ShowWeaponKills, NULL, SelectFlatMenuToggle, DrawFlatMenuToggle, NULL, 0 },
		{ 10, 150, 150, 150, 0, LT_MENU_NEW_DetailLevels16/*"Back"*/, FONT_Small, TEXTFLAG_CentreY,		NULL,		NULL,	MenuItemBack, DrawFlatMenuItem, NULL, 0 },
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	 
	}
};


MENU	MENU_NEW_ScreenRes = {
	"Select Screen Mode" , MakeModeList , NULL , NULL, 0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_ScreenRes0 /*"Choose From..."*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 15, 30, 200, 150, 0, "", FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &ModeList, NewMenuSelectMode , SelectList , DrawFlatMenuList, NULL, 0 } ,
		{ 0, 155, 200, 165, 0, LT_MENU_NEW_ScreenRes1 /*"press escape to go back"*/, FONT_Small, TEXTFLAG_CentreY | TEXTFLAG_CentreX, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

char *StereoColorSliderFunc( SLIDER *slider )
{
	switch( slider->value )
	{
	case ST_GREEN:
		return "green";
	case ST_BLUE:
		return "blue";
	case ST_CYAN:
		return "cyan";
	default:
		DebugPrintf( "Unknown stereo slider value: %d\n", slider->value );
		return NULL;
	}
}

bool SetStereoColor( SLIDER *slider )
{
	DebugPrintf( "Setting stereo color to %d\n", slider->value );
	switch( slider->value )
	{
	case ST_GREEN:
	case ST_BLUE:
	case ST_CYAN:
		render_info.stereo_right_color = (stereo_right_color_t) slider->value;
		return true;
	default:
		DebugPrintf( "Unknown stereo slider value: %d\n", slider->value );
		return false;
	}
}

char *StereoModeSliderFunc( SLIDER *slider )
{
	switch( slider->value )
	{
	case STEREO_MODE_COLOR:
		return "RED/BLUE"; 
	case STEREO_MODE_HALF_HEIGHT:
		return "HALF_HEIGHT"; 
	case STEREO_MODE_HALF_WIDTH:
		return "HALF_WIDTH";
	default:
		DebugPrintf( "Unknown stereo slider value: %d\n", slider->value );
		return NULL;
	}
}

bool SetStereoMode( SLIDER *slider )
{
	DebugPrintf( "Setting stereo color to %d\n", slider->value );
	switch( slider->value )
	{
	case STEREO_MODE_COLOR:
	case STEREO_MODE_HALF_HEIGHT:
	case STEREO_MODE_HALF_WIDTH:
		render_info.stereo_mode = (stereo_right_color_t) slider->value;
		return true;
	default:
		DebugPrintf( "Unknown stereo slider value: %d\n", slider->value );
		return false;
	}
}

SLIDER StereoColorSlider = {0, 2, 1, 2, 2, 0.0F, 0.0F, 0, true, StereoColorSliderFunc, SetStereoColor, NULL };
SLIDER StereoModeSlider = {0, 2, 1, 2, 2, 0.0F, 0.0F, 0, true, StereoModeSliderFunc, SetStereoMode, NULL };

MENU	MENU_NEW_VisualsStereo = {
	"Stereo", NULL, NULL, NULL, 0,
	{
		{  0,   0, 200,  20, 0,					"stereo",											FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,			NULL,						NULL,					DrawFlatMenuItem,	NULL, 0 },
		{ 20,  40, 150,  50, 0,					"enable stereo",								FONT_Small, TEXTFLAG_CentreY,				&render_info.stereo_enabled,			NULL,						SelectFlatMenuToggle,	DrawFlatMenuToggle,	NULL, 0 },		 
		{ 20,  50, 150,  70, SLIDER_User,		"stereo mode",								FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,				&StereoModeSlider,			NULL,						SelectSlider,	DrawFlatMenuSlider,	NULL, 0 },		 
		{ 20,  70, 150,  70, SLIDER_User,		"right eye color",									FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&StereoColorSlider,	NULL,					SelectSlider,			DrawFlatMenuSlider,			NULL, 0 },		 
		{ 20, 140, 100, 150, 0,					"back",												FONT_Small, TEXTFLAG_CentreY,						NULL,			NULL,						MenuItemBack,			DrawFlatMenuItem,	NULL, 0 },		 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Visuals = {
	"", NULL, NULL, NULL, TITLE_TIMER_Visuals,
	{
		{  0,   0, 200,  20, 0,					LT_MENU_NEW_Visuals0 /*"Visuals"*/,					FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,			NULL,						NULL,					DrawFlatMenuItem,	NULL, 0 },
		{ 20,  40, 200,  50, 0,					LT_MENU_NEW_Visuals1 /*"Change Detail Levels"*/,	FONT_Small, TEXTFLAG_CentreY,						NULL,			&MENU_NEW_DetailLevels,		MenuChange,				DrawFlatMenuItem,	NULL, 0 },
		{ 20,  60, 200,  70, 0,					LT_MENU_NEW_Visuals2 /*"Change Screen Res"*/,		FONT_Small, TEXTFLAG_CentreY,						NULL,			&MENU_NEW_ScreenRes,		MenuChange,				DrawFlatMenuItem,	NULL, 0 },
		{ 20, 80,  50, 90, SLIDER_Percent,	LT_MENU_NEW_Visuals4 /*"gamma"*/,					FONT_Small,	TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &GammaSlider,	NULL,						SelectSlider,			DrawFlatMenuSlider, NULL, 0 },
		
		{ 20, 100, 200, 100, 0,					LT_MENU_InGame2  /*"Toggle Full Screen" */,			FONT_Small, TEXTFLAG_CentreY,						NULL,			NULL,						MenuGoFullScreen,		DrawFlatMenuItem,	NULL, 0 },
		{ 20, 112, 150, 112, 0,					"Vertical Sync (requires restart)",			FONT_Small, TEXTFLAG_CentreY,	&render_info.vsync,	NewMenuSelectMode,	SelectFlatMenuToggle, DrawFlatMenuToggle,		NULL, 0 },
		{ 20, 124, 200, 124, 0,					"anaglyph stereo 3d options",						FONT_Small, TEXTFLAG_CentreY,						NULL,			&MENU_NEW_VisualsStereo,	MenuChange,				DrawFlatMenuItem,	NULL, 0 },		 
		{ 20, 136, 150, 136, 0,					"Tint Bike to Team Color",						FONT_Small, TEXTFLAG_CentreY,		&TintBikeTeamColor,		NULL,	SelectFlatMenuToggle,	DrawFlatMenuToggle,	NULL, 0 },		 
		{ 20, 160, 100, 160, 0,					LT_MENU_NEW_Visuals5 /*"back"*/,					FONT_Small, TEXTFLAG_CentreY,						NULL,			NULL,						MenuItemBack,			DrawFlatMenuItem,	NULL, 0 },		 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_JoinWaitingToStart = {
	"" , InitHostWaitingToStart, BailMultiplayerFrontEnd , UpdateSessions, TITLE_TIMER_PanToLeftVDU,
	{

		{ 0, 0, 200, 10, 0, LT_MENU_NEW_JoinWaitingToStart0 /*"waiting for host..."*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 5, 27, 100, 37, SLIDER_Value, LT_MENU_NEW_JoinWaitingToStart1 /*"Num of Players"*/, FONT_Small, TEXTFLAG_CentreY, &NumOfPlayersSlider, NULL, NULL, DrawFlatMenuSlider, NULL, 0 },
		{ 5, 40, 100, 125, 0, "", FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &PlayersList, NULL , NULL , DrawFlatMenuList, NULL, 0 } ,

		{ 5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 155, 25, 162, 0, LT_MENU_NEW_JoinWaitingToStart2 /*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL, NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_GetTeamPlayers = {
	"" , GetInitialPlayers, BailMultiplayer , CheckForMenuChange, 0,
	{

		{ 0, 0, 200, 10, 0, LT_MENU_NEW_GetTeamPlayers0 /*"waiting for host..."*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
		{ 5, 27, 100, 37, SLIDER_Value, LT_MENU_NEW_GetTeamPlayers1 /*"Num of Players"*/, FONT_Small, TEXTFLAG_CentreY, &NumOfPlayersSlider, NULL, NULL, DrawFlatMenuSlider, NULL, 0 },
		{ 5, 40, 100, 165, 0, "", FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &PlayersList, NULL , NULL , DrawFlatMenuList, NULL, 0 } ,

		{ 5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 155, 25, 162, 0, LT_MENU_NEW_GetTeamPlayers2 /*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL, NULL , NULL, NULL, 0 }
	}
};

void JoiningExit(MENU *Menu)
{
	network_cleanup();
}

void JoiningEnter(MENU *Menu)
{
	network_return_t rv;

	DebugPrintf("JoiningEnter\n");

	SetGamePrefs(); // save the last TCPAddress set

	// initialize levels
	if ( !InitLevels( MULTIPLAYER_LEVELS ) && !InitLevels( DEFAULT_LEVELS ) )
	{
		Msg( "No multiplayer levels" );
		PrintErrorMessage (LT_NoLevelsInstalled, 1, NULL, ERROR_USE_MENUFUNCS );
		return;
	}
	
	PlayDemo = false;
	
	// this means I need a ship number
	WhoIAm = UNASSIGNED_SHIP;

	// cleanup everything
	SetupNetworkGame();

	// set bike mode to normal
	SetBikeMods( 0 );

	// we are a joiner
	IsHost = false;

	// reset player list
	PlayersList.items = 0;
	PlayersList.top_item = 0;
	PlayersList.display_items = 16;
	PlayersList.selected_item = -1;

	// convert ports
	local_port = atoi(local_port_str.text);
	host_port = atoi(host_port_str.text);

	// setup
	rv = network_setup( &biker_name[0], local_port );

	if( rv != NETWORK_OK )
	{
		char error_str[600] = "Failed to setup network!";
		switch( rv )
		{
		case NETWORK_ERROR_INIT:
			break;
		case NETWORK_ERROR_BIND:
			{
				char * str = "";
				if( local_port < 1024 )
					str = "Ports bellow 1024 are normally restricted.";
				sprintf(error_str, "%s %s %s %s",
						"The selected port is in use or invalid.", 
						str,
						"Disable the program using the port or",
						"select a different local port." );
			}
			break;
		}
		PrintErrorMessage(error_str, 2, &MENU_NEW_ChooseConnectionToJoin, 0);
		return;
	}

	// convert host
	strncpy( host_address, TCPAddress.text, sizeof(host_address) );

	network_join( host_address, host_port );
}

char join_status_msg[255] = "";
void update_join_status( char* str )
{
	strncpy( join_status_msg, str, 254 );
	join_status_msg[254] = 0;
}

char show_f1[100] = "( press F1 to retry )";
void CheckJoinStatus( int * i )
{
	// disconnect and reconnect if f1 is pressed
	if ( input_buffer_find( SDLK_F1 ) )
		network_join( host_address, host_port );

	// process network routines
	ReceiveGameMessages();

	switch( network_state )
	{
	// at this point network_event will take over and change the screen
	case NETWORK_CONNECTED:
		update_join_status("Connected...");
		strncpy( &show_f1[0], "", 100 );
		break;
	case NETWORK_SYNCHING:
		update_join_status("Synching with other players please wait...");
		strncpy( &show_f1[0], "", 100 );
		break;
	case NETWORK_CONNECTING:
		update_join_status("Connecting to host please wait...");
		strncpy( &show_f1[0], "", 100 );
		break;
	case NETWORK_DISCONNECTED:
		update_join_status("Failed to connect to host...");
		strncpy( &show_f1[0], "( press F1 to retry )", 100 );
		break;
	case NETWORK_SYNCH_FAILED:
		update_join_status("Failed to synch with other players...");
		strncpy( &show_f1[0], "( press F1 to retry )", 100 );
		break;
	}
}


MENU	MENU_NEW_Joining = {

		"" , JoiningEnter, JoiningExit, CheckJoinStatus, TITLE_TIMER_PanToLeftVDU,
	{
		{ 0, 0, 200, 20, 0, "", FONT_Medium, TEXTFLAG_CheckForRefresh | TEXTFLAG_CentreX | TEXTFLAG_CentreY, join_status_msg, NULL , NULL , DrawFlatMenuName, NULL, 0 } ,

		// join in watch mode (add 'if host allows' later)
//		{ 10, 30, 200, 93, 0, NULL, FONT_Small, TEXTFLAG_CentreY, "YOOYYO TEST", NULL , SelectList , DrawFlatMenuName, NULL, 0 } ,

		{ 0, 15, 200, 30, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_CentreX | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, &show_f1, NULL, NULL, DrawFlatMenuName, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENUITEM MENU_ITEM_JoinMultiplayer = 
		{ 0, 0, 0, 0, 0, "", 0, 0,  NULL, &MENU_NEW_Joining, MenuChange, NULL, NULL, 0  };

#define MAX_GAME_LIST 7
#define MAX_GAME_LENGTH 100

char GameName[MAX_GAME_LIST][MAX_GAME_LENGTH];
char GameList[MAX_GAME_LIST][MAX_GAME_LENGTH];

void UpdateGameList( void )
{	
	int i;
	int length;

	games_update();

	length = games_length();
	snprintf(GamesLength.text, sizeof(GamesLength.text), "%d", length);

	for( i = 0; i < MAX_GAME_LIST; i++ )
	{
		// the game list starts at 1
		int x = i+1;

		// valid game
		if( i < length )
		{
			// set the game line
			snprintf(GameList[i], sizeof(GameList[i]),
						"%d. %s %s:%s", x, games_name_at(x), games_ip_at(x), games_port_at(x));

			// set the game name so we can find the index later
			strncpy(GameName[i], games_name_at(i+1), sizeof(GameName[i]));
		}
		// empty out the game
		else
		{
			snprintf(GameList[i], sizeof(GameList[i]), "%d.", x);
			memset(GameName[i],0,sizeof(GameName[i]));
		}
	}
}

px_timer_t game_list_timer;

void Enter_ChooseConnectionToJoin( MENU *Menu )
{
	timer_clear( &game_list_timer );
	UpdateGameList();
}

void Update_ChooseConnectionToJoin( int * z )
{
	// check time passed
	if ( timer_peek( &game_list_timer ) < 2 ) return;
	timer_run( &game_list_timer );
	UpdateGameList();
}

MENU	MENU_NEW_ChooseConnectionToJoin = {
	"", Enter_ChooseConnectionToJoin, NULL, Update_ChooseConnectionToJoin, TITLE_TIMER_PanToLeftVDU,
	{
		{ 0, 10, 200, 10, 0, "Join Multiplayer Game",	FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,				NULL,	NULL,						DrawFlatMenuItem,	NULL, 0  },

		{ 5, 25, 60, 25, 0, "connect",					FONT_Medium,	TEXTFLAG_CentreY,						NULL,				NULL,	SelectConnectionToJoin,		DrawFlatMenuItem,	NULL, 0  },

		{ 5, 40, 60, 40, 0, "host address:",			FONT_Small,		TEXTFLAG_ForceFit | TEXTFLAG_CentreY,	&TCPAddress,		NULL,	SelectFlatMenutext,			DrawFlatMenuText,	NULL, 0 } ,
		{ 5, 48, 60, 48, 0,	"host port:",				FONT_Small,		TEXTFLAG_ForceFit | TEXTFLAG_CentreY,	&host_port_str,		NULL,	SelectFlatMenutext,			DrawFlatMenuText,	NULL, 0 } ,

		{ 5, 60, 60, 60, 0,	"local port:",				FONT_Small,		TEXTFLAG_ForceFit | TEXTFLAG_CentreY,	&local_port_str,	NULL,	SelectFlatMenutext,			DrawFlatMenuText,	NULL, 0 } ,

		//

		{ 0, 75, 200, 75, 0, "Public Game List",		FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,				NULL,	NULL,						DrawFlatMenuItem,	NULL, 0 },
		{ 0, 83, 200, 83, 0, "Select a game bellow to join", FONT_Small,TEXTFLAG_CentreX | TEXTFLAG_CentreY,	NULL,				NULL,	NULL,						DrawFlatMenuItem,	NULL, 0 } ,

		// number of games
		{ 5, 97, 60, 97, 0, "Games Found:",			FONT_Small,		TEXTFLAG_CheckForRefresh | TEXTFLAG_CentreY,	&GamesLength,	NULL,	NULL,						DrawFlatMenuText,	NULL, 0 } ,

		// list of games
		{ 5, 111, 200, 111, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[0], (void *)GameName[0], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 119, 200, 119, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[1], (void *)GameName[1], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 127, 200, 127, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[2], (void *)GameName[2], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 135, 200, 135, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[3], (void *)GameName[3], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 143, 200, 143, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[4], (void *)GameName[4], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 151, 200, 151, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[5], (void *)GameName[5], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,
		{ 5, 159, 200, 159, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)GameList[6], (void *)GameName[6], SelectConnectionToJoin, DrawFlatMenuName, NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};


MENU	MENU_NEW_ChangeBike = {
	"", InitBikeListVDU, KillBikeChar, NULL, 0,
	{
		{ 0, 0, 200 , 20, 0, "", FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &bike_name, NULL, NULL, DrawFlatMenuName, NULL, 0 } ,
		{ 15, 20, 135 , 128, 0, "", FONT_Small, TEXTFLAG_ForceFit , &biker_text, NULL, NULL, DrawFlatMenuName, NULL, 0 } ,

		{ 27, 130, 72 , 137, SLIDER_Bike, LT_MENU_NEW_ChangeBike0 /*"speed"*/, FONT_Small, TEXTFLAG_CentreY , &SpeedFeat, NULL, NULL, DrawFlatMenuSlider, NULL, 0 } ,
		{ 27, 137, 72 , 144, SLIDER_Bike, LT_MENU_NEW_ChangeBike1 /*"accel"*/, FONT_Small, TEXTFLAG_CentreY , &AccelFeat, NULL, NULL, DrawFlatMenuSlider, NULL, 0 } ,
		{ 65, 144, 110 , 151, SLIDER_Bike, LT_MENU_NEW_ChangeBike2 /*"handling"*/, FONT_Small, TEXTFLAG_CentreY , &HandleFeat, NULL, NULL, DrawFlatMenuSlider, NULL, 0 } ,
		
		{ 114, 130, 157 , 137, SLIDER_Bike, LT_MENU_NEW_ChangeBike3 /*"shield"*/, FONT_Small, TEXTFLAG_CentreY , &ShieldFeat, NULL, NULL, DrawFlatMenuSlider, NULL, 0 } ,
		{ 114, 137, 157 , 144, SLIDER_Bike, LT_MENU_NEW_ChangeBike4 /*"hull"*/, FONT_Small, TEXTFLAG_CentreY , &HullFeat, NULL, NULL, DrawFlatMenuSlider, NULL, 0 } ,

		{ 125 , 20, 200, 130, 0, "" , FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , NULL, NULL , NULL , LoadBikeChar, NULL, 0},

		{ -1, -1, 0, 0, 0,"", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }

	}
};

MENU	MENU_NEW_ChoosePlayer = {
	"", InitVDUPilotMenu, KillBikeChar, NULL, 0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_ChoosePlayer0 /*"Choose Player"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 125 , 130, 200, 150, 0, "" , FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh, &bike_name, NULL , NULL , DrawFlatMenuName, NULL, 0},
		{ 15 , 30, 100, 150, 0, LT_MENU_NEW_ChoosePlayer1 /*"list..."*/ , FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &PilotList, NULL , NULL , DrawFlatMenuList, NULL, 0},

		{ 125 , 20, 200, 130, 0, "" , FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , NULL, NULL , NULL , LoadBikeChar, NULL, 0},
			
		{ -1, -1, 0, 0, 0,"", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }

	}
};

MENU	MENU_NEW_NewPlayer = {
	"", InitPilotName, ExitBikerMenu, NULL, TITLE_TIMER_PanToRightVDU,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_NewPlayer0 /*"Enter Player Name"*/, FONT_Medium, TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 15 , 50, 15, 70, 0, "" , FONT_Medium, TEXTFLAG_SuppressHighlight | TEXTFLAG_AutoSelect | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &PilotName, NULL, SelectFlatMenutext, DrawFlatMenuText, NULL, 0},
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};										 

MENU MENU_NEW_ChangePlayerName = {
	"", InitPilotReName, MoveConfigFile, NULL, TITLE_TIMER_PanToRightVDU,
	{
		
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_ChangePlayerName0 /*"Enter new Name"*/, FONT_Medium, TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },

		{ 15 , 50, 15, 70, 0, "" , FONT_Medium, TEXTFLAG_SuppressHighlight | TEXTFLAG_AutoSelect | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &PilotReName, NULL, SelectFlatMenutext, DrawFlatMenuText, NULL, 0},
		
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_ChangeBikeComputer;

MENU	MENU_NEW_SelectBiker = {
	"", NewInitBikeMenu, ExitBikerMenu, NULL, TITLE_TIMER_PanToRightVDU,
	{
		{ 5 , 0, 195, 20, 0, "" , FONT_Large, TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY , (void *)biker_name, NULL , NULL , DrawFlatMenuName, NULL, 0},
		{ 15 , 25, 85, 45, 0, LT_MENU_NEW_SelectBiker0 /*"Choose Player"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, &MENU_NEW_ChoosePlayer , MenuChange , DrawFlatMenuItem, NULL, 0},
		{ 15 , 45, 85, 65, 0, LT_MENU_NEW_SelectBiker1 /*"New Player"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, &MENU_NEW_NewPlayer, MenuChange, DrawFlatMenuItem, NULL, 0},
		{ 15 , 65, 85, 85, 0, LT_MENU_NEW_SelectBiker2 /*"rename player"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, &MENU_NEW_ChangePlayerName , MenuChange , DrawFlatMenuItem, NULL, 0},
		{ 15 , 85, 85, 105, 0, LT_MENU_NEW_SelectBiker3 /*"Change Bike"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, &MENU_NEW_ChangeBike , MenuChange , DrawFlatMenuItem, NULL, 0},
		{ 15 , 105, 85, 125, 0, LT_MENU_NEW_SelectBiker4 /*"Change Bike computer"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, &MENU_NEW_ChangeBikeComputer , MenuChange , DrawFlatMenuItem, NULL, 0},
		{ 15 , 125, 85, 145, 0, LT_MENU_NEW_SelectBiker5 /*"Exit"*/ , FONT_Small, TEXTFLAG_CentreY , NULL, NULL , MenuItemBack , DrawFlatMenuItem, NULL, 0},
		{ 125 , 130, 200, 150, 0, "" , FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &bike_name, NULL , NULL , DrawFlatMenuName, NULL, 0},

		{ 125 , 20, 200, 130, 0, "" , FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , NULL, NULL , NULL , LoadBikeChar, NULL, 0},

		{ -1, -1, 0, 0, 0,  "",0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};										 

MENUITEM BikeCharItem = 
		{ 125 , 20, 200, 130, 0, "" , FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , NULL, NULL , NULL , LoadBikeChar, NULL, 0};

#ifdef DEMO_SUPPORT

MENU	MENU_NEW_DemoPlayBack = {
	"Demo Playback" , InitDemoList, NULL, NULL, 0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_DemoPlayBack0 /*"play back demo"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 0, 20, 200, 30, 0, LT_MENU_NEW_DemoPlayBack1 /*"Start"*/, FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, StartDemoPlayback , DrawFlatMenuItem, NULL, 0 } ,
		{ 10, 40, 100, 50, SLIDER_Demo, LT_MENU_NEW_DemoPlayBack2 /*"playback speed"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &DemoSpeed, NULL, SelectSlider, DrawFlatMenuSlider, NULL, 0 } ,
		{ 10, 60, 90, 60, SLIDER_Value, LT_MENU_NEW_DemoPlayBack3 /*"watch player"*/, FONT_Small, TEXTFLAG_AutoSelect | TEXTFLAG_CentreY, &DemoEyesSelect, NULL, SelectSlider, DrawFlatMenuSlider, NULL, 0 } ,

		{ 10, 75, 200, 75, 0, LT_MENU_NEW_DemoPlayBack4 /*"choose demo"*/, FONT_Small, TEXTFLAG_CentreY, &DemoList, NULL, SelectVDUList, DrawFlatMenuItem, NULL, 0 } ,
 		{ 20, 90, 200, 150, 0, "",  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, &DemoList, NULL, SelectList, DrawFlatMenuList, NULL, 0 } ,
		
		{ 5, 160, 200, 160, 0, "", FONT_Small, TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh,  (void *)DemoAvgFpsText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		{ 5, 170, 200, 170, 0, "", FONT_Small, TEXTFLAG_CentreY | TEXTFLAG_CheckForRefresh,  (void *)DemoTotalFramesText, NULL, NULL, DrawFlatMenuName, NULL, 0  },
		
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

#endif

int16_t InGameLoadGameLevelNum;
extern	bool	CountDownOn;
bool InGameLoadASinglePlayerGame( MENUITEM * Item )
{
	int16_t OldNewLevelNum;
	OldNewLevelNum = NewLevelNum;
	WhoIAm = 0;								// I was the first to join...
	NewLevelNum = -1;
	PreInGameLoad( Item );

	if( NewLevelNum == -1 )
	{
		NewLevelNum = OldNewLevelNum;
		return false;
	}
	InGameLoadGameLevelNum = NewLevelNum;
	NewLevelNum = OldNewLevelNum;

	CountDownOn = false;
	MyGameStatus = STATUS_InGameLoadGameStartingSinglePlayer;
	return true;
}

MENU	MENU_LoadSavedGame = {
	LT_MENU_LoadSavedGame0/*"Load Saved Game"*/, InitLoadSavedGameList, NULL, NULL, 0,
	{
		{ 100 , 128, 0, 0, 0, LT_MENU_LoadSavedGame1/*"Choose From..."*/, 0, 0, &LoadSavedGameList, InGameLoadASinglePlayerGame, SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Sound = {
	LT_MENU_NEW_Sound0 /*"sound"*/ , NULL, ExitSoundMenu, SetSoundLevels, TITLE_TIMER_NormalPanToRightVDU,
	{
		{ 0,	0,		200,	20,	0, LT_MENU_NEW_Sound1 /*"sound options"		*/, FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,		NULL,								NULL, NULL,							DrawFlatMenuItem,		NULL, 0 } ,
		{ 10,	40,	100,	50,	0, LT_MENU_NEW_Sound2 /*"sfx volume"			*/, FONT_Small,		TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&SfxSlider,						NULL, SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{ 10,	60,	100,	70,	0, LT_MENU_NEW_Sound4 /*"bike engines"		*/, FONT_Small,		TEXTFLAG_CentreY,										&BikeEnginesOn,				NULL, SelectFlatMenuToggle,	DrawFlatMenuToggle,	NULL, 0 } ,
		{ 0,	80,	200,	100,	0, LT_MENU_NEW_Sound5 /*"speech options"	*/, FONT_Medium,	TEXTFLAG_CentreX | TEXTFLAG_CentreY,		NULL,								NULL, NULL,							DrawFlatMenuItem,		NULL, 0 } ,
		{ 10, 100,	100,	110,	0, LT_MENU_NEW_Sound6 /*"biker speech"		*/, FONT_Small,		TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&BikerSpeechSlider,			NULL, SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{ 10, 110,	100,	120,	0, LT_MENU_NEW_Sound7 /*"bike computer"		*/, FONT_Small,		TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&BikeCompSpeechSlider,	NULL, SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		{ 10,	120,	100,	130,	0, LT_MENU_NEW_Sound8 /*"flag sfx"				*/, FONT_Small,		TEXTFLAG_AutoSelect | TEXTFLAG_CentreY,	&FlagSfxSlider,					NULL, SelectSlider,				DrawFlatMenuSlider,		NULL, 0 } ,
		
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_InGameSound = {
	LT_MENU_NEW_InGameSound0 /*"sound"*/ , NULL, NULL, SetSoundLevels, 0,
	{
		{ 200, 128				 , 0, 0, 0, LT_MENU_NEW_InGameSound1 /*"sfx volume    "		*/, 0, 0, &SfxSlider,						NULL,						SelectSlider,	DrawSlider,		NULL, 0 },
		{ 200, 128 + (2*16)	 , 0, 0, 0, LT_MENU_NEW_InGameSound3 /*"bike engines  "		*/, 0, 0, &BikeEnginesOn,				ToggleBikeEngines,	SelectToggle,	DrawToggle,	NULL, 0 },
		{ 200, 128 + (3*16)  , 0, 0, 0, LT_MENU_NEW_InGameSound4 /*"biker volume  "		*/, 0, 0, &BikerSpeechSlider,			NULL,						SelectSlider,	DrawSlider,		NULL, 0 },
		{ 200, 128 + (4*16)  , 0, 0, 0, LT_MENU_NEW_InGameSound5 /*"bike computer "	*/, 0, 0, &BikeCompSpeechSlider,	NULL,						SelectSlider,	DrawSlider,		NULL, 0 },
		{ 200, 128 + (5*16)  , 0, 0, 0, LT_MENU_NEW_InGameSound6 /*"flag sfx "				*/, 0, 0, &FlagSfxSlider,					NULL,						SelectSlider,	DrawSlider,		NULL, 0 },
		
		{	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

#ifdef DEMO_SUPPORT

MENU	MENU_NEW_DemoCleaning = {
	"" , InitDemoList, NULL, NULL, 0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_DemoCleaning0 /*"clean a demo"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 0 , 20, 200, 30, 0, LT_MENU_NEW_DemoCleaning1 /*"Start"*/ , FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, &MENU_NotYet , StartDemoCleaning , DrawFlatMenuItem, NULL, 0 } ,
		
		{ 10, 45, 200, 55, 0, LT_MENU_NEW_DemoCleaning2 /*"choose demo"*/, FONT_Small, TEXTFLAG_CentreY, &DemoList, NULL, SelectVDUList, DrawFlatMenuItem, NULL, 0 } ,
		{ 20 , 60, 200, 170, 0, "", FONT_Small,TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &DemoList, NULL , SelectList , DrawFlatMenuList, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

#endif

#define MAX_SAVEDGAME_INFO_LENGTH 128

char CurrentSavedGameLevel[ MAX_SAVEDGAME_INFO_LENGTH ];
char CurrentSavedGameDate[ MAX_SAVEDGAME_INFO_LENGTH ];
char CurrentSavedGameTimeLevelPlayed[ MAX_SAVEDGAME_INFO_LENGTH ];
char CurrentSavedGameEnemiesKilled[ MAX_SAVEDGAME_INFO_LENGTH ];

MENU	MENU_NEW_LoadGame = {
	"" , InitTitleLoad , ExitTitleLoad, NULL, 0,
	{
		{ 0,  0, 200,  30, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, &CurrentSavedGameLevel,           NULL, NULL, DrawFlatMenuName, NULL, 0 },
		{ 0, 30, 200,  60, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, &CurrentSavedGameDate,            NULL, NULL, DrawFlatMenuName, NULL, 0 },
		{ 0, 60, 200,  90, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, &CurrentSavedGameEnemiesKilled,   NULL, NULL, DrawFlatMenuName, NULL, 0 },
		{ 0, 90, 200, 120, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, &CurrentSavedGameTimeLevelPlayed, NULL, NULL, DrawFlatMenuName, NULL, 0 },

		{ 0,   120,  20, 170, 0, "", FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &LoadSavedGameList, NULL, NULL, DrawLeftArrow,  NULL, 0 },
		{ 180, 120, 200, 170, 0, "", FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &LoadSavedGameList, NULL, NULL, DrawRightArrow, NULL, 0 },

		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};


MENU	MENU_NEW_RestoreGame = {
	"", NULL, NULL, NULL, TITLE_TIMER_PanToLeftVDU,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_RestoreGame0 /*"Restore Game"*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, NULL, NULL, DrawFlatMenuItem, NULL, 0  },
#ifdef DEMO_SUPPORT
		{ 5, 30, 100, 40, 0, LT_MENU_NEW_RestoreGame1 /*"Play Demo"*/, FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_DemoPlayBack, MenuChange, DrawFlatMenuItem, NULL, 0  },
		{ 5, 50, 100, 60, 0, LT_MENU_NEW_RestoreGame2 /*"Clean Demo"*/, FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_DemoCleaning, MenuChange, DrawFlatMenuItem, NULL, 0  },
#endif
		{ 5, 70, 100, 80, 0, LT_MENU_NEW_RestoreGame3 /*"load game"*/, FONT_Small, TEXTFLAG_CentreX | TEXTFLAG_CentreY,  NULL, &MENU_NEW_LoadGame, MenuChange, DrawFlatMenuItem, NULL, 0  },
						 
		{ -1, -1, 0, 0, 0, "", 0, 0,  NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Options = {
	"", SetDiscStatus, NULL, NULL, TITLE_TIMER_ChooseDiscPan, 
	{
		{ 0, TITLE_MODEL_Disc1, 0, 0, 5,LT_MENU_NEW_Options0 /*"Options"*/, 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc2, 0, 0, 8, LT_MENU_NEW_Options1 /*"Sound"*/, 0, 0, NULL, &MENU_NEW_Sound, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc3, 0, 0, 14, LT_MENU_NEW_Options2 /*"Visuals"*/, 0, 0, NULL, &MENU_NEW_Visuals, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc4, 0, 0, 2, LT_MENU_NEW_Options3 /*"back"*/, 0, 0, NULL, NULL, MenuItemBack, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc5, 0, 0, 2, "blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc6, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },

		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/* setup */
MENU	MENU_NEW_Setup = {
	"", NULL, NULL, NULL, TITLE_TIMER_ChooseDiscPan,
	{
		{ 0, TITLE_MODEL_Disc1, 0, 0, 4,LT_MENU_NEW_Setup0 /*"Setup Biker"*/, 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc2, 0, 0, 6, LT_MENU_NEW_Setup1 /*"Select Bike"*/, 0, 0, NULL, &MENU_NEW_SelectBiker, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc3, 0, 0, 12, LT_MENU_NEW_Setup2 /*"Controls"*/, 0, 0, NULL, &MENU_NEW_Controls, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc4, 0, 0, 4, LT_MENU_NEW_Setup3 /*"Weapons"*/, 0, 0, NULL, &MENU_NEW_Weapons, MenuChange , NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc5, 0, 0, 3, "back"/*"back"*/, 0, 0, NULL, NULL, MenuItemBack, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc6, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },

		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};  

/* battle */
MENU	MENU_NEW_Battle = {
	"", InitBattleMenu, NULL, NULL, TITLE_TIMER_ChooseDiscPan, 
	{
		{ 0, TITLE_MODEL_Disc1, 0, 0, 3, LT_MENU_NEW_Battle0 /*"death match"*/, 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc2, 0, 0, 2, LT_MENU_NEW_Battle1 /*"Create Game"*/, 0, 0, NULL, &MENU_NEW_CreateGame, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc3, 0, 0, 2, LT_MENU_NEW_Battle2 /*"Join Game"  */, 0, 0, NULL, &MENU_NEW_ChooseConnectionToJoin, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc4, 0, 0, 3, LT_MENU_NEW_Battle3 /*"back"       */, 0, 0, NULL, NULL, MenuItemBack, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc5, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc6, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },

		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/* single */
MENU	MENU_NEW_MissionMenu = {
	"", InitDifficultyLevel, NULL, NULL, TITLE_TIMER_ChooseDiscPan, 
	{
		{ 0, TITLE_MODEL_Disc1, 0, 0, 0,LT_MENU_NEW_MissionMenu0 /*"single player"*/, 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc2, 0, 0, 4, LT_MENU_NEW_MissionMenu1 /*"Start Game"*/, 0, 0, NULL, &MENU_NEW_StartSinglePlayer, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc3, 0, 0, 6, "", 0, TEXTFLAG_AutoSelect, NULL, NULL, DifficultySet, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc4, 0, 0, 2, LT_MENU_NEW_MissionMenu2 /*"back"*/, 0, 0, NULL, NULL, MenuItemBack, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc5, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc6, 0, 0, 2, "Blank", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 },

		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

/* main */
MENU	MENU_NEW_Start = {
	"", InitStartMenu, NULL, NULL, TITLE_TIMER_ChooseDiscPan,
	{
		{ 0, TITLE_MODEL_Disc1, 0, 0, 1, LT_MENU_NEW_Start1 /*"single player" */, 0, 0, NULL, &MENU_NEW_MissionMenu, MenuChange, NULL, NULL, 0 },
		//{ 0, TITLE_MODEL_Disc1, 0, 0, 0, "CoOp",                                  0, 0, NULL, &MENU_NEW_CoOp,        MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc2, 0, 0, 0, LT_MENU_NEW_Start2 /*"death match"   */, 0, 0, NULL, &MENU_NEW_Battle,      MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc3, 0, 0, 0, LT_MENU_NEW_Start3 /*"Setup Biker"   */, 0, 0, NULL, &MENU_NEW_Setup,       MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc4, 0, 0, 0, LT_MENU_NEW_Start5 /*"load Game"     */, 0, 0, NULL, &MENU_NEW_RestoreGame, MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc5, 0, 0, 0, LT_MENU_NEW_Start6 /*"Options"       */, 0, 0, NULL, &MENU_NEW_Options,     MenuChange, NULL, NULL, 0 },
		{ 0, TITLE_MODEL_Disc6, 0, 0, 0, LT_MENU_NEW_Start7 /*"Exit"          */, 0, 0, NULL, NULL,                  SelectQuit, NULL, NULL, 0 } ,

		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

MENU	MENU_STATS_Start = {
	"a", InitStartMenu, NULL, NULL, TITLE_TIMER_NormalPanToRightVDU,
	{
		{ 0, 0, 200, 20, 0, "press enter", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , GoToNextLevel , DrawFlatMenuItem, NULL, 0 } ,
		
		{ -1, -1, 0, 0, 0, "", 0, 0, NULL, NULL, NULL, NULL, NULL, 0 }
	}
};

px_timer_t quick_text_timer;
bool quick_text_timer_initialized = false;
bool quick_text_timer_ok( void )
{
	if(!quick_text_timer_initialized)
	{
		timer_clear(&quick_text_timer);
		quick_text_timer_initialized = true;
		return true;
	}
	return timer_peek(&quick_text_timer) > 0.05f;
}
void InitQuickText( MENU *Menu )
{
	if(!quick_text_timer_ok())
		MenuExit();
}
void QuitQuickText( MENU *Menu )
{
	timer_run(&quick_text_timer);
}

MENU	MENU_QuickTextSend = {
	"" , InitQuickText, QuitQuickText, NULL, 0,
	{
		{ 48, 144, 0, 0, 0, "Message: "/*"Message: "*/, 0, TEXTFLAG_AutoSelect, &QuickText, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};
MENU	MENU_QuickTextSendWhisper = {
	"" , InitQuickText, NULL, NULL, 0,
	{
		{ 48, 144, 0, 0, 0, LT_MENU_QuickTextSendWhisper0 /*"Whisper Message: "*/, 0, TEXTFLAG_AutoSelect, &QuickTextWhisper, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENU	MENU_EditMacro1 = {
	LT_MENU_EditMacro10 /*"Edit Message 1"*/ , NULL, NULL, NULL, 0,
	{
		{ 48, 144, 0, 0, 0, LT_MENU_EditMacro11 /*"Enter Message 1 "*/, 0, 0, &MacroText1, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};
MENU	MENU_EditMacro2 = {
	LT_MENU_EditMacro20 /*"Edit Message 2"*/ , NULL, NULL, NULL, 0,
	{
		{ 48, 144, 0, 0, 0, LT_MENU_EditMacro21 /*"Enter Message 2 "*/, 0, 0, &MacroText2, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" ,0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_EditMacro3 = {
	LT_MENU_EditMacro30/*"Edit Message 1"*/ , NULL, NULL, NULL, 0,
	{
		{ 100, 144, 0, 0, 0, LT_MENU_EditMacro31/*"Enter Message 3 "*/, 0, 0, &MacroText3, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENU	MENU_SinglePlayer = { "Select Difficulty Level" , NULL, NULL , NULL , 0,
					{ {	200 , 144, 0, 0, 0, "Easy" , 0, 0, NULL , (void*) DIFF_Easy , SetDifficulty , MenuItemDrawName, NULL, 0 } ,
					  {	200 , 160, 0, 0, 0, "Normal" , 0, 0, NULL , (void*) DIFF_Norm , SetDifficulty , MenuItemDrawName, NULL, 0 } ,
					  {	200 , 176, 0, 0, 0, "Hard" , 0, 0, NULL , (void*) DIFF_Hard , SetDifficulty , MenuItemDrawName, NULL, 0 } ,
					  {	200 , 192, 0, 0, 0, "Bronson Mode" , 0, 0, NULL , (void*) DIFF_Vhard , SetDifficulty , MenuItemDrawName, NULL, 0 } ,
					  {	-1 , -1, 0, 0, 0, "" , 0, 0, NULL , NULL , NULL , NULL, NULL, 0 } } };



MENU	MENU_Team = {
	"Select Teams" , InitTeamSelection , NULL , NULL, 0,
	{
		{ 200, 112, 0, 0, 0, "Start", 0, 0, NULL, &MENU_NotYet, TeamGoToSynchup , MenuItemDrawName, NULL, 0 } ,
		{ 200, 128, 0, 0, 0, "Red Team", 0, 0, &TeamList[0], NULL , SelectList , DrawList, NULL, 0 } ,
		{ 400, 128, 0, 0, 0, "Green Team", 0, 0, &TeamList[1], NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" ,0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_Team = {
	
	"" , InitTeamSelection , BailMultiplayerFrontEnd , DistributeTeamsToLists, TITLE_TIMER_PanToLeftVDU,
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_Team0 /*"select teams"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 0, 15, 200, 30, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, PlayerConfirm , DrawReadyButton, NULL, 0 } ,
		{ 5, 30, 50, 40, 0, LT_MENU_NEW_Team1 /*"Red"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[0], &Team1 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 53, 30, 98, 40, 0, LT_MENU_NEW_Team2 /*"Green"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[1], &Team2 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 101, 30, 146, 40, 0, LT_MENU_NEW_Team3 /*"Blue"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[2], &Team3 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 149, 30, 194, 40, 0, LT_MENU_NEW_Team4 /*"Yellow"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[3], &Team4 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 5, 45, 50, 126, 0, LT_MENU_NEW_Team5 /*"Red"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[0], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 53, 45, 98, 126, 0, LT_MENU_NEW_Team6 /*"Green"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[1], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 101, 45, 146, 126, 0, LT_MENU_NEW_Team7 /*"Blue"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[2], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 149, 45, 194, 126, 0, LT_MENU_NEW_Team8 /*"Yellow"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[3], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		
		{ 0, 116, 200, 126, 0, LT_MENU_NEW_Team9 /*"waiting for team selection"*/, FONT_Small, TEXTFLAG_Instant | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_Pulse, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		
		{ 5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 155, 25, 162, 0, LT_MENU_NEW_Team10 /*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" ,0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENUITEM	NewTeamItem = { 0, 0, 0, 0, 0, "GoingToDoTeamGame", 0, 0,	NULL,	&MENU_NEW_Team,	MenuChange,	NULL, NULL, 0 };

MENU	MENU_NEW_WatchTeamSelect = {
	"" , InitTeamSelection , BailMultiplayerFrontEnd , DistributeTeamsToLists, 0,
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_WatchTeamSelect0 /*"select teams"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 0, 15, 200, 30, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, PlayerConfirm , DrawReadyButton, NULL, 0 } ,
		{ 5, 30, 50, 40, 0, LT_MENU_NEW_WatchTeamSelect1 /*"Red"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[0], &Team1 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 53, 30, 98, 40, 0, LT_MENU_NEW_WatchTeamSelect2 /*"Green"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[1], &Team2 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 101, 30, 146, 40, 0, LT_MENU_NEW_WatchTeamSelect3 /*"Blue"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[2], &Team3 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 149, 30, 194, 40, 0, LT_MENU_NEW_WatchTeamSelect4 /*"Yellow"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[3], &Team4 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 5, 45, 50, 126, 0, LT_MENU_NEW_WatchTeamSelect5 /*"Red"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[0], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 53, 45, 98, 126, 0, LT_MENU_NEW_WatchTeamSelect6 /*"Green"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[1], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 101, 45, 146, 126, 0, LT_MENU_NEW_WatchTeamSelect7 /*"Blue"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[2], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 149, 45, 194, 126, 0, LT_MENU_NEW_WatchTeamSelect8 /*"Yellow"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[3], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,

		{ 0, 116, 200, 126, 0, LT_MENU_NEW_WatchTeamSelect9 /*"waiting for team selection"*/, FONT_Small, TEXTFLAG_Instant | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_Pulse, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		
		{ 5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 155, 25, 162, 0, LT_MENU_NEW_WatchTeamSelect10 /*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" ,0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU  MENU_NEW_ChooseTeamForExistingGame = {
	"" , InitExistingGameJoin , BailMultiplayerFrontEnd , DistributeTeamsToLists, 0,
	{
		{ 0, 0, 200, 15, 0, LT_MENU_NEW_ChooseTeamForExistingGame0 /*"select teams"*/, FONT_Large, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 0, 15, 200, 30, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL, PlayerConfirm , DrawReadyButton, NULL, 0 } ,
		{ 5, 30, 50, 40, 0, LT_MENU_NEW_ChooseTeamForExistingGame1 /*"Red"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[0], &Team1 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 53, 30, 98, 40, 0, LT_MENU_NEW_ChooseTeamForExistingGame2 /*"Green"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[1], &Team2 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 101, 30, 146, 40, 0, LT_MENU_NEW_ChooseTeamForExistingGame3 /*"Blue"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[2], &Team3 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 149, 30, 194, 40, 0, LT_MENU_NEW_ChooseTeamForExistingGame4 /*"Yellow"*/,  FONT_Small,TEXTFLAG_ForceFit | TEXTFLAG_CentreX | TEXTFLAG_CentreY | TEXTFLAG_TeamItem, &TeamList[3], &Team4 , SelectTeamList, DrawFlatMenuItem, NULL, 0 } ,
		{ 5, 45, 50, 126, 0, LT_MENU_NEW_ChooseTeamForExistingGame5 /*"Red"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[0], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 53, 45, 98, 126, 0, LT_MENU_NEW_ChooseTeamForExistingGame6 /*"Green"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[1], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 101, 45, 146, 126, 0, LT_MENU_NEW_ChooseTeamForExistingGame7 /*"Blue"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[2], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
		{ 149, 45, 194, 126, 0, LT_MENU_NEW_ChooseTeamForExistingGame8 /*"Yellow"*/,  FONT_Small, TEXTFLAG_Unselectable | TEXTFLAG_SuppressHighlight | TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TeamList[3], NULL , SelectList, DrawFlatMenuList, NULL, 0 } ,
#if 0
		{ 5, 118, 50, 125, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)TeamCurrentScore[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 53, 118, 98, 125, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)TeamCurrentScore[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 101, 118, 146, 125, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)TeamCurrentScore[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 149, 118, 194, 125, 0, "", FONT_Small, TEXTFLAG_CheckForRefresh | TEXTFLAG_ForceFit | TEXTFLAG_CentreY, (void *)TeamCurrentScore[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
#endif

		{ 5, 126, 200, 133, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[3], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 133, 200, 140, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[2], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 140, 200, 147, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[1], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,
		{ 5, 147, 200, 154, 0, "", FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, (void *)TitleMessage[0], NULL , NULL , DrawFlatMenuName, NULL, 0 } ,

		{ 5, 155, 25, 162, 0, LT_MENU_NEW_ChooseTeamForExistingGame9 /*"msg:"*/, FONT_Small, TEXTFLAG_ForceFit | TEXTFLAG_CentreY | TEXTFLAG_Instant, &TitlePlayerMessage, NULL ,SelectFlatMenutext , DrawFlatMenuText, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" ,0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENUITEM	TeamItem = { 0, 0, 0, 0, 0, "GoingToDoTeamGame", 0, 0,	NULL,	&MENU_Team,	MenuChange,		MenuItemDrawName, NULL, 0 };

MENU	MENU_HostWaitingToStart = {
	"Hosting a session" , NULL , BailMultiplayer , NULL, 0,
	{
		{ 200, 112, 0, 0, 0, "Start", 0, 0, NULL, &MENU_NotYet, GoToSynchup , MenuItemDrawName, NULL, 0 } ,
		{ 200, 128, 0, 0, 0, "Num of Players ", 0, 0, &NumOfPlayersSlider, NULL, NULL, DrawSlider, NULL, 0 },
		{ 200, 128, 0, 0, 0, "", 0, 0, &PlayersList, NULL , NULL , GetPlayersInCurrentSession, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_JoinWaitingToStart = {
	"Waiting for Host" , NULL , BailMultiplayer , NULL, 0,
	{
		{ 200, 128, 0, 0, 0, "Num of Players ", 0, 0, &NumOfPlayersSlider, NULL, NULL, DrawSlider, NULL, 0 },
		{ 200, 128, 0, 0, 0, "", 0, 0, &PlayersList, NULL , NULL , GetPlayersInCurrentSession, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL, NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_Host = {
	"Hosting a MultiPlayer Game" , InitMultiplayerHost , NULL , NULL, 0,
	{
		{ 200, 112, 0, 0, 0, "Start", 0, 0, NULL, &MENU_HostWaitingToStart, StartAHostSession , MenuItemDrawName, NULL, 0 } ,
		{ 200, 128, 0, 0, 0, "Team Game", 0, 0, &TeamGame, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 144, 0, 0, 0, "Record Demo", 0, 0, &RecordDemo, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 160, 0, 0, 0, "Demo Name  ", 0, 0, &DemoGameName, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ 200, 176, 0, 0, 0, "Time Limit ", 0, 0, &MyTimeLimit, NULL, SelectSlider, DrawSlider, NULL, 0 },
		{ 200, 192, 0, 0, 0, "Max Players ", 0, 0, &MaxPlayersSlider, NULL, SelectSlider, DrawSlider, NULL, 0 },
		{ 200, 224, 0, 0, 0, "Level...", 0, 0, &LevelList, NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_LevelSelect = {
	LT_MENU_LevelSelect0 /*"Level Selection"*/ , InitInGameLevelSelect , ExitLevelSelect , NULL, 0,
	{
		{ 200, 112, 0, 0, 0, LT_MENU_LevelSelect1 /*"Ability To Change "*/, 0, 0, &IsHost, NULL, NULL, DrawToggle, NULL, 0 },
		{ 200, 128, 0, 0, 0, LT_MENU_LevelSelect2 /*"Level..."*/, 0, 0, &LevelList, NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENUITEM	JoinItem = { 0, 0, 0, 0, 0, "GoingToJoinASession", 0, 0,	NULL,	&MENU_JoinWaitingToStart,	MenuChange,		MenuItemDrawName, NULL, 0 };
MENUITEM	NewJoinItem = { 0, 0, 0, 0, 0, "GoingToJoinASession", 0, 0,	NULL,	&MENU_NEW_JoinWaitingToStart,	MenuChange,		DrawFlatMenuItem, NULL, 0 };
MENUITEM	WatchTeamSelectionItem = { 0, 0, 0, 0, 0, "GoingToJoinASession", 0, 0,	NULL,	&MENU_NEW_Team,	MenuChange,		DrawFlatMenuItem, NULL, 0 };

						
MENU	MENU_ChooseSessionToJoin = {
	"" , NULL , NULL, NULL, 0,
	{
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_HostOrJoin = {
	"MultiPlayer" , NULL , NULL, NULL, 0,
	{
		{ 200, 112, 0, 0, 0, "Start a Game", 0, 0,	NULL,	&MENU_Host,	MenuChange,		MenuItemDrawName, NULL, 0 },
		{ 200, 128, 0, 0, 0, "Join a Network Game", 0, 0,	NULL,	&MENU_ChooseSessionToJoin,	MenuChange,		MenuItemDrawName, NULL, 0 },
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_LoadGame;
MENU	MENU_SaveGame = {
	LT_MENU_SaveGame0 /*"Save Game"*/, InitLoadSavedGameList, NULL, NULL, 0,
	{
		{ 100 , 128, 0, 0, 0, LT_MENU_SaveGame1 /*"Choose From..."*/, 0, 0, &LoadSavedGameList, InGameSaveASinglePlayerGame, SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_Joystick;

MENU	MENU_WeaponPriority =
{
	"Weapon Priority", NULL, NULL, NULL, 0,
	{
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_WeaponKeys =
{
	"Weapon Selection", NULL, NULL, NULL, 0,
	{
		KEYDEF( 32, 112,"Pulsar", (void *)&Config.select_primary[ PULSAR ] ),
		KEYDEF( 32, 128,"Trojax", (void *)&Config.select_primary[ TROJAX ] ),
		KEYDEF( 32, 144,"Suss Gun", (void *)&Config.select_primary[ SUSS_GUN ] ),
		KEYDEF( 32, 160,"Transpulse", (void *)&Config.select_primary[ TRANSPULSE_CANNON ] ),
		KEYDEF( 32, 176,"Pyrolite", (void *)&Config.select_primary[ PYROLITE_RIFLE ] ),
		KEYDEF( 32, 192,"Beam Laser", (void *)&Config.select_primary[ LASER ] ),
		
		KEYDEF( 32, 224,"Next Prim", (void *)&Config.select_next_primary ),
		KEYDEF( 32, 240,"Prev Prim", (void *)&Config.select_prev_primary ),
		
		KEYDEF( 32, 272,"Next Sec", (void *)&Config.select_next_secondary ),
		KEYDEF( 32, 288,"Prev Sec", (void *)&Config.select_prev_secondary ),
		
		KEYDEF( 352, 112, "Mug Missile", (void *)&Config.select_secondary[ MUGMISSILE ] ),
		KEYDEF( 352, 128,"Solaris", (void *)&Config.select_secondary[ SOLARISMISSILE ] ),
		KEYDEF( 352, 160, "Scatter", (void *)&Config.select_secondary[ SCATTERMISSILE ] ),
		KEYDEF( 352, 176, "Gravgon", (void *)&Config.select_secondary[ GRAVGONMISSILE ] ),
		KEYDEF( 352, 192, "MFRL", (void *)&Config.select_secondary[ MULTIPLEMISSILE ] ),
		KEYDEF( 352, 208, "Titan Star", (void *)&Config.select_secondary[ TITANSTARMISSILE ] ),
		KEYDEF( 352, 224, "Thief Missile", (void *)&Config.select_secondary[ THIEFMISSILE ] ),

		KEYDEF( 352, 256, "Purge Mine", (void *)&Config.select_secondary[ PURGEMINE ] ),
		KEYDEF( 352, 272, "Pine Mine", (void *)&Config.select_secondary[ PINEMINE ] ),
		KEYDEF( 352, 288, "Quantum Mine", (void *)&Config.select_secondary[ QUANTUMMINE ] ),
		KEYDEF( 352, 304, "Spider Mine", (void *)&Config.select_secondary[ SPIDERMINE ] ),


		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

#if 1
MENU	MENU_Keyboard =
{
	LT_MENU_Keyboard0 /*"Configure controls"*/, InitKeyDefPage, NULL, NULL, 0,
	{
		KDEF( 0, 200, 112, LT_MENU_Keyboard1 /*"Rotate Left"*/, KDleft ),
		KDEF( 0, 200, 128, LT_MENU_Keyboard2 /*"Rotate Right"*/, KDright ),
		KDEF( 0, 200, 144, LT_MENU_Keyboard3 /*"Rotate Up"*/, KDup ),	      
		KDEF( 0, 200, 160, LT_MENU_Keyboard4 /*"Rotate Down"*/, KDdown ),
			      
		KDEF( 0, 200, 184, LT_MENU_Keyboard5 /*"Slide Left"*/, KDmove_left ),	         
		KDEF( 0, 200, 200, LT_MENU_Keyboard6 /*"Slide Right"*/, KDmove_right ),
			   
		KDEF( 0, 200, 216, LT_MENU_Keyboard7 /*"Slide Up"*/, KDmove_up ),         
		KDEF( 0, 200, 232, LT_MENU_Keyboard8 /*"Slide Down"*/, KDmove_down ),
			   
		KDEF( 0, 200, 256, LT_MENU_Keyboard9 /*"Roll Left"*/, KDroll_left ),  
		KDEF( 0, 200, 272, LT_MENU_Keyboard10 /*"Roll Right"*/, KDroll_right ),

		KDEF( 0, 200, 296, LT_MENU_Keyboard11 /*"Slide Mode"*/, KDmove ),
		KDEF( 0, 200, 312, LT_MENU_Keyboard12 /*"Roll Mode"*/, KDroll ),

#ifdef PLAYER_SPEECH_TAUNTS
		KDEF( 0, 200, 328,LT_MENU_Keyboard36 /*"speech taunt"*/, KDsend_speech),
#endif

		// 2nd page

		KDEF( 1, 200, 112, LT_MENU_Keyboard13 /*"Forward"*/, KDmove_forward ),
		KDEF( 1, 200, 128, LT_MENU_Keyboard14 /*"Reverse"*/, KDmove_backward ),
		KDEF( 1, 200, 144, LT_MENU_Keyboard15 /*"Nitro"*/, KDturbo ),

		KDEF( 1, 200, 168, LT_MENU_Keyboard16 /*"Speed Up"*/, KDcruise_faster ),
		KDEF( 1, 200, 184, LT_MENU_Keyboard17 /*"Slow Down"*/, KDcruise_slower ),

		KDEF( 1, 200, 208, LT_MENU_Keyboard18 /*"Fire Prim"*/, KDfire_primary ),
		KDEF( 1, 200, 224, LT_MENU_Keyboard19 /*"Fire Sec"*/, KDfire_secondary ),
		KDEF( 1, 200, 240, LT_MENU_Keyboard20 /*"Drop Mine"*/, KDfire_mine ),
		KDEF( 1, 200, 264, LT_MENU_Keyboard21 /*"Drop Prim"*/, KDdrop_primary ),
		KDEF( 1, 200, 280, LT_MENU_Keyboard22 /*"Drop Sec"*/, KDdrop_secondary ),
		KDEF( 1, 200, 296, LT_MENU_Keyboard23 /*"Drop Shield"*/, KDdrop_shield ),
		KDEF( 1, 200, 312, LT_MENU_Keyboard24 /*"Drop Ammo"*/, KDdrop_ammo ),

		// 3rd page

		KDEF( 2, 200, 112,LT_MENU_Keyboard25 /*"Pulsar"*/, KDselect_primary[ PULSAR ] ),
		KDEF( 2, 200, 128,LT_MENU_Keyboard26 /*"Trojax"*/, KDselect_primary[ TROJAX ] ),
		KDEF( 2, 200, 144,LT_MENU_Keyboard27 /*"Pyrolite"*/, KDselect_primary[ PYROLITE_RIFLE ] ),
		KDEF( 2, 200, 160,LT_MENU_Keyboard28 /*"Transpulse"*/, KDselect_primary[ TRANSPULSE_CANNON ] ),
		KDEF( 2, 200, 176,LT_MENU_Keyboard29 /*"Suss Gun"*/, KDselect_primary[ SUSS_GUN ] ),
		KDEF( 2, 200, 192,LT_MENU_Keyboard30 /*"Beam Laser"*/, KDselect_primary[ LASER ] ),
		KDEF( 2, 200, 216,LT_MENU_Keyboard31 /*"Next Prim"*/, KDselect_next_primary ),
		KDEF( 2, 200, 232,LT_MENU_Keyboard32 /*"Prev Prim"*/, KDselect_prev_primary ),
		KDEF( 2, 200, 256,LT_MENU_Keyboard33 /*"Next Sec"*/, KDselect_next_secondary ),
		KDEF( 2, 200, 272,LT_MENU_Keyboard34 /*"Prev Sec"*/, KDselect_prev_secondary ),
		
		KDEF( 2, 200, 296,LT_MENU_Keyboard35 /*"Send Msg"*/, KDsend_msg),
		KDEF( 2, 200, 312,LT_MENU_Keyboard1a /*"headlights"*/, KDheadlights),
		KDEF( 2, 200, 328,LT_MENU_Keyboard1b /*"full rear view"*/, KDfull_rearview),
		// 4th page
		
		KDEF( 3, 200, 112, LT_MENU_Keyboard37 /*"Mug Missile"*/, KDselect_secondary[ MUGMISSILE ] ),
		KDEF( 3, 200, 128, LT_MENU_Keyboard38 /*"Solaris"*/, KDselect_secondary[ SOLARISMISSILE ] ),
		KDEF( 3, 200, 144, LT_MENU_Keyboard39 /*"Scatter"*/, KDselect_secondary[ SCATTERMISSILE ] ),
		KDEF( 3, 200, 160, LT_MENU_Keyboard40 /*"Gravgon"*/, KDselect_secondary[ GRAVGONMISSILE ] ),
		KDEF( 3, 200, 176, LT_MENU_Keyboard41 /*"MFRL"*/, KDselect_secondary[ MULTIPLEMISSILE ] ),
		KDEF( 3, 200, 192, LT_MENU_Keyboard42 /*"Titan Star"*/, KDselect_secondary[ TITANSTARMISSILE ] ),
		KDEF( 3, 200, 208, LT_MENU_Keyboard46 /*"Thief Missile"*/, KDselect_secondary[ THIEFMISSILE ] ),

		KDEF( 3, 200, 232, LT_MENU_Keyboard43 /*"Purge Mine"*/, KDselect_secondary[ PURGEMINE ] ),
		KDEF( 3, 200, 248, LT_MENU_Keyboard44 /*"Pine Mine"*/, KDselect_secondary[ PINEMINE ] ),
		KDEF( 3, 200, 264, LT_MENU_Keyboard45 /*"Quantum Mine"*/, KDselect_secondary[ QUANTUMMINE ] ),
		KDEF( 3, 200, 280, LT_MENU_Keyboard47 /*"Spider Mine"*/, KDselect_secondary[ SPIDERMINE ] ),

		KDEF( 3, 200, 310, LT_MENU_Keyboard48 /*"Show Messages"*/, KDshow_messages ),
		KDEF( 3, 200, 326, LT_MENU_Keyboard49 /*"Show Statistics"*/, KDshow_stats ),
		KDEF( 3, 200, 342, LT_MENU_Keyboard50 /*"Show Network Info"*/, KDshow_networkinfo ),

		{ 200, 32, 0, 0, 0, "help3", 0, 0, NULL, NULL, NULL, DrawKeyDefHelp3, NULL, 0 },
		{ 200, 336, 0, 0, 0, "help1", 0, 0, NULL, NULL, NULL, DrawKeyDefHelp1, NULL, 0 },
		{ 200, 352, 0, 0, 0, "help2", 0, 0, NULL, NULL, NULL, DrawKeyDefHelp2, NULL, 0 },
		{ 200, 368, 0, 0, 0, "Page", 0, 0, NULL, NULL, NULL, DrawKeyDefPage, NULL, 0 },
//		{ 200, 368, 0, 0, 0,  "Weapon selection keys", 0, 0, NULL, &MENU_WeaponKeys, MenuChange, MenuItemDrawName, NULL, 0 },

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};
#else
MENU	MENU_Keyboard =
{
	"Keyboard", NULL, NULL, NULL, 0,
	{
		KEYDEF( 32, 112, "Rotate Left", (void *)&Config.left ),

		KEYDEF( 32, 128, "Rotate Right", (void *)&Config.right ),

		KEYDEF( 32, 144, "Rotate Up", (void *)&Config.up ),
			      
		KEYDEF( 32, 160, "Rotate Down", (void *)&Config.down ),

			      
		KEYDEF( 32, 192, "Slide Left", (void *)&Config.move_left ),
			         
		KEYDEF( 32, 208, "Slide Right", (void *)&Config.move_right ),
			   
		KEYDEF( 32, 224, "Slide Up", (void *)&Config.move_up ),
			         
		KEYDEF( 32, 240, "Slide Down", (void *)&Config.move_down ),

			   
		KEYDEF( 32, 272, "Roll Left", (void *)&Config.roll_left ),
			      
		KEYDEF( 32, 288, "Roll Right", (void *)&Config.roll_right ),


		KEYDEF( 32, 320, "Slide Mode", (void *)&Config.move ),

		KEYDEF( 32, 336, "Roll Mode", (void *)&Config.roll ),

		// right column

		KEYDEF( 352, 112, "Forward", (void *)&Config.move_forward ),

		KEYDEF( 352, 128, "Reverse", (void *)&Config.move_backward ),
		
		KEYDEF( 352, 144, "Nitro", (void *)&Config.turbo ),


		KEYDEF( 352, 176, "Speed Up", (void *)&Config.cruise_faster ),

		KEYDEF( 352, 192, "Slow Down", (void *)&Config.cruise_slower ),


		KEYDEF( 352, 224, "Fire Prim", (void *)&Config.fire_primary ),

		KEYDEF( 352, 240, "Fire Sec", (void *)&Config.fire_secondary ),
		
		KEYDEF( 352, 256, "Drop Mine", (void *)&Config.fire_mine ),

		KEYDEF( 352, 288, "Drop Prim", (void *)&Config.drop_primary ),

		KEYDEF( 352, 304, "Drop Sec", (void *)&Config.drop_secondary ),

		KEYDEF( 352, 320, "Drop Shield", (void *)&Config.drop_shield ),

		KEYDEF( 352, 336, "Drop Ammo", (void *)&Config.drop_ammo ),


		{ 200, 368, 0, 0, 0,  "Weapon selection keys", 0, 0, NULL, &MENU_WeaponKeys, MenuChange, MenuItemDrawName, NULL, 0 },

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};
#endif

MENU	MENU_Primary;
MENU	MENU_Secondary;
MENU	MENU_Controls = {
	LT_MENU_Controls0 /*"Bike Controls"*/, InitControls, ExitControls, NULL, 0,
	{
#if 0
		{ 200, 144 - 32, 0, 0, 0, "Mouse", 0, 0,				(void *)&ControlMethod,	(void *)CONTROL_Mouse,		SelectRadioButton,	DrawRadioButton, NULL, 0 },
		{ 200, 160 - 32, 0, 0, 0, "Keyboard", 0, 0,				(void *)&ControlMethod,	(void *)CONTROL_Keyboard,	SelectRadioButton,	DrawRadioButton, NULL, 0 },
		{ 200, 176 - 32, 0, 0, 0, "Joystick", 0, 0,				(void *)&ControlMethod,	(void *)CONTROL_Joystick,	SelectRadioButton,	DrawRadioButton, NULL, 0 },
		{ 200, 192 - 32, 0, 0, 0, "Gamepad", 0, 0,				(void *)&ControlMethod,	(void *)CONTROL_Gamepad,	SelectRadioButton,	DrawRadioButton, NULL, 0 },
		
		{ 200, 176 + 16, 0, 0, 0, "Invert X",  0, 0,			&Config.invert_turn,			NULL,				SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 192 + 16, 0, 0, 0, "Invert Y",  0, 0,			&Config.invert_pitch,			NULL,				SelectToggle,		DrawToggle, NULL, 0 },
				
		{ 200, 224, 0, 0, 0, "X Sensitivity  ", 0, 0,		&SensitivityXSlider,		NULL,						SelectSlider,		DrawSlider, NULL, 0 },
		{ 200, 240, 0, 0, 0, "Y Sensitivity  ", 0, 0,		&SensitivityYSlider,		NULL,						SelectSlider,		DrawSlider, NULL, 0 },
		{ 200, 272, 0, 0, 0, "Autoleveling",  0, 0,			&Autoleveling,			SetAutolevel,				SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 288, 0, 0, 0, "Calibrate Joystick", 0, 0,	NULL,					&MENU_NotYet,				MenuChange,			MenuItemDrawName, NULL, 0 },
		{ 200, 320, 0, 0, 0, "Configure Keyboard", 0, 0,	NULL,					&MENU_Keyboard,				MenuChange,			MenuItemDrawName, NULL, 0 },
		{ 200, 336, 0, 0, 0, "Weapon Priority", 0, 0,		NULL,					&MENU_NotYet,				MenuChange,			MenuItemDrawName, NULL, 0 },
		{ 200, 352, 0, 0, 0, "Reorder Secondary", 0, 0,	NULL,					&MENU_NotYet,				MenuChange,			MenuItemDrawName, NULL, 0 },
#else
		{ 200, 112, 0, 0, 0, LT_MENU_Controls1 /*"Configure controls"*/, 0, 0,	NULL,					&MENU_Keyboard,				MenuChange,			MenuItemDrawName, NULL, 0 },
		{ 200, 160, 0, 0, 0, LT_MENU_Controls3 /*"Joystick          "*/, 0, 0,	&JoystickInput,			NULL,						SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 192, 0, 0, 0, LT_MENU_Controls4 /*"Autoleveling      "*/,  0, 0,			&Autoleveling,			SetAutolevel,				SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 224, 0, 0, 0, LT_MENU_Controls5 /*"Invert X          "*/,  0, 0,			&Config.invert_turn,			NULL,				SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 240, 0, 0, 0, LT_MENU_Controls6 /*"Invert Y          "*/,  0, 0,			&Config.invert_pitch,			NULL,				SelectToggle,		DrawToggle, NULL, 0 },
		{ 200, 272, 0, 0, 0, LT_MENU_Controls7 /*"X Sensitivity     "*/, 0, 0,		&SensitivityXSlider,		NULL,						SelectSlider,		DrawSlider, NULL, 0 },
		{ 200, 288, 0, 0, 0, LT_MENU_Controls8 /*"Y Sensitivity     "*/, 0, 0,		&SensitivityYSlider,		NULL,						SelectSlider,		DrawSlider, NULL, 0 },
#endif
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}																					
};

// in-game-menu
MENU	MENU_Detail = {
	LT_MENU_Detail0 /*"Detail Levels"*/, NULL, ExitDetailLevels, NULL, 0,
	{
		{ 200, 180, 0, 0, 0, LT_MENU_Detail4/*"Team Info"*/,     0, 0,  &ShowTeamInfo,     NULL,SelectToggle,DrawToggle,NULL, 0 },
		{ 200, 196, 0, 0, 0, "MATURE CONTENT      ", 0, 0, &MatureContent, NULL, SelectMatureContentToggle, DrawMatureContentToggle,	NULL, 0 },
		{ 200, 212, 0, 0, 0, LT_MENU_Detail12/*"Water Detail"*/, 0, 0,	&WaterDetailSlider,NULL,SelectSlider,DrawSlider,NULL, 0 },
		{ 200, 228, 0, 0, 0, LT_MENU_Detail13/*"gamma"*/,        0, 0,	&GammaSlider,      NULL,SelectSlider,DrawSlider,NULL, 0 },
		{-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_DebugMode = {
	"Select Debug Mode" , InitDebugMode , NULL , NULL, 0,
	{
		
		{ 200, 96, 0, 0, 0, "Show Plane RGB", 0, 0, &ShowPlaneRGB, NULL, SelectToggle, DrawToggle, NULL, 0 },
		
		{ 200, 144, 0, 0, 0, "external forces", 0, 0, &ShowEFZones, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 160, 0, 0, 0, "show teleports", 0, 0, &ShowTeleports, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 176, 0, 0, 0, "trigger zones", 0, 0, &ShowTrigZones, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 192, 0, 0, 0, "bounding boxes", 0, 0, &ShowBoundingBoxes, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 208, 0, 0, 0, "collision zones", 0, 0, &ShowColZones, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 224, 0, 0, 0, "Untriggered Enemies", 0, 0, &ShowUntriggeredNMEs, NULL, SelectToggle, DrawToggle, NULL, 0 },

		{ 200, 256, 0, 0, 0, "Screen Capture Mode ...on F8 key", 0, 0,		&ScreenSaving,		NULL,	SelectToggle,	DrawToggle, NULL, 0 },
		{ 200, 272, 0, 0, 0, "Old BSP Cube", 0, 0, &OldNodeCube, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 288, 0, 0, 0, "BSP Node Cube", 0, 0, &NodeCube, NULL, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 304, 0, 0, 0, "BSP Nodes tested", 0, 0, &NodeCubeType, NULL, SelectToggle, DrawToggle, NULL, 0 },
		
		{ 200, 336, 0, 0, 0, "Show Nodes", 0, 0, &ShowNode, ShowNodeToggle, SelectToggle, DrawToggle, NULL, 0 },
		{ 200, 352, 0, 0, 0, "Show Startpoints", 0, 0, &ShowStartPoints, ShowStartPointsToggle, SelectToggle, DrawToggle, NULL, 0 },

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_SelectBike = {
	LT_MENU_SelectBike0 /*"Select Bike"*/ , InitBikeList , NULL , NULL,	0,
	{
		{ 200, 128, 0, 0, 0, LT_MENU_SelectBike1 /*"Choose From..."*/, 0, 0, &BikeList, ExitBikeSelection , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_SelectBikeComputer = {
	LT_MENU_SelectBikeComputer0 /*"Select Bike Computer"*/ , InitBikeComputerMenu , NULL , NULL,	0,
	{
		{ 200, 128, 0, 0, 0, LT_MENU_SelectBikeComputer1 /*"Choose From..."*/, 0, 0, &BikeComputerList, ExitBikeComputerSelection , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_NEW_ChangeBikeComputer = {
	LT_MENU_NEW_ChangeBikeComputer0 /*"Select Bike Computer"*/ , InitBikeComputerMenu , NULL , NULL,	0,
	{
		{ 0, 0, 200, 20, 0, LT_MENU_NEW_ChangeBikeComputer1 /*"Choose bike computer..."*/, FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY, NULL, NULL , NULL , DrawFlatMenuItem, NULL, 0 } ,
		{ 15 , 30, 100, 150, 0, LT_MENU_NEW_ChangeBikeComputer2 /*"list..."*/ , FONT_Small, TEXTFLAG_SuppressHighlight | TEXTFLAG_AutoSelect | TEXTFLAG_ForceFit | TEXTFLAG_CentreY , &BikeComputerList, ExitBikeComputerSelection , SelectList , DrawFlatMenuList, NULL, 0},

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENU	MENU_Bike;
MENU	MENU_Computer;
MENU	MENU_Restore;

MENU	MENU_SelectScreenMode = {
	LT_MENU_SelectScreenMode0 /*"Select Screen Mode"*/ , MakeModeList , MenuSelectMode , NULL, 0,
	{
		{ 200, 128, 0, 0, 0, LT_MENU_SelectScreenMode1 /*"Choose From..."*/, 0, 0, &ModeList, NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

/*XXX*/
MENU	MENU_Visuals = {
	LT_MENU_Visuals0 /*"Visuals"*/, NULL, NULL, NULL, 0,
	{
		{ 200, 128 + ( 0*16 ), 0, 0, 0, LT_MENU_Visuals1		/*"Select Screen Mode"		*/, 0, 0, NULL,									&MENU_SelectScreenMode,		MenuChange,			MenuItemDrawName,		NULL, 0 },
		
		{ 200, 128 + ( 2*16 ), 0, 0, 0, "TEXT SCALE", 0, 0,	&TextScaleSlider,	NULL,	SelectSlider,	DrawSlider, NULL, 0 },

		{ 200, 128 + ( 3*16 ), 0, 0, 0, "HUD COLOUR ", 0, 0, &HUDColour,	NULL,		SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 4*16 ), 0, 0, 0, "MENU COLOUR ", 0, 0, &InGameMenuColour,	NULL,		SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 5*16 ), 0, 0, 0, LT_MENU_InGame28	/*"normal kill messages"		*/, 0, 0, &KillMessageColour,				NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 6*16 ), 0, 0, 0, LT_MENU_InGame29	/*"milestone kill messages"	*/, 0, 0,	&MilestoneMessagesColour,	NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 7*16 ), 0, 0, 0, LT_MENU_InGame30	/*"system messages"		*/, 0, 0,	&SystemMessageColour,		NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 8*16 ), 0, 0, 0, LT_MENU_InGame31	/*"flag/bounty messages"	*/, 0, 0,	&FlagMessageColour,				NULL,										SelectColourToggle, DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 9*16 ), 0, 0, 0, LT_MENU_InGame32	/*"player messages"			*/, 0, 0,	&PlayerMessageColour,			NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 10*16 ), 0, 0, 0, LT_MENU_InGame33	/*"pickup messages"			*/, 0, 0,	&PickupMessageColour,			NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 11*16 ), 0, 0, 0, LT_MENU_InGame34	/*"taunt messages"			*/, 0, 0,	&TauntMessageColour,			NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ 200, 128 + ( 12*16 ), 0, 0, 0, LT_MENU_InGame35	/*"your messages"			*/, 0, 0,	&MyMessageColour,				NULL,										SelectColourToggle,	DrawMessagesToggle,	NULL, 0 },
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

extern bool ShowFrameRate;
extern bool ShowInfo;

MENU	MENU_Options = {
	LT_MENU_Options0/*"Options"*/, NULL, (MenuFunc) SetGamePrefs, NULL, 0,
	{
		{ 200, 128, 0, 0, 0, LT_MENU_Options1	/*"Visuals"*/,				0, 0, NULL,						&MENU_Visuals,			MenuChange,		MenuItemDrawName,	NULL, 0 },
		{ 200, 144, 0, 0, 0, LT_MENU_Options2	/*"Sound FX and Music"*/,	0, 0, NULL,						&MENU_NEW_InGameSound,	MenuChange,		MenuItemDrawName,	NULL, 0 },
		{ 200, 160, 0, 0, 0, LT_MENU_Options3	/*"Detail Levels"*/,		0, 0, NULL,						&MENU_Detail,			MenuChange,		MenuItemDrawName,	NULL, 0 },
		{ 200, 176, 0, 0, 0, LT_MENU_Options4	/*"Show Frame Rate "*/,		0, 0, &ShowFrameRate,	NULL,					SelectToggle,	DrawToggle,			NULL, 0 },
		{ 200, 192, 0, 0, 0, LT_MENU_Options8	/*"Show Extra Info "*/,		0, 0, &ShowInfo,		NULL,					SelectToggle,	DrawToggle,			NULL, 0 },
		{ 200, 208, 0, 0, 0, LT_MENU_Options9	/*"Show Weapon Kills"*/,	0, 0, &ShowWeaponKills,			NULL,					SelectToggle,	DrawToggle,			NULL, 0 },
		{ 200, 224, 0, 0, 0, LT_MENU_Options13	/*"Show Players on HUD"*/,	0, 0, &ShowPlayersOnHUD,		NULL,					SelectToggle,	DrawToggle,			NULL, 0 },
		{ 200, 240, 0, 0, 0, "HUD Show Kills Only",	0, 0, &ShowPlayersOnHUDbyKills,NULL,SelectToggle,DrawToggle,NULL, 0 },
		{ 200, 256, 0, 0, 0, "HUD Show Inventory",	0, 0, &ShowWeaponsPossessedOnHUD,NULL,SelectToggle,DrawToggle,NULL, 0 },
		{ 200, 272, 0, 0, 0, "HUD Show Clock",	0, 0, &ShowClockOnHUD,NULL,SelectToggle,DrawToggle,NULL, 0 },
		{ 200, 288, 0, 0, 0, "HUD Show KPM",	0, 0, &ShowKPMOnHUD,NULL,SelectToggle,DrawToggle,NULL, 0 },
		{ 200, 304, 0, 0, 0, "HUD Show Speed",	0, 0, &ShowSpeedOnHUD,NULL,SelectToggle,DrawToggle,NULL, 0 },
        { 200, 320, 0, 0, 0, "Show Health Above Bikes", 0, 0, &ShowPlayerHealthAboveBikes,NULL,SelectToggle,DrawToggle,NULL, 0 },
        { 200, 336, 0, 0, 0, "Show Health By Scores", 0, 0, &ShowPlayerHealthByScores,NULL,SelectToggle,DrawToggle,NULL, 0 },
        { 200, 352, 0, 0, 0, "Scale Font Above Bikes", 0, 0, &ScaleFontPlayerHealthAboveBikes,NULL,SelectToggle,DrawToggle,NULL, 0 },
		{	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_DemoPlaying = {
	LT_MENU_DemoPlaying0 /*"Demo Playing"*/ , NULL, NULL, NULL, 0,
	{
		{ 200 , 128           , 0, 0, 0, LT_MENU_DemoPlaying1 /*"Pause Demo "*/, 0, 0,	&PauseDemo,	NULL, PauseDemoToggle,	DrawToggle, NULL, 0 },
		{ 200 , 128 + ( 2*16 ), 0, 0, 0, LT_MENU_DemoPlaying7 /*"Playback Speed"*/, 0, 0,	&DemoSpeed,		NULL,	SelectSlider,	DrawSlider, NULL, 0 },
		{ 200 , 128 + ( 4*16 ), 0, 0, 0, LT_MENU_DemoPlaying8 /*"Watch Player"*/, 0, 0,		&DemoEyesSelect,		NULL,	SelectSlider,	DrawSlider, NULL, 0 },
		{ 200 , 128 + ( 5*16 ), 0, 0, 0, LT_MENU_DemoPlaying9 /*"Options"*/ , 0, 0, NULL, &MENU_Options , MenuChange , MenuItemDrawName, NULL, 0 } ,
#ifdef DEBUG_ON
		{ 200 , 128 + ( 6*16 ), 0, 0, 0, LT_MENU_DemoPlaying10 /*"Debugging"*/, 0, 0,	&DebugInfo,	DebugModeChanged, SelectToggle,	DrawToggle, NULL, 0 },
#endif
		{ 200 , 128 + ( 7*16 ), 0, 0, 0, LT_MENU_DemoPlaying11 /*"Quit to Title Screen"*/ , 0, 0, NULL, NULL, SelectQuitCurrentGame , MenuItemDrawName, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


#ifdef DEMO_SUPPORT

MENU	MENU_DemoPlayBack = {
	"Demo Playback"/*"Demo Playback"*/ , InitDemoList, NULL, NULL, 0,
	{
		{ 200 , 128, 0, 0, 0, "Start"/*"Start"*/ , 0, 0, NULL, &MENU_NotYet , StartDemoPlayback , MenuItemDrawName, NULL, 0 } ,
		{ 200 , 144, 0, 0, 0, "Playback Speed"/*"Playback Speed"*/, 0, 0,	&DemoSpeed,		NULL,	SelectSlider,	DrawSlider, NULL, 0 },
		{ 200 , 176, 0, 0, 0, "Watch Player"/*"Watch Player"*/, 0, 0,		&DemoEyesSelect,		NULL,	SelectSlider,	DrawSlider, NULL, 0 },
		{ 200 , 192, 0, 0, 0, "Choose From..."/*"Choose From..."*/, 0, 0, &DemoList, NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_DemoCleaning = {
	LT_MENU_DemoCleaning0 /*"Clean A Demo"*/ , InitDemoList, NULL, NULL, 0,
	{
		{ 200 , 128, 0, 0, 0, LT_MENU_DemoCleaning1 /*"Start"*/ , 0, 0, NULL, &MENU_NotYet , StartDemoCleaning , MenuItemDrawName, NULL, 0 } ,
		{ 200 , 144, 0, 0, 0, LT_MENU_DemoCleaning2 /*"Output Name  "*/, 0, 0, &DemoGameName, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ 200 , 160, 0, 0, 0, LT_MENU_DemoCleaning3 /*"Choose From..."*/, 0, 0, &DemoList, NULL , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

#endif

MENU	MENU_SetUpBiker = {
	LT_MENU_SetUpBiker0 /*"Set Up Biker"*/ , InitBikerMenu, ExitBikerMenu, NULL, 0,
	{
		{ 200, 128, 0, 0, 0, LT_MENU_SetUpBiker1 /*"Create New Biker  "*/, 0, 0, &PilotNameInGame, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ 200, 144, 0, 0, 0, LT_MENU_SetUpBiker2 /*"Bike Controls"*/, 0, 0, NULL, &MENU_Controls, MenuChange, MenuItemDrawName, NULL, 0 },
		{ 200, 160, 0, 0, 0, LT_MENU_SetUpBiker3 /*"Select Bike"*/, 0, 0, NULL, &MENU_SelectBike, MenuChange, MenuItemDrawName, NULL, 0 },
		{ 200, 176, 0, 0, 0, LT_MENU_SetUpBiker4 /*"Select Bike Computer"*/, 0, 0, NULL, &MENU_SelectBikeComputer, MenuChange, MenuItemDrawName, NULL, 0 },
		{ 200, 192, 0, 0, 0, LT_MENU_SetUpBiker5 /*"Choose From..."*/, 0, 0, &PilotList, SelectPilot , SelectList , DrawList, NULL, 0 } ,
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};


MENU	MENU_Save = { LT_MENU_Save0 /*"Save Menu"*/ , NULL , NULL , NULL, 0,
					{ {	200 , 176, 0, 0, 0, LT_MENU_Save1 /*"Save Pickups"*/ , 0, 0, NULL, NULL , SavePickups , MenuItemDrawName, NULL, 0 } ,
					  {	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } };

MENU	MENU_Start = { "Forsaken" , InitStartMenu , NULL , NULL, 0,

					{ OLDMENUITEM( 200 , 112,  "Set Up Biker  ", (void *)biker_name, &MENU_SetUpBiker, MenuChange, DrawNameVar ),
					  OLDMENUITEM( 200 , 128, LT_MENU_InGame2,/*Toggle fullscreen*/ NULL, NULL , MenuGoFullScreen , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 144, "Single Player", NULL, &MENU_SinglePlayer , MenuChange , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 160, "Multi Player", NULL, &MENU_HostOrJoin , MenuChange , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 176, "Load Game", NULL, &MENU_NotYet , MenuChange , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 192, "Save Game", NULL, &MENU_NotYet , MenuChange , MenuItemDrawName ),
#ifdef DEMO_SUPPORT
					  OLDMENUITEM( 200 , 208, "Play Demo", NULL, &MENU_DemoPlayBack , MenuChange , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 224, "Clean Demo", NULL, &MENU_DemoCleaning , MenuChange , MenuItemDrawName ),
#endif
					  OLDMENUITEM( 200 , 240, "Options", NULL, &MENU_Options , MenuChange , MenuItemDrawName ),
					  OLDMENUITEM( 200 , 256, "Quit", NULL, NULL, SelectQuit , MenuItemDrawName ),
					{ 200 , 272, 0, 0, 0, "New Menu System" , 0, 0, NULL, &MENU_NEW_Start, MenuChange , MenuItemDrawName, NULL, 0, 0 } ,

					  {	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } };

MENU	MENU_Host_Options = { "HOST OPTIONS FOR THIS GAME" , InitHostMenu , NULL , NULL,	0,
			{
					OLDMENUITEM( 200, 112, LT_MENU_InGame27		/*"collision perspective"		*/,	&ColPerspective,				NULL,								SelectToggle,	DrawColToggle),
					OLDMENUITEM( 200, 128, LT_MENU_InGame36		/*"short packets"				*/,	&UseShortPackets,			NULL,								SelectToggle,	DrawToggle),
					OLDMENUITEM( 200, 160, LT_MENU_Options5		/*"Packets Per Second"		*/,	(void*)&PacketsSlider,		NULL,								SelectSlider,	DrawSlider),
					OLDMENUITEM( 200, 176, LT_MENU_InGame6		/*"Level Select"				*/,	NULL,								&MENU_LevelSelect,			MenuChange,	MenuItemDrawName),
					OLDMENUITEM( 200, 192, LT_MENU_RemovePlayer	/*"remove player"				*/,	&HostPlayersList,				HostListPlayerSelected,		SelectList,		DrawList ),  

			{	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } 
};

MENU	MENU_Watch_Options = { LT_MENU_InGame24 /*"Watch Options"*/ , InitWatchList , NULL , NULL,	0,
			{
				OLDMENUITEM( 200, 112, LT_MENU_InGame24	/*"Watch Player"*/,	&HostPlayersList, WatchPlayerSelected, SelectList, DrawList ),  
			{	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } 
};

MENU	MENU_InGame = { LT_MENU_InGame0 /*"Forsaken"*/ , InitInGameMenu , ExitInGameMenu , NULL,	0,
			{
					  OLDMENUITEM( 200, 112, LT_MENU_InGame1  /*"Set Up Biker"				*/,	(void *)biker_name,			&MENU_SetUpBiker,		MenuChange,				DrawNameVar),
					  OLDMENUITEM( 200, 128, LT_MENU_InGame2  /*"Toggle Full Screen"		*/,	NULL,						NULL,					MenuGoFullScreen,		MenuItemDrawName),
					  OLDMENUITEM( 200, 144, LT_MENU_InGame3  /*"Load Game"					*/,	NULL,						&MENU_LoadSavedGame,	MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 160, LT_MENU_InGame4  /*"Save Game"					*/,	NULL,						&MENU_SaveGame,			MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 176, LT_MENU_InGame5  /*"Options"					*/,	NULL,						&MENU_Options,			MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 192, LT_MENU_InGame26 /*"Host Options"				*/,	NULL,						&MENU_Host_Options,		MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 224, LT_MENU_InGame8  /*"Quit to Main Menu"			*/,	NULL,						NULL,					SelectQuitCurrentGame,	MenuItemDrawName),
					  OLDMENUITEM( 200, 240, LT_MENU_InGame25 /*"Quit to desktop"			*/,	NULL,						NULL,					SelectQuit,				MenuItemDrawName),
				
 					  OLDMENUITEM( 200, 256, LT_MENU_InGame24 /*"Watch Player"*/, NULL, &MENU_Watch_Options, MenuChange, MenuItemDrawName),
#ifndef EXTERNAL_DEMO
#ifdef DEBUG_ON
					  OLDMENUITEM( 200, 272, LT_MENU_InGame11 /*"Debugging"					*/,	&DebugInfo,					DebugModeChanged,		SelectToggle,			DrawToggle),
#endif
#endif
					  OLDMENUITEM( 200, 304, LT_MENU_InGame14 /*"Save Menu"					*/,	NULL,						&MENU_Save,				MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 320, LT_MENU_InGame15 /*"Debug Menu"				*/,	NULL,						&MENU_DebugMode,		MenuChange,				MenuItemDrawName),
					
					  
					  {	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } 
};


MENU	MENU_InGameSingle = { LT_MENU_InGame0 /*"Forsaken"*/ , InitInGameMenu , ExitInGameMenu , NULL,	0,
			{
					  OLDMENUITEM( 200, 112, LT_MENU_InGame1  /*"Set Up Biker"				*/,	(void *)biker_name,			&MENU_SetUpBiker,		MenuChange,				DrawNameVar),
					  OLDMENUITEM( 200, 128, LT_MENU_InGame2  /*"Toggle Full Screen"		*/,	NULL,						NULL,					MenuGoFullScreen,		MenuItemDrawName),
					  OLDMENUITEM( 200, 144, LT_MENU_InGame3  /*"Load Game"					*/,	NULL,						&MENU_LoadSavedGame,	MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 160, LT_MENU_InGame4  /*"Save Game"					*/,	NULL,						&MENU_SaveGame,			MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 176, LT_MENU_InGame5  /*"Options"					*/,	NULL,						&MENU_Options,			MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 224, LT_MENU_InGame8  /*"Quit to Main Menu"			*/,	NULL,						NULL,					SelectQuitCurrentGame,	MenuItemDrawName),
					  OLDMENUITEM( 200, 240, LT_MENU_InGame25 /*"Quit to desktop"			*/,	NULL,						NULL,					SelectQuit,				MenuItemDrawName),
#ifndef EXTERNAL_DEMO
#ifdef DEBUG_ON
					  OLDMENUITEM( 200, 272, LT_MENU_InGame11 /*"Debugging"					*/,	&DebugInfo,					DebugModeChanged,		SelectToggle,			DrawToggle),
#endif
#endif
					  OLDMENUITEM( 200, 304, LT_MENU_InGame14 /*"Save Menu"					*/,	NULL,						&MENU_Save,				MenuChange,				MenuItemDrawName),
					  OLDMENUITEM( 200, 320, LT_MENU_InGame15 /*"Debug Menu"				*/,	NULL,						&MENU_DebugMode,		MenuChange,				MenuItemDrawName),
					  					  
					  {	-1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 } } 
};

MENU	MENU_ForceAbort = {
	"Sorry Directplay has forced you to quit due to 1 of its bugs" , NULL , NULL , NULL, 0,
	{
		 OLDMENUITEM(200, 112, "Restart",	NULL,	&MENU_Start,	MenuChange,		MenuItemDrawName),
		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};



MENU	MENU_GotoRoom = {
	"Go to Specific Room" , InitDebugMenu, NULL, NULL, 0,
	{
		{ 200, 128, 0, 0, 0, "Enter Room Name  ", 0, 0, &RoomName, NULL,  SelectText, DrawTextItem, NULL, 0 } ,
		{ 200, 144, 0, 0, 0, "Choose Room From...", 0, 0, &RoomList, ChooseRoom , SelectList , DrawList, NULL, 0 } ,

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_DebugHelp =
{
	"Debug Help", NULL, NULL, NULL, 0,
	{
		HELPKEY( 200, 112, "S+F1",	"Toggle debug help screen" ),
		HELPKEY( 200, 128, "S+F2",	"Increase FOV" ),
		HELPKEY( 200, 144, "S+F3",	"Decrease FOV" ),
		HELPKEY( 200, 160, "C+S+F2","Reset FOV" ),
		HELPKEY( 200, 176, "S+F4",	"Go to specific room" ),
		HELPKEY( 200, 192, "S+F5",	"Go to next start position" ),
		HELPKEY( 200, 208, "S+F6",	"toggle object clipping" ),
		HELPKEY( 200, 224, "S+F7",	"show bounding boxes" ),
		HELPKEY( 200, 240, "S+F8",	"show external forces" ),
		HELPKEY( 200, 256, "S+F9",	"Cycle portal display" ),
		HELPKEY( 200, 272, "S+F10",	"show trigger zones" ),
		HELPKEY( 200, 288, "S+F11",	"Cycle collision skin" ),
		HELPKEY( 200, 304, "S+F12",	"Toggle collision detection" ),

		HELPKEY( 200, 320, "C+F1",	"show teleports" ),
		HELPKEY( 200, 336, "S+F2",	"show trigger zones" ),

		{ 200, 352, 0, 0, 0, "Return to game", 0, 0, NULL, NULL, MenuItemBack, MenuItemDrawName, NULL, 0 },

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

MENU	MENU_Help =
{
	LT_MENU_Help0/*"In-Game Help"*/, NULL, NULL, NULL, 0,
	{							 
		HELPKEY( 200, 112, "ESC",LT_MENU_Help1	/*"Toggle menu"*/ ),
		HELPKEY( 200, 128, "F1", LT_MENU_Help2	/*"Toggle help screen" */),
		HELPKEY( 200, 144, "F2", LT_MENU_Help3	/*"Increase window size" */),
		HELPKEY( 200, 160, "F3", LT_MENU_Help4	/*"Decrease window size" */),
		HELPKEY( 200, 176, "F4", LT_MENU_Help5	/*"Toggle panel text" */),
		HELPKEY( 200, 192, "F5", LT_MENU_Help6	/*"Toggle remote camera"*/ ),
		HELPKEY( 200, 208, "F6", LT_MENU_Help7	/*"Toggle rear camera" */),
		HELPKEY( 200, 224, "F7", LT_MENU_Help8	/*"Toggle missile camera"*/ ),
		HELPKEY( 200, 240, "F8", LT_MENU_Help9	/*"Toggle player names" */),

		{ 200, 272, 0, 0, 0, LT_MENU_Help10/*"Return to game"*/, 0, 0, NULL, NULL, MenuItemBack, MenuItemDrawName, NULL, 0 },

		{ -1 , -1, 0, 0, 0, "" , 0, 0, NULL, NULL , NULL , NULL, NULL, 0 }
	}
};

char *BikerText[MAXBIKETYPES] =
{
	
	//BORG
	LT_BorgBikerText,

	//BEARD
	LT_BeardBikerText,

	//LAJAY
	LT_LAJayBikerText,

	//EXCOP
	LT_ExCopBikerText,

	//TRUCKER
	LT_TruckerBikerText,

	//FOETOED
	LT_FoetoedBikerText,

	//JAPG
	LT_JapBirdBikerText,

	//NUTTA
	LT_NuttaBikerText,

	//SEPTRE
	LT_SceptreBikerText,
		
	//JO
	LT_JoBikerText,

	//SHARK - CUVEL CLARK
	LT_SharkBikerText,

	//HK-5
	LT_HK5BikerText,

	//NUBIA
	LT_NubiaBikerText,

	//MEPHISTOFUN
	LT_MophistoBikerText,

	//CERBERO
	LT_CerberoBikerText,

	//SLEEK - EARL SLEEK
	LT_SleekBikerText,

	//FLYGIRL
	LT_FlygirlBikerText,
};


/*===================================================================
		Globals ...
===================================================================*/

#define MAXHIGHLIGHTITEMS 8
#define HIGHLIGHT_Pulsing	0
#define HIGHLIGHT_Static	1

MENU * CurrentMenu = NULL;
MENUITEM * CurrentMenuItem = NULL;
TEXTINFO * HighlightItem[MAXHIGHLIGHTITEMS];
int HighlightStatus[MAXHIGHLIGHTITEMS];
int CurrentHighlightItem;

MENU *MenuStack[ MAXMENULEVELS ];
MENUITEM *MenuItemStack[ MAXMENULEVELS ];
TEXTINFO *TextStack[ MAXTEXTITEMS ];
TEXTINFO *BikeNameTextPtr;
int MenuStackLevel = 0;
int TextStackLevel = 0;
bool ForceConfigSave = false;

bool TitleOnceOnly = true;

VECTOR View, Look, Up, PanFrom, PanTo, PanFrom2, PanTo2, VDULookPos, DiscLookPos, DiscViewPos, VDUViewPos, StartLookPos, StartViewPos;
float CurrentFOV;
float CurrentCameraPanTheta, CurrentCameraPanTheta2;
VECTOR OrigPos[NUMOFTITLEMODELS];

float rot[6], rotspeed[6], RotPause[6], StackY[7];

#define ROTATE_DISC_All 0
#define ROTATE_DISC_DifficultyForward 1
#define ROTATE_DISC_DifficultyBack 2

int DiscStatus, StackStatus, StackMode, RotWiggle[6], CurrentTextures[6], CurrentDiscHighlight, CurrentVDUHighlight, CameraStatus, DifficultyDir;

#define Judders 7

float JudderAngle[Judders] = {180.0F, 182.0F, 178.0F, 181.0F, 179.0F, 180.0F, 180.0F};

bool Turned[6], LastTurned[6], RotateSetup, DoHighlight, 
	 OnRotateVDU, OnVDUflip, RotVDU, PanDone, VDU_Active;


MXLOADHEADER ModelHeaders[MAXMODELHEADERS];	//as defined in models.c

POLYANIM *PolyAnim[6];
MXLOADHEADER * Mxloadheader[6];

u_int16_t fmpoly[2];
u_int16_t scrpoly[MAXVDULINES][MAXVDULINELENGTH];
u_int16_t bikerfmpoly;
u_int16_t BikerScrPoly;
u_int16_t LevelScrPoly;
int CurrentPos[MAXVDULINELENGTH];

typedef struct 
{
	VECTOR	Pos;
	VECTOR	Look;
	VECTOR	View;
	float	StartRot;
	float	EndRot;
	VECTOR BracketPos;
	float BracketStartRot;
	float BracketEndRot;
}VDUINFO;

#define Left	true
#define	Right	false

VECTOR CurrentVDUPos;
float VDUrot;

VDUINFO	LeftVDU, RightVDU;

//offsets for biker character
VECTOR CharOffset = {-670.0F, 40.0F, 345.0F};

int KeyDefLines[MAXVDULINES];

int CurrentToggle, CurrentTextItem, NoOfLists, CurrentKeyDef;

bool VduProcessed, LineExists;
bool AutoSelectConnection = false;
bool InitialTexturesSet;
bool LevelDisplayed;
bool BikerDisplayed;
int BikerSet;

bool DoNotRotateText;
int BikeChars[MAXBIKETYPES] = {0,7,11,10,6,15,14,12,4,8,2,9,13,5,1, 3, -1};
u_int16_t BikeModelLookup[MAXBIKETYPES] = {TITLE_MODEL_Lokasenna, TITLE_MODEL_Beard, TITLE_MODEL_LAJay, TITLE_MODEL_ExCop, TITLE_MODEL_RexHardy,
								TITLE_MODEL_Foetoid, TITLE_MODEL_NimSoo, TITLE_MODEL_Nutta, TITLE_MODEL_Sceptre, TITLE_MODEL_Jo,
								TITLE_MODEL_Shark, TITLE_MODEL_HK5, TITLE_MODEL_Nubia, TITLE_MODEL_Mofisto, TITLE_MODEL_Cerbero, TITLE_MODEL_Slick, TITLE_MODEL_ExtraModels};
float BikeModelScale[MAXBIKETYPES] = {0.73F, 0.8F, 0.85F, 0.74F, 0.6F, 0.61F, 0.71F, 0.65F, 0.7F, 0.64F, 0.57F, 0.66F, 0.84F, 0.66F, 0.66F, 0.66F, 1.0F};

float HoloModelScale;

u_int16_t InterLevelModelLookup[MAXBIKETYPES] = { TITLE_MODEL_Crystal, TITLE_MODEL_GoldBar };
float InterLevelModelScale[MAXBIKETYPES] = {0.41F, 0.6F };

float *HoloScaleLookupTable;
u_int16_t *HoloModelLookup;

#define NOT_ROTATED false
#define ROTATED true

float	DiscExpand;
float	DiscSpeed; 
float	DiscWiggleDistance;
bool	DiscExpandOut;
float	DiscPauseTime;
bool	DiscWiggleUp;
float WipeTime;
float VduDisplayTime = 0.5F;
float HoloRotx = 0.0F;
float HoloRoty = 0.0F;
float HoloRotz = 0.0F;

#define	WipeOut	true
#define	WipeIn	false
bool WipeStatus;

VECTOR Holopad = {-486.0F, -86.0F, 497.0F};
float HolopadRange = 150.0F;
int testvert;

EXCLUDEDVERTICES MenuTVExcluded = {0, 0, 4, {30, 31, 32, 33}};
EXCLUDEDVERTICES MenuTVDummyExcluded = {0, 0, 4, {36, 37, 38, 39}};
EXCLUDEDVERTICES RoomExcluded = {0, 0, 8, {457, 458, 459, 460, 461, 462, 463, 464}};
EXCLUDEDVERTICES VduScreenExcluded = {0, 1, EXCLUDE_ALL, {0}};
EXCLUDEDVERTICES VduFrameExcluded = {0, 0, EXCLUDE_ALL, {0}};
float rtintamount, gtintamount, btintamount, atintamount1, atintamount2, atintamount3, atintamount4, tinttheta;

//int HoloMorphVerts[24] = {8, 9, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54};
//int HoloMorphVerts[24] = {40, 9, 18, 14, 48, 16, 34, 8, 20, 44, 24, 42, 26, 50, 12, 28, 30, 54, 32, 38, 46, 36, 52, 22 };
int HoloMorphVerts[24] = {9, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52};
int HoloTintVerts[24] = {10, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55};
/*float FlatXOffset = 176.0F;
float FlatYOffset = 44.0F;
float FlatXScale = 0.75F;
float FlatYScale = 0.62F;
float FlatTextScale = 0.79F;*/

float FlatXOffset = 0.0F;
float FlatYOffset = 0.0F;
float FlatXScale = 1.0F;
float FlatYScale = 1.0F;
float FlatTextScale = 1.0F;

float BikeAlpha = 1.0F;
float BikeCol = 0.5F;

/***************
new text globals
***************/
#define VDU_Width 200

TEXTINFO VDUTextItems[MAXTEXTITEMS];
int Current2dBox;
bool DrawTextItemBox;
float VDUoffsetX = 105.0F;
float VDUoffsetY = 15.0F;
u_int16_t screenpoly[MAXNUMOFSCRPOLYS];
int CurrentScreenPoly;
float	TEXTINFO_currentx;					
float	TEXTINFO_currenty;					
float	TEXTINFO_currentheight;				
float	TEXTINFO_TextSpace;				
float	TEXTINFO_LineSpace;				
int		TEXTINFO_currentstartpoint;			
int		TEXTINFO_currentendpoint;
int CurrentTextItem;
u_int16_t testboxpoly;	
u_int16_t TextLookup[NUMSCREENFONTS][256];		
int character = 0;
int CurrentTeletype;
MENUITEM *LastMenuItem;
bool ScreenPolyCursorActive;
u_int16_t CursorScreenPoly;
bool NewCursorActive;
bool ForceNewLine;

/***************
holo-bike stuff
***************/
int SelectedBikeFrame;
u_int16_t SelectedBikeModel;
u_int16_t OldBikeModel;
float BikeRot = 0.0F;
u_int16_t BikeLine[MAXNUMOF2DPOLYS];
u_int16_t CurrentHoloModel = (u_int16_t)-1;
bool LinesActive;
int CurrentLine;
VECTOR BikePos;
SCANLINES scanline[MAXNUMOF2DPOLYS];
bool KnowExtremeVerts, BikeLoaded;
float CurrentOffset;
bool BikeOnSwap, OnBikeShrink, LoadNewBike;
bool LightHolopad;

bool BikeExpanded = false;

/**************
new event globals
***************/
VECTOR OldLookPos;
VECTOR OldViewPos;
float OldVDUrot;
float RoomDarkness, OldRoomDarkness;

bool VDU_Ready;

float HoloLightBrightness, OldHoloLightBrightness;

float OldHoloPadHeight;
float StackRot;

VECTOR stackpos[8];

bool StackDown;
float MinStackTheta = 0.0F;
float MaxStackTheta = 170.0F;

int		StackJudderNum;
float	StackRotAmount;
float	OldStackRotAmount;

float BikeSwapScale, OldBikeSwapScale;

float SelectedBikeScale, OldSelectedBikeScale;
int	OldSelectedBike;
float	OldBikeCharAlpha, BikeCharAlpha, LevelPicAlpha;
bool HolopadJudderReq;

float BracketRot, OldBracketRot;

float TeleTypeTimer;
int TeleTypeSkip;
bool NewTextCharDisplayed = false;

bool ListRedrawReq;

int OldMenuStatus;

// customisable display messages
char *ToggleColourRed = LT_RED;
char *ToggleColourBlue = LT_BLUE;
char *ToggleColourDisabled = LT_DISABLED;


char *ToggleStatusOn = LT_ToggleStatusOn;
char *ToggleStatusOff = LT_ToggleStatusOff;
char *ToggleStatusShooter = LT_ToggleStatusShooter;	// collision perspective
char *ToggleStatusTarget = LT_ToggleStatusTarget;
char *RadioBtnSelected = "*";
char *RadioBtnNotSelected = "";
char *EmptyString = "";
char *UndefinedKey = LT_Undef;
char *WeaponSelectFirst = "Use arrow keys to select weapon to move then press enter";
char *WeaponSelectSecond = "use right arrow to place item in unused list~"
					"use left arrow to place item in used list~"
					"or use up/down arrows to change position, then press enter to place";
char *WaitingForConfirm = LT_SelectWhenReady;
char *WaitingForReset = LT_WaitingForReset;
int MAXSLIDERINFOLENGTH = 32;
/**********************
new slider globals
***********************/
#define MAXSLIDERSPERMENU 8
#define NUMPOLYSPERSLIDER 1
#define SLIDERSPEED 1.0F
#define SLIDER_RefreshNeeded true
#define SLIDER_RefreshNotNeeded false

typedef struct
{
	MENUITEM *Item;
	bool	needs_refresh;
}ACTIVESLIDERINFO;

ACTIVESLIDERINFO ActiveSliderItem[MAXSLIDERSPERMENU];
u_int16_t SliderPoly[MAXSLIDERSPERMENU][NUMPOLYSPERSLIDER];
/**********************
highlight box globals
***********************/
#define MAXEDGEPOLYS 32

typedef struct
{
	float xmin;
	float xmax;
}HIGHLIGHTPOLYINFO;

HIGHLIGHTPOLYINFO HighlightPolyList[MAXEDGEPOLYS];
int HighlightPolyListStart;
int HighlightPolyListEnd;
#define HIGHLIGHT_SPEED			0.5F	// lowest res pixels per frame
#define HIGHLIGHT_POLY_WIDTH	10	// lowest res pixels
bool HighlightListEmpty;
bool TestPoly = false;
bool WasteAFrame = false;

/********************
weapon priority
*********************/
extern bool PrimaryValid[];
extern bool SecondaryValid[];
int WeaponList[ MAX( MAX_PRIMARY_WEAPONS, MAX_SECONDARY_WEAPONS ) ];
int16_t SelectedWeapon;
u_int16_t Num_Unused_Weapons;
MENUITEM *WeaponItem;
#define WEAPON_ChoosingFirst	false
#define WEAPON_ChoosingSecond	true
bool WeaponOrderStatus;
u_int16_t MaxWeapons;
int16_t *PriorityList;
int16_t *OrderList;
char **DescriptionList;
TEXTINFO *IdiotText;
char *IdiotTextPtr;
bool *WeaponValid;

/*******************
text editing
********************/
float CursorTimer = 0.0F;
#define CursorInterval	1.0F


bool RescanLists = true;
int oldlistitems;

bool TitleInitDone;
bool HostQuitting = false;

/*******************
Team Select
*******************/
int TeamSelectStatus;
LIST *SourceTeamList;

/*********************
Multi-player messaging
*********************/
#define MAX_TITLE_MSGS 16
#define MAX_TITLE_MSG_SIZE 128
#define	MAX_TITLE_MSG_TIME 100.0F
typedef struct
{
	char text[MAX_TITLE_MSG_SIZE];
	float timer;
}TITLEMSG;
TITLEMSG TitleMsgQue[MAX_TITLE_MSGS];
int		TitleMsgQueStart, TitleMsgQueEnd, TotalTitleMsgs;
int TestMessage = 0;
float TitleMessageTimer;
int MessagesDisplayed;

#define MAX_TIMERS 16

/**************
event stuff
***************/
TITLE_EVENT Title_Events[MAXTITLEEVENTS] = {

	{InitEventDiscPanFromRightInit, EventDiscPanInit, ExitEventDiscPanInit,	TITLE_TIMER_PanToDiscsInit, 0.0F, 0.8F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLowerStack, EventLowerStack, ExitEventLowerStack,			TITLE_TIMER_PanToDiscsInit, 0.2F, 0.8F, TITLE_EVENT_STATUS_IDLE},
	{InitEventJudderStack, EventJudderStack, NULL,			TITLE_TIMER_PanToDiscsInit, 0.8F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLightenRoom, EventLightenRoom, NULL,			TITLE_TIMER_PanToDiscsInit, 0.0F, 0.8F, TITLE_EVENT_STATUS_IDLE},

	{InitEventDiscPan, EventDiscPan, ExitEventDiscPanFromRight,				TITLE_TIMER_PanToDiscsFromRight, 0.3F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateInRightVDU, EventRotateInRightVDU, NULL,				TITLE_TIMER_PanToDiscsFromRight, 0.2F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLightenRoom, EventLightenRoom, NULL,							TITLE_TIMER_PanToDiscsFromRight, 0.3F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventFadeOffHoloLight, EventFadeOffHoloLight, ExitEventFadeOffHoloLight,				TITLE_TIMER_PanToDiscsFromRight, 0.0F, 0.2F, TITLE_EVENT_STATUS_IDLE},
	{InitEventShrinkBike, NULL, NULL,				TITLE_TIMER_PanToDiscsFromRight, 0.0F, 0.2F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLowerHoloPad, EventLowerHoloPad, NULL,						TITLE_TIMER_PanToDiscsFromRight, 0.2F, 0.5F, TITLE_EVENT_STATUS_IDLE},
	
	{InitEventDiscPan, EventDiscPan, NULL,	TITLE_TIMER_PanToDiscsFromLeft, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateInLeftVDU, EventRotateInLeftVDU, NULL,		TITLE_TIMER_PanToDiscsFromLeft, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLightenRoom, EventLightenRoom, NULL,				TITLE_TIMER_PanToDiscsFromLeft, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	
	{InitEventRightVDUPan, EventRightVDUPan, ExitEventVDUPan,	TITLE_TIMER_PanToRightVDU, 0.0F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateOutRightVDU, EventRotateOutRightVDU, ExitEventRotateOutVDU,	TITLE_TIMER_PanToRightVDU, 0.3F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventDarkenRoom, EventDarkenRoom, NULL,				TITLE_TIMER_PanToRightVDU, 0.0F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventFadeOnHoloLight, EventFadeOnHoloLight, NULL,		TITLE_TIMER_PanToRightVDU, 0.8F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRaiseHoloPad, EventRaiseHoloPad, ExitEventRaiseHolopad,			TITLE_TIMER_PanToRightVDU, 0.3F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventJudderHoloPad, EventJudderHoloPad, ExitEventJudderHoloPad,			TITLE_TIMER_PanToRightVDU, 0.6F, 0.8F, TITLE_EVENT_STATUS_IDLE},
	{InitEventExpandBike, NULL, NULL, TITLE_TIMER_PanToRightVDU, 0.8F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{InitEventRightVDUPan, EventRightVDUPan, ExitEventVDUPan,	TITLE_TIMER_InterLevelPan, 0.0F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateOutRightVDU, EventRotateOutRightVDU, ExitEventRotateOutVDU,	TITLE_TIMER_InterLevelPan, 0.3F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventDarkenRoom, EventDarkenRoom, NULL,				TITLE_TIMER_InterLevelPan, 0.0F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventFadeOnHoloLight, EventFadeOnHoloLight, NULL,		TITLE_TIMER_InterLevelPan, 0.8F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRaiseHoloPad, EventRaiseHoloPad, ExitEventRaiseHolopad,			TITLE_TIMER_InterLevelPan, 0.3F, 0.6F, TITLE_EVENT_STATUS_IDLE},
	{InitEventJudderHoloPad, EventJudderHoloPad, ExitEventJudderHoloPad,			TITLE_TIMER_InterLevelPan, 0.6F, 0.8F, TITLE_EVENT_STATUS_IDLE},
	{InitEventExpandCrystal, NULL, NULL, TITLE_TIMER_InterLevelPan, 0.8F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	
	{InitEventLeftVDUPan, EventLeftVDUPan, ExitEventVDUPan,		TITLE_TIMER_PanToLeftVDU, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateOutLeftVDU, EventRotateOutLeftVDU, ExitEventRotateOutVDU,	TITLE_TIMER_PanToLeftVDU, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventDarkenRoom, EventDarkenRoom, NULL,				TITLE_TIMER_PanToLeftVDU, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	
	{InitEventFadeOffHoloLight, EventFadeOffHoloLight, ExitEventFadeOffHoloLight,				TITLE_TIMER_SwapBikes, 0.0F, 0.25F, TITLE_EVENT_STATUS_IDLE},
	{InitEventFadeOnHoloLight, EventFadeOnHoloLight, NULL,					TITLE_TIMER_SwapBikes, 0.25F, 0.5F, TITLE_EVENT_STATUS_IDLE},
	{TriggerBikeSpeech, NULL, NULL,					TITLE_TIMER_SwapBikes, 1.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{InitEventRightVDUPan, EventRightVDUPan, ExitEventVDUPan,	TITLE_TIMER_Visuals, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateOutRightVDU, EventRotateOutRightVDU, ExitEventRotateOutVDU,	TITLE_TIMER_Visuals, 0.5F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventDarkenRoom, EventDarkenRoom, NULL,				TITLE_TIMER_Visuals, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{InitEventDiscPan, EventDiscPan, ExitEventDiscPanFromRight,				TITLE_TIMER_PanToDiscsFromVisuals, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateInRightVDU, EventRotateInRightVDU, NULL,				TITLE_TIMER_PanToDiscsFromVisuals, 0.0F, 0.5F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLightenRoom, EventLightenRoom, NULL,							TITLE_TIMER_PanToDiscsFromVisuals, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{InitEventRightVDUPan, EventRightVDUPan, ExitEventVDUPan,	TITLE_TIMER_NormalPanToRightVDU, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateOutRightVDU, EventRotateOutRightVDU, ExitEventRotateOutVDU,	TITLE_TIMER_NormalPanToRightVDU, 0.5F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventDarkenRoom, EventDarkenRoom, NULL,				TITLE_TIMER_NormalPanToRightVDU, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	
	{InitEventDiscPan, EventDiscPan, ExitEventDiscPanFromRight,				TITLE_TIMER_NormalPanToDiscsFromRight, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},
	{InitEventRotateInRightVDU, EventRotateInRightVDU, NULL,				TITLE_TIMER_NormalPanToDiscsFromRight, 0.0F, 0.5F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLightenRoom, EventLightenRoom, NULL,							TITLE_TIMER_NormalPanToDiscsFromRight, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{InitEventFadeOffHoloLight, EventFadeOffHoloLight, ExitEventFadeOffHoloLight,				TITLE_TIMER_LowerHolopad, 0.0F, 0.4F, TITLE_EVENT_STATUS_IDLE},
	{InitEventShrinkBike, NULL, NULL,				TITLE_TIMER_LowerHolopad, 0.0F, 0.4F, TITLE_EVENT_STATUS_IDLE},
	{InitEventLowerHoloPad, EventLowerHoloPad, NULL,						TITLE_TIMER_LowerHolopad, 0.4F, 1.0F, TITLE_EVENT_STATUS_IDLE},

//	{InitEventFadeOnHoloLight, EventFadeOnHoloLight, NULL,					TITLE_TIMER_ExpandObject, 0.0F, 1.0F, TITLE_EVENT_STATUS_IDLE},

	{NULL, NULL, NULL, -1, 0.0F, 0.0F, TITLE_EVENT_STATUS_IDLE},
};

TITLE_EVENT_TIMER Title_Timers[MAXTITLETIMERS] = {

	{0.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, NULL},		//NULL timer
	{1.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitStartDiscPanTimer},		//pan to discs initially
	{1.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitRightDiscPanTimer},		//pan to discs from right
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitLeftDiscPanTimer},		//pan to discs from left
	{1.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitRightVDUPanTimer},		//pan to right VDU
	{1.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitRightVDUPanTimer},		//inter level pan
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitLeftVDUPanTimer},		//pan to left VDU
	{2.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, NULL},		//swap bikes
	{0.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, ChooseDiscTimer},		//disc choose dummy timer
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitVisualsTimer},		//disc choose dummy timer
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, ChooseDiscTimer},		//disc choose dummy timer
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitRightVDUPanTimer},		//normal pan right
	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitRightDiscPanTimer},		//normal disc pan from right
//	{1.0F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitGeneralTimer},		//swap inter level objects
	{0.75F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitGeneralTimer},		//lower holopad
	{0.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitGeneralTimer},		// shrink object
	{0.5F,	0.0F, TITLE_EVENT_TIMER_IDLE, InitGeneralTimer},		// expand object
	{-1.0F, -1.0F, TITLE_EVENT_TIMER_IDLE, NULL}
};

extern bool InitView( void );
extern bool render_mode_select( render_info_t * info );
bool RenderModeSelect( int mode, bool fullscreen, bool vsync )
{
	render_info_t old_info = render_info;
	// this happens if Msg() or something else is called during resize etc...
	if(!render_info.Mode)
		return true;
	render_info.default_mode.h    = render_info.Mode[mode].h;
	render_info.default_mode.w    = render_info.Mode[mode].w;
#ifdef PANDORA
	render_info.fullscreen        = true;
#else
	render_info.fullscreen        = fullscreen;
#endif
	render_info.vsync             = vsync;
DebugPrintf("2 vsync = %d\n",render_info.vsync);
	if(!render_mode_select( &render_info ))
	{
		DebugPrintf("RenderModeSelect: failed to change mode\n");
		render_info = old_info;
		if(!render_mode_select( &render_info ))
		{
			DebugPrintf("RenderModeSelect: failed to change back\n");
			exit(1);
		}
	}
	if (!InitView())
	{
		Msg("InitView failed.\n");
		return false;
	}

	// set default font scaling by resolution
	TextScaleSlider.value = 1;
	if(render_info.default_mode.w < 1024)
			TextScaleSlider.value = 0;
	else if(render_info.default_mode.w >= 1280)
			TextScaleSlider.value = 2;
	SetTextScale(&TextScaleSlider);

	SetGamePrefs();
	return true;
}

extern bool render_reset( render_info_t * info );
bool RenderModeReset( void )
{
	if(!render_reset( &render_info ))
		return true;
	if (!InitView())
	{
	    Msg("InitView failed.\n");
        return false;
	}
	SetGamePrefs();
	return true;
}

/*===================================================================
	Procedure	:		Init Title load in all graphics etc for Titles..
	Output		:		bool true/false
===================================================================*/
extern void input_grab( bool clip );
bool InitTitle()
{
	framelag = 0;

	if( ! render_info.fullscreen )
		input_grab( false );
    
	InitModeCase();

    memset(&Names, 0, sizeof(SHORTNAMETYPE) );

	InitFont();

	return true;
}

/*===================================================================
	Procedure	:		Release Tile Screen Graphics etc...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void
ReleaseTitle(void)
{
 	int i;
	DebugPrintf("ReleaseTitle\n");
	for( i = 0; i < NUM_TITLE_LOOPS; i++ )
	{
		StopSfx( TitleLoopId[ i ] );
		TitleLoopId[ i ] = 0;
	}
}

/*===================================================================
	Procedure	:		Set up initail parameters for title room...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void SetInitialTitleParams()
{
	int i;

	/**********************************************************************************
	Positioning
	**********************************************************************************/
	
	// Position of point on VDU to home in on
	RightVDU.Look.x = -398.0F; RightVDU.Look.y = 58.0F; RightVDU.Look.z = -7.0F;
	RightVDU.View.x = -640.0F; RightVDU.View.y = 58.0F; RightVDU.View.z = 614.0F;
	RightVDU.Pos.x = -755.0F; RightVDU.Pos.y = -381.0F; RightVDU.Pos.z = 331.0F;
	RightVDU.StartRot = -43.0F;
	RightVDU.EndRot = 21.3F;
	RightVDU.BracketPos.x = -746.0F; RightVDU.BracketPos.y = -383; RightVDU.BracketPos.z = 335;
	RightVDU.BracketStartRot = -41.3F;
	RightVDU.BracketEndRot = 23.0F;

	LeftVDU.Look.x = 278.0F; LeftVDU.Look.y = 91.0F; LeftVDU.Look.z = 0.0F;
	LeftVDU.View.x = 453.0F; LeftVDU.View.y = 91.0F; LeftVDU.View.z = 401.0F;
	LeftVDU.Pos.x = 446.0F; LeftVDU.Pos.y = -348.0F; LeftVDU.Pos.z = 164.0F;
	LeftVDU.StartRot = 240.0F;
	LeftVDU.EndRot = 156.4F;
	LeftVDU.BracketPos.x = 440.0F; LeftVDU.BracketPos.y = -349.0F; LeftVDU.BracketPos.z = 165.0F;
	LeftVDU.BracketStartRot = 259.0F;
	LeftVDU.BracketEndRot = 175.0F;

				  
	//Position of point on discs to home in on
	DiscLookPos.x = 0.0F; DiscLookPos.y = 72.0F; DiscLookPos.z = 0.0F;

	// 'Home' position of camera
//	DiscViewPos.x = 131.0F; DiscViewPos.y = 121.0F; DiscViewPos.z = 142.0F;
	DiscViewPos.x = 131.0F; DiscViewPos.y = 121.0F; DiscViewPos.z = 200.0F;

	StartViewPos.x = 197.0F; StartViewPos.y = 97.0F; StartViewPos.z = 908.0F;
	StartLookPos.x = 57.0F; StartLookPos.y = 0.0F; StartLookPos.z = 0.0F;

	// Initial camera parameters
	View = StartViewPos;
	Look = StartLookPos;
	Up.x = 0.0F; Up.y = 1.0F; Up.z = 0.0F;
	CurrentFOV = 80.0F;
	

	/**********************************************************************************
	Disc Stack
	**********************************************************************************/
	DiscExpand = 3.0F;	// y distance that disc 1 will expand out by
	DiscSpeed = 0.2F; 	// multiplier for disc rotate fn
	DiscWiggleDistance = 4.0F;	// max y amount that discs wiggle by after expanding
	DiscExpandOut = true;	 
	DiscPauseTime = 0.0F;
	DiscWiggleUp = true;
	CurrentDiscHighlight = -1;
	RotateSetup = false;
	StackStatus = DISC_NOTHING;
	InitialTexturesSet = false;
					 
	for (i=0; i<6; i++)
		Turned[i] = true;  
					 

	/**********************************************************************************
	VDU Screen
	**********************************************************************************/
	CurrentVDUHighlight = -1;


	VduProcessed = false;


	/**********************************************************************************
	Viewpoint movement
	**********************************************************************************/
   	CameraStatus = CAMERA_AtStart;
	PanDone = false;


	/***********************
	new text stuff
	************************/
	Current2dBox = 0;
	DrawTextItemBox = false;	//draw the bounding box for VDU text items?
	CurrentScreenPoly = 0;
	CurrentTextItem = 0;
	CurrentTeletype = 0;
	LastMenuItem = CurrentMenuItem;
	ScreenPolyCursorActive = false;


	/***********************
	holo-bike stuff
	************************/
	LinesActive = false;
	BikeLoaded = false;
	CurrentOffset = 0.0F;
	BikeOnSwap = false;

	VDU_Ready = false;
	StackRot = 0.0F;

	StackDown = false;
	BikeSwapScale = 0.0F;

	OldSelectedBike = -1;

	HolopadJudderReq = true;
	ListRedrawReq = true;

	RoomDarkness = 1.0F;

	CurrentTextures[0] = 1;
	for (i = 1; i < 6; i++)
    	CurrentTextures[i] = 0;

	/***********************
	new slider stuff
	***********************/
	for (i=0; i<MAXSLIDERSPERMENU; i++)
		ActiveSliderItem[i].Item = NULL;


	/**********************
	new highlight stuff
	***********************/
	HighlightPolyListStart = 0;
	HighlightPolyListEnd = 0;
	HighlightListEmpty = true;

	ResetAllEvents();

	CurrentHighlightItem = 0;

	for (i=0; i < MAXHIGHLIGHTITEMS; i++)
		HighlightItem[ i ] = NULL;

	LevelDisplayed = false;
	BikerDisplayed = false;

	InitTitleMessaging();

	TitleMessageTimer = 0.0F;
	MessagesDisplayed = 0;

	// team game stuff....
	memset( &PlayerReady, false, sizeof(bool) * MAX_PLAYERS);
	memset( TitleMessage, 0, sizeof(char) * MAXTEXTMSG * TITLE_TEXT_MSGS_STORED);

}

/*===================================================================
	Procedure	:		LoadTitleModels
	Input		:		nothing...
	Output		:		bool true/false
===================================================================*/
bool LoadTitleModels(void)
{
	int i;
	u_int16_t CurrentModel;

   	// which set of models do we want to load?
	if ( MyGameStatus == STATUS_BetweenLevels )
	{
		TitleModelSet = InterLevelModelNames;
		HoloScaleLookupTable = InterLevelModelScale;
		HoloModelLookup = InterLevelModelLookup;
	}
	else
	{
		TitleModelSet = TitleModelNames;
		HoloScaleLookupTable = BikeModelScale;
		HoloModelLookup = BikeModelLookup;
	}

	NumTitleModelsToLoad = 15;

	for (i=0; i < NumTitleModelsToLoad; i++)
	{
		BackgroundModel[i] = FindFreeModel();
				
		if ( BackgroundModel[i] != (u_int16_t) -1)
		{	CurrentModel = BackgroundModel[i];
			Models[CurrentModel].Flags = MODFLAG_Original;
			Models[CurrentModel].Pos.x = 0.0F;
			Models[CurrentModel].Pos.y = 0.0F;
			Models[CurrentModel].Pos.z = 0.0F;
			Models[CurrentModel].Mat = MATRIX_Identity;
			MatrixTranspose( &Models[CurrentModel].Mat, &Models[CurrentModel].InvMat );
			
			if ( (CurrentModel == TITLE_MODEL_HoloLight) || 
				(CurrentModel == TITLE_MODEL_Bracket) ||
				(CurrentModel == TITLE_MODEL_MenuTVDummy) )
				Models[CurrentModel].Visible = 0;
			else
				Models[CurrentModel].Visible = 1;

			Models[CurrentModel].Group	= 0;
			Models[CurrentModel].Func	= MODFUNC_Nothing;
			Models[CurrentModel].ModelNum	= i;
		}else
			return false;

	}

	SetInitialTitleParams();

	PlaceObjects();

	TitleInitDone = false;

	// init all holo-object stuff...
	
	// actual model...
	CurrentHoloModel = (u_int16_t)-1;

	// scan lines...
	for ( i = 0; i < MAXNUMOF2DPOLYS; i++ )
	{
		BikeLine[ i ] = (u_int16_t)-1;
	}

	return true;
		
}

void KillLines(void)
{
	int i;
	
	if (LinesActive)
	{ 
		for (i=0; i<CurrentLine; i++)
		{
			if ( BikeLine[ i ] != (u_int16_t)-1 )
			{
				KillUsedPoly( BikeLine[i] );
				BikeLine[ i ] = (u_int16_t)-1;
			}
		}

		LinesActive = false;
		CurrentLine = 0;	  
	}

}

#define POINT_TO_PLANE( P, N ) ( (P)->x * (N)->Normal.x + (P)->y * (N)->Normal.y + (P)->z * (N)->Normal.z + ( (N)->Offset) )

bool SetUpLines (u_int16_t Model, PLANE plane, VECTOR *rot)
{
	int num_polys;	
	int poly, edge;
	LPTRIANGLE poly_ptr;
	LPLVERTEX vertex_ptr;
	VECTOR tempv1, tempv2, tempv3;
	VECTOR start, end, IntPoint;
	float point1dist, point2dist, distance2plane;
	int intersection_points;
	VECTOR point[MAXLINES];
   	int i;
	float thickness = 3.0F;
	int font;
	int edgebits;
	int scanlinenum;
	bool startset, endset;
	MXALOADHEADER	* mxaheader;
	MXLOADHEADER	* mxheader;
	int Group, ExecBuf;
	MATRIX Mat_R;
	float scaleout = 1.01F;

	font = GetScreenFont(FONT_Large);
	 
	BuildRotMatrix(rot->x, rot->y, rot->z, &Mat_R);

	if ( TitleModelSet[ Model ].DoIMorph )
	{
		mxaheader = &MxaModelHeaders[ Models[Model].ModelNum];

		for( Group = 0; Group < mxaheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < mxaheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
  
				num_polys = mxaheader->Group[Group].num_polys_per_execbuf[ExecBuf];
				poly_ptr = mxaheader->Group[Group].poly_ptr[ExecBuf];
				vertex_ptr = mxaheader->Group[Group].originalVerts[ExecBuf];

				intersection_points = 0;
				scanlinenum = 0;


				for (poly=0; poly<num_polys; poly++)
				{
					tempv1.x = vertex_ptr[poly_ptr->v1].x;
					tempv1.y = vertex_ptr[poly_ptr->v1].y;
					tempv1.z = vertex_ptr[poly_ptr->v1].z;

					tempv2.x = vertex_ptr[poly_ptr->v2].x;
					tempv2.y = vertex_ptr[poly_ptr->v2].y;
					tempv2.z = vertex_ptr[poly_ptr->v2].z;

					tempv3.x = vertex_ptr[poly_ptr->v3].x;
					tempv3.y = vertex_ptr[poly_ptr->v3].y;
					tempv3.z = vertex_ptr[poly_ptr->v3].z;
					
					ApplyMatrix( &Mat_R, &tempv1, &tempv1 );
					ApplyMatrix( &Mat_R, &tempv2, &tempv2 );
					ApplyMatrix( &Mat_R, &tempv3, &tempv3 );
					/*
					tempv1.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv1.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv1.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					*/
					tempv1.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv1.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv1.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					
					startset = false;
					endset = false;

					for (edge = 0; edge < 3; edge++)
					{					 
						switch (edge)
						{
						case 0:
							start.x = tempv1.x;	  
							start.y = tempv1.y;
							start.z = tempv1.z;
							end.x = tempv2.x;
							end.y = tempv2.y;
							end.z = tempv2.z;
							edgebits = 1;
							break;
						case 1:
							start.x = tempv2.x;
							start.y = tempv2.y;
							start.z = tempv2.z;
							end.x = tempv3.x;
							end.y = tempv3.y;
							end.z = tempv3.z;
							edgebits = 4;
							break;
						case 2:
							start.x = tempv3.x;
							start.y = tempv3.y;
							start.z = tempv3.z;
							end.x = tempv1.x;
							end.y = tempv1.y;
							end.z = tempv1.z;
							edgebits = 2;
							break;
						}

						point1dist = POINT_TO_PLANE( &start, &plane );
						point2dist = POINT_TO_PLANE( &end, &plane );

						if (!(((point1dist > 0.0F) && (point2dist > 0.0F)) || ((point1dist < 0.0F) && (point2dist < 0.0F))))
						{	//store points of intersection here...
							distance2plane = (point1dist / (point1dist - point2dist));
							
							IntPoint.x = start.x + ( distance2plane * (end.x - start.x ) );
							IntPoint.y = start.y + ( distance2plane * (end.y - start.y ) );
							IntPoint.z = start.z + ( distance2plane * (end.z - start.z ) );

							point[intersection_points++] = IntPoint;

							if (!startset)
							{	scanline[scanlinenum].start = IntPoint;
								startset = true;
							}else
							{
								scanline[scanlinenum].end = IntPoint;
								endset = true;
							}

						}
					}
					
					scanline[scanlinenum].poly = *poly_ptr;
					
					if ((startset) && (endset))
						scanlinenum++;

					poly_ptr++;
				}
				for (i=0; i<scanlinenum; i++)
				{

					BikeLine[CurrentLine] = FindFreePoly();
						
   					if( BikeLine[CurrentLine] != (u_int16_t) -1 )
					{
						LinesActive = true;

						Polys[ BikeLine[CurrentLine] ].Col1.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col1.G = 255;
						Polys[ BikeLine[CurrentLine] ].Col1.B = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.G = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.B = 255;
	   					Polys[ BikeLine[CurrentLine] ].Col3.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col3.G = 255;
   						Polys[ BikeLine[CurrentLine] ].Col3.B = 255;
   						Polys[ BikeLine[CurrentLine] ].Col4.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col4.G = 255;
   						Polys[ BikeLine[CurrentLine] ].Col4.B = 255;
   						Polys[ BikeLine[CurrentLine] ].Trans1 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Trans2 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Trans3 = 255;
   						Polys[ BikeLine[CurrentLine] ].Trans4 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Scale = 0.0F;
	   					Polys[ BikeLine[CurrentLine] ].Flags = POLY_FLAG_NOTHING;
   						Polys[ BikeLine[CurrentLine] ].Frm_Info = &Title_Fonts_Header;
	   					Polys[ BikeLine[CurrentLine] ].Frame = TextLookup[font][1];

						Polys[ BikeLine[CurrentLine] ].Pos1.x = Models[Model].Pos.x + (scanline[i].start.x - plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos1.y = Models[Model].Pos.y + (scanline[i].start.y - plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos1.z = Models[Model].Pos.z + (scanline[i].start.z - plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.x = Models[Model].Pos.x + (scanline[i].end.x - plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.y = Models[Model].Pos.y + (scanline[i].end.y - plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.z = Models[Model].Pos.z + (scanline[i].end.z - plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.x = Models[Model].Pos.x + (scanline[i].end.x + plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.y = Models[Model].Pos.y + (scanline[i].end.y + plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.z = Models[Model].Pos.z + (scanline[i].end.z + plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.x = Models[Model].Pos.x + (scanline[i].start.x + plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.y = Models[Model].Pos.y + (scanline[i].start.y + plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.z = Models[Model].Pos.z + (scanline[i].start.z + plane.Normal.z * (thickness / 2.0F));

						AddPolyToTPage( BikeLine[CurrentLine], GetTPage( *Polys[ BikeLine[CurrentLine] ].Frm_Info, (int16_t) Polys[ BikeLine[CurrentLine] ].Frame ) );

						CurrentLine++;
					}
					else
						return false;
				}
			}

		}
	}else
	{
		mxheader = &ModelHeaders[ Models[Model].ModelNum];

		for( Group = 0; Group < mxheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < mxheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
  
				num_polys = mxheader->Group[Group].num_polys_per_execbuf[ExecBuf];
				poly_ptr = mxheader->Group[Group].poly_ptr[ExecBuf];
				vertex_ptr = mxheader->Group[Group].originalVerts[ExecBuf];

				intersection_points = 0;
				scanlinenum = 0;


				for (poly=0; poly<num_polys; poly++)
				{
					tempv1.x = vertex_ptr[poly_ptr->v1].x;
					tempv1.y = vertex_ptr[poly_ptr->v1].y;
					tempv1.z = vertex_ptr[poly_ptr->v1].z;

					tempv2.x = vertex_ptr[poly_ptr->v2].x;
					tempv2.y = vertex_ptr[poly_ptr->v2].y;
					tempv2.z = vertex_ptr[poly_ptr->v2].z;

					tempv3.x = vertex_ptr[poly_ptr->v3].x;
					tempv3.y = vertex_ptr[poly_ptr->v3].y;
					tempv3.z = vertex_ptr[poly_ptr->v3].z;
					
					ApplyMatrix( &Mat_R, &tempv1, &tempv1 );
					ApplyMatrix( &Mat_R, &tempv2, &tempv2 );
					ApplyMatrix( &Mat_R, &tempv3, &tempv3 );

					/*
					tempv1.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv1.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv1.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv2.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.x *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.y *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					tempv3.z *= SelectedBikeScale * scaleout; //* BikeSwapScale;
					*/

					tempv1.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv1.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv1.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv2.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.x *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.y *= HoloModelScale * scaleout; //* BikeSwapScale;
					tempv3.z *= HoloModelScale * scaleout; //* BikeSwapScale;
					
					startset = false;
					endset = false;

					for (edge = 0; edge < 3; edge++)
					{					 
						switch (edge)
						{
						case 0:
							start.x = tempv1.x;	  
							start.y = tempv1.y;
							start.z = tempv1.z;
							end.x = tempv2.x;
							end.y = tempv2.y;
							end.z = tempv2.z;
							edgebits = 1;
							break;
						case 1:
							start.x = tempv2.x;
							start.y = tempv2.y;
							start.z = tempv2.z;
							end.x = tempv3.x;
							end.y = tempv3.y;
							end.z = tempv3.z;
							edgebits = 4;
							break;
						case 2:
							start.x = tempv3.x;
							start.y = tempv3.y;
							start.z = tempv3.z;
							end.x = tempv1.x;
							end.y = tempv1.y;
							end.z = tempv1.z;
							edgebits = 2;
							break;
						}

						point1dist = POINT_TO_PLANE( &start, &plane );
						point2dist = POINT_TO_PLANE( &end, &plane );

						if (!(((point1dist > 0.0F) && (point2dist > 0.0F)) || ((point1dist < 0.0F) && (point2dist < 0.0F))))
						{	//store points of intersection here...
							distance2plane = (point1dist / (point1dist - point2dist));
							
							IntPoint.x = start.x + ( distance2plane * (end.x - start.x ) );
							IntPoint.y = start.y + ( distance2plane * (end.y - start.y ) );
							IntPoint.z = start.z + ( distance2plane * (end.z - start.z ) );

							point[intersection_points++] = IntPoint;

							if (!startset)
							{	scanline[scanlinenum].start = IntPoint;
								startset = true;
							}else
							{
								scanline[scanlinenum].end = IntPoint;
								endset = true;
							}

						}
					}
					
					scanline[scanlinenum].poly = *poly_ptr;
					
					if ((startset) && (endset))
						scanlinenum++;

					poly_ptr++;
				}
				for (i=0; i<scanlinenum; i++)
				{

					BikeLine[CurrentLine] = FindFreePoly();
						
   					if( BikeLine[CurrentLine] != (u_int16_t) -1 )
					{
						LinesActive = true;

						Polys[ BikeLine[CurrentLine] ].Col1.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col1.G = 255;
						Polys[ BikeLine[CurrentLine] ].Col1.B = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.G = 255;
						Polys[ BikeLine[CurrentLine] ].Col2.B = 255;
	   					Polys[ BikeLine[CurrentLine] ].Col3.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col3.G = 255;
   						Polys[ BikeLine[CurrentLine] ].Col3.B = 255;
   						Polys[ BikeLine[CurrentLine] ].Col4.R = 255;
						Polys[ BikeLine[CurrentLine] ].Col4.G = 255;
   						Polys[ BikeLine[CurrentLine] ].Col4.B = 255;
   						Polys[ BikeLine[CurrentLine] ].Trans1 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Trans2 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Trans3 = 255;
   						Polys[ BikeLine[CurrentLine] ].Trans4 = 255;
	   					Polys[ BikeLine[CurrentLine] ].Scale = 0.0F;
	   					Polys[ BikeLine[CurrentLine] ].Flags = POLY_FLAG_NOTHING;
   						Polys[ BikeLine[CurrentLine] ].Frm_Info = &Title_Fonts_Header;
	   					Polys[ BikeLine[CurrentLine] ].Frame = TextLookup[font][1];

						Polys[ BikeLine[CurrentLine] ].Pos1.x = Models[Model].Pos.x + (scanline[i].start.x - plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos1.y = Models[Model].Pos.y + (scanline[i].start.y - plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos1.z = Models[Model].Pos.z + (scanline[i].start.z - plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.x = Models[Model].Pos.x + (scanline[i].end.x - plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.y = Models[Model].Pos.y + (scanline[i].end.y - plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos4.z = Models[Model].Pos.z + (scanline[i].end.z - plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.x = Models[Model].Pos.x + (scanline[i].end.x + plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.y = Models[Model].Pos.y + (scanline[i].end.y + plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos3.z = Models[Model].Pos.z + (scanline[i].end.z + plane.Normal.z * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.x = Models[Model].Pos.x + (scanline[i].start.x + plane.Normal.x * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.y = Models[Model].Pos.y + (scanline[i].start.y + plane.Normal.y * (thickness / 2.0F));
						Polys[ BikeLine[CurrentLine] ].Pos2.z = Models[Model].Pos.z + (scanline[i].start.z + plane.Normal.z * (thickness / 2.0F));

						AddPolyToTPage( BikeLine[CurrentLine], GetTPage( *Polys[ BikeLine[CurrentLine] ].Frm_Info, (int16_t) Polys[ BikeLine[CurrentLine] ].Frame ) );

						CurrentLine++;
					}
					else
						return false;
				}
			}

		}
	}
	
	return true;
}

/*===================================================================
	Procedure	:		Place title room objects into their correct positions
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void PlaceObjects(void)
{

	VECTOR Mtop, Mbot, Discs;
	int i;
	MATRIX Mat_R;

	Mtop.x = 0.0F; Mtop.y = 220.0F; Mtop.z = -158.0F;
	Mbot.x = 0.0F; Mbot.y = 35.0F; Mbot.z = -100.0F;
	CurrentVDUPos = RightVDU.Pos;
	VDUrot = RightVDU.StartRot;
									 
	Models[BackgroundModel[TITLE_MODEL_Mtop]].Pos = Mtop;
	Models[BackgroundModel[TITLE_MODEL_Mbot]].Pos = Mbot;
	Models[BackgroundModel[TITLE_MODEL_VDU]].Pos = CurrentVDUPos;
	Models[BackgroundModel[TITLE_MODEL_Bracket]].Pos = RightVDU.BracketPos;
	BracketRot = RightVDU.BracketStartRot;


	Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.x = Holopad.x;
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y - HolopadRange;
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.z = Holopad.z;

	BikePos.x = -501.0F;
	BikePos.y = Holopad.y + 100.0F;
	BikePos.z = Holopad.z;

  
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Pos.x = Holopad.x;
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Pos.y = Holopad.y - 2.0F;
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Pos.z = Holopad.z;


	OrigPos[TITLE_MODEL_Mtop] = Mtop;
	OrigPos[TITLE_MODEL_Mbot] = Mbot;

	for (i=0; i<=5; i++)
	{	Discs.x = 0.0F; Discs.y = (float)(i) * 28; Discs.z = -70.0F;
		Models[BackgroundModel[5 - i]].Pos = Discs;
		OrigPos[5 - i] = Discs;
	}

	stackpos[0] = Models[BackgroundModel[TITLE_MODEL_Mtop]].Pos;
	stackpos[1] = Models[BackgroundModel[TITLE_MODEL_Disc1]].Pos;
	stackpos[2] = Models[BackgroundModel[TITLE_MODEL_Disc2]].Pos;
	stackpos[3] = Models[BackgroundModel[TITLE_MODEL_Disc3]].Pos;
	stackpos[4] = Models[BackgroundModel[TITLE_MODEL_Disc4]].Pos;
	stackpos[5] = Models[BackgroundModel[TITLE_MODEL_Disc5]].Pos;
	stackpos[6] = Models[BackgroundModel[TITLE_MODEL_Disc6]].Pos;
	stackpos[7] = Models[BackgroundModel[TITLE_MODEL_Mbot]].Pos;
	
	// build rotate matrix...
	BuildRotMatrix(0.0F, VDUrot, 0.0F, &Mat_R);

	// rotate vdu...
	Models[BackgroundModel[TITLE_MODEL_VDU]].Mat = Mat_R;
	MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_VDU]].InvMat);

	BuildRotMatrix(0.0F, BracketRot, 0.0F, &Mat_R);

	// rotate vdu...
	Models[BackgroundModel[TITLE_MODEL_Bracket]].Mat = Mat_R;
	MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_Bracket]].InvMat);


	// build rotate matrix for holopad & light...
//	HoloRotx = 0.0F; HoloRoty = 0.0F; HoloRotz = 0.0F;
	HoloRotx = -6.0F; HoloRoty = 0.0F; HoloRotz = 3.0F;
	BuildRotMatrix(HoloRotx, HoloRoty, HoloRotz, &Mat_R);
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Mat = Mat_R;
	MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_Holopad]].InvMat);
	//Models[BackgroundModel[TITLE_MODEL_HoloLight]].Mat = Mat_R;
	//MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_HoloLight]].InvMat);

	HoloLightBrightness = 0.0F;

	RotateDiscStack(MaxStackTheta);
}

/*===================================================================
	Procedure	:		Expand / Contract discs, according to DiscExpandOut 
	Input		:		nothing...
	Output		:		nothing...
===================================================================*/
void ExpandDiscs(void)
{

	int i, CurrentDisc;
	float dy;
	float *currentY;

	dy = 0.3F * framelag;
	
	for (i = 0; i <= 6; i++)
	{
		CurrentDisc = i;
		currentY = &Models[BackgroundModel[CurrentDisc]].Pos.y;
		if (DiscExpandOut)
			*currentY -= (dy * (i + 1) * 3.0F);
		else
			*currentY += (dy * (i + 1));
	}

	if ((DiscExpandOut) && (Models[BackgroundModel[TITLE_MODEL_Disc1]].Pos.y < (OrigPos[TITLE_MODEL_Disc1].y - DiscExpand)))
	{	for (i = 0; i <= 6; i++)
		{	CurrentDisc = i;
			Models[BackgroundModel[CurrentDisc]].Pos.y = (OrigPos[CurrentDisc].y - DiscExpand * (i + 1));
			StackY[i] = Models[BackgroundModel[CurrentDisc]].Pos.y;
		}
		DiscExpandOut = false;
		StackStatus = DISC_WIGGLE;
	}

	if ((!DiscExpandOut) && (Models[BackgroundModel[TITLE_MODEL_Disc1]].Pos.y > OrigPos[TITLE_MODEL_Disc1].y))
	{	for (i = 0; i <= 6; i++)
		{	CurrentDisc = i;
			Models[BackgroundModel[CurrentDisc]].Pos.y = OrigPos[CurrentDisc].y;
		}
		StackStatus = DISC_NOTHING;
		DiscExpandOut = true;
		RotateSetup = false;
	}
}

/*===================================================================
	Procedure	:		Wiggle all discs along the y axis after expanding...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void WiggleDiscs(void)
{
	int i;

	DiscPauseTime += (framelag / (0.05F * 60.0F));

	if (DiscPauseTime >= 1.0F)
	{	DiscWiggleUp = (!DiscWiggleUp);
		DiscPauseTime = 0.0F;
		DiscWiggleDistance -= 1.0F;
		if (DiscWiggleDistance < 0.0F)
			DiscWiggleDistance = 0.0F;
	}

	for (i=0; i<=6; i++)
	{	if (DiscWiggleUp)
			Models[BackgroundModel[i]].Pos.y = StackY[i] + 0.143F * (i + 1) * DiscWiggleDistance;	//0.143 = 1/7
		else
			Models[BackgroundModel[i]].Pos.y = StackY[i];
	}

	if (DiscWiggleDistance <= 0.0F)
	{	DiscWiggleDistance = 0.0F;
		StackStatus = DISC_ROTATE;
	}

}


/*===================================================================
	Procedure	:		Get a random speed for a disc
	Input		:		disc number...
	Output		:		nothing...
===================================================================*/
void SetUpRotate(int disc)
{
	float rnd;

		rot[disc] = 0.0F; RotWiggle[disc] = 0; RotPause[disc] = 0.0F;
		Turned[disc] = !Turned[disc]; LastTurned[disc] = !Turned[disc];
		rnd	= (float)Random_Range(2000);
		if (rnd > 1000.0F)
		{	rnd = 1000.0F - rnd;
			rotspeed[disc] = rnd/1000.0F - 1.0F;
		}else{
			rotspeed[disc] = 1.0F + rnd/1000.0F;
		}
	
}

/*===================================================================
	Procedure	:		Get next "judder" amount for a disc (after disc has rotated)
	Input		:		disc number
	Output		:		nothing
===================================================================*/
void DiscJudder(int disc)
{
	float angle;
	float clockwise;;

	if (rot[disc] == 180)
		clockwise = 1.0F;
	else
		clockwise = -1.0F;

	RotPause[disc] += (framelag / (0.05F * 60.0F));

	if (RotPause[disc] >= 1.0F)
	{	RotWiggle[disc]++;
		if (RotWiggle[disc] > (Judders - 1))
			RotWiggle[disc] = (Judders - 1);
		RotPause[disc] = 0.0F;
	}

	angle = clockwise * JudderAngle[RotWiggle[disc]];

	if (Turned[disc])
		angle += 180.0F;

	BuildRotMatrix(0.0F, angle, 0.0F, &Models[BackgroundModel[disc]].Mat);
	MatrixTranspose(&Models[BackgroundModel[disc]].Mat, &Models[BackgroundModel[disc]].InvMat);

}

/*===================================================================
	Procedure	:		RotateDiscs
	Input		:		disc number, OneOnly: ROTATE_DISC_All ( all discs )
											  ROTATE_DISC_DifficultyForward
											  ROTATE_DISC_DifficultyBack
	Output		:		nothing...
===================================================================*/
void RotateDiscs(int disc, int OneOnly)
{
	float angleoffset, viewangle, x, y;

	x = View.x;
	y = -70.0F - View.z;

	viewangle = -(float)ATAND ( y, x);

		rot[disc] += ( (framelag/ (rotspeed[disc] * DiscSpeed * 60.0F)) * 180.0F);

		if (rot[disc] >= 180.0F)
			rot[disc] = 180.0F;
		if (rot[disc] <= -180.0F)
			rot[disc] = -180.0F;

		if (Turned[disc] != LastTurned[disc])
		{	if ( (rot[disc] >= viewangle) || (rot[disc] <= (viewangle - 180.0F)) )
			{  		
				switch ( OneOnly )
				{
				case ROTATE_DISC_All:
					PolyAnim[disc]->newframe = CurrentTextures[disc];
					break;
				case ROTATE_DISC_DifficultyForward:
					PolyAnim[disc]->newframe = PolyAnim[disc]->currentframe + 2;
					if (PolyAnim[disc]->newframe > 11)
						PolyAnim[disc]->newframe = 5;
					CurrentTextures[disc] = PolyAnim[disc]->newframe - 1;
					break;
				case ROTATE_DISC_DifficultyBack:
					PolyAnim[disc]->newframe = PolyAnim[disc]->currentframe - 2;
					if (PolyAnim[disc]->newframe < 5)
						PolyAnim[disc]->newframe = 11;
					CurrentTextures[disc] = PolyAnim[disc]->newframe - 1;
					break;
				}
				Turned[disc] = (!Turned[disc]);
			}
		}

		if ((rot[disc] != 180.0F) && (rot[disc] != -180.0F))
		{ 	if (Turned[disc])
				angleoffset = 180.0F;
			else
				angleoffset = 0.0F;
		  
			BuildRotMatrix(0.0F, rot[disc] + angleoffset, 0.0F, &Models[BackgroundModel[disc]].Mat);
			MatrixTranspose(&Models[BackgroundModel[disc]].Mat, &Models[BackgroundModel[disc]].InvMat);
		}else
		 	DiscJudder(disc);
}

/*===================================================================
	Procedure	:		step through complete disc rotation sequence for all discs...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void RotateAllDiscs(void)
{

	int i, count;
	static bool onceonly = false;


	switch (StackStatus)
		{
		case DISC_EXPAND:
			if ( !onceonly )
			{
				PlaySfx( SFX_StakAnimates, 1.0F );
				onceonly = true;
			}
			SetTextures(CurrentMenu);
		   	ExpandDiscs();
			break;
		case DISC_WIGGLE:
			WiggleDiscs();
			break;
		case DISC_ROTATE:
			if (!RotateSetup)
			{	for (i = 0; i <=5; i++)
					SetUpRotate(i);
				RotateSetup = true;
			}

			for (i=0; i<=5; i++)
				RotateDiscs(i, ROTATE_DISC_All );

			count = 0;
			for (i = 0; i <= 5; i++)
			{	if (RotWiggle[i] == (Judders - 1))
				count++;
			}
			if (count == 6)
			{	StackStatus = DISC_RETRACT;
			}
			break;
		case DISC_RETRACT:
			ExpandDiscs();
			break;
		case DISC_NOTHING:
			StackMode = DISC_MODE_NONE;
			DiscWiggleDistance = 4.0F;
			onceonly = false;
		}

}


/*===================================================================
	Procedure	:		step through complete disc rotation sequence for one disc...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void RotateOneDisc(int disc)
{

	static bool onceonly = false;
	
	switch (StackStatus)
	{
	case DISC_EXPAND:
		if ( !onceonly )
		{
			PlaySfx( SFX_StakAnimates, 1.0F );
			onceonly = true;
		}
		ExpandDiscs();
		break;
	case DISC_WIGGLE:
		WiggleDiscs();
		break;
	case DISC_ROTATE:
		if (!RotateSetup)
				SetUpRotate(disc);
			RotateSetup = true;

		RotateDiscs(disc, DifficultyDir );

		if (RotWiggle[disc] == (Judders - 1))
		 	StackStatus = DISC_RETRACT;
		
		break;
	case DISC_RETRACT:
		ExpandDiscs();
		break;
	case DISC_NOTHING:
		StackMode = DISC_MODE_NONE;
		DiscWiggleDistance = 4.0F; 
		onceonly = false;
		break;
	}
}


void MorphHoloLight(void)
{
	int i;
	float intensity;
	static float theta = 0.0F;
	u_int16_t Model = BackgroundModel[TITLE_MODEL_HoloLight];

	theta += (float)((framelag/(0.75F * 60.0F)) * 90.0F);

	if (theta > 360.0F)
		theta -= 360.0F;

	for (i=0; i<24; i++)
	{
		intensity = 1.0F - (((float)SIND(theta + (i * 15)) + 1.0F)/2.0F * 0.4F) * 2.0f; // methods: final *2.0f added to increase affect of light pulse
		TintOneVertex( Model, 0, 0, HoloTintVerts[i], intensity, intensity, intensity, 1.0F );
	} 
}

void RotateHoloLight(void)
{
	MATRIX Mat_Rx, Mat_Ry, Mat_Rz, Mat_temp, Mat_res;

	HoloRoty += 0.75F * framelag;
	
	if (HoloRoty > 360.0F)
		HoloRoty -= 360.0F;

	BuildRotMatrix(HoloRotx, 0.0F, 0.0F, &Mat_Rx);
	BuildRotMatrix(0.0F, HoloRoty, 0.0F, &Mat_Ry);
	BuildRotMatrix(0.0F, 0.0F, HoloRotz, &Mat_Rz);

	MatrixMultiply(&Mat_Rz, &Mat_Ry, &Mat_temp);
	MatrixMultiply(&Mat_Rx, &Mat_temp, &Mat_res);

	
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Mat = Mat_res;
	MatrixTranspose(&Mat_res, &Models[BackgroundModel[TITLE_MODEL_HoloLight]].InvMat);

}

void PulsateVDU(void)
{
	static float theta = 0.0F;
	float tintamount;
	int RightVertices[4] = {12, 13, 14, 15};
	int LeftVertices[4] = {8, 9, 10, 11};
	int *UseThese;
	int i;

	if (CameraStatus == CAMERA_AtRightVDU)
		UseThese = RightVertices;
	else
		UseThese = LeftVertices;
	
	theta += (float)((framelag/(1.0 * 60.0F)) * 90.0F);

	if (theta > 360.0F)
		theta -= 360.0F;

	for (i=0; i<4; i++)
	{
		tintamount = 1.0F - (((float)SIND(theta + i * 90.0F) + 1.0F)/2.0F * 0.6F); 
		TintOneVertex( BackgroundModel[TITLE_MODEL_VDU], 0, 1, UseThese[i], tintamount,tintamount,tintamount, 1.0F );
	}
}

void InitHoloPad( void );
void KillHoloModel( void );
void LoadHoloModel( u_int16_t model );
void ScaleHoloModel( float scale );
void ShowHoloModel( u_int16_t model );
void ProcessHoloModel( void );

u_int8_t QuickStart = QUICKSTART_None; 

bool RenderCurrentMenu(void) // this renders broken main menu
{
	u_int16_t	group;

	Build_View();
	CurrentCamera.View = view;

	if (!FSSetView(&view))
	{
		Msg( "DisplayTitle() : SetMatrix failed\n" );
		return false;
	}

	if(!FSSetViewPort(&CurrentCamera.Viewport))
	{
#ifdef DEBUG_VIEWPORT
		SetViewportError( "DisplayTitle", &CurrentCamera.Viewport );
#else
		Msg( "DisplayTitle() : SetViewport failed\n" );
#endif
		return false;
	}

	if (ClearBuffers() != true )
	{
		Msg( "DisplayTitle() : ClearBuffers failed\n" );
		return false;
	}

	// reset all the normal execute status flags...
	set_normal_states();

	if( !ModelDisp( 0, /*lpDev,*/ TitleModelSet ) ) // bjd
	{
		return false;
	}

/*
	Display 0 solid Clipped Non Faceme Transluecent Polys
*/

		if( !DisplaySolidGroupClippedPolys( &RenderBufs[ 2 ], 0 ) ) // bjd
				return false;
#if 0
/*
Display 0 solid Clipped Faceme Transluecent Polys
*/
		if( !DisplaySolidGroupClippedFmPolys( &RenderBufs[ 1 ], 0, lpDev, lpView ) )
				return false;
#endif

	// set all the Translucent execute status flags...
	set_alpha_states();

	// display clipped translucencies
/*===================================================================
	Display 0 Clipped Non Faceme Transluecent Polys
===================================================================*/

		if( !DisplayGroupClippedPolys( &RenderBufs[ 2 ], 0 ) ) // bjd
				return false;

/*===================================================================
Display 0 Clipped Faceme Transluecent Polys
===================================================================*/
		
		if( !DisplayGroupClippedFmPolys( &RenderBufs[ 2 ], 0 ) ) // bjd
				return false;

		ExecuteTransExe( 0 );
		ExecuteTransExeUnclipped( 0 );

/*===================================================================
	Display Non 0 Clipped Faceme Transluecent Polys
===================================================================*/

	if( !DisplayGroupUnclippedFmPolys( &RenderBufs[ 2 ] ) ) // bjd
			return false;

/*===================================================================
Display Non 0 Clipped Non Faceme Transluecent Polys
===================================================================*/
	if( !DisplayGroupUnclippedPolys( &RenderBufs[ 2 ] ) ) // bjd
			return false;

/*===================================================================
	Display Transluecent Screen Polys
===================================================================*/

		if( !DisplayNonSolidScrPolys( &RenderBufs[ 3 ] ) )
			return false;

//
// turn off transparency mode
//

	set_normal_states();

/*===================================================================
	Display Solid Screen Polys
===================================================================*/

	if( !DisplaySolidScrPolys( &RenderBufs[ 3 ] ) )
		return false;

/*===================================================================
Display Opaque Lines
===================================================================*/
		group = (u_int16_t)-1;

		ExecuteLines( group, &RenderBufs[ 0 ] );

/*===================================================================
Display Solid Lines
===================================================================*/
		group = (u_int16_t)-1;

		ExecuteLines( group, &RenderBufs[ 0 ] );

	return true;
}

/*===================================================================
	Procedure	:		Title Display...

	Input		:		nothing...
	Output		:		bool true/false
===================================================================*/
bool IpOnCLI;
bool DisplayTitle(void)
{
#if 1 // bjd - CHECK IMPORTANT
	u_int16_t i;
	u_int16_t	group;
	MENUITEM *Item;
	LIST *l;

	if (!InitialTexturesSet && CameraStatus != CAMERA_AtStart)
	{
		InitialTexturesSet = true;

	   	for (i=0; i<6; i++)
		{
			Mxloadheader[i] = &ModelHeaders[BackgroundModel[i]];
			PolyAnim[i] = Mxloadheader[i]->Group[0].polyanim[0];
			PolyAnim[i]->newframe = CurrentTextures[i];
			PolyAnim[i]->State = TEXTUREANIM_Stop;
		}

		if ((CurrentDiscHighlight >= 0) && (CurrentDiscHighlight < 6))
       			HighlightDisc(CurrentDiscHighlight);

	}

	NumOfTransExe = 0;

	if( TitleOnceOnly )
	{

		InitShipActionList();

		InitLevels( SINGLEPLAYER_LEVELS );
		InitLevels( MULTIPLAYER_LEVELS );

		TitleOnceOnly = false;
		WhoIAm = (u_int8_t) UNASSIGNED_SHIP;

		InitScreenFonts();

		if ( CurrentMenu == &MENU_NEW_Error )
		{
			CameraStatus = CAMERA_AtLeftVDU;
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
		}
	}

	if (!TitleInitDone && (( MyGameStatus == STATUS_Title ) || ( MyGameStatus == STATUS_BetweenLevels )))
	{

		DarkenRoomForStart( NULL );
		TitleInitDone = true;
		NoTeamSelect = false;
		SetFOV( START_FOV );	// in case player was using nitro when finishing level!

		if (UseNewMenus)
			MENU_Start = MENU_NEW_Start;

		switch ( QuickStart )
		{
		case QUICKSTART_Start:

			InitStartMenu( NULL );
			CameraStatus = CAMERA_AtLeftVDU;
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
			
			MENU_Start = MENU_NEW_CreateGame;
			
			MenuRestart( &MENU_Start );
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;

			SelectConnectionToStart( NULL );

			break;
		case QUICKSTART_Join:
			InitStartMenu( NULL );
			CameraStatus = CAMERA_AtLeftVDU;
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
			
			MENU_Start = MENU_NEW_ChooseConnectionToJoin;
			
			MenuRestart( &MENU_Start );
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
			
			if(IpOnCLI)
				SelectConnectionToJoin( NULL );
			break;

		case QUICKSTART_Notify:
			InitStartMenu( NULL );
			CameraStatus = CAMERA_AtLeftVDU;
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
			
			MENU_Start = MENU_NEW_Notify;
			
			MenuRestart( &MENU_Start );
			StackMode = DISC_MODE_NONE;
			StackStatus = DISC_NOTHING;
			break;
		default:
			if ( MyGameStatus != STATUS_BetweenLevels )
				MenuRestart( &MENU_Start );
		}
	}

#if 0
	VduHeader = &ModelHeaders[BackgroundModel[TITLE_MODEL_VDU]];
	//VduAnim = VduHeader->Group[0].polyanim[0];
	//VertPtr = VduAnim->vert;

	for (i=0; i<4; i++)
	{
		TvVerts[i] = *VertPtr++;
	}
#endif

	// initialise sfx
	for( i = 0 ; i < MAX_SFX ; i++ )
		LastDistance[i] = 100000.0F;

	if (!FSBeginScene())
	{
		Msg( "DisplayTitle() : BeginScene failed\n" );
		return false;
	}

	//Set up camera
	MakeViewMatrix(&View, &Look, &Up, &CurrentCamera.Mat);
	MatrixTranspose( &CurrentCamera.Mat, &CurrentCamera.InvMat );
	CurrentCamera.Pos = View;
	CurrentCamera.GroupImIn = -1;
	CurrentCamera.Viewport = viewport;	
	CurrentCamera.Viewport.X = 0;
	CurrentCamera.Viewport.Y = 0;
	CurrentCamera.Viewport.Width = render_info.window_size.cx;
	CurrentCamera.Viewport.Height = render_info.window_size.cy;
	CurrentCamera.Viewport.ScaleX = CurrentCamera.Viewport.Width / (float)2.0;
	CurrentCamera.Viewport.ScaleY = CurrentCamera.Viewport.Height / (float)2.0;

/* bjd
	CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwWidth),
									  RENDERVAL(2 * CurrentCamera.Viewport.dvScaleX));
	CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwHeight),
									   RENDERVAL(2 * CurrentCamera.Viewport.dvScaleY));
*/

	if( render_info.stereo_enabled )
	{
		if(!RenderCurrentCameraInStereo(RenderCurrentMenu))
			return false;
	}
	else
	{
		if(!RenderCurrentMenu())
			return false;
	}

	if (!FSEndScene())
	{
		Msg( "DisplayTitle() : EndScene failed\n" );
		return false;
	}

	ScreenPolyProcess();

	PulsateVDU();
	RotateHoloLight();

	switch (StackMode)
	{
	case DISC_MODE_ALL:
		RotateAllDiscs();
		break;
	case DISC_MODE_ONE:
		RotateOneDisc(TITLE_MODEL_Disc3);
		break;
	}

/****************************************/
	ProcessHoloModel();
	GetTitleMessage();
/*******************
Event handling
*******************/
	ProcessTimers();
	ProcessEvents();

	MorphHoloLight();
	
	ProcessTextItems();

	if( CurrentMenu && CurrentMenuItem )
	{
		MenuDraw( CurrentMenu );

		if (!WasteAFrame)
		{
			MenuItemDrawCursor( CurrentMenuItem );
		
			MenuProcess();
		}else
		{	NewMenuSelectMode(NULL);
			WasteAFrame = false;
		}
	}
	else
	{
		if( MyGameStatus == STATUS_Title )
			MenuRestart( &MENU_Start );	
		SetFOV(START_FOV);
	}

	if (CurrentMenu)
	{
  		for( Item = CurrentMenu->Item ; Item->x >= 0 ; Item++ )
		{	
			//if( Item->highlightflags & TEXTFLAG_RefreshList)
			if ( (Item->FuncDraw == DrawFlatMenuList) || (Item->FuncDraw == DrawSplitMenuList) )
			{	
				l = (LIST *)(Item->Variable);

				if ( l->FuncInfo )
					l->FuncInfo( l );
				
				AddToFlatMenuList( Item );
			}
		}
	}

	return true;
#endif
}

static int SelectionColour( void )
{
	static int colour_table[ 16 ] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0
	};
	return colour_table[ ( SDL_GetTicks() / ( 14 * 4 ) ) & 15 ];
}

/*===================================================================
	Procedure	:		Menu restart...
	Input		:		MENU * ...
	Output		:		Nothing
===================================================================*/
void MenuRestart( MENU * Menu )
{
	if ( MyGameStatus == STATUS_SinglePlayer )
	{
		PauseAllSfx();
	}
	
	FramesIgnored = 0;

	input_buffer_reset();
	
	if ( !QuickStart )
	{
		if ((CameraStatus == CAMERA_AtDiscs) || (CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
		{
			StackMode = DISC_MODE_ALL;
	 		StackStatus = DISC_EXPAND;
			DoHighlight = true;
		}
	}

	if (((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU)) && LastMenu )
	{
		VduClear();
	}

	MenuStackLevel = 0;
	LastMenu = CurrentMenu;
	CurrentMenu = Menu;
	CurrentMenuItem = CurrentMenu->Item;

	// find first item with select callback and callit
	while ( !CurrentMenuItem->FuncSelect && (CurrentMenuItem->x >= 0 ) )
	{
		CurrentMenuItem++;
	}

	// fire menu entry function
	if ( CurrentMenu->FuncEntry )
	{
		CurrentMenu->FuncEntry( CurrentMenu );
	}

	// go into selection mode by default
	MenuState = MENUSTATE_Select;	
	
	// fire selection event on item if it exists
	// this must come last so it can change things like MenuState
	MenuItemSelect(CurrentMenuItem);
}


/*===================================================================
	Procedure	:		Menu exit (backing out nicely)...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuExit( void )
{
	while ( MenuStackLevel >= 0 && CurrentMenu )
	{
		MenuBack();
	}
	MenuState = MENUSTATE_Select;
}

/*===================================================================
	Procedure	:		Menu abort (immediately)...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuAbort( void )
{
	if ( (CameraStatus == CAMERA_AtLeftVDU || CameraStatus == CAMERA_AtRightVDU) && CurrentMenu )
	{
	   	LastMenu = CurrentMenu;
		VduClear();
	}

	CurrentMenu = NULL;			   
	CurrentMenuItem = NULL;
	MenuStackLevel = 0;
	MenuState = MENUSTATE_Select;
	OldMenuStatus = 0;
}

/*===================================================================
	Procedure	:		Draw the Name in a menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void	MenuItemDrawName( MENUITEM * Item )
{
	int	x;
	int	y;

	if ( Item->StrPnt )
	{
		x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
		y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

		Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	}
}


/*===================================================================
	Procedure	:		Draw the Name in a paged menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void	MenuItemDrawPageName( MENUITEM * Item )
{
	int	x;
	int	y;;
	int page;

	page = (int) Item->Value;
	if ( Item->StrPnt && page == CurrentPage )
	{
		x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
		y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

		Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	}
}



void DrawKeyDefPage( MENUITEM * Item )
{
	static char page[ 80 ];
	int	x;
	int	y;
	static int col = 0;

	sprintf( page, LT_PageNo, CurrentPage + 1, MaxPage );
	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );
	if ( MaxPage > 1 )
	{
		if ( !CurrentPage )
			strcat( page, LT_PageDown );
		else if ( CurrentPage < MaxPage - 1 )
			strcat( page, LT_PageUpDown );
		else
			strcat( page, LT_PageUp );
	}
	Print4x5Text( page, x, y, col );
}



void DrawKeyDefHelp1( MENUITEM * Item )
{
	static char help[ 80 ];
	int	x;
	int	y;
	static int col = 0;
	char *keyname;

	if ( CurrentMenuItem && CurrentMenuItem->FuncSelect == SelectKeyDef )
	{
		keyname = (char *) CurrentMenuItem->StrPnt;
		if ( MenuState == MENUSTATE_Keydef )
		{
			sprintf( help, LT_PressKey/*"press key or button for %s"*/, keyname );
		}
		else
		{
			DEFKEY *kdef;

			kdef = (DEFKEY *) CurrentMenuItem->Variable;
			if ( kdef->def->keys > 1 )
			{
				if ( kdef->selected_key < 1 )
					sprintf( help, LT_ViewAlternateKeys, keyname );
				else if ( kdef->selected_key < kdef->def->keys - 1 )
					sprintf( help, LT_ViewAlternateKeys/*"[left/right]=view alternate keys for %s"*/, keyname );
				else
					sprintf( help, LT_ViewAlternateKeys2, keyname );
			}
			else
			{
				help[ 0 ] = 0;
			}
		}
	}
	else
	{
		sprintf( help, LT_SelectControl/*"[up/down]=select control"*/ );
	}
	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );
	CenterPrint4x5Text( help, y, col );
}



void DrawKeyDefHelp2( MENUITEM * Item )
{
	static char help[ 80 ];
	int	x;
	int	y;
	static int col = 0;
	char *keyname;

	if ( CurrentMenuItem && CurrentMenuItem->FuncSelect == SelectKeyDef )
	{
		keyname = (char *) CurrentMenuItem->StrPnt;
		if ( MenuState == MENUSTATE_Keydef )
		{
			sprintf( help, LT_Cancel/*"[esc]=cancel"*/ );
		}
		else
		{
#ifdef KEY_ENTER_AT_EMPTY_ADDS_KEYDEF
			sprintf( help, LT_Edit/*"[enter]=edit, [del]=remove key for %s"*/, keyname );
#else
			sprintf( help, LT_Add/*"[enter]=add, [del]=remove key for %s"*/, keyname );
#endif
		}
	}
	else
	{
		help[ 0 ] = 0;
	}
	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );
	CenterPrint4x5Text( help, y, col );
}


#define HELP_MAX	64

#define UNCONNECTED1 LT_Unconnected1/*" is not connected"*/
#define UNCONNECTED2 LT_Unconnected2/*" unconnected"*/
#define DUPLICATE_SEP	"+"
#define ABBREVIATED	"..."

void DrawKeyDefHelp3( MENUITEM * Item )
{
	static char help[ HELP_MAX * 2 ];
	int	x;
	int	y;
	static int col = 0;
	char *keyname;

	help[ 0 ] = 0;
	if ( CurrentMenuItem && CurrentMenuItem->FuncSelect == SelectKeyDef )
	{
		if ( MenuState != MENUSTATE_Keydef )
		{
			DEFKEY *kdef;
			int duplicates;
			VirtualKeycode keycode;
			char *keyword;
			MENUITEM *item;
			int help_length;
			int k;
			int joystick;

			kdef = (DEFKEY *) CurrentMenuItem->Variable;
			if ( kdef->def->keys )
			{
				keycode = kdef->def->key[ kdef->selected_key ];
				if ( keycode )
				{
					if ( KEY_ON_JOYSTICK( keycode )
						&& ( ( joystick = KEY_JOYSTICK( keycode ) ) >= 0 )
						&& ( joystick < Num_Joysticks )
						&& !JoystickInfo[ joystick ].connected )
					{
						strncpy( help, JoystickInfo[ KEY_JOYSTICK( keycode ) ].Name, HELP_MAX );
						help[ HELP_MAX - 1 ] = 0;
						if ( strlen( help ) + strlen( UNCONNECTED1 ) < HELP_MAX )
							strcat( help, UNCONNECTED1 );
						else if ( strlen( help ) + strlen( UNCONNECTED2 ) < HELP_MAX )
							strcat( help, UNCONNECTED2 );
					}
					else
					{
						keyword = (char *) key_name( keycode );
						if ( keyword )
						{
							duplicates = 0;
							sprintf( help, LT_AlsoControls/*"[%s] also controls "*/, keyword );
							for ( item = &MENU_Keyboard.Item[ 0 ]; item->x >= 0; item++ )
							{
								if ( item->FuncDraw == DrawKeyDef && item != CurrentMenuItem )
								{
									kdef = (DEFKEY *) item->Variable;
									for ( k = 0; k < kdef->def->keys; k++ )
									{
										if ( kdef->def->key[ k ] == keycode )
										{
											keyname = (char *) item->StrPnt;
											help_length = strlen( help ) + strlen( keyname );
											if ( duplicates )
												help_length += strlen( DUPLICATE_SEP );
											if ( help_length < HELP_MAX )
											{
												if ( duplicates++ )
													strcat( help, DUPLICATE_SEP );
												strcat( help, keyname );
											}
											else
											{
												help_length = strlen( help );
												if ( help_length + 1 < HELP_MAX  )
													strncat( help, "+"ABBREVIATED, HELP_MAX - 1 - help_length );
												else
													strcpy( help + HELP_MAX - 4, ABBREVIATED );
												help[ HELP_MAX - 1 ] = 0;
											}
											break;
										}
									}
								}
							}
							if ( !duplicates )
								help[ 0 ] = 0;
						}
					}
				}
			}
		}
	}
	if ( help[ 0 ] )
	{
		x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
		y = (int) ( ( Item->y >> 1 ) * ModeScaleY );
		CenterPrint4x5Text( help, y, col );
	}
}

/*===================================================================
	Procedure	:		Draw the Name and string variable in a menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawNameVar( MENUITEM * Item )
{
	int	x;
	int	y;
	char *var;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	var = (char *)(Item->Variable);
	Print4x5Text( var, -1 , y , 1 );

}

void DrawCenteredNameVar( MENUITEM * Item )
{
	int	x;
	int	y;
	char *var;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	var = (char *)(Item->Variable);
	CenterPrint4x5Text( var, y, 5 );
}



/*===================================================================
	Procedure	:		Draw the help key and description in a menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void	DrawHelpKey( MENUITEM * Item )
{
	int	x;
	int	y;
	char *key;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	key = (char *)(Item->Value);
	if ( key )
		Print4x5Text( key, x , y , 1 );
	x = (int) ( ( ( Item->x + 64 ) >> 1 ) * ModeScaleX );
	Print4x5Text( Item->StrPnt , x, y , InGameMenuColour );
}



/*===================================================================
	Procedure	:		Menu Draw..draws the title and all other text..
	Input		:		MENU * Menu...
	Output		:		Nothing
===================================================================*/
void MenuDraw( MENU * Menu )
{
	MENUITEM * Item;
	int	y;

	y = (int) ( ( 80 >> 1 ) * ModeScaleY );

	if ( Menu->MenuFuncDraw )
		Menu->MenuFuncDraw ( &DiscStatus);

	//if ( (CameraStatus != CAMERA_AtDiscs) && (CameraStatus != CAMERA_AtVDU))
	if (CameraStatus == CAMERA_AtStart)
		CenterPrint4x5Text( Menu->Name , y , InGameMenuColour );

	// only use menu item draw functions if not at VDU
	if ((CameraStatus != CAMERA_AtLeftVDU) && (CameraStatus != CAMERA_AtRightVDU))
	{
		for( Item = Menu->Item ; Item->x >= 0 ; Item++ )
		{
			if( Item->FuncDraw )

			{
				Item->FuncDraw( Item );
			}
		}
	}


}


void MenuChangeEx( MENU *Menu )
{
	MENUITEM Item;

	memset( &Item, 0, sizeof( MENUITEM ) );
	Item.Value = ( void *)Menu;
	MenuChange( &Item );
}

/*===================================================================
	Procedure	:		Menu Change...
	Input		:		MENUI
	TEM * ITEM...
	Output		:		Nothing
===================================================================*/
void MenuChange( MENUITEM * Item )
{
	
	MenuState = MENUSTATE_Select;
	
	DoHighlight = true;	//menu has changed, therefore ensure option is highlighted

	VduProcessed = false;

	PanDone = false;

	OldMenuStatus = CurrentMenu->MenuStatus;

	if ( MenuStackLevel < MAXMENULEVELS )
	{
		MenuStack[ MenuStackLevel ] = CurrentMenu;
		MenuItemStack[ MenuStackLevel ] = CurrentMenuItem;
		LastMenu = CurrentMenu;
		MenuStackLevel++;
		CurrentMenu = (MENU*) Item->Value;


		// Animate discs if neccesary
		if ((CurrentMenu->MenuStatus == TITLE_TIMER_ChooseDiscPan) && (OldMenuStatus == TITLE_TIMER_ChooseDiscPan))
																	      
		{	StackMode = DISC_MODE_ALL;
			StackStatus = DISC_EXPAND;
		}

				 
		CurrentMenuItem = CurrentMenu->Item;
		while ( !CurrentMenuItem->FuncSelect && (CurrentMenuItem->x >= 0 ) )
		{
			CurrentMenuItem++;
		}
		NoMenuBack = false;
		if ( CurrentMenu->FuncEntry && !IgnoreMenuFuncs)
		{
			CurrentMenu->FuncEntry( CurrentMenu );
		}

		// Process VDU if neccessary
		if ( ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU)) && VDU_Ready)
		{
			VduClear();
			ProcessVduItems(CurrentMenu);
			//VDU_Ready = false;
			LastMenuItem = NULL;
		}
	}

}

/*===================================================================
	Procedure	:		Menu Change back a level...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuBack()
{
	if ( MyGameStatus == STATUS_BetweenLevels )
		return;

	if ( NoMenuBack )
		return;
	
	if ( MenuStackLevel )
	{
	   	if ( !InTitleRoom )
			PlayCursorSfx();
			
		DoHighlight = true;	//menu has changed, therefore ensure option is highlighted

		VduProcessed = false;

		PanDone = false;

		OldMenuStatus = CurrentMenu->MenuStatus;
	
		// if going back to old menu system...
		if ( CurrentMenu == &MENU_NEW_Start ) 
		{	
			CameraStatus = CAMERA_AtStart;
		}
	
			
		if ( CurrentMenu->FuncExit && !IgnoreMenuFuncs)
		{
			CurrentMenu->FuncExit( CurrentMenu );
			if (!CurrentMenu)
				return;
		}

		MenuStackLevel--;
		if ( MenuStackLevel < 0 )
		{
			MenuStackLevel = 0;
			return;
		}
		LastMenu = CurrentMenu;
		CurrentMenu = MenuStack[ MenuStackLevel ];
		CurrentMenuItem = MenuItemStack[ MenuStackLevel ];

		if (CameraStatus == CAMERA_AtDiscs)
			SetTextures(CurrentMenu);

		// Animate discs if neccesary
		if ((OldMenuStatus == TITLE_TIMER_ChooseDiscPan) && (CurrentMenu->MenuStatus == TITLE_TIMER_ChooseDiscPan))
		{	StackMode = DISC_MODE_ALL;
			StackStatus = DISC_EXPAND;
		}

 		// Process VDU if neccessary
		if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
		{
			VduClear();
			ProcessVduItems(CurrentMenu);
			//VDU_Ready = false;
			LastMenuItem = NULL;
		}

	}
	else if ( CurrentMenu )
	{
		if ( CurrentMenu->FuncExit && !IgnoreMenuFuncs)
		{
			CurrentMenu->FuncExit( CurrentMenu );
		}

		LastMenu = CurrentMenu;
		VduClear();

		CurrentMenu = NULL;
		CurrentMenuItem = NULL;

		if ( ( MyGameStatus == STATUS_Title ) && ( QuickStart ) )
		{
			TitleReset( NULL );
		}

		if ( MyGameStatus == STATUS_SinglePlayer )
		{
			ReTriggerSfx();
		}

		// ensure cheat message is not displayed next time in menus
		FlashTextActive = 0.0F;
	}
}

void MenuBackSpecific( MENU *Menu, bool UseExitFuncs )
{
	IgnoreMenuFuncs = !UseExitFuncs;
	
	while( ( CurrentMenu != Menu ) && ( CurrentMenu != &MENU_Start ) )
		MenuBack();

	IgnoreMenuFuncs = false;
}

/*===================================================================
	Procedure	:		Menu item for Change back a level...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuItemBack( MENUITEM *item )
{
	MenuBack();
}


/*===================================================================
	Procedure	:		Draw the Name in a menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void	SetDifficulty( MENUITEM * Item )
{
}



/*===================================================================
	Procedure	:		Draw the Name in a menu item...
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void	MenuItemDrawCursor( MENUITEM * Item )
{
	int	x;
	int	y;
	
	switch (CameraStatus)
	{
	case CAMERA_AtDiscs:
		if (((CurrentDiscHighlight != Item->y) || DoHighlight) && (StackStatus == DISC_NOTHING))
		{	if (CurrentDiscHighlight != -1)
				ResetDisc(CurrentDiscHighlight);
			CurrentDiscHighlight = Item->y;
			HighlightDisc(CurrentDiscHighlight);
			DoHighlight = false;
		} 
		break;
	case CAMERA_AtStart:
		if ( MenuState != MENUSTATE_Select && MenuState != MENUSTATE_SelectKeydef )
			return;
		x = (int) ( ( ( Item->x - 18 ) >> 1 ) * ModeScaleX );
		y = (int) ( ( Item->y >> 1 ) * ModeScaleY );
		
		if ( !( Item->highlightflags & TEXTFLAG_AutoSelect ) ) 
			Print4x5Text( "0" , x , y , 0 );
		break;
	}

		
}

/*===================================================================
	Procedure	:		Cursor Movement Functions...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void PlayCursorSfx( void )
{
	if ( InTitleRoom )
	{
		if ( CameraStatus == CAMERA_AtDiscs )
			PlaySfx( SFX_MoveStackOption, 1.0F );
		else
			PlaySfx( SFX_VidText, 1.0F );
	}else
		PlaySfx( SFX_VidText, 1.0F );
}

bool ItemSelectable( MENUITEM *item )
{
	if (item->highlightflags & TEXTFLAG_Unselectable)
		return false;

	if ( NoTeamSelect && ( item->highlightflags & TEXTFLAG_TeamItem ) )
		return false;

	return true;
}

void CursorUp()
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *wrap;
	MENUITEM *item;

	

	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;

	next = NULL;
	wrap = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect && ItemSelectable( item ) )
		{
			if ( ( item != CurrentMenuItem ) &&	( item->y < y ) &&
				( !next || ( ( abs( item->y - y ) + ( abs( item->x - x ) ) <= ( abs( next->y - y ) + abs( next->x - x ) ) ) ) ) )
			{
				next = item;
			}
			if ( !wrap || ( ( item->y >= wrap->y ) && ( abs( item->x - x ) <= abs( wrap->x - x ) ) ) )
			{
				wrap = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();
	}
	else
	{
		if ( wrap )
		{
			CurrentMenuItem = wrap;
			PlayCursorSfx();

		}
	}
}

void CursorDown()
{
		int x;
		int y;
		MENUITEM *next;
		MENUITEM *wrap;
		MENUITEM *item;

		
	
		x = CurrentMenuItem->x;
		y = CurrentMenuItem->y;
		next = NULL;
		wrap = NULL;
		for ( item = CurrentMenu->Item; item->x >= 0; item++ )
		{
			if ( item->FuncSelect && ItemSelectable( item ) )
			{
				if ( ( item != CurrentMenuItem ) &&	( item->y > y ) &&
					( !next || ( ( abs( item->y - y ) + ( abs( item->x - x ) ) <= ( abs( next->y - y ) + abs( next->x - x ) ) ) ) ) )
				{
					next = item;
				}
				if ( !wrap || ( ( item->y <= wrap->y ) && ( abs( item->x - x ) <= abs( wrap->x - x ) ) ) )
				{
					wrap = item;
				}
			}
		}
		if ( next )
		{
			CurrentMenuItem = next;
			PlayCursorSfx();

		}
		else
		{
			if ( wrap )
			{
				CurrentMenuItem = wrap;
				PlayCursorSfx();
			}
		}
	
}


void CursorLeft()
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *wrap;
	MENUITEM *item;

	

	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;
	next = NULL;
	wrap = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect && ItemSelectable( item ) )
		{
			if ( ( item != CurrentMenuItem ) &&	( item->x < x ) &&
				( !next || ( ( abs( item->y - y ) + ( abs( item->x - x ) ) <= ( abs( next->y - y ) + abs( next->x - x ) ) ) ) ) )
			{
				next = item;
			}
			if ( !wrap || ( ( item->x >= wrap->x ) && ( abs( item->y - y ) <= abs( wrap->y - y ) ) ) )
			{
				wrap = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();

	}
	else
	{
		if ( wrap )
		{
			CurrentMenuItem = wrap;
		}
	}
}

void CursorRight()
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *wrap;
	MENUITEM *item;

	

	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;
	next = NULL;
	wrap = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect && ItemSelectable( item ) )
		{
			if ( ( item != CurrentMenuItem ) &&	( item->x > x ) &&
				( !next || ( ( abs( item->y - y ) + ( abs( item->x - x ) ) <= ( abs( next->y - y ) + abs( next->x - x ) ) ) ) ) )
			{
				next = item;
			}
			if ( !wrap || ( ( item->x <= wrap->x ) && ( abs( item->y - y ) <= abs( wrap->y - y ) ) ) )
			{
				wrap = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();

	}
	else
	{
		if ( wrap )
		{
			CurrentMenuItem = wrap;
		}
	}
}


void CursorHome()
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *item;

	
	
	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;
	next = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect && ItemSelectable( item ) )
		{
			if ( !next || ( ( item->x <= next->x ) && ( item->y <= next->y ) ) )
			{
				next = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();
	}
}


void CursorEnd()
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *item;

	

	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;
	next = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect && ItemSelectable( item ) )
		{
			if ( !next || ( ( item->x >= next->x ) && ( item->y >= next->y ) ) )
			{
				next = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();
	}
}


void CursorSearch( char *keyword )
{
	int x;
	int y;
	MENUITEM *next;
	MENUITEM *wrap;
	MENUITEM *item;
	int keyword_len;

	

	if ( !keyword )
		return;
	keyword_len = strlen( keyword );
	if ( !keyword_len )
		return;
	x = CurrentMenuItem->x;
	y = CurrentMenuItem->y;
	next = NULL;
	wrap = NULL;
	for ( item = CurrentMenu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncSelect  && ItemSelectable( item ) )
		{
			if ( ( ( item->x > x ) || ( item->y > y ) )
				&& ( !next || ( ( abs( item->x - x ) + abs( item->y - y ) ) <= ( abs( next->x - x ) + abs( next->y - y ) ) ) )
				&& item->StrPnt && !strncasecmp( item->StrPnt, keyword, keyword_len ) )
			{
				next = item;
			}
			if ( ( ( item->x < x ) || ( item->y < y ) )
				&& ( !wrap || ( ( abs( item->x - x ) + abs( item->y - y ) ) >= ( abs( wrap->x - x ) + abs( wrap->y - y ) ) ) )
				&& item->StrPnt && !strncasecmp( item->StrPnt, keyword, keyword_len ) )
			{
				wrap = item;
			}
		}
	}
	if ( next )
	{
		CurrentMenuItem = next;
		PlayCursorSfx();

	}
	else if ( wrap )
	{
		CurrentMenuItem = wrap;
		PlayCursorSfx();
	}
	else
	{
		SearchKey( 0 );
	}
}


static VirtualKeycode *KeyDefined( USERCONFIG *cfg, VirtualKeycode key )
{
	USERKEY *keydef;
	int j;

	for ( keydef = &cfg->up; keydef < &cfg->select_secondary[ MAX_SECONDARY_WEAPONS ]; keydef++ )
	{
		for ( j = 0; j < keydef->keys; j++ )
		{
			if ( keydef->key[ j ] == key )
				return &keydef->key[ j ];
		}
	}

	return NULL;
}


static void
DefineKey( MENUITEM *item, VirtualKeycode key )
{
	VirtualKeycode *kdef;//, *other_kdef;

	kdef = (VirtualKeycode *)(item->Variable);
	//other_kdef = KeyDefined( &Config, key );
	//if ( other_kdef )
	//{
	//	*other_kdef = *kdef;
	//}
	*kdef = key;
}

static void KeySelect( DEFKEY *kdef, int key )
{
	int k;

	if ( !key )
		return;
	for ( k = 0; k < kdef->def->keys; k++ )
	{
		if ( kdef->def->key[ k ] == key )
			kdef->selected_key = k;
	}
}


static void
KeyDeleteRepeating( DEFKEY *kdef )
{
	int j, k;

	for ( j = 0; j < kdef->def->keys; )
	{
		if ( !kdef->def->key[ j ] )
			KeyDelete( kdef, j );
		else
		{
			for ( k = j + 1; k < kdef->def->keys; )
			{
				if ( kdef->def->key[ j ] == kdef->def->key[ k ] )
					KeyDelete( kdef, k );
				else
					k++;
			}
			j++;
		}
	}

	for ( k = kdef->def->keys; k < MAX_KEYS_PER_CONTROL; k++ )
	{
		kdef->def->key[ k ] = (VirtualKeycode) 0;
	}
}

static void
KeyDelete( DEFKEY *kdef, int key )
{
	int k;

	for ( k = key; k < kdef->def->keys - 1; k++ )
	{
		kdef->def->key[ k ] = kdef->def->key[ k + 1 ];
	}
	while ( k < MAX_KEYS_PER_CONTROL )
	{
		kdef->def->key[ k++ ] = (VirtualKeycode) 0;
	}
	if ( kdef->def->keys > 0 )
		kdef->def->keys--;
	if ( kdef->def->keys > 0 )
	{
		if ( kdef->selected_key >= kdef->def->keys )
			kdef->selected_key = kdef->def->keys - 1;
	}
	else
	{
		kdef->selected_key = 0;
	}
}


static void
KeyDefine( MENUITEM *item, VirtualKeycode key )
{
	DEFKEY *kdef;

	kdef = (DEFKEY *)(item->Variable);
	if ( kdef )
	{
		if ( key )
		{
			kdef->def->key[ kdef->selected_key ] = key;
			if ( KEY_ON_JOYSTICK( key ) )
			{
				int joystick;

				joystick = KEY_JOYSTICK( key );
				if ( joystick >= 0 && joystick < Num_Joysticks && JoystickInfo[ joystick ].connected )
				{
					JoystickInfo[ joystick ].assigned = true;
				}
			};
			PlaySfx( SFX_InGameMenuSelect, 1.0F );
		}
		else
		{
			if ( !kdef->def->key[ kdef->selected_key ] )
			{
				KeyDelete( kdef, kdef->selected_key );
			}
			PlayCursorSfx();
		}
		KeyDeleteRepeating( kdef );
		KeySelect( kdef, key );
	}
}



bool ProcessDefKey( int Key )
{
	bool done;

	done = false;
	switch( Key )
	{
	case SDLK_ESCAPE:
	case SDLK_F1:
	case SDLK_F2:
	case SDLK_F3:
	case SDLK_F4:
	case SDLK_F5:
	case SDLK_F6:
	case SDLK_F7:
	case SDLK_F8:
	case SDLK_F9:
	case SDLK_F10:
	case SDLK_F11:
	case SDLK_F12:
		if ( KeyItem->Variable )
			KeyDefine( KeyItem, (VirtualKeycode) 0 );
		done = true; // back out of key definition mode if illegal key selected
		break;
	default:
		if ( Key )
		{
			if ( KeyItem->Variable )
			{
				KeyDefine( KeyItem, (VirtualKeycode) Key );
			}
			done = true;
		}
		break;
	}

	return !done;
}


void SetKeyDefPage( int page )
{
	MENUITEM *item;

	CurrentPage = page;
	MaxPage = MAX_KEYDEF_PAGES;
	for ( item = MENU_Keyboard.Item; item->x >= 0; item++ )
	{
		if ( item->FuncDraw == DrawKeyDef )
		{
			item->FuncSelect = ( (int) item->Value == CurrentPage ) ? SelectKeyDef : NULL;
		}
	}
	CurrentMenuItem = CurrentMenu->Item;
	while ( !CurrentMenuItem->FuncSelect && (CurrentMenuItem->x >= 0 ) )
	{
		CurrentMenuItem++;
	}
}

void InitKeyDefPage( MENU *Menu )
{
	SetKeyDefPage( 0 );
	MenuState = MENUSTATE_SelectKeydef;
}


void InitKeyDef( MENU *Menu )
{
	MenuState = MENUSTATE_SelectKeydef;
}



int ValidBikeSelected( int bike)
{
	int j;

	if ( CanSelectBike[ bike ] )
		return bike;
	for ( j = 0; j < MAXBIKETYPES; j++ )
	{
		if ( CanSelectBike[ j ] )
		{
			return j;
		}
	}
	return -1;
}


void DeleteJoyButton( void )
{
	int joystick_num, num_buttons, i, j, num_povs;
	
	if ( JoystickConfigState == JOYCONFIG_SelectAction )
	{
		joystick_num = JoystickMap[JoystickList.selected_item];
		num_buttons = JoystickInfo[joystick_num].NumButtons;
		num_povs = JoystickInfo[joystick_num].NumPOVs;

		for (i = 0; i < num_buttons; i++)
		{
			if ( JoystickInfo[ joystick_num ].Button[ i ].action == ShipActionList.selected_item )
				JoystickInfo[ joystick_num ].Button[ i ].action = SHIPACTION_Nothing;
		}

		for ( i = 0; i < num_povs; i++ )
		{
			for ( j = 0; j < MAX_POV_DIRECTIONS; j++ )
			{
			if ( JoystickInfo[ joystick_num ].POV[ i ].action[j] == ShipActionList.selected_item )
				JoystickInfo[ joystick_num ].POV[ i ].action[j] = SHIPACTION_Nothing;
			}
		}
	}
}

void StartTimer( int timer )
{
	if (Title_Timers[ timer ].Status == TITLE_EVENT_TIMER_IDLE)
		if (Title_Timers[ timer ].InitFunc)
			Title_Timers[ timer ].InitFunc (&Title_Timers[ timer ]);
}

void CheckMenuTimer( void )
{
	if ((CurrentMenu) && (CurrentMenu->MenuStatus))
		StartTimer( CurrentMenu->MenuStatus );
}

void SetVolumeLevels( void )
{
	VECTOR	temp;
	float dist;

	temp.x = Models[BackgroundModel[TITLE_MODEL_VDU]].Pos.x - View.x;
	temp.y = Models[BackgroundModel[TITLE_MODEL_VDU]].Pos.y - View.y;
	temp.z = Models[BackgroundModel[TITLE_MODEL_VDU]].Pos.z - View.z;

	dist = (float) sqrt( ( temp.x * temp.x ) + ( temp.y * temp.y ) + ( temp.z * temp.z ) );
	if ( dist < 540.0F )
	{
		if ( !TitleLoopId[ TITLE_LOOP_VduAmbience ] )
		{
			TitleLoopId[ TITLE_LOOP_VduAmbience ] = PlaySfx( SFX_VDUAmbience, 0.3F );
		}
	}else
	{
		if ( TitleLoopId[ TITLE_LOOP_VduAmbience ] )
		{
			StopSfx( TitleLoopId[ TITLE_LOOP_VduAmbience ] );
			TitleLoopId[ TITLE_LOOP_VduAmbience ] = 0;
		}
	}

	if ( !TitleLoopId[ TITLE_LOOP_DiscAmbience ] )
	{
		TitleLoopId[ TITLE_LOOP_DiscAmbience ] = PlaySfx( SFX_TitleAmb, 0.3F );
	}

}

/*===================================================================
	Procedure	:		Select/operate a slider menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectSlider( MENUITEM *Item )
{
	SLIDER *s;

	if (Item->highlightflags & TEXTFLAG_AutoSelect)
		MenuState = MENUSTATE_Slider2;
	else
		MenuState = MENUSTATE_Slider;

	SliderItem = Item;
	s = (SLIDER *)(Item->Variable);
	OriginalSliderValue = s->value;

}


/*===================================================================
	Procedure	:		Decrement a slider value
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DecrementSlider( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	s->value -= s->step;
	if ( s->value < s->min )
		s->value = s->min;
}


/*===================================================================
	Procedure	:		Increment a slider value
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void IncrementSlider( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	s->value += s->step;
	if ( s->value > s->max )
		s->value = s->max;
}



/*===================================================================
	Procedure	:		Minimise a slider value
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SliderHome( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	s->value = s->min;
}


/*===================================================================
	Procedure	:		Maximise a slider value
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SliderEnd( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	s->value = s->max;
}


/*===================================================================
	Procedure	:		Cancel a slider selection
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SliderSet( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	if ( s->FuncSet )
	{
		s->FuncSet( s );
	}
}


/*===================================================================
	Procedure	:		Cancel a slider selection
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SliderCancel( MENUITEM *Item )
{
	SLIDER *s;

	s = (SLIDER *)(Item->Variable);
	s->value = OriginalSliderValue;
}



/*===================================================================
	Procedure	:		Draw a slider menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawSlider( MENUITEM *Item )
{
	int	x;
	int	y;
	int sx;
	int ex;
	char min[20], max[20], val[20];
	SLIDER *s;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	// text label for the slider
	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );

	// print a space and get the start position
	sx = Print4x5Text( " ", -1, y, InGameMenuColour );

	// calculate end position
	ex = sx + (int) floor( ( 128 >> 1 ) * ModeScaleX );

	// get the slider start/current/stop values
	s = (SLIDER *) (Item->Variable);
	sprintf( min, "%d", s->min );
	sprintf( max, "%d", s->max );
	sprintf( val, "%d", s->value );

	// slider value 
	Print4x5Text( val, sx + ( ex - sx ) * ( s->value - s->min ) / ( s->max - s->min ), y, RED );

	// minimum value
	Print4x5Text( min, sx, y, GRAY );

	// max value
	Print4x5Text( max, ex, y, GRAY );
}


/*===================================================================
	Procedure	:		Select a radio button item
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectRadioButton( MENUITEM *Item )
{
	if ( Item->Variable )
	{
		*((int *)(Item->Variable)) = (int) Item->Value;
	}
}



/*===================================================================
	Procedure	:		Draws a radio button item
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawRadioButton( MENUITEM *Item )
{
	int	x;
	int	y;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	if ( Item->Variable )
	{
		if ( *((int *)(Item->Variable)) == (int) Item->Value )
		{
			// draw selected item
			Print4x5Text( Item->StrPnt , x , y , 1 );
		}
		else
		{
			// draw unselected item
			Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
		}
	}
}


/*===================================================================
	Procedure	:		Select a toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectToggle( MENUITEM *Item )
{

	MenuItemFunc set_toggle;

	if ( Item->Variable )
	{
		*(bool *)(Item->Variable) = !*(bool *)(Item->Variable);
		if ( Item->Value )
		{
			set_toggle = (MenuItemFunc)(Item->Value);
			set_toggle( Item );
		}
	}

}


/*===================================================================
	Procedure	:		Draw a toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawToggle( MENUITEM *Item )
{
	int	x;
	int	y;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	if ( Item->Variable )
	{
		if ( *(bool *)(Item->Variable ) )
		{
			Print4x5Text( LT_ToggleOn/*" ON"*/, -1 , y , 1 );
		}
		else
		{
			Print4x5Text( LT_ToggleOff/*" OFF"*/, -1 , y , 1 );
		}
	}
}

/*===================================================================
	Procedure	:		Draw a collision perspective toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawColToggle( MENUITEM *Item )
{
	int	x;
	int	y;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );

	if ( ColPerspective == COLPERS_Forsaken )
		Print4x5Text( LT_ToggleShooter/*" SHOOTER"*/, -1 , y , 1 );
	else if ( ColPerspective == COLPERS_Descent )
		Print4x5Text( LT_ToggleTarget/*" TARGET"*/, -1 , y , 1 );
}

/*===================================================================
	Procedure	:		Select mature content toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectMatureContentToggle( MENUITEM *Item )
{
		if(*(int *) Item->Variable > 1)
			*(int *) Item->Variable = 0;
		else
			*(int *) Item->Variable =*(int *)Item->Variable + 1;
}

/*===================================================================
	Procedure	:		Draw mature content toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawMatureContentToggle( MENUITEM *Item )
{
	int	x;
	int	y;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	if ( Item->Variable )
	{
		switch(*(int*)(Item->Variable ))
		{
			case 0:
						Print4x5Text( "DISABLED", -1 , y , 1 );
						break;
			case 1:
						Print4x5Text( "TOM CRUISE", -1 , y , 1 );
						break;
			case 2:
						Print4x5Text( "GUTS", -1 , y , 1 );
						break;
		}
	}
}

/*===================================================================
	Procedure	:		Select a toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectColourToggle( MENUITEM *Item )
{
		if(*(int *) Item->Variable > 7)
			*(int *) Item->Variable = -1;
		else
			*(int *) Item->Variable =*(int *)Item->Variable + 1;
}

/*===================================================================
	Procedure	:		Draw message display customisation toggle menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawMessagesToggle( MENUITEM *Item )
{
	int	x;
	int	y;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	Print4x5Text( Item->StrPnt , x , y , InGameMenuColour );
	if ( Item->Variable )
	{
		switch(*(int*)(Item->Variable ))
		{
			case -1:
						Print4x5Text( LT_DISABLED, -1 , y , 1 );
						break;
			case 0:
						Print4x5Text( LT_WHITE, -1 , y , 1 );
						break;
			case 1:
						Print4x5Text( LT_RED, -1 , y , 1 );
						break;
			case 2:
						Print4x5Text( LT_LIGHTGREEN, -1 , y , 1 );
						break;
			case 3:
						Print4x5Text( LT_BLUE, -1 , y , 1 );
						break;
			case 4:
						Print4x5Text( LT_YELLOW, -1 , y , 1 );
						break;
			case 5:
						Print4x5Text( LT_LIGHTBLUE, -1 , y , 1 );
						break;
			case 6:
						Print4x5Text( LT_PINK, -1 , y , 1 );
						break;
			case 7:
						Print4x5Text( LT_TURQUOISE, -1 , y , 1 );
						break;
			case 8:
						Print4x5Text( LT_GREY, -1 , y , 1 );
						break;
		}

	}
}


/*===================================================================
	Procedure	:		Select a key menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectKey( MENUITEM *Item )
{	
	MenuState = MENUSTATE_Keydef;
	KeyItem = Item;

}



/*===================================================================
	Procedure	:		Draw a key menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawKey( MENUITEM *Item )
{
	VirtualKeycode keycode;
	char *keyword;
	int	x;
	int	y;
	int colour;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	if ( Item->Variable )
	{
		keycode = *(VirtualKeycode *)(Item->Variable);
		if ( MenuState == MENUSTATE_Keydef && Item == KeyItem )
		{
			colour = SelectionColour();
		}
		else
		{
			colour = 1;
		}
		if ( keycode )
		{
			keyword = (char *) key_name( keycode );
			if ( keyword )
			{
				Print4x5Text( keyword, x , y , colour );
			}
		}
		else
		{
			if ( MenuState == MENUSTATE_Keydef && Item == KeyItem )
			{
				Print4x5Text( LT_Undef/*"undef"*/, x , y , colour );
			}
		}
	}
 
}



/*===================================================================
	Procedure	:		Select a keydef menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectKeyDef( MENUITEM *Item )
{	
	MenuState = MENUSTATE_Keydef;
	KeyItem = Item;
	PlaySfx( SFX_InGameMenuSelect, 1.0F );
}



/*===================================================================
	Procedure	:		Draw a keydef menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawKeyDef( MENUITEM *Item )
{
	DEFKEY *keydef;
	VirtualKeycode keycode;
	char *keyword;
	int	x;
	int	y;
	int colour;
	static char place[ 80 ];
	int page;

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	page = (int) Item->Value;
	if ( Item->Variable && page == CurrentPage )
	{
		keydef = (DEFKEY *)(Item->Variable);
		keycode = keydef->def->key[ keydef->selected_key ];
		if ( MenuState == MENUSTATE_Keydef && Item == KeyItem )
		{
			colour = SelectionColour();
		}
		else
		{
			colour = 1;
		}
		if ( keydef->def->keys > 1 )
		{
			sprintf( place, "[%d/%d]", keydef->selected_key + 1, keydef->def->keys );
			Print4x5Text( place, x - 8 * FontWidth, y, colour );
		}
		if ( keycode )
		{
			keyword = (char *) key_name( keycode );
			if ( keyword )
			{
				Print4x5Text( keyword, x , y , colour );
			}
		}
		else
		{
			if ( MenuState == MENUSTATE_Keydef && Item == KeyItem )
			{
				Print4x5Text( LT_Undef, x , y , colour );
			}
		}
	}
 
}

/*===================================================================
	Procedure	:		Save values from the detail level menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/

extern bool InitView(void);
extern void ReleaseView(void);
void ExitDetailLevels( MENU *Menu )
{
	SetGamePrefs();
}

/*===================================================================
	Procedure	:		Initialise the biker menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void InitBikerMenu( MENU *Menu )
{
	Config = *player_config;
	Config.bike = ValidBikeSelected( Config.bike );
	SelectedBike = Config.bike;
	InitPilotMenu( Menu );
}

void InitBikeComputerMenu( MENU *Menu )
{
	static u_int32_t CurrentSpeechID = 0;
	BikeComputerList.selected_item = player_config->bikecomp;
	KillBikeChar( NULL );
	UpdateSfxForBikeComputer( player_config->bikecomp );
	if ( CurrentSpeechID ) StopSfx( CurrentSpeechID );
	CurrentSpeechID = PlaySfx( SFX_BIKECOMP_EX, 1.0F );
}

void NewInitBikeMenu ( MENU *Menu)
//same as above, but also stores biker name
{
	Config = *player_config;
	Config.bike = ValidBikeSelected( Config.bike );
	SelectedBike = Config.bike;
	UpdateSfxForBiker( SelectedBike );

	InitBikeList(NULL);

	SelectedBikeFrame = BikeChars[ BikeList.selected_item ];
	SelectedBikeModel = BikeModelLookup[ BikeList.selected_item ];
	SelectedBikeScale = BikeModelScale[ BikeList.selected_item ];
}

/*===================================================================
	Procedure	:		Save values from the biker menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void ExitBikerMenu( MENU *Menu )
{
//	if ( ( memcmp( &Config, player_config, sizeof( Config ) ) ) || ForceConfigSave )
	{
		ForceConfigSave = false;
		*player_config = Config;
		write_config( player_config, biker_config );
	}

	KillBikeChar( Menu );
}


void MoveConfigFile( MENU *Menu )
{
	if ( !PilotReName.text[ 0 ] )
	{
		// new name is empty
		PlaySfx( SFX_Error, 1.0F );
		return;
	}
	if( !strcasecmp( old_config, biker_config ) )
	{
		// name not changed
		return;
	}
	if ( !delete_file( old_config ) )
	{
		// error deleting old config
		PlaySfx( SFX_Error, 1.0F );
		return;
	}
}

/*===================================================================
	Procedure	:		Initialise the bike list menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void InitBikeList( MENU *Menu )
{
	BikeList.selected_item = ValidBikeSelected( SelectedBike );
	strcpy(bike_name,BikeList.item[BikeList.selected_item]);
}


// auto selects bike list and prevents box highlight.
void InitBikeListVDU( MENU *Menu )
{
	MenuState = MENUSTATE_SelectBiker;
	InitBikeList(Menu);

	AddBikeArrow( bike_name, BikeList.selected_item );

	OldSelectedBike = BikeList.selected_item;
	
	strcpy(biker_text,BikerText[BikeList.selected_item]);
	
}


void InitLevelSelectVDU( MENUITEM *Item )
{
	MenuState = MENUSTATE_SelectLevelQuick;
}
/*===================================================================
	Procedure	:		Initialise the control menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void InitControls( MENU *Menu )
{
	float sensi;
//	MENUITEM *item;

	Autoleveling = ( Config.autolevel_rate != 0.0F ) ? true : false;

	sensi = Config.mouse_x_sensitivity;
	SensitivityXSlider.value = (int) (sensi / 0.12F);
	
	sensi = Config.mouse_y_sensitivity;
	SensitivityYSlider.value = (int)(sensi / 0.12F);
	
	if ( SensitivityXSlider.value < SensitivityXSlider.min )
		SensitivityXSlider.value = SensitivityXSlider.min;

	if ( SensitivityYSlider.value < SensitivityYSlider.min )
		SensitivityYSlider.value = SensitivityYSlider.min;

	if ( SensitivityXSlider.value > SensitivityXSlider.max )
		SensitivityXSlider.value = SensitivityXSlider.max;

	if ( SensitivityYSlider.value > SensitivityYSlider.max )
		SensitivityYSlider.value = SensitivityYSlider.max;

}

void SetAutolevel( MENUITEM *item )
{
	Config.autolevel_rate = ( Autoleveling ) ? 0.05F : 0.0F;
}


/*===================================================================
	Procedure	:		Save values from the controls menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void ExitControls( MENU *Menu )
{
	float sensiX, sensiY;
	float halfrange;

	halfrange = ( SensitivityXSlider.max - SensitivityXSlider.min + 1 ) * 0.5F;
	sensiX = 1.0F + ( SensitivityXSlider.value - halfrange ) / halfrange;

	halfrange = ( SensitivityYSlider.max - SensitivityYSlider.min + 1 ) * 0.5F;
	sensiY = 1.0F + ( SensitivityYSlider.value - halfrange ) / halfrange;
	
	Config.mouse_x_sensitivity = sensiX;
	Config.mouse_y_sensitivity = sensiY;

	ExitBikerMenu( NULL );
}




/*===================================================================
	Procedure	:		compare string function for sorting pilot list
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
static int compare( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   return strcasecmp( ( char* ) arg1, ( char* ) arg2 );
}

#ifdef DEMO_SUPPORT

static int
_cdecl CompareDemoDate( const void *arg1, const void *arg2 )
{
	FILETIME date;
	HANDLE hfile;
	char *demo;	 
	LONGLONG longdate1, longdate2;

	// get creation date of file 1
	demo = DemoFileName( (char *)arg1 );
	hfile = CreateFile( demo,	// pointer to name of the file 
						GENERIC_READ,	// read mode 
						FILE_SHARE_READ,	// share mode 
						NULL,	// pointer to security descriptor 
						OPEN_EXISTING,	// how to create 
						FILE_ATTRIBUTE_NORMAL,	// file attributes 
						NULL 	// handle to file with attributes to copy  
						);
	if ( hfile != INVALID_HANDLE_VALUE )
	{
		GetFileTime( hfile,	NULL,  NULL, &date );
		longdate1 = ( LONGLONG )date.dwHighDateTime;
		longdate1 <<= 32;
		longdate1 |= ( LONGLONG )date.dwLowDateTime;
	}else
	{
		return 0;
	}
	CloseHandle(hfile);

	// get creation date of file 1
	demo = DemoFileName( (char *)arg2 );
	hfile = CreateFile( demo,	// pointer to name of the file 
						GENERIC_READ,	// read mode 
						FILE_SHARE_READ,	// share mode 
						NULL,	// pointer to security descriptor 
						OPEN_EXISTING,	// how to create 
						FILE_ATTRIBUTE_NORMAL,	// file attributes 
						NULL 	// handle to file with attributes to copy  
						);
	if ( hfile != INVALID_HANDLE_VALUE )
	{
		GetFileTime( hfile,	NULL,  NULL, &date );
		longdate2 = ( LONGLONG )date.dwHighDateTime;
		longdate2 <<= 32;
		longdate2 |= ( LONGLONG )date.dwLowDateTime;
	}else
	{
		return 0;
	}
	CloseHandle(hfile);

	if ( longdate1 < longdate2 )
		return 1;

	if ( longdate1 > longdate2 )
		return -1;

	return 0;
}

#endif // DEMO_SUPPORT

/*===================================================================
	Procedure	:		Initialise the pilot list
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitPilotList( void )
{
	int j;
	char *fname, *bname;

	// set PilotsList to defaults
	PilotList.items = 0;
	PilotList.top_item = 0;
	PilotList.selected_item = -1;
	if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
		PilotList.display_items = 16;
	else
		PilotList.display_items = 8;

	if(!(fname = find_file( "pilots\\*.txt" )))
		return;

	// for each found cfg
	do{

        // the next empty pilot slot to fill in
		bname = PilotList.item[PilotList.items];

		// size of the pilot name
		j = strlen( fname ) - strlen( ".txt" );

		// if pilot name size is within boundaries
		if ( j > 0 && j < MAX_PLAYER_NAME_LENGTH )
		{
			// set the pilot name
			strncpy( bname, fname, j );

			// set end of c string
			bname[ j ] = 0;

			// up the count of pilots
			PilotList.items++;
		}

	// get the next file
	}while(	(fname = find_next_file()) );

	// sort the pilot list
	qsort( (void *)PilotList.item, (size_t) PilotList.items, sizeof( PilotList.item[ 0 ] ), compare );

	// for each pilot
	for ( j = 0; j < PilotList.items; j++ )
	{
		// 
		if ( !strcasecmp( PilotList.item[ j ], biker_name ) )
			PilotList.selected_item = j;
	}

	// if selected pilot is ????
	if ( PilotList.selected_item >= PilotList.top_item + PilotList.display_items )

		// ???
		PilotList.top_item = PilotList.selected_item - PilotList.display_items + 1;

	// close the find handle
	find_close();

	// delete function
	PilotList.FuncDelete = DeletePilot;

}

/*===================================================================
	Procedure	:		Initialise the pilot name
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitPilotName( MENU *menu )
{

	// back up current config
	strcpy( old_config, biker_config );

	// copy in default name
	strcpy( PilotName.text, DEFAULT_NEWNAME );

	//
	PilotName.insert_pos = strlen( PilotName.text );

	// set play name length limitation
	PilotName.limit_length = MAX_PLAYER_NAME_LENGTH - 1;

	//
	KillBikeChar( NULL );

}


/*===================================================================
	Procedure	:		Save values from the pilot name
	Input		:		pointer to pilot name menu item
	Output		:		Nothing
===================================================================*/

void SetPilotName( MENUITEM *item )
{
	FILE *f;
	u_int16_t tempbike;
	static char filepath[ 255 ];

	// abort if pilot name empty
	if ( !strlen( PilotName.text ) )
		return;

	// path to the cfg file
	sprintf( filepath, "pilots\\%s.txt", PilotName.text );

	// open the new/existing file to cfg open
	f = file_open( filepath, "r" );

	// if cfg found report error
	// since we want to create a new pilot
	if ( f )
	{

		// close it
		fclose( f );

		// play an error sound
		PlaySfx( SFX_Error, 1.0F );

		// file allready exists !!!
		return;

	}

	//
	InitBikerName( PilotName.text );

	// set the default config
	*player_config = default_config;

	// set the pilot name
	strcpy( player_config->name, PilotName.text );

	// re-initialize joystick to remove any existing settings
	ReInitJoysticks();

	// set the default joystick settings
	DefaultJoystickSettings( player_config );

	//
	Config = *player_config;

	//
	Config.bike = ValidBikeSelected( Config.bike );

	// write the cfg file
	write_config( player_config, biker_config );

	// 
	tempbike = SelectedBike;

	//
	SelectedBike = Config.bike;
	
	// if config bike and selected bike are not the same
	// then swap displayed bike
	if ( tempbike != SelectedBike )
	{

		//
		GetBikeDetails(SelectedBike, NULL );

		// make a biker sound
		UpdateSfxForBiker( SelectedBike );

		// show the bike model
		ShowHoloModel( BikeModelLookup[ SelectedBike ] );

	}
	
	// re-initialize the biker list
	InitPilotList();

	// were done back out of menu
	if (CurrentMenu == &MENU_NEW_NewPlayer)
		MenuBack();

}


/*===================================================================
	Procedure	:		Save values from the pilot name
	Input		:		pointer to pilot name menu item
	Output		:		Nothing
===================================================================*/
void SetPilotNameInGame( MENUITEM *item )
{
	FILE *f;
	static char fname[ 255 ];

	if ( !strlen( PilotNameInGame.text ) )
		return; // abort if pilot name empty

	sprintf( fname, "pilots\\%s.txt", PilotNameInGame.text );

	f = file_open( fname, "r" );
	if ( f )
	{
		fclose( f );
		PlaySfx( SFX_Error, 1.0F );
		return; // config file already exists with that name (should display error message)
	}

	InitBikerName( PilotNameInGame.text );
	*player_config = default_config;
	strcpy( player_config->name, PilotNameInGame.text );
	ReInitJoysticks();
	DefaultJoystickSettings( player_config );
	Config = *player_config;
	Config.bike = ValidBikeSelected( Config.bike );
	write_config( player_config, biker_config );
	SelectedBike = Config.bike;
	GetBikeDetails(SelectedBike, NULL );
	UpdateSfxForBiker( SelectedBike );
	
	InitPilotList();
}


/*===================================================================
	Procedure	:		Initialise the pilot name
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitPilotReName( MENU *menu )
{
	strcpy( old_config, biker_config );
	strcpy( PilotReName.text, DEFAULT_NEWNAME );
	PilotReName.insert_pos = strlen( PilotReName.text );
	PilotReName.limit_length = MAX_PLAYER_NAME_LENGTH - 1;

	KillBikeChar( NULL );
}


/*===================================================================
	Procedure	:		Save values from the pilot name
	Input		:		pointer to pilot name menu item
	Output		:		Nothing
===================================================================*/
void RenamePilotName( MENUITEM *item )
{
	FILE *f;
	u_int16_t tempbike;
	static char fname[ 255 ];

	if ( !strlen( PilotReName.text ) )
		return; // abort if pilot name empty

	sprintf( fname, "pilots\\%s.txt", PilotReName.text );

	f = file_open( fname, "r" );
	if ( f )
	{
		fclose( f );
		PlaySfx( SFX_Error, 1.0F );
		return; // config file already exists with that name (should display error message)
	}
	InitBikerName( PilotReName.text );
	strcpy( player_config->name, PilotReName.text );
	Config = *player_config;
	Config.bike = ValidBikeSelected( Config.bike );
	write_config( player_config, biker_config );
	tempbike = SelectedBike;
	SelectedBike = Config.bike;
	
	if ( tempbike != SelectedBike )
	{
		// need to swap displayed bike
		GetBikeDetails(SelectedBike, NULL );
		UpdateSfxForBiker( SelectedBike );
		ShowHoloModel( BikeModelLookup[ SelectedBike ] );
	}
	
	InitPilotList();

	if (CurrentMenu == &MENU_NEW_NewPlayer)
		MenuBack();
}


/*===================================================================
	Procedure	:		Initialise the pilot menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void InitPilotMenu( MENU *Menu )
{
	InitPilotName(NULL);
	InitPilotList();
}

// as above, but auto selects list and highlights current player.
void InitVDUPilotMenu ( MENU *Menu )
{
  
	MENUITEM *Item;
	

	for( Item = Menu->Item ; Item->x >= 0 ; Item++ )
	{	
		if (Item->Variable == &PilotList)
			CurrentMenuItem = Item;
	}
	
	MenuState = MENUSTATE_SelectPlayer;
	InitPilotList();
}

void GetBikeDetails(int Bike, MENUITEM *item)
{

	SelectedBikeFrame = BikeChars[ Bike ];
	SelectedBikeModel = BikeModelLookup[ Bike ];
	SelectedBikeScale = BikeModelScale[ Bike ];
	strcpy(bike_name,BikeList.item[ Bike ]);
	if ( WhoIAm != (u_int8_t) -1 )
		Ships[ WhoIAm ].BikeNum = Bike;
}

/*===================================================================
	Procedure	:		Save values from the pilot menu items
	Input		:		pointer to menu
	Output		:		Nothing
===================================================================*/
void SelectPilot( MENUITEM *item )
{
	if ( PilotList.selected_item >= 0 && PilotList.selected_item < PilotList.items )
	{
		InitBikerName( PilotList.item[ PilotList.selected_item ] );
		read_config( player_config, biker_config );

		if ( player_config->bike > ( BikeList.items - 1 ) )
		{
			player_config->bike = 0;
		}		

		Config = *player_config;
		Config.bike = ValidBikeSelected( Config.bike );
		SelectedBike = Config.bike;
		if (SelectedBike > ( BikeList.items - 1 ) )
			SelectedBike = 0;
		UpdateSfxForBiker( SelectedBike );

   
		GetBikeDetails(SelectedBike, item);
	}
}

void SelectTeamList( MENUITEM *Item )
{
	MENUITEM *NewItem;
	
	TeamNumber[WhoIAm] = *((int *)Item->Value);

	// move cursor to ready button...
	for( NewItem = CurrentMenu->Item ; NewItem->x >= 0 ; NewItem++ )
	{	if( NewItem->FuncDraw == DrawReadyButton )
		{  
			CurrentMenuItem = NewItem;
			break;
		}
	}
}

/*===================================================================
	Procedure	:		Selects a list menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectList( MENUITEM *Item )
{
	PreListItem = Item;
	PreListMenu = CurrentMenu;
	CurrentList = (LIST *)(Item->Variable);
	CurrentList->Static = false;
	CurrentListItem = Item;
	if( CurrentList->items == 0 )
	{
		CurrentList = NULL;
		return;
	}
	MenuState = MENUSTATE_List;
	OriginalSelection = CurrentList->selected_item;
	if ( CurrentList->selected_item >= 0 && CurrentList->selected_item < CurrentList->items )
	{
		if ( CurrentList->selected_item < CurrentList->top_item
			|| CurrentList->selected_item >= CurrentList->top_item + CurrentList->display_items )
		{
			if ( CurrentList->items <= CurrentList->display_items )
				CurrentList->top_item = 0;
			else if ( CurrentList->selected_item + CurrentList->display_items <= CurrentList->items )
				CurrentList->top_item = CurrentList->selected_item;
			else
				CurrentList->top_item = CurrentList->items - CurrentList->display_items + 1;
		}
	}
	SearchKey( 0 ); // empty search string

}


/*===================================================================
	Procedure	:		Draw a list menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawList( MENUITEM *Item )
{
	int x;
	int y;
	LIST *l;
	int j;
	int it;
	int colour;

	l = (LIST *)(Item->Variable);

	if ( l->FuncInfo )
		l->FuncInfo( l );

	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	if ( !l )
		return;

	Print4x5Text( Item->StrPnt, x , y , InGameMenuColour );
	x += (int) ( ( 16 >> 1 ) * ModeScaleX );
	for ( j = 0; j < l->display_items; j++ )
	{
		it = l->top_item + j;
		if ( it >= l->items )
			break;
		if ( it == l->selected_item )
			colour = ( MenuState == MENUSTATE_List && CurrentList == l ) ? ( SelectionColour() ) : 1;
		else
			colour = InGameMenuColour;
		y += (int) ( (16 >> 1) * ModeScaleY );
		Print4x5Text( l->item[ it ], x , y , colour );
	}
}



/*===================================================================
	Procedure	:		List navigation/selection routines
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void SelectListPrev( LIST *l )
{
	if ( l->selected_item > 0 )
		l->selected_item--;
	if ( l->selected_item < l->top_item )
		l->top_item = l->selected_item;
}

void SelectListNext( LIST *l )
{
	if ( l->selected_item < l->items - 1 )
		l->selected_item++;
	if ( l->selected_item >= l->top_item + l->display_items )
		l->top_item = l->selected_item - l->display_items + 1;
}

void SelectListNextPage( LIST *l )
{
	if ( l->selected_item + l->display_items - 1 < l->items - 1 )
	{
		l->selected_item += l->display_items - 1;
		if ( l->items <= l->display_items )
			l->top_item = 0;
		else if ( l->top_item + l->display_items - 1 < l->items - 1 )
			l->top_item += l->display_items - 1;
		else
			l->top_item = l->items - l->display_items + 1;
	}
	else
	{
		l->selected_item = l->items - 1;
		if ( l->selected_item >= l->top_item + l->display_items )
			l->top_item = l->selected_item - l->display_items + 1;
	}
}

void SelectListPrevPage( LIST *l )
{
	if ( l->selected_item - ( l->display_items - 1 ) > 0 )
	{
		l->selected_item -= l->display_items - 1;
		if ( l->items <= l->display_items )
			l->top_item = 0;
		else if ( l->top_item - ( l->display_items - 1 ) > 0 )
			l->top_item -= l->display_items - 1;
		else
			l->top_item = 0;
	}
	else
	{
		l->selected_item = 0;
		if ( l->selected_item < l->top_item )
			l->top_item = l->selected_item;
	}
}

void SelectListHome( LIST *l )
{
	l->selected_item = 0;
	if ( l->selected_item < l->top_item )
		l->top_item = l->selected_item;
}

void SelectListEnd( LIST *l )
{
	l->selected_item = l->items - 1;
	if ( l->selected_item >= l->top_item + l->display_items )
		l->top_item = l->selected_item - l->display_items + 1;
}


void SelectListSearch( LIST *l, char *keyword )
{
	int next;
	int wrap;
	int item;
	int keyword_len;

	if ( !keyword )
		return;
	keyword_len = strlen( keyword );
	if ( !keyword_len )
		return;
	next = -1;
	wrap = -1;
	for ( item = 0; item < l->items; item++ )
	{
		if ( ( item >= l->selected_item ) && ( next == -1 ) && !strncasecmp( l->item[ item ], keyword, keyword_len ) )
		{
			next = item;
		}
		if ( ( item < l->selected_item ) && ( wrap == -1 ) && !strncasecmp( l->item[ item ], keyword, keyword_len ) )
		{
			wrap = item;
		}
	}
	if ( next != -1 )
	{
		l->selected_item = next;
	}
	else if ( wrap != -1 )
	{
		l->selected_item = wrap;
	}
	else
	{
		SearchKey( 0 );
	}
	if ( l->items <= l->display_items )
		l->top_item = 0;
	else if ( l->selected_item < l->top_item )
		l->top_item = l->selected_item;
	else if ( l->selected_item >= l->top_item + l->display_items )
	{
		if ( l->selected_item + l->display_items < l->items )
			l->top_item = l->selected_item;
		else
			l->top_item = l->items - l->display_items + 1;
	}
}

void CancelListSelection( LIST *l )
{
//	MENUITEM *NewItem;

	l->selected_item = OriginalSelection;
	if ( l->selected_item > 0 && l->selected_item < l->items )
	{
		if ( l->selected_item < l->top_item )
			l->top_item = l->selected_item;
		if ( l->selected_item >= l->top_item + l->display_items )
			l->top_item = l->selected_item - l->display_items + 1;
	}
/*
	// dirty hack!!!
	if (CurrentMenu == &MENU_NEW_Team)
	{
		for( NewItem = CurrentMenu->Item ; NewItem->x >= 0 ; NewItem++ )
		{	if(( CurrentMenuItem->Variable == NewItem->Variable ) && (CurrentMenuItem != NewItem))
			{  
				CurrentMenuItem = NewItem;
				break;
			}
		}
	}
*/
}



/*===================================================================
	Procedure	:		Selects a text menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/

void MenuEnterTextInputState(int state)
{
	MenuState = state;
// TODO is this right ?
#if SDL_VERSION_ATLEAST(2,0,0)
	SDL_StartTextInput();
#else
	if(!SDL_EnableUNICODE(-1))
		SDL_EnableUNICODE(1);
#endif
}

void SelectText( MENUITEM *Item )
{
	TEXT *t;
	MenuEnterTextInputState(MENUSTATE_Text);
	TextItem = Item;
	t = (TEXT *)(Item->Variable);
	OriginalText = *t;
	t->insert_pos = strlen( t->text );
}

void SelectFlatMenutext( MENUITEM *Item )
{
	TEXT *t;
	t = (TEXT *)(Item->Variable);
	SelectText ( Item );
	MenuEnterTextInputState(MENUSTATE_Text2);
	t->VDU_insert_pos = 0;
	TextType (t, 0);	// ensures VDU cursor is placed in correct position
	Item->TextInfo[0]->highlighttype = HIGHLIGHT_Static;
}


/*===================================================================
	Procedure	:		Draw a text menuitem
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void DrawTextItem( MENUITEM *Item )
{
	int x;
	int y;
	TEXT *t;
	int tx;
	int colour;
	char MessageBuff[150];
	int z,MAX;

	// calculate the start position for the text
	x = (int) ( ( Item->x >> 1 ) * ModeScaleX );
	y = (int) ( ( Item->y >> 1 ) * ModeScaleY );

	// print the initial message (e.g., "whisper message: ") and get
	// the start x position for the user text
	tx = Print4x5Text( Item->StrPnt, x , y , InGameMenuColour );

	// get the text message
	t = (TEXT *)(Item->Variable);

	// get the maximum number of characters we can print on a line
	MAX = floor((render_info.window_size.cx-tx-(FontWidth*3.0))/FontWidth);
	if(MAX < 10) MAX = 10;
	else if(MAX > 149) MAX = 149;

	// display each line of text
	for(z=0; z*MAX < t->insert_pos; z++)
	{
		strncpy(&MessageBuff[0],&t->text[MAX*z],MAX); 
		Print4x5Text( &MessageBuff[0], tx, y + (z*(FontHeight+16)), 1);
	}

	// show the flashing cursor at the end of the text
	if(z > 0) z--;
	if ( MenuState == MENUSTATE_Text && TextItem == Item )
	{
		char hilite[2] = { 0, 0 };
		x = tx + ((t->insert_pos - z*MAX)  *FontWidth);
		hilite[ 0 ] = t->text[ t->insert_pos ];
		if ( !hilite[ 0 ] )
			hilite[ 0 ] = '0';
		colour = SelectionColour();
		Print4x5Text( hilite, x, y + (z*(FontHeight+16)), colour );
	}
}


/*===================================================================
	Procedure	:		Text editing routines
	Input		:		MENUITEM * Item...
	Output		:		Nothing
===================================================================*/
void GetVduTextFormattingInfo (TEXT *t)
{
	t->VDU_insert_pos = 0;
	
	TextItem->TextInfo[1]->text = t->text;
	PrintTextItem( TextItem->TextInfo[1] );

	while ((TextItem->TextInfo[1]->endpoint[0] - TextItem->TextInfo[1]->startpoint[0]) < t->insert_pos - t->VDU_insert_pos - 1)
	{
		t->VDU_insert_pos++;
		TextItem->TextInfo[1]->text = &t->text[t->VDU_insert_pos];
		PrintTextItem( TextItem->TextInfo[1] );
	}

	// ensure text is displayed instantly...
	TextItem->TextInfo[1]->currentchar[0] = TextItem->TextInfo[1]->endpoint[0] - TextItem->TextInfo[1]->startpoint[0] + 1;
}


void TextLeft( TEXT *t )
{
	if ( t->insert_pos > 0 )
		t->insert_pos--;
}

void TextRight( TEXT *t )
{
	if ( t->insert_pos < (int) strlen( t->text ) )
		t->insert_pos++;
}

void TextType( TEXT *t, char c )
{
	int len;

	len = strlen( t->text );
	if ( t->FuncType )
		c = t->FuncType( t, c );
	if ( c && ( len < (int)(sizeof( t->text )-2) )
		&& ( !t->limit_length || ( len < t->limit_length ) ) )
	{
		memmove( t->text + t->insert_pos + 1, t->text + t->insert_pos, strlen( t->text ) - t->insert_pos + 1);
		t->text[ t->insert_pos ] = c;
		t->insert_pos++;
	}
}

void TextBackspace( TEXT *t )
{
	if ( t->insert_pos > 0 )
	{
		memmove( t->text + t->insert_pos - 1, t->text + t->insert_pos, strlen( t->text ) - t->insert_pos + 1 );
		t->insert_pos--;
	}

}

void TextDelete( TEXT *t )
{
	memmove( t->text + t->insert_pos, t->text + t->insert_pos + 1, strlen( t->text ) - t->insert_pos );
}

void TextClear( TEXT *t )
{
	t->text[ 0 ] = 0;
	t->insert_pos = 0;

}

void TextHome( TEXT *t )
{
	t->insert_pos = 0;
}

void TextEnd( TEXT *t )
{
	t->insert_pos = strlen( t->text );
}

void TextEnter( MENUITEM *item )
{
	TEXT *t;

	t = (TEXT *)(TextItem->Variable);
	if ( t->FuncSet )
	{
		t->FuncSet( item );
	}
}

void TextCancel( TEXT *t )
{
	*t = OriginalText;

	if (MenuState == MENUSTATE_Text2)
	{
		TextItem->TextInfo[1]->text = t->text;
		PrintTextItem( TextItem->TextInfo[1] );
	}
}

void InitBikerName( char *name )
{

	// copy biker name into biker_name
	strncpy( biker_name, name, sizeof( biker_name ) );

	// set the last character to end of c string
	biker_name[ sizeof( biker_name ) - 1 ] = 0;

	// set the biker config
	sprintf( biker_config, "pilots\\%s.txt", name );

	// set the player name in registry
	config_set_str("PlayerName",&biker_name[0]);

	// tell everyone else in multiplayer
	set_my_player_name();

	// tell everyone in the game that my player name has changed
	network_set_player_name(&biker_name[0]);

}


/********************************************\
|
|  Sets up the default selected pilot
|    Looks in registry for last used biker.
|    Then looks for first cfg file found.
|
\********************************************/


void GetDefaultPilot(void)
{

	// pointer to a char
	char * strptr;

	// holds the pilot name
	char pilot_name[256] = "";

	// size
	int size = sizeof( pilot_name );

	// did we find a player name ?
	bool found = false;

	config_get_strncpy( &pilot_name[0], size, "PlayerName", "" );

	// get the PlayerName from registry
	if ( strlen(pilot_name) )

		// found
		found = true;

	// if failed to get registry setting
	else
	{
		char * name;
		if( (name = find_file( "pilots\\*.txt" )))
		{
			strncpy( &pilot_name[0], name, sizeof(pilot_name) );

			// remove the .cfg
			strptr = strrchr( pilot_name , '.' );
			if ( strptr != NULL )
				*strptr = 0;

			// found
			found = true;

			// close the find handle
			find_close();
		}
	}
	
	// if found
	if ( found )

		// initialize this biker
		InitBikerName( pilot_name );

	// read in the biker config
	read_config( player_config, biker_config );

	// if the bike selected is not valid
	if ( player_config->bike > ( BikeList.items - 1 ) )

		// default it to the first bike
		player_config->bike = 0;

	// set the config
	Config = *player_config;

	// set the bike selected
	Config.bike = ValidBikeSelected( Config.bike );

	// set the bike selected
	SelectedBike = Config.bike;

	// if the bike selected is not valid
	if (SelectedBike > ( BikeList.items - 1 ) )

		// default it to the first bike
		SelectedBike = 0;

}

void InitStartMenu( MENU *Menu )
{
}


void ExitInGameMenu( MENU *Menu )
{
	input_grab( true );
}


void InitInGameMenu( MENU *Menu )
{
	MENUITEM *item;

	if ( ! render_info.fullscreen )
		input_grab( false );

	for ( item = Menu->Item; item->x >= 0; item++ )
	{
		if(!item)
			continue;
		if ( item->Value == &MENU_LoadSavedGame || item->Value == &MENU_SaveGame )
		{
			if ( MyGameStatus != STATUS_Normal ) // normal -> multiplayer game, so !normal -> single player
			{
				item->FuncSelect = MenuChange;
				item->FuncDraw = MenuItemDrawName;
			}
			else
			{
				item->FuncSelect = NULL;
				item->FuncDraw = NULL;
			}
		}

		else if ( item->Value == &MENU_Save || item->Value == &MENU_DebugMode )
		{
			if ( DebugInfo )
			{
				item->FuncSelect = MenuChange;
				item->FuncDraw = MenuItemDrawName;
			}
			else
			{
				item->FuncSelect = NULL;
				item->FuncDraw = NULL;
			}
		}

		if ( item->FuncSelect == SelectQuitCurrentGame )
				item->StrPnt = LT_QuitToMainMenu/*"Quit to Main Menu"*/;

		if ( item->Value == &MENU_LevelSelect )
		{
			item->FuncSelect = MenuChange;
			item->FuncDraw = MenuItemDrawName;
		}


		if ( item->FuncSelect == GoToStats )
		{
			item->FuncSelect = NULL;
			item->FuncDraw = NULL;
		}

		// only display host options if you are the host
		if ( item->Value == &MENU_Host_Options )
		{
			if ( IsHost )
			{
				item->FuncSelect = MenuChange;
				item->FuncDraw = MenuItemDrawName;
			}
			else
			{
				item->FuncSelect = NULL;
				item->FuncDraw = NULL;
			}
		}
	}
}

void InitDebugMode( MENU *Menu )
{
	MENUITEM *item;
	for ( item = Menu->Item; item->x >= 0; item++ )
	{
		if (
			item->Value == &ScreenSaving ||
			item->Value == &OldNodeCube ||
			item->Value == &NodeCube ||
			item->Value == &NodeCubeType 
		)
		{
			if ( DebugInfo )
			{
				item->FuncSelect = SelectToggle;
				item->FuncDraw = DrawToggle;
			}
			else
			{
				item->FuncSelect = NULL;
				item->FuncDraw = NULL;
			}
		}
	}
}



char *SearchKey( char c )
{
	static char search_key[ 256 ] = "";
	static int len = 0;
	static int last_time = 0;
	int this_time;

	this_time = SDL_GetTicks();
	if ( this_time - last_time > 300 )
	{
		len = 0;
	}
	if ( c )
	{
		search_key[ len ] = c;
		search_key[ len + 1 ] = 0;
		len++;
	}
	else
	{
		len = 0;
		search_key[ 0 ] = 0; // kill search key
	}
	last_time = this_time;

	return ( search_key[ 0 ] ) ? search_key : NULL;
}


void SelectQuit( MENUITEM *Item )
{	
	if(IsHost && tracker_enabled)
		send_tracker_finished( tracker_server, tracker_port );
	SetGamePrefs();
	MyGameStatus = STATUS_QuittingFromTitles;
	MenuAbort();
}


void InitMultiplayerHost( MENU *Menu )
{
	int i;

	LevelList.items = 0;
	LevelList.top_item = 0;
	LevelList.display_items = 8;
	LevelList.selected_item = 0;
	NewLevelNum = 0;

    for (i = 0; i < NumLevels; i++)
	{
		strcpy( (char*) &LevelList.item[LevelList.items][0] , (char*) &ShortLevelNames[i][0] );
		LevelList.items++;
    }

}

/* init */
void InitMultiplayerHostVDUPeerPeer( MENU *Menu )
{
	int selected_level = -1;
	int i;
	char level_name[64];

	config_get_strncpy( &level_name[0], 64, "LevelName", "ship" );

	if(level_name || (strlen(level_name) != 0) )
	{
		DebugPrintf("level select: config says last level was %s\n",level_name);
		DebugPrintf("level select: finding level by name in %d items\n",LevelList.items);
		for ( i = 0; i < LevelList.items; i++ )
		{
			//DebugPrintf("level select: testing level %d: %s\n",i,LevelList.item[i]);
			if ( !strncasecmp( LevelList.item[ i ], level_name, 7 ) )
			{
				DebugPrintf("level select: found last level at index %d\n",i);
				selected_level = (u_int16_t) i;
				break;
			}
		}
	}
	else
	{
		DebugPrintf("level select: config contained empty/missing LevelName\n");
	}

	if(selected_level == -1)
	{
		selected_level = 0;
		DebugPrintf("level select: could not find the level defaulting to level 0\n");
	}

	DebugPrintf("level select: using level %d\n",(int)selected_level);

	// load the multiplayer levels into the global
	if ( !InitLevels( MULTIPLAYER_LEVELS ) )
	{
		Msg( "No multiplayer levels" );
		PrintErrorMessage (LT_NoLevelsInstalled, 1, NULL, ERROR_USE_MENUFUNCS );
		return;
	}

	// reset the selected level
	LevelList.selected_item = selected_level;

	// set the selected level to the last level played
	NewLevelNum = LevelList.selected_item;

	// load level names to be displayed in the list
	LoadLevelText( NULL );

	// copy in my valid pickups
	memset( PickupValid, 0, sizeof(PickupValid) );
	memmove( PickupValid, MyPickupValid, sizeof(PickupValid) );//memcpy

}

/* exit */
void ExitMultiplayerHostVDUPeerPeer ( MENU *Menu )
{
	// save any changes to multi player prefrences
	SetGamePrefs();
}

void RedrawMultiplayerHostVDUPeerPeer( int * i )
{
	strncpy( GameTypeName, GameTypeNameTable[ GameType ], sizeof(GameTypeName) );
	config_set_str( "LevelName", LevelList.item[ LevelList.selected_item ] );
}

void LoadLevelText( MENU *Menu )
{
	FILE *f;
	char *newlinechar;

	f = file_open( MissionTextNames[ NewLevelNum ], "r" );

	if ( !f )
	{
	 	SelectedLevelText[ 0 ] = 0;
		strncpy( SelectedLevel, LevelList.item[ NewLevelNum ], MAX_TEXT_LENGTH );
		return;
	}

	// read level name
	if ( !fgets( SelectedLevel, MAX_LEVEL_NAME_LENGTH, f ) )
	{
	  	// if error ( as oposed to end of file )
		if ( ferror( f ) != 0 )
		{
		 	SelectedLevel[ 0 ] = 0;
			strncpy( SelectedLevel, LevelList.item[ NewLevelNum ], MAX_TEXT_LENGTH );
			fclose( f );
			return;
		}
	}

	// remove newline char, if any
	newlinechar = strchr( SelectedLevel, '\n' );
	if ( newlinechar )
		*newlinechar = 0;
		

	// read mission text
	if ( !fgets( SelectedLevelText, MAX_LEVEL_TEXT_LENGTH, f ) )
	{
	  	// if error ( as oposed to end of file )
	  	if ( ferror( f ) != 0 )
		{		
			SelectedLevelText[ 0 ] = 0;
			fclose( f );
			return;
		}
	}

	// remove newline char, if any
	newlinechar = strchr( SelectedLevelText, '\n' );
	if ( newlinechar )
		*newlinechar = 0;

	fclose( f );
}

void GetSavedGameData( void )
{
	struct filetime ftime;
	FILE	*	fp;
	int8_t		buf[ 256 ];
	int8_t		biker[ 256 ];
	int8_t		filename[ 256 ];
	int16_t		i;
	int16_t		Hours, Minutes, Seconds;
	int16_t		KilledEnemiesNum, InitEnemiesNum;
	char *ext;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

	CurrentSavedGameDate[0] = 0;
	CurrentSavedGameLevel[0] = 0;
	CurrentSavedGameTimeLevelPlayed[0] = 0;
	CurrentSavedGameEnemiesKilled[0] = 0;

#ifdef SAVEGAME_SLOTS
	strcpy( filename, SaveGameFileName( LoadSavedGameList.selected_item ) );
#else
	sprintf(filename, "savegame\\%s", LoadSavedGameList.item[ LoadSavedGameList.selected_item ] ); 
#endif

	if ( file_time( filename, &ftime ) )
		sprintf( CurrentSavedGameDate, "saved %d-%d-%d at %2d:%02d", ftime.month, ftime.day, ftime.year, ftime.hour, ftime.minute );
	else
	{
#ifdef SAVEGAME_SLOTS
		sprintf( CurrentSavedGameDate, SavedGameInfo( LoadSavedGameList.selected_item ) );
#else
		Msg("invalid saved game file\n");
		return;
#endif
	}

	fp = file_open( filename, "r" );

	if( fp )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n\n");
		/*
		{
			fclose( fp );
#ifdef SAVEGAME_SLOTS
			sprintf( CurrentSavedGameDate, SavedGameInfo( LoadSavedGameList.selected_item ) );
			Models[ BackgroundModel[ TITLE_MODEL_MenuTVDummy ] ].Visible = 0;
			Models[ BackgroundModel[ TITLE_MODEL_MenuTV ] ].Visible = 1;
			return;
#else
			Msg( "invalid saved game file\n" );
			return;
#endif
		}
		*/

		i = 0;
		do
		{
			fread( &buf[i], sizeof(char), 1, fp );
		}
		while( buf[ i++ ] != 0 );
		i = 0;
		do
		{
			fread( &biker[i], sizeof(char), 1, fp );
		}
		while( biker[ i++ ] != 0 );

		fread( &Hours, sizeof( Hours ), 1, fp );
		fread( &Minutes, sizeof( Minutes ), 1, fp );
		fread( &Seconds, sizeof( Seconds ), 1, fp );
		fread( &InitEnemiesNum, sizeof( InitEnemiesNum ), 1, fp );
		fread( &KilledEnemiesNum, sizeof( KilledEnemiesNum ), 1, fp );

		fclose( fp );

		sprintf( CurrentSavedGameLevel, LT_LevelName/*"level name: %s"*/, GetMissionName( buf ) );
		if ( strcasecmp( biker, DEFAULT_PLAYER_NAME ) )
			sprintf( CurrentSavedGameTimeLevelPlayed, LT_LevelPlayedFor1/*"%s played for %hd mins %hd secs"*/, biker, Hours * 60 + Minutes, Seconds );
		else
			sprintf( CurrentSavedGameTimeLevelPlayed, LT_LevelPlayedFor2/*"played for %hd mins %hd secs"*/, Hours * 60 + Minutes, Seconds );
		sprintf( CurrentSavedGameEnemiesKilled, LT_EnemiesKilled/*"enemies killed: %hd of %hd"*/, KilledEnemiesNum, InitEnemiesNum );
	}else
	{
#ifndef SAVEGAME_SLOTS
		Msg("invalid saved game file\n");
		return;
#endif
	}

	ext = strstr( filename, SAVEGAME_EXTENSION );
	strcpy( ext, SAVEGAMEPIC_EXTENSION );

	if ( File_Exists( filename ) )
	{
		LoadSavedGamePic( filename );
	}else
	{
		// ensure normal tv is displayed
		Models[ BackgroundModel[ TITLE_MODEL_MenuTVDummy ] ].Visible = 0;
		Models[ BackgroundModel[ TITLE_MODEL_MenuTV ] ].Visible = 1;
	}
}

void InitTitleLoad( MENU *Menu )
{
	InitSinglePlayerGame( NULL );
	InitLoadSavedGameList( NULL );

	if ( LevelList.items ) 
	{
		// if no saved games, go to error screen
		if ( !LoadSavedGameList.items )
		{
			PrintErrorMessage (LT_NoSavedGamesFound/*"no saved games found"*/, 1, NULL, ERROR_DONTUSE_MENUFUNCS);
		}else
		{
			MenuState = MENUSTATE_SelectSavedGame;
			GetSavedGameData();
		}
	}
}

void ExitTitleLoad( MENU *Menu )
{
	// display proper tv
	Models[ BackgroundModel[ TITLE_MODEL_MenuTVDummy ] ].Visible = 0;
	Models[ BackgroundModel[ TITLE_MODEL_MenuTV ] ].Visible = 1;

}

void InitSinglePlayerGame( MENU *Menu )
{
	if ( !InitLevels( SINGLEPLAYER_LEVELS ) && !InitLevels( DEFAULT_LEVELS ) )
	{
		//Msg( "No single player levels" );
		PrintErrorMessage (LT_NoSinglePlayerLevelsInstalled, 1, NULL, ERROR_USE_MENUFUNCS );
		return;
	}
	Lives = 5;
}

void SelectQuitCurrentGame( MENUITEM *Item )
{
	// If the host is performing transfer of stuff to other players wait until hes finished before he quits..
	if( HostDuties )
	{
		if( IsHost )
		{
		 	WaitingToQuit = true;
			return;
		}
	}
	if(IsHost && tracker_enabled)
		send_tracker_finished( tracker_server, tracker_port );
	MyGameStatus = STATUS_QuitCurrentGame;
}

void AddBikeArrow( char *name, int num )
{
	int i;
	bool leftarrow = false;
	bool rightarrow = false;
	char tempbikename[16];
	char leftarrowchar[2];
	char rightarrowchar[2];

	leftarrowchar[0] = 17;
	leftarrowchar[1] = 0;
	rightarrowchar[0] = 16;
	rightarrowchar[1] = 0;

	for ( i = 0; i < BikeList.items; i++ )
	{
		if ( i < num )
		{
				leftarrow = true;
		}

		if ( i > num )
		{
				rightarrow = true;
		}
	}				   

	strcpy( tempbikename, name );

	if ( leftarrow )
	{
		strcpy( tempbikename, leftarrowchar );
		strcat( tempbikename, name );
	}

	if ( rightarrow )
	{
		strcat( tempbikename, rightarrowchar );
	}

	strcpy( name, tempbikename );
}

void ExitBikeSelection( MENUITEM * item )
{
	
#if 0
	if( ( BikeList.selected_item >= 0 ) && ( BikeList.selected_item < MAXBIKETYPES ) )
	{
		BikeList.selected_item = ValidBikeSelected( BikeList.selected_item );
		if ((CameraStatus != CAMERA_AtStart) && (OldSelectedBike != BikeList.selected_item))
		{
			if ((Title_Timers[TITLE_TIMER_SwapBikes].Status != TITLE_EVENT_TIMER_ACTIVE) && (Title_Timers[TITLE_TIMER_SwapBikes].Status != TITLE_EVENT_TIMER_FINISHED))
			{
				Title_Timers[TITLE_TIMER_SwapBikes].Status = TITLE_EVENT_TIMER_ACTIVE;
 				Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime = 0.0F;
				OldSelectedBikeScale = SelectedBikeScale;
			}else
			{	if (Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime > 0.5F)
				{	for (Event = Title_Events; Event->TriggeredBy >= 0; Event++)
					{
						if (Event->TriggeredBy == TITLE_TIMER_SwapBikes)
						{
				
							if ((Event->Status == TITLE_EVENT_STATUS_ACTIVE) || (Event->Status == TITLE_EVENT_STATUS_FINISHED))
							{
								Event->Status = TITLE_EVENT_STATUS_IDLE;

								if (Event->Status == TITLE_EVENT_STATUS_ACTIVE)
								{
									if (Event->ExitFunc)
										Event->ExitFunc(Event);
								}

								Title_Timers[TITLE_TIMER_SwapBikes].Status = TITLE_EVENT_TIMER_ACTIVE;
			 					Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime = 0.0F;
								OldSelectedBikeScale = BikeSwapScale;
							}

						}
					}
		
				}
			}
		}
	}
#endif

	if( CameraStatus != CAMERA_AtStart )
		ShowHoloModel( HoloModelLookup[ BikeList.selected_item ] );

//		SelectedBike = BikeModels[ BikeList.selected_item ];
	SelectedBike = BikeList.selected_item;
	SelectedBikeFrame = BikeChars[ BikeList.selected_item ];
	SelectedBikeModel = BikeModelLookup[ BikeList.selected_item ];
	OldSelectedBikeScale = SelectedBikeScale;
	SelectedBikeScale = BikeModelScale[ BikeList.selected_item ];
	Config.bike = SelectedBike;
	player_config->bike = Config.bike;
	ForceConfigSave = false;
	write_config( player_config, biker_config );
	strcpy(bike_name,BikeList.item[BikeList.selected_item]);
	AddBikeArrow( bike_name, SelectedBike );
	strcpy(biker_text,BikerText[BikeList.selected_item]);
	if ( WhoIAm != (u_int8_t) -1 )
	{
		Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	}
	UpdateSfxForBiker( SelectedBike );
}

void ExitBikeComputerSelection( MENUITEM * item )
{
	static u_int32_t CurrentSpeechID = 0;

	player_config->bikecomp = BikeComputerList.selected_item;
	Config.bikecomp = player_config->bikecomp;	// backward compatability
	
	UpdateSfxForBikeComputer( player_config->bikecomp );

	if ( CurrentSpeechID ) StopSfx( CurrentSpeechID );
	CurrentSpeechID = PlaySfx( SFX_BIKECOMP_EX, 1.0F );

	if( MyGameStatus == STATUS_Title )
		MenuBack();
}

/*===================================================================
	Procedure	:		Menu Go full Screen....or window....
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuGoFullScreen( MENUITEM *Item )
{
	if( MyGameStatus == STATUS_Title )
	{
		LastMenu = CurrentMenu;	
		VduClear();
	}

	bIgnoreWM_SIZE = true;

    RenderModeSelect( render_info.CurrMode, !render_info.fullscreen, render_info.vsync );

	// user goes into window mode to do something else
	// so release their inputs and let them click to activate
	if ( ! render_info.fullscreen )
		input_grab( false );

	bIgnoreWM_SIZE = false;

	if( MyGameStatus == STATUS_Title )
	{
		FadeHoloLight(HoloLightBrightness);
		DarkenRoom2(RoomDarkness);
		ProcessVduItems( CurrentMenu );
   		InitialTexturesSet = false;
	}
}
	


/*===================================================================
		Mode changing stuff..
===================================================================*/
void MakeModeList( MENU *Menu )
{
	int i;
	ModeList.items = 0;
	ModeList.top_item = 0;

	if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
		ModeList.display_items = 16;
	else
		ModeList.display_items = 8;

	ModeList.selected_item = 0;

	for( i = 0; i < render_info.NumModes; i++ )
	{
		if( ModeList.items < MAXLISTITEMS )
		{
			WhichMode[ModeList.items] = i;
			if(i==0)
				sprintf( &ModeList.item[ModeList.items][0], "default");
			else
				sprintf( &ModeList.item[ModeList.items][0], "%d x %d" ,
					render_info.Mode[i].w , render_info.Mode[i].h );

			if( (render_info.Mode[i].w == render_info.ThisMode.w) &&				
				(render_info.Mode[i].h == render_info.ThisMode.h))
			{
				ModeList.selected_item = ModeList.items;
			}
			
			ModeList.items++;
		}
	}

}

/*===================================================================
	Procedure	:		Menu Select a Mode...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void MenuSelectMode( MENU *Menu )
{
	if ( render_info.CurrMode != WhichMode[ModeList.selected_item] )
		RenderModeSelect(WhichMode[ModeList.selected_item], render_info.fullscreen, render_info.vsync);
}

void NewMenuSelectMode( MENUITEM *Item )
{
	int mode = WhichMode[ModeList.selected_item];

	LastMenu = CurrentMenu;	
	VduClear();

	if ( render_info.fullscreen )
	{
        RenderModeSelect( mode, render_info.fullscreen, render_info.vsync );
	}
	else
	{
		bIgnoreWM_SIZE = true;
        RenderModeSelect( mode, render_info.fullscreen, render_info.vsync );
		bIgnoreWM_SIZE = false;

		// user goes into window mode to do something else
		// just let them click to regain inputs
		input_grab( false );
	}

	FadeHoloLight(HoloLightBrightness);
	DarkenRoom2(RoomDarkness);
	ProcessVduItems( CurrentMenu );

   	InitialTexturesSet = false;
}

void InitDebugMenu( MENU *Menu )
{
	InitRoomList();
	if ( RoomList.selected_item >= 0 && RoomList.selected_item < RoomList.items )
		strncpy( RoomName.text, RoomList.item[ RoomList.selected_item ], sizeof( RoomName.text ) );
}


void InitRoomList( void )
{
	int j;

	if ( Mloadheader.state )
	{
		RoomList.items = Mloadheader.num_groups;
		RoomList.selected_item = -1;
		for ( j = 0; j < RoomList.items; j++ )
		{
			strncpy( RoomList.item[ j ], Mloadheader.Group[ j ].name, sizeof( RoomList.item[ j ] ) );
			if ( WhoIAm != (u_int8_t) -1 && Ships[WhoIAm].Object.Group == j )
				RoomList.selected_item = j;
		}
	}
	else
	{
		RoomList.items = 0;
		RoomList.selected_item = -1;
	}
}


void SetRoomName( MENUITEM *item )
{
	if ( !strlen( RoomName.text ) )
		return; // abort if room name empty
	if ( GotoRoom( WhoIAm, RoomName.text ) )
	{
		RoomList.selected_item = Ships[WhoIAm].Object.Group;
		AddColourMessageToQue( SystemMessageColour, "You are now in %s", RoomName.text );
	}
	else
	{
		AddColourMessageToQue( SystemMessageColour, "Could not find %s", RoomName.text );
		AddColourMessageToQue( SystemMessageColour, "You are still in %s", Mloadheader.Group[ Ships[WhoIAm].Object.Group ].name );
	}
	MenuExit();
}


void ChooseRoom( MENUITEM *item )
{
	if ( RoomList.selected_item < 0 || RoomList.selected_item >= Mloadheader.num_groups )
		return; // abort if invalid room
	if ( GotoRoom( WhoIAm, RoomList.item[ RoomList.selected_item ] ) )
	{
		strncpy( RoomName.text, RoomList.item[ RoomList.selected_item ], sizeof( RoomName.text ) );
		AddColourMessageToQue( SystemMessageColour, "You are now in %s", RoomList.item[ RoomList.selected_item ] );
	}
	else
	{
		AddColourMessageToQue( SystemMessageColour, "Could not find %s", RoomList.item[ RoomList.selected_item ] );
		AddColourMessageToQue( SystemMessageColour, "You are still in %s", Mloadheader.Group[ Ships[WhoIAm].Object.Group ].name );
	}
	MenuExit();
}

int8_t	DebuggingString[] = "Cheating";
int8_t	NotDebuggingString[] = "No Longer Cheating";

void DebugModeChanged( MENUITEM *item )
{
	if( DebugInfo )
	{
		strcpy( &MacroText4.text[0], &DebuggingString[0] );
		SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt1, 0);
		if( !GodMode ) ToggleGodMode( NULL );
	}
	else
	{
		strcpy( &MacroText4.text[0], &NotDebuggingString[0] );
		SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt1, 0);
		if( GodMode ) ToggleGodMode( NULL );
	}
	InitInGameMenu( &MENU_InGame );
}

void SavePickups( MENUITEM *item )
{
	SavePickupsPositions();
}

void ShowNodeToggle( MENUITEM *item )
{
	ShowNodeNetwork( ( ShowNode ) ? 1 : 0 );
}

void ShowStartPointsToggle( MENUITEM *item )
{
	if( ShowStartPoints )
	{
		DisplayStartPoints();
	}
	else
	{
		InitLines();
	}
}

#ifdef DEMO_SUPPORT

void InitAvgFrameRateGlobals( MENU *Menu )
{
    sprintf( DemoAvgFpsText, "Avg %.2f FPS", DemoAvgFps );
	sprintf( DemoTotalFramesText, "%d frames %d.%02d",
		(int)DemoGameLoops,
		(int) DemoTotalTime / 60, (int) DemoTotalTime % 60 );
}

void RestoreDemoSettings( void )
{
	PauseDemo = false;
	DemoSpeed.value = 8;
	DemoEyesSelect.value = 0;
	ShowNamesAnyway = false;
}

/*===================================================================
	Procedure	:		Initialise Demo Name list
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitDemoList( MENU * Menu )
{
	HANDLE h;
	WIN32_FIND_DATA DemoFiles;
	int j;
//	char *fname, *bname;
	FILE *DemoFp;

	RestoreDemoSettings();

	if ( !InitLevels( DEMO_LEVELS ) && !InitLevels( DEFAULT_LEVELS ) )
	{
		//Msg( "No demo levels" );
		PrintErrorMessage (LT_NoSinglePlayerLevelsInstalled, 1, NULL, ERROR_USE_MENUFUNCS );
		return;
	}
	DemoList.items = 0;
	DemoList.top_item = 0;
	if (CameraStatus != CAMERA_AtStart)
		DemoList.display_items = 10;
	else
		DemoList.display_items = 8;

	DemoList.selected_item = 0;
	DemoList.item[0][0] = 0;
	h = FindFirstFile( DEMOFILE_SEARCHPATH,	// pointer to name of file to search for  
						(LPWIN32_FIND_DATA) &DemoFiles );	// pointer to returned information 

	if ( h == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do{
		strncpy( DemoList.item[ DemoList.items ], DemoName( DemoFiles.cFileName ), sizeof( DemoList.item[ 0 ] ) - 1 );
		DemoList.item[ DemoList.items ][ sizeof( DemoList.item[ 0 ] ) - 1 ] = 0;

		DemoFp = file_open( DemoFileName( DemoList.item[ DemoList.items ] ), "rb" );

		if ( DemoFp )
		{
			u_int32_t mp_version;

			mp_version = ~MULTIPLAYER_VERSION;
			fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
			if ( (mp_version <= MULTIPLAYER_VERSION) && (mp_version >= DEMO_MULTIPLAYER_VERSION) )
			{
				u_int16_t CopyOfSeed1;
				u_int16_t CopyOfSeed2;
				bool RandomPickups;
				u_int32_t flags;
				u_int32_t PackedInfo[ MAX_PICKUPFLAGS ];
				u_int16_t RandomStartPosModify;
				static char buf[ 256 ];
				int i;

				fread( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
				fread( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
				fread( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
				fread( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );
				
				fread( &flags, sizeof( flags ), 1, DemoFp );
				
				fread( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );
				
				for( i = 0 ; i < 256 ; i++ )
				{
					fread( &buf[i], sizeof(char), 1, DemoFp );
					if( buf[i] == 0 )
					{
						break;
					}
				}
				
				for (i = 0; i < NumLevels; i++)
				{
					
					if( strcasecmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 ) 
					{
						break;
					}
				}
				
				if( i < NumLevels )
				{
					DemoList.items++;
				}
			}
			fclose( DemoFp );
		}
	}while(	FindNextFile( h , (LPWIN32_FIND_DATA) &DemoFiles ) && DemoList.items < MAXLISTITEMS );

	qsort( (void *)DemoList.item, (size_t) DemoList.items, sizeof( DemoList.item[ 0 ] ), CompareDemoDate );

	for ( j = 0; j < DemoList.items; j++ )
	{
		if ( !strcmp( DemoList.item[ j ], biker_name ) )
			DemoList.selected_item = j;
	}
	if ( DemoList.selected_item >= DemoList.top_item + DemoList.display_items )
		DemoList.top_item = DemoList.selected_item - DemoList.display_items + 1;

	FindClose(h);

	InitAvgFrameRateGlobals( NULL );
	DemoList.FuncDelete = ( DemoList.items > 0 ) ? DeleteDemo : NULL;
}

#endif // DEMO_SUPPORT

/*===================================================================
	Procedure	:		Initialise Load Game Name list
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void InitLoadSavedGameList( MENU * Menu )
{
#ifndef SAVEGAME_SLOTS
	char *fname, *bname;
#endif

	LoadSavedGameList.items = 0;

	if (CameraStatus != CAMERA_AtStart)	LoadSavedGameList.display_items = 10;
	else LoadSavedGameList.display_items = 8;

	LoadSavedGameList.item[0][0] = 0;
#ifdef SAVEGAME_SLOTS
	while ( LoadSavedGameList.items < MAX_SAVEGAME_SLOTS )
	{
		char *gameinfo;

		gameinfo = SavedGameInfo( LoadSavedGameList.items );
		strncpy( LoadSavedGameList.item[ LoadSavedGameList.items ], gameinfo, sizeof( LoadSavedGameList.item[ 0 ] ) - 1 );
		LoadSavedGameList.item[ LoadSavedGameList.items ][ sizeof( LoadSavedGameList.item[ 0 ] ) - 1 ] = 0;
		LoadSavedGameList.items++;
	}
	if ( LoadSavedGameList.selected_item < 0 || LoadSavedGameList.selected_item >= LoadSavedGameList.items )
		LoadSavedGameList.selected_item = 0;
	if ( Menu == &MENU_SaveGame )
	{
		int slot;

		for ( slot = 0; slot < LoadSavedGameList.items; slot++ )
		{
			if ( !SaveGameSlotUsed( ( LoadSavedGameList.selected_item + slot ) % LoadSavedGameList.items ) )
			{
				LoadSavedGameList.selected_item = ( LoadSavedGameList.selected_item + slot ) % LoadSavedGameList.items;
				break;
			}
		}
	}
	if ( LoadSavedGameList.selected_item < LoadSavedGameList.top_item )
	{
		if ( LoadSavedGameList.selected_item < LoadSavedGameList.display_items )
			LoadSavedGameList.top_item = 0;
		else
			LoadSavedGameList.top_item = LoadSavedGameList.selected_item;
	}
	else if ( LoadSavedGameList.selected_item >= LoadSavedGameList.top_item + LoadSavedGameList.display_items )
	{
		if ( LoadSavedGameList.selected_item + LoadSavedGameList.display_items > LoadSavedGameList.items )
			LoadSavedGameList.top_item = LoadSavedGameList.items - LoadSavedGameList.display_items + 1;
		else
			LoadSavedGameList.top_item = LoadSavedGameList.selected_item;
	}
#else
	fname = find_file( SAVEGAME_FOLDER "\\" SAVEGAME_FILESPEC SAVEGAME_EXTENSION );

	do
	{
		if(fname)
		{
			for ( bname = LoadSavedGameList.item[ LoadSavedGameList.items ]; fname && *fname; bname++, fname++ )
			{
				if ( *fname == 0 ) //'.' )
					break;
				*bname = *fname;
			}
			*bname = 0;
		}
		else
		{
			strcpy(LoadSavedGameList.item[ LoadSavedGameList.items ], "Empty");
		}
		fname = find_next_file();
		LoadSavedGameList.items++;
	}
	while(	LoadSavedGameList.items < MAX_SAVEGAME_SLOTS );

	qsort( (void *)LoadSavedGameList.item, (size_t) LoadSavedGameList.items, sizeof( LoadSavedGameList.item[ 0 ] ), compare );

	find_close();
#endif
//	LoadSavedGameList.FuncDelete = ( LoadSavedGameList.items > 0 ) ? DeleteSavedGame : NULL;
}

/*===================================================================
	Procedure	:		Demo Pause Compensate
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

void PauseDemoToggle( MENUITEM *Item )
{
	SelectToggle( Item );
}

/*===================================================================
	Procedure	:		Init the level select menu...
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/

void InitLevelSelect( MENU *Menu )
{
	int i;

    for (i = 0; i < NumLevels; i++)
	{
		strcpy( (char*) &LevelList.item[LevelList.items][0] , (char*) &ShortLevelNames[i][0] );
		LevelList.items++;
    }

}

void GoToStats( MENUITEM *Item )
{
	CurrentMenu = NULL;
	CurrentMenuItem = NULL;
	MenuStackLevel = 0;
	NewLevelNum++;
	if( NewLevelNum >= LevelList.items )
		NewLevelNum = 0;
	LevelNum = -1;			// Force Level change
}

/*===================================================================
	Procedure	:	Level has been Changed...
	Input		:	MENU * Menu
	Output		:	nothing
===================================================================*/
void ExitLevelSelect( MENU * Menu )
{
	CurrentMenu = NULL;
	CurrentMenuItem = NULL;
	MenuStackLevel = 0;
	NewLevelNum = LevelList.selected_item;
//	LevelNum = -1;			// Force Level change even if same level;

}

/*===================================================================
	Procedure	:		Get Game Prefs..
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
extern void InitValidPickups();
void GetGamePrefs( void )
{
	// default allow all pickups
	InitValidPickups();

	// ip / port

	config_get_strncpy( TCPAddress.text,		sizeof(TCPAddress.text),		"HostAddress",	"" );		// remote address
	config_get_strncpy( host_port_str.text,		sizeof(host_port_str.text),		"HostPort",		"2300" );	// remote port
	config_get_strncpy( local_port_str.text,	sizeof(local_port_str.text),	"LocalPort",	"2300" );	// local port

	// tracker

	config_get_strncpy( tracker_server, sizeof(tracker_server), "TrackerServer", "fly.thruhere.net" );
	tracker_port = config_get_int( "TrackerPort", 47624 );
	tracker_enabled = config_get_bool( "TrackerEnabled", true );

	// booleans
 
    TintBikeTeamColor                = config_get_bool( "TintBikeTeamColor",		true );
    MyResetKillsPerLevel             = config_get_bool( "ResetKillsPerLevel",		false );
    MyBrightShips                    = config_get_bool( "BrightShips",				false );
    MyRandomPickups                  = config_get_bool( "RandomPickups",			false );

    MissileCameraEnable              = config_get_bool( "MissileCameraEnable",		true );
    RearCameraActive                 = config_get_bool( "RearCameraActive",			true );
    BikeEnginesOn                    = config_get_bool( "BikeEnginesOn",			true );
	ShowWeaponsPossessedOnHUD		 = config_get_bool( "ShowWeaponsPossessedOnHUD", false );
	JoystickInput					 = config_get_bool( "JoystickInput", true );
	ShowClockOnHUD				     = config_get_bool( "ShowClockOnHUD", true );
	ShowKPMOnHUD				     = config_get_bool( "ShowKPMOnHUD", false );
	ShowSpeedOnHUD				     = config_get_bool( "ShowSpeedOnHUD", true );
    ShowPlayersOnHUD                 = config_get_bool( "ShowPlayersOnHUD",			true );
    ShowPlayersOnHUDbyKills          = config_get_bool( "ShowPlayersOnHUDbyKills",		true );
    ShowPlayerHealthAboveBikes       = config_get_bool( "ShowPlayerHealthAboveBikes", true );
    ShowPlayerHealthByScores         = config_get_bool( "ShowPlayerHealthByScores", true );
    ScaleFontPlayerHealthAboveBikes  = config_get_bool( "ScaleFontPlayerHealthAboveBikes", true );
    BikeExhausts                     = config_get_bool( "BikeExhausts",				true );
    BountyBonus                      = config_get_bool( "BountyBonus",				true );
    MyUseShortPackets                = config_get_bool( "UseShortPackets",			true );
    ShowTeamInfo                     = config_get_bool( "ShowTeamInfo",				true );
	render_info.fullscreen			 = config_get_bool( "FullScreen",				false );

	memset( MyPickupValid, 0, sizeof(MyPickupValid) );

	MyPickupValid[ PICKUP_Mugs ]              = config_get_bool( "AllowMugs",             true );
	MyPickupValid[ PICKUP_HeatseakerPickup ]  = config_get_bool( "AllowHeatseaker",       true );
	MyPickupValid[ PICKUP_Scatter ]           = config_get_bool( "AllowScatter",          true );
	MyPickupValid[ PICKUP_Gravgon ]           = config_get_bool( "AllowGravgon",          true );
	MyPickupValid[ PICKUP_Launcher ]          = config_get_bool( "AllowLauncher",         true );
	MyPickupValid[ PICKUP_TitanStar ]         = config_get_bool( "AllowTitanStar",        true );
	MyPickupValid[ PICKUP_PurgePickup ]       = config_get_bool( "AllowPurgePickup",      true );
	MyPickupValid[ PICKUP_QuantumPickup ]     = config_get_bool( "AllowQuantumPickup",    true );
	MyPickupValid[ PICKUP_Trojax ]            = config_get_bool( "AllowTrojax",           true );
	MyPickupValid[ PICKUP_Pyrolite ]          = config_get_bool( "AllowPyrolite",         true );
	MyPickupValid[ PICKUP_SussGun ]           = config_get_bool( "AllowSussGun",          true );
	MyPickupValid[ PICKUP_Laser ]             = config_get_bool( "AllowLaser",            true );
	MyPickupValid[ PICKUP_Nitro ]             = config_get_bool( "AllowNitro",            true );
	MyPickupValid[ PICKUP_Orb ]               = config_get_bool( "AllowOrb",              true );
	MyPickupValid[ PICKUP_GoldenPowerPod ]    = config_get_bool( "AllowGoldenPowerPod",   true );

	MyPickupValid[ PICKUP_SpiderPod ]         = config_get_bool( "AllowSpiderPod",        false );
	MyPickupValid[ PICKUP_PinePickup ]        = config_get_bool( "AllowPinePickup",       false );
	MyPickupValid[ PICKUP_Thief ]             = config_get_bool( "AllowThief",            false );
	MyPickupValid[ PICKUP_Transpulse ]        = config_get_bool( "AllowTranspulse",       false );
	MyPickupValid[ PICKUP_Mantle ]            = config_get_bool( "AllowMantle",           false );
	MyPickupValid[ PICKUP_Inv ]               = config_get_bool( "AllowInv",              false );

	memmove( PickupValid, MyPickupValid, sizeof(PickupValid) );//memcpy

    // integers

    WaterDetailSlider.value          = config_get_int( "water",						WaterDetailSlider.max );
	CLAMP( WaterDetailSlider.value, WaterDetailSlider.max );
	SetWaterDetail( &WaterDetailSlider );

#ifdef PANDORA
		TextScaleSlider.value = config_get_int("TextScale", 1.0F);	// need bigger font on the Pandora
#else
		TextScaleSlider.value = config_get_int("TextScale", 0.0F);
#endif
		CLAMP(TextScaleSlider.value, TextScaleSlider.max);
		SetTextScale( &TextScaleSlider );

    SfxSlider.value                  = config_get_int( "SfxVolume",					(int)(SfxSlider.max				* 1.00F) );
    FlagSfxSlider.value              = config_get_int( "FlagSfxVolume",				(int)(FlagSfxSlider.max			* 0.80F) );
	BikerSpeechSlider.value          = config_get_int( "BikerSpeechVolume",			(int)(BikerSpeechSlider.max		* 0.60F) );
    BikeCompSpeechSlider.value       = config_get_int( "BikeCompSpeechVol",			(int)(BikeCompSpeechSlider.max	* 0.60F) );

	CLAMP( SfxSlider.value,				SfxSlider.max );
	CLAMP( FlagSfxSlider.value,			FlagSfxSlider.max );
	CLAMP( BikerSpeechSlider.value,		BikerSpeechSlider.max );
	CLAMP( BikeCompSpeechSlider.value,	BikeCompSpeechSlider.max );

    GammaSlider.value                = config_get_int( "Gamma",						(int)(GammaSlider.max * 0.65F) );

	DebugPrintf("gamma read from config as %d, max %d\n",GammaSlider.value,GammaSlider.max);
	CLAMP( GammaSlider.value, GammaSlider.max );
	DebugPrintf("gamma after clamp: %d\n",GammaSlider.value);
	Gamma = ((float)GammaSlider.value / 100.0F);
	DebugPrintf("gamma variable: %d/100.0F = %f\n",GammaSlider.value,Gamma);

    MaxKillsSlider.value             = config_get_int( "MaxKills",					0 );
    MyTimeLimit.value                = config_get_int( "TimeLimit",					0);
    CTFSlider.value                  = config_get_int( "CTFrules",					CTF_STANDARD );
    GoalScoreSlider.value            = config_get_int( "FlagScore",					5 );
    BountyBonusSlider.value          = config_get_int( "BountyInterval",			10 );
    MyPacketsSlider.value            = config_get_int( "PacketsPerSecond",			10 );

	CLAMP( MaxKillsSlider.value,		MaxKillsSlider.max );
	CLAMP( MyTimeLimit.value,			MyTimeLimit.max );
	CLAMP( CTFSlider.value,				CTFSlider.max );
	CLAMP( GoalScoreSlider.value,		GoalScoreSlider.max );
	CLAMP( BountyBonusSlider.value,		BountyBonusSlider.max );
	CLAMP( MyPacketsSlider.value,		MyPacketsSlider.max );

    NumPrimaryPickupsSlider.value    = config_get_int( "NumPrimaryPickups",			1 );
	CLAMP( NumPrimaryPickupsSlider.value, NumPrimaryPickupsSlider.max );
	NumPrimaryPickups = NumPrimaryPickupsSlider.value;

    render_info.default_mode.w  = config_get_int( "ScreenWidth", 0 );
    render_info.default_mode.h  = config_get_int( "ScreenHeight", 0 );
    render_info.vsync  = config_get_int( "VSync", 0 );

		MatureContent										 = config_get_int( "MatureContent", 2);
		HUDColour												 = config_get_int( "HUDColour", YELLOW );
		InGameMenuColour								 = config_get_int( "InGameMenuColour", YELLOW );
    MilestoneMessagesColour          = config_get_int( "MilestoneMessagesColour",	RED );
    KillMessageColour                = config_get_int( "KillMessageColour",			GREEN );
    SystemMessageColour              = config_get_int( "SystemMessageColour",		YELLOW );
    FlagMessageColour                = config_get_int( "FlagMessageColour",			BLUE );
    PlayerMessageColour              = config_get_int( "PlayerMessageColour",		WHITE );
    PickupMessageColour              = config_get_int( "PickupMessageColour",		GREEN );
    TauntMessageColour               = config_get_int( "TauntMessageColour",		GREEN );
    MyMessageColour                  = config_get_int( "MyMessageColour",			  WHITE );
		MaxMessageTime                   = config_get_float( "MaxMessageTime",    5.0f );
		
	CLAMP( InGameMenuColour, MAXFONTCOLOURS );
	CLAMP( MilestoneMessagesColour,	MAXFONTCOLOURS );
	CLAMP( KillMessageColour,		MAXFONTCOLOURS );
	CLAMP( SystemMessageColour,		MAXFONTCOLOURS );
	CLAMP( FlagMessageColour,		MAXFONTCOLOURS );
	CLAMP( PlayerMessageColour,		MAXFONTCOLOURS );
	CLAMP( PickupMessageColour,		MAXFONTCOLOURS );
	CLAMP( TauntMessageColour,		MAXFONTCOLOURS );
	CLAMP( MyMessageColour,			MAXFONTCOLOURS );

    MyColPerspective                   = config_get_int( "ColPerspective",			COLPERS_Descent );
	CLAMP( MyColPerspective, 1 );

    GameType                         = config_get_int( "GameType",					GAME_Normal );
	CLAMP( GameType, MAX_GAMETYPE );

	// Stereo options
	render_info.stereo_enabled = config_get_bool( "StereoEnabled", false );
	render_info.stereo_mode = config_get_float( "StereoMode", STEREO_MODE_COLOR );
	render_info.stereo_eye_sep = config_get_float( "StereoEyeSep", 20 );
	render_info.stereo_focal_dist = config_get_float( "StereoFocalDist", 750 );
	render_info.stereo_right_color = config_get_float( "StereoRightColor", ST_CYAN );

	// initialize sliders to above values
	StereoColorSlider.value = render_info.stereo_right_color;
	StereoModeSlider.value = render_info.stereo_mode;

	// use original textures if requested and textures/original folder exists
	use_original_textures = config_get_bool( "UseOriginalTextures", false );
}

/*===================================================================
	Procedure	:		Set last game played info from the registry..
	Input		:		Nothing
	Output		:		Nothing
===================================================================*/
void SetGamePrefs( void )
{
	// ip / port
	
	config_set_str( "HostAddress",	TCPAddress.text );	// remote address
	config_set_str( "HostPort",	host_port_str.text );	// remote port
	config_set_str( "LocalPort",	local_port_str.text );	// local

	// tracker
	
	config_set_str( "TrackerServer",		tracker_server );
	config_set_int( "TrackerPort",			tracker_port );
	config_set_bool( "TrackerEnabled",		tracker_enabled );
	
	// booleans

  config_set_bool( "TintBikeTeamColor",     TintBikeTeamColor );
	config_set_bool( "ResetKillsPerLevel",		MyResetKillsPerLevel );
	config_set_bool( "BrightShips",			MyBrightShips );
	config_set_bool( "MissileCameraEnable",		MissileCameraEnable );
	config_set_bool( "RearCameraActive",		RearCameraActive );
	config_set_bool( "BikeEnginesOn",		BikeEnginesOn );
	config_set_bool( "ShowWeaponsPossessedOnHUD", ShowWeaponsPossessedOnHUD );
	config_set_bool( "JoystickInput", JoystickInput );
	config_set_bool( "ShowClockOnHUD", ShowClockOnHUD );
	config_set_bool( "ShowKPMOnHUD", ShowKPMOnHUD );
	config_set_bool( "ShowSpeedOnHUD", ShowSpeedOnHUD );
	config_set_bool( "ShowPlayersOnHUD",		ShowPlayersOnHUD );
	config_set_bool( "ShowPlayersOnHUDbyKills",	ShowPlayersOnHUDbyKills );
    config_set_bool( "ShowPlayerHealthAboveBikes", ShowPlayerHealthAboveBikes );
    config_set_bool( "ShowPlayerHealthByScores", ShowPlayerHealthByScores );
    config_set_bool( "ScaleFontPlayerHealthAboveBikes", ScaleFontPlayerHealthAboveBikes );
	config_set_bool( "BikeExhausts",		BikeExhausts );
	config_set_bool( "BountyBonus",			BountyBonus );
	config_set_bool( "RandomPickups",		MyRandomPickups );
	config_set_bool( "UseShortPackets",		MyUseShortPackets );
	config_set_bool( "ShowTeamInfo",		ShowTeamInfo );
	config_set_bool( "FullScreen",			render_info.fullscreen );

	config_set_bool( "AllowMugs",               MyPickupValid[ PICKUP_Mugs ] );
	config_set_bool( "AllowHeatseaker",         MyPickupValid[ PICKUP_HeatseakerPickup ] );
	config_set_bool( "AllowScatter",            MyPickupValid[ PICKUP_Scatter ] );
	config_set_bool( "AllowGravgon",            MyPickupValid[ PICKUP_Gravgon ] );
	config_set_bool( "AllowLauncher",           MyPickupValid[ PICKUP_Launcher ] );
	config_set_bool( "AllowTitanStar",          MyPickupValid[ PICKUP_TitanStar ] );
	config_set_bool( "AllowPurgePickup",        MyPickupValid[ PICKUP_PurgePickup ] );
	config_set_bool( "AllowQuantumPickup",      MyPickupValid[ PICKUP_QuantumPickup ] );
	config_set_bool( "AllowTrojax",             MyPickupValid[ PICKUP_Trojax ] );
	config_set_bool( "AllowPyrolite",           MyPickupValid[ PICKUP_Pyrolite ] );
	config_set_bool( "AllowSussGun",            MyPickupValid[ PICKUP_SussGun ] );
	config_set_bool( "AllowLaser",              MyPickupValid[ PICKUP_Laser ] );
	config_set_bool( "AllowNitro",              MyPickupValid[ PICKUP_Nitro ] );
	config_set_bool( "AllowOrb",                MyPickupValid[ PICKUP_Orb ] );
	config_set_bool( "AllowGoldenPowerPod",     MyPickupValid[ PICKUP_GoldenPowerPod ] );
	config_set_bool( "AllowSpiderPod",          MyPickupValid[ PICKUP_SpiderPod ] );
	config_set_bool( "AllowPinePickup",         MyPickupValid[ PICKUP_PinePickup ] );
	config_set_bool( "AllowThief",              MyPickupValid[ PICKUP_Thief ] );
	config_set_bool( "AllowTranspulse",         MyPickupValid[ PICKUP_Transpulse ] );
	config_set_bool( "AllowMantle",             MyPickupValid[ PICKUP_Mantle ] );
	config_set_bool( "AllowInv",                MyPickupValid[ PICKUP_Inv ] );
	config_set_bool( "VSync",                   render_info.vsync );

	// integers

	config_set_int( "ScreenWidth",			render_info.Mode[ render_info.CurrMode ].w );
	config_set_int( "ScreenHeight",			render_info.Mode[ render_info.CurrMode ].h );
	config_set_int( "SfxVolume",			SfxSlider.value );
	config_set_int( "FlagSfxVolume",		FlagSfxSlider.value );
	config_set_int( "Gamma",			GammaSlider.value );
	config_set_int( "water",			WaterDetailSlider.value );
	config_set_int( "TextScale", TextScaleSlider.value );
	config_set_int( "TimeLimit",			MyTimeLimit.value );
	config_set_int( "HUDColour", HUDColour );
	config_set_int( "InGameMenuColour", InGameMenuColour );
	config_set_int( "KillMessageColour",		KillMessageColour );
	config_set_int( "MilestoneMessagesColour",	MilestoneMessagesColour );
	config_set_int( "SystemMessageColour",		SystemMessageColour );
	config_set_int( "FlagMessageColour",		FlagMessageColour );
	config_set_int( "PlayerMessageColour",		PlayerMessageColour );
	config_set_int( "PickupMessageColour",		PickupMessageColour );
	config_set_int( "TauntMessageColour",		TauntMessageColour );
	config_set_int( "MyMessageColour",		MyMessageColour );
	config_set_int( "ColPerspective",		MyColPerspective );
	config_set_int( "MaxKills",			MaxKillsSlider.value );
	config_set_int( "GameType",			GameType );
	config_set_int( "CTFrules",			CTFSlider.value );
	config_set_int( "NumPrimaryPickups",		NumPrimaryPickupsSlider.value );
	config_set_int( "PacketsPerSecond",		MyPacketsSlider.value );
	config_set_int( "BountyInterval",		BountyBonusSlider.value );
	config_set_int( "BikerSpeechVolume",		BikerSpeechSlider.value );
	config_set_int( "BikeCompSpeechVol",		BikeCompSpeechSlider.value );
	config_set_int( "MatureContent", MatureContent	);

	config_set_int( "FlagScore",			GoalScoreSlider.value );
	GoalScore = GoalScoreSlider.value;

	// floats
	config_set_float( "MaxMessageTime",	MaxMessageTime );

	// Stereo options
	config_set_bool( "StereoEnabled",		render_info.stereo_enabled );
	config_set_float( "StereoMode",		render_info.stereo_mode );
	config_set_float( "StereoEyeSep",		render_info.stereo_eye_sep );
	config_set_float( "StereoFocalDist",		render_info.stereo_focal_dist );
	config_set_float( "StereoRightColor",		render_info.stereo_right_color );

	config_save();
}

bool CompareVectors(VECTOR vector1, VECTOR vector2)
{
	if ((vector1.x == vector2.x) && (vector1.y == vector2.y) && (vector1.z == vector2.z))
		return true;
	else
		return false;
}

void SetTextures (MENU *menu)
{
	int i;

	i = 0;

	while (menu->Item[i].x != -1)
	{	if (i>5)
			Msg("Too many textures to set");	// There should only ever be 6 textures set
		CurrentTextures[i] = menu->Item[i].FrameNo;
		i++;
	}
}

void SetDiscStatus(MENU *menu)
{
	DiscStatus = menu->MenuStatus;
}

/*===================================================================
	Procedure	:		sets each vertex intensity of a model to a given fraction of the original
	Input		:		Model num, fraction of orig, intensity, ptr to Exclude - list of vertices
						that are to be left unchanged.
	Output		:		nothing

	Note		:		Only allows vertices to be excluded from one group & exec buffer at present.
===================================================================*/
bool TintModelVertices( u_int16_t Model, float percent, EXCLUDEDVERTICES *Exclude )
{
	LPLVERTEX			DstlpD3DLVERTEX = NULL;
	u_int16_t					Group;
	u_int16_t					Vert;
	u_int16_t					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	LPLVERTEX			VertPtr;
	COLOR				Colour;
	u_int8_t red, green, blue, alpha;
	int i;
	int *CurrentExclude;
	int NumberToExclude;
	int ExcludedSoFar = 0;

	DstMloadheader = &ModelHeaders[ Model ];

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{   
			if ((Exclude) && (Exclude->ExecBuf == ExecBuf) && (Exclude->Group == Group))
			{	CurrentExclude = Exclude->VertexPtr;
				NumberToExclude = Exclude->NumVertices;
				ExcludedSoFar = 0;
			}
			else
			{
				CurrentExclude = NULL;
				NumberToExclude = 0;
			}
			if (NumberToExclude == EXCLUDE_ALL)
				continue;


			if (!(FSLockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf], &DstlpD3DLVERTEX)))
			{
				return false;
			}

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			VertPtr = DstMloadheader->Group[Group].originalVerts[ExecBuf];
		
			for (i=0; i<Vert; i++)
			{
				red = (u_int8_t)RGBA_GETRED(VertPtr->color);
				green = (u_int8_t)RGBA_GETGREEN(VertPtr->color);
				blue = (u_int8_t)RGBA_GETBLUE(VertPtr->color);
				alpha = (u_int8_t)RGBA_GETALPHA(VertPtr->color);
				
				//DebugPrintf("vertex color rgba=%d,%d,%d,%d\n",
				//				red, green, blue, alpha);

			  	if ((CurrentExclude) && (*CurrentExclude == i))
			  	{
			  		ExcludedSoFar++;
			  		if (ExcludedSoFar == NumberToExclude)
			  			CurrentExclude = NULL;
			  		else
			  			CurrentExclude++;
			  	}
				else
			  	{
			  		red = (u_int8_t)((float)red * percent);
			  		green = (u_int8_t)((float)green * percent);
			  		blue = (u_int8_t)((float)blue * percent);
			  	}

				//DebugPrintf("tinted vertex color by %f%% to rgba=%d,%d,%d,%d\n",
				//				percent, red, green, blue, alpha);
			  
				Colour = RGBA_MAKE( red, green, blue, alpha );
				if( !Colour ) 
					return false;
				
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;

				VertPtr++;
			}

			if (!(FSUnlockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf])))
			{
				return false;
			}
		}
	}
	return true;
}

void GetExtremeOffsets( u_int16_t Model, PLANE *plane, float *minoffset, float *maxoffset )
{
	u_int16_t					Group;
	u_int16_t					ExecBuf;
	MXALOADHEADER	*		ModelHeaderPtr;
	MXLOADHEADER	*		MxModelHeaderPtr;
	int i, TotalVerts;
	LPLVERTEX			VertPtr;
	VECTOR tempvert;
	float dist;


	if ( TitleModelSet[ Model ].DoIMorph )
	{
		ModelHeaderPtr = &MxaModelHeaders[ Models[Model].ModelNum];

		*minoffset = 0.0F;
		*maxoffset = 0.0F;
						  
		for( Group = 0; Group < ModelHeaderPtr->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < ModelHeaderPtr->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
				TotalVerts = ModelHeaderPtr->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];
				VertPtr = ModelHeaderPtr->Group[Group].originalVerts[ExecBuf];

				for (i=8; i<TotalVerts; i++)	//ignore bounding box, so start at 8...
				{
				tempvert.x = VertPtr[i].x * Models[Model].Xscale;
				tempvert.y = VertPtr[i].y * Models[Model].Yscale;
				tempvert.z = VertPtr[i].z * Models[Model].Zscale;

				//dist = -(POINT_TO_PLANE( &tempvert, plane ));
				dist = tempvert.z;	// assumes z plane!!!

				if (dist > *maxoffset)
					*maxoffset = dist;

				if (dist < *minoffset)
					*minoffset = dist;
				}
			}
		}
	}else
	{
		MxModelHeaderPtr = &ModelHeaders[ Models[Model].ModelNum];

		*minoffset = 0.0F;
		*maxoffset = 0.0F;
						  
		for( Group = 0; Group < MxModelHeaderPtr->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < MxModelHeaderPtr->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
				TotalVerts = MxModelHeaderPtr->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];
				VertPtr = MxModelHeaderPtr->Group[Group].originalVerts[ExecBuf];

				for (i=8; i<TotalVerts; i++)	//ignore bounding box, so start at 8...
				{
				tempvert.x = VertPtr[i].x * Models[Model].Xscale;
				tempvert.y = VertPtr[i].y * Models[Model].Yscale;
				tempvert.z = VertPtr[i].z * Models[Model].Zscale;

				//dist = -(POINT_TO_PLANE( &tempvert, plane ));
				dist = tempvert.z;

				if (dist > *maxoffset)
					*maxoffset = dist;

				if (dist < *minoffset)
					*minoffset = dist;
				}
			}
		}
	}
}



bool MakeTranslucent( u_int16_t Model )
{
//	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPLVERTEX			DstlpD3DLVERTEX = NULL;
	u_int16_t					Group;
	u_int16_t					Vert;
	u_int16_t					ExecBuf;
	MXALOADHEADER	*		DstMloadheader;
	MXLOADHEADER	*		DstMxloadheader;
	LPLVERTEX			ColourPtr;
	COLOR				Colour;
	u_int8_t red, green, blue, alpha;
	int i;

	if ( TitleModelSet[ Model ].DoIMorph )
	{
		DstMloadheader = &MxaModelHeaders[ Models[Model].ModelNum];

		for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
/*
				memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
				DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/
//				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
//								DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return false; // bjd
//				if (FSLockExecuteBuffer(DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK )
//					return false;
				if (!(FSLockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf], &DstlpD3DLVERTEX)))
				{
					return false;
				}

//				DstlpD3DLVERTEX = (LPLVERTEX) DstDebDesc.lpData;

				Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

				ColourPtr = DstMloadheader->Group[Group].originalVerts[ExecBuf];


				for (i=0; i<Vert; i++)
				{
					red = (u_int8_t)RGBA_GETRED(ColourPtr->color);
					green = (u_int8_t)RGBA_GETGREEN(ColourPtr->color);
					blue = (u_int8_t)RGBA_GETBLUE(ColourPtr->color);
					alpha = (u_int8_t)RGBA_GETALPHA(ColourPtr->color);
					
					//red = (u_int8_t)((float)red * BikeCol);
					//green = (u_int8_t)((float)green * BikeCol);
					//blue = (u_int8_t)((float)blue * BikeCol);
					//alpha = (u_int8_t)((float)alpha * BikeAlpha);
					red = 255;
					green = 255;
					blue = 255;
					alpha = 255;
					
					Colour = RGBA_MAKE( red, green, blue, alpha );
					
					DstlpD3DLVERTEX->color = Colour;
					DstlpD3DLVERTEX++;

					ColourPtr++;
				}


//				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
//				DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return false;
				if (!(FSUnlockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf])))
				{
					return false;
				}
			}
		}
		return true;
	}else
	{
		DstMxloadheader = &ModelHeaders[ Models[Model].ModelNum];

		for( Group = 0; Group < DstMxloadheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < DstMxloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{   
/*			
				memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
				DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/
//				if( DstMxloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
//								DstMxloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return false; // bjd
//				if (FSLockExecuteBuffer(DstMxloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK )
//					return false;

				if (!(FSLockVertexBuffer(&DstMxloadheader->Group[ Group ].renderObject[ExecBuf], &DstlpD3DLVERTEX)))
				{
					return false;
				}

				//DstlpD3DLVERTEX = (LPLVERTEX) DstDebDesc.lpData;

				Vert = DstMxloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

				DstMxloadheader->Group[ Group ].exec_type[ ExecBuf ] = 0;

				ColourPtr = DstMxloadheader->Group[Group].originalVerts[ExecBuf];

				for (i=0; i<Vert; i++)
				{
					red = (u_int8_t)RGBA_GETRED(ColourPtr->color);
					green = (u_int8_t)RGBA_GETGREEN(ColourPtr->color);
					blue = (u_int8_t)RGBA_GETBLUE(ColourPtr->color);
					alpha = (u_int8_t)RGBA_GETALPHA(ColourPtr->color);
					
					red = (u_int8_t)((float)red * BikeCol);
					green = (u_int8_t)((float)green * BikeCol);
					blue = (u_int8_t)((float)blue * BikeCol);
					alpha = (u_int8_t)((float)alpha * BikeAlpha);
					
					Colour = RGBA_MAKE( red, green, blue, alpha );
					
					DstlpD3DLVERTEX->color = Colour;
					DstlpD3DLVERTEX++;

					ColourPtr++;
				}


//				if( DstMxloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
//				DstMxloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return false;
				if (!(FSUnlockVertexBuffer(&DstMxloadheader->Group[ Group ].renderObject[ExecBuf])))
				{
					return false;
				}
			}
		}
		return true;
	}
}


bool TintOneVertex( u_int16_t Model, u_int16_t Group, u_int16_t ExecBuf, int VertexNo, float tr, float tg, float tb, float ta )
{
	LPLVERTEX			DstlpD3DLVERTEX = NULL;
	MXLOADHEADER	*		DstMloadheader;
	LPLVERTEX			VertPtr;
	COLOR				Colour;
	u_int8_t red, green, blue, alpha;
	u_int8_t vred, vgreen, vblue, valpha;

	DstMloadheader = &ModelHeaders[ Model ];

	if (!(FSLockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf], &DstlpD3DLVERTEX)))
	{
		return false;
	}

	VertPtr = &(DstMloadheader->Group[Group].originalVerts[ExecBuf])[VertexNo];
	  
	vred = (u_int8_t)RGBA_GETRED(VertPtr->color);
	vgreen = (u_int8_t)RGBA_GETGREEN(VertPtr->color);
	vblue = (u_int8_t)RGBA_GETBLUE(VertPtr->color);
	valpha = (u_int8_t)RGBA_GETALPHA(VertPtr->color);
	
 	red = (u_int8_t)((float)vred * tr);
  	green = (u_int8_t)((float)vgreen * tg);
  	blue = (u_int8_t)((float)vblue * tb);
  	alpha = (u_int8_t)((float)valpha * ta);

/*
	red = 255;
	green = 255;
	blue = 255;
	alpha = 255;
*/

	Colour = RGBA_MAKE( red, green, blue, alpha );
	if( !Colour ) 
		return false;
			
	DstlpD3DLVERTEX[VertexNo].color = Colour;

	if (!(FSUnlockVertexBuffer(&DstMloadheader->Group[ Group ].renderObject[ExecBuf])))
	{
		return false;
	}
   	
	return true;
}



/*===================================================================
	Procedure	:		increases the texture frame no. of a disc by one
	Input		:		disc number
	Output		:		nothing
===================================================================*/
void HighlightDisc(int disc)
{
	if( PolyAnim[disc] )
		PolyAnim[disc]->newframe = CurrentTextures[disc] + 1;

}

/*===================================================================
	Procedure	:		restores original texture frame of disc
	Input		:		disc number
	Output		:		nothing
===================================================================*/
void ResetDisc(int disc)
{
	 PolyAnim[disc]->newframe = CurrentTextures[disc];
}

/*===================================================================
	Procedure	:		initialises rotation sequence for difficulty level disc
	Input		:		menu item (not used)
	Output		:		nothing
===================================================================*/

void DifficultySet (MENUITEM *item)
{
#if 0
	StackMode = DISC_MODE_ONE;
	StackStatus = DISC_EXPAND;
	DifficultyLevel++;
	if( DifficultyLevel > DIFF_Vhard )
		DifficultyLevel = DIFF_Easy;

	PlaySfx( SFX_Stakmove, 1.0F );
#endif

	MenuState = MENUSTATE_DifficultySet;
}

void InitDifficultyLevel ( MENU *Menu )
{
#if 1
	MENUITEM *Item;
	static int DifficultyLookup[ 4 ] = { 4, 6, 8, 10 };

	for ( Item = Menu->Item; Item->x >= 0 ; Item++ )
	{
		if ( Item->y == TITLE_MODEL_Disc3 )
		{
			Item->FrameNo = DifficultyLookup[ DifficultyLevel ];
			break;
		}
	}
#endif
}

TEXTINFO *InitTextInfo( void )
{
	TEXTINFO *TextInfo;

	TextInfo = (TEXTINFO*)malloc ( sizeof (TEXTINFO) );
	memset( TextInfo, 0, sizeof(TEXTINFO) );

	if (!TextInfo)
	{	Msg("Title.c   InitTextInfo:  unable to allocate memory\n");
		exit(1);
	}

	TextInfo->text = NULL;						// text string
	TextInfo->highlight = false;					// item highlighted?
	TextInfo->highlighttype = HIGHLIGHT_Pulsing;
	TextInfo->xmin = 0;						// 
	TextInfo->ymin = 0;						// bounding box for text
	TextInfo->xmax = 0;						// (VDU co-ordinates)
	TextInfo->ymax = 0;						// 
	TextInfo->font = FONT_Small;						// font size (small, medium, large)
	TextInfo->flags = 0;						// formatting info
	TextInfo->num_lines = 0;					// number of lines once formatted
	TextInfo->boxdone = false;					
	TextInfo->num_pointers_to_free = 0;		
	TextInfo->force_fit = false;

	return TextInfo;
}

void RedrawFlatMenuList( MENUITEM *Item )
{
#if 1
	LIST *l;
	int i, current;
	TEXTINFO *TextInfo;
	
	l = (LIST *)(Item->Variable);

	if (l->old_top_item != l->top_item)
	{
		l->old_top_item = l->top_item;


		//redraw whole list
		for (i=0; i < l->display_items; i++)
		{
			current = l->top_item + i;

			if (i > (l->items - 1))
				break;

			//write relevent info into structure
			TextInfo = Item->TextInfo[i];

			TextInfo->xmin = Item->x;
			TextInfo->ymin = (((Item->ymax - Item->y)/l->display_items) * i) + Item->y;
			TextInfo->text = l->item[current];
			TextInfo->xmax = Item->xmax;
			TextInfo->ymax = (((Item->ymax - Item->y)/l->display_items) * (i + 1)) + Item->y;
			TextInfo->font = Item->font;
			TextInfo->flags = Item->highlightflags;
	
			if (current == l->selected_item)
				TextInfo->highlight = true;
			else
				TextInfo->highlight = false;

				//format text item...
			PrintTextItem(TextInfo);

		}
		
	}else
	{
	 	if (l->old_selected_item != l->selected_item)
		{
			l->old_selected_item = l->selected_item;


			for (i=0; i < l->display_items; i++)
			{
				if (i > (l->items - 1))
					break;

				current = l->top_item + i;

				if (current == l->selected_item)
					Item->TextInfo[i]->highlight = true;
				else
					Item->TextInfo[i]->highlight = false;
			}
		}
	}
#endif
}



void DrawFlatMenuListText ( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;
	int i = 0;
	int current = 0;

	l = (LIST *)(Item->Variable);

	l->old_top_item = l->top_item;
	l->old_selected_item = l->selected_item;

	for (i=0; i < l->display_items; i++)
	{

		TextInfo = Item->TextInfo[i];

		if (current == l->selected_item)
			TextInfo->highlight = true;
		else
			TextInfo->highlight = false;

		current = l->top_item + i;
		if (current >= l->items)
		{	TextInfo->text = EmptyString;
			PrintTextItem(TextInfo);
			continue;
		}
	
		if 	(TextInfo->text == l->item[current])
			continue;
		
		TextInfo->text = l->item[current];

		//format text item...
		PrintTextItem(TextInfo);

	}

}

void AddToFlatMenuList( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;
	int i, current;

	l = (LIST *)(Item->Variable);

 	for (i=0; i < Item->numtextitems; i++)
	{

		TextInfo = Item->TextInfo[i];

		if(!TextInfo)
			continue;

		current = l->top_item + i;

		if (current >= l->items)
		{
			if (TextInfo->text != (u_int8_t*)EmptyString)
			{	
				TextInfo->text = EmptyString;
				PrintTextItem(TextInfo);
			}
		}
		else
		{
			if (!TextInfo->text)
			{
				strncpy( l->old_item[current] , l->item[current] , sizeof(l->item[0])  );
				TextInfo->text = l->old_item[current];
				PrintTextItem(TextInfo);
			}else
			{
				if (strcmp( TextInfo->text, l->item[current] ) != 0)		// if text has changed
				{
					strncpy( l->old_item[current] , l->item[current] , sizeof(l->item[0])  );

					TextInfo->text = l->old_item[current];
					PrintTextItem(TextInfo);
				}
			}
		}

		if (current == l->selected_item)
			TextInfo->highlight = true;
		else
			TextInfo->highlight = false;

		if (!(CurrentMenuItem->highlightflags & TEXTFLAG_ManualListHighlight))
		{
			if (CurrentMenuItem == Item)
				TextInfo->highlighttype = HIGHLIGHT_Pulsing;
			else
				TextInfo->highlighttype = HIGHLIGHT_Static;
		}
	}
}

void AddToTextStack( TEXTINFO *TextInfo )
{
	int i;

	if(!TextInfo)return;
	
	for ( i = 0; i < TextStackLevel; i++ )
	{
		if ( !TextStack[ i ] )
		{
			TextStack[ i ] = TextInfo;
			return;
		}
	}

	if( TextStackLevel + 1 < MAXTEXTITEMS )
		TextStack[ TextStackLevel++ ] = TextInfo;
}

void DrawFlatMenuList( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;
	int i;

	l = (LIST *)(Item->Variable);
	l->old_top_item = l->top_item;
	l->old_selected_item = l->selected_item;
	l->Static = false;

	Item->numtextitems = 0;

	for (i=0; i < l->display_items; i++)
	{

		strncpy( l->old_item[i] , EmptyString , sizeof(l->item[0])  );
		
		Item->TextInfo[i] = InitTextInfo();

		Item->numtextitems++;
		
		//write relevent info into structure
		TextInfo = Item->TextInfo[i];

		TextInfo->xmin = Item->x;
		TextInfo->ymin = (((Item->ymax - Item->y)/l->display_items) * i) + Item->y;
		TextInfo->xmax = Item->xmax;
		TextInfo->ymax = (((Item->ymax - Item->y)/l->display_items) * (i + 1)) + Item->y;
		TextInfo->font = Item->font;
		TextInfo->flags = Item->highlightflags;
	
		AddToTextStack( TextInfo );

	}
	oldlistitems = l->items;

	AddToFlatMenuList( Item );
}

void DrawSplitMenuList( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;
	int i, column;
	int border, width, newxmax, newxmin, columnoffset;

	l = (LIST *)(Item->Variable);
	l->old_top_item = l->top_item;
	l->old_selected_item = l->selected_item;

	border = Item->x;
	width = Item->xmax - Item->x;
	newxmax = VDU_Width - border;
	newxmin = newxmax - width;
	columnoffset = newxmin - Item->x;

	for (column = 0; column < 2; column++)
	{
		for (i=0; i < l->display_items / 2; i++)
		{

			strncpy( l->old_item[i + column * (l->display_items / 2)] , EmptyString , sizeof(l->item[0])  );
		
			Item->TextInfo[i + column * (l->display_items / 2)] = InitTextInfo();

			Item->numtextitems++;
		
			//write relevent info into structure
			TextInfo = Item->TextInfo[i + column * (l->display_items / 2)];

			TextInfo->xmin = Item->x + column * columnoffset;
			TextInfo->ymin = (((Item->ymax - Item->y)/l->display_items) * i * 2) + Item->y;
			TextInfo->xmax = Item->xmax + column * columnoffset;
			TextInfo->ymax = (((Item->ymax - Item->y)/l->display_items) * (i * 2 + 1)) + Item->y;
			TextInfo->ymax += TextInfo->ymax - TextInfo->ymin;
			TextInfo->font = Item->font;
			TextInfo->flags = Item->highlightflags;
	
			//add to text stack...
			AddToTextStack( TextInfo );

		}
	}
	oldlistitems = l->items;

	AddToFlatMenuList( Item );
}


void DrawFlatTextStatus( MENUITEM *Item )
{
	TEXTINFO *TextInfo;
	TEXT *t;

	t = (TEXT *)(Item->Variable);
	TextInfo = Item->TextInfo[1];

	TextInfo->xmin = Item->xmax + 5;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = 195;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	TextInfo->text = t->text;
	
	//format text item...
	PrintTextItem(TextInfo);
}

void DrawFlatMenuText( MENUITEM *Item )
{
	// draw text in textinfo 0
	DrawFlatMenuItem(Item);

	//allocate space for text status
	Item->TextInfo[1] = InitTextInfo();

	Item->numtextitems++;

	//add to text stack...
	AddToTextStack( Item->TextInfo[1] );
	DrawFlatTextStatus( Item );
}
/*
		{ 0,160, 10 , 170, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &LoadSavedGameList, NULL, NULL, DrawLeftArrow, NULL, 0 } ,
		{ 190,160, 200 , 170, 0, "", FONT_Medium, TEXTFLAG_CentreX | TEXTFLAG_CentreY , &LoadSavedGameList, NULL, NULL, DrawRightArrow, NULL, 0 } ,
*/

void DrawLeftArrow( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;

	l = (LIST *)Item->Variable;

	if ( l->selected_item > 0 )
	{
		//allocate space for textinfo
		Item->TextInfo[0] = InitTextInfo();

		Item->numtextitems++;
			
		//write relevent info into structure
		TextInfo = Item->TextInfo[0];

		TextInfo->text = (char *)malloc( sizeof( char ) * 2 );
		TextInfo->text[ 0 ] = 17;
		TextInfo->text[ 1 ] = 0;

		TextInfo->xmin = Item->x;
		TextInfo->ymin = Item->y;
		TextInfo->xmax = Item->xmax;
		TextInfo->ymax = Item->ymax;
		TextInfo->font = Item->font;
		TextInfo->flags = Item->highlightflags;
		TextInfo->pointer[ TextInfo->num_pointers_to_free++ ] = TextInfo->text;

		//add to text stack...
		AddToTextStack( TextInfo );

		//format text item...
		PrintTextItem(TextInfo);
	}
}

void DrawRightArrow( MENUITEM *Item )
{
	LIST *l;
	TEXTINFO *TextInfo;

	l = (LIST *)Item->Variable;

	if ( l->selected_item < ( l->items - 1 ) )
	{
		//allocate space for textinfo
		Item->TextInfo[0] = InitTextInfo();

		Item->numtextitems++;
			
		//write relevent info into structure
		TextInfo = Item->TextInfo[0];

		TextInfo->text = (char *)malloc( sizeof( char ) * 2);
		TextInfo->text[ 0 ] = 16;
		TextInfo->text[ 1 ] = 0;

		TextInfo->xmin = Item->x;
		TextInfo->ymin = Item->y;
		TextInfo->xmax = Item->xmax;
		TextInfo->ymax = Item->ymax;
		TextInfo->font = Item->font;
		TextInfo->flags = Item->highlightflags;
		TextInfo->pointer[ TextInfo->num_pointers_to_free++ ] = TextInfo->text;

		//add to text stack...
		AddToTextStack( TextInfo );

		//format text item...
		PrintTextItem(TextInfo);
	}
}

void DrawFlatMenuItem( MENUITEM *Item )
{
	TEXTINFO *TextInfo;

	//allocate space for textinfo
	Item->TextInfo[0] = InitTextInfo();

	Item->numtextitems++;
		
	//write relevent info into structure
	TextInfo = Item->TextInfo[0];

	TextInfo->text = Item->StrPnt;
	TextInfo->xmin = Item->x;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//add to text stack...
	AddToTextStack( TextInfo );

	//format text item...
	PrintTextItem(TextInfo);
	
}

void DrawConditionalText( MENUITEM *Item )
{
	if ( *(bool *)(Item->Value ) )
	{
	 	DrawFlatMenuItem( Item );
	}
}

void DrawConditionalName( MENUITEM *Item )
{
	if ( *(bool *)(Item->Value ) )
	{
	 	DrawFlatMenuName( Item );
	}
}

void DrawReadyStatus(MENUITEM *Item)
{
	Item->TextInfo[0]->text = (PlayerReady[WhoIAm]) ? WaitingForReset : WaitingForConfirm;
	PrintTextItem(Item->TextInfo[0]);
}

void DrawReadyButton( MENUITEM *Item )
{
	DrawFlatMenuItem( Item );
	DrawReadyStatus(Item);
}

void PlayerConfirm( MENUITEM *Item )
{
	PlayerReady[WhoIAm] = !PlayerReady[WhoIAm];
	DrawReadyStatus(Item);

	if (PlayerReady[WhoIAm])
		NoTeamSelect = true;
	else
		NoTeamSelect = false;
}

TEXTINFO * DrawFlatRadioBtnStatus( MENUITEM *Item )
{

	TEXTINFO *TextInfo;

	TextInfo = Item->TextInfo[1];

	if (TextInfo)
	{
		TextInfo->xmin = Item->x - 10;
		TextInfo->ymin = Item->y;
		TextInfo->xmax = Item->x - 2;
		TextInfo->ymax = Item->ymax;
		TextInfo->font = Item->font;
		TextInfo->flags = Item->highlightflags;

		if ( *((int *)(Item->Variable)) == (int) Item->Value )
		{
			TextInfo->text = RadioBtnSelected;
		}else
		{
			TextInfo->text = RadioBtnNotSelected;
		}
	
		//format text item...
		PrintTextItem(TextInfo);
	}
		
	return TextInfo;
}


TEXTINFO * DrawFlatToggleStatus( MENUITEM *Item )
{
	TEXTINFO *TextInfo;
	char *status;

	if ( *(bool *)(Item->Variable ) )
	{
		status = ToggleStatusOn;
	}
	else
	{
		status = ToggleStatusOff;
	}
		
	//write relevent info into structure
	TextInfo = Item->TextInfo[1];

	TextInfo->text = status;
	TextInfo->xmin = Item->xmax + 5;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax + 45;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);

	return TextInfo;
}

TEXTINFO * DrawColFlatToggleStatus( MENUITEM *Item )
{
	TEXTINFO *TextInfo;
	char *status;

	if ( *(bool *)(Item->Variable ) )
	{
		status = ToggleStatusShooter;
	}
	else
	{
		status = ToggleStatusTarget;
	}
		
	//write relevent info into structure
	TextInfo = Item->TextInfo[1];

	TextInfo->text = status;
	TextInfo->xmin = Item->xmax + 5;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax + 45;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);

	return TextInfo;
}

void SelectFlatRadioButton( MENUITEM *Item )
{
	MENUITEM *TempItem;
	
	SelectRadioButton(Item);

	for( TempItem = CurrentMenu->Item ; TempItem->x >= 0 ; TempItem++ )
	{
		if (TempItem->Variable == Item->Variable)
		{
			DrawFlatRadioBtnStatus(TempItem);
		}
	}
}

void SelectFlatMenuToggle( MENUITEM *Item )
{
	if (VDU_Ready)
	{
		SelectToggle(Item);
		DrawFlatToggleStatus(Item);
	}
	
}

void SelectMultiToggle( MENUITEM *Item )
{
	if (VDU_Ready)
	{
		if ( Item->Variable )
		{
			*(bool *)(Item->Variable) = !*(bool *)(Item->Variable);
			if ( Item->Value )
			{
				*(bool *)(Item->Value) = !*(bool *)(Item->Value);
			}
		}

		DrawFlatToggleStatus(Item);
	}
}


void DrawFlatMenuToggle( MENUITEM *Item )
{
	// draw text in textinfo 0
	DrawFlatMenuItem(Item);

	//allocate space for toggle status
	Item->TextInfo[1] = InitTextInfo();

	Item->numtextitems++;

	//add to text stack...
	AddToTextStack( DrawFlatToggleStatus(Item) );

}


void SelectFlatMenuKey( MENUITEM *Item )
{
	MenuState = MENUSTATE_Keydef2;
	Item->TextInfo[0]->highlighttype = HIGHLIGHT_Static;
	KeyItem = Item;
}

void RedrawFlatMenuKey( MENUITEM *Item)
{
	if (VDU_Ready)
	{
		DrawFlatMenuKeyText( Item );
	}
}

void CheckKeysForChanges( void )
{
	MENUITEM *Item;
	VirtualKeycode keycode;
	char *keyword;
	DEFKEY *kdef;
	
	for( Item = CurrentMenu->Item ; Item->x >= 0 ; Item++ )
	{
		if( Item->FuncDraw == DrawFlatMenuKey )
		{	
#ifdef VDUKDEF
			kdef = (DEFKEY *)(Item->Variable);
			keycode = kdef->def->key[ kdef->selected_key ];
#else
			keycode = *(VirtualKeycode *)(Item->Variable);
#endif
			if ( keycode )
			{
				keyword = (char *) key_name( keycode );
				if ( !keyword )
					keyword = EmptyString;
			}
			else
				keyword = UndefinedKey;
			
			Item->TextInfo[0]->text = keyword; 
		}
		else if ( Item->FuncDraw == DrawFlatMenuKeyLeftArrow )
		{
			char *arrow;

			arrow = EmptyString;
			if ( Item->Variable )
			{
				kdef = (DEFKEY *)(Item->Variable);
				if ( kdef->def->keys > 1 && kdef->selected_key > 0 )
				{
					arrow = "\021"; // = 17
				}
			}
			Item->TextInfo[ 0 ]->text = arrow;
		}
		else if ( Item->FuncDraw == DrawFlatMenuKeyRightArrow )
		{
			char *arrow;

			arrow = EmptyString;
			if ( Item->Variable )
			{
				kdef = (DEFKEY *)(Item->Variable);
				if ( kdef->def->keys > 1 && kdef->selected_key < kdef->def->keys - 1 )
				{
					arrow = "\020"; // = 16
				}
			}
			Item->TextInfo[ 0 ]->text = arrow;
		}
		else if ( Item->FuncDraw == DrawFlatMenuKeyNum )
		{
			Item->TextInfo[ 0 ]->text[ 0 ] = 0;
			if ( Item->Variable )
			{
				kdef = (DEFKEY *)(Item->Variable);
				if ( kdef->def->keys > 1 )
				{
					sprintf( Item->TextInfo[ 0 ]->text, "%d/%d", kdef->selected_key + 1, kdef->def->keys );
				}
			}
		}
	}
}

void DrawFlatMenuKeyText( MENUITEM *Item )
{
	DEFKEY *kdef;
	VirtualKeycode keycode;
	char *keyword = NULL;
  	TEXTINFO *TextInfo;

	if ( Item->Variable )
	{
#ifdef VDUKDEF
		kdef = (DEFKEY *)(Item->Variable);
		keycode = kdef->def->key[ kdef->selected_key ];
#else
		keycode = *(VirtualKeycode *)(Item->Variable);
#endif
		if ( keycode )
		{
			keyword = (char *) key_name( keycode );
			if ( !keyword )
				keyword = EmptyString;
		}
		else
			keyword = UndefinedKey;
	}

	//write relevent info into structure
	TextInfo = Item->TextInfo[0];

	TextInfo->text = keyword;
	TextInfo->xmin = Item->x;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);

}

void DrawFlatMenuKey( MENUITEM *Item )
{

	//allocate space for textinfo
	Item->TextInfo[0] = InitTextInfo();

	Item->numtextitems++;
		
	//add to text stack...
	AddToTextStack( Item->TextInfo[0] );

	DrawFlatMenuKeyText(Item);
}

void DrawFlatMenuKeyNum( MENUITEM *Item )
{
	DEFKEY *kdef;
  	TEXTINFO *TextInfo;

	if ( Item->Variable )
	{
		kdef = (DEFKEY *)(Item->Variable);

		//allocate space for textinfo
		Item->TextInfo[0] = InitTextInfo();

		Item->numtextitems++;
			
		//write relevent info into structure
		TextInfo = Item->TextInfo[0];

		TextInfo->text = (char *)malloc( 16 );
		if ( kdef->def->keys > 1 )
		{
			sprintf( TextInfo->text, "%d/%d", kdef->selected_key + 1, kdef->def->keys );
		}
		else
			TextInfo->text[ 0 ] = 0;

		TextInfo->xmin = Item->x;
		TextInfo->ymin = Item->y;
		TextInfo->xmax = Item->xmax;
		TextInfo->ymax = Item->ymax;
		TextInfo->font = Item->font;
		TextInfo->flags = Item->highlightflags;
		TextInfo->pointer[ TextInfo->num_pointers_to_free++ ] = TextInfo->text;

		//add to text stack...
		AddToTextStack( TextInfo );

		//format text item...
		PrintTextItem(TextInfo);
	}
}

void DrawFlatMenuKeyLeftArrowText( MENUITEM *Item )
{
	DEFKEY *kdef;
	char *arrow;
  	TEXTINFO *TextInfo;

	arrow = EmptyString;
	if ( Item->Variable )
	{
		kdef = (DEFKEY *)(Item->Variable);
		if ( kdef->def->keys > 1 && kdef->selected_key > 0 )
		{
			arrow = "\021"; // = 17
		}
	}

	//write relevent info into structure
	TextInfo = Item->TextInfo[0];

	TextInfo->text = arrow;
	TextInfo->xmin = Item->x;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);

}

void DrawFlatMenuKeyLeftArrow( MENUITEM *Item )
{

	//allocate space for textinfo
	Item->TextInfo[0] = InitTextInfo();

	Item->numtextitems++;
		
	//add to text stack...
	AddToTextStack( Item->TextInfo[0] );

	DrawFlatMenuKeyLeftArrowText(Item);
}


void DrawFlatMenuKeyRightArrowText( MENUITEM *Item )
{
	DEFKEY *kdef;
	char *arrow;
  	TEXTINFO *TextInfo;

	arrow = EmptyString;
	if ( Item->Variable )
	{
		kdef = (DEFKEY *)(Item->Variable);
		if ( kdef->def->keys > 1 && kdef->selected_key < kdef->def->keys - 1 )
		{
			arrow = "\020"; // = 16
		}
	}

	//write relevent info into structure
	TextInfo = Item->TextInfo[0];

	TextInfo->text = arrow;
	TextInfo->xmin = Item->x;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);

}

void DrawFlatMenuKeyRightArrow( MENUITEM *Item )
{

	//allocate space for textinfo
	Item->TextInfo[0] = InitTextInfo();

	Item->numtextitems++;
		
	//add to text stack...
	AddToTextStack( Item->TextInfo[0] );

	DrawFlatMenuKeyRightArrowText(Item);
}


void DrawFlatRadioButton( MENUITEM *Item )
{

	// draw text in textinfo 0
	DrawFlatMenuItem(Item);

	//allocate space for toggle status
	Item->TextInfo[1] = InitTextInfo();

	Item->numtextitems++;

	//add to text stack...
	AddToTextStack( DrawFlatRadioBtnStatus(Item) );
}


void DrawFlatSliderStatus( MENUITEM *Item )
{
	TEXTINFO *TextInfo;
	SLIDER *slider;
	int num;
	int slidertype;
	int offset;
	float percentage;
	char *uservalue;

	slider = GetSlider ( Item, &slidertype );

	if (slider->value == slider->oldvalue)
		return;
	else
		slider->oldvalue = slider->value;

	TextInfo = Item->TextInfo[1];

	switch (slidertype)
	{
	case SLIDER_Time:
		switch (slider->value)
		{
		case 0:
			num = sprintf( TextInfo->text, LT_NoLimit/*"no limit"*/ );
			break;
		case 1:
			num = sprintf( TextInfo->text, LT_OneMinute/*"1 min"*/ );
			break;
		default:
			num = sprintf( TextInfo->text, LT_ManyMinutes/*"%d mins"*/, slider->value );
			break;
		}
		offset = 5;
		break;
	case SLIDER_Value:
		num = sprintf( TextInfo->text, "%d", slider->value );
		offset = 5;
		break;
	case SLIDER_Percent:
		num = sprintf( TextInfo->text, "%d %%", slider->value );
		offset = 58;
		break;
	case SLIDER_Sensitivity:
		num = sprintf( TextInfo->text, "%d", slider->value );
		offset = 58;
		break;
	case SLIDER_Bike:
		num = 0;
		TextInfo->text = EmptyString;
		offset = 0;
		break;
	case SLIDER_Demo:
		if( DemoSpeed.value > 8 )
		{
			// slower or normal playback speed...
			percentage = (1.0F / (float) ( DemoSpeed.value - 7 )) * 100.0F;
		}else{
			percentage = (1.0F * (float) ( 9 - DemoSpeed.value )) * 100.0F;
		}
		num = sprintf( TextInfo->text, "%3.1f%%", percentage );  
		offset = 58;
		break;
	case SLIDER_User:
		if ( slider->FuncUser )
		{
			uservalue = slider->FuncUser( slider );
			if ( uservalue )
			{
				strncpy( TextInfo->text, uservalue, MAXSLIDERINFOLENGTH - 1 );
				TextInfo->text[ MAXSLIDERINFOLENGTH - 1 ] = 0;
				num = strlen( TextInfo->text );
			}
			else
			{
				num = sprintf( TextInfo->text, LT_NoValue/*"no value" */);
			}
		}
		else
		{
			num = sprintf( TextInfo->text, "no func" );
		}
		offset = 5;
		break;
	default:
		num = sprintf( TextInfo->text, "%.0f%%", ((float)slider->value / (float)slider->max) * 100.0F );
		offset = 58;
		break;
	}
	if ( num >= MAXSLIDERINFOLENGTH)
	{
		Msg("Title.c: DrawFlatSliderStatus() sprintf trying to write too many chars\n");
		exit(1);
	}
	TextInfo->xmin = Item->xmax + offset;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax + 100;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	//format text item...
	PrintTextItem(TextInfo);
}

void UpdateSlider (MENUITEM *Item)
{
	float xmin, xmax, ymin, ymax;
	SLIDER *Slider;
	int slidertype, space, decreaseY;
	float width, height;
	float frac;

	Slider = GetSlider ( Item, &slidertype );

	frac = Slider->timer * ((float)( Slider->max - Slider->min ) / (float)Slider->max);
	frac += 1.0F - ((float)( Slider->max - Slider->min ) / (float)Slider->max);
	
	if (slidertype == SLIDER_Bike)
	{
		space = 1;
		width = 40.0F;
		decreaseY = 1;
	}else
	{
		space = 3;
		width = 50.0F;
		decreaseY = 2;
	}

	xmin = (Item->xmax + space + VDUoffsetX) * ModeScaleX;
	xmax = (Item->xmax + space + frac * width + VDUoffsetX) * ModeScaleX;
  
	ymin = (Item->y + VDUoffsetY + decreaseY) * ModeScaleY;
	ymax = (Item->ymax + VDUoffsetY - decreaseY) * ModeScaleY;

	height = (float)floor(ymax - ymin);

	xmin = (float)ceil(xmin);
	xmax = (float)ceil(xmax);
	ymin = (float)floor(ymin);
	ymax = ymin + height;
	
	ScrPolys[Slider->poly].x1 = xmin;
	ScrPolys[Slider->poly].y1 = ymin;	
	ScrPolys[Slider->poly].x2 = xmin;
	ScrPolys[Slider->poly].y2 = ymax;
	ScrPolys[Slider->poly].x3 = xmax;	
	ScrPolys[Slider->poly].y3 = ymax;
	ScrPolys[Slider->poly].x4 = xmax;					
	ScrPolys[Slider->poly].y4 = ymin;					

	ScrPolys[Slider->poly].Col1.R = 0;
	ScrPolys[Slider->poly].Col1.G = 32;
	ScrPolys[Slider->poly].Col1.B = 0;
	ScrPolys[Slider->poly].Col1.Trans = 255;
	ScrPolys[Slider->poly].Col2.R = (u_int8_t)(64.0F * frac);
	ScrPolys[Slider->poly].Col2.G = (u_int8_t)(223.0F * frac + 32.0F);
	ScrPolys[Slider->poly].Col2.B = (u_int8_t)(64.0F * frac);
	ScrPolys[Slider->poly].Col2.Trans = 255;
	ScrPolys[Slider->poly].Col3.R = (u_int8_t)(64.0F * frac);
	ScrPolys[Slider->poly].Col3.G = (u_int8_t)(223.0F * frac + 32.0F);
	ScrPolys[Slider->poly].Col3.B = (u_int8_t)(64.0F * frac);
	ScrPolys[Slider->poly].Col3.Trans = 255;
	ScrPolys[Slider->poly].Col4.R = 0;
	ScrPolys[Slider->poly].Col4.G = 32;
	ScrPolys[Slider->poly].Col4.B = 0;
	ScrPolys[Slider->poly].Col4.Trans = 255;

}

SLIDER *GetSlider( MENUITEM *Item, int *slidertype )
{
	int feature;
	SLIDER *slider;

	if (Item->FrameNo)
		*slidertype = Item->FrameNo;
	else
		*slidertype = SLIDER_Normal;

	if (*slidertype == SLIDER_Bike)
	{	
		feature = *((int *) (Item->Variable));
		slider = &BikeFeatureSlider[OldSelectedBike][feature];
	}else
		slider = (SLIDER *) (Item->Variable);

	return slider;
}

void DrawFlatMenuSlider( MENUITEM *Item )
{
	SLIDER *slider;
	bool SliderStored;
	int i;
	int slidertype;

	slider = GetSlider ( Item, &slidertype );

	slider->timer = 0.0F;
	slider->dt = framelag/( SLIDERSPEED * 60.0F);
	slider->redraw_req = true;
	slider->poly = (u_int16_t) -1;
	slider->oldvalue = -1;

	SliderStored = false;

	for (i=0; i<MAXSLIDERSPERMENU; i++)
	{
		if (!ActiveSliderItem[i].Item)
		{
			ActiveSliderItem[i].Item = Item;
			SliderStored = true;
			break;
		}
	}
	if (!SliderStored)
	{
		Msg("Title.c: DrawFlatMenuSlider() unable to store slider (increase MAXSLIDERSPERMENU?)\n");
		exit(1);
	}

	if (slidertype == SLIDER_Normal || slidertype == SLIDER_Bike || slidertype == SLIDER_Demo || slidertype == SLIDER_Percent || slidertype == SLIDER_Sensitivity )
	{
		slider->poly = FindFreeScrPoly();

		if (slider->poly == (u_int16_t)-1)
		{
			Msg("Title.c: DrawFlatMenuSlider() unable to allocate screen poly\n");
			exit(1);
		}
	
		ScrPolys[slider->poly].Type = SCRTYPE_Normal;
		ScrPolys[slider->poly].Flags = SCRFLAG_UseCoords;
		ScrPolys[slider->poly].Type = SCRTYPE_Normal;

		ScrPolys[slider->poly].Col1.R = 0;
		ScrPolys[slider->poly].Col1.G = 32;
		ScrPolys[slider->poly].Col1.B = 0;
		ScrPolys[slider->poly].Col1.Trans = 0;
		ScrPolys[slider->poly].Col2.R = 64;
		ScrPolys[slider->poly].Col2.G = 255;
		ScrPolys[slider->poly].Col2.B = 64;
		ScrPolys[slider->poly].Col2.Trans = 0;
		ScrPolys[slider->poly].Col3.R = 64;
		ScrPolys[slider->poly].Col3.G = 255;
		ScrPolys[slider->poly].Col3.B = 64;
		ScrPolys[slider->poly].Col3.Trans = 0;
		ScrPolys[slider->poly].Col4.R = 0;
		ScrPolys[slider->poly].Col4.G = 32;
		ScrPolys[slider->poly].Col4.B = 0;
		ScrPolys[slider->poly].Col4.Trans = 0;

		ScrPolys[slider->poly].Frame = 238.0F;
		ScrPolys[slider->poly].Frm_Info = NULL;
		AddScrPolyToTPage( slider->poly, GetTPage( NULL, 0 ) );

		UpdateSlider ( Item );
	}
	// draw text in textinfo 0
	DrawFlatMenuItem(Item);

	//allocate space for slider status
	Item->TextInfo[1] = InitTextInfo();

	//allocate space for slider info string
	Item->TextInfo[1]->text = (char*)malloc ( sizeof (char) * MAXSLIDERINFOLENGTH );

	if (!Item->TextInfo[1]->text)
	{	Msg("Title.c   DrawFlatMenuSlider:  unable to allocate memory\n");
		exit(1);
	}

	Item->TextInfo[1]->pointer[Item->TextInfo[1]->num_pointers_to_free++] = Item->TextInfo[1]->text;
	
	Item->numtextitems++;

	DrawFlatSliderStatus(Item);

	//add to text stack...
	AddToTextStack( Item->TextInfo[1] );

}

void ProcessFlatSliders(void)
{
	int i, slidertype;
	SLIDER *slider;
	float ReqFraction;
	
	for (i=0; i<MAXSLIDERSPERMENU; i++)
	{
		if (ActiveSliderItem[i].Item)
		{
			slider = GetSlider ( ActiveSliderItem[i].Item, &slidertype );
		 
			if (slider->redraw_req)
			{
				//DebugPrintf("slider redrawn\n");
				RedrawFlatMenuSlider(ActiveSliderItem[i].Item);
			}

			ReqFraction = (float)(slider->value - slider->min) / (float)(slider->max - slider->min);
			if (slider->timer < 0.0F)
				continue;
			slider->timer += slider->dt;
			if (( (slider->timer >= ReqFraction ) && ( slider->dt > 0.0F ) ) ||
				( (slider->timer <= ReqFraction ) && ( slider->dt < 0.0F ) ))
			{
			   	slider->timer = ReqFraction;
				if (slider->poly != (u_int16_t) -1)
				   	UpdateSlider(ActiveSliderItem[i].Item);
				slider->timer = -1.0F;
			}else
			{
				if (slider->poly != (u_int16_t) -1)
					UpdateSlider(ActiveSliderItem[i].Item);
			}

		}
	}
}
 
void RedrawFlatMenuSlider( MENUITEM *Item )
{
	SLIDER *slider;
	int slidertype;

	slider = GetSlider ( Item, &slidertype );

	slider->redraw_req = false;

	DrawFlatSliderStatus(Item);

	if (slidertype == SLIDER_Normal || slidertype == SLIDER_Demo || slidertype == SLIDER_Percent || slidertype == SLIDER_Sensitivity )
	{

		if (slider->timer < 0.0F)
			slider->timer = (slider->oldvalue - (float)slider->min) / (float)(slider->max - slider->min);

		if (slider->value != slider->oldvalue)
		{
			if (slider->value > slider->oldvalue)
				slider->dt = (float)fabs(slider->dt);
			else
				slider->dt = 0.0F - (float)fabs(slider->dt);
		}
	}
}

void DrawFlatMenuName( MENUITEM *Item )
{
	char *var;
	TEXTINFO *TextInfo;

	//var = (char *)(Item->Variable);
	var = Item->Variable;
	  
	//allocate space for textinfo
	Item->TextInfo[0] = InitTextInfo();
	Item->numtextitems++;
		
	//write relevent info into structure
	TextInfo = Item->TextInfo[0];

	TextInfo->text = var;
	TextInfo->xmin = Item->x;
	TextInfo->ymin = Item->y;
	TextInfo->xmax = Item->xmax;
	TextInfo->ymax = Item->ymax;
	TextInfo->font = Item->font;
	TextInfo->flags = Item->highlightflags;

	if (Item->highlightflags & TEXTFLAG_CheckForRefresh)
	{
		//TextInfo->oldtext = (char*)malloc ( sizeof (char) * strlen(var) );
		//DebugPrintf("DrawFlatMenuName() allocated %d\n", strlen(var));
		//TextInfo->pointer[TextInfo->num_pointers_to_free++] = TextInfo->oldtext;
		strncpy(TextInfo->oldtext, TextInfo->text, MAX_TEXT_LENGTH);
	}

	//add to text stack...
	AddToTextStack( TextInfo );

	//format text item...
	PrintTextItem(TextInfo);
	
}

void DrawGeneralWeaponText( MENUITEM *Item )
{
	int i, offset, Total_Slots, line;
	TEXTINFO *TextInfo;

	Total_Slots = 12;

	for (i = 0; i < MaxWeapons; i++)
	{
		if (i < MaxWeapons - Num_Unused_Weapons)
		{
			offset = 0;
			line = i;
		}
		else
		{
			offset = Item->xmax + 5;
			line = i - (MaxWeapons - Num_Unused_Weapons);
		}

		TextInfo = Item->TextInfo[i];

		// if the same as existing item, no need to redraw...
		if ((TextInfo->xmin == Item->x + offset) &&
			(TextInfo->ymin == (((Item->ymax - Item->y)/Total_Slots) * line) + Item->y) &&
			(TextInfo->text == (u_int8_t*)DescriptionList[WeaponList[i]]))
			continue;

		TextInfo->xmin = Item->x + offset;
		TextInfo->ymin = (((Item->ymax - Item->y)/Total_Slots) * line) + Item->y;

		TextInfo->text = DescriptionList[WeaponList[i]];
		
		TextInfo->xmax = Item->xmax + offset;
		TextInfo->ymax = (((Item->ymax - Item->y)/Total_Slots) * (line + 1)) + Item->y;
		TextInfo->font = Item->font;				  
		TextInfo->flags = Item->highlightflags;

		PrintTextItem(TextInfo);

	}


}

static u_int16_t GetNumUsed( int16_t *priority, bool *valid, u_int16_t Max )
{
	u_int16_t i, unused;

	unused = 0;

	for ( i = 0; i < MaxWeapons; i++ )
	{
		if ( ( priority[ i ] < 0 ) && ( valid[ i ] ) )
		{
			 unused++;
		}
	}

	return unused;
}

void DrawPrimaryWeapons( MENUITEM *Item )
{
	MaxWeapons = MAX_PRIMARY_WEAPONS;
	PriorityList = Config.primary_priority;
	OrderList = Config.primary_order;
	DescriptionList = PrimaryDescription;
	WeaponValid = PrimaryValid;

	Num_Unused_Weapons = GetNumUsed( PriorityList, WeaponValid, MaxWeapons );
	DrawGeneralWeapons( Item );
}

void DrawSecondaryWeapons( MENUITEM *Item )
{
	MaxWeapons = MAX_SECONDARY_WEAPONS;
	PriorityList = Config.secondary_priority;
	OrderList = Config.secondary_order;
	DescriptionList = SecondaryDescription;
	WeaponValid = SecondaryValid;

	Num_Unused_Weapons = GetNumUsed( PriorityList, WeaponValid, MaxWeapons );
	DrawGeneralWeapons( Item );
}

void StoreWeaponOrder( MENU *Menu )
{
	int i;

	for (i = 0; i < MaxWeapons - Num_Unused_Weapons; i++)
	{
		OrderList[i] = WeaponList[i];
		PriorityList[WeaponList[i]] = i;
	}

	for ( ; i < MaxWeapons; i++ )
	{
		OrderList[i] = -1;
		PriorityList[WeaponList[i]] = -1;
	}
	
//	if ( memcmp( &Config, player_config, sizeof( Config ) ) )
	{
		*player_config = Config;
		write_config( player_config, biker_config );
	}
}

void DrawGeneralWeapons( MENUITEM *Item )
{
	int i, num_weapons;

	bool used[ MAX( MAX_PRIMARY_WEAPONS, MAX_SECONDARY_WEAPONS ) ];

	memset( used, false, MAX( MAX_PRIMARY_WEAPONS, MAX_SECONDARY_WEAPONS ) * sizeof ( bool ) ); 

	SelectedWeapon = 0;
	WeaponItem = Item;
	WeaponOrderStatus = WEAPON_ChoosingFirst;
#if 0
	IdiotText = InitTextInfo();
	Item->numtextitems++;
	AddToTextStack( IdiotText );
	IdiotText->xmin = MENU_ITEM_IdiotText.x;
	IdiotText->ymin = MENU_ITEM_IdiotText.y;
	IdiotText->xmax = MENU_ITEM_IdiotText.xmax;
	IdiotText->ymax = MENU_ITEM_IdiotText.ymax;
	IdiotText->font = MENU_ITEM_IdiotText.font;
	IdiotText->text = WeaponSelectFirst;

	PrintTextItem(IdiotText);
#endif
	for (i = 0; i < MaxWeapons; i++)
	{
		Item->TextInfo[i] = InitTextInfo();
		Item->numtextitems++;
		AddToTextStack( Item->TextInfo[i] );
		Item->TextInfo[i]->highlight = false;
		Item->TextInfo[i]->highlighttype = HIGHLIGHT_Pulsing;
	}

//	Num_Unused_Weapons, MaxWeapons, WeaponList[];
	//	if ( !PrimaryValid[ WeaponList[ weapon ] ] )

	num_weapons = 0;
	for ( i = 0; i < MaxWeapons; i++ )
	{
		if ( ( OrderList[ i ] != -1 ) && ( !used[ OrderList[ i ] ] ) )
		{
			if ( WeaponValid[ OrderList[ i ] ] )
			{
				WeaponList[ num_weapons++ ] = OrderList[ i ];
				used[ OrderList[ i ] ] = true;
			}
		}
	}

	for ( i = num_weapons; i < MaxWeapons; i++ )
	{
		for ( i = 0; i < MaxWeapons; i++ )
		{
			if ( !used[ i ] && WeaponValid[ i ] )
				WeaponList[ num_weapons++ ] = i;
		}
	}

	MaxWeapons = num_weapons;

	DrawGeneralWeaponText( Item );
	Item->TextInfo[0]->highlight = true;
	Item->TextInfo[0]->highlighttype = HIGHLIGHT_Pulsing;

	MenuState = MENUSTATE_WeaponOrder;
}

/*===================================================================
	Procedure	:		Processes menu items for VDU menus
	Input		:		ptr to menu
	Output		:		nothing
===================================================================*/
void ProcessVduItems( MENU *Menu )
{
	MENUITEM *Item;
	if(!Menu)return;
	for( Item = Menu->Item ; Item->x >= 0 ; Item++ )
	{	if( Item->FuncDraw )
		{  
			Item->FuncDraw( Item );
		}
	}
}

void KillTextInfo ( TEXTINFO *TextInfo )
{
	int i;
	
	if ( TextInfo )
	{
		while(TextInfo->num_pointers_to_free)
		{	free(TextInfo->pointer[TextInfo->num_pointers_to_free - 1]);
			TextInfo->num_pointers_to_free--;
		}
		free (TextInfo);
		// remove ptr from stack...
		for ( i = 0; i < TextStackLevel; i++ )
		{
			if ( TextStack[ i ] == TextInfo )
			{
				TextStack[ i ] = NULL;
				break;
			}
		}
		TextInfo = NULL;
	}
}

/*===================================================================
	Procedure	:		clears everything from the VDU
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void VduClear ( void )
{
	int i, tempstacklevel, slidertype;
	MENUITEM *Item;
	SLIDER *slider;

	NewTextCharDisplayed = false;

	tempstacklevel = 0;

	for (i=0; i<TextStackLevel; i++)
		if ( TextStack[ i ] )
		{
			TextStack[i]->boxdone = false;
			KillTextInfo( TextStack[i] );
		}

	TextStackLevel = 0;
	CurrentTeletype = 0;

	if(LastMenu)
		for( Item = LastMenu->Item ; Item->x >= 0 ; Item++ )
			Item->numtextitems = 0;

		
	// kill all polys used for boxes...
	for (i=0; i<CurrentScreenPoly; i++)
	{
		KillUsedScrPoly( screenpoly[i] );
		screenpoly[i] = (u_int16_t)-1;
	}

	CurrentScreenPoly = 0;

	for (i=0; i<MAXSLIDERSPERMENU; i++)
		if (ActiveSliderItem[i].Item)
		{
			slider = GetSlider ( ActiveSliderItem[i].Item, &slidertype );
			if (slider->poly != (u_int16_t) -1)
			{
				KillUsedScrPoly( slider->poly );
				slider->poly = (u_int16_t)-1;
			}
			ActiveSliderItem[i].Item = NULL;
		}

	PlaySfx( SFX_VduClear, 0.4F );

}

/*********************************************************************************************************
start of VDU text routines
*********************************************************************************************************/

bool Plot2dBox (TEXTINFO *TextInfo)
{
	u_int8_t r, g, b;

	screenpoly[CurrentScreenPoly] = FindFreeScrPoly();					
	testboxpoly = screenpoly[CurrentScreenPoly];
	if( screenpoly[CurrentScreenPoly] != (u_int16_t ) -1 )
	{

		ScrPolys[screenpoly[CurrentScreenPoly]].Type = SCRTYPE_Normal;
		ScrPolys[screenpoly[CurrentScreenPoly]].Flags = SCRFLAG_UseCoords;
		ScrPolys[screenpoly[CurrentScreenPoly]].R = 255;					
		ScrPolys[screenpoly[CurrentScreenPoly]].G = 255;					
		ScrPolys[screenpoly[CurrentScreenPoly]].B = 255;				  
		ScrPolys[screenpoly[CurrentScreenPoly]].Trans = 255;				
		ScrPolys[screenpoly[CurrentScreenPoly]].Xscale = 1.0F;				
		ScrPolys[screenpoly[CurrentScreenPoly]].Yscale = 1.0F;			  
		ScrPolys[screenpoly[CurrentScreenPoly]].Frame = 11.0F;			 
		ScrPolys[screenpoly[CurrentScreenPoly]].Frm_Info = NULL;
		ScrPolys[screenpoly[CurrentScreenPoly]].x1 = ((float)TextInfo->xmin + VDUoffsetX) * ModeScaleX;
		ScrPolys[screenpoly[CurrentScreenPoly]].y1 = ((float)TextInfo->ymin + VDUoffsetY) * ModeScaleY;
		ScrPolys[screenpoly[CurrentScreenPoly]].x2 = ((float)TextInfo->xmax + VDUoffsetX) * ModeScaleX;
		ScrPolys[screenpoly[CurrentScreenPoly]].y2 = ((float)TextInfo->ymin + VDUoffsetY) * ModeScaleY;
		ScrPolys[screenpoly[CurrentScreenPoly]].x3 = ((float)TextInfo->xmax + VDUoffsetX) * ModeScaleX;
		ScrPolys[screenpoly[CurrentScreenPoly]].y3 = ((float)TextInfo->ymax + VDUoffsetY) * ModeScaleY;
		ScrPolys[screenpoly[CurrentScreenPoly]].x4 = ((float)TextInfo->xmin + VDUoffsetX) * ModeScaleX;
		ScrPolys[screenpoly[CurrentScreenPoly]].y4 = ((float)TextInfo->ymax + VDUoffsetY) * ModeScaleY;
		AddScrPolyToTPage(screenpoly[CurrentScreenPoly], GetTPage( NULL, 0 ) );
    	//AddScrPolyToTPage( screenpoly[CurrentScreenPoly], GetTPage( *ScrPolys[ screenpoly[CurrentScreenPoly] ].Frm_Info, 0 ) );
		
		r = 255; g = 255; b = 255;
		
		ScrPolys[screenpoly[CurrentScreenPoly]].Col1.R = r;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col1.G = g;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col1.B = b;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col1.Trans = 255;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col2.R = r;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col2.G = g;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col2.B = b;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col2.Trans = 255;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col3.R = r;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col3.G = g;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col3.B = b;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col3.Trans = 255;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col4.R = r;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col4.G = g;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col4.B = b;
		ScrPolys[screenpoly[CurrentScreenPoly]].Col4.Trans = 255;

		CurrentScreenPoly++;
	}
	else
		return false;

	return true;

}

int GetScreenFont(int fontsize)
{
	// was the highest settings around for 512
	TEXTINFO_TextSpace = 2.0F;
	TEXTINFO_LineSpace = 2.0F * ModeScaleY;
	// adding fontsize moves it up to medium or large in the array
	return FONT_512X384_Small + fontsize;
}

bool FormatTextItem(int start, int end, TEXTINFO *TextInfo, bool *newline)
{
	int i,font;
	float TotalWidth, RemainingWidth, WordWidth, TrailingSpaceWidth, TotalHeight, PreviousHeight;
	bool TrailingSpace, ForceNewLineNext;
	u_int16_t CurrentChar;
	float xmin, xmax, ymin, ymax;

	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	
	*newline = false;


	xmin = TextInfo->xmin * ModeScaleX;
	xmax = TextInfo->xmax * ModeScaleX;
	ymin = TextInfo->ymin * ModeScaleY;
	ymax = TextInfo->ymax * ModeScaleY;

	TotalWidth = (float)(xmax - xmin);
	TotalHeight = (float)(ymax - ymin);
	RemainingWidth = xmax - TEXTINFO_currentx - xmin;

	font = GetScreenFont(TextInfo->font);
	
	 //find total width of word...
	WordWidth = 0.0F;
	TrailingSpace = false;
	ForceNewLineNext = false;

	//store current height, as it will be needed if text wraps...
	PreviousHeight = TEXTINFO_currentheight;
	TEXTINFO_currentheight = 0.0F;

	for (i = start; i <= end; i++)
	{
		CurrentChar = TextLookup[font][TextInfo->text[i]];
					  
		if ((CurrentChar) || (TextInfo->text[i] == '~'))
		{
			Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(CurrentChar) );
			Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			WordWidth += Box_Ptr->xsize * VduScaleX;
		
		
			if ( i == end)
			{
				if (TextInfo->text[i] == ' ')
					TrailingSpace = true;
				if (TextInfo->text[i] == '~')
					ForceNewLineNext = true;
			
			}else
				WordWidth += TEXTINFO_TextSpace; //add a space between characters


			if (Box_Ptr->ysize * VduScaleY > TEXTINFO_currentheight)
				TEXTINFO_currentheight = Box_Ptr->ysize * VduScaleY;

		}
	}

	Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font][' ']) );
	Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	TrailingSpaceWidth = Box_Ptr->xsize * VduScaleX;


	// if word is too long for bounding box...
	if (WordWidth > TotalWidth)
	{	
		if (TextInfo->flags & TEXTFLAG_ForceFit)
		{
			TextInfo->force_fit = true;
			return false;
		}

		if (TrailingSpace)
		{
			if (WordWidth - TrailingSpaceWidth > TotalWidth)
				return false;
			else
			{	end--;	//word will fit without trailing space, so reduce end pointer
				WordWidth -= TrailingSpaceWidth;
			}
		}
		else
			return false;
	}


	// if word will not fit on current line...
	if ((WordWidth > RemainingWidth) || ForceNewLine)
	{
		
		//try stripping trailing space if any...
		if (TrailingSpace && (RemainingWidth >= WordWidth - TrailingSpaceWidth) && !ForceNewLine)
		{	end--;
			WordWidth -= TrailingSpaceWidth;
		}
		else
		{
			if (TEXTINFO_currenty + PreviousHeight + TEXTINFO_currentheight + TEXTINFO_LineSpace > TotalHeight)
			{
		 		if (TextInfo->flags & TEXTFLAG_ForceFit)
				{
					TextInfo->force_fit = true;
					return false;
				}
		
				return false;	//no room for new line
			}
			else
			{	
				//start new line...
				
				//remove trailing space if still there..
				if (TextInfo->text[TEXTINFO_currentendpoint] == ' ')
				{
					TEXTINFO_currentendpoint--;
					TEXTINFO_currentx -= TrailingSpaceWidth;
				}

				TextInfo->length[TextInfo->num_lines-1] = TEXTINFO_currentx;
				TextInfo->height[TextInfo->num_lines-1] = TEXTINFO_currentheight + TEXTINFO_LineSpace;
				TextInfo->endpoint[TextInfo->num_lines-1] = TEXTINFO_currentendpoint;
				TextInfo->startpoint[TextInfo->num_lines] = start;
				TextInfo->num_lines++;
				*newline = true;
				
				TEXTINFO_currenty += PreviousHeight + TEXTINFO_LineSpace;
				TEXTINFO_currentx = 0.0F;
				//TEXTINFO_currentheight = 0.0F;

				ForceNewLine = false;
			}
		}
	}

	if (!(*newline))	//if no new line was started, take previous height info into account
	{	if (PreviousHeight > TEXTINFO_currentheight)
			TEXTINFO_currentheight = PreviousHeight;
	}

	if (ForceNewLineNext)
	{	ForceNewLine = true;
		end--;
	}

	TEXTINFO_currentendpoint = end;
	 
	TEXTINFO_currentx += WordWidth + TEXTINFO_TextSpace;

	for ( i = 0; i < TextInfo->num_lines; i++ )
		TextInfo->finished[ i ] = false;

	return true;
	
}


bool DrawTextEditCursor( TEXTINFO *TextInfo, int font )
{
	float xmin, ymin;
	char cursor = '!';
	int16_t lookup;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	u_int16_t TempPoly;
	u_int8_t i;
  	TEXT *t;

	t = (TEXT *)(TextItem->Variable);

	xmin = TextInfo->char1x;
	ymin = TextInfo->text_y_min;

	Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font][cursor]) );
	Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	ymin += ( VduScaleY * Box_Ptr->ysize );

	for (i = t->VDU_insert_pos; i < t->insert_pos; i++)
	{
		lookup = TextLookup[font][TextInfo->text[TextInfo->startpoint[0] + i - t->VDU_insert_pos]];

		if (lookup != 0)
		{
			Bit_Ptr = ( Title_Fonts_Header->Bit_Info + lookup );
			Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			xmin += VduScaleX * ( Box_Ptr->xsize + TEXTINFO_TextSpace );
		}
	}

	xmin -= (TEXTINFO_TextSpace / 2.0F);

	TempPoly = FindFreeScrPoly();					
	if( TempPoly != (u_int16_t ) -1 )
	{
					   	
		ScrPolys[TempPoly].Type = SCRTYPE_LastAFrame;
		ScrPolys[TempPoly].Flags = SCRFLAG_Nothing;

		ScrPolys[TempPoly].Pos.x = xmin;			  
		ScrPolys[TempPoly].Pos.y = ymin;
		ScrPolys[TempPoly].R = 255;					
		ScrPolys[TempPoly].G = 255;					
		ScrPolys[TempPoly].B = 255;				  
		ScrPolys[TempPoly].Trans = 255;				
		ScrPolys[TempPoly].Xscale = VduScaleX;				
		ScrPolys[TempPoly].Yscale = VduScaleY;			  
		ScrPolys[TempPoly].Frame = TextLookup[font][cursor];
		ScrPolys[TempPoly].Frm_Info = &Title_Fonts_Header;
					
		AddScrPolyToTPage( TempPoly, GetTPage( *ScrPolys[ TempPoly ].Frm_Info, 0 ) );

	}
	else
		return false;

	return true;
}
 
void GetTextPos(TEXTINFO *TextInfo, float *xpos, float *ypos, int line, float totalheight)
{

	float xmin, ymin, xmax, ymax, currentx, currenty;

	currentx = TextInfo->currentx[line];
	currenty = TextInfo->currenty;

	xmin = (TextInfo->xmin + VDUoffsetX) * ModeScaleX;
	xmax = (TextInfo->xmax + VDUoffsetX) * ModeScaleX;
	ymin = (TextInfo->ymin + VDUoffsetY) * ModeScaleY;
	ymax = (TextInfo->ymax + VDUoffsetY) * ModeScaleY;

	//default x, y pos...
	*xpos = (currentx + xmin);	
		  
	*ypos = (currenty + ymin + TextInfo->height[0] - TEXTINFO_LineSpace);

	if (TextInfo->flags)
	{
			 
		if (TextInfo->flags & TEXTFLAG_RightX)
			*xpos = (currentx + xmin) + xmax - xmin - TextInfo->length[line];

		if (TextInfo->flags & TEXTFLAG_CentreX)
			*xpos = (xmin + currentx + (int)((float)(xmax - xmin)/2.0F - TextInfo->length[line]/2.0F)) - TEXTINFO_TextSpace * 2.0F;			  

		if (TextInfo->flags & TEXTFLAG_CentreY)
			*ypos = ((ymax - ymin)/(TextInfo->num_lines + 1)) * (line + 1) + ymin + (TextInfo->height[0] - TEXTINFO_LineSpace) * ((line+1)/(float)(TextInfo->num_lines + 1));
		
		if (TextInfo->flags & TEXTFLAG_BottomY)
			*ypos = (ymax - totalheight + currenty);
		
		if (TextInfo->flags & TEXTFLAG_CentreYBunched)
			*ypos = ( TextInfo->height[0] - TEXTINFO_LineSpace + currenty + (ymax - ymin - totalheight) / 2.0F ) + ymin;

	}

}

bool DisplayTextCharacter(TEXTINFO *TextInfo, int line, int pos, int font, float totalheight)
{
   
	float xpos, ypos, currentx, currenty, xmin;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	bool	OKtoProcess;
	u_int16_t TempPoly;

	currentx = TextInfo->currentx[line];
	currenty = TextInfo->currenty;

	xmin = (TextInfo->xmin + VDUoffsetX) * ModeScaleX;

	OKtoProcess = false;

	if ((pos != -1) && (pos != -2))
	{	if (TextLookup[font][TextInfo->text[pos]])
			OKtoProcess = true;		
//		else
			//TextInfo->currentchar[line]++;
	}
	
	if ((OKtoProcess) || (pos == -1) || (pos == -2))
	{
	
		GetTextPos(TextInfo, &xpos, &ypos, line, totalheight);

		if (pos == TextInfo->startpoint[line])
		{
			if ( xpos < TextInfo->char1x )
				 TextInfo->char1x = xpos;

			if (line == 0)
				TextInfo->text_y_min = ypos - TextInfo->height[line] + TEXTINFO_LineSpace;
			if (line == TextInfo->num_lines - 1)
				TextInfo->text_y_max = ypos;
		}

		switch(pos)
		{
		case -1:
			Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font][71]) );	// right arrow character
			Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			xpos = xmin - Box_Ptr->xsize * VduScaleX - TEXTINFO_TextSpace;

			break;
		case -2:
			break;
		default:
			Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font][TextInfo->text[pos]]) );
			Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			TextInfo->currentx[line] += Box_Ptr->xsize * VduScaleX + TEXTINFO_TextSpace;
			break;
		}

		TempPoly = FindFreeScrPoly();					
		if( TempPoly != (u_int16_t ) -1 )
		{
					   	
			ScrPolys[TempPoly].Type = SCRTYPE_LastAFrame;
			ScrPolys[TempPoly].Flags = SCRFLAG_Nothing;

			ScrPolys[TempPoly].Pos.x = xpos;			  
			ScrPolys[TempPoly].Pos.y = ypos;
			if (TextInfo->flags & TEXTFLAG_NotImplemented)
			{
				ScrPolys[TempPoly].R = 100;					
				ScrPolys[TempPoly].G = 100;
				ScrPolys[TempPoly].B = 100;				  
			}
			else
			{
				ScrPolys[TempPoly].R = 255;					
				ScrPolys[TempPoly].G = 255;					
				ScrPolys[TempPoly].B = 255;				  
			}

			ScrPolys[TempPoly].Trans = 255;				
			ScrPolys[TempPoly].Xscale = VduScaleX;				
			ScrPolys[TempPoly].Yscale = VduScaleY;			  
			switch (pos)
			{
				case -1:
					ScrPolys[TempPoly].Frame = TextLookup[font][16];
					break;
				case -2:
					ScrPolys[TempPoly].Frame = TextLookup[font]['*'];
					ScrPolys[TempPoly].Type = SCRTYPE_LastAFrame;
					break;
				default:
					ScrPolys[TempPoly].Frame = TextLookup[font][TextInfo->text[pos]];
			}
								
			ScrPolys[TempPoly].Frm_Info = &Title_Fonts_Header;
				
			AddScrPolyToTPage( TempPoly, GetTPage( *ScrPolys[ TempPoly ].Frm_Info, 0 ) );

		}
		else
		{
			return false;
		}
	}

	return true;
}

void Print3Dots(TEXTINFO *TextInfo, float totalheight)
{
	int font, i;
	float xpos, ypos, width;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	u_int16_t TempPoly;
	
	font = GetScreenFont(TextInfo->font);

	GetTextPos(TextInfo, &xpos, &ypos, TextInfo->num_lines - 1, totalheight);
		
	
	Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font]['.']) );
	Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	width = Box_Ptr->xsize;


	for (i = 0; i < 3; i++)
	{
		TempPoly = FindFreeScrPoly();					
		if( TempPoly != (u_int16_t ) -1 )
		{
			ScrPolys[TempPoly].Type = SCRTYPE_LastAFrame;
			ScrPolys[TempPoly].Flags = SCRFLAG_Nothing;

			ScrPolys[TempPoly].Pos.x = xpos;			  
			ScrPolys[TempPoly].Pos.y = ypos;
			ScrPolys[TempPoly].R = 255;					
			ScrPolys[TempPoly].G = 255;					
			ScrPolys[TempPoly].B = 255;				  
			ScrPolys[TempPoly].Trans = 255;				
			ScrPolys[TempPoly].Xscale = VduScaleX;				
			ScrPolys[TempPoly].Yscale = VduScaleY;			  
			ScrPolys[TempPoly].Frame = TextLookup[font]['.'];
			ScrPolys[TempPoly].Type = SCRTYPE_LastAFrame;
			ScrPolys[TempPoly].Frm_Info = &Title_Fonts_Header;
			AddScrPolyToTPage( TempPoly, GetTPage( *ScrPolys[ TempPoly ].Frm_Info, 0 ) );
		}
		else
		{
			Msg("Unable to allocate screen poly\n");
			exit(1);
		}
		xpos += width + TEXTINFO_TextSpace;
	}
}

bool VduFinished( MENU *Menu )
{
	int i, j;
	bool eventsactive = false;
	TITLE_EVENT *Event;

	if ( Menu && ( Menu != CurrentMenu ) )
		return false;

	if ( !TextStackLevel )
		return false;
	
	for (i=0; i<TextStackLevel; i++)
	{	
		for ( j = 0; j < TextStack[i]->num_lines; j++ )
		{
			if ( !TextStack[ i ]->finished[ j ] )
				return false;
		}
	}



	for (Event = Title_Events; Event->TriggeredBy >= 0; Event++)
	{
		switch( Event->Status )
		{
		case TITLE_EVENT_STATUS_IDLE:
			break;
		default:
			eventsactive = true;
		}
	}

	if ( !eventsactive )
		return true;
	else
		return false;
}

void DisplayTextItem (TEXTINFO *TextInfo)
{
	int i, j, font, num_invalid_chars, skip, UpToChar;
	float totalheight = 0.0;
	TEXTINFO TempTextInfo;
  	TEXT *t = NULL;
	bool DoForceFit;

	if (MenuState == MENUSTATE_Text2)
		t = (TEXT *)(TextItem->Variable);
	
	font = GetScreenFont(TextInfo->font);

	num_invalid_chars = 0;

	for (i = 0; i < TextInfo->num_lines; i++)
	{
		totalheight += TextInfo->height[i];
	}


	TextInfo->currenty = 0.0F;
	DoForceFit = false;

	for (i=0; i<TextInfo->num_lines; i++)
	{
		TextInfo->currentx[i] = 0.0F;
		skip = TeleTypeSkip;

		while (skip > 0)
		{
			if ((TextInfo->currentchar[i] + TextInfo->startpoint[i]) <= TextInfo->endpoint[i])
			{
				TextInfo->currentchar[i]++;
				NewTextCharDisplayed = true;
			}
			skip--;
		}

		if ( !NewTextCharDisplayed )
			TextInfo->finished[ i ] = true;
		
		if ( (i == TextInfo->num_lines - 1) && (TextInfo->currentchar[i] - 1 == TextInfo->endpoint[i] - TextInfo->startpoint[i]))
		{
			DoForceFit = true;
		}

		if( TextInfo->flags & TEXTFLAG_Instant)
			UpToChar = TextInfo->endpoint[i] + 1;
		else
			UpToChar = TextInfo->currentchar[i] ;

		for (j=0; j <= UpToChar; j++)
		{
			if ((MenuState == MENUSTATE_Text2) && (t->insert_pos - t->VDU_insert_pos == j))
			{	
				if ((TextInfo == TextItem->TextInfo[1]) && CursorTimer > 1.0F)
					DrawTextEditCursor( TextItem->TextInfo[1], font );
			}

			if (j != UpToChar)
			{
				if (TextLookup[font][TextInfo->text[TextInfo->startpoint[i] + j]])
					DisplayTextCharacter(TextInfo, i,TextInfo->startpoint[i] + j, font, totalheight);
			}
		}
	
		if (i != TextInfo->num_lines - 1)
		{
			TextInfo->currenty += TextInfo->height[i];
		}
	}

	// if a force fit is required and necessary, and we have already printed the last
	// character of the last line...
	if ((TextInfo->flags & TEXTFLAG_ForceFit) && (TextInfo->force_fit) &&
		( DoForceFit ))
	{
		Print3Dots(TextInfo, totalheight);
	}

	TextInfo->currenty = 0.0F;
	TempTextInfo = *TextInfo;

	
	if (!(TempTextInfo.flags & TEXTFLAG_Instant))
	{
		for (i=0; i<TempTextInfo.num_lines; i++)
		{
			if (TempTextInfo.currentchar[i] <= (TempTextInfo.endpoint[i] - TempTextInfo.startpoint[i]))  
			{
				DisplayTextCharacter(&TempTextInfo, i, -2, font, totalheight);
				TextInfo->finished[ i ] = false;
			}
			
			TempTextInfo.currenty += TempTextInfo.height[i];
		}
	}
}
 	
void GetFinalChars( TEXTINFO *TextInfo, int currentpos, int length)
{
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	int i;
	float width;
	int font;

	font = GetScreenFont(TextInfo->font);
	
	width = TextInfo->xmax * ModeScaleX - TextInfo->xmin * ModeScaleX;

	for (i = currentpos; i < length; i++)
	{
		Bit_Ptr = ( Title_Fonts_Header->Bit_Info + (int16_t)(TextLookup[font][TextInfo->text[i]]) );	// right arrow character
		Off_Ptr = ( Title_Fonts_Header->Off_Info + Bit_Ptr->startbit );
		Box_Ptr = ( Title_Fonts_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );


		if (TEXTINFO_currentx + Box_Ptr->xsize + TEXTINFO_TextSpace < width)
		{
			TEXTINFO_currentx += Box_Ptr->xsize + TEXTINFO_TextSpace;
			TEXTINFO_currentendpoint++;
		}else
			break;
	}

	TEXTINFO_currentendpoint--;
	
}
						 
void PrintTextItem (TEXTINFO *TextInfo)
{
	int StartPos;
	int length;
	u_int8_t *str = NULL;
	int i;
	bool newline;

	if(TextInfo && TextInfo->text)
		str = TextInfo->text;

	if (!str)
		str = EmptyString;

	length = strlen(str);

	TEXTINFO_currentx = 0.0F;
	TEXTINFO_currenty = 0.0F;	
	TEXTINFO_currentheight = 0.0F;
	TextInfo->num_lines = 1;
	TextInfo->char1x = 999.0F;

	StartPos = 0;
	TEXTINFO_currentendpoint = 0;
	TextInfo->startpoint[0] = 0;
	TextInfo->endpoint[0] = 0;
	TextInfo->force_fit = false;
	
	newline = true;
	ForceNewLine = false;

	if (length == 0)
	{
	 	TextInfo->num_lines = 0;
		return;	//do nothing if there is no text to process
	}
	
	for (i=0; i<length; i++)
	{
		if ((str[i] == ' ') || (str[i] == '~') || (i == length - 1))
		{
			if (newline)
				TEXTINFO_currentstartpoint = StartPos;
			if(!FormatTextItem(StartPos, i, TextInfo, &newline))
			{
			 	if (!(TextInfo->flags & TEXTFLAG_ForceFit))
				{
					//Msg( "FormatTextItem() : returned false\nbounding box for text ( %s ) too small??\n", TextInfo->text );
					DebugPrintf( "FormatTextItem() : returned false\tbounding box for text ( %s ) too small??\n", TextInfo->text );
				}
				else
				{
				  // need to find out how many chars of last word will fit...
					GetFinalChars( TextInfo, TEXTINFO_currentendpoint, length);
				}

				break;
			}
			else
				StartPos = i + 1;
		}


	}
	
	if (newline)
		TEXTINFO_currentstartpoint = StartPos;

	TextInfo->length[TextInfo->num_lines-1] = TEXTINFO_currentx;
	TextInfo->height[TextInfo->num_lines-1] = TEXTINFO_currentheight + TEXTINFO_LineSpace;
	TextInfo->endpoint[TextInfo->num_lines-1] = TEXTINFO_currentendpoint;

	for (i=0; i<TextInfo->num_lines; i++)
	{
		TextInfo->currentchar[i] = 0;
		TextInfo->currentx[i] = 0.0F;
	}

	TextInfo->currenty = 0.0F;

	//TeleTypeTimer = 0.0F;

	for (i = 0; i < TextInfo->num_lines; i++)
	{
		TextInfo->currentchar[i] = 0;
		TextInfo->currentx[i] = 0.0F;
		TextInfo->currenty = 0.0F;
	}

}

void ProcessTextItems (void)
{
	int i, teletypenum;
	static float VidTextSfxPause = 0.0F;
	static float theta = 0.0F;
	u_int8_t r, g, b;
	float multiplier;

	teletypenum = 0;

	for (i = 0; i < MAXHIGHLIGHTITEMS; i++)
	{	HighlightItem[i] = NULL;
		HighlightStatus[i] = HIGHLIGHT_Pulsing;
	}
	CurrentHighlightItem = 0;

	TeleTypeTimer += framelag/(0.05F * 60.0F);
	TeleTypeSkip = (int)floor((float)TeleTypeTimer);

	TeleTypeTimer -= (float)TeleTypeSkip;

	NewTextCharDisplayed = false;
	for (i=0; i<TextStackLevel; i++)
	{	
		if ( ! TextStack[ i ] )
			continue;

		if ((TextStack[i]->flags & TEXTFLAG_Pulse) && (Pulse >= 0.5F))

			PrintTextItem(TextStack[i]);

		if (
			(TextStack[i]->flags & TEXTFLAG_CheckForRefresh) &&
			(strncmp(TextStack[i]->oldtext, TextStack[i]->text, MAX_TEXT_LENGTH) != 0)
		)
		{
			strncpy(TextStack[i]->oldtext, TextStack[i]->text, MAX_TEXT_LENGTH);
			PrintTextItem(TextStack[i]);
		}

		if (TextStack[i]->highlight)
		{ 
			HighlightItem[ CurrentHighlightItem ] = TextStack[i];
			HighlightStatus[ CurrentHighlightItem++ ] = TextStack[i]->highlighttype;
		}

		if (DrawTextItemBox && !TextStack[i]->boxdone)
		{
			TextStack[i]->boxdone = true;
	  		if (!Plot2dBox (TextStack[i]))
	  			Msg( "Plot2dBox() : failed\n" );
		}
		
		DisplayTextItem (TextStack[i]);

		// do not do sfx for instant text
		if ( TextStack[i]->flags & TEXTFLAG_Instant )
			NewTextCharDisplayed = false;

	}

	VidTextSfxPause += framelag;

	if ( NewTextCharDisplayed && (VidTextSfxPause > 5.0F) )
	{
		PlaySfx( SFX_VidText, 0.4F );
		VidTextSfxPause = 0.0F;
	}

	if (
		VDU_Ready &&
		((CameraStatus == CAMERA_AtRightVDU) || (CameraStatus == CAMERA_AtLeftVDU)) &&
		((LastMenuItem != CurrentMenuItem))
	)
	{
		LastMenuItem = CurrentMenuItem;
	}
	
	if (
		 CurrentMenuItem	&&
		 VDU_Ready			&&
		 TextStackLevel		&&
		 CurrentMenuItem->TextInfo[0] &&
		 !(CurrentMenuItem->TextInfo[0]->flags & TEXTFLAG_SuppressHighlight) &&
		 !(CurrentMenuItem->TextInfo[0]->highlight)	// ensures item is not highlighted twice
	)
	{
		HighlightItem[ CurrentHighlightItem ] = CurrentMenuItem->TextInfo[0];
		HighlightStatus[ CurrentHighlightItem++ ] = CurrentMenuItem->TextInfo[0]->highlighttype;
	}

	CursorTimer += framelag/(CursorInterval * 60.0F) * 2.0F;

	if (CursorTimer >= 2.0F)
		CursorTimer -= 2.0F;


	ProcessFlatSliders();

/******************
Highlight box
*******************/

	theta += 360.0F * framelag/(60.0F);
	if (theta > 360.0F)
		theta -= 360.0F;
	multiplier = (float)(SIND(theta) + 1.0F)/2.0F;
	r = (u_int8_t)(64.0F * multiplier);
	b = r;
	g = (u_int8_t)(255.0F * multiplier);
	
	for (i = 0; i < MAXHIGHLIGHTITEMS; i++)
	{
		if (HighlightItem[i] && VDU_Ready)
		{	
			switch (HighlightStatus[i])
			{
			case HIGHLIGHT_Static:
//					if (CurrentMenu == &MENU_NEW_JoinWaitingToStart)
				DrawHighlightBox(HighlightItem[i], 64, 255, 64);
				break;
			default:
				DrawHighlightBox(HighlightItem[i], r, g, b);
				break;
			}
		}
	}
}


void InitScreenFonts (void)
{

	int i, font, offset, space;

	for (font=0; font<NUMSCREENFONTS; font++)
	{

		switch (font)
		{
		case FONT_512X384_Large:
			offset = 0;
			space = 224;
			break;
		case FONT_512X384_Medium:
			offset = 74;
			space = 223;
			break;
		//case FONT_512X384_Small:
		default:
			offset = 148;
			space = 222;
			break;
		}

		TextLookup[font][0] = 0 + offset;	//ASCII 0 used for cursor
		
		for (i=1; i<= 15; i++)
			TextLookup[font][i] = 0;
		
		TextLookup[font][16] = 71 + offset;	//	right arrow
		TextLookup[font][17] = 70 + offset;	// left arrow

		for (i=18; i<=29; i++)
			TextLookup[font][i] = 0;

		TextLookup[font][30] = 72 + offset;	// up arrow
		TextLookup[font][31] = 73 + offset;	// down arrow

		TextLookup[font][32] = space; // space

		for (i=33; i<=37; i++)
			TextLookup[font][i] = i - 32 + offset;	// ! " # $ %

		for (i=38; i<=39; i++)
			TextLookup[font][i] = 0;

		for (i=40; i<=43; i++)
			TextLookup[font][i] = i - 34 + offset;	// ( ) * +

		for (i=45; i<=90; i++)
			TextLookup[font][i] = i - 35 + offset; // - . / 0-9 : ; < = > ? @ A-Z

		for (i=91; i<=96; i++)
			TextLookup[font][i] = 0;

		for (i=97; i<=122; i++)
			TextLookup[font][i] = i - 67 + offset; // a-z

		for (i=123; i<=255; i++)
			TextLookup[font][i] = 0;

		TextLookup[font]['='] = 27 + offset;
		TextLookup[font]['['] = 6 + offset;
		TextLookup[font][']'] = 7 + offset;

		// foreign characters...
		TextLookup[font][(u_int8_t)'à'] = 56 + offset; 
		TextLookup[font][(u_int8_t)'à'] = 56 + offset; 
		TextLookup[font][(u_int8_t)'À'] = 56 + offset; 
		TextLookup[font][(u_int8_t)'á'] = 57 + offset; 
		TextLookup[font][(u_int8_t)'Á'] = 57 + offset; 
		TextLookup[font][(u_int8_t)'ä'] = 58 + offset; 
		TextLookup[font][(u_int8_t)'Ä'] = 58 + offset; 
		TextLookup[font][(u_int8_t)'è'] = 59 + offset; 
		TextLookup[font][(u_int8_t)'È'] = 59 + offset; 
		TextLookup[font][(u_int8_t)'é'] = 60 + offset; 
		TextLookup[font][(u_int8_t)'É'] = 60 + offset; 
		TextLookup[font][(u_int8_t)'ê'] = 61 + offset; 
		TextLookup[font][(u_int8_t)'ì'] = 62 + offset; 
		TextLookup[font][(u_int8_t)'ò'] = 63 + offset; 
		TextLookup[font][(u_int8_t)'Ò'] = 63 + offset; 
		TextLookup[font][(u_int8_t)'ó'] = 64 + offset; 
		TextLookup[font][(u_int8_t)'Ó'] = 64 + offset; 
		TextLookup[font][(u_int8_t)'ö'] = 65 + offset; 
		TextLookup[font][(u_int8_t)'Ö'] = 65 + offset; 
		TextLookup[font][(u_int8_t)'ù'] = 66 + offset; 
		TextLookup[font][(u_int8_t)'Ù'] = 66 + offset; 
		TextLookup[font][(u_int8_t)'ú'] = 67 + offset; 
		TextLookup[font][(u_int8_t)'Ú'] = 67 + offset; 
		TextLookup[font][(u_int8_t)'ü'] = 68 + offset; 
		TextLookup[font][(u_int8_t)'Ü'] = 68 + offset; 
		TextLookup[font][(u_int8_t)'ñ'] = 69 + offset; 
		TextLookup[font][(u_int8_t)'Ñ'] = 69 + offset;
		TextLookup[font][(u_int8_t)'ß'] = TextLookup[font][(u_int8_t)'B'];
		
/*
  128  Ç ( 199 )  144  É ( 201 )  160  á ( 225 )   
  129  ü ( 252 )  145  æ ( 230 )  161  í ( 237 )  
  130  é ( 233 )  146  Æ ( 198 )  162  ó ( 243 )  
  131  â ( 226 )  147  ô ( 244 )  163  ú ( 250 )  
  132  ä ( 228 )  148  ö ( 246 )  164  ñ ( 241 )  
  133  à ( 224 )  149  ò ( 242 )  165  Ñ ( 209 )  
  134  å ( 229 )  150  û ( 251 )  166  ª ( 170 )  
  135  ç ( 231 )  151  ù ( 249 )  167  º ( 186 )
  136  ê ( 234 )  152  ÿ ( 255 )  168  ¿ ( 191 )
  137  ë ( 235 )  153  Ö ( 214 )  169  _ ( 095 )
  138  è ( 232 )  154  Ü ( 220 )  170  ¬ ( 172 )  
  139  ï ( 239 )  155  ¢ ( 162 )  171  ½ ( 189 )  
  140  î ( 238 )  156  £ ( 163 )  172  ¼ ( 188 )  
  141  ì ( 236 )  157  ¥ ( 165 )  173  ¡ ( 161 )  
  142  Ä ( 196 )  158  P ( 080 )  174  « ( 171 )  
  143  Å ( 197 )  159  ƒ ( 131 )  175  » ( 187 )  
*/
		
		TextLookup[font][1] = 225;	// glowing effect for bike scan

		TextLookup[font]['_'] = space; // underscore prints as space

		switch (font)
		{
		case FONT_512X384_Small:
			TextLookup[font][39] = 228; // apostrophe
			TextLookup[font][96] = 228; // apostrophe
			TextLookup[font][','] = 231; // comma

			TextLookup[font][(u_int8_t)'¡'] = 234;
			TextLookup[font][(u_int8_t)'¿'] = 237;
			TextLookup[font][(u_int8_t)'Ç'] = 240;
			TextLookup[font][(u_int8_t)'ç'] = 240;

			TextLookup[font][(u_int8_t)'â'] = 243;
			TextLookup[font][(u_int8_t)'å'] = 246;
			TextLookup[font][(u_int8_t)'ë'] = 249;
			TextLookup[font][(u_int8_t)'ï'] = 252;
			TextLookup[font][(u_int8_t)'î'] = 255;
			TextLookup[font][(u_int8_t)'í'] = 258;
			TextLookup[font][(u_int8_t)'Í'] = 258;
			TextLookup[font][(u_int8_t)'ô'] = 261;
			TextLookup[font][(u_int8_t)'û'] = 264;
			break;
		case FONT_512X384_Medium:
			TextLookup[font][39] = 227; // apostrophe
			TextLookup[font][96] = 227; // apostrophe
			TextLookup[font][','] = 230; // comma

			TextLookup[font][(u_int8_t)'¡'] = 233;
			TextLookup[font][(u_int8_t)'¿'] = 236;
			TextLookup[font][(u_int8_t)'Ç'] = 239;
			TextLookup[font][(u_int8_t)'ç'] = 239;

			TextLookup[font][(u_int8_t)'â'] = 242;
			TextLookup[font][(u_int8_t)'å'] = 245;
			TextLookup[font][(u_int8_t)'ë'] = 248;
			TextLookup[font][(u_int8_t)'ï'] = 251;
			TextLookup[font][(u_int8_t)'î'] = 254;
			TextLookup[font][(u_int8_t)'í'] = 257;
			TextLookup[font][(u_int8_t)'Í'] = 257;
			TextLookup[font][(u_int8_t)'ô'] = 260;
			TextLookup[font][(u_int8_t)'û'] = 263;
			break;
		case FONT_512X384_Large:
			TextLookup[font][39] = 226; // apostrophe
			TextLookup[font][96] = 226; // apostrophe
			TextLookup[font][','] = 229; // comma

			TextLookup[font][(u_int8_t)'¡'] = 232;
			TextLookup[font][(u_int8_t)'¿'] = 235;
			TextLookup[font][(u_int8_t)'Ç'] = 238;
			TextLookup[font][(u_int8_t)'ç'] = 238;

			TextLookup[font][(u_int8_t)'â'] = 241;
			TextLookup[font][(u_int8_t)'å'] = 244;
			TextLookup[font][(u_int8_t)'ë'] = 247;
			TextLookup[font][(u_int8_t)'ï'] = 250;
			TextLookup[font][(u_int8_t)'î'] = 253;
			TextLookup[font][(u_int8_t)'í'] = 256;
			TextLookup[font][(u_int8_t)'Í'] = 256;
			TextLookup[font][(u_int8_t)'ô'] = 259;
			TextLookup[font][(u_int8_t)'û'] = 262;
			break;
		}
	}
}

/*********************************************************************************************************
end of VDU text routines
*********************************************************************************************************/



void SinPan2(VECTOR ViewFrom, VECTOR ViewTo, VECTOR *CurrentView,
			 VECTOR LookFrom, VECTOR LookTo, VECTOR *CurrentLook, float Time)
{
	float SinTheta, OneMinus, theta;

	theta = Time * 90.0F;

	SinTheta = (float)SIND(theta);
	OneMinus = 1 - SinTheta;	

	CurrentView->x = OneMinus * ViewFrom.x + SinTheta * ViewTo.x;
	CurrentView->y = OneMinus * ViewFrom.y + SinTheta * ViewTo.y;
	CurrentView->z = OneMinus * ViewFrom.z + SinTheta * ViewTo.z;

	CurrentLook->x = OneMinus * LookFrom.x + SinTheta * LookTo.x;
	CurrentLook->y = OneMinus * LookFrom.y + SinTheta * LookTo.y;
	CurrentLook->z = OneMinus * LookFrom.z + SinTheta * LookTo.z;
  
}

void RotateVDU2(float time, float start, float end)
{
	float SinTheta, OneMinus, theta;
	MATRIX Mat_R;

	theta = time * 90.0F;

	SinTheta = (float)SIND(theta);
	OneMinus = 1 - SinTheta;	

	VDUrot = OneMinus * start + SinTheta * end;
	
	// build rotate matrix...
	BuildRotMatrix(0.0F, VDUrot, 0.0F, &Mat_R);

	// rotate vdu...
	Models[BackgroundModel[TITLE_MODEL_VDU]].Mat = Mat_R;
	MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_VDU]].InvMat);
}

void RotateBracket(float time, float start, float end)
{
	float SinTheta, OneMinus, theta;
	MATRIX Mat_R;

	theta = time * 90.0F;

	SinTheta = (float)SIND(theta);
	OneMinus = 1 - SinTheta;	

	BracketRot = OneMinus * start + SinTheta * end;
	
	// build rotate matrix...
	BuildRotMatrix(0.0F, BracketRot, 0.0F, &Mat_R);

	// rotate vdu...
	Models[BackgroundModel[TITLE_MODEL_Bracket]].Mat = Mat_R;
	MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_Bracket]].InvMat);
}


void DarkenRoomForStart( MENU *Menu )
{
	RoomDarkness = 0.0F;
	DarkenRoom2(RoomDarkness);
}

void DarkenRoom2(float darkness)
{
	int model;
	float RoomDim;

	darkness = 1.0F - darkness;
	
	RoomDim = ((1.0F - darkness) * 0.6F) + 0.4F;	//1 - 0.4
	
	for (model = 0; model < NumTitleModelsToLoad; model++)
	{	
		switch (model)
		{
		case TITLE_MODEL_HoloLight:
			break;	//do not darken HoloLight
		case TITLE_MODEL_MenuTV:
			TintModelVertices( BackgroundModel[model], RoomDim, &MenuTVExcluded);
			break;
		case TITLE_MODEL_MenuTVDummy:
			TintModelVertices( BackgroundModel[model], RoomDim, &MenuTVDummyExcluded);
			break;
		case TITLE_MODEL_VDU:
			TintModelVertices( BackgroundModel[model], RoomDim, &VduScreenExcluded);
			break;
		case TITLE_MODEL_Room:
			TintModelVertices( BackgroundModel[model], RoomDim, &RoomExcluded);
			break;
		default:
			TintModelVertices( BackgroundModel[model], RoomDim, NULL );
			break;
		}
		
	}
}

void RotateDiscStack(float theta)
{
	MATRIX Mat_R;
	VECTOR rotpoint, stackoffset;
	int i;
	int modellookup [8] = {10, 0, 1, 2, 3, 4, 5, 6};

	rotpoint.x = stackpos[0].x;
	rotpoint.y = stackpos[0].y + 70.0F;
	rotpoint.z = stackpos[0].z - 60.0F;

	for (i=0; i<8; i++)
	{
		
		stackoffset.x = stackpos[i].x - rotpoint.x;
		stackoffset.y = stackpos[i].y - rotpoint.y;
		stackoffset.z = stackpos[i].z - rotpoint.z;

		BuildRotMatrix(theta, 0.0F, 0.0F, &Mat_R);

		ApplyMatrix(&Mat_R, &stackoffset, &stackoffset);

		Models[BackgroundModel[modellookup[i]]].Pos.x = rotpoint.x + stackoffset.x;
		Models[BackgroundModel[modellookup[i]]].Pos.y = rotpoint.y + stackoffset.y;
		Models[BackgroundModel[modellookup[i]]].Pos.z = rotpoint.z + stackoffset.z;
		
		Models[BackgroundModel[TITLE_MODEL_Mtop]].Mat = Mat_R;
		MatrixTranspose(&Mat_R, &Models[BackgroundModel[modellookup[i]]].InvMat);

	}

}

float GetEventCurrentTime(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	if (Title_Timers[TitleEvent->TriggeredBy].CurrentTime >= TitleEvent->EndTime)
		currenttime = 1.0F;
	else
		currenttime = (Title_Timers[TitleEvent->TriggeredBy].CurrentTime - TitleEvent->StartTime) / (TitleEvent->EndTime - TitleEvent->StartTime);

	return currenttime;

}

void LoadBikeChar(MENUITEM *Item)
{
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	int Biker;
	float xmin, xmax, ymin, ymax, newxmax, newymax, xgap, ygap;
	float BikeCharScale = 0.7F;
	FRAME_INFO **header;
	
	xmin = (Item->x + VDUoffsetX) * ModeScaleX;
	xmax = (Item->xmax + VDUoffsetX) * ModeScaleX;
	ymin = (Item->y + VDUoffsetY) * ModeScaleY;
	ymax = (Item->ymax + VDUoffsetY) * ModeScaleY;
	
	Biker = SelectedBikeFrame;

	if( Biker < 0 )
	{
		KillBikeCharPic( NULL );
		return;
	}

	if ((Biker <= 7) && (Biker >= 0))
	{	
		header = &Title_Chars1_Header;
	}
	else
	{	
		if ((Biker >= 8) && (Biker <= 15))
		{
			header = &Title_Chars2_Header;
			Biker -= 8;
		}
		else
		{	
			Msg("Title.c: LoadBikeChar() invalid char no.");
			exit(1);
		}
	}
	
	if (!LoadGeneralPic(0, 0, 0, 0, header, &Biker, &BikerScrPoly, &BikerDisplayed))
	{
		Msg("Title.c LoadBikeChar() unable to allocate screen poly\n");
		exit(1);
	}

	Bit_Ptr = ( (*ScrPolys[ BikerScrPoly ].Frm_Info)->Bit_Info + (int16_t) ScrPolys[ BikerScrPoly ].Frame );
	Off_Ptr = ( (*ScrPolys[ BikerScrPoly ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( (*ScrPolys[ BikerScrPoly ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	newxmax = Box_Ptr->xsize * ModeScaleX * BikeCharScale + xmin;
	newymax = Box_Ptr->ysize * ModeScaleY * BikeCharScale + ymin;

	xgap = (xmax - newxmax) / 2.0F;
	ygap = (ymax - newymax) / 2.0F;

	xmin = xmin + xgap;
	ymin = ymin + ygap;	
	xmax = newxmax + xgap;					
	ymax = newymax + xgap;	

	ScrPolys[BikerScrPoly].x1 = xmin;
	ScrPolys[BikerScrPoly].y1 = ymin;	
	ScrPolys[BikerScrPoly].x3 = xmax;					
	ScrPolys[BikerScrPoly].y3 = ymax;					
}

void LoadBikeCharPic(MENUITEM *Item)
{
	int Biker;
	float xmin, xmax, ymin, ymax;
	FRAME_INFO **header;

	xmin = (Item->x + VDUoffsetX) * ModeScaleX;
	xmax = (Item->xmax + VDUoffsetX) * ModeScaleX;
	ymin = (Item->y + VDUoffsetY) * ModeScaleY;
	ymax = (Item->ymax + VDUoffsetY) * ModeScaleY;

	Biker = SelectedBikeFrame;

	if ((Biker <= 7) && (Biker >= 0))
		header = &Title_Chars1_Header;
	else
	{	if ((Biker >= 8) && (Biker <= 15))
		{
			Biker -= 8;
			header = &Title_Chars2_Header;
		}
		else
			return;
	}
}

void KillBikeCharPic( MENU *Menu )
{
	if (BikerDisplayed)
	{	KillUsedScrPoly ( BikerScrPoly );
		BikerScrPoly = (u_int16_t)-1;
		BikerDisplayed = false;
	}
}

void LoadLevelPic(MENUITEM *Item)
{
	float xmin, xmax, ymin, ymax;
	FRAME_INFO **header;
	int frame = 0;

	strcpy( Tloadheader.PlaceHolderFile[ Title_LevelPics_Header->vid_tpage_index ], MissionTextPics[ NewLevelNum ] );

	if ( !TloadReloadPlaceHolder( &Tloadheader, Title_LevelPics_Header->vid_tpage_index ) )
		return;

	xmin = (Item->x + VDUoffsetX) * ModeScaleX;
	xmax = (Item->xmax + VDUoffsetX) * ModeScaleX;
	ymin = (Item->y + VDUoffsetY) * ModeScaleY;
	ymax = (Item->ymax + VDUoffsetY) * ModeScaleY;

	header = &Title_LevelPics_Header;
	if (!LoadGeneralPic(xmin, ymin, xmax, ymax, header, &frame, &LevelScrPoly, &LevelDisplayed))
	{
		Msg("Title.c LoadLevelPic() unable to allocate screen poly\n");
		exit(1);
	}
}

u_int16_t SavedGamePicPoly;
u_int16_t TVFramePoly;
bool SavedGamePicDisplayed = false;
bool TVFrameDisplayed = false;

void LoadSavedGamePic( char *file )
{
	if ( DummyTextureIndex == -1 )
		return;
	
	strcpy( Tloadheader.PlaceHolderFile[ DummyTextureIndex ], file );

	if ( !TloadReloadPlaceHolder( &Tloadheader, DummyTextureIndex ) )
		return;

	// display dummy tv
	Models[ BackgroundModel[ TITLE_MODEL_MenuTVDummy ] ].Visible = 1;
	Models[ BackgroundModel[ TITLE_MODEL_MenuTV ] ].Visible = 0;
}


void KillLevelPic( MENU *Menu )
{
	if (LevelDisplayed)
	{	KillUsedScrPoly ( LevelScrPoly );
		LevelScrPoly = (u_int16_t)-1;
		LevelDisplayed = false;
	}
}

void GetTVCoords( POLY *poly, float zoffset )
{
	int g, e, vert;
	LPLVERTEX VertPtr;

	for ( g = 0; g < ModelHeaders[ TITLE_MODEL_MenuTV ].num_groups; g++ )
	{
		for ( e = 0; e < ModelHeaders[ TITLE_MODEL_MenuTV ].Group[ g ].num_execbufs; e++ )
		{
			if ( ModelHeaders[ TITLE_MODEL_MenuTV ].Group[ g ].polyanim[ e ]->vertices )
			{
				for ( vert = 0; vert < ModelHeaders[ TITLE_MODEL_MenuTV ].Group[ g ].polyanim[ e ]->vertices; vert++ )
				{
					VertPtr = &ModelHeaders[ TITLE_MODEL_MenuTV ].Group[ g ].originalVerts[ e ][ ModelHeaders[ TITLE_MODEL_MenuTV ].Group[ g ].polyanim[ e ]->vert[ vert ] ];
					
					switch( vert )
					{
						case 0:
							poly->Pos1.x = VertPtr->x;
							poly->Pos1.y = VertPtr->y;
							poly->Pos1.z = VertPtr->z + zoffset;
							break;
						case 1:
							poly->Pos2.x = VertPtr->x;
							poly->Pos2.y = VertPtr->y;
							poly->Pos2.z = VertPtr->z + zoffset;
							break;
						case 2:
							poly->Pos3.x = VertPtr->x;
							poly->Pos3.y = VertPtr->y;
							poly->Pos3.z = VertPtr->z + zoffset;
							break;
						case 3:
							poly->Pos4.x = VertPtr->x;
							poly->Pos4.y = VertPtr->y;
							poly->Pos4.z = VertPtr->z + zoffset;
							break;
					}
				}
			}
		}
	}
}

bool DisplayPicOnTV( FRAME_INFO **header, int *frame, u_int16_t *poly, bool *displayed, float zoffset )
{
	if (*displayed)
	{	KillUsedPoly ( *poly );
		*poly = (u_int16_t)-1;
		*displayed = false;
	}

	if (!*displayed)
	{
		*poly = FindFreePoly();					
		if( *poly != (u_int16_t ) -1 )
		{	
			*displayed = true;

			GetTVCoords( &Polys[ *poly ], zoffset );

			Polys[ *poly ].Col1.R = 255;
			Polys[ *poly ].Col1.G = 255;
			Polys[ *poly ].Col1.B = 255;
			Polys[ *poly ].Col2.R = 255;
			Polys[ *poly ].Col2.G = 255;
			Polys[ *poly ].Col2.B = 255;
	   		Polys[ *poly ].Col3.R = 255;
			Polys[ *poly ].Col3.G = 255;
   			Polys[ *poly ].Col3.B = 255;
   			Polys[ *poly ].Col4.R = 255;
			Polys[ *poly ].Col4.G = 255;
   			Polys[ *poly ].Col4.B = 255;
   			Polys[ *poly ].Trans1 = 255;
	   		Polys[ *poly ].Trans2 = 255;
	   		Polys[ *poly ].Trans3 = 255;
   			Polys[ *poly ].Trans4 = 255;
	   		Polys[ *poly ].Scale = 0.0F;
	   		Polys[ *poly ].Flags = POLY_FLAG_SOLID;
   		    Polys[ *poly ].Frm_Info = header;
			Polys[ BikeLine[CurrentLine] ].Frame = (float)(*frame);

			AddPolyToTPage( BikeLine[CurrentLine], GetTPage( *Polys[ *poly ].Frm_Info, (int16_t) Polys[ *poly ].Frame ) );

		}
		else
			return false;


	}

	return true;

}

bool LoadGeneralPic(float xmin, float ymin, float xmax, float ymax, FRAME_INFO **header, int *frame, u_int16_t *scrpoly, bool *displayed)
{
	if (*displayed)
	{	KillUsedScrPoly ( *scrpoly );
		*scrpoly = (u_int16_t)-1;
		*displayed = false;
	}

	if (!*displayed)
	{
		*scrpoly = FindFreeScrPoly();					
		if( *scrpoly != (u_int16_t ) -1 )
		{	
			*displayed = true;

			ScrPolys[*scrpoly].Type = SCRTYPE_Normal;
			ScrPolys[*scrpoly].Flags = SCRFLAG_UseCoords;
			ScrPolys[*scrpoly].R = 255;
			ScrPolys[*scrpoly].G = 255;
			ScrPolys[*scrpoly].B = 255;
			ScrPolys[*scrpoly].Trans = 255;
			ScrPolys[ *scrpoly ].Frm_Info = header;

			AddScrPolyToTPage( *scrpoly, GetTPage( *ScrPolys[ *scrpoly ].Frm_Info, 0 ) );

			ScrPolys[*scrpoly].Xscale = 1.0F;				
			ScrPolys[*scrpoly].Yscale = 1.0F;
		}
		else
			return false;
	}

	ScrPolys[*scrpoly].x1 = xmin;
	ScrPolys[*scrpoly].y1 = ymin;	
	ScrPolys[*scrpoly].x3 = xmax;					
	ScrPolys[*scrpoly].y3 = ymax;					
	ScrPolys[ *scrpoly ].Frame = (float)*frame;

	return true;
}

void KillBikeChar(MENU *Menu)
{
	if (BikerDisplayed)
	{	KillUsedScrPoly( BikerScrPoly );
		BikerScrPoly = (u_int16_t)-1;
		BikerDisplayed = false;
	}

	InitBikeList( Menu );
}

void InitEventGeneralPan(TITLE_EVENT *TitleEvent)
{
	OldLookPos = Look;
	OldViewPos = View;

	ResetAllEvents();

	PlaySfx( SFX_TitlePan, 0.7F );
}

void InitEventDiscPan(TITLE_EVENT *TitleEvent)
{
	//InitEventGeneralPan(TitleEvent);
	//VduClear();
	//VDU_Ready = false;
}



void EventDiscPan(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	SinPan2(OldLookPos, DiscLookPos ,&Look, OldViewPos, DiscViewPos, &View, currenttime);
}

void ExitEventDiscPanFromRight(TITLE_EVENT *TitleEvent)
{
	OldSelectedBike = -1;
}

void InitEventRightVDUPan(TITLE_EVENT *TitleEvent)
{
	MATRIX Mat_R;

	//InitEventGeneralPan(TitleEvent);
	
	if (!CompareVectors(Models[BackgroundModel[TITLE_MODEL_VDU]].Pos, RightVDU.Pos))
	{
		Models[BackgroundModel[TITLE_MODEL_VDU]].Pos = RightVDU.Pos;
		Models[BackgroundModel[TITLE_MODEL_Bracket]].Pos = RightVDU.BracketPos;
		VDUrot = RightVDU.StartRot;
		BracketRot = RightVDU.BracketStartRot;
	
		BuildRotMatrix(0.0F, BracketRot, 0.0F, &Mat_R);

		Models[BackgroundModel[TITLE_MODEL_Bracket]].Mat = Mat_R;
		MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_Bracket]].InvMat);

		// build rotate matrix...
		BuildRotMatrix(0.0F, VDUrot, 0.0F, &Mat_R);

		// rotate vdu...
		Models[BackgroundModel[TITLE_MODEL_VDU]].Mat = Mat_R;
		MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_VDU]].InvMat);

	}
}

void EventRightVDUPan(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	SinPan2(OldLookPos, RightVDU.Look ,&Look, OldViewPos, RightVDU.View, &View, currenttime);
}

void ExitEventVDUPan(TITLE_EVENT *TitleEvent)
{
	ProcessVduItems( CurrentMenu );
	VDU_Ready = true;
}

void InitEventLeftVDUPan(TITLE_EVENT *TitleEvent)
{
	MATRIX Mat_R;

	//InitEventGeneralPan(TitleEvent);

	if (!CompareVectors(Models[BackgroundModel[TITLE_MODEL_VDU]].Pos, LeftVDU.Pos))
	{
		Models[BackgroundModel[TITLE_MODEL_VDU]].Pos = LeftVDU.Pos;
		Models[BackgroundModel[TITLE_MODEL_Bracket]].Pos = LeftVDU.BracketPos;
		VDUrot = LeftVDU.StartRot;
	
		BracketRot = RightVDU.BracketStartRot;
	
		BuildRotMatrix(0.0F, BracketRot, 0.0F, &Mat_R);

		Models[BackgroundModel[TITLE_MODEL_Bracket]].Mat = Mat_R;
		MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_Bracket]].InvMat);

		// build rotate matrix...
		BuildRotMatrix(0.0F, VDUrot, 0.0F, &Mat_R);

		// rotate vdu...
		Models[BackgroundModel[TITLE_MODEL_VDU]].Mat = Mat_R;
		MatrixTranspose(&Mat_R, &Models[BackgroundModel[TITLE_MODEL_VDU]].InvMat);
	}
}

void EventLeftVDUPan(TITLE_EVENT *TitleEvent)
{
 	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	SinPan2(OldLookPos, LeftVDU.Look ,&Look, OldViewPos, LeftVDU.View, &View, currenttime);
}


void InitEventRotateOutRightVDU(TITLE_EVENT *TitleEvent)
{
	OldVDUrot = VDUrot;
	OldBracketRot = BracketRot;
}

void EventRotateOutRightVDU(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	RotateVDU2(currenttime, OldVDUrot, RightVDU.EndRot);
	RotateBracket(currenttime, OldBracketRot, RightVDU.BracketEndRot);

}

void ExitEventRotateOutVDU(TITLE_EVENT *TitleEvent)
{
	PlaySfx( SFX_VduOn, 0.8F );
}

void InitEventRotateInRightVDU(TITLE_EVENT *TitleEvent)
{
	OldVDUrot = VDUrot;
	OldBracketRot = BracketRot;

	PlaySfx( SFX_VduOff, 0.8F );

}

void EventRotateInRightVDU(TITLE_EVENT *TitleEvent)
{
	float currenttime;
	
	currenttime = GetEventCurrentTime(TitleEvent);

	RotateVDU2(currenttime, OldVDUrot, RightVDU.StartRot);
	RotateBracket(currenttime, OldBracketRot, RightVDU.BracketEndRot);
}

void ExitEventRotateInRightVDU(TITLE_EVENT *TitleEvent)
{
	int i;

	i = 0;
}

void InitEventRotateOutLeftVDU(TITLE_EVENT *TitleEvent)
{
	OldVDUrot = VDUrot;
	OldBracketRot = BracketRot;
}

void EventRotateOutLeftVDU(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);
	
	RotateVDU2(currenttime, OldVDUrot, LeftVDU.EndRot);
	RotateBracket(currenttime, OldBracketRot, LeftVDU.BracketEndRot);
}

void InitEventRotateInLeftVDU(TITLE_EVENT *TitleEvent)
{
	OldVDUrot = VDUrot;
	OldBracketRot = BracketRot;

	PlaySfx( SFX_VduOff, 0.8F );
}

void EventRotateInLeftVDU(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	RotateVDU2(currenttime, OldVDUrot, LeftVDU.StartRot);
	RotateBracket(currenttime, OldBracketRot, LeftVDU.BracketStartRot);
}


void InitEventDarkenRoom(TITLE_EVENT *TitleEvent)
{
	OldRoomDarkness = RoomDarkness;
}
void EventDarkenRoom(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);
	
	currenttime = 1.0F - currenttime;
	
	RoomDarkness = OldRoomDarkness * currenttime;
	
	DarkenRoom2(RoomDarkness);

}

void InitEventLightenRoom(TITLE_EVENT *TitleEvent)
{
	OldRoomDarkness = RoomDarkness;
}
void EventLightenRoom(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);
	
	RoomDarkness = ((1.0F - OldRoomDarkness) * currenttime) + OldRoomDarkness;
	
	DarkenRoom2(RoomDarkness);
}

void InitEventFadeOnHoloLight(TITLE_EVENT *TitleEvent)
{
	OldHoloLightBrightness = HoloLightBrightness;
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Visible = 1;
}

void TriggerBikeSpeech( TITLE_EVENT *TitleEvent )
{
	static u_int32_t CurrentSpeechID = 0;

	if( MyGameStatus != STATUS_Title )
		return;

	if ( CurrentSpeechID )
		StopSfx( CurrentSpeechID );

	CurrentSpeechID = PlaySfx( SFX_BIKER_EX, 1.0F );
}

void FadeHoloLight(float Brightness )
{
	TintModelVertices( BackgroundModel[TITLE_MODEL_HoloLight], Brightness, NULL );
}

void EventFadeOnHoloLight(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	HoloLightBrightness = ((1.0F - OldHoloLightBrightness) * currenttime) + OldHoloLightBrightness;

	FadeHoloLight(HoloLightBrightness);
}

void ExitEventFadeOffHoloLight(TITLE_EVENT *TitleEvent)
{
}

void InitEventFadeOffHoloLight(TITLE_EVENT *TitleEvent)
{
	OldHoloLightBrightness = HoloLightBrightness;
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Visible = 0;
}

void EventFadeOffHoloLight(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	currenttime = 1.0F - currenttime;
	
	HoloLightBrightness = OldHoloLightBrightness * currenttime;

	FadeHoloLight(HoloLightBrightness);
}

void InitEventRaiseHoloPad(TITLE_EVENT *TitleEvent)
{
	OldHoloPadHeight = Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y;
}

void EventRaiseHoloPad(TITLE_EVENT *TitleEvent)
{
	float currenttime, raiseby;

	currenttime = GetEventCurrentTime(TitleEvent);

	raiseby = ((Holopad.y - OldHoloPadHeight) * currenttime) + OldHoloPadHeight;
	
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = raiseby;


}

void ExitEventRaiseHolopad(TITLE_EVENT *TitleEvent)
{
}

void InitEventLowerHoloPad(TITLE_EVENT *TitleEvent)
{
	OldHoloPadHeight = Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y;
	HolopadJudderReq = true;
}

void EventLowerHoloPad(TITLE_EVENT *TitleEvent)
{
	float currenttime, raiseby;

	currenttime = GetEventCurrentTime(TitleEvent);

	currenttime = 1.0F - currenttime;
	
	raiseby = (currenttime * (OldHoloPadHeight - (Holopad.y - HolopadRange) ) ) + (Holopad.y - HolopadRange);
	
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = raiseby;
}

void InitEventJudderHoloPad(TITLE_EVENT *TitleEvent)
{
}

void EventJudderHoloPad(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	
	if (HolopadJudderReq)
	{
		currenttime = GetEventCurrentTime(TitleEvent);

		if (currenttime <= 0.2F)
			Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y - 0.8F;

		if ((currenttime > 0.2F) && (currenttime <= 0.4F))
			Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y + 0.6F;

		if ((currenttime > 0.4F) && (currenttime <= 0.6F))
			Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y - 0.4F;

		if ((currenttime > 0.6F) && (currenttime <= 0.8F))
			Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y + 0.2F;

		if (currenttime > 0.8F)
			Models[BackgroundModel[TITLE_MODEL_Holopad]].Pos.y = Holopad.y;
	}

}

void ExitEventJudderHoloPad(TITLE_EVENT *TitleEvent)
{
	HolopadJudderReq = false;
}


void InitEventDiscPanFromRightInit(TITLE_EVENT *TitleEvent)
{
	//InitEventGeneralPan(TitleEvent);
}

void EventDiscPanInit(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	SinPan2(OldLookPos, DiscLookPos ,&Look, OldViewPos, DiscViewPos, &View, currenttime);
}

void InitEventLowerStack( TITLE_EVENT *TitleEvent )
{
	PlaySfx( SFX_StackDown, 1.0F );
}

void EventLowerStack(TITLE_EVENT *TitleEvent)
{
	float currenttime;

	currenttime = GetEventCurrentTime(TitleEvent);

	currenttime = 1.0F - currenttime;

	currenttime = (float)SIND(currenttime * 90.0F);

	//RotateDiscStack((currenttime * (MaxStackTheta - MinStackTheta)) + MinStackTheta);
	RotateDiscStack (currenttime * MaxStackTheta);

}

void ExitEventLowerStack(TITLE_EVENT *TitleEvent)
{
}
 
void InitEventJudderStack(TITLE_EVENT *TitleEvent)
{
	StackJudderNum = 0;
	StackRotAmount = 0.0F;
	OldStackRotAmount = 0.0F;
}

void EventJudderStack(TITLE_EVENT *TitleEvent)
{
	int i;
	float currenttime, ReqRotAmount;
	float RotLookup[5] = {-2.0F, 2.0F, -1.0F, 1.0F, 0.0F};
	float start, end;

	currenttime = GetEventCurrentTime(TitleEvent);

	for (i=0; i<5; i++)
	{
		if ( (currenttime > (0.2F * (i + 1))) && (StackJudderNum == i))
		{
			OldStackRotAmount = StackRotAmount;
			StackJudderNum++;
		}
	}

	ReqRotAmount = RotLookup[StackJudderNum];

	start = 0.2F * StackJudderNum;
	end = 0.2F * (StackJudderNum + 1);
	
	currenttime = (currenttime - start) / (end - start);

	currenttime = (float)SIND(currenttime * 90.0F);

	StackRotAmount = ((ReqRotAmount - OldStackRotAmount) * currenttime) + OldStackRotAmount;

	RotateDiscStack(StackRotAmount);
}

void ExitEventDiscPanInit( TITLE_EVENT *TitleEvent )
{
}

void InitEventExpandBike(TITLE_EVENT *TitleEvent)
{
	ShowHoloModel( HoloModelLookup[ BikeList.selected_item ] );
}

void InitEventExpandCrystal(TITLE_EVENT *TitleEvent)
{
	ShowHoloModel( TITLE_MODEL_Crystal );
}

void ExitEventExpandBike(TITLE_EVENT *TitleEvent)
{
	BikeExpanded = true;
	
	if ( CurrentMenu != &MENU_NEW_ChangeBike )
		OldBikeSwapScale = BikeSwapScale;
}

void InitEventShrinkBike(TITLE_EVENT *TitleEvent)
{
	/*
	BikeExpanded = false;
	
	KillLines();

	PlaySfx( SFX_HoloChange, 0.5F );
	*/

	ShowHoloModel( (u_int16_t)-1 );
}

void InitStartDiscPanTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtDiscs;
	InitEventGeneralPan(NULL);
	
	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void InitGeneralTimer( TITLE_EVENT_TIMER *TitleEventTimer )
{
	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void InitRightDiscPanTimer(TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtDiscs;
	InitEventGeneralPan(NULL);

	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;

}

void InitLeftDiscPanTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtDiscs;
	InitEventGeneralPan(NULL);

	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void InitRightVDUPanTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtRightVDU;
	InitEventGeneralPan(NULL);
	VDU_Ready = false;

	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void InitLeftVDUPanTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtLeftVDU;
	InitEventGeneralPan(NULL);
	VDU_Ready = false;

	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void InitVisualsTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	CameraStatus = CAMERA_AtRightVDU;
	InitEventGeneralPan(NULL);
	VDU_Ready = false;
	PlaySfx( SFX_TitlePan, 1.0F );

	TitleEventTimer->Status = TITLE_EVENT_TIMER_ACTIVE;
	TitleEventTimer->CurrentTime = 0.0F;
}

void ChooseDiscTimer (TITLE_EVENT_TIMER *TitleEventTimer)
{
	switch (OldMenuStatus)
	{
	case TITLE_TIMER_NULL:
		InitStartDiscPanTimer(&Title_Timers[TITLE_TIMER_PanToDiscsInit]);
		break;
	case TITLE_TIMER_PanToRightVDU:
		InitRightDiscPanTimer(&Title_Timers[TITLE_TIMER_PanToDiscsFromRight]);
		break;
	case TITLE_TIMER_PanToLeftVDU:
		InitLeftDiscPanTimer(&Title_Timers[TITLE_TIMER_PanToDiscsFromLeft]);
		break;
	case TITLE_TIMER_Visuals:
		InitStartDiscPanTimer(&Title_Timers[TITLE_TIMER_PanToDiscsFromVisuals]);
		break;
	case TITLE_TIMER_NormalPanToRightVDU:
		InitStartDiscPanTimer(&Title_Timers[TITLE_TIMER_NormalPanToDiscsFromRight]);
		break;
	default:
		InitStartDiscPanTimer(&Title_Timers[TITLE_TIMER_PanToDiscsInit]);
		break;
	}

	TitleEventTimer->Status = TITLE_EVENT_TIMER_AWAITINGRESET;
}

void ResetAllEvents(void)
{
	TITLE_EVENT *Event;
	TITLE_EVENT_TIMER *Timer;

	for (Event = Title_Events; Event->TriggeredBy >= 0; Event++)
	{
		if ((Event->Status == TITLE_EVENT_STATUS_ACTIVE) || (Event->Status == TITLE_EVENT_STATUS_FINISHED))
		{
			if (Event->ExitFunc)
				Event->ExitFunc(Event);

			Event->Status = TITLE_EVENT_STATUS_IDLE;
		}
	}
	
	for (Timer = Title_Timers; Timer->Interval >= 0; Timer++)
	{
		Timer->Status = TITLE_EVENT_TIMER_IDLE;
	}

} 


void ProcessEvents(void)
{
	
	TITLE_EVENT *Event;
	TITLE_EVENT_TIMER Timer;

	for (Event = Title_Events; Event->TriggeredBy >= 0; Event++)
	{
		Timer = Title_Timers[Event->TriggeredBy];

		if (Event->Status == TITLE_EVENT_STATUS_IDLE)
		{
			if ((Timer.Status == TITLE_EVENT_TIMER_ACTIVE) || (Timer.Status == TITLE_EVENT_TIMER_FINISHED))
			{
				if (Timer.CurrentTime >= Event->StartTime)
				{
					if (Event->InitFunc)
						Event->InitFunc (Event);

					Event->Status = TITLE_EVENT_STATUS_ACTIVE;
				}
			}	
		}

		if (Event->Status == TITLE_EVENT_STATUS_ACTIVE)
		{
			if (Event->ProcessFunc)
				Event->ProcessFunc (Event);		// if timer is > than end time, process fn will take timer as exactly end time
			if (Timer.CurrentTime >= Event->EndTime)
			{
				if (Event->ExitFunc)
					Event->ExitFunc (Event);
				Event->Status = TITLE_EVENT_STATUS_FINISHED;
			}
		}

		if (Event->Status == TITLE_EVENT_STATUS_FINISHED)
		{
			if (Timer.Status == TITLE_EVENT_TIMER_FINISHED)
			{
				Event->Status = TITLE_EVENT_STATUS_IDLE;
			}
		}

	}

}

void ProcessTimers(void)
{
	TITLE_EVENT_TIMER *Timer;

	// ignore first few framelags - they are invariably huge
	if ( FramesIgnored < 3 )
	{
	 	FramesIgnored++;
		return;
	}

	for (Timer = Title_Timers; Timer->Interval >= 0; Timer++)
	{
		switch (Timer->Status)
		{
		case TITLE_EVENT_TIMER_ACTIVE:
			Timer->CurrentTime += framelag/(Timer->Interval * 60.0F);
			if (Timer->CurrentTime >= 1.0F)
				Timer->Status = TITLE_EVENT_TIMER_FINISHED;
			break;
		case TITLE_EVENT_TIMER_FINISHED:
			Timer->Status = TITLE_EVENT_TIMER_AWAITINGRESET;

		}
	}
}
							   
void DrawHighlightBox(TEXTINFO *TextInfo, u_int8_t r, u_int8_t g, u_int8_t b)
{
	float xmin, ymin, xmax, ymax, widthX, widthY, height, length, templength, actuallength;
	int i, font;

	if(!TextInfo)return;

	font = GetScreenFont(TextInfo->font);

	widthX = (float)ceil(ModeScaleX);
	widthY = (float)ceil(ModeScaleY);

	height = 0.0F;
	length = 0.0F;
	actuallength = 0.0F;
	templength = 0.0F;
	for (i = 0; i < TextInfo->num_lines; i++)
	{
		height += TextInfo->height[i];
		
		if (TextInfo->currentx[i] > length)
			length = TextInfo->currentx[i];

		if (TextInfo->length[i] > actuallength)
			actuallength = TextInfo->length[i];
	}

	length += 2.0F * ModeScaleX;
	if (length > actuallength)
		length = actuallength;
	
	//xmin = (float)(TextInfo->xmin + VDUoffsetX) * ModeScaleX;
	xmin = TextInfo->char1x - widthX;
	xmax = xmin + length + widthX * 2.0F;
	ymin = TextInfo->text_y_min - widthY;
	ymax = TextInfo->text_y_max + widthY;

	PlotHighlightPoly (xmin, ymin - widthY, xmax, ymin, r, g, b); //top
	PlotHighlightPoly (xmin, ymax, xmax, ymax + widthY, r, g, b); //bottom
	PlotHighlightPoly (xmin - widthX, ymin - widthY, xmin, ymax + widthY, r, g, b); //left

	if (actuallength == length)
		PlotHighlightPoly (xmax, ymin - widthY, xmax + widthX, ymax + widthY, r, g, b); //right
}

u_int16_t PlotHighlightPoly (float xmin, float ymin, float xmax, float ymax, u_int8_t r, u_int8_t g, u_int8_t b)
{
	u_int16_t poly;
	
	poly =FindFreeScrPoly();

	ScrPolys[poly].Flags = SCRFLAG_UseCoords;
	ScrPolys[poly].Type = SCRTYPE_LastAFrame;
	ScrPolys[poly].Frm_Info = NULL;
	ScrPolys[poly].Frame = 238.0F;
	ScrPolys[poly].x1 = xmin;
	ScrPolys[poly].y1 = ymin;	
	ScrPolys[poly].x3 = xmax;					
	ScrPolys[poly].y3 = ymax;					

	ScrPolys[poly].Col1.R = r;
	ScrPolys[poly].Col1.G = g;
	ScrPolys[poly].Col1.B = b;
	ScrPolys[poly].Col1.Trans = 255;
	ScrPolys[poly].Col2.R = r;
	ScrPolys[poly].Col2.G = g;
	ScrPolys[poly].Col2.B = b;
	ScrPolys[poly].Col2.Trans = 255;
	ScrPolys[poly].Col3.R = r;
	ScrPolys[poly].Col3.G = g;
	ScrPolys[poly].Col3.B = b;
	ScrPolys[poly].Col3.Trans = 255;
	ScrPolys[poly].Col4.R = r;
	ScrPolys[poly].Col4.G = g;
	ScrPolys[poly].Col4.B = b;
	ScrPolys[poly].Col4.Trans = 255;

	AddScrPolyToTPage( poly, GetTPage( NULL, 0 ) );

	return poly;
}

void SelectConnectionToStart (MENUITEM *Item)
{
	MenuState = MENUSTATE_Select;
}

void SelectConnectionToJoin (MENUITEM *Item)
{
	// a game in the list has been selected
	if( Item && Item->Value )
	{
		int index = 0;
		// find the selected game
		char * game_name = Item->Value;
		index = games_index_at( game_name );
		if( index )
		{
			// update the host settings
			strncpy(TCPAddress.text, games_ip_at(index), sizeof(TCPAddress.text));
			strncpy(host_port_str.text, games_port_at(index), sizeof(host_port_str.text));
		}
		// game was not found
		else
		{
			return;
		}
	}
	// go to the join screen
	MenuChange ( &MENU_ITEM_JoinMultiplayer );
}


void UpdateSessions ( int *dummy )
{
	GetPlayersInCurrentSession ( NULL );
}

void GetInitialPlayers ( MENU *menu )
{
	NumOfPlayersSlider.oldvalue = -1;
	PlayersList.display_items = 16;
	GetPlayersInCurrentSession ( NULL );
	InitTitleMessaging();
}

void SendTitleMessage(MENUITEM *Item)
{
	TEXT *text;
	TEXTMSG TextMsg;

	text = (TEXT *)Item->Variable;
   	
	strncpy( &MacroText4.text[0], &text->text[0], MAXTEXTMSG );
	MacroText4.text[ MAXTEXTMSG - 1 ] = 0;
	SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_TitleMessage, 0);

	strcpy ( &TextMsg.Text[0], &MacroText4.text[0] );
	TextMsg.WhoIAm = WhoIAm;
	AddTitleMessage(&TextMsg);

	TextClear( &TitlePlayerMessage );
	GetVduTextFormattingInfo ( &TitlePlayerMessage );
}

void AddTitleMessage(LPTEXTMSG lpTextMsg)
{
	char tempbuf[ 256 ];
	
	if (TotalTitleMsgs != MAX_TITLE_MSGS)
	{
		char * name = (WhoIAm == lpTextMsg->WhoIAm) ? biker_name : Names[lpTextMsg->WhoIAm];
		snprintf( TitleMsgQue[TitleMsgQueStart].text, MAX_TITLE_MSG_SIZE,
			"%s: %s", name, &lpTextMsg->Text[0] );
		TitleMsgQue[TitleMsgQueStart].timer = MAX_TITLE_MSG_TIME;
		TitleMsgQueStart++;
		TotalTitleMsgs++;
		if (TitleMsgQueStart == MAX_TITLE_MSGS)
			TitleMsgQueStart = 0;
	}
}

void GetTitleMessage(void)
{
 	MENUITEM *Item;
	TEXTINFO dummytext;
	int i, j;
	
	TitleMessageTimer -= framelag;
	if (TitleMessageTimer < 0.0F)
	{
		TitleMessageTimer = 0.0F;
		if (TotalTitleMsgs > 0)
		{
			dummytext.xmin = 5;
			dummytext.ymin = 0;						
			dummytext.xmax = 200;					  
			dummytext.ymax = 16;						
			dummytext.font = FONT_Small;				   
			dummytext.flags = TEXTFLAG_ForceFit;						
			dummytext.num_lines = 0;				
			dummytext.boxdone = false;					
			dummytext.num_pointers_to_free = 0;		
			dummytext.force_fit = false;
			dummytext.text = &TitleMsgQue[TitleMsgQueEnd].text[0];
			PrintTextItem( &dummytext );

			for (i = 0; i < dummytext.num_lines; i++)
			{

				j = TITLE_TEXT_MSGS_STORED;
				while (--j)
				{
					strcpy( TitleMessage[j], TitleMessage[j - 1] );
				}
				
				strncpy( &TitleMessage[0][0], &dummytext.text[dummytext.startpoint[i]], dummytext.endpoint[i] - dummytext.startpoint[i] + 1);
				TitleMessage[0][dummytext.endpoint[i] - dummytext.startpoint[i] + 1] = '\0';
			}
			TitleMsgQueEnd++;
			TitleMessageTimer = MAX_TITLE_MSG_TIME;

			TotalTitleMsgs--;
			if (TitleMsgQueEnd == MAX_TITLE_MSGS)
				TitleMsgQueEnd = 0;

			for( Item = CurrentMenu->Item ; Item->x >= 0 ; Item++ )
			{	
				if (Item->Variable)
				{
					for (i = 0; i < MAX_TITLE_MSG_TIME; i++)
					{
						if (Item->Variable == TitleMessage[i])
							PrintTextItem(Item->TextInfo[0]);
					}
				}
			}
		}
	}
}

// type - whether or not to execute exit/entry functions of menus passed through
// NewMenu - new menu to go into after stepping back (can be NULL)
void PrintErrorMessage (char *ErrorStr, int BackBy, MENU *NewMenu, int type)
{
	MENUITEM ChangeItem;

	MenuState = MENUSTATE_Select;

	// make sure message will be displayed on VDU...
	if ((CameraStatus != CAMERA_AtLeftVDU) && (CameraStatus != CAMERA_AtRightVDU))
		MENU_NEW_Error.MenuStatus = TITLE_TIMER_NormalPanToRightVDU;
	else
		MENU_NEW_Error.MenuStatus = CurrentMenu->MenuStatus;

	strcpy (ErrorMessage, ErrorStr);
	if ( type & ERROR_QUIT )
		ErrorMoveBackBy = -1;
	else
		ErrorMoveBackBy = BackBy;

	ErrorNewMenu = NewMenu;

	if (type & ERROR_DONTUSE_MENUFUNCS)
		IgnoreMenuFuncs = true;
	else
		IgnoreMenuFuncs = true;
	
	ChangeItem.Value = &MENU_NEW_Error;
	MenuChange( &ChangeItem );
}

void MenuItemBackFromError ( MENUITEM *Item )
{
	MENUITEM ChangeItem;
	int i;

	if ( ErrorMoveBackBy < 0 )
	{
		LastMenu = CurrentMenu;
		VduClear();
		MyGameStatus = STATUS_QuittingFromTitles;
		return;
	}

	MenuBack();
	CheckMenuTimer();
	for (i = 0; i < ErrorMoveBackBy; i++)
	{
		if ( CurrentMenu != &MENU_Start )
		{
			MenuBack();
			CheckMenuTimer();
		}
	}

	if (ErrorNewMenu)
	{
		ChangeItem.Value = ErrorNewMenu;
		MenuChange( &ChangeItem );
	}

	IgnoreMenuFuncs = false;


}

void SelectConfigureAxisMenu ( MENUITEM *Item )
{
	if ( !strcasecmp( JoystickList.item[ JoystickList.selected_item ], "Spacetec SpaceOrb 360" ) 
		&& !SpaceOrbSetup )
	{
		// do not allow axis setup
		PrintErrorMessage (LT_SpaceOrb/*"You must use spaceware to set up the spaceball axis"*/, 0, NULL, ERROR_DONTUSE_MENUFUNCS);
	}else
	{
		MenuChange( &AxisScreenItem );
	}
}

void InitJoystickList ( MENU *Menu )
{
	int i, j;
	bool jsfound = false;

	// if no joysticks connected, display error message & return to last menu...
	for (i = 0; i < Num_Joysticks; i++)
	{
		if (JoystickInfo[i].connected)
		{
			jsfound = true;
			break;
		}
	}

	if (!jsfound)
	{	PrintErrorMessage (LT_NoJoysticks/*"No Joysticks Connected~~(you cannot connect a joystick while the game is running)"*/, 1, NULL, ERROR_DONTUSE_MENUFUNCS);
		return;
	}

	JoystickList.items = 0;
	JoystickList.display_items = 3;
	JoystickList.top_item = 0;
	JoystickMap[JoystickList.selected_item] = 0;

	j = 0;
	for (i = 0; i < Num_Joysticks; i++)
	{
		if ( JoystickInfo[i].connected )
		{
			strncpy( JoystickList.item[j], JoystickInfo[i].Name, sizeof(JoystickList.item[0]));
			JoystickList.items++;
			JoystickMap[j] = i;
			j++;
		}
	}

}


void InitJoystickAxisConfig ( MENU *Menu)
{
	int i, j;
//	float mid;
	SCRPOLY_RGB light, dark, polycol[NUM_SHIP_AXIS_ACTIONS][2];
	float sensitivity;

	light.R = 64;
	light.G = 255;
	light.B = 64;
	light.Trans = 0;
	dark.R = 0;
	dark.G = 0;
	dark.B = 0;
	dark.Trans = 0;

	polycol[0][0] = dark;
	polycol[0][1] = light;

	polycol[1][0] = light;
	polycol[1][1] = dark;


	JoystickAxisList.items = 0;
	JoystickAxisList.display_items = 1;
	JoystickAxisList.top_item = 0;
	JoystickAxisList.selected_item = 0;

	j = 0;
	for (i = 0; i < MAX_JOYSTICK_AXIS; i++)
	{
		if (JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[i].exists)
		{
			if(JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[i].name)
			{
				strncpy(
					JoystickAxisList.item[j],
					JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[i].name,
					sizeof(JoystickList.item[0])
				);
			}
			else
			{
				snprintf(
					JoystickAxisList.item[j],
					sizeof(JoystickList.item[0]),
					"Axis %d",
					j
				);
			}
			JoystickAxisList.items++;
			AxisMap[j] = i;
			j++;
		}
	}

	for (i = 0; i < NUM_SHIP_AXIS_ACTIONS; i++)
	{
		if (JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[AxisMap[0]].action == ShipAxisLookup[i])
		{
			AxisActionList.selected_item = i;
			break;
		}
	}
	
	AxisSlider.min = 0;
	AxisSlider.max = JoystickAxisList.items - 1;
	AxisSlider.step = 1;
	AxisSlider.value = JoystickAxisList.selected_item;
	AxisSlider.FuncUser = AxisName;
	AxisSlider.FuncSet = SetAxisSlider;

	AxisActionSlider.min = 0;
	AxisActionSlider.max = AxisActionList.items - 1;
	AxisActionSlider.step = 1;
	AxisActionSlider.value = AxisActionList.selected_item;
	AxisActionSlider.FuncUser = AxisActionName;
	AxisActionSlider.FuncSet = SetAxisActionSlider;

	AxisInvert = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[AxisMap[0]].inverted; 
	FineControl = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[AxisMap[0]].fine; 
	DeadzoneSlider.value = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[AxisMap[0]].deadzone; 
	sensitivity = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[AxisMap[0]].sensitivity;
	SensitivitySlider.value =
		( sensitivity < 1.0F ) ? (int) floor( 0.49F + ( 100 * sensitivity - 1 ) / sensitivity ) : 100;

	// set up axis test scrpolys
	for (i = 0; i < NUM_AXIS_TEST_SCRPOLYS; i++)
	{
		AxisTestScrPoly[i] = FindFreeScrPoly();

		if (AxisTestScrPoly[i] == (u_int16_t)-1)
		{
			Msg("Title.c: InitJoystickAxisConfig() unable to allocate screen poly\n");
			exit(1);
		}

		ScrPolys[AxisTestScrPoly[i]].Type = SCRTYPE_Normal;
		ScrPolys[AxisTestScrPoly[i]].Flags = SCRFLAG_UseCoords;
		ScrPolys[AxisTestScrPoly[i]].Type = SCRTYPE_Normal;

		ScrPolys[AxisTestScrPoly[i]].Col1 = polycol[i][0];
		ScrPolys[AxisTestScrPoly[i]].Col2 = polycol[i][1];
		ScrPolys[AxisTestScrPoly[i]].Col3 = polycol[i][1];
		ScrPolys[AxisTestScrPoly[i]].Col4 = polycol[i][0];

		ScrPolys[AxisTestScrPoly[i]].Frm_Info = NULL;
		AddScrPolyToTPage( AxisTestScrPoly[i], GetTPage( NULL, 0 ) );
	}

	AxisPtr_mid = (AXISTEST_XPos + AXISTEST_Width / 2.0F) + VDUoffsetX;
	AxisPtr_mid *= ModeScaleX;
//	mid = (AXISTEST_XPos + AXISTEST_Width / 2.0F); 
	
//	AxisPtr_xmin = mid - AXISTEST_PtrWidth / 2.0F + VDUoffsetX;
//	AxisPtr_xmax = mid + AXISTEST_PtrWidth / 2.0F + VDUoffsetX;
	AxisPtr_ymin = AXISTEST_YPos + VDUoffsetY;
	AxisPtr_ymax = AXISTEST_YPos + AXISTEST_Height + VDUoffsetY;

	AxisPtr_xmin *= ModeScaleX;
	AxisPtr_xmax *= ModeScaleX;
	AxisPtr_ymin *= ModeScaleY;
	AxisPtr_ymax *= ModeScaleY;
	
	UpdateAxisPtr( 0.0F );	// 0 = centered

}

// deltaX = -1 is fully left
// deltaX = +1 is fully right
void UpdateAxisPtr( float pos )
{
	float distance, dx, xmin[2], xmax[2];
	SCRPOLY_RGB light, dark, polycol[NUM_SHIP_AXIS_ACTIONS][2];
	int i;
	
	light.R = 64;
	light.G = 255;
	light.B = 64;
	light.Trans = 0;
	dark.R = 0;
	dark.G = 0;
	dark.B = 0;
	dark.Trans = 0;

	distance = (AXISTEST_Width / 2.0F) * ModeScaleX;
	
	if ( pos < -1.0F )
		pos = -1.0F;
	else if ( pos > 1.0F )
		pos = 1.0F;
	dx = pos * distance;

	if ( pos > 0.01F )
	{
		// right of centre
		polycol[0][0] = dark;
		polycol[0][1] = ( pos < 0.99F ) ? dark : light;
		polycol[1][0] = light;
		polycol[1][1] = dark;
	}
	else if ( pos < -0.01F )
	{
		// left of centre
		polycol[0][0] = dark;
		polycol[0][1] = light;
		polycol[1][0] = ( pos > -0.99F ) ? dark : light;
		polycol[1][1] = dark;
	}
	else
	{
		// in centre
		polycol[0][0] = dark;
		polycol[0][1] = light;
		polycol[1][0] = light;
		polycol[1][1] = dark;
	}

	xmin[0] = AxisPtr_mid - AXISTEST_PtrWidth;
	xmax[0] = AxisPtr_mid;
	xmin[1] = AxisPtr_mid;
	xmax[1] = AxisPtr_mid + AXISTEST_PtrWidth;

	for (i = 0; i < 2; i++)
	{
		ScrPolys[AxisTestScrPoly[i]].x1 = xmin[i] + dx;
		ScrPolys[AxisTestScrPoly[i]].y1 = AxisPtr_ymin;	
		ScrPolys[AxisTestScrPoly[i]].x2 = xmin[i] + dx;
		ScrPolys[AxisTestScrPoly[i]].y2 = AxisPtr_ymax;
		ScrPolys[AxisTestScrPoly[i]].x3 = xmax[i] + dx;	
		ScrPolys[AxisTestScrPoly[i]].y3 = AxisPtr_ymax;
		ScrPolys[AxisTestScrPoly[i]].x4 = xmax[i] + dx;					
		ScrPolys[AxisTestScrPoly[i]].y4 = AxisPtr_ymin;
		ScrPolys[AxisTestScrPoly[i]].Col1 = polycol[i][0];
		ScrPolys[AxisTestScrPoly[i]].Col2 = polycol[i][1];
		ScrPolys[AxisTestScrPoly[i]].Col3 = polycol[i][1];
		ScrPolys[AxisTestScrPoly[i]].Col4 = polycol[i][0];
	}
}

void ChooseJoyAxis( MENUITEM *Item )
{
	int i, joystick, axis, action;
	MENUITEM *ToggleItem=NULL, *SliderItem=NULL, *CheckItem=NULL, *AxisActionItem=NULL, *SensitivityItem=NULL, *FineItem=NULL;
	float sensitivity;

	joystick = JoystickMap[JoystickList.selected_item];
	axis = AxisMap[JoystickAxisList.selected_item];
	
	action = JoystickInfo[joystick].Axis[axis].action;

	AxisActionList.selected_item = 0;
	for (i = 0; i < NUM_SHIP_AXIS_ACTIONS; i++)
	{
		if (action == ShipAxisLookup[i])
		{
			AxisActionList.selected_item = i;
			break;
		}
	}
	AxisActionSlider.value = AxisActionList.selected_item;

	// find toggle & slider items...
	for( CheckItem = CurrentMenu->Item ; CheckItem->x >= 0 ; CheckItem++ )
	{  
		if ( CheckItem->Variable == &AxisInvert) 
			ToggleItem = CheckItem;

		if ( CheckItem->Variable == &DeadzoneSlider) 
			SliderItem = CheckItem;

		if ( CheckItem->Variable == &AxisActionSlider )
			AxisActionItem = CheckItem;

		if ( CheckItem->Variable == &SensitivitySlider) 
			SensitivityItem = CheckItem;

		if ( CheckItem->Variable == &FineControl )
			FineItem = CheckItem;
	}

	// set toggle...
	if (AxisInvert != JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].inverted)
	{
		AxisInvert = !AxisInvert;
		DrawFlatToggleStatus( ToggleItem );
	}

	if ( FineControl != JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].fine)
	{
		FineControl = !FineControl;
		DrawFlatToggleStatus( FineItem );
	}

	// set deadzone...
	DeadzoneSlider.oldvalue = DeadzoneSlider.value;
	DeadzoneSlider.value = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].deadzone; 
	RedrawFlatMenuSlider( SliderItem );

	RedrawFlatMenuSlider( AxisActionItem );

	sensitivity = JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[ axis ].sensitivity;
	SensitivitySlider.oldvalue = SensitivitySlider.value;
	SensitivitySlider.value =
		( sensitivity < 1.0F ) ? (int) floor( 0.49F + ( 100 * sensitivity - 1 ) / sensitivity ) : 100;
	RedrawFlatMenuSlider( SensitivityItem );

	LastMenu = CurrentMenu;
	VduClear();
	ProcessVduItems( &MENU_NEW_ConfigJoyAxis );
}

void InitShipActionList( void )
{
	int i;

	ShipActionList.items = 0;
	ShipActionList.display_items = 28;
	ShipActionList.top_item = 0;
	ShipActionList.selected_item = 0;
	for (i = 0; i < NUM_SHIP_ACTIONS; i++)
	{
		strncpy( ShipActionList.item[i], ShipActionText[i], sizeof( ShipActionList.item[0] ) );
		ShipActionList.items++;
	}

	AxisActionList.items = 0;
	AxisActionList.display_items = 7;
	AxisActionList.top_item = 0;
	AxisActionList.selected_item = 0;
	for (i = 0; i < NUM_SHIP_AXIS_ACTIONS; i++)
	{
		strncpy( AxisActionList.item[i], ShipAxisText[i], sizeof( AxisActionList.item[0] ) );
		AxisActionList.items++;
	}
}

void AssignShipActionToJoyAxis( MENUITEM *Item)
{
	int axis;
	
	axis = AxisMap[JoystickAxisList.selected_item];

	JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].action = ShipAxisLookup[AxisActionList.selected_item];
}

void SetAxis( MENU *Menu )
{
	int i;
	
	SetUpJoystickAxis(JoystickMap[JoystickList.selected_item]);

	// remove axistest scrpoly...
	for( i = 0; i < NUM_AXIS_TEST_SCRPOLYS; i++ )
		KillUsedScrPoly(AxisTestScrPoly[i]);
}

void SetAxisInv( MENUITEM *Item )
{
	int axis;

	axis = AxisMap[JoystickAxisList.selected_item];
	JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].inverted = !(JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].inverted);
}

void SetAxisFineControl( MENUITEM *Item )
{
	int axis;

	axis = AxisMap[JoystickAxisList.selected_item];
	JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].fine = !(JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].fine);
}

void SetAxisDeadzone( MENUITEM *Item )
{
	int axis;

	axis = AxisMap[JoystickAxisList.selected_item];
	JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].deadzone = DeadzoneSlider.value;

	SetUpJoystickAxis(JoystickMap[JoystickList.selected_item]);
}

void SetAxisSensitivity( MENUITEM *Item )
{
	int axis;

	axis = AxisMap[JoystickAxisList.selected_item];
	JoystickInfo[JoystickMap[JoystickList.selected_item]].Axis[axis].sensitivity =
		( SensitivitySlider.value < 100 ) ?	1.0F / ( 100 - SensitivitySlider.value ) : 1.0F;
}

void CheckJoyAxis( int *dummy )
{
	int joystick = JoystickMap[JoystickList.selected_item];
	int axis;
	float pos;

#ifdef DINPUTJOY

	// this is a good thing cause sdl also considers a slider an axis
	long *axisptr[MAX_JOYSTICK_AXIS] =
	{
		&js[ new_input ][joystick].lX,  // 0
		&js[ new_input ][joystick].lY,  // 1
		&js[ new_input ][joystick].lZ,  // 2
		&js[ new_input ][joystick].lRx, // 3
		&js[ new_input ][joystick].lRy, // 4
		&js[ new_input ][joystick].lRz, // 5
		&js[ new_input ][joystick].rglSlider[0], // 6
		&js[ new_input ][joystick].rglSlider[1]  // 7
	};

#endif

	char *left;
	char *right;

	axis = AxisMap[JoystickAxisList.selected_item];

#ifdef DINPUTJOY
	pos = (float) *axisptr[axis];
#else
	pos = (float) joy_axis_state[ joystick ][ axis ];
#endif

	//DebugPrintf("%f\n",pos);

	pos = pos * JoystickInfo[ joystick ].Axis[ axis ].sensitivity;

	if ( JoystickInfo[ joystick ].Axis[ axis ].fine )
		pos *= (float) fabs( pos );

	UpdateAxisPtr( pos );

	if (JoystickInfo[joystick].Axis[axis].inverted)
	{
		left = ShipAxisSeperateText[AxisActionList.selected_item * 2 + 1];
		right = ShipAxisSeperateText[AxisActionList.selected_item * 2];
	}
	else
	{
		right = ShipAxisSeperateText[AxisActionList.selected_item * 2 + 1];
		left = ShipAxisSeperateText[AxisActionList.selected_item * 2];
	}

	strcpy(AxisExtremeLeftText, left);
	strcpy(AxisExtremeRightText, right);

	// if choosing axis, & selected item has changed...
	if (CurrentListItem)
	{
		if ((LIST *)CurrentListItem->Variable == &JoystickAxisList)
			ChooseJoyAxis( NULL );
	}
}

void ExitJoySetup( MENU *Menu )
{
   int joystick, i, j, k;
	
	for ( i = 0; i < JoystickList.items; i++ )
	{
		joystick = JoystickMap[ i ];
		
		// check all axis
		for (j = 0; j < MAX_JOYSTICK_AXIS; j++)
		{
			if ( JoystickInfo[ joystick ].Axis[ j ].exists &&  JoystickInfo[ joystick ].Axis[ j ].action )
			{
				JoystickInfo[ joystick ].assigned = true;
			}
		}

		// check all povs
		for (j = 0; j < JoystickInfo[ joystick ].NumPOVs; j++)
		{
			for ( k = 0; k < MAX_POV_DIRECTIONS; k++ )
			{
				if ( JoystickInfo[ joystick ].POV[ j ].action[ k ] )
				{
					JoystickInfo[ joystick ].assigned = true;
				}
			}
		}

		// check all buttons
		for (j = 0; j < JoystickInfo[ joystick ].NumButtons; j++)
		{
			if ( JoystickInfo[ joystick ].Button[ j ].action )
			{
			 	JoystickInfo[ joystick ].assigned = true;
			}
		}
	}

	write_config( player_config, biker_config );
}

void ExitMouseSetup( MENU *Menu )
{
//	if ( memcmp( &Config, player_config, sizeof( Config ) ) )
	{
		*player_config = Config;
		write_config( player_config, biker_config );
	}
}


void SelectVDUList ( MENUITEM *Item )
{
	MENUITEM *NewItem;
	LIST *List;

	List = (LIST *)(Item->Variable);

	for( NewItem = CurrentMenu->Item ; NewItem->x >= 0 ; NewItem++ )
	{	
		if ( NewItem->Variable == List && (NewItem != Item))
		{	CurrentMenuItem = NewItem;
			SelectList( CurrentMenuItem );
			PreListItem = Item;
			break;
		}
	}
}

void GoToNextLevel( MENUITEM *Item )
{
	StartASinglePlayerGame( NULL );
}

void TitleLoadGame( MENUITEM *Item )
{
	LoadASinglePlayerGame( NULL );
}


void RestoreDefaultControlSettings( MENUITEM *Item )
{
	// replace config...
	Config = default_config;
	Config.bike = ValidBikeSelected( Config.bike );
	SelectedBike = Config.bike;
	ReInitJoysticks();
	DefaultJoystickSettings( &Config );

	// re-init controls...
	InitControls( NULL );

	LastMenu = CurrentMenu;	
	VduClear();
	ProcessVduItems( CurrentMenu );
}

void InitSelectedLevelText( MENU *Menu )
{
//	strcpy( SelectedLevelText, MissionBriefing[ CurrentLevel ] );
}

void PrepareNextLevelStart( MENU *Menu )
{
	LoadLevelText( NULL );
	MyGameStatus = STATUS_WaitingToStartSinglePlayer; 
}

void HostAboutToStart( MENUITEM *Item )
{
	if( TeamGame )
	{
		MenuChange( &NewTeamItem );
	}else
	{
		MyGameStatus = STATUS_WaitingToStartMultiPlayerHost;
		MenuChange( Item );
	}
}

void ExitSoundMenu( MENU *Menu )
{
	write_config( player_config, biker_config );
}

void MakeUnselectable( MENUITEM *Item )
{
	Item->highlightflags |= TEXTFLAG_Unselectable;
	CursorDown();
}

void SendQuickText( MENUITEM *Item )
{
	MenuExit();
	SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_QuickTaunt, 0);
	QuickText.text[0] = 0;
	JustExitedMenu = false;
	flush_input = true;
}
void SendQuickTextWhisper( MENUITEM *Item )
{
	MenuExit();
	SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_QuickTauntWhisper, 0);
	QuickTextWhisper.text[0] = 0;
	JustExitedMenu = false;
	flush_input = true;
}


void InitCrystalsFoundText( MENU *Menu )
{
	if ( CrystalsFound == 1 )
	{
		sprintf( CrystalsFoundText, LT_OneCrystalFound, CrystalsFound );
	}
	else
	{
		sprintf( CrystalsFoundText, LT_CrystalsFound, CrystalsFound );
	}

	if ( NumGoldBars == 1 )
	{
		sprintf( GoldBarsFoundText, LT_OneGoldBarFound, (int)NumGoldBars );
	}
	else
	{
		sprintf( GoldBarsFoundText, LT_GoldBarsFound, (int)NumGoldBars );
	}
	
	if( Secrets == 1 )
	{
		sprintf( SecretsFoundText, LT_OneSecretFoundOutOf, Secrets, TotalSecrets );
	}
	else
	{
		sprintf( SecretsFoundText, LT_SecretsFoundOutOf, Secrets, TotalSecrets );
	}

	if( NumKilledEnemies == 1 )
	{
		sprintf( EnemiesKilledText, LT_OneEnemyKilled, (int)NumKilledEnemies, (int)NumInitEnemies );
	}
	else
	{
		sprintf( EnemiesKilledText, LT_EnemysKilled, (int)NumKilledEnemies, (int)NumInitEnemies );
	}

	/*
	sprintf( TimeTakenText, "time taken %d min%s %d sec%s", 
		IntLevelTimeTaken / 60,
		( ( IntLevelTimeTaken / 60 ) == 1 ) ? "" : "s",
		IntLevelTimeTaken % 60,
		( ( IntLevelTimeTaken % 60 ) == 1 ) ? "" : "s"); 
	*/
	if ( ( ( LevelTimeTaken / 60 ) == 1 ) && ( ( (int)LevelTimeTaken % 60 ) == 1 ) )
	{
		sprintf( TimeTakenText, LT_TimeTaken1/*"time taken %d min %d sec"*/,	(int)(LevelTimeTaken / 60),	(int)((int)LevelTimeTaken % 60) ); 
	}
	if ( ( ( LevelTimeTaken / 60 ) != 1 ) && ( ( (int)LevelTimeTaken % 60 ) == 1 ) )
	{
		sprintf( TimeTakenText, LT_TimeTaken2/*"time taken %d mins %d sec"*/,	(int)(LevelTimeTaken / 60),	(int)((int)LevelTimeTaken % 60) ); 
	}
	if ( ( ( LevelTimeTaken / 60 ) == 1 ) && ( ( (int)LevelTimeTaken % 60 ) != 1 ) )
	{
		sprintf( TimeTakenText, LT_TimeTaken3/*"time taken %d min %d secs"*/,	(int)(LevelTimeTaken / 60),	(int)((int)LevelTimeTaken % 60) ); 
	}
	if ( ( ( LevelTimeTaken / 60 ) != 1 ) && ( ( (int)LevelTimeTaken % 60 ) != 1 ) )
	{
		sprintf( TimeTakenText, LT_TimeTaken4/*"time taken %d mins %d secs"*/,	(int)(LevelTimeTaken / 60),	(int)((int)LevelTimeTaken % 60) ); 
	}
}

void SelectGoldBarModel( MENU *Menu )
{
	ShowHoloModel( TITLE_MODEL_GoldBar );
}

bool InitScene(void);
void AfterSpecialMessage( MENUITEM *Item )
{
	switch( GameCompleted )
	 {
	 case GAMECOMPLETE_GotAllCrystalsSoFar:
		 MenuChangeEx( &MENU_NEW_BetweenLevels );
		 return;
	 }

	ReleaseView();
	MyGameStatus = STATUS_Title;
	MenuRestart( &MENU_Start );
	TitleInitDone = false;
	InitialTexturesSet = false;
	InitScene();
	InitView();
}

void NextLevelOrGameComplete( MENUITEM *Item )
{
	// kill off hololight, holopad & holo-object
	KillHoloModel();
	Models[BackgroundModel[TITLE_MODEL_HoloLight]].Visible = 0;
	Models[BackgroundModel[TITLE_MODEL_Holopad]].Visible = 0;
	
	switch( GameCompleted )
	 {
	 case GAMECOMPLETE_NotComplete:
		 MenuChangeEx( &MENU_NEW_BetweenLevels );
		 return;
	 case GAMECOMPLETE_WithoutAllCrystals:
		 MenuChangeEx( &MENU_NEW_GeneralLoading );
		 MyGameStatus = STATUS_WaitingToStartEndGameSequence;
		 return;
	 case GAMECOMPLETE_Cheated:
		 strcpy( GameCompleteText, GameCompleteWithCheating );
		 break;
 	 case GAMECOMPLETE_GotAllCrystalsSoFar:
		 strcpy( GameCompleteText, GameCompleteGotAllCrystalsSoFar );
		 break;
	 case GAMECOMPLETE_WithAllCrystals:
		 MenuChangeEx( &MENU_NEW_GeneralLoading );
		 MyGameStatus = STATUS_WaitingToStartEndGameSequence;
		 return;
	 case GAMECOMPLETE_WithAllCrystalsExceptLast:
		 MenuChangeEx( &MENU_NEW_GeneralLoading );
		 MyGameStatus = STATUS_WaitingToStartEndGameSequence;
		 return;
	 case GAMECOMPLETE_AdditionalLevels:
		 strcpy( GameCompleteText, GameCompleteAdditionalLevels );
		 break;
	 }
	
	 MenuChangeEx( &MENU_NEW_SpecialMessage );
}

char TypeFileName( TEXT *t, char c )
{
	switch ( c )
	{
	case '\\':
	case '/':
	case ':':
	case '*':
	case '?':
	case '"':
	case '<':
	case '>':
	case '|':
		return 0;
	default:
		return c;
	}
}


char *CTF_Type( SLIDER *s )
{
	switch ( s->value )
	{
	case CTF_STANDARD:
		return LT_CTF_Normal/*"normal"*/;
	case CTF_MUST_CARRY:
		return LT_CTF_MustCarry/*"carry it back"*/;
	case CTF_CAN_CARRY:
		return LT_CTF_CanCarry/*"can pick up"*/;
	case CTF_ONE_TOUCH:
		return LT_CTF_OneTouch/*"1 touch return"*/;
	case CTF_NO_RETURN:
		return LT_CTF_NoReturn/*"can't pick up"*/;
	default:
		return LT_CTF_Standard/*"standard"*/;
	}
}


bool ListDelete( LIST *l, int item )
{
	if ( item >= 0 && item < l->items )
	{
		while ( ++item < l->items )
		{
			strncpy( l->item[ item - 1 ], l->item[ item ], sizeof( l->item[ 0 ] ) );
		}
		l->items--;
		if ( l->selected_item >= l->items )
			l->selected_item = l->items - 1;
		if ( l->top_item >= l->items )
			l->top_item = l->items - 1;
		return true;
	}
	else
		return false;
}


bool DeleteDemo( LIST *l, int item )
{
	return delete_file( DemoFileName( l->item[ item ] ) ) ? ListDelete( l, item ) : false;
}


void InGameSaveASinglePlayerGame( MENUITEM *item )
{
	if ( InGameSave( item ) )
	{
		strncpy( LoadSavedGameList.item[ LoadSavedGameList.selected_item ], SavedGameInfo( LoadSavedGameList.selected_item ), sizeof( LoadSavedGameList.item[ 0 ] ) - 1 );
		LoadSavedGameList.item[ LoadSavedGameList.selected_item ][ sizeof( LoadSavedGameList.item[ 0 ] ) - 1 ] = 0;
	}
}


void DebugLastError( void )
{
#ifdef WIN32
	DWORD err;
	LPVOID lpMsgBuf;

	err = GetLastError();

	FormatMessage( 
		  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		  NULL,
		  err,
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		  (LPTSTR) &lpMsgBuf,
		  0,
		  NULL 
		  );
	DebugPrintf( "DeleteSavedGame failed: err=0x%08X msg=%s\n",
		err, lpMsgBuf );
	LocalFree( lpMsgBuf );
#endif
}


bool DeleteSavedGame( LIST *l, int item )
{
	if ( delete_file( SaveGameFileName( item ) )
		&& delete_file( SaveGamePicFileName( item ) ) )
	{
		strncpy( l->item[ item ], SavedGameInfo( item ), sizeof( l->item[ 0 ] ) - 1 );
		l->item[ item ][ sizeof( l->item[ 0 ] ) - 1 ] = 0;
		return true;
	}
	DebugLastError();
	return false;
}

void SaveMacros( MENUITEM *Item )
{
	strcpy( player_config->macro1, MacroText1.text ); 
	strcpy( player_config->macro2, MacroText2.text ); 
	strcpy( player_config->macro3, MacroText3.text ); 

	write_config( player_config, biker_config );
}

bool SetGamma( SLIDER *slider )
{
	double tempgamma;

	tempgamma = Gamma;
	
	Gamma = (double)((float)slider->value / 100.0F);

	// convert to float to get rid of small rounding errors
	if ( (float)tempgamma == (float)Gamma )
		return true;

	DebugPrintf("Gamma set to %f\n",Gamma);

	ReleaseView();
	if ( !InitView() )
	{
		Msg("Failed to InitView() in SetGamma()\n");
		exit( 1 );
	}

	InitialTexturesSet = false;

	if( MyGameStatus == STATUS_Title )
	{
		FadeHoloLight(HoloLightBrightness);
		DarkenRoom2(RoomDarkness);
	}

	return true;
}

bool SetWaterDetail( SLIDER *slider )
{
	switch( slider->value )
	{
	case 1:
		WATER_CELLSIZE = 128.0F;
		break;
	case 2:
		WATER_CELLSIZE = 64.0F;
		break;
	default:
		WATER_CELLSIZE = 64.0F;
	}

	return true;
}

bool SetTextScale( SLIDER *slider )
{
	FontWidth = 8.0F + (4.0F*slider->value);
	FontHeight = 8.0F + (4.0F*slider->value);
	return true;
}

bool SetNumPrimaryPickups( SLIDER *slider )
{
	NumPrimaryPickups = slider->value;

	return true;
}

char *AxisName( SLIDER *s )
{
	return ( s->value >= 0 && s->value < JoystickAxisList.items ) ? JoystickAxisList.item[ s->value ] : NULL;
}


char *AxisActionName( SLIDER *s )
{
	return ( s->value >= 0 && s->value < AxisActionList.items ) ? AxisActionList.item[ s->value ] : NULL;
}


bool SetAxisSlider( SLIDER *s )
{
	if ( s->value >= 0 && s->value < JoystickAxisList.items )
	{
		if ( JoystickAxisList.selected_item != s->value )
		{
			JoystickAxisList.selected_item = s->value;
			ChooseJoyAxis( NULL );
		}
		return true;
	}
	else
		return false;
}


bool SetAxisActionSlider( SLIDER *s )
{
	if ( s->value >= 0 && s->value < AxisActionList.items )
	{
		int joystick = JoystickMap[JoystickList.selected_item];
		int axis;

		AxisActionList.selected_item = s->value;
		axis = AxisMap[JoystickAxisList.selected_item];
		JoystickInfo[joystick].Axis[axis].action = ShipAxisLookup[ AxisActionList.selected_item ];
		CheckJoyAxis( NULL );
		return true;
	}
	else
		return false;
}
#if 1

#define HOLOMODEL_Done		0
#define HOLOMODEL_Expand	1
#define HOLOMODEL_Shrink	2

float HoloModelTimer;
int HoloModelMode;
u_int16_t NextHoloModel;

#define HOLOSCANSPEED 0.5F
#define NUM_HOLO_SCANPLANES 2
PLANE HoloPlane[ NUM_HOLO_SCANPLANES ] = {
	{ 0.0F, 0.0F, -1.0F, 0.0F },
	{ 0.0F, 0.0F, 1.0F, 0.0F },
};
float MinHoloPlaneOffset[ NUM_HOLO_SCANPLANES ];
float MaxHoloPlaneOffset[ NUM_HOLO_SCANPLANES ];
u_int8_t CurrentHoloPlane;
float CurrentHoloOffset;

void InitHoloScanLine( void )
{
	int i;
	
	CurrentHoloPlane = 0;

	for( i = 0; i < NUM_HOLO_SCANPLANES; i++ )
	{
		HoloPlane[ i ].Offset = 0.0F;
		GetExtremeOffsets( CurrentHoloModel, &HoloPlane[ i ], &MinHoloPlaneOffset[ i ], &MaxHoloPlaneOffset[ i ] );
	}

	CurrentHoloOffset = MinHoloPlaneOffset[ CurrentHoloPlane ];
}

VECTOR HoloRot = { 0.0F, 0.0F, 0.0F };

void PlotHoloScanLine( void )
{
	MATRIX Mat_R;
	PLANE tempplane;

	if ( CurrentHoloOffset >= MaxHoloPlaneOffset[ CurrentHoloPlane ] )
	{	
		if (++CurrentHoloPlane == NUM_HOLO_SCANPLANES)
		{
			CurrentHoloPlane = 0;
		}

		CurrentHoloOffset = MinHoloPlaneOffset[ CurrentHoloPlane ];
	}

	KillLines();


	BuildRotMatrix(0.0F, HoloRot.y, 0.0F, &Mat_R);
	tempplane = HoloPlane[ CurrentHoloPlane ];
	tempplane.Offset = CurrentHoloOffset;
	ApplyMatrix( &Mat_R, &HoloPlane[ CurrentHoloPlane ].Normal, &tempplane.Normal);

   	if (!SetUpLines( CurrentHoloModel, tempplane, &HoloRot))
		Msg("SetUpLines() returned false");

	Models[ CurrentHoloModel ].IntersectionPlane = HoloPlane[ CurrentHoloPlane ];
	
	CurrentHoloOffset += HOLOSCANSPEED * framelag;
}


void InitHoloPad( void )
{
	CurrentHoloModel = (u_int16_t)-1;
	HoloModelScale = 1.0F;
	HoloModelTimer = 0.0F;
	HoloModelMode = HOLOMODEL_Done;
	NextHoloModel = (u_int16_t)-1;
}

void KillHoloModel( void )
{

	if ( CurrentHoloModel != (u_int16_t)-1 )
		KillUsedModel( CurrentHoloModel );

	CurrentHoloModel = (u_int16_t)-1;

}			  

bool FlyGirlActive = false;

void LoadHoloModel( u_int16_t model )
{
	int i;

	if ( CurrentHoloModel != ( u_int16_t ) -1 )
	{
		KillHoloModel();
	}

	if ( model == TITLE_MODEL_ExtraModels )
	{
		FlyGirlActive = true;
		HoloModelScale = 1.0F;
		for( i = 0; i < MAXBIKETYPES; i++ )
		{
			if ( HoloModelLookup[ i ] == model )
			{
				HoloModelScale = HoloScaleLookupTable[ i ];
				break;
			}
		}
		return;
	}
	else
	{
		FlyGirlActive = false;
	}
	
	CurrentHoloModel = FindFreeModel();

	if (CurrentHoloModel != (u_int16_t) -1)
	{
		HoloModelScale = 1.0F;
		for( i = 0; i < MAXBIKETYPES; i++ )
		{
			if ( HoloModelLookup[ i ] == model )
			{
				HoloModelScale = HoloScaleLookupTable[ i ];
				break;
			}
		}
		
		Models[CurrentHoloModel].Pos = BikePos;
		Models[CurrentHoloModel].Mat = MATRIX_Identity;
		MatrixTranspose ( &Models[ CurrentHoloModel ].Mat, &Models[ CurrentHoloModel ].InvMat );
		Models[CurrentHoloModel].Group = 0;
		Models[CurrentHoloModel].Func = MODFUNC_ScaleXYZandRotate;
		Models[CurrentHoloModel].ModelNum = model;
		Models[CurrentHoloModel].Xscale = 0.0F;
		Models[CurrentHoloModel].Yscale = HoloModelScale;
		Models[CurrentHoloModel].Zscale = 0.0F;
		Models[CurrentHoloModel].Scale = 1.0F;
		Models[CurrentHoloModel].Visible = 1;
		Models[CurrentHoloModel].Frame = 0;
		Models[CurrentHoloModel].InterpFrame = 0;
		Models[CurrentHoloModel].InterpTime = 0;
		Models[CurrentHoloModel].Rot.x = 0.0F;
		Models[CurrentHoloModel].Rot.y = 0.0F;
		Models[CurrentHoloModel].Rot.z = 0.0F;
		Models[CurrentHoloModel].Flags = MODFLAG_Original;
	}
}

void ScaleHoloModel( float scale )
{
   	if( CurrentHoloModel != (u_int16_t)-1 )
	{
		Models[CurrentHoloModel].Xscale = scale * HoloModelScale;
		Models[CurrentHoloModel].Yscale = HoloModelScale;
		Models[CurrentHoloModel].Zscale = scale * HoloModelScale;
	}
}

#define HOLO_SPEECH_DELAY 60.0F
float HoloSpeechDelay;
bool HoloSpeechDone = false;

// specify (u_int16_t)-1 for no new model
void ShowHoloModel( u_int16_t model )
{
	HoloSpeechDelay = HOLO_SPEECH_DELAY;
	HoloSpeechDone = false;
	
	if ( ( CurrentHoloModel == (u_int16_t)-1 ) && !FlyGirlActive )
	{
		if ( model == (u_int16_t)-1  )
			return;
		
		LoadHoloModel( model );
		HoloModelTimer = 0.0F;
		HoloModelMode = HOLOMODEL_Expand;

	}else
	{
		KillLines();
		
		switch( HoloModelMode )
		{
		case HOLOMODEL_Done:
			HoloModelMode = HOLOMODEL_Shrink;
			break;
		case HOLOMODEL_Expand:
			HoloModelMode = HOLOMODEL_Shrink;
			break;
		case HOLOMODEL_Shrink:
			break;
		}
	}

	NextHoloModel = model;
}

float flygirl_scale = 0.0F;
VECTOR flygirloffset = { 0.0F, 20.0F, 0.0F };

void ProcessFlyGirl( float scale )
{
	MATRIX TempMatrix, TempRotMatrix, TempTransMatrix;
	VECTOR flygirlpos;

	flygirlpos = BikePos;
	flygirlpos.y += flygirloffset.y;

	TempMatrix = MATRIX_Identity;
	TempMatrix._11 *= scale * HoloModelScale;
	TempMatrix._22 *= HoloModelScale;
	TempMatrix._33 *= scale * HoloModelScale;

	BuildRotMatrix(0.0F, HoloRot.y, 0.0F, &TempRotMatrix);

	MatrixMultiply( &TempMatrix, &TempRotMatrix, &TempTransMatrix );

	UpdateFlyGirl( &flygirlpos, &TempTransMatrix, true );

}

void ProcessHoloModel( void )
{
	float oldtimer;
	bool reload = false;

	HoloRot.y -= 0.5F * framelag;
	if (HoloRot.y < 0.0F)
		HoloRot.y += 360.0F;

	HoloRot.x = 0.0F;
	HoloRot.z = 0.0F;

	if ( ( CurrentHoloModel == (u_int16_t)-1 ) && !FlyGirlActive )
	{
		return;
	}

	oldtimer = HoloModelTimer;
	
	switch( HoloModelMode )
	{
	case HOLOMODEL_Done:
		if( !FlyGirlActive )
		{
			PlotHoloScanLine();
		}
		if ( !HoloSpeechDone )
		{
			if ( HoloSpeechDelay < 0.0F )
			{
				TriggerBikeSpeech( NULL );
				HoloSpeechDone = true;
			}

			HoloSpeechDelay -= framelag;
		}
		if( FlyGirlActive )
		{
			ProcessFlyGirl((float)fabs(HoloModelTimer));
		}
		return;
	case HOLOMODEL_Expand:
		HoloModelTimer += framelag / 18.0F;
		break;
	case HOLOMODEL_Shrink:
		HoloModelTimer -= framelag / 18.0F;
		if ( HoloModelTimer < 0.0F )
		{
			HoloModelMode = HOLOMODEL_Expand;
			HoloModelTimer = (float)fabs(HoloModelTimer);
			reload = true;
		}
		break;
	}

	if ( HoloModelTimer > 1.0F )
	{
		HoloModelMode = HOLOMODEL_Done;
		HoloModelTimer = 1.0F;
	}
	
	if ( reload )
	{
		if ( NextHoloModel == (u_int16_t)-1 )
		{
			if ( FlyGirlActive )
			{
				UpdateFlyGirl( &BikePos, &MATRIX_Identity, false );
				FlyGirlActive = false;
			}else
			{
				KillHoloModel();
			}
			HoloModelMode = HOLOMODEL_Done;
			return;
		}
		else
		{
			LoadHoloModel( NextHoloModel );
		}
	}

	if( FlyGirlActive )
	{
		ProcessFlyGirl((float)fabs(HoloModelTimer));
	}else
	{
		UpdateFlyGirl( &BikePos, &MATRIX_Identity, false );
		ScaleHoloModel( (float)fabs(HoloModelTimer));
	}

	if( ( HoloModelMode == HOLOMODEL_Done ) && ( !FlyGirlActive ) )
	{
		InitHoloScanLine();
	}
}


bool DeletePilot( LIST *l, int item )
{
	static char fname[ 255 ];

	sprintf( fname, "pilots\\%s.txt", l->item[ item ] );
	if ( l->items > 1 && delete_file( fname ) && ListDelete( l, item ) )
	{
		SelectPilot( NULL );
		return true;
	}
	else
		return false;
}

void UnToggleOption( bool *toggle )
{
	MENUITEM *Item;
	bool toggled = false;

	for( Item = CurrentMenu->Item ; Item->x >= 0 ; Item++ )
	{
		if( Item->Variable )
		{
			if ( (bool *)Item->Variable == toggle )
			{
				if( !toggled )
				{
					SelectFlatMenuToggle( Item );
					toggled = true;
				}
			}
		}
	}

	if ( !toggled )
	{
		*toggle = !(*toggle);
	}
}

void ToggleBikeEngines( MENUITEM *Item )
{
	if ( !BikeEnginesOn )
	{
		// call to stop bike engines
	}
}

void InitTitleMessaging( void )
{
	int i;

	for( i = 0; i < TITLE_TEXT_MSGS_STORED; i++ )
	{
		TitleMessage[ i ][ 0 ] = 0;
	}

	TitlePlayerMessage.text[ 0 ] = 0;
}

#endif


void WatchPlayerSelected( MENUITEM *Item)
{
	WatchPlayerSelect.value = HostGamePlayersWhoIAm[HostPlayersList.selected_item];
}

void InitWatchList( MENU *Menu )
{
	BYTE i;

	// refresh the list of players so we can select one for watch mode
	HostPlayersList.items = 0;
	for(i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( GameStatus[ i ] == STATUS_Normal || i == WhoIAm )
		{
			strncpy( HostPlayersList.item[ HostPlayersList.items ], Names[ i ], sizeof( HostPlayersList.item[ 0 ] ) );
			
			if(i == WhoIAm)
				strcat( HostPlayersList.item[ HostPlayersList.items ], " (YOU)");

			HostGamePlayersWhoIAm[ HostPlayersList.items ] = i;
			HostPlayersList.items++;
		}
	}

	strncpy( HostPlayersList.item[ HostPlayersList.items ], "--OBSERVATORY--", sizeof( HostPlayersList.item[ 0 ] ) );
	HostGamePlayersWhoIAm[ HostPlayersList.items ] = MAX_PLAYERS+1;
	HostPlayersList.items++;

	HostPlayersList.display_items = MAX_PLAYERS;
	HostPlayersList.selected_item = -1;
}

void RefreshHostPlayersList( LIST *List )
{
	BYTE i;

	HostPlayersList.items = 0;
	
	for(i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( GameStatus[ i ] == STATUS_Normal && i != WhoIAm)
		{
			strncpy( HostPlayersList.item[ HostPlayersList.items ], Names[ i ], sizeof( HostPlayersList.item[ 0 ] ) );
			HostGamePlayersWhoIAm[ HostPlayersList.items ] = i;
			HostPlayersList.items++;
		}
	}
}

void InitHostMenu( MENU *Menu )
{
	HostPlayersList.display_items = 16;
	HostPlayersList.selected_item = -1;
	HostPlayersList.FuncInfo = RefreshHostPlayersList;
}

void HostListPlayerSelected( MENUITEM *Item )
{
	// this is called directly after a player is selected
	SendGameMessage(MSG_YOUQUIT, 0, (BYTE) HostGamePlayersWhoIAm[HostPlayersList.selected_item] , 0, 0);

}

void InitHostWaitingToStart( MENU *Menu )
{
	GetInitialPlayers( Menu );
}

bool GeneralTimeout( float *timer )
{
	char buf[ 64 ];

	*timer -= framelag;

	if ( *timer < 0.0F )
	{
		return true;
	}else
	{
 		sprintf( buf, "%d", (int)( *timer / 60.0F ) );
		Print4x5Text( buf, 10, 10, InGameMenuColour );
	}

	return false;
}

void TitleReset( MENUITEM *item )
{
	MenuAbort();
	SetInitialTitleParams();
	PlaceObjects();
	TitleInitDone = false;
	CameraStatus = CAMERA_AtStart;
	QuickStart = QUICKSTART_None;
}

void SetTitleNotify( char *str )
{
	strncpy( NotifyMessage, str, sizeof( NotifyMessage ) );
	QuickStart = QUICKSTART_Notify;
}

void InitInGameLevelSelect( MENU *menu )
{
	MENUITEM *item;

	for ( item = menu->Item; item->x >= 0; item++ )
	{
		if ( item->FuncDraw == DrawToggle )
				item->Variable = &IsHost;
	}
}

void InitBattleMenu( MENU *menu )
{
	MENUITEM *item;

	for ( item = menu->Item; item->x >= 0; item++ )
	{
		switch ( item->y  )
		{
		case TITLE_MODEL_Disc4:
			item->StrPnt = LT_MENU_NEW_Battle3;
			item->FrameNo = 2;
			item->Value = NULL;
			item->FuncSelect = MenuItemBack;
			break;
		case TITLE_MODEL_Disc5:
			item->StrPnt = "Blank";
			item->FrameNo = 2;
			item->Value = NULL;
			item->FuncSelect = NULL;
			break;
		}
	}
}







//
//
//
//  MENU PROCESSING
//
//
//







bool ProcessSelectKeydef( int Key )
{
	bool done;

	done = false;
	if (StackStatus == DISC_NOTHING)
	{
		switch(Key)
		{

		case UP_MOUSE:
		case SDLK_UP:
			CursorUp();
			break;
			
		case DOWN_MOUSE:
		case SDLK_DOWN:
			CursorDown();
			break;
			
		case SDLK_LEFT:
			if ( CurrentMenuItem->FuncSelect == SelectKeyDef
				|| CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
			{
				DEFKEY *kdef;
				
				kdef = (DEFKEY *)(CurrentMenuItem->Variable);
				if ( kdef->selected_key > 0 )
				{
					kdef->selected_key--;
					PlayCursorSfx();
					if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
						CheckKeysForChanges();
				}
			}else
				CursorLeft();
			break;

		case SDLK_RIGHT:
			if ( CurrentMenuItem->FuncSelect == SelectKeyDef
				|| CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
			{
				DEFKEY *kdef;
				
				kdef = (DEFKEY *)(CurrentMenuItem->Variable);
				if ( kdef->selected_key < kdef->def->keys - 1 )
				{
					kdef->selected_key++;
					PlayCursorSfx();
					if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
						CheckKeysForChanges();
				}
			}else
				CursorRight();
			break;

		case SDLK_HOME:
			CursorHome();
			break;
			
		case SDLK_END:
			CursorEnd();
			break;

		case SDLK_RETURN:
			if ( CurrentMenuItem->FuncSelect )
			{
				if ( CurrentMenuItem->FuncSelect == SelectKeyDef
					|| CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
				{
					DEFKEY *kdef;
					
					kdef = (DEFKEY *)(CurrentMenuItem->Variable);
						if ( kdef->def->keys < MAX_KEYS_PER_CONTROL )
						{
							kdef->selected_key = kdef->def->keys++;
							kdef->def->key[ kdef->selected_key ] = (VirtualKeycode) 0;
							CurrentMenuItem->FuncSelect( CurrentMenuItem );
							if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
								CheckKeysForChanges();
						}
						else
							PlaySfx( SFX_Error, 1.0F );
				}
				else
					CurrentMenuItem->FuncSelect( CurrentMenuItem );
			}
			break;

		case SDLK_INSERT:
			if ( CurrentMenuItem->FuncSelect == SelectKeyDef
				|| CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
			{
				DEFKEY *kdef;
				
				kdef = (DEFKEY *)(CurrentMenuItem->Variable);
				if ( kdef->def->keys < MAX_KEYS_PER_CONTROL )
				{
					kdef->selected_key = kdef->def->keys++;
					kdef->def->key[ kdef->selected_key ] = (VirtualKeycode) 0;
					CurrentMenuItem->FuncSelect( CurrentMenuItem );
					if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
						CheckKeysForChanges();
				}
				else
					PlaySfx( SFX_Error, 1.0F );
			}
			break;

		case SDLK_DELETE:
			if ( CurrentMenuItem->FuncSelect == SelectKeyDef
				|| CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
			{
				DEFKEY *kdef;
				
				kdef = (DEFKEY *)(CurrentMenuItem->Variable);
				KeyDelete( kdef, kdef->selected_key );
			}
			
			if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
				CheckKeysForChanges();
			
			break;
			
		case SDLK_ESCAPE:
			MenuBack();
			done = true;
			break;
			
		case SDLK_PAGEUP:
			if ( CurrentMenu == &MENU_Keyboard )
			{
				if ( CurrentPage > 0 )
				{
					PlaySfx( SFX_InGameMenuSelect, 1.0F );
					SetKeyDefPage( CurrentPage - 1 );
				}
			}
			break;
			
		case SDLK_PAGEDOWN:
			if ( CurrentMenu == &MENU_Keyboard )
			{
				if ( CurrentPage < MaxPage - 1 )
				{
					PlaySfx( SFX_InGameMenuSelect, 1.0F );
					SetKeyDefPage( CurrentPage + 1 );
				}
			}
			break;
		
		// TODO - need to pass unicode char to this function
		/*
		default:
			if ( Key )
			{
				char *c;
				
				c = (char *) key_name( Key );
				if ( c && strlen( c ) == 1 )
				{
					CursorSearch( SearchKey( *c ) );
				}
			}
			break;
		*/
		}
	}
	
	return !done;
}

bool ProcessKeydef( int Key )
{
	bool done;
	int KeyDefNo;
	MENUITEM *Item, *nextitem;

	done = false;
	switch( Key )
	{
	case SDLK_ESCAPE:
	case SDLK_F1:
	case SDLK_F2:
	case SDLK_F3:
	case SDLK_F4:
	case SDLK_F5:
	case SDLK_F6:
	case SDLK_F7:
	case SDLK_F8:
	case SDLK_F9:
	case SDLK_F10:
	case SDLK_F11:
	case SDLK_F12:
		done = true; // back out of key definition mode if illegal key selected
		break;
	default:
		if ( Key )
		{
			if ( KeyItem->Variable )
			{
				DefineKey( KeyItem, (VirtualKeycode) Key );
			}
			done = true;
		}
		break;
	}

	nextitem = CurrentMenu->Item;

	for ( KeyDefNo = 0; KeyDefNo <= CurrentKeyDef; KeyDefNo++)
	{
		for( Item = nextitem; Item->x >= 0; Item++ )
		{	
			if (Item->FrameNo == KeyDefLines[KeyDefNo])
			{
				CurrentKeyDef--;
				nextitem = Item;	
				break;
			}
		}
	}

	return !done;
}


bool ProcessSlider( int Key )
{
	bool done;
	SLIDER *slider;
	MenuItemFunc set_fn;

	slider = (SLIDER *)SliderItem->Variable;
	
	done = false;
	switch( Key )
	{
	case SDLK_LEFT:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		DecrementSlider( SliderItem );
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		IncrementSlider( SliderItem );
		break;
	case SDLK_HOME:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		SliderHome( SliderItem );
		break;
	case SDLK_END:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		SliderEnd( SliderItem );
		break;
	case SDLK_RETURN:
		done = true;
		break;
	case SDLK_ESCAPE:
		slider->oldvalue = slider->value;
		SliderCancel( SliderItem );
		done = true;
		break;
	default:
		break;
	}

	if (done)
	{
		if ( slider->FuncRefresh )
		{
			slider->FuncRefresh( slider );
		}
		if ( SliderItem->Value )
		{
			set_fn = (MenuItemFunc)(SliderItem->Value);
			set_fn ( SliderItem );
		}
	}

	return !done;
}


bool ProcessSlider2( int Key )
{
	bool done, redraw, refresh;
	SLIDER *slider;
	MenuItemFunc set_fn;

	slider = (SLIDER *)SliderItem->Variable;
	
	done = false;
	redraw = false;
	refresh = false;
	switch( Key )
	{
	case SDLK_LEFT:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		DecrementSlider( SliderItem );
		SliderSet( SliderItem );
		redraw = true;
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();
		slider->oldvalue = slider->value;
		IncrementSlider( SliderItem );
		SliderSet( SliderItem );
		redraw = true;
		break;
	case SDLK_HOME:
		CursorHome();
		done = true;
		break;
	case SDLK_END:
		CursorEnd();
		done = true;
		break;
	case UP_MOUSE:
	case SDLK_UP:
		CursorUp();
		done = true;
		break;
	case DOWN_MOUSE:
	case SDLK_DOWN:
		CursorDown();
		done = true;
		break;
	case SDLK_ESCAPE:
		MenuBack();
		done = true;
		break;
	case SDLK_RETURN:
		refresh = true;
		done = true;
		break;
	default:
		if ( Key )
		{
			char *c;
			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
			{
				CursorSearch( SearchKey( *c ) );
				if (CurrentMenuItem != SliderItem)
				{
					done = true;
				}
			}
		}
		break;
	}

	if ( ( refresh ) || ( CurrentMenuItem != SliderItem ) )
	{
		if ( slider->FuncRefresh )
		{
			slider->FuncRefresh( slider );
		}
	}


	if (redraw && ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU)))
	{
		slider->redraw_req = true;
	}

	if (done || redraw)
	{
		if ( SliderItem->Value )
		{
			set_fn = (MenuItemFunc)(SliderItem->Value);
			set_fn ( SliderItem );
		}
	}
	
	return !done;
}


/* process list movement
   lists are not the rotating disks
   but text lists ... 
   while in the pregame menu's */

bool ProcessList( int Key )
{
	bool done;
	MenuItemFunc FuncSet;
	int i;

	if (CurrentList->Static)
	{
	 	if ( Key == SDLK_ESCAPE || Key == RIGHT_MOUSE )
			CurrentList->Static = false;
		return true;
	}

	done = false;

	if ( CurrentList->selected_item < 0 )
		SelectListHome( CurrentList );

	if ( CurrentList->selected_item > CurrentList->items )
		SelectListEnd( CurrentList );

	switch( Key )
	{

	case SDLK_RIGHT:
		// if there is an item of the same height to the left, move to this item
		for (i = 0; i < CurrentList->items; i++)
		{
			if (i != CurrentList->selected_item && CurrentListItem->TextInfo[i] && CurrentListItem->TextInfo[CurrentList->selected_item])
			{
				if ((CurrentListItem->TextInfo[i]->ymin == CurrentListItem->TextInfo[CurrentList->selected_item]->ymin) &&
					(CurrentListItem->TextInfo[CurrentList->selected_item]->xmin < CurrentListItem->TextInfo[i]->xmin))
				{
					CurrentList->selected_item = i;
					PlayCursorSfx();
				}
			}
		}
		break;

	case SDLK_LEFT:
		// if there is an item of the same height to the right, move to this item
		for (i = 0; i < CurrentList->items; i++)
		{
			if (i != CurrentList->selected_item && CurrentListItem->TextInfo[i] && CurrentListItem->TextInfo[CurrentList->selected_item])
			{
				if ((CurrentListItem->TextInfo[i]->ymin == CurrentListItem->TextInfo[CurrentList->selected_item]->ymin) &&
					(CurrentListItem->TextInfo[CurrentList->selected_item]->xmin > CurrentListItem->TextInfo[i]->xmin))
				{
					CurrentList->selected_item = i;
					PlayCursorSfx();
				}
			}
		}
		break;

	case UP_MOUSE:
	case SDLK_UP:
		PlayCursorSfx();
		SelectListPrev( CurrentList );
		break;

	case DOWN_MOUSE:
	case SDLK_DOWN:
		PlayCursorSfx();
		SelectListNext( CurrentList );
		break;

	case SDLK_PAGEDOWN:
		PlayCursorSfx();
		SelectListNextPage( CurrentList );
		break;

	case SDLK_PAGEUP:
		PlayCursorSfx();
		SelectListPrevPage( CurrentList );
		break;

	case SDLK_HOME:
		PlayCursorSfx();
		SelectListHome( CurrentList );
		break;

	case SDLK_END:
		PlayCursorSfx();
		SelectListEnd( CurrentList );
		break;
	
	case LEFT_MOUSE:
	case SDLK_RETURN:
			FuncSet = (MenuItemFunc) CurrentMenuItem->Value;
			if ( FuncSet )
			{
				FuncSet( CurrentMenuItem );
			}
			done = true;
		break;

	case MIDDLE_MOUSE:
	case RIGHT_MOUSE:
	case SDLK_ESCAPE:
		CancelListSelection( CurrentList );
		done = true;
		if (CurrentMenuItem->highlightflags & TEXTFLAG_AutoSelect)
			MenuBack();
		break;

	case SDLK_DELETE:
		if ( CurrentList->FuncDelete )
		{
			if ( CurrentList->selected_item >= 0 && CurrentList->selected_item < CurrentList->items 
				&& !CurrentList->FuncDelete( CurrentList, CurrentList->selected_item ) )
			{
				PlaySfx( SFX_Error, 1.0F );
			}
		}
		break;

	default:
		if ( Key )
		{
			char *c;
			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
			{
				SelectListSearch( CurrentList, SearchKey( *c ) );
				PlayCursorSfx();
			}
		}
		break;
	}

	return !done;
}


bool ProcessText( int Key )
{
	bool done;
	TEXT *t;

	done = false;
	t = (TEXT *)(TextItem->Variable);

	switch( Key )
	{
	case SDLK_LEFT:
		TextLeft( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_RIGHT:
		TextRight( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_BACKSPACE:
		TextBackspace( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_DELETE:
#ifdef MACOSX
		TextBackspace( (TEXT *)(TextItem->Variable) );
#else
		TextDelete( (TEXT *)(TextItem->Variable) );
#endif
		break;
	case SDLK_TAB:
		TextClear( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_HOME:
		TextHome( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_END:
		TextEnd( (TEXT *)(TextItem->Variable) );
		break;
	case SDLK_RETURN:
	case UP_MOUSE:
	case DOWN_MOUSE:
	case SDLK_UP:
	case SDLK_DOWN:
		TextEnter( TextItem );
		done = true;
		break;
	case SDLK_ESCAPE:
		TextCancel( (TEXT *)(TextItem->Variable) );
		done = true;
		break;
	case SDLK_SPACE:
		TextType( (TEXT *)(TextItem->Variable), ' ' );
		break;
	default:
		if ( Key )
		{
			char * c = key_char( Key );
			if ( c && strlen(c) == 1)
				TextType( (TEXT *)(TextItem->Variable), *c );
		}
	}

	// if we are doing text editing on the VDU, reformat if any key was pressed (except return)
	if ((MenuState == MENUSTATE_Text2) && Key && !done)
	{
		GetVduTextFormattingInfo ( t );
	}

// TODO is this right ?
//      will probably need to support SDL_TextInputEvent
	if(done)
#if SDL_VERSION_ATLEAST(2,0,0)
		SDL_StopTextInput();
#else
		SDL_EnableUNICODE(0);
#endif

	return !done;
}


void ProcessSelect( int Key )
{

	/* should we process ? */
	if (StackStatus != DISC_NOTHING)
		return;
	
	/*********************\
	|
	| Key Definitions
	|
	\*********************/


	if
	(
		CurrentMenuItem->Variable &&
		(
		   ( CurrentMenuItem->FuncSelect == SelectKey ) ||
		   ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
		)
	)
	{

		/* delete, backspace, right click */
		if ( Key == SDLK_DELETE || Key == SDLK_BACKSPACE ){

			/* reset the key to nothing */
			*(VirtualKeycode *)(CurrentMenuItem->Variable) = (VirtualKeycode) 0; 
			
			/* */
			if ( CurrentMenuItem->FuncSelect == SelectFlatMenuKey )
				CheckKeysForChanges();

		}

		return; /* finish key definitions */
	}


			
	/************************\
	|
	| Selection & Navigation
	|
	\************************/



	/* cursor up */
	if ( Key == SDLK_UP || Key == UP_MOUSE )
		CursorUp();

	/* cursor down */
	else if ( Key == SDLK_DOWN || Key == DOWN_MOUSE )

		CursorDown();

	/* cursor left */
	else if ( Key == SDLK_LEFT )
	{

		/* if were acting on a selection list */

		if (
			(CurrentMenuItem->FuncSelect == SelectFlatMenuToggle)
		)
		{

				if
				(
					  (CurrentMenuItem->Variable && *(bool *)(CurrentMenuItem->Variable)) &&
					  CurrentMenuItem->FuncSelect
				)
				{
				  CurrentMenuItem->FuncSelect( CurrentMenuItem );
				  PlayCursorSfx();
				}

		}
		else
			
			CursorLeft();

	}
				
	/* cursor left */
	else if ( Key == SDLK_RIGHT  )
	{

		/* if were acting on a selection list */

		if (
			(CurrentMenuItem->FuncSelect == SelectFlatMenuToggle)
		)
		{

			if (  CurrentMenuItem->Variable &&
				  !*(bool *)(CurrentMenuItem->Variable) &&
				  CurrentMenuItem->FuncSelect )
			{
				CurrentMenuItem->FuncSelect( CurrentMenuItem );
				PlayCursorSfx();
			}

		}
		else

			CursorRight();
	
	}

	/* cursor first */
	else if ( Key == SDLK_HOME )

		CursorHome();

	/* cursor last */
	else if ( Key == SDLK_END )

		CursorEnd();

	/* go back */
	/* escape, right click */
	else if ( Key == SDLK_ESCAPE || Key == RIGHT_MOUSE )

		MenuBack();

	/* select */
	/* enter, left mouse */
	else if ( Key == SDLK_RETURN || Key == LEFT_MOUSE || Key == MIDDLE_MOUSE )
	{

		/* perform the menu's callback */
		if ( CurrentMenuItem->FuncSelect )
			CurrentMenuItem->FuncSelect( CurrentMenuItem );

	}
	else
	{

		/* move to position which starts with the letter you just hit */
		if ( Key )
		{
			char *c;
			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
				CursorSearch( SearchKey( *c ) );
		}
	}
}



bool ProcessDifficultySet ( int Key )
{
	bool done = false;

	if ( StackStatus != DISC_NOTHING )
		return !done;

	switch(Key)
	{
	case UP_MOUSE:
	case SDLK_UP:
		CursorUp();
		done = true;
		break;

	case DOWN_MOUSE:
	case SDLK_DOWN:
		CursorDown();
		done = true;
		break;

	case SDLK_LEFT:
		StackMode = DISC_MODE_ONE;
		StackStatus = DISC_EXPAND;
		DifficultyDir = ROTATE_DISC_DifficultyBack;
		DifficultyLevel--;
		if( DifficultyLevel < DIFF_Easy )
			DifficultyLevel = DIFF_Vhard;
		break;
	case SDLK_RIGHT:
		StackMode = DISC_MODE_ONE;
		StackStatus = DISC_EXPAND;
		DifficultyDir = ROTATE_DISC_DifficultyForward;
		DifficultyLevel++;
		if( DifficultyLevel > DIFF_Vhard )
			DifficultyLevel = DIFF_Easy;
		break;
		break;
	case SDLK_HOME:
		CursorHome();
		done = true;
		break;
	case SDLK_END:
		done = true;
		CursorEnd();
		break;
	case SDLK_ESCAPE:
		MenuBack();
		done = true;
		break;
	default:
		if ( Key )
		{
			char *c;

			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
			{
				CursorSearch( SearchKey( *c ) );
			}
			done = true;
		}
		break;
	}

	return !done;
}


/*===================================================================
	Procedure	:		Processes biker list, auto selects highlighted item and redraws biker char and text
	Input		:		key pressed
	Output		:		bool - indicates when biker list is no longer being used
===================================================================*/
bool ProcessBikerList ( int Key )
{
	bool done, redraw;
	int current;

	done = false;
	redraw = false;

	switch(Key)
	{
	case SDLK_LEFT:
		PlayCursorSfx();
		//make selected item 1 to the left
		current = BikeList.selected_item;
		do {
			SelectListPrev( &BikeList );
		} while ( BikeList.selected_item > 0 && !CanSelectBike[ BikeList.selected_item ] );
		if ( !CanSelectBike[ BikeList.selected_item ] )
			BikeList.selected_item = current;
		BikeList.selected_item = ValidBikeSelected( BikeList.selected_item );
		if (current != BikeList.selected_item)
		{
			ExitBikeSelection(NULL);
			redraw = true;
		}
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();

		//make selected item 1 to the right
		current = BikeList.selected_item;
		do {
			SelectListNext( &BikeList );
		} while ( BikeList.selected_item < BikeList.items - 1 && !CanSelectBike[ BikeList.selected_item ] );
		if ( !CanSelectBike[ BikeList.selected_item ] )
			BikeList.selected_item = current;
		BikeList.selected_item = ValidBikeSelected( BikeList.selected_item );
		if (current != BikeList.selected_item)
		{
			ExitBikeSelection(NULL);
			redraw = true;
		}
		break;
	case SDLK_RETURN:
		done = true;
		MenuBack();
		break;
	case SDLK_ESCAPE:
		done = true;
		MenuBack();
		break;
	}

	if (redraw)
	{
		LastMenu = CurrentMenu;	// because we are not actually changing menus, 
								// but we still want to clear this menu
		VduClear();
		OldSelectedBike = BikeList.selected_item;
		ProcessVduItems(CurrentMenu);
	}

	return !done;
}

bool ProcessSavedGameList( int Key )
{
	bool done, redraw;
	int current;

	done = false;
	redraw = false;

	switch(Key)
	{
	case SDLK_LEFT:
		PlayCursorSfx();

		current = LoadSavedGameList.selected_item;
		SelectListPrev( &LoadSavedGameList );
		if (current != LoadSavedGameList.selected_item)
		{
			redraw = true;
		}
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();

		current = LoadSavedGameList.selected_item;
		SelectListNext( &LoadSavedGameList );
		if (current != LoadSavedGameList.selected_item)
		{
			redraw = true;
		}
		break;
	case SDLK_RETURN:
		done = true;
		TitleLoadGame( NULL );
		break;
	case SDLK_ESCAPE:
		done = true;
		MenuBack();
		break;
	}

	if (redraw)
	{
		GetSavedGameData();
		LastMenu = CurrentMenu;	// because we are not actually changing menus, 
								// but we still want to clear this menu
		VduClear();
		ProcessVduItems(CurrentMenu);
	}

	return !done;
}

/*===================================================================
	Procedure	:		Processes the player list - auto selects highlighted item, 
						and draws corresponding biker character.
	Input		:		key pressed...
	Output		:		bool - indicates when player list is no longer being used.
===================================================================*/
bool ProcessPlayerList ( int Key )
{
	bool done, redraw;
	char old_bike_name[16];

	done = false;	 
	redraw = false;

	strcpy(old_bike_name, bike_name);

	switch(Key)
	{
		
	case UP_MOUSE:
	case SDLK_UP:
		//make selected item 1 up
		PlayCursorSfx();
		SelectListPrev( &PilotList );
		redraw = true;
		break;
	case DOWN_MOUSE:
	case SDLK_DOWN:
		//make selected item 1 down
		PlayCursorSfx();
		SelectListNext( &PilotList );
		redraw = true;
		break;
	case SDLK_RETURN:
		done = true;
		redraw = true;
		MenuBack();
		break;
	case SDLK_ESCAPE:
		done = true;
		redraw = true;
		MenuBack();
		break;
	case SDLK_PAGEDOWN:
		PlayCursorSfx();
		SelectListNextPage( &PilotList );
		redraw = true;
		break;
	case SDLK_PAGEUP:
		PlayCursorSfx();
		SelectListPrevPage( &PilotList );
		redraw = true;
		break;
	case SDLK_HOME:
		PlayCursorSfx();
		SelectListHome( &PilotList );
		redraw = true;
		break;
	case SDLK_END:
		PlayCursorSfx();
		SelectListEnd( &PilotList );
		redraw = true;
		break;
	case SDLK_DELETE:
		if ( PilotList.FuncDelete )
		{
			if ( PilotList.selected_item >= 0 && PilotList.selected_item < PilotList.items 
				&& !PilotList.FuncDelete( &PilotList, PilotList.selected_item ) )
			{
				PlaySfx( SFX_Error, 1.0F );
			}
			redraw = true;
		}
		break;
	default:
		if ( Key )
		{
			char *c;
			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
			{
				SelectListSearch( &PilotList, SearchKey( *c ) );
				redraw = true;
				PlayCursorSfx();
			}
		}
		break;
	}
  
	if (redraw)
	{	
		SelectPilot(CurrentMenuItem);
		if (strcmp(old_bike_name, bike_name))	// if bike name has changed....
		{

			ShowHoloModel( HoloModelLookup[ SelectedBike ] );

#if 0
			PrintTextItem(BikeNameTextPtr); 

			if ((Title_Timers[TITLE_TIMER_SwapBikes].Status != TITLE_EVENT_TIMER_ACTIVE) && (Title_Timers[TITLE_TIMER_SwapBikes].Status != TITLE_EVENT_TIMER_FINISHED))
			{
				Title_Timers[TITLE_TIMER_SwapBikes].Status = TITLE_EVENT_TIMER_ACTIVE;
 				Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime = 0.0F;
				OldSelectedBikeScale = SelectedBikeScale;
			}else
			{	if (Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime > 0.5F)
				{	for (Event = Title_Events; Event->TriggeredBy >= 0; Event++)
					{
						if (Event->TriggeredBy == TITLE_TIMER_SwapBikes)
						{
				
							if ((Event->Status == TITLE_EVENT_STATUS_ACTIVE) || (Event->Status == TITLE_EVENT_STATUS_FINISHED))
							{
								Event->Status = TITLE_EVENT_STATUS_IDLE;

								if (Event->Status == TITLE_EVENT_STATUS_ACTIVE)
								{
									if (Event->ExitFunc)
										Event->ExitFunc(Event);
								}

								Title_Timers[TITLE_TIMER_SwapBikes].Status = TITLE_EVENT_TIMER_ACTIVE;
			 					Title_Timers[TITLE_TIMER_SwapBikes].CurrentTime = 0.0F;
								OldSelectedBikeScale = BikeSwapScale;
							}

						}
					}
		
				}
			}
#endif
		}

		KillBikeChar( NULL );
		LoadBikeChar( &BikeCharItem );

//		RedrawFlatMenuList(CurrentMenuItem);
	}

	
	return !done;
}

bool ProcessWeaponOrder ( int Key )
{
	bool done, redraw;
	int highlighttype, temp, oldselectedweapon, i;

	done = false;
	redraw = false;
	
	switch(Key)
	{
	case DOWN_MOUSE:
	case SDLK_DOWN:
		PlayCursorSfx();
		oldselectedweapon = SelectedWeapon;

		if (WeaponOrderStatus == WEAPON_ChoosingSecond)
		{
			// if currently at bottom of used list...
			if (SelectedWeapon == MaxWeapons - Num_Unused_Weapons - 1)
			{
				Num_Unused_Weapons++;
				redraw = true;
			}else
			{
				// if last item...
				if (SelectedWeapon == MaxWeapons - 1)
				{
					// if no weapons in unused list...
					if (Num_Unused_Weapons == 0)
						Num_Unused_Weapons++;
						redraw = true;
				}else
				{
					// swap current weapon with one below...
					temp = WeaponList[SelectedWeapon];
					WeaponList[SelectedWeapon] = WeaponList[SelectedWeapon + 1];
					WeaponList[SelectedWeapon + 1] = temp;
					redraw = true;

					SelectedWeapon++;
				}
			}
		}else
		// choosing first weapon...
		{
			// if not last weapon...
			if (SelectedWeapon != MaxWeapons - 1)
				SelectedWeapon++;
		}

		// move cursor if necessary...
		if (oldselectedweapon != SelectedWeapon)
		{
			WeaponItem->TextInfo[oldselectedweapon]->highlight = false;
			highlighttype = WeaponItem->TextInfo[oldselectedweapon]->highlighttype;
			WeaponItem->TextInfo[SelectedWeapon]->highlight = true;
			WeaponItem->TextInfo[SelectedWeapon]->highlighttype = highlighttype;
		}
		break;
		
	case UP_MOUSE:
	case SDLK_UP:
		PlayCursorSfx();
		oldselectedweapon = SelectedWeapon;

		if (WeaponOrderStatus == WEAPON_ChoosingSecond)
		{
			// if currently at top of unused list...
			if (SelectedWeapon == MaxWeapons - Num_Unused_Weapons)
			{
				Num_Unused_Weapons--;
				redraw = true;
			}else
			{
				// if first item...
				if (SelectedWeapon == 0)
				{
					// if no weapons in used list...
					if (Num_Unused_Weapons == MaxWeapons)
						Num_Unused_Weapons--;
						redraw = true;
				}else
				{
					// swap current weapon with one above...
					temp = WeaponList[SelectedWeapon];
					WeaponList[SelectedWeapon] = WeaponList[SelectedWeapon - 1];
					WeaponList[SelectedWeapon - 1] = temp;
					redraw = true;

					SelectedWeapon--;
				}
			}
		}else
		// choosing first weapon...
		{
			// if not first weapon...
			if (SelectedWeapon != 0)
				SelectedWeapon--;
		}

		// move cursor if necessary...
		if (oldselectedweapon != SelectedWeapon)
		{
			WeaponItem->TextInfo[oldselectedweapon]->highlight = false;
			highlighttype = WeaponItem->TextInfo[oldselectedweapon]->highlighttype;
			WeaponItem->TextInfo[SelectedWeapon]->highlight = true;
			WeaponItem->TextInfo[SelectedWeapon]->highlighttype = highlighttype;
		}
		break;
	case SDLK_LEFT:
		PlayCursorSfx();
		// if currently in unused list...
		if (SelectedWeapon >= MaxWeapons - Num_Unused_Weapons)
		{
			oldselectedweapon = SelectedWeapon;

			if (WeaponOrderStatus == WEAPON_ChoosingSecond)
			{
				// if at top of unused list...
				if (SelectedWeapon == MaxWeapons - Num_Unused_Weapons)
				{
					Num_Unused_Weapons--;
					redraw = true;
					SelectedWeapon++;
					if (SelectedWeapon > MaxWeapons - 1)
						SelectedWeapon = MaxWeapons - 1;
				}else
				{
					temp = WeaponList[SelectedWeapon];
					for (i = SelectedWeapon; i > MaxWeapons - Num_Unused_Weapons; i--)
					{
						WeaponList[i] = WeaponList[i - 1];

					}
					WeaponList[i] = temp;
					Num_Unused_Weapons--;
					redraw = true;
				}
				// return to choosing first weapon...
				WeaponItem->TextInfo[SelectedWeapon]->highlighttype = HIGHLIGHT_Pulsing;
				WeaponItem->TextInfo[oldselectedweapon]->highlighttype = HIGHLIGHT_Pulsing;
				WeaponOrderStatus = WEAPON_ChoosingFirst;
			}else
			{
				// if there are items in the used list...
				if (Num_Unused_Weapons < MaxWeapons)
					SelectedWeapon = MaxWeapons - Num_Unused_Weapons - 1;
			}

			// move cursor if necessary...
			if (oldselectedweapon != SelectedWeapon)
			{
				WeaponItem->TextInfo[oldselectedweapon]->highlight = false;
				highlighttype = WeaponItem->TextInfo[oldselectedweapon]->highlighttype;
				WeaponItem->TextInfo[SelectedWeapon]->highlight = true;
				WeaponItem->TextInfo[SelectedWeapon]->highlighttype = highlighttype;
			}
		}
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();
		// if currently in used list...
		if (SelectedWeapon < MaxWeapons - Num_Unused_Weapons)
		{
			oldselectedweapon = SelectedWeapon;

			if (WeaponOrderStatus == WEAPON_ChoosingSecond)
			{
				// if at bottom of used list...
				if (SelectedWeapon == MaxWeapons - Num_Unused_Weapons - 1)
				{
					Num_Unused_Weapons++;
					redraw = true;
					SelectedWeapon--;
					if (SelectedWeapon < 0)
						SelectedWeapon = 0;
				}else
				{
					temp = WeaponList[SelectedWeapon];
					for (i = SelectedWeapon; i < MaxWeapons - 1 - Num_Unused_Weapons; i++)
					{
						WeaponList[i] = WeaponList[i + 1];

					}
					WeaponList[i] = temp;
					Num_Unused_Weapons++;
					redraw = true;
				}
				// return to choosing first weapon...
				WeaponItem->TextInfo[SelectedWeapon]->highlighttype = HIGHLIGHT_Pulsing;
				WeaponItem->TextInfo[oldselectedweapon]->highlighttype = HIGHLIGHT_Pulsing;
				WeaponOrderStatus = WEAPON_ChoosingFirst;
			}else
			{
				// if there are items in the unused list...
				if (Num_Unused_Weapons > 0)
					SelectedWeapon = MaxWeapons - Num_Unused_Weapons;
			}

			// move cursor if necessary...
			if (oldselectedweapon != SelectedWeapon)
			{
				WeaponItem->TextInfo[oldselectedweapon]->highlight = false;
				highlighttype = WeaponItem->TextInfo[oldselectedweapon]->highlighttype;
				WeaponItem->TextInfo[SelectedWeapon]->highlight = true;
				WeaponItem->TextInfo[SelectedWeapon]->highlighttype = highlighttype;
			}
		}
		break;
	case SDLK_RETURN:
		if (WeaponOrderStatus == WEAPON_ChoosingFirst)
		{
			WeaponItem->TextInfo[SelectedWeapon]->highlighttype = HIGHLIGHT_Static;
			WeaponOrderStatus = WEAPON_ChoosingSecond;
		}else
		{
			WeaponItem->TextInfo[SelectedWeapon]->highlighttype = HIGHLIGHT_Pulsing;
			WeaponOrderStatus = WEAPON_ChoosingFirst;
		}
		break;
	case SDLK_ESCAPE:
		done = true;
		MenuBack();
		break;
	}

	if (redraw)
		DrawGeneralWeaponText(WeaponItem);

	return !done;
}



bool ProcessLevelList ( int Key )
{
	bool done, redraw;
	int current;

	done = false;
	redraw = false;

	switch(Key)
	{
	case SDLK_LEFT:
		PlayCursorSfx();
		//make selected item 1 to the left
		current = LevelList.selected_item;
		SelectListPrev( &LevelList );
		if (current != LevelList.selected_item)
			redraw = true;
		break;
	case SDLK_RIGHT:
		PlayCursorSfx();
		//make selected item 1 to the right
		current = LevelList.selected_item;
		SelectListNext( &LevelList );
		if (current != LevelList.selected_item)
			redraw = true;
		break;
	case SDLK_RETURN:
		if ( MenuState == MENUSTATE_SelectLevelQuick )
		{
			if ( CurrentMenu != &MENU_NEW_CreateGame )
			{
				MenuState = MENUSTATE_SelectLevelSlow;
				MenuChange ( &SlowLevelSelectItem );
			}
		}else
		{
			done = true;
			MenuBack();
		}
		break;
	case SDLK_ESCAPE:
		done = true;
		MenuBack();
		break;
		
	case UP_MOUSE:
	case SDLK_UP:
		if (MenuState == MENUSTATE_SelectLevelQuick)
		{
			CursorUp();
			done = true; 
		}
		break;
	case DOWN_MOUSE:
	case SDLK_DOWN:
		if (MenuState == MENUSTATE_SelectLevelQuick)
		{
			CursorDown();
			done = true;
		}
		break;
	default:
		if ( Key )
		{
			char *c;

			current = LevelList.selected_item;
			c = (char *) key_name( Key );
			if ( c && strlen( c ) == 1 )
			{
				SelectListSearch( &LevelList, SearchKey( *c ) );
			}
			if (current != LevelList.selected_item)
			{
				PlayCursorSfx();
				redraw = true;
			}
		}
		break;
	}

	if( MyGameStatus != STATUS_Normal && MyGameStatus != STATUS_SinglePlayer )
		NewLevelNum = LevelList.selected_item;

	if (redraw)
	{
		if (MenuState == MENUSTATE_SelectLevelSlow)
		{
			LastMenu = CurrentMenu;	// because we are not actually changing menus, 
									// but we still want to clear this menu
			LoadLevelText( NULL );
			VduClear();
			ProcessVduItems(CurrentMenu);
		}
		LoadLevelText( NULL );
	}

	return !done;
}

void MenuItemSelect( MENUITEM * item )
{
	if(!item)
		return;
	if( ! (item->highlightflags & TEXTFLAG_AutoSelect))
		return;
	item->FuncSelect(item);
}

void MenuProcess()
{
	int i;
	int Key;

	if( CurrentMenu && CurrentMenuItem )
		JustExitedMenu = true;

	// used for flashing text
	Pulse += framelag/60.0F;
	if (Pulse > 1.0F)
		Pulse -= (float)floor((double)Pulse);

	// TODO - need to handle MENU_NEW_Error and MenuFrozen

	for( i = 0 ; i < input_buffer_count ; i++ )
	{
		Key = input_buffer[i];

		//
		CheckCheats( Key );

	    	// if last key pressed exited from menu system
		// do not process any more keys
		if ( !CurrentMenu )
			goto end;
				
		//  set global to define if we are in a title room
		switch ( MyGameStatus )
		{
			case	STATUS_StartingMultiplayer:
			case	STATUS_GetPlayerNum:
			case	STATUS_Title:
			case	STATUS_BetweenLevels:
			case	STATUS_StartingSinglePlayer:
				InTitleRoom = true;
				break;
			default:
				InTitleRoom = false;
		}

		// no menu processing done while loading...
		switch ( MyGameStatus )
		{
		  case STATUS_WaitingToStartSinglePlayer:
		  case STATUS_WaitingToStartMultiPlayerHost:
		  case STATUS_WaitingToStartMultiPlayerClient:
		  case STATUS_WaitingToStartTeamGame:
		  case STATUS_WaitingToStartDemo:
			goto end;
			break;
		}

		if(!Key)
			goto end;

		switch( Key )
		{
		case SDLK_RETURN:
			{
				if ( InTitleRoom )
				{
					// if were looking at the discs
					if ( CameraStatus == CAMERA_AtDiscs )
						PlaySfx( SFX_SelectStackOption, 1.0F );
					else
						PlaySfx( SFX_VidText, 1.0F );
				}
				else
				{
					PlaySfx( SFX_VidText, 1.0F );
				}
			}
			break;
		}

		switch ( MenuState )
		{
			case MENUSTATE_SelectKeydef:
				if ( !ProcessSelectKeydef( Key ) )
					MenuState = MENUSTATE_Select;
				break;


			case MENUSTATE_Keydef:
#ifdef KDEF
				if ( !ProcessDefKey( Key ) )
					MenuState = MENUSTATE_SelectKeydef;
#else
				if ( !ProcessKeydef( Key ) )
					MenuState = MENUSTATE_Select;
#endif
				break;


			case MENUSTATE_Keydef2:
#ifdef VDUKDEF
				if ( !ProcessDefKey( Key ) )
				{	
					MenuState = MENUSTATE_SelectKeydef;
#else
				if ( !ProcessKeydef( Key ) )
				{	
					MenuState = MENUSTATE_Select;
#endif
	//				RedrawFlatMenuKey( KeyItem);
					CheckKeysForChanges();
					CurrentMenuItem->TextInfo[0]->highlighttype = HIGHLIGHT_Pulsing;
				}
				break;


			case MENUSTATE_Slider:
				if ( !ProcessSlider( Key ) )
					MenuState = MENUSTATE_Select;
				break;


			case MENUSTATE_Slider2:
				if ( !ProcessSlider2( Key ) )
					MenuState = MENUSTATE_Select;
				break;


			case MENUSTATE_List:
				/* process text list keys */
				if ( !ProcessList( Key ) )
				{
					if ( CurrentMenuItem &&
						(!(CurrentMenuItem->highlightflags & TEXTFLAG_AutoSelect)) )
					{
						CurrentList = NULL;
						MenuState = MENUSTATE_Select;
						if (PreListMenu == CurrentMenu)
							CurrentMenuItem = PreListItem;
						CurrentListItem = NULL;
					}
				}
			break;


		    case MENUSTATE_Text:
				if ( !ProcessText( Key ) )
				{
					MenuState = MENUSTATE_Select;
					if (CurrentMenu && (CurrentMenuItem->highlightflags & TEXTFLAG_AutoSelect))	// safe to assume only menu item if autoselected
					{
						MenuBack();
					}
				}
				break;


			case MENUSTATE_Text2:
				if ( !ProcessText( Key ) )
				{
					MenuState = MENUSTATE_Select;
					CurrentMenuItem->TextInfo[0]->highlighttype = HIGHLIGHT_Pulsing;
					if (CurrentMenu && (CurrentMenuItem->highlightflags & TEXTFLAG_AutoSelect))	// safe to assume only menu item if autoselected
					{
						MenuBack();
					}
				}
				break;


			case MENUSTATE_SelectBiker:
				if ( !ProcessBikerList ( Key ) )
				{	MenuState = MENUSTATE_Select;
				}
				break;


			case MENUSTATE_SelectSavedGame:
				if ( !ProcessSavedGameList ( Key ) )
				{	MenuState = MENUSTATE_Select;
				}
				break;


			case MENUSTATE_SelectPlayer:
				if ( !ProcessPlayerList ( Key ) )
				{	MenuState = MENUSTATE_Select;
				}
				break;


			case MENUSTATE_WeaponOrder:
				if ( !ProcessWeaponOrder ( Key ) )
				{	MenuState = MENUSTATE_Select;
				}
				break;


			case MENUSTATE_SelectLevelQuick:
			case MENUSTATE_SelectLevelSlow:
				if ( !ProcessLevelList ( Key ) )
					MenuState = MENUSTATE_Select;
				break;

			case MENUSTATE_DifficultySet:
				if ( !ProcessDifficultySet ( Key ) )
					MenuState = MENUSTATE_Select;
				break;


			case MENUSTATE_Select:
				ProcessSelect( Key );

			default:
				MenuItemSelect(CurrentMenuItem);
				break;

		} // menustate switch
	} // for each buffered input

end:

	CheckMenuTimer();
	ProcessMenuFlashText();

	if( InTitleRoom )
		SetVolumeLevels();
}






//#pragma optimize( "", off )
