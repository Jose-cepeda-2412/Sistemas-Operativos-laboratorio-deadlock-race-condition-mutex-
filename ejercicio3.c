/************************************************************************************************
*   Autor: José Jesús Cepeda                                                                    *
*   Pontificia Universidad Javeriana                                                            *
*   Materia: Sistemas Operativos                                                                *
*   Fecha: 05 noviembre 2025                                                                    *
*   Tema: Sincronización de hilos con mutex y variables de condicion(ptrhead_cond)              *
*   Descripcion del programa:                                                                   *
*   Este programa implementa hilos recurrentes que incrementan un contador global compartido,   *  
*   utiliza mutex y variables de condiciones para coordinar cuando cada hilo puede ejecutar     *
*   su seccion critica, evitando condiciones de carrera y controlando el fluno de ejecucion.    *
*                                                                                               *   
*************************************************************************************************/

#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <pthread.h>////biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)

/* Declaracion de mutex y variables globales */
//mutex para "proteger" el acceso a la variable count
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
//mutex adicional usado junto con la variable de condicion
pthread_mutex_t contidition_mutex = PTHREAD_MUTEX_INITIALIZER;
//variable de condicion usada para suspender y reanudar hilos segun el valor de count
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;
//variable global compartida entre hilos
int count = 0;
//valor final del contador al terminar el programa
#define COUNT_DONE 15
//primer valor inferior para detener temporalmente un hilo
#define COUNT_HALT1 4
//segundo limite superior para detener temporalmente un hilo
#define COUNT_HALT2 11

/**
 * funcion 1, ejecutada por el primer hilo, incrementa el contador mientras no este dentro del rango [COUNT_HALT1, COUNT_HALT2]
 * si el contador esta entre COUNT_HALT1, COUNT_HALT2, el hilo entra en espera, cuando el otro hilo envia una señal
 * (pthread_cond_signal), se despierta y continua.
 */
void *count1(){
    for(;;){
        //bloqueo de mutex asociado a la condicion antes de verificar el rango
        pthread_mutex_lock(&contidition_mutex);
        //mientras el contador este entre 4 y 11, el hilo se bloquea (espera)
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2)
        {
            pthread_cond_wait (&condition_cond, &contidition_mutex);
        }
        //desbloquea el mutex de condicion (ya no necesita esperar)
        pthread_mutex_unlock(&contidition_mutex);
        //bloquea el mutex principal para modificar "count" de forma segura
        pthread_mutex_lock(&count_mutex);
        //incrementa en 1 el contador
        count++;
        //imprime el valor actual del contador
        printf("Counter value functionCount ==> 01 <==: %d\n", count);
        //desbloquea el mutex principal
        pthread_mutex_unlock(&count_mutex);
        //si el contador llego al valor maximo, el hilo termina
        if(count >= COUNT_DONE)return (NULL);
    }
}

/**
 * funcion 2, ejecutada por el segundo hilo
 * incrementa el contador y despierta el hilo 1 cuando el contador sale del rango [COUNT_HALF1, COUNT_HALF2]
 * si el contador esta fuera del rango 4-11, este hilo envia una señal para despertar el hilo 1 mediante pthread_cond_signal()
 */
void *count2(){

    for(;;){
        //bloquea el mutex asociado a la condicion antes de evaluar el rango
        pthread_mutex_lock(&contidition_mutex);
        //si el contador esta fuera del rango, envia señal para reanudar el hilo 1
        if (count < COUNT_HALT1 || count > COUNT_HALT2)
        {
            pthread_cond_signal (&condition_cond);
        }
        //desbloquea el mutex de condicion
        pthread_mutex_unlock(&contidition_mutex);
        //bloquea el mutex principal para modificar "count"
        pthread_mutex_lock(&count_mutex);
        //incrementa en 1 el contador
        count++;
        //imprime el valor actual del contador
        printf("Counter value functionCount ==> 02 <==: %d\n", count);
        //desbloquea el mutex principal
        pthread_mutex_unlock(&count_mutex);
        //si el contador alcanza el valor maximo, el hilo termina
        if(count >= COUNT_DONE)return (NULL);
    }
}
/**
 * funcion principal
 * crea dos hilos que ejecutan las funciones count1, count2
 * luego, el hilo principal espera a que ambos terminen su ejecucion antes de finalizar
 */
int main(){
    //declaracion de variables para los identificadores de hilo
    pthread_t thread1, thread2;
    //creacion de ambos hilos (ambos comienzan a ejecutarse concurrentemente)
    pthread_create (&thread1, NULL, &count1, NULL);
    pthread_create (&thread2, NULL, &count2, NULL);
    //espera a que ambos hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    //termina la ejecucion del programa correctamente
    return 0;
    
}