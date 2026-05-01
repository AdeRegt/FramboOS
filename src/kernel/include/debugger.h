#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define FALSE 0
#define TRUE 1
#define INTERRUPT __attribute__((interrupt)) void

void printk(char *format, ...);
