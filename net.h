/*
 *
 * Copyright (C) 2009  Daniel Aquino
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>, or write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#ifndef NET_INCLUDED
#define NET_INCLUDED

#define NETWORK_DEFAULT_PORT 2300

typedef enum {
	NETWORK_OK,			// success
	NETWORK_ERROR_INIT,	// failed to initialize
	NETWORK_ERROR_BIND	// failed to bind socket
} network_return_t;

/*
 *  Network State
 */

typedef enum {
	NETWORK_CONNECTED,		// in the game
	NETWORK_CONNECTING,		// connecting to host
	NETWORK_DISCONNECTED,	// not connect yet
	NETWORK_SYNCHING,		// connecting to all players
	NETWORK_SYNCH_FAILED,	// failed to synch with other players
} network_state_t;

extern network_state_t network_state;

/*
 *  Main Functions
 */

// 0 for default port
network_return_t network_setup( char* player_name, int local_port ); // must be called first
int  network_join( char* address, int port );
void network_host();

void network_pump();	// process network routines, fire events, marshal packets
void network_cleanup();	// stop and cleanup networking

/*
 *  Players
 */

#define NETWORK_MAX_NAME_LENGTH 15
#define INET_ADDRSTRLEN 16 // 255.255.255.255\0

typedef struct _network_player_t network_player_t;

struct  _network_player_t {
	char name[ NETWORK_MAX_NAME_LENGTH ]; // 14 chars for player name + 1 /0
	char ip[INET_ADDRSTRLEN];
	int  port;
	unsigned long int ping;
	unsigned long int packet_loss;
	unsigned long int packets_lost;
	unsigned long int bw_in;
	unsigned long int bw_out;
	network_player_t * prev;
	network_player_t * next;
	void* data; // internal use only
};

typedef struct {
	int length;
	network_player_t * first;
	network_player_t * last;
} network_players_t;

extern network_players_t network_players;

void network_set_player_name( char* name );

/*
 *  Sending Data
 */

// FLAGS

// unsequenced  + unreliable	= is the default
// reliable     + unsequenced	= is not supported
// reliable                   = always sequenced
// sequenced    + unreliable	= will drop late pkts & instantly deliver arrived pkts (position updates)
// sequenced	  + reliable		= will wait for late pkts to arive

// CHANNELS (STREAMS)

// note the word "and" here means different types of packets mixed on a single stream
// the "+" symbol means a single packet with those flags set

// sequenced                  = will take part in the same sequencing order on a channel
// unreliable and unsequenced = all on their own
// unreliable + sequenced     = will get dropped if they are late...
// unreliable and sequenced   = packets will respect sequencing and reliability always

// NOTES

// no need to over channalize 
// use different channels for different streams

typedef enum {
	NETWORK_RELIABLE	= 2, // note: reliable is also sequenced
	NETWORK_SEQUENCED	= 4, //
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

// this function will be most likely converted to an argument to network_setup
void network_event( network_event_type_t, void* data );

#endif // NET_INCLUDED
