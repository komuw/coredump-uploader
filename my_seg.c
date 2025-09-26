#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr = NULL; // Declare a null pointer

    // Attempt to write to a null pointer, causing a segmentation fault
    printf("Attempting to dereference a null pointer...\n");
    *ptr = 10;

    printf("This line will not be reached.\n"); // This line will not be executed
    return 0;
}

/*
gcc -g my_seg.c -o my_seg
ulimit -c unlimited && ./my_seg
gdb my_seg core.583354.my_seg.1758864537.kw
  (gdb) bt
  (gdb) thread apply all bt

https://g.co/gemini/share/82790b325e57 : good.

man systemd.unit >> man_systemd_unit.txt
*/