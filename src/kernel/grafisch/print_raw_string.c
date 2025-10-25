#include "beeldscherm.h"

void print_raw_string(char *message)
{
  int length = 0;
  while (1)
  {
    char deze = message[length++];
    if (deze == '\0')
    {
      break;
    }
    putc(deze);
  }
}
