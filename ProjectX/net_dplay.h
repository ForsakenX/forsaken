
#define IDIRECTPLAY2_OR_GREATER
#include "typedefs.h"
#include <dplay.h>

/* still required to enumerate sessions */
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

HRESULT DPlaySetPlayerName(DPID pid, char * Name, DWORD dwFlags);

void StoreSessionUserFields( LPDPSESSIONDESC2 lpDesc );
BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );


