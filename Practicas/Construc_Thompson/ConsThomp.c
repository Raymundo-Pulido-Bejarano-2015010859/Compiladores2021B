/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Practica1 - Construcciones de Thompson
 * Correo : rayescomed@gmail.com 
 * Entrada:    
 *      Expresion Regular
 * Salida:
 *      AFN 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack/stack.h"
#include "ArbolBin/TreeBin.h"
#include "RegexToPostfix/RegexPost.h"
#include "StackTreeBin/stack.h"
#include "AFN/AFN.h"

void Proceso_Base(void);
void Proceso_CLI(char*);
void Proceso_Archivo( FILE*);
void AgregarFinCad(char*);
char* ReservandoRegex( int );
nodoBin* Creacion_Arbol_Sintactico(char*);
AFN* ConstruirThompson( AFN* AFN_Izq, AFN* AFN_Der, char simbolo, int *Count );
AFN* ConstruirAlfabetoThompson( char simbolo, int *ident );
void ConstruirCKlenneThompson( AFN* AFN_I, int *ident );
void ConstruirConcatenacionThompson( AFN* AFN_I, AFN* AFN_D, int *ident );
void ConstruirUnionThompson( AFN* AFN_I, AFN* AFN_D, int *ident );
AFN* CreacionAFNThompson(nodoBin* Central, int* index);
void GuardarAFN(AFN* afn_Completo, char* path);
void MostrarAFN(AFN* afn_Completo);

int main(int argc, char const *argv[]){
    /*
        Opciones:
            - 1 <regex>; se recive la regex desde la terminal
            - 2 <PATH>; se recive un archivo con las regex
    */

    if( argc == 1 ){
        Proceso_Base( );
        return 0;
    }else if ( argc == 3 ){
        int opc = atoi(*(argv+1));
        if ( opc == 1 ){
            Proceso_CLI( (char*)*(argv+2) );
            return 0;
        }else if( opc == 2 ){
            FILE* archivo = fopen( *(argv+2), "r" );
            if ( !archivo ){
                perror( "Error al abrir el archivo" );
                exit( EXIT_FAILURE );
            }
            Proceso_Archivo( archivo );
            return 0;             
        }
    }else
        printf( "Estructura incorrecta | Ejecutable.out <NumOP> <Cadena_OP>" );
    return -1;
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
 *  ConstruirThompson :
 *  Construir AFN de Thompson
 *      Valores ASCII de los posibles simbolos
 *          [a-z]   - alfabeto 		        ( 97 - 122 )
 *          E       - epsilon				( 69 )
 *          *       - Cerradura de Klenne	( 42 )
 *          .       - Concatenacion         ( 46 )
 *          |       - Union				    ( 124 )
 *          #       - Simbolo metodo Arbol	( 35 )
 *  Entrada:
 *      AFN_Izq :  Apuntador al AFN Izquierdo
 *      AFN_Der :  Apuntador al AFN Derecho
 *      simbolo :  El simbolo para detectar que estructura crear
 *      Count   :  Valor para el nodo
 *  Salida:
 *      AFN     :  AFN con la estructura de Thompson, dependiendo el tipo
 */
AFN* ConstruirThompson( AFN* AFN_Izq, AFN* AFN_Der, char simbolo, int *Count ){

    if ( ( simbolo <= 122 && simbolo >= 97 ) || simbolo == 35 || simbolo == 69 ){ // [a-z] | # | E
        return ConstruirAlfabetoThompson( simbolo, Count );
    }else if ( simbolo == 42 ){ // *
        if ( AFN_Izq )
            ConstruirCKlenneThompson( AFN_Izq, Count );
        return AFN_Izq;
    }else if ( simbolo == 46 ){ // .
        if ( AFN_Izq && AFN_Der){
            ConstruirConcatenacionThompson( AFN_Izq, AFN_Der, Count );
            return AFN_Izq;
        }
    }else if ( simbolo == 124 ){ // |
        if ( AFN_Izq && AFN_Der){
            ConstruirUnionThompson( AFN_Izq, AFN_Der, Count );
            return AFN_Izq;
        }
    }

    return NULL;
}

/*
 *  ConstruirAlfabetoThompson :
 *  Construir estructura de simbolo segun Thompson
 *  Entrada:
 *      simbolo :  El simbolo de la estructura de Thompson para transicion
 *      ident   :  Valor para los nodos
 *  Salida:
 *      AFN     :  AFN con la estructura de Thompson
 */
AFN* ConstruirAlfabetoThompson( char simbolo, int *ident ){

    AFN *ThompsonAlfabeto = NULL;
    NodoAFN* aux = NULL;

    ThompsonAlfabeto = CrearAFN();

    // Inicial
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(ThompsonAlfabeto,aux);
	if( simbolo != 'E' )
    	AgregarSimboloAFN(ThompsonAlfabeto,simbolo);
    ThompsonAlfabeto->Inicio = aux;

    // Final
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(ThompsonAlfabeto,aux);
    ThompsonAlfabeto->Final = aux;

    // Transicion Inicio --simbolo--> Final
    AgregarEstadoSig( ThompsonAlfabeto->Inicio, ThompsonAlfabeto->Final, simbolo );
    
    return ThompsonAlfabeto;
}

/*
 *  ConstruirCKlenneThompson :
 *  Construir estructura de Cerradura de Klenne segun Thompson
 *  Entrada:
 *      AFN_I   :  AFN al que se le aplicara la Cerradura de Klenne
 *      ident   :  Valor para los nodos
 *  Salida:
 *      AFN     :  AFN con la estructura de Thompson en AFN_I
 */
void ConstruirCKlenneThompson( AFN* AFN_I, int *ident ){
    NodoAFN* aux = NULL;
    
    // Coneccion antihoraria
    // Transicion Final --Epsilon--> Inicio
    AgregarEstadoSig( AFN_I->Final, AFN_I->Inicio, 'E' );

    // Nuevo estado inicial 
    // Transicion epsilon del nuevo Inicion al viejo Inicio
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(AFN_I,aux);
    AgregarEstadoSig( aux, AFN_I->Inicio,'E');
    AFN_I->Inicio = aux;
    
    // Nuevo estado Final
    // Transicion epsilon del viejo Final al nuevo Final
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(AFN_I,aux);
    AgregarEstadoSig( AFN_I->Final, aux,'E');
    AFN_I->Final = aux;

    // Coneccion horaria
    // Transicion Inicio --Epsilon--> Final
    AgregarEstadoSig( AFN_I->Inicio, AFN_I->Final, 'E' );

}

/*
 *  ConstruirConcatenacionThompson :
 *  Construir estructura de Concatenacion segun Thompson
 *  Entrada:
 *      AFN_I   :  AFN que llega por el lado izquierdo al que se le aplicara la Concatenacion 
 *      AFN_D   :  AFN que llega por el lado derecho al que se le aplicara la Concatenacion 
 *      ident   :  Valor para los nodos
 *  Salida:
 *      AFN     :  AFN con la concatenacion en estructura de Thompson en AFN_I
 */
void ConstruirConcatenacionThompson( AFN* AFN_I, AFN* AFN_D, int *ident ){
    
    // Agregando estados de AFN_D a AFN_I
    for (int est = 0; est < AFN_D->NumEstados ; est++)
        if( *(AFN_D->Estados+est) != AFN_D->Inicio)
            AgregarEstadoAFN( AFN_I, *(AFN_D->Estados+est) );

    // Agregando alfabeto de AFN_D a AFN_I
    for (int sim = 0; sim < AFN_D->NumSimbolos ; sim++)
        AgregarSimboloAFN( AFN_I, *(AFN_D->alfabeto+sim) );

    // Conectando el final del AFN Izquierdo a todos los estados que conectaba el inicio del AFN derecho
    for (int ind = 0; ind < ((AFN_D->Inicio)->NumeroDeEstadosSig); ind++ ){
        AgregarEstadoSig( AFN_I->Final, *((AFN_D->Inicio)->EstadosSiguiente+ind), *((AFN_D->Inicio)->SimbolosTransiciones+ind) );
    }

    AFN_I->Final = AFN_D->Final;

}

/*
 *  ConstruirUnionThompson :
 *  Construir estructura de Union segun Thompson
 *  Entrada:
 *      AFN_I   :  AFN que llega por el lado izquierdo al que se le aplicara la Union 
 *      AFN_D   :  AFN que llega por el lado derecho al que se le aplicara la Union 
 *      ident   :  Valor para los nodos
 *  Salida:
 *      AFN     :  AFN con la union en estructura de Thompson en AFN_I
 */
void ConstruirUnionThompson( AFN* AFN_I, AFN* AFN_D, int *ident ){
    NodoAFN* aux = NULL;

    // Agregando estados de AFN_D a AFN_I
    for (int est = 0; est < AFN_D->NumEstados ; est++)
        AgregarEstadoAFN( AFN_I, *(AFN_D->Estados+est) );

    // Agregando alfabeto de AFN_D a AFN_I
    for (int sim = 0; sim < AFN_D->NumSimbolos ; sim++)
        AgregarSimboloAFN( AFN_I, *(AFN_D->alfabeto+sim) );
    
    // Nuevo estado inicial 
    // Transicion epsilon del nuevo Inicion a los viejos Inicios
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(AFN_I,aux);
    AgregarEstadoSig( aux, AFN_I->Inicio,'E');
    AgregarEstadoSig( aux, AFN_D->Inicio,'E');
    AFN_I->Inicio = aux;

    // Nuevo estado Final
    // Transicion epsilon de los viejos Finales al nuevo Final
    aux = CrearNodoAFN( (*ident)++ );
    AgregarEstadoAFN(AFN_I,aux);
    AgregarEstadoSig( AFN_I->Final, aux,'E');
    AgregarEstadoSig( AFN_D->Final, aux,'E');
    AFN_I->Final = aux;

}

/*
 *  CreacionAFNThompson :
 *  Construir AFN de Thompson recorriendo el arbol sintactico
 *  Se recorre el arbol de forma inorden (Izq,Der,Cen)
 *  Entrada:
 *      Central :  Nodo de Arbol central 
 *      ident   :  Valor para los nodos del AFN
 *  Salida:
 *      AFN     :  AFN de Thompson 
 */
AFN* CreacionAFNThompson(nodoBin* Central, int* index){

    AFN *AFN_D = NULL, *AFN_I = NULL, *AFN_R = NULL;

    if ( Central->izquierdo )
        AFN_I = CreacionAFNThompson( Central->izquierdo, index );
    if ( Central->derecho )
        AFN_D = CreacionAFNThompson( Central->derecho, index );
    AFN_R = ConstruirThompson( AFN_I, AFN_D, Central->datos, index );
    if ( !AFN_R ){
        printf( "Error creando AFN, en: %c", Central->datos );
        exit(EXIT_FAILURE);
    }

    return AFN_R;

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
 *  GuardarAFN :
 *  Guardar el AFN en formato txt 
 */
void GuardarAFN(AFN* afn_Completo, char* path){
    FILE *Salida = fopen(path,"w");
    for(int simb = 0; simb < afn_Completo->NumSimbolos; simb++ )
        fprintf(Salida,"%c,", *(afn_Completo->alfabeto+simb) );
    
    fprintf(Salida,"\n%%\n");
    fprintf(Salida,"%d", afn_Completo->Inicio->ID);
    fprintf(Salida,"\n%%\n");
    fprintf(Salida,"%d", afn_Completo->Final->ID);
    fprintf(Salida,"\n%%\n");
    for(int indice = 0; indice < afn_Completo->NumEstados ; indice++){
        fprintf(Salida, "%d\n", (*(afn_Completo->Estados+indice))->ID );
    }
    fprintf(Salida,"%%\n");
    for(int indice = 0; indice < afn_Completo->NumEstados ; indice++){
        for(int estados = 0; estados < (*(afn_Completo->Estados+indice))->NumeroDeEstadosSig ; estados++ ){
            fprintf(Salida,"%d,%d,%c\n", (*(afn_Completo->Estados+indice))->ID,(*((*(afn_Completo->Estados+indice))->EstadosSiguiente+estados))->ID, *((*(afn_Completo->Estados+indice))->SimbolosTransiciones+estados) );
        }
    }
    fclose(Salida);
}

/*
 *  MostrarAFN :
 *  Mostramos el AFN en pantalla
 */
void MostrarAFN(AFN* afn_Completo){
    printf("Alfabeto:");
    for(int simb = 0; simb < afn_Completo->NumSimbolos; simb++ )
        printf("%c,",*(afn_Completo->alfabeto+simb));
    printf("\n%%\n");
    printf("Estado Inicial: %d", afn_Completo->Inicio->ID);
    printf("\n%%\n");
    printf("Estado Final %d\n", afn_Completo->Final->ID);
    printf("%%\nTransiciones: (I,F,S)\n");
    for(int indice = 0; indice < afn_Completo->NumEstados ; indice++){
        for(int estados = 0; estados < (*(afn_Completo->Estados+indice))->NumeroDeEstadosSig ; estados++ ){
            printf("%d,%d,%c\n", (*(afn_Completo->Estados+indice))->ID,(*((*(afn_Completo->Estados+indice))->EstadosSiguiente+estados))->ID, *((*(afn_Completo->Estados+indice))->SimbolosTransiciones+estados) );
        }
    }

}

/*
 *  AFNtoPNG :
 *  Guardar el AFN en formato gv, para crear un png con dot 
 */
void AFNtoPNG(AFN* afnThompson,char* Path,char *regex){
    
    // Resive path: nombre_#.gv

    FILE *Archivo = NULL;
    char *cad_aux = NULL;
    int longitud = 100;
    Archivo = fopen(Path,"w");
    if ( !Archivo ){
        perror("Error creando el Archivo dot");
        exit(EXIT_FAILURE);
    }
    cad_aux = (char*) malloc(longitud);
    if ( !cad_aux ){
        perror("Error reservando cadena");
        exit(EXIT_FAILURE);
    }
    strcpy(cad_aux,"digraph AFN{\n\trankdir=LR;\n\tnode [shape=circle];\n\tsecret_node [style=invis];\n");
    fwrite(cad_aux,1,76,Archivo);
    
    if ( strlen(regex) + 27 >= longitud){
        cad_aux = (char*)realloc( (void *) cad_aux,strlen(regex)+29);
    }
    sprintf(cad_aux,"\tlabelloc=\"t\";\n\tlabel=\"%s\";\n",regex);
    fwrite(cad_aux,1,strlen(cad_aux),Archivo);

    //Estado inicial
    sprintf(cad_aux,"\tsecret_node -> %d [label = \"I\"];\n",afnThompson->Inicio->ID);
    fwrite(cad_aux,1,strlen(cad_aux),Archivo);

    //Estado Final
    sprintf(cad_aux,"\t%d [shape=doublecircle];\n",afnThompson->Final->ID);
    fwrite(cad_aux,1,strlen(cad_aux),Archivo);

    //Transicion entre estados
    for(int indice = 0; indice < afnThompson->NumEstados ; indice++){
        for(int estados = 0; estados < (*(afnThompson->Estados+indice))->NumeroDeEstadosSig ; estados++ ){
            sprintf(cad_aux,"\t%d -> %d [label = \"%c\"]; \n", (*(afnThompson->Estados+indice))->ID, (*((*(afnThompson->Estados+indice))->EstadosSiguiente+estados))->ID, *((*(afnThompson->Estados+indice))->SimbolosTransiciones+estados) );
            fwrite(cad_aux,1,strlen(cad_aux),Archivo);
        }
    }

    fwrite("\n}",1,2,Archivo);
    fclose(Archivo);
}

/* 
 * Procesamiento de una sola regex 
 */
void Proceso_Base( void ){
    
    AFN *AFNThompson = NULL;
    nodoBin* Padre = NULL;
    char* original = NULL, *regex = NULL; 
    int indice = 0;
    
    printf("Ingrese regex: ");
    regex = LeerRegex( 70 );
    if( !ValidacionDeSimbolosRegex( regex ) ){
        printf("La Regex contiene elementos invalidos: %s",regex);
        return ;
    }
    original = strdup( regex );
    if ( !( regex = AgregarConcatenacionRegex( regex ) )  ){
		perror("Error en la expresion regular, estructura invalida");
        exit(EXIT_FAILURE);	
	} 
    regex = RegexToPostfix(regex);
    Padre = Creacion_Arbol_Sintactico(regex);
    AFNThompson = CreacionAFNThompson(Padre,&indice);
    if ( !AFNThompson ){
        printf( "Error creando AFN de Thompson" );
    }
    MostrarAFN(AFNThompson);
    GuardarAFN(AFNThompson,"./AFN_FORMAT/AFN_1.txt");
    AFNtoPNG(AFNThompson,"./AFN_PNG/AFN_1.gv",original);

    free( regex );
    free( original );
    free( Padre );
    free( AFNThompson->alfabeto );
    free( AFNThompson->Estados );
    free( AFNThompson->Final );
    free( AFNThompson->Inicio );
    free( AFNThompson );

    printf("\nEl AFN en formato (txt) esta en el directorio './AFN_FORMAT/' \n");
    printf("El AFN en formato (gv), para generar un png, esta en el directorio './AFN_PNG/' \n");

}

/* 
 * Procesamiento de una sola regex desde la terminal
 */
void Proceso_CLI( char* regex ){

    AFN *AFNThompson = NULL;
    nodoBin* Padre = NULL;
    int indice = 0;
    char* original = strdup( regex );

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
    AFNThompson = CreacionAFNThompson(Padre,&indice);
    if ( !AFNThompson ){
        printf( "Error creando AFN de Thompson" );
    }
    MostrarAFN(AFNThompson);
    GuardarAFN(AFNThompson,"./AFN_FORMAT/AFN_1.txt");
    AFNtoPNG(AFNThompson,"./AFN_PNG/AFN_1.gv",original);

    free( original );
    free( Padre );
    free( AFNThompson->alfabeto );
    free( AFNThompson->Estados );
    free( AFNThompson->Final );
    free( AFNThompson->Inicio );
    free( AFNThompson );

    printf("\nEl AFN en formato (txt) esta en el directorio './AFN_FORMAT/' \n");
    printf("El AFN en formato (gv), para generar un png, esta en el directorio './AFN_PNG/' \n");

}

/* 
 * Procesamiento de un archivo, que contiene las regex
 */
void Proceso_Archivo(FILE* archivo){
    
    char *original = NULL, *regex= NULL;
    int indice = 0,AFN_C = 0;
    nodoBin* Padre = NULL;
    AFN *AFNThompson = NULL;

    regex = ReservandoRegex( 70 );
    original = ReservandoRegex( 70 );

    while( !feof(archivo) ){
        if( fgets( original, 70, archivo ) ){
            AgregarFinCad( original );
            if( ValidacionDeSimbolosRegex( original ) ){

                strcpy(regex,original);
                printf("\nregex original:\n%s\n",regex);

                if ( !( regex = AgregarConcatenacionRegex( regex ) )  ){
                    perror("Error en la expresion regular, estructura invalida");
                    exit(EXIT_FAILURE);	
                } 
                regex = RegexToPostfix( regex );
                //printf("\nregex postfix: %s\n",regex);
                Padre = Creacion_Arbol_Sintactico(regex);
                AFNThompson = CreacionAFNThompson(Padre,&indice);
                if ( !AFNThompson ){
                    printf( "Error creando AFN de Thompson" );
                }
                
                MostrarAFN(AFNThompson);

                sprintf(regex,"./AFN_PNG/AFN_%d.gv",AFN_C);
                AFNtoPNG(AFNThompson,regex,original);
                
                sprintf(regex,"./AFN_FORMAT/AFN_%d.txt",AFN_C);
                GuardarAFN(AFNThompson,regex);
                
                AFN_C++;
                indice = 0;

                free( Padre );
                free( AFNThompson );

            }else   
                printf("La Regex contiene elementos invalidos: %s",regex);
        }
    }
    free(regex);
    free(original);

    printf("\nLos AFN en formato (txt) estan en el directorio './AFN_FORMAT/' \n");
    printf("Los AFN en formato (gv), para generar archivos png, estan en el directorio './AFN_PNG/' \n");
    
}
