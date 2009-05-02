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

int config_save( void )
{
	int err = 0;
	lua_settop(L1, 0);
	lua_getglobal(L1, "config_save");    /* [bottom] init [top] */
#ifdef DEBUG_ON
	lua_pushstring(L1, "debug");
#else
	lua_pushstring(L1, "main");
#endif
	err = lua_pcall(L1, 1, 0, 0);
	if (err)
		Msg("error lua init: %s\n", lua_tostring(L1, -1));
	return err;
}

int config_get_bool(const char *opt)
{
	int i;
	lua_getglobal(L1, "config");
	lua_getfield(L1, -1, opt);
	i = lua_toboolean(L1, -1);
	lua_pop(L1, 2);
	return i;
}

int config_get_int(const char *opt)
{
	int i;
	lua_getglobal(L1, "config");
	lua_getfield(L1, -1, opt);
	if (lua_isboolean(L1, -1))
		i = lua_toboolean(L1, -1);
	else
		i = luaL_checkint(L1, -1);
	lua_pop(L1, 2);
	return i;
}

float config_get_float(const char *opt)
{
	float f;
	lua_getglobal(L1, "config");
	lua_getfield(L1, -1, opt);
	f = (float) luaL_checknumber(L1, -1);
	lua_pop(L1, 2);
	return f;
}

/* NOTE: static storage of at most 8 option values. */
char *config_get_str(const char *opt)
{
	static char str[256*8];
	static int rot = 0;
	lua_getglobal(L1, "config");
	lua_getfield(L1, -1, opt);
	strncpy(&str[256*rot], luaL_checkstring(L1, -1), 256);
	str[256*rot+255] = '\0';
	lua_pop(L1, 2);
	rot = (rot + 1) % 8;
	return str;
}

void config_set_int(const char *opt, int i)
{
	lua_getglobal(L1, "config");
	lua_pushstring(L1, opt);
	lua_pushinteger(L1, i);
	lua_settable(L1,-3);
}

void config_set_float(const char *opt, float f)
{
	lua_getglobal(L1, "config");
	lua_pushstring(L1, opt);
	lua_pushnumber(L1, f);
	lua_settable(L1,-3);
}

void config_set_bool(const char *opt, int i)
{
	lua_getglobal(L1, "config");
	lua_pushstring(L1, opt);
	lua_pushboolean(L1, i);
	lua_settable(L1,-3);
}

void config_set_str(const char *opt, char* str)
{
	lua_getglobal(L1, "config");
	lua_pushstring(L1, opt);
	lua_pushstring(L1, str);
	lua_settable(L1,-3);
}

void config_set_strn(const char *opt, char* str, size_t size)
{
	lua_getglobal(L1, "config");
	lua_pushstring(L1, opt);
	lua_pushlstring(L1, str, size);
	lua_settable(L1,-3);
}
