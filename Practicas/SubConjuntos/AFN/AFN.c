/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Biblioteca de Automata Finito No Determinista 
 * Correo : rayescomed@gmail.com 
 */

#include <stdio.h>
#include <stdlib.h>
#include "defAFN.h"

/*
 *  CrearNodoAFN:  
 *      Crea un Nodo del Automata Finito No determinista
 *      Entrada:
 *          ID: Identificador del nodo
 *       Salida:
 *          nodoAux: nodo inicializado con el identificador   
 */
NodoAFN* CrearNodoAFN(int ID){
    NodoAFN* nodoAux = (NodoAFN*) malloc( sizeof( NodoAFN ) );
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
 *  CrearAFN:  
 *      Crea un Automata Finito No determinista vacio, para contener todos los elementos de estos.
 *      Entrada:
 *          
 *       Salida:
 *          afnAux: AFN inicializado en nulo. 
 */
AFN* CrearAFN( void ){
    AFN *afnAux = (AFN*) malloc( sizeof( AFN ) );
    if ( !afnAux ){
        perror( "Error al reservar espacio para AFN" );
        exit(EXIT_FAILURE);
    }
    afnAux->alfabeto = NULL;
    afnAux->Estados = NULL;
    afnAux->Inicio = NULL;
    afnAux->Final = NULL;
    afnAux->NumSimbolos = 0;
    afnAux->NumEstados = 0;
    afnAux->NumEstadosFinales = 0;
    return afnAux;
}

/*
 *  CrearAFN:  
 *      Crea un Automata Finito No determinista vacio, para contener todos los elementos de estos.
 *      Entrada:
 *         alfabeto: Arreglo con los simbolos de un AFN
 *          NumAlfa: Numero de simbolos del AFN
 *          simbolo: simbolo a comprobar si ya es parte del alfabeto del AFN
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
 *  AgregarEstadoAFN:  
 *      Agregar un estado al conjunto de estados del AFN
 *      Entrada:
 *             afn: AFN al que se le agregara el estado
 *          Estado: Estado que se agregara a el AFN
 *       Salida:
 *             El afn contendra el estado
 */
void AgregarEstadoAFN(AFN *afn, NodoAFN* Estado){
    
    afn->Estados = (NodoAFN**)realloc( ( void* )afn->Estados, sizeof( NodoAFN* ) * ( ++afn->NumEstados ) );
    if ( !(afn->Estados) ){
        perror( "Error al reservar espacio para Estados" );
        exit(EXIT_FAILURE);
    }
    *(afn->Estados+(afn->NumEstados-1)) = Estado;

}

/*
 *  AgregarSimboloAFN:  
 *      Agregar un simbolo al alfabeto del AFN
 *      Entrada:
 *             afn: AFN al que se le agregara el simbolo
 *         simbolo: simbolo que se agregara a el AFN
 *       Salida:
 *             El afn contendra el simbolo en el Alfabeto
 */
void AgregarSimboloAFN(AFN *afn, char simbolo){

    if ( ComprobarAlfabeto( afn->alfabeto, afn->NumSimbolos, simbolo ) ){
        afn->alfabeto = (char *)realloc( (void*)afn->alfabeto, sizeof( char ) * ( ++afn->NumSimbolos ) );
        if ( !(afn->alfabeto) ){
            perror( "Error al reservar espacio para el alfabeto" );
            exit(EXIT_FAILURE);
        }
        *(afn->alfabeto+(afn->NumSimbolos-1)) = simbolo;
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
void AgregarEstadoSig(NodoAFN* Estado, NodoAFN* EstadoSig, char simbolo){

    Estado->EstadosSiguiente = (NodoAFN**)realloc( (void*)Estado->EstadosSiguiente, sizeof( NodoAFN* ) * ( ++Estado->NumeroDeEstadosSig ) );
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

/*
 *  AgregarEstadoFinalAFN:  
 *      Agregar un estado al conjunto de estados finales del AFN
 *      Entrada:
 *             afn: AFN al que se le agregara el estado
 *          Estado: Estado que se agregara a el AFN
 *       Salida:
 *             El afn contendra el estado
 */
void AgregarEstadoFinalAFN(AFN *afn, NodoAFN* Estado){
    
    afn->Final = (NodoAFN**)realloc( ( void* )afn->Final, sizeof( NodoAFN* ) * ( ++afn->NumEstadosFinales ) );
    if ( !(afn->Estados) ){
        perror( "Error al reservar espacio para Estados Finales" );
        exit(EXIT_FAILURE);
    }
    *(afn->Final+(afn->NumEstadosFinales-1)) = Estado;

}
/*
 *  ComprobarESFinal:  
 * Comprobar si un identificador es una estado final
 *      Entrada:
 *             afn: AFN al que se le agregara el estado
 *   identificador: identificador del estado
 *       Salida:
 *             1: se encuentra
 *             0: No se encuentra
 */
int ComprobarESFinal(AFN *afn, int identificador){
    for(int inde = 0; inde < afn->NumEstadosFinales ; inde++ ){
        if( identificador == (*(afn->Final+inde))->ID )
            return 1;
    }
    return 0;
}

/*
 *  MostrarAFN :
 *  Mostramos el AFN en pantalla
 */
void MostrarAFN(AFN* afn_Completo){
    printf("Alfabeto:");
    for(int simb = 0; simb < afn_Completo->NumSimbolos; simb++ )
        printf("%c,",*(afn_Completo->alfabeto+simb));
    printf("\n%%\n");
    printf("Estado Inicial: %d", afn_Completo->Inicio->ID);
    printf("\n%%\n");
    printf("Estados Finales: ");
    for(int simb = 0; simb < afn_Completo->NumEstadosFinales ; simb++ )
        printf("%d,", (*(afn_Completo->Final+simb))->ID );
    printf("\n%%\nTransiciones: (I,F,S)\n");
    for(int indice = 0; indice < afn_Completo->NumEstados ; indice++){
        for(int estados = 0; estados < (*(afn_Completo->Estados+indice))->NumeroDeEstadosSig ; estados++ ){
            printf("%d,%d,%c\n", (*(afn_Completo->Estados+indice))->ID,(*((*(afn_Completo->Estados+indice))->EstadosSiguiente+estados))->ID, *((*(afn_Completo->Estados+indice))->SimbolosTransiciones+estados) );
        }
    }

}