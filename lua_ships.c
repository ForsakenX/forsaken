/* Functions for accessing ship data from Lua.
 *
 * me = Ships[WhoIAm()]
 * print(me.Primary.name)
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
#include "2dtextures.h" /* FIXME -- needed for primary.h and secondary.h */
#include "primary.h"
#include "secondary.h"
#include "lua_vecmat.h"
#include "lua_weapons.h"

extern BYTE WhoIAm;
extern BYTE GameStatus[MAX_PLAYERS+1];

extern int8_t PrimaryToFireLookup[ MAXPRIMARYWEAPONS ];
extern int8_t SecondaryToFireLookup[ MAXSECONDARYWEAPONS ];

static void lua_pushobjptr(lua_State *L, void *v, const char *type)
{
	void **vp = lua_newuserdata(L, sizeof(void *));
	*vp = v;
	luaL_getmetatable(L, type);
	lua_setmetatable(L, -2);
}

#define GETTABLEFORPRIM(num) do {                     \
	lua_getglobal(L, "weapons");                      \
	lua_pushinteger(L, PrimaryToFireLookup[num] + 1); \
	lua_gettable(L, -2);                              \
	lua_remove(L, -2);                                \
} while (0)

#define GETTABLEFORSEC(num) do {                                     \
	lua_getglobal(L, "weapons");                                     \
	lua_pushinteger(L, SecondaryToFireLookup[num] + 1 + SECOFFSET); \
	lua_gettable(L, -2);                                             \
	lua_remove(L, -2);                                               \
} while (0)

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
	int shipidx;
	GLOBALSHIP *ship;
	shipidx = *((int *) luaL_checkudata(L, 1, "GLOBALSHIPIDX"));
	ship = &Ships[shipidx];
	lua_createtable(L, 0, 43);
	/* Get a table for the Object field */
	/* This should work but apparently doesn't :-/  --
	lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
	lua_getfield(L, -1, "table");
	lua_pushvalue(L, -2);
	lua_call(L, 1, 1);
	*/
	/* Get the C function that builds the table for Object... */
	luaL_getmetatable(L, "OBJECTPTR");
	lua_getfield(L, -1, "__index");
	lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
	lua_pushliteral(L, "table");
	lua_call(L, 2, 1);
	/* ...and call it with the Object pointer */
	lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
	lua_call(L, 1, 1);
	lua_setfield(L, -3, "Object");
	lua_pop(L, 1); /* Get rid of the metatable */
	/* Add index field */
	lua_pushinteger(L, shipidx);
	lua_setfield(L, -2, "index");
	/* Add status field */
	lua_pushinteger(L, GameStatus[shipidx]);
	lua_setfield(L, -2, "status");
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
	/* FIELD(Primary, integer); -- no longer used */
	/* FIELD(Secondary, integer); -- no longer used */
	GETTABLEFORPRIM(ship->Primary);
	lua_setfield(L, -2, "Primary");
	GETTABLEFORSEC(ship->Secondary);
	lua_setfield(L, -2, "Secondary");
	FIELD(ModelNum, integer);
	FIELD(Pickups, integer);
	FIELD(RegenSlots, integer);
	FIELD(Mines, integer);
	FIELD(JustRecievedPacket, boolean);
	FIELDPTR(LastMove, vector);
	FIELDPTR(Move_Off, vector);
	/* TODO: OrbModels (u_int16_t array) */
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
	/* TODO: TempLines? (u_int16_t array) */
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
	int shipidx = *((int *) luaL_checkudata(L, 1, "GLOBALSHIPIDX"));
	ship = &Ships[shipidx];
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "table"))
	{
		lua_pushcfunction(L, luaship_table);
		return 1;
	}
	else if (!strcmp(name, "index"))
	{
		lua_pushinteger(L, shipidx);
		return 1;
	}
	else if (!strcmp(name, "Object"))
	{
		lua_pushobjptr(L, &ship->Object, "OBJECTPTR");
		return 1;
	}
	else if (!strcmp(name, "status"))
	{
		lua_pushinteger(L, GameStatus[shipidx]);
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
	/* FIELD(Primary, integer); -- no longer used */
	/* FIELD(Secondary, integer); -- no longer used */
	if (!strcmp(name, "Primary"))
	{
		GETTABLEFORPRIM(ship->Primary);
		return 1;
	}
	if (!strcmp(name, "Secondary"))
	{
		GETTABLEFORSEC(ship->Secondary);
		return 1;
	}
	FIELD(ModelNum, integer);
	FIELD(Pickups, integer);
	FIELD(RegenSlots, integer);
	FIELD(Mines, integer);
	FIELD(JustRecievedPacket, boolean);
	FIELDPTR(LastMove, vector);
	FIELDPTR(Move_Off, vector);
	/* TODO: OrbModels (u_int16_t array) */
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
	/* TODO: TempLines? (u_int16_t array) */
	FIELD(ShakeTimer, number);
	FIELD(ShakeDirTimer, number);
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

static int luaship_equals(lua_State *L)
{
	lua_pushboolean(L,
		isudatatype(L, 1, "GLOBALSHIPIDX") &&
		isudatatype(L, 2, "GLOBALSHIPIDX") &&
		*((int *) lua_touserdata(L, 1)) == *((int *) lua_touserdata(L, 2))
	);
	return 1;
}

static int luaship_arrayindex(lua_State *L)
{
	GLOBALSHIP *ships;
	int *shipidx;
	int i;
	ships = *((GLOBALSHIP **) luaL_checkudata(L, 1, "GLOBALSHIPARRAYPTR"));
	i = luaL_checkint(L, 2);
	if (i < 0 || i >= MAX_PLAYERS)
		return luaL_argerror(L, 2, "invalid ship index");
	shipidx = lua_newuserdata(L, sizeof(int));
	*shipidx = i;
	luaL_getmetatable(L, "GLOBALSHIPIDX");
	lua_setmetatable(L, -2);
	return 1;
}

static int luaship_arraylength(lua_State *L)
{
	(void) luaL_checkudata(L, 1, "GLOBALSHIPARRAYPTR");
	lua_pushinteger(L, sizeof(Ships)/sizeof(Ships[0]));
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
		{ "__len", luaship_arraylength },
		{ NULL, NULL }
	};
	static const luaL_Reg shipmt[] = {
		{ "__index", luaship_index },
		{ "__eq", luaship_equals },
		{ NULL, NULL }
	};
	GLOBALSHIP **ships = lua_newuserdata(L, sizeof(void *));
	*ships = Ships;
	luaL_newmetatable(L, "GLOBALSHIPARRAYPTR");
	luaL_register(L, NULL, shiparrmt);
	lua_setmetatable(L, -2);
	luaL_newmetatable(L, "GLOBALSHIPIDX");
	luaL_register(L, NULL, shipmt);
	lua_pop(L, 1);
	lua_setglobal(L, "Ships");
	lua_pushcfunction(L, luaship_whoiam);
	lua_setglobal(L, "WhoIAm");
	return 0;
}
