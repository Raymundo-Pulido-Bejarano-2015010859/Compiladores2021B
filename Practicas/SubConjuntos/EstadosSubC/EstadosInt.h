#include "../Lista/defList.h"
#include "defEstadosInt.h"

#ifndef ESTADOSINTH
#define ESTADOSINTH

void MostrarEstadosInt( EstadoInt *lista );
void EliminarEstadoInt( EstadoInt *lista );
EstadoInt *CrearNodoEstadoInt( int identificador );
int ListaVaciaEstadoInt( EstadoInt *lista );
void AppendEstadoInt( EstadoInt **lista, EstadoInt *nodo );
EstadoInt *ObtenerPorKernel( EstadoInt *lista, List *kernel );
int AgregarEstadoIntSig( EstadoInt *Estado, EstadoInt* EstadoSig, char simbolo);
EstadoInt *ObtenerEstadoPozo( EstadoInt *lista, List *kernel );
EstadoInt *ObtenerEstadoPorID( EstadoInt *lista, int identificador );
EstadoInt *ObtenerEstadosPorElemCerradura( EstadoInt *lista, int identificadorAFN );
EstadoInt *CopiarEstadoInt( EstadoInt *estado );

#endif