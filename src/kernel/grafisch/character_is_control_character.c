#include "beeldscherm.h"

uint8_t character_is_control_character(char c)
{
  if (c == '\n')
  {
    return 1;
  }
  return 0;
}
