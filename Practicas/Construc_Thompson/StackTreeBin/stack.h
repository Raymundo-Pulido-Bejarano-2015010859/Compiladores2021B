#include "defStack.h"
#ifndef STACKN
#define STACKN

stackN* CrearStackN( void );
void PushN( stackN* pila, TIPOS dato );
TIPOS PopN( stackN *pila );
int emptyN( stackN *pila );
TIPOS stacktopN( stackN *pila);

#endif