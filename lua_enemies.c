#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include "enemies.h"

extern ENEMY Enemies[MAXENEMIES];

static void pushenemy(lua_State *L, int index)
{
	int *id = lua_newuserdata(L, sizeof(int));
	*id = index;
	luaL_getmetatable(L, "ENEMYIDX");
	lua_setmetatable(L, -2);
}


#define FIELD(f, type) do {             \
	if (!strcmp(name, #f))              \
	{                                   \
		lua_push ## type(L, enemy->f);  \
		return 1;                       \
	}                                   \
} while (0)
#define FIELDPTR(f, type) do {          \
	if (!strcmp(name, #f))              \
	{                                   \
		lua_push ## type(L, &enemy->f); \
		return 1;                       \
	}                                   \
} while (0)
static int luaenem_index(lua_State *L)
{
	ENEMY *enemy;
	const char *name;
	void **objptr;
	int enemyidx = *((int *) luaL_checkudata(L, 1, "ENEMYIDX"));
	enemy = &Enemies[enemyidx];
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "index"))
	{
		lua_pushinteger(L, enemyidx);
		return 1;
	}
	FIELD(Used, boolean);
	FIELD(Alive, boolean);
	if (!strcmp(name, "Object"))
	{
		objptr = lua_newuserdata(L, sizeof(void *));
		*objptr = &enemy->Object;
		luaL_getmetatable(L, "OBJECTPTR");
		lua_setmetatable(L, -2);
		return 1;
	}
	else if (!strcmp(name, "NextUsed") && enemy->NextUsed != NULL)
	{
		pushenemy(L, enemy->NextUsed - Enemies);
		return 1;
	}
	else if (!strcmp(name, "PrevUsed") && enemy->PrevUsed != NULL)
	{
		pushenemy(L, enemy->PrevUsed - Enemies);
		return 1;
	}
	else if (!strcmp(name, "enabled"))
	{
		lua_pushboolean(L, enemy->Status & ENEMY_STATUS_Enable);
		return 1;
	}
	return luaL_argerror(L, 2, "unknown field name");
}
#undef FIELDPTR
#undef FIELD

/* TODO: move to a common function file and export */
static bool isudatatype(lua_State *L, int index, const char *mt)
{
	bool ret;
	if (!lua_getmetatable(L, index))
		return false;
	lua_getfield(L, LUA_REGISTRYINDEX, mt);
	ret = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	return ret;
}

static int luaenem_equals(lua_State *L)
{
	lua_pushboolean(L,
		isudatatype(L, 1, "ENEMYIDX") &&
		isudatatype(L, 2, "ENEMYIDX") &&
		*((int *) lua_touserdata(L, 1)) == *((int *) lua_touserdata(L, 2))
	);
	return 1;
}

int luaopen_enemies(lua_State *L)
{
	static const luaL_Reg enemymt[] = {
		{ "__index", luaenem_index },
		{ "__eq", luaenem_equals },
		{ NULL, NULL }
	};
	int i;

	luaL_newmetatable(L, "ENEMYIDX");
	luaL_register(L, NULL, enemymt);
	lua_pop(L, 1);
	lua_createtable(L, MAXENEMIES, 0);
	for (i=0; i<MAXENEMIES; i++)
	{
		lua_pushinteger(L, i+1);
		pushenemy(L, i);
		lua_settable(L, -3);
	}
	lua_setglobal(L, "enemies");
	return 0;
}
