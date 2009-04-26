#ifndef NET_INCLUDED
#define NET_INCLUDED

typedef DWORD network_id_t;

int network_ready( void );
int network_get_player_name( network_id_t id, char* name );
int network_host( char* name );
int network_join(void);
int network_create_player( network_id_t * id, LPTSTR lpPlayerName );

void network_initialize( char * TCPAddress );
void network_pump( void );
void network_set_player_name( network_id_t id, char * NamePnt );
void network_cleanup();
void network_send( network_id_t to, void* data, DWORD size, int guaranteed );

extern void network_event_new_message( network_id_t from, BYTE * MsgPnt, DWORD nBytes );
extern void network_event_new_player( network_id_t id, char * player_name );
extern void network_event_i_am_host( void );
extern void network_event_destroy_player( network_id_t id );
extern void network_event_player_name( network_id_t id, char* name );

#endif // NET_INCLUDED