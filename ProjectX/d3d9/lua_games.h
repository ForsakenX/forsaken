#ifndef LUA_GAMES
#define LUA_GAMES

void games_update( void );
int games_length( void );
int games_index_at( char * name );
const char * games_port_at( int index );
const char * games_name_at( int index );
const char * games_ip_at( int index );

#endif // LUA_GAMES