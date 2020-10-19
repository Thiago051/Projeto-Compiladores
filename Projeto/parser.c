#include <stdio.h>
#include <string.h>
#include "util.h"
#include "scanner.h"
#include "codegen.h"
#include "parser.h"

/* rotinas do analisador sintático */

/* analisa e traduz um fator matemático */
void factor()
{
    if (token == '(')
    {
        nexttoken();
        boolexpression();
        matchstring(")");
    }
    else
    {
        if (token == 'x')
            asm_loadvar(value);
        else if (token == '#')
            asm_loadconst(value);
        else
            expected("Math Factor");
        nexttoken();
    }
}

/* reconhece e traduz uma multiplicação */
void multiply()
{
    nexttoken();
    factor();
    asm_popmul();
}

/* reconhece e traduz uma divisão */
void divide()
{
    nexttoken();
    factor();
    asm_popdiv();
}

/* analisa e traduz um termo matemático */
void term()
{
    factor();
    while (ismulop(token))
    {
        asm_push();
        switch (token)
        {
        case '*':
            multiply();
            break;
        case '/':
            divide();
            break;
        }
    }
}

/* reconhece e traduz uma adição */
void add()
{
    nexttoken();
    term();
    asm_popadd();
}

/* reconhece e traduz uma subtração*/
void subtract()
{
    nexttoken();
    term();
    asm_popsub();
}

/* analisa e traduz uma expressão matemática */
void expression()
{
    if (isaddop(token))
        asm_clear();
    else
        term();
    while (isaddop(token))
    {
        asm_push();
        switch (token)
        {
        case '+':
            add();
            break;
        case '-':
            subtract();
            break;
        }
    }
}

/* analisa e traduz uma relação */
void relation()
{
    char op;

    expression();
    if (isrelop(token))
    {
        op = token;
        nexttoken(); /* só para remover o operador do caminho */
        if (op == '<')
        {
            if (token == '>')
            { /* <> */
                nexttoken();
                op = '#';
            }
            else if (token == '=')
            {
                nexttoken();
                op = 'L';
            }
        }
        else if (op == '>' && token == '=')
        {
            nexttoken();
            op = 'G';
        }
        asm_push();
        expression();
        asm_popcompare();
        asm_relop(op);
    }
}

/* analisa e traduz um fator booleano com NOT inicial */
void notfactor()
{
    if (token == '!')
    {
        nexttoken();
        relation();
        asm_not();
    }
    else
        relation();
}

/* analisa e traduz um termo booleano */
void boolterm()
{
    notfactor();
    while (token == '&')
    {
        asm_push();
        nexttoken();
        notfactor();
        asm_popand();
    }
}

/* reconhece e traduz um "OR" */
void boolor()
{
    nexttoken();
    boolterm();
    asm_popor();
}

/* reconhece e traduz um "xor" */
void boolxor()
{
    nexttoken();
    boolterm();
    asm_popxor();
}

/* analisa e traduz uma expressão booleana */
void boolexpression()
{
    boolterm();
    while (isorop(token))
    {
        asm_push();
        switch (token)
        {
        case '|':
            boolor();
            break;
        case '~':
            boolxor();
            break;
        }
    }
}

/* analisa e traduz um comando de atribuição */
void assignment()
{
    char name[MAXTOKEN + 1];

    strcpy(name, value);
    checktable(name);
    nexttoken();
    matchstring("=");
    boolexpression();
    asm_store(name);
}

/* analiza e traduz um comando IF-ELSE-ENDIF */
void doif()
{
    int l1, l2;

    nexttoken();
    boolexpression();
    l1 = newlabel();
    l2 = l1;
    asm_jmpfalse(l1);
    block();
    if (token == 'l')
    {
        nexttoken();
        l2 = newlabel();
        asm_jmp(l2);
        printf("L%d:\n", l1);
        block();
    }
    printf("L%d:\n", l2);
    matchstring("ENDIF");
}

/* analiza e traduz um comando WHILE-ENDWHILE */
void dowhile()
{
    int l1, l2;

    nexttoken();
    l1 = newlabel();
    l2 = newlabel();
    printf("L%d:\n", l1);
    boolexpression();
    asm_jmpfalse(l2);
    block();
    matchstring("ENDWHILE");
    asm_jmp(l1);
    printf("L%d:\n", l2);
}

/* lê uma variável única */
void readvar()
{
    checkident();
    checktable(value);
    asm_read(value);
    nexttoken();
}

/* analiza e traduz um comando READ */
void doread()
{
    nexttoken();
    matchstring("(");
    for (;;)
    {
        readvar();
        if (token != ',')
            break;
        nexttoken();
    }
    matchstring(")");
}

/* analiza e traduz um comando WRITE */
void dowrite()
{
    nexttoken();
    matchstring("(");
    for (;;)
    {
        expression();
        asm_write();
        if (token != ',')
            break;
        nexttoken();
    }
    matchstring(")");
}

/* analiza e traduz um bloco de comandos */
void block()
{
    int follow = 0;

    do
    {
        scan();
        switch (token)
        {
        case 'i':
            doif();
            break;
        case 'w':
            dowhile();
            break;
        case 'R':
            doread();
            break;
        case 'W':
            dowrite();
            break;
        case 'x':
            assignment();
            break;
        case 'e':
        case 'l':
            follow = 1;
            break;
        }
        if (!follow)
            semicolon();
    } while (!follow);
}

/* declaração de variáveis */
void decl()
{
    nexttoken();
    if (token != 'x')
        expected("Variable name");
    checkdup(value);
    addsymbol(value, 'v');
    allocvar(value, 0);
    nexttoken();
}

/* analisa e traduz declarações */
void topdecls()
{
    scan();
    while (token == 'v')
    {
        do
        {
            decl();
        } while (token == ',');
        semicolon();
    }
}
