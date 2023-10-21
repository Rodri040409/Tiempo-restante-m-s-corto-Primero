#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Proceso {
    char nombre;
    int llegada;
    int duracion_original;
    int duracion;
    int instante;
    int t_fin;
    int t_e; // Tiempo de espera (t.e)
    int t_r; // Tiempo de retorno (t.r)
    double penalizacion;
    int primera_vez;
    int ultima_vez;
};

int compararProcesos(const void *a, const void *b) {
    struct Proceso *procesoA = (struct Proceso *)a;
    struct Proceso *procesoB = (struct Proceso *)b;

    if (procesoA->nombre < procesoB->nombre) {
        return -1;
    } else if (procesoA->nombre > procesoB->nombre) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    printf("+------------------------------------------------------+\n");
    printf("|        El mas corto tiempo restante primero          |\n");
    printf("+------------------------------------------------------+\n");

    int num_procesos;
    printf("Ingrese el numero de procesos: ");
    scanf("%d", &num_procesos);

    struct Proceso procesos[num_procesos];

    for (int i = 0; i < num_procesos; i++) {
        printf("Ingrese la hora de llegada del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].llegada);
        printf("Ingrese la duracion del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].duracion_original);
        procesos[i].duracion = procesos[i].duracion_original;
        procesos[i].nombre = 'A' + i;
        procesos[i].primera_vez = -1;
        procesos[i].ultima_vez = -1;
    }

    int tiempo_actual = 0;
    double penalizacion_total = 0;

    while (1) {
        int proceso_ejecutar = -1;
        int menor_tiempo_duracion = INT_MAX;

        for (int i = 0; i < num_procesos; i++) {
            if (procesos[i].llegada <= tiempo_actual && procesos[i].duracion > 0) {
                if (procesos[i].duracion < menor_tiempo_duracion) {
                    proceso_ejecutar = i;
                    menor_tiempo_duracion = procesos[i].duracion;
                }
            }
        }

        if (proceso_ejecutar == -1) {
            break;
        }

        struct Proceso *p = &procesos[proceso_ejecutar];
        p->instante = tiempo_actual;
        p->t_fin = p->instante + 1;
        p->duracion--;
        
        if (p->primera_vez == -1) {
            p->primera_vez = p->instante;
        }
        
        // Calcula t.e correctamente
        p->t_e = p->primera_vez - p->llegada;

        // Calcula t.r utilizando t.e y duración original
        p->t_r = p->t_e + p->duracion_original;

        if (p->duracion == 0) {
            p->ultima_vez = p->t_fin;
        }
        
        if (p->t_e == 0) {
            p->penalizacion = 0;
        } else {
            p->penalizacion = (double)p->t_r / p->t_e; // Calcula penalización
        }

        penalizacion_total += p->penalizacion;
        tiempo_actual++;
    }

    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");
    printf("| proceso | llegada | cpu | instante | t.fin | t.e  |  t.r  | penalizacion |\n");
    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");
    for (int i = 0; i < num_procesos; i++) {
         printf("|    %c    |   %2d    |%4d |    %2d    |   %2d  |  %2d  |  %2d   |  %.8f  |\n",
               procesos[i].nombre, procesos[i].llegada, procesos[i].duracion_original, procesos[i].primera_vez, procesos[i].ultima_vez, procesos[i].t_e, procesos[i].t_r, procesos[i].penalizacion);
    }
    printf("+---------+-----------+--------+----------------+------+-------+-----------------------+\n");

    double promedio_penalizacion = 0;
    for (int i = 0; i < num_procesos; i++) {
        promedio_penalizacion += procesos[i].penalizacion;
    }
    promedio_penalizacion /= num_procesos;
    printf("Promedio de penalizacion: %.8f\n", promedio_penalizacion);

    return 0;
}

//IG:rodrigoruiz_11
