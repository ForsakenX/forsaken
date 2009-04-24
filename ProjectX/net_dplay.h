#ifndef NET_DPLAY_INCLUDED
#define NET_DPLAY_INCLUDED

/*
 * Prototypes
 */
HRESULT DPlayClose( LPDIRECTPLAY4A dp );
HRESULT DPlayCreatePlayer(LPDPID lppidID, LPTSTR lpPlayerName, HANDLE hEvent, 
                          LPVOID lpData, DWORD dwDataSize);
HRESULT DPlayCreateSession(LPTSTR lptszSessionName);
HRESULT DPlayDestroyPlayer(DPID pid);
HRESULT DPlayEnumSessions(DWORD dwTimeout, LPDPENUMSESSIONSCALLBACK2 lpEnumCallback, 
                          LPVOID lpContext, DWORD dwFlags);
HRESULT DPlayGetPlayerData(DPID pid, LPVOID lpData, LPDWORD lpdwDataSize, DWORD dwFlags);
HRESULT DPlayGetSessionDesc(void);
HRESULT DPlayOpenSession(LPGUID lpSessionGuid);
HRESULT DPlayRelease(void);
HRESULT DPlaySetPlayerData(DPID pid, LPVOID lpData, DWORD dwSize, DWORD dwFlags);
HRESULT DPlaySetSessionDesc(DWORD flags);

LPDPSESSIONDESC2 lpDPlaySession;
void GetCurrentSessions( void );

BOOL SetupDPlay( char * TCPAddress );
HRESULT DPlayCreateLobby( void );

HRESULT DPlaySetPlayerName(DPID pid, char * Name, DWORD dwFlags);

void StoreSessionUserFields( LPDPSESSIONDESC2 lpDesc );
BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );

void network_pump( void );

#endif // NET_DPLAY_INCLUDED