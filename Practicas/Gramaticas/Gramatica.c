/*
	Pulido Bejarano Raymundo
	Analizador por descenso recursivo
*/
#include <stdio.h>
#include <string.h>
// Gramatica
// A:: aBa			
// B:: bAb | c	

int indice = 0;

int A( char* cadena ); 
int B( char* cadena ); 
int consumir( char terminal, char* cadena );

void main(void){
	char correcta[] = "abacaba";
	char incorrecta[] = "abcba";
	
	if( A( correcta ) && ( indice == strlen( correcta ) ) ){
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

// A:: aBa			
int A( char *cadena ){

	if ( consumir( 'a',cadena ) ){
		if ( B( cadena ) ){
			return consumir( 'a', cadena );
		}
	}

	return 0;
}

// B:: bAb | c
int B( char *cadena ){
	
	if( consumir( 'b', cadena ) ){
		if ( A( cadena ) ){
			return consumir( 'b', cadena );
		}
	}else if( consumir( 'c', cadena ) ){
		return 1;
	}
	
	return 0;
}

