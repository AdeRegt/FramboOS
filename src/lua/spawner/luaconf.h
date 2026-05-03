#ifndef luaconf_h
#define luaconf_h

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

/* 
** Voorkom dat math.h van het systeem geladen wordt. 
*/
#ifndef _MATH_H
#define _MATH_H
#endif

/*
** ===================================================================
** SYSTEM CONFIGURATION
** ===================================================================
*/

#ifndef LUA_USE_C89
#define LUA_USE_C89
#endif

#undef LUA_USE_LINUX
#undef LUA_USE_WINDOWS
#undef LUA_USE_POSIX

#define l_likely(x) (__builtin_expect(((x) != 0), 1))
#define l_unlikely(x)   (__builtin_expect(((x) != 0), 0))
#define luai_likely(x)  l_likely(x)
#define luai_unlikely(x) l_unlikely(x)

#define LUA_API    extern
#define LUALIB_API LUA_API
#define LUAMOD_API LUA_API
#define LUAI_FUNC  extern
#define LUAI_DDEC(dec)  LUAI_FUNC dec
#define LUAI_DDEF  /* empty */

/*
** ===================================================================
** NUMBER TYPES (64-bit Integer Only)
** ===================================================================
*/

#define LUA_INT_TYPE    3 /* LUA_INT_LONGLONG */
#define LUA_FLOAT_TYPE  1 /* LUA_FLOAT_FLOAT (faked met integers) */

#define LUA_INTEGER     long long
#define LUA_UNSIGNED    unsigned long long
#define LUA_NUMBER      long long

#define LUAI_UACNUMBER  long long
#define LUAI_UACINT     long long

#define LUA_MAXINTEGER  LLONG_MAX
#define LUA_MININTEGER  LLONG_MIN
#define LUA_MAXUNSIGNED ULLONG_MAX

#define LUA_INTEGER_FMT "%lld"
#define LUA_NUMBER_FMT  "%lld"

/*
** ===================================================================
** MATH STUBS & SSE-SAFE l_mathop
** ===================================================================
*/

/* Inline stubs voor kernel-omgeving */
static inline long long l_stub_pow(long long a, long long b) {
    long long res = 1;
    if (b < 0) return 0;
    while (b > 0) {
        if (b & 1) res *= a;
        a *= a; b >>= 1;
    }
    return res;
}
static inline long long l_stub_fmod(long long a, long long b) { return (b != 0) ? (a % b) : 0; }
static inline long long l_stub_frexp(long long a, int *e) { *e = 0; return a; }
static inline long long l_stub_ldexp(long long a, int e) { return a; }

/* Omleiden van standaardnamen */
#define pow     l_stub_pow
#define fmod    l_stub_fmod
#define frexp   l_stub_frexp
#define ldexp   l_stub_ldexp

/* 
** SSE-FIX: 
** Gebruik GCC/Clang built-ins om te voorkomen dat 'double' constanten 
** eindigen in SSE-registers (verboden in kernel-mode).
*/
#if defined(__GNUC__) || defined(__clang__)
  #define l_mathop(x) \
    __builtin_choose_expr(__builtin_types_compatible_p(__typeof__(x), double), \
    ((long long)(x)), (x))
#else
  #define l_mathop(x) (x)
#endif

#define l_floor(x)      (x)
#define l_ceil(x)       (x)

#undef HUGE_VAL
#define HUGE_VAL        ((long long)0x7FFFFFFFFFFFFFFFLL)
#define l_mathlim(n)    LLONG_MAX
#define MANT_DIG        0
#define MAX_10_EXP      0
#define l_floatatt(a)   (0)

/* String conversies */
#define lua_number2str(s,sz,n)  l_sprintf((s), sz, LUA_NUMBER_FMT, (n))
#define lua_integer2str(s,sz,n) l_sprintf((s), sz, LUA_INTEGER_FMT, (n))
#define lua_pointer2str(s,sz,p) l_sprintf((s), sz, "%p", (p))
#define lua_numbertointeger(n,p) (*(p) = (LUA_INTEGER)(n), 1)
#define lua_str2number(s,p)    strtoll((s), (p), 10)

/*
** ===================================================================
** CONTEXT, EXTRASPACE & PATHS
** ===================================================================
*/

#define LUA_KCONTEXT    ptrdiff_t
#define LUA_EXTRASPACE  (sizeof(void *))

#define LUA_PATH_SEP    ";"
#define LUA_PATH_MARK   "?"
#define LUA_DIRSEP      "/"
#define LUA_IGMARK      "-"
#define LUA_EXEC_DIR    "!"
#define LUA_PATH_DEFAULT  ""
#define LUA_CPATH_DEFAULT ""

/*
** ===================================================================
** FREESTANDING STUBS
** ===================================================================
*/

#define lua_getlocaledecpoint() ('.')
#define l_sprintf(s,sz,f,i)    snprintf(s,sz,f,i)

#define LUAI_MAXALIGN          \
  union { LUA_INTEGER i; void *s; long l; }

#define LUAI_MAXSTACK          15000
#define LUA_IDSIZE             60
#define LUAL_BUFFERSIZE        512

#include <setjmp.h>
void printk(char *format, ...);
#define lua_writestring(s,l)   printk(s,l)
#endif