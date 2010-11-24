/* Functions for accessing ship controls from Lua.
 *
 * print(controls.pitch)
 * controls.yaw = 1
 * t = controls.table()
 *
 * see also controls.h
 */
#include <lua.h>
#include <lauxlib.h>
#include "controls.h"

extern SHIPCONTROL control;

#define FIELD(f, type) do {       \
	lua_push ## type(L, ctrl->f); \
	lua_setfield(L, -2, #f);      \
} while (0)
static int luactrl_table(lua_State *L)
{
	SHIPCONTROL *ctrl;
	ctrl = *((SHIPCONTROL **) luaL_checkudata(L, 1, "SHIPCONTROLPTR"));
	lua_createtable(L, 0, 24);
	FIELD(pitch, number);
	FIELD(yaw, number);
	FIELD(roll, number);
	FIELD(bank, number);
	FIELD(right, number);
	FIELD(up, number);
	FIELD(forward, number);
	FIELD(cruise_control, integer);
	FIELD(turbo, integer);
	FIELD(slide_mode, integer);
	FIELD(roll_mode, integer);
	FIELD(fire_primary, integer);
	FIELD(fire_secondary, integer);
	FIELD(fire_mine, integer);
	FIELD(select_primary, integer);
	FIELD(select_secondary, integer);
	FIELD(select_next_primary, integer);
	FIELD(select_prev_primary, integer);
	FIELD(select_next_secondary, integer);
	FIELD(select_prev_secondary, integer);
	FIELD(drop_primary, integer);
	FIELD(drop_secondary, integer);
	FIELD(drop_shield, integer);
	FIELD(drop_ammo, integer);
	return 1;
}
#undef FIELD

#define FIELD(f, type) do {           \
	if (!strcmp(name, #f))            \
	{                                 \
		lua_push ## type(L, ctrl->f); \
		return 1;                     \
	}                                 \
} while (0)
static int luactrl_index(lua_State *L)
{
	SHIPCONTROL *ctrl;
	const char *name;
	ctrl = *((SHIPCONTROL **) luaL_checkudata(L, 1, "SHIPCONTROLPTR"));
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "table"))
	{
		lua_pushcfunction(L, luactrl_table);
		return 1;
	}
	FIELD(pitch, number);
	FIELD(yaw, number);
	FIELD(roll, number);
	FIELD(bank, number);
	FIELD(right, number);
	FIELD(up, number);
	FIELD(forward, number);
	FIELD(cruise_control, integer);
	FIELD(turbo, integer);
	FIELD(slide_mode, integer);
	FIELD(roll_mode, integer);
	FIELD(fire_primary, integer);
	FIELD(fire_secondary, integer);
	FIELD(fire_mine, integer);
	FIELD(select_primary, integer);
	FIELD(select_secondary, integer);
	FIELD(select_next_primary, integer);
	FIELD(select_prev_primary, integer);
	FIELD(select_next_secondary, integer);
	FIELD(select_prev_secondary, integer);
	FIELD(drop_primary, integer);
	FIELD(drop_secondary, integer);
	FIELD(drop_shield, integer);
	FIELD(drop_ammo, integer);
	return 1;
}
#undef FIELD

#define FIELD(f, type) do {                 \
	if (!strcmp(name, #f))                  \
	{                                       \
		ctrl->f = luaL_check ## type(L, 3); \
		return 0;                           \
	}                                       \
} while (0)
static int luactrl_newindex(lua_State *L)
{
	SHIPCONTROL *ctrl;
	const char *name;
	ctrl = *((SHIPCONTROL **) luaL_checkudata(L, 1, "SHIPCONTROLPTR"));
	name = luaL_checkstring(L, 2);
	FIELD(pitch, number);
	FIELD(yaw, number);
	FIELD(roll, number);
	FIELD(bank, number);
	FIELD(right, number);
	FIELD(up, number);
	FIELD(forward, number);
	FIELD(cruise_control, integer);
	FIELD(turbo, integer);
	FIELD(slide_mode, integer);
	FIELD(roll_mode, integer);
	FIELD(fire_primary, integer);
	FIELD(fire_secondary, integer);
	FIELD(fire_mine, integer);
	FIELD(select_primary, integer);
	FIELD(select_secondary, integer);
	FIELD(select_next_primary, integer);
	FIELD(select_prev_primary, integer);
	FIELD(select_next_secondary, integer);
	FIELD(select_prev_secondary, integer);
	FIELD(drop_primary, integer);
	FIELD(drop_secondary, integer);
	FIELD(drop_shield, integer);
	FIELD(drop_ammo, integer);
	return 1;
}
#undef FIELD

int luaopen_controls(lua_State *L)
{
	static const luaL_Reg ctrlmt[] = {
		{ "__index", luactrl_index },
		{ "__newindex", luactrl_newindex },
		{ NULL, NULL }
	};
	SHIPCONTROL **ctrl = lua_newuserdata(L, sizeof(void *));
	*ctrl = &control;
	luaL_newmetatable(L, "SHIPCONTROLPTR");
	luaL_register(L, NULL, ctrlmt);
	lua_setmetatable(L, -2);
	lua_setglobal(L, "control");
	return 0;
}
