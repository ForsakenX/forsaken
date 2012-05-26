#ifdef MINIUPNP

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <lua.h>
#include <lauxlib.h>
#include <miniupnpc.h>
#include <upnpcommands.h>
#include <upnperrors.h>
#include "util.h"

#define DEFAULT_DISCOVERY_DELAY 2000 /* in milliseconds */

static int luaupnp_discover(lua_State *L)
{
	int delay;
	struct UPNPDev *dev, *igddev;
	struct UPNPUrls urls;
	struct IGDdatas data;
	char lanaddr[64];
	int ret;

	delay = luaL_optint(L, 1, DEFAULT_DISCOVERY_DELAY);
	dev = upnpDiscover(delay, NULL, NULL, 1);
	switch (UPNP_GetValidIGD(dev, &urls, &data, lanaddr, sizeof(lanaddr)))
	{
		case 3:
		case 2:
			DebugPrintf("Returning possibly invalid IGD/UPnP device %s\n",
				urls.controlURL);
		case 1:
			lua_createtable(L, 0, 2);
			/* Try to find the correct device since miniupnp
			 * doesn't expose this data directly. :-/ */
			for (
				igddev = dev;
				igddev && strcmp(igddev->st,
					"urn:schemas-upnp-org:device:InternetGatewayDevice:1");
				igddev = igddev->pNext);
			lua_pushstring(L, igddev->descURL);
			lua_setfield(L, -2, "descURL");
			lua_pushstring(L, urls.controlURL);
			lua_setfield(L, -2, "controlURL");
			lua_pushstring(L, urls.controlURL_CIF);
			lua_setfield(L, -2, "controlURL_CIF");
			lua_pushstring(L, data.first.servicetype);
			lua_setfield(L, -2, "servicetype");
			lua_pushstring(L, data.CIF.servicetype);
			lua_setfield(L, -2, "servicetype_CIF");
			lua_pushstring(L, lanaddr);
			lua_setfield(L, -2, "lanaddr");
			luaL_getmetatable(L, "upnp_igd_mt");
			lua_setmetatable(L, -2);
			ret = 1;
			break;
		default:
			lua_pushnil(L);
			lua_pushliteral(L, "no routers found");
			ret = 2;
	}

	freeUPNPDevlist(dev);
	return ret;
}

static int luaupnp_getigdat(lua_State *L)
{
	const char *descURL;
	struct UPNPUrls urls;
	struct IGDdatas data;
	char lanaddr[64];

	descURL = luaL_checkstring(L, 1);
	if (UPNP_GetIGDFromUrl(descURL, &urls, &data, lanaddr, sizeof(lanaddr)))
	{
		lua_createtable(L, 0, 2);
		lua_pushvalue(L, 1);
		lua_setfield(L, -2, "descURL");
		lua_pushstring(L, urls.controlURL);
		lua_setfield(L, -2, "controlURL");
		lua_pushstring(L, urls.controlURL_CIF);
		lua_setfield(L, -2, "controlURL_CIF");
		lua_pushstring(L, data.first.servicetype);
		lua_setfield(L, -2, "servicetype");
		lua_pushstring(L, data.CIF.servicetype);
		lua_setfield(L, -2, "servicetype_CIF");
		lua_pushstring(L, lanaddr);
		lua_setfield(L, -2, "lanaddr");
		luaL_getmetatable(L, "upnp_igd_mt");
		lua_setmetatable(L, -2);
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushliteral(L, "no router found at specified address");
		return 2;
	}
}

static void getigddata(lua_State *L, int index, int cif,
	const char **controlURL,
	const char **servicetype,
	const char **lanaddr
)
{
	if (controlURL)
	{
		lua_getfield(L, index, cif ? "controlURL_CIF" : "controlURL");
		*controlURL = luaL_checkstring(L, -1);
		lua_pop(L, 1);
	}
	if (servicetype)
	{
		lua_getfield(L, index, cif ? "servicetype_CIF" : "servicetype");
		*servicetype = luaL_checkstring(L, -1);
		lua_pop(L, 1);
	}
	if (lanaddr)
	{
		lua_getfield(L, index, "lanaddr");
		*lanaddr = luaL_checkstring(L, -1);
		lua_pop(L, 1);
	}
}

static int luaupnp_addportmap(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	const char *lanaddr;
	int port;
	const char *portstr;
	const char *descr;
	const char *proto;
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, &lanaddr);
	port = lua_tointeger(L, 2);
	luaL_argcheck(L, port > 0 && port < 65536, 2, "invalid port number");
	portstr = lua_tostring(L, 2);
	proto = luaL_checkstring(L, 3);
	descr = luaL_optstring(L, 4, "projectx");
	ret = UPNP_AddPortMapping(
		controlURL, servicetype, portstr, portstr, lanaddr, descr, proto, NULL);
	if (!ret)
	{
		lua_pushboolean(L, 1);
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static int luaupnp_rmportmap(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	int port;
	const char *portstr;
	const char *proto;
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);
	port = lua_tointeger(L, 2);
	luaL_argcheck(L, port > 0 && port < 65536, 2, "invalid port number");
	portstr = lua_tostring(L, 2);
	proto = luaL_checkstring(L, 3);
	ret = UPNP_DeletePortMapping(controlURL, servicetype, portstr, proto, NULL);
	if (!ret)
	{
		lua_pushboolean(L, 1);
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static char *inttostr(int n)
{
	static char s[16];
	sprintf(s, "%d", n);
	return s;
}

static int luaupnp_listportmaps(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	char extport[6];
	char localclient[16];
	char localport[6];
	char protocol[4];
	char description[256];
	char enabled[4];
	char remotehost[64];
	char duration[16];
	unsigned int n;
	int ret;
	int i;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);

	if (!UPNP_GetPortMappingNumberOfEntries(controlURL, servicetype, &n))
		lua_createtable(L, n, 0);
	else
		lua_newtable(L);

	for (
		i=0;
		(ret = UPNP_GetGenericPortMappingEntry(
		controlURL, servicetype, inttostr(i),
		extport, localclient, localport, protocol, description, enabled,
		remotehost, duration)) == 0;
		i++)
	{
		lua_pushinteger(L, i+1);
		lua_createtable(L, 0, 8);
		lua_pushinteger(L, atoi(extport));
		lua_setfield(L, -2, "extport");
		lua_pushstring(L, localclient);
		lua_setfield(L, -2, "localclient");
		lua_pushinteger(L, atoi(localport));
		lua_setfield(L, -2, "localport");
		lua_pushstring(L, protocol);
		lua_setfield(L, -2, "protocol");
		lua_pushstring(L, description);
		lua_setfield(L, -2, "description");
		lua_pushboolean(L,
			enabled[0] == '1' ||
			(enabled[0] | 0x20) == 'y' ||
			(enabled[0] | 0x20) == 't');
		lua_setfield(L, -2, "enabled");
		lua_pushstring(L, remotehost);
		lua_setfield(L, -2, "remotehost");
		lua_pushinteger(L, atoi(duration));
		lua_setfield(L, -2, "duration");
		lua_settable(L, -3);
	}

	return 1;
}

static int luaupnp_findportmap(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	int extport;
	const char *extportstr;
	const char *protocol;
	char localclient[16];
	char localport[6];
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);
	extport = lua_tointeger(L, 2);
	luaL_argcheck(L, extport > 0 && extport < 65536, 2, "invalid port number");
	extportstr = lua_tostring(L, 2);
	protocol = luaL_checkstring(L, 3);
	if (!(ret = UPNP_GetSpecificPortMappingEntry(controlURL, servicetype,
		extportstr, protocol, localclient, localport)))
	{
		lua_createtable(L, 0, 2);
		lua_pushstring(L, localclient);
		lua_setfield(L, -2, "localclient");
		lua_pushstring(L, localport);
		lua_setfield(L, -2, "localport");
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static int luaupnp_getwanaddr(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	char wanaddr[16];
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);
	ret = UPNP_GetExternalIPAddress(controlURL, servicetype, wanaddr);
	if (!ret && wanaddr[0])
	{
		lua_pushstring(L, wanaddr);
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static int luaupnp_getconntype(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	char conntype[64];
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);
	ret = UPNP_GetConnectionTypeInfo(controlURL, servicetype, conntype);
	if (!ret && conntype[0])
	{
		lua_pushstring(L, conntype);
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static int luaupnp_getconnstatus(lua_State *L)
{
	const char *controlURL;
	const char *servicetype;
	char status[64];
	unsigned int uptime;
	time_t now;
	char lasterror[64];
	int ret;

	getigddata(L, 1, 0, &controlURL, &servicetype, NULL);
	ret = UPNP_GetStatusInfo(controlURL, servicetype,
		status, &uptime, lasterror);
	if (!ret)
	{
		now = time(NULL);
		lua_createtable(L, 0, 3);
		lua_pushstring(L, status);
		lua_setfield(L, -2, "status");
		lua_pushinteger(L, uptime);
		lua_setfield(L, -2, "uptime");
		lua_pushinteger(L, now - uptime);
		lua_setfield(L, -2, "starttime");
		lua_pushstring(L, lasterror);
		lua_setfield(L, -2, "lasterror");
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

static int luaupnp_getbandwidth(lua_State *L)
{
	const char *controlURL_CIF;
	const char *servicetype_CIF;
	unsigned int down, up;
	int ret;

	getigddata(L, 1, 1, &controlURL_CIF, &servicetype_CIF, NULL);
	ret = UPNP_GetLinkLayerMaxBitRates(controlURL_CIF, servicetype_CIF,
		&down, &up);
	if (!ret)
	{
		lua_createtable(L, 0, 2);
		lua_pushinteger(L, down);
		lua_setfield(L, -2, "downbps");
		lua_pushinteger(L, up);
		lua_setfield(L, -2, "upbps");
		return 1;
	}
	else
	{
		lua_pushnil(L);
		lua_pushfstring(L, "%s (%d)", strupnperror(ret), ret);
		return 2;
	}
}

int luaopen_miniupnp(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{ "discover", luaupnp_discover },
		{ "getigdat", luaupnp_getigdat },
		{ NULL, NULL }
	};
	static const luaL_Reg igd_mt[] = {
		{ "add", luaupnp_addportmap },
		{ "remove", luaupnp_rmportmap },
		{ "list", luaupnp_listportmaps },
		{ "find", luaupnp_findportmap },
		{ "wanaddr", luaupnp_getwanaddr },
		{ "conntype", luaupnp_getconntype },
		{ "connstatus", luaupnp_getconnstatus },
		{ "bandwidth", luaupnp_getbandwidth },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "upnp_igd_mt");
	lua_createtable(L, 0, 3);
	luaL_register(L, NULL, igd_mt);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
	luaL_register(L, "upnp", funcs);
	lua_pop(L, 1);

	return 0;
}

#endif /* MINIUPNP */
