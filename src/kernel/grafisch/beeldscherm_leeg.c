#include "beeldscherm.h"

void beeldscherm_leeg()
{
  for (int y = 0; y < graphics_info->Height; y++)
  {
    for (int x = 0; x < graphics_info->Width; x++)
    {
      beeldscherm_plaats_pixel(x, y, BEELDSCHERM_ACHTERGROND_KLEUR);
    }
  }
}