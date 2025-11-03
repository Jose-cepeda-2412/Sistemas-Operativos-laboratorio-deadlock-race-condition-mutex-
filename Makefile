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


# Lista de programas a compilar
PROGRAMAS = ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8

ejercicio1:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio2:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio3:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio4:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio5:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio6:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio7:
	$(GCC) $(FLAGS) $@.c -o $@

ejercicio8:
	$(GCC) $(FLAGS) $@.c -o $@

clean:
	$(RM) $(PROGRAMAS)