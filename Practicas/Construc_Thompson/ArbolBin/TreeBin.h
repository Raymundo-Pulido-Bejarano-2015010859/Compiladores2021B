#include "defTreeBin.h"
#ifndef TREEBINH
#define TREEBINH

nodoBin* CrearNodoBinario( TIPO data );
void AgregarNodoBinario( nodoBin *padre, nodoBin *hijo, short lado );
void DestruirNodobinario(nodoBin *nodo);
void PreOrden( nodoBin *nodo );
void InOrden( nodoBin *nodo );
void PostOrden( nodoBin *nodo );

#endif