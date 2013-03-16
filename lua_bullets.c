/* Functions for accessing bullet data from Lua.
 *
 * OWNER_SHIP = 1
 *
 * for i, b in ipairs(bullets) do
 *     if b.Used then
 *         if b.OwnerType == OWNER_SHIP and b.Owner == me then
 *             print("Bullet " .. i .. " is mine")
 *         else
 *             print("Bullet " .. i .. " is NOT mine")
 *         end
 *     end
 * end
 *
 * see also primary.h and secondary.h
 */
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include "main.h" /* FIXME -- needed for primary.h and secondary.h */
#include "2dtextures.h" /* FIXME -- needed for primary.h and secondary.h */
#include "new3d.h" /* FIXME -- needed for primary.h and secondary.h */
#include "object.h" /* FIXME -- needed for networking.h */
#include "networking.h" /* FIXME -- needed for primary.h and secondary.h */
#include "primary.h"
#include "secondary.h"
#include "lua_vecmat.h"
#include "lua_weapons.h"

extern PRIMARYWEAPONBULLET PrimBulls[MAXPRIMARYWEAPONBULLETS];
extern SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];

static void pushprimbull(lua_State *L, u_int16_t index)
{
	int *id = lua_newuserdata(L, sizeof(int));
	*id = (int) index;
	luaL_getmetatable(L, "PRIMARYWEAPONBULLETIDX");
	lua_setmetatable(L, -2);
}

static void pushsecbull(lua_State *L, u_int16_t index)
{
	int *id = lua_newuserdata(L, sizeof(int));
	*id = (int) index;
	luaL_getmetatable(L, "SECONDARYWEAPONBULLETIDX");
	lua_setmetatable(L, -2);
}

static int pushentity(lua_State *L, u_int16_t type, u_int16_t index)
{
	const char *mt;
	int *id;

	switch (type)
	{
		case OWNER_SHIP: mt = "GLOBALSHIPIDX"; break;
		case OWNER_ENEMY: mt = "ENEMYIDX"; break;
		case OWNER_MINE: mt = "SECONDARYWEAPONBULLETIDX"; break;
		default:
			lua_pushnil(L);
			lua_pushliteral(L, "unknown target type");
			return 2;
	}
	id = lua_newuserdata(L, sizeof(int));
	*id = index;
	luaL_getmetatable(L, mt);
	lua_setmetatable(L, -2);
	return 1;
}

#define GETTABLEFORPRIM(index) do { \
	lua_getglobal(L, "weapons");    \
	lua_pushinteger(L, index + 1);  \
	lua_gettable(L, -2);            \
	lua_remove(L, -2);              \
} while (0)

#define FIELD(f, type) do {             \
	if (!strcmp(name, #f))              \
	{                                   \
		lua_push ## type(L, bullet->f); \
		return 1;                       \
	}                                   \
} while (0)
#define FIELDPTR(f, type) do {         \
	if (!strcmp(name, #f))             \
	{                                  \
		lua_push ## type(L, &bullet->f); \
		return 1;                      \
	}                                  \
} while (0)
static int luaprimbull_index(lua_State *L)
{
	PRIMARYWEAPONBULLET *bullet;
	const char *name;
	int *id;
	int bullidx = *((int *) luaL_checkudata(L, 1, "PRIMARYWEAPONBULLETIDX"));
	bullet = &PrimBulls[bullidx];
	name = luaL_checkstring(L, 2);
	/*if (!strcmp(name, "table")) -- TODO
	{
		lua_pushcfunction(L, luaprimbull_table);
		return 1;
	}
	else*/ if (!strcmp(name, "index") || !strcmp(name, "Index"))
	{
		lua_pushinteger(L, bullidx + 1);
		return 1;
	}
	else if (!strcmp(name, "category"))
	{
		lua_pushliteral(L, "primary");
		return 1;
	}
	FIELD(Used, boolean);
	if (!strcmp(name, "Next") && bullet->Next != (u_int16_t) -1)
	{
		pushprimbull(L, bullet->Next);
		return 1;
	}
	else if (!strcmp(name, "Prev") && bullet->Prev != (u_int16_t) -1)
	{
		pushprimbull(L, bullet->Prev);
		return 1;
	}
	else if (!strcmp(name, "Type"))
	{
		GETTABLEFORPRIM(bullet->Type);
		return 1;
	}
	FIELD(OwnerType, integer);
	if (!strcmp(name, "Owner"))
		return pushentity(L, bullet->OwnerType, bullet->Owner);
	/* FIELD(EnemyGun, ??); -- TODO */
	if (!strcmp(name, "Weapon"))
	{
		GETTABLEFORPRIM(bullet->Weapon);
		return 1;
	}
	FIELD(PowerLevel, integer);
	FIELD(TrojPower, number);
	FIELD(LifeCount, number);
	FIELD(Speed, number);
	FIELD(ColRadius, number);
	FIELD(ColType, integer);
	FIELDPTR(Offset, vector);
	FIELDPTR(Pos, vector);
	FIELDPTR(Dir, vector);
	FIELDPTR(LocalDir, vector);
	FIELDPTR(UpVector, vector);
	FIELDPTR(ColStart, vector);
	FIELD(ColDist, number);
	FIELD(ColFlag, integer);
	FIELDPTR(ColPoint, vector); /* VERT */
	FIELDPTR(ColPointNormal, vector); /* NORMAL */
	FIELD(ColGroup, integer);
	FIELD(GroupImIn, integer);
	FIELDPTR(Mat, matrix);
	FIELD(line, integer);
	FIELD(fmpoly, integer);
	FIELD(numfmpolys, integer);
	FIELD(poly, integer);
	FIELD(numpolys, integer);
	FIELD(light, integer);
	FIELD(lightsize, number);
	FIELD(r, number);
	FIELD(g, number);
	FIELD(b, number);
	FIELD(Bounces, integer);
	FIELD(TimeInterval, number);
	FIELD(TimeCount, number);
	FIELD(FirePoint, integer);
	FIELD(SpotFX, integer);
	FIELD(FramelagAddition, number);
	return luaL_argerror(L, 2, "unknown field name");
}

#define GETTABLEFORSEC(index) do {               \
	lua_getglobal(L, "weapons");               \
	lua_pushinteger(L, index + 1 + SECOFFSET); \
	lua_gettable(L, -2);                       \
	lua_remove(L, -2);                         \
} while (0)

#define SECFLAG(name) do {                                 \
	lua_pushboolean(L, bullet->Flags & SECFLAGS_ ## name); \
	lua_setfield(L, -2, #name);                            \
} while (0)

static int luasecbull_index(lua_State *L)
{
	SECONDARYWEAPONBULLET *bullet;
	const char *name;
	int *id;
	int bullidx = *((int *) luaL_checkudata(L, 1, "SECONDARYWEAPONBULLETIDX"));
	bullet = &SecBulls[bullidx];
	name = luaL_checkstring(L, 2);
	/*if (!strcmp(name, "table")) -- TODO
	{
		lua_pushcfunction(L, luasecbull_table);
		return 1;
	}
	else*/ if (!strcmp(name, "index") || !strcmp(name, "Index"))
	{
		lua_pushinteger(L, bullidx + 1 + MAXPRIMARYWEAPONBULLETS);
		return 1;
	}
	else if (!strcmp(name, "category"))
	{
		lua_pushliteral(L, "secondary");
		return 1;
	}
	FIELD(Used, boolean);
	if (!strcmp(name, "Next") && bullet->Next != (u_int16_t) -1)
	{
		pushsecbull(L, bullet->Next);
		return 1;
	}
	else if (!strcmp(name, "Prev") && bullet->Prev != (u_int16_t) -1)
	{
		pushsecbull(L, bullet->Prev);
		return 1;
	}
	else if (!strcmp(name, "NextInGroup") && bullet->NextInGroup != NULL)
	{
		pushsecbull(L, bullet->NextInGroup - SecBulls);
		return 1;
	}
	else if (!strcmp(name, "PrevInGroup") && bullet->PrevInGroup != NULL)
	{
		pushsecbull(L, bullet->PrevInGroup - SecBulls);
		return 1;
	}
	else if (!strcmp(name, "Flags"))
	{
		/* FIXME: not really consistent */
		lua_createtable(L, 0, 3);
		SECFLAG(CanShoot);
		SECFLAG(CanTarget);
		SECFLAG(NoIncoming);
		return 1;
	}
	FIELD(SecType, integer);
	if (!strcmp(name, "Type"))
	{
		GETTABLEFORSEC(bullet->Type);
		return 1;
	}
	FIELD(MoveType, integer);
	FIELD(State, integer);
	FIELD(OwnerType, integer);
	if (!strcmp(name, "Owner"))
		return pushentity(L, bullet->OwnerType, bullet->Owner);
	FIELD(ID, integer);
	if (!strcmp(name, "Weapon"))
	{
		GETTABLEFORSEC(bullet->Weapon);
		return 1;
	}
	FIELD(Lensflare, boolean);
	FIELD(LifeCount, number);
	FIELD(LifeSpan, number);
	FIELD(SpeedInc, number);
	FIELD(SpeedWanted, number);
	FIELD(Speed, number);
	FIELD(DropCount, number);
	FIELDPTR(DropVector, vector);
	FIELD(Size, number);
	FIELDPTR(Pos, vector);
	FIELDPTR(Offset, vector);
	FIELDPTR(StartPos, vector);
	FIELDPTR(StartDir, vector);
	FIELDPTR(StartMat, matrix);
	FIELDPTR(ColStart, vector);
	FIELD(ColDist, number);
	FIELD(ColFlag, integer);
	FIELD(ColGroup, integer);
	FIELDPTR(ColPoint, vector); /* VERT */
	FIELDPTR(ColPointNormal, vector); /* NORMAL */
	FIELD(GroupImIn, integer);
	FIELD(ModelNum, integer);
	FIELD(ModelIndex, integer);
	FIELDPTR(Mat, matrix);
	FIELD(fmpoly, integer);
	FIELD(numfmpolys, integer);
	FIELD(poly, integer);
	FIELD(numpolys, integer);
	FIELD(xsize, number);
	FIELD(ysize, number);
	FIELD(light, integer);
	FIELD(lightsize, number);
	FIELD(r, number);
	FIELD(g, number);
	FIELD(b, number);
	FIELD(TurnSpeed, number);
	FIELD(ViewCone, number);
	FIELD(TargetType, integer);
	if (!strcmp(name, "Target"))
		return pushentity(L, bullet->TargetType, bullet->Target);
	/* FIELDPTR(DirQuat, quat); -- TODO */
	FIELDPTR(DirVector, vector);
	FIELDPTR(UpVector, vector);
	FIELD(Shield, number);
	FIELD(Damage, number);
	FIELD(Ammo, integer);
	FIELD(NumBounces, integer);
	FIELD(NumOldPos, integer);
	FIELD(RetractPos, number);
	FIELD(ColRadius, number);
	FIELD(Interval, number);
	FIELD(Time, number);
	FIELD(FramelagAddition, number);
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

static int luaprimbull_equals(lua_State *L)
{
	lua_pushboolean(L,
		isudatatype(L, 1, "PRIMARYWEAPONBULLETIDX") &&
		isudatatype(L, 2, "PRIMARYWEAPONBULLETIDX") &&
		*((int *) lua_touserdata(L, 1)) == *((int *) lua_touserdata(L, 2))
	);
	return 1;
}

static int luasecbull_equals(lua_State *L)
{
	lua_pushboolean(L,
		isudatatype(L, 1, "SECONDARYWEAPONBULLETIDX") &&
		isudatatype(L, 2, "SECONDARYWEAPONBULLETIDX") &&
		*((int *) lua_touserdata(L, 1)) == *((int *) lua_touserdata(L, 2))
	);
	return 1;
}

int luaopen_bullets(lua_State *L)
{
	static const luaL_Reg primbullmt[] = {
		{ "__index", luaprimbull_index },
		{ "__eq", luaprimbull_equals },
		{ NULL, NULL }
	}, secbullmt[] = {
		{ "__index", luasecbull_index },
		{ "__eq", luasecbull_equals },
		{ NULL, NULL }
	};
	int i;

	luaL_newmetatable(L, "PRIMARYWEAPONBULLETIDX");
	luaL_register(L, NULL, primbullmt);
	lua_pop(L, 1);
	luaL_newmetatable(L, "SECONDARYWEAPONBULLETIDX");
	luaL_register(L, NULL, secbullmt);
	lua_pop(L, 1);
	lua_createtable(L, MAXPRIMARYWEAPONBULLETS + MAXSECONDARYWEAPONBULLETS, 0);
	for (i=0; i<MAXPRIMARYWEAPONBULLETS; i++)
	{
		lua_pushinteger(L, i+1);
		pushprimbull(L, i);
		lua_settable(L, -3);
	}
	for (i=0; i<MAXSECONDARYWEAPONBULLETS; i++)
	{
		lua_pushinteger(L, i+1 + MAXPRIMARYWEAPONBULLETS);
		pushsecbull(L, i);
		lua_settable(L, -3);
	}
	lua_setglobal(L, "bullets");
	return 0;
}
