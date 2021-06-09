#include "../Lista/defList.h"

#ifndef DEFESTADOSINTH
#define DEFESTADOSINTH

typedef struct EstadoInt{
    int ID;
    short Marca;
    List *Kernel;
    List *CerraduraC;
    struct EstadoInt **EstadosSiguientes;
    char *SimbolosTransiciones; 
    int NumSimbols;
    struct EstadoInt *sig;
} EstadoInt;


#endif