//GESTIÓN BÁSICA DE HILOS

//EJERCICIO 1

/* Implementa un programa en C que calcule el factorial de los numeros que se le ´
pasan como argumentos. El programa creara tantos hilos como argumentos se in- ´
troduzcan y cada uno de los hilos recibira uno de esos argumentos y calcular ´ a su ´
factorial. No importa que se mezcle la salida que imprimen los hilos.
Para pasar el argumento al hilo puedes tomar como referencia el ejemplo 4 de
las tansparencias. Pero recuerda que cada hilo ha de recibir como argumento el
numero ´ cuyo factorial debe calcular.
Un ejemplo de la ejecucion del programa podr ´ ´ıa ser la siguiente:


$ factorial 3 6 4 1 0
Factorial de 1: 1
Factorial de 0: 1
Factorial de 4: 24
Factorial de 6: 720
Factorial de 3: 6
$
*/


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *factorial(void *arg) {

    //pasar parámetros a entero
    int numero = *(int *) arg;

    //variable resultado
    int resultado = 1;

    //calcular el factorial
    for (int i = 1; i <= numero; i++) {
        resultado *= i;
    }

    printf("El factorial de %d es %d\n", numero, resultado);

    //Salida del hilo
    pthread_exit(0);

}

int main(int argc, char *argv[]) {

    int i, nhilos;

    //Comprobar el número de parámetros
    if (argc < 2) {
        perror("Argumentos insuficientes");
        exit(-1);
    }

    //Número de hilos que deben de crearse
    nhilos = argc - 1;

    //Definir la variable de tipo hilo
    pthread_t hilos[nhilos];

    //Reservar espacio en memoria para el vector que contendrá los resultados de los factoriales
    int datos[nhilos]; //guardar para cada hilo el número a calcular su factorial


    //crear los hilos
    for (i = 0; i < nhilos; i++) {
        datos[i] = atoi(argv[1+i]);
        pthread_create(&hilos[i], NULL, &factorial, (void *) &datos[i]);
    }

    //por cada hilo, esperar a su ejecución
    for (i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_exit(0);

}

// EJERCICIO 2

/* Modifica el programa anterior para que no se mezclen las impresiones de los diferentes hilos. Una posible solucion consistir ´ ´ıa en que cada hilo devolviese al hilo
principal, mediante la funcion´ pthread exit, el resultado que ha calculado y
que el hilo principal lo imprima a continuacion. ´
Puedes tomar como referencia el ejemplo 6 de las tansparencias.*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *factorial(void *arg) {

    //pasar el argumento a entero mediante cast
    int numero = *(int *) arg;

    //resultado
    int fact = 1;

    //calcular el factorial de ese número
    for (int i = 1; i <= numero; i++) {
        fact *= i;
    }

    //retornar el valor a la función principal
    pthread_exit((void *) fact);

}


int main(int argc, char *argv[]) {

    int i, *resultado; //indice de los bucles y el valor de retorno (el factorial)

    //Comprobación de número de argumentos
    if (argc < 2) {
        perror("Argumentos insuficientes");
        exit(-1);
    }

    //Número de hilos necesarios
    int nhilos = argc - 1;

    //vector de hilos
    pthread_t hilos[nhilos];

    //almacenar los datos
    int datos[nhilos];

    //crear los hilos
    for (i = 0; i < nhilos; i++) {
        datos[i] = atoi(argv[1+i]);
        pthread_create(&hilos[i], NULL, &factorial, (void *) &datos[i]);
    }

    //ejecución de los hilos
    for (i = 0; i< nhilos; i++) {
        pthread_join(hilos[i], (void *) &resultado);
        printf("Factorial de %d es %d\n", datos[i], *resultado);
    }

    pthread_exit(0);
}