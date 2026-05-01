#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

extern void* fshook;

void wachten_op_bestandssysteem();
void bs_regristreer(void* fsdata);
void bs_leeg();
void* bestand(uint8_t* path);
uint8_t* directory();
extern int execvp (const char *file, char *const argv[]);
size_t fwrite(const void* p, size_t s, size_t n, void* f);
size_t fread(void* p, size_t s, size_t n, void* f);