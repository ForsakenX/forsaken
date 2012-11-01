/* Lua bindings for collision detection functions.
 *
 * collided, endpos, endgroup, normal, newtarget =
 *     bgcollide(startpos, startgroup, move_off, bgcol)
 *
 * me = Ships[WhoIAm()]
 * meobj = me.Object
 * if wouldcollide(meobj, meobj.Mat * vec(0, 0, 100)) then
 *     print("I would collide if I moved forward 100 units")
 * end
 *
 * see also collision.h
 */
#include <lua.h>
#include <lauxlib.h>
#include "object.h" /* FIXME: needed by collision.h */
#include "collision.h"
#include "ships.h" /* for SHIP_RADIUS */
#include "lua_vecmat.h"

extern MCLOADHEADER MCloadheadert0;
extern MLOADHEADER Mloadheader;

/* [-0, +1, m] */
#define NEWOBJ(L, objtype, var) do {           \
	var = lua_newuserdata(L, sizeof(objtype)); \
	luaL_getmetatable(L, #objtype);            \
	lua_setmetatable(L, -2);                   \
} while (0)

static int luacoll_bgcollide(lua_State *L)
{
	VECTOR *startpos, *move_off, *endpos, *normal, *newtarget;
	u_int16_t startgroup, endgroup;
	bool bgcol;

	/* get input parameters */
	startpos = luaL_checkudata(L, 1, "VECTOR");
	startgroup = luaL_checkint(L, 2);
	move_off = luaL_checkudata(L, 3, "VECTOR");
	(void) luaL_checkint(L, 4);
	bgcol = lua_toboolean(L, 4);

	/* initialize userdata memory for output values */
	NEWOBJ(L, VECTOR, endpos);
	NEWOBJ(L, VECTOR, normal);
	NEWOBJ(L, VECTOR, newtarget);

	/* call collision check function and push result as first output value */
	lua_pushboolean(L,
		BackgroundCollide(
			&MCloadheadert0, &Mloadheader, /* fixed arguments */
			startpos, startgroup, move_off, /* inputs */
			endpos, &endgroup, (NORMAL *) normal, newtarget, /* outputs */
			bgcol, NULL
		)
	);

	/* finally, push the other outputs */
	lua_pushvector(L, endpos);
	lua_pushinteger(L, endgroup);
	lua_pushvector(L, normal);
	lua_pushvector(L, newtarget);

	return 5;
}

static int luacoll_wouldcollide(lua_State *L)
{
	OBJECT *obj;
	VECTOR *move_off;
	float radius;

	obj = luaL_checkudata(L, 1, "OBJECT");
	move_off = luaL_checkudata(L, 2, "VECTOR");
	if (lua_isnoneornil(L, 3))
		radius = SHIP_RADIUS;
	else
		radius = luaL_checknumber(L, 3);

	lua_pushboolean(L, WouldObjectCollide(obj, move_off, radius, NULL));
	return 1;
}

int luaopen_collision(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{ "bgcollide", luacoll_bgcollide },
		{ "wouldcollide", luacoll_wouldcollide },
		{ NULL, NULL }
	};
	int i;

	for (i=0; funcs[i].name; i++)
	{
		lua_pushcfunction(L, funcs[i].func);
		lua_setglobal(L, funcs[i].name);
	}

	return 0;
}
