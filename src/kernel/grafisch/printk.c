#include "beeldscherm.h"

void printk(char *format, ...)
{
  va_list arg;
  va_start(arg, format);
  int length = 0;
  while (1)
  {
    char deze = format[length];
    if (deze == '\0')
    {
      break;
    }
    else if (deze == '%')
    {
      length++;
      deze = format[length];
      if (deze == 'c')
      {
        char i = va_arg(arg, int);
        putc(i);
      }
      else if (deze == '%')
      {
        putc('%');
      }
      else if (deze == 's')
      {
        char *s = va_arg(arg, char *);
        print_raw_string(s);
      }
      else if (deze == 'x')
      {
        int t = va_arg(arg, unsigned int);
        putc('0');
        putc('x');
        char *convertednumber = convert(t, 16);
        print_raw_string(convertednumber);
      }
      else if (deze == 'd')
      {
        int t = va_arg(arg, unsigned int);
        char *convertednumber = convert(t, 10);
        print_raw_string(convertednumber);
      }
      else if (deze == 'o')
      {
        int t = va_arg(arg, unsigned int);
        char *convertednumber = convert(t, 8);
        print_raw_string(convertednumber);
      }
      else if (deze == 'l')
      {
        length++;
        deze = format[length];
        if (deze == 'x')
        {
          uint64_t t = va_arg(arg, uint64_t);
          putc('0');
          putc('x');
          char *convertednumber = convertl(t, 16);
          print_raw_string(convertednumber);
        }
        else if (deze == 'd')
        {
          uint64_t t = va_arg(arg, uint64_t);
          char *convertednumber = convertl(t, 10);
          print_raw_string(convertednumber);
        }
        else if (deze == 'o')
        {
          uint64_t t = va_arg(arg, uint64_t);
          char *convertednumber = convertl(t, 8);
          print_raw_string(convertednumber);
        }
      }
      length++;
    }
    else
    {
      putc(deze);
      length++;
    }
  }
  va_end(arg);
}
