#ifndef NET_DPLAY_INCLUDED
#define NET_DPLAY_INCLUDED

HRESULT DPlayCreateSession(LPTSTR lptszSessionName);
HRESULT DPlayDestroyPlayer(DPID pid);
HRESULT DPlayGetSessionDesc(void);
HRESULT DPlayRelease(void);
HRESULT DPlaySetSessionDesc(DWORD flags);

LPDPSESSIONDESC2 lpDPlaySession;
void GetCurrentSessions( void );

BOOL SetupDPlay( char * TCPAddress );

BOOL GetIPFromDP( char *add, DPID dpid );
void UpdateSessionName( char *name );

void network_pump( void );
HRESULT network_open_session(void);
HRESULT network_create_player( LPDPID lppidID, LPTSTR lpPlayerName );
void network_set_player_name(DPID pid, char * NamePnt);

#endif // NET_DPLAY_INCLUDED