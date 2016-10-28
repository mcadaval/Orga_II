/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "i386.h"  //Para magic breakpoint
 
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

void print_exception_message(unsigned int e) {
    const char* messages[21] = {
        "Exception 0: Divide Error",
        "Exception 1: Debug Exception",
        "Exception 2: Nonmaskable external interrupt (NMI)",
        "Exception 3: Breakpoint (INT 3 instruction)",
        "Exception 4: Overflow (INTO instruction)",
        "Exception 5: BOUND Range Exceeded (BOUND instruction)",
        "Exception 6: Invalid Opcode (Undefined opcode)",
        "Exception 7: Device Not Available (No Math Coprocessor)",
        "Exception 8: Double Fault",
        "Exception 9: Coprocessor Segment Overrun",
        "Exception 10: Invalid TTS",
        "Exception 11: Segment Not Present",
        "Exception 12: Stack Segment Fault",
        "Exception 13: General Protection",
        "Exception 14: Page Fault",
        "Exception 15: Intel Reserved. Do not use",
        "Exception 16: x87 FPU Floating-Point Error (Math Fault)",
        "Exception 17: Alignment Check",
        "Exception 18: Machine Check",
        "Exception 19: SIMD Floating-Point Exception",
        "Exception 20: Virtualization Exception"
    };
    print(messages[e], 0, 0, C_BG_BLUE | C_FG_BLACK);
}

void print_modo_estado() {  
    // pintamos primer fila de la pantalla con fondo negro y nombre de grupo
    for (int j = 0; j < VIDEO_COLS; j++) {
        print(" ", j, 0, C_BG_BLACK | C_FG_WHITE);
    }
    print("Casa Danzante - Frank Gehry", 0, 0, C_BG_BLACK | C_FG_WHITE);

    // pintamos fondo blanco en las siguientes 15 filas (1 a 15)
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

    // pintamos letras en el cuadro de la derecha
    for (int i = 2; i < 15; i++) {
        print(message[i-2], 51, i, C_BG_BLACK | C_FG_WHITE);
    }

    // pintamos las siguientes 8 filas (16 a 23) 
    for (int i = 16; i < 24; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {
            // si es la primer columna fondo negro
            if (j == 0) {
                print(" ", j, i, C_BG_BLACK | C_FG_BLACK);
            // si es la segunda columna fondo blanco y numero entre 1 y 8
            } else if (j == 1) {
                print_int(i - 15, j, i, C_BG_LIGHT_GREY | C_FG_BLACK);
            // si es cualquier otra columna fondo azul
            } else {
                print(" ", j, i, C_BG_CYAN | C_FG_BLACK);
            }
        }
    }

    // pintamos fondo negro en la última linea (24)
    for (int j = 0; j < VIDEO_COLS; j++) {
        print(" ", j, 24, C_BG_BLACK | C_FG_BLACK);
    }
}

void print_modo_mapa() {    
    // pintamos fondo verde en las primeras 3 filas (0 a 2)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {
            print(" ", j, i, C_BG_GREEN | C_FG_WHITE);
        }
    }

    // pintamos fondo verde para las primeras 16 posiciones de la cuarta fila (3). Para el resto fondo azul 
    for (int j = 0; j < VIDEO_COLS; j++) {
        if (j < 16)
            print(" ", j, 3, C_BG_GREEN | C_FG_WHITE);
        else
            print(" ", j, 3, C_BG_CYAN | C_FG_WHITE);
    }

    // pintamos fondo azul para las siguientes 20 filas (4 a 23)
    for (int i = 4; i < 24; i++) {
        for (int j = 0; j < VIDEO_COLS; j++) {
            print(" ", j, i, C_BG_CYAN | C_FG_WHITE);
        }
    }

    // pintamos fondo negro en la última linea (24)
    for (int j = 0; j < VIDEO_COLS; j++) {
        print(" ", j, 24, C_BG_BLACK | C_FG_BLACK);
    }
}


