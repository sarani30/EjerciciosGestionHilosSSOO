//
// Created by saran on 08/11/2026.
//

//EJERCICIO 7

/*7. Create a multithreaded program to perform the column vector–row vector
product. The sizes of both vectors and the resulting matrix will be fixed
–they have to be coherent–, but the number of threads should be specified
as an argument using the command line. This number of threads should
be lower or equal to the dimension of the column vector. Each thread will
receive the index of the for loop in which it was created and it should
generate several consecutive rows of the output matrix. The main thread
will wait for the completion of the work and print the result of the column
vector-row vector product.

Exercise 13 in the slides of unit 2 could be used as a reference to compute the number
of rows to be processed by each thread and to pass the corresponding argument to the
threads

TRADUCCIÓN DEL ENUNCIADO:

7. Crea un programa multihilo para realizar el producto de un vector columna por un vector
fila. Los tamaños de ambos vectores y de la matriz resultante serán fijos —deben ser coherentes—,
pero el número de hilos se debe especificar como argumento a través de la línea de comandos.
Este número de hilos debe ser menor o igual que la dimensión del vector columna. Cada hilo recibirá el índice
del bucle for en el que fue creado y deberá generar varias filas consecutivas de la matriz resultante.
El hilo principal esperará a que finalice el trabajo e imprimirá el resultado del producto del vector columna
por el vector fila.

Se puede utilizar como referencia el Ejercicio 13 de las diapositivas del Tema 2 para calcular el número de filas que
procesará cada hilo y pasar el argumento correspondiente a los hilos.*/

//NOTAS

//producto vector fila*columna
//tamaño fijo de los vectores
//num de hilos como argumento en línea de comandos
//num de hilos menor o igual que la dimensión del vector columna
//cada hilo recibe índice del bucle for que fue creado
//generar x filas consecutivas
//hilo principal espera a que finalice el trabajo e impre el resultado

//1. IMPORTAR LIBRERÍAS NECESARIAS

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//2. DEFINIR TAMAÑO VECTOR FILA Y COLUMNA

#define MAX_FILA 3
#define MAX_COLUMNA 3

//3. ESTRUCTURA DE DATOS PARA PASAR COMO PARÁMETRO
typedef struct {
    int id;
    int ini;
    int fin;
} parametros;

// 4. ESTRUCTURAS DE DATOS GLOBALES
int V_col[MAX_COLUMNA];    // Vector columna (Mx1)
int V_fila[MAX_FILA];   // Vector fila (1xN)
int R[MAX_COLUMNA][MAX_FILA];     // Matriz resultante (MxN)

void *multi(void *arg) {

    int i, j;

    parametros *argumentos = (parametros *) arg;

    //hacer la operación de la multiplicación
    for (i = argumentos->ini; i <= argumentos->fin; i++) {

        for (j = 0; j < MAX_FILA; j++) {
            R[i][j] = V_col[i] * V_fila[j];
        }
    }


    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {


    //Comprobar el número de argumentos
    if (argc < 2) {
        perror("Número insuficiente de argumentos");
        exit(-1);
    }

    //Comprobar el número de hilos especificado en como argumento
    int nhilos;
    nhilos = atoi(argv[1]);

    int i, vi[nhilos];
    pthread_t hilos[nhilos];
    parametros param[nhilos];

    if (nhilos > MAX_FILA || nhilos < 1) {
        perror("Debe de ser igual al tamaño de la fila o menor");
        exit(-1);
    }

    //Inicializar los vectores
    srandom(time(NULL));
    printf("--- VECTOR COLUMNA (%dx1) ---\n", MAX_COLUMNA);
    for (int i = 0; i < MAX_COLUMNA; i++) {
        V_col[i] = random() % 10 + 1;
        printf("[%d]\n", V_col[i]);
    }

    printf("\n--- VECTOR FILA (1x%d) ---\n", MAX_FILA);
    for (int j = 0; j < MAX_FILA; j++) {
        V_fila[j] = random() % 10 + 1;
        printf("%d ", V_fila[j]);
    }
    printf("\n\n");

    //Reparto de trabajo por hilos
    int dim_subvector = MAX_FILA / nhilos;

    //creación de hilos
    for (i = 0; i < nhilos; i++) {
        param[i].id = i;
        param[i].ini = i * dim_subvector;

        if (i == nhilos - 1) param[i].fin = MAX_FILA - 1;
        else param[i].fin = (param[i].ini) + dim_subvector - 1;

        pthread_create(&hilos[i], NULL, &multi, (void *) &param[i]);
    }

    //ejecución de los hilos
    for (i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], NULL);

    }

    // Impresión de la matriz resultante
    printf("--- MATRIZ RESULTANTE R (%dx%d) ---\n", MAX_FILA, MAX_COLUMNA);
    for (int i = 0; i < MAX_FILA; i++) {
        for (int j = 0; j < MAX_COLUMNA; j++) {
            printf("%4d ", R[i][j]);
        }
        printf("\n");
    }

    pthread_exit(0);
}

//EJERCICIO 8

/*8. Modify the previous code so that each thread receives as argument a structure containing
 *the starting row index of the matrix that it will calculate
and the number of rows to be generated. Each thread has to return the
number of multiplications performed during the vector–vector product.
The main thread will display in the console the number of multiplications
returned by each worker thread before this main thread prints the result
of the column vector-row vector product.
Exercise 14 in the slides of unit 2 could be used as a reference to compute the number
of rows to be processed by each thread, to pass the arguments to the threads, and to
collect the data returned by the threads.


TRADUCCIÓN DEL ENUNCIADO:
8. Modifica el código anterior de forma que cada hilo reciba como argumento una estructura que contenga el índice de la
fila inicial de la matriz que calculará y el número de filas a generar. Cada hilo debe devolver el número de multiplicaciones
realizadas durante el producto vector-vector. El hilo principal mostrará por consola el número de multiplicaciones devuelto
por cada hilo trabajador (worker thread) antes de imprimir el resultado del producto del vector columna por el vector fila.

Se puede utilizar como referencia el Ejercicio 14 de las diapositivas del Tema 2 para calcular el número de filas que
procesará cada hilo, pasar los argumentos a los hilos y recoger los datos devueltos por estos.*/

//1. IMPORTAR LIBRERÍAS NECESARIAS

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//2. DEFINIR TAMAÑO VECTOR FILA Y COLUMNA

#define MAX_FILA 3
#define MAX_COLUMNA 3

//3. ESTRUCTURA DE DATOS PARA PASAR COMO PARÁMETRO
typedef struct {
    int id;
    int ini;
    int num_filas;
} parametros;

// 4. ESTRUCTURAS DE DATOS GLOBALES
int V_col[MAX_COLUMNA];    // Vector columna (Mx1)
int V_fila[MAX_FILA];   // Vector fila (1xN)
int R[MAX_COLUMNA][MAX_FILA];     // Matriz resultante (MxN)

void *multi(void *arg) {

    int i, j;

    parametros *argumentos = (parametros *) arg;

    int *res = malloc(sizeof(int));
    *res = argumentos->num_filas * MAX_COLUMNA; // cada fila requiere MAX_COLUMNA multiplicaciones

    //hacer la operación de la multiplicación
    // Recorre desde 'ini' avanzando exactamente 'num_filas' iteraciones
    for (i = argumentos->ini; i < (argumentos->ini + argumentos->num_filas); i++) {
        for (j = 0; j < MAX_COLUMNA; j++) {
            R[i][j] = V_col[i] * V_fila[j];
        }
    }


    pthread_exit((void *) res);
}

int main(int argc, char *argv[]) {

    //variable retorno
    int *retorno;

    //Comprobar el número de argumentos
    if (argc < 2) {
        perror("Número insuficiente de argumentos");
        exit(-1);
    }

    //Comprobar el número de hilos especificado en como argumento
    int nhilos;
    nhilos = atoi(argv[1]);

    int i, vi[nhilos];
    pthread_t hilos[nhilos];
    parametros param[nhilos];

    if (nhilos > MAX_FILA || nhilos < 1) {
        perror("Debe de ser igual al tamaño de la fila o menor");
        exit(-1);
    }

    //Inicializar los vectores
    srandom(time(NULL));
    printf("--- VECTOR COLUMNA (%dx1) ---\n", MAX_COLUMNA);
    for (int i = 0; i < MAX_COLUMNA; i++) {
        V_col[i] = random() % 10 + 1;
        printf("[%d]\n", V_col[i]);
    }

    printf("\n--- VECTOR FILA (1x%d) ---\n", MAX_FILA);
    for (int j = 0; j < MAX_FILA; j++) {
        V_fila[j] = random() % 10 + 1;
        printf("%d ", V_fila[j]);
    }
    printf("\n\n");

    //Reparto de trabajo por hilos
    int dim_subvector = MAX_FILA / nhilos;

    //creación de hilos
    for (i = 0; i < nhilos; i++) {
        param[i].id = i;
        param[i].ini = i * dim_subvector;

        // El último hilo se queda con todas las filas que queden hasta el final
        if (i == nhilos - 1) param[i].num_filas = MAX_FILA - param[i].ini;
        else param[i].num_filas = dim_subvector; // Los demás hilos reciben el tamaño base del subvector

        pthread_create(&hilos[i], NULL, &multi, (void *) &param[i]);
    }

    //ejecución de los hilos
    for (i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], (void **) &retorno);
        printf("Cada hilo ha hecho %d multiplicaciones\n", *retorno);
        free(retorno);

    }

    // Impresión de la matriz resultante
    printf("--- MATRIZ RESULTANTE R (%dx%d) ---\n", MAX_FILA, MAX_COLUMNA);
    for (int i = 0; i < MAX_FILA; i++) {
        for (int j = 0; j < MAX_COLUMNA; j++) {
            printf("%4d ", R[i][j]);
        }
        printf("\n");
    }

    pthread_exit(0);
}