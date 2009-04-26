
#define IDIRECTPLAY2_OR_GREATER

#include "stdwin.h"
#include <dplay.h>
#include <dplobby.h>
#include <stdio.h>
#include "net.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "d3dappi.h"
#include "main.h"
#include "title.h"
#include "primary.h"
#include "XMem.h"
#include "multiplayer.h"
#include "typedefs.h"


/*
 * Externals
 */
extern	LIST	PlayersList;
extern BOOL	BrightShips;
extern	BOOL	MyBrightShips;
extern BOOL	BikeExhausts;
extern int32 ColPerspective;
extern SLIDER CTFSlider;
extern	int16	MaxKills;
extern	BOOL ResetKillsPerLevel;
extern network_id_t	PlayerIDs[ MAX_PLAYERS ];
extern SLIDER  PacketsSlider;
extern	network_id_t                    dcoID;
extern	LPDIRECTPLAY4A              glpDP;     // directplay object pointer
extern	BOOL					TeamGame;
extern	HANDLE					hPlayerEvent;					// player event to use
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	TimeLimit;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	BOOL	BountyHunt;
extern	uint16	RandomStartPosModify;
extern	BOOL	UseShortPackets;
extern	BOOL					IsHost;   // is the user hosting/joining a game
extern	SHORTNAMETYPE			Names;	// all the players short Names....

void DebugPrintf( const char * format, ... );
BOOL JoinASession ( MENUITEM * Item );


/*
 * Globals
 */

LPDPSESSIONDESC2					network_session=NULL;
LPDIRECTPLAY4A						glpDP=NULL;     // directplay object pointer
LPDPSESSIONDESC2                    glpdpSD = NULL;            // current session description
LPDIRECTPLAYLOBBY2A					lpDPlayLobby = NULL;		//Lobby stuff...

													   
/*
 * DPlayClose
 *
 * Wrapper for DirectPlay Close API
 */
HRESULT DPlayClose( LPDIRECTPLAY4A dp )
{
    HRESULT hr=E_FAIL;

    if ( dp ) 
        hr = IDirectPlayX_Close( dp );
    
    return hr;
}

int network_create_player( network_id_t * id, LPTSTR lptszPlayerName )
{
    HRESULT hr = E_FAIL;
    DPNAME name;
	
	if (!glpDP)
		return hr;

	// name structure
	ZeroMemory(&name,sizeof(name));
    name.dwSize = sizeof(DPNAME);
    name.lpszShortNameA = lptszPlayerName;

	hr = IDirectPlayX_CreatePlayer(glpDP, id, &name, NULL, NULL, 0, 0);

	switch( hr )
	{
	case DPERR_CANTADDPLAYER:
		DebugPrintf("DPERR_CANTADDPLAYER\n");
		break;
	case DPERR_CANTCREATEPLAYER:
		DebugPrintf("DPERR_CANTCREATEPLAYER\n");
		break;
	case DPERR_INVALIDFLAGS:
		DebugPrintf("DPERR_INVALIDFLAGS\n");
		break;
	case DPERR_INVALIDPARAMS:
		DebugPrintf("DPERR_INVALIDPARAMS\n");
		break;
	case DPERR_NOCONNECTION:
		DebugPrintf("DPERR_NOCONNECTION\n");
		break;
	}

	if(FAILED(hr))
		return 0;

    return 1;
}

int network_host(LPTSTR lptszSessionName)
{
    HRESULT hr = E_FAIL;
    DPSESSIONDESC2 dpDesc;

	ZeroMemory(&dpDesc, sizeof(dpDesc));
    dpDesc.dwSize = sizeof(dpDesc);

	// cannot be changed once the session has started:
	// DPSESSION_CLIENTSERVER, DPSESSION_DIRECTPLAYPROTOCOL,
	// DPSESSION_KEEPALIVE, DPSESSION_MIGRATEHOST,
	// DPSESSION_MULTICASTSERVER, DPSESSION_NOMESSAGEID,
	// DPSESSION_OPTIMIZELATENCY, and DPSESSION_SECURESERVER.
	
	// The other flags can be changed by the host:
	// DPSESSION_JOINDISABLED, DPSESSION_NEWPLAYERSDISABLED,
	// DPSESSION_NODATAMESSAGES, DPSESSION_NOPRESERVEORDER,
	// DPSESSION_PASSWORDREQUIRED, and DPSESSION_PRIVATE,

	// DPSESSION_MIGRATEHOST cannot be used with:
	// DPSESSION_CLIENTSERVER, DPSESSION_MULTICASTSERVER,
	// or DPSESSION_SECURESERVER

    dpDesc.dwFlags = DPSESSION_MIGRATEHOST |

	// DPSESSION_KEEPALIVE
	// Detects if a player abnormally dropped out of the game.
	// If so they are informed that their connected was lost
	// when they return.  In the meantime it removes them
	// from the session.

		             DPSESSION_KEEPALIVE |
					 
	// DPSESSION_DIRECTPLAYPROTOCOL 
	// Specifies that this session should use the,
	// "DirectPlay guaranteed protocol" instead of the one
	// implemented by the service provider.
		// Use this flag if message throttling is desired
		// (and you want the performance gains it provides),
		// or if you want to ensure the availability of all
		// the features of IDirectPlay4::SendEx.
			// This flag also enables guaranteed sends on
			// unreliable providers and uses DirectPlay's
			// reliability code on TCP/IP, which generally
			// gives better performance both in throughput
			// and delivery.

		             DPSESSION_DIRECTPLAYPROTOCOL |

	// DPSESSION_NOPRESERVEORDER
	// Removes uneeded delays in delivery if the order
	// of packets is not important.  Games mostly send
	// short updates of player position data.  Late
	// packets are not normally not desired at all.
	// This may result in some weapon events happening
	// out of order but it's worse if you a stream of
	// events all got affected cause of one late packet.

		             DPSESSION_NOPRESERVEORDER |

	// DPSESSION_OPTIMIZELATENCY
	// Turns off tcp nagling... Many unix heads know this
	// as the common NODELAY setting.  Naggling collects
	// tinygrams into larger packets for sending.  This
	// is beneficial for bandwidth but bad for time sensi-
	// tive applications that need to get tiny updates out
	// as fast as possible and at a steady rate...

					 DPSESSION_OPTIMIZELATENCY;

	dpDesc.dwMaxPlayers = MaxPlayersSlider.value;
    dpDesc.lpszSessionNameA = lptszSessionName;

    // set the application guid
	dpDesc.guidApplication = PROJX_GUID;

    if (!glpDP)
		DebugPrintf("network_host: !glpDP Could not create direct play session.");

    hr = IDirectPlayX_Open(glpDP, &dpDesc, DPOPEN_CREATE);

	switch(hr)
	{
	case DP_OK:
		break;
	case DPERR_ALREADYINITIALIZED:
		DebugPrintf("network_host: DPERR_ALREADYINITIALIZED\n",hr);
		break;
	case DPERR_INVALIDFLAGS:
		DebugPrintf("network_host: DPERR_INVALIDFLAGS\n",hr);
		break;
	case DPERR_INVALIDPARAMS:
		DebugPrintf("network_host: DPERR_INVALIDPARAMS\n",hr);
		break;
	case DPERR_NOCONNECTION:
		DebugPrintf("network_host: DPERR_NOCONNECTION\n",hr);
		break;
	case DPERR_TIMEOUT:
		DebugPrintf("network_host: DPERR_TIMEOUT\n",hr);
		break;
	case DPERR_UNINITIALIZED:
		DebugPrintf("network_host: DPERR_UNINITIALIZED\n",hr);
		break;
	case DPERR_USERCANCEL:
		DebugPrintf("network_host: DPERR_USERCANCEL\n",hr);
		break;
	default:
		DebugPrintf("network_host: failed %x\n",hr);
	}

	if(FAILED(hr))
		return 0;

    return 1;
}

HRESULT DPlayEnumSessions(DWORD dwTimeout, LPDPENUMSESSIONSCALLBACK2 lpEnumCallback, LPVOID lpContext, DWORD dwFlags)
{

	// default failed
    HRESULT hr = E_FAIL;

	// describes the sessions we want to find
    DPSESSIONDESC2 dpDesc;

	// set it up
    ZeroMemory(&dpDesc, sizeof(dpDesc));
    dpDesc.dwSize = sizeof(dpDesc);

	// guid of applicaton we want to enumerate
	// use GUID_NULL to enumerate all applications
    dpDesc.guidApplication = PROJX_GUID;

	// we want to find private sessions as well
	// do we need to provide a password now ???
	dpDesc.lpszPasswordA = "";

	// if we have a direct play lobby available
	// then enumerate the sessions
    if (glpDP)
		hr = IDirectPlayX_EnumSessions(
				glpDP,			// pointer to lobby object
				&dpDesc,		// describes the sessions we want to find
				dwTimeout,		// interval to ask for sessions
				lpEnumCallback,	// callback which receives each found session
				lpContext,		// pointer to object to pass to the callback
				dwFlags			// flags for the enumeration process
				);

	// we cannot enumerate sessions
	else
	{
		DebugPrintf("DPlayEnumSessions: !glpDP Could not enumerate sessions...");
		Msg("Cannot enumerate sessions.\n DirectPlay Interface Missing!");
	}

	// return result
    return hr;
}

LPDPSESSIONDESC2 network_get_description(void)
{
    HRESULT hr=E_FAIL;
    DWORD dwSize;

	if(!glpDP)
		return NULL;

    if (glpdpSD)
    {
        free(glpdpSD);
        glpdpSD = NULL;
    }

    if ((hr = IDirectPlayX_GetSessionDesc(glpDP, NULL, &dwSize)) == DPERR_BUFFERTOOSMALL)
    {
        glpdpSD = (LPDPSESSIONDESC2) malloc(dwSize);
        if (glpdpSD)
            hr = IDirectPlayX_GetSessionDesc(glpDP, glpdpSD, &dwSize);
    }

	return glpdpSD;
}

void network_set_description( void )
{
    HRESULT hr=E_FAIL;

    if ( ! glpDP || ! glpdpSD)
		return;

	hr = IDirectPlayX_SetSessionDesc(glpDP, glpdpSD, 0 );

	if ( hr != DP_OK )
		switch ( hr )
		{
		case DPERR_ACCESSDENIED:
			DebugPrintf("network_set_description() error - DPERR_ACCESSDENIED\n");
			break;
		case DPERR_INVALIDPARAMS:
			DebugPrintf("network_set_description() error - DPERR_INVALIDPARAMS\n");
			break;
		case DPERR_NOSESSIONS:
			DebugPrintf("network_set_description() error - DPERR_NOSESSIONS\n");
			break;
		default:
			DebugPrintf("network_set_description() error - unknown\n");
		}
}

int network_join( void )
{
    HRESULT hr = E_FAIL;
    DPSESSIONDESC2 dpDesc;

    if( !glpDP || !network_session )
		return 0;

    ZeroMemory(&dpDesc, sizeof(dpDesc));
    dpDesc.dwSize = sizeof(dpDesc);
    dpDesc.guidInstance = network_session->guidInstance;
    dpDesc.guidApplication = PROJX_GUID;

	hr = IDirectPlayX_Open(glpDP, &dpDesc, DPOPEN_JOIN);

	switch(hr)
	{
	case DP_OK:
	case DPERR_ALREADYINITIALIZED:
		return 1;
		break;
	case DPERR_CANTCREATEPLAYER:
		DebugPrintf("network_join: to many players\n");
		break;
	case DPERR_INVALIDFLAGS:
		DebugPrintf("network_join: DPERR_INVALIDFLAGS\n",hr);
		break;
	case DPERR_INVALIDPARAMS:
		DebugPrintf("network_join: DPERR_INVALIDPARAMS\n",hr);
		break;
	case DPERR_NOCONNECTION:
		DebugPrintf("network_join: DPERR_NOCONNECTION\n",hr);
		break;
	case DPERR_TIMEOUT:
		DebugPrintf("network_join: DPERR_TIMEOUT\n",hr);
		break;
	case DPERR_UNINITIALIZED:
		DebugPrintf("network_join: DPERR_UNINITIALIZED\n",hr);
		break;
	case DPERR_USERCANCEL:
		DebugPrintf("network_join: DPERR_USERCANCEL\n",hr);
		break;
	case DPERR_NOSESSIONS:
		DebugPrintf("network_join: DPERR_NOSESSIONS\n",hr);
		break;
	default:
		DebugPrintf("network_join: failed %x\n",hr);
	}

    return 0;
}

HRESULT DPlayRelease(void)
{
    HRESULT hr = E_FAIL;

    if (glpDP != NULL)
    {
        hr = IDirectPlayX_Release(glpDP);
        glpDP = NULL;
    }

    return hr;
}

HRESULT DPlayCreateLobby( void )
{
	LPDIRECTPLAYLOBBYA	lpDPlayLobbyA = NULL;
	LPDIRECTPLAYLOBBY2A	lpDPlayLobby2A = NULL;
	HRESULT				hr;
		
	if (lpDPlayLobby)
	{
		lpDPlayLobby->lpVtbl->Release(lpDPlayLobby);
		lpDPlayLobby = NULL;
	}
	
	// get ANSI DirectPlayLobby interface
	hr = DirectPlayLobbyCreate(NULL, &lpDPlayLobbyA, NULL, NULL, 0);
	if FAILED(hr)
		goto FAILURE;
	// get ANSI DirectPlayLobby2 interface
	hr = lpDPlayLobbyA->lpVtbl->QueryInterface(lpDPlayLobbyA,
							&IID_IDirectPlayLobby2A, (LPVOID *) &lpDPlayLobby2A);
	if FAILED(hr)
		goto FAILURE;
	 
	// don't need DirectPlayLobby interface anymore
	lpDPlayLobbyA->lpVtbl->Release(lpDPlayLobbyA);
	lpDPlayLobbyA = NULL;

	// Fill in the ANSI lobby interface
	lpDPlayLobby = lpDPlayLobby2A;
	 
	
	return (DP_OK);

FAILURE:
	DebugPrintf("DPlayCreateLobby failure\n");
	if (lpDPlayLobbyA)
		lpDPlayLobbyA->lpVtbl->Release(lpDPlayLobbyA);
	if (lpDPlayLobby2A)
		lpDPlayLobby2A->lpVtbl->Release(lpDPlayLobby2A);

	return (hr);
}

static HRESULT CreateAddress( LPVOID *lplpAddress, char * TCPIPAddress )
{
	LPVOID						lpAddress = NULL;
	DWORD						dwAddressSize = 0;
	HRESULT						hr;
	DWORD dwElementCount = 0;

	// TCP/IP needs a service provider and an IP address
	DPCOMPOUNDADDRESSELEMENT addressElements[2];

	// service provider
	addressElements[dwElementCount].guidDataType = DPAID_ServiceProvider;
	addressElements[dwElementCount].dwDataSize = sizeof(GUID);
	addressElements[dwElementCount].lpData = (LPVOID) &DPSPGUID_TCPIP;
	dwElementCount++;

	if ( TCPIPAddress != NULL )
	{
		// This is where you would fill in the IP Address..
		addressElements[dwElementCount].guidDataType = DPAID_INet;
		addressElements[dwElementCount].dwDataSize = lstrlen(TCPIPAddress) + 1;
		addressElements[dwElementCount].lpData = (LPVOID) TCPIPAddress;
		dwElementCount++;
	}

	// see how much room is needed to store this address
	hr = lpDPlayLobby->lpVtbl->CreateCompoundAddress(
		lpDPlayLobby, addressElements, dwElementCount, NULL, &dwAddressSize );

	if (hr != DPERR_BUFFERTOOSMALL)
	{
		switch( hr )
		{

		case DPERR_BUFFERTOOSMALL:
			goto FAILURE;
			break;
		case DPERR_INVALIDFLAGS:
			goto FAILURE;
			break;

		case DPERR_INVALIDPARAMS:
			goto FAILURE;
			break;

		default:
			break;
		}

		goto FAILURE;

	}

	// allocate space
	lpAddress = malloc(dwAddressSize);
	if (lpAddress == NULL)
	{
		hr = DPERR_NOMEMORY;
		goto FAILURE;
	}

	// create the address
	hr = lpDPlayLobby->lpVtbl->CreateCompoundAddress(lpDPlayLobby,
						addressElements, dwElementCount,
						lpAddress, &dwAddressSize);
	if FAILED(hr)
		goto FAILURE;

	// return the address info
	*lplpAddress = lpAddress;

	return (DP_OK);

FAILURE:
	if (lpAddress)
	{
		free(lpAddress);
		lpAddress = NULL;
	}
	return (hr);
}

BOOL InitializeDPlay( char * TCPIPAddress )
{
	LPVOID			lpAddress = NULL;
	HRESULT			hr;

	// get service provider address from information in dialog
	if ( FAILED( CreateAddress( &lpAddress, TCPIPAddress ) ) )
		return FALSE;

	// initialize the connection using the address
	hr = glpDP->lpVtbl->InitializeConnection(glpDP, lpAddress, 0);
	if FAILED(hr)
	{
		switch ( hr )
		{
		case DPERR_ALREADYINITIALIZED:
			DebugPrintf("DPERR_ALREADYINITIALIZED\n");
			break;
		case DPERR_INVALIDFLAGS:
			DebugPrintf("DPERR_INVALIDFLAGS\n");
			break;
		case DPERR_INVALIDPARAMS:
			DebugPrintf("DPERR_INVALIDPARAMS\n");
			break;
		case DPERR_UNAVAILABLE:
			DebugPrintf("DPERR_UNAVAILABLE\n");
			break;
		}
		return FALSE;
	}
	return TRUE;
}

void network_initialize( char * TCPIPAddress )
{
	// close everything up
	network_cleanup();

	// create dplay lobby interface
	if (!lpDPlayLobby)
		DPlayCreateLobby();

	// create a DirectPlay ANSI interface
	if FAILED(
		CoCreateInstance(
			&CLSID_DirectPlay,
			NULL,
			CLSCTX_INPROC_SERVER,
			&IID_IDirectPlay4A,
			(LPVOID*)&glpDP
			))
		goto FAILURE;

	// must be initialized
	if(!InitializeDPlay( TCPIPAddress ))
		goto FAILURE;

	return;

FAILURE:
	network_cleanup();
}

HRESULT DPLobbyRelease(void)
{
	HRESULT hr=E_FAIL;

	// release the lobby object
	if (lpDPlayLobby)
	{
		hr = IDirectPlayLobby_Release(lpDPlayLobby);
		lpDPlayLobby = NULL;
	}
	return hr;
}

int network_get_player_name( network_id_t id, char* name )
{
	HRESULT hr;
	int size = 256;
	char dp_name[256];
	LPDPNAME lp_name;
	if(!glpDP)return 0;
	hr = IDirectPlayX_GetPlayerName( glpDP, id, (void*) &dp_name, &size );
	if(FAILED(hr)) return 0;
	lp_name = (LPDPNAME) &dp_name[0];
	lp_name->dwSize = sizeof(DPNAME);
	strncpy(name,lp_name->lpszShortNameA,8);
	name[7] = 0;
	return 1;
}

void network_set_player_name( network_id_t pid, char * NamePnt )
{
	DPNAME	Name;
    if (!glpDP)
		return;
	memset(&Name, 0, sizeof(DPNAME));
	Name.dwSize = sizeof(DPNAME);
	Name.lpszShortNameA = NamePnt;
    Name.lpszLongNameA = NamePnt;
    IDirectPlayX_SetPlayerName(glpDP, pid, &Name, DPSET_GUARANTEED);
}

BOOL WINAPI EnumSessionsCallback(LPCDPSESSIONDESC2 lpDPSessionDesc, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID refreshing )
{
	// sessions have been enumerated
    if(dwFlags & DPESC_TIMEDOUT)
	{
		// we are done refreshing the list
		*((int*)refreshing) = 0;

		// stop enumeration
		return FALSE;
	}

	// store away pointer to session description
	if( network_session != NULL ) free( network_session );
	network_session = malloc( sizeof( DPSESSIONDESC2 ) );
	*network_session = *lpDPSessionDesc;

	// stop enumeration
    return FALSE;
}

// enumerate sessions
int network_ready( void )
{
	static int refreshing = 0;

	// ready
	if( network_session )
		return 1;

	// 
	if( refreshing ) return 0;
	refreshing = 1;

	// Enumerate Sessions
	// and we will decide the timeout
	DPlayEnumSessions(
		10000, // interval
		EnumSessionsCallback,	// callback
		(void *) &refreshing,	// user pointer
		DPENUMSESSIONS_ASYNC // do not block
		);

	// not ready
	return 0;
}

void EvalSysMessage( DWORD len , LPDPMSG_GENERIC lpMsg )
{
	if (!lpMsg)
		return;

    switch( lpMsg->dwType)
    {

	case DPSYS_CREATEPLAYERORGROUP:
		{
			LPDPMSG_CREATEPLAYERORGROUP lpAddMsg = (LPDPMSG_CREATEPLAYERORGROUP) lpMsg;
			network_event_new_player( lpAddMsg->dpId, lpAddMsg->dpnName.lpszShortNameA );
		}
		break;

    case DPSYS_HOST:
		{
			network_event_i_am_host();
		}
		break;

	case DPSYS_DESTROYPLAYERORGROUP:
		{
			LPDPMSG_DESTROYPLAYERORGROUP lpDestroyMsg = ( LPDPMSG_DESTROYPLAYERORGROUP ) lpMsg;
			if( lpDestroyMsg->dwPlayerType != DPPLAYERTYPE_PLAYER )
				return;
			network_event_destroy_player( lpDestroyMsg->dpId );
		}
		break;

	case DPSYS_SETPLAYERORGROUPNAME:
		{
			LPDPMSG_SETPLAYERORGROUPNAME lpNameMsg = (LPDPMSG_SETPLAYERORGROUPNAME) lpMsg;
			if( lpNameMsg->dwPlayerType != DPPLAYERTYPE_PLAYER )
				return;
			network_event_player_name( lpNameMsg->dpId, lpNameMsg->dpnName.lpszShortNameA );
		}
		break;
	}
}

void network_pump( void )
{
	network_id_t dcoReceiveID;
    HRESULT hr;

	if( ! glpDP )
		return;

	while(1)
	{
		network_id_t from_dcoID;
		BYTE ReceiveCommBuff[1024];
		DWORD nBytes = 1024;
		hr = glpDP->lpVtbl->Receive(
								glpDP, &from_dcoID, &dcoReceiveID,
								DPRECEIVE_ALL, &ReceiveCommBuff[0], &nBytes );

		switch( hr )
		{
		case DP_OK:

			if ( from_dcoID == DPID_SYSMSG )
				EvalSysMessage( nBytes, (LPDPMSG_GENERIC) &ReceiveCommBuff[0] );

			else
				network_event_new_message( from_dcoID, &ReceiveCommBuff[0], nBytes );

			break;
		case DPERR_BUFFERTOOSMALL:
			DebugPrintf("DPERR_BUFFERTOOSMALL\n");
			break;  
		case DPERR_GENERIC:
			DebugPrintf("DPERR_GENERIC\n");
			break;  
		case DPERR_INVALIDOBJECT:
			DebugPrintf("DPERR_INVALIDOBJECT\n");
			break;  
		case DPERR_INVALIDPARAMS:
			DebugPrintf("DPERR_INVALIDPARAMS\n");
			break;  
		case DPERR_INVALIDPLAYER:
			DebugPrintf("DPERR_INVALIDPLAYER\n");
			break;  
		case DPERR_NOMESSAGES:
			return; // finished
			break;  
		}
	}
}

void network_cleanup()
{
	if(!glpDP)return;
	if(network_session != NULL)
	{
		free(network_session);
		network_session = NULL;
	}
	IDirectPlayX_DestroyPlayer(glpDP, dcoID);
	DPlayRelease();
}

void network_send( network_id_t to, void* data, DWORD size, int guaranteed )
{
	HRESULT hr;
	DWORD flags = DPSEND_ASYNC;

	if(!glpDP)
		return;

	if( guaranteed )
		flags |= DPSEND_GUARANTEED;

	hr = glpDP->lpVtbl->SendEx(	glpDP, dcoID, to, flags, data, size, 0, 0, 0, 0 );

	switch ( hr )
	{
	case DP_OK:
	case DPERR_PENDING: // DPSEND_ASYNC so packet went to queue
		break;
	case DPERR_BUSY:
		DebugPrintf( "network_send DPERR_BUSY\n");
		break;
	case DPERR_CONNECTIONLOST:
		DebugPrintf( "network_send DPERR_CONNECTIONLOST\n");
		break;
	case DPERR_INVALIDPARAMS:
		DebugPrintf( "network_send DPERR_INVALIDPARAMS\n");
		break;
	case DPERR_INVALIDPLAYER:
		DebugPrintf( "network_send DPERR_INVALIDPLAYER id %d\n",to);
		break; 
	case DPERR_NOTLOGGEDIN:
		DebugPrintf( "network_send DPERR_NOTLOGGEDIN\n");
		break;
	case DPERR_SENDTOOBIG:
		DebugPrintf( "network_send DPERR_SENDTOOBIG %d bytes\n",size);
		break;
	case DPERR_UNSUPPORTED:
		DebugPrintf( "network_send DPERR_UNSUPPORTED\n");
		break;
	default:
		DebugPrintf( "network_send unknown send error\n");
	}
}
