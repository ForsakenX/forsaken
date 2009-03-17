


/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       comm.c
 *  Content:    DirectPlay related code
 *
 *
 ***************************************************************************/

#include "stdwin.h"
#include <dplay.h>
#include <stdio.h>
#include "comm.h"
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "d3dappi.h"
#include "main.h"
#include "mydplay2.h"
#include "title.h"
#include "primary.h"
#include "XMem.h"
#include "multiplayer.h"

/*
 * Externals
 */
extern	LIST	PlayersList;
extern	BOOL	HarmTeamMates;
extern BOOL	BrightShips;
extern	BOOL	MyBrightShips;
extern BOOL	BikeExhausts;
extern int32 ColPerspective;
extern SLIDER CTFSlider;
extern	int16	MaxKills;
extern	BOOL ResetKillsPerLevel;
extern DPID	PlayerIDs[ MAX_PLAYERS ];
extern BOOL PseudoHostCanSetMaxPlayers;
extern SLIDER  PacketsSlider;

/*
 * Globals
 */

LPDPSESSIONDESC2                    glpdpSD = NULL;            // current session description
#ifdef MANUAL_SESSIONDESC_PROPAGATE
LPDPSESSIONDESC2                    glpdpSD_copy = NULL;            // current session description
#endif
LPDIRECTPLAYLOBBY2A					lpDPlayLobby = NULL;		//Lobby stuff...
LPDPLCONNECTION						glpdplConnection = NULL;	// connection settings

extern	DPID                    dcoID;

extern	LPDIRECTPLAY4A              glpDP;     // directplay object pointer

extern	DPSESSIONDESC2			Old_Session;
extern	DWORD                   Old_WhoIAm;
extern	DWORD					Old_Kills;
extern	DWORD					Old_Deaths;
extern	char					Old_Name[256];
extern	BOOL					Rejoining;
extern	BOOL					TeamGame;
extern	HANDLE					hPlayerEvent;					// player event to use
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	TimeLimit;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	BOOL	BountyHunt;
extern	uint16	RandomStartPosModify;
extern	BOOL	DplayRecieveThread;
extern	BOOL	UseShortPackets;
extern	BOOL					IsHost;   // is the user hosting/joining a game
extern	SHORTNAMETYPE			Names;	// all the players short Names....

void DebugPrintf( const char * format, ... );
BOOL JoinASession ( MENUITEM * Item );
													   
/*
 * DPlayClose
 *
 * Wrapper for DirectPlay Close API
 */
HRESULT DPlayClose(void)
{
    HRESULT hr=E_FAIL;

    if (glpDP) 
        hr = IDirectPlayX_Close(glpDP);
    
    return hr;
}

/*
 * DPlayCreate
 *
 * Wrapper for DirectPlay Create API.
 * 
 */
HRESULT DPlayCreate(LPGUID lpGuid)
{
    HRESULT hr=E_FAIL;
    LPDIRECTPLAY lpDP=NULL;

    // create a DirectPlay1 interface
    if ((hr = DirectPlayCreate(lpGuid, &lpDP, NULL)) == DP_OK)
    {
        if (lpDP)
        {
            // query for a DirectPlay interface
            hr = IDirectPlay_QueryInterface(lpDP,&IID_IDirectPlay4A,(LPVOID *)&glpDP);
            // no longer need the DirectPlay1 interface
            IDirectPlay_Release(lpDP);
        }
    }

    return hr;
}
/*
 * DPlayCreatePlayer
 *
 * Wrapper for DirectPlay CreatePlayer API. 
 */

HRESULT DPlayCreatePlayer(LPDPID lppidID, LPTSTR lptszPlayerName, HANDLE hEvent, 
                          LPVOID lpData, DWORD dwDataSize)
{
    HRESULT hr=E_FAIL;
    DPNAME name;
    
	SetupConnection( myglobs.hInstApp );
	
	ZeroMemory(&name,sizeof(name));
    name.dwSize = sizeof(DPNAME);

    name.lpszShortNameA = lptszPlayerName;

	if( DplayRecieveThread )
	{
		if (glpDP)
			hr = IDirectPlayX_CreatePlayer(glpDP, lppidID, &name, hPlayerEvent, lpData, 
										  dwDataSize, 0);
	}else{
		if (glpDP)
			hr = IDirectPlayX_CreatePlayer(glpDP, lppidID, &name, NULL, lpData, 
										  dwDataSize, 0);
	}

	switch( hr )
	{
	case DPERR_CANTADDPLAYER :
		DebugPrintf("DPERR_CANTADDPLAYER\n");
		break;
	case DPERR_CANTCREATEPLAYER :
		DebugPrintf("DPERR_CANTCREATEPLAYER\n");
		break;
	case DPERR_INVALIDFLAGS :
		DebugPrintf("DPERR_INVALIDFLAGS\n");
		break;
	case DPERR_INVALIDPARAMS :
		DebugPrintf("DPERR_INVALIDPARAMS\n");
		break;
	case DPERR_NOCONNECTION :
		DebugPrintf("DPERR_NOCONNECTION\n");
		break;
	case DP_OK:
		DebugPrintf("IDirectPlayX_CreatePlayer DP_OK\n");
		break;
	}
                                    
    return hr;
}

void StoreSessionUserFields( LPDPSESSIONDESC2 lpDesc )
{
	SYSTEMTIME	SystemTime;
	FILETIME FileTime;
	WORD	date;
	WORD	time;

	// store the creation time for this session
	GetSystemTime( &SystemTime );
	SystemTimeToFileTime( &SystemTime , &FileTime );
	FileTimeToDosDateTime( &FileTime , &date , &time );
	lpDesc->dwUser1 = date + (time << 16);
	lpDesc->dwUser2 = RandomStartPosModify;	// only lower word is used...

	if ( MaxKills > 255 )	// ensure 8 bit
		MaxKills = 255;

	lpDesc->dwUser2 |= ( MaxKills << MaxKills_Shift );
	
	if ( TimeLimit.value > 30 )
		TimeLimit.value = 30;
	
	lpDesc->dwUser3 = 0;
	lpDesc->dwUser3 |= ( TimeLimit.value << GameTimeBit_Shift );
	lpDesc->dwUser3 |= ( TimeLimit.value << CurrentGameTime_Shift );

	if( TeamGame )
		lpDesc->dwUser3 |= TeamGameBit;
	if( CTF )
		lpDesc->dwUser3 |= CTFGameBit;
	if( CaptureTheFlag )
		lpDesc->dwUser3 |= FlagGameBit;
	if ( BountyHunt )
		lpDesc->dwUser3 |= BountyGameBit;
	if ( UseShortPackets )
		lpDesc->dwUser3 |= ShortPacketsBit;

	// new additions ( previously in MSG_INIT )
	if ( HarmTeamMates )
		lpDesc->dwUser3 |= HarmTeamMatesBit;
	if ( MyBrightShips )
		lpDesc->dwUser3 |= BrightShipsBit;
	if(ResetKillsPerLevel )
		lpDesc->dwUser3 |= ResetKillsPerLevelBit;

	if ( BikeExhausts )
		lpDesc->dwUser3 |= BikeExhaustBit;

	lpDesc->dwUser3 |= ( ColPerspective << Collision_Type_BitShift );

	lpDesc->dwUser3 |= CTF_Type_Encode( ( DWORD )CTFSlider.value );

	if ( PseudoHostCanSetMaxPlayers )
		lpDesc->dwUser3 |= EnableMaxPlayersChangeBit;

	lpDesc->dwUser4 |= ( MaxPlayersSlider.value << MaxPlayers_Shift );

	lpDesc->dwUser4 |= ( PacketsSlider.value << PacketsPerSecond_Shift );
}

/*
 * DPlayCreateSession
 *
 * Wrapper for DirectPlay CreateSession API.Uses the global application guid (PROJX_GUID).
 */
HRESULT DPlayCreateSession(LPTSTR lptszSessionName)
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

	StoreSessionUserFields( &dpDesc );

	Old_Session = dpDesc;
	Old_WhoIAm = 0;
	Old_Kills = 0;
	Old_Deaths = 0;

    dpDesc.lpszSessionNameA = lptszSessionName;

    // set the application guid
	dpDesc.guidApplication = PROJX_GUID;

    if (glpDP)
        hr = IDirectPlayX_Open(glpDP, &dpDesc, DPOPEN_CREATE);
	else
		DebugPrintf("DPlayCreateSession: !glpDP Could not create direct play session.");

    return hr;
}

/*
 * DPlayDestroyPlayer
 * 
 * Wrapper for DirectPlay DestroyPlayer API. 
 */
HRESULT DPlayDestroyPlayer(DPID pid)
{
    HRESULT hr=E_FAIL;
    
    if (glpDP)
        hr = IDirectPlayX_DestroyPlayer(glpDP, pid);

	ShutdownConnection();

    return hr;
}

/*
 * DPlayEnumPlayers
 *
 * Wrapper for DirectPlay API EnumPlayers
 */
HRESULT DPlayEnumPlayers(LPGUID lpSessionGuid, LPDPENUMPLAYERSCALLBACK2 lpEnumCallback, 
                         LPVOID lpContext, DWORD dwFlags)
{
    HRESULT hr=E_FAIL;

    if (glpDP)
        hr = IDirectPlayX_EnumPlayers(glpDP, lpSessionGuid, lpEnumCallback, lpContext, dwFlags);

    return hr;
}

/*
 * DPlayEnumSessions
 *
 * Wrapper for DirectPlay EnumSessions API.
 */

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
		Msg("%s\n%s",
			"Cannot enumerate sessions."
			"DirectPlay Lobby Interface Missing!");
	}

	// return result
    return hr;
}

/*
 * DPlayGetPlayerData
 * 
 * Wrapper for DirectPlay GetPlayerData API.
 */
HRESULT DPlayGetPlayerData(DPID pid, LPVOID lpData, LPDWORD lpdwDataSize, DWORD dwFlags)
{
    HRESULT hr=E_FAIL;

    if (glpDP) 
        hr = IDirectPlayX_GetPlayerData(glpDP, pid, lpData, lpdwDataSize, dwFlags);

    return hr;
}

/*
 * DPlayGetSessionDesc
 *
 * Wrapper for DirectPlay GetSessionDesc API. 
 */
HRESULT DPlayGetSessionDesc(void)
{
    HRESULT hr=E_FAIL;
    DWORD dwSize;


    // free old session desc, if any
    if (glpdpSD)
    {
        free(glpdpSD);
        glpdpSD = NULL;
    }

	if (glpDP)
    {
#ifdef MANUAL_SESSIONDESC_PROPAGATE
		if ( glpdpSD_copy )
		{
			glpdpSD = (LPDPSESSIONDESC2) malloc( sizeof( *glpdpSD_copy ) );
			if ( glpdpSD )
			{
				*glpdpSD = *glpdpSD_copy;
				return DP_OK;
			}else
			{
				return E_OUTOFMEMORY; 
			}
		}
#endif
		// first get the size for the session desc
        if ((hr = IDirectPlayX_GetSessionDesc(glpDP, NULL, &dwSize)) == DPERR_BUFFERTOOSMALL)
        {
			// allocate memory for it
            glpdpSD = (LPDPSESSIONDESC2) malloc(dwSize);
            if (glpdpSD)
            {
                // now get the session desc
                hr = IDirectPlayX_GetSessionDesc(glpDP, glpdpSD, &dwSize);
            }
            else
            {
                hr = E_OUTOFMEMORY;
            }
        }
    }

    return hr;
}

void UpdateSessionName( char *name )
{
	DPSESSIONDESC2 tempsd;

	DPlayGetSessionDesc();

	if ( glpdpSD )
	{
		tempsd = *glpdpSD;
		glpdpSD->lpszSessionNameA = name;
		DPlaySetSessionDesc( 1 );	// 1 ensures new session desc is sent to all players via guaranteed msg

		*glpdpSD = tempsd;	// restore old SD so that it is freed properly.
		DPlayGetSessionDesc(); // getting new SD will free up old one
	}
}

/*
 * DPlaySetSessionDesc
 *
 * Wrapper for DirectPlay SetSessionDesc API. 
 */
HRESULT DPlaySetSessionDesc(DWORD flags)
{
    HRESULT hr=E_FAIL;

    if (glpDP && glpdpSD)
    {
#ifdef MANUAL_SESSIONDESC_PROPAGATE
		if ( glpdpSD_copy )
		{
			free ( glpdpSD_copy );
			glpdpSD_copy = NULL;
		}
#endif
		
		// now set the session desc
		hr = IDirectPlayX_SetSessionDesc(glpDP, glpdpSD, /*flags*/0 );
    }

#ifdef MANUAL_SESSIONDESC_PROPAGATE

	if ( hr == DP_OK )
	   	if ( !flags )	// lazy way of preventing messages, since no flags are currently defined
						// for IDirectPlayX::SetSessionDesc !!
			SendGameMessage( MSG_SESSIONDESC, 0, 0, 0, 0 );	

#endif

	if ( hr != DP_OK )
		switch ( hr )
		{
		case DPERR_ACCESSDENIED:
			DebugPrintf("DPlaySetSessionDesc() error - DPERR_ACCESSDENIED\n");
			break;
		case DPERR_INVALIDPARAMS:
			DebugPrintf("DPlaySetSessionDesc() error - DPERR_INVALIDPARAMS\n");
			break;
		case DPERR_NOSESSIONS:
			DebugPrintf("DPlaySetSessionDesc() error - DPERR_NOSESSIONS\n");
			break;
		default:
			DebugPrintf("DPlaySetSessionDesc() error - unknown\n");
		}

    return hr;
}

/*
 * IsDPlay
 *
 * Returns TRUE if a DirectPlay interface exists, otherwise FALSE.
 */
BOOL IsDPlay(void)
{
    return (glpDP ? TRUE:FALSE);
}

/*
 * DPlayOpenSession
 *
 * Wrapper for DirectPlay OpenSession API. 
 */
HRESULT DPlayOpenSession(LPGUID lpSessionGuid)
{
    HRESULT hr = E_FAIL;
    DPSESSIONDESC2 dpDesc;

    ZeroMemory(&dpDesc, sizeof(dpDesc));
    dpDesc.dwSize = sizeof(dpDesc);

    // set the session guid
    if (lpSessionGuid)
        dpDesc.guidInstance = *lpSessionGuid;

    // set the application guid
    dpDesc.guidApplication = PROJX_GUID;

    // open it
    if (glpDP)
        hr = IDirectPlayX_Open(glpDP, &dpDesc, DPOPEN_JOIN);
	else
		DebugPrintf("DPlayOpenSession: could not join !glpDP\n");

    return hr;
}



/*
 * DPlayRelease
 *
 * Wrapper for DirectPlay Release API.
 */
HRESULT DPlayRelease(void)
{
    HRESULT hr = E_FAIL;

	// flush all waiting guaranteed messages
	ProcessGuaranteedMessages( TRUE , FALSE , FALSE );

    if (glpDP != NULL)
    {
        // free session desc, if any
//        if (glpdpSD) 
//        {
//            free(glpdpSD);
//            glpdpSD = NULL;
//        }

// WINEBUG
// game freezes right here...

        // release dplay
        hr = IDirectPlayX_Release(glpDP);
        glpDP = NULL;
    }

//	if( lpDPlayLobby )
//	{
//		lpDPlayLobby->lpVtbl->Release(lpDPlayLobby);
//		lpDPlayLobby = NULL;
//	}

    return hr;
}

/*
 * DPlaySetPlayerData
 *
 * Wrapper for DirectPlay SetPlayerData API
 */
HRESULT DPlaySetPlayerData(DPID pid, LPVOID lpData, DWORD dwSize, DWORD dwFlags)
{
    HRESULT hr=E_FAIL;

    if (glpDP)
        hr = IDirectPlayX_SetPlayerData(glpDP, pid, lpData, dwSize, dwFlags);
    
    return hr;
}

/*
 * DPlay Create Lobby interface...
 *
 * Wrapper for DirectPlay Lobby Create API
 */
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



/*
 * DPlay Create a sevice provider address...
 *
 * Wrapper for DirectPlay CreateCompoundAddress API
 */

HRESULT	CreateServiceProviderAddress( LPGUID lpGuid ,LPDIRECTPLAYLOBBY2A lpDPlayLobby, LPVOID *lplpAddress, LPDWORD lpdwAddressSize , char * TCPIPAddress)
{
	DPCOMPOUNDADDRESSELEMENT	addressElements[3];
	LPVOID						lpAddress = NULL;
	DWORD						dwAddressSize = 0;
	DWORD						dwElementCount;
	HRESULT						hr;

	dwElementCount = 0;

	// internet TCP/IP service provider
	if (IsEqualGUID(lpGuid, &DPSPGUID_TCPIP))
	{
		// TCP/IP needs a service provider and an IP address

		// service provider
		addressElements[dwElementCount].guidDataType = DPAID_ServiceProvider;
		addressElements[dwElementCount].dwDataSize = sizeof(GUID);
		addressElements[dwElementCount].lpData = (LPVOID) &DPSPGUID_TCPIP;
		dwElementCount++;

		// This is where you would fill in the IP Address..
		addressElements[dwElementCount].guidDataType = DPAID_INet;
		addressElements[dwElementCount].dwDataSize = lstrlen(TCPIPAddress) + 1;
		addressElements[dwElementCount].lpData = (LPVOID) TCPIPAddress;
		dwElementCount++;
	}

	// IPX service provider
	else if (IsEqualGUID(lpGuid, &DPSPGUID_IPX))
	{
		// IPX just needs a service provider

		// service provider
		addressElements[dwElementCount].guidDataType = DPAID_ServiceProvider;
		addressElements[dwElementCount].dwDataSize = sizeof(GUID);
		addressElements[dwElementCount].lpData = (LPVOID) &DPSPGUID_IPX;
		dwElementCount++;
	}

	// see how much room is needed to store this address
	hr = lpDPlayLobby->lpVtbl->CreateCompoundAddress(lpDPlayLobby,
						addressElements, dwElementCount,
						NULL, &dwAddressSize);

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
	*lpdwAddressSize = dwAddressSize;

	return (DP_OK);

FAILURE:
	if (lpAddress)
	{
		free(lpAddress);
		lpAddress = NULL;
	}
	return (hr);
}


/*
 * DPlay Once a Sevice Provider is chosen then create the DirectPlay Interfence
 *
 * Wrapper for DirectPlay CreateCompoundAddress API
 */
HRESULT	OnceServiceProviderChosen( LPGUID lpGuid ,LPDIRECTPLAYLOBBY2A lpDPlayLobby, LPDIRECTPLAY4A *lplpDPlay , char * TCPIPAddress)
{
	LPDIRECTPLAY4A	lpDPlay = NULL;
	LPVOID			lpAddress = NULL;
	DWORD			dwAddressSize = 0;
	HRESULT			hr;

	// bail if we don't have a lobby interface
	if (lpDPlayLobby == NULL)
		return (DPERR_INVALIDOBJECT);

	// get service provider address from information in dialog
	if (FAILED(
		  CreateServiceProviderAddress( lpGuid, lpDPlayLobby, &lpAddress,
		                                &dwAddressSize, TCPIPAddress )))
		goto FAILURE;

	// if direct play interface exists
	if (*lplpDPlay)
	{
		// release it
		(*lplpDPlay)->lpVtbl->Release(*lplpDPlay);
		*lplpDPlay = NULL;
	}

	// create a DirectPlay ANSI interface
	if FAILED(
		CoCreateInstance(
			&CLSID_DirectPlay,
			NULL,
			CLSCTX_INPROC_SERVER,
			&IID_IDirectPlay4A,
			(LPVOID*)&lpDPlay
			))
		goto FAILURE;

	// initialize the connection using the address
	hr = lpDPlay->lpVtbl->InitializeConnection(lpDPlay, lpAddress, 0);
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

		goto FAILURE;
	}

	// return the connected interface
	*lplpDPlay = lpDPlay;

	// set to NULL so we don't release it below
	lpDPlay = NULL;

FAILURE:
	if (lpDPlay)
	{
		lpDPlay->lpVtbl->Close(lpDPlay);
		lpDPlay->lpVtbl->Release(lpDPlay);
	}
	if( lpAddress )
	{
		free(lpAddress);
	}
	return (hr);
}

/*
 * DPLobbyRelease
 *
 * Wrapper for DirectPlayLobby Release API
 */
HRESULT DPLobbyRelease(void)
{
	HRESULT hr=E_FAIL;

	// free our connection settings
	if (glpdplConnection)
	{
		free(glpdplConnection);
		glpdplConnection = NULL;
	}

	// release the lobby object
	if (lpDPlayLobby)
	{
		hr = IDirectPlayLobby_Release(lpDPlayLobby);
		lpDPlayLobby = NULL;
	}
	return hr;
}

/*
 * DPlaySetPlayerName
 *
 * Wrapper for DirectPlay SetPlayerName API
 */
HRESULT DPlaySetPlayerName(DPID pid, char * NamePnt, DWORD dwFlags)
{
    HRESULT hr=E_FAIL;
	DPNAME	Name;

    if (glpDP)
	{
		memset(&Name, 0, sizeof(DPNAME));
		Name.dwSize = sizeof(DPNAME);
		Name.lpszShortNameA = NamePnt;
        Name.lpszLongNameA = NamePnt;
        hr = IDirectPlayX_SetPlayerName(glpDP, pid, &Name, dwFlags);
	}
    return hr;
}

BOOL GetIPFromDP( char *add, DPID dpid )
{
    char* pTrueAddress;
    DWORD   dwTempSize;
    DWORD   dwIndex;
    DWORD   dwInetSize;
	char *abyTemp;
	HRESULT hr;
	char buf[ 128 ];

    dwIndex = 0;
    pTrueAddress = NULL;

    // get size required for address
	hr = IDirectPlayX_GetPlayerAddress( glpDP, dpid, NULL, &dwTempSize );
	if ( ( hr != DP_OK ) && ( hr != DPERR_BUFFERTOOSMALL ) )
	{
		switch ( hr )
		{
		case DPERR_INVALIDOBJECT :
			strcpy( buf, "DPERR_INVALIDOBJECT" );
			break;
		case DPERR_INVALIDPARAMS :
			strcpy( buf, "DPERR_INVALIDPARAMS" );
			break;
		case DPERR_INVALIDPLAYER :
			strcpy( buf, "DPERR_INVALIDPLAYER" );
			break;
		default:
			strcpy( buf, "unknown error" );
			break;
		}
		DebugPrintf("GetIPFromDP() IDirectPlayXA_GetPlayerAddress cannot get address size ( %s )\n", buf );
		return FALSE;
	}

	// malloc space for address buffer
	abyTemp = ( char * )malloc( dwTempSize );
	if ( !abyTemp )
	{
	 	DebugPrintf("GetIPFromDP() malloc failed\n");
		return FALSE;
	}

    hr = IDirectPlayX_GetPlayerAddress( glpDP, dpid, (void *)abyTemp, &dwTempSize );
	if ( hr != DP_OK )
	{
	 	DebugPrintf("IDirectPlayXA_GetPlayerAddress failed\n");
		return FALSE;
	}

    else
    {    
	    while ( dwIndex < dwTempSize )
        {        
			if ( ! memcmp( abyTemp+dwIndex , &DPAID_INet , sizeof(GUID) ) )
            {
				dwInetSize   = * (  (DWORD*) (abyTemp + dwIndex + sizeof(GUID))  );
	            pTrueAddress = abyTemp + dwIndex + sizeof(GUID) + sizeof(DWORD);

		        if ( '\0' == *pTrueAddress )
				{
				    DebugPrintf("GetIPFromDP() Can't get true address.\n");
					return FALSE;
				}
		        else
				{
				    DebugPrintf( "GetIPFromDP() True address %s\n", pTrueAddress );
				}
			    break;
            }
	        else
            {
		        //NOTE: This is a hack, should truly move along on a chunk by chunk basis...
			    dwIndex++;    
			}
        }
    }
	
	strncpy( add, pTrueAddress, 16 );

	return TRUE;
}

