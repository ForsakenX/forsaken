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
ENetHost* enet_socket = NULL; // used in net_tracker.c

// the host player
static ENetPeer* host;

// my settings
static int i_am_host = 0;
char* my_player_name = NULL; // used in net_tracker.c

// settings
static int max_players		= 25;
static int max_channels		= 50;
static int system_channel	= 0;
static int default_port		= 2300;

/*
 *
 *  enet helpers
 *
 */

static void enet_cleanup( void )
{
	initialized = 0;
	enet_host_destroy( enet_socket );
	enet_socket = NULL;
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

	{
		char ip[INET_ADDRSTRLEN] = "";
		enet_address_get_host_ip( &address, &ip[0], INET_ADDRSTRLEN );
		DebugPrintf("enet_setup: address %s port %d\n",ip,address.port);
	}

	enet_socket = enet_host_create( &address, max_players, 0, 0 );

	if ( enet_socket == NULL )
	{
		return -2;
	}

	DebugPrintf("enet_setup: finished\n");

	return 1;
}

static int enet_connect( char* str_address, int port )
{
	ENetAddress address;
	ENetPeer* peer;

	if ( ! str_address )	address.host = ENET_HOST_ANY;
	else					enet_address_set_host( &address, str_address ); 

	address.port = (port) ? port : default_port;
	
	{
		char ip[INET_ADDRSTRLEN] = "";
		enet_address_get_host_ip( &address, &ip[0], INET_ADDRSTRLEN );
		DebugPrintf("enet_connect: connecting to address %s port %d\n",ip,address.port);
	}

	peer = enet_host_connect( enet_socket, &address, max_channels );

	if (peer == NULL)
	{
		return -1;
	}
	
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
		enet_host_flush( enet_socket );
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

static ENetPeer * get_player_with_lowest_address( void )
{
    size_t x;
	ENetPeer* lowest = NULL;
	ENetPeer* peers = enet_socket->peers;
	for( x = 0; x < enet_socket->peerCount; x++ )
	{
		if( peers[x].data == NULL ) continue; // network_player_t
		if( !lowest )
		{
			lowest = &peers[x];
			continue;
		}
		DebugPrintf("lowest_address: %d, %d vs %d, %d\n",
					&lowest->address.host, &lowest->address.port,
					&peers[x].address.host, &peers[x].address.port );
		if( lowest_address( &lowest->address, &peers[x].address ) )
			lowest = &peers[x];
		DebugPrintf("winner: %d, %d\n", &lowest->address.host, &lowest->address.port );
	}
	return lowest;
}

// BUG: this needs to first detect for a mixture of lan and internet addresses
//      if a mixture is found then only internet addressses should be viable hosts...

static void migrate_host( void )
{
	host = get_player_with_lowest_address();
	// if my_external_address is not set your not even in the cloud anyway
	if( host == NULL || lowest_address( &host->address, my_external_address ) )
	{
		i_am_host = 1;
		host = NULL;
	}
	network_event( NETWORK_HOST, host );
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
	size_t x;
	if( enet_socket == NULL ) return;
	for( x = 0; x < enet_socket->peerCount; x++ )
		if( enet_socket->peers[x].data != NULL ) // network_player_t
			update_player( (network_player_t *) enet_socket->peers[x].data );
}

static void update_player_name( network_player_t * player, char * name )
{
	strncpy( player->name, name, NETWORK_MAX_NAME_LENGTH-1 );
}

static network_player_t * create_player( char * name, ENetPeer * peer )
{
	network_player_t * player = malloc( sizeof( network_player_t ) );

	// name
	strncpy( player->name, name, NETWORK_MAX_NAME_LENGTH-1 );

	// dotted decimal ip
	enet_address_get_host_ip( &peer->address, player->ip, sizeof(player->ip) );

	// static settings
	player->port   = peer->address.port;
	player->bw_in  = peer->incomingBandwidth;
	player->bw_out = peer->outgoingBandwidth;

	// append to players list
	player->prev   = network_players.last;
	player->next   = NULL;
	network_players.last = player;
	if( network_players.first == NULL )
		network_players.first = player;
	network_players.length++;

	// this is how we maintain relationship between peer and player
	player->data   = peer;
	peer->data     = player;

	// update dynamic data (ping etc..)
	update_player( player );

	return player;
}

static void destroy_player( network_player_t * player )
{
	// remove from players list
	if( player->prev != NULL )	player->prev->next = player->next;
	if( player->next != NULL ) 	player->next->prev = player->prev;
	network_players.length--;

	// remove peer association
	player->data = NULL;

	// destroy the player
	free( player );
}

static void destroy_players( void )
{
	size_t x;
	for ( x = 0; x < enet_socket->peerCount; x++ )
	{
		if ( enet_socket->peers[x].data != NULL )
		{
			destroy_player( enet_socket->peers[x].data );
			enet_socket->peers[x].data = NULL;
		}
	}
	network_players.length = 0;
	network_players.first  = NULL;
	network_players.last   = NULL;
}

void disconnect_all( void )
{
	size_t x;
	for( x = 0; x < enet_socket->peerCount; x++ )
		if ( enet_socket->peers[x].data != NULL ) // network_player_t
			enet_peer_disconnect_now( &enet_socket->peers[x], 0 );
}

/*
 *
 *  event system
 *
 */

typedef enum {
	NAME		= 2, // player tells us his name
	CONNECT		= 4, // host tells us to connect to another player
	IP_REQUEST	= 6, // someone asked for their external ip address
	IP_RESPONSE	= 8,
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
	connections++;

	{
		char ip[INET_ADDRSTRLEN] = "";
		enet_address_get_host_ip( &peer->address, &ip[0], INET_ADDRSTRLEN );
		DebugPrintf("new_connection: from address %s port %d connection count now %d.\n",
					ip, peer->address.port, connections);
	}

	// tell everyone to connect to new player
	if ( i_am_host && enet_socket->peerCount > 1 ){
		p2p_address_packet_t packet;
		packet.type = CONNECT;
		packet.address = peer->address;
		network_broadcast( &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
		DebugPrintf("new_connection: Told everyone to connect to new player.\n");
	}

	// Send my player name to the new connection
	{
		p2p_name_packet_t packet;
		packet.type = NAME;
		strncpy( packet.name, my_player_name, NETWORK_MAX_NAME_LENGTH-1 );
		enet_send( peer, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
		DebugPrintf("new_connection: Sent my name to new player\n");
	}
	
    // request my internet address
    if ( !my_external_address ){
        p2p_packet_t packet;
        packet.type = IP_REQUEST;
        enet_send( peer, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel, NO_FLUSH );
    }
}

static void lost_connection( ENetPeer * peer )
{
	if(connections != 0)
		connections--;

	// print debug info
	{
		char name[NETWORK_MAX_NAME_LENGTH+1] = "NULL";
		char ip[INET_ADDRSTRLEN] = "";
		enet_address_get_host_ip( &peer->address, &ip[0], INET_ADDRSTRLEN );
		if( peer->data )
		{
			network_player_t * player = peer->data;
			if( player->name )
				strncpy( name, player->name, NETWORK_MAX_NAME_LENGTH+1 );
		}
		DebugPrintf("lost_connection: from player '%s' address %s port %d connection count now %d.\n",
					name, ip, peer->address.port, connections );
	}

	if( network_state == NETWORK_CONNECTING )
		if( host == peer )
		{
			network_state = NETWORK_DISCONNECTED;
			return;
		}

	if(peer->data)
	{
		network_event( NETWORK_LEFT, (network_player_t*) peer->data );
		if ( host == peer )
		{
			host = NULL;
			migrate_host();
		}
		destroy_player( (network_player_t*) peer->data );
		peer->data = NULL;
	}
}

static void new_packet( ENetEvent * event )
{
	if ( event->channelID == system_channel )
	{
		p2p_packet_t * packet = ((p2p_packet_t *) event->packet->data);
		//DebugPrintf("new_packet: system message\n");
		switch( packet->type )
		{
		case NAME:
			{
				p2p_name_packet_t* packet = (p2p_name_packet_t*) event->packet->data;
				char* name = packet->name;

				// name update message
				if ( event->peer->data != NULL )
				{
					network_player_t * player = event->peer->data;
					if ( player->name )
					{
						DebugPrintf("new_packet: '%s' has updated their name to '%s'.\n",
									player->name,name);
						update_player_name( player, name );
						network_event( NETWORK_NAME, player ); // new name
						return;
					}
				}

				// initial name message
				else
				{
					network_player_t * player = create_player( name, event->peer );
					DebugPrintf("new_packet: player '%s' joined the game.\n",player->name);
					if ( !i_am_host && connections == 1 )
					{
						DebugPrintf("new_packet: we have joined the game.\n");
						network_state = NETWORK_CONNECTED;
						host = event->peer;
						network_event( NETWORK_JOIN, NULL );    // we joined the game
						network_event( NETWORK_JOIN, player );  // new player joined
						network_event( NETWORK_HOST, player );  // host has been set
					}
					else
					{
						network_event( NETWORK_JOIN, player );  // new player joined
					}
					network_event( NETWORK_NAME, player ); // fire player name to update
				}
			}
			break;
		case CONNECT:
			{
				p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
				ENetAddress address = packet->address;
				ENetPeer* peer = enet_host_connect( enet_socket, &address, max_channels );
				char ip[INET_ADDRSTRLEN] = "";
				enet_address_get_host_ip( &peer->address, &ip[0], INET_ADDRSTRLEN );
				DebugPrintf("new_packet: host told us to connect to address %s port %d.\n",
							ip, peer->address.port );
			}
			break;
		case IP_REQUEST:
			{
				network_player_t * player = (network_player_t*)event->peer->data;
				p2p_address_packet_t packet;
				packet.type = IP_RESPONSE;
				packet.address = event->peer->address;
				network_send( player, &packet, sizeof(packet), convert_flags(NETWORK_RELIABLE), system_channel );
			}
			break;
		case IP_RESPONSE:
			{
				p2p_address_packet_t * packet = (p2p_address_packet_t*) event->packet->data;
				ENetAddress * my_external_address = malloc( sizeof( ENetAddress ) );
				my_external_address->host = packet->address.host;
				my_external_address->port = packet->address.port;
				//DebugPrintf("*** external address set to: %s:%d\n",(char*)ip_to_str(my_external_address->host),my_external_address->port);
			}
			break;

		default:
			DebugPrintf("new_packet: recieved unknown packet type: %d\n",packet->type);
		}

	}
	else if( event->peer->data != NULL ) // network_player_t
	{
		network_packet_t packet;
		//DebugPrintf("new_packet: application message\n");
		packet.size = (int) event->packet->dataLength;
		packet.data = (void*) event->packet->data;
		packet.from = (network_player_t*) event->peer->data;
		network_event( NETWORK_DATA, &packet );
	}
	else
	{
		char ip[INET_ADDRSTRLEN] = "";
		enet_address_get_host_ip( &event->peer->address, &ip[0], INET_ADDRSTRLEN );
		DebugPrintf("new_packet: got application packet from non player, address %s port %d.\n",
					ip, event->peer->address.port );	
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
	DebugPrintf("network_setup: player name set to '%s'\n",
				player_name, local_port);
	my_player_name = player_name;
	network_state = NETWORK_DISCONNECTED;
	return enet_setup( NULL, local_port );
}

int network_join( char* address, int port )
{
	if( enet_socket == NULL )
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
	if( enet_socket == NULL ) return;
	DebugPrintf("network_host\n");
	i_am_host = 1;
	host = NULL;
	network_state = NETWORK_CONNECTED;
}

void network_cleanup( void )
{
	if( enet_socket == NULL ) return;
	DebugPrintf("network_cleanup\n");
	disconnect_all();
	destroy_players();
	enet_cleanup();
	network_state = NETWORK_DISCONNECTED;
}

void network_send( network_player_t* player, void* data, int size, network_flags_t flags, int channel )
{
	if( enet_socket == NULL ) return;
	enet_send( (ENetPeer*) player->data, data, size, convert_flags(flags), channel, (flags & NETWORK_FLUSH) );
}

void network_broadcast( void* data, int size, network_flags_t flags, int channel )
{
	size_t x;
	ENetPacket * packet = enet_packet_create( data, size, convert_flags(flags) );
	if( enet_socket == NULL ) return;
	if( packet == NULL )
	{
		DebugPrintf("network_broadcast: failed to create packet");
		return;
	}
	for( x = 0; x < enet_socket->peerCount; x++ )
		if( enet_socket->peers[x].data != NULL ) // network_player_t
			enet_send_packet( &enet_socket->peers[x], packet, channel, NO_FLUSH );
	enet_host_flush( enet_socket );
}

void network_pump()
{
	ENetEvent event;
	if( enet_socket == NULL ) return;
	while( enet_host_service( enet_socket, &event, 0 ) > 0 )
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
	size_t x;
	ENetPacket * packet;
	p2p_name_packet_t name_packet;
	if( enet_socket == NULL ) return;
	DebugPrintf("network_set_player_name: set to '%s'\n",name);
	name_packet.type = NAME;
	strncpy( name_packet.name, name, NETWORK_MAX_NAME_LENGTH-1 );
	packet = enet_packet_create( &name_packet, sizeof(name_packet), convert_flags(NETWORK_RELIABLE|NETWORK_FLUSH) );
	if( packet == NULL )
	{
		DebugPrintf("network_set_player_name: failed to create packet");
		return;
	}
	for( x = 0; x < enet_socket->peerCount; x++ )
		if( enet_socket->peers[x].data != NULL ) // network_player_t
			enet_send_packet( &enet_socket->peers[x], packet, system_channel, NO_FLUSH );
	enet_host_flush( enet_socket );
}

#endif