/***********************************************************************************
*   Autor: José Jesús Cepeda                                                       *
*   Pontificia Universidad Javeriana                                               *
*   Fecha: 30/10/2025                                                              *
*   Materia: Sistemas Operativos                                                   *
*   Docente: J. Corredor PhD                                                       *
*   Tema:posix para la creacion de hilos concurrentes                              *
*   Descripcion: "Suponer que x*x es altamente costoso en calculo                  *
*                 computacional, se desea calcular la suma de los cuadrados        *   
*                 de una cantidad de numeros, se aplica paralelizar el calculo     *
*                 de cada cuadrado usando hilos"                                   *
*   En esta version se corrige el codigo anterior para eliminar la condicion de    *
*   carrera mediante el uso de mutex, que protege la varibale compartida acumulado *
*   durante las operaciones concurrentes                                           *
************************************************************************************/

#include <stdio.h> //biblioteca para funciones de entrada y salida
#include <stdlib.h>
#include <stdint.h>//biblioteca para tipos de enteros (intptr_t)
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)

//variable compartida entre todos los hilos
//almacena la suma total de los cuadrados
int acumulado = 0;

/*bloqueoCC: condicion de carrera*/
/**
 * mutex para evitar la condicion de carrera, garantiza el acceso seguro a la variable aumulado
 */
pthread_mutex_t  bloqueoCC = PTHREAD_MUTEX_INITIALIZER;


/*funcion para calculo de cuadrados*/
/**
 * funcion que ejecutara cada hilo
 * calcula la suma de los cuadradosde un numero y lo suma a la variable global acumulada
 * recibe x que es un puntero que contiene un numero entero convertido a puntero
 * se calcula el cuadrado del valor, se suma el acumulador global acumulador.
 * En esta funcion no existe la condicion de carrera ya que el acceso a la variable global 
 * esta protegida por pthread_mutex_lock y pthread_mutex_unlock.
 */
void *cuadrado(void*x){
    //convierte el puntero recibido en un entero 
    int xi = (intptr_t)x;
    //bloquea el mutex antes de modificar la variable compartida
    pthread_mutex_lock(&bloqueoCC);
    //calcula el cuadrado y lo suma en el acumulador global, solo un hilo puede modificar la variable compartida
    acumulado += xi*xi;
    //libera el mutex, permitiendo que otro hilo acceda al recurso compartido 
    pthread_mutex_unlock(&bloqueoCC);
    //finaliza el hilo sin retornar un valor
    return NULL;
}


/**
 * Funcion principal
 * crea 20 hilos concurrentes, cada hilo ejecuta la funcion cuadrado de su numero, el hilo principal espera 
 * a que todos los hilos terminen, finalmente imprime el valor acumulado.
 * En esta version el resultado es consistente en todas las ejecuciones, gracias al uso de mmutex que elimina
 * la condicion de carrera
 */
int main(int argc, char* argv[]){
    /*se crean 20 hilos inicialmente*/
    //arreglo para almacenar los identificadores de cada hilo
    pthread_t hilos [20];
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
 * Este programa es consistente, el resultado simepre es: 1000 Acumulado= 2870 (100%), debido a que ya 
 * se corrigio el problema de condicion de carrera, un hilo solo puede modificar la variable compartida
 * al mismo tiempo.
 */