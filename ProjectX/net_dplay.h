#ifndef NET_DPLAY_INCLUDED
#define NET_DPLAY_INCLUDED

HRESULT DPlayCreatePlayer(LPDPID lppidID, LPTSTR lpPlayerName, HANDLE hEvent, 
                          LPVOID lpData, DWORD dwDataSize);
HRESULT DPlayCreateSession(LPTSTR lptszSessionName);
HRESULT DPlayDestroyPlayer(DPID pid);
HRESULT DPlayGetSessionDesc(void);
HRESULT DPlayOpenSession(LPGUID lpSessionGuid);
HRESULT DPlayRelease(void);
HRESULT DPlaySetSessionDesc(DWORD flags);

LPDPSESSIONDESC2 lpDPlaySession;
void GetCurrentSessions( void );

BOOL SetupDPlay( char * TCPAddress );

HRESULT DPlaySetPlayerName(DPID pid, char * Name, DWORD dwFlags);

BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );

void network_pump( void );

#endif // NET_DPLAY_INCLUDED