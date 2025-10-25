#include "beeldscherm.h"

void beeldscherm_plaats_pixel(unsigned int x, unsigned int y, unsigned int colour)
{
  unsigned int BBP = 4;
  *(unsigned int *)((x * BBP) + (y * graphics_info->PixelsPerScanLine * BBP) + graphics_info->BaseAddress) = colour;
}
