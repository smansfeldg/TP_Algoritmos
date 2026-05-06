#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/entidades.h"
#include "include/archivos.h"
#include "include/juego.h"
#include "include/menu.h"
#include "include/abb.h"

void jugar();

int main()
{
    srand((unsigned)time(NULL));

    int opcion_elegida = 0;

    do {
        limpiar_pantalla();
        mostrar_menu_principal();
        opcion_elegida = obtener_opcion(3); // Actualizar al agregar futuras opciones

        switch(opcion_elegida) {
            case 1:
                printf("Iniciando juego...\n");
                pausar_consola();
                jugar();
                break;
            case 2:
                printf("Mostrando ranking de jugadores...\n");
                // TODO: Implementar mostrar_ranking()
                pausar_consola();
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
        }
    } while(opcion_elegida != 3);

    return 0;
}

void jugar()
{
    char nombre[MAX_NOMBRE];

    printf("\nIngrese su nombre: ");
    while (getchar() != '\n');
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    if (strlen(nombre) == 0) {
        strcpy(nombre, "Jugador1");
    }

    tJuego juego;
    tConfiguracion cfg;

    cargarConfiguracion("config.txt", &cfg);
    mostrarConfiguracion(&cfg);

    inicializarJuego(&juego, &cfg, nombre);

    printf("\nTablero generado! Posicion inicial: 0 (Inicio)\n");
    printf("Objetivo: Llegar a la posicion %d (Refugio)\n", cfg.totalCasillas - 1);
    pausar_consola();

    while (juego.juegoActivo) {
        int resultado = ejecutarTurno(&juego);

        if (resultado == 2) {
            printf("\n*** HAS GANADO! ***\n");
            break;
        }
        if (resultado == 0) {
            printf("\n*** HAS PERDIDO ***\n");
            break;
        }

        printf("\nContinuar...");
        pausar_consola();
    }

    printf("\n--- Resumen de Movimientos ---\n");
    mostrarColaMovimientos(&juego.colaMovimientos);

    guardarCaravana("caravana.txt", &juego);

    liberarJuego(&juego);

    printf("\nPartida finalizada!\n");
    pausar_consola();
}