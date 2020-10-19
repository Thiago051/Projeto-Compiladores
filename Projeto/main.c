#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "scanner.h"
#include "parser.h"
#include "parser.h"
#include "codegen.h"

/* PROGRAMA PRINCIPAL */

int main()
{
	init();

	matchstring("PROGRAM");
	semicolon();
	header();
	topdecls();
	matchstring("BEGIN");
	prolog();
	block();
	matchstring("END");
	epilog();

	return 0;
}

