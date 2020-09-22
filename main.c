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
    //expression();
    assignment();
    if (look != '\n')
            expected("NewLine");
    
    return 0;
}
