/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 27 $
 *
 * $Header: /PcProjectX/Include/Text.h 27    4/21/98 10:01a Phillipd $
 *
 * $Log: /PcProjectX/Include/Text.h $
 * 
 * 27    4/21/98 10:01a Phillipd
 * 
 * 26    3/20/98 10:36a Phillipd
 * 
 * 25    3/11/98 4:09p Phillipd
 * 
 * 24    3/10/98 10:46a Phillipd
 * 
 * 23    7/03/98 19:10 Collinsd
 * Added new message for olly ( TeamGoals ) Also added pos and group to
 * explodesecondary.
 * 
 * 22    3/05/98 5:58p Phillipd
 * 
 * 21    3/05/98 11:52a Phillipd
 * 
 * 20    3/05/98 11:02a Phillipd
 * 
 * 19    1/02/98 11:13a Phillipd
 * 
 * 18    12/31/97 9:20a Phillipd
 * 
 * 17    12/03/97 5:25p Phillipd
 * 
 * 16    4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 15    10/08/97 2:06p Phillipd
 * 
 * 14    1-09-97 5:35p Philipy
 * changes made for 3 & 4 player team game
 * 
 * 13    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 12    20/12/96 15:19 Oliverc
 * Changed debug menu to be on SHIFT-F1
 * 
 * 11    12/10/96 2:06p Phillipd
 * 
 * 10    11/25/96 11:59a Phillipd
 * 
 * 9     11/13/96 9:08a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 8     26/10/96 18:33 Collinsd
 * Centered and moved up name tags.
 * 
 * 7     24/10/96 16:18 Collinsd
 * shortened text time.
 * 
 * 6     9/27/96 10:26a Phillipd
 * 
 * 5     9/18/96 5:35p Phillipd
 * 
 * 4     8/20/96 12:06p Phillipd
 * lots of new panel / text stuff...new stats array...
 * 
 * 3     8/19/96 8:51a Phillipd
 * 
 * 2     8/14/96 4:30p Phillipd
 * 
 * 1     8/14/96 9:30a Phillipd
 */
#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

/*
 * Defines
 */

#define	MAX_MESSAGES 4
#define	MAXMESSAGETIME 480.0F
//#define MAX_TEAMS 4
#ifdef DEBUG_ON
#define MAXTEXTMESSAGES (128+256)
#else
#define MAXTEXTMESSAGES 128
#endif
#define MAXPERLINE 62

#define MAX_STATS_MESSAGES 32
#define MAX_STATS_MESSAGE_VARIATIONS 16
#define MAX_STATS_MESSAGE_LENGTH 128
#define MAX_STATS_FORMATING_VARS 8

/*
 * Typedefs
 */
typedef struct
{
	BOOL valid;
	BOOL valid_this_time;
	uint16 num_variations;
	uint16 current_variation;
	char text[MAX_STATS_MESSAGE_VARIATIONS][MAX_STATS_MESSAGE_LENGTH];
	void *variable_ptr[MAX_STATS_MESSAGE_VARIATIONS][MAX_STATS_FORMATING_VARS];
} STATSMESSAGE;
typedef struct
{
	BOOL	Activated;
	int16	ActivatedOrder;
	int16	Type;
} TEXTMSGINFO;


/*
 * fn prototypes
 */

void PrintScoreSort( void );
void ScoreSort( void );
void CenterPrint4x5Text( char * Text , int y , int col );
void CenterPrint4x5Text2( char * Text , int x, int y, int col );
int	 Print4x5Text( char * Text , int x , int y , int col );
void PrintClipped4x5Text( char * Text , int x , int y , int col );
void Printuint16( uint16 tempnum , int x , int y , int col );
void MessageQuePrint( void );
void AddMessageToQue( char * Text, ... );
void RightJustifyPrint4x5Text( char * Text , int x , int y, int col );
void DoFontBlt(int sx , int sy , int sw , int sh , int x ,int y);
void Printuint16AnySurface( uint16 tempnum , int x , int y , int col , DWORD flags ,LPDIRECTDRAWSURFACE DestSurface );
void InitTextMessages( void );

void InitFont( BOOL OverridePolytext );
void ReInitFont( void );



BOOL ReadTxtFile( char *Filename );
void FreeTxtFile( void );
void TriggerTextMessage( uint16 * Data );
void InitStatsMessages( void );
BOOL ReadMsgFile( char *Filename );
void FreeMsgFile( void );

void NextTextMessage( void );
void LastTextMessage( void );

FILE * LoadAllText( FILE * fp );
FILE * SaveAllText( FILE * fp );


void FillStatusTab( void );
void DisplayStatusMessages( void );
void BuildReliabilityTab( void );
void DisplayConnectionStatus( int num , int x , int y);

#endif	// TEXT_INCLUDED


