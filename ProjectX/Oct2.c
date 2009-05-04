
//#define REFLECTION

#define INSIDE_BSP // disable to use bounding box inside check instead
#define BSP_ONLY

#include <stdio.h>
#include "main.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "Node.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "ddsurfhand.h"
#include "getdxver.h"
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "d3dmain.h"
#include "2dtextures.h"
#include "mload.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"
#include "Ships.h"
#include "exechand.h"
#include "collision.h"
#include "ddutil.h"
#include "2dpolys.h"
#include "lines.h"
#include "polys.h"
#include "lights.h"
#include "models.h"
#include "2dtextures.h"
#include "visi.h"
#include "tload.h"
#include "sfx.h"
#include "transexe.h"
#include "Text.h"
#include "Mxaload.h"
#include "dinput.h"
#include "camera.h"
#include "title.h"
#include "screenpolys.h"
#include "controls.h"
#include "net.h"
#include "triggers.h"
#include "enemies.h"
#include "bsp.h"
#include "trigarea.h"
#include "multiplayer.h"
#include  <fcntl.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <malloc.h>
#include  <string.h>
#include  "file.h"
#include  "PolySort.h"
#include  "Ai.h"
#include  "Water.h"
#include  "spotfx.h"
#include <process.h>
#include  "ExtForce.h"
#include  "Teleport.h"
#include  "rtlight.h"
#include  "restart.h"
#include "Local.h"
#include  "goal.h"
#include  "LoadSave.h"
#include  "XMem.h"
#include "stats.h"
#include "timer.h"
#include "demo.h"
#include "file.h"
#include "singleplayer.h"

#ifdef SHADOWTEST
#include "triangles.h"
#include "shadows.h"
#endif

extern BOOL Bsp_Duplicate( BSP_HEADER *src, BSP_HEADER *dup );
extern BOOL Bsp_Identical( BSP_HEADER *b1, BSP_HEADER *b2 );
BSP_HEADER Bsp_Original;

extern int    BountyInLevel;
extern int    FlagsInLevel;
extern int    TeamFlagsInLevel[ MAX_TEAMS ];
extern int    TeamFlagsInShips[ MAX_TEAMS ];

extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;

BOOL OriginalLevels = FALSE;
BOOL CheatsDisabled = FALSE;
BOOL WaitingToQuit;

void ConfigureSpaceorbAxis( int joystick );
void DefaultJoystickSettings( USERCONFIG *u );

extern const char last_compiled[];
extern  float Countdown_Float;

extern  uint32 RecPacketSize;
extern  uint32 MaxRecPacketSize;

extern  uint32 CurrentBytesPerSecRec;
extern  uint32 CurrentBytesPerSecSent;
extern  uint32 MaxCurrentBytesPerSecRec;
extern  uint32 MaxCurrentBytesPerSecSent;


extern char *JoystickPOVDirections[];
extern VECTOR BikePos;

extern BOOL CTF;

extern uint8 QuickStart;

extern char *TeamName[ MAX_TEAMS ];
extern int16 DummyTextureIndex;

extern void Printint16( int16 num , int x , int y , int col );

extern void InitIndirectVisible( uint16 g );

extern void AddIndirectVisible( uint16 g );
extern  BOOL  RearCameraDisable;
extern  MODELNAME   SplashModelNames[MAXMODELHEADERS];
extern USERCONFIG *player_config;
extern char biker_config[];

extern MODELNAME *TitleModelSet;
extern float LevelTimeTaken;

extern BYTE PreDemoEndMyGameStatus;
extern int NumDupCompoundBuffers;
extern COMPOUND_SFX_INFO CompoundSfxBuffer[MAX_COMPOUND_BUFFERS];
extern  TEXT  DemoGameName;

extern  BOOL FullRearView;
extern  BOOL ShowStats;

extern  int AckMsgsActiveMax;
extern  int AckMsgsActive;

extern MENU MENU_Controls;
extern MENU MENU_Options;
extern MENU MENU_LoadSavedGame;
extern MENU MENU_SaveGame;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_NumberOfCrystals;
extern MENU MENU_NEW_WatchTeamSelect;

extern float VDUoffsetX;
extern float VDUoffsetY;
extern  int16 NextworkOldBikeNum;
extern  BOOL  DS;

extern  size_t  MemUsed;
extern  size_t  ExecMemUsed;

#ifdef DEBUG_ON
extern size_t SBufferMemUsedSW;
extern size_t SBufferMemUsedHW;
extern size_t MaxSBufferMemUsedHW;
extern size_t MaxSBufferMemUsedSW;
extern uint32 CurrentBikeCompSpeech;
#endif

BOOL PreventFlips = FALSE;
BOOL Cheated = FALSE;

uint16 Current_Max_Score;

void InitSoundInfo( MLOADHEADER * Mloadheader );
void InitShipSpeeds( void );

#define DEFAULT_SCREEN_ASPECT_RATIO ( 4.0F / 3.0F )

#define SAVESCREEN_3DFX

uint32        AnimOncePerFrame = 0;         // used for stuff that is displayed more than once in a single frame..
int CurrentLoadingStep = 0;

extern BOOL bSoundEnabled;

extern LPDIRECTDRAWSURFACE    Lp_AVI_DDSurface;

extern TeamCol[];
extern int Num_StatsMessage_Parts;
extern LIST BikeList;
extern LIST LevelList;
extern DWORD BufferedKey[];

extern  BOOL ResetKillsPerLevel;

extern BOOL DebugVisible;

extern int  outside_map;

extern  int16 NextNewModel;

BOOL  bPolyText = FALSE;
BOOL  PolygonText = FALSE;
extern  BOOL  PolyText[255];
extern MENUSTATE MenuState;
extern JOYSTICKINFO JoystickInfo[MAX_JOYSTICKS]; 
extern  char * LogFilename;
extern  BOOL  ShowEFZones;
extern  BOOL  ShowTeleports;
extern BOOL MenuFrozen;
extern  int16 MaxKills;
extern SPOT_SFX_LIST LoopingSfxListStart[];
extern  int16 NumGoldBars;
extern BOOL ReloadSfx;
extern float ReloadSfxPause;
extern  BOOL  IllegalTime;
extern  int   SinglePlayerTimeLimit;
extern  BOOL  BilinearSolidScrPolys;
extern  uint16  RandomStartPosModify;
extern  LONGLONG  LastPacketTime[MAX_PLAYERS+1];
extern  char * StatusTab[256];

extern BOOL ShowMessages; // show long list of chat messages
extern BOOL ShowStatistics; // show in-game statistics

BYTE  PreSynchupStatus;
char *CurrentLevelsList;

BOOL HideCursor = FALSE;

float Old_LevelTime_Float;
void InitFontTransTable( BOOL BlitText );

void InitModeCase(void);
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
void InitPolyText( void );

void DebugState( const char * str );


// Ai Modes
char * AiModes[] = {
  "AIMODE_PLAYER",          // Controlled by PLAYER
  "AIMODE_FOLLOWPATH",        // Follow node path
  "AIMODE_IDLE",          // Do nothing for a while
  "AIMODE_SCAN",          // Look around for a target
  "AIMODE_MOVETOTARGET",      // Move towards selected target   
  "AIMODE_FIRE",      // Move towards selected target   
  "AIMODE_DOGFIGHT",        // Dogfight with the selected target   
  "AIMODE_KILLMINE",        // Attack And kill a Mine....
  "AIMODE_RETREAT",         // Try and get away from PLAYER
  "AIMODE_FORMATION",       // Stay in Formation and attack whan necessary...

  "AIMODE_DEATH_CRASHNBURN",    // Fall to the ground and Shatter...
  "AIMODE_DEATH_PINGOFF",     // Ping off in the dir of the bullet that killed me spinning as I go...
  "AIMODE_DEATH_PSYCHO",      // Twist and turn and shoot and explode...
  "",
  "",
  "",
};

extern  int EnemiesActive;
void CheckTimeLimit( void );
extern  int16 InGameLoadGameLevelNum;
#ifdef REFLECTION
void WierdShit( void );
#endif

BOOL  ScoreDisplaySfx = TRUE;
BOOL  IMustQuit = FALSE;
// Capture The Flag Stuff
BOOL  CaptureTheFlag	= FALSE;
BOOL  IHaveAFlag			= FALSE;
// End of Capture The Flag

// Bounty Hunt
BOOL  BountyHunt = FALSE;

static char fname[256];
static int fnum = 0;
extern int CameraStatus;  

char *  InitViewMessages[] = {
                 "Loading... 1 of 10" ,
                 "Loading... 2 of 10" ,
                 "Loading... 3 of 10" ,
                 "Loading... 4 of 10" ,
                 "Loading... 5 of 10" ,
                 "Loading... 6 of 10" ,
                 "Loading... 7 of 10" ,
                 "Loading... 8 of 10" ,
                 "Loading... 9 of 10" ,
                 "Loading... 10 of 10" ,
                 /*
                 "Loading OffScreen Surfaces" ,
                 "Texture Load Prep         " ,
                 "Loading Textures          " ,
                 "Loading Models            " ,
                 "Loading World Mesh        " ,
                 "Loading Collision Skins   " ,
                 "Init Sound Info           " ,
                 "Init Ambient Lighting     " ,
                 "Loading Sfx               " ,
                 "Everything Else...Nodes   " ,
                 */
                 "" 
};

extern  float MaxMoveSpeed;
extern  float MoveAccell;
extern  float MoveDecell;
extern  float MaxTurboSpeed;
extern  float TurboAccell;
extern  float TurboDecell;
extern  float MaxTurnSpeed;
extern  float TurnAccell;
extern  float TurnDecell;
extern  float MaxRollSpeed;
extern  float RollAccell;
extern  float RollDecell;
extern  float MaxBankAngle;
extern  float BankAccell;
extern  float BankDecell;

extern BOOL BrightShips;

extern  float LastDistance[MAX_SFX];

BOOL  Is3Dfx;
BOOL  Is3Dfx2;

extern int GameCompleted;
extern  OFF_FILES OffsetFiles[];
extern  OFF_FILES Title_OffsetFiles[];
extern  OFF_FILES Splash_OffsetFiles[];

extern  int   DifficultyLevel;

extern int outside_group;

extern  BOOL  ScreenSaving;
extern  float WhiteOut;
extern float mouse_dx;
extern float mouse_dy;

void	MessageQuePrintAll( void );

BOOL  InsideResizeViewport = FALSE;

DWORD CurrentSrcBlend;
DWORD CurrentDestBlend;
DWORD CurrentTextureBlend;
 
LONGLONG  GameStartedTime;    // when the game started
LONGLONG  GameElapsedTime;    // Real how long the game has been going in game time not real..
LONGLONG  TempGameElapsedTime;  // Real how long the game has been going in game time not real..
LONGLONG  GameCurrentTime;    // How long the game has been going...
LONGLONG  LargeTime;
LONGLONG  LastTime;
LONGLONG  TimeDiff;
LONGLONG  Freq;
BOOL  JustExitedMenu =FALSE;

BOOL  Inside;
int   Depth;

void CalculateFramelag( void );

MENU  MENU_EditMacro1;
MENU  MENU_EditMacro2;
MENU  MENU_EditMacro3;

extern  FILE  * DemoFp;
extern  FILE  * DemoFpClean;
extern  BOOL  PlayDemo;
extern  BOOL  PauseDemo;
extern  BOOL  RecordDemo;
extern  SLIDER  DemoSpeed;
extern  SLIDER  DemoEyesSelect;
extern  BOOL  DemoScreenGrab;
extern  BOOL  ShowWeaponKills;
extern  BOOL ShowStats; 

LPDIRECT3DEXECUTEBUFFER lpD3DNormCmdBuf;
LPDIRECT3DEXECUTEBUFFER lpD3DTransCmdBuf;
LPDIRECT3DEXECUTEBUFFER lpD3DSpcFxTransCmdBuf;
BOOL InitSpecialExecBufs( void );
void ReleaseSpecialExecBufs( void );

extern  BOOL  flush_input; // tells control routines to ignore next player input

extern  uint16  IsGroupVisible[MAXGROUPS];

extern  MXLOADHEADER ModelHeaders[MAXMODELHEADERS];
extern  MODELNAME TitleModelNames[MAXMODELHEADERS]; 

extern  char  TitleNames[8][64];  

extern  ENEMY * FirstEnemyUsed;

extern  CAMERA  CurrentCamera;
extern  CAMERA  MainCamera;     // the main viewing screen...

float FPS = 0.0F;         // Frames Per Second...
double  TPS = 0.0;          // Textures Per Second...     

#define MYTIMER
#undef MYTIMER

#define LEVELSFILE      "levels.dat"

extern  MENU  *       GetPlayerNumMenu;


#define FOV_GROW(A)     ((A) + 10.0F)
#define FOV_SHRINK(A)   ((A) - 10.0F)

LONGLONG  DemoStartedTime;    // when the game started
LONGLONG  DemoEndedTime;      // when the game started
float   DemoTotalTime = 0.0F; // total game time (in seconds)
int32   DemoGameLoops = 0;
float DemoAvgFps = 0.0F;

#define MIN_VIEWPORT_WIDTH  (64)
#define MIN_VIEWPORT_HEIGHT (64)

#define VIEWPORT_GROW   (1.1F)
#define VIEWPORT_SHRINK   (1.0F/1.1F)

extern size_t	SBufferMemUsedSW;
extern size_t	SBufferMemUsedHW;
extern size_t	MaxSBufferMemUsedHW;
extern size_t	MaxSBufferMemUsedSW;

extern  TLOADHEADER Tloadheader;
extern  int16   SecondaryAmmo[ MAXSECONDARYWEAPONS ];
BOOL LockOutWindows = TRUE;

extern uint16 OnceOnly;
extern char         MyName[ 32 ];
extern  BOOL                    IsHost;
extern float  MessageTime[MAX_MESSAGES];
extern  char MessageBank[MAX_MESSAGES][200];
extern  int CurrentMessage;
extern  BYTE          MyGameStatus;
BYTE PreWaitingToSendMessagesStatus;
extern  BYTE          GameStatus[]; // Game Status for every Ship...
extern  BYTE          OverallGameStatus;      // what the host says he is doing...
extern  LPDIRECT3DEXECUTEBUFFER Skin_Execs[ MAXGROUPS ];
extern  LPDIRECT3DEXECUTEBUFFER Portal_Execs[ MAXGROUPS ];
extern  float PyroliteAmmo;
extern  float SussGunAmmo;
extern  float GeneralAmmo;
extern  SHIPCONTROL control;
extern  REGENPOINT  * RegenPoints;
extern  int16     NumRegenPoints;
extern  float PowerLevel;
extern  float LaserTemperature;
extern  float NitroFuel;
extern  SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern  ENEMY * TestEnemy;

extern  char  biker_name[256];
extern  int16 SelectedBike;
extern  BOOL  AutoDetail;
extern  LONGLONG  DemoTimeSoFar;

extern  float cral;
extern  int   HullHit;
extern  int   ShieldHit;
extern  BOOL  TargetComputerOn;

extern  int16 NumPickupsPerGroup[ MAXGROUPS ];
extern  PICKUP  Pickups[ MAXPICKUPS ];

extern  BOOL  DebugInfo;
extern  BOOL  GodMode;
extern  BOOL  GodModeOnceOnly;
extern  BOOL  EnhancedXHair;

extern  BOOL  TeamGame;
extern  BYTE  TeamNumber[MAX_PLAYERS];

extern  int   no_collision;   // disables player ship-to-background collisions
extern  SLIDER  TimeLimit;
extern  BOOL  CountDownOn;
extern  REGENPOINT  * RegenSlotsCopy[ MAX_PLAYERS ];

extern  BOOL  ShowTrigZones;
extern  BOOL  ShowColZones;
extern  BOOL  NodeCube;
extern  BOOL  OldNodeCube;
extern  MATRIX  MATRIX_Identity;

extern  uint32  TotalFmPolysInUse;
extern  uint32  TotalPolysInUse;
extern  uint32  TotalScrPolysInUse;
extern  MODELNAME ModelNames[MAXMODELHEADERS];
extern  BOOL  quitting; 
extern  int16 MakeColourMode;
extern  BOOL  ShowBoundingBoxes;

BOOL InitViewport( float scale );


BYTE  InitView_MyGameStatus;
BYTE  ChangeLevel_MyGameStatus;

LPDIRECTDRAWPALETTE ddpal;

void ProcessGameKeys( void );


float GetPlayerNumCount1 = 0.0F;
float GetPlayerNumCount2 = 0.0F;
int   GetPlayerNumCount = 0;

timer framelag_timer;

int i, j; // index counters

BOOL RearCameraActive;
int MissileCameraActive = 0;
uint16  CameraMissile = (uint16) -1;
extern BOOL MissileCameraEnable;
BYTE  TempMissileCam;

extern  REMOTECAMERA * ActiveRemoteCamera;

BOOL
Our_CalculateFrameRate(void);
BOOL VduFinished( MENU *Menu );
BOOL WriteMessage(const char *format, ...); // printf-a-like for bottom line of window

BOOL ScoreDisplay();
BOOL FreeScoreDisplay();
BOOL InitScoreDisplay();
BOOL StatsDisplay();
BOOL FreeStatsDisplay();
BOOL InitStatsDisplay();
BOOL ScoreDisplayOrig(void);

typedef enum {
	NO_BG,
	FULL_BG,
	BOX_BG
} stats_mode_t;

void ShowDetailedStats(int NumActivePlayers, BOOL TeamsGame, BOOL KillsBased, BOOL DetailedStats);
void ShowBasicStats(int NumActivePlayers);
void ShowInGameStats();
void ShowDeathModeStats();
void ShowGameStats(BOOL ShowBLTBackground);

int Secrets = 0;
int TotalSecrets = 0;
int CrystalsFound = 0;
extern  int16   NumInitEnemies;
extern  int16   NumKilledEnemies;

BOOL  ResizeViewport( float scale );
float CurrentViewportScale;
BOOL  FullScreenViewport();

BOOL InitDInput(void);
BOOL TermDInput( void );
BOOL  ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer );
BOOL  ClearZBuffer();

BOOL  RenderCurrentCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );
void FreeSfxHolder( int index ) ;

void  PlotSimplePanel( void );

RECT cursorclip;

LPDIRECT3DDEVICE lpD3Ddev = NULL;
D3DVIEWPORT viewport;
D3DVIEWPORT oldviewport;
HRESULT hresult;
int initfov = 0;
float viewplane_distance;
float hfov = START_FOV;
float chosen_fov = START_FOV;
float normal_fov = START_FOV;
float screen_aspect_ratio = DEFAULT_SCREEN_ASPECT_RATIO;
uint16 PanelHeight = 48;

BOOL  DrawPanel = FALSE;
BOOL  DrawSimplePanel = TRUE;
BOOL  ReMakeSimplePanel = TRUE;

BOOL  OldDrawPanel = TRUE;
BOOL  DrawCrosshair = TRUE;
BOOL  Panel = TRUE;

BOOL  UsedStippledAlpha = FALSE;
BOOL  CanCullFlag = FALSE;


BOOL ChangeLevel( void );
void SelectQuitCurrentGame( MENUITEM *Item );

float pixel_aspect_ratio;
float ticksperframe = 14.0F;  
float Oldframelag;  
float framelag = 0.0F; 
float real_framelag = 0.0F; 
float avgframelag = 0.0F; 
float Demoframelag = 0.5F;

extern int FontWidth;
extern int FontHeight;
extern int FontSourceWidth;
extern int FontSourceHeight;

extern  int PlayerSort[MAX_PLAYERS];
extern int16 NumOfActivePlayers;

void GeneralBltFast( int srcx, int srcy , int w , int h  , int dstx , int dsty , LPDIRECTDRAWSURFACE Surface ,  char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface);
void GeneralBlt( int srcx, int srcy , int w , int h  , int dstx , int dsty , int dstw , int dsth , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface);

void CALLBACK TimerProc( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 );

int16   LevelNum = 0 ;
int16   NewLevelNum = 0 ;
int16   NumLevels = 0;
char  ShortLevelNames[MAXLEVELS][32];
char  LevelNames[MAXLEVELS][128];
char  CollisionNames[MAXLEVELS][128];
char  CollisionZNames[MAXLEVELS][128];
char  BspNames[MAXLEVELS][128];
char  BspZNames[MAXLEVELS][128];
char  ZoneNames[MAXLEVELS][128];
char  WaterNames[MAXLEVELS][128];
char  TextNames[MAXLEVELS][128];
char  MsgNames[MAXLEVELS][128];
char  ExternalForceNames[MAXLEVELS][128];
char  TeleportNames[MAXLEVELS][128];

char MissionTextNames[MAXLEVELS][128];
char MissionTextPics[MAXLEVELS][128];
char level_names[MAXLEVELS][128];

int16 CameraRendering = CAMRENDERING_None;

BOOL SeriousError = FALSE;
BOOL DoClipping = TRUE;
BOOL OnceOnlyChangeLevel = FALSE;

LPDIRECT3DEXECUTEBUFFER RenderBufs[ 2 ] = { NULL, NULL };
void InitRenderBufs( LPDIRECT3DDEVICE lpDev );
void ReleaseRenderBufs( void );
BOOL ChangeBackgroundColour( float R, float G, float B );
BOOL SetMatrixViewPort( void );
void PrintInitViewStatus( BYTE Status );
void GetHardwareCaps( void );

void UpdateBGObjectsClipGroup( CAMERA * Camera );
void UpdateEnemiesClipGroup( CAMERA * Camera  );
void SpecialDestroyGame( void );
BOOL SaveFullScreenSnapShot( int8 * Filename );

extern  int16 PrimaryInLevel[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryInPlayers[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInPlayers[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryToGenerate[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryToGenerate[ MAXSECONDARYWEAPONS ];
extern  int16 MinesInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 OrbsInLevel;
extern  int16 OrbsToGenerate;
extern  int16 OrbsInPlayers;

// if met max kill limit, sets flag to change level
void CheckMetKillLimit();

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;

// watch mode select player (Title.c)
extern SLIDER WatchPlayerSelect;

// (Ships.c)
extern int SwitchedToWatchMode;
extern int GetName(int Player);

// (stats.c)
extern void ScoreSort();

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Weapon Names...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
char PrimaryNames[7][16] = {
            { "Pulsar" } ,
            { "Trojax" } ,
            { "Pyrolite" } ,
            { "Transpulse" } ,
            { "Suss Gun" } ,
            { "Laser" } ,
            { "" }  
};
char SecondaryNames[12][16] = {
            { "Mug" } ,
            { "Solaris" } ,
            { "Thief" } ,
            { "Scatter" } ,
            { "Gravgon" } , 
            { "MFRL" } , 
            { "Titan" } ,
            { "Purge Mine" } ,
            { "Pine Mine" } ,
            { "Quantum Mine" } ,
            { "Spider Mine" } ,
            { "" }  

};
int PrimaryLengths[7] ={ 
             sizeof("Puls")-1  ,
             sizeof("Troj")-1  ,
             sizeof("Pyro")-1  ,
             sizeof("Tran")-1  ,
             sizeof("Suss")-1  ,
             sizeof("Beam")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug ")-1 ,
             sizeof("Sola")-1 ,
             sizeof("Thei")-1 ,
             sizeof("Scat")-1 ,
             sizeof("Grav")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Tita"  )-1 ,
             sizeof("Purg")-1 ,
             sizeof("Pine")-1 ,
             sizeof("Quan")-1,
             sizeof("Spid")-1 };
#if 0
int PrimaryLengths[7] ={ 
             sizeof("Pulsar")-1  ,
             sizeof("Trojax")-1  ,
             sizeof("Pyrolite")-1  ,
             sizeof("Transpulse")-1  ,
             sizeof("Suss Gun")-1  ,
             sizeof("Beam Laser")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug")-1 ,
             sizeof("Solaris")-1 ,
             sizeof("Thief")-1 ,
             sizeof("Scatter")-1 ,
             sizeof("Gravgon")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Titan")-1 ,
             sizeof("Purge Mine")-1 ,
             sizeof("Pine Mine")-1 ,
             sizeof("Quantum Mine")-1,
             sizeof("Spider Mine")-1 };
#endif


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Panel Description Stuff...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
char  PanelNames[8][32] ={  
               { "data\\pictures\\p320X50.bmp" } ,
               { "data\\pictures\\p320X60.bmp" } ,
               { "data\\pictures\\p320X100.bmp" } ,
               { "data\\pictures\\p512X96.bmp" } ,
               { "data\\pictures\\p640X100.bmp" } ,
               { "data\\pictures\\p640X120.bmp" } ,
               { "data\\pictures\\p800X150.bmp" } ,
               { "" }
};

//  How much can we get away with not rendering... and how much do we have to blit???
int16 PanelVisibleY[8] = { 50,      //320X200
               60,      //320X240
               100,     //320X400
               96,      //512X384
               100,     //640X400
               120,     //640X480
               150 };     //800X600

int16 PanelVisibleX[8] = { 320,     //320X200
               320,     //320X240
               320,     //320X400
               512,     //512X384
               640,     //640X400
               640,     //640X480
               800 };     //800X600
              
int16 CrossHairX[10] = { 192 ,192 ,160 , 128, 96, 224, 192, 160, 128, 96 };
int16 CrossHairY[10] = {  64, 64, 64, 64, 64, 32, 32, 32, 32, 32 };

int16 PrimaryX[6] = { 64+(48*0), 64+(48*1) , 64+(48*2) , 64+(48*3) , 64+(48*0), 64+(48*1) } ;
int16 PrimaryY[6] = { 96 , 96 , 96 , 96 , 96+(32*1) , 96+(32*1) };


int16 ModesX[8] = { 320,      //320X200
            320,      //320X240
            320,      //320X400
            512,      //512X384
            640,      //640X400
            640,      //640X480
            800 };      //800X600
int16 ModesY[8] = { 200,      //320X200
            240,      //320X240
            400,      //320X400
            384,      //512X384
            400,      //640X400
            480,      //640X480
            600 };      //800X600

float ModeScaleX[8] ={ 1.0F ,     //320X200
             1.0F ,     //320X240
             1.0F ,     //320X400
             1.6F ,     //512X384
             2.0F ,     //640X400
             2.0F ,     //640X480
             2.5F  };     //800X600
    
float ModeScaleY[8] ={ 1.0F  ,      //320X200
             1.2F  ,      //320X240
             2.0F  ,      //320X400
             1.92F ,      //512X384
             2.0F  ,      //640X400
             2.4F  ,      //640X480
             3.0F   };      //800X600


char  FontNames[8][64] = {
	           { "data\\pictures\\font.bmp"		} , //320X200
               { "data\\pictures\\font.bmp"		} , //320X240
               { "data\\pictures\\font.bmp"		} , //320X400
               { "data\\pictures\\font512.bmp"	} , //512X384
               { "data\\pictures\\font512.bmp"	} , //640X400
               { "data\\pictures\\font800.bmp"	} , //640X480
               { "data\\pictures\\font800.bmp"	}	//800X600
			   };

char  DynamicFontNames[ 64 ];

char  ScoreNames[8][64] ={  
               { "data\\pictures\\S320X200.bmp" } ,
               { "data\\pictures\\S320X240.bmp" } ,
               { "data\\pictures\\S320X400.bmp" } ,
               { "data\\pictures\\S512X384.bmp" } ,
               { "data\\pictures\\S640X400.bmp" } ,
               { "data\\pictures\\S640X480.bmp" } ,
               { "data\\pictures\\S800X600.bmp" } ,
               { "" }
};

char  DynamicScoreNames[ 64 ];

char  StatsNames[8][64] ={  
               { "data\\pictures\\T320X200.bmp" } ,
               { "data\\pictures\\T320X240.bmp" } ,
               { "data\\pictures\\T320X400.bmp" } ,
               { "data\\pictures\\T512X384.bmp" } ,
               { "data\\pictures\\T640X400.bmp" } ,
               { "data\\pictures\\T640X480.bmp" } ,
               { "data\\pictures\\T800X600.bmp" } ,
               { "" }
};




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Positions of stuff to go on the panel....
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int16 ModeCase = 0;
uint16  PrimaryChanged;
uint16  SecondaryChanged;
uint16  ShieldChanged;
uint16  HullChanged;
uint16  PowerChanged;
uint16  PrimaryNumChanged;
uint16  SecondaryNumChanged;

int   PrimaryWeaponShowX;
int   PrimaryWeaponShowY;
int   PrimaryWeaponTextX;
int   PrimaryWeaponTextY;
int   SecondaryWeaponShowX;
int   SecondaryWeaponShowY;
int   SecondaryWeaponTextX;
int   SecondaryWeaponTextY;
int   PanelShieldPosX;
int   PanelShieldPosY;
int   PanelHullPosX;
int   PanelHullPosY;
int   PanelShieldTextPosX;
int   PanelShieldTextPosY;
int   PanelHullTextPosX;
int   PanelHullTextPosY;
int   PanelShieldBarPosX;
int   PanelShieldBarPosY;
int   PanelHullBarPosX;
int   PanelHullBarPosY;
int   PrimaryWeaponNumX;
int   PrimaryWeaponNumY;
int   SecondaryWeaponNumX;
int   SecondaryWeaponNumY;
int   PanelPowerPosX;
int   PanelPowerPosY;  

int   WeaponSizeX;
int   WeaponSizeY;

void FillInPanelPositions()
{
  if( ModeCase != -1 )
  {
    PrimaryChanged = (uint16) -1;
    HullChanged = (uint16) -1;
    ShieldChanged = (uint16) -1;
    PowerChanged = (uint16) -1;   
    PrimaryNumChanged = (uint16) -1;    
    SecondaryNumChanged = (uint16) -1;    
    
    WeaponSizeX = (int) ceil(36.0F * ModeScaleX[ModeCase]);
    WeaponSizeY = (int) ceil(37.0F * ModeScaleY[ModeCase]);
    PrimaryWeaponShowX = (int) floor( ( ModesX[ModeCase] * 0.5F ) - ( 149.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponShowY = (int) floor( ( 6.0F * ModeScaleY[ModeCase] ) );
    SecondaryChanged = (uint16) -1;
    SecondaryWeaponShowX = (int) floor( ( ModesX[ModeCase] * 0.5F ) + ( 113.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponShowY = (int) floor( ( 6.0F * ModeScaleY[ModeCase] ) );

    PanelShieldBarPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 80.0F * ModeScaleX[ModeCase] ) );
    PanelShieldBarPosY = (int) ( PanelVisibleY[ModeCase] - ( 16.0F * ModeScaleY[ModeCase] ) );
    PanelShieldPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 28.0F * ModeScaleX[ModeCase] ) );
    PanelShieldPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );
    PanelShieldTextPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 98.0F * ModeScaleX[ModeCase] ) );
    PanelShieldTextPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );

    PanelHullBarPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 28.0F * ModeScaleX[ModeCase] ) );
    PanelHullBarPosY = (int) ( PanelVisibleY[ModeCase] - ( 16.0F * ModeScaleY[ModeCase] ) );
    PanelHullPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 12.0F * ModeScaleX[ModeCase] ) );
    PanelHullPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );
    PanelHullTextPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 86.0F * ModeScaleX[ModeCase] ) );
    PanelHullTextPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );

    PrimaryWeaponTextX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 44.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponTextY = (int) ( PanelVisibleY[ModeCase] - ( 45.0F * ModeScaleY[ModeCase] ) );
    SecondaryWeaponTextX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 29.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponTextY = (int) ( PanelVisibleY[ModeCase] - ( 45.0F * ModeScaleY[ModeCase] ) );

    PrimaryWeaponNumX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 44.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponNumY = (int) ( PanelVisibleY[ModeCase] - ( 39.0F * ModeScaleY[ModeCase] ) );
    SecondaryWeaponNumX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 29.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponNumY = (int) ( PanelVisibleY[ModeCase] - ( 39.0F * ModeScaleY[ModeCase] ) );

    
    PanelPowerPosX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( (FontWidth*7)>>1 ) );
    PanelPowerPosY = (int) ( PanelVisibleY[ModeCase] - ( 38 *ModeScaleY[ModeCase] ) );
  
  }
}
  
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Off Screen Sufaces...Used to Blit to screen...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
LPDIRECTDRAWSURFACE     lpDDSOne;       // crosshair
LPDIRECTDRAWSURFACE     lpDDSTwo = NULL;       // Font Bitmap
LPDIRECTDRAWSURFACE     lpDDSThree;     // Panel
LPDIRECTDRAWSURFACE     lpDDSFour;     // Panel Contents

//LPDIRECTDRAWSURFACE     lpDDSOverlay;     // Panel
DDCOLORKEY ddcolorkey;

LPDIRECT3DMATERIAL lpBmat;    // a Material for the Background clearing

MLOADHEADER Mloadheader;
BOOL TestTransformClip(LPDIRECT3DVIEWPORT lpView);

MCLOADHEADER MCloadheader;          //  inner skin collision map...
MCLOADHEADER MCloadheadert0;        //  0 thickness collision map...

extern  LPDIRECT3DEXECUTEBUFFER Portal_lpD3DExBuf;

extern  uint16      FirstFmPolyUsed;
extern  uint16      FirstPolyUsed;
extern  uint16      FirstLineUsed;
extern  uint16      FirstScrPolyUsed;
extern  GLOBALSHIP    Ships[MAX_PLAYERS+1];
extern  BYTE      WhoIAm;
extern  BYTE      Current_Camera_View;    // which object is currently using the camera view....
extern  BOOL      RemoteCameraActive;

extern  int16     ShowPortal;
extern  int16     ShowSkin;
extern  int16         NamesAreLegal;
extern  SHORTNAMETYPE     Names;  // all the players short Names....

BOOL MainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

void Build_View();
BOOL  Disp3dPanel( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
BOOL DispTracker( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

/**************************************************************************
  DirectInput Globals
 **************************************************************************/
LPDIRECTINPUT                   lpdi = NULL;
LPDIRECTINPUTDEVICE             lpdiMouse = NULL;
LPDIRECTINPUTDEVICE       lpdiKeyboard = NULL;
LPDIRECTINPUTDEVICE       lpdiBufferedKeyboard = NULL;
LPDIRECTINPUTDEVICE2      lpdiJoystick[MAX_JOYSTICKS];
DIDEVCAPS           diJoystickCaps[MAX_JOYSTICKS];
int               Num_Joysticks;



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Global Variables
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/   
#define Near  ( 100.0F * GLOBAL_SCALE )
//#define Far   ( 65535.0F * GLOBAL_SCALE )
#define Far   ( 49152.0F * GLOBAL_SCALE )
#define MinNear ( 50.0F * GLOBAL_SCALE )

D3DMATRIX proj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

D3DMATRIX panelproj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

D3DMATRIX trackerproj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

MATRIX  ProjMatrix = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };


  VECTOR  SlideLeft = { -1.0F, 0.0F, 0.0F };
  VECTOR  SlideRight  = { 1.0F, 0.0F, 0.0F };
  VECTOR  SlideUp   = { 0.0F, 1.0F, 0.0F };
  VECTOR  SlideDown = { 0.0F, -1.0F, 0.0F };
  VECTOR  Forward   = { 0.0F, 0.0F, 1.0F };
  VECTOR  Backward  = { 0.0F, 0.0F,-1.0F };
  VECTOR  NullVector  = { 0.0F, 0.0F, 0.0F };

  VECTOR  Camera_Dir = { 0.0F, 0.0F,0.0F };


  D3DLIGHT light;


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
static D3DEXECUTEDATA d3dExData;
static D3DEXECUTEBUFFERDESC debDesc;

/*
 * Global projection, view, world and identity matricies
 */
D3DMATRIXHANDLE hProj;
D3DMATRIXHANDLE hView;
D3DMATRIXHANDLE hWorld;
D3DMATRIX view;
D3DMATRIX identity = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};
MATRIX Identity = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};


D3DMATRIX world = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};

BOOL ZClearsOn;
BOOL g_OddFrame = FALSE;


BOOL SetZProj( void )
{
  float Scale, NewNear;

  if( hfov > START_FOV )
  {
    Scale = ( ( MAX_FOV - hfov ) / ( MAX_FOV - START_FOV ) );
    NewNear = ( MinNear + ( ( Near - MinNear ) * Scale ) );
    proj._33 = D3DVAL(Far/(Far-NewNear));
    proj._43 = D3DVAL(-Far*NewNear/(Far-NewNear));
  }
  else
  {
    proj._33 = D3DVAL(Far/(Far-Near));
    proj._43 = D3DVAL(-Far*Near/(Far-Near));
  }

#ifdef Z_TRICK
  if( g_OddFrame )
  {
    proj._33 = ( 1.0f - ( 0.5f * proj._33 ) );
    proj._43 *= -0.5f;
  }else
  {
    proj._33 *= 0.5f;
    proj._43 *= 0.5f;
  }
  ProjMatrix._33 = proj._33;
  ProjMatrix._43 = proj._43;
  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
    return FALSE;
  }
#endif
  return TRUE;
}


SetFOV( float fov )
{
    HRESULT rval;
  float screen_width, screen_height;
  float Scale, NewNear;

  if ( fov <= 1.0F || fov >= 150.0F )
    fov = hfov;
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;
  viewplane_distance = (float) ( viewport.dwWidth / ( 2 * tan( DEG2RAD( fov ) * 0.5 ) ) );
  proj._11 = 2 * viewplane_distance / viewport.dwWidth;
  proj._22 = 2 * viewplane_distance / ( viewport.dwHeight / pixel_aspect_ratio );

  if( fov > START_FOV )
  {
    Scale = ( ( MAX_FOV - fov ) / ( MAX_FOV - START_FOV ) );
    NewNear = ( MinNear + ( ( Near - MinNear ) * Scale ) );
    proj._33 = D3DVAL(Far/(Far-NewNear));
    proj._34 = D3DVAL( 1.0 );
    proj._43 = D3DVAL(-Far*NewNear/(Far-NewNear));
    proj._44 = D3DVAL( 0.0 );
  }
  else
  {
    proj._33 = D3DVAL(Far/(Far-Near));
    proj._34 = D3DVAL( 1.0 );
    proj._43 = D3DVAL(-Far*Near/(Far-Near));
    proj._44 = D3DVAL( 0.0 );
  }

#ifdef Z_TRICK
  if ( !ZClearsOn )
  {
    if( g_OddFrame )
    {
      proj._33 = ( 1.0f - ( 0.5f * proj._33 ) );
      proj._43 *= -0.5f;
    }else
    {
      proj._33 *= 0.5f;
      proj._43 *= 0.5f;
    }
  }
#endif

  ProjMatrix._11 = proj._11;
  ProjMatrix._22 = proj._22;
  ProjMatrix._33 = proj._33;
  ProjMatrix._34 = proj._34;
  ProjMatrix._43 = proj._43;
  ProjMatrix._44 = proj._44;

  hfov = fov;
  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }
  return TRUE;
}


void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval )
{
  static char msg[1024];

  sprintf( msg, "SetViewport failed in %s.\n"
    "%s\n"
    "size=%d(=sizeof()%d?)\n"
    "xpos=%d ypos=%d\n"
    "width=%d height=%d\n"
    "xscale=%f yscale=%f\n"
    "xmax=%f ymax=%f\n"
    "zmin=%f zmax=%f\n",
    where,
    D3DAppErrorToString( rval ),
    vp->dwSize, sizeof( *vp ),
    vp->dwX, vp->dwY,
    vp->dwWidth, vp->dwHeight,
    vp->dvScaleX, vp->dvScaleY,
    vp->dvMaxX, vp->dvMaxY,
    vp->dvMinZ, vp->dvMaxZ );
  Msg( msg );
}

BOOL
ResizeViewport( float scale )
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;
  BOOL  NewDrawPanel;
  BOOL  NewDrawSimplePanel;
  
  InsideResizeViewport = TRUE;

  scale = 1.0F;

  /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;


//  DrawSimplePanel = FALSE;
  if( scale < 1.0F )
  {
    NewDrawSimplePanel = TRUE;
  }
  if( scale >= 1.0F )
  {
    NewDrawSimplePanel = FALSE;
  }
  
  if( NewDrawSimplePanel )
  {
    maxheight = d3dapp->szClient.cy - PanelVisibleY[ModeCase];
  }else{
    maxheight = d3dapp->szClient.cy;
  }
  if ( scale < 1.01F )
  {
    width = (int) floor( maxwidth * scale );
    if ( width < MIN_VIEWPORT_WIDTH )
    {
      width = MIN_VIEWPORT_WIDTH;
      scale = (float) width / maxwidth;
    }
    height = (int) floor( maxheight * scale );

    NewDrawPanel = FALSE;

    left = ( ( maxwidth - width ) >> 1 ) & ~1;
    top = ( ( maxheight - height ) >> 1 ) & ~1;
  }
  else
  {
    width = maxwidth;
    height = maxheight;
    if ( scale > 1.11F )
    {
      NewDrawPanel = TRUE;
      scale = 1.2F;
    }
    else
    {
      NewDrawPanel = FALSE;
    }
    left = 0;
    top = 0;
  }
  CurrentViewportScale = scale;

  // This disables the 3d panel...
  NewDrawPanel = DrawPanel;

    
  if( (NewDrawPanel != DrawPanel) || (NewDrawSimplePanel != DrawSimplePanel) )
  {
//    ReleaseView();
//    DrawPanel = NewDrawPanel;
//    DrawSimplePanel = NewDrawSimplePanel;
//    InitView();
  }
  
  
  viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "ResizeViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );

  // clear viewport
  D3DAppIClearBuffers();

  return TRUE;
}
BOOL
FullScreenViewport()
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;

    /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;
  maxheight = d3dapp->szClient.cy;
  width = maxwidth;
  height = maxheight;
  left = 0;
  top = 0;

    viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "FullScreenViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );
  // clear viewport
  D3DAppIClearBuffers();
  D3DAppIClearBuffers();
  return TRUE;
}

// show or hide the cursor
void ReallyShowCursor( BOOL show )
{

  // increment display count by one
  if ( show )
    while ( ShowCursor( TRUE ) < 0 )
      ; // try again

  // deincrement display count by one
  else
    while ( ShowCursor( FALSE ) >= 0 )
      ; // try again

}

void
SetInputAcquired( BOOL acquire )
{
    HRESULT         err;
	if ( acquire )
	{
		/*
												// set desired access mode -- RESET BACK TO DISCL_EXCLUSIVE -- D0 N0T M355!!!111
												err = IDirectInputDevice_SetCooperativeLevel(
													lpdiMouse,			// mouse handle
													myglobs.hWndMain,	// window handle

													DISCL_EXCLUSIVE |	// application requires exclusive access to device
																		// this cuases the mouse to disapear
																		// and be fully controlled by direct input

													DISCL_FOREGROUND);	// Application only wants mouse access when it's in the foreground
																		// automatically unacquires on window de-activate

												if(err != DI_OK)
												{
													switch(err)
													{
													case DIERR_INVALIDPARAM:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
														break;
													case DIERR_NOTINITIALIZED:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
														break;
													case E_HANDLE:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
														break;
													case E_NOTIMPL:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
														break;
													}
												}
												*/

		if ( lpdiMouse )			err = IDirectInputDevice_Acquire(lpdiMouse);
		if ( lpdiKeyboard )			err = IDirectInputDevice_Acquire(lpdiKeyboard);
		if ( lpdiBufferedKeyboard ) err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);
		DebugPrintf( "Input has been Acquired.\n" );
	}
	else
	{
/*
												// set desired access mode -- RESET BACK TO DISCL_EXCLUSIVE -- D0 N0T M355!!!111
												err = IDirectInputDevice_SetCooperativeLevel(
													lpdiMouse,			// mouse handle
													myglobs.hWndMain,	// window handle
													DISCL_NONEXCLUSIVE | // this mode does not lock the mouse down
																		// the mouse still works but is free to roam to other windows...
													DISCL_BACKGROUND);	// allows mouse to be acquired even when it's not active window

												if(err != DI_OK)
												{
													switch(err)
													{
													case DIERR_INVALIDPARAM:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
														break;
													case DIERR_NOTINITIALIZED:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
														break;
													case E_HANDLE:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
														break;
													case E_NOTIMPL:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
														break;
													}
												}

*/

		if ( lpdiMouse )			err = IDirectInputDevice_Unacquire(lpdiMouse);
		if ( lpdiKeyboard )			err = IDirectInputDevice_Unacquire(lpdiKeyboard);
		if ( lpdiBufferedKeyboard )	err = IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
		DebugPrintf( "Input has been UN-Acquired.\n" );
	}
}

BOOL cursor_clipped;
BOOL NoCursorClip = FALSE;
void SetCursorClip( BOOL clip )
{
	if ( !d3dapp )
	{
		DebugPrintf( "SetCursorClip: d3dapp is NULL\n" );
		ClipCursor( NULL );
		ReallyShowCursor( TRUE );
		return;
	}

// yea but exclusive mouse mode hides it on us without asking...
// maybe just set cusor_clipped to true when acquiring with exclusive...

	// we already are in this state...
	if ( cursor_clipped && clip ) return;
	if ( !cursor_clipped && !clip ) return;

	// the clipping area
	cursorclip.left = d3dapp->pClientOnPrimary.x + d3dapp->szClient.cx / 2;
	cursorclip.top = d3dapp->pClientOnPrimary.y + d3dapp->szClient.cy / 2;
	cursorclip.right = cursorclip.left + 1;
	cursorclip.bottom = cursorclip.top + 1;

	// save last state
	cursor_clipped = clip;
	//DebugPrintf( "SetCursorClip: cursor is %s\n", ( clip ? "clipped" : "not clipped" ));

	// this shit is just shoved into this function
	// cause it's pretty tied to the procedure...
	if ( clip )
	{
		SetInputAcquired( TRUE );	// must acquire before clipping
		ReallyShowCursor( FALSE );
	}
	else
	{
		//SetInputAcquired( FALSE );
		ReallyShowCursor( TRUE );
	}

	// don't do any clipping if cli option says not to
	// wine already clips the mouse and it causes input to go haywire
	if ( ! NoCursorClip )
	{
		if ( clip ) // clip
		{
			ClipCursor( &cursorclip );
			SetCursorPos( cursorclip.left, cursorclip.top );
		}
		else // unclip
		{
			ClipCursor( NULL );
		}
	}
	else
	{
		// logs
		if ( NoCursorClip )
			DebugPrintf("NoCursorClip.\n");
	}
}

BOOL InitLevels( char * levels_list )
{

  FILE *f;
  char levels_file[ 256 ];

  if ( !levels_list ){
    levels_list = LEVELSFILE;
  }
  
  // path to data\\levels\\<type>.bat
  sprintf( levels_file, "data\\levels\\%s", levels_list );
  
  // set some globals
  CurrentLevelsList = levels_list;
  OriginalLevels = TRUE;

  /////
  // Read Levels From File
  /////

  f = fopen( levels_file, "r" );
  if (!f )
    return FALSE;

  // scan each level name into ShortLevelNames[j]
  j=0;
  while ( j < MAXLEVELS && fscanf( f, " %s", ShortLevelNames[ j ] ) == 1 )
  {


	// get the path to the current mxv file
	sprintf( LevelNames[ j ], "data\\levels\\%s\\%s.mxv", ShortLevelNames[ j ], ShortLevelNames[ j ] );

	// check to see if the level mxv file exists
	if ( !File_Exists( LevelNames[ j ] ) )
	 continue;

	// add paths for a level to the lists
	sprintf( CollisionNames[ j ],		"data\\levels\\%s\\%s.mc",    ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( CollisionZNames[ j ],		"data\\levels\\%s\\%sz.mc",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( BspNames[ j ],				"data\\levels\\%s\\%s.bsp",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( BspZNames[ j ],			"data\\levels\\%s\\%sz.bsp",  ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( ZoneNames[ j ],			"data\\levels\\%s\\%s.zon",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( WaterNames[ j ],			"data\\levels\\%s\\%s.wat",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( TextNames[ j ],			"data\\levels\\%s\\%s.txt",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MsgNames[ j ],				"data\\levels\\%s\\%s.msg",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( ExternalForceNames[ j ],	"data\\levels\\%s\\%s.gf",    ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( TeleportNames[ j ],		"data\\levels\\%s\\%s.tel",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MissionTextNames[ j ],		"data\\levels\\%s\\%s.mis",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MissionTextPics[ j ],		"data\\levels\\%s\\%s.ppm",   ShortLevelNames[ j ], ShortLevelNames[ j ] );

	// increment counter
	j++;

  }

  NumLevels = j;
  fclose( f );

  // no levels were found
  // this should never happen
  // unless you delete all the level directories
  // or the level file doesn't exist
  if( !NumLevels ) return FALSE;

  // select default level
  LevelList.items = 0;
  LevelList.top_item = 0;
  LevelList.display_items = 8;
  LevelList.selected_item = 0;  // you can only ever start game on first level!
  NewLevelNum = 0;

  // add each level to the master list
  for (j = 0; j < NumLevels; j++)
  {
    strcpy( (char*) &LevelList.item[LevelList.items][0] , (char*) &ShortLevelNames[j][0] );
    LevelList.items++;
  }

  // Read TXT in of default level
  LoadLevelText( NULL );

  // success !
  return TRUE;

}


BOOL SetMatrixViewPort( void )
{
    size_t size;
    LPVOID lpBufStart, lpInsStart, lpPointer;
    LPDIRECT3DEXECUTEBUFFER lpD3DExCmdBuf;
    D3DMATERIAL bmat;
    D3DMATERIALHANDLE hBmat;
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;

    memset(&bmat, 0, sizeof(D3DMATERIAL));
  bmat.dwSize = sizeof(D3DMATERIAL);
  bmat.dwRampSize = 1;
  if (lpD3D->lpVtbl->CreateMaterial(lpD3D, &lpBmat, NULL) != D3D_OK) {
      return FALSE;
  }
  if (lpBmat->lpVtbl->SetMaterial(lpBmat, &bmat) != D3D_OK) {
      return FALSE;
  }
  if (lpBmat->lpVtbl->GetHandle(lpBmat, lpDev, &hBmat) != D3D_OK) {
      return FALSE;
  }
  if (lpView->lpVtbl->SetBackground(lpView, hBmat) != D3D_OK) {
      return FALSE;
  }
  
   
    ticksperframe = 14.0F;
       
  /*
   * Set the view, world and projection matrices
   * Create a buffer for matrix set commands etc.
   */
  MAKE_MATRIX(lpDev, hView, identity);
  MAKE_MATRIX(lpDev, hProj, proj);
  MAKE_MATRIX(lpDev, hWorld, world);
  world = identity;
  size = 0;
  size += sizeof(D3DINSTRUCTION) * 3;
  size += sizeof(D3DSTATE) * 5;
   
       
    if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DExCmdBuf , size ) != TRUE ) return FALSE;
   
   
  /*
   * lock it so it can be filled
   */
  if (lpD3DExCmdBuf->lpVtbl->Lock(lpD3DExCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;
  OP_STATE_TRANSFORM(3, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_PROJECTION, hProj, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_VIEW, hView, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_WORLD, hWorld, lpPointer);
  OP_STATE_LIGHT(1, lpPointer);
      STATE_DATA(D3DLIGHTSTATE_AMBIENT, RGBA_MAKE(255, 255, 255, 0), lpPointer);
  OP_EXIT(lpPointer);
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DExCmdBuf->lpVtbl->Unlock(lpD3DExCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DExCmdBuf->lpVtbl->SetExecuteData(lpD3DExCmdBuf, &d3dExData);
  lpDev->lpVtbl->BeginScene(lpDev);
  lpDev->lpVtbl->Execute(lpDev, lpD3DExCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  lpDev->lpVtbl->EndScene(lpDev);
  /*
   * We are done with the command buffer.
   */
  XRELEASE(lpD3DExCmdBuf);
   
    viewport.dwSize = sizeof(D3DVIEWPORT);
    if ((hresult = lpView->lpVtbl->GetViewport(lpView, (LPD3DVIEWPORT)&viewport)) != D3D_OK)
    {
      viewport.dwWidth = 320;
      viewport.dwHeight = 200;
    }
    lpD3Ddev = lpDev;
    if ( !initfov )
    {
      SetFOV( chosen_fov = normal_fov );
      initfov = 1;
    }

    if( InsideResizeViewport == FALSE )
    {
      InitViewport( 1.1F );
    }else{
    InsideResizeViewport = FALSE;
  }

  return TRUE;
}

struct {

  DWORD ModeMask;
  DWORD SourceMask;
  DWORD DestMask;
  DWORD Mode;
  DWORD Source;
  DWORD Dest;

} PreferredCaps[] = {

  { D3DPTBLENDCAPS_MODULATE,  D3DPBLENDCAPS_ONE,  D3DPBLENDCAPS_ONE,          // Mask
    D3DTBLEND_MODULATE,   D3DBLEND_ONE,   D3DBLEND_ONE },           // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_SRCALPHA,   // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_SRCALPHA },    // Value

  { D3DPTBLENDCAPS_ADD, D3DPBLENDCAPS_INVSRCALPHA,  D3DPBLENDCAPS_SRCALPHA,     // Mask
    D3DTBLEND_ADD,      D3DBLEND_INVSRCALPHA,     D3DBLEND_SRCALPHA },      // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_INVSRCALPHA,  // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_INVSRCALPHA },   // Value

};

struct {

  DWORD ModeMask;
  DWORD SourceMask;
  DWORD DestMask;
  DWORD Mode;
  DWORD Source;
  DWORD Dest;

} NonPrimPreferredCaps[] = {

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_SRCALPHA,   // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_SRCALPHA },    // Value

  { D3DPTBLENDCAPS_MODULATE,  D3DPBLENDCAPS_ONE,  D3DPBLENDCAPS_ONE,          // Mask
    D3DTBLEND_MODULATE,   D3DBLEND_ONE,   D3DBLEND_ONE },           // Value

  { D3DPTBLENDCAPS_ADD, D3DPBLENDCAPS_INVSRCALPHA,  D3DPBLENDCAPS_SRCALPHA,     // Mask
    D3DTBLEND_ADD,      D3DBLEND_INVSRCALPHA,     D3DBLEND_SRCALPHA },      // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_INVSRCALPHA,  // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_INVSRCALPHA },   // Value
};

void  GetHardwareCaps( void )
{   
  int16         Count;
  int16         NumPreferredCaps;
  struct  _D3DPrimCaps *  TriCapsPtr;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Check for Culling Caps
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  CanCullFlag = FALSE;
  if( ( d3dappi.ThisDriver.Desc.dpcTriCaps.dwMiscCaps & D3DPMISCCAPS_CULLCCW ) &&
    ( d3dappi.ThisDriver.Desc.dpcTriCaps.dwMiscCaps & D3DPMISCCAPS_CULLNONE ) )
    CanCullFlag = TRUE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Check for Translucency Caps
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  UsedStippledAlpha = FALSE;

  if( !Is3Dfx && !Is3Dfx2 )
  {
    TriCapsPtr = &d3dappi.ThisDriver.Desc.dpcTriCaps;
    NumPreferredCaps = ( sizeof( PreferredCaps ) / sizeof( PreferredCaps[ 0 ] ) );
    
    for( Count = 0; Count < NumPreferredCaps; Count++ )
    {
      if( ( TriCapsPtr->dwDestBlendCaps & PreferredCaps[ Count ].ModeMask ) &&
        ( TriCapsPtr->dwSrcBlendCaps & PreferredCaps[ Count ].SourceMask ) &&
        ( TriCapsPtr->dwDestBlendCaps & PreferredCaps[ Count ].DestMask ) )
      {
        CurrentTextureBlend = PreferredCaps[ Count ].Mode;
        CurrentSrcBlend = PreferredCaps[ Count ].Source;
        CurrentDestBlend = PreferredCaps[ Count ].Dest;
        break;
      }
    }
  }
  else
  {
    TriCapsPtr = &d3dappi.ThisDriver.Desc.dpcTriCaps;
    NumPreferredCaps = ( sizeof( NonPrimPreferredCaps ) / sizeof( NonPrimPreferredCaps[ 0 ] ) );
    
    for( Count = 0; Count < NumPreferredCaps; Count++ )
    {
      if( ( TriCapsPtr->dwDestBlendCaps & NonPrimPreferredCaps[ Count ].ModeMask ) &&
        ( TriCapsPtr->dwSrcBlendCaps & NonPrimPreferredCaps[ Count ].SourceMask ) &&
        ( TriCapsPtr->dwDestBlendCaps & NonPrimPreferredCaps[ Count ].DestMask ) )
      {
        CurrentTextureBlend = NonPrimPreferredCaps[ Count ].Mode;
        CurrentSrcBlend = NonPrimPreferredCaps[ Count ].Source;
        CurrentDestBlend = NonPrimPreferredCaps[ Count ].Dest;
        break;
      }
    }
  }

  if( Count == NumPreferredCaps )
  {
    UsedStippledAlpha = TRUE;
    if( d3dappi.ThisDriver.Desc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND )
      UsedStippledAlpha = FALSE;
    if( d3dappi.ThisDriver.Desc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND )
      UsedStippledAlpha = FALSE;
  }

}



float HealthCount = 0.0F;
int PowerSizes[6] = { 0 , 4 , 16 , 24 , 40 , 56 };
void TestBlt()
{
#if 0
    RECT    src;
    HRESULT ddrval;
  RECT    dest;
  DDBLTFX fx;
  int x;
  int y;
#endif
  int energy;

//  while( 1 )
  {
    if( DrawCrosshair && WhoIAm == Current_Camera_View)
    {
      if( bPolyText && PolyText[MyGameStatus])
      {
        // Add Crosshair Polygon..
        AddScreenPolyText( (uint16) 63 , (float) (viewport.dwX + (viewport.dwWidth>>1)) , (float) (viewport.dwY + (viewport.dwHeight>>1)) , 64, 255, 64, 255 );

        // Blt trojax PowerLevel / LaserTemperature
        energy = (int) ( ( PowerLevel * 0.01F ) * 9.0F );
        if( !energy )
        {
          energy = (int) ( ( LaserTemperature *0.01F ) * 9.0F );
          if( energy > 8 ) energy = 8;
        }
        if( energy )
        {
          AddScreenPolyText( (uint16) (64+8-energy) , (float) (viewport.dwX + (viewport.dwWidth>>1))-16 , (float) (viewport.dwY + (viewport.dwHeight>>1))+4 , 64, 255, 64, 255 );
        }

        if ( ( control.turbo || Ships[WhoIAm].Object.CruiseControl == CRUISE_NITRO ) && NitroFuel )
        {
          AddScreenPolyTextScale( 72, (float) ( (d3dappi.szClient.cx>>1) - (NitroFuel - 8) ), (float) (viewport.dwY + (viewport.dwHeight>>1)-7 ) ,
                      (float) ( ( ( 1.0F / 100.0F ) * ( NitroFuel * 0.5F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                      64, 255, 64, 255 );
        }

      
      }
	  else
	  {
        //  Blt Crosshair
        if( !EnhancedXHair )
        {
          GeneralBltFast( 0 , 0 , 16 , 16  , (viewport.dwX + (viewport.dwWidth>>1))-8 , (viewport.dwY + (viewport.dwHeight>>1))-8 ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
        //  Blt Nitro...
        if ( ( control.turbo || Ships[WhoIAm].Object.CruiseControl == CRUISE_NITRO ) && NitroFuel )
        {
          Printuint16( (uint16) NitroFuel , (d3dappi.szClient.cx>>1)-(1*FontWidth), (viewport.dwY + (viewport.dwHeight>>1))+8 , 2 );
          GeneralBltFast( 0 , 24 , (int)NitroFuel , 3  , (d3dappi.szClient.cx>>1)-(int)(NitroFuel*0.5F) , (viewport.dwY + (viewport.dwHeight>>1))-8 ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
        // Blt trojax PowerLevel / LaserTemperature
        energy = (int) ( ( PowerLevel * 0.01F ) * 9.0F );
        if( !energy )
        {
          energy = (int) ( ( LaserTemperature *0.01F ) * 9.0F );
          if( energy > 8 ) energy = 8;
        }
        if( energy )
        {
          GeneralBltFast( 0 , 24-energy , 4 , energy  , (viewport.dwX + (viewport.dwWidth>>1))-16 , (viewport.dwY + (viewport.dwHeight>>1))+4-energy ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
      }

      if( Ships[WhoIAm].Invul )
      {
        Print4x5Text( "I" , FontWidth , d3dappi.szClient.cy-((FontHeight*4)+8) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].InvulTimer / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*4)+8) , 2 );
      }
      if( Ships[WhoIAm].Object.Flags & SHIP_SuperNashram )
      {
        Print4x5Text( "S" , FontWidth , d3dappi.szClient.cy-((FontHeight*5)+10) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].SuperNashramTimer / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*5)+10) , 2 );
      }
      if( Ships[WhoIAm].Object.Flags & SHIP_Stealth )
      {
        Print4x5Text( "C" , FontWidth , d3dappi.szClient.cy-((FontHeight*6)+12) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].StealthTime / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*6)+12) , 2 );
      }

    }
    
    if( Panel && !PlayDemo)
    {
      if( DrawSimplePanel )
      {
        PlotSimplePanel();
      }
	  else
	  {
        if( !DrawPanel && (WhoIAm == Current_Camera_View ) ) 
        {
          // Full Screen Minimum Stats...

          if( bPolyText && PolyText[MyGameStatus])
          {
            // blt hull
            AddScreenPolyText( (uint16) 56 , (float) FontWidth , (float) (d3dappi.szClient.cy-((FontHeight*1)+2) ), 32, 255, 32, 255 );
            // blt shld
            AddScreenPolyText( (uint16) 55 , (float) FontWidth , (float) (d3dappi.szClient.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
            // Blt Primary
            AddScreenPolyText( (uint16)( Ships[WhoIAm].Primary + 38 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] ) ), (float) (d3dappi.szClient.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
            // Blt Secondary
            AddScreenPolyText( (uint16)( Ships[WhoIAm].Secondary + 44 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) ) , (float) (d3dappi.szClient.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
            // Blt Power Pods
            AddScreenPolyText( (uint16) (Ships[WhoIAm].Object.PowerLevel + 57 ), (float) (d3dappi.szClient.cx >> 1) - ( ( FontWidth * 7) >>1 ) , (float) (d3dappi.szClient.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
            // Blt Mine..
            energy = (int) GetBestMine();
            if( energy != 65535 )
            {
              AddScreenPolyText( (uint16)( energy + 44 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) ), (float) (FontHeight), 32, 255, 32, 255 );
              Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
            }


            
            // poly shld bar
            AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (d3dappi.szClient.cy-(FontHeight*2)-2) ,
                        (float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Shield *0.25F ) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                        (uint8)(63+(ShieldHit * (192/24) )), (uint8)(255-(ShieldHit * (192/24) )), 64, 255 );
            if( ShieldHit )
              ShieldHit -=1;
            
            // poly hull bar
            AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (d3dappi.szClient.cy-(FontHeight*1)-2) ,
                        (float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Hull * 0.25F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                        (uint8)(63+(HullHit * (192/24) )), (uint8)(255-(HullHit * (192/24) )), 64, 255 );
            if( HullHit )
              HullHit -=1;
            

          }
		  else
		  {
            // blt hull
            DoFontBlt( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight , FontWidth , d3dappi.szClient.cy-((FontHeight*1)+2) );
            // blt shld
            DoFontBlt( FontWidth*28 , 0 , FontWidth*4 , FontHeight , FontWidth , d3dappi.szClient.cy-((FontHeight*2)+4) );
            // Blt Primary
            DoFontBlt( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] ) , d3dappi.szClient.cy-((FontHeight*2)+4)  );
            // Blt Secondary
            DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) , d3dappi.szClient.cy-((FontHeight*1)+2)  );
            // Blt Power Pods
            DoFontBlt( FontWidth*(32-7) , FontHeight*(7+Ships[WhoIAm].Object.PowerLevel) , FontWidth*7 , FontHeight , (d3dappi.szClient.cx >> 1) - ( ( FontWidth * 7) >>1 ) , d3dappi.szClient.cy-((FontHeight*1)+2) );
            // Blt Mine..
            energy = (int) GetBestMine();
            if( energy != 65535 )
            {
              DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*energy , FontWidth*SecondaryLengths[energy] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) , FontHeight  );
              Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
            }
            // blt shld bar
            if( ShieldHit == 0 )
            {
              GeneralBltFast( 0 , 24 , (int)(Ships[WhoIAm].Object.Shield * 0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*2)+3) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, d3dapp->lpBackBuffer);
            }
			else
			{
              ShieldHit -=1;
              GeneralBltFast( 0 , 24 + ((ShieldHit>>2)*8) , (int)(Ships[WhoIAm].Object.Shield * 0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*2)+3) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }
            Printuint16( (uint16) Ships[WhoIAm].Object.Shield , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
            // blt hull bar
            if( HullHit == 0 )
            {
              GeneralBltFast( 0 , 24 ,(int) (Ships[WhoIAm].Object.Hull *0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*1)+1) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }
			else
			{
              HullHit -=1;
              GeneralBltFast( 0 , 24 + ((HullHit>>2)*8) , (int)(Ships[WhoIAm].Object.Hull *0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*1)+1) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }

          }

          // blt shield num
          Printuint16( (uint16) Ships[WhoIAm].Object.Shield , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
          // blt hull num
          if( Ships[WhoIAm].Object.Hull > 0.0F && Ships[WhoIAm].Object.Hull < 1.0F )
            Printuint16( (uint16) 1 , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
		  else
            Printuint16( (uint16) Ships[WhoIAm].Object.Hull , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
          // Blt Primary ammo
          if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
            energy =  (int) PyroliteAmmo;
		  else
		  {
            if( Ships[WhoIAm].Primary == SUSS_GUN )
              energy = (int) SussGunAmmo;
			else
              energy = (int) GeneralAmmo;
          }
          Printuint16( (uint16) energy , d3dappi.szClient.cx - ( FontWidth*5) , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
          
          // Blt Secondary ammo
          Printuint16( (uint16) GetCurSecAmmo() , d3dappi.szClient.cx - ( FontWidth*5) , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
          
        }
      }
    
      if( (NamesAreLegal != 0) || IsHost )
      {
        ScoreSort();
        PrintScoreSort();
		CheckMetKillLimit();
      }
      
	  // show long message history
	  if(ShowMessages) 
		  MessageQuePrintAll();
	  // show last 3 recent messages
	  else
		MessageQuePrint();

	  // if we are dead and waiting for a game
	  // show statistics
	  if( Ships[ WhoIAm ].Object.Mode == LIMBO_MODE )
	  {
		  ShowDeathModeStats();

	  // if we have show stats activated (ex: you pressed the stats button)
	  // show statistics
	  } else if ( ShowStatistics )
		  ShowInGameStats();

	  // watch mode
	  if(SwitchedToWatchMode)
	  {
			// show who i am watching
			CenterPrint4x5Text( (char *)GetName(WatchPlayerSelect.value), d3dapp->szClient.cy - 15, 4 );
			// display cross-hair
			GeneralBltFast( 0 , 0 , 16 , 16  , (viewport.dwX + (viewport.dwWidth>>1))-8 , (viewport.dwY + (viewport.dwHeight>>1))-8 ,
				lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
			// invulnerable
			if( Ships[WatchPlayerSelect.value].Invul )
				Print4x5Text( "Invulnerable" , FontWidth , d3dappi.szClient.cy-((FontHeight*4)+8) , 2 );
			// golden power pod
			if( Ships[WatchPlayerSelect.value].Object.Flags & SHIP_SuperNashram )
				Print4x5Text( "Golden Power Pod" , FontWidth , d3dappi.szClient.cy-((FontHeight*5)+10) , 2 );
			// stealthed
			if( Ships[WatchPlayerSelect.value].Object.Flags & SHIP_Stealth )
				Print4x5Text( "Stealth" , FontWidth , d3dappi.szClient.cy-((FontHeight*6)+12) , 2 );	  
	  }
    }
	else
	{
      if( Panel )
      {
        ScoreSort();
        PrintScoreSort();

		if(ShowMessages) 
			MessageQuePrintAll();
		else
			MessageQuePrint();

        if( DemoEyesSelect.value != MAX_PLAYERS )
          Print4x5Text( Names[DemoEyesSelect.value] ,d3dappi.szClient.cx - (FontWidth*9), FontHeight , 0 );
      }
    }
    if( Ships[WhoIAm].Object.Mode == GAMEOVER_MODE )
      CenterPrint4x5Text( "Game Over" , (d3dappi.szClient.cy >> 1) - (FontHeight*2) , 2 );
  }

}

void ReleaseLevel(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_Title:
  case  STATUS_BetweenLevels:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    if( GodMode ) LoseAllWeapons();

    ReleaseComponentedShips();
    KillThermo();
    NodeRelease();
    CameraRelease();
    ReleaseExternalForces();
    ReleaseTeleports();
    ReleaseTriggers();
    ReleaseTriggerArea();
    ReleaseRTLights();
    ReleaseGoal();

    if( CountDownOn )
      DeleteCountdownDigits();

    KillAllBGObjects();
    ReleaseAllEnemies();
    ReleaseAllRestartPoints();
    DestroySound( DESTROYSOUND_All );
    break;
  }

}



void
ReleaseScene(void)
{
  int16 Count;

  if( RegenPoints != NULL )
  {
    free( RegenPoints );
    RegenPoints = NULL;
    NumRegenPoints = 0;

    for( Count = 0; Count < MAX_PLAYERS; Count++ )
    {
      if( RegenSlotsCopy[ Count ] != NULL )
      {
        free( RegenSlotsCopy[ Count ] );
        RegenSlotsCopy[ Count ] = NULL;
      }
    }
  }

  switch( MyGameStatus )
  {
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    DestroyGame();
    break;
  }

}

void
ReleaseView(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_QuittingFromTitles:
  case  STATUS_WaitingToStartMultiPlayerClient:
  case  STATUS_WaitingToStartDemo:        // Added by DC 24/1/98

    ReleaseFlyGirl();

    FreeAllLastAFrameScrPolys();
    ReleaseTitle();
    ReleaseSpecialExecBufs();
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    ReleaseTitleModels();
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
    ReleasePolySort();
    break;

  case STATUS_ViewingScore:
#if 0 // used to release solid scr poly stuff
    ReleaseSpecialExecBufs();
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
    ReleasePolySort();
#endif
    FreeScoreDisplay();
    break;

  case STATUS_ViewingStats:
    FreeStatsDisplay();
    break;

  default:
    ReleaseSpecialExecBufs();

    Bspfree();

    
    FreeAllLastAFrameScrPolys();
    ReleaseMloadheader(&Mloadheader);
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    ReleaseModels();
    
    if ( MCloadheader.Buffer )
    {
      free( MCloadheader.Buffer );
      MCloadheader.Buffer = NULL;
    }
    if ( MCloadheadert0.Buffer )
    {
      free( MCloadheadert0.Buffer );
      MCloadheadert0.Buffer = NULL;
    }
  
    Free_All_Off_Files( &OffsetFiles[ 0 ] );
    ReleaseSkinExecs();
    ReleasePortalExecs();
    ReleaseRenderBufs();
  
    WaterRelease();
    
    FreeTxtFile();
    FreeMsgFile();

    if( lpDDSOne != NULL ) {
      ReleaseDDSurf(lpDDSOne);
      lpDDSOne = NULL;
    }
 
    if( lpDDSTwo != NULL ) {
      ReleaseDDSurf(lpDDSTwo);
      lpDDSTwo = NULL;
    }

    if( !DrawPanel && DrawSimplePanel ) {
      if( lpDDSThree != NULL ) {
        ReleaseDDSurf(lpDDSThree);
        lpDDSTwo = NULL;
      }
      if( lpDDSFour != NULL ) {
        ReleaseDDSurf(lpDDSFour);
        lpDDSTwo = NULL;
      }
    }

    RELEASE(lpBmat);

  }
}
//extern  uint16  BackgroundModel;

BOOL
InitScene(void)
{
  int i;
  //BackgroundModel = (uint16) -1;

  OnceOnlyInitModel();
  InitXLights();
  InitPrimBulls();
  InitSecBulls();
  InitFmPoly();

#ifdef SHADOWTEST
  InitTriangles();
#endif

  InitPolys();
  InitScrPolys();
  InitLines();
  SetupSpotFX();
  InitPickups();
  InitAmmoUsed();
  InitAfterBurners();
  InitBGObjects();
  InitRestartPoints();
  InitEnemies();

  for( i = 0 ; i < MAX_PLAYERS ; i++ )
  {
    Ships[i].ModelNum = (uint16) -1;
  }

  QueryPerformanceFrequency((LARGE_INTEGER *) &Freq);

  switch( MyGameStatus )
  {
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
    if (!LoadTitleModels())
    { Msg( "LoadTitleModels failed.\n" );
      return FALSE;
    }
    break;
  }
  
  return TRUE;

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init the view...This might be title or score
            or main game....
  Input   :   LPDIRECTDRAW lpDD, LPDIRECT3D lpD3D, LPDIRECT3DDEVICE lpDev,
            LPDIRECT3DVIEWPORT lpView
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

BOOL
InitView( void )
{
  LPDIRECTDRAW lpDD				= d3dapp->lpDD;
  LPDIRECT3D lpD3D				= d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev		= d3dapp->lpD3DDevice;
  LPDIRECT3DVIEWPORT lpView		= d3dapp->lpD3DViewport;
  DWORD dwItems					= INFINITE;

  DebugPrintf("InitView Starting...\n");

  CheatsDisabled = FALSE;

  // flush keyboard buffer...
  if (lpdiBufferedKeyboard)
  {
    IDirectInputDevice_GetDeviceData( lpdiBufferedKeyboard, sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0); 
  }

  if(d3dapp->CurrDriver != 0)
  {
    if( UsedStippledAlpha != TRUE ) MakeColourMode = MCM_Normal;
    else MakeColourMode = MCM_Stipple;
  }
  else
  {
    MakeColourMode = MCM_Software;
  }

  GetHardwareCaps();
  InitModeCase();

  switch( MyGameStatus )
  {
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
    if( InitTitle( lpDD, lpD3D, lpDev, lpView ) != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if ( !bSoundEnabled )
    {
      if (! InitializeSound( DESTROYSOUND_All ))
      {
        DebugPrintf("unable to initialise sound in initview\n");
        Msg("unable to initialise sound in initview\n");
        exit(1);
      }
    }

    CurrentSysTexture = 0;
        
    InitPolySort();

    InitRenderBufs(lpDev );

    InitSpecialExecBufs();
    
    if( !SetMatrixViewPort() )
    {
      SeriousError = TRUE;
      Msg( "SetMatrixViewPort() Failed\n" );
      return FALSE;
    }
    // Init the Texture Handler
    InitTload( &Tloadheader );

    if( !Load_All_Off_Files( &Title_OffsetFiles[ 0 ] ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    if( !PreLoadFlyGirl() )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    

    if( !PreInitModel( lpDev , TitleModelSet ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    //  Load in And if nescessary ReScale Textures...
    if( !Tload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    // Load all system memory textures...
    if( !SysTload( SystemMemTPages, CurrentSysTexture ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if( !InitModel( lpDev , TitleModelSet ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
      
    if ( !AllocateCompFlyGirl() )
    {
      SeriousError = TRUE;
      return FALSE;
    }else
    {
      UpdateFlyGirl( &BikePos, &MATRIX_Identity, FALSE );
    }
    

    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    //DummyTextureIndex = FindTexture( &Tloadheader, "data\\textures\\dummy.ppm" );
    DummyTextureIndex = FindTexture( &Tloadheader, "data\\textures\\dummy.bmp" );
    if ( DummyTextureIndex != -1 )
    {
      Tloadheader.PlaceHolderFile[ DummyTextureIndex ] = ( char * )malloc( sizeof( char ) * 256 );
      Tloadheader.PlaceHolder[ DummyTextureIndex ] = TRUE;
    }

    if ( !CurrentMenu )
      MenuRestart( &MENU_Start );

    break;

  case STATUS_ViewingScore:
    InitScoreDisplay();
    break;

  case STATUS_ViewingStats:
    InitStatsDisplay();
    break;
  
  default:
    InitView_MyGameStatus = MyGameStatus;
    MyGameStatus = STATUS_InitView_0;
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    D3DAppClearScreenOnly();
    CameraStatus = CAMERA_AtStart;  //prevents on screen menus from being suppressed
    break;
  }


  return TRUE;
}

BOOL FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, 
                                  LPVOID pvRef) 
{ 
   LPDIRECTINPUT pdi = pvRef; 
   LPDIRECTINPUTDEVICE pdev;
   HRESULT hr;
   char tempstr[MAX_PATH];

   // create the DirectInput joystick device 
   if (pdi->lpVtbl->CreateDevice(pdi, &pdinst->guidInstance, 
                                 &pdev, NULL) != DI_OK) 
   { 
      return DIENUM_CONTINUE; 
   } 

   strcpy (tempstr, &pdinst->tszProductName[0]);
   JoystickInfo[Num_Joysticks].Name = (char *) malloc ( strlen( tempstr ) + 1 );
   strcpy (JoystickInfo[Num_Joysticks].Name, tempstr);

   hr = pdev->lpVtbl->QueryInterface( pdev, &IID_IDirectInputDevice2,
                       (LPVOID *)&lpdiJoystick[Num_Joysticks]);
   pdev->lpVtbl->Release(pdev);

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   diJoystickCaps[ Num_Joysticks ].dwSize = sizeof( DIDEVCAPS );
   hr = IDirectInputDevice2_GetCapabilities( lpdiJoystick[ Num_Joysticks ], &diJoystickCaps[ Num_Joysticks ] );

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   Num_Joysticks++;

   if (Num_Joysticks > MAX_JOYSTICKS)
     Num_Joysticks = MAX_JOYSTICKS;

   return DIENUM_CONTINUE;
}

/* this is called for object in our joystick */
/* objects can be buttons, axis, sliders etc... */
BOOL CALLBACK DIEnumDeviceObjectsProc( 
                      LPCDIDEVICEOBJECTINSTANCE lpddoi, /* the object instance */
                      LPVOID pvRef) /* pointer to void we passed in from calling block */
{ 

  int joysticknum, axis, dir;

  /* you have to use these or you get a warning */
  GUID AxisGUID[MAX_JOYSTICK_AXIS - 1];
  GUID ButtonGUID  = GUID_Button;
  GUID POVGUID     = GUID_POV;
  GUID CompareGUID = lpddoi->guidType; /* pointer to the current object GUID type */
  
  /* pvRef is a pointer to a passed in object from our calling code */
  /* here we passed in the current joystick number */
  joysticknum = *((int *)(pvRef));

  /* types of axis's we have to support */
  AxisGUID[0] = GUID_XAxis;
  AxisGUID[1] = GUID_YAxis;
  AxisGUID[2] = GUID_ZAxis;
  AxisGUID[3] = GUID_RxAxis;
  AxisGUID[4] = GUID_RyAxis;
  AxisGUID[5] = GUID_RzAxis;
  AxisGUID[6] = GUID_Slider; // This holds an array of up to 2 sliders

  /* loop over the AxisGUID[] Array */
  /* find out if current object is one of type in AxisGUID[] */
  for (axis = 0; axis < MAX_JOYSTICK_AXIS - 1; axis++)
  {

    /* if the object is not equal to AxisGUID[axis] then continue */
    if ( ! IsEqualGuid( &AxisGUID[axis], &CompareGUID ))
      continue;

    /* allocate the memory */
    JoystickInfo[joysticknum].Axis[axis].name = (char*) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (JoystickInfo[joysticknum].Axis[axis].name, 0, (MAX_JOYNAME+1));

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy( JoystickInfo[joysticknum].Axis[axis].name, lpddoi->tszName, MAX_JOYNAME );
    else
      /* use generic name */
      _snprintf( JoystickInfo[joysticknum].Axis[axis].name, MAX_JOYNAME, "Axis %d", axis );

    /* this current axis exists */
    JoystickInfo[joysticknum].Axis[axis].exists = TRUE;

    /* up the count */
    /* NumAxis==1 means JoystickInfo[0] is set */
    JoystickInfo[joysticknum].NumAxis++;

    /* were done we found out what object we were dealing with */
    return DIENUM_CONTINUE; 

  }

  /* if the object is a button */
  if (IsEqualGuid( &ButtonGUID, &CompareGUID ))
  {

    /* allocate the memory */
    JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name = 
        (char *) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (
       JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
       0,
       (MAX_JOYNAME+1)
    );

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy(
         JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
         lpddoi->tszName,
         MAX_JOYNAME
      );
    else
      /* use generic name */
      _snprintf(
         JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
         MAX_JOYNAME,
         "Button %d",
         JoystickInfo[joysticknum].NumButtons
      );

    /* up the count */
    JoystickInfo[joysticknum].NumButtons++;

    /* were done with this object go to next one */
    return DIENUM_CONTINUE;

  }

  /* if the object is a POV aka Hat Switch */
  if (IsEqualGuid( &POVGUID, &CompareGUID ))
  {
    
    /* allocate the memory */
    JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name = 
        (char *) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (
       JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
       0,
       (MAX_JOYNAME+1)
    );

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         lpddoi->tszName,
         MAX_JOYNAME
      );
    else
      /* use generic name */
      _snprintf(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         MAX_JOYNAME,
         "Hat %d",
         JoystickInfo[joysticknum].NumPOVs
      );

    /* set the name of the direction */
    for ( dir = 0; dir < MAX_POV_DIRECTIONS; dir++ )
    {

      /* allocate the memory */
      JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ] = 
          (char *) malloc (MAX_JOYNAME+1);
  
      /* assign all /0's to the stack */
      memset (
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ],
         0,
         (MAX_JOYNAME+1)
      );

      /* use generic name */
      _snprintf(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ],
         MAX_JOYNAME,
         "%s %d",
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         JoystickInfo[joysticknum].NumPOVs
      );
    }
  
    /* up the count */
    JoystickInfo[joysticknum].NumPOVs++;

    /* were done with this object go to next */
    return DIENUM_CONTINUE; 

  }

  /* default we couldn't find out what this object is so continue */
  return DIENUM_CONTINUE; 

}


#ifdef USE_DINPUT_KEYNAMES

#define MAX_KEYS  (256)
DWORD KeyMax;
DIDEVICEOBJECTINSTANCE KeyInfo[ MAX_KEYS ];

BOOL CALLBACK DIEnumKeyboardObjectsProc( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef )
{
  DWORD key = lpddoi->dwOfs;
  if ( key < MAX_KEYS )
    KeyInfo[ key ] = *lpddoi;
  if ( KeyMax < key )
    KeyMax = key;
  return DIENUM_CONTINUE;
}


char *DI_KeyName( DWORD key )
{
  return ( key < MAX_KEYS && KeyInfo[ key ].dwSize ) ? KeyInfo[ key ].tszName : NULL;
}

#endif // USE_DINPUT_KEYNAMES


/**************************************************************************
  InitDInput
 **************************************************************************/
BOOL MouseExclusive = FALSE;
extern BOOL ActLikeWindow;
extern D3DAppInfo d3dappi;
BOOL InitDInput(void)
{
  HRESULT  err;
  GUID     guid_mouse		= GUID_SysMouse;
  GUID     guid_keyboard	= GUID_SysKeyboard;
  DIPROPDWORD dipdw =
        {
            {
                sizeof(DIPROPDWORD),        // diph.dwSize
                sizeof(DIPROPHEADER),       // diph.dwHeaderSize
                0,                          // diph.dwObj
                DIPH_DEVICE,                // diph.dwHow
            },
            DINPUT_BUFFERSIZE,              // dwData
        };
  LPDIRECTINPUTDEVICE     tempJoystick = NULL;
  LPVOID joysticknumptr;
  int i, j, k;
  BOOL failjoystick;

    err = DirectInputCreateA(myglobs.hInstApp, DIRECTINPUT_VERSION, &lpdi, NULL);

    if(err != DI_OK)
    {
            return FALSE;
    }


    // Create a mouse.
    err = IDirectInput_CreateDevice(lpdi, &guid_mouse, &lpdiMouse, NULL);
  if ( err != DI_OK )
  {
    goto fail;
  }

    // Tell DirectInput that we want to receive data in mouse format
    err = IDirectInputDevice_SetDataFormat(lpdiMouse, &c_dfDIMouse);

    if(err != DI_OK)
    {
            goto fail;
    }

	DebugPrintf("Setting mouse mode: %s\n",MouseExclusive?"Exclusive":"Non Exclusive");

	if ( MouseExclusive )
	{
		err = IDirectInputDevice_SetCooperativeLevel(
			lpdiMouse,			// mouse handle
			myglobs.hWndMain,	// window handle
			DISCL_EXCLUSIVE |	// application requires exclusive access to device
								// this cuases the mouse to disapear
								// and be fully controlled by direct input
			DISCL_FOREGROUND);	// Application only wants mouse access when it's in the foreground
								// automatically unacquires on window de-activate

		// doesn't work as it should...

		// if acting like a window or not fullscreen
		if ( ActLikeWindow || !d3dappi.bFullscreen )
		{
			SetInputAcquired( FALSE );
			SetCursorClip( FALSE );
		}

	}
	else
	{
		err = IDirectInputDevice_SetCooperativeLevel(
			lpdiMouse,			// mouse handle
			myglobs.hWndMain,	// window handle
			DISCL_NONEXCLUSIVE |// this mode does not lock the mouse down
								// the mouse still works but is free to roam to other windows...
			DISCL_BACKGROUND);	// allows mouse to be acquired even when it's not active window
	}

    if(err != DI_OK)
    {
		switch(err)
		{
		case DIERR_INVALIDPARAM:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
			break;
		case DIERR_NOTINITIALIZED:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
			break;
		case E_HANDLE:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
			break;
		case E_NOTIMPL:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
			break;
		}
        goto fail;
    }

	// this is the moment the mouse disapears when usnig exclusive access.
	// we would most likely just want a global state flag defining the type of mouse state we want.
	// and then make sure the mouse is properly set in the right state.
	err = IDirectInputDevice_Acquire(lpdiMouse);

    // try to create keyboard device
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiKeyboard, myglobs.hWndMain,
                     DISCL_EXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

#ifdef USE_DINPUT_KEYNAMES
  // get key info
  KeyMax = 0;
  memset( KeyInfo, 0, sizeof( KeyInfo ) );
  if (IDirectInputDevice_EnumObjects(lpdiKeyboard, DIEnumKeyboardObjectsProc, NULL, DIDFT_ALL ) != DI_OK)
  {
    // don't care -- fall back on internal key name info
  }
#endif // USE_DINPUT_KEYNAMES

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiKeyboard);

    if(err != DI_OK)
    {
//            goto fail;
    }

    // try to create another keyboard device (which will be buffered, for menu screens)
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiBufferedKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiBufferedKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiBufferedKeyboard, myglobs.hWndMain,
                     DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

    // set the buffer size...
  err = lpdiBufferedKeyboard->lpVtbl->SetProperty(lpdiBufferedKeyboard, DIPROP_BUFFERSIZE, &dipdw.diph);

    if(err != DI_OK)
    {
            goto fail;
    }

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);

    if(err != DI_OK)
    {
 //           goto fail;
    }

  // try to create Joystick devices
  for ( i = 0; i < MAX_JOYSTICKS; i++ )
    lpdiJoystick[i] = NULL;
  Num_Joysticks = 0;

  lpdi->lpVtbl->EnumDevices(lpdi, DIDEVTYPE_JOYSTICK, 
                         InitJoystickInput, lpdi, DIEDFL_ATTACHEDONLY); 

  failjoystick = FALSE;
  for (i = 0; i < Num_Joysticks; i++)
  {
    JoystickInfo[i].assigned = FALSE;
    JoystickInfo[i].connected = TRUE;
    JoystickInfo[i].NumAxis = 0;
    JoystickInfo[i].NumButtons = 0;
    JoystickInfo[i].NumPOVs = 0;

    joysticknumptr = (LPVOID)&i;
    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].exists = FALSE;
    }
    lpdiJoystick[i]->lpVtbl->EnumObjects(lpdiJoystick[i], DIEnumDeviceObjectsProc, 
                     joysticknumptr, DIDFT_ALL); 

    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].action = SHIPACTION_Nothing;
      JoystickInfo[i].Axis[j].inverted = FALSE;
      JoystickInfo[i].Axis[j].deadzone = 20;
      JoystickInfo[i].Axis[j].fine = TRUE;
    }

    for (j = 0; j < JoystickInfo[i].NumButtons; j++)
      JoystickInfo[i].Button[j].action = SHIPACTION_Nothing;

    for (j = 0; j < JoystickInfo[i].NumPOVs; j++)
    {
      for (k = 0; k < MAX_POV_DIRECTIONS; k++)
        JoystickInfo[i].POV[j].action[k] = SHIPACTION_Nothing;
    }

    // Tell DirectInput that we want to receive data in joystick format
    if (IDirectInputDevice2_SetDataFormat(lpdiJoystick[i], &c_dfDIJoystick2) == DI_OK)
    {                          
      // set cooperative level
      if(IDirectInputDevice2_SetCooperativeLevel(lpdiJoystick[i], myglobs.hWndMain,
                 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == DI_OK)
      {
        // try to acquire the Joystick
        err = IDirectInputDevice2_Acquire(lpdiJoystick[i]);
        if (err != DI_OK)
        {
          failjoystick = TRUE;
        }
      }else
      {
        failjoystick = TRUE;
      }
    }else
    {
      failjoystick = TRUE;
    }

    
    if (failjoystick)
    {
      failjoystick = FALSE;
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
#if NOW_DONE_INSIDE_DEFAULTJOYSTICKSETTINGS
    SetUpJoystickAxis(i);

     // if spaceorb...
    if ( !_stricmp( JoystickInfo[i].Name, "Spacetec SpaceOrb 360" ) )
    {
      DebugPrintf("Spaceorb detected - auto configuring axis...\n");
      ConfigureSpaceorbAxis( i );
    }
#endif
  }

  DebugPrintf( "InitDInput: %d joysticks connected\n", Num_Joysticks );

  // if we get here, all DirectInput objects were created ok
  return TRUE;

fail:
    if (lpdiMouse)  IDirectInputDevice_Release(lpdiMouse), lpdiMouse  = NULL;
    if (lpdiKeyboard)  IDirectInputDevice_Release(lpdiKeyboard), lpdiKeyboard  = NULL;
    if (lpdiBufferedKeyboard)  IDirectInputDevice_Release(lpdiBufferedKeyboard), lpdiBufferedKeyboard  = NULL;
  for (i = 0; i < Num_Joysticks; i++)
  {
      if (lpdiJoystick[i])
    {
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
  }
    if (lpdi)   IDirectInputDevice_Release(lpdi), lpdi     = NULL;
    return FALSE;               
}

BOOL TermDInput( void )
{
    int i;

  if (lpdiMouse)
  {
    IDirectInputDevice_Unacquire(lpdiMouse);
    IDirectInputDevice_Release(lpdiMouse);
    lpdiMouse  = NULL;
  }
    if (lpdiKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiKeyboard);
    IDirectInputDevice_Release(lpdiKeyboard);
    lpdiKeyboard  = NULL;
  }
    if (lpdiBufferedKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
    IDirectInputDevice_Release(lpdiBufferedKeyboard);
    lpdiBufferedKeyboard  = NULL;
  }

  for (i = 0; i < Num_Joysticks; i++)
  {
      if (lpdiJoystick[i])
    {
      IDirectInputDevice2_Unacquire(lpdiJoystick[i]);
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i]  = NULL;
    }
  }

    if (lpdi)
  {
    IDirectInputDevice_Release(lpdi);
    lpdi     = NULL;
  }

  return TRUE;
}

// read KEYS.txt and DEUBG_KEYS.txt before changing.
void ProcessGameKeys( void )
{

  static int16 startpos = 0;

  // if were not (!) in (normal,single,attract or demo) modes
  // then don't listen to keys...
  if ( ! (
           ( MyGameStatus == STATUS_Normal       ) ||
           ( MyGameStatus == STATUS_SinglePlayer ) ||
           ( MyGameStatus == STATUS_PlayingDemo  )
     )   )
  {
    return;
  }

  // ESCAPE
  if ( IsKeyPressed( DIK_ESCAPE ) )
    // if were not currently in a menu
    //   ignore keystroke if we just exited the menu
    //   stops going back itno menu from hitting to many times
    if ( !CurrentMenu && !JustExitedMenu )
    {
      // if were playing a demo
      if( PlayDemo )
        // show the demo menu
        MenuRestart( &MENU_DemoPlaying );
      // if were playing the game
      else
	  {
			// show the multi-player menu
			if(GameStatus[WhoIAm] != STATUS_PlayingDemo && GameStatus[WhoIAm] != STATUS_SinglePlayer)
				 MenuRestart( &MENU_InGame );
			// show the single-player menu
			else
				 MenuRestart( &MENU_InGameSingle );
	  }

    // if were not in the menu
    // and the above check failed
    // reset variable so 3rd escape takes us back in
	}else{
      JustExitedMenu = FALSE;
	}


  // debuggin keys
  if ( DebugInfo ) {

    // Shift Modifier
    if ( IsKeyHeld( DIK_LSHIFT ) || IsKeyHeld( DIK_RSHIFT ) )
    {

      // Shift + F1
      if ( IsKeyPressed( DIK_F1 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_DebugHelp );
        else if ( CurrentMenu == &MENU_DebugHelp )
          MenuExit();

      // Shift + Ctrl Modifier
      if ( IsKeyHeld( DIK_LCONTROL ) || IsKeyHeld( DIK_RCONTROL ) )
      {

        // Shift + Ctrl + F2
        if ( IsKeyPressed( DIK_F2 ) )
          SetFOV( chosen_fov = normal_fov );
        
        // Shift + Ctrl + F3
        if ( IsKeyPressed( DIK_F3 ) )
          SetFOV( chosen_fov = normal_fov );

      } // end shift + ctrl modifier
      else // shift only modifier
      {

        // Shift + F2
        if ( IsKeyPressed( DIK_F2 ) )
          SetFOV( chosen_fov = FOV_GROW( hfov ) );

        // Shift + F3
        if ( IsKeyPressed( DIK_F3 ) )
          SetFOV( chosen_fov = FOV_SHRINK( hfov ) );

      } // end shift only modifier

      // Shift + F4
      if ( IsKeyPressed( DIK_F4 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_GotoRoom );

      // Shift + F5
      if ( IsKeyPressed( DIK_F5 ) )
      {
        InitShipStartPos( WhoIAm, ++startpos );
        AddColourMessageToQue(SystemMessageColour,
          "You are now in %s\n",
          Mloadheader.Group[ Ships[ WhoIAm ].Object.Group ].name
        );
      }

      // Shift + F6
      if ( IsKeyPressed( DIK_F6 ) )
        DoClipping = !DoClipping;

      // Shift + F7
      if ( IsKeyPressed( DIK_F7 ) )
        ShowBoundingBoxes = !ShowBoundingBoxes;

      // Shift + F8
      if ( IsKeyPressed( DIK_F8 ) )
        ShowEFZones = !ShowEFZones;

      // Shift + F9
      if ( IsKeyPressed( DIK_F9 ) )
        if( ShowPortal == 4 )
          ShowPortal = 0;
        else
          ShowPortal++;

      // Shift + F10
      if ( IsKeyPressed( DIK_F10 ) )
        ShowColZones = !ShowColZones;

#ifdef POLYGONAL_COLLISIONS

      // Shift + F11
      if ( IsKeyPressed( DIK_F11 ) )
        if( ShowSkin == 6 )
          ShowSkin = 0;
        else
          ShowSkin++;

#endif

      // Shift + F12
      if( IsKeyPressed( DIK_F12 ) )
        no_collision = !no_collision;

    } // end shift modifier
    // CTRL Modifiers
    else if ( IsKeyHeld( DIK_LCONTROL ) || IsKeyHeld( DIK_RCONTROL ) )
    {

		// Ctrl + F1
		if ( IsKeyPressed( DIK_F1 ) )
			ShowTeleports = !ShowTeleports;

		// Ctrl + F2
		if ( IsKeyPressed( DIK_F2 ) )
			ShowTrigZones = !ShowTrigZones;

#ifndef POLYGONAL_COLLISIONS
#ifdef REMOTE_CAMERA_ENABLED
		// Ctrl + F3
		if ( IsKeyPressed( DIK_F3 ) )
			if( !RemoteCameraActive )
			{
				RemoteCameraActive = TRUE;
				Ships[MAX_PLAYERS].Object.Pos = Ships[WhoIAm].Object.Pos;
				Ships[MAX_PLAYERS].Object.Group = Ships[WhoIAm].Object.Group;
				Ships[MAX_PLAYERS].Object.Mode = NORMAL_MODE;
				Ships[MAX_PLAYERS].enable = 1;
				Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
			}else{
				RemoteCameraActive = FALSE;
				Ships[MAX_PLAYERS].enable = 0;
				Current_Camera_View = WhoIAm;   // which object is currently using the camera view....
			}
#endif
#endif

    } // end ctrl modifier
    else // no modifiers
    {

		// F8
		if ( IsKeyPressed( DIK_F8 ) )
			// Awesome !!!
			// We need to make this a standard feature!!!
			if( TargetComputerOn )
				TargetComputerOn = FALSE;
			else
				TargetComputerOn = TRUE;

    }

  } // end debug keys
  else // non debugging mode keys
  {

    // F1
    if ( IsKeyPressed( DIK_F1 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Controls );
      else if ( CurrentMenu == &MENU_Controls )
        MenuExit();

    // F2
    if ( IsKeyPressed( DIK_F2 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Options );
      else if ( CurrentMenu == &MENU_Options )
        MenuExit();

    // single player
    if ( MyGameStatus == STATUS_SinglePlayer )
    {

      // F3
      if ( IsKeyPressed( DIK_F3 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_LoadSavedGame );
        else if ( CurrentMenu == &MENU_LoadSavedGame )
          MenuExit();
  
      // F4        
      if ( IsKeyPressed( DIK_F4 ) )
        // quick save
        if ( !CurrentMenu )
          MenuRestart( &MENU_SaveGame );
        else if ( CurrentMenu == &MENU_SaveGame )
          MenuExit();

    }

	// multiplayer
	else
	{
		// F3
		// F4
	}

    // F5
    if ( IsKeyPressed( DIK_F5 ) )
      MissileCameraEnable = !MissileCameraEnable;

	// F6
	if ( IsKeyPressed( DIK_F6 ) )
		RearCameraActive = !RearCameraActive;

	// F7
	if ( IsKeyPressed( DIK_F7 ) )
		Panel = !Panel;

#ifdef SAVESCREEN_3DFX
    // capture a screen shot
    if ( IsKeyPressed( DIK_F8 ) && ScreenSaving )
    {
      sprintf( fname, ".\\ScreenShots\\screen%d.ppm", fnum++ );
      // wtf is this ?
      // _spawnl( _P_WAIT, "grab.exe", "grab.exe", "-fp" , "-w 512" , "-h 384" , fname, NULL );
      folder_exists( SNAPSHOT_FOLDER );
      SaveFullScreenSnapShot( fname );
    }
#endif

    // single player mode
    if( MyGameStatus == STATUS_SinglePlayer )
    {

      // F9
      if ( IsKeyPressed( DIK_F9 ) )
        LastTextMessage();

      // F10
      if ( IsKeyPressed( DIK_F10 ) )
        NextTextMessage();

    } // end single player mode
    else // not single player mode
    {

      // Shift Modifier
      if ( ( IsKeyHeld( DIK_LSHIFT ) || IsKeyHeld( DIK_RSHIFT ) ) )
      {

        // Shift + F9
        if ( IsKeyPressed( DIK_F9 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro1 );
          else if ( CurrentMenu == &MENU_EditMacro1 )
            MenuExit();

        // Shift + F10
        if ( IsKeyPressed( DIK_F10 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro2 );
          else if ( CurrentMenu == &MENU_EditMacro2 )
            MenuExit();

        // Shift + F11
        if ( IsKeyPressed( DIK_F11 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro3 );
          else if ( CurrentMenu == &MENU_EditMacro3 )
            MenuExit();

      } // Shift Modifier
      else // no modifier
      {

        // F9
        if ( IsKeyPressed( DIK_F9 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt1, 0);

        // F10
        if ( IsKeyPressed( DIK_F10 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt2, 0);

        // F11
        if ( IsKeyPressed( DIK_F11 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt3, 0);

      } // end (not) shift modifier
	} // end not single player
  } // end normal keys
} // ProcessGameKeys


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Set the View Matrix
  Input   :   nothing...  Current_Camera_View must be set
                  to a legal ship number...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

void Build_View()
{
  VECTOR  Trans;
  VECTOR  Trans2;

  
  Trans.x = -CurrentCamera.Pos.x;
  Trans.y = -CurrentCamera.Pos.y;
  Trans.z = -CurrentCamera.Pos.z;

  ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );      /* Set Translation */

  view._11 = CurrentCamera.Mat._11; 
  view._12 = CurrentCamera.Mat._12;
  view._13 = CurrentCamera.Mat._13;
  view._14 = CurrentCamera.Mat._14;
             
  view._21 = CurrentCamera.Mat._21;
  view._22 = CurrentCamera.Mat._22;
  view._23 = CurrentCamera.Mat._23;
  view._24 = CurrentCamera.Mat._24;
             
  view._31 = CurrentCamera.Mat._31;
  view._32 = CurrentCamera.Mat._32;
  view._33 = CurrentCamera.Mat._33;
  view._34 = CurrentCamera.Mat._34;
                                        
  view._41 = Trans2.x;
  view._42 = Trans2.y;
  view._43 = Trans2.z;
  view._44 = CurrentCamera.Mat._44;
}


char TriggerName[256];

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Inits And Loads New Level..
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ChangeLevel( void )
{
	//if( NewLevelNum != LevelNum )
	//  return TRUE;

	LevelNum = NewLevelNum;

	//NumGoldBars = 0;

	ReleaseLevel();
	KillNodeCubeLines();
	KillBoxLines();

	// $$$
	NextworkOldBikeNum = -1;

	ActiveRemoteCamera = NULL;
	MissileCameraActive = 0;
	CameraMissile = (uint16) -1;

	InitScene();
	InitTextMessages();
	InitMultiSfxHandle();

	OnceOnlyChangeLevel = TRUE;
	ChangeLevel_MyGameStatus = MyGameStatus;
	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &TriggerName[ 0 ], ".TRG" );
	if( !Triggerload( TriggerName ) )
	{
	  SeriousError = TRUE;
	  return( FALSE );
	}

	MyGameStatus = STATUS_ChangeLevelPostInitView;
	InitView();

	return( TRUE );
}

// draw the loading bar that fills from left to right
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps )
{
  HRESULT hr;
  DDBLTFX fx;
  RECT    dest, darkgreen, lightgreen;
  float xmin, xmax, ymin, ymax, loaded, one_step;
  int total_loading_steps = 11;
  int BarXMin = 20;
  int BarXMax = 180;
  int BarYMin = 155;
  int BarYMax = 160;
  float BorderX, BorderY;

  // if not in mission loading screen ( buffer flips disabled ), do not display loading status
  if ( !PreventFlips )
    return;

//  BorderX = (float)floor(ModeScaleX[ModeCase]);
//  BorderY = (float)floor(ModeScaleY[ModeCase]);
  BorderX = 1.0F;
  BorderY = 1.0F;

  if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
  {
    //coords for high res font...
    darkgreen.left = 10;
    darkgreen.right = 11;
    darkgreen.top = 194;
    darkgreen.bottom = 195;

    lightgreen.left = 50;
    lightgreen.right = 51;
    lightgreen.top = 194;
    lightgreen.bottom = 195;

  }else
  {
    //coords for low res font...
    darkgreen.left = 2;
    darkgreen.right = 3;
    darkgreen.top = 122;
    darkgreen.bottom = 123;

    lightgreen.left = 10;
    lightgreen.right = 11;
    lightgreen.top = 122;
    lightgreen.bottom = 123;
  }


  if ( !BorderX)
    BorderX = 1.0F;
  if ( !BorderY)
    BorderY = 1.0F;

  memset( &fx, 0, sizeof( DDBLTFX ) );
  fx.dwSize = sizeof( DDBLTFX );

  xmin = (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase];
  xmax = (BarXMax + VDUoffsetX) * ModeScaleX[ModeCase];
  ymin = (BarYMin + VDUoffsetY) * ModeScaleY[ModeCase];
  ymax = (BarYMax + VDUoffsetY) * ModeScaleY[ModeCase];

  loaded = (float)current_loading_step / (float)total_loading_steps;
  xmax = ( ( xmax - xmin ) * loaded ) + xmin;

  loaded = (float)current_substep / (float)total_substeps;

  one_step = (float)( xmax - xmin ) / (float)total_loading_steps;
  one_step *= loaded;
  xmax += one_step;

  // specify co-ordinates to plot to...
  dest.left = (LONG)xmin;
  dest.right = (LONG)xmax;
  dest.top = (LONG)ymin;
  dest.bottom = (LONG)ymax;

  //DebugPrintf("blitted to l:%d r:%d t:%d b:%d\n", dest.left, dest.right, dest.top, dest.bottom );
  
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &darkgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  if ( hr != DD_OK )
  {
    DebugPrintf("%s", D3DAppErrorToString(hr));
  }

  // top...
  dest.left = (LONG) ( (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMin + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  // bottom...
  dest.left = (LONG) ( (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMax + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  
  // left...
  dest.left = (LONG) ( (BarXMin - BorderX + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  // right...
  dest.left = (LONG) ( (BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + BorderX + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
}

void GetLevelName( char *buf, int bufsize, int level )
{
  FILE *f;
  char *pCh;
  
  buf[ 0 ] = 0;

  if ( level < 0 )
    return;
  
  f = fopen( MissionTextNames[ level ], "r" );
  if ( f )
  {
    // read level name
    if ( !fgets( buf, bufsize, f ) )
    {
        // if error ( as oposed to end of file )
      if ( ferror( f ) != 0 )
      {
        buf[ 0 ] = 0;
      }
    }
  }
  if ( !buf[ 0 ] )
  {
    strncpy( buf, LevelList.item[ level ], bufsize );
  }

  pCh = strchr( buf, '\n' );
  if ( pCh )
    *pCh = 0;
  
  buf[ bufsize - 1 ] = 0; // ensure null terminated 
}

float Browl = 0.0F;
float HostMultiPlayerTimeout;

int colourflash = 0;
char NodeName[256];

BOOL SetZCompare( void );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Game Status Control...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Game Status Control...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
RenderScene(LPDIRECT3DDEVICE Null1, LPDIRECT3DVIEWPORT Null2 )
{
  uint16  i,e;
  char  buf[256];
  LPDIRECTDRAWPALETTE ddpal;
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
  LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;
  //struct _stat stat_buf;
  //int result;
  static int WaitFrames = 2;
  BOOL done;
  LONGLONG  Time_Freq;
  LONGLONG  Time_Value;
  float   Time_Float;
  float   time_diff;

  //DebugPrintf("RenderScene Started\n");

  if ( SeriousError )
    return FALSE;

  //if ( !Bsp_Identical( &Bsp_Header[ 0 ], &Bsp_Original ) )
  //  hr = 0;

  CalculateFramelag();
  AnimOncePerFrame++;

  if ( bSoundEnabled )
  {
    CheckSBufferList();
  }

#ifndef MULTIPLE_READINPUTS

  // This is where in game we are getting input data read
  // Why would it be wrapped in this def check ?
  ReadInput();

#endif

  switch( MyGameStatus )
  {

  case STATUS_QuittingFromTitles:
	DebugState("STATUS_QuittingFromTitles\n");
    quitting = TRUE;
    break;

  case STATUS_WaitingToStartSinglePlayer:
	DebugState("STATUS_WaitingToStartSinglePlayer\n");

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    done = VduFinished( &MENU_NEW_BetweenLevels );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
          PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      StartASinglePlayerGame( NULL );
    }
    
    break;


  case STATUS_WaitingToStartDemo:
	DebugState("STATUS_WaitingToStartDemo\n");
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    break;


  case STATUS_WaitingToStartEndGameSequence:
	DebugState("STATUS_WaitingToStartEndGameSequence\n");
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    done = VduFinished( &MENU_NEW_GeneralLoading );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
      MyGameStatus = STATUS_Title;
      ReleaseView();
    }
    break;


  case STATUS_WaitingToStartMultiPlayerHost:
	DebugState("STATUS_WaitingToStartMultiPlayerHost\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
		Browl = 30.0F;
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
      MyGameStatus = STATUS_StartingMultiplayer;
      GoToSynchup( NULL );
    }
    break;


  case STATUS_WaitingToStartMultiPlayerClient:
	DebugState("STATUS_WaitingToStartMultiPlayerClient\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      
      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      MenuAbort();
      ReleaseView();
      LevelNum = -1;

      MyGameStatus = STATUS_PreStartingMultiplayerSynch;
      if( !ChangeLevel() ) 
        return( FALSE );
    }
    break;


  case STATUS_WaitingToStartTeamGame:
	DebugState("STATUS_WaitingToStartTeamGame\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      
      MenuAbort();

      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      MyGameStatus = PreSynchupStatus;
    }

    if( IMustQuit )
      SpecialDestroyGame();

    break;

  // the main menu screen
  case STATUS_Title:
	DebugState("STATUS_Title\n");

  // ??
  case STATUS_BetweenLevels:
	if(MyGameStatus != STATUS_Title)
		DebugState("STATUS_BetweenLevels\n");

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    break;


  case STATUS_Normal:
    DebugState("STATUS_Normal\n");

    PreventFlips = FALSE;

    if ( !CheatsDisabled )
      DisableCheats();
  
    if( IMustQuit )
      SpecialDestroyGame();

    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;

    if ( bSoundEnabled )
    {
      ProcessLoopingSfx();
#ifdef PLAYER_SPEECH_TAUNTS
      ProcessTaunt();
#endif
    }

    LevelTimeTaken += framelag;

    // if player is quiting nothing should stop him.....
    if( MyGameStatus != STATUS_QuitCurrentGame )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
          for( i = 0 ; i < MAX_PLAYERS ; i++ )
            if( (GameStatus[i] != STATUS_LeftCrashed ) &&
				(GameStatus[i] != STATUS_Left        ) &&
				(GameStatus[i] != STATUS_Null        ) &&
                (GameStatus[i] != STATUS_Normal      ) &&
				i != WhoIAm )
                SendGameMessage(MSG_YOUQUIT, 0, (BYTE) i , 0, 0);

          // the level has ended or changed ..tell everyone to go onto new level
          MyGameStatus = STATUS_LevelEnd;
          GameStatus[WhoIAm] = MyGameStatus;
          RandomStartPosModify += 1;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
        }
      }
	  else
	  {
        if( OverallGameStatus == STATUS_LevelEnd )
        {
          // tell the host that I am now in the same state as him....
          MyGameStatus = OverallGameStatus;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
    }

    if ( WaitingToQuit )
    {
      D3DAppClearScreenOnly();
      CenterPrint4x5Text( PLEASE_WAIT, (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );
      SelectQuitCurrentGame( NULL );
    }
    break;


  case STATUS_LevelEnd:
	DebugState("STATUS_LevelEnd\n");
    D3DAppClearScreenOnly();
    ReceiveGameMessages();
    Browl -= framelag;

    StopDemoRecording();

    if( Browl <= 0.0F )
    {
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	  else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }
    if( IsHost )
    {
      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( Ships[i].enable != 0 ) && ( GameStatus[i] != STATUS_LevelEnd ) && ( i != WhoIAm ) )
          break;
      }
      if( i != MAX_PLAYERS )
        break;  // not yet..

      // Added the if check to not call OutputVisiStats improperly
      // The order of code will always result in LevelNum being -1 here
      // So no point in even leaving this code uncommented...
      // We'll probably just end up getting rid of visi stats anyway later
      //if ( LevelNum != -1 )
      //	OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );

      // un load all graphics
      ReleaseView();
      ReleaseLevel();
	  
      // tell them all they can now look at the scores...
      MyGameStatus = STATUS_ViewingScore;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);

      InitScene();  // STATSTEST
      InitView();
	  // $$$
      NextworkOldBikeNum = -1;
      HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;

    }
	else
	{
      if( MyGameStatus != OverallGameStatus )
      {

        OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
        // un load all graphics
        ReleaseView();
        ReleaseLevel();
        // tell the host that I am now in the same state as him....
        MyGameStatus = STATUS_ViewingScore;

        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

        InitScene();  // STATSTEST
        InitView();
		// $$$
        NextworkOldBikeNum = -1;
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;
      }
    }
    break;


  case STATUS_ViewingScore:
	DebugState("STATUS_ViewingScore\n");

    ReceiveGameMessages();

    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }

    ScoreDisplay();
#ifdef MULTIPLE_READINPUTS
    ReadInput();
#endif

    HostMultiPlayerTimeout -= framelag;

    if( IsKeyPressed( DIK_SPACE ) ||
      ( OverallGameStatus == STATUS_WaitingAfterScore ) )
    {
      D3DAppIClearBuffers();
      HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F;

      if( IsHost )
      {
          // tell them all I Am waiting
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
          if( !ChangeLevel() )
			  return( FALSE );
      }
	  else
	  {
          // tell the host that I am now Waiting for him to finish viewing the score....
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;
          if( !ChangeLevel() )
			  return( FALSE );
      }
    }
    break;


  case STATUS_WaitingAfterScore:
	DebugState("STATUS_WaitingAfterScore\n");

    InitFontTransTable( TRUE );
    D3DAppClearScreenOnly();
    e = 0;

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;
    }

    if( DS )
    {
      DisplayStatusMessages();
    }
	else
	{	

		// you must clear player scores if MaxKills limit is set...
		// other wise the game just keeps on changing levels...
		// cause at the start of each level they already have max kills...

		// reset all player's stats
		if( MaxKills || ResetKillsPerLevel )
			ResetAllStats();

      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {      
        if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {

		//        if( GameStatus[i] == STATUS_ViewingScore )
		//        {
		//          wsprintf( buf, "%-8s status %8s\n", &Names[i][0] , "viewing score" );
		//          CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
		//        }
		//        else
          {
            wsprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
            CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_WaitingAfterScore ? 2 : 1 );
          }
          
          e++;
        }
      }
    }
    CenterPrint4x5Text( "waiting for all other players" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);

    Browl -= framelag;
    if( Browl < 0.0F )
    {
      colourflash++;
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }
    ReceiveGameMessages();

    if( IsHost )
    {

      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( ( ( GameStatus[ i ] >= STATUS_InitView_0 ) && ( GameStatus[ i ] <= STATUS_InitView_9 ) ) || ( GameStatus[i] == STATUS_ViewingScore ) ) && ( i != WhoIAm ) ) break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
      }

      //DebugPrintf("STATUS_WaitingAfterScore setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      InitFontTransTable( !bPolyText );
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);

    }else{
      if( OverallGameStatus == STATUS_Normal )
      {
        InitFontTransTable( !bPolyText );

        if( CountDownOn )
        {
          CreateCountdownDigits();
          StartCountDown( (int16) TimeLimit.value, 0 );
        }
        // tell the host that I am now in the same state as him....
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    break;


  case STATUS_Joining:
	DebugState("STATUS_Joining\n");
    PreventFlips = FALSE;
    D3DAppClearScreenOnly();

    ReceiveGameMessages();
    if( IMustQuit || IsHost || ( OverallGameStatus != STATUS_Normal ) )
    {
      SpecialDestroyGame();
      break;
    }
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    CenterPrint4x5Text( "Pickups Left   " , (d3dappi.szClient.cy>>1)-(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Pickups , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)-(FontHeight<<2), 2 );

    CenterPrint4x5Text( "RegenSlots Left" , (d3dappi.szClient.cy>>1)-(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].RegenSlots , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)-(FontHeight<<1), 2 );

    CenterPrint4x5Text( "Mines Left     " , (d3dappi.szClient.cy>>1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Mines , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1), 2 );

    CenterPrint4x5Text( "Triggers Left  " , (d3dappi.szClient.cy>>1)+(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Triggers , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)+(FontHeight<<1), 2 );

    CenterPrint4x5Text( "TrigVars Left  " , (d3dappi.szClient.cy>>1)+(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].TrigVars , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)+(FontHeight<<2), 2 );
    
    // wait for all the pickup and mine stuff to be sent to me.....
    if( ( Ships[WhoIAm].Pickups == 0 ) && ( Ships[WhoIAm].Mines == 0 ) && ( Ships[WhoIAm].RegenSlots == 0 ) &&
      ( Ships[WhoIAm].Triggers == 0 ) && ( Ships[WhoIAm].TrigVars == 0 ) && OverallGameStatus == STATUS_Normal)
    {
      // broadcast "x is joining the game" to all players...
      if (TeamGame)
        SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_JoiningTeamGame, 0);

      MyGameStatus = OverallGameStatus;
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
      GameStatus[WhoIAm] = OverallGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      SyncMines();
      InitShipRandomPos( WhoIAm );
      NextworkOldBikeNum = -1;
      set_player_name();
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
        IllegalTime = TRUE;
      }
    }
    break;

  // in the multiplayer screen waiting for the game to start
  case STATUS_StartingMultiplayer:
	DebugState("STATUS_StartingMultiplayer\n");

    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( TeamGame &&  (OverallGameStatus == STATUS_Normal) )
	  {
		  //SendGameMessage(MSG_SENDKILLSDEATHSBIKENUM, 0, 0, 0, 0);
	  }
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    ReceiveGameMessages();

    if( ( MyGameStatus == STATUS_QuitCurrentGame ) || ( MyGameStatus == STATUS_Title ) )
    {
      break;
    }
    
    if( ( CurrentMenu ) && ( CurrentMenuItem ) )
    {
      if( DisplayTitle() != TRUE )
        return FALSE;
    }

    // if status has been changed from titles, should exit this state straight away
    if ( MyGameStatus != STATUS_StartingMultiplayer )
      break;

    if( ( IsHost ) && ( !CurrentMenu ) && ( !CurrentMenuItem ) )
    {
      D3DAppIClearBuffers();
      ReleaseView();
      // tell them all to load up a level
      MyGameStatus = STATUS_StartingMultiplayerSynch;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

      LevelNum = -1;
      if( !ChangeLevel() ) return( FALSE );
      HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F;
    }
    else
    {
      if( !IsHost )
      {
        if( ( OverallGameStatus == STATUS_StartingMultiplayerSynch ) || 
          ( (OverallGameStatus == STATUS_Normal) && (!TeamGame || PlayerReady[WhoIAm]) ) )
        {

          if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
          { 
            //LastMenu = CurrentMenu;
            //VduClear();
            MenuChangeEx( &MENU_NEW_GeneralLoading );
            MenuDraw( CurrentMenu );  // because menu processing will not be done when MyGameStatus changes
          }else
            D3DAppIClearBuffers();


          PreSynchupStatus = MyGameStatus;
          MyGameStatus = STATUS_WaitingToStartMultiPlayerClient;
        }
      }
    }
    break;


  case  STATUS_PreStartingMultiplayerSynch:
	DebugState("STATUS_PreStartingMultiplayerSynch\n");

    PreventFlips = FALSE;

    ReceiveGameMessages();
    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }
    MyGameStatus = STATUS_StartingMultiplayerSynch;
    if( OverallGameStatus == STATUS_Normal )
    {
      initShip( WhoIAm );
      MyGameStatus = STATUS_Joining;
      Browl = 0.0F;
    }else{
      smallinitShip( WhoIAm );
      MyGameStatus = STATUS_StartingMultiplayerSynch;
    }
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    GameStatus[WhoIAm] = MyGameStatus;
    break;


  case  STATUS_StartingMultiplayerSynch:
	DebugState("STATUS_StartingMultiplayerSynch\n");

    PreventFlips = FALSE;
    InitFontTransTable( TRUE );

    ReceiveGameMessages();

    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }

    D3DAppClearScreenOnly();
    e = 0;

    if( DS )
    {
      DisplayStatusMessages();
    }else{
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( GameStatus[i] != STATUS_GetPlayerNum )&& (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {
          wsprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
          CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
          e++;
        }
      }
    }
    CenterPrint4x5Text( "all players synching" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      colourflash++;
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    if( CountDownOn )
    {
      CreateCountdownDigits();
      StartCountDown( (int16) TimeLimit.value, 0 );
      IllegalTime = FALSE;
    }

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;

      // have all the enabled players switched over to STATUS_StartingMultiplayer??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( (GameStatus[i] != STATUS_GetPlayerNum ) &&(GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && ( (GameStatus[i] != STATUS_Null ) && ( GameStatus[i] != STATUS_StartingMultiplayerSynch ) ) && ( i != WhoIAm ) )
          break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
      DebugPrintf("STATUS_StartingMultiplayerSynch setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      GameStatus[WhoIAm] = STATUS_Normal;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

	  // $$$
      NextworkOldBikeNum = -1;
      InitFontTransTable( !bPolyText );

    }
	else
	{
      if( OverallGameStatus == STATUS_Normal )
      {
        QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        // $$$
        NextworkOldBikeNum = -1;
        InitFontTransTable( !bPolyText );
      }
    }
    break;
  

  case STATUS_QuitCurrentGame:
	DebugState("STATUS_QuitCurrentGame\n");

    StopCompoundSfx();
    OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
    MenuAbort();
    MenuRestart( &MENU_Start );
    DestroyGame();  // DestroyGame changes MyGameStatus..So Set it back
    MyGameStatus = STATUS_QuitCurrentGame;
    ReleaseView();
    ReleaseLevel();
    MyGameStatus = STATUS_Title;
    InitScene();
    InitView();

    break;

  case STATUS_GetPlayerNum:
	DebugState("STATUS_GetPlayerNum\n");

    D3DAppClearScreenOnly();
    ReceiveGameMessages();
    
    CenterPrint4x5Text( "Requesting Player Number" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , 2 );
                                          
    sprintf( &buf[0] , "Attempt %d\n" , GetPlayerNumCount );
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );

    
    GetPlayerNumCount1 -= framelag;
    GetPlayerNumCount2 -= framelag;

    if( WhoIAm != 0xff )
    {
		DebugPrintf("WhoIAm: %d\n",WhoIAm);
      if( WhoIAm == MAX_PLAYERS )
      {
        PrintErrorMessage ( "The Game is Currently Un-Joinable", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }
      if( WhoIAm == MAX_PLAYERS+1 )
      {
        PrintErrorMessage ( "You Dont have the level they are playing", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }
      if( WhoIAm == MAX_PLAYERS+2 )
      {
        PrintErrorMessage ( "The Game is Full", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }
	  if( WhoIAm == MAX_PLAYERS+3 )
      {
		PrintErrorMessage ( "You do have the right version.", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }

      memset(&Names, 0, sizeof(SHORTNAMETYPE) );
      strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
      Names[WhoIAm][7] = 0;
      Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
    
      MyGameStatus = STATUS_StartingMultiplayer;
      MenuState = MENUSTATE_Select;

      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      set_player_name();

#ifdef DEMO_SUPPORT
      if( RecordDemo )
      {
        DemoFp = fopen( "Test.dmo" , "wb" );
        setvbuf( DemoFp, NULL, _IONBF , 0 );    // size of stream buffer...
        for( i = 0 ; i < 256 ; i++ )
        {
          Demo_fwrite( &ShortLevelNames[NewLevelNum][i], sizeof(char), 1, DemoFp );
          if( ShortLevelNames[NewLevelNum][i] == 0 )
          {
            break;
          }
        }
        // Best way I can Think of to send the Host Name to the demo file...
        Temp = 1;
        TempTime = 1;
        Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
        size = sizeof( NAMEMSG );
        Demo_fwrite( &size, sizeof(int), 1, DemoFp );
        Demo_fwrite( &Temp, sizeof(DWORD), 1, DemoFp );
        msg = MSG_NAME;
        Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
        msg = WhoIAm;
        Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );       // Whos Name it is..
        Demo_fwrite( &biker_name[0], 7, 1, DemoFp );
        msg = 0;
        Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );       // terminator for name..
      }
#endif

      // menu change is done when session description is changed for lobby session
       MenuChangeEx( GetPlayerNumMenu );

      break;
    }

    if( GetPlayerNumCount2 <= 0.0F )
    {
      // Time is Up No Player Number was got so quit back to the title screen...
      PrintErrorMessage ( "Couldnt Get a Player Number", 3, NULL, ERROR_USE_MENUFUNCS );
      MyGameStatus = STATUS_Title;
      break;
    }

    if( GetPlayerNumCount1 <= 0.0F )
    {
      GetPlayerNumCount++;

      // Time to send another request for a Player Number...
      GetPlayerNumCount1 = 60.0F;   // approx 1 Second...
      SendGameMessage(MSG_HEREIAM, 0, 0, 0, 0); // can I join your game...
      DebugPrintf("sending MSG_HEREIAM\n");
    }

    break;

  case STATUS_PlayingDemo:
	DebugState("STATUS_PlayingDemo\n");

    DemoGameLoops++;

    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
    
    Oldframelag = framelag;

    if( PauseDemo )
    {
      framelag = 0.0F;
    }else{
      framelag *= Demoframelag;
    }

    if( DemoScreenGrab && !CurrentMenu )
    {
	//      framelag = 2.24F;   // 30 fps
      framelag = 2.613333F; // 25 fps
      Demoframelag = 1.0F;
      Oldframelag = framelag;
    }

    if( DemoScreenGrab && !CurrentMenu )
    {
	sprintf( fname, "ScreenShots\\scr%04d.ppm", fnum );
	//      _spawnl( _P_WAIT, "grab.exe", "grab.exe", "-fp" , "-w 512" , "-h 384" , fname, NULL );
      folder_exists( FMVSNAPSHOT_FOLDER );
      SaveFullScreenSnapShot( fname );
      fnum++;
      if( fnum > 9999 )
        DemoScreenGrab = FALSE;
    }

    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;

    break;


  case STATUS_InitView_0:
	DebugState("STATUS_InitView_0\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    D3DAppClearScreenOnly();

    MenuFrozen = FALSE; // ensure that menus are OK to use once in game
    JustExitedMenu = FALSE; 

    QuickStart = QUICKSTART_None;
    WaitingToQuit = FALSE;
  
    ReceiveGameMessages();

    if (bPrimaryPalettized )
    {
      lpPalette = DDLoadPalette( lpDD , "data\\pictures\\pal.bmp");
      ddpal =  DDLoadPalette( lpDD , "data\\pictures\\pal.bmp");
      LastError = d3dappi.lpFrontBuffer->lpVtbl->SetPalette( d3dappi.lpFrontBuffer, ddpal );
      LastError = d3dappi.lpBackBuffer->lpVtbl->SetPalette( d3dappi.lpBackBuffer, ddpal );
    }
  
    if( !SetMatrixViewPort() )
    {
      SeriousError = TRUE;
      Msg( "SetMatrixViewPort() failed\n" );
      return FALSE;
    }
    
    InitFont(FALSE);

    MyGameStatus = STATUS_InitView_1;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_1:
	DebugState("STATUS_InitView_1\n");
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

    SetOurRenderStates( NULL );
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

    // Create the offscreen surface, by loading our bitmap
      lpDDSOne = DDLoadBitmap( lpDD, "data\\pictures\\panel.bmp" , 0, 0 );
    ddpal =  DDLoadPalette( lpDD , "data\\pictures\\panel.bmp");
    LastError = lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
    DDSetColorKey( lpDDSOne, RGB_MAKE( 0 , 0 , 0 ) );
    // Create the offscreen surface, by loading our bitmap.
    if( ( ModeCase != -1 ) && ( DrawPanel == FALSE ) && DrawSimplePanel )
    {
      lpDDSThree = DDLoadBitmap( lpDD, (char*) &PanelNames[ModeCase] , 0, 0 );
      ddpal =  DDLoadPalette( lpDD , (char*) &PanelNames[ModeCase]);
      LastError = lpDDSThree->lpVtbl->SetPalette( lpDDSThree , ddpal );
      DDSetColorKey( lpDDSThree, RGB_MAKE( 255 , 0 , 255 ) );
      PanelHeight = PanelVisibleY[ModeCase];

      lpDDSFour = DDLoadBitmap( lpDD, "data\\pictures\\pcontent.bmp" , 0, 0 );
      ddpal =  DDLoadPalette( lpDD , "data\\pictures\\pcontent.bmp" );
      LastError = lpDDSFour->lpVtbl->SetPalette( lpDDSFour , ddpal );
    }
  

    FillInPanelPositions();
    ReMakeSimplePanel = TRUE;
    
	//    InitVisiExecList( lpDev );
    InitSkinExecs();
    InitPortalExecs();
    InitRenderBufs( lpDev );

    // Init the Texture Handler
    if( !InitTload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "InitTLoad failed\n" );
      return FALSE;
    }
  
    //  Prep the Texture Handler.....
    if( !PreMload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }


    // Can Cope with no .Wat file!!!
    PreWaterLoad( (char*) &WaterNames[LevelNum][0] );
    
    
    if( OnceOnlyChangeLevel )
    {
      OnceOnlyChangeLevel = FALSE;

      if( !PreLoadShips() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadBGOFiles() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadRestartPoints() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadEnemies() )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }

    EnableRelavantModels( &ModelNames[0] );

	if( !PreInitModel( lpDev , &ModelNames[0] ) )
	{
		SeriousError = TRUE;
		return FALSE;
	}

	if( !Load_All_Off_Files( &OffsetFiles[ 0 ] ) )
	{
		SeriousError = TRUE;
		return FALSE;
	}

    MyGameStatus = STATUS_InitView_2;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_2:
	DebugState("STATUS_InitView_2\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

    D3DAppClearScreenOnly();

    ReceiveGameMessages();


    //  Load in And if nescessary ReScale Textures... 
    if( !Tload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
  
    FixTextureUVs( &OffsetFiles[ 0 ] );

    // Load all system memory textures...
    if( !SysTload( SystemMemTPages, CurrentSysTexture ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    MyGameStatus = STATUS_InitView_3;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_3:
	DebugState("STATUS_InitView_3\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

      if( !InitModel( lpDev , &ModelNames[0] ) )
        {
        SeriousError = TRUE;
        return FALSE;               // all 3d models....
      }
    MyGameStatus = STATUS_InitView_4;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_4:
	DebugState("STATUS_InitView_4\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

    if( !Mload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }

    InitVisiStats( &Mloadheader );

    MyGameStatus = STATUS_InitView_5;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_5:
	DebugState("STATUS_InitView_5\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();
    ReceiveGameMessages();

    // Can Cope with no Bsp file!!!
#ifdef LOAD_ZBSP
    Bspload( (char*) &BspZNames[LevelNum][0], &Bsp_Header[ 0 ] );
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 1 ] );
#else
#ifdef BSP_ONLY
    if ( !Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ) )
    {
      SeriousError = TRUE;
      Msg( "Bspload failed\n" );
      return FALSE;   // the collision data
    }
#else
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ); // load .BSP file into 0 skin
#endif
    Bsp_Header[ 1 ].State = FALSE; // no non-zero .BSP any more
#endif
	//    if ( !Bsp_Duplicate( &Bsp_Header[ 0 ], &Bsp_Original ) )
	//    {
	//		    SeriousError = TRUE;
	//		    Msg( "Bsp_Duplicate failed\n" );
	//		    return FALSE;
	//    }
    
    // might not be any water...
    WaterLoad();

    ReadTxtFile( (char*) &TextNames[LevelNum][0] );
    
    ReadMsgFile( (char*) &MsgNames[LevelNum][0] );
    
    if( !MCload( (char*) &CollisionNames[LevelNum][0] , &MCloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "MCload non zero failed\n" );
      return FALSE;   // the collision data
    }
    if( !MCload( (char*) &CollisionZNames[LevelNum][0] , &MCloadheadert0 ) )
    {
      SeriousError = TRUE;
      Msg( "MCload zero failed\n" );
      return FALSE; // the collision data skin thickness 0
    }
  
    SetUpShips();

    if( !InitSpecialExecBufs() )
    {
      SeriousError = TRUE;
      Msg( "InitSpecialExecBufs() failed\n" );
      return FALSE;
    }

    MyGameStatus = STATUS_InitView_6;
    PrintInitViewStatus( MyGameStatus );

    break;

  case STATUS_InitView_6:
	DebugState("STATUS_InitView_6\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();
    ReceiveGameMessages();

    InitSoundInfo( &Mloadheader );
    
    MyGameStatus = STATUS_InitView_7;
    PrintInitViewStatus( MyGameStatus );
    break;


  case STATUS_InitView_7:
	DebugState("STATUS_InitView_7\n");

    if( IsHost )
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	else
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

    D3DAppClearScreenOnly();
    ReceiveGameMessages();
#ifdef NO_PRECALCULATED_CELL_COLOURS
    CreateCellColours( &Mloadheader );
#endif
    MyGameStatus = STATUS_InitView_8;
    PrintInitViewStatus( MyGameStatus );
    break;


  case STATUS_InitView_8:
	DebugState("STATUS_InitView_8\n");

    if( IsHost )
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	else
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

    D3DAppClearScreenOnly();
    ReceiveGameMessages();
    MyGameStatus = STATUS_InitView_9;
    PrintInitViewStatus( MyGameStatus );
    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);
    // dummy call to timer ensures no pauses later...
    timeSetEvent( 10, 10, TimerProc, (DWORD)-1, TIME_ONESHOT ); 
    InitShipSpeeds();
    MyGameStatus = InitView_MyGameStatus;
    Current_Max_Score = 0;  // used by host to store highest score in session desc

	// lets keep this global next to the cursor clip for now
	// this is a good place to define that we are going into game mode...
	SetCursorClip( TRUE );
	HideCursor = TRUE;

    break;


  case STATUS_ChangeLevelPostInitView:
	DebugState("STATUS_ChangeLevelPostInitView\n");

    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".NOD" );
    if( !Nodeload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadBGOFiles() )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadEnemies() )
    {
      SeriousError = TRUE;
      Msg( "LoadEnemies() failed\n" );
      return( FALSE );
    }

    if( !LoadSpotFX() )
    {
      SeriousError = TRUE;
      Msg( "LoadSpotFX() failed\n" );
      return( FALSE );
    }

    if( !LoadStartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadStartPoints() failed\n" );
      return( FALSE );
    }

    if( !LoadRestartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadRestartPoints() failed\n" );
      return( FALSE );
    }

      if ( !InitializeSound( DESTROYSOUND_All ))
      {
        Msg("InitializeSound() failed\n");
        return FALSE;
      }


    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".CAM" );

    if( !Cameraload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    // might not be any real-time lights
    LoadRTLights();

    if ( CaptureTheFlag || CTF )
      GoalLoad();

    InitThermo();

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      LoadTimeForLevel();
    }

    if( !LoadPickupsPositions() )
    {
      SeriousError = TRUE;
      Msg( "LoadPickupsPositions() failed\n" );
      return( FALSE );
    }

    // might not be any External Forces...
    ExternalForcesLoad( (char*) &ExternalForceNames[LevelNum][0] );

    // might not be any Teleports...
    TeleportsLoad( (char*) &TeleportNames[LevelNum][0] );
	
    // Can Cope with no Zone file!!!
    TriggerAreaload( (char*) &ZoneNames[LevelNum][0] );


    InitShipsChangeLevel(&Mloadheader);

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) SinglePlayerTimeLimit, 0 );
      }
      else
      {
        DeleteCountdownDigits();
      }
    }

    GodModeOnceOnly = TRUE;

    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    MyGameStatus = ChangeLevel_MyGameStatus;

    break;


  case STATUS_ChangeLevelPostPlayingDemo:
	DebugState("STATUS_ChangeLevelPostPlayingDemo\n");

    WhoIAm = MAX_PLAYERS;
    Ships[WhoIAm].Object.Mode = DEMO_MODE;
    Ships[WhoIAm].enable = 1;
    IsHost = TRUE;
    Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
    AutoDetail = FALSE;
  
    for( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
      GameStatus[i] = STATUS_Null;
    }
  
    DemoTimeSoFar = 0;
    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
  
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &DemoStartedTime);
    GameElapsedTime = 0;
    DemoGameLoops = 0;
    TempGameElapsedTime = GameStartedTime;
    MyGameStatus = STATUS_PlayingDemo;
    break;

	//  *********************** Single Player Game Stuff **********************************


  case  STATUS_StartingSinglePlayer:
	DebugState("STATUS_StartingSinglePlayer\n");

    //D3DAppIClearBuffers();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_PostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    LevelNum = -1;
    if( !NewLevelNum )
    {
      NumGoldBars = 0;
      CrystalsFound = 0;
    }
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_PostStartingSinglePlayer:
	DebugState("STATUS_PostStartingSinglePlayer\n");

    smallinitShip( WhoIAm );
	//    if( CountDownOn )
	//    {
	//      CreateCountdownDigits();
	//      StartCountDown( (int16) TimeLimit.value, 0 );
	//    }
    
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelTimeTaken = 0.0F;

    QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
    QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
    Old_LevelTime_Float = ( ( Time_Value * 100.0F ) / Time_Freq );

    break;


  case STATUS_SinglePlayer:
	DebugState("STATUS_SinglePlayer\n");

    PreventFlips = FALSE;
      
    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;

    if ( bSoundEnabled && !CurrentMenu )
    {
      ProcessLoopingSfx();
      ProcessEnemyBikerTaunt();
    }

    //LevelTimeTaken += framelag;

    QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
    QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
    Time_Float = ( ( Time_Value * 100.0F ) / Time_Freq );
    time_diff = ( Time_Float - Old_LevelTime_Float );
    Old_LevelTime_Float = Time_Float;

    if ( framelag )
    {
      LevelTimeTaken += time_diff;
    }

    //sprintf( buf, "time so far %d", ((uint32)LevelTimeTaken) / 100 );
    //Print4x5Text( buf, 10, 30, 2 );

    // if player is quiting nothing should stop him.....
    if( MyGameStatus != STATUS_QuitCurrentGame )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
          D3DAppIClearBuffers();
          ReleaseView();
          // the level has ended or changed...
          MyGameStatus = STATUS_ViewingStats;
          GameStatus[WhoIAm] = MyGameStatus;
          InitView();
        }
      }
    }
    break;


  case STATUS_ViewingStats:
	DebugState("STATUS_ViewingStats\n");

    D3DAppIClearBuffers();

    ReleaseLevel();

    ReleaseView();
      
    MyGameStatus = STATUS_BetweenLevels;
    InitScene();
    InitView();
    MenuAbort();
    MenuRestart( &MENU_NEW_NumberOfCrystals );
    CameraStatus = CAMERA_AtRightVDU;
        break;


  case  STATUS_TitleLoadGameStartingSinglePlayer:
	DebugState("STATUS_TitleLoadGameStartingSinglePlayer\n");

    //D3DAppIClearBuffers();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_InGameLoadGameStartingSinglePlayer:
	DebugState("STATUS_InGameLoadGameStartingSinglePlayer\n");

    //D3DAppIClearBuffers();

    NewLevelNum = InGameLoadGameLevelNum;
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_TitleLoadGamePostStartingSinglePlayer:
	DebugState("STATUS_TitleLoadGamePostStartingSinglePlayer\n");

    smallinitShip( WhoIAm );
    InGameLoad( NULL );
    
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    break;

  default:
	DebugPrintf("MyGameStatus has a bad setting!!!!\n");
	break;

	//  ******************** End of Single Player Game Stuff *******************************

  }

  return TRUE;
}


float Hdiv[MAX_PLAYERS] = { 1.0F ,
                1.0F ,
                1.0F ,
                0.5F ,
                0.5F ,
                0.5F ,
                1.0F ,
                0.5F };
float Vdiv[MAX_PLAYERS] = { 1.0F ,
                0.5F ,
                0.33333333F ,
                0.5F ,
                0.2F ,
                0.33333333F ,
                0.142857142F ,
                0.25F  };
float Hpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 0.0F } ,
                       { 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F } ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F} ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F} ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F } };
float Vpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F} ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F , 5.0F , 6.0F} ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F , 3.0F , 3.0F } };
                       

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Main Routines...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void MainRoutines( void )
{
  if( PlayDemo )
  {
    DemoPlayingNetworkGameUpdate();
  }

  ProcessShips();

#ifdef SHADOWTEST
//  CreateSpotLight( (uint16) WhoIAm, SHIP_RADIUS, &Mloadheader );
//  CreateShadowsForShips();
#endif

  FirePrimary();
  FireSecondary();
  ProcessEnemies();
  ProcessSpotFX();
  ProcessPrimaryBullets();
  ProcessSecondaryBullets();
  if( !PlayDemo ) RegeneratePickups();
  ProcessPickups();
  ProcessBGObjects( TRUE );
  ProcessRestartPoints();
  ProcessModels();
  ProcessPolys();
  ProcessXLights( &Mloadheader );
  DoAfterBurnerEffects();
  FmPolyProcess();
  CheckTimeLimit();
  if( CountDownOn ) UpdateCountdownDigits();
  ShowScreenMultiples();
  ProcessActiveConditions();
  ProcessTriggerAreas();
  ProcessGoals();
#ifdef DOESNT_WORK_AND_A_BAD_IDEA_ANYWAY
  if ( outside_map && !DebugInfo && ( Ships[WhoIAm].Object.Mode != DEATH_MODE ) && ( Ships[WhoIAm].Object.Mode != LIMBO_MODE ) )
  {
    // bugger -- the player has managed to get outside the map, so kill him off...
    Ships[WhoIAm].ShipThatLastHitMe = MAX_PLAYERS;
    Ships[WhoIAm].Damage = 255 + 255; //make sure I Die....
    DoDamage( OVERRIDE_INVUL );
  
  
    Ships[WhoIAm].Object.Mode = DEATH_MODE;
    Ships[WhoIAm].Timer = 0.0F;
    ShipDiedSend( WEPTYPE_Primary, 0);
  }
#endif
  WaterProcess();
  ProcessRTLights();
  
	//DebugPrintf("MainRoutines Finished...\n");
}

void CheckForRogueSfx( void )
{ 
  DWORD current_time, dwStatus;
  int i;

  // Dirty hack to kill off any rogue sfx!! 
  if ( bSoundEnabled && NumDupCompoundBuffers )
  {
    current_time = GetTickCount();

    for ( i = 0; i < NumDupCompoundBuffers; i++ )
    {
      IDirectSoundBuffer_GetStatus( CompoundSfxBuffer[ i ].buffer, &dwStatus );

      // if buffer is playing, check whether it should have stopped by now...
      if (dwStatus & DSBSTATUS_PLAYING)
      {
        if ( current_time > ( CompoundSfxBuffer[ i ].finish_time + 50 ) )
        {
          DebugPrintf("Rogue SFX killed off: SfxNum %d, SndObj index %d, start time %d finish time %d ( current time %d ), timerID %d  \n",
          CompoundSfxBuffer[ i ].current_sfx, CompoundSfxBuffer[ i ].compound_buffer_lookup_index, CompoundSfxBuffer[ i ].start_time,
          CompoundSfxBuffer[ i ].finish_time, current_time, CompoundSfxBuffer[ i ].timerID);

          KillCompoundSfxBuffer( i );

          //SfxHolder[ CompoundSfxBuffer[ i ].SfxHolderIndex ].Used = FALSE;
          FreeSfxHolder( CompoundSfxBuffer[ i ].SfxHolderIndex );
  

        }
      }
    }
  }
}

void CheckLevelEnd ( void )
{

  if( HasLevelEnded() )
  {
    PauseAllSfx();
#ifdef FINAL_RELEASE
    if( OriginalLevels )
    {
#endif
      LevelNum = -1;
      if( NewLevelNum == ( LevelList.items - 2 ) )
      {
        // if they cheated at any point
        if ( Cheated )
        {
          GameCompleted = GAMECOMPLETE_Cheated;
        }else
        {
          // Check for Secret level...
          if( CrystalsFound == ( LevelList.items - 1 ) )
          {
            //They can go to the secret level
            NewLevelNum = ( LevelList.items - 1 );
            GameCompleted = GAMECOMPLETE_GotAllCrystalsSoFar;
          }else{
            // go straight to the end sequence
            GameCompleted = GAMECOMPLETE_WithoutAllCrystals;
          }
        }
      }else if( NewLevelNum == ( LevelList.items - 1 ) ){
        // They have completed the Secret level
        if( CrystalsFound == ( LevelList.items ) )
        {
          GameCompleted = GAMECOMPLETE_WithAllCrystals;
        }else
        {
          GameCompleted = GAMECOMPLETE_WithAllCrystalsExceptLast;
        }
      }else{
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
      
      return;
#ifdef FINAL_RELEASE
    }else
    {
      if( NewLevelNum == ( LevelList.items - 1 ) )
      {
        // completed all levels ( not original levels )
        GameCompleted = GAMECOMPLETE_AdditionalLevels;
        return;
      }else
      {
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
    }
#endif
  }

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Main Render Loop...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

BOOL
MainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  int i;
  static float fov_inc = 0.0F;

  QueryPerformanceCounter((LARGE_INTEGER *) &GameCurrentTime);

  if( PlayDemo )
  {
    if( PauseDemo )
    {
      TempGameElapsedTime = GameCurrentTime;
    }else{
      if( DemoScreenGrab && !CurrentMenu )
      {
        GameElapsedTime += (LONGLONG) ( ( ( 2 * ticksperframe ) / 1000.0F) * Freq );  // approx every 2 frames...
      }else{
        GameElapsedTime += (LONGLONG) ( ( GameCurrentTime - TempGameElapsedTime ) * Demoframelag );
      }
      TempGameElapsedTime = GameCurrentTime;

      GameCurrentTime = GameCurrentTime - GameStartedTime;
      GameCurrentTime = (LONGLONG) ( GameCurrentTime * Demoframelag );
    }
  }else{
    GameCurrentTime = GameCurrentTime - GameStartedTime;
  }

#ifdef DEBUG
  if ( framelag > 10.0F ) // check framelag out of reasonable range -> probably debugging
  {
    framelag = 10.0F; // clamp framelag to something reasonable
  }
#endif // DEBUG


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Main Routines to be called before Rendering....  
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

  InitIndirectVisible( Ships[Current_Camera_View].Object.Group );

  if( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) )
    AddIndirectVisible( (uint16) ( ( ActiveRemoteCamera ) ? ActiveRemoteCamera->Group : SecBulls[ CameraMissile ].GroupImIn ) );

  MainRoutines();

  if( MyGameStatus == STATUS_QuitCurrentGame )
    return TRUE;

  TloadCheckForLostSurfaces(&Tloadheader);
  memset( (void*) &IsGroupVisible[0] , 0 , MAXGROUPS * sizeof(uint16) );
  cral += (framelag*2.0F);

  for( i = 0 ; i < MAX_SFX ; i++ )
    LastDistance[i] = 100000.0F;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Now the Rendering can begin...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

  if (lpDev->lpVtbl->BeginScene(lpDev) != D3D_OK)
    return FALSE;

   if(ShowStats)
	  ScoreDisplay();

    else if( !FullRearView )
    {
      CameraRendering = CAMRENDERING_Main;
      MainCamera.enable = 1;
      MainCamera.GroupImIn = Ships[Current_Camera_View].Object.Group; 
      MainCamera.Mat = Ships[Current_Camera_View].Object.FinalMat;  
      MainCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat;  
      MainCamera.Pos = Ships[Current_Camera_View].Object.Pos; 
      MainCamera.Viewport = viewport; 
      MainCamera.Proj = proj; 

      CurrentCamera = MainCamera;

      HUDNames();
      DispEnhancedCrosshair();

      CurrentCamera.UseLowestLOD = FALSE;
      if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
        return FALSE;

#ifdef Z_TRICK
      if ( !ZClearsOn )
      {
        g_OddFrame = !g_OddFrame;
        SetZProj();
        if( !SetZCompare() )
        {
          Msg("unable to set z compare\n");
          return FALSE;
        }
      }
#endif
  
      if( RearCameraActive && !RearCameraDisable )
      {
        CameraRendering = CAMRENDERING_Rear;

        CurrentCamera.enable = 1;
        CurrentCamera.UseLowestLOD = TRUE;
        CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
        CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
        CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
        CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
        CurrentCamera.Viewport = viewport;  
        CurrentCamera.Proj = proj;  
        CurrentCamera.InvMat._31 *= -1.0F;
        CurrentCamera.InvMat._32 *= -1.0F;
        CurrentCamera.InvMat._33 *= -1.0F;
        CurrentCamera.InvMat._11 *= -1.0F;
        CurrentCamera.InvMat._12 *= -1.0F;
        CurrentCamera.InvMat._13 *= -1.0F;
        CurrentCamera.Mat._13 *= -1.0F;
        CurrentCamera.Mat._23 *= -1.0F;
        CurrentCamera.Mat._33 *= -1.0F;
        CurrentCamera.Mat._11 *= -1.0F;
        CurrentCamera.Mat._21 *= -1.0F;
        CurrentCamera.Mat._31 *= -1.0F;
        CurrentCamera.Viewport.dwX = ( viewport.dwX + viewport.dwWidth ) - ( (viewport.dwWidth >>4) + ( viewport.dwWidth >>2 ) );
        CurrentCamera.Viewport.dwY = viewport.dwY + (viewport.dwHeight >>4);
        CurrentCamera.Viewport.dwWidth = viewport.dwWidth >>2;
        CurrentCamera.Viewport.dwHeight = viewport.dwHeight >>2;
        CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
        CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
        
        CurrentCamera.UseLowestLOD = TRUE;

        if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
            return FALSE;
      }

      if( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) )
      {
        float main_fov;

        TempMissileCam = Current_Camera_View;
        Current_Camera_View = -1;
        if( ActiveRemoteCamera )
        {
          CameraRendering = CAMRENDERING_Pip;
          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = ActiveRemoteCamera->Group;  
          CurrentCamera.Mat = ActiveRemoteCamera->Mat;  
          CurrentCamera.InvMat = ActiveRemoteCamera->InvMat;  
          CurrentCamera.Pos = ActiveRemoteCamera->Pos;
        }
		else
		{
          CameraRendering = CAMRENDERING_Missile;

          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = SecBulls[ CameraMissile ].GroupImIn;  
          CurrentCamera.Mat = SecBulls[ CameraMissile ].Mat;  
          MatrixTranspose( &SecBulls[ CameraMissile ].Mat, &CurrentCamera.InvMat );
          CurrentCamera.Pos = SecBulls[ CameraMissile ].Pos;
        }
        CurrentCamera.Viewport = viewport;
        main_fov = hfov;
        SetFOV( normal_fov ); // was SetFOV( START_FOV ), but this doesn't work for wide angle fov
        CurrentCamera.Proj = proj;  
        
        CurrentCamera.Viewport.dwX = viewport.dwX + (viewport.dwWidth >>4);
        CurrentCamera.Viewport.dwY = viewport.dwY + (viewport.dwHeight >>4);
        CurrentCamera.Viewport.dwWidth = viewport.dwWidth >>2;
        CurrentCamera.Viewport.dwHeight = viewport.dwHeight >>2;
        CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
        CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
        
        
        CurrentCamera.UseLowestLOD = TRUE;

        if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
            return FALSE;
       
        Current_Camera_View=TempMissileCam;
        SetFOV( main_fov );

      }
    }
	else
	{
      // Full Screen Rear View....
      CameraRendering = CAMRENDERING_Rear;
      CurrentCamera.enable = 1;
      CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
      CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
      CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
      CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
      CurrentCamera.Viewport = viewport;  
      CurrentCamera.Proj = proj;  
      CurrentCamera.InvMat._31 *= -1.0F;
      CurrentCamera.InvMat._32 *= -1.0F;
      CurrentCamera.InvMat._33 *= -1.0F;
      CurrentCamera.InvMat._11 *= -1.0F;
      CurrentCamera.InvMat._12 *= -1.0F;
      CurrentCamera.InvMat._13 *= -1.0F;
      CurrentCamera.Mat._13 *= -1.0F;
      CurrentCamera.Mat._23 *= -1.0F;
      CurrentCamera.Mat._33 *= -1.0F;
      CurrentCamera.Mat._11 *= -1.0F;
      CurrentCamera.Mat._21 *= -1.0F;
      CurrentCamera.Mat._31 *= -1.0F;
      CurrentCamera.UseLowestLOD = FALSE;

      if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
          return FALSE;

#ifdef Z_TRICK
      if ( !ZClearsOn )
      {
        g_OddFrame = !g_OddFrame;
        SetZProj();
        if( !SetZCompare() )
        {
          Msg("unable to set z compare\n");
          return FALSE;
        }
      }
#endif
    }
 /* done with rendering camera stuff */

  if( DrawPanel && (WhoIAm == Current_Camera_View ))
    Disp3dPanel( lpDev, lpView );
  
  /* do the target c omputer trick */
  if( TargetComputerOn )
  {
		lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
		DispTracker( lpDev, lpView );
  }

#ifdef REFLECTION
  {
    VECTOR  TempUp;
    CameraRendering = CAMRENDERING_Missile;
//    CameraRendering = CAMRENDERING_Main;

    SetFOV( 90.0F );
    if( Ships[WhoIAm ^ 1].enable )
    {
      Current_Camera_View = WhoIAm ^ 1;
      CurrentCamera.enable = 1;
      CurrentCamera.UseLowestLOD = FALSE;
      CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;
      ApplyMatrix( &Ships[WhoIAm].Object.Mat, &SlideUp, &TempUp );      /* Calc Direction Vector */
      MakeViewMatrix( &Ships[Current_Camera_View].Object.Pos, &Ships[WhoIAm].Object.Pos, &TempUp, &CurrentCamera.Mat);
      MatrixTranspose( &CurrentCamera.Mat, &CurrentCamera.InvMat );

//    CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
//    CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
      CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
      CurrentCamera.Viewport = viewport;  
      CurrentCamera.Proj = proj;  

      CurrentCamera.Viewport.dwX = 0;
      CurrentCamera.Viewport.dwY = 0;
      CurrentCamera.Viewport.dwWidth = 128;
      CurrentCamera.Viewport.dwHeight = 128;
      CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
      CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
      CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                         D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
      CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                         D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
      
      {
        if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
          return FALSE;
      }
    }
    Current_Camera_View = WhoIAm;
    SetFOV( START_FOV );
  }
#endif

  if (lpDev->lpVtbl->EndScene(lpDev) != D3D_OK)
        return FALSE;

#ifdef REFLECTION
  WierdShit();
#endif

  ScreenPolyProcess();
  DispHUDNames();
  DispUntriggeredNMEs();

#ifdef INSIDE_BSP
  Inside = PointInsideSkin( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group );
#endif

  if( Our_CalculateFrameRate() != TRUE)
      return FALSE;

/* Secondary routines called after rendering */
    
  if( !PlayDemo )
    NetworkGameUpdate();

  if( (Ships[WhoIAm].Object.Speed.z) > (MaxMoveSpeed) )
  {
    float diff;
    diff = ( Ships[WhoIAm].Object.Speed.z - MaxMoveSpeed ) / (  MaxTurboSpeed - MaxMoveSpeed );

    if( diff > 0.0F )
    {
      fov_inc += diff * diff * framelag;
      if ( fov_inc > 30.0F )
        fov_inc = 30.0F;
    }
    else
    {
      fov_inc *= (float) pow( 0.95, framelag );
    }
  }
  else
  {
    fov_inc *= (float) pow( 0.95, framelag );
  }
  SetFOV( chosen_fov + fov_inc );

    if( CurrentMenu && CurrentMenuItem )
    {
      MenuDraw( CurrentMenu );

      MenuItemDrawCursor( CurrentMenuItem );

      if ( DrawSimplePanel )
        TestBlt();
// Just to make sure that another press of escape doesnt take you back into the menu you wanted to exit!!
      JustExitedMenu = TRUE;
      
      MenuProcess();
    }
    else
    {
#ifndef REFLECTION
        if( !FullRearView )
          TestBlt();
#endif
    }

  // here is where we process menu keys
  ProcessGameKeys();

  CheckForRogueSfx();

  CheckLevelEnd();

  return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Show In Game Stats...
  Input   :   nothing...
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

void ShowDeathModeStats()
{
	ShowGameStats( BOX_BG );
	if( Ships[ WhoIAm ].Timer < RESPAWN_TIMER )
		CenterPrint4x5Text( "Press any key to continue" , d3dappi.szClient.cy - (FontHeight*2) , GREEN );	
}

/* Display the Statistics in-game when key is pressed */
void ShowInGameStats()
{
	ShowGameStats( NO_BG );	// don't use BLT background
}

void PaintBackground( RECT * box ) // pass NULL to black out all of the screen
{
	HRESULT ddrval;
	DDBLTFX fx;
	memset(&fx, 0, sizeof(DDBLTFX));
	fx.dwSize = sizeof(DDBLTFX);
	while( 1 )
	{
		ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt(
			d3dapp->lpBackBuffer, box, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx 
		);
		if( ddrval == DD_OK )
			break;
		if( ddrval == DDERR_SURFACELOST )
		{
			DebugPrintf("ShowGameStats() Lost surface");
			d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
			d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
			DDReLoadBitmap( lpDDSOne,/* (char*) &ScoreNames[ModeCase] */DynamicScoreNames );
			break;
		}
		if( ddrval != DDERR_WASSTILLDRAWING )
			break;
	}
}

BOOL StatsNamePulse( void )
{
	static float pulse = 0.0F;
	pulse += framelag / 60.0F;

	if (pulse > 1.0F)
		pulse -= (float) floor( (double) pulse );

	if (pulse <= 0.5F)
		return TRUE;

	return FALSE;
}

extern int GetPlayerByRank( int rank );
extern int ReliabilityTab[MAX_PLAYERS+1];
void ShowGameStats( stats_mode_t mode )
{
	int active_players = 0;
	int total_rows = 0;
	int total_height = 0;
	int top_offset = 0;
	int row_height = (FontHeight+(FontHeight/2));
	int x_center = ( d3dappi.szClient.cx >>1 );
	int y_center = ( d3dappi.szClient.cy >>1 );

	// generate active players

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if ( scoreable_status(i) )
			active_players++;
	}

	// get the total height of everything

	total_rows = 9 + (2* active_players); // calculated statically based on the code  SUCKSSSSS
	total_height = (total_rows * row_height);
	top_offset = ( y_center - (total_height / 2) ); // center whole thing

// ROWS += 1

	// paint black background or black box

	if( !bPolyText && mode != NO_BG ) // you can't see anything in polytext mode
	{
		RECT box;
		if( mode == BOX_BG )
		{
			int x_padding = FontWidth*2;
			int y_padding = FontHeight*2;

			// whole width
			box.left = 0;
			box.right = d3dappi.szClient.cx;

			//box.left = left_offset - x_padding;						// begging of first col
			//box.right = column[ ncols-1 ] + col_width + x_padding;	// end of last col

			box.top = top_offset - y_padding;							// begging of first row
			box.bottom = top_offset + total_height + y_padding;			// bottom of last row
		}
		PaintBackground(&box);
	}

	//
	// Kill Matrix
	//

	{
		char* title = "KILL MATRIX";
		Print4x5Text( title, (x_center-((strlen(title)*FontWidth)/2)), top_offset, WHITE );
		top_offset += (row_height * 2);
	}

// ROWS += 2

	{
		int i;

		int col_width = (FontWidth * 3);
		int name_width = (FontWidth * MAXSHORTNAME);

		int line_width = name_width + (col_width * active_players);
		int left_offset = x_center - (line_width / 2); // center

		// print first letter of each name

		for (i = 0; i < active_players; i++)
		{
			// x axis
			int xpos = left_offset + name_width + (i * col_width);

			// create string of first letter of player name
			char FirstLetter[2];
			FirstLetter[0] = Names[GetPlayerByRank(i)][0];
			FirstLetter[1] = 0;

			//
			Print4x5Text( FirstLetter,  xpos, top_offset, player_left(GetPlayerByRank(i)) ? DARKGRAY : GRAY );
		}

		top_offset += row_height;

// ROWS += 1

		// display kill matrix
		for (i = 0; i < active_players; i++)
		{
			// reset value
			int j = 0;
			int xpos = left_offset;

			// print name
			Print4x5Text( (char*) &Names[GetPlayerByRank(i)], xpos, top_offset, GRAY );
			xpos += name_width;

			// print kill matrix line
			for (j = 0; j < active_players; j++)
			{
				int color = player_left(GetPlayerByRank(i)) ? DARKGRAY : ((i==j) ? RED : GRAY);
				Printint16( GetKillStats(GetPlayerByRank(i),GetPlayerByRank(j)), xpos, top_offset, color ); // suacides are red
				xpos+=col_width; // next col
			}

			// next row
			top_offset += row_height;
		}

// ROWS += active players

	}

	top_offset += (row_height * 4);

// ROWS += 4

	//
	// Player Stats
	//

	{
		char* title = "Player Stats";
		Print4x5Text( title, (x_center-((strlen(title)*FontWidth)/2)), top_offset, WHITE );
		top_offset += (row_height * 2);
	}

// ROWS += 2


	{
		int i;

		int col_width = (FontWidth * MAXSHORTNAME);
		int ncols = (TeamGame) ? 7 : 6;
		int line_width = col_width * ncols;
		int left_offset = x_center - (line_width / 2); // center

		{
			int xpos;
			int col;

			// names and colors
			int columns = 6;
			char* names[6] = {"PING", "RATIO", "TEAM", "OVERALL", "KILLS", "DEATHS"};
			int colors[6]  = {  GRAY,   CYAN,  YELLOW,      GRAY,   GREEN,      RED};

			// NOT team game
			if(!TeamGame)
				names[2] = NULL;

			// start at left
			xpos = left_offset;

			// print the names
			for( col = 0; col < columns; col++ )
				if( names[col] != NULL )
					Print4x5Text( names[col], (xpos+=col_width), top_offset, colors[col] );

		}

		top_offset += (row_height * 1);

// ROWS += 1

		for (i = 0; i < active_players; i++)
		{
			// ping
			uint16 ping = 0;

			// start of line
			int xpos = left_offset;

			// if the player left the game
			int left = (player_left(GetPlayerByRank(i)) ? DARKGRAY : 0 );

			// calculate name color
			int color = (left) ? left : ((StatsNamePulse() && i == WhoIAm) ? GRAY : (TeamGame ? TeamCol[TeamNumber[i]] : YELLOW));

			// get the ping
			if( Ships[GetPlayerRank(i)].network_player )
				ping = (uint16) Ships[GetPlayerRank(i)].network_player->ping;

			//
			// print line
			//
			
			// connection status
			// print the dot about 2 character space gap from the names
			if( GameStatus[GetPlayerRank(i)] == STATUS_Normal )
				DisplayConnectionStatus( ReliabilityTab[GetPlayerRank(i)], (xpos-(4*FontWidth)), top_offset );

			// name
			Print4x5Text( (char*)&Names[GetPlayerRank(i)],	xpos,				top_offset, color );

			// add width now so the ping column shows up even if a value printed
			xpos += col_width;

				if( GetPlayerRank(i) != WhoIAm && GameStatus[GetPlayerRank(i)] == STATUS_Normal )
			Printint16( ping,								xpos,				top_offset, (left) ? left : GRAY	);	// ping
			Printint16( GetEffeciency(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset, (left) ? left : CYAN	);	// positives / (positives - negatives)
				if(TeamGame)
			Printint16( GetTeamScore(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset,	(left) ? left : YELLOW	);	// all players (points + kills - suacides - friendly - deaths)
			Printint16( GetRealScore(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset, (left) ? left : GRAY	);	// points + kills - suacides - friendly - deaths
			Printint16( GetKills(GetPlayerByRank(i)),		(xpos+=col_width),	top_offset,	(left) ? left : GREEN	);	// kills - suacides - friendly
			Printint16( GetTotalDeaths(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset,	(left) ? left : RED		);	// suacides + deaths

			// go to next row
			top_offset += row_height;
		}
	}

// ROWS += active players

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Score Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
/* Display the Statistics */
BOOL ScoreDisplay()
{
	ShowGameStats( FULL_BG ); // use BLT background
	CenterPrint4x5Text( "Press Space to continue" , d3dappi.szClient.cy - (FontHeight*2) , 0 );	
	return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init Score Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  InitScoreDisplay()
{
#ifdef SCROLLING_MESSAGES
  int i;
#endif
   //Create the offscreen surface, by loading our bitmap.

  if( ModeCase != -1 )
  {
    lpDDSOne = DDLoadBitmap( d3dapp->lpDD,/* (char*) &ScoreNames[ModeCase]*/ DynamicScoreNames , 0, 0 );
    ddpal =  DDLoadPalette( d3dapp->lpDD , /*(char*) &ScoreNames[ModeCase]*/DynamicScoreNames);
    LastError = lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
  }

  InitFont(TRUE);
  ChangeBackgroundColour( 0, 0, 0 );

#ifdef SCROLLING_MESSAGES
  // get random phrases...
  for (i = 0; i < Num_StatsMessage_Parts; i++)
  {
    StatsMessages[i].current_variation = Random_Range(StatsMessages[i].num_variations);
  }
#endif
  // process scores...
  ScoreSort();

  ScoreDisplaySfx = TRUE;

  PreventFlips = FALSE;

  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Free Score Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  FreeScoreDisplay()
{
  ReleaseDDSurf(lpDDSOne);  
  ReleaseDDSurf(lpDDSTwo);
  lpDDSTwo = NULL;
  lpDDSOne = NULL;
  return TRUE;
}

void ScrollingTeamMessage(char **str, int num_strings, int *col)
{
  static uint16 StartChar = 0;
  static float TotalShift = 0;
  char *message;
  int MaxChars, i;
  uint16 CurrentChar;
  int TotalLength;
  uint16 CurrentMessagePos, ProcessedChars, CurrentCharOffset, CurrentString, CurrentStrLen;
  int xpos;

  TotalShift += 0.5F * framelag;

  TotalLength = 0;
  for (i = 0; i < num_strings; i++)
    TotalLength += strlen( str[i] );

  if (StartChar > (TotalLength - 1))
    StartChar = 0;

  while( TotalShift >= FontWidth )
  {
    StartChar++;
    if (StartChar == TotalLength)
      StartChar = 0;
    TotalShift -= FontWidth;
  }

  MaxChars = (d3dappi.szClient.cx / FontWidth) + 2;

  message = (char *)calloc( MaxChars, sizeof(char) );
  
  CurrentChar = StartChar;
  CurrentMessagePos = 0;
  ProcessedChars = 0;
  xpos = 0 - (int)TotalShift;

  while( ProcessedChars++ <= MaxChars )
  {
    // find out which string CurrentChar is in...
    CurrentCharOffset = 0;
    for (CurrentString = 0; CurrentString < num_strings; CurrentString++)
    {
      if ( CurrentChar < strlen( str[CurrentString] ) + CurrentCharOffset )
      {
        CurrentStrLen = strlen( str[CurrentString] );
        break;
      }
      else
        CurrentCharOffset += strlen( str[CurrentString] );
    }

    // get next character from string, and add to message...
    message[CurrentMessagePos++] = str[CurrentString][CurrentChar++ - CurrentCharOffset];

    if ( (CurrentChar - CurrentCharOffset) == CurrentStrLen )
    {
      // display text so far...
      message[CurrentMessagePos] = 0;
      PrintClipped4x5Text( message, xpos, d3dappi.szClient.cy - (FontHeight*2) , col[CurrentString] );
      xpos += FontWidth * strlen( message );
      message[0] = 0;
      CurrentMessagePos = 0;

      if (CurrentString == num_strings - 1)
        CurrentChar = 0;
    }
  }
  // display remaining text...
  message[CurrentMessagePos] = 0;
  PrintClipped4x5Text( message, xpos, d3dappi.szClient.cy - (FontHeight*2) , col[CurrentString] );

  free (message);
}


#if 0
  RECT rectOverlaySource, rectOverlayDest;
  DDOVERLAYFX ddofx;

  lpDDSOverlay =  DDLoadBitmapOverlay( lpDD, (char*) &PanelNames[0] , 320, 80 );
  DDSetColorKey( lpDDSOverlay, RGB_MAKE( 255 , 0 , 255 ) );
  
    /* Set the overlay source rect */
    SetRect(&rectOverlaySource,0,0,320,80);
    /* Fill the entire display with the overlay */
    SetRect(&rectOverlayDest, 0 , 0 , 320 , 80 );

  memset(&ddofx, 0, sizeof(DDOVERLAYFX));
    ddofx.dwSize = sizeof(DDOVERLAYFX);
    /* Position, set colorkey values, and show the front overlay buffer */
    lpDDSOverlay->lpVtbl->UpdateOverlay( lpDDSOverlay , &rectOverlaySource,
                                      lpDDSOverlay,
                                      &rectOverlayDest,
                                      DDOVER_SHOW | DDOVER_KEYDESTOVERRIDE,
                                      &ddofx);
#endif

#if 0
      memset(&fx, 0, sizeof(DDBLTFX));
      fx.dwSize = sizeof(DDBLTFX);
      fx.dwAlphaSrcConstBitDepth = 8;
      fx.dwAlphaSrcConst = 128;
      fx.dwAlphaDestConstBitDepth = 8;
      fx.dwAlphaDestConst = 255;
      dest.top = d3dappi.szClient.cy-PanelVisibleY[ModeCase];
      dest.bottom = d3dappi.szClient.cy;
      dest.left = 0;
      dest.right = d3dappi.szClient.cx;
#endif
#if 0
        ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSThree, &src, DDBLT_KEYSRC | DDBLT_WAIT | DDBLT_ALPHASRCCONSTOVERRIDE | DDBLT_ALPHADESTCONSTOVERRIDE | DDBLT_ALPHADEST | DDBLT_ALPHASRC , &fx );
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   General blit fast...
  Input   :   int   srcx,srcy , width , height , dstx , dsty
        :   LPDIRECTDRAWSURFACE Surface
        :   char * FileName , LPDIRECTDRAWSURFACE DestSurface
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
_inline
#endif
void GeneralBltFast( int srcx, int srcy , int w , int h  , int dstx , int dsty , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface)
{
    RECT    src;
    HRESULT ddrval;
  src.top = srcy;
  src.left = srcx;
  src.right = src.left+w;
  src.bottom = src.top+h;
  while( 1 )
  {
    ddrval = DestSurface->lpVtbl->BltFast( DestSurface, dstx, dsty, SrcSurface, &src, flags );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
  
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DestSurface->lpVtbl->Restore(DestSurface);
      DDReLoadBitmap( SrcSurface, FileName );
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   General blit fast...
  Input   :   int   srcx,srcy , width , height , dstx , dsty ,dstw , dsth ,
        :   LPDIRECTDRAWSURFACE Surface
        :   char * FileName , LPDIRECTDRAWSURFACE DestSurface
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
//_inline
#endif
void GeneralBlt( int srcx, int srcy , int w , int h  , int dstx , int dsty , int dstw , int dsth , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface)
{
  RECT  dst;
    RECT    src;
  DDBLTFX fx;
    HRESULT ddrval;
  src.top = srcy;
  src.left = srcx;
  src.right = src.left+w;
  src.bottom = src.top+h;

  dst.top = dsty;
  dst.left = dstx;
  dst.right = dst.left+dstw;
  dst.bottom = dst.top+dsth;

    memset(&fx, 0, sizeof(DDBLTFX));
    fx.dwSize = sizeof(DDBLTFX);

  
  while( 1 )
  {
    ddrval = DestSurface->lpVtbl->Blt( DestSurface, &dst, SrcSurface, &src, DDBLT_WAIT | DDBLT_ASYNC , &fx );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DestSurface->lpVtbl->Restore(DestSurface);
      DDReLoadBitmap( SrcSurface, FileName );
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Do font blt..
  Input   :   int sx , int sy , int sw , int sh , int x ,int y
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
_inline
#endif
void DoFontBlt(int sx , int sy , int sw , int sh , int x ,int y)
{
    RECT    src;
    HRESULT ddrval;
  src.left = sx;
  src.top = sy;
  src.right = sx+sw;
  src.bottom = sy+sh;
  while( 1 )
  {
    ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x, y, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      ReInitFont();
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Clear the Zbuffer / screen 
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer )
{
    int clearflags;
    D3DRECT dummy;
    if (!d3dappi.bRenderingIsOK)
  {
        return FALSE;
    }
    /*
     * Decided wether to clear just back buffer or also z-buffer
     */

  clearflags = 0;


  if( myglobs.bClearsOn || ClearScreen )  // toggle clearing the screen...
  {
    clearflags |= D3DCLEAR_TARGET;
  }

#ifdef Z_TRICK
  if ( ZClearsOn || ClearZBuffer )  // never clear Z buffer unless told to...
#else
  if (d3dapprs.bZBufferOn || ClearZBuffer ) // If a ZBuffer is enabled then always clear it..
#endif
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }

  if( clearflags != 0 )
  {
      dummy.x1 = CurrentCamera.Viewport.dwX;
      dummy.y1 = CurrentCamera.Viewport.dwY;
      dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth;
      dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight;
   
    LastError =
              d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                                   1, &dummy,
                                                   clearflags);
    if (LastError != D3D_OK)
    {
        return FALSE;
    }
  }
    return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Clear the Zbuffer
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  ClearZBuffer()
{
    int clearflags;
    D3DRECT dummy;

    if (!d3dappi.bRenderingIsOK) return FALSE;

  clearflags = D3DCLEAR_ZBUFFER;

  dummy.x1 = CurrentCamera.Viewport.dwX;
  dummy.y1 = CurrentCamera.Viewport.dwY;
  dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth;
  dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight;

  LastError = d3dappi.lpD3DViewport->lpVtbl->Clear( d3dappi.lpD3DViewport, 1, &dummy, clearflags );
  if (LastError != D3D_OK) return FALSE;
    return TRUE;
}


void InitRenderBufs( LPDIRECT3DDEVICE lpDev )
{
  int Count;
  D3DEXECUTEBUFFERDESC debdesc;

  for( Count = 0; Count < 2; Count++ )
  {
    if( RenderBufs[ Count ] != NULL )
    {
      XRELEASE( RenderBufs[ Count ] );
      RenderBufs[ Count ] = NULL;
    }
    MakeExecuteBuffer( &debdesc, lpDev, &RenderBufs[ Count ], 32767 );
  }
}


void ReleaseRenderBufs( void )
{
  int Count;

  for( Count = 0; Count < 2; Count++ )
  {
    if( RenderBufs[ Count ] != NULL )
    {
      XRELEASE( RenderBufs[ Count ] );
      RenderBufs[ Count ] = NULL;
    }
  }
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Render 1 Frame Using CurrentCamera...
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  BOOL  ClearScrOverride;
  BOOL  ClearZOverride;

BOOL  RenderCurrentCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
    HRESULT rval;
  int16 Count;
  VISGROUP  *g;
  uint16  group;
  uint16  i;
  float R, G, B;
  NumOfTransExe = 0;

  Build_View();
  CurrentCamera.View = view;
  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &CurrentCamera.Viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera1", &CurrentCamera.Viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }


  // Ship Model Enable/Disable
  SetShipsVisibleFlag();

  // translucent poly sort
  InitPolySort();

  // find visible groups
  FindVisible( &CurrentCamera, &Mloadheader );

  BuildVisibleLightList( CurrentCamera.GroupImIn );
  
  UpdateBGObjectsClipGroup( &CurrentCamera );
  UpdateEnemiesClipGroup( &CurrentCamera );

  ClearScrOverride = FALSE;
  ClearZOverride = FALSE;
  if( CurrentCamera.GroupImIn != (uint16) -1 && !DebugVisible )
  {
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
      if( Mloadheader.Group[ g->group ].BGClear_Flag )
      {
        if ( Mloadheader.Group[ g->group ].BGClear_Flag & 0x80 )
        {
          R = Mloadheader.Group[ g->group ].BGClear_Red;
          G = Mloadheader.Group[ g->group ].BGClear_Green;
          B = Mloadheader.Group[ g->group ].BGClear_Blue;
          ChangeBackgroundColour( R, G, B );
          ClearScrOverride = TRUE;
        }
        break;
      }
    }
  }

  if (ClearBuffers( ClearScrOverride, ClearZOverride ) != TRUE )
    return FALSE;

  // reset all the normal execute status flags...
  lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);

  // set all the Translucent execute status flags...
  if( WhiteOut != 0.0F)
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DSpcFxTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }


  // display background
  if ( !DisplayBackground( &Mloadheader, &CurrentCamera ) )
    return FALSE;

  // reset all the normal execute status flags...
  if( WhiteOut == 0.0F)
        lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !DisplaySolidGroupUnclippedPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;

#ifdef SHADOWTEST
    if( !DisplaySolidGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;
#endif
  
  // display clipped opaque objects
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;

    // Do the Background animation for that group.....
    BackGroundTextureAnimation( &Mloadheader , group );

#ifdef CLIP_LINES
    ClipGroup( &CurrentCamera, group );
#else
    ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );
#endif

#ifndef FINAL_RELEASE
//    if( d3dapp->bIsPrimary )
    {
      i = FirstLineUsed;
      while( i != (uint16) -1 )
      {
        if( LinesDispGroup( group, RenderBufs[ 0 ], &i ) )
        {
          if( lpDev->lpVtbl->Execute(lpDev, RenderBufs[ 0 ], lpView, D3DEXECUTE_CLIPPED ) != D3D_OK )
            return FALSE;
        }
      }
    }
#endif

    ClipGroup( &CurrentCamera, group );

      if( !ModelDisp( group, lpDev, &ModelNames[0] ) )
        return FALSE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

  if( !DisplaySolidGroupClippedPolys( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;
#ifdef SHADOWTEST
  if( !DisplaySolidGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;
#endif

//    if (Mod_Ship_Exec_Buffer( group, lpDev, lpView ) != TRUE)
//      return FALSE;
  }

  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );
//  DisplayWaterMesh();

  // set all the Translucent execute status flags...
  lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Skin & BSP Node Info
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( ShowSkin || OldNodeCube || NodeCube || ShowTrigZones || ShowColZones || ShowEFZones || ShowTeleports )
  {
    if( OldNodeCube || NodeCube ) ClearZBuffer();

    for( Count = 0; Count < MAXGROUPS; Count++ )
    {
      if( Skin_Execs[ Count ] != NULL )
      {
          if (lpDev->lpVtbl->Execute(lpDev, Skin_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED) != D3D_OK)
            return FALSE;
      }
    }

#if 0
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
      group = g->group;
      ClipGroup( &CurrentCamera, group );
  
//      if( d3dapp->bIsPrimary )
      {
        i = FirstLineUsed;
        while( i != (uint16) -1 )
        {
          if( LinesDispGroup( group, RenderBufs[ 0 ], &i ) )
          {
            if( lpDev->lpVtbl->Execute(lpDev, RenderBufs[ 0 ], lpView, D3DEXECUTE_CLIPPED ) != D3D_OK )
              return FALSE;
          }
        }
      }
    }
#endif

  }

  lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);

  // display clipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ClipGroup( &CurrentCamera, group );
    GroupWaterProcessDisplay( group );



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

  if( !DisplayGroupClippedPolys( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;

#ifdef SHADOWTEST
  if( !DisplayGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
Display Group Clipped Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( !DisplayGroupClippedFmPolys( RenderBufs[ 0 ], group, lpDev, lpView ) )
      return FALSE;

  ExecuteTransExe( group );

  }


  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

    if( !DisplayGroupUnclippedFmPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !DisplayGroupUnclippedPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;
#ifdef SHADOWTEST
    if( !DisplayGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
      return FALSE;
#endif
  
  // display unclipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ExecuteTransExeUnclipped( group );
  }


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Portals
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( ShowPortal )
  {
    for( Count = 0; Count < MAXGROUPS; Count++ )
    {
      if( Portal_Execs[ Count ] != NULL )
      {
          if (lpDev->lpVtbl->Execute(lpDev, Portal_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED) != D3D_OK)
            return FALSE;
      }
    }
  }
  
  DoLensflareEffect();
  DoAllSecBullLensflare();

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Transluecent Screen Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

    if( !DisplayNonSolidScrPolys( RenderBufs[ 1 ], lpDev, lpView ) )
      return FALSE;

  // reset all the normal execute status flags...
  lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Solid Screen Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  BilinearSolidScrPolys = FALSE;

  if( !DisplaySolidScrPolys( RenderBufs[ 1 ], lpDev, lpView ) )
    return FALSE;

  rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera2", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }

  return TRUE;
}
  

extern  int   NumOfVertsConsidered;
extern  int   NumOfVertsTouched;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Our Calculate frame rate...
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

timer our_timer;
int our_count = 0;

#ifdef DEBUG_ON
int our_last_polygons = 0;
#endif

BOOL Our_CalculateFrameRate(void)
{
	int polygons;
	D3DSTATS stats;
	char buf[256];
	static int avg_time_per_frame = 0;

	// every 25 frames
	our_count++;
	if (our_count >= 120) // every 2 seconds on vsync pc
	{
		// calculate time passed
		timer_run( &our_timer );

		// first time running so bank
		if ( our_timer.seconds != 0.0F )
		{

			// ask d3d for stats
			memset(&stats, 0, sizeof(D3DSTATS));
			stats.dwSize = sizeof(D3DSTATS);
			d3dapp->lpD3DDevice->lpVtbl->GetStats( d3dapp->lpD3DDevice, &stats);

#ifdef DEBUG_ON
			// calculate average triangels per second
			polygons = stats.dwTrianglesDrawn - our_last_polygons;
			our_last_polygons = polygons;
			TPS = (long)(polygons / our_timer.seconds);
#endif

			// calculate average frames per second
			FPS = (float) our_count / (float) our_timer.seconds;

			// average time per frame in milliseconds
			avg_time_per_frame = (int)((1.0F / FPS) * 1000.0F);

			our_count = 0;

		}
	}
  
	// display the framerate
	if( myglobs.bShowFrameRate )
	{
#ifdef DEBUG_ON
		sprintf(&buf[0], "FPS %d - AVG F %d MS - TPS %d", (int) FPS, avg_time_per_frame, (int) TPS );
#else
		sprintf(&buf[0], "FPS %d - AVG F %d MS", (int) FPS, avg_time_per_frame );
#endif
		CenterPrint4x5Text( (char *) &buf[0] , FontHeight, 2 );
	}

	if( myglobs.bShowInfo )
	{

		// group information
		sprintf(&buf[0], "Groups Visible %d - Current Group %s", (int) NumGroupsVisible,
			(GroupImIn == (uint16) -1) ? "(outside)" : Mloadheader.Group[GroupImIn].name );
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*3, 2 );

		// memory information
		sprintf(&buf[0], "Mem %d - ExecMem %d - vidBefore %d - vidAfter %d",
			(int)MemUsed, (int)ExecMemUsed,
			Tloadheader.VidMemBefore, Tloadheader.VidMemAfter );
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*4, 2 );

		// sound memory info
		sprintf(&buf[0], "SoundMemUsedHW %d (max %d) - SoundMemUsedSW %d (max %d)",
			SBufferMemUsedHW, MaxSBufferMemUsedHW,
			SBufferMemUsedSW, MaxSBufferMemUsedSW );
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*5, 2 );

		// show polygon information
		sprintf(&buf[0], "FmPolys %d - Polys %d - ScrPolys %d - VertsTouched %d",
			(int) TotalFmPolysInUse,(int) TotalPolysInUse,(int) TotalScrPolysInUse, NumOfVertsTouched);
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*6, 2 );

		// ???
/*
		sprintf( &buf[0] , "Clear=%c%c", ClearScrOverride ? 'S' : ' ', ClearZOverride ? 'Z' : ' ' );
#ifdef INSIDE_BSP
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*8, (!Inside) ? 1 : 2 );
#else
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*8, (outside_group) ? 1 : 2 );
#endif
*/

		if ( ! ShowWeaponKills ) //ShowNetworkInfo)
		{

			// newtork info
			sprintf( &buf[0], "Network" );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*8, 2 );

			sprintf( &buf[0], "BPS %5d CurSent %5d MaxRec %5d MaxSent %5d",
				CurrentBytesPerSecRec , CurrentBytesPerSecSent , MaxCurrentBytesPerSecRec ,MaxCurrentBytesPerSecSent );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*9, 2 );

			sprintf( &buf[0], "ReceiveSize %4d MaxReceiveSize %4d",
				RecPacketSize , MaxRecPacketSize );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*10, 2 );

		}

		if (ShowWeaponKills)
		{
			// show all primary weapon kills
			for( i = 0 ; i < MAXPRIMARYWEAPONS+1 ; i++ )
			{
				// display primary weapon name
				Print4x5Text( GetWeaponName(WEPTYPE_Primary,i),	(d3dappi.szClient.cx>>1)-(11*FontWidth),	(viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
				// display primary weapon kills
				Printuint16( GetWeaponKillStats(WhoIAm,WEPTYPE_Primary,i),	(d3dappi.szClient.cx>>1)-(15*FontWidth), (viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
			}

			// show all secondary weapon kills
			for( i = 0 ; i < TOTALSECONDARYWEAPONS ; i++ )
			{		
				// display secondary weapon name
				Print4x5Text( GetWeaponName(WEPTYPE_Secondary,i),	(d3dappi.szClient.cx>>1)+(5*FontWidth),	(viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
				// display secondary weapon kills
				Printuint16( GetWeaponKillStats(WhoIAm,WEPTYPE_Secondary,i) , (d3dappi.szClient.cx>>1)+(1*FontWidth),	(viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
			}
		}

	}

    // some stupid place for a demo calculation
	if( MyGameStatus == STATUS_PlayingDemo )
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &DemoEndedTime);
		TimeDiff = DemoEndedTime - DemoStartedTime;
		DemoTotalTime = ( (float) TimeDiff / (float) Freq );
		DemoAvgFps = DemoGameLoops / DemoTotalTime;
	}

	return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Disp3d Panel..
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  Disp3dPanel( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  VECTOR  Trans;
  MATRIX  Matrix;
  MATRIX  rotMatrix;
  MATRIX  finalMatrix;
  VECTOR  Pos;
  VECTOR  Temp;
  VECTOR  Scale;
    int clearflags;
    D3DRECT dummy;
  D3DVIEWPORT newviewport;
  float screen_width, screen_height;


//  return TRUE;

    newviewport.dwSize = sizeof(D3DVIEWPORT);
    newviewport.dwX = 0;
  newviewport.dwY = 0;
    newviewport.dwWidth = d3dapp->szClient.cx;
    newviewport.dwHeight = d3dapp->szClient.cy;
    newviewport.dvScaleX = newviewport.dwWidth / (float)2.0;
    newviewport.dvScaleY = newviewport.dwHeight / (float)2.0;
    newviewport.dvMaxX = (float)D3DDivide(D3DVAL(newviewport.dwWidth),
                                       D3DVAL(2 * newviewport.dvScaleX));
    newviewport.dvMaxY = (float)D3DDivide(D3DVAL(newviewport.dwHeight),
                                       D3DVAL(2 * newviewport.dvScaleY));
  
    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &newviewport) != D3D_OK )
    return FALSE;

  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;
  viewplane_distance = (float) ( newviewport.dwWidth / ( 2 * tan( DEG2RAD( normal_fov ) * 0.5 ) ) );
  panelproj._11 = 2 * viewplane_distance / newviewport.dwWidth;
  panelproj._22 = 2 * viewplane_distance / ( newviewport.dwHeight / pixel_aspect_ratio );

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &panelproj) != D3D_OK)
  {
      return FALSE;
  }

  
  
  clearflags = 0;
  if (d3dapprs.bZBufferOn != 0 )      // If a ZBuffer is enabled then always clear it..
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }
  if( clearflags != 0 )
  {
    dummy.x1 = newviewport.dwX;
    dummy.x2 = newviewport.dwX+newviewport.dwWidth;

    dummy.y1 = newviewport.dwY + ( newviewport.dwHeight >> 1 );
    dummy.y2 = dummy.y1+( newviewport.dwHeight >> 1);

    if( d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                             1, &dummy,
                                             clearflags) != D3D_OK )
                         return FALSE;
  }

  Trans.x = 0.0F;
    Trans.y = -180.0F;
  Trans.z = 280.0F;

  Trans.x -= ( ( Ships[WhoIAm].Object.Speed.x * -4.0F ) + ( Ships[WhoIAm].Object.Angle.y * 8.0F ) );
  Trans.y -= ( ( Ships[WhoIAm].Object.Speed.y * -2.0F ) + ( Ships[WhoIAm].Object.Angle.x * 4.0F ) );
  Trans.z += ( Ships[WhoIAm].Object.Speed.z * 4.0F );
  

  Pos.x = -( ( Ships[WhoIAm].Object.Speed.x * -4.0F ) + ( Ships[WhoIAm].Object.Angle.y * 8.0F ) );
  Pos.y = -( ( Ships[WhoIAm].Object.Speed.y * -2.0F ) + ( Ships[WhoIAm].Object.Angle.x * 4.0F ) );
  Pos.z = +( Ships[WhoIAm].Object.Speed.z * 4.0F );

  ApplyMatrix( &MainCamera.Mat, &Pos, &Temp );

  Temp.x += MainCamera.Pos.x;
  Temp.y += MainCamera.Pos.y;
  Temp.z += MainCamera.Pos.z;

  Matrix = MainCamera.Mat;
  Matrix._41 = Temp.x;
  Matrix._42 = Temp.y;
  Matrix._43 = Temp.z;

  Scale.x = 0.5F;
  Scale.y = 0.5F;
  Scale.z = 0.5F;
  ScaleMatrix( &Matrix, &Scale );
  

  BuildRotMatrix( Ships[WhoIAm].Object.Angle.x, Ships[WhoIAm].Object.Angle.y, Ships[WhoIAm].Object.Angle.y+Ships[WhoIAm].Object.Angle.z, &rotMatrix);


  MatrixMultiply( &Matrix, &rotMatrix, &finalMatrix );

//  XLightMxloadHeader( &ModelHeaders[MODEL_Panel] , &Temp , SHIP_RADIUS*4.0F , &finalMatrix );
  XLightMxloadHeader( &ModelHeaders[MODEL_Eyeball] , &Temp , SHIP_RADIUS*4.0F , &finalMatrix );
  
  
  
//  view = identity;
  view._41 = Trans.x;
  view._42 = Trans.y;
  view._43 = Trans.z;
  view._11 = rotMatrix._11; 
  view._12 = rotMatrix._12;
  view._13 = rotMatrix._13;
  view._14 = rotMatrix._14;
             
  view._21 = rotMatrix._21;
  view._22 = rotMatrix._22;
  view._23 = rotMatrix._23;
  view._24 = rotMatrix._24;
             
  view._31 = rotMatrix._31;
  view._32 = rotMatrix._32;
  view._33 = rotMatrix._33;
  view._34 = rotMatrix._34;
                                        
  view._44 = rotMatrix._44;

  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

//  if (ExecuteMxloadHeader( &ModelHeaders[MODEL_Panel], (uint16) -1 ) != TRUE )
  if (ExecuteMxloadHeader( &ModelHeaders[MODEL_Eyeball], (uint16) -1 ) != TRUE )
    return FALSE;

    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport) != D3D_OK )
    return FALSE;

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }


  return TRUE;
}





#if 0
        if( ModeCase != -1 )
        {
          //  Blt Panel
          GeneralBltFast( 0 , 0 , PanelVisibleX[ModeCase] , PanelVisibleY[ModeCase]  , 0  , d3dappi.szClient.cy-PanelVisibleY[ModeCase] ,
                  lpDDSThree , (char*) &PanelNames[ModeCase] , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
          //  Primary one
          GeneralBltFast( PrimaryX[Ships[WhoIAm].Primary] , PrimaryY[Ships[WhoIAm].Primary] , 48 , 32  , PrimaryWeaponShowX , PrimaryWeaponShowY ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
          
          HealthCount+=  framelag;
          //  Health one
          src.top = ( ( (int) (HealthCount * 0.25F) ) & 15 ) * 8;
          src.left = 0;
          src.right = 32;
          src.bottom = src.top+8;
      
          memset(&fx, 0, sizeof(DDBLTFX));
          fx.dwSize = sizeof(DDBLTFX);
          dest.top = ( d3dappi.szClient.cy-10 )- ( Ships[WhoIAm].Shield >> 3 );
          dest.bottom = d3dappi.szClient.cy-10;
          dest.left = (d3dappi.szClient.cx >> 1) - 16 - 4;
          dest.right = (d3dappi.szClient.cx >> 1) - 4;
          
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_KEYSRC | DDBLT_WAIT , &fx );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
      
          //  Hull one
          src.top = ( ( (int) ( (HealthCount * 0.25F) + 8 ) ^ 15 ) & 15 ) * 8;
          src.left = 0;
          src.right = 32;
          src.bottom = src.top+8;
      
          memset(&fx, 0, sizeof(DDBLTFX));
          fx.dwSize = sizeof(DDBLTFX);
          dest.top = ( d3dappi.szClient.cy-10 )- ( Ships[WhoIAm].Hull >> 3 );
          dest.bottom = d3dappi.szClient.cy-10;
          dest.left = (d3dappi.szClient.cx >> 1) + 4;
          dest.right = (d3dappi.szClient.cx >> 1) + 4 + 16;
          
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_KEYSRC | DDBLT_WAIT , &fx );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
          //  Powerpods
          src.top = 0;
          src.left = 32+64;
          src.right = src.left+PowerSizes[Ships[WhoIAm].PowerLevel];
          src.bottom = src.top+8;
          y =  d3dappi.szClient.cy-74;
          x =  ( d3dappi.szClient.cx >> 1 ) - 28;
      
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x, y, lpDDSOne, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
        }
        
        //  Blt Score
        Printuint16( Ships[WhoIAm].Shield , PanelShieldPosX, PanelShieldPosY , 2 );
        Printuint16( Ships[WhoIAm].Hull , PanelHullPosX, PanelHullPosY , 2 );
        // Blt Primary
        if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
        {
          energy =  (int) PyroliteAmmo;
        }else{
          if( Ships[WhoIAm].Primary == SUSS_GUN )
          {
            energy = (int) SussGunAmmo;
          }else{
            energy = (int) GeneralAmmo;
          }
        }
        Printuint16( (uint16) energy , ( d3dappi.szClient.cx >> 1) - 64 , d3dappi.szClient.cy-( 48 - FontHeight - (FontHeight>>1) ), 2  );
      
        DoFontBlt( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight , PrimaryWeaponTextX , PrimaryWeaponTextY   );
        // Blt Secondary
        DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight ,SecondaryWeaponTextX - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) , SecondaryWeaponTextY );
        Printuint16( (uint16) GetCurSecAmmo() , SecondaryWeaponNumX - ( 4*FontWidth)  , SecondaryWeaponNumY + (FontHeight + (FontHeight>>1) ) , 2 );
        // blt shld
        DoFontBlt( FontWidth*28 , 0 , FontWidth*4 , FontHeight , PanelShieldTextPosX , PanelShieldTextPosY );
        // blt hull
        DoFontBlt( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight , PanelHullTextPosX , PanelHullTextPosY );
#endif
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Init View port without calling initview..
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
InitViewport( float scale )
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;

    /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;

  DrawSimplePanel = FALSE;
  if( scale < 1.01F )
    DrawSimplePanel = TRUE;

  if( DrawSimplePanel )
  {
    maxheight = d3dapp->szClient.cy - PanelVisibleY[ModeCase];
  }else{
    maxheight = d3dapp->szClient.cy;
  }
  if ( scale < 1.01F )
  {
    width = (int) floor( maxwidth * scale );
    if ( width < MIN_VIEWPORT_WIDTH )
    {
      width = MIN_VIEWPORT_WIDTH;
      scale = (float) width / maxwidth;
    }
    height = (int) floor( maxheight * scale );
    DrawPanel = FALSE;
    left = ( ( maxwidth - width ) >> 1 ) & ~1;
    top = ( ( maxheight - height ) >> 1 ) & ~1;
  }
  else
  {
    width = maxwidth;
    height = maxheight;
    if ( scale > 1.11F )
    {
      DrawPanel = TRUE;
      scale = 1.2F;
    }
    else
    {
      DrawPanel = FALSE;
    }
    left = 0;
    top = 0;
  }
  CurrentViewportScale = scale;

    viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "InitViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );

  // clear viewport
  D3DAppIClearBuffers();

  return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Plot Simple Panel...
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void  PlotSimplePanel( void )
{
  int energy;

  // Blt Primary
  if( ( Ships[WhoIAm].Primary != PrimaryChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    PrimaryChanged = Ships[WhoIAm].Primary;
    GeneralBlt( ( Ships[WhoIAm].Primary & 3 ) * 64 ,
            ( ( Ships[WhoIAm].Primary >> 2 )& 3 ) * 64 ,
          64 , 64 , PrimaryWeaponShowX , PrimaryWeaponShowY ,
          WeaponSizeX , WeaponSizeY ,
          lpDDSFour , "data\\pictures\\pcontent.bmp" , 0 , lpDDSThree);

    GeneralBltFast( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary  , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight ,
              PrimaryWeaponTextX , PrimaryWeaponTextY , lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
             DDBLTFAST_WAIT, lpDDSThree);
  }
  
  

  // Blt Secondary
  if( ( Ships[WhoIAm].Secondary != SecondaryChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    SecondaryChanged = Ships[WhoIAm].Secondary;
    GeneralBlt( ( (Ships[WhoIAm].Secondary+6) & 3 ) * 64 ,
            ( ( (Ships[WhoIAm].Secondary+6) >> 2 )& 3 ) * 64 ,
          64 , 64 , SecondaryWeaponShowX , SecondaryWeaponShowY ,
          WeaponSizeX , WeaponSizeY ,
          lpDDSFour , "data\\pictures\\pcontent.bmp" , 0 , lpDDSThree);
    GeneralBltFast( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary  , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight ,
              SecondaryWeaponTextX , SecondaryWeaponTextY , lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_WAIT, lpDDSThree);
  }


  // blt shld
  if( ReMakeSimplePanel == TRUE )
  {
    GeneralBltFast( FontWidth*28 , 0 , FontWidth*4 , FontHeight  , PanelShieldTextPosX , PanelShieldTextPosY ,
            lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, lpDDSThree);
  }
  if( ( Ships[WhoIAm].Object.Shield != ShieldChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    ShieldChanged = (uint16) Ships[WhoIAm].Object.Shield;

    Printuint16AnySurface( (uint16) Ships[WhoIAm].Object.Shield , PanelShieldPosX, PanelShieldPosY , 2 , DDBLTFAST_WAIT, lpDDSThree );
    
    if( ShieldHit == 0 )
    {
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Shield *0.5F) , 24 , 56 , 3 , PanelShieldBarPosX , PanelShieldBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT, lpDDSThree);
    }else{
      ShieldHit -=1;
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Shield *0.5F) , 24 + ((ShieldHit>>2)*8) , 56 , 3  , PanelShieldBarPosX , PanelShieldBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT , lpDDSThree);
      ShieldChanged = (uint16)-1;
    }
  }
  // blt Hull
  if( ReMakeSimplePanel == TRUE )
  {
    GeneralBltFast( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight  , PanelHullTextPosX , PanelHullTextPosY ,
            lpDDSTwo , 
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, lpDDSThree);
  }

  if( ( Ships[WhoIAm].Object.Hull != HullChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    HullChanged = (uint16)Ships[WhoIAm].Object.Hull;
    Printuint16AnySurface( (uint16) Ships[WhoIAm].Object.Hull , PanelHullPosX, PanelHullPosY , 2 , DDBLTFAST_WAIT, lpDDSThree);
    if( HullHit == 0 )
    {
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Hull *0.5F) , 24 , 56 , 3  , PanelHullBarPosX , PanelHullBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT, lpDDSThree);
    }else{
      HullHit -=1;
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Hull *0.5F) , 24 + ((HullHit>>2)*8) , 56 , 3  , PanelHullBarPosX , PanelHullBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT , lpDDSThree);
      HullChanged = (uint16)-1;
    }
  }


  // Blt Power Pods
  if( ( PowerChanged != Ships[WhoIAm].Object.PowerLevel ) || ( ReMakeSimplePanel == TRUE ) )
  {
    GeneralBltFast( FontWidth*(32-7) , FontHeight*(7+Ships[WhoIAm].Object.PowerLevel) , FontWidth*7 , FontHeight  , PanelPowerPosX , PanelPowerPosY ,
            lpDDSTwo , 
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_WAIT, lpDDSThree);

    PowerChanged = Ships[WhoIAm].Object.PowerLevel;
  }
  
  //  Blt Primary ammo
  if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
  {
    energy =  (int) PyroliteAmmo;
  }else{
    if( Ships[WhoIAm].Primary == SUSS_GUN )
    {
      energy = (int) SussGunAmmo;
    }else{
      energy = (int) GeneralAmmo;
    }
  }
  if( ( energy != PrimaryNumChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    PrimaryNumChanged = energy;
    Printuint16AnySurface( (uint16) energy , PrimaryWeaponNumX , PrimaryWeaponNumY , 2 , DDBLTFAST_WAIT, lpDDSThree);
  }

  // Blt Secondary ammo
  if( ( (uint16) GetCurSecAmmo() != SecondaryNumChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    SecondaryNumChanged = (uint16) GetCurSecAmmo();
      Printuint16AnySurface( (uint16) GetCurSecAmmo() , SecondaryWeaponNumX , SecondaryWeaponNumY , 2 , DDBLTFAST_WAIT, lpDDSThree );
  }
  
  //  Blt Panel
  GeneralBltFast( 0 , 0 , PanelVisibleX[ModeCase] , PanelVisibleY[ModeCase]  , 0  , d3dappi.szClient.cy-PanelVisibleY[ModeCase] ,
          lpDDSThree , (char*) &PanelNames[ModeCase] , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);


  

  energy = (int) GetBestMine();
  // Blt Mine..
  if( energy != 65535 )
  {
    DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*energy , FontWidth*SecondaryLengths[energy] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) , FontHeight  );
    Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
  }

  ReMakeSimplePanel = FALSE;
}

void  FlipToGDISurface()
{
  d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Make Specific Cmd Buffers...
  Input   :   nothing..
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
InitSpecialExecBufs( void )
{
    LPVOID lpBufStart, lpInsStart, lpPointer;
    size_t size;

  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;


  size = 0;
  size += sizeof(D3DSTATE) * 9;
    size += sizeof(D3DINSTRUCTION) * 2;
  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DNormCmdBuf , size ) != TRUE ) return FALSE;
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DNormCmdBuf->lpVtbl->Lock(lpD3DNormCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;

	//  this is for a normal non translucent buffers....
	OP_STATE_RENDER( 8, lpPointer);
    STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, FALSE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, TRUE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , FALSE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO, lpPointer );
	STATE_DATA( D3DRENDERSTATE_WRAPU, FALSE, lpPointer );
	STATE_DATA( D3DRENDERSTATE_WRAPV, FALSE, lpPointer );
    OP_EXIT(lpPointer);
  
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DNormCmdBuf->lpVtbl->Unlock(lpD3DNormCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DNormCmdBuf->lpVtbl->SetExecuteData(lpD3DNormCmdBuf, &d3dExData);


  
  // Start of Trans exec Buffer...
  
  size = 0;

  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( !UsedStippledAlpha )
    {
      size += sizeof(D3DSTATE) * 6;
    }else{
      size += sizeof(D3DSTATE) * 1;
    }
  }else{
    size += sizeof(D3DSTATE) * 1;
  }
    size += sizeof(D3DINSTRUCTION) * 2;

  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DTransCmdBuf , size ) != TRUE ) return FALSE;
  
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DTransCmdBuf->lpVtbl->Lock(lpD3DTransCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;

  //  this is for Translucent buffers....
  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( UsedStippledAlpha != TRUE )
    {

      OP_STATE_RENDER( 5, lpPointer);

#if ACTUAL_TRANS
      STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, TRUE, lpPointer );
#else
      STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, FALSE, lpPointer );
#endif

      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_SRCBLEND, CurrentSrcBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_DESTBLEND, CurrentDestBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, CurrentTextureBlend, lpPointer );

    }
    else
    {
      OP_STATE_RENDER( 1, lpPointer);
//      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, FALSE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, FALSE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , TRUE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR, lpPointer );
    }
  }
  else
  {
    OP_STATE_RENDER( 1, lpPointer);
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
  }

    OP_EXIT(lpPointer);
  
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DTransCmdBuf->lpVtbl->Unlock(lpD3DTransCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DTransCmdBuf->lpVtbl->SetExecuteData(lpD3DTransCmdBuf, &d3dExData);

  // Start of SpcFxTrans exec Buffer...
  
  size = 0;

  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( !UsedStippledAlpha )
    {
      size += sizeof(D3DSTATE) * 5;
    }else{
      size += sizeof(D3DSTATE) * 1;
    }
  }else{
    size += sizeof(D3DSTATE) * 1;
  }

    size += sizeof(D3DINSTRUCTION) * 2;
  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DSpcFxTransCmdBuf , size ) != TRUE ) return FALSE;
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DSpcFxTransCmdBuf->lpVtbl->Lock(lpD3DSpcFxTransCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;


  //  this is for SpcFxTranslucent buffers....
  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( UsedStippledAlpha != TRUE )
    {
      OP_STATE_RENDER( 4, lpPointer);
      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_SRCBLEND, CurrentSrcBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_DESTBLEND, CurrentDestBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, CurrentTextureBlend, lpPointer );
    }
    else
    {
      OP_STATE_RENDER( 1, lpPointer);
      STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , TRUE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR, lpPointer );
    }
  }
  else
  {
    OP_STATE_RENDER( 1, lpPointer);
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
  }

    OP_EXIT(lpPointer);
 
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DSpcFxTransCmdBuf->lpVtbl->Unlock(lpD3DSpcFxTransCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DSpcFxTransCmdBuf->lpVtbl->SetExecuteData(lpD3DSpcFxTransCmdBuf, &d3dExData);

  
  return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Release Specific Cmd Buffers...
  Input   :   nothing..
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

void ReleaseSpecialExecBufs( void )
{
  if( lpD3DNormCmdBuf )
  {
    XRELEASE(lpD3DNormCmdBuf);
    lpD3DNormCmdBuf = NULL;
  }
  if( lpD3DTransCmdBuf )
  {
    XRELEASE(lpD3DTransCmdBuf);
    lpD3DTransCmdBuf = NULL;
  }
  if( lpD3DSpcFxTransCmdBuf )
  {
    XRELEASE(lpD3DSpcFxTransCmdBuf);
    lpD3DSpcFxTransCmdBuf = NULL;
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Special Destroy game for dplay fuck up..
  Input   :   nothing..
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void SpecialDestroyGame( void )
{
	IMustQuit = FALSE;

	DebugPrintf("SpecialDestroyGame()\n");

	if ( PlayDemo )
		PlayDemo = FALSE;

	switch( MyGameStatus )
	{
	case STATUS_WaitingToStartTeamGame:
	case STATUS_StartingMultiplayer:
	case STATUS_GetPlayerNum:
		MyGameStatus = STATUS_Title;
		MenuRestart( &MENU_ForceAbort );
		break;
	default:
		PreventFlips = FALSE;
		MyGameStatus = STATUS_QuitCurrentGame;
		break;
	}

	network_cleanup();
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Calculate the framelag..
  Input   :   nothing..
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CalculateFramelag( void )
{

  // forsaken multiplies seconds * 71.0F
  // the idea of multiplying by anything here would be a base conversion
  // such as from seconds into ms or ns to instead of a long fraction

  // but 71.0F is not a power of 10 so this has really bad affects
  // you can end up with exponential varying results on diff systems

  // one loop sample on a 2GHZ P4 with 768MB of ram and an nivdia vanta
  // 0.00744508 * 71  = 0.52860068

  // lets say that computer B taks 50% longer to compute the game loop
  // 0.00744508 * 1.5 = 0.01116762

  // that means the calculation look like the following for computer B
  // 0.01116762 * 71  = 0.80406864

  // now our game loop timing is being tweaked differently on both pc's
  // the problem is where our game loop has a real difference of:
  // 0.00372254 seconds 1.5% apart

  // they are now this far apart
  // 0.27546796 seconds 1.5211267605633802816901408450704%

  // resulting in a drift per loop of
  // 0.27546796 - 0.00372254 = 0.27174542 seconds

  // now to get the number of loops in 1 second on the slower pc
  // 1 / 0.01116762 = 89.544594103309389108870108402686

  // the slower pc is adding the drift 89.5 times per second
  // 89.544594103309389108870108402686 * 0.27174542 = 24.333333333333333333333333333332

  // thus  internal clock between the two pc's will be altered by 24.3
  // with the slower pc being *ahead* of the faster one in game time
  
  // not sure on the meaning of the math here but it looks like
  // this should represent a change of +0.18th of a second
  // 24.3 * 0.007 = 0.18225

  // on top of all this fact is that this is just a single sample
  // pc's could perhaps very more than 1.5% at fluctuating rates
  // until we have some real data from all our pc's we won't really know

  // forsaken uses framelag (fraction) to multiply by other numbers
  // so the faster pc with a smaller fraction gets smaller results
  // meaning that a faster pc will evolve the world at smaller steps
  // and the slower pc you should see the world moving at faster steps

  // now this is only relavant to places that actually use this variable
  // but it could have impact on various time based world updates such as:
  // packes per second, pickup rotation, mine movement/shooting,
  // bikes speed, input reading, lighting etc...

  // thus all of the above would work faster on a slower pc...
  // or perhaps even that missiles could fly *slower* on faster machines!!!!

  // note that player positions are not affected
  // since they are constantly updated via networking
  // and are not time based at all....

  real_framelag  = timer_run( &framelag_timer );
  
  framelag = real_framelag * 71.0F;

  // average framelag of the last time and this time
  // probably cheap way to clamp value down if a loop takes a long time
  avgframelag = (avgframelag + framelag) * 0.5F;

  // debugging
  //if (TimeWorst == 0.0 || seconds > TimeWorst) TimeWorst = seconds;
  //if (TimeBest  == 0.0 || seconds < TimeBest)  TimeBest  = seconds;
  //DebugPrintf("Loop Time: seconds=%5f - worst:%5f - best:%5f\n",seconds,TimeWorst,TimeBest);
  
  //  who knows why
  if( CurrentMenu && (MyGameStatus == STATUS_SinglePlayer) )
    framelag = 0.0F;

}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Disp Tracker
  Input   : LPDIRECT3DDEVICE  lpDev
        : LPDIRECT3DVIEWPORT  lpView
  Output    : BOOL        TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL DispTracker( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  uint16    i;
    int     clearflags;
    D3DRECT   dummy;
  D3DVIEWPORT newviewport;
  float   screen_width, screen_height;
  VECTOR    TempVector;
  MATRIX    TempMatrix;
  float   TargetDistance;
  float   Red, Green, Blue, Trans;
  VECTOR    ShipDir, TargetDir;
  float   Cos;

  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;

    newviewport.dwSize = sizeof(D3DVIEWPORT);
    newviewport.dwX = 0;
  newviewport.dwY = 0;
    newviewport.dwWidth = ( d3dapp->szClient.cx / 3 ) & -2;
    newviewport.dwHeight = (uint32) ( (float) newviewport.dwWidth * pixel_aspect_ratio );
    newviewport.dvScaleX = newviewport.dwWidth / (float)2.0;
    newviewport.dvScaleY = newviewport.dwHeight / (float)2.0;
    newviewport.dvMaxX = (float)D3DDivide(D3DVAL(newviewport.dwWidth),
                                       D3DVAL(2 * newviewport.dvScaleX));
    newviewport.dvMaxY = (float)D3DDivide(D3DVAL(newviewport.dwHeight),
                                       D3DVAL(2 * newviewport.dvScaleY));
  
    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &newviewport) != D3D_OK )
    return FALSE;

  viewplane_distance = (float) ( newviewport.dwWidth / ( 2 * tan( DEG2RAD( normal_fov ) * 0.5 ) ) );
  panelproj._11 = 2 * viewplane_distance / newviewport.dwWidth;
  panelproj._22 = 2 * viewplane_distance / ( newviewport.dwHeight / pixel_aspect_ratio );

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &panelproj) != D3D_OK)
  {
      return FALSE;
  }

  clearflags = 0;
  if (d3dapprs.bZBufferOn != 0 )      // If a ZBuffer is enabled then always clear it..
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }
  if( clearflags != 0 )
  {
    dummy.x1 = newviewport.dwX;
    dummy.x2 = newviewport.dwX + newviewport.dwWidth;

    dummy.y1 = newviewport.dwY;
    dummy.y2 = newviewport.dwY + newviewport.dwHeight;

    if( d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                             1, &dummy,
                                             clearflags) != D3D_OK )
                         return FALSE;
  }

  MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
  ShadeModel( MODEL_Tracker, &TempMatrix, ( 40.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ) );

  TempMatrix = MATRIX_Identity;
  TempMatrix._11 = 1.25F;
  TempMatrix._22 = 1.25F;
  TempMatrix._33 = 1.25F;
  MatrixMultiply( &TempMatrix, &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );

  view._11 = TempMatrix._11; 
  view._12 = TempMatrix._12;
  view._13 = TempMatrix._13;
  view._14 = TempMatrix._14;
             
  view._21 = TempMatrix._21;
  view._22 = TempMatrix._22;
  view._23 = TempMatrix._23;
  view._24 = TempMatrix._24;
             
  view._31 = TempMatrix._31;
  view._32 = TempMatrix._32;
  view._33 = TempMatrix._33;
  view._34 = TempMatrix._34;

  view._41 = 0.0F;
  view._42 = 0.0F;
  view._43 = ( 280.0F * GLOBAL_SCALE );
  view._44 = 1.0F;

  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

  if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Tracker], (uint16) -1 ) != TRUE )
    return FALSE;

#if 0
  i = FindClosestPickup();
#else
  i = FindClosestShip();
#endif
  
  if( i != (uint16) -1 )
  {
    Red = 255.0F;
    Green = 255.0F;
    Blue = 255.0F;
    Trans = 255.0F;
#if 0
    TempVector.x = ( ( Pickups[ i ].Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Pickups[ i ].Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Pickups[ i ].Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#else
    TempVector.x = ( ( Ships[ i ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Ships[ i ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Ships[ i ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#endif
    TargetDistance = VectorLength( &TempVector );

    if( TargetDistance > ( 130.0F * GLOBAL_SCALE ) )
    {
      NormaliseVector( &TempVector );
      TempVector.x *= ( 130.0F * GLOBAL_SCALE );
      TempVector.y *= ( 130.0F * GLOBAL_SCALE );
      TempVector.z *= ( 130.0F * GLOBAL_SCALE );
      Green = 64.0F;
      Blue = 64.0F;
    }
    else
    {
      ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Forward, &ShipDir );
      NormaliseVector( &ShipDir );
      TargetDir = TempVector;
      NormaliseVector( &TargetDir );
      Cos = DotProduct( &ShipDir, &TargetDir );
      if( Cos < 0.0F )
      {
        Red = 256.0F + ( Cos * 192.0F );
        Green = 256.0F + ( Cos * 192.0F );
      }
    }

    TintModel( MODEL_Ping, Red, Green, Blue, Trans );

    MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
    ApplyMatrix( &TempMatrix, &TempVector, &TempVector ); // Calc Up Vector

    view = identity;
    view._11 = 1.25F;
    view._22 = 1.25F;
    view._33 = 1.25F;
    view._41 = TempVector.x;
    view._42 = TempVector.y;
    view._43 = TempVector.z + ( 280.0F * GLOBAL_SCALE );
    view._44 = 1.0F;

    if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
      return FALSE;

    if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Ping], (uint16) -1 ) != TRUE )
      return FALSE;
  }

    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport) != D3D_OK )
    return FALSE;

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }

  return TRUE;
}


/***************************************\
|
|  Release Joysticks
|
\***************************************/

void  ReleaseJoysticks( void )
{
  int joystick, i, j;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    free (JoystickInfo[joystick].Name);

    if ( JoystickInfo[ joystick ].connected )
    {
      for (i = 0; i < JoystickInfo[joystick].NumButtons; i++)
        free (JoystickInfo[joystick].Button[i].name);

      for (i = 0; i < JoystickInfo[joystick].NumPOVs; i++)
      {
        free (JoystickInfo[joystick].POV[i].name);
        for ( j = 0; j < MAX_POV_DIRECTIONS; j++ )
        {
          if ( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] )
            free( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] );
          JoystickInfo[ joystick ].POV[ i ].dirname[ j ] = NULL;
        }
      }

      for (i = 0; i < MAX_JOYSTICK_AXIS; i++)
      {
        if (JoystickInfo[joystick].Axis[i].exists)
          free (JoystickInfo[joystick].Axis[i].name);     
      }
    }
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Change Background Colour
  Input   : float R ( 0.0F - 1.0F )
        : float G ( 0.0F - 1.0F )
        : float B ( 0.0F - 1.0F )
  Output    : BOOL  TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ChangeBackgroundColour( float R, float G, float B )
{
    D3DMATERIAL bmat;
    D3DMATERIALHANDLE hBmat;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;

  BOOL  HadtoCreate = FALSE;


  memset(&bmat, 0, sizeof(D3DMATERIAL));
  bmat.diffuse.r = R;
  bmat.diffuse.g = G;
  bmat.diffuse.b = B;
    bmat.dwSize = sizeof(D3DMATERIAL);
    bmat.dwRampSize = 1;


  if( !lpBmat )
  {
    if (lpD3D->lpVtbl->CreateMaterial(lpD3D, &lpBmat, NULL) != D3D_OK) return FALSE;
    HadtoCreate = TRUE;

  }

  if( lpBmat->lpVtbl->SetMaterial( lpBmat, &bmat ) != D3D_OK) return FALSE;
    if( lpBmat->lpVtbl->GetHandle(lpBmat, d3dapp->lpD3DDevice, &hBmat) != D3D_OK) return FALSE;
    if( d3dapp->lpD3DViewport->lpVtbl->SetBackground( d3dapp->lpD3DViewport, hBmat ) != D3D_OK) return FALSE;

  if( HadtoCreate )
  {
    RELEASE(lpBmat);
  }

  return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Print up how far the init view has to go...
  Input   : BYTE GameStatus
  Output    : void
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void PrintInitViewStatus( BYTE Status )
{
  int i;

  for( i = 0 ; i < ( Status - STATUS_InitView_0 )+1 ; i ++ )
  {
    CenterPrint4x5Text( InitViewMessages[i], ( d3dappi.szClient.cy >> 2 ) + ( i * ( FontHeight + ( FontHeight>>1 ) ) ) , 2 );
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init Stats Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  InitStatsDisplay()
{
  // Create the offscreen surface, by loading our bitmap.

//  if( ModeCase != -1 )
//  {
//    lpDDSOne = DDLoadBitmap( d3dapp->lpDD, (char*) &StatsNames[ModeCase] , 0, 0 );
//    ddpal =  DDLoadPalette( d3dapp->lpDD , (char*) &StatsNames[ModeCase]);
//    lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
//  }
  InitFont(FALSE);
  ChangeBackgroundColour( 0, 0, 0 );

  HowManySecrets( &TotalSecrets , &Secrets );

  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Free Stats Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  FreeStatsDisplay()
{
//  ReleaseDDSurf(lpDDSOne);
  ReleaseDDSurf(lpDDSTwo);
  lpDDSTwo = NULL;
  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Stats Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL StatsDisplay()
{
#if 0
    HRESULT ddrval;
    RECT    src;
  RECT    dest;
  DDBLTFX fx;
  int x,y;
#endif
  char buf[256];

#if 0
  //  Blt Background
  src.top = 0;
  src.left = 0;
  src.right = d3dappi.szClient.cx;
  src.bottom = d3dappi.szClient.cy;
  x = 0;
  y = 0;
  memset(&fx, 0, sizeof(DDBLTFX));
  fx.dwSize = sizeof(DDBLTFX);
  dest.top = 0;
  dest.bottom = d3dappi.szClient.cy;
  dest.left = 0;
  dest.right = d3dappi.szClient.cx;
      
  while( 1 )
  {
    ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_WAIT , &fx );
    if( ddrval == DD_OK )
      break;
    if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DDReLoadBitmap( lpDDSOne, (char*) &StatsNames[ModeCase] );
      break;
    }
    if( ddrval != DDERR_WASSTILLDRAWING )
      break;
  }
#endif
  if( !GameCompleted )
  {
    if( Secrets == 1 )
    {
      sprintf( (char*) &buf ,"%d Secret found out of %d", Secrets , TotalSecrets );
    }else{
      sprintf( (char*) &buf ,"%d Secrets found out of %d", Secrets , TotalSecrets );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) - (FontHeight*2) , 2 );
    if( NumKilledEnemies == 1 )
    {
      sprintf( (char*) &buf ,"%d Enemy killed out of %d", NumKilledEnemies , NumInitEnemies );
    }else{
      sprintf( (char*) &buf ,"%d Enemies killed out of %d", NumKilledEnemies , NumInitEnemies );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) + (FontHeight*0) , 2 );
    if( CrystalsFound == 1 )
    {
      sprintf( (char*) &buf ,"%d Crystal found so far", CrystalsFound );
    }else{
      sprintf( (char*) &buf ,"%d Crystals found so far", CrystalsFound );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
  }else{
    CenterPrint4x5Text( "Congratulations" , (d3dappi.szClient.cy >> 1 ) - (FontHeight*2) , 2 );
    CenterPrint4x5Text( "Demo Complete" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*0) , 2 );
    if( DifficultyLevel != 3 )
    {
      CenterPrint4x5Text( "Now Try a Harder Difficulty Setting" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
    }else{
      CenterPrint4x5Text( "Try the real Game for a harder challenge" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
    }
  }
  
  CenterPrint4x5Text( "Press Space to continue" , d3dappi.szClient.cy - (FontHeight*2) , 2 );
//  Our_CalculateFrameRate();


  return TRUE;
}

void InitModeCase(void)
{
#if 0
  if( d3dappi.szClient.cx >= 1024 && d3dappi.szClient.cy >= 768 )
    ModeCase = Mode1024X768;
  else if( d3dappi.szClient.cx >= 800 && d3dappi.szClient.cy >= 600 )
    ModeCase = Mode800X600;
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 480 )
    ModeCase = Mode640X480;
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 400 )
    ModeCase = Mode640X400;
  else if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
    ModeCase = Mode512X384;
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 400 )
    ModeCase = Mode320X400;
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 240 )
    ModeCase = Mode320X240;
  else
    ModeCase = Mode320X200;
#endif

  ModeCase = 0; // now dynamic

  ModeScaleX[ 0 ] = (float)d3dappi.szClient.cx / 320.0F;
  ModeScaleY[ 0 ] = (float)d3dappi.szClient.cy / 200.0F;

  if ( ( d3dappi.szClient.cx >= 512 ) && ( d3dappi.szClient.cy >= 384 ) )
  {
    strcpy( DynamicFontNames, FontNames[ Mode512X384 ] );
  }else
  {
    strcpy( DynamicFontNames, FontNames[ Mode320X200 ] );
  }

  if( d3dappi.szClient.cx >= 800 && d3dappi.szClient.cy >= 600 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode800X600 ] );
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 480 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode640X480 ] );
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 400 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode640X400 ] );
  else if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode512X384 ] );
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 400 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X400 ] );
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 240 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X240 ] );
  else
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X200 ] );

  // Check for Polygon Text.....
  bPolyText = FALSE;
  if(( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 ) && PolygonText )
    bPolyText = TRUE;

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Get Bit Shift required to make first bit of
        :   mask bit 0
  Input   :   int32 Mask
  Output    :   int16 Shift Value
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int16 GetBitShift( int32 Mask )
{
  int16 Count;

  if( Mask )
  {
    Count = 0;

    while( !( Mask & 1 ) )
    {
      Count++;
      Mask >>= 1;
    }

    return( Count );
  }

  return( 0 );
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Render Snapshot
  Input   :   Nothing
  Output    :   Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void RenderSnapshot( void )
{
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;

  lpDev->lpVtbl->BeginScene(lpDev);

  CurrentCamera.enable = 1;
  CurrentCamera.UseLowestLOD = TRUE;
  CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
  CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
  CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
  CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
  CurrentCamera.Viewport = viewport;  
  CurrentCamera.Proj = proj;  
  
  CurrentCamera.Viewport.dwX = 0;
  CurrentCamera.Viewport.dwY = 0;
  CurrentCamera.Viewport.dwWidth = 128;
  CurrentCamera.Viewport.dwHeight = 128;
  CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
  CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
  CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                     D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
  CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                     D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
  
  CurrentCamera.UseLowestLOD = TRUE;
  if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
    return;

  lpDev->lpVtbl->EndScene(lpDev);
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save PPM Bitmap
  Input   :   int8  * Filename
        :   int8  * Screen Pointer
        :   uint32    Width
        :   uint32    Height
        :   uint32    Bytes Per Pixel
        :   uint32    LineSize
        :   uint32    RedMask
        :   uint32    GreenMask
        :   uint32    BlueMask
        :   uint32    x1, y1, x2 ,y2 ( Portion of Screen )
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SavePPM( uint8 * Filename, uint8 * ScreenPtr, uint32 Width, uint32 Height, uint32 BytesPerPixel,
        uint32 LineSize, uint32 RedMask, uint32 GreenMask, uint32 BlueMask,
        uint32 x1, uint32 y1, uint32 x2, uint32 y2 )
{
  uint32    XCount, YCount;
  uint32    RedMul, GreenMul, BlueMul;
  uint8   Red, Green, Blue;
  int16   RedShift, GreenShift, BlueShift;
  uint8 * LinePtr;
  int8    Header[ 256 ];
  FILE  * fp;
  int16   i;
  uint32    Val;
  uint32    Xoff, Yoff;
  uint32    Xsize, Ysize;

  if( ( x1 == x2 ) && ( y1 == y2 ) )
  {
    x1 = 0;
    y1 = 0;
    x2 = Width;
    y2 = Height;
  }

  Xoff = x1;
  Yoff = y1;
  Xsize = ( x2 - x1 );
  Ysize = ( y2 - y1 );

  fp = fopen( Filename, "wb" );

  if( fp != NULL )
  {
    sprintf( &Header[ 0 ], "P6%c#Forsaken (c)1998%c%d %d%c255%c", 10, 10, Xsize, Ysize, 10, 10 );

    i = 0;
    while( Header[ i ] )
    {
      fwrite( &Header[ i ], sizeof( int8 ), 1, fp );
      i++;
    }

    RedShift = GetBitShift( RedMask );
    GreenShift = GetBitShift( GreenMask );
    BlueShift = GetBitShift( BlueMask );

    RedMul = ( 256 / ( ( RedMask >> RedShift ) + 1 ) );
    GreenMul = ( 256 / ( ( GreenMask >> GreenShift ) + 1 ) );
    BlueMul = ( 256 / ( ( BlueMask >> BlueShift ) + 1 ) );

    ScreenPtr += ( Yoff * LineSize );

    for( YCount = 0; YCount < Ysize; YCount++ )
    {
      LinePtr = ( ScreenPtr + Xoff );

      for( XCount = 0; XCount < Xsize; XCount++ )
      {
        Val = 0;

        switch( BytesPerPixel )
        {
          case 1:
            Val = (uint32) LinePtr[0];
            break;
          case 2:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 );
            break;
          case 3:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 ) |
                   ( ( (uint32) LinePtr[2] ) << 16 );
            break;
        }
        LinePtr += BytesPerPixel;

        Red   = (uint8) ( ( ( Val & RedMask ) >> RedShift ) * RedMul );
        Green = (uint8) ( ( ( Val & GreenMask ) >> GreenShift ) * GreenMul );
        Blue  = (uint8) ( ( ( Val & BlueMask ) >> BlueShift ) * BlueMul );

        fwrite( &Red, sizeof( Red ), 1, fp );
        fwrite( &Green, sizeof( Green ), 1, fp );
        fwrite( &Blue, sizeof( Blue ), 1, fp );
      }

      ScreenPtr += LineSize;
    }

    fclose( fp );
  }
  else
  {
    DebugPrintf( "Failed to create %s\n", Filename );
    return( FALSE );
  }

  return( TRUE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save SnapShot screen for saved game
  Input   :   int8  * Filename
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SaveSnapShot( int8 * Filename )
{
  HRESULT     hr;
  DDSURFACEDESC SurfaceDesc;

  FreeAllLastAFrameScrPolys();
  RenderSnapshot();

  memset( &SurfaceDesc, 0, sizeof( SurfaceDesc ) );
  SurfaceDesc.dwSize = sizeof( SurfaceDesc );

  hr = d3dapp->lpBackBuffer->lpVtbl->Lock( d3dapp->lpBackBuffer, NULL, &SurfaceDesc,
        DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY, NULL );

  if ( hr != DD_OK )
  {
    switch( hr )
    {
      case DDERR_INVALIDOBJECT:
        DebugPrintf( "Error Locking Surface ( Invalid Object )\n" );
        break;

      case DDERR_INVALIDPARAMS:
        DebugPrintf( "Error Locking Surface ( Invalid Params )\n" );
        break;

      case DDERR_OUTOFMEMORY:
        DebugPrintf( "Error Locking Surface ( Out of Memory )\n" );
        break;

      case DDERR_SURFACEBUSY:
        DebugPrintf( "Error Locking Surface ( Surface Busy )\n" );
        break;

      case DDERR_SURFACELOST:
        DebugPrintf( "Error Locking Surface ( Surface Lost )\n" );
        break;

      case DDERR_WASSTILLDRAWING:
        DebugPrintf( "Error Locking Surface ( Was Still Drawing )\n" );
        break;
    }
    return( FALSE );
  }

  SavePPM( Filename, SurfaceDesc.lpSurface, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight,
        ( ( SurfaceDesc.ddpfPixelFormat.dwRGBBitCount + 7 )  / 8 ),
        SurfaceDesc.lPitch, SurfaceDesc.ddpfPixelFormat.dwRBitMask, SurfaceDesc.ddpfPixelFormat.dwGBitMask,
        SurfaceDesc.ddpfPixelFormat.dwBBitMask, 0, 0, 128, 128 );

  hr = d3dapp->lpBackBuffer->lpVtbl->Unlock( d3dapp->lpBackBuffer, NULL );
  if ( hr != DD_OK )
  {
    DebugPrintf( "Error Unlocking Surface\n" );
    return( FALSE );
  }

  if( CurrentMenu ) MenuDraw( CurrentMenu );
  MainGame( d3dapp->lpD3DDevice, d3dapp->lpD3DViewport );

  return( TRUE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save SnapShot screen
  Input   :   int8  * Filename
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SaveFullScreenSnapShot( int8 * Filename )
{
  HRESULT     hr;
  DDSURFACEDESC SurfaceDesc;

  memset( &SurfaceDesc, 0, sizeof( SurfaceDesc ) );
  SurfaceDesc.dwSize = sizeof( SurfaceDesc );

  hr = d3dapp->lpFrontBuffer->lpVtbl->Lock( d3dapp->lpFrontBuffer, NULL, &SurfaceDesc,
        DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY, NULL );

  if ( hr != DD_OK )
  {
    switch( hr )
    {
      case DDERR_INVALIDOBJECT:
        DebugPrintf( "Error Locking Surface ( Invalid Object )\n" );
        break;

      case DDERR_INVALIDPARAMS:
        DebugPrintf( "Error Locking Surface ( Invalid Params )\n" );
        break;

      case DDERR_OUTOFMEMORY:
        DebugPrintf( "Error Locking Surface ( Out of Memory )\n" );
        break;

      case DDERR_SURFACEBUSY:
        DebugPrintf( "Error Locking Surface ( Surface Busy )\n" );
        break;

      case DDERR_SURFACELOST:
        DebugPrintf( "Error Locking Surface ( Surface Lost )\n" );
        break;

      case DDERR_WASSTILLDRAWING:
        DebugPrintf( "Error Locking Surface ( Was Still Drawing )\n" );
        break;
    }
    return( FALSE );
  }

  SavePPM( Filename, SurfaceDesc.lpSurface, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight,
        ( ( SurfaceDesc.ddpfPixelFormat.dwRGBBitCount + 7 )  / 8 ),
        SurfaceDesc.lPitch, SurfaceDesc.ddpfPixelFormat.dwRBitMask, SurfaceDesc.ddpfPixelFormat.dwGBitMask,
        SurfaceDesc.ddpfPixelFormat.dwBBitMask, 0, 0, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight );

  hr = d3dapp->lpFrontBuffer->lpVtbl->Unlock( d3dapp->lpFrontBuffer, NULL );
  if ( hr != DD_OK )
  {
    DebugPrintf( "Error Unlocking Surface\n" );
    return( FALSE );
  }
  return( TRUE );
}

static int CheckFileWriteable( char *fname )
{
    HANDLE      fhandle;

    if ( ( fhandle = CreateFile( fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ))
    == INVALID_HANDLE_VALUE )
  {
    DebugPrintf( "CreateFile( %s ) failed\n", fname );
    return 0;
  }
    else
  {
    DWORD attr;

        CloseHandle( fhandle );
    if ( ( attr = GetFileAttributes( fname ) ) == 0xFFFFFFFFL )
    {
      DebugPrintf( "GetFileAttributes( %s ) failed\n", fname );
      return -1;
    }
        if ( !SetFileAttributes( fname, FILE_ATTRIBUTE_NORMAL ) )
    {
      DebugPrintf( "SetFileAttributes( %s, 0x%x ) failed\n", fname, FILE_ATTRIBUTE_NORMAL );
      return -2;
    }
        if ( !SetFileAttributes( fname, attr ) )
    {
      DebugPrintf( "SetFileAttributes( %s, 0x%x ) failed\n", fname, attr );
      return -3;
    }
  }

  return 1;
}

#ifdef  REFLECTION
LPDIRECTDRAWSURFACE lpDestTextureSurf = NULL;
void WierdShit( void )
{
    HDC                 hdcPrimary;
    HDC                 hdcDest;
    HRESULT             hr;
  DDSURFACEDESC ddsd2;
    LPDIRECT3DTEXTURE lpDestTexture = NULL;

  if( MyGameStatus != STATUS_Normal )
    return;


  if( !lpDestTextureSurf )
  {
    memcpy(&ddsd2, &d3dappi.ThisTextureFormat.ddsd, sizeof(DDSURFACEDESC));
    ddsd2.dwSize = sizeof(DDSURFACEDESC);
    ddsd2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

    ddsd2.dwHeight = 128;
    ddsd2.dwWidth = 256;
    
    if (d3dappi.lpDD->lpVtbl->CreateSurface(d3dappi.lpDD , &ddsd2, &lpDestTextureSurf, NULL) != DD_OK)
      lpDestTextureSurf = NULL;
  }
  if( !lpDestTextureSurf )
    return;


    if ((hr = d3dapp->lpBackBuffer->lpVtbl->GetDC( d3dapp->lpBackBuffer, &hdcPrimary )) == DD_OK)
  {
      if ((hr = lpDestTextureSurf->lpVtbl->GetDC( lpDestTextureSurf, &hdcDest )) == DD_OK)
    {

        StretchBlt(hdcDest, 0, 0, 128, 128, hdcPrimary, 0, 0, 128, 128, SRCCOPY);

          lpDestTextureSurf->lpVtbl->ReleaseDC(lpDestTextureSurf, hdcDest);
    }
        d3dapp->lpBackBuffer->lpVtbl->ReleaseDC( d3dapp->lpBackBuffer, hdcPrimary);
  }
  LastError = lpDestTextureSurf->lpVtbl->QueryInterface(lpDestTextureSurf,
                       &IID_IDirect3DTexture,
                       (LPVOID*)&lpDestTexture);

    Tloadheader.lpTexture[12]->lpVtbl->Load(Tloadheader.lpTexture[12], lpDestTexture);

}
#endif

// if we met the max kills limit then set flag to change level
void CheckMetKillLimit()
{
	BOOL	KillsReached = FALSE;
	BOOL teamOK[MAX_TEAMS];
	BOOL teamleft[MAX_TEAMS];

	// i'm the host so i should check if we have met the level kill threshold if there is one
	if(IsHost && MaxKills && (MyGameStatus == STATUS_Normal) )
	{
		// not team game
		if( !TeamGame )
		{
			// for every player
			for ( i = 0; i < MAX_PLAYERS; i++ )
			{
				// whose status is normal
				if( (GameStatus[GetPlayerRank(i)] == STATUS_Normal) )
				{
					// if they scored more than the kills threshold then flag we finished it
					if( GetScoreStats(GetPlayerRank(i)) >= MaxKills && LevelNum != -1 )
						KillsReached = TRUE;
				}
			}
		}
		// team game
		else
		{
			// identify which teams are active
			for (i = 0; i < MAX_TEAMS; i++)
			{	
				teamOK[i] = FALSE;
				teamleft[i] = TRUE;
			}

			// if any of the active teams scored more than the kills threshold then flag we finished it
			for( i = 0 ; i < MAX_TEAMS ; i++ )
			{
				if( GetTeamScore(i) >= MaxKills && !teamleft[i] && teamOK[i])
					KillsReached = TRUE;
			}
		}

		// kill threshold reached so change level
		if( KillsReached )
		{
			LevelNum = -1;
			NewLevelNum++;
			if( NewLevelNum >= NumLevels ) NewLevelNum = 0;
			NetworkGameUpdate();
		}
	}

}

