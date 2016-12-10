/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

// arreglo de unos y ceros que indica para cada tarea/bandera si esta muerta(0) o viva(1)
unsigned char arreglo_scheduler[8];
// indica cuantas tareas/banderas consecutivas se ejecutaron
unsigned short contador_tareas;
// indica con 1 si se esta ejecutando una tarea y con 0 si se ejecuta una bandera
unsigned char tarea;
// indica el número de tarea que se está ejecutando
unsigned short tarea_actual;
// indica la última tarea ejecutada antes de pasar a ejecutar banderas 
unsigned short ultima_tarea;
// indica la cantidad de tareas/banderas vivas
unsigned short num_tareas_vivas;
// indica si se esta ejecutando la tarea idle (1) o no (0)
unsigned char tarea_idle_flag;
// indica la cantidad de banderas que se tienen que ejecutar en una vuelta 
unsigned char cantidad_banderas;

void sched_inicializar() {
    for (int i = 0; i < CANT_TAREAS; i++)
        arreglo_scheduler[i] = 1;
    tarea_actual = 0;
    tarea = 1;
    contador_tareas = 0;
    ultima_tarea = 0;
    num_tareas_vivas = 8;
    cantidad_banderas = 8;
    tarea_idle_flag = 1;
}

unsigned short sched_proximo_indice() {
    // si estamos corriendo idle entonces estamos por pasar a otra tarea / bandera
    if (tarea_idle_flag == 1)
        tarea_idle_flag = 0;

    // si el reloj interrumpio a la tarea inicial entonces pasamos a la tarea 1 (posicion 1 de la GDT)
    if (tarea_actual == 0) {
        tarea_actual = 1;
        ultima_tarea = 1;
        contador_tareas++;
        return 1;
    }
    
    // si estamos ejecutando tarea
    if (tarea == 1) {
        // si ya ejecutamos 3 tareas cambiamos a banderas (posiciones de la GDT entre 9 y 16)
        if (contador_tareas == 3) {
            tarea = 0;
            contador_tareas = 1;
            tarea_actual = obtener_siguiente_tarea_viva(0);
            cantidad_banderas = num_tareas_vivas;
            return tarea_actual + 8;
        // sino seguimos ejecutando tareas (posiciones de la GDT entre 1 y 8)
        } else {
            tarea_actual = obtener_siguiente_tarea_viva(tarea_actual);
            contador_tareas++;
            ultima_tarea = tarea_actual;
            return tarea_actual;
        }
    // si estamos ejecutando bandera
    } else {
        // si ya ejecutamos todas las banderas vivas cambiamos a tareas (posiciones de la GDT entre 1 y 8)
        if (contador_tareas == cantidad_banderas) {
            tarea = 1;
            contador_tareas = 1;
            tarea_actual = obtener_siguiente_tarea_viva(ultima_tarea);
            ultima_tarea = tarea_actual;
            return tarea_actual;
        // sino seguimos ejecutando banderas (posiciones de la GDT entre 9 y 16)
        } else {
            tarea_actual = obtener_siguiente_tarea_viva(tarea_actual);
            contador_tareas++;
            return tarea_actual + 8;
        }
    }
}

// deuelve el id de la tarea actual
unsigned short dame_tarea_actual() {
    if (tarea_idle_flag == 1)
        return 17;
    return tarea_actual;
}

// deuelve el id de la tarea acutal si es que se ejecuta, o de la ultima ejecutada en caso de estar ejecutando idle
unsigned short dame_tarea_no_idle() {
    return tarea_actual;
}

unsigned short obtener_siguiente_tarea_viva(unsigned int desde) {
    unsigned char encontrado = 0;
    unsigned int actual = desde;
    while (encontrado == 0) {
        if (actual == 8) {
            actual = 1;
        } else {
            actual++;
        }
        encontrado = (arreglo_scheduler[actual-1]);
    }
    return actual;
}

void matar_tarea() {
    arreglo_scheduler[tarea_actual-1] = 0;
    num_tareas_vivas--;
}

void actualizar_flag_idle() {
    tarea_idle_flag = 1;
}

unsigned char tarea_activa(unsigned int tarea) {
    return arreglo_scheduler[tarea-1];
}

// indica si se esta ejecutando una tarea o una bandera en el momento en que es invocada (1 es tarea, 0 es bandera)
unsigned char es_tarea() {
    return tarea;
}

unsigned char actual_es_idle() {
    return tarea_idle_flag;
}