/**
 * @file menu.c
 * @brief Implementación de las abstracciones del menú.
 */

#include <stdio.h>
#include <stdlib.h>
#include "include/menu.h"

void mostrar_menu_principal() {
    printf("========================================\n");
    printf("             MENU PRINCIPAL             \n");
    printf("========================================\n");
    printf("1. Iniciar Juego\n");
    printf("2. Cargar Partida\n");
    printf("3. Opciones\n");
    printf("4. Salir\n");
    printf("========================================\n");
}

int obtener_opcion(int max_opcion) {
    int opcion;
    int escaneados;
    do {
        printf("Ingrese una opcion (1-%d): ", max_opcion);
        escaneados = scanf("%d", &opcion);
        while(getchar() != '\n'); // Limpiar el buffer
        
        if (escaneados != 1 || opcion < 1 || opcion > max_opcion) {
            printf("Opcion invalida. Intente nuevamente.\n");
            opcion = -1;
        }
    } while (opcion == -1);
    
    return opcion;
}

void limpiar_pantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar_consola() {
    printf("Presione Enter para continuar...");
    while(getchar() != '\n');
}