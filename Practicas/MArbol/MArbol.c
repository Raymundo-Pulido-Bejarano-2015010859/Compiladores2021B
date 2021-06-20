/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Practica3 - Metodo del Arbol
 * Correo : rayescomed@gmail.com 
 * Entrada:    
 *      Expresion Regular
 * Salida:
 *      AFD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack/stack.h"
#include "ArbolBin/TreeBin.h"
#include "RegexToPostfix/RegexPost.h"
#include "StackTreeBin/stack.h"
#include "TabSig/TabSig.h"
#include "AFD/AFD.h"


void Proceso_Base(void);
void Proceso_CLI(char*);
void AgregarFinCad(char*);
char* ReservandoRegex( int );
nodoBin* Creacion_Arbol_Sintactico(char*);

void AgregarPrimeras(nodoBin* nodoV,int prim);
void AgregarUltimas(nodoBin* nodoV, int ultim);
void AnotarPosiciones(nodoBin* Central, int* posicion, tabsig* tablaSigPos);
void AnotarAnulables(nodoBin* Central);
void AnotarPrimeros(nodoBin* Central);
void AnotarUltimas(nodoBin* Central);
void LlenarTablaSiguiente(nodoBin* Central, tabsig* tablaSigPos);
void CrearAFDArbol(nodoBin* Padre, tabsig* tablaSPos,AFD *afdCompleto);

int main(int argc, char const *argv[]){
    /*
        Opciones:
            - 1 <regex>; se recive la regex desde la terminal
    */

    if( argc == 1 ){
        Proceso_Base( );
        return 0;
    }else if ( argc == 3 ){
        int opc = atoi(*(argv+1));
        if ( opc == 1 ){
            Proceso_CLI( (char*)*(argv+2) );
            return 0;
        }
    }else
        printf( "Estructura incorrecta | Ejecutable.out <NumOP> <Cadena_OP>" );
    return -1;
}

void AgregarPrimeras(nodoBin* nodoV, int prim){
    nodoV->primeras = ( int* )realloc( nodoV->primeras, sizeof(int) * ++(nodoV->Nprimeras) );
    if ( !nodoV->primeras ){
        perror("Error al recervar primeras del nodo");
        exit(EXIT_FAILURE);
    }

    nodoV->primeras[(nodoV->Nprimeras)-1] = prim;
}

void AgregarUltimas(nodoBin* nodoV, int ultim){
    nodoV->ultimas = ( int* )realloc( nodoV->ultimas, sizeof(int) * ++(nodoV->Nultimas) );
    if ( !nodoV->ultimas ){
        perror("Error al recervar primeras del nodo");
        exit(EXIT_FAILURE);
    }

    nodoV->ultimas[(nodoV->Nultimas)-1] = ultim;
}

void AnotarPosiciones(nodoBin* Central, int* posicion, tabsig* tablaSigPos){

    if ( Central->izquierdo )
        AnotarPosiciones( Central->izquierdo, posicion, tablaSigPos );
    if ( Central->derecho )
        AnotarPosiciones( Central->derecho, posicion, tablaSigPos );

    if ( EsSimboloAlfabeto( Central->datos ) ){
        Central->posicion = (*posicion)++;
        AgregarNodo( tablaSigPos, Central->posicion, Central->datos );
    }
}

void AnotarAnulables(nodoBin* Central){

    if ( Central->izquierdo )
        AnotarAnulables( Central->izquierdo );
    if ( Central->derecho )
        AnotarAnulables( Central->derecho );

    if ( !EsSimboloAlfabeto( Central->datos ) ){
        
        if ( Central->datos == '*' || Central->datos == 'E' ){
            Central->anulable = 1;
        }else if ( Central->datos == '.' ){
            if ( Central->izquierdo->anulable && Central->derecho->anulable ){
                Central->anulable = 1;
            }else {
                Central->anulable = 0;
            }
        }else if ( Central->datos == '|' ){
            if ( Central->izquierdo->anulable || Central->derecho->anulable ) {
                Central->anulable = 1;
            }else{
                Central->anulable = 0;
            }
        }
    }else{
        Central->anulable = 0;
    }
}

void AnotarPrimeros(nodoBin* Central){

    if ( Central->izquierdo )
        AnotarPrimeros( Central->izquierdo );
    if ( Central->derecho )
        AnotarPrimeros( Central->derecho );

    if ( !EsSimboloAlfabeto( Central->datos ) ){
        if ( Central->datos == '*' ){
            if( Central->izquierdo->Nprimeras > 0){
                for(int nump = 0; nump < Central->izquierdo->Nprimeras ; nump++ ){
                    AgregarPrimeras( Central, Central->izquierdo->primeras[nump] );
                }
            }
        }else if ( Central->datos == '.' ){
            if ( Central->izquierdo->anulable ){
                if( Central->izquierdo->Nprimeras > 0){
                    for(int nump = 0; nump < Central->izquierdo->Nprimeras ; nump++ ){
                        AgregarPrimeras( Central, Central->izquierdo->primeras[nump] );
                    }
                }
                if( Central->derecho->Nprimeras > 0){
                    for(int nump = 0; nump < Central->derecho->Nprimeras ; nump++ ){
                        AgregarPrimeras( Central, Central->derecho->primeras[nump] );
                    }
                }
            }else {
                if( Central->izquierdo->Nprimeras > 0){
                    for(int nump = 0; nump < Central->izquierdo->Nprimeras ; nump++ ){
                        AgregarPrimeras( Central, Central->izquierdo->primeras[nump] );
                    }
                }
            }
        }else if ( Central->datos == '|' ){
            if( Central->izquierdo->Nprimeras > 0){
                for(int nump = 0; nump < Central->izquierdo->Nprimeras ; nump++ ){
                    AgregarPrimeras( Central, Central->izquierdo->primeras[nump] );
                }
            }
            if( Central->derecho->Nprimeras > 0){
                for(int nump = 0; nump < Central->derecho->Nprimeras ; nump++ ){
                    AgregarPrimeras( Central, Central->derecho->primeras[nump] );
                }
            }
        }

    }else{
        AgregarPrimeras( Central, Central->posicion );
    }
}

void AnotarUltimas(nodoBin* Central){

    if ( Central->izquierdo )
        AnotarUltimas( Central->izquierdo );
    if ( Central->derecho )
        AnotarUltimas( Central->derecho );

    if ( !EsSimboloAlfabeto( Central->datos ) ){
        if ( Central->datos == '*' ){
            if( Central->izquierdo->Nultimas > 0){
                for(int numU = 0; numU < Central->izquierdo->Nultimas ; numU++ ){
                    AgregarUltimas( Central, Central->izquierdo->ultimas[numU] );
                }
            }
        }else if ( Central->datos == '.' ){
            if ( Central->derecho->anulable ){
                if( Central->izquierdo->Nultimas > 0){
                    for(int numU = 0; numU < Central->izquierdo->Nultimas ; numU++ ){
                        AgregarUltimas( Central, Central->izquierdo->ultimas[numU] );
                    }
                }
                if( Central->derecho->Nultimas > 0){
                    for(int numU = 0; numU < Central->derecho->Nultimas ; numU++ ){
                        AgregarUltimas( Central, Central->derecho->ultimas[numU] );
                    }
                }
            }else {
                if( Central->derecho->Nultimas > 0){
                    for(int numU = 0; numU < Central->derecho->Nultimas ; numU++ ){
                        AgregarUltimas( Central, Central->derecho->ultimas[numU] );
                    }
                }
            }
        }else if ( Central->datos == '|' ){
            if( Central->izquierdo->Nultimas > 0){
                for(int numU = 0; numU < Central->izquierdo->Nultimas ; numU++ ){
                    AgregarUltimas( Central, Central->izquierdo->ultimas[numU] );
                }
            }
            if( Central->derecho->Nultimas > 0){
                for(int numU = 0; numU < Central->derecho->Nultimas ; numU++ ){
                    AgregarUltimas( Central, Central->derecho->ultimas[numU] );
                }
            }
        }

    }else{
        AgregarUltimas( Central, Central->posicion );
    }
}

void LlenarTablaSiguiente(nodoBin* Central, tabsig* tablaSigPos){
    if ( Central->izquierdo )
        LlenarTablaSiguiente( Central->izquierdo, tablaSigPos );
    if ( Central->derecho )
        LlenarTablaSiguiente( Central->derecho, tablaSigPos );

    if( Central->datos == '.' ){
        if(Central->izquierdo->Nultimas > 0 && Central->derecho->Nprimeras > 0){
            for(int numU = 0; numU < Central->izquierdo->Nultimas ; numU++ ){
                for(int nump = 0; nump < Central->derecho->Nprimeras ; nump++ ){
                    AgregarSiguientePos(tablaSigPos, Central->izquierdo->ultimas[numU], Central->derecho->primeras[nump] );
                }
            }
        }
    }else if( Central->datos == '*' ){
        if(Central->Nultimas > 0 && Central->Nprimeras > 0){
            for(int numU = 0; numU < Central->Nultimas ; numU++ ){
                for(int nump = 0; nump < Central->Nprimeras ; nump++ ){
                    AgregarSiguientePos(tablaSigPos, Central->ultimas[numU], Central->primeras[nump] );
                }
            }
        }
    }
}

void CrearAFDArbol(nodoBin* Padre, tabsig* tablaSPos, AFD *afdCompleto){

    tabsig* tablaEstadosAux = CrearTabSig();

    AgregarNodo(tablaEstadosAux,1,'x');
    int ultimoNodo = 0;
    int numero = 1;
    int FinalFlag = 0;
    for(int ind = 0; ind < Padre->Nprimeras; ind++){
        AgregarSiguientePos(tablaEstadosAux,1,Padre->primeras[ind]);
        if(Padre->primeras[ind] == tablaSPos->nodos[tablaSPos->NumNodos-1] ){
            FinalFlag = 1;
        }
    }
    for(int indA = 0; indA < tablaSPos->NumNodos-1; indA++){
        AgregarSimboloAFD(afdCompleto, tablaSPos->simbolos[indA]);
    }

    NodoAFD *aux = CrearNodoAFD(1); //Inicial
    NodoAFD *aux2 = NULL;          
    AgregarEstadoAFD(afdCompleto, aux);
    afdCompleto->Inicio = aux;
    if ( FinalFlag ){
        AgregarEstadoFinalAFD(afdCompleto, aux);
    }
    int estadonuevo[ tablaSPos->NumNodos ] ;
    
    do {

        //Obtener el nodo del estado a trabajar
        for(int estadosind = 0; estadosind < afdCompleto->NumEstados; estadosind++){
            if( afdCompleto->Estados[estadosind]->ID == ultimoNodo+1 ){
                aux = afdCompleto->Estados[estadosind];
                break;
            }
        }        
        
        for (int Lalfabeto = 0; Lalfabeto < afdCompleto->NumSimbolos; Lalfabeto++){
            //Limpia el estado auxiliar
            for (int i = 0; i < tablaSPos->NumNodos; i++){ 
                estadonuevo[i] = -1;
            }
            for(int elem = 0; elem < tablaEstadosAux->NumSigPos[ultimoNodo] ; elem++ ){
                if ( tablaSPos->simbolos[ tablaEstadosAux->sigpos[ultimoNodo][elem] - 1 ] == afdCompleto->alfabeto[Lalfabeto] ){ 
                    // comprobamos que cada elemento del estado es de que simbolo para las transiciones
                    for(int indiceSigPosTab = 0; indiceSigPosTab < tablaSPos->NumSigPos[tablaEstadosAux->sigpos[ultimoNodo][elem] - 1] ; indiceSigPosTab++ ){
                        int esta = 0;
                        for (int indiceEstadoNuevo1 = 0; indiceEstadoNuevo1 < tablaSPos->NumNodos; indiceEstadoNuevo1++){
                            if( estadonuevo[indiceEstadoNuevo1] == tablaSPos->sigpos[tablaEstadosAux->sigpos[ultimoNodo][elem]-1][indiceSigPosTab]){
                                esta = 1;
                                break;
                            }
                        }
                        if (!esta){
                            for (int indiceEstadoNuevo2 = 0; indiceEstadoNuevo2 < tablaSPos->NumNodos; indiceEstadoNuevo2++){
                                if (estadonuevo[indiceEstadoNuevo2] == -1){
                                    estadonuevo[indiceEstadoNuevo2] = tablaSPos->sigpos[tablaEstadosAux->sigpos[ultimoNodo][elem]-1][indiceSigPosTab];
                                    break;
                                }
                            }
                        }
                    }
                    
                }
            }
            //Comprobar que el estado no exista
            if( estadonuevo[0] != -1 ){
                int estadoyaexiste = 0;

                int lenestadonuevo = 0;
                int nodoexiste = -1;
                for(int indecenuevo = 0; indecenuevo < tablaSPos->NumNodos ; indecenuevo++){
                    if ( estadonuevo[indecenuevo] != -1){
                        lenestadonuevo++;
                    }else{
                        break;
                    }
                }
                //Comprobar si existe
                for(int indicetab = 0; indicetab < tablaEstadosAux->NumNodos; indicetab++){
                    int ayudaa = 0;
                    
                    for(int indicesig = 0; indicesig < tablaEstadosAux->NumSigPos[indicetab]; indicesig++){
                        for(int indecenuevo = 0; indecenuevo < lenestadonuevo ; indecenuevo++){
                            if ( estadonuevo[indecenuevo] == tablaEstadosAux->sigpos[indicetab][indicesig]){
                                ayudaa ++;
                                break;
                            }
                        }
                    }

                    if ( ayudaa == lenestadonuevo && ayudaa == tablaEstadosAux->NumSigPos[indicetab] ){
                        estadoyaexiste = 1;
                        nodoexiste = indicetab;
                    }
                }

                if (!estadoyaexiste) {
                    
                    int FinalFlag = 0;
                    AgregarNodo(tablaEstadosAux, ++numero , afdCompleto->alfabeto[Lalfabeto]);
                    // ya tenemos el estado nuevo
                    for(int indiceCarga = 0; indiceCarga < lenestadonuevo; indiceCarga++ ){
                        AgregarSiguientePos(tablaEstadosAux, numero , estadonuevo[indiceCarga] );
                        if( estadonuevo[indiceCarga] == tablaSPos->nodos[tablaSPos->NumNodos-1] ){
                            FinalFlag = 1;
                        }
                    }
                    aux2 = CrearNodoAFD(numero);
                    AgregarEstadoAFD(afdCompleto, aux2 );
                    if ( FinalFlag )
                        AgregarEstadoFinalAFD(afdCompleto, aux2); 
                    AgregarEstadoSig(aux,aux2,afdCompleto->alfabeto[Lalfabeto]);

                    for (int i = 0; i < tablaSPos->NumNodos; i++){ //Limpia el estado auxiliar
                        estadonuevo[i] = -1;
                    }
                }else{
                    for(int estadosind = 0; estadosind < afdCompleto->NumEstados; estadosind++){
                        if( afdCompleto->Estados[estadosind]->ID == tablaEstadosAux->nodos[nodoexiste] ){
                            aux2 = afdCompleto->Estados[estadosind];
                            break;
                        }
                    }
                    AgregarEstadoSig(aux,aux2,afdCompleto->alfabeto[Lalfabeto]);
                    for (int i = 0; i < tablaSPos->NumNodos; i++){ //Limpia el estado auxiliar
                        estadonuevo[i] = -1;
                    }
                }
                
            }

        }
        ultimoNodo++;    
        
    }while(ultimoNodo != tablaEstadosAux->NumNodos);
    
}
/*
 *  Creacion_Arbol_Sintactico :
 *  Crear el arbol sintactico apartir de una regex en postfijo
 * 
 *  Entrada:
 *      regex: regex con concatenacion explicita en postfijo
 *  Salida:
 *      nodoCen: nodo padre del arbol sintactico
 */
nodoBin* Creacion_Arbol_Sintactico(char* Regex){

    nodoBin *nodoCen = NULL;
    stackN* arbol = CrearStackN();
    char actual;

    for( int indice = 0 ; indice < strlen( Regex ) ; indice++ ){
        actual = Regex[ indice ];
        if( actual != '*' && actual != '.' && actual != '|' ){
            nodoCen = CrearNodoBinario( actual );
            PushN( arbol, nodoCen ); 
        }else if( actual == '*' ){
            nodoCen = CrearNodoBinario( actual );
            nodoCen->izquierdo = PopN( arbol );
            PushN( arbol, nodoCen );
        }else if( actual == '.' || actual == '|' ){
            nodoCen = CrearNodoBinario( actual );
            nodoCen->derecho = PopN( arbol );
            nodoCen->izquierdo = PopN( arbol );
            PushN( arbol, nodoCen );
        }
    }
    
    nodoCen = PopN( arbol );

    return nodoCen;
}

/*
 *  AgregarFinCad :
 *  Se grega fin de cadena para cadenas leidas mediante fgets 
 *  Entrada:
 *      regex :  Cadena con \n al final 
 *  Salida:
 *      Cadena sin \n y con \0 al final 
 */
void AgregarFinCad( char* regex ){
    int indice = 0;
    while( regex[ indice ] != '\0' ){
        if ( regex[ indice ] == '\n' ){
            regex[ indice ] = '\0';
            break;
        }
        indice++;
    }
}

/*
 *  ReservandoRegex :
 *  Se reserva espacio para cadena
 *  Entrada:
 *      tam   :  Tamaño de la cadena
 *  Salida:
 *      regex :  Apuntador al espacio reservado
 */
char* ReservandoRegex( int tam ){
    char *regex = (char*)malloc( sizeof(char)*tam );
    if ( !regex ){
        perror("No se pudo reservar la memoria");
        exit(EXIT_FAILURE);
    }
    return regex;
}

/* 
 * Procesamiento de una sola regex 
 */
void Proceso_Base( void ){
    
    nodoBin* Padre = NULL;
    char *regex = NULL; 
    AFD *afdCompleto = CrearAFD();
    int posicion = 1;
    
    printf("Ingrese regex: ");
    regex = LeerRegex( 70 );
    regex = AgregarSMArbol( regex );

    if( !ValidacionDeSimbolosRegex( regex ) ){
        printf("La Regex contiene elementos invalidos: %s",regex);
        return ;
    }
    if ( !( regex = AgregarConcatenacionRegex( regex ) )  ){
		perror("Error en la expresion regular, estructura invalida");
        exit(EXIT_FAILURE);	
	} 
    regex = RegexToPostfix(regex);
    Padre = Creacion_Arbol_Sintactico(regex);

    tabsig* tablaSPos = CrearTabSig();
    AnotarPosiciones( Padre, &posicion, tablaSPos );
    AnotarAnulables( Padre );
    AnotarPrimeros( Padre );
    AnotarUltimas( Padre );
    LlenarTablaSiguiente(Padre, tablaSPos);
    printf("La tabla de siguientes posiciones es: \n");
    printf("Nodo     |   SiguientePos(n)\n");
    for(int indice1 = 0; indice1 < tablaSPos->NumNodos; indice1++){
        printf(" %3d    %c    |   ",tablaSPos->nodos[indice1],tablaSPos->simbolos[indice1]);
        for(int indice2 = 0; indice2 < tablaSPos->NumSigPos[indice1]; indice2++ ){
            printf("%d,",tablaSPos->sigpos[indice1][indice2]);
        }
        printf("\n");
    }
    CrearAFDArbol(Padre, tablaSPos, afdCompleto);

    
    printf("\nEl afd es: \n");
    printf("Estado Inicial: \n");
    printf("%d\n",afdCompleto->Inicio->ID);
    printf("Simbolos: \n");
    for(int a = 0; a < afdCompleto->NumSimbolos ; a++){
        printf("%c,",afdCompleto->alfabeto[a]);
    }
    printf("\nEstados: \n");
    for(int a = 0; a < afdCompleto->NumEstados; a++){
        printf("%d: \n",afdCompleto->Estados[a]->ID);
        for(int b = 0; b < afdCompleto->Estados[a]->NumeroDeEstadosSig; b++ ){
            printf(" -%c> %d \n",afdCompleto->Estados[a]->SimbolosTransiciones[b],afdCompleto->Estados[a]->EstadosSiguiente[b]->ID);
        }
    }
    printf("\nEstados Finales: \n");
    for(int a = 0; a < afdCompleto->NumFinales; a++){
        printf("%d, ",afdCompleto->Finales[a]->ID);
    }
    printf("\n");
    free(afdCompleto);
    free(Padre);
    free(tablaSPos);
    free(regex);

}

/* 
 * Procesamiento de una sola regex desde la terminal
 */
void Proceso_CLI( char* regex ){

    AFD *afdCompleto = CrearAFD();
    nodoBin* Padre = NULL;
    int posicion = 1;
    regex = AgregarSMArbol( regex );
    printf("La regex del arbol es: %s\n",regex);

    if( !ValidacionDeSimbolosRegex( regex ) ){
        printf("La Regex contiene elementos invalidos: %s",regex);
        return ;
    }

    if ( !( regex = AgregarConcatenacionRegex( regex ) )  ){
		perror("Error en la expresion regular, estructura invalida");
        exit(EXIT_FAILURE);	
	} 
    regex = RegexToPostfix(regex);
    Padre = Creacion_Arbol_Sintactico(regex);

    tabsig* tablaSPos = CrearTabSig();
    AnotarPosiciones( Padre, &posicion, tablaSPos );
    AnotarAnulables( Padre );
    AnotarPrimeros( Padre );
    AnotarUltimas( Padre );
    LlenarTablaSiguiente(Padre, tablaSPos);
    printf("La tabla de siguientes posiciones es: \n");
    printf("Nodo     |   SiguientePos(n)\n");
    for(int indice1 = 0; indice1 < tablaSPos->NumNodos; indice1++){
        printf(" %3d    %c    |   ",tablaSPos->nodos[indice1],tablaSPos->simbolos[indice1]);
        for(int indice2 = 0; indice2 < tablaSPos->NumSigPos[indice1]; indice2++ ){
            printf("%d,",tablaSPos->sigpos[indice1][indice2]);
        }
        printf("\n");
    }
    CrearAFDArbol(Padre, tablaSPos, afdCompleto);

    printf("\nEl afd es: \n");
    printf("Estado Inicial: \n");
    printf("%d\n",afdCompleto->Inicio->ID);
    printf("Simbolos: \n");
    for(int a = 0; a < afdCompleto->NumSimbolos ; a++){
        printf("%c,",afdCompleto->alfabeto[a]);
    }
    printf("\nEstados: \n");
    for(int a = 0; a < afdCompleto->NumEstados; a++){
        printf("%d: \n",afdCompleto->Estados[a]->ID);
        for(int b = 0; b < afdCompleto->Estados[a]->NumeroDeEstadosSig; b++ ){
            printf(" -%c> %d \n",afdCompleto->Estados[a]->SimbolosTransiciones[b],afdCompleto->Estados[a]->EstadosSiguiente[b]->ID);
        }
    }
    printf("\nEstados Finales: \n");
    for(int a = 0; a < afdCompleto->NumFinales; a++){
        printf("%d, ",afdCompleto->Finales[a]->ID);
    }
    printf("\n");
    free(afdCompleto);
    free(Padre);
    free(tablaSPos);
}
