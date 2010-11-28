#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include "new3d.h"

/*
#define FIELD(objtype, name, datatype)          \
	static int luavec_get ## name(lua_State *L) \
	{                                           \
		objtype *o;                             \
		o = luaL_checkudata(L, 1, #objtype);    \
		lua_push ## datatype(L, o->name);       \
		return 1;                               \
	}

FIELD(VECTOR, x, number)
FIELD(VECTOR, y, number)
FIELD(VECTOR, z, number)
*/

/* [-0, +1, m] */
#define NEWOBJ(L, objtype, var) do {           \
	var = lua_newuserdata(L, sizeof(objtype)); \
	luaL_getmetatable(L, #objtype);            \
	lua_setmetatable(L, -2);                   \
} while (0)

/* Vector functions */

static int luavec_crossproduct(lua_State *L)
{
	VECTOR *v1, *v2, *vr;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	v2 = luaL_checkudata(L, 2, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	CrossProduct(v1, v2, vr);
	return 1;
}

static int luavec_normalize(lua_State *L)
{
	VECTOR *v, *vr;
	v = luaL_checkudata(L, 1, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	vr->x = v->x;
	vr->y = v->y;
	vr->z = v->z;
	NormaliseVector(vr);
	return 1;
}

static int luavec_reflect(lua_State *L)
{
	VECTOR *v, *normal, *vr;
	v = luaL_checkudata(L, 1, "VECTOR");
	normal = luaL_checkudata(L, 2, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	ReflectVector(v, (NORMAL *) normal, vr);
	return 1;
}

static int luavec_length(lua_State *L)
{
	VECTOR *v;
	v = luaL_checkudata(L, 1, "VECTOR");
	lua_pushnumber(L, VectorLength(v));
	return 1;
}

static int luavec_distance(lua_State *L)
{
	VECTOR *v1, *v2;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	v2 = luaL_checkudata(L, 2, "VECTOR");
	lua_pushnumber(L, DistanceVector2Vector(v1, v2));
	return 1;
}

static int luavec_new(lua_State *L)
{
	VECTOR *v;
	NEWOBJ(L, VECTOR, v);
	v->x = luaL_checknumber(L, 1);
	v->y = luaL_checknumber(L, 2);
	v->z = luaL_checknumber(L, 3);
	return 1;
}

static int luavec_unpack(lua_State *L)
{
	VECTOR *v;
	v = luaL_checkudata(L, 1, "VECTOR");
	lua_pushnumber(L, v->x);
	lua_pushnumber(L, v->y);
	lua_pushnumber(L, v->z);
	return 3;
}

static int luavec_tostring(lua_State *L)
{
	VECTOR *v;
	luaL_Buffer b;
	v = luaL_checkudata(L, 1, "VECTOR");
	luaL_buffinit(L, &b);
	luaL_addstring(&b, "vec(");
	lua_pushnumber(L, v->x); luaL_addvalue(&b); luaL_addstring(&b, ", ");
	lua_pushnumber(L, v->y); luaL_addvalue(&b); luaL_addstring(&b, ", ");
	lua_pushnumber(L, v->z); luaL_addvalue(&b);
	luaL_addchar(&b, ')');
	luaL_pushresult(&b);
	return 1;
}

static int luavec_index(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{ "unp", luavec_unpack },
		{ "unpack", luavec_unpack },
		{ "cross", luavec_crossproduct },
		{ "norm", luavec_normalize },
		{ "normalize", luavec_normalize },
		{ "reflect", luavec_reflect },
		{ "len", luavec_length },
		{ "length", luavec_length },
		{ "dist", luavec_distance },
		{ "distance", luavec_distance },
		{ NULL, NULL }
	};
	const luaL_Reg *func;
	VECTOR *v;
	const char *name;
	v = luaL_checkudata(L, 1, "VECTOR");
	name = luaL_checkstring(L, 2);
	if (!strcmp(name, "x"))      { lua_pushnumber(L, v->x); return 1; }
	else if (!strcmp(name, "y")) { lua_pushnumber(L, v->y); return 1; }
	else if (!strcmp(name, "z")) { lua_pushnumber(L, v->z); return 1; }
	for (func = funcs; func->name; func++)
	{
		if (!strcmp(name, func->name))
		{
			lua_pushcfunction(L, func->func);
			return 1;
		}
	}
	return luaL_error(L, "VECTOR has no such method");
}

static int luavec_newindex(lua_State *L)
{
	VECTOR *v;
	const char *name;
	lua_Number value;
	v = luaL_checkudata(L, 1, "VECTOR");
	name = luaL_checkstring(L, 2);
	value = luaL_checknumber(L, 3);
	if (!strcmp(name, "x"))
		v->x = value;
	else if (!strcmp(name, "y"))
		v->y = value;
	else if (!strcmp(name, "z"))
		v->z = value;
	else
		return luaL_error(L, "VECTOR has no such field to set");
	return 0;
}

static int luavec_add(lua_State *L)
{
	VECTOR *v1, *v2, *vr;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	v2 = luaL_checkudata(L, 2, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	vr->x = v1->x + v2->x;
	vr->y = v1->y + v2->y;
	vr->z = v1->z + v2->z;
	return 1;
}

static int luavec_sub(lua_State *L)
{
	VECTOR *v1, *v2, *vr;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	v2 = luaL_checkudata(L, 2, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	vr->x = v1->x - v2->x;
	vr->y = v1->y - v2->y;
	vr->z = v1->z - v2->z;
	return 1;
}

static int luavec_mul(lua_State *L)
{
	VECTOR *v1, *v2, *vr;
	lua_Number s;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	if (lua_isnumber(L, 2))
	{
		/* vector * number -> scalar multiplication */
		s = lua_tonumber(L, 2);
		NEWOBJ(L, VECTOR, vr);
		vr->x = s * v1->x;
		vr->y = s * v1->y;
		vr->z = s * v1->z;
		return 1;
	}
	else
	{
		/* vector * vector -> dot product */
		v2 = luaL_checkudata(L, 2, "VECTOR");
		lua_pushnumber(L, DotProduct(v1, v2));
		return 1;
	}
}

static int luavec_div(lua_State *L)
{
	VECTOR *v, *vr;
	lua_Number s;
	v = luaL_checkudata(L, 1, "VECTOR");
	s = luaL_checknumber(L, 2);
	NEWOBJ(L, VECTOR, vr);
	vr->x = v->x / s;
	vr->y = v->y / s;
	vr->z = v->z / s;
	return 1;
}

static int luavec_negate(lua_State *L)
{
	VECTOR *v, *vr;
	v = luaL_checkudata(L, 1, "VECTOR");
	NEWOBJ(L, VECTOR, vr);
	vr->x = -v->x;
	vr->y = -v->y;
	vr->z = -v->z;
	return 1;
}

/* Note: they have to match exactly which may not always
 * be desirable with floating point components. */
static int luavec_equal(lua_State *L)
{
	VECTOR *v1, *v2;
	v1 = luaL_checkudata(L, 1, "VECTOR");
	v2 = luaL_checkudata(L, 2, "VECTOR");
	lua_pushboolean(L,
		v1->x == v2->x &&
		v1->y == v2->y &&
		v1->z == v2->z
	);
	return 1;
}

/* Matrix functions */

static int luamat_array(lua_State *L)
{
	float *f;
	int i;
	f = (float *) luaL_checkudata(L, 1, "MATRIX");
	lua_createtable(L, 16, 0);
	for (i=0; i<16; i++)
	{
		lua_pushinteger(L, i+1);
		lua_pushnumber(L, f[i]);
		lua_settable(L, -3);
	}
	return 1;
}

static int luamat_transpose(lua_State *L)
{
	MATRIX *m1, *m2;
	m1 = luaL_checkudata(L, 1, "MATRIX");
	NEWOBJ(L, MATRIX, m2);
	MatrixTranspose(m1, m2);
	return 1;
}

static int luamat_scale(lua_State *L)
{
	MATRIX *m1, *m2;
	VECTOR *v;
	m1 = luaL_checkudata(L, 1, "MATRIX");
	v = luaL_checkudata(L, 2, "VECTOR");
	NEWOBJ(L, MATRIX, m2);
	memcpy(m2, m1, sizeof(MATRIX));
	ScaleMatrix(m2, v);
	return 1;
}

static int luamat_translate(lua_State *L)
{
	MATRIX *m1, *m2;
	VECTOR *v;
	float x, y, z;
	m1 = luaL_checkudata(L, 1, "MATRIX");
	if (lua_isnumber(L, 2))
	{
		x = luaL_checknumber(L, 2);
		y = luaL_checknumber(L, 3);
		z = luaL_checknumber(L, 4);
		NEWOBJ(L, MATRIX, m2);
		memcpy(m2, m1, sizeof(MATRIX));
		AddMatrixTrans(x, y, z, m2);
	}
	else
	{
		v = luaL_checkudata(L, 2, "VECTOR");
		NEWOBJ(L, MATRIX, m2);
		memcpy(m2, m1, sizeof(MATRIX));
		AddMatrixTrans(v->x, v->y, v->z, m2);
	}
	return 1;
}

static int luamat_new(lua_State *L)
{
	MATRIX *m;
	float *f;
	int i;
	NEWOBJ(L, MATRIX, m);
	f = (float *) m;
	for (i=0; i<16; i++)
		f[i] = luaL_checknumber(L, i);
	return 1;
}

extern MATRIX MATRIX_Identity;
static int luamat_newidentity(lua_State *L)
{
	MATRIX *m;
	NEWOBJ(L, MATRIX, m);
	memcpy(m, &MATRIX_Identity, sizeof(MATRIX));
	return 1;
}

static int luamat_neweuler(lua_State *L)
{
	MATRIX *m;
	VECTOR *v;
	float x, y, z;
	if (lua_isnumber(L, 1))
	{
		x = luaL_checknumber(L, 1);
		y = luaL_checknumber(L, 2);
		z = luaL_checknumber(L, 3);
		NEWOBJ(L, MATRIX, m);
		BuildRotMatrix(x, y, z, m);
	}
	else
	{
		v = luaL_checkudata(L, 1, "VECTOR");
		NEWOBJ(L, MATRIX, m);
		BuildRotMatrix(v->x, v->y, v->z, m);
	}
	return 1;
}

static int luamat_newaxisangle(lua_State *L)
{
	MATRIX *m;
	VECTOR *axis;
	float angle;
	axis = luaL_checkudata(L, 1, "VECTOR");
	angle = luaL_checknumber(L, 2);
	NEWOBJ(L, MATRIX, m);
	MatrixFromAxisAndAngle(angle, axis, m);
	return 1;
}

static int luamat_newview(lua_State *L)
{
	MATRIX *m;
	VECTOR *pos, *dir, *up;
	pos = luaL_checkudata(L, 1, "VECTOR");
	dir = luaL_checkudata(L, 2, "VECTOR");
	up = luaL_checkudata(L, 3, "VECTOR");
	NEWOBJ(L, MATRIX, m);
	MakeViewMatrix(pos, dir, up, m);
	return 1;
}

static int luamat_tostring(lua_State *L)
{
	float *f;
	luaL_Buffer b;
	int i;
	f = luaL_checkudata(L, 1, "MATRIX");
	luaL_buffinit(L, &b);
	luaL_addstring(&b, "mat(");
	for (i=0; i<4; i++)
	{
		lua_pushnumber(L, f[i]);
		luaL_addvalue(&b);
		luaL_addstring(&b, ", ");
	}
	luaL_addstring(&b, "\n    ");
	for (i=4; i<8; i++)
	{
		lua_pushnumber(L, f[i]);
		luaL_addvalue(&b);
		luaL_addstring(&b, ", ");
	}
	luaL_addstring(&b, "\n    ");
	for (i=8; i<12; i++)
	{
		lua_pushnumber(L, f[i]);
		luaL_addvalue(&b);
		luaL_addstring(&b, ", ");
	}
	luaL_addstring(&b, "\n    ");
	for (i=12; i<15; i++)
	{
		lua_pushnumber(L, f[i]);
		luaL_addvalue(&b);
		luaL_addstring(&b, ", ");
	}
	lua_pushnumber(L, f[15]);
	luaL_addvalue(&b);
	luaL_addchar(&b, ')');
	luaL_pushresult(&b);
	return 1;
}

static int luamat_index(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{ "array", luamat_array },
		{ "flip", luamat_transpose },
		{ "transpose", luamat_transpose },
		{ "scale", luamat_scale },
		{ "move", luamat_translate },
		{ "translate", luamat_translate },
		{ NULL, NULL }
	};
	const luaL_Reg *func;
	const char *name = luaL_checkstring(L, 2);
	for (func = funcs; func->name; func++)
	{
		if (!strcmp(name, func->name))
		{
			lua_pushcfunction(L, func->func);
			return 1;
		}
	}
	return luaL_error(L, "MATRIX has no such method");
}

static int luamat_mul(lua_State *L)
{
	MATRIX *m1;
	void *x2, *xr;
	m1 = luaL_checkudata(L, 1, "MATRIX");
	x2 = lua_touserdata(L, 2);
	if (lua_getmetatable(L, 2))
	{
		lua_getfield(L, LUA_REGISTRYINDEX, "MATRIX");
		if (lua_rawequal(L, -1, -2))
		{
			/* Matrix multiplication */
			NEWOBJ(L, MATRIX, xr);
			MatrixMultiply(m1, (MATRIX *) x2, (MATRIX *) xr);
			return 1;
		}
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, "VECTOR");
		if (lua_rawequal(L, -1, -2))
		{
			/* Apply matrix to vector */
			NEWOBJ(L, VECTOR, xr);
			ApplyMatrix(m1, (VECTOR *) x2, (VECTOR *) xr);
			return 1;
		}
	}
	return luaL_argerror(L, 2, "expected matrix or vector on right side");
}

int luaopen_vecmat(lua_State *L)
{
	static const luaL_Reg vecmt[] = {
		{ "__tostring", luavec_tostring },
		{ "__index", luavec_index },
		{ "__newindex", luavec_newindex },
		{ "__len", luavec_length },
		{ "__add", luavec_add },
		{ "__sub", luavec_sub },
		{ "__mul", luavec_mul },
		{ "__div", luavec_div },
		{ "__unm", luavec_negate },
		{ "__eq", luavec_equal },
		{ NULL, NULL }
	};
	static const luaL_Reg matfuncs[] = {
		{ "mat", luamat_new },
		{ "matid", luamat_newidentity },
		{ "mateuler", luamat_neweuler },
		{ "mataa", luamat_newaxisangle },
		{ "matview", luamat_newview },
		{ NULL, NULL }
	};
	static const luaL_Reg matmt[] = {
		{ "__tostring", luamat_tostring },
		{ "__index", luamat_index },
		{ "__mul", luamat_mul },
		{ NULL, NULL }
	};
	const luaL_Reg *func;
	lua_pushcfunction(L, luavec_new);
	lua_setglobal(L, "vec");
	for (func = matfuncs; func->name; func++)
	{
		lua_pushcfunction(L, func->func);
		lua_setglobal(L, func->name);
	}
	luaL_newmetatable(L, "VECTOR");
	luaL_register(L, NULL, vecmt);
	lua_pop(L, 1);
	luaL_newmetatable(L, "MATRIX");
	luaL_register(L, NULL, matmt);
	lua_pop(L, 1);
	return 0;
}

/* [-0, +1, m] */
void lua_pushvector(lua_State *L, VECTOR *v)
{
	VECTOR *nv = lua_newuserdata(L, sizeof(VECTOR));
	nv->x = v->x;
	nv->y = v->y;
	nv->z = v->z;
	luaL_getmetatable(L, "VECTOR");
	lua_setmetatable(L, -2);
}

/* [-0, +1, m] */
void lua_pushmatrix(lua_State *L, MATRIX *m)
{
	MATRIX *nm = lua_newuserdata(L, sizeof(MATRIX));
	memcpy(nm, m, sizeof(MATRIX));
	luaL_getmetatable(L, "MATRIX");
	lua_setmetatable(L, -2);
}
