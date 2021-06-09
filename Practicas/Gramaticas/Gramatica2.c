/*
	Pulido Bejarano Raymundo
	Analizador por descenso recursivo
*/
#include <stdio.h>
#include <string.h>
// Gramatica
// S:: abSA | bA			
// A:: aA | c

int indice = 0;

int S( char* cadena ); 
int A( char* cadena ); 
int consumir( char terminal, char* cadena );

void main(void){
	char correcta[] = "baaaaaaac";
	char correcta2[] = "ababbacacc";
	char incorrecta[] = "abcba";	
	
	if( S( correcta ) && ( indice == strlen( correcta ) ) ){
		printf("La cadena si pertenece");
	}else{
		printf("La cadena no pertence");
	}
}

int consumir( char terminal, char* cadena ){
	if( cadena[indice] == terminal ){
		indice++;
		return 1;
	}
	return 0;
}

// S:: abSA | bA			
int S( char *cadena ){

	if( consumir( 'a', cadena )  ){
		if ( consumir( 'b', cadena ) ){
			if( S( cadena ) ){
				return A( cadena );
			}
		}
	}else if( consumir( 'b', cadena ) ){
		return A( cadena );
	}

	return 0;
}

// A:: aA | c
int A( char *cadena ){
	
	if( consumir( 'a', cadena ) ){
		return A( cadena );
	}else if( consumir( 'c', cadena ) ){
		return 1;
	}else{
		return 0;
	}

}

