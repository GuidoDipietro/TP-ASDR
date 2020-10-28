#include <string.h>
#include <stdio.h>
#include "compilador.h"

#define TAMLEX (32+1)

extern char buffer[TAMLEX];

void ErrorSintactico() {
    printf("Error Sintactico\n");
}

///// PAS /////

// Axioma y un poco más
void Objetivo(){
    // <objetivo> -> <programa> FDT #terminar
    Programa();
    Match(FDT);
    Terminar();
}
void Comenzar(){
    /* Inicializaciones Semanticas, en este caso no corresponde ninguna */
}
void Programa(){
    // <programa> -> #comenzar INICIO <listaSentencias> FIN
    Comenzar(); // (inicio semántico en caso de corresponder)
    Match(INICIO);
    ListaSentencias();
    Match(FIN);
}

// Otras PAS

static REG_EXPRESION ProcesarCte(){
	// Cadena numérica a entero + registro semántico
	REG_EXPRESION reg;
	reg.clase = CONSTANTE;
	strcpy(reg.nombre, buffer);
	sscanf(buffer, "%d", &reg.valor);

	return reg;
}
static REG_EXPRESION ProcesarID(){
	REG_EXPRESION reg;
	Chequear(buffer); // Analiza si está en la TS, si está genera instrucción
	// Asigna valores para el Struct
	reg.clase = ID;
	strcpy(reg.nombre, buffer);
	return reg;
}

void Id(REG_EXPRESION* reg){
	// <id> -> ID #procesarID
	Match(ID);
	*reg = ProcesarID(); // crea registro semántico
}
void ListaIDs(){
	// <listaIDs> -> <id> #leerID { COMA <id> #leerID }
	TOKEN t;
	REG_EXPRESION reg;
	Id(&reg);
	Leer(reg);
	for (t=ProximoToken(); t==COMA; t=ProximoToken()){
		Match(COMA);
		Id(&reg);
		Leer(reg);
	}
}

/*static char* NomReg(REG_EXPRESION* reg) {
    // Retorna la cadena del registro semantico
    return reg->nombre;
}*/
static REG_EXPRESION generarInfijo(REG_EXPRESION opIzq, char* op, REG_EXPRESION opDer){
	REG_EXPRESION reg;
	static unsigned numTemp = 1;
	// Cadenas
		// "Variable temporal"
	char cadTemp[TAMLEX] = "Temp";
	char cadNumTemp[TAMLEX];
		// Operador
	char cadOp[TAMLEX];
	// Instrucción MV
		// COP
	if (op[0]=='-') strcpy(cadOp, "RESTAR");
	if (op[0]=='+') strcpy(cadOp, "SUMAR");
		// "Variable temporal"
	sprintf(cadNumTemp, "%d", numTemp);
	numTemp++; strcat(cadTemp, cadNumTemp);
		// Instrucción "Declara X TIPO" si son IDs + colocar a TS etc
	if (opIzq.clase==ID) Chequear( opIzq.nombre );
	if (opDer.clase==ID) Chequear( opDer.nombre );
	Chequear(cadTemp); // Me parece bizarro agregar las variables Temporales a la TS, pero bueno

	Generar(cadOp, opIzq.nombre, opDer.nombre, cadTemp); // instrucción MV
	// Registro temporal TempN creado
	strcpy(reg.nombre, cadTemp);
	return reg;
}
void Primaria(REG_EXPRESION* reg){
	TOKEN tok = ProximoToken();
	switch (tok){
		// <primaria> -> <id>
		case ID:
			Id(reg);
			break;
		// <primaria> -> CONSTANTE #procesarCte
		case CONSTANTE:
			Match(CONSTANTE);
			*reg = ProcesarCte();
			break;
		// <primaria> -> ( <expresion> )
		case PARENL:
			Match(PARENL);
			Expresion(reg);
			Match(PARENR);
			break;
		default: return;
	}
}
void OperadorAditivo(char* op){
	// <operadorAditivo> -> SUMA # ProcesarOp | RESTA # ProcesarOp
	TOKEN t = ProximoToken();
	if (t==SUMA || t==RESTA){
		Match(t);
		strcpy(op, buffer);
	}
	else
		ErrorSintactico();
}

void Expresion(REG_EXPRESION* reg){
	// <expresion> -> <primaria> { <operadorAditivo> <primaria> #generarInfijo }
	REG_EXPRESION opIzq, opDer;
	char op[TAMLEX];
	TOKEN t;

	Primaria(&opIzq);
	for (t=ProximoToken(); t==SUMA||t==RESTA; t=ProximoToken()){
		OperadorAditivo(op);
		Primaria(&opDer);
		opIzq = generarInfijo(opIzq, op, opDer); // Evalúa la expresión
	}
	*reg = opIzq;
}
void ListaExpresiones(){
	// <listaExpresiones> -> <expresion> #escribirExp { COMA <expresion> #escribirExp }
	TOKEN t;
	REG_EXPRESION reg;
	Expresion(&reg);
	Escribir(reg);
	for (t=ProximoToken(); t==COMA; t=ProximoToken()){
		Match(COMA);
		Expresion(&reg);
		Escribir(reg);
	}
}
void Sentencia(){
	TOKEN tok = ProximoToken();
	REG_EXPRESION izq, der;
	// tok puede ser LEER, ESCRIBIR, ID
	switch (tok){
		// ID := <expresion> ; #Asignar
		case ID:
			Id(&izq);
			Match(ASIGNACION);
			Expresion(&der);
			Asignar( izq, der ); // a la TS (rutina semántica)
			Match(PC);
			break;
		// LEER ( <listaDeIDs> ) ;
		case LEER:
			Match(LEER);
			Match(PARENL);
			ListaIDs();
			Match(PARENR);
			Match(PC);
			break;
		// ESCRIBIR ( <listaExpresiones> ) ;
		case ESCRIBIR:
			Match(ESCRIBIR);
			Match(PARENL);
			ListaExpresiones();
			Match(PARENR);
			Match(PC);
			break;
		default: return;
	}
}

void ListaSentencias(){
	// <listaSentencias> -> <sentencia> {<sentencia>}
	Sentencia(); // primera sentencia
	// Lista de sentencias adicionales
	while(14){
		switch (ProximoToken()){
			case ID:
			case LEER:
			case ESCRIBIR:
				Sentencia();
			break;
			default: return;
		}
	}
}

