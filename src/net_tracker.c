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

#include "main.h"
#include "enet/enet.h"
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "version.h"
#include "net.h"
#include "networking.h"
#include "oct2.h"

// created in net_enet.c
extern ENetHost* enet_host;
extern char my_player_name[];
extern int my_local_port;

static void send_tracker_message( char* host, int port, char* message );

/*
 *  Wrappers
 */

// 13 = hosting + white space + null byte
// +1 = comma for separating names
#define MAX_PORT 5
#define MAX_HOSTING_MSG (MAX_PORT + (MAXSHORTNAME+1) * MAX_PLAYERS + MAX_PXVersion + MAX_SHORT_LEVEL_NAME + 13)

void send_tracker_update( char* host, int port )
{
	int i;
	char message[ MAX_HOSTING_MSG ];
	sprintf(message,"hosting %d %s %s",my_local_port,PXVersion,my_player_name);
	for(i=0; i<MAX_PLAYERS; i++)
	{
		if ( i != WhoIAm && STATUS_VALID_PLAYER(GameStatus[i]) )
		{
			//DebugPrintf("net_tracker_update: adding player %d with status %d\n",
			//	i, GameStatus[i]);
			strcat(message,",");
			strncat(message,Names[i],MAXSHORTNAME);
		}
	}
	strcat(message," ");
	strncat(message,ShortLevelNames[NewLevelNum],MAX_SHORT_LEVEL_NAME);
	strcat(message,"\n");
	send_tracker_message( host, port, message );
}

void send_tracker_finished( char* host, int port )
{
	char message[256] = "";
	sprintf(message,"finished %d\n",my_local_port);
	send_tracker_message( host, port, message );
}

/*
 *  Sender
 */

// protocol is line oriented so you must send a new line to end message
static void send_tracker_message( char* host, int port, char* message )
{
	int sent = 0;
	int size = strlen(message);
	//ENetSocket socket;
	ENetAddress address;
	ENetBuffer send_buffer;

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

	send_buffer.data = message;
	send_buffer.dataLength = size;

	DebugPrintf("tracker message: %s\n",message);

	if(enet_host)
		sent = enet_socket_send( enet_host->socket, &address, &send_buffer, 1 );

	//DebugPrintf("tracker sent %d bytes\n",sent);

	//enet_socket_destroy(socket);
}
