#include "beeldscherm.h"

void drawCharacterInDefaultFont(char character, unsigned int colour, unsigned long x, unsigned long y, uint8_t shadow)
{
  drawCharacter(getDefaultFont(), character, colour, x, y, shadow);
}
