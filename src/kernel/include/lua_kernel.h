#ifndef LUA_KERNEL_H
#define LUA_KERNEL_H

#include "lua.h"

lua_State* lua_kernel_init(void);
lua_State* lua_kernel_get_state(void);
int lua_kernel_dostring(const char *chunk);
int lua_kernel_dofile(const char *filename);
void lua_kernel_close(void);
void* lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);

#endif