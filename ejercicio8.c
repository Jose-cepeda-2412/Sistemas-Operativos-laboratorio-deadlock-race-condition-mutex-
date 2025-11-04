/***********************************************************************************
*   Autor: José Jesús Cepeda                                                       *
*   Pontificia Universidad Javeriana                                               *
*   Fecha: 05 noviembre 2025                                                       *
*   Materia: Sistemas Operativos                                                   *
*   Docente: J. Corredor PhD                                                       *
*   Tema:posix para la creacion de hilos concurrentes                              *
*   Descripcion: se tiene 2 funciones para asignar y reportar en                   *
*                funcion de la llamada desde los hilos creados, desde el proceso   *
*                principal (main)                                                  *
*   El hilo asignar modifica el valor de una variable global y envia una señal     *
*   para que el hilo reportar imprima el valor actualizado de forma sincronizada   *
************************************************************************************/

#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <stdint.h>//biblioteca para tipos de enteros (intptr_t)
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)
#include <stdbool.h>//biblioteca para el uso de bool, true, false

//variables globales compartidas entre hilos
//variable compartida cuyo valor sera modificado y leido por distintos hilos
int valor = 100;
//variable booleana para saber cuando se puede imprimir el valor
bool notificar = false;

//variables de sincronizacion globales
//variable de condicion
pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER;
//mutex para exclusion mutua
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * funcion reportar que imprime el valor compartido una vez se ha sido actualizado
 * bloquea el mutex antes de acceder a las variables compartidas, mientras notificar sea falso, espera la señal 
 * en var_cond, cuando asignar emite la señal, imprime el valor actualizado, libera el mutex y termina
 */
void *reportar(void* nousados){
    //bloquea el mutex antes de acceder a las variables compartidas
    pthread_mutex_lock(&mutex);
    //espera mientras la variable notificar sea falsa
    while (!notificar)
    {
        //libera el mutex y espera la señal
        pthread_cond_wait(&var_cond, &mutex);
    }
    //al recibir la señal, imprime el valor actualizado
    printf("El valor es: %d\n", valor);
    //libera el mutex e imprime el valor actualizado
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

/**
 * funcion asignar: cambia el valor y notifica al hilo reportar
 * hilo que cambia el valor de la variable compartida y envia la señal para que el hilo reportar pueda continuar
 * modifica la variable valor, bloquea el mutex antes de modificar notificar, cambia notificar a verdadero y envia 
 * una señal para "despertar" a reportar
 */
void *asignar(void* nousados){
    //modifica la variable compartida
    valor = 20;
    //bloquea el mutex para acceder de forma segura a notificar
    pthread_mutex_lock(&mutex);
    //indica que el valor ya fue asignado 
    notificar = true;
    //envia la señal a reportar
    pthread_cond_signal(&var_cond);
    //libera el mutex
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

/**
 * Funcion principal
 * crea ambos hilos y espera su finalizacion
 * crea el hilo reportar que esperara la señal, crea el hilo asignar que cambia el valor y envia la señal,
 * espera a que ambos hilos terminen con pthread_join()
 */
int main(int argc, char *argv[]){
    pthread_t reporte, asigne;//identificadores de los hilos
    //creacion de los hilos
    
    pthread_create(&reporte, NULL, reportar, NULL);
    pthread_create(&asigne, NULL, asignar, NULL);
    //espera a que ambos hilos terminen
    void * nousado;
    pthread_join(reporte, &nousado);
    pthread_join(asigne, &nousado);
}

