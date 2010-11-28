/* Functions for accessing object data from Lua.
 *
 * me = Ships[WhoIAm()]
 * print(me.Object.Shield)
 * meobj = me.Object:table()
 *
 * see also object.h
 */
#include <lua.h>
#include <lauxlib.h>
#include "object.h"
#include "lua_vecmat.h"

#define FIELD(f, type) do {       \
	lua_push ## type(L, obj->f); \
	lua_setfield(L, -2, #f);      \
} while (0)
#define FIELDPTR(f, type) do {     \
	lua_push ## type(L, &obj->f); \
	lua_setfield(L, -2, #f);       \
} while (0)
static int luaobj_table(lua_State *L)
{
	OBJECT *obj;
	obj = *((OBJECT **) luaL_checkudata(L, 1, "OBJECTPTR"));
	lua_createtable(L, 0, 38);
	FIELD(Flags, integer); /* TODO */
	FIELD(Type, integer); /* TODO - enum */
	/* FIELD(ID, ???); ... TODO */
	FIELD(AI_Mode, integer); /* TODO - enum */
	FIELD(Mode, integer); /* TODO - enum */
	FIELD(ControlType, integer); /* TODO - enum */
	FIELDPTR(Pos, vector);
	/* FIELDPTR(Quat, quat); -- TODO */
	FIELD(Bank, number);
	FIELDPTR(Mat, matrix);
	FIELDPTR(InvMat, matrix);
	FIELDPTR(FinalMat, matrix);
	FIELDPTR(FinalInvMat, matrix);
	/* FIELDPTR(FinalQuat, quat); -- TODO */
	FIELDPTR(DirVector, vector);
	FIELD(Group, integer);
	FIELDPTR(Speed, vector);
	FIELDPTR(Angle, vector);
	FIELD(CruiseControl, integer);
	FIELD(Shield, number);
	FIELD(Hull, number);
	FIELD(Energy, integer);
	FIELD(PowerLevel, integer);
	FIELDPTR(ExternalForce, vector);
	FIELDPTR(InternalForce, vector);
	/* FIELDPTR(RotationForce, quat); -- TODO */
	FIELD(Autolevel, number);
	FIELD(BobCount, number);
	FIELDPTR(LastBob, vector);
	FIELD(light, integer);
	FIELD(NodeNetwork, integer);
	/* FIELD(NearestNode, ???); ... TODO */
	/* FIELD(LastNearestNode, ???); ... TODO */
	FIELD(HowManyGuns, integer);
	/* FIELD(FirstGun, gunobject) ... TODO (?) */
	FIELD(Time, number);
	FIELD(OverallTime, number);
	FIELD(AnimSpeed, number);
	FIELDPTR(TopLeft, vector);
	FIELDPTR(BottomRight, vector);
	/* FIELD(AnimSeqs, animseq) ... TODO (?) */
	FIELD(CurAnimSeq, integer);
	FIELD(Animating, boolean);
	/* FIELD(Components, compobj) ... TODO (?) */
	/* FIELD(UserContComps, compobj) ... TODO (?) */
	FIELD(Red, number);
	FIELD(Green, number);
	FIELD(Blue, number);
	return 1;
}
#undef FIELDPTR
#undef FIELD

#define FIELD(f, type) do {           \
	if (!strcmp(name, #f))            \
	{                                 \
		lua_push ## type(L, obj->f); \
		return 1;                     \
	}                                 \
} while (0)
#define FIELDPTR(f, type) do {         \
	if (!strcmp(name, #f))             \
	{                                  \
		lua_push ## type(L, &obj->f); \
		return 1;                      \
	}                                  \
} while (0)
static int luaobj_index(lua_State *L)
{
	OBJECT *obj;
	const char *name;
	obj = *((OBJECT **) luaL_checkudata(L, 1, "OBJECTPTR"));
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "table"))
	{
		lua_pushcfunction(L, luaobj_table);
		return 1;
	}
	FIELD(Flags, integer); /* TODO */
	FIELD(Type, integer); /* TODO - enum */
	/* FIELD(ID, ???); ... TODO */
	FIELD(AI_Mode, integer); /* TODO - enum */
	FIELD(Mode, integer); /* TODO - enum */
	FIELD(ControlType, integer); /* TODO - enum */
	FIELDPTR(Pos, vector);
	/* FIELDPTR(Quat, quat); -- TODO */
	FIELD(Bank, number);
	FIELDPTR(Mat, matrix);
	FIELDPTR(InvMat, matrix);
	FIELDPTR(FinalMat, matrix);
	FIELDPTR(FinalInvMat, matrix);
	/* FIELDPTR(FinalQuat, quat); -- TODO */
	FIELDPTR(DirVector, vector);
	FIELD(Group, integer);
	FIELDPTR(Speed, vector);
	FIELDPTR(Angle, vector);
	FIELD(CruiseControl, integer);
	FIELD(Shield, number);
	FIELD(Hull, number);
	FIELD(Energy, integer);
	FIELD(PowerLevel, integer);
	FIELDPTR(ExternalForce, vector);
	FIELDPTR(InternalForce, vector);
	/* FIELDPTR(RotationForce, quat); -- TODO */
	FIELD(Autolevel, number);
	FIELD(BobCount, number);
	FIELDPTR(LastBob, vector);
	FIELD(light, integer);
	FIELD(NodeNetwork, integer);
	/* FIELD(NearestNode, ???); ... TODO */
	/* FIELD(LastNearestNode, ???); ... TODO */
	FIELD(HowManyGuns, integer);
	/* FIELD(FirstGun, gunobject) ... TODO (?) */
	FIELD(Time, number);
	FIELD(OverallTime, number);
	FIELD(AnimSpeed, number);
	FIELDPTR(TopLeft, vector);
	FIELDPTR(BottomRight, vector);
	/* FIELD(AnimSeqs, animseq) ... TODO (?) */
	FIELD(CurAnimSeq, integer);
	FIELD(Animating, boolean);
	/* FIELD(Components, compobj) ... TODO (?) */
	/* FIELD(UserContComps, compobj) ... TODO (?) */
	FIELD(Red, number);
	FIELD(Green, number);
	FIELD(Blue, number);
	return luaL_argerror(L, 2, "unknown field name");
}
#undef FIELDPTR
#undef FIELD

int luaopen_object(lua_State *L)
{
	static const luaL_Reg objmt[] = {
		{ "__index", luaobj_index },
		{ NULL, NULL }
	};
	luaL_newmetatable(L, "OBJECTPTR");
	luaL_register(L, NULL, objmt);
	lua_pop(L, 1);
	return 0;
}
