/* vim:set sw=4 ts=4:
 *
 * Copyright (C) 2009  Pim Goossens
 *
 * Modified by Daniel Aquino (aka methods) for ProjectX
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"
#include "lua_common.h"
#include "lua_games.h"

void games_update( void )
{
	lua_getglobal(L1, "games");
	lua_getfield(L1, -1, "update");
	lua_call(L1, 0, 0);
	lua_pop(L1, 1);
}

int games_length( void )
{
	int length = 0;
	lua_getglobal(L1, "games");
	lua_getfield(L1, -1, "length");
	lua_call(L1, 0, 1);
	length = (int) lua_tonumber(L1, -1);
	lua_pop(L1, 2);
	return length;
}

int games_index_at( char * name )
{
	int i = 0;
	for( i = 1; i <= games_length(); i++ )
		if( strcmp(name, games_name_at(i)) == 0 )
			return i;
	return 0; // 0 means no game found
}

const char * games_port_at( int index )
{
	const char * port = "";
	lua_getglobal(L1, "games");
	lua_getfield(L1, -1, "port_at");
	lua_pushinteger(L1, index);
	lua_call(L1, 1, 1);
	port = lua_tostring(L1, -1);
	lua_pop(L1, 2);
	return port;
}

const char * games_name_at( int index )
{
	const char * name = "";
	lua_getglobal(L1, "games");
	lua_getfield(L1, -1, "name_at");
	lua_pushinteger(L1, index);
	lua_call(L1, 1, 1);
	name = lua_tostring(L1, -1);
	lua_pop(L1, 2);
	return name;
}

const char * games_ip_at( int index )
{
	const char * ip = "";
	lua_getglobal(L1, "games");
	lua_getfield(L1, -1, "ip_at");
	lua_pushinteger(L1, index);
	lua_call(L1, 1, 1);
	ip = lua_tostring(L1, -1);
	lua_pop(L1, 2);
	return ip;
}

