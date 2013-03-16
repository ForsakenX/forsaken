#ifndef LUA_VECMAT_INCLUDED
#define LUA_VECMAT_INCLUDED

#include <lua.h>
#include "new3d.h"

/* [-0, +1, m] */
void lua_pushvector(lua_State *L, VECTOR *v);

/* [-0, +1, m] */
void lua_pushmatrix(lua_State *L, MATRIX *m);

int luaopen_vecmat(lua_State *L);

#endif /* LUA_VECMAT_INCLUDED */
