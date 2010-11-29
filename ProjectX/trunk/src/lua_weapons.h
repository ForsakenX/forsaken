#ifndef LUA_WEAPONS_INCLUDED
#define LUA_WEAPONS_INCLUDED

#include <lua.h>
#include "main.h" /* FIXME -- needed for primary.h and secondary.h */
#include "2dtextures.h" /* FIXME -- needed for primary.h and secondary.h */
#include "new3d.h" /* FIXME -- needed for primary.h and secondary.h */
#include "object.h" /* FIXME -- needed for networking.h */
#include "networking.h" /* FIXME -- needed for primary.h and secondary.h */
#include "primary.h"
#include "secondary.h"

#define SECOFFSET TOTALPRIMARYWEAPONS

int luaopen_weapons(lua_State *L);

#endif /* LUA_WEAPONS_INCLUDED */
