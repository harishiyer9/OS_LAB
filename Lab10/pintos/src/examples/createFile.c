/* createFile.c

   Simple program to test whether running a user program works.

 */

#include <syscall.h>

int
main (void)
{
  const char *name = "XYZ.txt";
  create(name,10);
}
