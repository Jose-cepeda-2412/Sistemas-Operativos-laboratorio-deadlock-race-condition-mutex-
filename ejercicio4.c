/******************************************************************************************
*   Autor: José Jesús Cepeda                                                              *
*   Pontificia Universidad Javeriana                                                      *
*   Materia: Sistemas Operativos                                                          *
*   Fecha: 05 noviembre 2025                                                              *
*   Docente : J.Corredor PhD                                                              *
*   Tema: Creacion y sincronizacion de hilos usando mutex(pthread)                        *
*   Descripcion:                                                                          *
*   Este programa crea tres hilos concurrentes que ejecutan una misma funcion (compute)   *
*   cada hilo accede a una variable global compartida (counter) de forma segura, usando   *
*   un mutex para evitar condiciones de carrera.                                          *
*   se muestran mensajes indicando el inico y finalizacion de cada hilos.                 *
*                                                                                         *
*******************************************************************************************/

#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)
#include <string.h>//bibloteca para manejo de cadenas

//Declaraciones globales
//Arreglo para almacenar los identificadores [id] de los hilos creados
pthread_t tid[3];
//variable global compartida entre hilos
int counter = 0;
//inicializacion de un mutex global para proteger el acceso a counter
pthread_mutex_t lock =  PTHREAD_MUTEX_INITIALIZER;
//funcion ejecutada por cada hilo
/**
 * cada hilo: bloquea el mutex para acceder a la variable global counter, incrementa el contador global,
 * imprime el mensaje indicando el incio del trabajo, ejecuta un ciclo vacio para simular el procesamiento,
 * imprime el mensake indicando que finalizo el trabajo, desbloquea el mutex al finalizar
 */
void *compute(void *arg){
    //variable usada en el bucle de simulacion de trabajo
    unsigned long i = 0;
    //bloquea el mutex antes de acceder a la variable compartida
    pthread_mutex_lock(&lock);
    //incrementa en 1 el contador global de forma segura
    counter += 1;
    //imprime mensaje indicando que el trbajo del hilo ha comenzado
    printf("\n Job %d has started \n", counter);
    //bucle vacio simulando un trbajo
    for (i = 0; i < (0xFFFFFFFF); i++);
    //imprime mensaje indicando que el trabajo del hilo ha finalizado
    printf("\n Job %d  has finished\n", counter);
    //desbloquea el mutex (permite que otros hilos accedan al recurso compartido)
    pthread_mutex_unlock(&lock);
    //retorna null indicando el fin de la ejecucion del hilo
    return NULL;
}

/**
 * funcion principal:
 * crea tres hilos que ejecutan la funcion compute
 * espera a que todos los hilos terminen usando pthread_join
 */
int main(void){
    //contador de bucles
    int i = 0;
    //variable para almacenar codigos de error devueltos por thread_create
    int error;
    //creacion de hilos
    while (i < 3)
    {
        //crea un nuevo hilo, tid[i] = direccion donde se almacena el id del hilo, NULL = usa los atributos por defecto del hilo
        //&comupte = funcion que el hilo ejecuta, NULL = argumento que se pasa a la funcion
        error = pthread_create(&(tid[i]),NULL, &compute, NULL);
        //si pthread_create devuelve un valor diferente de 0, ocurrio un error
        if (error != 0)
        {
            //muestra un mensaje de error si no se pudo crear el hilo
            printf("\n Thread can't be created: [%s]", strerror(error));
        }
        i++;
    }
    //esperar la finalizacion de los hilos
    i = 0;
    while (i < 3)
    {
        /**
         * espera a que el hilo tid[i], finalice antes de continuar.
         * pthread_join() bloque el hilo principal hasta que el hilo indica que termino
         */
        pthread_join(tid[i], NULL);
        i++;
    }
    //fin del programa
    return 0;
}
