#include "../AFN/defAFN.h"
#ifndef DELISTH
#define DELISTH

#define TIPO NodoAFN*

typedef struct nodo{
    TIPO datos;
    struct nodo* sig;
}List;

#endif