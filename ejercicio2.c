/*
*   Autor: José Jesús Cepeda
*   Pontificia Universidad Javeriana
*   Materia: Sistemas Operativos
*   Tema:
*
*/

#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <pthread.h>////biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)

//numero total de hilos a crear
#define NTHREADS 10
//inicializacion de mutex (bloqueo para evitar condiciones de carrera)
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//contador globla compartido por todos los hilos
int counter = 0;
//funcion que ejecutara cada hilo, recibe un numero (id del hilo) y lo imprime junto con
// su id interno, incrementa el contador global de forma segura usando mutex
void *thread_function(void *arg) {
    int i = *(int *)(arg);//convertir el argumento recibido a entero
    printf("Thread number: %d | Thread ID %lu\n", i, (unsigned long)pthread_self());
    //bloquea el mutex
    pthread_mutex_lock(&mutex1);
    //incrementa 1 el contador global
    counter++;
    //desbloquea el mutex
    pthread_mutex_unlock(&mutex1);
    //termina el hilo
    return NULL;
}
//funcion principal
int main() {
    //arreglo con los ids de cada hilo
    pthread_t thread_id[NTHREADS];
    int i, j;
    //arreglo con argumentos para cada hilo
    int args[NTHREADS];             
    //crea los hilos y asigna el numero correspondiente
    for (i = 0; i < NTHREADS; i++) {
        /**
         * Crea el hilo i y guarda su codigo de retorno
         * &thread_id[i]: almacena el id de cada hilo
         * NULL usa atributos por defecto
         * thread_function funcion que ejecutara el hilo
         * &args[i] argumento unico pasado a la funcion
         */
        args[i] = i;                 
        pthread_create(&thread_id[i], NULL, thread_function, &args[i]);
    }

    // Esperar a que todos los hilos terminen su ejecucion 
    for (j = 0; j < NTHREADS; j++) {
        /**
         * espera a que cada hilo j finalice antes de continuar 
         * thread_id[j] id del hilo a esperar
         * NULL ignora el valor esperado de retorno
         */
        pthread_join(thread_id[j], NULL);
    }
    //imprime el valor final del contador (debe ser igual a NTHREADS)
    printf("Final counter value: %d\n", counter);
    //fin del programa
    return 0;
}
