#include <string.h>
#include "compilador.h"

extern RegTS TS[1000];

// Analiza si una cadena está en la TS, si está genera instrucción
void Chequear(char* s){
	TOKEN t;
	if (!Buscar(s, &t)){
		Colocar(s);
		Declarar(s);
	}
}

void Colocar(char* s){
	int i=4;
	for(i; strcmp("$", TS[i].id); i++); // fin de TS
	if (i < 999){
		// Crea registro
		strcpy(TS[i].id, s);
		TS[i].t = ID;
		// Mueve fin de tabla
		strcpy(TS[i+1].id, "$");
	}
}

int Buscar(char* s, TOKEN* t){
	for(int i=0; strcmp("$",TS[i].id); i++){
		if (!strcmp(s, TS[i].id)){
			*t = TS[i].t; // Token hallado
			return 1;
		}
	}
	return 0;
}