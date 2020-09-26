/*
    Projeto Final de Compiladores
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "lib.h"

/* PROGRAMA PRINCIPAL */
int main()
{
    init();
    do
    {
        switch (look)
        {
        case '?':
            input();
            break;
        case '!':
            output();
            break;
        default:
            assignment();
            break;
        }
        newLine();
    } while (look != '.');

    return 0;
}