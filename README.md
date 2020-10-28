# TP ASDR

Analizador sintáctico descendente recursivo del lenguaje Micro programado en C. Genera las instrucciones para la MV, no ejecuta realmente el código en Micro.

# Compilar

(Se refiere al código en la carpeta ASDR)  

Para compilar este analizador, ejecutar el comando `gcc asdr.c instrucciones.c lexer.c PAS.c TS.c -o compilador`, o hacer doble click en el archivo `compilar.bat`.  
Para ejecutar un análisis sintáctico en un archivo `archivo.m`, escribir en consola `compilador archivo.m`.

# Ejemplo

Programa:
```c
inicio

a := 5;
b := a+14;

leer (a,b);
escribir( 1+2 );
escribir(a);

fin
```

Salida:
```
DECLARAR a ENTERA
ALMACENAR 5 a
DECLARAR b ENTERA
DECLARAR Temp1 ENTERA
SUMAR a 14 Temp1
ALMACENAR Temp1 b
LEER a ENTERA
LEER b ENTERA
DECLARAR Temp2 ENTERA
SUMAR 1 2 Temp2
ESCRIBIR Temp2 ENTERA
ESCRIBIR a ENTERA
HALT
```
