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

#include "enet/enet.h"
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "version.h"
#include "net.h"

// created in net_enet.c
extern ENetHost* enet_socket;
extern char * my_player_name;

// example
// send_tracker_update("fly.thruhere.net",47624);

void send_tracker_update( char* host, int port )
{
	int sent, size;
	//ENetSocket socket;
	ENetAddress address;
	ENetBuffer send_buffer;

	// tracker message
	char data[256] = "";
	sprintf(data,"hosting %s %s\n",my_player_name,PXVersion);
	size = strlen(data);

	/*
	if(enet_initialize())
	{
		DebugPrintf("Enet failed to initialize\n");
		return;
	}
	*/

	enet_address_set_host( &address, host ); 
	address.port = port;

	/*
	socket = enet_socket_create( ENET_SOCKET_TYPE_DATAGRAM );

	if( socket == ENET_SOCKET_NULL )
	{
		DebugPrintf("Failed to create socket\n");
		return;
	}

	enet_socket_set_option (socket, ENET_SOCKOPT_NONBLOCK, 1);
	enet_socket_set_option (socket, ENET_SOCKOPT_RCVBUF, 0 ); // ENET_HOST_RECEIVE_BUFFER_SIZE); // we receive no data
	enet_socket_set_option (socket, ENET_SOCKOPT_SNDBUF, size );// ENET_HOST_SEND_BUFFER_SIZE); // we send very little data
	*/

	send_buffer.data = data;
	send_buffer.dataLength = size;

	if(enet_socket)
		sent = enet_socket_send( enet_socket->socket, &address, &send_buffer, 1 );

	DebugPrintf("tracker sent %d bytes\n",sent);

	//enet_socket_destroy(socket);
}
