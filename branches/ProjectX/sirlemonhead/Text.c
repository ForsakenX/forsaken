
/*===================================================================
		Include File...	
===================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include "typedefs.h"
#include "New3D.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "ships.h"
#include "main.h"
#include "Title.h"
#include "Text.h"
#include "screenpolys.h"
#include "magic.h"
#include "XMem.h"
#include "ddsurfhand.h"
#include "local.h"
#include "stats.h"
#include "util.h"
#include "d3dappi.h"

#define MSG_VERSION_NUMBER 1


/*===================================================================
		Externals ...
===================================================================*/

//#define SFX_DEBUG
extern BOOL Debug;
extern BOOL	CanDoDamage[MAX_PLAYERS+1];
extern BOOL	PlayDemo;
extern LONGLONG	LastPacketTime[MAX_PLAYERS+1];
extern LONGLONG	Freq;
extern BOOL	CTF;
extern BOOL CaptureTheFlag;
extern BOOL BountyHunt;

extern LPDIRECT3DSURFACE9      lpFontSurface;       // Font Bitmap
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
extern	BOOL	bPolyText;
extern	int16			Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16					Lives;
extern	int16					MaxKills;
extern	BOOL                    IsHost;
extern	int16			NewLevelNum;
extern	int16			NumLevels;
extern	int				TeamFlag[ MAX_TEAMS ];
extern	LONGLONG	LargeTime;

// color names are defined in text.h colors_t
uint8 Colourtrans[MAXFONTCOLOURS][3] = {
	// r, g, b   values...
	{ 192,192,192 },	// gray 
	{ 255,64,64 },		// red 
	{ 64,255,64 },		// green 
	{ 64,64,255 },		// blue 
	{ 255,255,64 },		// yellow
	{ 64,255,255 },		// cyan
	{ 255,64,255 },		// purple
	{ 128,255,128 },	// off green
	{ 64,64,64 },		// dark gray
	{ 0,0,0 },			// black
	{ 255,255,255 }		// white
};

// custom colour messages (Title.c)
extern int SystemMessageColour;

// (stats.c)
extern int GetPlayerByRank(int Player);

/*===================================================================
		Globals ...
===================================================================*/

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

BOOL DS = FALSE;

int FontWidth = 4;
int FontHeight = 5;
int FontSourceWidth = 4;
int FontSourceHeight = 5;

int16	NumOfActivePlayers = 0;

int	PermX;
uint16 big[4] = { 10000 , 1000 , 100 , 10 };
uint16 little[4] = { 1000 , 100 , 10 , 1 };

int CurrentMessage = 0;
float MessageTime[MAX_MESSAGES] = { 0.0F , 0.0F , 0.0F , 0.0F };
int MessageColour[MAX_MESSAGES];
char MessageBank[MAX_MESSAGES][MAXPERLINE];
char MessageBankLong[MAX_MESSAGES_LONG][MAXPERLINE];
int MessageColourLong[MAX_MESSAGES_LONG];
uint8	CharTrans[256];

BOOL	PolyText[255];

uint16 NumOfTextMessages = 0;
char * TextMessagesPnt[MAXTEXTMESSAGES];
char * TextMessages = NULL;
TEXTMSGINFO * TextMsgInfo = NULL;

char StatsMessageFile[] = "data\\txt\\statsmessages.txt";

STATSMESSAGE StatsMessages[MAX_STATS_MESSAGES];


/*===================================================================
	Procedure	:		Print a int16 number in small 4x4 chars..
	Input		:		int16 num, uint16 x , uint16 y
	Output		:		nothing
===================================================================*/

int Printint16( int16 num , int x , int y , int color )
{
	static char buf[ 128 ];

	if( num != -32767 )
	{
		sprintf( buf, "%hd", num );
	}else{
		sprintf( buf, "%hd", 0 );
	}
	Print4x5Text( buf, x, y, color );
	return strlen(buf);
}


/*===================================================================
	Procedure	:		Print a uint16 number in small 4x4 chars..
	Input		:		uint16 num, uint16 x , uint16 y
	Output		:		nothing
===================================================================*/

void Printuint16( uint16 tempnum , int x , int y , int col )
{
	int i;
	int num;
    RECT  src;
	POINT dest;
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
			}
			else
			{	
				src.top = TextSrcY[col][num+1];
				src.left = TextSrcX[col][num+1];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;

				dest.x = x;
				dest.y = y;

				FSBlit( lpFontSurface, FSBackBuffer, &src, &dest );
			}
			x += FontWidth;
		}
	}
}





/*===================================================================
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, uint16 y
	Output		:		nothing
===================================================================*/
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

/*===================================================================
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, uint16 y
	Output		:		nothing
===================================================================*/
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

/*===================================================================
	Procedure	:		Print some Right Justified text at a specified Y
	Input		:		char * Text, int x , int y , int col
	Output		:		nothing
===================================================================*/
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


/*===================================================================
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, uint16 x , uint16 y
	Output		:		last x position
===================================================================*/
int Print4x5Text( char * Text , int x , int y , int color )
{
	uint8 num;
    RECT    src, dest;
	POINT	destp;
	uint8 r , g , b;
	BOOL ignore;

	if( (y + FontHeight ) >= d3dapp->szClient.cy )
		return PermX;

	if( bPolyText && PolyText[MyGameStatus])
	{
		r = Colourtrans[color][0];
		g = Colourtrans[color][1];
		b = Colourtrans[color][2];
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
			}
			else
			{
		
				src.top = TextSrcY[color][num];
				src.left = TextSrcX[color][num];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;
				dest.bottom = y + FontHeight;
				dest.top = y;
				dest.left = PermX;
				dest.right = PermX + FontWidth;

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
						}
						else
						{
							src.right -= (dest.right - d3dapp->szClient.cx);
							dest.right = d3dapp->szClient.cx;
						}
						ignore = FALSE;
					}
				}
				
				destp.x = dest.left;
				destp.y = dest.top;

				if( !ignore )
					FSBlit( lpFontSurface, FSBackBuffer, &src, &destp );
			}
		}
	
		PermX += FontWidth;
	}

	return PermX;
}


/*===================================================================
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, uint16 x , uint16 y
	Output		:		last x position
===================================================================*/
void PrintClipped4x5Text( char * Text , int x , int y , int col )
{
	uint8 num;
    RECT    src, dest;
	POINT	destp;
	uint8 r , g , b;
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
			}
			else
			{
		
				src.top = TextSrcY[col][num];
				src.left = TextSrcX[col][num];
				
				src.right = src.left+FontSourceWidth;
				src.bottom = src.top+FontSourceHeight;
				dest.bottom = y + FontHeight;
				dest.top = y;
				dest.left = PermX;
				dest.right = PermX + FontWidth;

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
 				
				destp.x = dest.left;
				destp.y = dest.top;

				if( !ignore )
					FSBlit( lpFontSurface, FSBackBuffer, &src, &destp );

			}
		}
	
		PermX += FontWidth;
	}

}
				
/*===================================================================
	Procedure	:		Print All Message in the normal Que (last 3 messages)...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void	MessageQuePrint( void )
{
	int i,e,y,z,MAX;
	char MessageBuff[80];

	// scale in-game text according to screenwidth
	// small::: less than 800 res (e.g. 640x480)
	if(d3dappi.szClient.cx < 800)
		MAX = 38;
	// medium::: less than 1024 res (e.g. 800x600)
	else if(d3dappi.szClient.cx < 1024)
		MAX = 59;
	// big::: 1024+ (e.g. 1024x786, 1280x1024)
	else
		MAX = 80;

	e = CurrentMessage + 1;
	e &= 3;
	MessageTime[e] = 0.0F;
	MessageBank[e][0] = 0;
	e = CurrentMessage -2;
	e &= 3;
	for( i = 2, y=0 ; i >-1 ; i-- )
	{
		MessageTime[e] -= framelag;
		if( MessageTime[e] < 0.0F && (MessageBank[(e-1)&3][0] == 0) )
		{
			MessageTime[e] = 0.0F;
			MessageBank[e][0] = 0;
		}
		else
		{
			
				// for all lines of this message
				for(z=0; z*MAX < MAXPERLINE; z++)
				{
					// that have contents 
					if(strcmp(&MessageBankLong[i][MAX*z],(const char*) "\0") != 0)
					{
						// display it to the screen
						strncpy(&MessageBuff[0],&MessageBankLong[i][MAX*z],MAX); 
						// centered
						CenterPrint4x5Text( &MessageBuff[0],((y*(FontHeight+1))+16) , MessageColourLong[i] );
						// left-aligned
						//Print4x5Text( &MessageBuff[0], FontWidth*20 , ((y*(FontHeight+1))+16) , MessageColourLong[i] );
						y++; // move down to next line on screen
					}
					else
						break;
				}	
			
			//CenterPrint4x5Text( &MessageBank[e][0] , (i*(FontHeight+1))+16 , MessageColour[e] );
		}
		e++;
		e &= 3;
	}
}

/*===================================================================
	Procedure	:		Print All Message in the long Que (last 50 messages)...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void	MessageQuePrintAll( void )
{
	int i,y,z,MAX;
	char MessageBuff[150];

	// scale in-game text according to screenwidth
	// small::: less than 800 res (e.g. 640x480)
	if(d3dappi.szClient.cx < 800)
		MAX = 55;
	// medium::: less than 1024 res (e.g. 800x600)
	else if(d3dappi.szClient.cx < 1024)
		MAX = 73;
	// big::: 1024+ (e.g. 1024x786, 1280x1024)
	else
		MAX = 100;

	// for all messages in history
	for (i=0, y=0; i < MAX_MESSAGES_LONG; i++)
	{
			// for all lines of this message
			for(z=0; z*MAX < MAXPERLINE; z++)
			{
				// that have contents 
				if(strcmp(&MessageBankLong[i][MAX*z],(const char*) "\0") != 0)
				{
					// display it to the screen
					strncpy(&MessageBuff[0],&MessageBankLong[i][MAX*z],MAX); 
					//CenterPrint4x5Text( &MessageBuff[0],((y*(FontHeight+1))+16) , MessageColourLong[i] );
					Print4x5Text( &MessageBuff[0], FontWidth*25 , ((y*(FontHeight+1))+16) , MessageColourLong[i] );
		
					y++; // move down to next line on screen
				}
				else
					break;
			}
	}

	
}

char TempMessage[512];
float MessageSize;
float ThisMessageTime;


/*===================================================================
	Procedure	:		Add Colour Message to the Que Short...
	Input		:		char * Text
	Output		:		nothing
===================================================================*/
void AddColourMessageToQueShort( char ** Text, int Colour )
{
	char * Pnt;
	int i,x,j;
	Pnt = *Text;

	CurrentMessage++;
	CurrentMessage&=3;
	MessageTime[CurrentMessage] = ThisMessageTime;
	MessageColour[CurrentMessage] = Colour; // Set colour

	/* save to longer buffer */
	// shunt long que up one
	for(x = MAX_MESSAGES_LONG-1; x > 0; x--)
	{
		// clean buffer
		for(j = 0; j < MAXPERLINE; j++)
			MessageBankLong[x][j] = 0;

		// move message up que by one
		strcpy((char *)MessageBankLong[x], (char *)MessageBankLong[x-1]);
		MessageColourLong[x] = MessageColourLong[x-1];
	}
	/* end of saving to longer buffer */

	// clean first line of buffer
	for(j = 0; j < MAXPERLINE; j++)
			MessageBankLong[0][j] = 0;

	// add message to buffers
	for( i = 0 ; i < MAXPERLINE ; i++ )
	{
		MessageBank[CurrentMessage][i] = *Pnt;

		// save current message to long que
		MessageBankLong[0][i] = *Pnt;
		MessageColourLong[0] = Colour;
		
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

void AddColourMessageToQue( int Colour, char * Text, ... )
{
	// -1 = do not display
	if(Colour > -1)
	{
		char * Pnt;
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
			AddColourMessageToQueShort( &Pnt, Colour );
		}
	}
}

//#define SFX_DEBUG
	
/*===================================================================
	Procedure	:		Print the Scores....
	Input		:		nothing
	Output		:		nothing
===================================================================*/
#define	FLASH_RATE	( 15.0F )
float	FlashSpeed = 0.0F;
BOOL	FlashToggle = FALSE;

// (stats.c)
extern int GetScoreStats(int Player);


extern BOOL ShowPlayersOnHUD;
void PrintScoreSort( void )
{
	BOOL teamOK[MAX_TEAMS];
	BOOL teamleft[MAX_TEAMS];
	int i,e, pos;
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

	// single player
	if( MyGameStatus == STATUS_SinglePlayer )
	{
		Print4x5Text( LIVES , 8 , FontHeight , 2 );
		Printuint16( (uint16) Lives , 8+(8*FontWidth) , FontHeight , 1 );
		return;
	}

	// flash players name
	FlashSpeed += framelag;
	if( FlashSpeed >= FLASH_RATE )
	{
		FlashToggle = !FlashToggle;
		FlashSpeed = FMOD( FlashSpeed, FLASH_RATE );
	}
	
	if( !TeamGame )
	{
		if( ShowPlayersOnHUD )
 		{
			int top_offset = FontHeight; // initial gap
			int line_height = FontHeight+1;

			// print player lines
			for( i = 0 ; i < MAX_PLAYERS ; i++ )
			{
				int left_offset = 0; // offset from left
				int len = 0; // length of string

				// make sure it's a valid player
				if( GameStatus[GetPlayerByRank(i)] != STATUS_Normal )
					continue;

				// print name
				if ( !( Ships[ GetPlayerByRank(i) ].Object.Flags & SHIP_CarryingBounty ) || FlashToggle )
				{
					// blue dot for bad ping
					if( GameStatus[GetPlayerByRank(i)] == STATUS_Normal )
						DisplayConnectionStatus( ReliabilityTab[GetPlayerByRank(i)], 2, top_offset );

					// give blue dot space
					left_offset = 8;

					// player name
					Print4x5Text( &Names[GetPlayerByRank(i)][0], left_offset, top_offset, (( WhoIAm == GetPlayerByRank(i) ) ? GRAY : RED) );
					left_offset += ( 8 * FontWidth ); // 8 = max characters in short player name
				}
				
				left_offset += ( 1 * FontWidth ); // give a padding space
				
				// print real score
				{
					int width = 0;
					width = Printint16( GetRealScore(GetPlayerByRank(i)), left_offset, top_offset, GRAY );// points + kills - suacides - friendly - deaths
					left_offset += (width * FontWidth);
				}

				left_offset += ( 1 * FontWidth ); // give a padding space

				// Show pings for everyone except your self
				if( GetPlayerByRank(i) != WhoIAm )
				{
					if( Ships[GetPlayerByRank(i)].network_player != NULL )
					{
						sprintf( (char*) &buf[0] ,"Ping %d", Ships[GetPlayerByRank(i)].network_player->ping );
						Print4x5Text( &buf[0] , left_offset, top_offset, ((GameStatus[i] == STATUS_Left) ? DARKGRAY : GREEN) );
					}
				}

				top_offset += line_height;
			}
		}
	}

	// team game
	else
	{
		for (i = 0; i < MAX_TEAMS; i++)
		{	teamOK[i] = FALSE;
			teamleft[i] = TRUE;
			TeamCol[i] = i + 1;
			TeamBounty[ i ] = 0;
			TeamBadConnection[ i ] = 0;
		}
		
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if ( (TeamNumber[i] < MAX_TEAMS) && (GameStatus[i] == STATUS_Normal ) )
			{
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
					Printint16( GetTeamScoreByTeamNumber(i) , 8+(7*FontWidth) , pos*(FontHeight+1)+FontHeight , col );
				}
				pos++;
			}
		}

		// show team info
		if( ShowTeamInfo )
		{
			e = pos + 1;
			for( i = 0 ; i < MAX_PLAYERS ; i++ )
			{
				if( ( TeamNumber[WhoIAm] == TeamNumber[i] ) && (GameStatus[i] == STATUS_Normal) )
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

		// update number of active players
		for( i = 0 ; i < MAX_PLAYERS ; i++ )
		{
			if ( (GameStatus[i] == STATUS_Normal ) )
				NumOfActivePlayers++;
		}
	}
}
		

/*===================================================================
	Procedure	:		Print a uint16 number to any surface..
	Input		:		uint16 num, uint16 x , uint16 y
	Output		:		nothing
===================================================================*/

void Printuint16AnySurface( uint16 tempnum , int x , int y , int col , DWORD flags, LPDIRECT3DSURFACE9 DestSurface )
{
	int i;
	int num;
    RECT    src;
	POINT	dest;
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
		dest.x = x;
		dest.y = y;
		FSBlit( lpFontSurface, DestSurface, &src, &dest );
		x += FontWidth;
	}
}

/*===================================================================
	Procedure	:		Re-Init the font surface...
	Input		:		nothing
	Output		:		nothing
===================================================================*/

void ReInitFont(void)
{
#if 0
	if( ModeCase < Mode512X384 )
	{
		DDReLoadBitmap( lpFontSurface , "data\\pictures\\font.bmp" );
		FontWidth = 4;
		FontHeight = 5;
		FontSourceWidth = 4;
		FontSourceHeight = 5;
	}else{
//		if( ModeCase < Mode640X480 )
//		{
			DDReLoadBitmap( lpFontSurface , "data\\pictures\\font512.bmp" );
			FontWidth = 8;
			FontHeight = 8;
			FontSourceWidth = 8;
			FontSourceHeight = 8;
//		}else{
//			DDReLoadBitmap( lpFontSurface , "data\\pictures\\font800.bmp" );
//			FontWidth = 9;
//			FontHeight = 16;
//			FontSourceWidth = 9;
//			FontSourceHeight = 16;
//		}
	}
#endif

	if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
	{
		lpFontSurface = FSLoadBitmap( "data\\pictures\\font512.bmp" );
		FontWidth = 8;
		FontHeight = 8;
		FontSourceWidth = 8;
		FontSourceHeight = 8;
	}else
	{
		lpFontSurface = FSLoadBitmap( "data\\pictures\\font.bmp" );
		FontWidth = 4;
		FontHeight = 5;
		FontSourceWidth = 4;
		FontSourceHeight = 5;
	}
}

/*===================================================================
	Procedure	:		Init PolyText allowed Array..
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void InitPolyText( void )
{
	int i;

	for( i = 0 ; i < 256 ; i++ )
	{
		PolyText[i] = FALSE;
	}

	PolyText[STATUS_Title]											= TRUE;
	PolyText[STATUS_BetweenLevels]								= TRUE;
	PolyText[STATUS_StartingMultiplayer]							= TRUE;			
	PolyText[STATUS_PlayingDemo]									= TRUE;
	PolyText[STATUS_Normal]											= TRUE;					
	PolyText[STATUS_SinglePlayer]									= TRUE;					
	//PolyText[STATUS_ViewingScore]								= TRUE;					
	PolyText[STATUS_WaitingToStartSinglePlayer]				= TRUE;					
	PolyText[STATUS_WaitingToStartMultiPlayerHost]			= TRUE;					
	PolyText[STATUS_WaitingToStartMultiPlayerClient]			= TRUE;					
	PolyText[STATUS_WaitingToStartTeamGame]				= TRUE;					
	PolyText[STATUS_WaitingToStartDemo]						= TRUE;
	PolyText[STATUS_WaitingToStartEndGameSequence]		= TRUE;
}


/*===================================================================
	Procedure	:		Read in the level specific text messages.......
	Input		:		char *Filename
	Output		:		BOOL TRUE/FALSE
===================================================================*/
BOOL ReadTxtFile( char *Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;

	if ( Debug )
	{
		int i;
		for( i = 0 ; i < MAXTEXTMESSAGES ; i ++ )
			TextMessagesPnt[i] = "\n";
	}


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
/*===================================================================
	Procedure	:		Free level specific text messages.......
	Input		:		void
	Output		:		void
===================================================================*/
void FreeTxtFile( void )
{
	if( TextMessages )
	{
		free( TextMessages );
		TextMessages = NULL;
	}
	NumOfTextMessages = 0;
}
/*===================================================================
	Procedure	:		Trigger text message.......
	Input		:		uint16 * Data
	Output		:		void
===================================================================*/
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
		AddColourMessageToQue( SystemMessageColour, "%s\n", TextMessagesPnt[*Data] );
		if( Tmi->ActivatedOrder == -1 )
		{
			Tmi->ActivatedOrder = TextActivatedOrder++;
		}
	}
}


/*===================================================================
	Procedure	:		Init the text messages......
	Input		:		void
	Output		:		void
===================================================================*/
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


void InitStatsMessages( void )
{

#ifdef SCROLLING_MESSAGES

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

#endif

}


/*===================================================================
	Procedure	:		Read in the level specific text message Info.......
	Input		:		char *Filename
	Output		:		BOOL TRUE/FALSE
===================================================================*/
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
	TextMessagesPnt[i] = FROM_YOUR_OWN_TEAM							   ;
	Tmi->ActivatedOrder = i++;
	Tmi++;
	TextMessagesPnt[i] = YOU											   ;
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
/*===================================================================
	Procedure	:		Free level specific text message info.......
	Input		:		void
	Output		:		void
===================================================================*/
void FreeMsgFile( void )
{
	if( TextMsgInfo )
	{
		free( TextMsgInfo );
		TextMsgInfo = NULL;
	}
}


/*===================================================================
	Procedure	:		Trigger text message.......
	Input		:		uint16 * Data
	Output		:		void
===================================================================*/
void DisplayOldTextMessage( void )
{
	TEXTMSGINFO * Tmi;
	int16	i;

	Tmi = TextMsgInfo;
	if( !Tmi )
		return;

	for( i = 0 ; i < ((Debug)?MAXTEXTMESSAGES:NumOfTextMessages) ; i++ )
	{
		if( Tmi->ActivatedOrder == CurrentTextActivated )
		{
			PlaySfx( SFX_MessageBeep, 1.0F );
			AddColourMessageToQue( SystemMessageColour, "%s\n", TextMessagesPnt[i] );
			return;
		}
		Tmi++;
	}
}

/*===================================================================
	Procedure	:		Show next previously Triggerd text message.......
	Input		:		void
	Output		:		void
===================================================================*/
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
/*===================================================================
	Procedure	:		Show Last previously Triggerd text message.......
	Input		:		void
	Output		:		void
===================================================================*/
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

/*===================================================================
	Procedure	:		Save all text...
	Input		:		FILE * fp
	Output		:		BOOL
===================================================================*/
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

/*===================================================================
	Procedure	:		Save all text...
	Input		:		FILE * fp
	Output		:		BOOL
===================================================================*/
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



/*===================================================================
	Procedure	:		Fill Status messages..
	Input		:		void
	Output		:		void
===================================================================*/
char * StatusTab[256];
void FillStatusTab( void )
{
	int i;
	for( i = 0 ; i < 256 ; i++ )
	{
		StatusTab[i] = "No Status Defined";
	}

    StatusTab[STATUS_Title                                ] = "Title        ";
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
    StatusTab[STATUS_QuittingFromInGame                   ] = "QuittingFromInGame         ";
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
/*===================================================================
	Procedure	:		Display Status messages..
	Input		:		void
	Output		:		void
===================================================================*/
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


/*===================================================================
	Procedure	:		Build Reliability Tab...
	Input		:		void
	Output		:		void
===================================================================*/
void BuildReliabilityTab( void )
{
	int i;
	int Temp;
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		ReliabilityTab[i] = 0;

		if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) && (i != WhoIAm) )
		{
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

/*===================================================================
	Procedure	:		Display a connection setting...
	Input		:		void
	Output		:		void
===================================================================*/
__inline
void DisplayConnectionStatus( int num , int x , int y)
{
    RECT src;
	POINT dest;
	int offset;

	if( !num )
		return;
	num--;

	if( bPolyText && PolyText[MyGameStatus])
	{
		AddScreenPolyText( (uint16) (83+num), (float) x , (float) y+2, 255, 255, 255, 255 );
	}
	else
	{

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

		dest.x = x;
		dest.y = y+offset;

		FSBlit( lpFontSurface, FSBackBuffer, &src, &dest );
	}
}


/*===================================================================
	Procedure	:		Init the font surface...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void InitFont( BOOL OverridePolytext )
{
//	LPDIRECTDRAWPALETTE ddpal;
	uint16 i;
	uint8 e;
	int x,y;
	int col;

	// if font already initialised...
	/*
	if (lpFontSurface && !OverridePolytext )
		return;
	if( lpFontSurface )
	{
		ReleaseDDSurf(lpFontSurface);
		lpFontSurface = NULL;
	}
	*/

#if 0
	if( ModeCase < Mode512X384 )
   	{
   		lpFontSurface = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font.bmp", 0, 0 );
   		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font.bmp");
		lpFontSurface->lpVtbl->SetPalette( lpFontSurface , ddpal );
   		DDSetColorKey( lpFontSurface, RGB_MAKE( 0 , 0 , 0 ) );
   		FontWidth = 4;
   		FontHeight = 5;
   		FontSourceWidth = 4;
   		FontSourceHeight = 5;

   	}
	else{
//   			if( ModeCase < Mode640X480 )
//   			{
   		
   			lpFontSurface = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font512.bmp", 0, 0 );

			ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font512.bmp");
			lpFontSurface->lpVtbl->SetPalette( lpFontSurface , ddpal );
   			DDSetColorKey( lpFontSurface, RGB_MAKE( 0 , 0 , 0 ) );
   			FontWidth = 8;
   			FontHeight = 8;
   			FontSourceWidth = 8;
   			FontSourceHeight = 8;
//   			}else{
//   				lpFontSurface = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font800.bmp", 0, 0 );
//   				ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font800.bmp");
//   				lpFontSurface->lpVtbl->SetPalette( lpFontSurface , ddpal );
//   				DDSetColorKey( lpFontSurface, RGB_MAKE( 255 , 0 , 255 ) );
//   				FontWidth = 9;
//   				FontHeight = 16;
//   				FontSourceWidth = 9;
//   				FontSourceHeight = 16;
//   			}
   	}
#endif
	/*
	if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
	{
		lpFontSurface = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font512.bmp", 0, 0 );

		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font512.bmp");
		if (lpFontSurface && ddpal)
		{
			LastError = lpFontSurface->lpVtbl->SetPalette( lpFontSurface , ddpal );
			DDSetColorKey( lpFontSurface, RGB_MAKE( 0 , 0 , 0 ) );
		}
		FontWidth = 8;
		FontHeight = 8;
		FontSourceWidth = 8;
		FontSourceHeight = 8;
	}else
	{
   		lpFontSurface = DDLoadBitmap( d3dapp->lpDD, "data\\pictures\\font.bmp", 0, 0 );
   		ddpal =  DDLoadPalette( d3dapp->lpDD , "data\\pictures\\font.bmp");
		if (lpFontSurface && ddpal)
		{
			LastError = lpFontSurface->lpVtbl->SetPalette( lpFontSurface , ddpal );
   			DDSetColorKey( lpFontSurface, RGB_MAKE( 0 , 0 , 0 ) );
		}
   		FontWidth = 4;
   		FontHeight = 5;
   		FontSourceWidth = 4;
   		FontSourceHeight = 5;
	}
	*/
	ReInitFont();

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
	}
	else
	{
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


/*===================================================================
	Procedure	:		Init the font surface...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
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
