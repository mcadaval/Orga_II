/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "i386.h"  //Para magic breakpoint
#include "sched.h"

#define BANDERA_BUFFER  0x40001000

// si esta variable esta en 0 indica que estamos en modo mapa, 1 indica que estamos en modo estado
unsigned char modo_pantalla;
// en este arreglo guardamos las 8 banderas
ca banderas[CANT_TAREAS][BANDERA_FILS][BANDERA_COLS];
// en este arreglo guardamos el estado de todos los registros al momento de l ultima excepcion
int ultimo_estado[20];
// en este arreglo guardamos el numero de excepcion arrojada para cada tarea (-1 si aun no se arrojo ninguna) 
int excepciones_tareas[CANT_TAREAS]; 
// en esta variable guardamos el numero de la ultima excepcion arrojada (-1 si aun no se arrojo ninguna) 
int ultima_excepcion;
// en esta variable guardamos el numero de la tarea que arrojo la ultima excepcion (-1 si aun no se arrojo ninguna) 
int ultima_excepcion_tarea;
// en este arreglo guardamos el estado del reloj de cada tarea
char relojes_tareas[CANT_TAREAS];
// en este arreglo guardamos el estado del reloj de cada bandera
char relojes_banderas[CANT_TAREAS];
// en este arreglo guardamos para cada tarea las 3 paginas fisicas que esta ocupando
unsigned int memoria_tareas[CANT_TAREAS][3];
// en este arreglo guardamos toda la información necesaria para imprimir el mapa. Para cada tarea i indicamos en la posicion i un 1 que esa pagina esta mapeada para la tarea i. El noveno elemento indica si hay misil  
unsigned paginas_mapa[VIDEO_FILS][VIDEO_COLS][9];

// en este arreglo guardamos los mensajes de excepcion  
const char* excepciones[23] = {
        "Divide Error            ",
        "Debug Exception         ",
        "NMI                     ",
        "Breakpoint              ",
        "Overflow                ",
        "B. Range Exceeded       ",
        "Invalid Opcode          ",
        "No Math Coprocessor     ",
        "Double Fault            ",
        "Coprocessor Seg. Overrun",
        "Invalid TSS             ",
        "Segment Not Present     ",
        "Stack Segment Fault     ",
        "General Protection      ",
        "Page Fault              ",
        "Intel Reserved          ",
        "Math Fault              ",
        "Alignment Check         ",
        "Machine Check           ",
        "SIMD Floating-Point     ",
        "Virtualization Exception",
        "Func. bandera por tarea ",
        "Syscall 0x50 por bandera"
    };


// imprime a partir de la posicion y,x de la pantalla la secuencia text con los colores indicados por attr
void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

// imprime un entero hexa a partir de la posicion y,x de la pantalla con los colores indicados por attr y la longitud indicada por size
void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

// imprime un entero decimal a partir de la posicion y,x de la pantalla con los colores indicados por attr
void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
        int a = n / 10;
        n -= 10 * a;
        print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

// imprime un char a partir de la posicion y,x de la pantalla con los colores indicados por attr
void print_char(char n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = n;
    p[y][x].a = attr;
}

// pinta la pantalla entera de negro
void screen_pintar_pantalla() {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    for (int i = 0; i < VIDEO_FILS; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {
            p[i][j].c = ' ';
            p[i][j].a = C_BG_BLACK | C_FG_WHITE;
        }
    } 
}

void print_modo_estado() {
    modo_pantalla = 1;  
    // pintamos primer fila de la pantalla con fondo negro y nombre de grupo
    for (int j = 0; j < VIDEO_COLS; j++) {
        print(" ", j, 0, C_BG_BLACK | C_FG_WHITE);
    }
    print("Casa Danzante - Frank Gehry", 0, 0, C_BG_BLACK | C_FG_WHITE);

    // pintamos fondo gris en las siguientes 15 filas (1 a 15)
    for (int i = 1; i < 16; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {
            print(" ", j, i, C_BG_LIGHT_GREY | C_FG_WHITE);
        }
    }

    // pintamos fondo del cuadro de la derecha
    for (int i = 1; i < 15; i++) {
        for (int j = 50; j < 78; j++) {
            if (i == 1) {
                print(" ", j, i, C_BG_CYAN | C_FG_BLACK);
            } else {
                print(" ", j, i, C_BG_BLACK | C_FG_WHITE);
            }
        }
    }
    
    // imprimimos nombres de navios sobre cada bandera
    for (int i = 0; i < 8; i++) {
        unsigned int y;
        unsigned int x1;
        unsigned int x2;
        if (i < 4) {
            y = 2;
            x1 = 5+i*12;
            x2 = 11+i*12;
        } else {
            y = 8;
            x1 = 5+(i-4)*12;
            x2 = 11+(i-4)*12;
        }
        print("NAVIO", x1, y, C_BG_LIGHT_GREY | C_FG_BLACK);
        print_int(i+1, x2, y, C_BG_LIGHT_GREY | C_FG_BLACK);
    }

    const char* message[13] = {
        " EAX            CS",
        " EBX            DS",
        " ECX            ES",
        " EDX            FS",
        " ESI            GS",
        " EDI            SS",
        " EBP",
        " ESP            EFLAGS",
        " EIP",
        " CR0",
        " CR2",
        " CR3",
        " CR4"
    };

    // pintamos nombres de registros en el cuadro de la derecha
    for (int i = 2; i < 15; i++) {
        print(message[i-2], 51, i, C_BG_BLACK | C_FG_WHITE);
    }

    // pitamos la palabra "NAV" arriba del cuadro de ultima excepcion
    print("NAV", 74, 1, C_BG_CYAN | C_FG_BLACK);

    // pintamos contenidos de registros en el cuadro de la derecha y excepciones si es que hay
    if (ultima_excepcion != -1) {
        imprimir_registros_y_excepcion();
        imprimir_excepciones_por_tarea();
    }

    // pintamos las siguientes 8 filas (16 a 23) 
    for (int i = 16; i < 24; i++) {
        unsigned char color;
        // elegimos color para cada linea dependiendo de si la tarea esta viva o no
        if (tarea_activa(i-15) == 1)
            color = C_BG_CYAN | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;

        for (int j = 0; j < VIDEO_COLS; j++) {
            switch(j) {
                case 0:
                case VIDEO_COLS - 1:
                    print(" ", j, i, C_BG_BLACK | C_FG_BLACK);
                    break;
                case 1:
                    print_int(i - 15, j, i, C_BG_LIGHT_GREY | C_FG_BLACK);
                    break;
                case 3:
                    print("P1:", j, i, color);
                    j += 2;
                    break;
                case 17:
                    print("P2:", j, i, color);
                    j += 2;
                    break;
                case 31: 
                    print("P3:", j, i, color);
                    j += 2;
                    break;
                default:
                    print(" ", j, i, color);
                    break;
            }
        }
    }

    // imprimimos paginas fisicas de cada tarea
    imprimir_paginas();

    // imprime la ultima excepcion para cada tarea
    imprimir_excepciones_por_tarea();

    // pintamos última linea (24)
    for (int j = 0; j < VIDEO_COLS; j++) {
        print(" ", j, 24, C_BG_BLACK | C_FG_BLACK);
    }

    // imprimimos numeros de los relojes de tareas
    unsigned int num = 1;
    for (int i = 4; i < 26; i++) {
        unsigned char color;
        if (tarea_activa(num) == 1)
            color = C_BG_LIGHT_GREY | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;
        print_int(num, i, 24, color);
        num++;
        i += 2;
    }

    // imprimimos numeros de los relojes de banderas
    num = 1;
    for (int i = 32; i < 54; i++) {
        unsigned char color;
        if (tarea_activa(num) == 1)
            color = C_BG_BROWN | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;
        print_int(num, i, 24, color);
        num++;
        i += 2;
    }

    // imprimimos todos los relojes
    imprimir_relojes();


    // pintamos banderas
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            if (i % 2 == 0)
                print(" ", 2 + j, 3 + i, C_BG_RED | C_FG_BLACK);
            else
                print(" ", 2 + j, 3 + i, C_BG_BLUE | C_FG_BLACK);  
        }   
    }

    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            if (j < 5)
                print(" ", 14 + j, 3 + i, C_BG_BLUE | C_FG_BLACK);
            else
                print(" ", 14 + j, 3 + i, C_BG_GREEN | C_FG_BLACK);
        }
    }
    
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            if (j < 2)
                print(" ", 26 + j, 3 + i, C_BG_GREEN | C_FG_BLACK);
            else if (2 <= j && j < 4)
                print(" ", 26 + j, 3 + i, C_BG_BROWN | C_FG_BLACK);
            else if ( 4 <= j && j < 6)
                print(" ", 26 + j, 3 + i, C_BG_RED | C_FG_BLACK);
            else if ( 6 <= j && j < 8)
                print(" ", 26 + j, 3 + i, C_BG_CYAN | C_FG_BLACK);
            else if ( 8 <= j && j < 10)
                print(" ", 26 + j, 3 + i, C_BG_BLUE | C_FG_BLACK);

        }
    }
    
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            if ( i < 2)
                print(" ", 38 + j, 3 + i, C_BG_CYAN | C_FG_BLACK);
            else if (i == 2)
                print(" ", 38 + j, 3 + i, C_BG_BLACK | C_FG_BLACK); 
            else if ( i > 2)
                print(" ", 38 + j, 3 + i, C_BG_BROWN | C_FG_BLACK);   
        }
    }

    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
                print(" ", 2 + j, 9 + i, C_BG_BROWN | C_FG_BLACK);
            if ( i == 0 && j < 3)
                print(" ", 2 + j, 9 + i, C_BG_BLUE | C_FG_BLACK); 
            else if ( i == 1 && j > 0 && j < 5)
                print(" ", 2 + j, 9 + i, C_BG_BLUE | C_FG_BLACK);
            else if ( i == 2 && j > 2 && j < 7)
                print(" ", 2 + j, 9 + i, C_BG_BLUE | C_FG_BLACK);
            else if ( i == 3 && j > 4 && j < 9)
                print(" ", 2 + j, 9 + i, C_BG_BLUE | C_FG_BLACK);
            else if ( i == 4 && j > 6 && j < 10)
                print(" ", 2 + j, 9 + i, C_BG_BLUE | C_FG_BLACK);

        }
    }

    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            print(" ", 14 + j, 9 + i, C_BG_BLUE | C_FG_BLACK);
            if ( i == 0 && (j == 8 || j == 9 || j == 7))
                print(" ", 14 + j, 9 + i, C_BG_BROWN | C_FG_BLACK);
            else if ( i == 1 && ( j == 2 || j == 4))
                print(" ", 14 + j, 9 + i, C_BG_MAGENTA | C_FG_BLACK);
            else if ( i == 1 && ( j == 8 || j == 9 || j == 7))
                print(" ", 14 + j, 9 + i, C_BG_BROWN | C_FG_BLACK);
            else if ( i == 2 &&  j == 3 )
                print(" ", 14 + j, 9 + i, C_BG_MAGENTA | C_FG_BLACK);
             else if ( i == 2 &&  ( j == 8 || j == 9 || j == 7) )
                print(" ", 14 + j, 9 + i, C_BG_BROWN | C_FG_BLACK);
            else if ( i == 3 && ( j == 2 || j == 4))
                print(" ", 14 + j, 9 + i, C_BG_MAGENTA | C_FG_BLACK);
            else if ( i == 4 &&  j == 1)
                print(" ", 14 + j, 9 + i, C_BG_MAGENTA | C_FG_BLACK);

        }
    }

    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            print(" ", 26 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);
            if ( i == 0 && j > 2 && j < 7)
                print(" ", 26 + j, 9 + i, C_BG_RED | C_FG_BLACK);
            else if ( i == 1 && (j == 2 || j == 4 || j == 5 || j == 7)) 
                print(" ", 26 + j, 9 + i, C_BG_RED | C_FG_BLACK);
            else if ( i == 2 && j > 1 && j < 8)  
                print(" ", 26 + j, 9 + i, C_BG_RED | C_FG_BLACK);
            else if ( i == 3 && (j == 3 || j == 6))  
                print(" ", 26 + j, 9 + i, C_BG_RED | C_FG_BLACK);
            else if ( i == 4 && (j == 4 || j == 5))  
                print(" ", 26 + j, 9 + i, C_BG_RED | C_FG_BLACK);
            

        }
    }

    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            print(" ", 38 + j, 9 + i, C_BG_CYAN | C_FG_BLACK);
            if ( i == 0 && (j == 2 || j == 7))
                print(" ", 38 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);
            else if ( i == 1 && ( j == 2 || j == 4 || j == 5 || j == 7))
                print(" ", 38 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);
            else if ( i == 2 && ( j > 1 && j < 8))
                print(" ", 38 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);
            else if ( i == 3 && ( j == 2 || j == 4 || j == 5 || j == 7))
                print(" ", 38 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);
            else if ( i == 4 && (j == 2 || j == 7))
                print(" ", 38 + j, 9 + i, C_BG_BLACK | C_FG_BLACK);

        }
    }

}

void print_modo_mapa() {   
    modo_pantalla = 0; 
    for (int i = 0; i < VIDEO_FILS; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {

            // imprimimos numeros de los relojes de tareas
            if (i == 24) {
                unsigned int num = 1;
                for (int i = 4; i < 26; i++) {
                    unsigned char color;
                    if (tarea_activa(num) == 1)
                        color = C_BG_LIGHT_GREY | C_FG_BLACK;
                    else
                        color = C_BG_RED | C_FG_WHITE;
                    print_int(num, i, 24, color);
                    num++;
                    i += 2;
                }

                // imprimimos numeros de los relojes de banderas
                num = 1;
                for (int i = 32; i < 54; i++) {
                    unsigned char color;
                    if (tarea_activa(num) == 1)
                        color = C_BG_BROWN | C_FG_BLACK;
                    else
                        color = C_BG_RED | C_FG_WHITE;
                    print_int(num, i, 24, color);
                    num++;
                    i += 2;
                }

                // imprimimos todos los relojes
                imprimir_relojes();
                continue;
            }

            // determinamos cual es el estado:
            // Si es 0 hay que imprimir el color de fondo verde
            // Si es 1 hay que imprimir el color de fondo celeste 
            // Si es 2 hay que imprimir rojo con el numero de tarea
            // Si es 3 hay mas de una pagina mapeada y hay que imprimir naranja
            // Si es 4 hay ultimo misil y hay que imprimir amarillo
            unsigned char estado;
            if ((i < 3) || (i == 3 && j < 16))
                estado = 0;
            else
                estado = 1;
            unsigned char tarea = 0;
            unsigned char cantidad_tareas = 0;
            for (int k = 0; k < 9; k++) {
                if (k != 8) {
                    if (paginas_mapa[i][j][k] == 1) {
                        tarea = k+1;
                        cantidad_tareas++;
                        if (cantidad_tareas > 1)
                            estado = 3;
                        else
                            estado = 2;
                    }
                } else {
                    if (paginas_mapa[i][j][k] == 1)
                        estado = 4;
                }
            }

            unsigned char color;
            char * msj; 
            switch(estado) {
                case 0:
                    color = C_BG_GREEN | C_FG_WHITE;
                    msj = " ";
                    print(msj, j, i, color);
                    break;
                case 1:
                    color = C_BG_CYAN | C_FG_WHITE;
                    msj = " ";
                    print(msj, j, i, color);
                    break;
                case 2:
                    color = C_BG_RED | C_FG_WHITE;
                    print_int(tarea, j, i, color);
                    break;
                case 3:
                    color = C_BG_BROWN | C_FG_WHITE;
                    msj = "X";
                    print(msj, j, i, color);
                    break;
                case 4:
                    color = C_BG_MAGENTA | C_FG_WHITE;
                    msj = " ";
                    print(msj, j, i, color);
                    break;
            }
        }  
    } 
}

// inicializa el arreglos banderas con todas las banderas 
void inicializar_banderas() {

    // inicializamos banderas
    unsigned char colores_banderas[CANT_TAREAS] = {C_BG_RED | C_FG_BLACK, C_BG_BLUE | C_FG_BLACK, 
                                                   C_BG_GREEN | C_FG_BLACK, C_BG_CYAN | C_FG_BLACK, 
                                                   C_BG_BROWN | C_FG_BLACK, C_BG_MAGENTA | C_FG_BLACK,
                                                   C_BG_BLACK | C_FG_BLACK, C_BG_RED | C_FG_BLACK};
    for(int n = 0; n < CANT_TAREAS; n++) {                                      
        for (int i = 0; i < BANDERA_FILS; i++) {
            for (int j = 0; j < BANDERA_COLS; j++) {
                ca aux;
                aux.c = ' ';
                aux.a = colores_banderas[n];
                banderas[n][i][j] = aux;
            }
        }
    }
}

// anula un bandera poniendola en rojo 
void anular_bandera(unsigned char n) {
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            ca aux;
            aux.c = ' ';
            aux.a = C_BG_RED | C_FG_BLACK;
            banderas[n][i][j] = aux;
        }
    }
}

void flamear_bandera() {
    unsigned short tarea = dame_tarea_actual();
    ca (*buffer)[10] = (ca(*)[10]) BANDERA_BUFFER;
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            banderas[tarea-1][i][j].c = buffer[i][j].c;
            banderas[tarea-1][i][j].a = buffer[i][j].a;
        }
    }
    if (modo_pantalla == 1)
        imprimir_bandera(tarea);
}

void imprimir_bandera(unsigned char n) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int offset_filas;
    int offset_columnas;
    if (n < 5) 
        offset_filas = 3;
    else
        offset_filas = 9;

    // con esta cuentita calculamos el offset de las columnas dependiendo de n
    int aux = (n-1) % 4;
    offset_columnas = aux * 10 + (aux + 1) * 2;

    // pegamos bandera en pantalla
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            ca aux = banderas[n-1][i][j];
            p[offset_filas+i][offset_columnas+j] = aux;
        }
    } 
}

void inicializar_pantalla() {
    // incializamos estructuras utilizadas para imprimir con valores default
    modo_pantalla = 1;
    inicializar_banderas();
    ultima_excepcion = -1;
    ultima_excepcion_tarea = -1;
    for (int i = 0; i < CANT_TAREAS; i++) {
        excepciones_tareas[i] = -1;
        relojes_banderas[i] = '|';
        relojes_tareas[i] = '|';
    } 
}

void guardar_estado_registros(unsigned int cr0, unsigned int cr2, unsigned int cr3, unsigned int cr4, unsigned int eax, unsigned int ebx, unsigned int ecx, unsigned int edx, unsigned int esi, unsigned int edi, unsigned int ebp, unsigned int ds, unsigned int es, unsigned int fs, unsigned int gs, unsigned int excepcion, unsigned int errCode, unsigned int eip, unsigned int cs, unsigned int eflags, unsigned int esp, unsigned int ss) {
    ultima_excepcion = excepcion;
    excepciones_tareas[dame_tarea_actual()-1] = excepcion;
    ultima_excepcion_tarea = dame_tarea_actual();
    ultimo_estado[0] = eax;
    ultimo_estado[1] = ebx;
    ultimo_estado[2] = ecx;
    ultimo_estado[3] = edx;
    ultimo_estado[4] = esi;
    ultimo_estado[5] = edi;
    ultimo_estado[6] = ebp;
    ultimo_estado[7] = esp;
    ultimo_estado[8] = eip;
    ultimo_estado[9] = cr0;
    ultimo_estado[10] = cr2;
    ultimo_estado[11] = cr3;
    ultimo_estado[12] = cr4;
    ultimo_estado[13] = cs;
    ultimo_estado[14] = ds;
    ultimo_estado[15] = es;
    ultimo_estado[16] = fs;
    ultimo_estado[17] = gs;
    ultimo_estado[18] = ss;
    ultimo_estado[19] = eflags;
    
    if (modo_pantalla == 1)
        imprimir_registros_y_excepcion();
}

void imprimir_registros_y_excepcion() {
        
    unsigned short tarea = dame_tarea_actual();

    // si estamos en cualquier excepcion exepto la 21 o 22
    if (excepciones_tareas[dame_tarea_actual()-1] < 21) {
        // imprimimos primera columna de registros
        for (int i = 0; i < 13; i++) {
            print_hex(ultimo_estado[i], 8, 56, 2+i, C_BG_BLACK| C_FG_WHITE);
        }

        // imprimimos segunda columna de registros
        for (int i = 13; i < 18; i++) {
            print_hex(ultimo_estado[i], 8, 70, 2+(i-13), C_BG_BLACK| C_FG_WHITE);
        }

        // imprimimos eflags
        print_hex(ultimo_estado[19], 8, 69, 10, C_BG_BLACK| C_FG_WHITE);        
    }

    // pintamos la linea correspondiente a la tarea de rojo
    unsigned char color = C_BG_RED | C_FG_WHITE;  
    for (int j = 0; j < VIDEO_COLS; j++) {
        switch(j) {
            case 0:
            case VIDEO_COLS - 1:
                print(" ", j, 15+tarea, C_BG_BLACK | C_FG_WHITE);
                break;
            case 1:
                print_int(tarea, j, 15+tarea, color);
                break;
            case 3:
                print("P1:", j, 15+tarea, color);
                j += 2;
                break;
            case 17:
                print("P2:", j, 15+tarea, color);
                j += 2;
                break;
            case 31: 
                print("P3:", j, 15+tarea, color);
                j += 2;
                break;
            default:
                print(" ", j, 15+tarea, color);
                break;
        }
    }

    // imprimimos mensaje de excepcion en el cuadro y numero de navio que arrojo la excepcion
    if (excepciones_tareas[dame_tarea_actual()-1] < 21) {
        print(excepciones[ultima_excepcion], 50, 1, C_BG_CYAN | C_FG_BLACK);
        print_int(ultima_excepcion_tarea, 77, 1, C_BG_CYAN | C_FG_BLACK);
    }
    
    // imprimimos mensaje de excepcion en la fila correspondiente a la tarea
    print(excepciones[excepciones_tareas[dame_tarea_actual()-1]], 51, 15+tarea, C_BG_RED | C_FG_WHITE);

    // imprimimos paginas de memoria porque las pisamos antes
    imprimir_paginas();
}

void excepcion_bandera() {
    excepciones_tareas[dame_tarea_actual()-1] = 21;
    if (modo_pantalla == 1)
        imprimir_registros_y_excepcion();
}

void excepcion_tarea() {
    excepciones_tareas[dame_tarea_actual()-1] = 22;
    if (modo_pantalla == 1)
        imprimir_registros_y_excepcion();
}

void imprimir_excepciones_por_tarea() {
    for (int i = 0; i < CANT_TAREAS; i++) {
        if (excepciones_tareas[i] != -1) {
            print(excepciones[excepciones_tareas[i]], 51, 16+i, C_BG_RED | C_FG_WHITE);
        }
    }
}

void registar_memoria_tarea(unsigned short tarea, unsigned char numero_pag, unsigned int fisica) {
    memoria_tareas[tarea-1][numero_pag-1] = fisica;
    fisica = fisica / 0x1000;
    int y = fisica / VIDEO_COLS;
    int x = fisica % VIDEO_COLS;
    paginas_mapa[y][x][tarea-1] = 1;
}

void desregistrar_memoria_tarea(unsigned short tarea, unsigned char numero_pag) {
    // print_int(tarea, 1, 1, C_BG_CYAN | C_FG_BLACK);
    // print_int(numero_pag, 10, 1, C_BG_CYAN | C_FG_BLACK);
    unsigned int fisica  = memoria_tareas[tarea-1][numero_pag-1] / 0x1000;
    int y = fisica / VIDEO_COLS;
    int x = fisica % VIDEO_COLS;
    paginas_mapa[y][x][tarea-1] = 0;   
}


// imprime todos los relojes
void imprimir_relojes() {
    // imprimimos relojes tareas
    unsigned int x = 5;
    unsigned int y = 24;
    for (int i = 0; i < CANT_TAREAS; i++) {
        unsigned char color;
        if (tarea_activa(i+1) == 1)
            color = C_BG_LIGHT_GREY | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;
        print_char(relojes_tareas[i], x, y, color);
        x += 3;
    }

    // imprimimos relojes banderas
    x = 33;
    for (int i = 0; i < CANT_TAREAS; i++) {
        unsigned char color;
        if (tarea_activa(i+1) == 1)
            color = C_BG_BROWN | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;
        print_char(relojes_banderas[i], x, y, color);
        x += 3;
    }
}

// actualiza el reloj de la tarea/bandera que se esta ejecutando, y lo imprime
void actualizar_reloj_actual() {
    // nos fijamos si estamos ejecutando una tarea o una bandera
    unsigned int offset;
    char reloj; 
    unsigned short tarea = dame_tarea_no_idle();

    // si es idle o la tarea murio salimos, no hay nada que hacer
    if (tarea == 0 || tarea_activa(tarea) == 0)
        return;
    
    unsigned char color;
    if (es_tarea() == 1) {
        offset = 5;
        relojes_tareas[tarea-1] = siguiente_reloj(relojes_tareas[tarea-1]);
        reloj = relojes_tareas[tarea-1];
        color = C_BG_LIGHT_GREY | C_FG_BLACK;
    } else {
        // breakpoint();
        offset = 33;
        relojes_banderas[tarea-1] = siguiente_reloj(relojes_banderas[tarea-1]);
        reloj = relojes_banderas[tarea-1];
        color = C_BG_BROWN | C_FG_BLACK;
    }
    print_char(reloj, offset+(tarea-1)*3, 24, color);
}

// dada la posicion de un reloj devuelve la siguiente posicion
char siguiente_reloj(char reloj) {
    if (reloj == '|') return '/';
    if (reloj == '/') return '-';
    if (reloj == '-') return '\\';
    return '|';
}

void matar_en_screen() {
    unsigned short tarea = dame_tarea_actual();
    relojes_tareas[tarea-1] = ' ';
    relojes_banderas[tarea-1] = ' ';

    // matamos la bandera de la tarea
    matar_bandera(tarea);
    
    // imprimimos numeros del reloj de la tarea y la bandera a matar
    unsigned char color = C_BG_RED | C_FG_WHITE;
    print_int((unsigned int) tarea, 4+(tarea-1)*3, 24, color);
    print_int((unsigned int) tarea, 32+(tarea-1)*3, 24, color);

    // imprimimos relojes
    print_char(relojes_tareas[tarea-1], 5+(tarea-1)*3, 24, color);
    print_char(relojes_banderas[tarea-1], 33+(tarea-1)*3, 24, color);
   
}

void matar_bandera(unsigned short tarea){
    if (modo_pantalla == 0) return;  
    for (int i = 0; i < BANDERA_FILS; i++) {
        for (int j = 0; j < BANDERA_COLS; j++) {
            banderas[tarea-1][i][j].a = C_BG_BLACK | C_FG_BLACK;
            banderas[tarea-1][i][j].c = ' ';
            if (i == 0 && (j == 0 || j == 1 || j == 2 || j == 4 || j == 6 || j == 7 || j == 8))
                banderas[tarea-1][i][j].a = C_BG_RED | C_FG_BLACK;
            else if ( i == 1 && ( j == 0 || j == 2 || j == 4 || j == 6 || j == 8))
                banderas[tarea-1][i][j].a = C_BG_RED | C_FG_BLACK;
            else if ( i == 2 && ( j == 0 || j == 1 || j == 4 || j == 6 || j == 7 || j == 8))
                banderas[tarea-1][i][j].a = C_BG_RED | C_FG_BLACK;
            else if ( i == 3 && ( j == 0 || j == 2 || j == 4 || j == 6))
                banderas[tarea-1][i][j].a = C_BG_RED | C_FG_BLACK;
            else if ( i == 4 && ( j == 0 || j == 2 || j == 4 || j == 6 ))
                banderas[tarea-1][i][j].a = C_BG_RED | C_FG_BLACK;
        }
    }
    imprimir_bandera(tarea);
}

void imprimir_paginas() {
    if (modo_pantalla == 0) return;
    for (int i = 1; i <= CANT_TAREAS; i++) {
        unsigned char color;
        if (tarea_activa(i) == 1)
            color = C_BG_CYAN | C_FG_BLACK;
        else
            color = C_BG_RED | C_FG_WHITE;
            print("0x", 6, 15+i, color);
            print_hex(memoria_tareas[i-1][0], 8, 8, 15+i, color);
            print("0x", 20, 15+i, color);
            print_hex(memoria_tareas[i-1][1], 8, 22, 15+i, color);
            print("0x", 34, 15+i, color);
            print_hex(memoria_tareas[i-1][2], 8, 36, 15+i, color);
    }
}   