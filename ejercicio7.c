/***********************************************************************************
*   Autor: José Jesús Cepeda                                                       *
*   Pontificia Universidad Javeriana                                               *
*   Fecha: 30/10/2025                                                              *
*   Materia: Sistemas Operativos                                                   *
*   Docente: J. Corredor PhD                                                       *
*   Tema:posix para la creacion de hilos concurrentes                              *
*   Descripcion: se tiene 2 funciones para asignar y reportar en                   *
*                funcion de la llamada desde los hilos creados, desde el proceso   *
*                principal (main)                                                  *
*   Este programa tiene condicion de carrera, el valor es leido y escrito por      *
*   hilos distintos y el mutex estan declarados, pero no se usan (estan comentados)*
************************************************************************************/



#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <stdint.h>//biblioteca para tipos de enteros (intptr_t)
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)
#include <stdbool.h>//biblioteca para el uso de bool, true, false

//variables compartidas globales
int valor = 100;//valor que un hilo imprime y sobreescribe
bool notificar = false;//delcarado pero no se usa

//se declaran pero no se usan
pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//funcion que se encarga de imprimir el valor actual
void *reportar(void* nousados){
    
    /*
    pthread_mutex_lock(&mutex);
    while (!notificar)
    {
        pthread_cond_wait(&var_cond, &mutex);
    }
    */
   //lectura sin proteccion, genera condicion de carrera con asignar, si asignar escribe el valor al mismo 
   //tiempo, la lectura puede ver 100 0 20
    printf("El valor es: %d\n", valor);
    /*
    pthread_mutex_unlock(&mutex);
    */
    return NULL;
}


/**
 * Hilo asignar cambia el valor a 20
 */
void *asignar(void* nousados){
    /**
     * escritura sin proteccion, genera condicion de carrera con reportar, no se usa 
     * mutex, el cambio puede intercalar con la impresion
     */
    valor = 20;
    /*
    pthread_mutex_lock(&mutex);
    notificar = true;
    pthread_cond_signal(&var_cond);
    pthread_mutex_unlock(&mutex);
    */
    return NULL;
}

/**
 * funcion principal, crea ambos hilos y espera a que terminen (join)
 */
int main(int argc, char *argv[]){
    pthread_t reporte, asigne;
    //crea un hilo que imprime valor y otro que le asigna a 20
    pthread_create(&reporte, NULL, reportar, NULL);
    pthread_create(&asigne, NULL, asignar, NULL);
    //espera la finalizacion de ambos hilos
    void * nousado;
    pthread_join(reporte, &nousado);
    pthread_join(asigne, &nousado);

    //fin correcto del programa
    return 0;
}