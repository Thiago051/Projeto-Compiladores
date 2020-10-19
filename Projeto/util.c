#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "util.h"

/* inicialização do compilador */
void init()
{
	nsym = 0;

	nextchar();
	nexttoken();
}

/* lê o próximo caracter da entrada em lookahead */
void nextchar_x()
{
	look = getchar();
}

/* lê próximo caracter da entrada e pula quaisquer comentários */
void nextchar()
{
	nextchar_x();
	if (look == '{')
		skipcomment();
}

/* imprime mensagem de erro sem sair */
void error(char *s)
{
	fprintf(stderr, "Error: %s\n", s);
}

/* imprime mensagem de erro e sai */
void fatal(char *s)
{
	error(s);
	exit(1);
}

/* mensagem de erro para string esperada */
void expected(char *s)
{
	fprintf(stderr, "Error: \'%s\' expected\n", s);
	exit(1);
}

/* avisa a respeito de um identificador desconhecido */
void undefined(char *name)
{
	fprintf(stderr, "Error: Undefined identifier \'%s\'\n", name);
	exit(1);
}

/* avisa a respeito de um identificador desconhecido */
void duplicated(char *name)
{
	fprintf(stderr, "Error: Duplicated identifier \'%s\'\n", name);
	exit(1);
}

/* reporta um erro se token NÃO é identificador */
void checkident()
{
	if (token != 'x')
		expected("Identifier");
}

/* testa operadores de adição */
int isaddop(char c)
{
	return (c == '+' || c == '-');
}

/* testa operadores de multiplicaçãoo */
int ismulop(char c)
{
	return (c == '*' || c == '/');
}

/* testa operadores OU */
int isorop(char c)
{
	return (c == '|' || c == '~');
}

/* testa operadores relacionais */
int isrelop(char c)
{
	return (strchr("=#<>", c) != NULL);
}

/* procura por string em tabela,
   usado para procurar palavras-chave e símbolos */
int lookup(char *s, char *list[], int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (strcmp(list[i], s) == 0)
			return i;
	}

	return -1;
}

/* retorna o endereço do identificador na tabela de símbolos */
int locate(char *name)
{
	return lookup(name, symtbl, nsym);
}

/* verifica se "name" consta na tabela de s�mbolos */
int intable(char *name)
{
	if (lookup(name, symtbl, nsym) < 0)
		return 0;
	return 1;
}

/* reporta um erro se identificador NÃO constar na tabela de símbolos */
void checktable(char *name)
{
	if (!intable(name))
		undefined(name);
}

/* reporta um erro se identificador J� constar na tabela de s�mbolos */
void checkdup(char *name)
{
	if (intable(name))
		duplicated(name);
}

/* adiciona novo identificador à tabela de símbolos */
void addsymbol(char *name, char type)
{
	char *newsym;

	checkdup(name);

	if (nsym >= SYMTBL_SZ)
	{
		fatal("Symbol table full!");
	}

	newsym = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	if (newsym == NULL)
		fatal("Out of memory!");

	strcpy(newsym, name);

	symtbl[nsym] = newsym;
	symtype[nsym] = type;
	nsym++;
}

/* gera um novo rótulo */
int newlabel()
{
	return lblcount++;
}