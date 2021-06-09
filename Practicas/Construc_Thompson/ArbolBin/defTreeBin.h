#ifndef DEFTREEBINH
#define DEFTREEBINH

/*
 *  TIPO : Tipo de dato con el que se gestionara los datos del Arbol binario
 *  DIMP : Formato para la escritura de TIPO 
 */
#define TIPO char
#define DIMP "%c"

/*
 *       dato: Datos en nodo del Arbol
 *    derecho: Apuntador a la rama Derecha del Arbol
 *  izquierdo: Apuntador a la rama Izquierda del Arbol
 */
typedef struct TreeBin{
    TIPO datos;
    struct TreeBin* derecho;
    struct TreeBin* izquierdo;
}nodoBin;

#endif