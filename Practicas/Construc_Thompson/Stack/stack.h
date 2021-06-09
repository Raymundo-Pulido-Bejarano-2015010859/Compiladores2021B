#include "defStack.h"
#ifndef STACK
#define STACK

stack* CrearStack( void );
void Push( stack *pila, TIPO dato );
void Imprimir( stack *pila );
TIPO Pop( stack *pila );
int empty( stack *pila );
TIPO stacktop( stack *pila );


#endif