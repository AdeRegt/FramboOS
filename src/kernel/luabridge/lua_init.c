#include <stddef.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_kernel.h"
#include "debugger.h"

static lua_State *kernel_lua_state = NULL;

/* Kernel library registration */
extern int luaopen_kernel(lua_State *L);
extern int luaopen_base(lua_State *L);

int l_kernel_print(lua_State *L) {
    // 1. Haal het argument van de Lua-stack
    const char *str = luaL_checkstring(L, 1);
    
    // 2. Roep je eigen kernel-functie aan
    printk("[LUA]: %s\n", str);
    
    // 3. Geef het aantal return-waarden door aan Lua
    return 0; 
}

/* Initialize Lua runtime in kernel */
lua_State* lua_kernel_init(void)
{
    printk("DEBUG: lua_kernel_init START\n");
    if (kernel_lua_state != NULL) {
        printk("DEBUG: lua_kernel_init - already initialized\n");
        return kernel_lua_state;  /* Already initialized */
    }
    
    printk("DEBUG: about to lua_newstate\n");
    /* Create new Lua state with kernel allocator */
    kernel_lua_state = luaL_newstate();
    printk("DEBUG: lua_newstate returned %x\n", (uint64_t)kernel_lua_state);
    if (kernel_lua_state == NULL) {
        printk("ERROR: lua_newstate failed\n");
        return NULL;  /* Allocation failed */
    }
    
    printk("DEBUG: about to open base library\n");
    /* Open base library - luaL_dostring needs it */
    lua_pushcfunction(kernel_lua_state, luaopen_base);
    printk("DEBUG: pushed luaopen_base\n");
    lua_pushstring(kernel_lua_state, "");
    printk("DEBUG: pushed empty string\n");
    int result = lua_pcall(kernel_lua_state, 1, 0, 0);
    printk("DEBUG: base library opened, result=%d\n", result);
    if (result != 0) {
        printk("ERROR: Failed to open base library: %s\n", lua_tostring(kernel_lua_state, -1));
        lua_pop(kernel_lua_state, 1);
    }
    
    lua_pushcfunction(kernel_lua_state, l_kernel_print);
    printk("DEBUG: pushed l_kernel_print\n");
    lua_setglobal(kernel_lua_state, "kprint");
    printk("DEBUG: set kprint global\n");
    
    printk("DEBUG: lua_kernel_init COMPLETE\n");
    return kernel_lua_state;
}

/* Get the global Lua state */
lua_State* lua_kernel_get_state(void)
{
    return kernel_lua_state;
}

/* Execute a Lua string/chunk */
int lua_kernel_dostring(const char *chunk)
{
    printk("DEBUG: lua_kernel_dostring START\n");
    if (kernel_lua_state == NULL) {
        printk("ERROR: lua_kernel_dostring - state is NULL\n");
        return -1;  /* Not initialized */
    }
    
    printk("DEBUG: about to luaL_loadstring\n");
    int result = luaL_loadstring(kernel_lua_state, chunk);
    printk("DEBUG: luaL_loadstring returned %d\n", result);
    
    if (result != 0) {
        printk("ERROR: luaL_loadstring failed: %s\n", lua_tostring(kernel_lua_state, -1));
        lua_pop(kernel_lua_state, 1);
        return result;
    }
    
    printk("DEBUG: about to lua_pcall\n");
    result = lua_pcall(kernel_lua_state, 0, 0, 0);
    printk("DEBUG: lua_pcall returned %d\n", result);
    
    if (result != 0) {
        printk("ERROR: lua_pcall failed: %s\n", lua_tostring(kernel_lua_state, -1));
        lua_pop(kernel_lua_state, 1);
    }
    
    return result;
}

/* Execute a Lua file */
int lua_kernel_dofile(const char *filename)
{
    if (kernel_lua_state == NULL) {
        return -1;  /* Not initialized */
    }
    
    return luaL_dofile(kernel_lua_state, filename);
}

/* Cleanup (call before shutdown) */
void lua_kernel_close(void)
{
    if (kernel_lua_state != NULL) {
        lua_close(kernel_lua_state);
        kernel_lua_state = NULL;
    }
}