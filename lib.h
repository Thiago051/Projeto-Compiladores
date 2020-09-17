/* Protótipos */

/* 1 INTRODUÇÃO */
void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char getName();
char getNum();
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
