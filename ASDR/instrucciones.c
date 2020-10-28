#include <stdio.h>
#include "compilador.h"

void Leer(REG_EXPRESION reg){
    Generar("LEER", reg.nombre, "ENTERA", "");
}
void Escribir(REG_EXPRESION reg){
	Generar("ESCRIBIR", reg.nombre, "ENTERA", "");
}
void Declarar(char* s){
	Generar("DECLARAR", s, "ENTERA", "");
}
void Asignar(REG_EXPRESION izq, REG_EXPRESION der) {
    Generar("ALMACENAR", der.nombre, izq.nombre, "");
}
void Terminar(void) {
    Generar("HALT", "", "", "");
}

// Printea instrucción
void Generar(char * COP, char * a, char * b, char * c) {
	// Instrucción para la MV
    printf("%s %s %s %s\n", COP, a, b, c);
}