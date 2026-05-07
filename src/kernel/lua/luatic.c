#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
LUALIB_API void (luaL_openlibs) (lua_State *L);

void luatic(void *buffer) {
    // 1. Initialiseer de Lua state
    // Let op: als je luaL_newstate hebt gestript, gebruik dan lua_newstate met je eigen allocator
    lua_State *L = luaL_newstate(); 
    
    if (L == NULL) {
        printk("Lua: Kon state niet initialiseren (geheugenprobleem?)\n");
        return;
    }

    // 2. Open de standaard libraries die je hebt behouden (zoals base en table)
    luaL_openlibs(L);

    // 4. Laad de string en voer hem uit
    // luaL_dostring is een macro die luaL_loadstring en lua_pcall combineert
    int status = luaL_dostring(L, buffer);

    if (status != LUA_OK) {
        // Als er iets misgaat, staat de foutmelding bovenop de stack
        const char *msg = lua_tostring(L, -1);
        printk("Lua Fout: %s\n", msg);
        lua_pop(L, 1); // Ruim de foutmelding op
    }

    // 5. Sluit de state netjes af
    lua_close(L);
}