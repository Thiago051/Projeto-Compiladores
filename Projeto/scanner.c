#include <ctype.h>
#include <string.h>
#include "util.h"
#include "scanner.h"

/* códigos e lista de palavras-chave */
char kwcode[KWLIST_SZ + 1] = "ileweRWve";
char *kwlist[KWLIST_SZ] = {"IF", "ELSE", "ENDIF", "WHILE", "ENDWHILE",
						   "READ", "WRITE", "VAR", "END"};

/* pula caracteres em branco */
void skipwhite()
{
	while (isspace(look))
		nextchar();
}

/* reconhece um ponto e vírgula */
void semicolon()
{
	if (token == ';')
		nexttoken();
}

/* pula um campo de comentário */
void skipcomment()
{
	while (look != '}')
	{
		nextchar_x();
	}
	nextchar_x();
}

/* analisa e traduz um nome (identificador ou palavra-chave) */
void getname()
{
	int i;

	skipwhite();
	if (!isalpha(look))
		expected("Identifier");
	for (i = 0; isalnum(look) && i < MAXTOKEN; i++)
	{
		value[i] = toupper(look);
		nextchar();
	}
	value[i] = '\0';
	token = 'x';
}

/* analisa e traduz um número inteiro */
void getnum()
{
	int i;

	skipwhite();
	if (!isdigit(look))
		expected("Number");
	for (i = 0; isdigit(look) && i < MAXTOKEN; i++)
	{
		value[i] = look;
		nextchar();
	}
	value[i] = '\0';
	token = '#';
}

/* analisa e traduz um operador */
void getop()
{
	skipwhite();
	token = look;
	value[0] = look;
	value[1] = '\0';
	nextchar();
}

/* pega o próximo token de entrada */
void nexttoken()
{
	skipwhite();
	if (isalpha(look))
		getname();
	else if (isdigit(look))
		getnum();
	else
		getop();
}

/* analisador léxico. analisa identificador ou palavra-chave */
void scan()
{
	int kw;

	if (token == 'x')
	{
		kw = lookup(value, kwlist, KWLIST_SZ);
		if (kw >= 0)
			token = kwcode[kw];
	}
}

/* compara string com texto do token atual */
void matchstring(char *s)
{
	if (strcmp(value, s) != 0)
		expected(s);
	nexttoken();
}