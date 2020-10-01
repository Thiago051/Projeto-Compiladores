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
    header();
    topdecls();
    epilog();
    return 0;
}