#ifndef STRUCTS
#define STRUCTS

#define TAMLEX (32+1)

// Token //
typedef enum {
    INICIO,
    FIN,
    LEER,
    ESCRIBIR,
    ID,
    CONSTANTE,
    PARENL,
    PARENR,
    PC,
    COMA,
    ASIGNACION,
    SUMA,
    RESTA,
    FDT,
    ERRORLEXICO
} TOKEN;

// Registro para las expresiones
typedef struct {
    TOKEN clase;			// tipo del registro
    char nombre[TAMLEX];	// valor que contiene (nombre del ID, valor de constante como cadena, etc)
    int valor;				// valor entero if(clase==CONSTANTE)
} REG_EXPRESION;

typedef struct {
    char id[TAMLEX]; 		// ID
    TOKEN t; 				// Tipo de Token, t < 4 es palabra reservada
} RegTS;

#endif

//////

#ifndef INSTRUCCIONES
#define INSTRUCCIONES

void Leer(REG_EXPRESION reg);
void Escribir(REG_EXPRESION reg);
void Declarar(char* s);
void Asignar(REG_EXPRESION izq, REG_EXPRESION der);
void Generar(char * COP, char * a, char * b, char * c);

#endif

//////

#ifndef RUTINAS_PAS
#define RUTINAS_PAS

void ErrorSintactico();
void Objetivo();
void Comenzar();
void Terminar();
void Programa();
void Id(REG_EXPRESION* reg);
void ListaIds();
void Primaria(REG_EXPRESION* reg);
void OperadorAditivo(char* op);
void Expresion(REG_EXPRESION* reg);
void ListaExpresiones();
void Sentencia();
void ListaSentencias();

#endif

//////

#ifndef TABLA_DE_SIMBOLOS
#define TABLA_DE_SIMBOLOS

void Chequear(char* s);
void Colocar(char* s);
int Buscar(char* s, TOKEN* t);

#endif

//////

#ifndef LEXER
#define LEXER

void ErrorLexico();
void Match(TOKEN t);
TOKEN ProximoToken();
TOKEN scanner();

#endif