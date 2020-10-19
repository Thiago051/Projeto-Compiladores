#define SYMTBL_SZ 1000
#define KWLIST_SZ 9
#define MAXTOKEN 16

int lblcount; /* indica o rótulo atual */

/* tabela de símbolos */
char *symtbl[SYMTBL_SZ];
char symtype[SYMTBL_SZ];
int nsym; /* número de entradas atuais na tabela de símbolos */


char look; /* O caracter lido "antecipadamente" (lookahead) */
char token; /* código do token atual */
char value[MAXTOKEN+1]; /* texto do token atual */

/* analisador léxico */
void skipwhite();
void semicolon();
void skipcomment();
void getname();
void getnum();
void getop();
void nexttoken();
void scan();
void matchstring(char *s);