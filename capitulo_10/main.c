/*
    Projeto Final de Compiladores
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"

/* PROGRAMA PRINCIPAL */
int main()
{
    init();
    prog();

    if (look != '\n')
        fatal("Unexpected data after \'.\'");

    return 0;
}