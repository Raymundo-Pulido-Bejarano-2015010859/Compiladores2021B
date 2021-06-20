#include "defAFD.h"
#ifndef AFDH
#define AFDH

NodoAFD* CrearNodoAFD( int Identificador);
AFD* CrearAFD( void );
int ComprobarAlfabeto( char *alfabeto, int numSimbolos, char simbolo);
void AgregarEstadoAFD( AFD *afd, NodoAFD *estado );
void AgregarSimboloAFD( AFD *afd, char simbolo );
void AgregarEstadoSig( NodoAFD *estado , NodoAFD *estadoSig, char simbolo );
void AgregarEstadoFinalAFD(AFD *afd, NodoAFD* EstadoFinal);

#endif