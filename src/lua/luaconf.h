/*
** $Id: luaconf.h $
** Configuration file for Lua - KERNEL MODIFIED (Integer Only)
** Geoptimaliseerd voor een freestanding x86_64 kernel omgeving.
*/

#ifndef luaconf_h
#define luaconf_h

#include <limits.h>
#include <stddef.h>

/* 
** ===================================================================
** Systeem Configuratie
** ===================================================================
*/

#if !defined(LUA_ANSI)
#define LUA_ANSI
#endif


/* Prototypes voor kernel stubs */
extern long long kernel_atoll(const char *s);
extern int sprintf(char *str, const char *format, ...);

/* Controleer of int minstens 32 bits is */
#define LUAI_IS32INT    ((UINT_MAX >> 30) >= 3)

/*
** ===================================================================
** Numerieke Types (Integer-Only Modus)
** ===================================================================
*/

#define LUA_INT_INT         1
#define LUA_INT_LONG        2
#define LUA_INT_LONGLONG    3

#define LUA_FLOAT_FLOAT     1
#define LUA_FLOAT_DOUBLE    2
#define LUA_FLOAT_LONGDOUBLE 3

/* Gebruik 64-bit integers voor alles */
#define LUA_INT_TYPE    LUA_INT_LONGLONG
#define LUA_FLOAT_TYPE  LUA_INT_LONGLONG 

/* De basis types */
#define LUA_NUMBER      long long
#define LUA_INTEGER     long long
#define LUA_UNSIGNED    unsigned long long

/* CRUCIAAL: Definieer de 'Unpromoted' types voor de VM */
#define LUAI_UACNUMBER  long long
#define LUAI_UACINT     long long

#define LUA_MAXINTEGER  LLONG_MAX
#define LUA_MININTEGER  LLONG_MIN
#define LUA_MAXUNSIGNED ULLONG_MAX

/* 
** ===================================================================
** Low-level Conversie & Math Macros
** ===================================================================
*/

#define LUA_INTEGER_FRMLEN  "ll"
#define LUA_INTEGER_FMT     "%" LUA_INTEGER_FRMLEN "d"
#define LUA_NUMBER_FRMLEN   "ll"
#define LUA_NUMBER_FMT      "%lld"
#define LUA_NUMBER_FMT_N    "%lld"

#define l_mathop(op)        op
#define l_floor(x)          (x)
#define l_floatatt(n)       (0)

#define lua_str2number(s,p)     ((void)p, kernel_atoll(s))
#define lua_strx2number(s,p)    ((void)p, kernel_atoll(s))

#define lua_integer2str(s,sz,n) sprintf((s), LUA_INTEGER_FMT, (long long)(n))
#define lua_number2str(s,sz,n)  sprintf((s), LUA_NUMBER_FMT, (long long)(n))

#define lua_getlocaledecpoint() ('.')

/*
** ===================================================================
** Pad & Export Instellingen
** ===================================================================
*/

#define LUA_PATH_SEP            ";"
#define LUA_PATH_MARK           "?"
#define LUA_EXEC_DIR            "!"
#define LUA_PATH_DEFAULT        ""
#define LUA_CPATH_DEFAULT       ""
#define LUA_DIRSEP              "/"

#define LUA_API     extern
#define LUALIB_API  LUA_API
#define LUAMOD_API  LUA_API

/*
** ===================================================================
** Interne VM Instellingen
** ===================================================================
*/

#define LUA_KCONTEXT    ptrdiff_t

#define LUA_EXTRASPACE      (sizeof(void *))
#define LUA_IDSIZE          60
#define LUAL_BUFFERSIZE     1024

/* Alignment voor de VM */
#define LUAI_MAXALIGN       void *s; long long l

#if !defined(luai_likely)
#define luai_likely(x)      (x)
#define luai_unlikely(x)    (x)
#endif
/* 
** KERNEL MOD: Fix voor implicit declarations van sprintf-gerelateerde functies
*/

/* Zorg dat l_sprintf bekend is voor lobject.c */
#if !defined(l_sprintf)
  #define l_sprintf(s,sz,f,i) sprintf(s,f,i)
#endif

/* Zorg dat lua_pointer2str bekend is voor lobject.c */
#if !defined(lua_pointer2str)
  #define lua_pointer2str(buff,sz,p) sprintf(buff, "%p", p)
#endif

/* 
** KERNEL MOD: Fix voor Continuation Context 
*/
#define LUA_KCONTEXT    ptrdiff_t/* 
** KERNEL MOD: Schakel OS en I/O afhankelijkheden volledig uit
*/

/* Voorkom dat Lua 'tmpnam' of 'system' gebruikt */
#undef LUA_USE_POSIX
#undef LUA_USE_WINDOWS

/* Forceer Lua om GEEN gebruik te maken van de standaard C streams */
#define LUA_NOCVTN2S
#define LUA_NOCVTS2N

/* 
** Definieer deze macro's als 'leeg' of 'fout' om te voorkomen dat 
** lbaselib.c of liolib.c functies zoals fopen gebruiken.
*/
#define lua_stdin_is_tty()	0
#define lua_readline(L,b,p)	((void)L, (void)b, (void)p, 0)
#define lua_saveline(L,line)	((void)L, (void)line)
#define lua_freeline(L,b)	((void)L, (void)b)/*
** ===================================================================
** KERNEL SPECIFIC OVERRIDES
** ===================================================================
*/

/* 1. Schakel File I/O functies in de core uit */

/* 2. Zorg dat l_sprintf en andere macros geen externe headers nodig hebben */
#if !defined(l_sprintf)
  #define l_sprintf(s,sz,f,i) sprintf(s,f,i)
#endif

#if !defined(lua_pointer2str)
  #define lua_pointer2str(buff,sz,p) sprintf(buff, "%p", p)
#endif

/* 3. Voorkom dat Lua standaard C streams gebruikt (stdin/stdout) */
#define lua_stdin_is_tty()	0
#define lua_stdout_is_tty()	0

/* 4. Verwijder afhankelijkheden van os.exit, os.system, etc. */
/* Dit zorgt dat de linker niet meer naar 'system' of 'exit' vraagt */
#define l_oslib			/* leeg */

/* 5. Continuation context type (moest er ook nog bij) */
#if !defined(LUA_KCONTEXT)
  #define LUA_KCONTEXT	ptrdiff_t
#endif


#endif /* luaconf_h */