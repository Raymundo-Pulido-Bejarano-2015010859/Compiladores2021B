#include "defAFN.h"
#ifndef AFNH
#define AFNH

NodoAFN* CrearNodoAFN( int Identificador);
AFN* CrearAFN( void );
int ComprobarAlfabeto( char *alfabeto, int numSimbolos, char simbolo);
void AgregarEstadoAFN( AFN *afn, NodoAFN *estado );
void AgregarSimboloAFN( AFN *afn, char simbolo );
void AgregarEstadoSig( NodoAFN *estado , NodoAFN *estadoSig, char simbolo );
void MostrarAFN( AFN* afn_Completo );
void AgregarEstadoFinalAFN(AFN *afn, NodoAFN* Estado);
int ComprobarESFinal(AFN *afn, int identificador);

#endif