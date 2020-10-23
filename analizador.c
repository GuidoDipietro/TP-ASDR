int tabla[NUMESTADOS][NUMCOLS] =
 // L D + - ( ) , ; : = EOF ´ ´ OTRO
{
	{1,3,5,6,7,8,9,10,11,14,13,0,14},			// 0
	{1,1,2,2,2,2,2,2,2,2,2,2,2},				// 1
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 2 ID
	{4,3,4,4,4,4,4,4,4,4,4,4,4},				// 3
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 4 CTE
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 5 +
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 6 -
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 7 (
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 8 )
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 9 ,
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 10 ;
	{14,14,14,14,14,14,14,14,14,12,14,14,14},	// 11
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 12 ASIG
	{14,14,14,14,14,14,14,14,14,14,14,14,14},	// 13 FDT
	{14,14,14,14,14,14,14,14,14,14,14,14,14}	// 14 err
};

/**********Procedimientos de Analisis Sintactico (PAS) *****************/
/**********Procedimientos de Analisis Sintactico (PAS) *****************/
void Objetivo(void){
	/* <objetivo> -> <programa> FDT #terminar */
	Programa();
	Match(FDT);
	Terminar();
}

void Programa(void){
	/* <programa> -> #comenzar INICIO <listaSentencias> FIN */
	Comenzar();//de inicio semántico en caso de corresponder
	Match(INICIO);
	ListaSentencias();
	Match(FIN);
}

void ListaSentencias(void){
	/* <listaSentencias> -> <sentencia> {<sentencia>} */
	Sentencia();
	while ( 1 ) {// se repite hasta que retorna al no encontrar sentencia {<sentencia>}
	switch ( ProximoToken() ) {
		case ID : case LEER : case ESCRIBIR :
		Sentencia();
		break;
		default : return; //si no es sentencia termina la funcion
		}// fin del switch
	}// fin del while
}// fin funcion



/***************************Programa Principal************************/
int main(int argc, char * argv[]){
	TOKEN tok; char nomArchi[TAMNOM]; int l;

	if ( argc == 1 ) { printf("......\n"); return -1; } // no puso nombre de archivo fuente
	if ( argc != 2 ) { printf("......"); return -1; } //los argumentos deben ser 2

	strcpy(nomArchi, argv[1]); l = strlen(nomArchi);
	if ( l > TAMNOM ) {
		printf("Nombre incorrecto del Archivo Fuente\n");
		return -1;
	}

	if ( nomArchi[l-1] != 'm' || nomArchi[l-2] != '.' ) {
		printf("Nombre incorrecto del Archivo Fuente\n");
		return -1;
	}

	if ( (in = fopen(nomArchi, "r") ) == NULL ) {
		printf("No se pudo abrir archivo fuente\n"); return -1; //no pudo abrir archivo
	}
	/*************************Inicio Compilacion***************************/
	Objetivo();
	/**************************Se cierra el Archivo Fuente******************/
	fclose(in);
	return 0;
}
