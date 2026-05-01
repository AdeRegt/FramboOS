#include <stddef.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_kernel.h"

static lua_State *kernel_lua_state = NULL;

/* Kernel library registration */
extern int luaopen_kernel(lua_State *L);
extern int luaopen_base(lua_State *L);

/* Initialize Lua runtime in kernel */
lua_State* lua_kernel_init(void)
{
    if (kernel_lua_state != NULL) {
        return kernel_lua_state;  /* Already initialized */
    }
    
    /* Create new Lua state with kernel allocator */
    kernel_lua_state = lua_newstate(lua_alloc, NULL, 0);
    if (kernel_lua_state == NULL) {
        return NULL;  /* Allocation failed */
    }
    
    /* Open only base library */
    lua_pushcfunction(kernel_lua_state, luaopen_base);
    lua_pushstring(kernel_lua_state, "");
    lua_call(kernel_lua_state, 1, 0);
    
    /* Register kernel library */
    lua_getglobal(kernel_lua_state, "package");
    lua_getfield(kernel_lua_state, -1, "preload");
    lua_pushcfunction(kernel_lua_state, luaopen_kernel);
    lua_setfield(kernel_lua_state, -2, "kernel");
    lua_pop(kernel_lua_state, 2);
    
    /* Load kernel library into global namespace */
    luaL_requiref(kernel_lua_state, "kernel", luaopen_kernel, 1);
    lua_pop(kernel_lua_state, 1);
    
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
    if (kernel_lua_state == NULL) {
        return -1;  /* Not initialized */
    }
    
    return luaL_dostring(kernel_lua_state, chunk);
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