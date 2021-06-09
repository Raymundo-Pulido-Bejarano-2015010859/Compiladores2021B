#ifndef DEFAFNH
#define DEFAFNH

/*
 * NodoAFN:
 *  ID                  : Identificador del Nodo
 *  NumeroDeEstadosSig  : Contador de elementos conectados
 *  EstadosSiguiente    : Arreglo de estados siguientes
 *  SimbolosTransiciones: Simbolos de transicion al estado siguiente
 */

typedef struct NodoAFN{
    int ID;
    int NumeroDeEstadosSig;
    struct NodoAFN **EstadosSiguiente;
    char *SimbolosTransiciones;
}NodoAFN;

/*
 * AFN :
 *  alfabeto     : Alfabeto del AFN
 *  NumSimbolos  : Numero de simbolos en el alfabeto
 *  Estados      : Estados del AFN
 *  NumEstados   : Numero de estados del AFN
 *  Inicio       : Apuntador al estado inicial
 *  Final        : Apuntador al estado final
 */

typedef struct AFN {
    char *alfabeto;
    int NumSimbolos;
    NodoAFN **Estados;
    int NumEstados;
    NodoAFN *Inicio;
    NodoAFN *Final;
}AFN;


#endif