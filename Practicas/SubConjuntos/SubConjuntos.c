/*
    SubConjuntos.c     
    Practica 2 - Algoritmo de los SubConjuntos
    Entrada:    
        AFN
    Salida:
        AFD
    Nombre: Pulido Bejarano Raymundo
    Materia: Compiladores
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AFN/AFN.h"
#include "Lista/List.h"
#include "EstadosSubC/EstadosInt.h"

AFN* LeerAFN(char* Path);
char *LeerLinea( FILE *Descriptor, int numCarac );
List *CerraduraEpsilon( List *Kernel, AFN* AFN_C );
List* MoverSC( List *estados, char simbolo );
EstadoInt *SubConjuntos( AFN* AFN_C );
void MostrarAFD( AFN* AFN_C, EstadoInt* AFD_Estados);
void GuardarAFD( AFN* AFN_C, EstadoInt* AFD_Estados, char* path );
void AFNtoPNG( AFN* AFN_C, EstadoInt* AFD_Estados, char* Path);
EstadoInt *ObtenerEstadosFinales( AFN* AFN_C, EstadoInt* AFD_Estados );
void RecibirAFN(char* Path);

int main( int argc, char **argv ){

    char *Path = NULL;

    if( argc == 3){
        if( strcmp(*(argv+1),"-p") == 0)
            Path = *(argv+2);   
    }else if( argc == 2){
        if( strcmp(*(argv+1),"-TC") == 0 ){
            printf("Ingrese el path del archivo que contiene el AFN: \n");
            Path = LeerLinea( stdin, 30 );
        }
    }else{
        Path = "./AFN_Recibido.txt";
        RecibirAFN(Path);
    }


    AFN* AFN_Recibido = LeerAFN(Path);

    MostrarAFN( AFN_Recibido );

    EstadoInt *Res = SubConjuntos( AFN_Recibido );

    MostrarAFD(AFN_Recibido,Res);
    GuardarAFD(AFN_Recibido, Res, "AFD_Format/AFD_1.txt");
    AFNtoPNG( AFN_Recibido,Res,"AFD_PNG/AFD_1.gv");
    
    return 0;
}

void AFNtoPNG( AFN* AFN_C, EstadoInt* AFD_Estados, char* Path){
    
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
    
    sprintf(cad_aux,"\tlabelloc=\"t\";\n\tlabel=\"AFN->AFD\";\n");
    fwrite(cad_aux,1,strlen(cad_aux),Archivo);

    //Estado inicial
    sprintf(cad_aux,"\tsecret_node -> 0 [label = \"I\"];\n");
    fwrite(cad_aux,1,strlen(cad_aux),Archivo);

    //Estados Finales

    for( EstadoInt *lista = ObtenerEstadosFinales( AFN_C, AFD_Estados ) ; lista ; lista = lista->sig )
        fprintf(Archivo,"\t%d [shape=doublecircle];\n", lista->ID);
    
    //Transicion entre estados
    for (EstadoInt *Res = AFD_Estados ; Res ; Res = Res->sig)
        for( int ind = 0; ind < Res->NumSimbols; ind ++)
            fprintf(Archivo,"\t%d -> %d [label = \"%c\"]; \n", Res->ID, (*(Res->EstadosSiguientes+ind))->ID, *(Res->SimbolosTransiciones+ind) );


    fwrite("\n}",1,2,Archivo);
    fclose(Archivo);
}

EstadoInt *ObtenerEstadosFinales( AFN* AFN_C, EstadoInt* AFD_Estados ){
    EstadoInt *lista = NULL, *aux = NULL, *aux2 = NULL, *aux3 = NULL;
    for( int ind = 0; ind < AFN_C->NumEstadosFinales ; ind++ ){
        AppendEstadoInt( &lista, ObtenerEstadosPorElemCerradura(AFD_Estados, (*(AFN_C->Final+ind))->ID ) );
    }
    aux = lista;
    while( aux ){
        aux3 = aux;
        aux2 = aux->sig;
        while( aux2 ){
            if( aux->ID == aux2->ID ){
                aux3->sig = aux2->sig;
                aux2 = aux3->sig;
            }else{
                aux3 = aux2;
                aux2 = aux2->sig;
            }
        }
        aux = aux->sig;
    }

    return lista;

}
/*
 *  GuardarAFD :
 *  Guardar el AFd en formato txt 
 */
void GuardarAFD( AFN* AFN_C, EstadoInt* AFD_Estados, char* path ){
    FILE *Salida = fopen(path,"w");

    for(int simb = 0; simb < AFN_C->NumSimbolos; simb++ )
        fprintf(Salida,"%c,",*(AFN_C->alfabeto+simb));
    
    fprintf(Salida,"\n%%\n");
    fprintf(Salida,"0");
    fprintf(Salida,"\n%%\n");
    for( EstadoInt *lista = ObtenerEstadosFinales( AFN_C, AFD_Estados ) ; lista ; lista = lista->sig )
        fprintf(Salida,"%d\n", lista->ID );
    fprintf(Salida,"%%\n");
    for( EstadoInt *lista = AFD_Estados ; lista ; lista = lista->sig )
        fprintf(Salida,"%d\n", lista->ID );
    fprintf(Salida,"%%\n");
    for (EstadoInt *Res = AFD_Estados ; Res ; Res = Res->sig)
        for( int ind = 0; ind < Res->NumSimbols; ind ++)
            fprintf(Salida,"%d,%d,%c\n", Res->ID, (*(Res->EstadosSiguientes+ind))->ID, *(Res->SimbolosTransiciones+ind) );

    fclose(Salida);
}

void MostrarAFD( AFN* AFN_C, EstadoInt* AFD_Estados){
    EstadoInt *Res = NULL;
    printf("\nAlfabeto:");
    for(int simb = 0; simb < AFN_C->NumSimbolos; simb++ )
        printf("%c,",*(AFN_C->alfabeto+simb));
    
    printf("\n%%\n");
    printf("Estado Inicial: 0");
    printf("\n%%\n");
    printf("Estados Finales: \n");
    Res = ObtenerEstadosFinales( AFN_C, AFD_Estados );
    if( Res != NULL){
        MostrarEstadosInt( ObtenerEstadosFinales( AFN_C, AFD_Estados ) );
    }else{
        perror("Error: Error al Generar el AFD");
        exit(EXIT_FAILURE);
    }
    printf("%%\nTransiciones: (I,F,S)\n");
    for (EstadoInt *Res = AFD_Estados ; Res ; Res = Res->sig)
        for( int ind = 0; ind < Res->NumSimbols; ind ++)
            printf("%d,%d,%c\n", Res->ID, (*(Res->EstadosSiguientes+ind))->ID, *(Res->SimbolosTransiciones+ind) );
    
}

EstadoInt *SubConjuntos( AFN* AFN_C ){
    int count = 0;
    EstadoInt *Estados = NULL, *Resultado = NULL;
    EstadoInt *EstadoAux = NULL;
    List *KernelAux= NULL;

    // Paso 1: Calcular Cerradura Epsilon (Estado inicial)
    EstadoAux = CrearNodoEstadoInt( count++ );
    Append( &(EstadoAux->Kernel), AFN_C->Inicio );
    EstadoAux->CerraduraC = CerraduraEpsilon( EstadoAux->Kernel, AFN_C );
    
    AppendEstadoInt( &Estados, EstadoAux );

    Resultado = Estados;
    while( Estados ){
        Estados->Marca = 1;
        for(int i = 0; i < AFN_C->NumSimbolos ; i++ ){
            KernelAux = MoverSC( Estados->CerraduraC, *(AFN_C->alfabeto+i) );
            if ( KernelAux )
                EstadoAux = ObtenerPorKernel( Resultado, KernelAux ); 
            else
                EstadoAux = ObtenerEstadoPozo( Resultado, KernelAux );
            
            if ( EstadoAux ){
                AgregarEstadoIntSig( Estados, EstadoAux, *(AFN_C->alfabeto+i) );
            }else{
                EstadoAux = CrearNodoEstadoInt( count++ );
                EstadoAux->Kernel = KernelAux;
                EstadoAux->CerraduraC = CerraduraEpsilon(EstadoAux->Kernel,AFN_C);
                AppendEstadoInt( &Resultado, EstadoAux );

                AgregarEstadoIntSig( Estados, EstadoAux, *(AFN_C->alfabeto+i) );
            }
        }
        Estados = Estados->sig;
    }


    return Resultado;
}

List* MoverSC( List *estados, char simbolo ){
    List *Resultado = NULL;
    while ( estados ){
        for (int ind = 0; ind < estados->datos->NumeroDeEstadosSig; ind++)
            if( *(estados->datos->SimbolosTransiciones+ind) == simbolo )
                Append( &Resultado, *(estados->datos->EstadosSiguiente+ind) );

        estados = estados->sig;
    }
    return Resultado;
}

List *CerraduraEpsilon( List *Kernel, AFN* AFN_C ){
    List *Cerradura = NULL, *Resultado = NULL;

    for ( ; Kernel ; Kernel = Kernel->sig )
        Append( &Cerradura, Kernel->datos );

    Resultado = Cerradura;

    while ( Cerradura ){

        int ind = 0;
        while( (*(AFN_C->Estados+ind++))->ID != Cerradura->datos->ID );
        ind--;
        for( int i = 0; i < (*(AFN_C->Estados+ind))->NumeroDeEstadosSig ; i++ )
            if( *((*(AFN_C->Estados+ind))->SimbolosTransiciones+i) == 'E' )
                if( !ExisteEnLista( Resultado, *((*(AFN_C->Estados+ind))->EstadosSiguiente+i) ) )
                    Append( &Cerradura, *((*(AFN_C->Estados+ind))->EstadosSiguiente+i) );

        Cerradura = Cerradura->sig;
    }
    return Resultado;
}

char* LeerLinea( FILE *Descriptor, int numCarac ){

    int indice = 0;
    char *Linea = ( char* )malloc( numCarac );
    if ( !Linea ){
        perror("Error reservando espacio para linea");
        exit(EXIT_FAILURE);
    }

    fgets( Linea, numCarac, Descriptor);

    while( Linea[indice++] != '\n');
    Linea[indice-1] = '\0';

    return Linea;

}

void RecibirAFN(char* Path){
    FILE *ArchvioAFN = fopen(Path,"w");
    char *buffer = (char*)malloc( 50 );
    int Estado = 0;

    if ( !ArchvioAFN ){
        perror("Error abriendo archvio de AFN");
        exit(EXIT_FAILURE);
    }
    if ( !buffer ){
        perror("Error al reservar buffer");
        exit(EXIT_FAILURE);
    }

    // Leemos el Alfabeto 
    printf("Ingrese el alfabeto [a-z], separados por ' , ' : ");
    if ( fgets( buffer, 50, stdin ) ){
        if ( strcmp( buffer, "\n" ) == 0 ){
            perror("Error al recibir el alfabeto");
            exit(EXIT_FAILURE);
        }
        for( int simbolo = 0 ; *(buffer+simbolo) != '\n' ; simbolo++ )
            if( *(buffer+simbolo) != ',' && simbolo%2 == 0 ){
                if( ( *(buffer+simbolo) <= 122 && *(buffer+simbolo) >= 97 ) ){
                    fprintf(ArchvioAFN,"%c,",*(buffer+simbolo));
                }else{
                    perror("Error en el alfabeto");
                    exit(EXIT_FAILURE);
                }
            }else if( *(buffer+simbolo) != ',' && simbolo%2 != 0 ){
                perror("Error: Ingrese el alfabeto en el formato correcto");
                exit(EXIT_FAILURE);
            }
    }else{
        perror("Error en el alfabeto");
        exit(EXIT_FAILURE);
    }

    //Descartando separador (%)
    fprintf(ArchvioAFN,"\n%%\n");

    // Leemos el estado Inicial
    printf("Ingrese el estado inicial (#Numero): ");
    if ( fgets( buffer, 50, stdin ) ){
        if(strcmp( buffer, "\n" ) == 0){
            perror("Error al recibir el estado inicial");
            exit(EXIT_FAILURE);
        }
        Estado = atoi(buffer);
        fprintf(ArchvioAFN,"%d", Estado );
    }else{
        perror("Error al recibir el estado inicial");
        exit(EXIT_FAILURE);
    }
    
    //Descartando separador (%)
    fprintf(ArchvioAFN,"\n%%\n");

    printf("Ingrese los estados finales, ingresar ENTER para terminar : \n");
    fgets( buffer, 50, stdin );
    if ( strcmp( buffer, "\n" ) == 0 ){
        perror("Error al recibir el estado Final");
        exit(EXIT_FAILURE);
    }
    while( strcmp( buffer, "\n" ) != 0 ){
        Estado = atoi(buffer);
        fprintf(ArchvioAFN,"%d\n", Estado );

        fgets( buffer, 50, stdin ); 
    }
    
    //Descartando separador (%)
    fprintf(ArchvioAFN,"%%\n");

    printf("Ingrese todos los estados del AFN, ingresar ENTER para terminar : \n");
    fgets( buffer, 50, stdin );
    if ( strcmp( buffer, "\n" ) == 0 ){
        perror("Error al recibir los estados");
        exit(EXIT_FAILURE);
    }
    while( strcmp( buffer, "\n" ) != 0 ){
        Estado = atoi(buffer);
        fprintf(ArchvioAFN,"%d\n", Estado );

        fgets( buffer, 50, stdin ); 
    }

    //Descartando separador (%)
    fprintf(ArchvioAFN,"%%\n");
    
    printf("Ingrese las transiciones del AFN, ( Inicion, Fin, simbolo ) ENTER para terminar: \n");
    fgets( buffer, 50, stdin );
    if ( strcmp( buffer, "\n" ) == 0 ){
        perror("Error al recibir las transiciones");
        exit(EXIT_FAILURE);
    } 
    while( strcmp( buffer, "\n" ) != 0 ){

        fprintf(ArchvioAFN,"%s", buffer );

        fgets( buffer, 50, stdin ); 
    }

    fclose(ArchvioAFN);


}

AFN* LeerAFN(char* Path){
    FILE *ArchvioAFN = fopen(Path,"r");
    char *buffer = (char*)malloc( 50 );
    AFN *Resultado = CrearAFN();
    int Inicial = 0, Final = 0, aux = 0, C_Ini = -1, C_Fin = -1;
    char trans_aux ;

    if ( !ArchvioAFN ){
        perror("Error abriendo archvio de AFN");
        exit(EXIT_FAILURE);
    }
    if ( !buffer ){
        perror("Error al reservar buffer");
        exit(EXIT_FAILURE);
    }

    // Leemos el Alfabeto 
    if ( fgets( buffer, 50, ArchvioAFN ) ){
        for( int simbolo = 0 ; *(buffer+simbolo) != '\n' ; simbolo++ )
            if( *(buffer+simbolo) != ',')
                AgregarSimboloAFN(Resultado,*(buffer+simbolo));
    }else
        return NULL;

    //Descartando separador (%)
    fgets( buffer, 50, ArchvioAFN);

    // Leemos el estado Inicial
    fgets( buffer, 50, ArchvioAFN);
    Inicial = atoi(buffer);
    Resultado->Inicio = CrearNodoAFN(Inicial);
    AgregarEstadoAFN(Resultado,Resultado->Inicio);
    

    //Descartando separador (%)
    fgets( buffer, 50, ArchvioAFN);

    // Leemos el estado Final 
    fgets( buffer, 50, ArchvioAFN);
    while( strncmp( buffer, "%", 1 ) != 0 ){
        Final = atoi(buffer);
        AgregarEstadoFinalAFN(Resultado,CrearNodoAFN(Final));
        AgregarEstadoAFN(Resultado,*( Resultado->Final+(Resultado->NumEstadosFinales-1) ) );

        fgets( buffer, 50, ArchvioAFN);
    }


    // Estados
    fgets( buffer, 50, ArchvioAFN);
    while( strncmp( buffer, "%", 1 ) != 0 ){
        aux = atoi(buffer);
        if (aux != Inicial && ComprobarESFinal(Resultado,aux) == 0 )
            AgregarEstadoAFN(Resultado,CrearNodoAFN(aux));
        fgets( buffer, 50, ArchvioAFN);
    }
    
    // Transiciones
    while( !feof(ArchvioAFN) ){
        if( !fgets( buffer, 50, ArchvioAFN ) ){
            break;
        }
        if( sscanf(buffer,"%d,%d,%c",&Inicial,&Final,&trans_aux) != 3 )
            printf("Error en transiciones");
        C_Ini = C_Fin = -1;
        for(int indice = 0; indice < Resultado->NumEstados; indice++){
            if( (*(Resultado->Estados+indice))->ID == Inicial )
                C_Ini = indice;
            if( (*(Resultado->Estados+indice))->ID == Final )
                C_Fin = indice;
        }
        if ( C_Ini == -1 || C_Fin == -1){
            perror("Error en el AFN");
            exit(EXIT_FAILURE);
        }
        AgregarEstadoSig(*(Resultado->Estados+C_Ini),*(Resultado->Estados+C_Fin), trans_aux);
        
    }

    fclose(ArchvioAFN);

    return Resultado;

}