/*
	Pulido Bejarano Raymundo
	Analizador sintactico por descenso recursivo
*/
#include <stdio.h>
#include <string.h>
// Gramatica
// A ::= BCDEa
// B ::= bCD | a
// C ::= f | cA
// D ::= d
// E ::= e

int indice = 0;

int A( char* cadena ); 
int B( char* cadena );
int C( char* cadena ); 
int D( char* cadena ); 
int E( char* cadena ); 
int consumir( char terminal, char* cadena );

void main(void){
	char correcta[] = "bfdcafdeadea";
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

// A ::= BCDEa			
int A( char *cadena ){

	if ( B( cadena )  ){
		if ( C( cadena ) ){
			if ( D( cadena ) ){
				if ( E( cadena )){
					return consumir( 'a', cadena );
				}
			}
		}
	}

	return 0;
}

// B ::= bCD | a
int B( char *cadena ){
	
	if( consumir( 'b', cadena ) ){
		if ( C( cadena ) ){
			return D( cadena );
		}
	}else if( consumir( 'a', cadena ) ){
		return 1;
	}

	return 0;
}

// C ::= f | cA
int C( char *cadena ){
	if( consumir( 'f', cadena ) ){
		return 1;
	}else if( consumir( 'c', cadena ) ){
		return A( cadena );
	}

	return 0;
}

// D ::= d
int D( char *cadena ){
	return consumir( 'd', cadena );
}

// E ::= e
int E( char *cadena ){
	return consumir( 'e', cadena );
}
