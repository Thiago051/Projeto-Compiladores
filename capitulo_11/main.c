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
    matchstring("PROGRAM");
    header();
    topdecls();
    matchstring("BEGIN");
    prolog();
    block();
    matchstring("END");
    epilog();

    return 0;
}