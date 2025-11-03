/*
*   Autor: José Jesús Cepeda
*   Pontificia Universidad Javeriana
*   Materia: Sistemas Operativos
*   Tema:
*
*/
#include <stdio.h>//biblioteca para funciones de entrada y salida
#include <stdlib.h>//
#include <pthread.h>//biblioteca para usar posix (ptrhead_t,pthread_create, pthread_join)

/*
*funcion que ejecutara cada hilo, recibe y devuelve un void*
*/
void *print_message_function(void *ptr) {
    char *message; //puntero a char para almacenar el argumento como string
    message = (char *) ptr; //casteo de void* a char*, el hilo recibe void*
    printf("%s \n", message); //imprime el mensaje propio del hilo (Thread1, Thread2)
    return NULL;//fin del hilo 
}

int main() {
    pthread_t thread1, thread2;//identificadores de los dos hilos
    char *message1 = "Thread 1";//mensaje que se pasara al hilo 1
    char *message2 = "Thread 2";//mensaje que se pasara al hilo 2
    int  iret1, iret2;//codigos de retorno de ptrhead_create (0 = exitoso, !=0 error)

    /* Create independent threads each of which will execute function */
    /**
     * Crea el hilo 1 y guarda su codigo de retorno en iret1
     * &thread1: almacena el id del hilo
     * NULL usa atributos por defecto
     * print_message_function funcion que ejecutara el hilo
     * (void*) message1 argumento pasado a la funcion
     */
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1);
    /**
     * Crea el hilo 2 y guarda su codigo de retorno en iret2
     * &thread2: almacena el id del hilo
     * NULL usa atributos por defecto
     * print_message_function funcion que ejecutara el hilo
     * (void*) message2 argumento pasado a la funcion
     */
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2);

    /**
     * muestra el codigo de retorno de creacion del hilo 1
     */
    printf("Thread 1 returns: %d\n", iret1);
     /**
     * muestra el codigo de retorno de creacion del hilo 2
     */
    printf("Thread 2 returns: %d\n", iret2);

    /**
     * se hace la sincronizacion, se espera a que termine la creacion de los hilos
     * pthread_join(thread1, NULL), espera hasta que thread1 finaliza 
     */
    pthread_join(thread1, NULL);
    //pthread_join(thread2, NULL), espera a que thread2 finaliza
    pthread_join(thread2, NULL);
    //fin del programa
    return 0;
}
