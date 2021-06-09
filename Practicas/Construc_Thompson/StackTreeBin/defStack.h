#include "../ArbolBin/defTreeBin.h"
#ifndef DEFSTACKN
#define DEFSTACKN

#define TIPOS nodoBin*
#define DIMP "%c"

typedef struct PilaN {
    TIPOS* datos;
    int top;
    int longitud;
}stackN;

#endif