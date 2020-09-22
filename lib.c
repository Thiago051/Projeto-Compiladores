#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "lib.h"

//char look; /* O caracter lido "antecipadamente" (lookahead) */

/********** 1 Introdução **********/

/* inicialização do compilador */
void init()
{
    nextChar();
    skipWhite();
}

/* lê próximo caracter da entrada */
void nextChar()
{
    look = getchar();
}

/* exibe uma mensagem de erro formatada */
void error(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
}

/* exibe uma mensagem de erro formatada e sai */
void fatal(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);

    exit(1);
}

/* alerta sobre alguma entrada esperada */
void expected(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputs(" expected!\n", stderr);

    exit(1);
}

/* verifica se entrada combina com o esperado */
void match(char c)
{
    if (look != c)
        expected("'%c'", c);
    nextChar();
    skipWhite();
}

/* recebe o nome de um identificador */
void getName(char *name)
{
    int i;
    if (!isalpha(look))
        expected("Name");
    for (i = 0; isalnum(look); i++)
    {
        if (i >= MAXNAME)
            fatal("Identifier too long!");
        name[i] = toupper(look);
        nextChar();
    }
    name[i] = '\0';
    skipWhite();
}

/* recebe um número inteiro */
void getNum(char *num)
{
    int i;
    if (!isdigit(look))
        expected("Integer");
    for (i = 0; isdigit(look); i++)
    {
        if (i >= MAXNUM)
            fatal("Integer too long!");
        num[i] = look;
        nextChar();
    }
    num[i] = '\0';
    skipWhite();
}

/* emite uma instrução seguida por uma nova linha */
void emit(char *fmt, ...)
{
    va_list args;

    putchar('\t');

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
}

/********** 2 Análise de Expressões **********/

/* analisa e traduz um fator */
void factor()
{
    char num[MAXNUM + 1];
    if (look == '(')
    {
        match('(');
        expression();
        match(')');
    }
    else if (isalpha(look))
    {
        ident();
    }
    else
    {
        getNum(num);
        emit("MOV AX, %s", num);
    }
}

/* analisa e traduz um termo */
void term()
{
    factor();
    while (look == '*' || look == '/')
    {
        emit("PUSH AX");
        switch (look)
        {
        case '*':
            multiply();
            break;
        case '/':
            divide();
            break;
        default:
            expected("MulOp");
            break;
        }
    }
}

/* reconhece e traduz uma adição */
void add()
{
    match('+');
    term();
    emit("POP BX");
    emit("ADD AX, BX");
}

/* reconhece e traduz uma subtração */
void subtract()
{
    match('-');
    term();
    emit("POP BX");
    emit("SUB AX, BX");
    emit("NEG AX");
}

/* reconhece e traduz uma multiplicação */
void multiply()
{
    match('*');
    factor();
    emit("POP BX");
    emit("IMUL BX");
}

/* reconhece e traduz uma divisão */
void divide()
{
    match('/');
    factor();
    emit("POP BX");
    emit("XCHG AX, BX");
    emit("CWD");
    emit("IDIV BX");
}

/* reconhece e traduz uma expressão */
void expression()
{
    if (isAddOp(look))
        emit("XOR AX, AX");
    else
        term();
    while (isAddOp(look))
    {
        emit("PUSH AX");
        switch (look)
        {
        case '+':
            add();
            break;
        case '-':
            subtract();
            break;
        default:
            expected("AddOp");
            break;
        }
    }
}

/* reconhece operador aditivo */
int isAddOp(char c)
{
    return (c == '+' || c == '-');
}

/********** 3 Mais expressões **********/

/* analisa e traduz um identificador */
void ident()
{
    char name[MAXNAME + 1];
    getName(name);
    if (look == '(')
    {
        match('(');
        match(')');
        emit("CALL %s", name);
    }
    else
        emit("MOV AX, [%s]", name);
}

/* analisa e traduz um comando de atribuição */
void assignment()
{
    char name[MAXNAME + 1];
    getName(name);
    match('=');
    expression();
    emit("MOV [%s], AX", name);
}

/* pula caracteres de espaço */
void skipWhite()
{
    while (look == ' ' || look == '\t')
        nextChar(); 
}

/********** 4 Interpretadores **********/