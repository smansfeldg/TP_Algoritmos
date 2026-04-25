/**
 * @file main.c
 * @brief Punto de entrada del programa.
 *
 * Capa de Presentación / Control Principal.
 * Controla el flujo principal del menú interactivo, la inicialización del sistema
 * y el bucle principal del juego.
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/entidades.h"
#include "include/archivos.h"
#include "include/juego.h"
#include "include/menu.h"

int main()
{
    int opcion_elegida = 0;
    // TODO: Inicializar entidades y cargar configuraciones iniciales
    
    do {
        limpiar_pantalla();
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(4);
        
        switch(opcion_elegida) {
            case 1:
                printf("Iniciando juego...\n");
                // TODO: Bucle principal del juego
                pausar_consola();
                break;
            case 2:
                printf("Cargando partida...\n");
                pausar_consola();
                break;
            case 3:
                printf("Opciones...\n");
                pausar_consola();
                break;
            case 4:
                printf("Saliendo del sistema...\n");
                break;
        }
    } while(opcion_elegida != 4);
    
    return 0;
}
