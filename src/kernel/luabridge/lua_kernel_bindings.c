#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_kernel.h"
#include <string.h>

/* Forward declaration */
extern int printk(const char *fmt, ...);

/* Lua function: print(msg) - calls kernel printk */
static int lua_kernel_print(lua_State *L) {
    const char *msg = luaL_checkstring(L, 1);
    printk("%s\n", msg);
    return 0;
}

/* Lua function: printk(fmt, ...) - direct printk access */
static int lua_kernel_printk(lua_State *L) {
    const char *fmt = luaL_checkstring(L, 1);
    
    /* Simple version: only handle %d, %s, %x */
    int num_args = lua_gettop(L) - 1;
    
    /* For simplicity, just support one format specifier */
    if (num_args > 0) {
        if (strstr(fmt, "%d") != NULL) {
            int val = luaL_checkinteger(L, 2);
            printk(fmt, val);
        } else if (strstr(fmt, "%s") != NULL) {
            const char *val = luaL_checkstring(L, 2);
            printk(fmt, val);
        } else if (strstr(fmt, "%x") != NULL) {
            long val = luaL_checkinteger(L, 2);
            printk(fmt, val);
        } else {
            printk("%s", fmt);
        }
    } else {
        printk("%s", fmt);
    }
    
    return 0;
}

/* Initialize kernel library for Lua */
static const luaL_Reg kernel_lib[] = {
    {"print", lua_kernel_print},
    {"printk", lua_kernel_printk},
    {NULL, NULL}
};

int luaopen_kernel(lua_State *L) {
    luaL_newlib(L, kernel_lib);
    return 1;
}
