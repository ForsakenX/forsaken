/*
 * The X Men, August 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 169 $
 *
 * $Header: /PcProjectX/Text.c 169   22/10/98 17:40 Phillipd $
 *
 * $Log: /PcProjectX/Text.c $
 * 
 * 169   22/10/98 17:40 Phillipd
 * 
 * 168   22/10/98 17:33 Phillipd
 * 
 * 167   17/09/98 15:29 Phillipd
 * 
 * 166   10/09/98 10:52 Philipy
 * fixed bug in ping stuff
 * changed checksum kicking out to use in game message ( now also works in
 * peer-peer )
 * 
 * 165   4/09/98 11:26 Phillipd
 * 
 * 164   18/08/98 18:14 Philipy
 * all pings in server game are now from server perspective
 * windowed mode re-enabled ( still needs some work )
 * 
 * 163   11/08/98 17:22 Philipy
 * pings sent in heartbeat packet are now obtained from normal
 * MSG_PINGREQUEST.
 * pings requests are always sent even if not shown if you are the server,
 * or you are responding to UDP status requests, or if you are sending out
 * heartbeat packets.
 * 
 * 162   10/08/98 17:33 Philipy
 * rewrote AVI player
 * 
 * 161   30/07/98 12:06 Philipy
 * increased max ping to 999999
 * 
 * 160   7/16/98 12:25p Phillipd
 * 
 * 159   7/16/98 11:37a Phillipd
 * 
 * 158   7/16/98 11:26a Phillipd
 * 
 * 157   7/09/98 11:50a Phillipd
 * Polytext now works being turned off for the Server.....HooRay...
 * 
 * 156   6/29/98 2:42p Phillipd
 * 
 * 155   6/26/98 2:13p Phillipd
 * 
 * 154   12/06/98 16:06 Philipy
 * fixed lobby stuff for Wireplay
 * 
 * 153   6/11/98 2:36p Phillipd
 * 
 * 152   6/10/98 10:48a Phillipd
 * 
 * 151   6/08/98 9:19a Phillipd
 * 
 * 150   5/21/98 9:42a Phillipd
 * 
 * 149   5/20/98 12:14p Phillipd
 * 
 * 148   20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 147   5/13/98 3:59p Phillipd
 * 
 * 146   5/12/98 2:46p Phillipd
 * 
 * 145   5/11/98 12:29p Phillipd
 * 
 * 144   28/04/98 13:05 Oliverc
 * Disabled text message debugging unless DEBUG_TEXT_MESSAGES if defined
 * AND fixed probable bugette in Tmi->ActivateOrder setting
 * 
 * 143   24/04/98 10:46 Oliverc
 * Added default display chars for weird foreign stuff we don't have in
 * our font images
 * 
 * 142   24/04/98 10:24 Oliverc
 * Fixed missing uppercase chars in font table
 * 
 * 141   4/23/98 5:49p Phillipd
 * 
 * 140   4/22/98 2:14p Phillipd
 * 
 * 139   21/04/98 15:42 Collinsd
 * 
 * 138   4/21/98 11:13a Phillipd
 * 
 * 137   4/21/98 10:01a Phillipd
 * 
 * 136   15/04/98 12:31 Oliverc
 * In-game text substituted for localised definitions
 * 
 * 135   4/08/98 5:33p Phillipd
 * 
 * 134   4/04/98 14:22 Philipy
 * mode scaling stuff is now calculated rather than based on fixed values
 * added -NoBlitTextScaling option to ReadIni and command line options
 * 
 * 133   4/02/98 2:07p Phillipd
 * 
 * 132   3/31/98 3:56p Phillipd
 * 
 * 131   3/31/98 3:55p Phillipd
 * 
 * 130   3/31/98 2:51p Phillipd
 * 
 * 129   3/31/98 2:47p Phillipd
 * 
 * 128   3/30/98 3:37p Phillipd
 * 
 * 127   3/30/98 3:35p Phillipd
 * 
 * 126   3/27/98 4:44p Phillipd
 * 
 * 125   3/27/98 4:42p Phillipd
 * 
 * 124   3/27/98 4:38p Phillipd
 * 
 * 123   3/27/98 4:38p Phillipd
 * 
 * 122   3/26/98 9:13a Phillipd
 * 
 * 121   3/25/98 11:22a Phillipd
 * 
 * 120   24/03/98 16:20 Philipy
 * added new sfx
 * 
 * 119   3/21/98 2:34p Phillipd
 * 
 * 118   3/20/98 10:36a Phillipd
 * 
 * 117   15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 116   3/12/98 9:26a Phillipd
 * 
 * 115   3/11/98 4:09p Phillipd
 * 
 * 114   3/11/98 11:06a Phillipd
 * 
 * 113   3/10/98 3:54p Phillipd
 * 
 * 112   3/10/98 10:45a Phillipd
 * 
 * 111   6/03/98 16:28 Oliverc
 * Changed way teams names flash grey to indicate bad connections
 * 
 * 110   3/05/98 5:58p Phillipd
 * Text Load/Save done just needs calling....
 * 
 * 109   3/05/98 12:47p Phillipd
 * 
 * 108   3/05/98 11:52a Phillipd
 * Previously triggered text message are now accessable in single player
 * by pressing
 * f9-10 previous and last
 * 
 * 107   3/05/98 11:02a Phillipd
 * Text message timing changed slightly.....
 * 
 * 106   3/03/98 17:00 Oliverc
 * New multiplayer CTF mode stuff (1st attempt)
 * 
 * 105   27/02/98 16:32 Oliverc
 * Fixed bug: player name displayed topleft after all lives lost in
 * singleplayer game
 * 
 * 104   25/02/98 16:19 Oliverc
 * More multiplayer tweaks
 * 
 * 103   25/02/98 14:52 Oliverc
 * Fixed bug in bounty team display
 * and added BadConnection[] display
 * 
 * 102   24/02/98 22:00 Oliverc
 * Tweaks to multiplayer games
 * 
 * 101   24/02/98 16:56 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 100   23/02/98 15:31 Philipy
 * implemented single player level timer
 * 
 * 99    21/02/98 16:25 Philipy
 * added text messages for capture flag
 * 
 * 98    21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 97    20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 96    20/02/98 15:29 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 95    11/02/98 12:57 Philipy
 * Changed PlaySfx calls to use Vol instead of Dist
 * 
 * 94    29/01/98 14:19 Oliverc
 * 
 * 93    28/01/98 21:43 Oliverc
 * Fixed bugs in team tally screen display
 * 
 * 92    1/28/98 9:42a Phillipd
 * New text message stuff done....
 * 
 * 91    26/01/98 18:23 Philipy
 * fixed video memory leaks
 * splash screens now display after release view, and call InitScene,
 * InitView after  completion
 * 
 * 90    1/23/98 11:29a Phillipd
 * 
 * 89    22/01/98 9:47 Philipy
 * team game stats
 * 
 * 88    21/01/98 12:19 Philipy
 * Added attract mode for shareware
 * fixed looping sfx volume bug
 * 
 * 87    19/01/98 13:01 Philipy
 * fixed mission text not being displayed
 * possibly fixed stats bug ?
 * 
 * 86    1/19/98 10:10a Phillipd
 * max kills for team game works...
 * 
 * 85    18/01/98 23:45 Philipy
 * added debuging text for sfx ( if SFX_DEBUG defined )
 * 
 * 84    1/12/98 3:45p Phillipd
 * 
 * 83    10/01/98 17:39 Oliverc
 * Fixed bug in multiplayer max kills test (used to fail when only 1
 * multiplayer level)
 * Added new keyboard chars to font translation table
 * 
 * 82    9/01/98 11:14 Philipy
 * CD nows plays last track
 * CD now replays current track from seperate  ( low priority ) thread -
 * but still causes pause
 * loading bar now displayed when loading
 * 
 * 81    7/01/98 9:34 Philipy
 * added title room sfx
 * added ability to select bike computer, biker with sfx loaded
 * 
 * 80    1/03/98 2:57p Phillipd
 * 
 * 79    12/31/97 9:19a Phillipd
 * 
 * 78    12/03/97 5:18p Phillipd
 * 
 * 77    2/12/97 11:52 Philipy
 * boot demo stuff
 * 
 * 76    11/29/97 4:35p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 75    11/21/97 10:56a Phillipd
 * Max Kills to end a level....
 * 
 * 74    12/11/97 14:48 Philipy
 * fixed joining multiplayer bug
 * 
 * 73    7/11/97 14:25 Philipy
 * fixed bug when changing mode from title room: InitTitle was not being
 * called
 * 
 * 72    11/06/97 12:04p Phillipd
 * 
 * 71    4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 70    27/10/97 10:39 Philipy
 * if'd out AVI display on poly routines
 * compile with AVI_UsePolys if needed
 * 
 * 69    17/10/97 15:13 Philipy
 * 
 * 68    10/08/97 2:05p Phillipd
 * 
 * 67    9/30/97 4:20p Phillipd
 * 
 * 66    9/29/97 9:08a Phillipd
 * 
 * 65    9/18/97 3:56p Phillipd
 * Fixed stats bug....
 * 
 * 64    9/18/97 10:48a Phillipd
 * 
 * 63    17-09-97 4:09p Philipy
 * now always uses blitted text when in STATUS_Copyright (because poly
 * text texture is not loaded yet)
 * 
 * 62    14-09-97 6:55p Philipy
 * stopped space from appearing between teams.
 * Team now greys out if all players in team leave
 * 
 * 61    3-09-97 12:31p Philipy
 * only teams with > 0 players have their score displayed
 * 
 * 60    1-09-97 5:47p Philipy
 * removed warnings....oops!
 * 
 * 59    1-09-97 5:35p Philipy
 * changes made for 3 & 4 player team game
 * 
 * 58    31-07-97 3:48p Philipy
 * Text flashes if in self play mode and showing 'press return'
 * (Print4X5Text() )
 * 
 * 57    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 56    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 55    6/24/97 5:11p Phillipd
 * 
 * 54    6/24/97 11:12a Phillipd
 * 
 * 53    6/23/97 9:53a Phillipd
 * 
 * 52    6/23/97 9:43a Phillipd
 * 
 * 51    6/19/97 2:29p Phillipd
 * 
 * 50    6/19/97 2:28p Phillipd
 * 
 * 49    6/19/97 2:17p Phillipd
 * 
 * 48    19/06/97 12:36 Collinsd
 * Added sprite text for menus
 * 
 * 47    6/18/97 6:01p Phillipd
 * 
 * 46    6/18/97 5:47p Phillipd
 * 
 * 45    6/17/97 5:06p Phillipd
 * 
 * 44    6/17/97 4:03p Phillipd
 * 
 * 43    6/16/97 11:10a Phillipd
 * 
 * 42    6/10/97 9:46a Phillipd
 * 
 * 41    6/07/97 10:52a Phillipd
 * 
 * 40    4/30/97 5:35p Phillipd
 * 
 * 39    3/21/97 12:23p Phillipd
 * 
 * 38    3/20/97 3:41p Phillipd
 * removed c&c backdrop from scores...
 * 
 * 37    3/13/97 11:28a Phillipd
 * Auto Detail level added..
 * Text all one colour unless printing own name...
 * 
 * 36    2/27/97 2:41p Phillipd
 * 
 * 35    2/27/97 2:08p Phillipd
 * 
 * 34    2/26/97 3:27p Phillipd
 * 
 * 33    2/26/97 3:10p Phillipd
 * 
 * 32    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 31    12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 30    20/12/96 15:19 Oliverc
 * Changed debug menu to be on SHIFT-F1
 * 
 * 29    12/19/96 2:52p Phillipd
 * new 512x384/640x400 font added
 * 
 * 28    12/19/96 12:02p Phillipd
 * got rid of dpid from short ship packet
 * added location names to team members...
 * 
 * 27    12/17/96 4:57p Phillipd
 * Version Control Added..
 * 
 * 26    12/17/96 2:56p Phillipd
 * 
 * 25    12/17/96 12:55p Phillipd
 * 
 * 24    12/17/96 9:20a Phillipd
 * 
 * 23    12/10/96 2:06p Phillipd
 * 
 * 22    12/02/96 4:26p Phillipd
 * 
 * 21    12/01/96 1:21p Phillipd
 * You can now quit a game go back to the title screen and start or join
 * another....
 * 
 * 20    11/25/96 11:59a Phillipd
 * 
 * 19    11/13/96 9:08a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 18    7/11/96 10:47 Collinsd
 * Shortened short ships structures.
 * 
 * 17    10/31/96 4:31p Phillipd
 * light float to int in assembler....
 * 
 * 16    26/10/96 18:33 Collinsd
 * Centered and moved up name tags.
 * 
 * 15    10/20/96 4:13p Phillipd
 * 
 * 14    18/10/96 17:46 Collinsd
 * Changed all file loading to load from tidy directories.
 * 
 * 13    10/17/96 4:43p Phillipd
 * proper score sort....dont work on stats yet...
 * 
 * 12    10/11/96 2:34p Phillipd
 * Players who leave or who crash should now be greyed
 * out..
 * 
 * 11    10/06/96 5:04p Phillipd
 * We now have our own text debug print info thing...
 * which speeds up the game by 10%.....
 * 
 * 10    9/27/96 10:26a Phillipd
 * 
 * 9     9/18/96 5:35p Phillipd
 * 
 * 8     9/18/96 3:59p Phillipd
 * 
 * 7     9/16/96 10:16a Phillipd
 * Big changes to Full screen display...
 * And Pickups being set to joining player
 * 
 * 6     9/04/96 5:54p Phillipd
 * 
 * 5     8/20/96 12:06p Phillipd
 * lots of new panel / text stuff...new stats array...
 * 
 * 4     8/19/96 8:51a Phillipd
 * 
 * 3     8/14/96 4:30p Phillipd
 * 
 * 2     8/14/96 9:41a Phillipd
 * 
 * 1     8/14/96 9:30a Phillipd
 * 
 * 
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include File...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include "typedefs.h"
#include "New3D.h"

#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "ships.h"
#include "main.h"
#include "Title.h"

#include "Text.h"

#include "screenpolys.h"
#include "magic.h"
#include "XMem.h"
#include "ddsurfhand.h"
#include "local.h"
#include "dpthread.h"


#define MSG_VERSION_NUMBER 1

#define	MAXFONTCOLOURS	9
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
//#define SFX_DEBUG
extern	BOOL	CanDoDamage[MAX_PLAYERS+1];

extern	BOOL	PlayDemo;
extern	LONGLONG	LastPacketTime[MAX_PLAYERS+1];
extern	LONGLONG	Freq;
extern BOOL	CTF;
extern BOOL CaptureTheFlag;
extern BOOL BountyHunt;

extern LPDIRECTDRAWSURFACE     lpDDSTwo;       // Font Bitmap
extern LPDIRECTDRAWSURFACE     lpDDSThree;     // Panel
extern SHORTNAMETYPE		   Names;	// all the players short Names....
extern GLOBALSHIP              Ships[MAX_PLAYERS+1];
extern float	framelag;
extern	int16	ModeCase;
extern	BYTE	GameStatus[MAX_PLAYERS];	// Game Status for every Ship...

extern	BOOL	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	BOOL	ShowTeamInfo;
extern	MLOADHEADER Mloadheader;
extern	BOOL Is3Dfx;
extern	BOOL PowerVR;
extern	BOOL	bPolyText;
extern	int16			Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern uint16	PingTimes[MAX_PLAYERS];		// How long does it take for a ping???
extern BOOL		ShowPing;
extern SLIDER  PingFreqSlider;
void PingNonGuarenteed(void);
extern	int16					Lives;
extern	int16					MaxKills;
extern	BOOL                    IsHost;
extern	int16			NewLevelNum;
extern	int16			NumLevels;
extern	int				TeamFlag[ MAX_TEAMS ];
extern	LONGLONG	LargeTime;
extern BOOL InSplashDemo;
extern	BOOL IsServerGame;
extern	BOOL IsServer;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int TextSrcX[MAXFONTCOLOURS][256];
int	TextSrcY[MAXFONTCOLOURS][256];


int ReliabilityTab[MAX_PLAYERS+1];

int16	TextActivatedOrder = 0;
int16	CurrentTextActivated = 0;
int	TeamCol[MAX_TEAMS] = { 1, 2, 3, 4 };
char *TeamName[ MAX_TEAMS ] = // team colour names should match TeamCol[] colours (given in Colourtrans[][] below)
{
	LT_TEAM_1, // red
	LT_TEAM_2, // green
	LT_TEAM_3, // blue
	LT_TEAM_4  // yellow
};
BOOL BadConnection[ MAX_PLAYERS+1 ];

BOOL DS = FALSE;

float PingRefresh = 0.0F;

int FontWidth = 4;
int FontHeight = 5;
int FontSourceWidth = 4;
int FontSourceHeight = 5;

int16	NumOfActivePlayers = 0;

int	ScoreSortTab[MAX_PLAYERS + 1];

int	PermX;
uint16 big[4] = { 10000 , 1000 , 100 , 10 };
uint16 little[4] = { 1000 , 100 , 10 , 1 };

int CurrentMessage = 0;
float MessageTime[MAX_MESSAGES] = { 0.0F , 0.0F , 0.0F , 0.0F };
char MessageBank[MAX_MESSAGES][200];
uint8	CharTrans[256];
uint8 Colourtrans[9][3] = { { 192,192,192 }, // gray 
						  { 255,64,64 }, // red 
						  { 64,255,64 }, // green 
						  { 64,64,255 }, // blue 
						  { 255,255,64 }, // yellow
						  { 64,255,255 }, // cyan
						  { 255,64,255 }, // purple
						  { 128,255,128 }, // off green
						  { 64,64,64 } // dark gray
						  };


void DebugPrintf( const char * format, ... );
BOOL	PolyText[255];

uint16 NumOfTextMessages = 0;
char * TextMessagesPnt[MAXTEXTMESSAGES];
char * TextMessages = NULL;
TEXTMSGINFO * TextMsgInfo = NULL;

char StatsMessageFile[] = "statsmessages.txt";

STATSMESSAGE StatsMessages[MAX_STATS_MESSAGES];


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print a int16 number in small 4x4 chars..
	Input		:		int16 num, uint16 x , uint16 y
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Printint16( int16 num , int x , int y , int col )
{
	static char buf[ 128 ];

	if( num != -32767 )
	{
		sprintf( buf, "%hd", num );
	}else{
		sprintf( buf, "%hd", 0 );
	}
	Print4x5Text( buf, x, y, col );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print a uint16 number in small 4x4 chars..
	Input		:		uint16 num, uint16 x , uint16 y
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Printuint16( uint16 tempnum , int x , int y , int col )
{
	int i;
	int num;
    RECT    src;
    HRESULT ddrval;
	int		Zeros= 0 ;
	uint8 r , g , b;

	if( (y + FontHeight ) >= d3dapp->szClient.cy )
		return;
	
	if( bPolyText && PolyText[MyGameStatus])
	{
		r = Colourtrans[col][0];
		g = Colourtrans[col][1];
		b = Colourtrans[col][2];
	}

	for( i = 0 ; i < 4 ; i++ )
	{
		num = tempnum % big[i];
		num /= little[i];
		if( ( num + Zeros != 0 ) || ( i == 3 ) )
		{
			Zeros = 1;
			if( bPolyText && PolyText[MyGameStatus])
			{

				AddScreenPolyText( (uint16) (num+1), (float) x , (float) y, r, g, b, 255 );

			
			}else{
				
				src.top = TextSrcY[col][num+1];
				src.left = TextSrcX[col][num+1];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;
				while( 1 )
				{
					ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x , y, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
				    if( ddrval == DD_OK )
				        break;
				    if( ddrval == DDERR_SURFACELOST )
					{
						d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
						d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
						ReInitFont();
						break;
					}
				    if( ddrval != DDERR_WASSTILLDRAWING )
				        break;
				}
			}
			x += FontWidth;
		}
	}
}





/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, uint16 y
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CenterPrint4x5Text( char * Text , int y, int col )
{
	char * Text2;
	int x;
	int	i;

	Text2 = Text;
	i = 0;
	while( *Text2++ != 0 ) i++;

	x = (d3dappi.szClient.cx >> 1 ) - (i * ( FontWidth >> 1 )  ); // i *2 is half the size of the chars...
	
	Print4x5Text( Text , x , y , col );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, uint16 y
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CenterPrint4x5Text2( char * Text , int x, int y, int col )
{
	char * Text2;
	int	i;

	Text2 = Text;
	i = 0;
	while( *Text2++ != 0 ) i++;

	x -= (i * ( FontWidth >> 1 )  ); // i *2 is half the size of the chars...
	
	Print4x5Text( Text , x , y , col );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print some Right Justified text at a specified Y
	Input		:		char * Text, int x , int y , int col
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RightJustifyPrint4x5Text( char * Text , int x , int y, int col )
{
	char * Text2;
	int	i;

	Text2 = Text;
	i = 0;
	while( *Text2++ != 0 ) i++;
	x = x - (i * FontWidth);
	Print4x5Text( Text , x , y , col );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, uint16 x , uint16 y
	Output		:		last x position
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int Print4x5Text( char * Text , int x , int y , int col )
{
	uint8 num;
    RECT    src, dest;
    HRESULT ddrval;
	uint8 r , g , b;
    DDBLTFX fx;
	BOOL ignore;

	if ( InSplashDemo )
		return x;

	if( (y + FontHeight ) >= d3dapp->szClient.cy )
		return PermX;

	if( bPolyText && PolyText[MyGameStatus])
	{
		r = Colourtrans[col][0];
		g = Colourtrans[col][1];
		b = Colourtrans[col][2];
	}

	if( x != -1 )
		PermX = x;
	while( ( num = *Text++ ) != 0 )
	{
		num = CharTrans[num];

		if ( num )
		{

			if( bPolyText && PolyText[MyGameStatus] )
			{
				AddScreenPolyText( (uint16) num, (float) PermX , (float) y, r , g , b, 255 );
			}else{
		
				src.top = TextSrcY[col][num];
				src.left = TextSrcX[col][num];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;
				dest.bottom = y + FontHeight;
				dest.top = y;
				dest.left = PermX;
				dest.right = PermX + FontWidth;
				memset(&fx, 0, sizeof(DDBLTFX));
				fx.dwSize = sizeof(DDBLTFX);

				if ((dest.right < 0) || (dest.left > d3dapp->szClient.cx))
					ignore = TRUE;
				else
				{
					
					if ((dest.left >= 0) || (dest.right <= d3dapp->szClient.cx))
						ignore = FALSE;
					else
					{
						
						// partial clipping required...
						if (dest.left < 0)
						{
							src.left -= dest.left;
							dest.left = 0;
						}else
						{
							src.right -= (dest.right - d3dapp->szClient.cx);
							dest.right = d3dapp->szClient.cx;
						}
						ignore = FALSE;
					}
				}
				
				while( !ignore )
				{
					//ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, PermX , y, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
					ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSTwo, &src, DDBLT_KEYSRC | DDBLT_WAIT, &fx );
					if( ddrval == DD_OK )
						break;
					if( ddrval == DDERR_SURFACELOST )
					{
						d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
						d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
				
						ReInitFont();
						break;
					}
					if( ddrval != DDERR_WASSTILLDRAWING )
						break;
				}
			}
		}
	
		PermX += FontWidth;
	}

	return PermX;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, uint16 x , uint16 y
	Output		:		last x position
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void PrintClipped4x5Text( char * Text , int x , int y , int col )
{
	uint8 num;
    RECT    src, dest;
    HRESULT ddrval;
	uint8 r , g , b;
    DDBLTFX fx;
	BOOL ignore;
	int dummy;


	if (x < 0)
		dummy = 0;

	if( bPolyText && PolyText[MyGameStatus])
	{
		r = Colourtrans[col][0];
		g = Colourtrans[col][1];
		b = Colourtrans[col][2];
	}

	PermX = x;

	while( ( num = *Text++ ) != 0 )
	{
		num = CharTrans[num];

		if ( num )
		{

			if( bPolyText && PolyText[MyGameStatus] )
			{
				AddScreenPolyText( (uint16) num, (float) PermX , (float) y, r , g , b, 255 );
			}else{
		
				src.top = TextSrcY[col][num];
				src.left = TextSrcX[col][num];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;
				dest.bottom = y + FontHeight;
				dest.top = y;
				dest.left = PermX;
				dest.right = PermX + FontWidth;
				memset(&fx, 0, sizeof(DDBLTFX));
				fx.dwSize = sizeof(DDBLTFX);

				if ((dest.right < 0) || (dest.left > d3dapp->szClient.cx))
					ignore = TRUE;
				else
				{
					if ((dest.left >= 0) && (dest.right <= d3dapp->szClient.cx))
						ignore = FALSE;
					else
					{
						// partial clipping required...
 						if (dest.left < 0)
 						{
 							src.left -= dest.left;
 							dest.left = 0;
 						}else
	 					{
 							src.right -= (dest.right - d3dapp->szClient.cx);
 							dest.right = d3dapp->szClient.cx;
 						}
 						ignore = FALSE;
	 				}
				}
 				
				while( !ignore )
				{
					//ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, PermX , y, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
					ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSTwo, &src, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
				    if( ddrval == DD_OK )
				        break;
				    if( ddrval == DDERR_SURFACELOST )
					{
						d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
						d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
				
						ReInitFont();
				        break;
					}
				    if( ddrval != DDERR_WASSTILLDRAWING )
				        break;
				}
			}
		}
	
		PermX += FontWidth;
	}

}
				
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print All Message in the Que...
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	MessageQuePrint( void )
{
	int i,e;

	e = CurrentMessage + 1;
	e &= 3;
	MessageTime[e] = 0.0F;
	MessageBank[e][0] = 0;
	e = CurrentMessage -2;
	e &= 3;
	for( i = 0 ; i < 3 ; i++ )
	{
		MessageTime[e] -= framelag;
		if( MessageTime[e] < 0.0F && (MessageBank[(e-1)&3][0] == 0) )
		{
			MessageTime[e] = 0.0F;
			MessageBank[e][0] = 0;
		}else{
			if( MessageBank[e][0] != 0 )
			{
				CenterPrint4x5Text( &MessageBank[e][0] , (i*(FontHeight+1))+16 , 2 );
			}
		}
		e++;
		e &= 3;
	}
}


uint8 TempMessage[512];
float MessageSize;
float ThisMessageTime;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Message to the Que Short...
	Input		:		char * Text
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddMessageToQueShort( uint8 ** Text )
{
	uint8 * Pnt;
	int i;
	Pnt = *Text;

	CurrentMessage++;
	CurrentMessage&=3;
	MessageTime[CurrentMessage] = ThisMessageTime;
	
	for( i = 0 ; i < MAXPERLINE ; i++ )
	{
		MessageBank[CurrentMessage][i] = *Pnt;
		if( *Pnt == 0 )
		{
			*Text = NULL;
			return;
		}
		Pnt++;
	}

	i = MAXPERLINE-1;
	while( *Pnt != 0x20 && i >= 0 )
	{
		Pnt--;
		i--;
	}

	if( i >= 0 )
	{
		MessageBank[CurrentMessage][i+1] = 0;
		*Text = Pnt+1;
		return;
	}

	MessageBank[CurrentMessage][0] = 0;
	*Text = NULL;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Add Message to the Que...
	Input		:		char * Text
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddMessageToQue( char * Text, ... )
{
	uint8 * Pnt;
	va_list args;

	va_start( args, Text );
	vsprintf( &TempMessage[0], Text, args);
	va_end( args );

	TempMessage[511] = 0;

	Pnt = &TempMessage[0];

	MessageSize = (float) strlen(&TempMessage[0]);
	ThisMessageTime = MAXMESSAGETIME * (MessageSize / MAXPERLINE);
	if( ThisMessageTime < (MAXMESSAGETIME * 0.25F) )
		ThisMessageTime = MAXMESSAGETIME * 0.25F;


	while( Pnt )
	{
		AddMessageToQueShort( &Pnt );
	}
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Sort players according to Score.....
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ScoreSort( void )
{
	int16 i,e;
	BOOL flag;
	BOOL	oneswapped;
	int16	temp;
	BOOL	KillsReached = FALSE;
	int16 TeamScore[MAX_TEAMS];
	BOOL teamOK[MAX_TEAMS];
	BOOL teamleft[MAX_TEAMS];
	

	if( MyGameStatus == STATUS_SinglePlayer )
		return;
	
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		if( (GameStatus[i] == STATUS_GetPlayerNum) || (GameStatus[i] == STATUS_LeftCrashed)  || (GameStatus[i] == STATUS_Null) || (GameStatus[i] == STATUS_StartingMultiplayerSynch) )
			Ships[i].Kills = -32767;
		if( GameStatus[i] == STATUS_Joining )
			Ships[i].Kills = 0;
	}

		
	flag = TRUE;
	while( flag )
	{
		oneswapped = FALSE;
		for( i = 0 , e = 1 ; i < ( MAX_PLAYERS-1 ) ; i++ , e++ )
		{
			if( (Ships[ScoreSortTab[i]].Kills < Ships[ScoreSortTab[e]].Kills ) || ( (Ships[ScoreSortTab[i]].Kills == Ships[ScoreSortTab[e]].Kills ) && (Ships[ScoreSortTab[i]].Deaths > Ships[ScoreSortTab[e]].Deaths) ) )
			{
				// swap them...
				temp  = ScoreSortTab[i];
				ScoreSortTab[i] = ScoreSortTab[e];
				ScoreSortTab[e] = temp;
				oneswapped = TRUE;
			}
		}
		if( !oneswapped )
			flag = FALSE;
	}

#ifdef SCROLLING_MESSAGES	// stuff below not working!!
	// only allow valid players...
	NumOfActivePlayers = 0;
	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		if(!( ( GameStatus[ScoreSortTab[i]] == STATUS_Left) || ( Ships[ScoreSortTab[i]].enable)  ))
		{
			// squeeze out...(if not last player)
			if (i != MAX_PLAYERS - 1)
			{
				for (e = i; e < (MAX_PLAYERS - 1); e++)
				{
					ScoreSortTab[e] = ScoreSortTab[e + 1];
				}
			}
		}else
			NumOfActivePlayers++;
	}
#endif
	if( (MyGameStatus == STATUS_Normal) && IsHost && MaxKills )
	{
	
		if( !TeamGame )
		{
			for ( i = 0; i < MAX_PLAYERS; i++ )
			{
//				if( (GameStatus[ScoreSortTab[i]] == STATUS_Normal) && (Ships[ScoreSortTab[i]].enable) )
				if( (GameStatus[ScoreSortTab[i]] == STATUS_Normal) )
				{
					if( Ships[ScoreSortTab[i]].Kills >= MaxKills && LevelNum != -1 )
					{
						KillsReached = TRUE;
					}

				}
			}
		}else{

			memset(&TeamScore, 0, sizeof(int16) * MAX_TEAMS);

			for (i = 0; i < MAX_TEAMS; i++)
			{	teamOK[i] = FALSE;
				teamleft[i] = TRUE;
			}
			for (i = 0; i < MAX_PLAYERS; i++)
			{
				if ((TeamNumber[i] < MAX_TEAMS) && (GameStatus[i] == STATUS_Normal))
				{
					TeamScore[TeamNumber[i]] += Ships[i].Kills;
					teamOK[TeamNumber[i]] = TRUE;
					if (GameStatus[i] != STATUS_Left)
						teamleft[TeamNumber[i]] = FALSE;
				}
			}

			for( i = 0 ; i < MAX_TEAMS ; i++ )
			{
				if( TeamScore[i] >= MaxKills && !teamleft[i] && teamOK[i])
				{
					KillsReached = TRUE;
				}
			}
		}

		if( KillsReached )
		{
			//Level Has Been Completed...
			LevelNum = -1;
			NewLevelNum++;
			if( NewLevelNum >= NumLevels ) NewLevelNum = 0;
			DplayGameUpdate();
		}

	}
}

//#define SFX_DEBUG
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print the Scores....
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	FLASH_RATE	( 15.0F )
float	FlashSpeed = 0.0F;
BOOL	FlashToggle = FALSE;

void PrintScoreSort( void )
{
	BOOL teamOK[MAX_TEAMS];
	BOOL teamleft[MAX_TEAMS];
	int i,e, pos;
	int16 TeamScore[MAX_TEAMS];
	int16 TeamBounty[MAX_TEAMS];
	int TeamBadConnection[MAX_TEAMS];
	char buf[256];
#ifdef SFX_DEBUG
	int	numfreeholders;
#endif
	int col;

	NumOfActivePlayers = 0;
#ifdef SFX_DEBUG
	numfreeholders = 0;
	for ( i = 0; i < MAX_ANY_SFX; i++ )
	{
		if ( !SfxHolder[ i ].Used )
			numfreeholders++;
	}
	sprintf( buf, "num free sfx holders = %d", numfreeholders ); 
	Print4x5Text( buf , 8 , FontHeight * 3 , 2 );
#endif
	if( MyGameStatus == STATUS_SinglePlayer )
	{
		Print4x5Text( LIVES , 8 , FontHeight , 2 );
		Printuint16( (uint16) Lives , 8+(8*FontWidth) , FontHeight , 1 );
		return;
	}

	if ( MyGameStatus == STATUS_QuitCurrentGame )
		return;
	
	// pings must always be sent if server, or if heartbeat packets are being sent.
	if( ShowPing || IsServer || ( IsHost && WSA_Active ) && !TeamGame )
	{
		PingRefresh -= framelag;
		if( PingFreqSlider.value >= 1 && PingRefresh < 0.0 )
		{
			PingRefresh = 71.0F * PingFreqSlider.value;
			if ( IsServerGame )
			{
				if ( IsServer )
				{
					PingNonGuarenteed();
					SendGameMessage( MSG_PLAYERPINGS , 0, 0, 0, 0 );
				}
			}else
			{
				PingNonGuarenteed();
			}
		}
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( ScoreSortTab[i] == WhoIAm)
			{
				if( IsServerGame )
				{
					if( ScoreSortTab[i] != 0 )
					{
						if ( ShowPing && !TeamGame )
						{
							sprintf( (char*) &buf[0] ,"Ping %d", PingTimes[ScoreSortTab[i]] );
							Print4x5Text( &buf[0] , 8+(12*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 2 );
						}

						NumOfActivePlayers++;
					}
				}else{
					NumOfActivePlayers++;
				}
			}
			else if( GameStatus[ScoreSortTab[i]] == STATUS_Left )
			{
				if ( ShowPing && !TeamGame )
				{
					sprintf( (char*) &buf[0] ,"Ping %d", PingTimes[ScoreSortTab[i]] );
					Print4x5Text( &buf[0] , 8+(12*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 8 );
				}
				NumOfActivePlayers++;
			}else{
//				if( GameStatus[ScoreSortTab[i]] == STATUS_Normal && !( IsServerGame && i == 0 ) && Ships[ScoreSortTab[i]].enable )
				if( GameStatus[ScoreSortTab[i]] == STATUS_Normal && !( IsServerGame && ScoreSortTab[i] == 0 ) )
				{
					if ( ShowPing && !TeamGame )
					{
						sprintf( (char*) &buf[0] ,"Ping %d", PingTimes[ScoreSortTab[i]] );
						Print4x5Text( &buf[0] , 8+(12*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 2 );
					}
#ifdef DEBUG_ON
					if(  IsServer && !CanDoDamage[ScoreSortTab[i]] )
					{
						Print4x5Text( "Cheater" , 8+(24*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 2 );
					}
#endif
					NumOfActivePlayers++;
				}
			}
		}
		NumOfActivePlayers = 0;
	}else{
		PingRefresh = 0.0F;
	}
	

	FlashSpeed += framelag;
	if( FlashSpeed >= FLASH_RATE )
	{
		FlashToggle = !FlashToggle;
		FlashSpeed = FMOD( FlashSpeed, FLASH_RATE );
	}
	
	if( !TeamGame )
	{
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if( GameStatus[ScoreSortTab[i]] == STATUS_Left )
			{
				Print4x5Text( &Names[ScoreSortTab[i]][0] , 8 , NumOfActivePlayers*(FontHeight+1)+FontHeight , 8 );

				Printint16( Ships[ScoreSortTab[i]].Kills , 8+(8*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 8 );
				NumOfActivePlayers++;
			}else{
//				if( (GameStatus[ScoreSortTab[i]] == MyGameStatus || PlayDemo ) && Ships[ScoreSortTab[i]].enable &&
				if( (GameStatus[ScoreSortTab[i]] == STATUS_Normal) &&
					!( IsServerGame && ScoreSortTab[i] == 0 ) )
				{
					if ( !( Ships[ ScoreSortTab[ i ] ].Object.Flags & SHIP_CarryingBounty ) || FlashToggle )
					{
						col =  ( WhoIAm == ScoreSortTab[i] ) ? 0 : 1;
						Print4x5Text( &Names[ScoreSortTab[i]][0] , 8 , NumOfActivePlayers*(FontHeight+1)+FontHeight , col );
						Printint16( Ships[ScoreSortTab[i]].Kills , 8+(8*FontWidth) , NumOfActivePlayers*(FontHeight+1)+FontHeight , 2 );
						DisplayConnectionStatus( ReliabilityTab[ScoreSortTab[i]] , 2 , NumOfActivePlayers*(FontHeight+1)+FontHeight );

					}
					NumOfActivePlayers++;
				}
			}
		}
	}else{

		memset(&TeamScore, 0, sizeof(int16) * MAX_TEAMS);

		for (i = 0; i < MAX_TEAMS; i++)
		{	teamOK[i] = FALSE;
			teamleft[i] = TRUE;
			TeamCol[i] = i + 1;
			TeamBounty[ i ] = 0;
			TeamBadConnection[ i ] = 0;
		}
		
		for (i = 0; i < MAX_PLAYERS; i++)
		{
//			if ( Ships[ScoreSortTab[i]].enable && (TeamNumber[i] < MAX_TEAMS) && (GameStatus[i] != STATUS_StartingMultiplayer) && (GameStatus[i] == MyGameStatus || PlayDemo ) && !( IsServerGame && i == 0 ))
			if ( (TeamNumber[i] < MAX_TEAMS) && (GameStatus[i] == STATUS_Normal ) && !( IsServerGame && i == 0 ))
			{
				if( Ships[i].Kills != -32767 )
				{
					TeamScore[TeamNumber[i]] += Ships[i].Kills;
				}
				teamOK[TeamNumber[i]] = TRUE;
				switch (GameStatus[i] )
				{
				case STATUS_Left:
				case STATUS_LeftCrashed:
					break;
				default:
					teamleft[TeamNumber[i]] = FALSE;
					if( ReliabilityTab[i] > TeamBadConnection[ TeamNumber[ i ] ] )
						TeamBadConnection[ TeamNumber[ i ] ] = ReliabilityTab[i];
					if ( Ships[ i ].Object.Flags & SHIP_CarryingBounty )
						TeamBounty[ TeamNumber[ i ] ]++;
				}
			}
		}

		pos = 0;
		buf[ 0 ] = 0;

		for (i = 0; i < MAX_TEAMS; i++)
		{
			if (teamOK[i])
			{
				BOOL show;

				show = FALSE;
				if ( CaptureTheFlag || CTF )
				{
					if ( !TeamFlag[ i ] || FlashToggle )
						show = TRUE;
				}
				else if ( BountyHunt )
				{
					if ( !TeamBounty[ i ] || FlashToggle )
						show = TRUE;
				}
				else
					show = TRUE;
				if ( show )
				{
					col = ( teamleft[i] ) ? 8 :
						  ( TeamNumber[ WhoIAm ] == i && FlashToggle ) ? 0 : TeamCol[i] ;
#ifdef TEAM_NUMBERS
					Print4x5Text( TEAM, 8 , pos*(FontHeight+1)+FontHeight , col );
					Printuint16( (uint16)(i + 1) , 8+(5*FontWidth) , pos*(FontHeight+1)+FontHeight , col );
#else
					Print4x5Text( TeamName[ i ] , 8 , pos*(FontHeight+1)+FontHeight , col );
#endif
					Printint16( TeamScore[i] , 8+(7*FontWidth) , pos*(FontHeight+1)+FontHeight , col );
				}
				pos++;
			}
		}

		if( ShowTeamInfo )
		{
			e = pos + 1;
			for( i = 0 ; i < MAX_PLAYERS ; i++ )
			{
//				if( Ships[ScoreSortTab[i]].enable &&( TeamNumber[WhoIAm] == TeamNumber[i] ) && (GameStatus[i] == MyGameStatus || PlayDemo) && !( IsServerGame && i == 0 ))
				if( ( TeamNumber[WhoIAm] == TeamNumber[i] ) && (GameStatus[i] == STATUS_Normal) && !( IsServerGame && i == 0 ))
				{
					col = ( WhoIAm == i ) ? 0 : TeamCol[ TeamNumber[ i ] ];
					Print4x5Text( &Names[i][0] , 8 , e*(FontHeight+1)+(FontHeight*4) , col );
					Print4x5Text( &Mloadheader.Group[Ships[i].Object.Group].name[0] , 8+(FontWidth*8) , e*(FontHeight+1)+(FontHeight*4) , 2 );
					DisplayConnectionStatus( TeamBadConnection[TeamNumber[i]] , 2 , e*(FontHeight+1)+(FontHeight*4) );
					e++;
					NumOfActivePlayers++;
				}
			}
		}

		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if ( (GameStatus[i] == STATUS_Normal ) )
				NumOfActivePlayers++;
		}
	}
}
		

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Print a uint16 number to any surface..
	Input		:		uint16 num, uint16 x , uint16 y
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void Printuint16AnySurface( uint16 tempnum , int x , int y , int col , DWORD flags ,LPDIRECTDRAWSURFACE DestSurface )
{
	int i;
	int num;
    RECT    src;
    HRESULT ddrval;
	int		Zeros= 0 ;
	for( i = 0 ; i < 4 ; i++ )
	{
		num = tempnum % big[i];
		num /= little[i];
		if( ( num + Zeros != 0 ) || ( i == 3 ) )
		{
			Zeros = 1;
			if( col < 8 )
			{
				src.top = col*(FontSourceHeight * 3);
				src.left = (num * FontSourceWidth)+FontSourceWidth;
			}else{
				src.top =  (6*(FontSourceHeight*3));
				src.left = ( (num * FontSourceWidth)+FontSourceWidth ) + ( FontSourceWidth << 4 );
			}

			src.right = src.left+FontSourceWidth;
			src.bottom = src.top+FontSourceHeight;
		}else{
			src.top = 0;
			src.left = 0;
			src.right = FontSourceWidth;
			src.bottom = FontSourceHeight;
		}
		while( 1 )
		{
			ddrval = DestSurface->lpVtbl->BltFast( DestSurface, x , y, lpDDSTwo, &src, flags );
		    if( ddrval == DD_OK )
		        break;
		    if( ddrval == DDERR_SURFACELOST )
			{
				d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
				DestSurface->lpVtbl->Restore( DestSurface );
				ReInitFont();
		        break;
			}
		    if( ddrval != DDERR_WASSTILLDRAWING )
		        break;
		}
		x += FontWidth;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Re-Init the font surface...
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReInitFont(void)
{
#if 0
	if( ModeCase < Mode512X384 )
	{
		DDReLoadBitmap( lpDDSTwo , "data\\pictures\\font.bmp" );
		FontWidth = 4;
		FontHeight = 5;
		FontSourceWidth = 4;
		FontSourceHeight = 5;
	}else{
//		if( ModeCase < Mode640X480 )
//		{
			DDReLoadBitmap( lpDDSTwo , "data\\pictures\\font512.bmp" );
			FontWidth = 8;
			FontHeight = 8;
			FontSourceWidth = 8;
			FontSourceHeight = 8;
//		}else{
//			DDReLoadBitmap( lpDDSTwo , "data\\pictures\\font800.bmp" );
//			FontWidth = 9;
//			FontHeight = 16;
//			FontSourceWidth = 9;
//			FontSourceHeight = 16;
//		}
	}
#endif

	if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
	{
		DDReLoadBitmap( lpDDSTwo , "data\\pictures\\font512.bmp" );
		FontWidth = 8;
		FontHeight = 8;
		FontSourceWidth = 8;
		FontSourceHeight = 8;
	}else
	{
		DDReLoadBitmap( lpDDSTwo , "data\\pictures\\font.bmp" );
		FontWidth = 4;
		FontHeight = 5;
		FontSourceWidth = 4;
		FontSourceHeight = 5;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Init PolyText allowed Array..
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitPolyText( void )
{
	int i;

	for( i = 0 ; i < 256 ; i++ )
	{
		PolyText[i] = FALSE;
	}

	PolyText[STATUS_Title]					= TRUE;
	PolyText[STATUS_BetweenLevels]			= TRUE;
//	PolyText[STATUS_Copyright]				= TRUE;					
	PolyText[STATUS_StartingMultiplayer]	= TRUE;			

	PolyText[STATUS_PlayingDemo]			= TRUE;
	PolyText[STATUS_Normal]					= TRUE;
	PolyText[STATUS_AttractMode]			= TRUE;					
	PolyText[STATUS_SinglePlayer]			= TRUE;					

	//PolyText[STATUS_ViewingScore]			= TRUE;					

	PolyText[STATUS_WaitingToStartSinglePlayer]			= TRUE;					
	PolyText[STATUS_WaitingToStartMultiPlayerHost]		= TRUE;					
	PolyText[STATUS_WaitingToStartMultiPlayerClient]	= TRUE;					
	PolyText[STATUS_WaitingToStartTeamGame]				= TRUE;					
	PolyText[STATUS_WaitingToStartDemo]					= TRUE;
	PolyText[STATUS_WaitingToStartEndGameSequence]		= TRUE;
	PolyText[STATUS_PseudoHostWaitingForAck]			 = TRUE;
	PolyText[STATUS_WaitingForLobbyConnect]			 = TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Read in the level specific text messages.......
	Input		:		char *Filename
	Output		:		BOOL TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ReadTxtFile( char *Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
#ifdef DEBUG_ON
	int i;

	for( i = 0 ; i < MAXTEXTMESSAGES ; i ++ )
	{
		TextMessagesPnt[i] = "\n";
	}
#endif


	TextMessages = NULL;
	File_Size = Get_File_Size( Filename );
	if( !File_Size )
	{
		// dont need water...
		return TRUE;
	}
	Buffer = calloc( File_Size+1 , 1 );
	if( !Buffer )
	{
		Msg( "ReadTxtFile : Unable to allocate file buffer\n", Filename );
		return( FALSE );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "ReadTxtFile Load Error reading %s\n", Filename );
		return( FALSE );
	}
	TextMessages = Buffer;


	TextMessagesPnt[0] = strtok( TextMessages , "\n" );

	NumOfTextMessages = 0;

	while( TextMessagesPnt[NumOfTextMessages] )
	{
//		DebugPrintf( "message %d = %s\n", NumOfTextMessages , TextMessagesPnt[NumOfTextMessages]);

		TextMessagesPnt[++NumOfTextMessages] = strtok( NULL , "\n" );
	}
	return TRUE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Free level specific text messages.......
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FreeTxtFile( void )
{
	if( TextMessages )
	{
		free( TextMessages );
		TextMessages = NULL;
	}
	NumOfTextMessages = 0;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Trigger text message.......
	Input		:		uint16 * Data
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TriggerTextMessage( uint16 * Data )
{
	TEXTMSGINFO * Tmi;
	BOOL	DoneBefore = FALSE;

	Tmi = TextMsgInfo;
	if( Tmi )
	{
		Tmi += *Data;
		if( Tmi->Type )
		{
			DoneBefore = Tmi->Activated;
			Tmi->Activated = TRUE;
		}
	}

	if(!DoneBefore)
	{
		PlaySfx( SFX_MessageBeep, 1.0F );
		AddMessageToQue( "%s\n", TextMessagesPnt[*Data] );
		if( Tmi->ActivatedOrder == -1 )
		{
			Tmi->ActivatedOrder = TextActivatedOrder++;
		}
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Init the text messages......
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitTextMessages( void )
{
	int i;
	CurrentMessage = 0;
	for( i = 0 ; i < MAX_MESSAGES ; i++ )
	{
		MessageTime[i] = 0.0F;
		MessageBank[i][0] = 0;
	}
}
typedef struct {
	char *keyword;
	int  num;
} MESSAGE_NUM_LOOKUP;

typedef struct {
	char *keyword;
	void *var_ptr;
} MESSAGE_VAR_LOOKUP;

#ifdef SCROLLING_MESSAGES

void InitStatsMessages( void )
{
	int i, j;
	char *str;

   	MESSAGE_NUM_LOOKUP MessageNumLookup[] = {
		{ "ConvincingWin",	STATSMESSAGE_ConvincingWin	},
		{ "CloseWin",		STATSMESSAGE_CloseWin		},
		{ "BadLoss",		STATSMESSAGE_BadLoss		},
		{ "CloseLoss",		STATSMESSAGE_CloseLoss		},
		{ "WinnerTie",		STATSMESSAGE_WinnerTie		},
		{ "LoserTie",		STATSMESSAGE_LoserTie		},
		{ "NoPoints",		STATSMESSAGE_NoPoints		},
		{ "JointNoPoints",	STATSMESSAGE_JointNoPoints	},
		{ NULL,				0							}
	};

   	MESSAGE_VAR_LOOKUP MessageVarLookup[] = {
		{ "WinnerName",			Stats_WinnerName		},
		{ "WinnerPoints",		Stats_WinnerPoints		},
		{ "WinnerPointsDiff",	Stats_WinnerPointsDiff	},
		{ "SecondPlaceName",	Stats_SecondPlaceName 	},
		{ "LoserName",			Stats_LoserName			},
		{ "LoserPoints",		Stats_LoserPoints		},
		{ "SecondLastName",		Stats_SecondLastName	},
		{ "NoPointsName",		Stats_NoPointsName		},
		{ NULL,					0					   	}
	};



/*
typedef struct
{
	BOOL valid;
	BOOL valid_this_time;
	uint16 num_variations;
	uint16 current_variation;
	char text[MAX_STATS_MESSAGE_VARIATIONS][MAX_STATS_MESSAGE_LENGTH];
	void *variable_ptr[MAX_STATS_MESSAGE_VARIATIONS];
} STATSMESSAGE;
*/

	FILE *f;
	char token[80];
	int j, k;

	// init all messages

	f = fopen( StatsMessagesFile, "r" );
	if ( !f )
		return;

	while ( fscanf( f, " %80s", token ) == 1)   
	{
		if ( !_stricmp( token, "NewMessage" ) )
		{
			rv = GetMessageName())
			if (!rv)
				continue;
			if (rv == -1)


			if (!GetMessageString())
				continue;

			if (!GetMessageVars())
				continue;
		}
	}
		
		if ( !_stricmp( token, "NewMessage" ) )
		{
			// read in message name
			if ( fscanf( f, " %80s", token ) != 1 )
				break;
			for ( j = 0; MessageNumLookup[ j ].keyword; j++ )
			{
				if ( !_stricmp( token, MessageNumLookup[ j ].keyword ) )
					break;
			}
			if (!MessageNumLookup[ j ].keyword)		// not a valid message token
				continue;

			MessageNum = MessageNumLookup[ j ].num;

			// read in next line, store in StatsMessages
			fgets( tempstr, MAX_STATS_MESSAGE_LENGTH, f );
			
			if ( !_stricmp( token, "EndMessage") )
				continue;

			if ( feof( f ) )
				break;	 // end of file, so break out of loop
			if ( (strlen(tempstr) == 0))
				continue;	// zero length string, so ignore and try to get next message

			// terminate string
			tempstr[strlen(tempstr) - 1] = 0;

			strcpy (StatsMessages[MessageNum].text[num_variations], tempstr);

			// read in all tokens, use to look up address of variable

			k = 0;
			if ( fscanf( f, " %80s", token ) != 1 )
				break;
			IgnoreMessage = FALSE;
			while ( _strcmp( token, "EndVars" ) 
			{
	 			for ( j = 0; MessageVarLookup[ j ].keyword; j++ )
				{
					if ( !_stricmp( token, MessageVarLookup[ j ].keyword ) )
						break;
				}
				if (!MessageVarLookup[ j ].keyword)		// not a valid message token
					continue;

				StatsMessages[MessageNum].variable_ptr[num_variations][ k++ ] = MessageVarLookup[ j ].var_ptr;
				if ( fscanf( f, " %80s", token ) != 1 )
				{
				 	IgnoreMessage = TRUE;
					break;				
				}
			}
			if (IgnoreMessage)
				break;

		}

	}

	fclose( f );



}

#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Read in the level specific text message Info.......
	Input		:		char *Filename
	Output		:		BOOL TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ReadMsgFile( char *Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int16		*	int16Pnt;
	uint32		MagicNumber;
	uint32		VersionNumber;
	uint32		* uint32Pnt;
	TEXTMSGINFO * Tmi;
	int i;

	TextActivatedOrder = 0;
	CurrentTextActivated = 0;
	TextMsgInfo = NULL;
	if( !NumOfTextMessages)
		return TRUE;
	
	File_Size = Get_File_Size( Filename );
	if( !File_Size )
	{
		// dont need water...
		return TRUE;
	}
	Buffer = calloc( File_Size+1 , 1 );
	if( !Buffer )
	{
		Msg( "ReadMsgFile : Unable to allocate file buffer\n", Filename );
		return( FALSE );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "ReadMsgFile Load Error reading %s\n", Filename );
		return( FALSE );
	}
	OrgBuffer = Buffer;

	uint32Pnt = (uint32*) Buffer;
	MagicNumber = *uint32Pnt++;
	VersionNumber = *uint32Pnt++;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MSG_VERSION_NUMBER  ) )
	{
		Msg( "ReadMsgFile() Incompatible msg file %s", Filename );
		free( OrgBuffer );
		return( FALSE );
	}

	int16Pnt = (int16*) uint32Pnt;
	if( *int16Pnt++ != NumOfTextMessages )
	{
		Msg( "ReadMsgFile : .Msg not compatible with .Txt\n" );
		return( FALSE );
	}
#ifdef DEBUG_TEXT_MESSAGES
	TextMsgInfo = (TEXTMSGINFO*) malloc( ( NumOfTextMessages + 1024 ) * sizeof(TEXTMSGINFO) );
#else
	TextMsgInfo = (TEXTMSGINFO*) malloc( NumOfTextMessages * sizeof(TEXTMSGINFO) );
#endif
	if( !TextMsgInfo )
	{
		Msg( "ReadMsgFile : Unable to allocate memory buffer\n", Filename );
		return( FALSE );
	}
	Tmi = TextMsgInfo;

	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		Tmi->Activated = FALSE;
		Tmi->Type = *int16Pnt++;
		Tmi->ActivatedOrder = -1;
		Tmi++;
	}
#ifdef DEBUG_TEXT_MESSAGES

	
	Tmi = TextMsgInfo;
	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		Tmi->Activated = TRUE;
		Tmi->ActivatedOrder = i;
		Tmi++;
	}

	TextMessagesPnt[i] = LT_TEAM_1									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_TEAM_2									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_TEAM_3									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_TEAM_4									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LIVES										   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = TEAM											   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = IS_JOINING_THE_GAME							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_SESSION_HAS_BEEN_LOST_PLEASE_QUIT		   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_BECOME_THE_HOST						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = HAS_LEFT_THE_GAME							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOUVE_BEEN_SCATTERED							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = FROM_YOUR_OWN_TEAM							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU											   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = GOT_CAUGHT_WITH_A_BOMB						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = WITH_THE_BOUNTY								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = HIMSELF										   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = ON_HIS_OWN_TEAM								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = PACKETS_PER_SECOND_SET						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_COLOUR_TEAM_HAVE_SCORED 					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG		   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED		   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = HAS_GOT_THE_BOUNTY							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_GOT_A_NEW_BOMB 						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = ON_YOUR_OWN_TEAM								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE  ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = CONNECTION_INITIALIZATION_ERROR				   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COULDNT_GET_SERVICE_PROVIDER_CAPS			   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COULDNT_OPEN_SESSION							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COULDNT_CREATE_PLAYER 						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = NO_MULTIPLAYER_LEVELS 						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_TROJAX								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_PYROLITE							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_TRANSPULSE							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SUSSGUN							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_LASER								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_MUG								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_MUG_PACK							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SOLARIS							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SOLARIS_PACK						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_THIEF								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SCATTER							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GRAVGON							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_MFRL								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_TITAN								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_PURGE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_PINE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_QUANTUM							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SPIDER								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_PARASITE							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_FLARE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GENAMMO							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_PYROFUEL							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SUSSAMMO							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_POWER_POD							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SHIELD								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_INVULBERABILITY					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_EXTRA_LIFE							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_TARGETING_COMPUTER					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_SMOKE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_NITRO								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GOGGLES							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GOLD_BARS							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_STEALTH							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_CRYSTAL							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_ORBITAL							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GOLDEN_POWER_POD					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_DNA								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_BOMB								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_PICKUP_GOLDEN_IDOL						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_ALREADY_HAVE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_ALREADY_HAVE_A							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_ALREADY_HAVE_MAX							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = FULL_POWER_LEVEL								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = POWER_POD_LEVEL								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_ALREADY_HAVE_MAX_POWER					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_CANT_HANDLE_ANY_MORE						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOUVE_EARNED_EXTRA_LIFE						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_ONE_GOLD_BAR						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_GOLD_BARS							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_ONE_CRYSTAL							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_CRYSTALS							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = TAKE_FLAG_TO_GOAL							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = SOMEONE_HAS_GOT_THE_FLAG						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_HAVE_GOT_THE_BOUNTY						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = TEAM_FLAG_RETURNED							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = RETURN_TEAM_FLAG								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = CANNOT_PICKUP_OWN_FLAG						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = SOMEONE_HAS_OTHER_TEAM_FLAG					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = FLAG_DRIFTED_BACK							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = FLAG_ESCAPED									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = A_BOMB_KILLED_YOU							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = AN_ENEMY_KILLED_YOU							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = ENEMY_KILLED_YOU								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_KILLED_YOURSELF_HOW						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = SOMEONE_KILLED_YOU							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = FROM_YOUR_OWN_TEAM							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_DIED										   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_DEATHMETHOD_KILLED						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_DEATHMETHOD_PURGE_MINED					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_DEATHMETHOD_PINE_MINED					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_DEATHMETHOD_QUANTUM_MINED					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LT_DEATHMETHOD_SPIDER_MINED					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_DONT_HAVE_ANY_AMMO						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_DONT_HAVE_THAT_WEAPON					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LASER_OVERHEATED								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = INCOMING_MISSILE								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOUVE_BEEN_SCATTERED							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = TEAM_SCORED									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = OTHER_TEAM_FLAG_RETURNED						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_NEED_FLAG								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU_KILLED_YOURSELF							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = NO_LIVES_LEFT								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LAST_LIFE									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = ONE_LIFE_LEFT								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = LIVES_LEFT									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = RESTART_ACTIVATED							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = SHIELD_CRITICAL								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = HULL_CRITICAL								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = OUT_OF_TIME									   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = IF_YOU_CANT_TAKE_THE_HEAT					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = GET_OUT_OF_THE_KITCHEN						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = GAME_COMPLETE1								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = GAME_COMPLETE2								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = GAME_COMPLETE3								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_NOT_ALL_CRYSTALS1					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_NOT_ALL_CRYSTALS2					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_NOT_ALL_CRYSTALS3					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_NOT_ALL_CRYSTALS4					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_NOT_ALL_CRYSTALS5					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALL_CRYSTALS1						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALL_CRYSTALS2						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALL_CRYSTALS3						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALL_CRYSTALS4						   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALLBUT1_CRYSTALS1					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALLBUT1_CRYSTALS2					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALLBUT1_CRYSTALS3					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALLBUT1_CRYSTALS4					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = COMPLETE_ALLBUT1_CRYSTALS5					   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = PRESS_ANY_KEY1								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = PRESS_ANY_KEY2								   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	Tmi->ActivatedOrder = -1;

	TextActivatedOrder = i;
#endif
	free( OrgBuffer );
	return TRUE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Free level specific text message info.......
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FreeMsgFile( void )
{
	if( TextMsgInfo )
	{
		free( TextMsgInfo );
		TextMsgInfo = NULL;
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Trigger text message.......
	Input		:		uint16 * Data
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayOldTextMessage( void )
{
	TEXTMSGINFO * Tmi;
	int16	i;


	Tmi = TextMsgInfo;
	if( !Tmi )
		return;

#ifdef DEBUG_ON
	for( i = 0 ; i < MAXTEXTMESSAGES ; i++ )
#else
	for( i = 0 ; i < NumOfTextMessages ; i++ )
#endif
	{
		if( Tmi->ActivatedOrder == CurrentTextActivated )
		{
			PlaySfx( SFX_MessageBeep, 1.0F );
			AddMessageToQue( "%s\n", TextMessagesPnt[i] );
			return;
		}
		Tmi++;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Show next previously Triggerd text message.......
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void NextTextMessage( void )
{
	CurrentTextActivated++;
	if( CurrentTextActivated >= TextActivatedOrder )
	{
		CurrentTextActivated = 0;
	}
	if( TextActivatedOrder )
	{
		DisplayOldTextMessage();
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Show Last previously Triggerd text message.......
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void LastTextMessage( void )
{
	CurrentTextActivated--;
	if( CurrentTextActivated < 0 )
	{
		CurrentTextActivated = TextActivatedOrder;
		if( TextActivatedOrder )
			CurrentTextActivated--;
	}
	if( TextActivatedOrder )
	{
		DisplayOldTextMessage();
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Save all text...
	Input		:		FILE * fp
	Output		:		BOOL
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SaveAllText( FILE * fp )
{
	TEXTMSGINFO * Tmi;
	int i;

	fwrite( &NumOfTextMessages, sizeof( NumOfTextMessages ), 1, fp );
	fwrite( &TextActivatedOrder, sizeof( TextActivatedOrder ), 1, fp );
	fwrite( &CurrentTextActivated, sizeof( CurrentTextActivated ), 1, fp );

	Tmi = TextMsgInfo;
	if( !Tmi )
		return fp;

	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		fwrite( &Tmi->Activated, sizeof( Tmi->Activated ), 1, fp );
		fwrite( &Tmi->ActivatedOrder, sizeof( Tmi->ActivatedOrder ), 1, fp );
		Tmi++;
	}
	return fp;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Save all text...
	Input		:		FILE * fp
	Output		:		BOOL
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE *	LoadAllText( FILE * fp )
{
	TEXTMSGINFO * Tmi;
	int i;
	uint16 TempNumOfTextMessages = 0;


	fread( &TempNumOfTextMessages, sizeof( NumOfTextMessages ), 1, fp );
	if( TempNumOfTextMessages != NumOfTextMessages )
		return NULL;

	fread( &TextActivatedOrder, sizeof( TextActivatedOrder ), 1, fp );
	fread( &CurrentTextActivated, sizeof( CurrentTextActivated ), 1, fp );

	Tmi = TextMsgInfo;
	if( !Tmi )
		return fp;

	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		fread( &Tmi->Activated, sizeof( Tmi->Activated ), 1, fp );
		fread( &Tmi->ActivatedOrder, sizeof( Tmi->ActivatedOrder ), 1, fp );
		Tmi++;
	}
	return fp;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Fill Status messages..
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
char * StatusTab[256];
void FillStatusTab( void )
{
	int i;
	for( i = 0 ; i < 256 ; i++ )
	{
		StatusTab[i] = "No Status Defined";
	}

    StatusTab[STATUS_PlayingAVI                           ] = "PlayingAVI   ";
    StatusTab[STATUS_Title                                ] = "Title        ";
    StatusTab[STATUS_Copyright                            ] = "Copyright    ";
    StatusTab[STATUS_StartingMultiplayer                  ] = "Preping      ";
    StatusTab[STATUS_PreStartingMultiplayerSynch          ] = "Preping      ";
    StatusTab[STATUS_StartingMultiplayerSynch             ] = "Ready        ";
    StatusTab[STATUS_QuitCurrentGame                      ] = "Quiting      ";
    StatusTab[STATUS_GetPlayerNum                         ] = "Preping      ";
    StatusTab[STATUS_PlayingDemo                          ] = "PlayingDemo  "; 
    StatusTab[STATUS_Normal                               ] = "Ready        ";
    StatusTab[STATUS_LevelEnd                             ] = "Preping      ";
    StatusTab[STATUS_BetweenLevels                        ] = "Preping      ";
    StatusTab[STATUS_ViewingScore                         ] = "Viewing Score";
    StatusTab[STATUS_WaitingAfterScore                    ] = "Ready        ";
    StatusTab[STATUS_Joining                              ] = "Joining      ";
    StatusTab[STATUS_Left                                 ] = "Left         ";
    StatusTab[STATUS_ChangeLevelPostInitView              ] = "Preping      ";
    StatusTab[STATUS_ChangeLevelPostPlayingDemo           ] = "Preping      ";
    StatusTab[STATUS_ChangeLevelPostAttractMode           ] = "Preping      ";
    StatusTab[STATUS_AttractMode                          ] = "AttractMode  ";
    StatusTab[STATUS_StartingSinglePlayer                 ] = "StartingSingl";
    StatusTab[STATUS_PostStartingSinglePlayer             ] = "PostStartingS";
    StatusTab[STATUS_SinglePlayer                         ] = "SinglePlayer ";
    StatusTab[STATUS_ViewingStats                         ] = "ViewingStats ";
    StatusTab[STATUS_TitleLoadGameStartingSinglePlayer    ] = "TitleLoadGame";
    StatusTab[STATUS_TitleLoadGamePostStartingSinglePlayer] = "TitleLoadGame";
    StatusTab[STATUS_ChangeLevelPostLoadGame              ] = "ChangeLevelPo";
    StatusTab[STATUS_InGameLoadGameStartingSinglePlayer   ] = "InGameLoadGam";
    StatusTab[STATUS_WaitingToStartSinglePlayer           ] = "WaitingToStar";
    StatusTab[STATUS_WaitingToStartMultiPlayerHost        ] = "Preping      ";
    StatusTab[STATUS_WaitingToStartMultiPlayerClient      ] = "Preping      ";
    StatusTab[STATUS_WaitingToStartTeamGame               ] = "Perping      ";
    StatusTab[STATUS_QuittingFromTitles                   ] = "QuittingFromT";
    StatusTab[STATUS_WaitingToStartDemo                   ] = "WaitingToStar";
    StatusTab[STATUS_WaitingToStartEndGameSequence        ] = "WaitingToStar";
    StatusTab[STATUS_WaitingForLobbyConnect               ] = "WaitingForLob";
    StatusTab[STATUS_QuittingFromInGame                   ] = "Quiting      ";
    StatusTab[STATUS_SplashScreen                         ] = "SplashScreen ";
    StatusTab[STATUS_FinishedShowingSplashScreen          ] = "FinishedShowi";
    StatusTab[STATUS_InitView_0                           ] = "Loading 1/10 ";
    StatusTab[STATUS_InitView_1                           ] = "Loading 2/10 ";
    StatusTab[STATUS_InitView_2                           ] = "Loading 3/10 ";
    StatusTab[STATUS_InitView_3                           ] = "Loading 4/10 ";
    StatusTab[STATUS_InitView_4                           ] = "Loading 5/10 ";
    StatusTab[STATUS_InitView_5                           ] = "Loading 6/10 ";
    StatusTab[STATUS_InitView_6                           ] = "Loading 7/10 ";
    StatusTab[STATUS_InitView_7                           ] = "Loading 8/10 ";
    StatusTab[STATUS_InitView_8                           ] = "Loading 9/10 ";
    StatusTab[STATUS_InitView_9                           ] = "Loading 10/10";
    StatusTab[STATUS_InitView_10                          ] = "Loading 10/10";
    StatusTab[STATUS_LeftCrashed                          ] = "LeftCrashed  ";
    StatusTab[STATUS_Null                                 ] = "Null         ";




#if 0
    StatusTab[STATUS_PlayingAVI                           ] = "PlayingAVI                 ";
    StatusTab[STATUS_Title                                ] = "Title                      ";
    StatusTab[STATUS_Copyright                            ] = "Copyright                  ";
    StatusTab[STATUS_StartingMultiplayer                  ] = "StartingMultiplayer        ";
    StatusTab[STATUS_PreStartingMultiplayerSynch          ] = "PreStartingMultiplayerSy...";
    StatusTab[STATUS_StartingMultiplayerSynch             ] = "StartingMultiplayerSynch   ";
    StatusTab[STATUS_QuitCurrentGame                      ] = "QuitCurrentGame            ";
    StatusTab[STATUS_GetPlayerNum                         ] = "GetPlayerNum               ";
    StatusTab[STATUS_PlayingDemo                          ] = "PlayingDemo                "; 
    StatusTab[STATUS_Normal                               ] = "Normal                     ";
    StatusTab[STATUS_LevelEnd                             ] = "LevelEnd                   ";
    StatusTab[STATUS_BetweenLevels                        ] = "BetweenLevels              ";
    StatusTab[STATUS_ViewingScore                         ] = "ViewingScore               ";
    StatusTab[STATUS_WaitingAfterScore                    ] = "WaitingAfterScore          ";
    StatusTab[STATUS_Joining                              ] = "Joining                    ";
    StatusTab[STATUS_Left                                 ] = "Left                       ";
    StatusTab[STATUS_ChangeLevelPostInitView              ] = "ChangeLevelPostInitView    ";
    StatusTab[STATUS_ChangeLevelPostPlayingDemo           ] = "ChangeLevelPostPlayingDe...";
    StatusTab[STATUS_ChangeLevelPostAttractMode           ] = "ChangeLevelPostAttractMo...";
    StatusTab[STATUS_AttractMode                          ] = "AttractMode                ";
    StatusTab[STATUS_StartingSinglePlayer                 ] = "StartingSinglePlayer       ";
    StatusTab[STATUS_PostStartingSinglePlayer             ] = "PostStartingSinglePlayer   ";
    StatusTab[STATUS_SinglePlayer                         ] = "SinglePlayer               ";
    StatusTab[STATUS_ViewingStats                         ] = "ViewingStats               ";
    StatusTab[STATUS_TitleLoadGameStartingSinglePlayer    ] = "TitleLoadGameStartingSin...";
    StatusTab[STATUS_TitleLoadGamePostStartingSinglePlayer] = "TitleLoadGamePost...       ";
    StatusTab[STATUS_ChangeLevelPostLoadGame              ] = "ChangeLevelPostLoadGame    ";
    StatusTab[STATUS_InGameLoadGameStartingSinglePlayer   ] = "InGameLoadGameStartingSi...";
    StatusTab[STATUS_WaitingToStartSinglePlayer           ] = "WaitingToStartSinglePlay...";
    StatusTab[STATUS_WaitingToStartMultiPlayerHost        ] = "WaitingToStartMultiPlaye...";
    StatusTab[STATUS_WaitingToStartMultiPlayerClient      ] = "WaitingToStartMultiPlaye...";
    StatusTab[STATUS_WaitingToStartTeamGame               ] = "WaitingToStartTeamGame     ";
    StatusTab[STATUS_QuittingFromTitles                   ] = "QuittingFromTitles         ";
    StatusTab[STATUS_WaitingToStartDemo                   ] = "WaitingToStartDemo         ";
    StatusTab[STATUS_WaitingToStartEndGameSequence        ] = "WaitingToStartEndGameSeq...";
    StatusTab[STATUS_WaitingForLobbyConnect               ] = "WaitingForLobbyConnect     ";
    StatusTab[STATUS_QuittingFromInGame                   ] = "QuittingFromInGame         ";
    StatusTab[STATUS_SplashScreen                         ] = "SplashScreen               ";
    StatusTab[STATUS_FinishedShowingSplashScreen          ] = "FinishedShowingSplashScr...";
    StatusTab[STATUS_InitView_0                           ] = "InitView_0                 ";
    StatusTab[STATUS_InitView_1                           ] = "InitView_1                 ";
    StatusTab[STATUS_InitView_2                           ] = "InitView_2                 ";
    StatusTab[STATUS_InitView_3                           ] = "InitView_3                 ";
    StatusTab[STATUS_InitView_4                           ] = "InitView_4                 ";
    StatusTab[STATUS_InitView_5                           ] = "InitView_5                 ";
    StatusTab[STATUS_InitView_6                           ] = "InitView_6                 ";
    StatusTab[STATUS_InitView_7                           ] = "InitView_7                 ";
    StatusTab[STATUS_InitView_8                           ] = "InitView_8                 ";
    StatusTab[STATUS_InitView_9                           ] = "InitView_9                 ";
    StatusTab[STATUS_InitView_10                          ] = "InitView_10                ";
    StatusTab[STATUS_LeftCrashed                          ] = "LeftCrashed                ";
    StatusTab[STATUS_Null                                 ] = "Null                       ";
#endif
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Display Status messages..
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayStatusMessages( void )
{
	int i;
	char buf[256];
	int	Temp;
	if( !DS )
		return;
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{

		Temp = (int32) ((((LargeTime - LastPacketTime[i]) ) / Freq ) );
		if( (Temp > 999999) || (GameStatus[i] == STATUS_Null) || (i == WhoIAm) )
			Temp = 999999;

		sprintf( buf, "%-8s status %8s %3d lpt %1d \n", &Names[i][0] , StatusTab[GameStatus[i]] , Temp , Ships[i].enable );
		CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (i * (FontHeight+(FontHeight>>1)) ) , 1 );
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Build Reliability Tab...
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void BuildReliabilityTab( void )
{
	int i;
	int Temp;
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		ReliabilityTab[i] = 0;
		if( !IsServerGame || IsServer )
		{
			if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) && (i != WhoIAm) )
			{
				if( BadConnection[i] )
				{
					ReliabilityTab[i]++;
				}
				Temp = (int32) (((LargeTime - LastPacketTime[i]) * 1000 ) / Freq);

				if( Temp >= 10000 )
				{
					ReliabilityTab[i]++;
					if( Temp >= 30000 )
						ReliabilityTab[i]++;
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Display a connection setting...
	Input		:		void
	Output		:		void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
__inline
void DisplayConnectionStatus( int num , int x , int y)
{
    RECT    src;
    HRESULT ddrval;
	int offset;

	if( !num )
		return;
	num--;

	if( bPolyText && PolyText[MyGameStatus])
	{
		AddScreenPolyText( (uint16) (83+num), (float) x , (float) y+2, 255, 255, 255, 255 );
	}else{

		if( ModeCase < Mode512X384 )
		{
			src.top = 128-8;
			src.left = 16 + (num*8);
			offset = 0;

		}else{
			src.top = 256-8;
			src.left = 0 + (num*8);
			offset = 2;
		}
		src.bottom = src.top + 4;
		src.right = src.left + 4;
		while( 1 )
		{
			ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x , y+offset, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
			if( ddrval == DD_OK )
				break;
			if( ddrval == DDERR_SURFACELOST )
			{
				d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
				d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
				ReInitFont();
				break;
			}
			if( ddrval != DDERR_WASSTILLDRAWING )
				break;
		}

	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Init the font surface...
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitFont( BOOL OverridePolytext )
{
	LPDIRECTDRAWPALETTE ddpal;
	uint16 i;
	uint8 e;
	int x,y;
	int col;

	// if font already initialised...
	if (lpDDSTwo && !OverridePolytext )
		return;
	if( lpDDSTwo )
	{
		ReleaseDDSurf(lpDDSTwo);
		lpDDSTwo = NULL;
	}

#if 0
	if( ModeCase < Mode512X384 )
   	{
   		lpDDSTwo = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font.bmp", 0, 0 );
   		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font.bmp");
		lpDDSTwo->lpVtbl->SetPalette( lpDDSTwo , ddpal );
   		DDSetColorKey( lpDDSTwo, RGB_MAKE( 0 , 0 , 0 ) );
   		FontWidth = 4;
   		FontHeight = 5;
   		FontSourceWidth = 4;
   		FontSourceHeight = 5;

   	}
	else{
//   			if( ModeCase < Mode640X480 )
//   			{
   		
   			lpDDSTwo = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font512.bmp", 0, 0 );

			ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font512.bmp");
			lpDDSTwo->lpVtbl->SetPalette( lpDDSTwo , ddpal );
   			DDSetColorKey( lpDDSTwo, RGB_MAKE( 0 , 0 , 0 ) );
   			FontWidth = 8;
   			FontHeight = 8;
   			FontSourceWidth = 8;
   			FontSourceHeight = 8;
//   			}else{
//   				lpDDSTwo = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font800.bmp", 0, 0 );
//   				ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font800.bmp");
//   				lpDDSTwo->lpVtbl->SetPalette( lpDDSTwo , ddpal );
//   				DDSetColorKey( lpDDSTwo, RGB_MAKE( 255 , 0 , 255 ) );
//   				FontWidth = 9;
//   				FontHeight = 16;
//   				FontSourceWidth = 9;
//   				FontSourceHeight = 16;
//   			}
   	}
#endif
	
	if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
	{
		lpDDSTwo = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font512.bmp", 0, 0 );

		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font512.bmp");
		lpDDSTwo->lpVtbl->SetPalette( lpDDSTwo , ddpal );
		DDSetColorKey( lpDDSTwo, RGB_MAKE( 0 , 0 , 0 ) );
		FontWidth = 8;
		FontHeight = 8;
		FontSourceWidth = 8;
		FontSourceHeight = 8;
	}else
	{
   		lpDDSTwo = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font.bmp", 0, 0 );
   		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font.bmp");
		lpDDSTwo->lpVtbl->SetPalette( lpDDSTwo , ddpal );
   		DDSetColorKey( lpDDSTwo, RGB_MAKE( 0 , 0 , 0 ) );
   		FontWidth = 4;
   		FontHeight = 5;
   		FontSourceWidth = 4;
   		FontSourceHeight = 5;
	}

	for( i = 0; i < 0x100 ; i++ )
	{
		CharTrans[i] = 0;
	}

	e = 1;
	for( i = 0 ; i < 0x30 ; i++ )
	{
		CharTrans[i] = 0;
	}
	for( ; i < 0x3a ; i++ , e++ )		// 0 - 9
	{
		CharTrans[i] = e;
	}
	for( ; i < 0x41 ; i++ )
	{
		CharTrans[i] = 0;
	}
	for( ; i < 0x5b ; i++ , e++ )		// a - z
	{
		CharTrans[i] = e;
	}
	for( ; i < 0x61 ; i++ )
	{
		CharTrans[i] = 0;
	}
	e = 11;
	for( ; i < 0x7b ; i++ , e++ )		// A - Z
	{
		CharTrans[i] = e;
	}
	CharTrans['.'] = e++;
	if ( bPolyText && !OverridePolytext)
		e = 73;
	CharTrans[(uint8)','] = e++;
	CharTrans[(uint8)';'] = e++;
	CharTrans[(uint8)'['] = e;
	CharTrans[(uint8)'('] = e++;
	CharTrans[(uint8)']'] = e;
	CharTrans[(uint8)')'] = e++;
	CharTrans[(uint8)'*'] = e++;
	CharTrans[(uint8)'/'] = e++;
	CharTrans[(uint8)'+'] = e++;
	CharTrans[(uint8)'-'] = e++;
	CharTrans[(uint8)'='] = e++;
	CharTrans[(uint8)'\''] = e++;

	CharTrans[(uint8)'å'] = 11; //'A'
	CharTrans[(uint8)'Å'] = 11; //'A'
	CharTrans[(uint8)'ç'] = 13; //'C'
	CharTrans[(uint8)'Ç'] = 13; //'C'

	if ( bPolyText && !OverridePolytext)
	{
		e = 86;
		CharTrans[(uint8)'à'] = e;
		CharTrans[(uint8)'À'] = e++;
		CharTrans[(uint8)'á'] = e;
		CharTrans[(uint8)'Á'] = e++;
		CharTrans[(uint8)'ä'] = e;
		CharTrans[(uint8)'Ä'] = e++;
		CharTrans[(uint8)'â'] = e;
		CharTrans[(uint8)'Â'] = e++;
		CharTrans[(uint8)'è'] = e;
		CharTrans[(uint8)'È'] = e++;
		CharTrans[(uint8)'é'] = e;
		CharTrans[(uint8)'É'] = e++;
		CharTrans[(uint8)'ë'] = e;
		CharTrans[(uint8)'Ë'] = e++;
		CharTrans[(uint8)'ê'] = e;
		CharTrans[(uint8)'Ê'] = e++;
		CharTrans[(uint8)'ì'] = e;
		CharTrans[(uint8)'Ì'] = e++;
		CharTrans[(uint8)'í'] = e;
		CharTrans[(uint8)'Í'] = e++;
		CharTrans[(uint8)'ï'] = e;
		CharTrans[(uint8)'Ï'] = e++;
		CharTrans[(uint8)'î'] = e;
		CharTrans[(uint8)'Î'] = e++;
		CharTrans[(uint8)'ò'] = e;
		CharTrans[(uint8)'Ò'] = e++;
		CharTrans[(uint8)'ó'] = e;
		CharTrans[(uint8)'Ó'] = e++;
		CharTrans[(uint8)'ö'] = e;
		CharTrans[(uint8)'Ö'] = e++;
		CharTrans[(uint8)'ô'] = e;
		CharTrans[(uint8)'Ô'] = e++;
		CharTrans[(uint8)'ù'] = e;
		CharTrans[(uint8)'Ù'] = e++;
		CharTrans[(uint8)'ú'] = e;
		CharTrans[(uint8)'Ú'] = e++;
		CharTrans[(uint8)'ü'] = e;
		CharTrans[(uint8)'Ü'] = e++;
		CharTrans[(uint8)'û'] = e;
		CharTrans[(uint8)'Û'] = e++;
		CharTrans[(uint8)'ñ'] = e;
		CharTrans[(uint8)'Ñ'] = e++;
	}else{
		CharTrans[(uint8)'à'] = 'à';
		CharTrans[(uint8)'À'] = 'à';
		CharTrans[(uint8)'á'] = 'á';
		CharTrans[(uint8)'Á'] = 'á';
		CharTrans[(uint8)'ä'] = 'ä';
		CharTrans[(uint8)'Ä'] = 'ä';
		CharTrans[(uint8)'â'] = 'â';
		CharTrans[(uint8)'Â'] = 'â';
		CharTrans[(uint8)'è'] = 'è';
		CharTrans[(uint8)'È'] = 'è';
		CharTrans[(uint8)'é'] = 'é';
		CharTrans[(uint8)'É'] = 'é';
		CharTrans[(uint8)'ë'] = 'ë';
		CharTrans[(uint8)'Ë'] = 'ë';
		CharTrans[(uint8)'ê'] = 'ê';
		CharTrans[(uint8)'Ê'] = 'ê';
		CharTrans[(uint8)'ì'] = 'ì';
		CharTrans[(uint8)'Ì'] = 'ì';
		CharTrans[(uint8)'í'] = 'í';
		CharTrans[(uint8)'Í'] = 'í';
		CharTrans[(uint8)'ï'] = 'ï';
		CharTrans[(uint8)'Ï'] = 'ï';
		CharTrans[(uint8)'î'] = 'î';
		CharTrans[(uint8)'Î'] = 'î';
		CharTrans[(uint8)'ò'] = 'ò';
		CharTrans[(uint8)'Ò'] = 'ò';
		CharTrans[(uint8)'ó'] = 'ó';
		CharTrans[(uint8)'Ó'] = 'ó';
		CharTrans[(uint8)'ö'] = 'ö';
		CharTrans[(uint8)'Ö'] = 'ö';
		CharTrans[(uint8)'ô'] = 'ô';
		CharTrans[(uint8)'Ô'] = 'ô';
		CharTrans[(uint8)'ù'] = 'ù';
		CharTrans[(uint8)'Ù'] = 'ù';
		CharTrans[(uint8)'ú'] = 'ú';
		CharTrans[(uint8)'Ú'] = 'ú';
		CharTrans[(uint8)'ü'] = 'ü';
		CharTrans[(uint8)'Ü'] = 'ü';
		CharTrans[(uint8)'û'] = 'û';
		CharTrans[(uint8)'Û'] = 'û';
		CharTrans[(uint8)'ñ'] = 'ñ';
		CharTrans[(uint8)'Ñ'] = 'ñ';
	}

	for( col = 0 ; col < MAXFONTCOLOURS ; col++ )
	{
		for( e = 0 ; e < 255 ; e++ )
		{
			if( col < 8 )
			{
				y = ( ( e / 16 )* FontSourceHeight) + (col*(FontSourceHeight*3));
				x = ( e & 15 )* FontSourceWidth ;
			}else{
				y = ( ( e / 16 )* FontSourceHeight) + (6*(FontSourceHeight*3));
				x = ( ( e & 15 )* FontSourceWidth)+ ( FontSourceWidth * 16 );
			}
			TextSrcX[col][e] = x;
			TextSrcY[col][e] = y;
		}

		TextSrcY[col][(uint8)'à'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'à'] = ( 0 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'á'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'á'] = ( 1 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ä'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ä'] = ( 2 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'â'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'â'] = ( 3 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'è'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'è'] = ( 4 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'é'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'é'] = ( 5 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ë'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ë'] = ( 6 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ê'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ê'] = ( 7 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ì'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ì'] = ( 8 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'í'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'í'] = ( 9 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ï'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ï'] = ( 10 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'î'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'î'] = ( 11 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ò'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ò'] = ( 12 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ó'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ó'] = ( 13 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ö'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ö'] = ( 14 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ô'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ô'] = ( 15 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ù'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ù'] = ( 0 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ú'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ú'] = ( 1 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ü'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ü'] = ( 2 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'û'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'û'] = ( 3 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ñ'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ñ'] = ( 4 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:		Init the font surface...
	Input		:		nothing
	Output		:		nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitFontTransTable( BOOL BlitText )
{
	uint16 i;
	uint8 e;
	int x,y;
	int col;


	for( i = 0; i < 0x100 ; i++ )
	{
		CharTrans[i] = 0;
	}

	e = 1;
	for( i = 0 ; i < 0x30 ; i++ )
	{
		CharTrans[i] = 0;
	}
	for( ; i < 0x3a ; i++ , e++ )		// 0 - 9
	{
		CharTrans[i] = e;
	}
	for( ; i < 0x41 ; i++ )
	{
		CharTrans[i] = 0;
	}
	for( ; i < 0x5b ; i++ , e++ )		// a - z
	{
		CharTrans[i] = e;
	}
	for( ; i < 0x61 ; i++ )
	{
		CharTrans[i] = 0;
	}
	e = 11;
	for( ; i < 0x7b ; i++ , e++ )		// A - Z
	{
		CharTrans[i] = e;
	}
	CharTrans['.'] = e++;
	if ( !BlitText)
		e = 73;
	CharTrans[(uint8)','] = e++;
	CharTrans[(uint8)';'] = e++;
	CharTrans[(uint8)'['] = e;
	CharTrans[(uint8)'('] = e++;
	CharTrans[(uint8)']'] = e;
	CharTrans[(uint8)')'] = e++;
	CharTrans[(uint8)'*'] = e++;
	CharTrans[(uint8)'/'] = e++;
	CharTrans[(uint8)'+'] = e++;
	CharTrans[(uint8)'-'] = e++;
	CharTrans[(uint8)'='] = e++;
	CharTrans[(uint8)'\''] = e++;

	CharTrans[(uint8)'å'] = 11; //'A'
	CharTrans[(uint8)'Å'] = 11; //'A'
	CharTrans[(uint8)'ç'] = 13; //'C'
	CharTrans[(uint8)'Ç'] = 13; //'C'

	if ( !BlitText)
	{
		e = 86;
		CharTrans[(uint8)'à'] = e;
		CharTrans[(uint8)'À'] = e++;
		CharTrans[(uint8)'á'] = e;
		CharTrans[(uint8)'Á'] = e++;
		CharTrans[(uint8)'ä'] = e;
		CharTrans[(uint8)'Ä'] = e++;
		CharTrans[(uint8)'â'] = e;
		CharTrans[(uint8)'Â'] = e++;
		CharTrans[(uint8)'è'] = e;
		CharTrans[(uint8)'È'] = e++;
		CharTrans[(uint8)'é'] = e;
		CharTrans[(uint8)'É'] = e++;
		CharTrans[(uint8)'ë'] = e;
		CharTrans[(uint8)'Ë'] = e++;
		CharTrans[(uint8)'ê'] = e;
		CharTrans[(uint8)'Ê'] = e++;
		CharTrans[(uint8)'ì'] = e;
		CharTrans[(uint8)'Ì'] = e++;
		CharTrans[(uint8)'í'] = e;
		CharTrans[(uint8)'Í'] = e++;
		CharTrans[(uint8)'ï'] = e;
		CharTrans[(uint8)'Ï'] = e++;
		CharTrans[(uint8)'î'] = e;
		CharTrans[(uint8)'Î'] = e++;
		CharTrans[(uint8)'ò'] = e;
		CharTrans[(uint8)'Ò'] = e++;
		CharTrans[(uint8)'ó'] = e;
		CharTrans[(uint8)'Ó'] = e++;
		CharTrans[(uint8)'ö'] = e;
		CharTrans[(uint8)'Ö'] = e++;
		CharTrans[(uint8)'ô'] = e;
		CharTrans[(uint8)'Ô'] = e++;
		CharTrans[(uint8)'ù'] = e;
		CharTrans[(uint8)'Ù'] = e++;
		CharTrans[(uint8)'ú'] = e;
		CharTrans[(uint8)'Ú'] = e++;
		CharTrans[(uint8)'ü'] = e;
		CharTrans[(uint8)'Ü'] = e++;
		CharTrans[(uint8)'û'] = e;
		CharTrans[(uint8)'Û'] = e++;
		CharTrans[(uint8)'ñ'] = e;
		CharTrans[(uint8)'Ñ'] = e++;
	}else{
		CharTrans[(uint8)'à'] = 'à';
		CharTrans[(uint8)'À'] = 'à';
		CharTrans[(uint8)'á'] = 'á';
		CharTrans[(uint8)'Á'] = 'á';
		CharTrans[(uint8)'ä'] = 'ä';
		CharTrans[(uint8)'Ä'] = 'ä';
		CharTrans[(uint8)'â'] = 'â';
		CharTrans[(uint8)'Â'] = 'â';
		CharTrans[(uint8)'è'] = 'è';
		CharTrans[(uint8)'È'] = 'è';
		CharTrans[(uint8)'é'] = 'é';
		CharTrans[(uint8)'É'] = 'é';
		CharTrans[(uint8)'ë'] = 'ë';
		CharTrans[(uint8)'Ë'] = 'ë';
		CharTrans[(uint8)'ê'] = 'ê';
		CharTrans[(uint8)'Ê'] = 'ê';
		CharTrans[(uint8)'ì'] = 'ì';
		CharTrans[(uint8)'Ì'] = 'ì';
		CharTrans[(uint8)'í'] = 'í';
		CharTrans[(uint8)'Í'] = 'í';
		CharTrans[(uint8)'ï'] = 'ï';
		CharTrans[(uint8)'Ï'] = 'ï';
		CharTrans[(uint8)'î'] = 'î';
		CharTrans[(uint8)'Î'] = 'î';
		CharTrans[(uint8)'ò'] = 'ò';
		CharTrans[(uint8)'Ò'] = 'ò';
		CharTrans[(uint8)'ó'] = 'ó';
		CharTrans[(uint8)'Ó'] = 'ó';
		CharTrans[(uint8)'ö'] = 'ö';
		CharTrans[(uint8)'Ö'] = 'ö';
		CharTrans[(uint8)'ô'] = 'ô';
		CharTrans[(uint8)'Ô'] = 'ô';
		CharTrans[(uint8)'ù'] = 'ù';
		CharTrans[(uint8)'Ù'] = 'ù';
		CharTrans[(uint8)'ú'] = 'ú';
		CharTrans[(uint8)'Ú'] = 'ú';
		CharTrans[(uint8)'ü'] = 'ü';
		CharTrans[(uint8)'Ü'] = 'ü';
		CharTrans[(uint8)'û'] = 'û';
		CharTrans[(uint8)'Û'] = 'û';
		CharTrans[(uint8)'ñ'] = 'ñ';
		CharTrans[(uint8)'Ñ'] = 'ñ';
	}

	for( col = 0 ; col < MAXFONTCOLOURS ; col++ )
	{
		for( e = 0 ; e < 255 ; e++ )
		{
			if( col < 8 )
			{
				y = ( ( e / 16 )* FontSourceHeight) + (col*(FontSourceHeight*3));
				x = ( e & 15 )* FontSourceWidth ;
			}else{
				y = ( ( e / 16 )* FontSourceHeight) + (6*(FontSourceHeight*3));
				x = ( ( e & 15 )* FontSourceWidth)+ ( FontSourceWidth * 16 );
			}
			TextSrcX[col][e] = x;
			TextSrcY[col][e] = y;
		}

		TextSrcY[col][(uint8)'à'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'à'] = ( 0 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'á'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'á'] = ( 1 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ä'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ä'] = ( 2 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'â'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'â'] = ( 3 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'è'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'è'] = ( 4 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'é'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'é'] = ( 5 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ë'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ë'] = ( 6 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ê'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ê'] = ( 7 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ì'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ì'] = ( 8 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'í'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'í'] = ( 9 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ï'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ï'] = ( 10 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'î'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'î'] = ( 11 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ò'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ò'] = ( 12 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ó'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ó'] = ( 13 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ö'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ö'] = ( 14 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ô'] = (7*(FontSourceHeight*3));
		TextSrcX[col][(uint8)'ô'] = ( 15 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ù'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ù'] = ( 0 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ú'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ú'] = ( 1 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ü'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ü'] = ( 2 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'û'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'û'] = ( 3 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
		TextSrcY[col][(uint8)'ñ'] = (7*(FontSourceHeight*3))+FontSourceHeight;
		TextSrcX[col][(uint8)'ñ'] = ( 4 * FontSourceWidth)+ ( FontSourceWidth * 16 );			
	}
}



#if 0
  Dec Chr  Dec Chr  Dec Chr  
  --- ---  --- ---  --- ---  
  128  Ç   144  É   160  á   
  129  ü   145  æ   161  í   
  130  é   146  Æ   162  ó   
  131  â   147  ô   163  ú   
  132  ä   148  ö   164  ñ   
  133  à   149  ò   165  Ñ   
  134  å   150  û   166  ª   
  135  ç   151  ù   167  º   
  136  ê   152  ÿ   168  ¿   
  137  ë   153  Ö   169  _   
  138  è   154  Ü   170  ¬   
  139  ï   155  ¢   171  ½   
  140  î   156  £   172  ¼   
  141  ì   157  ¥   173  ¡   
  142  Ä   158  P   174  «   
  143  Å   159  ƒ   175  »   
#endif
