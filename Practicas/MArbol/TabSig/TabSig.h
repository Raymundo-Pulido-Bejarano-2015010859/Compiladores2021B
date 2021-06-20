#include "defTabSig.h"
#ifndef TABSIGH
#define TABSIGH

void AgregarSiguientePos( tabsig* tabla, int posicion, int siguiente );

void AgregarNodo( tabsig* tabla, int numposicion, char simbolo );

tabsig* CrearTabSig();

#endif