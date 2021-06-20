/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Biblioteca de Automata Finito Determinista 
 * Correo : rayescomed@gmail.com 
 */

#include <stdio.h>
#include <stdlib.h>
#include "defAFD.h"

/*
 *  CrearNodoAFD:  
 *      Crea un Nodo del Automata Finito determinista
 *      Entrada:
 *          ID: Identificador del nodo
 *       Salida:
 *          nodoAux: nodo inicializado con el identificador   
 */
NodoAFD* CrearNodoAFD(int ID){
    NodoAFD* nodoAux = (NodoAFD*) malloc( sizeof( NodoAFD ) );
    if ( !nodoAux ){
        perror( "Error al reservar espacio para nodo" );
        exit(EXIT_FAILURE);
    }
    nodoAux->ID = ID;
    nodoAux->NumeroDeEstadosSig = 0;
    nodoAux->SimbolosTransiciones = NULL;
    nodoAux->EstadosSiguiente = NULL;
    return nodoAux;
}

/*
 *  CrearAFD:  
 *      Crea un Automata Finito determinista vacio, para contener todos los elementos de estos.
 *      Entrada:
 *       
 *       Salida:
 *          afnAux: AFD inicializado en nulo. 
 */
AFD* CrearAFD( void ){
    AFD *afdAux = (AFD*) malloc( sizeof( AFD ) );
    if ( !afdAux ){
        perror( "Error al reservar espacio para AFN" );
        exit(EXIT_FAILURE);
    }
    afdAux->alfabeto = NULL;
    afdAux->Estados = NULL;
    afdAux->Inicio = NULL;
    afdAux->Finales = NULL;
    afdAux->NumSimbolos = 0;
    afdAux->NumEstados = 0;
    afdAux->NumFinales = 0;
    return afdAux;
}

/*
 *  ComprobarAlfabeto:  
 *      Comprueba si un elemento existe en el alfabeto del automata
 *      Entrada:
 *         alfabeto: Arreglo con los simbolos de un AFD
 *          NumAlfa: Numero de simbolos del AFD
 *          simbolo: simbolo a comprobar si ya es parte del alfabeto del AFD
 *       Salida:
 *          0:  Simbolo encontro  
 *          1:  Simbolo Faltante / Sin simbolos
 */
int ComprobarAlfabeto(char *alfabeto, int NumAlfa, char simbolo){

    if ( NumAlfa == 0 )
        return 1;

    for ( int indice = 0; indice < NumAlfa ; indice++ )
        if ( alfabeto[indice] == simbolo )
            return 0;

    return 1;

}

/*
 *  AgregarEstadoAFD:  
 *      Agregar un estado al conjunto de estados del AFD
 *      Entrada:
 *             afd: AFD al que se le agregara el estado
 *          Estado: Estado que se agregara a el AFD
 *       Salida:
 *             El afd contendra el estado
 */
void AgregarEstadoAFD(AFD *afd, NodoAFD* Estado){
    
    afd->Estados = (NodoAFD**)realloc( ( void* )afd->Estados, sizeof( NodoAFD* ) * ( ++afd->NumEstados ) );
    if ( !(afd->Estados) ){
        perror( "Error al reservar espacio para Estados" );
        exit(EXIT_FAILURE);
    }
    *(afd->Estados+(afd->NumEstados-1)) = Estado;
}

void AgregarEstadoFinalAFD(AFD *afd, NodoAFD* EstadoFinal){
    
    afd->Finales = (NodoAFD**)realloc( ( void* )afd->Finales, sizeof( NodoAFD* ) * ( ++afd->NumFinales ) );
    if ( !(afd->Finales) ){
        perror( "Error al reservar espacio para Estados" );
        exit(EXIT_FAILURE);
    }
    *(afd->Finales+(afd->NumFinales-1)) = EstadoFinal;
}

/*
 *  AgregarSimboloAFD:  
 *      Agregar un simbolo al alfabeto del AFD
 *      Entrada:
 *             afd: AFD al que se le agregara el simbolo
 *         simbolo: simbolo que se agregara a el AFD
 *       Salida:
 *             El afd contendra el simbolo en el Alfabeto
 */
void AgregarSimboloAFD(AFD *afd, char simbolo){

    if ( ComprobarAlfabeto( afd->alfabeto, afd->NumSimbolos, simbolo ) ){
        afd->alfabeto = (char *)realloc( (void*)afd->alfabeto, sizeof( char ) * ( ++afd->NumSimbolos ) );
        if ( !(afd->alfabeto) ){
            perror( "Error al reservar espacio para el alfabeto" );
            exit(EXIT_FAILURE);
        }
        *(afd->alfabeto+(afd->NumSimbolos-1)) = simbolo;
    }
    
}

/*
 *  AgregarEstadoSig:  
 *      Crear la transicion entre 2 estados mediante un simbolo
 *      Entrada:
 *          Estado: Estado origen de la transicion
 *       EstadoSig: Estado final de la transicion
 *         simbolo: simbolo para la transicion
 *       Salida:
 *             El estado con la transicion al estado siguiente
 */
void AgregarEstadoSig(NodoAFD* Estado, NodoAFD* EstadoSig, char simbolo){

    Estado->EstadosSiguiente = (NodoAFD**)realloc( (void*)Estado->EstadosSiguiente, sizeof( NodoAFD* ) * ( ++Estado->NumeroDeEstadosSig ) );
    if ( !(Estado->EstadosSiguiente) ){
        perror( "Error al reservar espacio para Estados Siguientes" );
        exit(EXIT_FAILURE);
    }

    Estado->SimbolosTransiciones = (char *)realloc( (void*)Estado->SimbolosTransiciones, sizeof( char ) * ( Estado->NumeroDeEstadosSig ) );
    if ( !(Estado->SimbolosTransiciones) ){
        perror( "Error al reservar espacio para los simbolos de transicion" );
        exit(EXIT_FAILURE);
    }

    *(Estado->EstadosSiguiente+(Estado->NumeroDeEstadosSig-1)) = EstadoSig;
    *(Estado->SimbolosTransiciones+(Estado->NumeroDeEstadosSig-1)) = simbolo;

}

