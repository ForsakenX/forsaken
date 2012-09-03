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

#ifdef NET_ENET_2
#include "main.h"
#include "net.h"
#include "enet/enet.h"
#include <stdio.h>
#include <string.h>

// debug print f
#include "util.h"

// id stuff
typedef unsigned char peer_id_t;
#define NO_ID 0
#define LOWEST_ID 1
#define HIGHEST_ID 255

/*
 *
 *  settings
 *
 */

// state flags
static int initialized;
ENetHost* enet_host = NULL; // used in net_tracker.c

// the host player
static ENetPeer* host;

// net.h globals
network_players_t network_players;
network_state_t network_state;

// my settings
static int i_am_host = 0;
peer_id_t my_id = NO_ID; // used by nettest
char my_player_name[NETWORK_MAX_NAME_LENGTH] = {0}; // used in net_tracker.c
int my_local_port = 0; // used in net_tracker.c

// settings
static int max_peers = 40;
static int max_channels = 50;
static int system_channel = 0;

/*
 *
 * Debug Helpers
 *
 */

// returns host:port syntax copy data right away as ip is overwritten next call
static char* address_to_str( ENetAddress * address )
{
	char temp[INET_ADDRSTRLEN+10] = "";
	static char ip[INET_ADDRSTRLEN+10] = "";
	enet_address_get_host_ip( address, temp, INET_ADDRSTRLEN );
	sprintf( ip, "%s:%d", temp, address->port );
	return ip;
}

/*
 *
 *  peer helpers
 *
 */

typedef enum {
	PLAYING,    // peer is valid player in game
	UNUSED,     // unused peer
	CONNECTED,  // this must match other enum which defines CONNECTED as well
	CONNECTING, // host told us to connect
	ACKING,     // waiting for peer to ack my id
	SYNCHING,   // host telling everyone to synch
} network_peer_state_t;

// peer->data = 
typedef struct {
	peer_id_t id;
	network_peer_state_t state;
	network_player_t * player;
	int connect_port;
	ENetPeer ** connected_peers; // used by host to keep track of connections
} network_peer_data_t;

#define PEER_USED( peer )\
	( ( (network_peer_data_t*) (peer)->data )->state != UNUSED )

#define PEER_CONNECT_PORT( peer )\
	( (network_peer_data_t*) (peer)->data )->connect_port

#define PEER_STATE( peer )\
	( (network_peer_data_t*) (peer)->data )->state

#define PEER_ID( peer )\
	( (network_peer_data_t*) (peer)->data )->id

#define PLAYER_ID( player )\
	PEER_ID( ( (ENetPeer*) (player)->data ) )

static void init_connected_list( network_peer_data_t * peer_data )
{
	int x;
	for( x = 0; x < max_peers; x++ )
		peer_data->connected_peers[x] = NULL;
}

static void init_peer( ENetPeer * peer )
{
	network_peer_data_t * data = peer->data;
	if( data == NULL ) // first init
	{
		peer->data = malloc( sizeof(network_peer_data_t) );
		data = peer->data;
		data->connected_peers = malloc( sizeof(void*) * max_peers );
	}
	init_connected_list( data );
	data->state = UNUSED;
	data->player = NULL;
	data->connect_port = 0;
	data->id = NO_ID;
}

static void init_peers( void )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
		init_peer( &enet_host->peers[x] );
}

static void cleanup_peer( ENetPeer * peer )
{
	network_peer_data_t * data = peer->data;
	if( ! data ) return;
	if( data->connected_peers )
	{
		free( data->connected_peers );
		data->connected_peers = NULL;
	}
	free( data );
	peer->data = NULL;
}

static void cleanup_peers( void )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
		cleanup_peer( &enet_host->peers[x] );
}

static ENetPeer * find_peer_by_id( peer_id_t id )
{
	size_t x;
	if(!id) return NULL;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		if( PEER_USED(peer) && PEER_ID(peer) == id )
			return peer;
	}
	return NULL;
}

static peer_id_t find_free_id( void )
{
	peer_id_t id;
	static peer_id_t last = LOWEST_ID;
	if( last == HIGHEST_ID )
		last = LOWEST_ID;
	last++;
	for( id = last; id <= HIGHEST_ID; id++ )
	{
		if( id != my_id && ! find_peer_by_id( id ) )
		{
			last = id;
			return id;
		}
	}
	return NO_ID;
}

/*
 *
 *  enet helpers
 *
 */

static void enet_cleanup( void )
{
	initialized = 0;
	enet_host_destroy( enet_host );
	enet_host = NULL;
	enet_deinitialize();
	DebugPrintf("network: enet cleanup finished\n");
}

static network_return_t enet_setup( char* str_address, int port )
{
	ENetAddress address;

	if ( ! initialized )
	{
		if (enet_initialize() != 0)
		{
			return NETWORK_ERROR_INIT;
		}
		initialized = 1;
	}
	
	DebugPrintf("network: enet setup initialized\n");

	if ( ! str_address ) address.host = ENET_HOST_ANY;
	else enet_address_set_host( &address, str_address ); 

	address.port = (port) ? port : NETWORK_DEFAULT_PORT;
	my_local_port = address.port;

	DebugPrintf("network: enet setup address %s\n",
		address_to_str(&address));

	enet_host = enet_host_create( &address, max_peers, 0, 0 );

	if ( enet_host == NULL )
	{
		return NETWORK_ERROR_BIND;
	}

	DebugPrintf("network: enet setup initializing peers\n");

	init_peers();

	DebugPrintf("network: enet setup finished\n");

	return NETWORK_OK;
}

static int enet_connect( char* str_address, int port )
{
	ENetAddress address;
	ENetPeer* peer;
	network_peer_data_t * peer_data;

	if ( ! str_address ) address.host = ENET_HOST_ANY;
	else enet_address_set_host( &address, str_address ); 

	address.port = (port) ? port : NETWORK_DEFAULT_PORT;
	
	DebugPrintf("network: enet connect to address %s\n",
		address_to_str(&address));

	peer = enet_host_connect( enet_host, &address, max_channels );

	if (peer == NULL)
	{
		return -1;
	}

	init_peer( peer );
	peer_data = peer->data;
	peer_data->state = CONNECTING;
	
	if( network_state == NETWORK_CONNECTING )
		host = peer;

	DebugPrintf("network: enet connect pending\n");

	return 1;
}

typedef enum {
	NO_FLUSH = 0,
	FLUSH = 2,
} flush_t;

static void enet_send_packet( ENetPeer* peer, ENetPacket* packet, int channel, flush_t flush )
{
	if ( packet == NULL )
	{
		DebugPrintf("network: enet send packet received null for packet\n");
		return;
	}
	enet_peer_send( peer, channel, packet );
	if ( flush )
		enet_host_flush( enet_host );
}

static void enet_send(
	ENetPeer* peer, void* data, int size, 
	enet_uint32 type, int channel, flush_t flush 
)
{
	ENetPacket * packet = enet_packet_create( data, size, type );
	enet_send_packet( peer, packet, channel, flush );
}

static network_player_t * find_player_with_lowest_id( void )
{
	peer_id_t id = HIGHEST_ID;
	network_player_t * lowest = NULL;
	network_player_t * player = network_players.first;
	if( enet_host == NULL ) return NULL;
	while( player )
	{
		ENetPeer * player_peer = (ENetPeer*) player->data;
		peer_id_t player_id = PEER_ID(player_peer);
		if( player_id < id && player_id != NO_ID )
		{
			id = player_id;
			lowest = player;
		}
		player = player->next;
	}
	if ( my_id < id )
	{
		return NULL; // I am the lowest
	}
	return lowest;
}

static void migrate_host( void )
{
	network_player_t * player = find_player_with_lowest_id();
	if ( ! player ) 
	{
		i_am_host = 1;
		host = NULL;
		player = NULL;
		DebugPrintf("network: I am the new host\n");
	}
	else
	{
		host = (ENetPeer*) player->data;
		DebugPrintf("network: host has been migrated to player %d\n",
			PLAYER_ID(player));
	}
	network_event( NETWORK_HOST, player );
}

/*
 *
 *  internal helpers
 *
 */

static enet_uint32 convert_flags( network_flags_t flags )
{
	// our default is unreliable and unsequenced
	enet_uint32 enet_flags = ENET_PACKET_FLAG_UNSEQUENCED;
	if ( flags )
	{
		// sequenced and unreliable
		if ( flags & NETWORK_SEQUENCED )
			enet_flags = 0;
		// note: enet reliable is always sequenced
		if ( flags & NETWORK_RELIABLE  )
			enet_flags |= ENET_PACKET_FLAG_RELIABLE;
	}
	return enet_flags;
}

static void update_player( network_player_t* player )
{
	ENetPeer * peer = (ENetPeer*) player->data;
	if(!peer) return;
	if( player->ping != peer->roundTripTime )
	{
		player->ping = peer->roundTripTime;
		//DebugPrintf("network extra: player %s (%d) ping has changed to %d\n",
		//	player->name, PEER_ID(peer), player->ping);
	}
	if( player->packet_loss != peer->packetLoss )
	{
		player->packet_loss = peer->packetLoss;
		//DebugPrintf("network extra: player %s (%d) packet loss has changed to %d\n",
		//	player->name, PEER_ID(peer), player->packet_loss);
	}
	if( player->packets_lost != peer->packetsLost )
	{
		player->packets_lost = peer->packetsLost;
		//DebugPrintf("network extra: player %s (%d) packets lost has changed to %d\n",
		//	player->name, PEER_ID(peer), player->packets_lost);
	}
}

static void update_players( void )
{
	network_player_t * player = network_players.first;
	if( enet_host == NULL ) return;
	while( player )
	{
		update_player( player );
		player = player->next;
	}
}

static void set_player_name( char * target, char * source )
{
	strncpy( target, source, NETWORK_MAX_NAME_LENGTH-1 );
	target[NETWORK_MAX_NAME_LENGTH-1] = 0;
}

static network_player_t * create_player( ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;
	network_player_t * player = malloc( sizeof( network_player_t ) );

	// name
	set_player_name( player->name, "NULL" );

	// dotted decimal ip
	enet_address_get_host_ip( &peer->address, player->ip, sizeof(player->ip) );

	// static settings
	player->port   = peer->address.port;
	player->bw_in  = peer->incomingBandwidth;
	player->bw_out = peer->outgoingBandwidth;

	// this is how we maintain relationship between peer and player
	player->data = peer;
	peer_data->player = player;

	// player is only added to list once in game
	player->prev = NULL;
	player->next = NULL;

	// update dynamic data (ping etc..)
	update_player( player );

	return player;
}

static void append_player( network_player_t * player )
{
	// TODO - this should never happen but it does
	if(!player)
		return;
	player->prev = network_players.last;
	if(player->prev)
		player->prev->next = player;
	player->next = NULL;
	network_players.last = player;
	if( network_players.first == NULL )
		network_players.first = player;
	network_players.length++;
}

static void destroy_player( network_player_t * player )
{
	ENetPeer * peer = player->data;
	network_peer_data_t * peer_data = NULL;
	if( peer )
		peer_data = peer->data;

	// join previous and next players together
	if( player->prev != NULL )	player->prev->next = player->next;
	if( player->next != NULL ) 	player->next->prev = player->prev;

	// reset first and last player
	if( player == network_players.first ) network_players.first = player->next;
	if( player == network_players.last  ) network_players.last  = player->prev;

	// drop the count
	network_players.length--;

	// remove peer/player associations
	player->data = NULL;
	if( peer_data )
		peer_data->player = NULL;

	// cleanup peer data
	if( peer )
		init_peer( peer );

	// destroy the player
	free( player );
}

static void destroy_players( void )
{
	network_player_t * player = network_players.first;
	while( player )
	{
		// destroy_player frees player hence player->next will be invalid
		network_player_t * next_player = player->next;
		destroy_player( player );
		player = next_player;
	}
	network_players.length = 0;
	network_players.first  = NULL;
	network_players.last   = NULL;
}

/*
 *
 *  hand shaking helpers
 *
 */

static void add_peer_to_connected_list( ENetPeer ** connected_list, ENetPeer * peer )
{
	int x;
	DebugPrintf("network: adding peer to connected list\n");
	for( x = 0; x < max_peers; x++ )
	{
		ENetPeer * connected = connected_list[x];
		if( connected == peer )
		{
			DebugPrintf("network: peer already on connected list\n");
			return; // already added
		}
		if( connected == NULL )
		{
			connected_list[x] = peer;
			DebugPrintf("network: added peer to connected list\n");
			return; // added
		}
	}
	DebugPrintf("network: connected list is already full\n");
}

static int peer_on_connected_list( ENetPeer ** connected_list, ENetPeer * peer )
{
	int x;
	for( x = 0; x < max_peers; x++ )
		if( peer == connected_list[x] )
			return 1;
	return 0;
}

static int all_players_on_connected_list( ENetPeer ** connected_list, ENetPeer * joiner )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		if( PEER_STATE(peer) != PLAYING ) continue;
		if( peer == joiner ) continue;
		if( ! peer_on_connected_list( connected_list, peer ) )
			return 0;
	}
	return 1;
}

/*
 *
 *  event system
 *
 */

typedef enum {

	// packet types
	NAME,            // player tells us his name
	CONNECT,         // host tells player to connect to another player
	CONNECTED_TO,    // player tells the host they connected to a player
	LOST_CONNECTION, // tell host that I lost a connection
	DISCONNECT,      // host tells players to disconnect someone
	NEW_PLAYER,      // host tells others that player is now synched
	CONNECT_PORT,    // player telling host which port people should connect to
	YOUR_ID,         // host tells you your id when you connect to them
	MY_ID,           // player tells new player his id on connection
	ACK_ID,          // player tells another player that he now has his id
	ACK_MY_ID,       // player tells host that he has his id

	// disconnect reasons
	FAILED_SYNCH,    // host telling you that you failed to synch with someone
	HOST_SAID_SO,    // host told me to disconnect from you
	I_QUIT,          // I'm leaving the game

	// nat punching
	NAT_PUNCH_THROUGH_LIST, // see explanation in new_connection()
	
} p2p_event_t;

typedef struct {
	p2p_event_t type;
} p2p_packet_t;

typedef struct {
	p2p_event_t type;
	char name[ NETWORK_MAX_NAME_LENGTH ];
} p2p_name_packet_t;

typedef struct {
	p2p_event_t type;
	ENetAddress address;
	peer_id_t id;
} p2p_connect_packet_t;

typedef struct {
	p2p_event_t type;
	int number;
} p2p_number_packet_t;

typedef struct {
	p2p_event_t type;
	peer_id_t id;
} p2p_id_packet_t;

typedef struct {
	p2p_event_t type;
	ENetAddress address; // ip and port as the host see's it
	unsigned short int connect_port;
} p2p_punch_packet_t; // see explanation in net_connection()

static void disconnect_all( void )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		if ( enet_host->peers[x].state != ENET_PEER_STATE_DISCONNECTED )
			enet_peer_disconnect_now( &enet_host->peers[x], I_QUIT );
	}
	init_peers();
}

static void new_connection( ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;

	// print debug info
	DebugPrintf("network: new connection from address %s\n",
		address_to_str(&peer->address));

	// some reason connection list was getting dirty
	// this makes sure it's clean before we commence
	init_connected_list( peer_data );

	// create the player structure
	peer_data->player = create_player( peer );

	// tell player my id
	if( peer != host )
	{
		p2p_id_packet_t packet;
		packet.type = MY_ID;
		packet.id = my_id;
		enet_send( peer, &packet, sizeof(packet),
			convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("network: sent my id (%d) to new connection\n", my_id);
	}

	// tell the player my external port people should connect to
	// we need to send this to everyone in case they become host later
	{
		p2p_number_packet_t packet;
		packet.type = CONNECT_PORT;
		packet.number = my_local_port;
		enet_send( peer, &packet, sizeof(packet),
			convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("network: sent my connect port (%d) to new connection\n",
			my_local_port);
	}

	// Send my player name to the new connection
	{
		p2p_name_packet_t packet;
		packet.type = NAME;
		set_player_name( packet.name, my_player_name );
		enet_send( peer, &packet, sizeof(packet),
			convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("network: sent my name (%s) to new connection\n",
			my_player_name);
	}

	// 
	if ( i_am_host )
	{
		// tell the new player his id
		p2p_id_packet_t packet;
		peer_data->id = find_free_id();
		packet.type = YOUR_ID;
		packet.id = peer_data->id;
		enet_send( peer, &packet, sizeof(packet),
			convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("network: sent new player his id %d\n",
			peer_data->id);
		peer_data->state = CONNECTED;
		////
		// TODO A better approach would probably be to just have joiner connect
		//      to the other hosts as well.  To protect against multiple connections
		//      check if the player is already connected and disconnect the new connection.
		//      But by doing a real connection you still get udp punch through AND
		//      in the case of that the joiner has symmetric nat (which will fail with
		//      nat punch through) you still have the chance that if the other side
		//      has the port open so a connection can be made...
		////
		// send the list of player addressess and connect ports to the joiner
		// the joiner will send an empty udp packet at these addresses
		// this will cause the joiners nat router to allow return traffic
		// which effectively punches a hole so existing players can connect
		// http://en.wikipedia.org/wiki/UDP_hole_punching
		////
		size_t x;
		for( x = 0; x < enet_host->peerCount; x++ )
		{
			ENetPeer * _peer = &enet_host->peers[x];
			if ( ! PEER_USED( _peer ) ) continue;
			if ( peer == _peer ) continue; // the peer does not need to send packets to him self
			p2p_punch_packet_t packet;
			packet.type         = NAT_PUNCH_THROUGH_LIST;
			packet.address      = _peer->address; // ip and port as the host see's it
			packet.connect_port = PEER_CONNECT_PORT( _peer );
			enet_send( peer, &packet, sizeof(packet),
				convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
			DebugPrintf("network: sent new player %d the ip/port (%s) and connect_port (%hu) for peer %hhu\n",
				peer_data->id,
				address_to_str(&_peer->address),
				PEER_CONNECT_PORT(_peer),
				PEER_ID(_peer));
		}
	}

	// I'm not the host
	else
	{
		// if we started this connection
		if( peer_data->state == CONNECTING )
		{
			// we connected to the host
			if( peer == host )
			{
				network_state = NETWORK_SYNCHING;
				DebugPrintf("network: we have connected to the host...\n");
				peer_data->state = CONNECTED;
			}
			else
			{
				//  we connected to a new player
				//  we must now wait for them to ack our id
				//  then we can tell the host that we connected
				peer_data->state = ACKING;
			}
		}
		else
		{
			DebugPrintf("network security: we didn't start this connection\n");
			peer_data->state = CONNECTED;
		}
	}
}

static void lost_connection( ENetPeer * peer, enet_uint32 data )
{
	char* reason = &data;
	network_peer_data_t * peer_data = peer->data;

	// print debug info
	{
		char * name = "NULL";
		if( peer_data->player && (peer_data->player->name[0] != 0) )
			name = peer_data->player->name;
		DebugPrintf("network security: lost connection from %s %d (%s@%s)\n",
			( host==peer ) ? "host" : "player",
			peer_data->id,
			name,
			address_to_str(&peer->address));
		switch(reason[0])
		{
			case FAILED_SYNCH:
				DebugPrintf("network: reason was because you failed to synch with player %d\n", reason[1]); 
				break;
			case HOST_SAID_SO:
				DebugPrintf("network: reason was because host told me to disconnect from you\n"); 
				break;
			case I_QUIT:
				DebugPrintf("network: reason was because I'm leaving the game\n"); 
				break;
			default:
				DebugPrintf("network: reason was unspecified\n");
		}
	}

	// I'm still trying to connect to host
	if( network_state == NETWORK_CONNECTING )
	{
		// we failed to connect to host
		if ( host == peer )
			network_state = NETWORK_DISCONNECTED;
	}

	// I'm trying to synch with everyone
	if( network_state == NETWORK_SYNCHING )
	{
		// we failed to connect to host
		if ( host == peer )
			network_state = NETWORK_SYNCH_FAILED;
	}

	// we are in the game
	if ( network_state == NETWORK_CONNECTED ) 
	{
		// backup data so we can kill peer before we broadcast
		peer_id_t id = peer_data->id;

		// if player was in the game
		if( peer_data->state == PLAYING )
		{
			// we lost a player
			network_event( NETWORK_LEFT,  peer_data->player );
			if( peer_data->player )
				destroy_player( peer_data->player );
			peer_data->player = NULL;

			// host left the game
			if ( host == peer )
			{
				host = NULL;
				migrate_host();
			}
		}

		// connection had a valid id
		if( id )
		{
			// if i am host
			if( i_am_host )
			{
				// tell everyone to drop the connection
				p2p_id_packet_t packet;
				packet.type = DISCONNECT;
				packet.id = id;
				network_broadcast( &packet, sizeof(packet),
					NETWORK_RELIABLE, system_channel );
				DebugPrintf("network: telling everyone to drop connection\n");
			}
			// I am not the host
			else
			{
				// tell host that we lost a connection
				network_peer_data_t * host_data = host->data;
				p2p_id_packet_t packet;
				packet.type = LOST_CONNECTION;
				packet.id = id;
				network_send( host_data->player, &packet, sizeof(packet),
					NETWORK_RELIABLE, system_channel );
				DebugPrintf("network: telling host that we lost connection\n");
			}
		}
	}

	// make sure we don't leak and clean everything up

	if( peer_data->player )
		destroy_player( peer_data->player );
	peer_data->player = NULL;

	init_peer( peer );
}

// other wise multiple players joining at once would block each other
static void tell_peer_to_connect_to_synchers( network_player_t * player )
{
	size_t x;
	ENetPeer * player_peer = player->data;
	DebugPrintf("network: telling new player %d to connect to synchers\n",
		PEER_ID(player_peer));
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		if( PEER_STATE(peer) == SYNCHING && player_peer != peer )
		{
			p2p_connect_packet_t packet;
			packet.type = CONNECT;
			packet.id = PEER_ID(peer);
			packet.address = peer->address;
			packet.address.port = PEER_CONNECT_PORT(peer);
			network_send( player, &packet, sizeof(packet),
				NETWORK_RELIABLE, system_channel );
			DebugPrintf("network: sending connect for syncher %d\n",
				PEER_ID(player_peer));
		}
	}
}

static void send_new_player_event( ENetPeer * peer )
{
	p2p_id_packet_t packet;
	packet.type = NEW_PLAYER;
	packet.id = PEER_ID(peer);
	network_broadcast( &packet, sizeof(packet),
		NETWORK_RELIABLE, system_channel );
	DebugPrintf("network: sent new player event for player (%d) to everyone\n",
		PEER_ID(peer));
}

static void send_new_player_event_for_existing_players( network_player_t * joiner )
{
	network_player_t * player = network_players.first;
	DebugPrintf("network: sending new player events to new player %d for existing players\n",
		PLAYER_ID(joiner));
	while( player )
	{
		p2p_id_packet_t packet;
		packet.type = NEW_PLAYER;
		packet.id = PLAYER_ID(player);
		network_send( joiner, &packet, sizeof(packet),
			NETWORK_RELIABLE, system_channel );
		DebugPrintf("network: sent new player event for existing player %d\n",
			PLAYER_ID(player));
		player = player->next;
	}
	// still need to send a new player event for me
	{
		p2p_id_packet_t packet;
		packet.type = NEW_PLAYER;
		packet.id = my_id;
		network_send( joiner, &packet, sizeof(packet),
			NETWORK_RELIABLE, system_channel );
		DebugPrintf("network: sent new player event for existing player %d (HOST)\n", my_id );
	}
}

static void new_player( ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;

	// we are not even connected yet...
	// only thing that should cause this is host telling us to connect to someone
	// but new_connection hasn't fired yet and he's giving us a new player event
	if(peer_data->state == CONNECTING || peer_data->player->name[0] == 0)
	{
		DebugPrintf("network error: "
			"got new player message for %hhu "
			"but they are still in state=CONNECTING or their name got wiped out! "
			"This new player event will be ignored !!!\n", peer_data->id);
		return;
	}

	DebugPrintf("network: player %s joined the game\n",
		peer_data->player->name);

	if( i_am_host )
	{
		send_new_player_event( peer ); // player never receives his own new player event
		send_new_player_event_for_existing_players( peer_data->player );
		tell_peer_to_connect_to_synchers( peer_data->player );
	}

	// once host starts sending me new player events then I'm in the game
	if( network_state != NETWORK_CONNECTED )
	{
		DebugPrintf("network: we have joined the game\n");
		network_state = NETWORK_CONNECTED;
		network_event( NETWORK_JOIN, NULL   );  // we joined the game
		DebugPrintf("network: host will now send us new player events for all existing players\n");
	}

	append_player( peer_data->player );
	peer_data->state = PLAYING;
	network_event( NETWORK_JOIN, peer_data->player );  // new player joined

	if( peer == host )
		network_event( NETWORK_HOST, peer_data->player );  // host has been set

	// for simplicity user may desire to rely on only name events to set the name
	DebugPrintf("network: firing off name event because we received a new player event\n");
	network_event( NETWORK_NAME, peer_data->player );
}

static void peer_connected_to( ENetPeer * peer, peer_id_t id )
{
	network_peer_data_t * joiner_data;
	ENetPeer * joiner = find_peer_by_id( id );
	if( ! joiner )
	{
		DebugPrintf("network security: failed to find joiner by id %d\n", id);
		return;
	}
	if( joiner->state == PLAYING )
	{
		network_peer_data_t * peer_data = peer->data;
		DebugPrintf("network security: player %d @ %s sent us a succesfully-connected-to-new-player message for player %d but player %d is already in the game!\n",
			peer_data->id, address_to_str(&peer->address), id, id);
		return;
	}
	joiner_data = joiner->data;
	add_peer_to_connected_list( joiner_data->connected_peers, peer );
	if( all_players_on_connected_list( joiner_data->connected_peers, joiner ) )
	{
		DebugPrintf("network: all players have connected to player %d\n",
			joiner_data->id);
		new_player( joiner );
	}
}

static void pump_synchers( void )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		network_peer_data_t * peer_data = (network_peer_data_t*) peer->data;
		if( PEER_STATE(peer) == SYNCHING )
		{
			if( all_players_on_connected_list( peer_data->connected_peers, peer ) )
			{
				DebugPrintf("network: player %d is fully synched firing new player revent\n",
					peer_data->id);
				new_player( peer );
			}
		}
	}
}

static void new_packet( ENetEvent * event )
{
	ENetPeer * peer = event->peer;
	network_peer_data_t * peer_data = (network_peer_data_t*) peer->data;
	if ( event->channelID == system_channel )
	{
		p2p_packet_t * packet = ((p2p_packet_t *) event->packet->data);
		//DebugPrintf("network extra: new system packet type %d\n",
		//	packet->type);
		switch( packet->type )
		{
		case YOUR_ID:
			{
				if( peer == host )
				{
					p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
					my_id = packet->id;
					DebugPrintf("network: host says my id is %d\n", my_id );
					// ack my id
					{
						p2p_packet_t packet;
						packet.type = ACK_MY_ID;
						enet_send( peer, &packet, sizeof(packet),
							convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
						DebugPrintf("network: acking my id\n");
					}
				}
				else
				{
					DebugPrintf("network security: %s tried to give me my id but they aren't the host\n",
						address_to_str(&peer->address));
				}
			}
			break;
		case MY_ID:
			{
				p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
				if ( ! i_am_host )
				{
					peer_data->id = packet->id;
					DebugPrintf("network: %s says their id is %d\n",
						address_to_str(&peer->address), peer_data->id );
					// ack the player id
					{
						p2p_packet_t packet;
						packet.type = ACK_ID;
						enet_send( peer, &packet, sizeof(packet),
							convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
						DebugPrintf("network: acking his id\n");
					}
				}
				else
				{
					DebugPrintf("network security: %s is telling me their id (%d) but I'm the host\n",
						address_to_str(&peer->address), packet->id);
				}
			}
			break;
		case ACK_ID:
			DebugPrintf("network: player %d has acked my id\n", peer_data->id);
			if( peer_data->state == ACKING ) // we were waiting for him to ack so we could tell host
			{
				if( peer_data->id == NO_ID )
				{
					DebugPrintf("network security: %s has ack'd my id before telling me their id!\n",
						address_to_str(&peer->address));
				}
				else // tell the host that we connected successfully
				{
					network_peer_data_t * host_data = host->data;
					p2p_id_packet_t packet;
					packet.type = CONNECTED_TO;
					packet.id = peer_data->id;
					network_send( host_data->player, &packet, sizeof(packet), 
						NETWORK_RELIABLE, system_channel );
					DebugPrintf("network: telling host that we have connected to new player %d\n",
						peer_data->id);
					peer_data->state = CONNECTED;
				}
			}
			break;
		case CONNECT_PORT:
			{
				p2p_number_packet_t * packet = (p2p_number_packet_t*) event->packet->data;
				peer_data->connect_port = packet->number;
				DebugPrintf("network: player %d says their connect port is %d\n",
					peer_data->id, peer_data->connect_port );
			}
			break;
		case ACK_MY_ID:
			if( i_am_host )
			{
				DebugPrintf("network: player %d has acked their id\n",
					peer_data->id);
				if( peer_data->connect_port )
				{
					// tell everyone to connect to new player
					p2p_connect_packet_t packet;
					packet.type = CONNECT;
					packet.id = peer_data->id;
					packet.address = peer->address;
					packet.address.port = peer_data->connect_port;
					network_broadcast( &packet, sizeof(packet),
						NETWORK_RELIABLE, system_channel );
					peer_data->state = SYNCHING;
					DebugPrintf("network: telling everyone to connect to player %d\n",
						peer_data->id);
				}
				else
				{
					DebugPrintf("network security: %s acked their id before telling me their connect port!\n",
						address_to_str(&peer->address));
				}
			}
			else
			{
					DebugPrintf("network security: %s acked their id but I'm not the host!\n",
						address_to_str(&peer->address));
			}
			break;
		case NEW_PLAYER:
			if( ! i_am_host )
			{
				if( peer == host )
				{
					p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
					ENetPeer * new_peer = find_peer_by_id( packet->id );
					DebugPrintf("network: host sent us new player event for player %d\n",
						packet->id);
					if( new_peer )
					{
						new_player( new_peer );
					}
					else
					{
						DebugPrintf("network security: failed to find new player by id %d\n",
							packet->id);
						break;
					}
				}
				// peer is not the host
				else
				{
					DebugPrintf("network security: %s sent us new-player event but they are not the host\n",
						address_to_str(&peer->address));
				}
			}
			// I am not host
			else
			{
				DebugPrintf("network security: %s sent us new player event but I am the host\n",
						address_to_str(&peer->address));
			}
			break;
		case NAME:
			{
				p2p_name_packet_t* packet = (p2p_name_packet_t*) event->packet->data;

				DebugPrintf("network: player %d has updated their name from '%s'",
					peer_data->id, peer_data->player->name );

				set_player_name( peer_data->player->name, packet->name );

				DebugPrintf_(" to '%s'\n",
					peer_data->player->name );

				if( peer_data->state == PLAYING )
					network_event( NETWORK_NAME, peer_data->player );
			}
			break;
		case DISCONNECT:
			{
				// host is telling us to disconnect from someone
				if( peer == host )
				{
					p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
					ENetPeer * bad_peer = find_peer_by_id( packet->id );
					// bank if we can't find the peer
					if(!bad_peer)
					{
						DebugPrintf("network security: host told us to disconnect from unknown player %d\n",
							packet->id );
						break;
					}
					// if not the host
					else
					{
						DebugPrintf("network: host told us to disconnect from player %d\n",
							packet->id);
						// dissconnect message will fire and cleanup player
						enet_peer_disconnect(bad_peer,HOST_SAID_SO);
					}
				}
				else
				{
					DebugPrintf("network security: %s told us to disconnect but they are not the host\n",
						address_to_str(&peer->address));
				}
			}
			break;
		case LOST_CONNECTION:
			{
				// player is telling the host that he lost connection to someone
				p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
				ENetPeer * bad_peer = find_peer_by_id( packet->id );
				if(!bad_peer)
				{
					DebugPrintf("network security: player %d told us he lost connection from unknown player %d\n",
						peer_data->id, packet->id );
					break;
				}
				if( i_am_host )
				{
					if( PEER_STATE(bad_peer) == SYNCHING )
					{			
						// tell everyone to drop the connection
						DebugPrintf("network: dropping player %d because they failed to synch with %d\n",
							packet->id, peer_data->id );
						// lost connection event will fire
						// this will send a DISCONNECT message for us to everyone
						// and will cleanup the peer and everything else
						enet_peer_disconnect_later(bad_peer,FAILED_SYNCH | peer_data->id << 8);
					}
					if( PEER_STATE(bad_peer) == PLAYING )
					{
						// tell them to reconnect cause bad_peer is valid
						p2p_connect_packet_t packet2;
						packet2.type = CONNECT;
						packet2.id = packet->id;
						packet2.address = bad_peer->address;
						packet2.address.port = peer_data->connect_port;
						network_send( peer_data->player, &packet2, sizeof(packet2),
							NETWORK_RELIABLE, system_channel );
						DebugPrintf("network: telling player %d to reconnect to valid player %d\n",
							peer_data->id, packet->id );
					}
				}
				else
				{
					DebugPrintf("network security: player %d told us he lost-connection but we are not host\n",
						peer_data->id);
				}
			}
			break;
		case CONNECTED_TO:
			{
				p2p_id_packet_t * packet = (p2p_id_packet_t*) event->packet->data;
				// player tells host that he successfully connected to new connection
				if( i_am_host )
				{
					// if we are also connected to this connection
					ENetPeer * target = find_peer_by_id( packet->id );
					if( target )
					{
						DebugPrintf("network: player %d succesfully connected with player %d\n",
							peer_data->id, packet->id);
						peer_connected_to( peer, packet->id );
					}
					// if we are not connected to this connection
					else
					{
						// tell the player to drop the connection
						p2p_id_packet_t packet2;
						packet2.type = DISCONNECT;
						packet2.id = packet->id;
						network_send( peer_data->player, &packet2, sizeof(packet2),
							NETWORK_RELIABLE, system_channel );
						DebugPrintf("network security: telling player %d to drop unknown player %d\n", 
							peer_data->id, packet->id);
					}
				}
				else
				{
					DebugPrintf("network security: %s told me they connected to someone but I'm not the host\n",
						address_to_str(&peer->address));
				}
			}
			break;
		case CONNECT:
			{
				p2p_connect_packet_t * packet = (p2p_connect_packet_t*) event->packet->data;
				ENetAddress * address = &packet->address;
				// host tells player to connect to a new connection
				if( ! i_am_host )
				{
					ENetPeer* new_peer;
					network_peer_data_t * new_peer_data;
					DebugPrintf("network: host told us to connect to player %d address %s\n",
						packet->id, address_to_str( address ));
					new_peer = enet_host_connect( enet_host, address, max_channels );
					if(!new_peer)
					{
						DebugPrintf("network: enet host connect returned NULL.\n");
					}
					else
					{
						new_peer_data = new_peer->data;
						new_peer_data->state = CONNECTING;
						new_peer_data->id = packet->id;
					}
				}
				// I'm the host
				else
				{
					DebugPrintf("network: %s just told me to connect to someone but I am the host!\n",
						address_to_str(&peer->address));
				}
			}
			break;
		case NAT_PUNCH_THROUGH_LIST: // see explanation of this in net_connection()
			{
				p2p_punch_packet_t * packet = (p2p_punch_packet_t*) event->packet->data;
				ENetAddress * address  = &packet->address;
				ENetAddress * address2 = &packet->address;
				address2->port = packet->connect_port;
				ENetBuffer buffer;
				buffer.dataLength = 0;
				int i;
				for(i=0; i<4; i++)
				{
					DebugPrintf("network: sending nat punch through for %s (%d)\n",
						address_to_str(address), packet->connect_port);
					enet_socket_send( enet_host->socket, address,  &buffer, 1 ); // as host see's them
					enet_socket_send( enet_host->socket, address2, &buffer, 1 ); // their connect port
				}
			}
			break;
		default:
			DebugPrintf("network security: received unknown packet type: %d\n",
				packet->type);
		}

	}

	// application packet
	else
	{
		// valid player
		if( peer_data->state == PLAYING )
		{
			network_packet_t packet;
			packet.size = (int) event->packet->dataLength;
			packet.data = (void*) event->packet->data;
			packet.from = peer_data->player;
			network_event( NETWORK_DATA, &packet );
		}

		// not a valid player
		else
		{
			DebugPrintf("network security: got application packet from non player at address %s\n",
				address_to_str(&peer->address) );	
		}
	}
	enet_packet_destroy( event->packet );
}

/*
 *
 *  net.h implementation
 *
 */

network_return_t network_setup( char* player_name, int local_port )
{
	network_cleanup();
	DebugPrintf("network: setup name='%s' connect_port=%d\n",
		player_name, local_port);
	set_player_name(my_player_name,player_name);
	network_state = NETWORK_DISCONNECTED;
	return enet_setup( NULL, local_port );
}

int network_join( char* address, int port )
{
	if( enet_host == NULL )
		return 0;
	DebugPrintf("network: join address '%s', local port %d\n",
		address,port);
	i_am_host = 0;
	my_id = NO_ID;
	if( network_state == NETWORK_CONNECTING )
	{
		DebugPrintf("network: join already connecting...\n");
		return 1;
	}
	network_state = NETWORK_CONNECTING;
	return enet_connect( address, port );
}

void network_host( void )
{
	if( enet_host == NULL ) return;
	DebugPrintf("network: starting a host session\n");
	i_am_host = 1;
	my_id = LOWEST_ID;
	host = NULL;
	network_state = NETWORK_CONNECTED;
}

void network_cleanup( void )
{
	if( enet_host == NULL ) return;
	DebugPrintf("network: cleanup\n");
	enet_host_flush(enet_host); // send any pending packets
	disconnect_all();
	destroy_players();
	cleanup_peers();
	enet_cleanup();
	network_state = NETWORK_DISCONNECTED;
	my_id = NO_ID;
	i_am_host = 0;
}

void network_send(
	network_player_t* player, void* data, int size,
	network_flags_t flags, int channel 
)
{
	if( enet_host == NULL ) return;
	enet_send( (ENetPeer*) player->data, data, size,
		convert_flags(flags), channel, (flags & NETWORK_FLUSH) );
}

void network_broadcast( void* data, int size, network_flags_t flags, int channel )
{
	network_player_t * player = network_players.first;
	ENetPacket * packet;
	if(!data)
	{
		DebugPrintf("network: broadcast received bad pointer.\n");
		return;
	}
	packet = enet_packet_create( data, size, convert_flags( flags ) );
	if( enet_host == NULL ) return;
	if( packet == NULL )
	{
		DebugPrintf("network: broadcast failed to create packet.\n");
		return;
	}
	while(player)
	{
		ENetPeer * peer = (ENetPeer*) player->data;
		if( peer )
			enet_send_packet( peer, packet, channel, NO_FLUSH );
		player = player->next;
	}
	enet_host_flush( enet_host );
}

void network_pump()
{
	ENetEvent event;
	if( enet_host == NULL ) return;
	while( enet_host_service( enet_host, &event, 0 ) > 0 )
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			new_connection( event.peer );
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			lost_connection( event.peer, event.data );
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			new_packet( &event );
			break;
		}
	}
	update_players();
	if( i_am_host)
		pump_synchers();
}

void network_set_player_name( char* name )
{
	set_player_name( my_player_name, name );
	network_player_t * player = network_players.first;
	ENetPacket * packet;
	p2p_name_packet_t name_packet;
	if( enet_host == NULL ) return;
	DebugPrintf("network: set my player name to %s\n",name);
	name_packet.type = NAME;
	set_player_name( name_packet.name, name );
	packet = enet_packet_create( &name_packet, sizeof(name_packet),
		convert_flags(NETWORK_RELIABLE|NETWORK_FLUSH) );
	if( packet == NULL )
	{
		DebugPrintf("network: set player name failed to create packet\n");
		return;
	}
	while(player)
	{
		ENetPeer * peer = (ENetPeer*) player->data;
		if( ! peer ) continue;
		enet_send_packet( peer, packet, system_channel, NO_FLUSH );
		player = player->next;
	}
	enet_host_flush( enet_host );
}

#endif
