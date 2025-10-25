#include "beeldscherm.h"

void drawCharacterInDefaultFont(char character, unsigned int colour, unsigned long x, unsigned long y)
{
  drawCharacter(getDefaultFont(), character, colour, x, y);
}
