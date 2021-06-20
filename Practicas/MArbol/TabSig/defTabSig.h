#ifndef DEFTABSIG
#define DEFTABSIG


typedef struct TablaSig {
    int *nodos;
    char *simbolos;
    int **sigpos;
    int NumNodos;
    int *NumSigPos;
}tabsig;

#endif