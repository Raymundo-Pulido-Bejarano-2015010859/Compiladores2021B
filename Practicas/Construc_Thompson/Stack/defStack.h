#ifndef DEFSTACK
#define DEFSTACK

#define TIPO char
#define DIMP "%c"

typedef struct Pila {
    TIPO *datos;
    int top;
    int longitud;
}stack;

#endif