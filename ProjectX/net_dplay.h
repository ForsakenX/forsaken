
#define IDIRECTPLAY2_OR_GREATER
#include "typedefs.h"
#include <dplay.h>

/* still required to enumerate sessions */
#include <dplobby.h>

/*
 * Prototypes
 */
HRESULT DPlayClose( LPDIRECTPLAY4A dp );
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
HRESULT DPlayOpenSession(LPGUID lpSessionGuid);
HRESULT DPlayRelease(void);
HRESULT DPlaySetPlayerData(DPID pid, LPVOID lpData, DWORD dwSize, DWORD dwFlags);
HRESULT DPlaySetSessionDesc(DWORD flags);

BOOL SetupDPlay( char * TCPAddress );
HRESULT DPlayCreateLobby( void );

HRESULT DPlaySetPlayerName(DPID pid, char * Name, DWORD dwFlags);

void StoreSessionUserFields( LPDPSESSIONDESC2 lpDesc );
BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );


