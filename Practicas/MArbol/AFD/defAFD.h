#ifndef DEFAFDH
#define DEFAFDH

/*
 * NodoAFD:
 *  ID                  : Identificador del Nodo
 *  NumeroDeEstadosSig  : Contador de elementos conectados
 *  EstadosSiguiente    : Arreglo de estados siguientes
 *  SimbolosTransiciones: Simbolos de transicion al estado siguiente
 */

typedef struct NodoAFD{
    int ID;
    int NumeroDeEstadosSig;
    struct NodoAFD **EstadosSiguiente;
    char *SimbolosTransiciones;
}NodoAFD;

/*
 * AFD :
 *  alfabeto     : Alfabeto del AFD
 *  NumSimbolos  : Numero de simbolos en el alfabeto
 *  Estados      : Estados del AFD
 *  NumEstados   : Numero de estados del AFD
 *  Inicio       : Apuntador al estado inicial
 *  Final        : Apuntador al estado final
 */

typedef struct AFD {
    char *alfabeto;
    int NumSimbolos;
    NodoAFD **Estados;
    int NumEstados;
    NodoAFD *Inicio;
    NodoAFD **Finales;
    int NumFinales;
}AFD;


#endif