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

#include "lua_common.h"

lua_State *L1;

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
	return 0;
}

static int load_init_file( void )
{
	if(luaL_dofile(L1,"Scripts/init.lua"))
	{
		Msg("error failed to load init.lua");
		return 1;
	}
	return 0;
}

static int run_init_func( void )
{
	lua_settop(L1, 0);
	lua_getglobal(L1, "init");    /* [bottom] init [top] */
#ifdef DEBUG_ON
	lua_pushinteger(L1, 1);
#else
	lua_pushinteger(L1, 0);
#endif
	err = lua_pcall(L1, 1, 0, 0);
	if (err)
	{
		Msg("error lua init: %s\n", lua_tostring(L1, -1));
		return err;
	}
}

int lua_init()
{
	ASSERT(lua_create());
	ASSERT(load_init_file());
	ASSERT(run_init_func());
	return 0;
}

void lua_shutdown(void)
{
	if(L1)
		lua_close(L1);
}
