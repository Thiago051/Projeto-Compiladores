/* rotinas utilitárias */
void init();
void nextchar_x();
void nextchar();
void error(char *s);
void fatal(char *s);
void expected(char *s);
void undefined(char *name);
void duplicated(char *name);
void checkident();

/* reconhecedores */
int isaddop(char c);
int ismulop(char c);
int isorop(char c);
int isrelop(char c);

/* tratamento de símbolos */
int lookup(char *s, char *list[], int size);
int locate(char *name);
int intable(char *name);
void checktable(char *name);
void checkdup(char *name);
void addsymbol(char *name, char type);

/* rótulos */
int newlabel();