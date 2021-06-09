#include "defList.h"

#ifndef LISTH
#define LISTH

void MostrarLista( List *lista );
void EliminarLista( List *lista );
List* CrearNodoLista( TIPO dato );
int ListaVacia( List *lista );
void Append( List **lista, TIPO nodo );
void EliminarNodoLista( List **lista, TIPO dato );
short ExisteEnLista( List *lista, TIPO nodo );

#endif