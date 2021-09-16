#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "convertidor.h"

/** Lee el argumento A **/
#define LEER(A) if( strcmp(*(argv+*i), A) ==0)

/** Cantidad m�xima de digitos del n�mero (incluyendo el separador) **/
#define MAXNUM 16

/** Variable global encargada de mostrar los pasos realizados por consola **/
int *mostrarPasos;

/** Declaraci�n de funciones **/
int* guardarParametros(int *argc, char *argv[], char *numero, int *baseOrigen, int *baseDestino);

/** -- Definici�n de funciones -- **/
/**
    Funci�n: guardarParametros
    Descripci�n: Guarda los parametros ingresados por la consola en las variables a utilizar.
    Parametros:
                int *argc: Longitud de los parametros ingresados por consola
                char *argv[]: Parametros ingresados por consola
                char *numero: Variable donde se almacenar� el n�mero.
                int *baseOrigen: Variable donde se almacenar� la base de origen.
                int *baseDestino: Variable donde se almacenar� la base de destino.

    Retorna 0 en caso de que se hayan guardado los parametros correctamente
    Retorna 1 si el usuario pidio ayuda sobre el programa.
    Retorna x (x > 1) en caso de que haya ocurrido un error.
**/
int* guardarParametros(int *argc, char *argv[], char *numero, int *baseOrigen, int *baseDestino){
    int *retorno;
    retorno = (int*) malloc(sizeof(int));
    *retorno = 0;
    /** Valida que los argumentos sean correctos --> Rango de los argumentos [3, 9] **/
    if(*argc < 3 || *argc > 9) {
        printf("Usa: %s -n <numero> [-s <base_origen>] [-d <base_destino>] [-v] [-h]\n", *argv);
        *retorno = 1;
    }
    else {
        int *i;
        i = (int*) malloc(sizeof(int));
        *i = 1; // Iterador de los argumentos recibidos como parametros. Comienzo desde i=1 para no leer el parametro "converter"

        /*
                Si argv[1] != -n entonces no cumple con el formato pedido.
                Aclaraci�n: Despu�s del <number> puede ir cualquier parametro en cualquier orden, pero al principio se tiene que respetar el orden.
        */
        if( strcmp(*(argv+1), "-n") !=0)
            *retorno = 2;

        while(*i < (*argc) && *retorno == 0){

            /** Lee el n�mero **/
            LEER("-n"){
                    *i += 1; // Itera nuevamente para leer el numero ya que seguido al -n siempre debe estar el n�mero. De lo contrario el formato ingresado es inv�lido.
                    if(*(argv+*i) != NULL) {
                        if(strlen(*(argv+*i)) > MAXNUM)
                            *retorno = 3;
                        else strcpy(numero, *(argv+*i));
                    } else *retorno = 4;


            /** Lee la base origen **/
           } else LEER("-s"){
                    *i += 1; // Itera nuevamente para leer el numero ya que seguido al -s siempre debe estar la base origen. De lo contrario el formato ingresado es inv�lido.
                    if(*(argv+*i) != NULL) {
                        if( atoi(*(argv+*i)) >= 2 && atoi(*(argv+*i)) <= 16) {
                            *baseOrigen = atoi(*(argv+*i));
                        } else *retorno = 4;
                    } else *retorno = 4;


            /** Lee la base destino **/
           } else LEER("-d"){
                    *i += 1; // Itera nuevamente para leer el numero ya que seguido al -d siempre debe estar la base destino. De lo contrario el formato ingresado es inv�lido.
                    if(*(argv+*i) != NULL) {
                        if( atoi(*(argv+*i)) >= 2 && atoi(*(argv+*i)) <= 16) {
                            *baseDestino = atoi(*(argv+*i));
                        } else *retorno = 4;
                    } else *retorno = 4;


            /** Activa la ayuda del programa **/
           } else LEER("-h"){
                    *retorno = 1;
                    printf("%s: Convierte un numero de una base r a una base d.\n");
                    printf("Si no se ingresa ninguna base se asume que, r=d=10.\n");
                    printf("El comando -v mostrara paso por paso la conversion del numero.\n");
                    printf("Modo de uso: %s -n <numero> [-s <base_origen>] [-d <base_destino>] [-v] [-h]\n", *argv);
                    /** Completar algo m�s? **/

            /** Muestra por consola los pasos que se van haciendo para convertir al n�mero **/
           } else LEER("-v"){
                    *mostrarPasos = 1;
           } else *retorno = 5; // Ante cualquier otro parametro, indica un error.
            *i += 1;
        }
        free(i);
    }
    return retorno;
}


void main(int argc, char *argv[]){
    int *baseOrigen, *baseDestino;
    char *numero; // Arreglo donde se almacenar� el n�mero. Se usar� la aritmetica de punteros para manejarlo.
    int *retornado; // Retorno de la funci�n guardarParametros
    /** Asignaci�n de memoria **/

    baseOrigen = (int*) malloc(sizeof(int));
    baseDestino = (int*) malloc(sizeof(int));
    mostrarPasos = (int*) malloc(sizeof(int));
    numero = (char*) malloc(MAXNUM * sizeof(char));

    *baseOrigen = 10;
    *baseDestino = 10;
    *mostrarPasos = 0;

    /** Obtengo el valor de lo que retorne la funcion guardarParametros. Si *retornado > 0 entonces indica que hay un error. **/
    retornado = guardarParametros(&argc, argv, numero, baseOrigen, baseDestino);

    /** Existen dos posibilidades, que el usuario pida ayuda o no. En el caso de pedir ayuda (*retornado==1), el n�mero no se convierte **/
    if( *retornado == 0){
            if(*mostrarPasos == 1){ // Carteles a mostrar por consola si el usuario solicit� "-v"
                printf("[main] Numero a convertir: %s\n", numero);
                printf("[main] Base origen: %i | Base destino: %i\n", *baseOrigen, *baseDestino);
            }
            free(retornado);
            retornado = convertir(numero, baseOrigen, baseDestino, mostrarPasos);
            if(*retornado == 0) printf("Numero convertido -> %s (base %i)", numero, *baseDestino);
            else printf("Ocurrio un error y el numero no pudo ser convertido.");
    } else if(*retornado > 1){ // Si retornado == 1 es porque el usuario pidi� ayuda.
        printf("error: %i", *retornado);
    }
    /** Liberaci�n de memoria **/
    free(baseOrigen); free(baseDestino); free(numero); free(mostrarPasos);

    if(*retornado == 0) {
        /** Liberaci�n de memoria **/
        free(retornado);
        /** El programa finaliza correctamente **/
        exit(EXIT_SUCCESS);
    }
    if(*retornado > 1) {
        /** Liberaci�n de memoria **/
        free(retornado);
        /** El programa finaliza con un error **/
        exit(EXIT_FAILURE);
    }
}
