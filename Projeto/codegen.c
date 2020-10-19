#include <stdio.h>
#include "util.h"
#include "scanner.h"
#include "codegen.h"

/* ROTINAS DE GERAÇÃO DE CÓDIGO */

/* zera o registrador primário */
void asm_clear()
{
	printf("\txor ax, ax\n");
}

/* negativa o reg. primário */
void asm_negative()
{
	printf("\tneg ax\n");
}

/* carrega uma constante numérica no reg. prim. */
void asm_loadconst(char *val)
{
	printf("\tmov ax, %s\n", val);
}

/* carrega uma variável no reg. prim. */
void asm_loadvar(char *name)
{
	if (!intable(name))
		undefined(name);
	printf("\tmov ax, word ptr %s\n", name);
}

/* coloca reg. prim. na pilha */
void asm_push()
{
	printf("\tpush ax\n");
}

/* adiciona o topo da pilha ao reg. prim. */
void asm_popadd()
{
	printf("\tpop bx\n");
	printf("\tadd ax, bx\n");
}

/* subtrai o reg. prim. do topo da pilha */
void asm_popsub()
{
	printf("\tpop bx\n");
	printf("\tsub ax, bx\n");
	printf("\tneg ax\n");
}

/* multiplica o topo da pilha pelo reg. prim. */
void asm_popmul()
{
	printf("\tpop bx\n");
	printf("\tmul bx\n");
}

/* divide o topo da pilha pelo reg. prim. */
void asm_popdiv()
{
	printf("\tpop bx\n");
	printf("\txchg ax, bx\n");
	printf("\tcwd\n");
	printf("\tdiv bx\n");
}

/* armazena reg. prim. em variável */
void asm_store(char *name)
{
	printf("\tmov word ptr %s, ax\n", name);
}

/* inverte reg. prim. */
void asm_not()
{
	printf("\tnot ax\n");
}

/* "E" do topo da pilha com reg. prim. */
void asm_popand()
{
	printf("\tpop bx\n");
	printf("\tand ax, bx\n");
}

/* "OU" do topo da pilha com reg. prim. */
void asm_popor()
{
	printf("\tpop bx\n");
	printf("\tor ax, bx\n");
}

/* "OU-exclusivo" do topo da pilha com reg. prim. */
void asm_popxor()
{
	printf("\tpop bx\n");
	printf("\txor ax, bx\n");
}

/* compara topo da pilha com reg. prim. */
void asm_popcompare()
{
	printf("\tpop bx\n");
	printf("\tcmp bx, ax\n");
}

/* altera reg. primário (e flags, indiretamente) conforme a comparação */
void asm_relop(char op)
{
	char *jump;
	int l1, l2;

	l1 = newlabel();
	l2 = newlabel();

	switch (op)
	{
	case '=':
		jump = "je";
		break;
	case '#':
		jump = "jne";
		break;
	case '<':
		jump = "jl";
		break;
	case '>':
		jump = "jg";
		break;
	case 'L':
		jump = "jle";
		break;
	case 'G':
		jump = "jge";
		break;
	}

	printf("\t%s L%d\n", jump, l1);
	printf("\txor ax, ax\n");
	printf("\tjmp L%d\n", l2);
	printf("L%d:\n", l1);
	printf("\tmov ax, -1\n");
	printf("L%d:\n", l2);
}

/* desvio incondicional */
void asm_jmp(int label)
{
	printf("\tjmp L%d\n", label);
}

/* desvio se falso (0) */
void asm_jmpfalse(int label)
{
	printf("\tjz L%d\n", label);
}

/* lê um valor para o registrador primário e armazena em variável */
void asm_read()
{
	printf("\tcall READ\n");
	asm_store(value);
}

/* mostra valor do reg. primário */
void asm_write()
{
	printf("\tcall WRITE\n");
}

/* cabeçalho do código assembly */
void header()
{
	printf(".model small\n");
	printf(".stack\n");
	printf(".code\n");
	printf("extrn READ:near, WRITE:near\n");
	printf("PROG segment byte public\n");
	printf("\tassume cs:PROG,ds:PROG,es:PROG,ss:PROG\n");
}

/* prólogo da rotina principal */
void prolog()
{
	printf("MAIN:\n");
	printf("\tmov ax, PROG\n");
	printf("\tmov ds, ax\n");
	printf("\tmov es, ax\n");
}

/* epílogo da rotina principal */
void epilog()
{
	printf("\tmov ax,4C00h\n");
	printf("\tint 21h\n");
	printf("PROG ends\n");
	printf("\tend MAIN\n");
}

/* aloca memória para uma declaração de variável (+inicializador) */
void allocvar(char *name, int value)
{
	printf("%s:\tdw %d\n", name, value);
}