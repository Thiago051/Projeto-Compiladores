#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

/* PROGRAMA PRINCIPAL */
int main()
{
    init();
    topdecls();
    match('B');
    newline();
    block();
    dumptable();

    return 0;
}