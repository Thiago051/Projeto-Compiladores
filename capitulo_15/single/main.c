#include <stdio.h>
#include "input.h"
#include "output.h"
#include "errors.h"
#include "scanner1.h"

int main()
{
    char name = 'A';
    error("'%c' is not a variable", name);

    printf("%c", getName());

    match('=');

    printf("%c", getNum());

    match('+');

    printf("%c", getName());
    
    return 0;
}