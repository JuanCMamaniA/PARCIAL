#include <xinu.h>

#define BUFFER_SIZE 10 //definicion del tamaño del buffer

sid32 mutex;

struct arrayCircular
{                           // estructura de array circular
    int array[BUFFER_SIZE]; // array para almacenar los datos,tipo int
    int inicio;             // inicio del array
    int fin                 // posicion del ultimo dato gurdado
};

struct arrayCircular buffer;

void iniciar_buffer()
{
    buffer.inicio = 0;
    buffer.fin = 0;
    mutex = semcreate(0);//mutex para lectura de buffer
}

int leer_dato()
{
    int n;
    wait(mutex); // tomo el permiso si el buffer tiene almenos 1 elemento

    n = buffer.array[buffer.inicio];                   // tomo el elemento del inicio
    buffer.array[buffer.inicio] = NULL;                // reemplazo el valor tomado por null en la posion inicio
    buffer.inicio = (buffer.inicio + 1) % BUFFER_SIZE; // desplazo 1 lugar el inicio

    return n; // retorno el valor leido
}

void colocar_dato(int n)
{

    while (BUFFER_SIZE-1 < semcount(mutex))//9 < 10 caso de rechazo
    { // si el buffer esta lleno espero
    }

    buffer.array[buffer.fin] = n;                // tomo el elemento dela poscion fin del buffer
    buffer.fin = (buffer.fin + 1) % BUFFER_SIZE; // desplazo 1 lugar el fin

    signal(mutex);
}

void proceso_1()//proceso para simulacion de escritura
{
    printf("INICIO ESCRITURA\n");
    // sleep(10);//tiempo para verificar si se bloquea el Lector
    for (int i = 1; i <= 100; i++)
    {
        //printf("|E:%d|\n", i);//imprime BLOQUEO +1
        colocar_dato(i);
        printf("|E:%d|\n", i);
    }
}

void proceso_2()//proceso para simulacion de lectura
{
    printf("INICIO LECTURA\n");
    for (int i = 0; i < 100; i++)
    {
        if (i %10==0)
        {
          sleep(6);//tiempo de espera para verificar que el escritor se bloquea con 10 escrituras
        }
        printf("|L:%d|\n", leer_dato());
    }
}

void ej2()
{

    int pid_escritor, pid_lector;

    pid_escritor = create(proceso_1, 8192, 20, "ESCRITOR", 0);
    pid_lector = create(proceso_2, 8192, 20, "LECTOR", 0);
    resume(pid_lector);//inicio el proceso LECTOR
    resume(pid_escritor);//inicio el proceso ESCRITOR
    sleep(120); // tiempo de espera para que terminen de ejecutar el ESCRITOR Y LECTOR
    kill(pid_escritor);//ELIMINO AL ESCRITOR
    kill(pid_lector);//ELIMINO AL LECTOR

    exit();
}