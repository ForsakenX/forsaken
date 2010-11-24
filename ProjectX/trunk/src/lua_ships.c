/* Functions for accessing ship data from Lua.
 *
 * me = Ships[WhoIAm()]
 * print(me.Primary)
 * t = me:table()
 *
 * No support for modifying (yet?)
 *
 * see also networking.h
 */
#include <lua.h>
#include <lauxlib.h>
#include "main.h"
#include "new3d.h"
#include "object.h"
#include "networking.h"

extern BYTE WhoIAm;
extern GLOBALSHIP Ships[MAX_PLAYERS+1];

/* TODO - move this function to lua_vecmat.c and export it? */
static void lua_pushvector(lua_State *L, VECTOR *v)
{
	VECTOR *nv = lua_newuserdata(L, sizeof(VECTOR));
	nv->x = v->x;
	nv->y = v->y;
	nv->z = v->z;
	luaL_getmetatable(L, "VECTOR");
	lua_setmetatable(L, -2);
}

static void lua_pushobjptr(lua_State *L, void *v, const char *type)
{
	void **vp = lua_newuserdata(L, sizeof(void *));
	*vp = v;
	luaL_getmetatable(L, type);
	lua_setmetatable(L, -2);
}

#define FIELD(f, type) do {       \
	lua_push ## type(L, ship->f); \
	lua_setfield(L, -2, #f);      \
} while (0)
#define FIELDPTR(f, type) do {     \
	lua_push ## type(L, &ship->f); \
	lua_setfield(L, -2, #f);       \
} while (0)
static int luaship_table(lua_State *L)
{
	GLOBALSHIP *ship;
	ship = *((GLOBALSHIP **) luaL_checkudata(L, 1, "GLOBALSHIPPTR"));
	lua_createtable(L, 0, 41);
	/* Get a table for the Object field */
	lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
	lua_getfield(L, -1, "table");
	lua_pushvalue(L, -2);
	lua_call(L, 1, 1);
	lua_setfield(L, -1, "Object");
	lua_pop(L, 1); /* Get rid of the object pointer */
	FIELD(enable, integer);
	FIELD(ShipThatLastKilledMe, integer);
	FIELD(ShipThatLastHitMe, integer);
	FIELD(NumMultiples, integer);
	FIELD(StealthTime, number);
	FIELD(Timer, number);
	FIELD(InvulTimer, number);
	FIELD(Invul, boolean);
	FIELDPTR(LastAngle, vector);
	FIELD(PrimBullIdCount, integer);
	FIELD(SecBullIdCount, integer);
	FIELD(PickupIdCount, integer);
	FIELD(Damage, number);
	FIELD(Primary, integer);
	FIELD(Secondary, integer);
	FIELD(ModelNum, integer);
	FIELD(Pickups, integer);
	FIELD(RegenSlots, integer);
	FIELD(Mines, integer);
	FIELD(JustRecievedPacket, boolean);
	FIELDPTR(LastMove, vector);
	FIELDPTR(Move_Off, vector);
	/* TODO: OrbModels (uint16 array) */
	/* TODO: OrbAmmo (float array) */
	FIELD(PrimPowerLevel, number);
	FIELD(PrimID, integer);
	FIELD(SecID, integer);
	FIELD(SecWeapon, integer);
	FIELD(headlights, boolean);
	FIELD(DemoInterpolate, boolean);
	FIELDPTR(OldPos, vector);
	FIELDPTR(NextPos, vector);
	/* TODO: FIELDPTR(OldQuat, quat); */
	/* TODO: FIELDPTR(NextQuat, quat); */
	FIELD(OldBank, number);
	FIELD(NextBank, number);
	FIELD(SuperNashramTimer, number);
	/* TODO: TempLines? (uint16 array) */
	FIELD(ShakeTimer, number);
	FIELD(ShakeDirTimer, number);
	FIELD(ShakeForce, number);
	FIELD(PacketDelay, number);
	FIELD(ShieldHullCount, integer);
	FIELDPTR(RealPos, vector);
	FIELD(RealGroup, integer);
	return 1;
}
#undef FIELDPTR
#undef FIELD

#define FIELD(f, type) do {           \
	if (!strcmp(name, #f))            \
	{                                 \
		lua_push ## type(L, ship->f); \
		return 1;                     \
	}                                 \
} while (0)
#define FIELDPTR(f, type) do {         \
	if (!strcmp(name, #f))             \
	{                                  \
		lua_push ## type(L, &ship->f); \
		return 1;                      \
	}                                  \
} while (0)
static int luaship_index(lua_State *L)
{
	GLOBALSHIP *ship;
	const char *name;
	ship = *((GLOBALSHIP **) luaL_checkudata(L, 1, "GLOBALSHIPPTR"));
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "table"))
	{
		lua_pushcfunction(L, luaship_table);
		return 1;
	}
	else if (!strcmp(name, "Object"))
	{
		lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
		return 1;
	}
	FIELD(enable, integer);
	FIELD(ShipThatLastKilledMe, integer);
	FIELD(ShipThatLastHitMe, integer);
	FIELD(NumMultiples, integer);
	FIELD(StealthTime, number);
	FIELD(Timer, number);
	FIELD(InvulTimer, number);
	FIELD(Invul, boolean);
	FIELDPTR(LastAngle, vector);
	FIELD(PrimBullIdCount, integer);
	FIELD(SecBullIdCount, integer);
	FIELD(PickupIdCount, integer);
	FIELD(Damage, number);
	FIELD(Primary, integer);
	FIELD(Secondary, integer);
	FIELD(ModelNum, integer);
	FIELD(Pickups, integer);
	FIELD(RegenSlots, integer);
	FIELD(Mines, integer);
	FIELD(JustRecievedPacket, boolean);
	FIELDPTR(LastMove, vector);
	FIELDPTR(Move_Off, vector);
	/* TODO: OrbModels (uint16 array) */
	/* TODO: OrbAmmo (float array) */
	FIELD(PrimPowerLevel, number);
	FIELD(PrimID, integer);
	FIELD(SecID, integer);
	FIELD(SecWeapon, integer);
	FIELD(headlights, boolean);
	FIELD(DemoInterpolate, boolean);
	FIELDPTR(OldPos, vector);
	FIELDPTR(NextPos, vector);
	/* TODO: FIELDPTR(OldQuat, quat); */
	/* TODO: FIELDPTR(NextQuat, quat); */
	FIELD(OldBank, number);
	FIELD(NextBank, number);
	FIELD(SuperNashramTimer, number);
	/* TODO: TempLines? (uint16 array) */
	FIELD(ShakeTimer, number);
	FIELD(ShakeDirTimer, number);
	return luaL_argerror(L, 2, "unknown field name");
}
#undef FIELDPTR
#undef FIELD

static int luaship_arrayindex(lua_State *L)
{
	GLOBALSHIP *ships, **ship;
	int i;
	ships = *((GLOBALSHIP **) luaL_checkudata(L, 1, "GLOBALSHIPARRAYPTR"));
	i = luaL_checkint(L, 2);
	if (i >= MAX_PLAYERS)
		return luaL_argerror(L, 2, "array index exceeds MAX_PLAYERS limit");
	ship = lua_newuserdata(L, sizeof(void *));
	*ship = &Ships[i];
	luaL_getmetatable(L, "GLOBALSHIPPTR");
	lua_setmetatable(L, -2);
	return 1;
}

static int luaship_whoiam(lua_State *L)
{
	lua_pushinteger(L, WhoIAm);
	return 1;
}

int luaopen_ships(lua_State *L)
{
	static const luaL_Reg shiparrmt[] = {
		{ "__index", luaship_arrayindex },
		{ NULL, NULL }
	};
	static const luaL_Reg shipmt[] = {
		{ "__index", luaship_index },
		{ NULL, NULL }
	};
	GLOBALSHIP **ships = lua_newuserdata(L, sizeof(void *));
	*ships = Ships;
	luaL_newmetatable(L, "GLOBALSHIPARRAYPTR");
	luaL_register(L, NULL, shiparrmt);
	lua_setmetatable(L, -2);
	luaL_newmetatable(L, "GLOBALSHIPPTR");
	luaL_register(L, NULL, shipmt);
	lua_pop(L, 1);
	lua_setglobal(L, "Ships");
	lua_pushcfunction(L, luaship_whoiam);
	lua_setglobal(L, "WhoIAm");
	return 0;
}
