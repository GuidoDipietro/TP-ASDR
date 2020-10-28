#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compilador.h"

char buffer[TAMLEX]; 		// Temp para token leído
TOKEN tokenActual;			// Token actual
int flagToken = 0;			// flag

// Tabla de símbolos con las palabras reservadas cargadas previamente
RegTS TS[1000] = {
    {"inicio",INICIO},
    {"fin",FIN},
    {"leer",LEER},
    {"escribir",ESCRIBIR},
    {"$",99}
};

// in-file
#define TAMNOM (20+1)
FILE* in;

////// MAIN //////

int main(int argc, char** argv){
	TOKEN tok;
	char nombreFile[TAMNOM];
	int l;
	// Apertura de archivo
	if (argc==1){
		printf("No se ingreso nombre de archivo.");
		return EXIT_FAILURE;
	}
	if (argc!=2) {
		printf("Numero incorrecto de argumentos.");
		return EXIT_FAILURE;
	}
	// Sin error
	strcpy(nombreFile, argv[1]);
	l = strlen(nombreFile);
	if (l>TAMNOM){
		printf("Nombre muy largo (debe ser menor a 20 caracteres).");
		return EXIT_FAILURE;
	}
	if (nombreFile[l-1]!='m' || nombreFile[l-2]!='.'){
		printf("Error: La extension debe ser .m");
		return EXIT_FAILURE;
	}
	// Intenta abrir el archivo
	if ( (in = fopen(nombreFile, "r")) == NULL){
		printf("Error abriendo el archivo.");
	}

	////// INICIO COMPILACION //////
	Objetivo();
    fclose(in);
	return EXIT_SUCCESS;
}