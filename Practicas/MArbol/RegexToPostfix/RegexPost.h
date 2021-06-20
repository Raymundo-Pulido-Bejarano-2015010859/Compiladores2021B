#ifndef REGEXPOST
#define REGEXPOST

char* AgregarConcatenacionRegex( char *regex );
char* RegexToPostfix( char *regex );
short PrecedenciaOperadores( char OperadorAnterior, char OperadorSiguiente);
char* LeerRegex( int tamano);
int ValidacionDeSimbolosRegex( char *regex );
char* AgregarSMArbol( char* regex );
int EsSimboloAlfabeto( char simbolo );

#endif