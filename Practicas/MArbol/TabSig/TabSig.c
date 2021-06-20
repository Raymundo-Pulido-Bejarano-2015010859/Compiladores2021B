/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 19 - 06 - 21
 * Titulo : Tabla simbolos
 * Correo : rayescomed@gmail.com 
 */
#include <stdio.h>
#include <stdlib.h>
#include "defTabSig.h"


tabsig* CrearTabSig(){

    tabsig* aux = (tabsig*) malloc(sizeof(tabsig));
    aux->nodos = NULL;
    aux->sigpos = NULL;
    aux->NumNodos = 0;
    aux->NumSigPos = NULL;
    aux->simbolos = NULL;

    return aux; 
}

void AgregarNodo( tabsig* tabla, int numposicion, char simbolo ){
    tabla->nodos = ( int* )realloc( tabla->nodos, sizeof(int) * ++(tabla->NumNodos) );
    if ( !tabla->nodos ){
        perror("Error al recervar nodo para tabla Siguiente");
        exit(EXIT_FAILURE);
    }
    tabla->nodos[tabla->NumNodos-1] = numposicion;

    tabla->simbolos = ( char* )realloc( tabla->simbolos, sizeof(char) * (tabla->NumNodos) );
    if ( !tabla->simbolos ){
        perror("Error al recervar nodo para tabla Siguiente");
        exit(EXIT_FAILURE);
    }
    tabla->simbolos[tabla->NumNodos-1] = simbolo;

    tabla->sigpos = ( int** )realloc( (void*)tabla->sigpos, sizeof(int*) * (tabla->NumNodos) );
    if ( !tabla->sigpos ){
        perror("Error al recervar nodo para siguientepos(n)");
        exit(EXIT_FAILURE);
    }
    tabla->sigpos[tabla->NumNodos-1] = NULL;
    tabla->NumSigPos = ( int* )realloc( tabla->NumSigPos, sizeof(int) * (tabla->NumNodos) );
    if ( !tabla->NumSigPos ){
        perror("Error al recervar nodo contador de siguientepos(n)");
        exit(EXIT_FAILURE);
    }
    tabla->NumSigPos[tabla->NumNodos-1] = 0;
}
void AgregarSiguientePos( tabsig* tabla, int posicion, int siguiente ){
    for(int registro = 0; registro < tabla->NumNodos; registro++){
        if( tabla->nodos[registro] == posicion ){
            int flag = 1;
            for (int indice = 0; indice < tabla->NumSigPos[registro]; indice++){
                if( tabla->sigpos[registro][indice] == siguiente){
                    flag = 0;
                }
            }
            if (flag){
                tabla->sigpos[registro] = ( int* )realloc( tabla->sigpos[registro], sizeof(int) * ++(tabla->NumSigPos[registro]) );
                if ( !tabla->sigpos[registro] ){
                    perror("Error al recervar espacio para siguientepos(n)");
                    exit(EXIT_FAILURE);
                }
                tabla->sigpos[registro][tabla->NumSigPos[registro] -1] = siguiente;
            }
            break;
        }
    }
}