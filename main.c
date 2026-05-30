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

void jugar(){
    tJuego juego;
    tConfiguracion cfg;
    tRegistroPartida reg;
    int resultado;

    //HACER FUNCION PARA CARGAR INDICE
    //CARGAR INDICE

    if (!cargarConfiguracion("config.txt", &cfg)) {
        printf("No se pudo cargar config.txt. Revise que el archivo exista.\n");
        pausar_consola();
        return;
    }

    inicializarJuego(&juego, &cfg);
    iniciarPartida(&juego);

    guardarCaravana("caravana.txt", &juego);

    mostrarBienvenida();
    printf("\nTablero generado! Posicion inicial: 1 (Inicio)\n");
    printf("Objetivo: Llegar a la posicion %d (Refugio)\n", cfg.totalCasillas);
    pausar_consola();

    mostrarTableroConPosiciones(&juego);

    while (juego.juegoActivo) {
        limpiar_pantalla();
        mostrarTableroConPosiciones(&juego);

        resultado = ejecutarTurno(&juego);

        mostrarTableroConPosiciones(&juego);

        if (resultado == 2) {
            mostrarFinJuego(1, &juego.jugador);
            juego.juegoActivo = 0;//PUESTO PROVISORIAMENTE, DEBE IR EN VERIFICAR DERROTA Y VICTORIA
        }
        if (resultado == 0) {
            mostrarFinJuego(0, &juego.jugador);
            juego.juegoActivo = 0;//PUESTO PROVISORIAMENTE, DEBE IR EN VERIFICAR DERROTA Y VICTORIA
        }

        if (juego.juegoActivo) {
            pausar_consola();
        }
    }

    //HACER UNA FUNCION PARA GUARDAR INDICE
    //GUARDAR INDICE

    printf("\n--- Resumen de Movimientos ---\n");
    mostrarColaMovimientos(&juego.colaMovimientos);

    strncpy(reg.nombre, juego.jugador.nombre, MAX_NOMBRE - 1);
    reg.nombre[MAX_NOMBRE - 1] = '\0';
    reg.puntuacion = juego.jugador.puntos;

    /*HACER FUNCION - GUARDAR EN EL ARHIVO DE PARTIDAS
    guardarPuntaje("puntajes.dat", &reg);
    */

    liberarJuego(&juego);


    printf("\nPARTIDA FINALIZADA!\n");
    pausar_consola();
}
