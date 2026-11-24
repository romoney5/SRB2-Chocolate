// http://lua-users.org/wiki/CompatibilityWithLuaFive
#define lua_getref(L, ref) lua_rawgeti(L, LUA_REGISTRYINDEX, ref)

// https://www.lua.org/manual/5.1/manual.html
#define luaL_getn(L, index) lua_objlen(L, index)
