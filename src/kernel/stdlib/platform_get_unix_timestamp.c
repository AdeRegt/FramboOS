#include "lualib.h"

#include <stdint.h>

void outportb(uint16_t port, uint8_t value);
uint8_t inportb(uint16_t port);

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

// Helper om een specifiek RTC register te lezen
uint8_t read_rtc(int reg) {
    outportb(CMOS_ADDR, reg);
    return inportb(CMOS_DATA);
}

// Helper om BCD om te zetten naar binair
uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint64_t platform_get_unix_timestamp() {
    uint8_t sec  = bcd_to_bin(read_rtc(0x00));
    uint8_t min  = bcd_to_bin(read_rtc(0x02));
    uint8_t hour = bcd_to_bin(read_rtc(0x04));
    uint8_t day  = bcd_to_bin(read_rtc(0x07));
    uint8_t month= bcd_to_bin(read_rtc(0x08));
    uint32_t year= bcd_to_bin(read_rtc(0x09)) + 2000; // RTC geeft vaak 0-99

    // Berekening van dagen sinds 1 januari 1970
    // Dit is een versimpelde versie:
    uint64_t days_since_epoch = 0;

    // Voeg dagen toe voor de jaren
    for (uint32_t y = 1970; y < year; y++) {
        days_since_epoch += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
    }

    // Voeg dagen toe voor de maanden van het huidige jaar
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) days_in_month[1] = 29;

    for (int m = 0; m < month - 1; m++) {
        days_since_epoch += days_in_month[m];
    }

    days_since_epoch += (day - 1);

    // Zet alles om naar seconden
    return (days_since_epoch * 86400) + (hour * 3600) + (min * 60) + sec;
}