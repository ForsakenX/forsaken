#ifndef DPTHREAD_H
#define DPTHREAD_H

#define MASTER_PORT 22999
#define DEF_PORT 27990

#define HEARTBEAT_MsgSeperator		'/'
#define HEARTBEAT_FieldSeperator	'~'

#define HEARTBEAT_Raw			( 1 << 0 )
#define HEARTBEAT_BasicInfo		( 1 << 1 )	// session name(text), creation date / time( dos 64 bit ), level name(text), server game(byte), num players(word), max players(word)
#define HEARTBEAT_GameType		( 1 << 2 )	// game type(byte), CTF sub type(byte), harm team mates(byte)
#define HEARTBEAT_PacketInfo	( 1 << 3 )	// packet info: short(byte), grouped(byte), rate(word)
#define HEARTBEAT_KillsInfo		( 1 << 4 )	// max kills(word), current top(word)
#define HEARTBEAT_TimeInfo		( 1 << 5 )	// time limit(word), current elapsed time(word)
#define HEARTBEAT_PlayersList	( 1 << 6 )	// player(text), score(word), ping( 3 bytes ) 	( for all players )
#define HEARTBEAT_ValidPickups	( 1 << 7 )	// 64 bit packed
#define HEARTBEAT_ExtraInfo		( 1 << 8 )	// lag tolerance, bright bikes, bike exhausts, randomise pickups ( packed byte )
#define HEARTBEAT_GAMESPY_Pulse ( 1 << 9 )
#define HEARTBEAT_GAMESPY_Basic ( 1 << 10 )
#define HEARTBEAT_GAMESPY_Info	( 1 << 11 )
#define HEARTBEAT_GAMESPY_Rules ( 1 << 12 )
#define HEARTBEAT_GAMESPY_Players ( 1 << 13 )
#define HEARTBEAT_GAMESPY_Status ( 1 << 14 )
#define HEARTBEAT_GAMESPY_Echo ( 1 << 15 )
#define HEARTBEAT_GAMESPY_Secure ( 1 << 16 )

extern uint16 PlayerPort[];
extern u_short host_port;
extern struct sockaddr_in PlayerIP[ MAX_PLAYERS ];
extern uint32 tracker_addr;

extern char tracker_ip[];
extern u_short tracker_port;
extern DWORD	heartbeat_freq;
extern int	heartbeat_type;
extern BOOL SendShutdownPacket;
extern BOOL bGameSpy;
extern BOOL TrackerOveride;

extern BOOL WSA_Active;

void DPStopThread(void);
BOOL DPStartThread(void);
BOOL DPStartThreadListenOnly( void );
BOOL read_heartbeat_info( char *file );
void UpdatePlayerInfo( void );
BOOL StringFromGUID(LPCGUID lpguid, LPSTR lpsz);
void PostGamespyServerInfo( struct sockaddr_in *addr );
void InitTrackersList( MENU *menu );
void ExitSetUpTracker( MENU *menu );
BOOL GetTrackerFiles( void );


#endif	// DPTHREAD_H
