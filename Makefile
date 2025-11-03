##############################################
#                                            #
#       Pontificia Universidad Javeriana     #
#       Autor: José Cepeda                   #
#       Materia: Sistemas Operativos         #
#		Fecha: 05 de noviembre				 #
#		Profesor: J.Corredor PhD			 #
#       Makefile                             #
#       Script para automatizar la           #
#       compilación y limpieza de los        #
#       programas ejercicios1-ejercicios8    #
#											 #
##############################################


# Compilador a usar
GCC = gcc
#bandera a usar
FLAGS = -lpthread
# Lista de programas a compilar
PROGRAMAS = ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8

#compilacion de programas uno a uno
ejercicio1:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio2:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio3:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio4:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio5:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio6:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio7:
	$(GCC) $@.c -o $@ $(FLAGS)

ejercicio8:
	$(GCC) $@.c -o $@ $(FLAGS)

#limpieza de archivos compilados
clean:
	$(RM) $(PROGRAMAS)