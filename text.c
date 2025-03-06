
/*===================================================================
		Include File...	
===================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>

#include "new3d.h"

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "ships.h"
#include "main.h"
#include "title.h"
#include "text.h"
#include "screenpolys.h"

#include "local.h"
#include "stats.h"
#include "util.h"
#include "render.h"
#include "timer.h"

#define MSG_VERSION_NUMBER 1


/*===================================================================
		Externals ...
===================================================================*/

extern bool Debug;
extern bool	CanDoDamage[MAX_PLAYERS+1];
extern bool	PlayDemo;
extern px_timer_t LastPacketTime[MAX_PLAYERS+1];
extern bool	CTF;
extern bool CaptureTheFlag;
extern bool BountyHunt;
extern float	framelag;
extern float	real_framelag;
extern	BYTE	GameStatus[MAX_PLAYERS];	// Game Status for every Ship...

extern	bool	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	bool	ShowTeamInfo;
extern	MLOADHEADER Mloadheader;
extern	int16_t			Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16_t					Lives;
extern	int16_t					MaxKills;
extern	bool                    IsHost;
extern	int16_t			NumLevels;
extern	int				TeamFlag[ MAX_TEAMS ];

extern render_info_t render_info;

// color names are defined in text.h colors_t
u_int8_t Colourtrans[MAXFONTCOLOURS][3] = {
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
	{ 0,0,0 },		// black
	{ 255,255,255 }		// white
};

// (Title.c)
extern int SystemMessageColour;
extern int PlayerMessageColour;
extern int MyMessageColour;
extern SLIDER TextScaleSlider;
extern bool ShowPlayerHealthByScores;
// (stats.c)
extern int GetPlayerByRank(int Player);

// (networking.c)
extern SHIPHEALTHMSG PlayerHealths[ MAX_PLAYERS+1 ];
extern u_int8_t ShipHealthColour[ MAX_PLAYERS+1 ];

/*===================================================================
		Globals ...
===================================================================*/

int TextSrcX[MAXFONTCOLOURS][256];
int	TextSrcY[MAXFONTCOLOURS][256];

float MaxMessageTime = 5.0f;

int16_t	TextActivatedOrder = 0;
int16_t	CurrentTextActivated = 0;
int	TeamCol[MAX_TEAMS] = { 1, 2, 3, 4 };
char *TeamName[ MAX_TEAMS ] = // team colour names should match TeamCol[] colours (given in Colourtrans[][] below)
{
	LT_TEAM_1, // red
	LT_TEAM_2, // green
	LT_TEAM_3, // blue
	LT_TEAM_4  // yellow
};

int FontWidth = 8;
int FontHeight = 8;
int FontSourceWidth = 8;
int FontSourceHeight = 8;

int16_t	NumOfActivePlayers = 0;

int	PermX;
u_int16_t big[4] = { 10000 , 1000 , 100 , 10 };
u_int16_t little[4] = { 1000 , 100 , 10 , 1 };

// player text messages
float PlayerMessageTime[MAX_MESSAGES] = { 0.0F , 0.0F , 0.0F , 0.0F };
char PlayerMessageBank[MAX_MESSAGES][MAXPERLINE];
int PlayerMessageColours[MAX_MESSAGES];
// normal text messages short que
float MessageTime[MAX_MESSAGES] = { 0.0F , 0.0F , 0.0F , 0.0F };
int MessageColour[MAX_MESSAGES];
char MessageBank[MAX_MESSAGES][MAXPERLINE];
// all messages long que
char MessageBankLong[MAX_MESSAGES_LONG][MAXPERLINE];
int MessageColourLong[MAX_MESSAGES_LONG];

u_int8_t	CharTrans[256];

u_int16_t NumOfTextMessages = 0;
char * TextMessagesPnt[MAXTEXTMESSAGES];
char * TextMessages = NULL;
TEXTMSGINFO * TextMsgInfo = NULL;

char StatsMessageFile[] = "data\\txt\\statsmessages.txt";

STATSMESSAGE StatsMessages[MAX_STATS_MESSAGES];

/*===================================================================
	Procedure	:		Print a int16_t number in small 4x4 chars..
	Input		:		int16_t num, u_int16_t x , u_int16_t y
	Output		:		nothing
===================================================================*/

int Printint16_t( int16_t num , int x , int y , int color )
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
	Procedure	:		Print a u_int16_t number in small 4x4 chars..
	Input		:		u_int16_t num, u_int16_t x , u_int16_t y
	Output		:		nothing
===================================================================*/

void Printu_int16_t( u_int16_t tempnum , int x , int y , int col )
{
	int i;
	int num;
	int	Zeros= 0 ;
	u_int8_t r , g , b;

	if( (y + FontHeight ) >= render_info.window_size.cy )
		return;
	
	r = Colourtrans[col][0];
	g = Colourtrans[col][1];
	b = Colourtrans[col][2];

	for( i = 0 ; i < 4 ; i++ )
	{
		num = tempnum % big[i];
		num /= little[i];
		if( ( num + Zeros != 0 ) || ( i == 3 ) )
		{
			Zeros = 1;
			AddScreenPolyText( (u_int16_t) (num+1), (float) x , (float) y, r, g, b, 255 );
			x += FontWidth;
		}
	}
}

/*===================================================================
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, u_int16_t y
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

	x = (render_info.window_size.cx >> 1 ) - (i * ( FontWidth >> 1 )  ); // i *2 is half the size of the chars...
	
	Print4x5Text( Text , x , y , col );	
}

/*===================================================================
	Procedure	:		Print some Centered text at a specified Y
	Input		:		char * Text, u_int16_t y
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

int Print4x5TextSmall( char * Text , int x , int y , int color )
{
		int OldFontHeight = FontHeight;
		int OldFontWidth = FontWidth;
		int oldScale = TextScaleSlider.value;

		FontHeight = 8.0F;
		FontWidth = 8.0F;
		TextScaleSlider.value = 0;

		Print4x5Text( Text , x , y , color );

		FontHeight = OldFontHeight;
		FontWidth = OldFontWidth;
		TextScaleSlider.value = oldScale;
}

/*===================================================================
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, u_int16_t x , u_int16_t y
	Output		:		last x position
===================================================================*/
int Print4x5Text( char * Text , int x , int y , int color )
{
	u_int8_t num;
	u_int8_t r , g , b;

	if( (y + FontHeight ) >= render_info.window_size.cy )
		return PermX;

	r = Colourtrans[color][0];
	g = Colourtrans[color][1];
	b = Colourtrans[color][2];

	//printf("rgb = %d-%d-%d , text = %s\n",r,g,b,Text);

	if( x != -1 )
		PermX = x;
	while( ( num = *Text++ ) != 0 )
	{
		num = CharTrans[num];

		if ( num )
				AddScreenPolyText( (u_int16_t) num, (float) PermX , (float) y, r , g , b, 255 );
	
		PermX += FontWidth;
	}

	return PermX;
}


/*===================================================================
	Procedure	:		Print some text at a specified or the last pos
	Input		:		char * Text, u_int16_t x , u_int16_t y
	Output		:		last x position
===================================================================*/
void PrintClipped4x5Text( char * Text , int x , int y , int col )
{
	u_int8_t num;
	u_int8_t r , g , b;
	int dummy;


	if (x < 0)
		dummy = 0;

	r = Colourtrans[col][0];
	g = Colourtrans[col][1];
	b = Colourtrans[col][2];

	PermX = x;

	while( ( num = *Text++ ) != 0 )
	{
		num = CharTrans[num];

		if ( num )
			AddScreenPolyText( (u_int16_t) num, (float) PermX , (float) y, r , g , b, 255 );
	
		PermX += FontWidth;
	}

}
				
/*===================================================================
	Procedure	:		Print All Message in the player message Que (last 4 messages)...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void	PlayerMessageQuePrint( void )
{
	int i,y,z,MAX, ypos;
	char MessageBuff[150];

	MAX = floor((render_info.window_size.cx-FontWidth*27)/FontWidth);
	if(MAX < 10) MAX = 10;
	else if(MAX > 149) MAX = 149;

	ypos = render_info.window_size.cy- FontHeight;

	for( i=MAX_MESSAGES-1, y=0; i>-1; i-- )
	{
		PlayerMessageTime[i] -= (real_framelag/5.0F);
		if( PlayerMessageTime[i] <= 0.0F )
		{
			PlayerMessageTime[i] = 0.0F;
			PlayerMessageBank[i][0] = 0;
		}
		else
		{
				for(z=0; z*MAX < MAXPERLINE; z++)
				{
					if(strcmp(&PlayerMessageBank[i][MAX*z],(const char*) "\0") != 0)
						ypos-= FontHeight;
					else
						break;
				}	
		}
	}

	for( i=MAX_MESSAGES-1, y=0; i>-1; i-- )
	{
		if( PlayerMessageTime[i] > 0.0F )
		{
				for(z=0; z*MAX < MAXPERLINE; z++)
				{
					if(strcmp(&PlayerMessageBank[i][MAX*z],(const char*) "\0") != 0)
					{
						strncpy(&MessageBuff[0],&PlayerMessageBank[i][MAX*z],MAX); 
						
						Print4x5Text( &MessageBuff[0], FontWidth*12 , ypos+FontHeight*y , PlayerMessageColours[i] ); // left-aligned
						y++;
					}
					else
						break;
				}	
		}
	}
}
				
/*===================================================================
	Procedure	:		Print All Message in the normal Que (last 4 messages)...
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void	MessageQuePrint( void )
{
	int i,y,z,MAX;
	char MessageBuff[150];

	MAX = floor((render_info.window_size.cx-FontWidth*25)/FontWidth);
	if(MAX < 10) MAX = 10;
	else if(MAX > 149) MAX = 149;

	for( i=0, y=0; i<MAX_MESSAGES; i++ )
	{
		MessageTime[i] -= real_framelag;
		if( MessageTime[i] <= 0.0F )
		{
			MessageTime[i] = 0.0F;
			MessageBank[i][0] = 0;
		}
		else
		{
				for(z=0; z*MAX < MAXPERLINE; z++)
				{
					if(strcmp(&MessageBank[i][MAX*z],(const char*) "\0") != 0)
					{
						strncpy(&MessageBuff[0],&MessageBank[i][MAX*z],MAX); 
						CenterPrint4x5Text( &MessageBuff[0],((y*(FontHeight+1))+16) , MessageColour[i] );	// original centered
						y++;
					}
					else
						break;
				}	
		}
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

	MAX = floor((render_info.window_size.cx-FontWidth*25)/FontWidth);
	if(MAX < 10) MAX = 10;
	else if(MAX > 149) MAX = 149;

	for (i=0, y=0; i < MAX_MESSAGES_LONG; i++)
	{
			for(z=0; z*MAX < MAXPERLINE; z++)
			{
				if(strcmp(&MessageBankLong[i][MAX*z],(const char*) "\0") != 0)
				{
					strncpy(&MessageBuff[0],&MessageBankLong[i][MAX*z],MAX); 
					//CenterPrint4x5Text( &MessageBuff[0],((y*(FontHeight+1))+16) , MessageColourLong[i] );
					Print4x5Text( &MessageBuff[0], FontWidth*23 , ((y*(FontHeight+1))+16) , MessageColourLong[i] );
					y++;
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
	Procedure	:		Add Colour Player Message to the Que (text messages)...
	Input		:		char * Text
	Output		:		nothing
===================================================================*/

void AddPlayerMessageToQueShort( char ** Text, int Colour )
{
	char * Pnt;
	int i,x,j;
	Pnt = *Text;

	for(x = MAX_MESSAGES-1; x > 0; x--)
	{
		for(j = 0; j < MAXPERLINE; j++)
			PlayerMessageBank[x][j] = 0;
		strcpy((char *)PlayerMessageBank[x], (char *)PlayerMessageBank[x-1]);
		PlayerMessageColours[x] = PlayerMessageColours[x-1];
		PlayerMessageTime[x] = PlayerMessageTime[x-1];
		PlayerMessageColours[x] = PlayerMessageColours[x-1];
	}

	for(j = 0; j < MAXPERLINE; j++)
			PlayerMessageBank[0][j] = 0;
	PlayerMessageTime[0] = ThisMessageTime;
	PlayerMessageColours[0] = Colour;
	MessageColourLong[0] = Colour;

	for( i = 0 ; i < MAXPERLINE ; i++ )
	{
		PlayerMessageBank[0][i] = *Pnt;
		MessageBankLong[0][i] = *Pnt;

		if( *Pnt == 0 )
		{
			*Text = NULL;
			return;
		}
		Pnt++;
	}
}

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

	for(x = MAX_MESSAGES-1; x > 0; x--)
	{
		for(j = 0; j < MAXPERLINE; j++)
			MessageBank[x][j] = 0;
		strcpy((char *)MessageBank[x], (char *)MessageBank[x-1]);
		MessageColour[x] = MessageColour[x-1];
		MessageTime[x] = MessageTime[x-1];
	}

	for(j = 0; j < MAXPERLINE; j++)
			MessageBank[0][j] = 0;
	MessageTime[0] = ThisMessageTime;
	MessageColour[0] = Colour;
	MessageColourLong[0] = Colour;

	for( i = 0 ; i < MAXPERLINE ; i++ )
	{
		MessageBank[0][i] = *Pnt;
		MessageBankLong[0][i] = *Pnt;

		if( *Pnt == 0 )
		{
			*Text = NULL;
			return;
		}
		Pnt++;
	}
}


void AddPlayerMessageToQue( int Colour, char * Text, ... )
{
	int x, j;

	// -1 = do not display
	if(Colour > -1)
	{
		char * Pnt;
		va_list args;		

		for(x = MAX_MESSAGES_LONG-1; x > 0; x--)
		{
			for(j = 0; j < MAXPERLINE; j++)
				MessageBankLong[x][j] = 0;

			strcpy((char *)MessageBankLong[x], (char *)MessageBankLong[x-1]);
			MessageColourLong[x] = MessageColourLong[x-1];
		}

		for(j = 0; j < MAXPERLINE; j++)
				MessageBankLong[0][j] = 0;

		va_start( args, Text );
		vsnprintf( &TempMessage[0], sizeof(TempMessage)-1, Text, args);
		va_end( args );

		TempMessage[sizeof(TempMessage)-1] = 0;

		Pnt = &TempMessage[0];

		MessageSize = (float) strlen(&TempMessage[0]);
		ThisMessageTime = MaxMessageTime * (MessageSize / MAXPERLINE);
		if( ThisMessageTime < MaxMessageTime )
			ThisMessageTime = MaxMessageTime;

		while( Pnt )
			AddPlayerMessageToQueShort( &Pnt, Colour );
	}
}

void AddColourMessageToQue( int Colour, char * Text, ... )
{
	int x, j;

	// -1 = do not display
	if(Colour > -1)
	{
		char * Pnt;
		va_list args;		

		for(x = MAX_MESSAGES_LONG-1; x > 0; x--)
		{
			for(j = 0; j < MAXPERLINE; j++)
				MessageBankLong[x][j] = 0;

			strcpy((char *)MessageBankLong[x], (char *)MessageBankLong[x-1]);
			MessageColourLong[x] = MessageColourLong[x-1];
		}

		for(j = 0; j < MAXPERLINE; j++)
				MessageBankLong[0][j] = 0;

		va_start( args, Text );
		vsnprintf( &TempMessage[0], sizeof(TempMessage)-1, Text, args);
		va_end( args );

		TempMessage[sizeof(TempMessage)-1] = 0;

		Pnt = &TempMessage[0];

		MessageSize = (float) strlen(&TempMessage[0]);
		ThisMessageTime = MaxMessageTime * (MessageSize / MAXPERLINE);
		if( ThisMessageTime < MaxMessageTime )
			ThisMessageTime = MaxMessageTime;

		while( Pnt )
			AddColourMessageToQueShort( &Pnt, Colour );
	}
}
	
/*===================================================================
	Procedure	:		Print the Scores....
	Input		:		nothing
	Output		:		nothing
===================================================================*/
#define	FLASH_RATE	( 15.0F )
float	FlashSpeed = 0.0F;
bool	FlashToggle = false;

// (stats.c)
extern int GetScoreStats(int Player);


extern bool ShowPlayersOnHUD;
extern bool ShowPlayersOnHUDbyKills;
extern BYTE MyGameStatus;
void PrintScoreSort( void )
{
	bool teamOK[MAX_TEAMS];
	bool teamleft[MAX_TEAMS];
	int i,e, pos;
	int16_t TeamBounty[MAX_TEAMS];
	int TeamBadConnection[MAX_TEAMS];
	char buf[256];
	int col;
	u_int16_t scorewidth = 1;
	u_int16_t tempwidth = 0;
	BYTE ShipID;

	NumOfActivePlayers = 0;

	// single player
	if( MyGameStatus == STATUS_SinglePlayer )
	{
		Print4x5Text( LIVES , 8 , FontHeight , 2 );
		Printu_int16_t( (u_int16_t) Lives , 8+(8*FontWidth) , FontHeight , 1 );
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

			// calculate the width of the score column
			if(ShowPlayersOnHUDbyKills)
			{
				for(i=0; i<MAX_PLAYERS; i++)
				{
					tempwidth = (u_int16_t) log10(abs(GetKills(i)))+1;
					if(GetKills(i) < 0)	tempwidth++;
					if(tempwidth > scorewidth) scorewidth = tempwidth;			
				}
			}
			else
			{
				for(i=0; i<MAX_PLAYERS; i++)
				{
					tempwidth = (u_int16_t) log10(abs(GetRealScore(i)))+1;
					if(GetRealScore(i) < 0)	tempwidth++;
					if(tempwidth > scorewidth) scorewidth = tempwidth;
				}
			}
			scorewidth *= FontWidth;

			// print player lines
			for( i = 0 ; i < MAX_PLAYERS ; i++ )
			{
				int left_offset = 0; // offset from left

				// get player id
				ShipID = GetPlayerByRank(i);

				// make sure it's a valid player
				if( GameStatus[ShipID] != STATUS_Normal )
					continue;

				// blue dot for bad ping
				DisplayConnectionStatus( ShipID, 2, top_offset );

				// give packet loss flag a space
				left_offset = FontWidth;

				// print name
				if ( !( Ships[ShipID].Object.Flags & SHIP_CarryingBounty ) || FlashToggle )
					Print4x5Text( &Names[ShipID][0], left_offset, top_offset, (( WhoIAm == ShipID ) ? WHITE : RED) );
				else
					Print4x5Text( &Names[ShipID][0], left_offset, top_offset, WHITE );

				left_offset += ( 8 * FontWidth );
				
				// print real score
				if(ShowPlayersOnHUDbyKills)
					// kills - suicides - friendly
					Printint16_t( GetKills(ShipID), left_offset, top_offset, GRAY ); 
				else
					// points + kills - suicides - friendly - deaths
					Printint16_t( GetRealScore(ShipID), left_offset, top_offset, GRAY );

				left_offset += scorewidth+8.0F; // give a padding space

				// Show pings for everyone except yourself
				if( ShipID != WhoIAm )
				{
					if( Ships[ShipID].network_player != NULL )
					{
						// Show ping
						sprintf( (char*) &buf[0] ,"%*dms", 4, (u_int16_t) Ships[ShipID].network_player->ping);
						Print4x5TextSmall( &buf[0], left_offset, top_offset+((FontHeight-8.0F)/2.0F), ((GameStatus[ShipID] == STATUS_Left) ? DARKGRAY : GREEN) );

                        // Show % Health
                        if(ShowPlayerHealthByScores && GameStatus[ShipID] != STATUS_Left)
                        {
							sprintf( (char*) &buf[0], "H:%d", (u_int16_t) ceil(((PlayerHealths[ShipID].Shield + PlayerHealths[ShipID].Hull) /2.56F)));
							Print4x5TextSmall( &buf[0], left_offset+60.0F, top_offset+((FontHeight-8.0F)/2.0F), ShipHealthColour[ShipID] );
                        }

				/*		sprintf( (char*) &buf[0] ,"IP: %s PING: %d LOSS: %d LOST: %d BW IN: %d BW OUT: %d PORT: %d", 
													Ships[ShipID].network_player->ip,
													Ships[ShipID].network_player->ping,
													Ships[ShipID].network_player->packet_loss,
													Ships[ShipID].network_player->packets_lost,
													Ships[ShipID].network_player->bw_in,
													Ships[ShipID].network_player->bw_out,
													Ships[ShipID].network_player->port);

						Print4x5TextSmall( &buf[0] , left_offset, top_offset+((FontHeight-8.0F)/2.0F), GREEN );*/

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
		{	teamOK[i] = false;
			teamleft[i] = true;
			TeamCol[i] = i + 1;
			TeamBounty[ i ] = 0;
		}
		
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if ( (TeamNumber[i] < MAX_TEAMS) && (GameStatus[i] == STATUS_Normal ) )
			{
				teamOK[TeamNumber[i]] = true;
				switch ( GameStatus[i] )
				{
					case STATUS_Left:
					case STATUS_LeftCrashed:
						break;
					default:
						teamleft[TeamNumber[i]] = false;
						if ( Ships[ i ].Object.Flags & SHIP_CarryingBounty )
							TeamBounty[ TeamNumber[ i ] ]++;
						break;
					}
			}
		}

		pos = 0;
		buf[ 0 ] = 0;

		for (i = 0; i < MAX_TEAMS; i++)
		{
			if (teamOK[i])
			{
				bool show;

				show = false;
				if ( CaptureTheFlag || CTF )
				{
					if ( !TeamFlag[ i ] || FlashToggle )
						show = true;
				}
				else if ( BountyHunt )
				{
					if ( !TeamBounty[ i ] || FlashToggle )
						show = true;
				}
				else
					show = true;
				if ( show )
				{
					col = ( teamleft[i] ) ? 8 :
						  ( TeamNumber[ WhoIAm ] == i && FlashToggle ) ? 0 : TeamCol[i] ;
#ifdef TEAM_NUMBERS
					Print4x5Text( TEAM, 8 , pos*(FontHeight+1)+FontHeight , col );
					Printu_int16_t( (u_int16_t)(i + 1) , 8+(5*FontWidth) , pos*(FontHeight+1)+FontHeight , col );
#else
					Print4x5Text( TeamName[ i ] , 8 , pos*(FontHeight+1)+FontHeight , col );
#endif
					Printint16_t( GetTeamScoreByTeamNumber(i) , 8+(7*FontWidth) , pos*(FontHeight+1)+FontHeight , col );
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
				if( ( TeamNumber[WhoIAm] == TeamNumber[i] || SwitchedToWatchMode) 
						&& (GameStatus[i] == STATUS_Normal) && (Ships[i].Object.Mode != LIMBO_MODE) )
				{
					col = ( WhoIAm == i ) ? 0 : TeamCol[ TeamNumber[ i ] ];
					Print4x5Text( &Names[i][0] , FontWidth , e*(FontHeight+1)+(FontHeight*4) , col );

					Print4x5TextSmall( &Mloadheader.Group[Ships[i].Object.Group].name[0] , 8+(FontWidth*8), e*(FontHeight+1)+(FontHeight*4)+((FontHeight-8.0F)/2.0F), 2 );

					DisplayConnectionStatus( i , 2 , e*(FontHeight+1)+(FontHeight*4) );
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
	Procedure	:		Read in the level specific text messages.......
	Input		:		char *Filename
	Output		:		bool true/false
===================================================================*/
bool ReadTxtFile( char *Filename )
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
		return true;
	}
	Buffer = calloc( File_Size+1 , 1 );
	if( !Buffer )
	{
		Msg( "ReadTxtFile : Unable to allocate file buffer\n", Filename );
		return( false );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "ReadTxtFile Load Error reading %s\n", Filename );
		return( false );
	}
	TextMessages = Buffer;


	TextMessagesPnt[0] = strtok( TextMessages , "\n" );

	NumOfTextMessages = 0;

	while( TextMessagesPnt[NumOfTextMessages] )
	{
//		DebugPrintf( "message %d = %s\n", NumOfTextMessages , TextMessagesPnt[NumOfTextMessages]);

		TextMessagesPnt[++NumOfTextMessages] = strtok( NULL , "\n" );
	}
	return true;
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
	Input		:		u_int16_t * Data
	Output		:		void
===================================================================*/
void TriggerTextMessage( u_int16_t * Data )
{
	TEXTMSGINFO * Tmi;
	bool	DoneBefore = false;

	Tmi = TextMsgInfo;
	if( Tmi )
	{
		Tmi += *Data;
		if( Tmi->Type )
		{
			DoneBefore = Tmi->Activated;
			Tmi->Activated = true;
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
	for( i = 0 ; i < MAX_MESSAGES ; i++ )
	{
		MessageTime[i] = 0.0F;
		MessageBank[i][0] = 0;
		PlayerMessageTime[i] = 0.F;
		PlayerMessageBank[i][0] = 0;
		MessageBankLong[i][0] = 0;
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
	bool valid;
	bool valid_this_time;
	u_int16_t num_variations;
	u_int16_t current_variation;
	char text[MAX_STATS_MESSAGE_VARIATIONS][MAX_STATS_MESSAGE_LENGTH];
	void *variable_ptr[MAX_STATS_MESSAGE_VARIATIONS];
} STATSMESSAGE;
*/

	FILE *f;
	char token[80];
	int j, k;

	// init all messages

	f = file_open( StatsMessagesFile, "r" );
	if ( !f )
		return;

	while ( fscanf( f, " %80s", token ) == 1)   
	{
		if ( !strcasecmp( token, "NewMessage" ) )
		{
			rv = GetMessageName();
			if (!rv)
				continue;
			if (rv == -1)


			if (!GetMessageString())
				continue;

			if (!GetMessageVars())
				continue;
		}
	}
		
		if ( !strcasecmp( token, "NewMessage" ) )
		{
			// read in message name
			if ( fscanf( f, " %80s", token ) != 1 )
				break;
			for ( j = 0; MessageNumLookup[ j ].keyword; j++ )
			{
				if ( !strcasecmp( token, MessageNumLookup[ j ].keyword ) )
					break;
			}
			if (!MessageNumLookup[ j ].keyword)		// not a valid message token
				continue;

			MessageNum = MessageNumLookup[ j ].num;

			// read in next line, store in StatsMessages
			fgets( tempstr, MAX_STATS_MESSAGE_LENGTH, f );
			
			if ( !strcasecmp( token, "EndMessage") )
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
			IgnoreMessage = false;
			while ( _strcmp( token, "EndVars" ) )
			{
	 			for ( j = 0; MessageVarLookup[ j ].keyword; j++ )
				{
					if ( !strcasecmp( token, MessageVarLookup[ j ].keyword ) )
						break;
				}
				if (!MessageVarLookup[ j ].keyword)		// not a valid message token
					continue;

				StatsMessages[MessageNum].variable_ptr[num_variations][ k++ ] = MessageVarLookup[ j ].var_ptr;
				if ( fscanf( f, " %80s", token ) != 1 )
				{
				 	IgnoreMessage = true;
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
	Output		:		bool true/false
===================================================================*/
bool ReadMsgFile( char *Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int16_t		*	int16Pnt;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	u_int32_t		* u_int32Pnt;
	TEXTMSGINFO * Tmi;
	int i;

	TextActivatedOrder = 0;
	CurrentTextActivated = 0;
	TextMsgInfo = NULL;
	if( !NumOfTextMessages)
		return true;
	
	File_Size = Get_File_Size( Filename );
	if( !File_Size )
	{
		// dont need water...
		return true;
	}
	Buffer = calloc( File_Size+1 , 1 );
	if( !Buffer )
	{
		Msg( "ReadMsgFile : Unable to allocate file buffer\n", Filename );
		return( false );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "ReadMsgFile Load Error reading %s\n", Filename );
		return( false );
	}
	OrgBuffer = Buffer;

	u_int32Pnt = (u_int32_t*) Buffer;
	MagicNumber = *u_int32Pnt++;
	VersionNumber = *u_int32Pnt++;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != MSG_VERSION_NUMBER  ) )
	{
		Msg( "ReadMsgFile() Incompatible msg file %s", Filename );
		free( OrgBuffer );
		return( false );
	}

	int16Pnt = (int16_t*) u_int32Pnt;
	if( *int16Pnt++ != NumOfTextMessages )
	{
		Msg( "ReadMsgFile : .Msg not compatible with .Txt\n" );
		return( false );
	}
#ifdef DEBUG_TEXT_MESSAGES
	TextMsgInfo = (TEXTMSGINFO*) malloc( ( NumOfTextMessages + 1024 ) * sizeof(TEXTMSGINFO) );
#else
	TextMsgInfo = (TEXTMSGINFO*) malloc( NumOfTextMessages * sizeof(TEXTMSGINFO) );
#endif
	if( !TextMsgInfo )
	{
		Msg( "ReadMsgFile : Unable to allocate memory buffer\n", Filename );
		return( false );
	}
	Tmi = TextMsgInfo;

	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		Tmi->Activated = false;
		Tmi->Type = *int16Pnt++;
		Tmi->ActivatedOrder = -1;
		Tmi++;
	}
#ifdef DEBUG_TEXT_MESSAGES

	
	Tmi = TextMsgInfo;
	for( i = 0 ; i < NumOfTextMessages ; i++ )
	{
		Tmi->Activated = true;
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
	return true;
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
	Input		:		u_int16_t * Data
	Output		:		void
===================================================================*/
void DisplayOldTextMessage( void )
{
	TEXTMSGINFO * Tmi;
	int16_t	i;

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
	Output		:		bool
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
	Output		:		bool
===================================================================*/
FILE *	LoadAllText( FILE * fp )
{
	TEXTMSGINFO * Tmi;
	int i;
	u_int16_t TempNumOfTextMessages = 0;


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
	Procedure	:		Display a connection setting...
	Input		:		void
	Output		:		void
===================================================================*/
void DisplayConnectionStatus( int num , int x , int y)
{
    u_int16_t dot;

	// protect against bad input
	if( num < 0 || num > MAX_PLAYERS-1 || num == WhoIAm || Ships[num].network_player == NULL )
		return;

	// calculate which colour flag to use
    if(Ships[num].network_player->packets_lost == 0)
        dot = 84; // green
    else if(Ships[num].network_player->packets_lost == 1)
        dot = 83; // blue
    else
        dot = 85; // red
	
	// display
    AddScreenPolyText( dot, (float) x , (float) y+2, 255, 255, 255, 255 );
}


// maps ascii character codes to location on texture
void InitFont( void )
{
	u_int16_t i;
	u_int8_t e;

	ZERO_STACK_MEM(CharTrans);

	for( i = 0x30, e = 1 ; i < 0x3a ; i++ , e++ ) CharTrans[i] = e; // 0 - 9
	for( i = 0x41, e = 11; i < 0x5b ; i++ , e++ ) CharTrans[i] = e; // A - Z
	for( i = 0x61, e = 11; i < 0x7b ; i++ , e++ ) CharTrans[i] = e; // a - z

	CharTrans[(u_int8_t)'.'] = 37;

	e = 73;
	CharTrans[(u_int8_t)','] = e++;
	CharTrans[(u_int8_t)';'] = e++;
	CharTrans[(u_int8_t)'['] = CharTrans[(u_int8_t)'('] = e++; // render same
	CharTrans[(u_int8_t)']'] = CharTrans[(u_int8_t)')'] = e++; // render same
	CharTrans[(u_int8_t)'*'] = e++;
	CharTrans[(u_int8_t)'/'] = e++;
	CharTrans[(u_int8_t)'+'] = e++;
	CharTrans[(u_int8_t)'-'] = e++;
	CharTrans[(u_int8_t)'='] = e++;
	CharTrans[(u_int8_t)'\''] = e++;

	e = 86;
	CharTrans[(u_int8_t)'?'] = e++;
	CharTrans[(u_int8_t)'!'] = e++;
	CharTrans[(u_int8_t)'"'] = e++;
	CharTrans[(u_int8_t)'#'] = e++;
	CharTrans[(u_int8_t)'$'] = e++;
	CharTrans[(u_int8_t)'%'] = e++;
	CharTrans[(u_int8_t)'&'] = e++;
	CharTrans[(u_int8_t)':'] = e++;
	CharTrans[(u_int8_t)'@'] = e++;
	CharTrans[(u_int8_t)'\\'] = e++;
	CharTrans[(u_int8_t)'<'] = e++;
	CharTrans[(u_int8_t)'>'] = e++;
	CharTrans[(u_int8_t)'{'] = e++;
	CharTrans[(u_int8_t)'}'] = e++;
	CharTrans[(u_int8_t)'^'] = e++;
	CharTrans[(u_int8_t)'_'] = e++;
	CharTrans[(u_int8_t)'`'] = e++;
	CharTrans[(u_int8_t)'~'] = e++;
}
