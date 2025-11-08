#pragma once
#include <stdint.h>

#define NULL (void *)0
#define FALSE 0
#define TRUE 1
#define INTERRUPT __attribute__((interrupt)) void

void printk(char *format, ...);
