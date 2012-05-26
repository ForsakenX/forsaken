/* vim:set sw=4 ts=4:
 *
 * Copyright (C) 2010  Daniel Aquino
 * Copyright (C) 2010  Pim Goossens
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

#include <stdio.h>
#include "lua_common.h"
#include "luasocket.h"
#include "mime.h"
#include "main.h"

lua_State *L1;

int lua_dofile(lua_State *L, const char *name)
{
	char path[100];
	snprintf(path, 100, "scripts/%s", name);
	if (luaL_dofile(L,path))
	{
		if (lua_isstring(L, -1))
			DebugPrintf("Error while loading %s: %s\n", path, lua_tostring(L, -1));
		else
			DebugPrintf("Unknown error while loading %s\n", path);
		lua_pop(L, 1);
		return 1;
	}
	return 0;
}

static int lua_debug_str(lua_State *state)
{
	char * str = (char*) lua_tostring(state,1); // get argument
	DebugPrintf(str);
	return 0; // number of results
}

static int lua_alert(lua_State *state)
{
	char * str = (char*) lua_tostring(state,1); // get argument
	Msg( str );
	return 0; // number of results
}

static int lua_touch_file(lua_State *state)
{
	char* path = (char*) lua_tostring(state,1); // get argument
	touch_file( path );
	return 0; // number of results
}

static int lua_register_funcs(void)
{
	lua_register(L1,"touch_file",lua_touch_file);
	lua_register(L1,"debug",lua_debug_str);
	lua_register(L1,"alert",lua_alert);
	return 0;
}

static int load_init_file( void )
{
	if(lua_dofile(L1, "init.lua"))
	{
		Msg("error failed to load init.lua");
		return 1;
	}
	return 0;
}

static int run_init_func( void )
{
	int err = 0;
	lua_settop(L1, 0);
	lua_getglobal(L1, "init");    /* [bottom] init [top] */
#ifdef DEBUG_ON
	lua_pushboolean(L1, 1);
#else
	lua_pushboolean(L1, 0);
#endif
	err = lua_pcall(L1, 1, 0, 0);
	if (err)
	{
		const char * ptr = lua_tostring(L1, -1);
		if(!ptr)
			ptr = "Unknown Error";
		Write_File( "Logs/lua_init_error.txt", (char*) ptr, (long)strlen(ptr) );
		Msg("error lua init: %s\n", (char*) ptr );
	}
	return err;
}

static void assign_loader( char * name, lua_CFunction loader )
{
	// package.preload[ name ] = loader
	lua_getglobal(L1, "package");
	lua_getfield(L1, -1, "preload");
	lua_pushcfunction(L1, loader);
	lua_setfield(L1, -2, name);
	lua_pop(L1, 2);
}

#ifdef MINIUPNP
int luaopen_miniupnp(lua_State *L);
#endif

static void assign_loaders( void )
{
	assign_loader( "socket.core", luaopen_socket_core );
	assign_loader( "mime.core", luaopen_mime_core );
#ifdef MINIUPNP
	assign_loader( "miniupnp", luaopen_miniupnp );
#endif
}

static int lua_create(void)
{
	/* luaL_Reg x[] = { { NULL, NULL } }; */
	L1 = luaL_newstate();
	if (!L1)
	{
		Msg("error: while initializing Lua!\n");
		return 1;
	}
	luaL_openlibs(L1);
	assign_loaders();
	return 0;
}

int lua_init()
{
	ASSERT(lua_create());
	ASSERT(lua_register_funcs());
	ASSERT(load_init_file());
	ASSERT(run_init_func());
	return 0;
}

void lua_shutdown(void)
{
	if(L1)
		lua_close(L1);
}
