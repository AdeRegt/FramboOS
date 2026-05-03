#include "lualib.h"

extern unsigned long long system_ticks; // Wordt verhoogd door je timer interrupt

clock_t clock(void) {
    // Geeft het aantal ticks terug sinds boot. 
    // Zorg dat de schaling overeenkomt met CLOCKS_PER_SEC
    return (clock_t)system_ticks;
}