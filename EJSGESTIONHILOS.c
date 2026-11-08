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
    int *fact = (int *) malloc(sizeof(int));
    *fact = 1;

    //calcular el factorial de ese número
    for (int i = 1; i <= numero; i++) {
        *fact *= i;
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
        pthread_join(hilos[i], (void **) &resultado);
        printf("Factorial de %d es %d\n", datos[i], *resultado);
    }

    pthread_exit(0);
}

//EJERCICIO 3

/* Implementa un programa que calcule una secuencia de potencias de 2. El programa
creara tantos hilos como se especifique en su ´ unico argumento. Y cada hilo calcu- ´
lara la potencia de 2 de un n ´ umero que se le pasa como argumento. El primer hilo ´
recibira 0 como argumento y calcular ´ a´ 2
0
. El segundo hilo, 2
1
. El tercero, 2
2 y as´ı
sucesivamente. El hilo principal mostrara por pantalla el resultado calculado por ´
cada hilo subordinado.
Puedes tomar como referencia los ejemplos 4 y 6 de las tansparencias.
Un ejemplo de la ejecucion del programa podr ´ ´ıa ser la siguiente:
1

$ pot2 5
2 ˆ 0: 1
2 ˆ 1: 2
2 ˆ 2: 4
2 ˆ 3: 8
2 ˆ 4: 16
$
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h> //AÑADIDA PARA CALCULAR LA POTENCIA

void *pot2(void *arg) {
    //pasar el argumento a entero
    int numero = *(int *) arg;

    // Reservar memoria en el heap para devolver el resultado
    int *resultado = (int *) malloc(sizeof(int));
    //calcular el resultado
    *resultado = pow(2, numero);

    pthread_exit((void *) resultado);

}

int main(int argc, char *argv[]) {
    int i, *resultado;

    //Número de hilos
    int nhilos = atoi(argv[1]);

    //variable hilos
    pthread_t hilos[nhilos];

    //vector de índices de las potencias
    int vi[nhilos];

    //creación de los hilos
    for (i = 0; i < nhilos; i++) {
        vi[i] = i;
        pthread_create(&hilos[i], NULL, &pot2, (void * ) &vi[i]);
    }

    //ejecución de los hilos
    for (i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], (void **) &resultado);
        printf("2 ^ %d = %d\n", vi[i], *resultado);

        // LIBERACIÓN OBLIGATORIA DE LA MEMORIA RESERVADA POR CADA HILO
        free(resultado);
    }

    pthread_exit(0);
}

//EJERCICIO 4

/*Implementa un programa que genere un vector de MAX numeros enteros aleatorios ´
comprendidos entre 0 y 9. A continuacion buscar ´ a secuencialmente en dicho vector ´
el numero que se le pasa como primer argumento. El programa debe crear tantos ´
hilos como se le indique en el segundo argumento. Y cada hilo ha de realizar la
busqueda en un fragmento consecutivo del vector de un tama ´ no aproximadamente ˜
igual para todos los hilos. As´ı, si denotamos por N al numero total de hilos a ´
crear, el primero de ellos buscara en las posiciones ´ 0,1,...,(MAX div N)-1,
el segundo en (MAX div N),(MAX div N)+1,...,2*(MAX div N)-1
y as´ı sucesivamente. Una vez que un hilo encuentre el numero, todos los hilos ´
finalizaran la b ´ usqueda. El hilo principal esperar ´ a a que acaben todos los hilos y ´
mostrara por pantalla el ´ ´ındice de una posicion en la que est ´ e el n ´ umero ´ o -1 si el ´
numero no est ´ a en el vector. ´
Puedes tomar como referencia los ejercicios 13 y 14 de las tansparencias as´ı como
el siguiente programa, que genera un vector de 20 numeros aleatorios:
´
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <time.h>

    #define MAX 20

    int main()
    {   time_t t;
        int i, v[MAX];

        srandom(time(&t)); // Genera semilla para no crear siempre
                            // los mismos n´umeros aleatorios
        for (i=0;i<MAX;i++)
        {   v[i]=random()%10; // Crea un n´umero aleatorio entre 0 y 9
            printf("V[%d]=%d\n",i,v[i]);
        }
        exit(0);
    }

Un ejemplo de la ejecucion del programa podr ´ ´ıa ser la siguiente:

    $ $1 7 5
    VECTOR
    V[0]=3
    V[1]=8
    ...
    V[15]=8
    V[16]=7
    V[17]=8
    V[18]=6
    V[19]=2

    Hilo 0: Buscar´a entre las componenetes 0 y 3
    Hilo 1: Buscar´a entre las componenetes 4 y 7
    Hilo 2: Buscar´a entre las componenetes 8 y 11
    Hilo 3: Buscar´a entre las componenetes 12 y 15
    Hilo 4: Buscar´a entre las componenetes 16 y 19
    ****Hilo 3035712368: Encuentra el n´umero en V[16]
    El n´umero 7 est´a en la posici´on 16 (V[16]=7)
    $
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> //para generar los números aleatorios

#define MAX 20 //numero de componentes del vector

int v[MAX]; //para que todos los hilos puedan mirar el vector

//Datos compartidos por todos los hilos
//parametros por hilo
typedef struct {
    int n;
    int id;
    int ini;
    int fin;
} parametros;

void *buscar_numero(void *arg) {
    int pos = -1; //posicion del número en el vector
    parametros p = *(parametros *) arg;
    free(arg);

    for (int i = p.ini; i <= p.fin; i++) {
        if (v[i] == p.n) {
            pos = i;
            printf("****Hilo %lu: Encuentra el número %d en V[%d]\n", pthread_self(), p.n, pos);
            break;
        }
    }

    pthread_exit((void *) pos);
}

int main(int argc, char *argv[]) {

    int i; //indice y el vector de tamaño 20
    int dimbloque;
    int *res;

    //Comprobación de argumentos
    if (argc < 3) {
        perror("Argumentos insuficientes");
        exit(-1);
    }

    //Número a buscar
    int num = atoi(argv[1]);

    //Número de hilos (Especificado en el segundo argumento)
    int nhilos = atoi(argv[2]);
    pthread_t hilos[nhilos];

    //dimensión de números del vector que cada hilo debe de buscar
    dimbloque= MAX / nhilos;


    //generar el vector
    time_t t;
    srandom(time(&t)); // Genera semilla para no crear siempre
                        // los mismos n´umeros aleatorios
    for (i=0;i<MAX;i++)
    {   v[i]=random()%10; // Crea un n´umero aleatorio entre 0 y 9
        printf("V[%d]=%d\n",i,v[i]); //MUESTRA EL VECTOR
    }

    //crear los hilos
    for (i = 0; i < nhilos; i++) {
        parametros *p = (parametros *) malloc(sizeof(parametros));

        //número a buscar
        p->n = num;

        p->id = i; //pasamos el número de hilo
        p->ini = i * dimbloque; //el inicio

        if (i < (nhilos-1)) p->fin = p->ini + dimbloque - 1; //hilos intermedios deben de ocuparse de cada parte igual
        else p->fin = MAX - 1; //el último hilo debe de ocuparse de todos los que queden

        pthread_create(&hilos[i], NULL, &buscar_numero, (void *) p);

        //mensaje inicial
        printf("Hilo %d: Buscará entre las componentes %d y %d\n", p->id, p->ini, p->fin);
    }

    //ejecución de los hilos
    for (i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], (void **) &res);
        int resultado  = (int) res;
        if (resultado != -1) {
            break;
        }

    }

    exit(0);
}

//EJERCICIO 5

/*5. Sea el siguiente programa:
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>

    #define NUMTHREADS 4

    typedef struct interval{
        int iter;
        int ini;
        int fin;
    } t_interval;

    int suma_parcial[NUMTHREADS];
    int suma = 0;

    void *hilo(void *arg)
    {
        int i;
        int ini1,fin1, itera;
        t_interval *inter;

        inter = ((t_interval *) (arg));
        itera = inter->iter;

        ini1 = inter->ini;
        fin1 = inter->fin;
        suma_parcial[itera] = 0;

        for(i=ini1;i<=fin1;i++){
            suma_parcial[itera] = suma_parcial[itera] + i;
        }
        printf("HIlo i=%d, suma = %d\n",itera,suma_parcial[itera]);
        pthread_exit(0);
    }

    int main()
    {
        pthread_t thread[NUMTHREADS];
        int ini,fin;
        int i,elem,inicio;
        t_interval param[NUMTHREADS];

        printf("Introduce inicio intervalo: ");
        scanf("%d",&ini);

        printf("Introduce fin intervalo: ");
        scanf("%d",&fin);
        elem = ((fin-ini)+1)/NUMTHREADS;

        inicio=ini;

        for (i=0;i<NUMTHREADS;i++)
        {
            param[i].iter = i;
            param[i].ini = inicio;

            if (i==(NUMTHREADS-1)) param[i].fin = fin;

            else param[i].fin = param[i].ini + elem;

            pthread_create(&thread[i], NULL, hilo, (void *) &param[i]);
            printf("hilo 1 iter = %d, ini = %d, fin = %d\n",param[i].iter,
            param[i].ini,param[i].fin);
            inicio = param[i].fin+1;
        }

        for (i=0; i<NUMTHREADS; i++) pthread_join(thread[i],NULL);

        for (i=0; i<NUMTHREADS; i++) suma = suma + suma_parcial[i];
        printf("Suma = %d\n ",suma);
        pthread_exit (0);
    }

A partir de este programa responde a las siguientes preguntas:

a) Explica la tarea que realiza cada uno de los hilos subordinados. ¿Donde se ´
almacena el resultado que calcula cada hilo?

SOLUCIÓN: Cada hilo, a partir de un subintervalo, calcula la suma parcial de los elementos de ese intervalo.
Posterioremente, guarda el resultado en la variable 'suma_parcial'.

b) Cada hilo modifica un elemento del vector suma parcial. ¿Ven estas modificaciones el resto de hilos y la hebra principal?

SOLUCIÓN: El resto de hilos ve las modificaciones sobre 'suma_parcial'. Sin embargo, no usan los mismos datos o posiciones, puesto que cada hilo tiene una posición del vector donde
se almacena el resultado. De esta manera, no se generan condiciones de carrera entre las variables compartidas.

c) ¿Que resultado muestra por pantalla el programa?

SOLUCIÓN: El programa mostrará por pantalla para cada hilo, sus datos más importantes antes de hacer la suma parcial
(número de iteración, índice de inicio y de final). Posteriormente, mostrará para cada hilo la suma parcial calculada.
Por último, cuando estén todas las sumas parciales calculadas, mostrará por pantalla la suma total de todos los elementos del vector.

*/

//EJERCICIO 6

/*6. Any required number of parameters can be sent to a thread upon its creation using
the *arg pointer specified in the pthread create() prototype. Read carefully
the code of the max vector.c example. Notice what parameter is passed to each
thread, its meaning and its use. Modify the max vector program so that each
thread returns the maximum value it computes via the pthread exit function.
Create a new version of your modified program in which each thread receives as
parameters the starting and ending points of the input vector to be processed. To
send many parameters to a thread, group them into a C structure and send a pointer
to this structure.
Could we use in the max vector.c code a global integer variable which was
updated by each thread if the maximum value it has computed is bigger than the
actual value of the global variable? Justify your answer.

$ cat max_vector.c

    #include <pthread.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <time.h>

    #define MAX_NUMTHREADS 20 /* We create MAX_NUMTHREADS at most

    /* #define VSIZE 2000000 // Problem size

    int num_threads, subvector_dim;
    int v [VSIZE];
    int vmax [MAX_NUMTHREADS];
    int index_v [MAX_NUMTHREADS];

    void *f_max(void *arg)
    {
        int i, begin_v, end_v, nthread, max_for_thread;
        nthread = *(int *)arg;
        begin_v = nthread * subvector_dim;
        if ( nthread == (num_threads-1) ) end_v = VSIZE - 1;
        else end_v = begin_v + subvector_dim -1;
        max_for_thread=0;
        for (i=begin_v; i<=end_v; i++)
            if (v[i]>max_for_thread) max_for_thread=v[i];

        printf("Maximum computed by thread %02d (%u): %07d\n", nthread,
        pthread_self(), max_for_thread);
        vmax[nthread] = max_for_thread;

        pthread_exit(0);
    }

    int main(int argc, char * argv[])
    {
        int i, max;
        time_t t;
        pthread_t thread[MAX_NUMTHREADS];
        pthread_attr_t attr;
        pthread_attr_init(&attr); // They are to be joinable
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        srandom(time(&t));

        if (argc==1)
        {   printf("Error: The number of threads is required as an argument\n");
            exit(-1);
        }

        num_threads=atoi(argv[1]);
        for (i=0; i<VSIZE; i++) //We create the vector
            v[i] = random() % VSIZE;

        puts("Data vector created");
        subvector_dim = VSIZE / num_threads;

        for (i = 0; i < num_threads; i++) // We create threads, it can fail
        {   index_v[i]=i;
            if (pthread_create(&thread[i], &attr, f_max, (void *)&index_v[i]))
            {   printf("On iteration %d: ", i);
                perror("error creating thread.");
                exit(-1);
            }
        }

        printf("%d Threads successfully created.\n\n", i);

        for (i = 0; i < num_threads; i++) { // We wait for their completion
            if ( pthread_join(thread[i],NULL ) )
            {   printf("On iteration %d: ", i); // It can fail
                perror("error joining thread.");
                exit(-1);
            }
        }

        printf("\n%d Threads successfully joined.\n\n", i);

        max=0;
        for (i = 0; i < num_threads; i++)
            // We get the maximum element returned by threads
            if ( vmax[i] > max ) max = vmax[i];

        printf("Final maximum: %d\n", max);
        pthread_attr_destroy(&attr); // To release memory
        exit(0);
    }
*/ /* */

    #include <pthread.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <time.h>

    #define MAX_NUMTHREADS 20 // We create MAX_NUMTHREADS at most

    #define VSIZE 2000000 // Problem size

    int num_threads, subvector_dim;
    int v [VSIZE];
    int vmax [MAX_NUMTHREADS];
    int index_v [MAX_NUMTHREADS];

    void *f_max(void *arg)
    {
        int i, begin_v, end_v, nthread, max_for_thread;
        nthread = *(int *)arg;
        begin_v = nthread * subvector_dim;
        if ( nthread == (num_threads-1) ) end_v = VSIZE - 1;
        else end_v = begin_v + subvector_dim -1;
        max_for_thread=0;
        for (i=begin_v; i<=end_v; i++)
            if (v[i]>max_for_thread) max_for_thread=v[i];

        printf("Maximum computed by thread %02d (%u): %07d\n", nthread,
        pthread_self(), max_for_thread);
        vmax[nthread] = max_for_thread;

        pthread_exit(0);
    }

    int main(int argc, char * argv[])
    {
        int i, max;
        time_t t;
        pthread_t thread[MAX_NUMTHREADS];
        pthread_attr_t attr;
        pthread_attr_init(&attr); // They are to be joinable
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        srandom(time(&t));

        if (argc==1)
        {   printf("Error: The number of threads is required as an argument\n");
            exit(-1);
        }

        num_threads=atoi(argv[1]);
        for (i=0; i<VSIZE; i++) //We create the vector
            v[i] = random() % VSIZE;

        puts("Data vector created");
        subvector_dim = VSIZE / num_threads;

        for (i = 0; i < num_threads; i++) // We create threads, it can fail
        {   index_v[i]=i;
            if (pthread_create(&thread[i], &attr, f_max, (void *)&index_v[i]))
            {   printf("On iteration %d: ", i);
                perror("error creating thread.");
                exit(-1);
            }
        }

        printf("%d Threads successfully created.\n\n", i);

        for (i = 0; i < num_threads; i++) { // We wait for their completion
            if ( pthread_join(thread[i],NULL ) )
            {   printf("On iteration %d: ", i); // It can fail
                perror("error joining thread.");
                exit(-1);
            }
        }

        printf("\n%d Threads successfully joined.\n\n", i);

        max=0;
        for (i = 0; i < num_threads; i++)
            // We get the maximum element returned by threads
            if ( vmax[i] > max ) max = vmax[i];

        printf("Final maximum: %d\n", max);
        pthread_attr_destroy(&attr); // To release memory
        exit(0);
    }