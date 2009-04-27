#ifndef NET_INCLUDED
#define NET_INCLUDED

/*
 *  NETWORKING API
 */

typedef DWORD network_id_t; // no need for this to be so large after new network is done

int network_setup( char* player_name, int local_port );
int network_join( char * address, int remote_port );
int network_host();
void network_pump();
void network_cleanup();

typedef enum {
	NETWORK_RELIABLE	= 2,
	NETWORK_SEQUENCED	= 4,
	NETWORK_FLUSH		= 6,
} network_flags_t;

void network_send( network_id_t to, void* data, int size, network_flags_t flags, int channel );

// events defined by application

extern void network_event_new_message( network_id_t from, BYTE * message, int size );
extern void network_event_new_host( network_id_t id );
extern void network_event_player_joined( network_id_t id, char * player_name );
extern void network_event_player_left( network_id_t id );

/*
 *  DPLAY stuff that will not be needed later
 */

int network_retry();
int network_open();

// player names will be migrated to connection/disconnection events after enet is implemented
int network_get_player_name( network_id_t id, char* name );
void network_set_player_name( char* name );
extern void network_event_player_name( network_id_t id, char* name );


#endif // NET_INCLUDED