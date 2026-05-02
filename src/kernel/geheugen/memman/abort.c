#include "geheugen.h"

void abort(void) {
    printk("__PANIC__\n");
    for(;;); /* Kernel Panic */
}