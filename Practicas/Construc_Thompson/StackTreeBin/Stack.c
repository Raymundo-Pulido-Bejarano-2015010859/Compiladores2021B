/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Biblioteca de pila con nodos
 * Correo : rayescomed@gmail.com 
 */
#include <stdio.h>
#include <stdlib.h>
#include "defStack.h"

stackN* CrearStackN(void){
    stackN* pila = (stackN*)malloc(sizeof(stackN));
    if ( !pila ){
        perror("Error al recervar memoria");
        exit(EXIT_FAILURE);
    }
    pila->top = 0;
    pila->longitud = 0;
    pila->datos = NULL;
    return pila;
}
void PushN(stackN* pila,TIPOS dato){
   if(pila->longitud == pila->top){
        pila->datos = (TIPOS*)realloc(pila->datos, sizeof(TIPOS) * (pila->longitud+3) );
        if ( !pila->datos ){
            perror("Error al recervar memoria");
            exit(EXIT_FAILURE);
        }
        pila->longitud = pila->longitud + 3;
    }
    *(pila->datos+pila->top++) = dato;
}
TIPOS PopN(stackN* pila){
    if (pila->top == 0){
        perror("StackUnderFlow");
        exit(EXIT_FAILURE);
    }    
    return *(pila->datos+(--pila->top));
}
int emptyN(stackN* pila){
    return (pila->top == 0) ? 1: 0;
}
TIPOS stacktopN(stackN* pila){
    return pila->datos[pila->top-1];
}