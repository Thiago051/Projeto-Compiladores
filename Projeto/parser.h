/* rotinas do analisador sintático */

/* expressões aritméticas */
void factor();
void multiply();
void divide();
void term();
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
void readvar();
void doread();
void writevar();
void dowrite();
void block();

/* declarações */
void decl();
void topdecls();
