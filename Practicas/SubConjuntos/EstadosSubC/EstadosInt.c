#include <stdio.h>
#include <stdlib.h>
#include "defEstadosInt.h"
#include "../Lista/List.h"

void MostrarEstadosInt( EstadoInt *lista ){
    for( ; lista ; lista = lista->sig )
        printf("|%d|", lista->ID );
    printf("\n");
}

void EliminarEstadoInt( EstadoInt *lista ){
    EstadoInt *aux = NULL;
    if ( lista ){
        while ( lista ){
            aux = lista;
            lista = lista->sig;
            free( aux );
        }
    }
}

EstadoInt* CrearNodoEstadoInt( int identificador ){
    EstadoInt *Nuevo = (EstadoInt*) malloc ( sizeof( EstadoInt ) );
    if ( !Nuevo ){
        perror( "Error al crear nodo de EstadoInt" );
        exit(EXIT_FAILURE);
    }
    Nuevo->ID = identificador;
    Nuevo->Marca = 0;
    Nuevo->NumSimbols = 0;
    Nuevo->Kernel = NULL;
    Nuevo->CerraduraC = NULL;
    Nuevo->EstadosSiguientes = NULL;
    Nuevo->SimbolosTransiciones = NULL;
    Nuevo->sig = NULL;
    return Nuevo;
}

int ListaVaciaEstadoInt( EstadoInt *lista ){
    return ( lista == NULL );
}

void AppendEstadoInt( EstadoInt **lista, EstadoInt *nodo ){

    EstadoInt *aux = *lista;

    if( !(*lista) ){
        (*lista) = nodo;
        return ;
    }

    while( aux->sig  != NULL )
        aux = aux->sig;

    aux->sig = nodo;

}

EstadoInt *ObtenerPorKernel( EstadoInt *lista, List *kernel ){
    
    List *Naux = kernel;
    int son = 0, hay = 0;

    while( lista ){
        son = 0, hay = 0; 
        Naux = kernel;
        while(Naux){
            son++;
            hay += ExisteEnLista(lista->Kernel,Naux->datos);
            Naux = Naux->sig;   
        }
        if (son == hay)
            return lista;
        lista = lista->sig;
    }

    return NULL;
}

EstadoInt *ObtenerEstadoPozo( EstadoInt *lista, List *kernel ){

    while( lista ){
        if ( lista->Kernel == kernel)
            return lista;
        lista = lista->sig;
    }

    return NULL;
}

void AgregarEstadoIntSig( EstadoInt *Estado, EstadoInt* EstadoSig, char simbolo){
    
    Estado->EstadosSiguientes = (EstadoInt**)realloc( (void*)Estado->EstadosSiguientes, sizeof( EstadoInt* ) * ( ++Estado->NumSimbols ) );
    if ( !(Estado->EstadosSiguientes) ){
        perror( "Error al reservar espacio para Estados Siguientes" );
        exit(EXIT_FAILURE);
    }

    Estado->SimbolosTransiciones = (char *)realloc( (void*)Estado->SimbolosTransiciones, sizeof( char ) * ( Estado->NumSimbols ) );
    if ( !(Estado->SimbolosTransiciones) ){
        perror( "Error al reservar espacio para los simbolos de transicion" );
        exit(EXIT_FAILURE);
    }

    *(Estado->EstadosSiguientes+(Estado->NumSimbols-1)) = EstadoSig;
    *(Estado->SimbolosTransiciones+(Estado->NumSimbols-1)) = simbolo;

}

EstadoInt *ObtenerEstadoPorID( EstadoInt *lista, int identificador ){
    while( lista ){
        if( lista->ID == identificador )
            return lista;

        lista = lista->sig;
    }
    return NULL;
}

EstadoInt *CopiarEstadoInt( EstadoInt *estado ){
    EstadoInt *Nuevo = (EstadoInt*) malloc ( sizeof( EstadoInt ) );
    if ( !Nuevo ){
        perror( "Error al crear nodo de EstadoInt" );
        exit(EXIT_FAILURE);
    }
    Nuevo->ID = estado->ID;
    Nuevo->Marca = 0;
    Nuevo->NumSimbols = estado->NumSimbols;
    Nuevo->Kernel = NULL;
    Nuevo->CerraduraC = estado->CerraduraC;
    Nuevo->EstadosSiguientes = estado->EstadosSiguientes;
    Nuevo->SimbolosTransiciones = estado->SimbolosTransiciones;
    Nuevo->sig = NULL;

    return Nuevo;
}

EstadoInt *ObtenerEstadosPorElemCerradura( EstadoInt *lista, int identificadorAFN ){
    EstadoInt *listaEstados = NULL;
    List *aux = NULL;
    while ( lista ){

        aux = lista->CerraduraC;
        while( aux ){
            if( aux->datos->ID == identificadorAFN ){
                AppendEstadoInt( &listaEstados, CopiarEstadoInt( lista ) );
                break;
            }

            aux = aux->sig;
        }
        aux = NULL;

        lista = lista->sig;
    }

    return listaEstados;
}
