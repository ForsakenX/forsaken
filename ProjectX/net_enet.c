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

#ifdef NET_ENET
#include "net.h"
#include "enet/enet.h"
#include <stdio.h>
#include <string.h>

// debug print f
#include "util.h"

/*
 *
 *  settings
 *
 */

// state flags
static int initialized;
static unsigned int connections = 0;
static ENetAddress* my_external_address = NULL;
ENetHost* enet_host = NULL; // used in net_tracker.c
int my_local_port = 0; // used in net_tracker.c

// the host player
static ENetPeer* host;

// my settings
static int i_am_host = 0;
char* my_player_name = NULL; // used in net_tracker.c

// settings
static int max_peers		= 25;
static int max_channels		= 50;
static int system_channel	= 0;
static int default_port		= 2300;

/*
 *
 * Debug Helpers
 *
 */

// returns host:port syntax copy data right away as ip is overwritten next call
static char* address_to_str( ENetAddress * address )
{
	static char ip[INET_ADDRSTRLEN+10] = "";
	enet_address_get_host_ip( address, &ip[0], INET_ADDRSTRLEN );
	sprintf( &ip[0], "%s:%d", &ip, address->port );
	return &ip[0];
}

/*
 *
 *  peer helpers
 *
 */

typedef enum {
	PLAYING,	// peer is valid player in game
	UNUSED,
	CONNECTING, // host told us to connect
	CONNECTED,
	SYNCHING,   // host telling everyone to synch
} network_peer_state_t;

// peer->data = 
typedef struct {
	network_peer_state_t state;
	network_player_t * player;
	ENetPeer ** connected_peers; // used by host to keep track of connections
} network_peer_data_t;

static void init_peer( ENetPeer * peer )
{
	network_peer_data_t * data = peer->data;
	if( data == NULL ) // first init
	{
		peer->data = malloc( sizeof(network_peer_data_t) );
		data = peer->data;
		data->connected_peers = malloc( sizeof(void*) * max_peers );
	}
	memset( data->connected_peers, 0, sizeof(data->connected_peers) );
	data->state = UNUSED;
	data->player = NULL;
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

static ENetPeer * find_peer_by_address( ENetAddress * address )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		network_peer_data_t * peer_data = peer->data;
		if( peer_data->state != UNUSED )
			if( peer->address.host == address->host &&
				peer->address.port == address->port )
					return peer;
	}
	return NULL;
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
	DebugPrintf("enet_cleanup: finished\n");
}

static int enet_setup( char* str_address, int port )
{
	ENetAddress address;

	if ( ! initialized ){	
		if (enet_initialize() != 0)
		{
			return -1;
		}
		initialized = 1;
	}
	
	DebugPrintf("enet_setup: initialized\n");

	if ( ! str_address )	address.host = ENET_HOST_ANY;
	else					enet_address_set_host( &address, str_address ); 

	address.port = (port) ? port : default_port;

	DebugPrintf("enet_setup: address %s\n", address_to_str(&address));

	enet_host = enet_host_create( &address, max_peers, 0, 0 );

	if ( enet_host == NULL )
	{
		return -2;
	}

	DebugPrintf("enet_setup: initializing peers\n");

	init_peers();

	DebugPrintf("enet_setup: finished\n");

	return 1;
}

static int enet_connect( char* str_address, int port )
{
	ENetAddress address;
	ENetPeer* peer;
	network_peer_data_t * peer_data;

	if ( ! str_address )	address.host = ENET_HOST_ANY;
	else					enet_address_set_host( &address, str_address ); 

	address.port = (port) ? port : default_port;
	
	DebugPrintf("enet_connect: connecting to address %s\n", address_to_str(&address));

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

	DebugPrintf("enet_connect: waiting for connection.\n", str_address);

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
		DebugPrintf("enet_send_packet: received null for packet.\n");
		return;
	}
	enet_peer_send( peer, channel, packet );
	if ( flush )
		enet_host_flush( enet_host );
}

static void enet_send( ENetPeer* peer, void* data, int size, enet_uint32 type, int channel, flush_t flush )
{
	ENetPacket * packet = enet_packet_create( data, size, type );
	enet_send_packet( peer, packet, channel, flush );
}

static int lowest_address( ENetAddress * address1, ENetAddress * address2 )
{
	if(!address1)return 1;
	if(!address2)return -1;
	// if they come from the same ip
	if( address1->host == address2->host )
		// then test for lowest port
		return ( address1->port < address2->port ) ?  -1: 1;
	// other wise test for lowest ip
	return ( address1->host < address2->host ) ? -1: 1;
}

static network_player_t * get_player_with_lowest_address( void )
{
	network_player_t * lowest = network_players.first;
	network_player_t * current = NULL;

	if(!lowest) return  NULL;
	current = lowest->next;

	while( current )
	{
		ENetPeer * lowest_peer = lowest->data;
		ENetPeer * current_peer = current->data;

		DebugPrintf("lowest_address: %d, %d vs %d, %d\n",
					lowest_peer->address.host, lowest_peer->address.port,
					current_peer->address.host,	current_peer->address.port );

		if( lowest_address( &lowest_peer->address, &current_peer->address ) == 1 )
			lowest = current;

		DebugPrintf("winner: %d, %d\n",
					lowest_peer->address.host, lowest_peer->address.port );

		current = current->next;
	}

	return lowest;
}

// BUG: this needs to first detect for a mixture of lan and internet addresses
//      if a mixture is found then only internet addressses should be viable hosts...

static void migrate_host( void )
{
	network_player_t * player;
	host = NULL;
	player = get_player_with_lowest_address();
	if( player )
		host = player->data;
	if( host == NULL || lowest_address( &host->address, my_external_address ) == 1 )
	{
		i_am_host = 1;
		host = NULL;
		player = NULL;
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
	enet_uint32 enet_flags = ENET_PACKET_FLAG_UNSEQUENCED;  // our default is unreliable and unsequenced
	if ( flags )
	{
		if ( flags & NETWORK_SEQUENCED ) enet_flags = 0; // sequenced and unreliable
		if ( flags & NETWORK_RELIABLE  ) enet_flags |= ENET_PACKET_FLAG_RELIABLE; // note: enet reliable is always sequenced
	}
	return enet_flags;
}

static void update_player( network_player_t* player )
{
	ENetPeer * peer = (ENetPeer*) player->data;
	player->ping = peer->roundTripTime;
	player->packet_loss = peer->packetLoss;
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

static void update_player_name( network_player_t * player, char * name )
{
	strncpy( player->name, name, NETWORK_MAX_NAME_LENGTH-1 );
}

static network_player_t * create_player( char * name, ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;
	network_player_t * player = malloc( sizeof( network_player_t ) );

	// name
	update_player_name( player, name );

	// dotted decimal ip
	enet_address_get_host_ip( &peer->address, player->ip, sizeof(player->ip) );

	// static settings
	player->port   = peer->address.port;
	player->bw_in  = peer->incomingBandwidth;
	player->bw_out = peer->outgoingBandwidth;

	// append to players list
	player->prev   = network_players.last;
	if(player->prev)
		player->prev->next = player;
	player->next   = NULL;
	network_players.last = player;
	if( network_players.first == NULL )
		network_players.first = player;
	network_players.length++;

	// this is how we maintain relationship between peer and player
	player->data		= peer;
	peer_data->player	= player;

	// player state
	peer_data->state	= CONNECTED;

	// update dynamic data (ping etc..)
	update_player( player );

	return player;
}

static void destroy_player( network_player_t * player )
{
	ENetPeer * peer = player->data;
	network_peer_data_t * peer_data = peer->data;

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
	peer_data->player = NULL;

	// cleanup peer data
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

static void disconnect_all( void )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		if ( enet_host->peers[x].state != ENET_PEER_STATE_DISCONNECTED )
			enet_peer_disconnect_now( &enet_host->peers[x], 0 );
	}
	init_peers();
}

/*
 *
 *  hand shaking helpers
 *
 */

static void add_peer_to_connected_list( ENetPeer ** connected_list, ENetPeer * peer )
{
	int x;
	for( x = 0; x < max_peers; x++ )
	{
		ENetPeer * connected = connected_list[x];
		if( connected == peer )
		{
			DebugPrintf("-- peer already on connected list...\n");
			return; // already added
		}
		if( connected == NULL )
		{
			DebugPrintf("-- added peer to connected list...\n");
			connected_list[x] = peer;
			return; // added
		}
	}
	DebugPrintf("-- connected list is already full...\n");
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
	DebugPrintf("all_players_on_connected_list: STUB need joiners to have SYNCHING state\n");
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		network_peer_data_t * peer_data = peer->data;
		if( peer_data->state != PLAYING ) continue;
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
	NAME		= 2,  // player tells us his name
	CONNECT		= 4,  // host tells player to connect to another player
	DISCONNECT	= 6,  // tell host connection broken, or tell players to break
	IP_REQUEST	= 8,  // someone asked for their external ip address
	IP_RESPONSE	= 10,
	NEW_PLAYER	= 12, // host tells others that player is now synched
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
} p2p_address_packet_t;

static void new_connection( ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;

	// up the connection count
	connections++;

	// print debug info
	DebugPrintf("new_connection: from address %s connection count now %d.\n", 
				address_to_str(&peer->address), connections);

	// i am the host
	if ( i_am_host )
	{
		// tell everyone to connect to new player
		if( network_players.length > 0 )
		{
			p2p_address_packet_t packet;
			packet.type = CONNECT;
			packet.address = peer->address;
			network_broadcast( &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			peer_data->state = SYNCHING;
			DebugPrintf("-- telling everyone to connect to new connection.\n");
		}

		// no players exist yet
		else
		{
			peer_data->state = CONNECTED;
		}
	}

	// i am not host
	else
	{
		// if we started this connection
		if( peer_data->state == CONNECTING )
		{
			// we connected to the host
			if( peer == host )
			{
				DebugPrintf("-- we have connected to the host...\n");
			}
			// we connected to a new player
			else
			{
				// tell the host that we connected successfully
				network_peer_data_t * host_data = host->data;
				p2p_address_packet_t packet;
				packet.type = CONNECT;
				packet.address = peer->address;
				network_send( host_data->player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
				DebugPrintf("-- telling host that we have successfully connect to new player...\n");
			}
		}
		peer_data->state = CONNECTED;

		// once we know if we are already synched into the cloud
		// then we can drop un-invited connections
		DebugPrintf("-- STUB: need to deny un-invited connections\n");
	}

	// Send my player name to the new connection
	{
		p2p_name_packet_t packet;
		packet.type = NAME;
		strncpy( packet.name, my_player_name, NETWORK_MAX_NAME_LENGTH-1 );
		enet_send( peer, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("-- sent my name to new connection\n");
	}
	
    // request my internet address
    if ( !my_external_address )
	{
        p2p_packet_t packet;
        packet.type = IP_REQUEST;
        enet_send( peer, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("-- requesting my external address\n");
    }
}

static void lost_connection( ENetPeer * peer )
{
	network_peer_data_t * peer_data = peer->data;

	// lower connection count
	if(connections != 0)
		connections--;

	// print debug info
	{
		char * name = "NULL";
		if( peer_data->state == PLAYING )
			name = peer_data->player->name;
		DebugPrintf("lost_connection: from %s '%s' @ %s, connection count now %d.\n",
			( host==peer ) ? "host" : "player",	name, address_to_str(&peer->address), connections );
	}

	// I'm still trying to connect to host
	if( network_state == NETWORK_CONNECTING )
	{
		// if failed connection is host
		if ( host == peer )

			// we failed to connect
			network_state = NETWORK_DISCONNECTED;
	}

	// we are in the game
	else if ( network_state == NETWORK_CONNECTED ) 
	{
		// backup the address so we can kill peer before we broadcast
		ENetAddress address = peer->address;

		// if player was in the game
		if( peer_data->state == PLAYING )
		{
			// we lost a player
			network_event( NETWORK_LEFT,  peer_data->player );
			destroy_player( peer_data->player );
			peer_data->player = NULL;

			// host left the game
			if ( host == peer )
			{
				host = NULL;
				migrate_host();
			}
		}

		// if i am host
		if( i_am_host )
		{
			// tell everyone to drop the connection
			p2p_address_packet_t packet;
			packet.type = DISCONNECT;
			packet.address = address;
			network_broadcast( &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			DebugPrintf("-- telling everyone to drop connection\n");
		}

		// I am not the host
		else
		{
			network_peer_data_t * host_data = host->data;
			p2p_address_packet_t packet;
			packet.type = DISCONNECT;
			packet.address = address;
			network_send( host_data->player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			DebugPrintf("-- telling host that we lost connection\n");
		}
	}

	init_peer( peer ); // cleanup the peer data
}

static void tell_peer_to_connect_to_synchers( network_player_t * player )
{
	size_t x;
	for( x = 0; x < enet_host->peerCount; x++ )
	{
		ENetPeer * peer = &enet_host->peers[x];
		network_peer_data_t * data = peer->data;
		if( data->state == SYNCHING )
		{
			p2p_address_packet_t packet;
			packet.type = CONNECT;
			packet.address = peer->address;
			network_send( player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			DebugPrintf("-- telling %s to drop new connection ", address_to_str(&peer->address));
		}
	}
}

static void peer_connected_to( ENetPeer * peer, ENetAddress * address )
{
	network_peer_data_t * joiner_data;
	ENetPeer * joiner = find_peer_by_address( address );
	if( ! joiner )
	{
		DebugPrintf("-- ERROR: failed to find joiner by address\n");
		return;
	}
	joiner_data = joiner->data;
	add_peer_to_connected_list( joiner_data->connected_peers, peer );
	if( all_players_on_connected_list( joiner_data->connected_peers, joiner ) )
	{
		DebugPrintf("-- all players have connected to %s\n", address_to_str(address));
		DebugPrintf("-- sending new player event to everyone\n");
		{
			p2p_address_packet_t packet;
			packet.type = NEW_PLAYER;
			packet.address = peer->address;
			network_broadcast( &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
		}
		DebugPrintf("-- STUB need to tell new player to connect to synchers\n");
		// other wise multiple players joining at once would block each other
		//DebugPrintf("-- telling new player to connect to synching connections\n");
		//tell_peer_to_connect_to_synchers( joiner );
	}
}

static void new_packet( ENetEvent * event )
{
	ENetPeer * peer = event->peer;
	network_peer_data_t * peer_data = peer->data;
	if ( event->channelID == system_channel )
	{
		p2p_packet_t * packet = ((p2p_packet_t *) event->packet->data);
		//DebugPrintf("new_packet: system message\n");
		switch( packet->type )
		{
		case NEW_PLAYER:
			if( i_am_host )
			{
				DebugPrintf("-- connection %s tried to send us a new player event but we are host...\n",
							address_to_str(&peer->address));
			}
			// I am not host
			else
			{
				if( peer == host )
				{
					p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
					ENetAddress * address = &packet->address;
					DebugPrintf("-- host sent us new player event for connection %s\n", address_to_str(address));
					DebugPrintf("-- STUB: need to handle new player event\n");
					// check if we are the new player
					if( my_external_address->host == address->host && my_external_address->port == address->port )
					{
						DebugPrintf("-- we are the new player\n");
					}
					// we are not the new player
					else
					{
						ENetPeer * new_peer = find_peer_by_address( address );
						if(new_peer)
						{
							DebugPrintf("-- someone else is the new player\n");
						}
						else
						{
							DebugPrintf("-- could not find peer by address...\n");
							break;
						}
					}
				}
				// peer is not the host
				else
				{
					DebugPrintf("-- connection %s tried to send us new player event but they are not the host...",
								address_to_str(&peer->address));
				}
			}
			break;
		case NAME:
			{
				p2p_name_packet_t* packet = (p2p_name_packet_t*) event->packet->data;
				char* name = packet->name;

				// name update message
				if ( peer_data->state == PLAYING )
				{
					if ( peer_data->player )
					{
						DebugPrintf("-- '%s' has updated their name to '%s'.\n",
									peer_data->player->name, name);
						update_player_name( peer_data->player, name );
						network_event( NETWORK_NAME, peer_data->player ); // new name
						return;
					}
				}

				// initial name message
				else
				{
					network_player_t * player = create_player( name, peer );
					peer_data->state = PLAYING;
					DebugPrintf("-- player '%s' joined the game.\n",player->name);
					if ( network_state == NETWORK_CONNECTING )
					{
						DebugPrintf("-- we have joined the game.\n");
						network_state = NETWORK_CONNECTED;
						host = peer;
						network_event( NETWORK_JOIN, NULL   );  // we joined the game
						network_event( NETWORK_JOIN, player );  // new player joined
						network_event( NETWORK_HOST, host   );  // host has been set
					}
					else
					{
						network_event( NETWORK_JOIN, player );  // new player joined
					}
					// sure they could just use the new player event
					// but for simplicity they may desire to rely on one event for names
					network_event( NETWORK_NAME, player );
				}
			}
			break;
		case DISCONNECT:
			{
				p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
				ENetAddress * address = &packet->address;
				ENetPeer * bad_peer = find_peer_by_address( address );
				if(!bad_peer) break; // finished
				// i am the host
				if( i_am_host )
				{
					// player telling host that they failed to connect to new connection
					if( bad_peer->state == SYNCHING )
					{			
						// tell everyone to drop the connect
						p2p_address_packet_t packet;
						packet.type = DISCONNECT;
						packet.address = *address;
						network_broadcast( &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
						DebugPrintf("-- telling everyone to drop new connection %s ", address_to_str(address) );
						DebugPrintf("cause %s failed to connect to them...\n", address_to_str(&peer->address));
						enet_peer_disconnect(bad_peer,0); // dissconnect message will fire and cleanup player
					}
					else
					{
						// tell them to reconnect cause bad_peer is valid
						p2p_address_packet_t packet;
						packet.type = CONNECT;
						packet.address = *address;
						network_send( peer_data->player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
						DebugPrintf("-- telling %s to reconnect to ", address_to_str(&peer->address));
						DebugPrintf("valid player %s cause they lost connection...\n", address_to_str(address));
					}
				}
				// if not the host
				else
				{
					// host is telling us to disconnect from a player
					if( peer == host )
					{
						enet_peer_disconnect(bad_peer,0); // dissconnect message will fire and cleanup player
					}
					else
					{
						DebugPrintf("-- %s told us to disconnect from a connection but they are not the host...\n",
									address_to_str(&peer->address));
					}
				}
			}
			break;
		case CONNECT:
			{
				p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
				ENetAddress * address = &packet->address;
				// player tells host that he successfully connected to new connection
				if( i_am_host )
				{
					ENetPeer * connector = find_peer_by_address( address );
					// if we are connected to this connection
					if( connector )
					{
						DebugPrintf("-- player '%s' %s ", peer_data->player->name, address_to_str( &peer->address ));
						DebugPrintf("successfully connected with %s\n",	address_to_str( address ));		
						peer_connected_to( peer, address );
					}
					// if we are not connected to this connection
					else
					{
						// tell the player to drop the connection
						p2p_address_packet_t packet;
						packet.type = DISCONNECT;
						packet.address = *address;
						network_send( peer_data->player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
						DebugPrintf("-- telling %s to drop new connection ", address_to_str(&peer->address));
						DebugPrintf("%s because we are not connected to them...\n", address_to_str(address) );
					}
				}
				// host tells player to connect to a new connection
				else
				{
					ENetPeer* new_peer;
					network_peer_data_t * new_peer_data;
					DebugPrintf("-- host told us to connect to address %s\n", address_to_str( address ));
					new_peer = enet_host_connect( enet_host, address, max_channels );
					if(!new_peer)
						DebugPrintf("-- enet_host_connect returned NULL.\n");
					new_peer_data = new_peer->data;
					new_peer_data->state = CONNECTING;
				}
			}
			break;
		case IP_REQUEST:
			{
				p2p_address_packet_t packet;
				packet.type = IP_RESPONSE;
				packet.address = peer->address;
				network_send( peer_data->player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			}
			break;
		case IP_RESPONSE:
			if( ! my_external_address )
			{
				p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
				my_external_address = malloc( sizeof( ENetAddress ) );
				*my_external_address = packet->address;
				DebugPrintf("*** external address set to: %s\n", address_to_str(my_external_address) );
			}
			break;

		default:
			DebugPrintf("-- recieved unknown packet type: %d\n",packet->type);
		}

	}

	// application packet
	else
	{
		// valid player
		if( peer_data->state == PLAYING )
		{
			network_packet_t packet;
			//DebugPrintf("new_packet: application message\n");
			packet.size = (int) event->packet->dataLength;
			packet.data = (void*) event->packet->data;
			packet.from = peer_data->player;
			network_event( NETWORK_DATA, &packet );
		}

		// not a valid player
		else
		{
			DebugPrintf("-- got application packet from non player at address %s\n",
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

int network_setup( char* player_name, int local_port )
{
	network_cleanup();
	DebugPrintf("network_setup: player name set to '%s'\n",
				player_name, local_port);
	my_local_port = local_port;
	my_player_name = player_name;
	network_state = NETWORK_DISCONNECTED;
	return enet_setup( NULL, local_port );
}

int network_join( char* address, int port )
{
	if( enet_host == NULL )
		return 0;
	DebugPrintf("network_join: address '%s', local port %d\n",address,port);
	i_am_host = 0;
	if( network_state == NETWORK_CONNECTING )
	{
		DebugPrintf("network_join: already connecting...\n");
		return 1;
	}
	network_state = NETWORK_CONNECTING;
	return enet_connect( address, port );
}

void network_host( void )
{
	if( enet_host == NULL ) return;
	DebugPrintf("network_host\n");
	i_am_host = 1;
	host = NULL;
	network_state = NETWORK_CONNECTED;
}

void network_cleanup( void )
{
	if( enet_host == NULL ) return;
	DebugPrintf("network_cleanup\n");
	enet_host_flush(enet_host); // send any pending packets
	disconnect_all();
	destroy_players();
	cleanup_peers();
	enet_cleanup();
	connections = 0; // no disconnect messages had a chance to fire
	network_state = NETWORK_DISCONNECTED;
}

void network_send( network_player_t* player, void* data, int size, network_flags_t flags, int channel )
{
	if( enet_host == NULL ) return;
	enet_send( (ENetPeer*) player->data, data, size, convert_flags(flags), channel, (flags & NETWORK_FLUSH) );
}

void network_broadcast( void* data, int size, network_flags_t flags, int channel )
{
	network_player_t * player = network_players.first;
	ENetPacket * packet;
	if(!data)
	{
		DebugPrintf("network_broadcast: received bad pointer.\n");
		return;
	}
	packet = enet_packet_create( data, size, convert_flags( flags ) );
	if( enet_host == NULL ) return;
	if( packet == NULL )
	{
		DebugPrintf("network_broadcast: failed to create packet.\n");
		return;
	}
	while(player)
	{
		enet_send_packet( (ENetPeer*) player->data, packet, channel, NO_FLUSH );
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
				lost_connection( event.peer );
				break;
	        case ENET_EVENT_TYPE_RECEIVE:
				new_packet( &event );
				break;
	        }
	}
	update_players();
}

void network_set_player_name( char* name )
{
	network_player_t * player = network_players.first;
	ENetPacket * packet;
	p2p_name_packet_t name_packet;
	if( enet_host == NULL ) return;
	DebugPrintf("network_set_player_name: set to '%s'\n",name);
	name_packet.type = NAME;
	strncpy( name_packet.name, name, NETWORK_MAX_NAME_LENGTH-1 );
	packet = enet_packet_create( &name_packet, sizeof(name_packet), convert_flags(NETWORK_RELIABLE|NETWORK_FLUSH) );
	if( packet == NULL )
	{
		DebugPrintf("network_set_player_name: failed to create packet\n");
		return;
	}
	while(player)
	{
		enet_send_packet( (ENetPeer*) player->data, packet, system_channel, NO_FLUSH );
		player = player->next;
	}
	enet_host_flush( enet_host );
}

#endif