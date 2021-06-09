/*
 * Nombre : Pulido Bejarano Raymundo
 * Fecha  : 17 - 04 - 21
 * Titulo : Biblioteca de Manejo de regex to postfijo
 * Correo : rayescomed@gmail.com 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Stack/stack.h"

/*
 *  ValidacionDeSimbolosRegex:  
 *		Validacion de los elementos de la regex
 * 			Los elementos validos para las regex son (Valor ASCII):
 * 			[a-z] - alfabeto 		( 97 - 122 )
 * 			E - epsilon				( 69 )
 * 			* - Cerradura de Klenne	( 42 )
 * 			| - Union				( 124 )
 * 			() - Parentesis			( 40 - 41 )
 * 			# Simbolo metodo Arbol	( 35 )
 *	Entrada:
 *		regex: regex con elementos a validar
 *  Salida:
 * 			0:	La regex contiene elementos invalidos
 * 			1:	La regex solo contiene elementos validos
 */
int ValidacionDeSimbolosRegex( char* regex ){

	char elem;
	for ( int indice = 0; indice < strlen( regex ); indice++ ){
		elem = regex[indice];
		if( elem != 69 && elem != 124 && elem != 35 && ( elem > 122 || elem < 97 ) && ( elem > 42 || elem < 40) )
			return 0;	
	}

	return 1;
	
}

/*
 *  LeerRegex:  
 *		Leer la regex, es decir reservar memoria leer la regex y agregar fin de cadena
 *	Entrada:
 *		tamano: Tamaño maximo a leer de la regex
 *  Salida:
 * 		 regex: apuntador a la regex
 */
char* LeerRegex( int tamano ){
	char* regex = ( char* )malloc( tamano );
	if( !regex ){
		perror("Error al reservar espacio para regex");
		exit(EXIT_FAILURE);
	}
	fgets(regex,tamano,stdin);
	for ( int indice = 0 ; regex[ indice ] != '\0' ; indice++ )
		if ( regex[ indice ] == '\n' ){
            regex[ indice ] = '\0';
            break;
        }

	return regex;
}

/*
 *  AgregarConcatenacionRegex:  
 *	Se agregan las concatenaciones explicita a la regex y se comprueba la estructura de la regex
 *	Operadores:
 *		* -> Cerradura de Klenne
 *      . -> Concatenacion
 *      | -> Union
 *      A -> Cualquier elemento del alfabeto
 *	Entrada:
 *		regex: regex a agregar la concatenacion explicita
 *  Salida:
 * 		Apuntador a la Regex con concatenaciones explicitas
 */
char* AgregarConcatenacionRegex( char* regex ){
   
	char anterior = 0,actual = 0;
	int longitud = strlen( regex ), ContSim = 0;
   	char* regexConConcatenaciones = (char*) malloc( strlen( regex ) );
   	
	// validar espacio para nueva cadena
	if( !regexConConcatenaciones ){
		perror("Error reservando memoria");
		exit(EXIT_FAILURE);
	}

    for( int indice = 0; indice < strlen( regex ) ; indice++){
        if( indice == 0 ){
			actual = regex[ indice ];
			// Se comprueba que el primer elemento no sea | o *
			if ( actual != '|' && actual != '*' )
				regexConConcatenaciones[ ContSim++ ] = actual;
			else
				return NULL;
        }else{
			anterior = regexConConcatenaciones[ ContSim -1 ];
            actual = regex[ indice ];

			// Comprueba si existe una estructura invalida en la regex,
			// Error si (|, (), (*, ||, |), |*, **
            if ( ( anterior == '(' && actual == '|' ) || ( anterior == '(' && actual == ')' ) || ( anterior == '(' && actual == '*' ) || ( anterior == '|' && actual == '|' ) || ( anterior == '|' && actual == ')' ) || ( anterior == '|' && actual == '*' ) || ( anterior == '*' && actual == '*' ) )
                return NULL;

			// Se agrega una simbolo de concatenacion si se encuentra una de las siguientes opciones
			// AA , A( , )A , )( , *A , *( 
            if( ( anterior != '(' && anterior != '|') && (actual != ')' && actual != '*' && actual != '|' ) ){
                 
				if ( ( longitud - ContSim ) <=1 ){
					regexConConcatenaciones = ( char* )realloc( (void*)regexConConcatenaciones, longitud + ( 2 - ( longitud - ContSim ) )  );
					longitud = longitud + ( 2 - (longitud - ContSim) );
					if ( !regexConConcatenaciones ){
						perror("Error al modificar el tamaño de la cadena");
						exit(EXIT_FAILURE);
					}
				}
				regexConConcatenaciones[ ContSim++ ] = '.';
				regexConConcatenaciones[ ContSim++ ] = actual;

            }else{	
				// Se agrega el elemento si no se encuentra ni una estructura para agregar concatenacion 
				if( ContSim >= longitud  ){
					regexConConcatenaciones = (char*)realloc( (void*)regexConConcatenaciones, longitud+1 );
					longitud = longitud + 1;
					if ( !regexConConcatenaciones ){
						perror("Error al modificar el tamaño de la cadena");
						exit(EXIT_FAILURE);
					}
				}	
				regexConConcatenaciones[ ContSim++ ] = actual ;
			}  
        }
	}
	// Se comprueba si el ultimo elemento es |
	if ( actual == '|')
		return NULL;

	regexConConcatenaciones = (char*)realloc( (void*)regexConConcatenaciones, longitud+1 );
	regexConConcatenaciones[ContSim] = '\0';
    return regexConConcatenaciones;
}

/*
 *  PrecedenciaOperadores:  
 *		Determina que operador tiene mas presedencia
 *	Precedencia de operadores en Regex * > . > |
 *		ascii: 	* -> 42
 *				. -> 46
 *				| -> 124
 *	Entrada:
 *		 OperadorAnterior: Operador anterior
 * 		OperadorSiguiente: Operador siguiente
 *  Salida:
 * 		-1 : si OpSig tiene menor precedencia que OpAnt
 *		 0 : si OpSig tiene igual precedencia que OpAnt
 *		 1 : si OpSig tiene mayor precedencia que OpAnt
 */
short PrecedenciaOperadores( char OperadorAnterior, char OperadorSiguiente){
	
	if( OperadorAnterior == OperadorSiguiente )
		return 0;
	else if ( OperadorSiguiente > OperadorAnterior )
		return -1;
	return 1;

}

/*
 *  RegexToPostfix:  
 *		Convierte una regex de infija a postfija
 *	Entra: 
 * 		regex: Expresion Regular con operador de concatenacion explicito (.) Infija
 *  Salida: 
 * 		regex: Expresion Regular con operador de concatenacion explicito (.) Postfija 
 */
char* RegexToPostfix(char* regex){

	stack* simbolos = CrearStack();
	char* regex2Postfix = ( char* )malloc( strlen( regex ) );
	char simbolo;
	int indicePost = 0, C_P = 0;

	// Se recorre la regex
	for(int indice = 0; indice < strlen( regex ); indice++){
		simbolo = regex[ indice ];
		// Comprueba si es un parentesis para contarlo y restar el espacio reservado para el resultado
		if( simbolo == '(' || simbolo == ')')
			C_P++;

		// Si es un simbolo del alfabeto se agrega directamente a el resultado
		if( simbolo != '(' && simbolo != ')' && simbolo != '*' && simbolo != '.' && simbolo != '|' )
			regex2Postfix[ indicePost++ ] = simbolo;
		else
			while( 1 ){ // se pone dentro del ciclo infinito por el caso de que el operador entrante tenga una precedencia menor que el ya esta dentro
				if( empty( simbolos ) || stacktop( simbolos ) == '('){
					Push( simbolos, simbolo );
					break;
				}else if( simbolo == '(' ){
					Push( simbolos, simbolo );
					break;
				}else if( simbolo == ')' ){ 
					while( stacktop( simbolos ) != '(' )
						regex2Postfix[ indicePost++ ] = Pop( simbolos );
					Pop( simbolos );
					break;
				}else if( PrecedenciaOperadores( stacktop( simbolos ), simbolo ) ==  1 ){
					Push( simbolos, simbolo );
					break;
				}else if( PrecedenciaOperadores( stacktop( simbolos ), simbolo ) == -1 ){
					regex2Postfix[ indicePost++ ] = Pop( simbolos );
				}else if( PrecedenciaOperadores( stacktop( simbolos ), simbolo ) ==  0 ){
					regex2Postfix[ indicePost++ ] = Pop( simbolos );
					Push( simbolos, simbolo );
					break;
				}
			}	
	}
	while( !empty( simbolos ) )
		regex2Postfix[ indicePost++ ] = Pop( simbolos );
	free( (void*)simbolos );
	regex2Postfix[ strlen( regex ) - C_P ] = '\0';
	return regex2Postfix;
}