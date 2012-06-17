#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "net.h"

////////////////////////////////////////////////////////
// Program
////////////////////////////////////////////////////////

extern unsigned char my_id;

#define MAIN_CHANNEL 1
#define NETWORK_UNRELIABLE 0

#define MSGS 31

// is this full list ?
char * msg_name[] = { "MSG_BGOUPDATE","MSG_BIKENUM","MSG_DROPPICKUP","MSG_FUPDATE","MSG_GROUPONLY_VERYSHORTFUPDATE","MSG_HEREIAM","MSG_INIT","MSG_KILLPICKUP","MSG_LONGSTATUS","MSG_NETSETTINGS","MSG_PRIMBULLPOSDIR","MSG_REQTIME","MSG_SECBULLPOSDIR","MSG_SETTIME","MSG_SHIPDIED","MSG_SHIPHIT","MSG_SHOCKWAVE","MSG_SHORTMINE","MSG_SHORTPICKUP","MSG_SHORTREGENSLOT","MSG_SHORTSHIPHIT","MSG_SHORTTRIGGER","MSG_SHORTTRIGVAR","MSG_STATUS","MSG_TEAMGOALS","MSG_TEXTMSG","MSG_TITANBITS","MSG_VERYSHORTDROPPICKUP","MSG_VERYSHORTFUPDATE","MSG_VERYSHORTUPDATE","MSG_YOUQUIT" };

unsigned char msg_id[] = {175,244,136,31,236,34,51,153,223,170,85,241,102,239,119,68,255,47,238,79,252,95,111,187,249,63,253,237,246,245,250};

size_t msg_size[] = {12,3,56,68,28,3,408,8,64,20,72,2,76,8,20,56,28,232,264,244,36,244,244,24,10,515,184,36,44,40,3};

char msg_data[1000];

void init() { memset(msg_data,1,sizeof(msg_data)); }

void loop()
{
	static int i = 0;
	printf("Sending message %s (%d) size=%d\n",(char*)msg_name[i],msg_id[i],msg_size[i]);
	msg_data[0] = msg_id[i];
	msg_data[1] = my_id;
	network_broadcast( msg_data, msg_size[i], NETWORK_UNRELIABLE, MAIN_CHANNEL );
	if( ++i >= MSGS ) i = 0;
}

////
// Pending
// 1339927845.545 net_msg: MSG_INIT
// 1339927845.545 net_msg: MSG_STATUS
// 1339927845.645 MSG_INIT: WhoIAm = 1
// 1339927845.645 host says level is 
// 1339927845.645 client cannot find level 
// 1339927845.651 net_msg: MSG_STATUS
// 1339927845.849 test LongStatus = 1
////

////
// Fixed
// 1. valid message id
// 2. valid message size
// 3. valid from player id
// 4. text messages end in \0 and fit in receiving bucket
////

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

void describe_player( network_player_t * player )
{
	printf( "{ name: %s, source: %s:%d, ping: %ld/ms, bw{ in: %ld, out: %ld }, loss: %ld }\n",
		player->name, player->ip, player->port, player->ping,
		player->bw_in, player->bw_out, player->packet_loss );
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

int _state_joined = 0;

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
			init();
			_state_joined = 1;
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
			printf("%s > %p\n", packet->from->name, packet->data);
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
		if(_state_joined) loop();
		network_pump();
		usleep(1000*100); // 100ms, 0.1s
	}

	return 0;
}
