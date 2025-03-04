#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "main.h"
#include "net.h"
#include "networking.h"

////////////////////////////////////////////////////////
// Program
////////////////////////////////////////////////////////

extern unsigned char my_id;
extern char* msg_to_str( int msg_type );

#define MAIN_CHANNEL 1
#define NETWORK_UNRELIABLE 0

#define MSGS 31

int msg_indexes[] = { MSG_BGOUPDATE,MSG_BIKENUM,MSG_DROPPICKUP,MSG_FUPDATE,MSG_GROUPONLY_VERYSHORTFUPDATE,MSG_HEREIAM,MSG_INIT,MSG_KILLPICKUP,MSG_LONGSTATUS,MSG_NETSETTINGS,MSG_PRIMBULLPOSDIR,MSG_REQTIME,MSG_SECBULLPOSDIR,MSG_SETTIME,MSG_SHIPDIED,MSG_SHIPHIT,MSG_SHOCKWAVE,MSG_SHORTMINE,MSG_SHORTPICKUP,MSG_SHORTREGENSLOT,MSG_SHORTSHIPHIT,MSG_SHORTTRIGGER,MSG_SHORTTRIGVAR,MSG_STATUS,MSG_TEAMGOALS,MSG_TEXTMSG,MSG_TITANBITS,MSG_VERYSHORTDROPPICKUP,MSG_VERYSHORTFUPDATE,MSG_VERYSHORTUPDATE,MSG_YOUQUIT };

int msg_index( int msg_type )
{
	int i;
	for(i=0;i<MSGS;i++)
		if (msg_indexes[i] == msg_type)
			return i;
	printf("ERROR: msg_index could not find type = %d\n",msg_type);
	return 0;
}

char* msg_to_str( int msg_type )
{
	switch( msg_type )
	{
	case MSG_YOUQUIT:                        return "MSG_YOUQUIT";                      break;
	case MSG_BIKENUM:                        return "MSG_BIKENUM";                      break;
	case MSG_HEREIAM:                        return "MSG_HEREIAM";                      break;
	case MSG_INIT:                           return "MSG_INIT";                         break;
	case MSG_VERYSHORTUPDATE:                return "MSG_VERYSHORTUPDATE";              break;
	case MSG_UPDATE:                         return "MSG_UPDATE";                       break;
	case MSG_FUPDATE:                        return "MSG_FUPDATE";                      break;
	case MSG_VERYSHORTFUPDATE:               return "MSG_VERYSHORTFUPDATE";             break;
	case MSG_GROUPONLY_VERYSHORTFUPDATE:     return "MSG_GROUPONLY_VERYSHORTFUPDATE";   break;
	case MSG_DROPPICKUP:                     return "MSG_DROPPICKUP";                   break;
	case MSG_VERYSHORTDROPPICKUP:            return "MSG_VERYSHORTDROPPICKUP";          break;
	case MSG_KILLPICKUP:                     return "MSG_KILLPICKUP";                   break;
	case MSG_TEAMGOALS:                      return "MSG_TEAMGOALS";                    break;
	case MSG_SHOCKWAVE:                      return "MSG_SHOCKWAVE";                    break;
	case MSG_BGOUPDATE:                      return "MSG_BGOUPDATE";                    break;
	case MSG_PRIMBULLPOSDIR:                 return "MSG_PRIMBULLPOSDIR";               break;
	case MSG_SECBULLPOSDIR:                  return "MSG_SECBULLPOSDIR";                break;
	case MSG_TITANBITS:                      return "MSG_TITANBITS";                    break;
	case MSG_SHIPHIT:                        return "MSG_SHIPHIT";                      break;
	case MSG_SHORTSHIPHIT:                   return "MSG_SHORTSHIPHIT";                 break;
	case MSG_SHIPDIED:                       return "MSG_SHIPDIED";                     break;
	case MSG_REQTIME:                        return "MSG_REQTIME";                      break;
	case MSG_SETTIME:                        return "MSG_SETTIME";                      break;
	case MSG_STATUS:                         return "MSG_STATUS";                       break;
	case MSG_NETSETTINGS:                    return "MSG_NETSETTINGS";                  break;
	case MSG_LONGSTATUS:                     return "MSG_LONGSTATUS";                   break;
	case MSG_SHORTPICKUP:                    return "MSG_SHORTPICKUP";                  break;
	case MSG_SHORTREGENSLOT:                 return "MSG_SHORTREGENSLOT";               break;
	case MSG_SHORTTRIGGER:                   return "MSG_SHORTTRIGGER";                 break;
	case MSG_SHORTTRIGVAR:                   return "MSG_SHORTTRIGVAR";                 break;
	case MSG_SHORTMINE:                      return "MSG_SHORTMINE";                    break;
	case MSG_TEXTMSG:                        return "MSG_TEXTMSG";                      break;
	case MSG_VERYSHORTINTERPOLATE:           return "MSG_VERYSHORTINTERPOLATE";         break;
	case MSG_INTERPOLATE:                    return "MSG_INTERPOLATE";                  break;
	}
	return "UNKNOWN";
}

// is this full list ?
char * msg_name[] = { "MSG_BGOUPDATE","MSG_BIKENUM","MSG_DROPPICKUP","MSG_FUPDATE","MSG_GROUPONLY_VERYSHORTFUPDATE","MSG_HEREIAM","MSG_INIT","MSG_KILLPICKUP","MSG_LONGSTATUS","MSG_NETSETTINGS","MSG_PRIMBULLPOSDIR","MSG_REQTIME","MSG_SECBULLPOSDIR","MSG_SETTIME","MSG_SHIPDIED","MSG_SHIPHIT","MSG_SHOCKWAVE","MSG_SHORTMINE","MSG_SHORTPICKUP","MSG_SHORTREGENSLOT","MSG_SHORTSHIPHIT","MSG_SHORTTRIGGER","MSG_SHORTTRIGVAR","MSG_STATUS","MSG_TEAMGOALS","MSG_TEXTMSG","MSG_TITANBITS","MSG_VERYSHORTDROPPICKUP","MSG_VERYSHORTFUPDATE","MSG_VERYSHORTUPDATE","MSG_YOUQUIT" };

unsigned char msg_id[] = {175,244,136,31,236,34,51,153,223,170,85,241,102,239,119,68,255,47,238,79,252,95,111,187,249,63,253,237,246,245,250};

size_t msg_size[] = {12,3,56,68,28,3,408,8,64,20,72,2,76,8,20,56,28,232,264,244,36,244,244,24,10,515,184,36,44,40,3};

unsigned char msg_data[1000];

void receive(network_packet_t * packet)
{
	printf("%s > %s %p\n", msg_to_str(packet->data), packet->from->name, (unsigned char *)packet->data);
}

void randomize_msg_data( void )
{
	int n;
	srand(rand()%UINT_MAX);
	for(n=0;n<sizeof(msg_data);n++)
		msg_data[n] = rand()%UCHAR_MAX;
}

void send_msg_by_index( int i )
{
	msg_data[1] = my_id;
	msg_data[0] = msg_id[i];
	network_broadcast( msg_data, msg_size[i], NETWORK_UNRELIABLE, MAIN_CHANNEL );
}
void send_msg_by_type( int t ) { send_msg_by_index(msg_index(t)); }

void init() { printf("my_id = %d\n",my_id); }

void loop()
{
	static int i = 0;
	randomize_msg_data();
	send_msg_by_type(MSG_STATUS); // flood valid status messages so we get in game
	printf("Sending message %s (%d) size=%d\n",(char*)msg_name[i],msg_id[i],msg_size[i]);
	// fix up messages to get around known security checks
	if(msg_id[i] == MSG_TEXTMSG) msg_data[2+MAXTEXTMSG] %= 17; // only 17 types of text messages
	// send it and move to next message
	send_msg_by_index(i);
	if( ++i >= MSGS ) i = 0;
}

////////////////////////////////////////////////////////
// Settings
////////////////////////////////////////////////////////

_Bool Debug = true;

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
			receive(packet);
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
