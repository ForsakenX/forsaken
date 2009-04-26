#ifndef NET_DPLAY_INCLUDED
#define NET_DPLAY_INCLUDED

int network_ready( void );

void network_initialize( char * TCPAddress );
void network_pump( void );
void network_set_player_name(DPID pid, char * NamePnt);
void network_cleanup();
void network_send( DPID to, void* data, DWORD size, int guaranteed );

HRESULT network_host( char* name );
HRESULT network_join(void);
HRESULT network_create_player( LPDPID lppidID, LPTSTR lpPlayerName );
HRESULT network_get_player_name( DPID id, char* name );

#endif // NET_DPLAY_INCLUDED