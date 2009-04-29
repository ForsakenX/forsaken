#ifndef NET_INCLUDED
#define NET_INCLUDED

/*
 *  Network State
 */

typedef enum {
	NETWORK_CONNECTED,
	NETWORK_CONNECTING,
	NETWORK_DISCONNECTED,
} network_state_t;

network_state_t network_state;

/*
 *  Main Functions
 */

// 0 for default port
int  network_setup( char* player_name, int local_port ); // must be called first
int  network_join( char* address, int port );
void network_host();

void network_pump();	// process network routines, fire events, marshal packets
void network_cleanup();	// stop and cleanup networking

/*
 *  Players
 */

#define NETWORK_MAX_NAME_LENGTH 15
#define INET_ADDRSTRLEN 16

typedef struct _network_player_t network_player_t;

struct  _network_player_t {
	char name[ NETWORK_MAX_NAME_LENGTH ]; // 14 chars for player name + 1 /0
	char ip[INET_ADDRSTRLEN];
	int  port;
	long int ping;
	long int packet_loss;
	long int bw_in;
	long int bw_out;
	network_player_t * prev;
	network_player_t * next;
	void* data; // internal use only
};

typedef struct {
	int length;
	network_player_t * first;
	network_player_t * last;
} network_players_t;

network_players_t network_players;

void network_set_player_name( char* name );

/*
 *  Sending Data
 */

typedef enum { // default is unsequenced and unreliable
	NETWORK_RELIABLE	= 2, // note: enet reliable is also sequenced
	NETWORK_SEQUENCED	= 4,
	NETWORK_FLUSH		= 6, // flushes the current packet
} network_flags_t;

// channel 0 is reserved
// data is copied... so you can free it right after function call
void network_send( network_player_t*, void* data, int size, network_flags_t flags, int channel );
void network_broadcast( void* data, int size, network_flags_t flags, int channel );

/*
 *  Network Events
 */

typedef enum { // data = struct network_player_t or NULL (local player)
	NETWORK_JOIN	= 2,
	NETWORK_LEFT	= 4,
	NETWORK_HOST	= 6,
	NETWORK_NAME	= 8, // player changed name
	NETWORK_DATA	= 10, // data = network_packet_t
} network_event_type_t;

typedef struct {
	int size;
	void* data;
	network_player_t* from;
} network_packet_t;

void network_event( network_event_type_t, void* data ); // implemented by application, see network_event for explanation of (void*)data

#endif // NET_INCLUDED