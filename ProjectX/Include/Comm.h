
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


