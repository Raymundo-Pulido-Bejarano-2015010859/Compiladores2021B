/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Biblioteca de Arbol Binario
 * Correo : rayescomed@gmail.com  
 */
#include <stdio.h>
#include <stdlib.h>
#include "defTreeBin.h"

/*
 *  CrearNodoBinario:  
 *      Crea un Nodo del Arbol Binario
 *      Entrada:
 *          data: dato que se almacenar en el nodod
 *       Salida:
 *          nodoBin aux: nodo inicializado con el dato   
 */
nodoBin* CrearNodoBinario(TIPO data){
    nodoBin* aux = (nodoBin*)malloc(sizeof(nodoBin));
    if ( !aux ){
        perror("Error al crear nodo Binario");
        exit(EXIT_FAILURE);
    }
    aux->datos = data;
    aux->derecho = aux->izquierdo = NULL;
    return aux;
}

/*
 *  AgregarNodoBinario:  
 *      Crea un Nodo del Arbol Binario
 *      Entrada:
 *         padre: nodo padre del arbol
 *          hijo: nodo hijo que se agregara al padre
 *          lado:
 *              -1 = lado izquierdo
 *               1 = lado derecho
 *       Salida:
 *          padre contiene a el hijo en lado señalada   
 */
void AgregarNodoBinario(nodoBin* padre, nodoBin* hijo, short lado){
 
    if ( !padre || !hijo || lado == 0 ){
        perror("Error al agregar nodo Binario");
        exit(EXIT_FAILURE);
    }
    if( lado == -1 )
        padre->izquierdo = hijo;
    else if( lado == 1)
        padre->derecho = hijo;
}

/*
 *  DestruirNodobinario:  
 *      Destruccion del nodo del Arbol Binario
 *      Entrada:
 *         nodo: nodo del arbol 
 *       Salida:
 * 
 */
void DestruirNodobinario(nodoBin* nodo){
    free(nodo);
}

/*
 *  DestruirNodobinario:  
 *      Recorido del Arbol del PreOrden
 *      Entrada:
 *         nodo: nodo del arbol 
 *       Salida:
 */
void PreOrden(nodoBin* nodo){
    printf("%c->",nodo->datos);
    if ( nodo->izquierdo )
        PreOrden(nodo->izquierdo);
    if ( nodo->derecho )
        PreOrden(nodo->derecho);
}

/*
 *  DestruirNodobinario:  
 *      Recorido del Arbol del InOrden
 *      Entrada:
 *         nodo: nodo del arbol 
 *       Salida:
 */
void InOrden(nodoBin* nodo){
    if ( nodo->izquierdo )
        InOrden(nodo->izquierdo);
    printf(DIMP,nodo->datos);
    printf("->");
    if ( nodo->derecho )
        InOrden(nodo->derecho);
}

/*
 *  DestruirNodobinario:  
 *      Recorido del Arbol del PostOrden
 *      Entrada:
 *         nodo: nodo del arbol 
 *       Salida:
 */
void PostOrden(nodoBin* nodo){
    if ( nodo->izquierdo )
        PostOrden(nodo->izquierdo);
    if ( nodo->derecho )
        PostOrden(nodo->derecho);
    printf(DIMP,nodo->datos);
    printf("->");
}
