#include <stdio.h>
#include <ctype.h>
#include "compilador.h"

extern int flagToken;
extern TOKEN tokenActual;
extern char buffer[TAMLEX];
extern RegTS TS;

void ErrorLexico() {
    printf("Error Lexico\n");
}

// Comprueba que el token que se le pasa sea igual al que lee
void Match(TOKEN t){
	if (!(t==ProximoToken())) ErrorSintactico();
	flagToken = 0;
}

// Próximo token que retorna el SCANNER/LEXER
TOKEN ProximoToken(){
	if(!flagToken){
		tokenActual = scanner();
		if (tokenActual == ERRORLEXICO) ErrorLexico();
		flagToken = 1;
		if (tokenActual == ID) Buscar(buffer, &tokenActual);
	}
	return tokenActual;
}

static int columna(int c){
    if (isalpha(c)) return 0;
    if (isdigit(c)) return 1;
    if (c == '+') return 2;
    if (c == '-') return 3;
    if (c == '(') return 4;
    if (c == ')') return 5;
    if (c == ',') return 6;
    if (c == ';') return 7;
    if (c == ':') return 8;
    if (c == '=') return 9;
    if (c == EOF) return 10;
    if (isspace(c)) return 11;
    return 12;
}
static int estadoFinal(int e){
    if (e == 0 || e == 1 || e == 3 || e == 11 || e == 14) return 0;
    return 1;
}

#define NUMESTADOS 15
#define NUMCOLS 13
extern FILE* in;
TOKEN scanner(){
    static int tabla[NUMESTADOS][NUMCOLS] = 
      //  L   D   +   -   (   )   ,   ;   :   =   EOF ´ ´ OTRO
    {
        { 1 , 3 , 5 , 6 , 7 , 8 , 9 , 10, 11, 14, 13, 0 , 14 }, // 0
        { 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2  }, // 1
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 2 ID
        { 4 , 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4  }, // 3
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 4 CTE
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 5 +
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 6 -
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 7 (
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 8 )
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 9 ,
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 10 ;
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 }, // 11 esp
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 12 ASIG
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }, // 13 FDT
        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }  // 14 err
    };

    int c; 			// Carácter a leer
    int col; 		// Columna de dicho carácter
    int estado = 0;	// Estado actual
    int ind = 0;	// Índice buffer
    do {
        c = fgetc(in);
        col = columna(c);
        estado = tabla[estado][col];
        if (col != 11) { //si es espacio no lo agrega al buffer
            buffer[ind] = c;
            ind++;
        }
    } while (!estadoFinal(estado) && !(estado == 14));

    buffer[ind] = '\0'; // Fin de cadena

    // ¿Qué token es?
    switch (estado) {
	    case 2:
	        if (col != 11) { //si el carácter espúreo no es blanco
	            ungetc(c, in); // lo retorna al flujo
	            buffer[ind - 1] = '\0';
	        }
	        return ID;
	    case 4:
	        if (col != 11) { // lo mismo
	            ungetc(c, in);
	            buffer[ind - 1] = '\0';
	        }
	        return CONSTANTE;
	    // Todos los otros tokens se reconocen sin necesidad del centinela
	    case 5:
	        return SUMA;
	    case 6:
	        return RESTA;
	    case 7:
	        return PARENL;
	    case 8:
	        return PARENR;
	    case 9:
	        return COMA;
	    case 10:
	        return PC;
	    case 12:
	        return ASIGNACION;
	    case 13:
	        return FDT;
	    case 14:
	        return ERRORLEXICO;
    }
    return 0;
}