#include <stdio.h>
#include <stdlib.h>
#include "defList.h"

/*
 *  MostrarLista:
 *  Mostrar los identificadores de los elementos de la lista 
 *  Entrada:
 *      lista: Lista de los estados 
 */
void MostrarLista( List *lista ){
    for( ; lista ; lista = lista->sig )
        printf("|%d|", lista->datos->ID );
    printf("\n");
}

/*
 *  EliminarLista:
 *  Eliminar una lista de elementos
 *  Entrada:
 *      lista: Lista de los estados 
 */
void EliminarLista( List *lista ){
    List *aux = NULL;
    if ( lista ){
        while ( lista ){
            aux = lista;
            lista = lista->sig;
            free( aux );
        }
    }
}

/*
 * CrearNodoLista:
 * Crear nodos de la lista
 * Entrada:
 *  dato: Dato del nodo en la lista
 */
List* CrearNodoLista( TIPO dato ){
    List *Nuevo = (List*) malloc ( sizeof( List ) );
    if ( !Nuevo ){
        perror( "Error al crear nodo de la Lista" );
        exit(EXIT_FAILURE);
    }
    Nuevo->datos = dato;
    Nuevo->sig = NULL;
    return Nuevo;
}

/*
 * ListaVacia:
 * Determinar si la lista esta vacia
 * Entrada:
 *      lista: Lista de los estados 
 */
int ListaVacia( List *lista ){
    return ( lista == NULL );
}

/*
 * Append:
 *  Funcion para agregar al final de la lista
 * Entrada:
 *      lista: Referencia de un apuntador a la lista
 *       nodo: Valor para el nuevo nodo de la lista
 */
void Append( List **lista, TIPO nodo ){

    List *aux = *lista;
    if( !(*lista) ){
        (*lista) = CrearNodoLista( nodo );
        return ;
    }

    while( aux->sig  != NULL )
        aux = aux->sig;

    aux->sig = CrearNodoLista( nodo );

}

/**
 * ExisteEnLista:
 *  Comprobar si existe un dato en la lista
 * Entrada:
 *      lista: apuntador a la lista
 *       nodo: Valor para el nuevo nodo de la lista
 */
short ExisteEnLista( List *lista, TIPO nodo ){
    
    while( lista ){
        if ( lista->datos == nodo)
            return 1;
        lista = lista->sig;
    }

    return 0;
}

/*
 * EliminarNodoLista:
 *  Eliminar nodos de una lista
 *      lista: Referencia de un apuntador a la lista
 *       nodo: Valor para el nuevo nodo de la lista
 */
void EliminarNodoLista( List **lista, TIPO dato){
    List *ant = NULL, *act = NULL;
    if ( !lista ){
        perror( "Error lista vacia" );
        exit(EXIT_FAILURE);
    } 
    act = *lista;
    while( act ){
        if ( act->datos == dato )
            break;
        ant = act;
        act = act->sig;
    }
    if ( !ant ){
        *lista = act->sig;
    }else if( !act->sig ) {
        ant->sig = NULL;
    }else{
        ant->sig = act->sig;
    }
}

