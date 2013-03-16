#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "net.h"

#define MAX_LINE 100
#define CHAT_CHANNEL 1

////////////////////////////////////////////////////////
// Settings
////////////////////////////////////////////////////////

BOOL Debug = TRUE;

char * my_player_name;
int my_connect_port = 0;
int hosting = 0;

char * host_ip;
int host_port = 0;

int usage( char* str )
{
	if(str != NULL)
		puts(str);
	printf( "Usage: ./client <player-name> [<local-port> [<remote-address> [<remote-port>]]\n" );
	return 1;
}

int parse_command_line( int argc, char ** argv )
{
	if ( argc < 2 )
		return usage("You must supply at least a <player-name>.");
	my_player_name = (char*) argv[1];
	my_connect_port = argv[2] ? atoi(argv[2]) : 0 ;
	hosting = (argc < 4) ? 1 : 0 ;
	if(!hosting)
	{
		host_ip = (char*) argv[3];
		host_port = argv[4] ? atoi(argv[4]) : 0 ;
	}
	printf("-- command line arguments: ");
	printf("name=%s, connect_to=%d, hosting=%d host_ip=%s, host_port=%d\n",
		my_player_name, my_connect_port, hosting, host_ip, host_port);
	return 0;
}

////////////////////////////////////////////////////////
// Command Line
////////////////////////////////////////////////////////

void send_text_msg( char * line )
{
	line[strlen(line)-1] = 0; // wipe out the \n char
	network_broadcast( (void*) line, strlen(line)+1,
		NETWORK_RELIABLE, CHAT_CHANNEL );
}

void help(void)
{
	printf("-- commands: /help /info /quit\n");
}

void describe_player( network_player_t * player )
{
	printf( "{ name: %s, source: %s:%d, ping: %ld/ms, bw{ in: %ld, out: %ld }, loss: %ld }\n",
		player->name, player->ip, player->port, player->ping,
		player->bw_in, player->bw_out, player->packet_loss );
}

void describe_players( void )
{
	network_player_t * player = network_players.first;
	while( player )
	{
		describe_player( player );
		player = player->next;
	}
}

#define PRINT_STATE(x) case x: puts("-- network state " #x); break;
void debug_network_state( void )
{
	switch( network_state )
	{
	PRINT_STATE( NETWORK_CONNECTED );
	PRINT_STATE( NETWORK_CONNECTING );
	PRINT_STATE( NETWORK_DISCONNECTED );
	PRINT_STATE( NETWORK_SYNCHING );
	PRINT_STATE( NETWORK_SYNCH_FAILED );
	}
}

#define MATCH( line, str )\
  ( strncmp( line, str, strlen(str) ) == 0 )

void handle_input( char * line )
{
	if( ! MATCH( line, "/" ) )
		return send_text_msg( line );
	if( MATCH( line, "/help" ) )
		return help();
	if( MATCH( line, "/info" ) )
		return describe_players();
	if( MATCH( line, "/state" ) )
		return debug_network_state();
	if( MATCH( line, "/quit" ) )
		exit(0);
	printf("-- unknown command: %s\n",line);
}

////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////

int stdin_ready( void )
{
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec  = 0;
	tv.tv_usec = 1; // microsecond

	FD_ZERO(&readfds);
	FD_SET(fileno(stdin), &readfds);

	if ( select( fileno(stdin)+1, &readfds, NULL, NULL, &tv ) > 0 )
	{
		// (FD_ISSET(fileno(stdin), &readfds))
		//{
			return 1;
		//}
	}

	return 0;
}

int read_line( char* line )
{
	int size, c = 0;
	while( stdin_ready() )
	{
		size = strlen(line);
		c = getchar();
		if( c != EOF && c != '\n' && size < (MAX_LINE-1) )
		{
			line[size] = c;
		}
		else
		{
			line[size] = 0;
			return (size==0)?0:size+1;
		}
	}
	return 0; // don't have a full line yet
}

void check_input( void )
{
	static char line[ MAX_LINE ];
	bzero( line, sizeof(line) );
	int size = read_line( line );
	if( size > 0 )
	{
		handle_input( line );
		memset( line, 0, size );
	}
}

////////////////////////////////////////////////////////
// Networking
////////////////////////////////////////////////////////

int bind_port( void )
{
	int rv = network_setup( my_player_name, my_connect_port ); // name, source port
	if( rv != NETWORK_OK )
	{
		switch( rv )
		{
		case NETWORK_ERROR_BIND:
			puts("-- either your local port is already in use or your arguments were invalid.");
			break;
		case NETWORK_ERROR_INIT:
			puts("-- enet failed to initialize.");
			break;
		}
		return 1;
	}
	return 0;
}

int connect_to_host( void )
{
	int rv = network_join( host_ip, host_port );
	if( rv != 1 )
	{
		puts("-- failed to join game");
		puts("-- most likely your arguments were invalid");
		return usage(NULL);
	}
	printf("-- waiting to connect to host.\n");
	while( network_state == NETWORK_CONNECTING )
		network_pump();
	if( network_state == NETWORK_DISCONNECTED )
	{
		puts("-- failed to connect to host to host.");
		exit(1);
	}
	printf("-- synching with other players.\n");
	while( network_state == NETWORK_SYNCHING )
		network_pump();
	if( network_state == NETWORK_SYNCH_FAILED )
	{
		puts("-- synching failed");
		exit(1);
	}
	if( network_state != NETWORK_CONNECTED )
	{
		debug_network_state();
		puts("-- disconnect during synch");
		exit(1);
	}
	puts("-- successfully synched with all players");
	return 0;
}

int setup_network()
{
	atexit(network_cleanup);
	if(bind_port())
		return 1;
	if(hosting)
	{
		network_host();
		printf("-- waiting for connections.\n");
	}
	else
	{
		connect_to_host();
	}
	return 0;
}

// network layer will call this function
void network_event( network_event_type_t type, void* data )
{
	network_player_t * player = (network_player_t *) data;  // used by most events
	switch( type )
	{
	case NETWORK_JOIN:
		if ( data == NULL )
		{
			puts("-- we have joined the game");
			puts("-- enter text to be sent to other players");
		}
		else
		{
			printf("-- new player ");
			describe_player( player );
		}
		break;
	case NETWORK_LEFT:
		if ( data == NULL )
		{
			puts("-- we left the game.");
		}
		else
		{		
			printf( "-- player left the game " );
			describe_player( player );
		}
		break;
	case NETWORK_HOST:
		if ( data == NULL )
		{
			puts("-- we are now the host.");
		}
		else
		{
			printf( "-- host has been set/changed to " );
			describe_player( player );
		}
		break;
	case NETWORK_DATA:
		{
			network_packet_t * packet = (network_packet_t*) data;
			printf("%s > %s\n", packet->from->name, (char*) packet->data);
		}
		break;
	case NETWORK_NAME:
		{
			printf("-- received name '%s' from address %s:%d\n",
				player->name, player->ip, player->port);
		}
		break;
	default:
		printf("-- unknown network event type: %d\n", type);
	}
}

////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////

int main (int argc, char ** argv) 
{
	setvbuf( stdin, NULL, _IONBF, 0 );
	setvbuf( stdout, NULL, _IONBF, 0 );

	if(parse_command_line( argc, argv ))
		return 1;

	if(setup_network())
		return 1;

	while( 1 )
	{
		check_input();
		network_pump();
		usleep(1000*100); // 100ms, 0.1s
	}

	return 0;
}
