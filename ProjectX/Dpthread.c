
// This is the only file that included gsutils.h

// #define INITGUID
#include <windows.h>
#include <dplay.h>
#include <dplobby.h>
#include <stdio.h>
#include <time.h>

#include <stdarg.h>
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
#include "pickups.h"
#include "comm.h"
#include "dpthread.h"
#include "Local.h"
#include "primary.h"

//#define	DPTEST
extern	LPDIRECTPLAYLOBBY2A					lpDPlayLobby;		//Lobby stuff...
extern LPDIRECTPLAY3 glpDP;
extern	BYTE	WhoIAm;
extern	BYTE	GameStatus[];
extern	char    IPAddressText[16];
extern	LPDPSESSIONDESC2	glpdpSD;
extern	SHORTNAMETYPE	Names;	// all the players short Names....
extern	BOOL	IsHost;
extern	GLOBALSHIP		Ships[];
extern GUID	ServiceProvidersGuids[];
extern	LIST	ServiceProvidersList;
extern uint16 PingTimes[MAX_PLAYERS];
extern BYTE	TeamNumber[];

extern BOOL NoMenuBack;

// (stats.c)
extern int GetScoreStats(int Player); // get individual player's score
// custom message colours (Title.c)
extern int SystemMessageColour;

// TEMP?
extern char MissionTextNames[MAXLEVELS][128];
extern LIST	LevelList;
extern	SLIDER  PacketsSlider;

void DebugPrintf( const char * format, ... );
extern void SetMultiplayerPrefs( void );

//
// modify this struction with what ever you want.
// just make sure you modify the sprintf in ProcessForsakenInfo
// to print the values
//
typedef struct playerlist_tag {
	char	PlayerName[ 8 ];
	int		CurScore;
	DWORD	Ping;
	int16	Team;
} playerlist_t;

playerlist_t PlayerInfo[ MAX_PLAYERS ];
BOOL SendShutdownPacket;

uint16 PlayerPort[ MAX_PLAYERS ];
BOOL ServerHeartbeat = FALSE;
BOOL PeerPeerHeartbeat = FALSE;
BOOL ForceHeartbeat = FALSE;
BOOL DoHeartbeat = FALSE;

BOOL TrackerOveride = FALSE;

char heartbeat_location[ 32 ];
char heartbeat_servername[ 32 ];
char heartbeat_serverspec[ 32 ];
char heartbeat_serverconnection[ 32 ];
char heartbeat_serveradmin[ 32 ];

CRITICAL_SECTION PlayerInfoKey;

BOOL WinsockActive = FALSE;
BOOL WSA_Active = FALSE;
SOCKET sockfd;

HANDLE	DPSendThreadEvent = NULL;
HANDLE	DPSendThreadKillEvent = NULL;
HANDLE	DPListenThreadEvent = NULL;
HANDLE	DPListenThreadKillEvent = NULL;

HANDLE	ghDPSendThread = NULL;
HANDLE	ghDPListenThread = NULL;

char *udp_header = "\377\377\377\377";
char *UDP_MSG_Ping = "ping\n";
char *UDP_MSG_BouncedPing = "gnip\n";
char *UDP_MSG_Status = "status\n";
char *UDP_MSG_Forsaken = "forsaken\n";

#define THIS_GAMENAME "Forsaken"

DWORD WINAPI DPThread(LPVOID p);
BOOL LoadDP(void);
void PostServerInfo( struct sockaddr_in *addr );
char pBuf[4096];
char *srv_info;
HANDLE SendMutex;

struct sockaddr_in HostIP, TrackerIP;
struct sockaddr_in PlayerIP[ MAX_PLAYERS ];

u_short host_port;
u_short tracker_port;

char host_ip[ 16 ];
char tracker_ip[ 16 ];
uint32 tracker_addr = 0;
char tracker_name[ 128 ];

DWORD	heartbeat_freq;
int	heartbeat_type;

static const BYTE GuidMap[] = { 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 
                                8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
static const char szDigits[] = "0123456789ABCDEF"; 

BOOL StringFromGUID(LPCGUID lpguid, LPSTR lpsz) 
{ 
    int i; 
 
    const BYTE * pBytes = (const BYTE *) lpguid; 
 
    *lpsz++ = '{'; 
 
    for (i = 0; i < sizeof(GuidMap); i++) 
    { 
        if (GuidMap[i] == '-') 
        { 
            *lpsz++ = '-'; 
        } 
        else 
        { 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0xF0) >> 4 ]; 
            *lpsz++ = szDigits[ (pBytes[GuidMap[i]] & 0x0F) ]; 
        } 
    } 
    *lpsz++ = '}'; 
    *lpsz   = '\0'; 
 
    return TRUE; 
} 
void GrabMutex( HANDLE *mutex, char *name) {

	if (*mutex == NULL) {
		*mutex = CreateMutex (NULL, FALSE, name);
	}

	// Enter Mutex

	WaitForSingleObject( *mutex, INFINITE );
}

void UpdatePlayerInfo( void )
{
	int i;

	if ( !WSA_Active )
		return;

	EnterCriticalSection( &PlayerInfoKey );

	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( ( i == WhoIAm ) || ( GameStatus[ i ] == STATUS_Normal ) || ( GameStatus[ i ] == STATUS_StartingMultiplayer ) )
		{
			// never called in multi?
			PlayerInfo[ i ].CurScore = GetScoreStats(i);

			// copy names / pings here ( inside critical section ) in case of simultaneous access by seperate threads
			strcpy( PlayerInfo[ i ].PlayerName, Names[ i ] );
			PlayerInfo[ i ].Ping = PingTimes[ i ];
			PlayerInfo[ i ].Team = TeamNumber[ i ];
		}
	}

	LeaveCriticalSection( &PlayerInfoKey );
}

void RequestPings( void )
{
	int i;
	char buf[ 256 ];
	char *pCh;

	if ( !ghDPSendThread )	// if no send thread active, no point in trying to send pings
		return;
	
	GrabMutex(&SendMutex, NULL);

	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( i != WhoIAm )
		{
			PlayerInfo[ i ].Ping = 999999;
			if ( GameStatus[ i ] == STATUS_Normal )
			{
				if ( !PlayerIP[ i ].sin_port )	// get IP address if neccesary
				{
					char add[ 16 ];

					memset(&PlayerIP[ i ], 0, sizeof(PlayerIP[ i ]));
					if ( GetIPFromDP( add, Ships[ i ].dcoID ) )
					{
						PlayerIP[ i ].sin_family = AF_INET;
						PlayerIP[ i ].sin_port = htons( DEF_PORT );
						PlayerIP[ i ].sin_addr.s_addr = inet_addr( add );
					}
				}

				if ( PlayerIP[ i ].sin_port )
				{
					pCh = buf;
					pCh += sprintf( pCh, "%s", udp_header );
					pCh += sprintf( pCh, UDP_MSG_Ping );
					pCh += sprintf( pCh, "\\player\\%d", i );
					pCh += sprintf( pCh, "\\time\\%d", timeGetTime() );

					*(++pCh) = 0;	// ensure NULL terminated

					sendto(sockfd, buf, pCh - buf, 0, 
					(struct sockaddr*)&PlayerIP[ i ], sizeof(struct sockaddr_in));
				}
			}
		}else
		{
			PlayerInfo[ i ].Ping = 0;
		}
	}

	ReleaseMutex(SendMutex);
}


void GetUpTime( char *str, DWORD dwTime )
{
   FILETIME FileTime;
   FILETIME sysTime;
   SYSTEMTIME _sysTime;
   // use of 64bit integers should be fine as long
   // as you compile with MS Devstudio.
   __int64 t1, t2;
   int uptime;

   if (dwTime &&
         DosDateTimeToFileTime((WORD)(dwTime & 0xffff),
         (WORD)((dwTime >> 16) & 0xffff), &FileTime)) {

      GetSystemTime(&_sysTime);
      if (SystemTimeToFileTime(&_sysTime, &sysTime)) {

         t1 = ((__int64)FileTime.dwLowDateTime +
            ((__int64)FileTime.dwHighDateTime << 32));
         t2 = ((__int64)sysTime.dwLowDateTime +
            ((__int64)sysTime.dwHighDateTime << 32));
         // t1 = start time, t2 = Current time
         // Get the difference then divide by 10,000,000 to get seconds
         // MS Devstudio will convert the 64bit operations to multiple
         // 32bit operations.
         uptime = (int)((t2 - t1)/10000000);
         // Add an uptime key to the srv_info in the form of
         // "hours:minutes:seconds"
         sprintf(str, "\\Uptime\\%dh:%2.2dm:%2.2ds",
            uptime / 3600, (uptime/60) % 60, uptime % 60);
      }
   }
}

void GetGameType( char *buf, LPDPSESSIONDESC2 pSD )
{
	// game type
	if ( pSD->dwUser3 & CTFGameBit ) 
		strcpy( buf, "ctf" );
	else if ( pSD->dwUser3 & FlagGameBit ) 
		strcpy( buf, "flag chase" );
	else if ( pSD->dwUser3 & BountyGameBit ) 
	{
		if ( pSD->dwUser3 & TeamGameBit )
			strcpy( buf, "team bounty hunt" );
		else
			strcpy( buf, "bounty hunt" );
	} 
	else 
	{
		if ( pSD->dwUser3 & TeamGameBit )
			strcpy( buf, "team game" );
		else
			strcpy( buf, "free for all" );
	}
}

void GetGameSubType( char *buf, LPDPSESSIONDESC2 pSD )
{
	// CTF sub type
	if ( pSD->dwUser3 & CTFGameBit )
	{
		switch ( CTF_Type_Decode( pSD->dwUser3 ) )
		{
		case CTF_STANDARD:
			strcpy( buf, LT_CTF_Normal/*"normal"*/ );
			break;
		case CTF_MUST_CARRY:
			strcpy( buf, LT_CTF_MustCarry/*"carry it back"*/ );
			break;
		case CTF_CAN_CARRY:
			strcpy( buf, LT_CTF_CanCarry/*"can pick up"*/ );
			break;
		case CTF_ONE_TOUCH:
			strcpy( buf, LT_CTF_OneTouch/*"1 touch return"*/ );
			break;
		case CTF_NO_RETURN:
			strcpy( buf, LT_CTF_NoReturn/*"can't pick up"*/ );
			break;
		default:
			strcpy( buf, LT_CTF_Standard/*"standard"*/ );
		}
	}
}

BOOL IsGameJoinable( LPDPSESSIONDESC2 pSD )
{
	return ( ( pSD->dwFlags & DPSESSION_JOINDISABLED ) ? FALSE : TRUE );
}

BOOL ProcessForsakenInfo( int type )
{
	static char strAppGuid[128], strInstGuid[128];
	char	 *pChar;
	char buf[ 128 ];
	FILE *f = NULL;
	int i;
	LPDPSESSIONDESC2 pSD = NULL;
	DWORD dwSize;
	HRESULT hr;

	// first get the size for the session desc
    if ((hr = IDirectPlay3_GetSessionDesc(glpDP, NULL, &dwSize)) == DPERR_BUFFERTOOSMALL)
    {
		// allocate memory for it
        pSD = (LPDPSESSIONDESC2) malloc(dwSize);
        if (pSD)
        {
            // now get the session desc
            hr = IDirectPlay3_GetSessionDesc(glpDP, pSD, &dwSize);
        }
        else
        {
            hr = E_OUTOFMEMORY;
        }
    }

	if( !pSD )
		return FALSE;

	if ( hr != DP_OK )
	{
		free( pSD );
		return FALSE;
	}

	GrabMutex(&SendMutex, NULL);

	srv_info = &pBuf[0];

	strcpy ( srv_info, udp_header );
	srv_info += strlen( udp_header );

	if ( type & HEARTBEAT_Raw )
	{
		srv_info += sprintf( srv_info, "heartbeat\n" );
		ReleaseMutex(SendMutex);
		free( pSD );
		return TRUE;
	}							   	  

	srv_info += sprintf( srv_info, "forsaken\n" );

	StringFromGUID(&pSD->guidApplication, strAppGuid);
	StringFromGUID(&pSD->guidInstance, strInstGuid);

	srv_info += sprintf(srv_info, "\\AppGuid\\%s", strAppGuid);
	srv_info += sprintf(srv_info, "\\InstGuid\\%s", strInstGuid);

	if ( !type || ( type & HEARTBEAT_BasicInfo ) )
	{
		// session name
		pChar = srv_info + sprintf( srv_info, "\\session\\%s", pSD->lpszSessionNameA );
		srv_info = strchr( srv_info, '~' );
		if ( !srv_info )
		{
			srv_info = pChar;
		}
					    
		/*
		// session creation time / date
		srv_info += sprintf( srv_info, "\\created\\%x", pSD->dwUser1 );
		*/
		// session up time
		GetUpTime( buf, pSD->dwUser1 );
		srv_info += sprintf( srv_info, "%s", buf );

		strncpy( buf, pSD->lpszSessionNameA, 128 );
		buf[ 127 ] = 0;
		pChar = strchr( buf, '~' );
		if ( pChar )
		{
			srv_info += sprintf( srv_info, "\\level\\%s", ++pChar );
		}

		// num players
		srv_info += sprintf( srv_info, "\\curplayers\\%d", pSD->dwCurrentPlayers );

		// max players
			srv_info += sprintf( srv_info, "\\maxplayers\\%d", pSD->dwMaxPlayers );
	}

	if ( !type || ( type & HEARTBEAT_GameType ) )
	{
		GetGameType( buf, pSD );
		
		srv_info += sprintf( srv_info, "\\game\\%s", buf );

		buf[ 0 ] = 0;
		GetGameSubType( buf, pSD );
		if ( buf[ 0 ] )
		{
			srv_info += sprintf( srv_info, "\\ctftype\\%s", buf );
		}
		
		// harm team mates?
		if ( pSD->dwUser3 & TeamGameBit )
		{
			srv_info += sprintf( srv_info, "\\HarmTM\\%d",( pSD->dwUser3 & HarmTeamMatesBit ) );
		}
	}

	if ( !type || ( type & HEARTBEAT_PacketInfo ) )
	{																  
		// short packets
		srv_info += sprintf( srv_info, "\\shortpackets\\%d",( pSD->dwUser3 & ShortPacketsBit ) ? 1 : 0 );

		// group packets
		srv_info += sprintf( srv_info, "\\bigpackets\\%d",( pSD->dwUser3 & BigPacketsBit ) ? 1 : 0 );

		// packet rate
		srv_info += sprintf( srv_info, "\\packetrate\\%d", PacketsSlider.value );
	}

	if ( !type || ( type & HEARTBEAT_KillsInfo ) )
	{
		// max kills
		srv_info += sprintf( srv_info, "\\maxkills\\%d", ( pSD->dwUser2 & MaxKillsBits ) >> MaxKills_Shift );

		// current top
		srv_info += sprintf( srv_info, "\\topkills\\%d", ( ( pSD->dwUser2 & CurrentMaxKillsBits ) >> CurrentMaxKills_Shift ) );
	}

	if ( !type || ( type & HEARTBEAT_TimeInfo ) )
	{	 
		// time limit
		srv_info += sprintf( srv_info, "\\timelimit\\%d", ( pSD->dwUser3 & GameTimeBit ) >> GameTimeBit_Shift );

		// current top
		srv_info += sprintf( srv_info, "\\elapsedtime\\%d", 
			( ( pSD->dwUser3 & CurrentGameTimeBits ) >> CurrentGameTime_Shift ) +
			( ( ( pSD->dwUser3 & GameTimeBit ) >> GameTimeBit_Shift == ( pSD->dwUser3 & CurrentGameTimeBits ) >> CurrentGameTime_Shift ) ? 0 : 1 ) );
	}

	if ( !type || ( type & HEARTBEAT_ValidPickups ) )
	{
		uint32 pickupinfo[ MAX_PICKUPFLAGS ];

		PackPickupInfo( pickupinfo );
		srv_info += sprintf(srv_info, "\\validpickups\\" );

		for ( i = 0; i < MAX_PICKUPFLAGS; i++ )
		{
			srv_info += sprintf(srv_info, "%x ", pickupinfo[ i ] );
		}
	}										 

	if ( !type || ( type & HEARTBEAT_ExtraInfo ) )
	{
		switch( ( pSD->dwUser3 & CollisionTypeBits ) >> Collision_Type_BitShift )
		{
		case COLPERS_Forsaken:
			pChar = "on";
			break;
		case COLPERS_Descent:
			pChar = "off";
			break;
			srv_info += sprintf(srv_info, "\\lagtolerance\\%s ", pChar );
		}

		srv_info += sprintf(srv_info, "\\brightbikes\\%d ", ( pSD->dwUser3 & BrightShipsBit ) ? 1 : 0 );
		srv_info += sprintf(srv_info, "\\exhausts\\%d ", ( pSD->dwUser3 & BikeExhaustBit ) ? 1 : 0 );
	}

	if ( !type || ( type & HEARTBEAT_PlayersList ) )
	{
		srv_info += sprintf( srv_info, "\n" );
		
		EnterCriticalSection( &PlayerInfoKey );
	
		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
			if ( ( i == WhoIAm ) || ( GameStatus[ i ] == STATUS_Normal ) || ( GameStatus[ i ] == STATUS_StartingMultiplayer ) )
			{
				srv_info += sprintf(srv_info, "\"%s\" %d %d\n", PlayerInfo[ i ].PlayerName, PlayerInfo[ i ].CurScore, PlayerInfo[ i ].Ping);
			}
		}
		
		LeaveCriticalSection( &PlayerInfoKey );
	}

	ReleaseMutex(SendMutex);
   	free( pSD );
	return TRUE;
}


DWORD WINAPI ListenThread(LPVOID p) {

	struct sockaddr_in from;
	int len, from_len, SelectVal;
	fd_set	in;
	struct timeval tv;
	static char tmpbuf[ 4096 ];
	char *msgptr;
#ifdef UDP_PINGS
	char *tempptr;
#endif
#ifdef DPTEST	
	char buf[ 4096 ];
#endif
	uint16 status_type;

	HANDLE		eventHandles[2];

	eventHandles[0] = DPListenThreadEvent;
	eventHandles[1] = DPListenThreadKillEvent;

	while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)	// while just one event is TRUE
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		FD_ZERO (&in);
		FD_SET (sockfd, &in);
		SelectVal = select ( sockfd+1, &in, 0, 0, &tv );
		
		from_len = sizeof(struct sockaddr_in);

		if ( SelectVal < 0 )
		{
			break;
		} else if ( SelectVal != 0 )
		{
			len = recvfrom ( sockfd, &tmpbuf[0], sizeof(tmpbuf), 0, 
				(struct sockaddr *) &from, &from_len );
			if (len == SOCKET_ERROR && WSAGetLastError() != WSAEMSGSIZE)
				break;
			
			if ( len < ( sizeof( udp_header ) - 1 ) )
				continue;	// message is not even big enough for header

#ifdef DPTEST
			sprintf( buf, "recieving packet" );
			AddColourMessageToQue ( SystemMessageColour, buf );
#endif

			if ( !memcmp( tmpbuf, udp_header, strlen( udp_header ) ) )	// if we have correct header...
			{
				msgptr = tmpbuf + strlen( udp_header );
#ifdef UDP_PINGS
				// ping message?
			   if ( !_strnicmp( msgptr, UDP_MSG_Ping, strlen( UDP_MSG_Ping ) ) )
			   {
				   // replace message with 'bounced' version
				   memcpy( (void *)msgptr, (void *)UDP_MSG_BouncedPing, strlen( UDP_MSG_Ping ) );

				   // bounce back message
				   GrabMutex( &SendMutex, NULL );

				   sendto(sockfd, tmpbuf, len, 0, 
						(struct sockaddr*)&from, sizeof(struct sockaddr_in));

				   ReleaseMutex( SendMutex );

			   // returned ping request?
			   }else if ( !_strnicmp( msgptr, UDP_MSG_BouncedPing, strlen( UDP_MSG_BouncedPing ) ) )
			   {
				   DWORD senttime;
				   BYTE player;

				   msgptr += strlen( UDP_MSG_BouncedPing );

				   tempptr = strstr( msgptr, "player\\" );
				   if ( tempptr )
				   {
					   if ( sscanf( tempptr, "player\\%d\\", &player ) != 1 )
					   {
						   player = WhoIAm;
					   }
				   }

				   tempptr = strstr( msgptr, "time\\" );
				   if ( tempptr )
				   {
					   if ( sscanf( tempptr, "time\\%d\\", &senttime ) != 1 )
					   {
						   senttime = 0;
					   }
				   }

				   if ( ( player != WhoIAm ) && senttime )
				   {

					   EnterCriticalSection( &PlayerInfoKey );
					   PlayerInfo[ player ].Ping = timeGetTime() - senttime;
					   if ( PlayerInfo[ player ].Ping > 999999 )
						   PlayerInfo[ player ].Ping = 999999;
					   LeaveCriticalSection( &PlayerInfoKey );
#ifdef DPTEST
					   sprintf( buf, "ping of %d recieved for player %d ( %s )", PlayerInfo[ player ].Ping, player, Names[ player ] );
					   AddColourMessageToQue ( SystemMessageColour, buf );
#endif
				   }
			   // status request?
			   }else
#endif
			   if ( !_strnicmp( msgptr, UDP_MSG_Status, strlen( UDP_MSG_Status ) ) )
			   {
				  msgptr += strlen( UDP_MSG_Status );
				  
				  if( !(*msgptr) )	// added for Kali
				  {
					    ProcessForsakenInfo( 0 );
#ifdef DPTEST
						sprintf( buf, "sending status msg type 0\n");
					    AddColourMessageToQue ( SystemMessageColour, buf );
#endif
						PostServerInfo( &from );
				  }else
				  {
					  if ( sscanf( msgptr, "%x", &status_type ) == 1 )
					  {
							ProcessForsakenInfo( status_type );
#ifdef DPTEST
							sprintf( buf, "sending status msg type %d\n", status_type);
						    AddColourMessageToQue ( SystemMessageColour, buf );
#endif					
							PostServerInfo( &from );
					  }
				  }
#ifdef DPTEST
			   // game info?
			   }else if ( !_strnicmp( msgptr, UDP_MSG_Forsaken, strlen( UDP_MSG_Forsaken ) ) )
			   {
				  msgptr += strlen( UDP_MSG_Forsaken );
				  AddColourMessageToQue(SystemMessageColour, "game info recieved" );
				  DebugPrintf( "\nrecieving: %s\n", msgptr );
			   }else
			   {
					AddColourMessageToQue ( SystemMessageColour, msgptr );
			   }
#else
			   }
#endif
			}
		}

	}
	
	DebugPrintf("listen thread exited OK\n");
	ExitThread(0);

	return 0;
}

void BroadcastUDP()
{
	int i;
	
	GrabMutex(&SendMutex, NULL);

	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( ( i != WhoIAm ) && ( GameStatus[ i ] == STATUS_Normal ) )
		{
			if (srv_info != &pBuf[0]) 
			{
				*(++srv_info) = 0;	// ensure NULL terminated
				sendto(sockfd, (char *)&pBuf[0], srv_info - &pBuf[0], 0, 
				(struct sockaddr*)&PlayerIP[ i ], sizeof(struct sockaddr_in));
			}
		}
	}

	ReleaseMutex(SendMutex);
}

void PostServerInfo( struct sockaddr_in *addr ) 
{

	GrabMutex(&SendMutex, NULL);

	if ( !srv_info || ( srv_info == pBuf ) )
		return;

	//DebugPrintf("\nsending: %s\n", pBuf);

	*(++srv_info) = 0;	// ensure NULL terminated

	sendto(sockfd, (char *)&pBuf[0], srv_info - &pBuf[0], 0, 
		(struct sockaddr*)addr, sizeof(struct sockaddr_in));

	ReleaseMutex(SendMutex);
}

BOOL ConnectToWinsock(void)
{
	int err;
	WSADATA wsaData;
	uint32 addr;
	struct hostent *hp;
	char FAR * FAR *add; 

	memset(&HostIP, 0, sizeof(HostIP));
	memset(&TrackerIP, 0, sizeof(TrackerIP));

	
	// setup HostIP
	HostIP.sin_family = AF_INET;
	HostIP.sin_port = htons(host_port);
	HostIP.sin_addr.s_addr = inet_addr( IPAddressText );

	// setup TrackerIP	( address to send UDP packets to )
	TrackerIP.sin_family = AF_INET;
	TrackerIP.sin_port = htons (tracker_port);

	if ( !tracker_addr )
	{
		addr = 0;
		if ( tracker_name[ 0 ] )
		{
			hp = gethostbyname ( tracker_name ); 
			if ( hp )
			{
				add = hp->h_addr_list;
				if ( *add )
				{
					addr = *(uint32 *) *add;
				}
			}
		}

		if ( !addr )
		{
			addr = inet_addr( tracker_ip );
		}

		if ( addr == INADDR_NONE )
			return FALSE;

		tracker_addr = addr;
	}else
	{
		addr = tracker_addr;
	}

	TrackerIP.sin_addr.s_addr = addr;
	
/*
	// setup HostIP
	HostIP.sin_family = AF_INET;
	HostIP.sin_port = htons(DEF_PORT);
	HostIP.sin_addr.s_addr = inet_addr("100.106.0.237");

	// setup TrackerIP	( address to send UDP packets to )
	TrackerIP.sin_family = AF_INET;
	TrackerIP.sin_port = htons (MASTER_PORT);
	TrackerIP.sin_addr.s_addr = inet_addr("100.106.0.236");
*/	
	// Let's see if winsock is running
	err = WSAStartup( 0x0101, &wsaData );
	if (err !=0 ) {
		Msg ( "process: No winsock!\n" );
		return FALSE;
	}

	if ( LOBYTE (wsaData.wVersion) != 1 || HIBYTE (wsaData.wVersion) != 1 ) {
		Msg ( "process: Wrong winsock version!\n" );
		WSACleanup();
		return FALSE;
	}

	if ( ( sockfd = socket ( AF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
		Msg ( "process: Can't open datagram socket!\n" );
		WSACleanup();
		return FALSE;
	}

	if ( bind(sockfd, (struct sockaddr *) &HostIP, sizeof(HostIP) ) < 0 ) {
		Msg( "process: can't bind local address!\n" );
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI DPThread(LPVOID p)
{
	HANDLE		eventHandles[2];

	eventHandles[0] = DPSendThreadEvent;
	eventHandles[1] = DPSendThreadKillEvent;

	while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)	// while just one event is TRUE
	{
		if ( IsHost )
		{							

				if ( ProcessForsakenInfo( heartbeat_type ) )
				{
					PostServerInfo( &TrackerIP );	
				}
		}

		WaitForSingleObject( DPSendThreadKillEvent, heartbeat_freq );
	}

	DebugPrintf("send thread exited OK\n");
	ExitThread(0);

	return 0;
}

void DPStopThread(void) 
{
	TrackerOveride = FALSE;
	tracker_addr = 0;
	
	if ( !WSA_Active )
		return;

	// send one final shutdown packet if required...
	if ( SendShutdownPacket )
	{
		GrabMutex(&SendMutex, NULL);

		srv_info = &pBuf[0];

		strcpy ( srv_info, udp_header );
		srv_info += strlen( udp_header );

		srv_info += sprintf( srv_info, "shutdown\n" );
		ReleaseMutex(SendMutex);

		PostServerInfo( &TrackerIP );
	}
	
	// kill off all threads...
	
	if ( ghDPSendThread )
	{
		// wake up thread and wait for it to quit
		ResetEvent( DPSendThreadEvent );
		SetEvent( DPSendThreadKillEvent );
		WaitForSingleObject( ghDPSendThread , INFINITE);
		CloseHandle( ghDPSendThread );
		ghDPSendThread = NULL;
	}

	if ( ghDPListenThread )
	{
		// wake up thread and wait for it to quit
		ResetEvent( DPListenThreadEvent );
		SetEvent( DPListenThreadKillEvent );
		WaitForSingleObject( ghDPListenThread , INFINITE);
		CloseHandle( ghDPListenThread );
		ghDPListenThread = NULL;
	}

	WSACleanup();
	WSA_Active = FALSE;

	if ( SendMutex ) 
		CloseHandle( SendMutex );

	if ( DPSendThreadEvent )
	{
		CloseHandle( DPSendThreadEvent );
		DPSendThreadEvent = NULL;
	}
	if ( DPSendThreadKillEvent )
	{
		CloseHandle( DPSendThreadKillEvent );
		DPSendThreadKillEvent = NULL;
	}
	if ( DPListenThreadEvent )
	{
		CloseHandle( DPListenThreadEvent );
		DPListenThreadEvent = NULL;
	}
	if ( DPSendThreadKillEvent )
	{
		CloseHandle( DPSendThreadKillEvent );
		DPSendThreadKillEvent = NULL;
	}

}

BOOL DPStartThread( void ) {

	DWORD tID;

	if ( !TrackerOveride )
	{
		if ( !DoHeartbeat )
			return TRUE;

		if ( !PeerPeerHeartbeat )
			return TRUE;
		
		if ( !ForceHeartbeat )
		{
			if ( memcmp( &ServiceProvidersGuids[ServiceProvidersList.selected_item], &DPSPGUID_TCPIP , sizeof(GUID) ) != 0 )
				 return TRUE;
		}
	}

	if (ConnectToWinsock() == FALSE) 
	{
		//Msg( "Error Loading Winsock" );
		DebugPrintf( "Error Loading Winsock" );
		WSA_Active = FALSE;
		return FALSE;
	}

	WSA_Active = TRUE;

	// create all events for send / listen threads....
	DPSendThreadEvent = CreateEvent			(	NULL,		// no security
												TRUE,		// manual reset
												FALSE,		// initial event reset
												NULL);		// no name
	DPSendThreadKillEvent = CreateEvent		(	NULL,		// no security
												TRUE,		// manual reset
												FALSE,		// initial event reset
												NULL);		// no name
	DPListenThreadEvent = CreateEvent		(	NULL,		// no security
												TRUE,		// manual reset
												FALSE,		// initial event reset
												NULL);		// no name
	DPListenThreadKillEvent = CreateEvent	(	NULL,		// no security
												TRUE,		// manual reset
												FALSE,		// initial event reset
												NULL);		// no name

	SendMutex = NULL;
	pBuf[0] = 0;
	memset( PlayerInfo, 0, sizeof( PlayerInfo ) );
	InitializeCriticalSection( &PlayerInfoKey );

	memset( PingTimes, 0, sizeof ( PingTimes ) ); 

	ghDPSendThread = NULL;
	ghDPListenThread = NULL;

	if ( heartbeat_freq )
	{
		SetEvent( DPSendThreadEvent );

		ghDPSendThread = CreateThread( NULL, 16384, DPThread, NULL, 0, &tID );

		if ( !ghDPSendThread )
		{
			Msg( "Error Creating UDP send thread" );
			DPStopThread();
			return FALSE;
		}

	}

	SetEvent( DPListenThreadEvent );

	ghDPListenThread = CreateThread( NULL, 8192, ListenThread, NULL, 0, &tID);

	if ( !ghDPListenThread )
	{
		Msg( "Error Creating UDP listen thread" );
		DPStopThread();
		return FALSE;
	}

	return TRUE;
}

typedef BOOL (*ReadHeartbeatInfo)( FILE *, char * );

typedef struct {
	char *keyword;
	ReadHeartbeatInfo handle;
} READHEARTBEATINFOTABLE;	

#define TRACKERFOLDER				"TRACKER"
#define TRACKERFILE_EXTENSION		".trk"
#define TRACKERFILE_SEARCHPATH		TRACKERFOLDER"\\*"TRACKERFILE_EXTENSION
