#ifndef NET_DPLAY_INCLUDED
#define NET_DPLAY_INCLUDED

LPDPSESSIONDESC2 network_session;

void network_get_ip( char *add, DPID dpid );

void network_session_name( char *name );
void network_get_session( void );
void network_initialize( char * TCPAddress );
void network_pump( void );
void network_set_player_name(DPID pid, char * NamePnt);
void network_cleanup( DPID dcoID );
void network_send( DPID to, void* data, DWORD size, int guaranteed );

HRESULT network_host( char* name );
HRESULT network_open_session(void);
HRESULT network_create_player( LPDPID lppidID, LPTSTR lpPlayerName );
HRESULT network_get_description(void);
HRESULT network_set_description(void);

#endif // NET_DPLAY_INCLUDED