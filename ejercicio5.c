/***********************************************************************************
*   Autor: José Jesús Cepeda                                                       *
*   Pontificia Universidad Javeriana                                               *
*   Fecha: 05 noviembre 2025                                                       *
*   Materia: Sistemas Operativos                                                   *
*   Docente: J. Corredor PhD                                                       *
*   Tema:posix para la creacion de hilos concurrentes                              *
*   Descripcion: "Suponer que x*x es altamente costoso en calculo                  *
*                 computacional, se desea calcular la suma de los cuadrados        *   
*                 de una cantidad de numeros, se aplica paralelizar el calculo     *
*                 de cada cuadrado usando hilos"                                   *
************************************************************************************/

#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <stdint.h>//biblioteca para tipos de enteros (intptr_t)
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)

//variable compartida entre todos los hilos
//almacena la suma total de los cuadrados
int acumulado = 0;
/**
 * funcion que ejecutara cada hilo
 * calcula la suma de los cuadradosde un numero y lo suma a la variable global acumulada
 * recibe x que es un puntero que contiene un numero entero convertido a puntero
 * se calcula el cuadrado del valor, se suma el acumulador global acumulador,
 * Esta funcion tien eun problema: genera condicion de carrera ya que varios hilos pueden 
 * modificar simultaneamente la variable global "acumulado", sin ningun tipo de sincronizacion
 * (no se usa mutex)
 */
void *cuadrado(void *x){
    //convierte el puntero recibido en un entero 
    int xi = (intptr_t)x;
    //calcula el cuadrado y lo suma en el acumulador global que es compartido por todos los hilos
    acumulado += xi*xi;
    //finaliza el hilo sin retornar un valor
    return NULL;
}

/**
 * Funcion principal
 * crea 20 hilos concurrentes, cada hilo ejecuta la funcion cuadrado de su numero, el hilo principal espera 
 * a que todos los hilos terminen, finalmente imprime el valor acumulado
 * 
 * Como hace falta el uso de sincronizacio por ejemplo: (pthread_mutex_t), esto produce una condicion de carrera, 
 * al modificar acumulado, por eso al ejecutar varias veces varia el resultado:
 * 1 Acumulado= 2861
 * 2 Acumulado= 2866
 * 997 Acumulado= 2870
 * esto demuestra que los hilos no estan accediendo de forma ordenada al recurso compartido
 */
int main(int argc, char* argv[]){
    /*se crean 20 hilos inicialmente*/

    pthread_t hilos [20]; //arreglo para almacenar los identificadores de cada hilo
    //creacion de hilos
    for (int i = 0; i < 20; i++)
    {
        /**
         * crea un nuevo hilo con pthread_create 
         * &hilos[i] = direcciones donde se guarda el id del hilo creado, NULL = atributo por defecto del hilo,
         * cuadrado = funcion que el hilo ejecutara
         * (void*)(intptr_t)(i + 1) = argumento entero convertido a puntero
         */
        pthread_create(&hilos[i], NULL, cuadrado, (void*)(intptr_t)(i + 1));
    }
    /**
     * espera la finalizacion de los hilos
     */
    for (int i = 0; i < 20; i++)
    {
        //puntero que guarda el valor de retorno
        void * resultado;
        //espera a que cada hilo termine 
        pthread_join(hilos[i], &resultado);
    }
    //imprime el resultado final
    printf("Acumulado= %d\n", acumulado);
    //finalizacion correcta del programa 
    return 0;
}
/**
 * El resultado del programa es irregular aproximadamente el 99.7% de la veces
 * debido a una condicion de carrera, varios hilos acceden y modifican la variable global
 * acumulado al mismo tiempo, sin el uso de mutex ni exclusion mutua, causando resultados inconsistentes 
 */