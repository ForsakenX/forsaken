/*
 * The X Men, October 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 14 $
 *
 * $Header: /PcProjectX/include/Comm.h 14    14/08/98 15:25 Philipy $
 *
 * $Log: /PcProjectX/include/Comm.h $
 * 
 * 14    14/08/98 15:25 Philipy
 * added trilinear option to menus
 * fixed level name / shutdown packet in heartbeat
 * 
 * 
 * 13    14/08/98 9:13 Phillipd
 * DirectX6 is in effect....
 * 
 * 12    3/07/98 11:55 Philipy
 * heartbeat & quickstart stuff
 * 
 * 11    20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 10    4/27/98 4:02p Phillipd
 * 
 * 9     6/03/98 17:37 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 8     12/10/97 10:39a Phillipd
 * 
 * 7     9/30/97 4:21p Phillipd
 * 
 * 6     9/30/97 8:46a Phillipd
 * 
 * 5     9/29/97 11:56a Phillipd
 * 
 * 4     6/14/97 1:09p Phillipd
 * 
 * 3     4/25/97 2:51p Phillipd
 * 
 * 2     2/07/97 4:59p Phillipd
 * 
 * 1     10/10/96 3:09p Phillipd
 * First SS update fo some of this...
 * DirectPlay 2 here we go.....
 */

/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       comm.h
 *  Content:    communication routines include file
 *
 *
 ***************************************************************************/
#define IDIRECTPLAY2_OR_GREATER
#include "typedefs.h"
#include <dplay.h>
#include <dplobby.h>

/*
 * Prototypes
 */
HRESULT DPlayClose(void);
HRESULT DPlayCreate(LPGUID lpGuid);
HRESULT DPlayCreatePlayer(LPDPID lppidID, LPTSTR lpPlayerName, HANDLE hEvent, 
                          LPVOID lpData, DWORD dwDataSize);
HRESULT DPlayCreateSession(LPTSTR lptszSessionName);
HRESULT DPlayDestroyPlayer(DPID pid);
HRESULT DPlayEnumPlayers(LPGUID lpSessionGuid, LPDPENUMPLAYERSCALLBACK2 lpEnumCallback, 
                         LPVOID lpContext, DWORD dwFlags);
HRESULT DPlayEnumSessions(DWORD dwTimeout, LPDPENUMSESSIONSCALLBACK2 lpEnumCallback, 
                          LPVOID lpContext, DWORD dwFlags);
HRESULT DPlayGetPlayerData(DPID pid, LPVOID lpData, LPDWORD lpdwDataSize, DWORD dwFlags);
HRESULT DPlayGetSessionDesc(void);
BOOL    IsDPlay(void);
HRESULT DPlayOpenSession(LPGUID lpSessionGuid);
HRESULT DPlayRelease(void);
HRESULT DPlaySetPlayerData(DPID pid, LPVOID lpData, DWORD dwSize, DWORD dwFlags);
HRESULT DPlaySetSessionDesc(DWORD flags);


HRESULT DPlayCreateLobby( void );
HRESULT	CreateServiceProviderAddress( LPGUID lpGuid ,LPDIRECTPLAYLOBBY2A lpDPlayLobby, LPVOID *lplpAddress, LPDWORD lpdwAddressSize , char * TCPIPAddress);
HRESULT	OnceServiceProviderChosen( LPGUID lpGuid ,LPDIRECTPLAYLOBBY2A lpDPlayLobby, LPDIRECTPLAY4A *lplpDPlay , char * TCPIPAddress);


HRESULT DPLobbyGetConnectionSettings(void);
HRESULT DPLobbyRelease(void);
HRESULT DPLobbySetConnectionSettings(void);
HRESULT DPLobbyConnect(void);
HRESULT DPlaySetPlayerName(DPID pid, char * Name, DWORD dwFlags);
void ContinueLobbyLaunch( void );
BOOL LaunchedByLobby(void);

HRESULT DPlayCreateSessionServer( LPTSTR lptszSessionName );
HRESULT DPlayCreatePlayerServer(LPDPID lppidID, LPTSTR lptszPlayerName, HANDLE hEvent, 
		                        LPVOID lpData, DWORD dwDataSize);

void StoreSessionUserFields( LPDPSESSIONDESC2 lpDesc );
BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );


