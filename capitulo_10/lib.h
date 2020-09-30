char look; /* O caracter lido "antecipadamente" (lookahead) */

#define SYMTBL_SZ 1000

#define KWLIST_SZ 11

#define MAXTOKEN 16

int lblcount; /* indica o rótulo atual */

/* tabela de símbolos */

char *symtbl[SYMTBL_SZ];

int nsym; /* número de entradas atuais na tabela de símbolos */

/* códigos e lista de palavras-chave 

char kwcode[KWLIST_SZ + 1] = "ileweRWvbep";

char *kwlist[KWLIST_SZ] = {"IF", "ELSE", "ENDIF", "WHILE", "ENDWHILE",
                           "READ", "WRITE", "VAR", "BEGIN", "END", "PROGRAM"};
*/

char look; /* O caracter lido "antecipadamente" (lookahead) */

char token; /* código do token atual */

char value[MAXTOKEN + 1]; /* texto do token atual */

/* PROTÓTIPOS */

/* rotinas utilitárias */

void init();
void nextchar();
void error(char *s);
void fatal(char *s);
void expected(char *s);
void undefined(char *name);

/* reconhecedores */

int isaddop(char c);
int ismulop(char c);
int isorop(char c);
int isrelop(char c);

/* tratamento de símbolos */

int lookup(char *s, char *list[], int size);
int intable(char *name);
void addsymbol(char *name);

/* analisador léxico */

void skipwhite();
void newline();
void match(char c);
void getname();
int getnum();
void scan();
void matchstring(char *s);

/* rótulos */

int newlabel();

/* rotinas de geração de código */

void asm_clear();
void asm_negative();
void asm_loadconst(int i);
void asm_loadvar(char *name);
void asm_push();
void asm_popadd();
void asm_popsub();
void asm_popmul();
void asm_popdiv();
void asm_store(char *name);
void asm_not();
void asm_popand();
void asm_popor();
void asm_popxor();
void asm_popcompare();
void asm_relop(char op);
void asm_jmp(int label);
void asm_jmpfalse(int label);
void asm_read();
void asm_write();
void header();
void prolog();
void epilog();

/* rotinas do analisador sintático */

/* expressões aritméticas */

void factor();
void negfactor();
void firstfactor();
void multiply();
void divide();
void term1();
void term();
void firstterm();
void add();
void subtract();
void expression();

/* expressões booleanas e relações */

void relation();
void notfactor();
void boolterm();
void boolor();
void boolxor();
void boolexpression();

/* bloco, estruturas de controle e comandos */

void assignment();
void doif();
void dowhile();
void doread();
void dowrite();
void block();

/* declarações */

void allocvar(char *name);
void decl();
void topdecls();

/* programa principal */

void mainblock();
void prog();