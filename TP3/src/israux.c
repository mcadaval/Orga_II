/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
 funciones auxiliares para rutinas de atencion de interrupciones
*/

 #include "israux.h"

void rutina_teclado(unsigned char scan_code) {
    if (scan_code == 0x32)
        print_modo_mapa();

    if (scan_code == 0x12)
        print_modo_estado();

    switch (scan_code) {
        case 0xB:
            print("0", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x52:
            print("0", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x2:
            print("1", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x4F:
            print("1", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x3:
            print("2", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x50:
            print("2", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x4:
            print("3", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x51:
            print("3", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x5:
            print("4", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x4B:
            print("4", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x6:
            print("5", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x4C:
            print("5", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x7:
            print("6", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x4D:
            print("6", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x8:
            print("7", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x47:
            print("7", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x9:
            print("8", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x48:
            print("8", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0xA:
            print("9", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        case 0x49:
            print("9", 79, 0, C_BG_RED | C_FG_WHITE);
            break;
        default:
            break;
    }
}