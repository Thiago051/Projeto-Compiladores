#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "lib.h"

char look; /* O caracter lido "antecipadamente" (lookahead) */

/****************************** 1 INTRODUÇÃO */

/* inicialização do compilador */
void init()
{
    nextChar();
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
}

/* recebe o nome de um identificador */
char getName()
{
    char name;

    if (!isalpha(look))
        expected("Name");
    name = toupper(look);
    nextChar();

    return name;
}

/* recebe um número inteiro */
char getNum()
{
    char num;

    if (!isdigit(look))
        expected("Integer");
    num = look;
    nextChar();

    return num;
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

/****************************** 2 Análise de Expressões */

/* analisa e traduz um fator */
void factor()
{
    if (look == '(')
    {
        match('(');
        expression();
        match(')');
    }
    else
        emit("MOV AX, %c", getNum());
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