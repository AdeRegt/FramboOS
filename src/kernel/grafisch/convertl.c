#include "beeldscherm.h"

char *convertl(uint64_t num, int base)
{
  static char Representation[] = "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  for (int i = 0; i < 50; i++)
  {
    buffer[i] = 0x00;
  }

  if (num == 0)
  {
    ptr = &buffer[0];
    buffer[0] = '0';
    buffer[1] = 0;
    return ptr;
  }

  ptr = &buffer[49];
  *--ptr = '\0';
  *--ptr = '\0';
  *--ptr = '\0';

  do
  {
    *--ptr = Representation[num % base];
    num /= base;
  } while (num != 0);

  return (ptr);
}
