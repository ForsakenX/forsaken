#ifndef LUA_GAMES
#define LUA_GAMES

int games_length( void );
int games_port_at( int index );
char * games_name_at( int index );
char * games_ip_at( int index );

#endif // LUA_GAMES