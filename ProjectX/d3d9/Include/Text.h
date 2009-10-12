
#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

/*
 * Defines
 */

#define	MAX_MESSAGES 4
#define  MAX_MESSAGES_LONG 100
#define	MAXMESSAGETIME 480.0F
//#define MAX_TEAMS 4
#ifdef DEBUG_ON
#define MAXTEXTMESSAGES (128+256)
#else
#define MAXTEXTMESSAGES 128
#endif
#define MAXPERLINE 512 // message buffer width - line width set in (Text.c) MessageQuePrint/All

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
void CenterPrint4x5Text( char * Text , int y , int color );
void CenterPrint4x5Text2( char * Text , int x, int y, int color );
int	 Print4x5Text( char * Text , int x , int y , int color );
void PrintClipped4x5Text( char * Text , int x , int y , int color );
void Printuint16( uint16 tempnum , int x , int y , int color );
void MessageQuePrint( void );
void AddColourMessageToQue(  int Colour, char * Text, ... );
void RightJustifyPrint4x5Text( char * Text , int x , int y, int color );
void InitTextMessages( void );

void InitFont( void );
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



// if you add a color then up this count...
#define	MAXFONTCOLOURS	11

// must align this enum with rgb values in Colourtrans (text.c)
typedef enum {
 GRAY,
 RED,
 GREEN,
 BLUE,
 YELLOW,
 CYAN,
 PURPLE,
 OFFGREEN,
 DARKGRAY,
 BLACK,
 WHITE
} colors_t;



#endif	// TEXT_INCLUDED


