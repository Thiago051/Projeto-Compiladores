char look; /* O caracter lido "antecipadamente" (lookahead) */
#define MAXNAME 30
#define MAXNUM 5
/* Protótipos */

/* 1 Introdução */
void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
void getName(char *name);
void getNum(char *num);
void emit(char *fmt, ...);

/* 2 Análise de Expressões */
void term();
void expression();
void add();
void subtract(); 
void multiply();
void divide();
void factor();
int isAddOp(char c);

/* 3 Mais expressões*/
void ident();
void assignment();
void skipWhite();

/* 4 Interpretadores */